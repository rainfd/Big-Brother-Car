#include "bluetooth.h"

#include "common.h"
#include "uart.h"
#include "motor.h"

#define SCOPE 0

void parameter(char data[], int len);

extern int left_pwm, right_pwm;
extern float ANGLE_P;
extern float ANGLE_D;

float ap = 53328;
float ad = 53328;
int str_sign = 0;
char rec[10] = {0};

static void UART_RX_ISR(uint16_t byteReceived)
{
    /* 将接收到的数据发送回去 */
#if 0   
    UART_WriteByte(HW_UART4, byteReceived);
#else
    static int index = -1;
    
    if (byteReceived == 0x53) {// S Start
        str_sign = 1;
        index++;
    }
    else if (byteReceived == 0x45) {// E End
        str_sign = 0;
        parameter(rec, index);
        index = -1;
    }
    else if (str_sign = 1 && index > -1) {
        rec[index++] = byteReceived;
    }
#endif
}

uint32_t UARTB_Init(int rate)
{
    uint32_t instance;
    
    instance = UART_QuickInit(UART4_RX_PE25_TX_PE24, rate);
    /*  配置UART 中断配置 打开接收中断 安装中断回调函数 */
    UART_CallbackRxInstall(HW_UART4, UART_RX_ISR);
    /* 打开串口接收中断功能 IT 就是中断的意思*/
    UART_ITDMAConfig(HW_UART4, kUART_IT_Rx, true);
    
    return instance;
}

void parameter(char data[], int len)
{
    if (len != 6) {
#if SCOPE
        printf("ERROR\n");
#endif
        return;
    }
    
    
    if (data[0] == 0x61 && data[1] == 0x70) {// a 0x61 p 0x70
        ap = 0;
        ap = data[2]*1000 + data[3]*100 + data[4]*10 + data[5];
    } else if (data[0] == 0x61 && data[1] == 0x64) { // d 0x64
        ad = 0;
        ad = data[2]*1000 + data[3]*100 + data[4]*10 + data[5];
    }
    
    /* 蜜汁错误 */
    ANGLE_P = ap-53328;
    ANGLE_D = ad-53328;
    
#if SCOPE
    printf("ap:%d ad: %d\n", (int)ANGLE_P, (int)ANGLE_D);
#endif
}