#include "bootloader.h"
#include "mq.h"
#include "bootloader_util.h"
#include <string.h>
#include <common.h>

#define RCV_OK              0xA5
#define RCV_ERR             0x5A
#define RCV_SKIP            0x5B

Boot_t Bootloader;

typedef  void (*pFunction)(void);

//����֡��ʽ
#pragma pack(1)
typedef struct
{
    uint8_t  cmd;
    uint16_t currentPkgNo;
    uint8_t content[4096+30];
} DataFrame_t;


//оƬ��Ϣ֡���ݲ��ָ�ʽ
#pragma pack(1)
typedef struct
{
    uint8_t     cmd;
    uint8_t     name[32];
    uint32_t    FlashPageSize;
    uint32_t    AppStartAddr;
} ChipInfo_t;
       
//��Ӧ֡���ݲ��ָ�ʽ
#pragma pack(1)
typedef struct
{
    uint8_t cmd;
    uint16_t pkg_no;
    uint8_t status;
} ResponseFrame_t;


//ͨ�����ݰ��ĸ�ʽ
#pragma pack(1)
typedef struct
{
    uint8_t cmd;
    uint8_t content[4096+30];
} GenericRecvFrame_t;


//������Ϣ����Ļص�����
typedef void(*pFuncCallback)(msg_t *pMsg);


static msg_t* pMsg;         /* ��Ϣָ�� */
static pFuncCallback pExecFun;      /* �ص��������� */
uint32_t SysTimeOut;


/* ����������� */
static pFuncCallback MsgCallbackFind(msg_t* pMsg);

/* ������Ϣ������ */
static void ProcessUartMsg(msg_t* pMsg);

/* Ӧ�ó�����Ϣ������ */
static void ProcessAppInfoMsg(msg_t* pMsg);

/* ����������Ϣ������ */
static void ProcessTransDataMsg(msg_t* pMsg);

/* Ӧ�ü�����Ϣ������ */
static void ProcessAppVerificationMsg(msg_t* pMsg);

/* Ӧ�ó����麯�� */
static void ProccessAppCheckMsg(msg_t* pMsg);

/* Tick������ */
static void TickProcess(void);

static void ProcessChipInfoMsg(msg_t* pMsg);
void GoToUserApp(uint32_t app_start_addr);


/*
    Ѱ�Һ��ʵ���Ϣ�ص������������ظú���
*/
static pFuncCallback MsgCallbackFind(msg_t* pMsg)
{
    pFuncCallback pCallBack = NULL;

    switch(pMsg->cmd)
    {
    case CMD_SERIALPORT:
        pCallBack = ProcessUartMsg;
        break;

    case CMD_CHIPINFO:
        pCallBack = ProcessChipInfoMsg;
        break;

    case CMD_APP_INFO:
        pCallBack = ProcessAppInfoMsg;
        break;

    case CMD_TRANS_DATA:
        pCallBack = ProcessTransDataMsg;
        break;

    case CMD_VERIFICATION:
        pCallBack = ProcessAppVerificationMsg;
        break;

    case CMD_APP_CHECK:
        pCallBack = ProccessAppCheckMsg;
        break;

    default:
        pCallBack = NULL;
        break;
    }

    return pCallBack;
}

static void ProcessUartMsg(msg_t* pMsg)
{
    GenericRecvFrame_t *pRcvFrame;
    msg_t m_Msg;

    pRcvFrame = (GenericRecvFrame_t *)(((uint8_t*)pMsg->pMessage)) ;
    MainControl.IsBootMode = true;

    m_Msg.cmd = pRcvFrame->cmd;
    m_Msg.pMessage = pRcvFrame;

    mq_push(m_Msg);
}

static void ProcessChipInfoMsg(msg_t* pMsg)
{
    ChipInfo_t info;

    info.cmd = CMD_CHIPINFO;
    if(!strlen(Bootloader.name))
    {
        memcpy(info.name, "Unknown Devices", strlen("Unknown Devices"));
    }
    else
    {
        memcpy(info.name, Bootloader.name, strlen(Bootloader.name));
    }
    info.FlashPageSize = Bootloader.FlashPageSize;
    SendResp((uint8_t*)&info, 0, sizeof(info));
}

static void ProcessAppInfoMsg(msg_t* pMsg)
{
    MainControl.write_addr = Bootloader.AppStartAddr;
    MainControl.currentPkgNo = 0;
    MainControl.retryCnt = 0;

    ResponseFrame_t Resp = {CMD_APP_INFO, 0, RCV_OK};
    SendResp((uint8_t*)&Resp, 0, sizeof(Resp));
}

static void ProcessTransDataMsg(msg_t* pMsg)
{
    DataFrame_t* pDataFrame;
    ResponseFrame_t Resp;

    pDataFrame = (DataFrame_t*)pMsg->pMessage;
    Resp.cmd = CMD_TRANS_DATA;
    //�������һ���������ظ���
    MainControl.write_addr = Bootloader.AppStartAddr + (pDataFrame->currentPkgNo-1)*Bootloader.FlashPageSize;
    Bootloader.flash_erase(MainControl.write_addr);
    if(Bootloader.flash_write(MainControl.write_addr, pDataFrame->content, Bootloader.FlashPageSize) == BL_FLASH_OK)
    {
        MainControl.retryCnt = 0;
        MainControl.currentPkgNo = pDataFrame->currentPkgNo;
        MainControl.op_state = RCV_OK;
    }
    else
    {
        MainControl.op_state = RCV_ERR;
        MainControl.retryCnt++;
    }
    Resp.status = MainControl.op_state;
            
//    if((MainControl.currentPkgNo == (pDataFrame->currentPkgNo-1)) || (MainControl.currentPkgNo == pDataFrame->currentPkgNo))
//    {
//        if(MainControl.currentPkgNo != pDataFrame->currentPkgNo)
//        {
//            needWrite = 1;
//            MainControl.write_addr = Bootloader.AppStartAddr + (pDataFrame->currentPkgNo-1)*Bootloader.FlashPageSize;
//        }
//        else if(MainControl.op_state != RCV_OK)
//        {
//            needWrite = 1;
//            MainControl.write_addr = Bootloader.AppStartAddr + pDataFrame->currentPkgNo*Bootloader.FlashPageSize;
//        }
//        if(needWrite)
//        {
//            Bootloader.flash_erase(MainControl.write_addr);
//            if(Bootloader.flash_write(MainControl.write_addr, pDataFrame->content, Bootloader.FlashPageSize) == BL_FLASH_OK)
//            {
//                MainControl.retryCnt = 0;
//                MainControl.currentPkgNo = pDataFrame->currentPkgNo;
//                MainControl.op_state = RCV_OK;
//            }
//            else
//            {
//                MainControl.op_state = RCV_ERR;
//                MainControl.retryCnt++;
//            }
//            Resp.status = MainControl.op_state;
//        }
//        else
//        {
//            Resp.status = RCV_OK;
//        }

        Resp.pkg_no = pDataFrame->currentPkgNo;
        SendResp((uint8_t*)&Resp, 0, sizeof(Resp));
//    }
}

static void ProcessAppVerificationMsg(msg_t* pMsg)
{
    ResponseFrame_t Resp = {CMD_VERIFICATION, 0, RCV_OK};
    SendResp((uint8_t*)&Resp, 0, sizeof(Resp));
    SendResp((uint8_t*)&Resp, 0, sizeof(Resp));
    
    msg_t m_Msg = {CMD_APP_CHECK, 0, 0, &m_Msg};
    mq_push(m_Msg);
}

/* �������Ƿ���Ч�������Ч�� ����ת��Ӧ�ó���ִ�� */
static void ProccessAppCheckMsg(msg_t* pMsg)
{
    GoToUserApp( Bootloader.AppStartAddr);
}

static void SysTick_Cfg(uint32_t ticks)
{
    if (ticks > SysTick_LOAD_RELOAD_Msk)  return;            /* Reload value impossible */

    SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
    SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */                                                 /* Function successful */
}

static void TickProcess(void)
{
    if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
    {
        ++MainControl.timeout;
    }
}


void GoToUserApp(uint32_t app_start_addr)
{
    pFunction jump_to_application;
    uint32_t jump_addr;
    jump_addr = *(uint32_t*)(app_start_addr + 4);  //RESET�ж�
    
    //���ڲ�����bootloader, �ʳ����jump_addr��ַӦ���� (0x5000, END_ADDR] ��Χ��
    if(*((uint32_t*)app_start_addr) != 0xFFFFFFFFUL && (jump_addr > Bootloader.AppStartAddr))
    {
        jump_to_application = (pFunction)jump_addr;
        __set_MSP(*(uint32_t*)app_start_addr); //ջ��ַ
        SCB->VTOR = app_start_addr;
        jump_to_application();
    }
}

uint32_t BootloaderInit(Boot_t* boot)
{
    mq_init();
    SysTick_Cfg(GetClock(kBusClock)/1000);
    memcpy(&Bootloader, boot, sizeof(Boot_t));
    SysTimeOut = Bootloader.TimeOut; 
    return 0;
}

void BootloaderProc(void)
{
    if (mq_exist())
    {
        pMsg =  mq_pop();
        pExecFun = MsgCallbackFind(pMsg);
        if(pExecFun != NULL)
        {
            pExecFun(pMsg);
            pExecFun = NULL;
        }
    }
    if((MainControl.timeout >= SysTimeOut) && (!MainControl.IsBootMode))
    {
        msg_t m_Msg = {CMD_APP_CHECK,0, 0, &m_Msg};
        MainControl.timeout = 0;
        mq_push(m_Msg);
    }
    TickProcess();
}

void SendResp(uint8_t* content, uint8_t cipherFlg,  uint16_t len)
{
    uint8_t header[5] = {0xFF, 0xFF, 0, 0, 0};
    uint8_t footer[3] = {0, 0xFF, 0xFE};
    uint8_t fcs = 0;
    int i;

    header[3] = (uint8_t)(len & 0xFF);
    header[4] = (uint8_t)((len>>8) & 0xFF);

    /* cipter */
    if(cipherFlg == 1)
    {

    }

    /* make checksum */
    for(i=0; i<len; i++)
    {
        fcs += content[i];
    }
    footer[0] = fcs;
    
    Bootloader.send(header, sizeof(header));
    Bootloader.send(content, len);
    Bootloader.send(footer, sizeof(footer));
}

