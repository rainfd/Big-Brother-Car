#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "adc.h"
/* CH Kinetis�̼��� V2.50 �汾 */
/* �޸���Ƶ ���޸� CMSIS��׼�ļ� system_MKxxxx.c �е� CLOCK_SETUP �� */

/*
     ʵ�����ƣ�ADC�ж�ʵ��
     ʵ��ƽ̨����ѻ������
     ����оƬ��MK60DN512ZVQ10
 ʵ��Ч����ʹ��оƬ��DM0���Ž���ad�ɼ���ad�ɼ���ͨ��Ϊ19��������10λ
        adת����ɺ�����жϣ���ad�ж��л�ȡad�ɼ����� 
        ͨ�����ڿ������ϵĵ�λ�������Ը���ad�ɼ����
*/

static int32_t ADC_Value;

//AD�жϴ�����
void ADC_ISR(void)
{
    ADC_Value = ADC_ReadValue(HW_ADC0, kADC_MuxA);
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("ADC test\r\n");
    
    /* ��ʼ��ADCģ�� ADC0_SE19_DM0 */
    ADC_InitTypeDef ADC_InitStruct1; //����һ���ṹ��
    ADC_InitStruct1.instance = HW_ADC0; //ʹ��ADC0ģ��
    ADC_InitStruct1.clockDiv = kADC_ClockDiv8; /* ADC����ʱ��2��Ƶ */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff10or11; //����10λ��11λ����
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* �������ת�� */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*����ģʽ */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* ��������ת�� ת��һ�κ� �Զ���ʼ��һ��ת��*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverage_32; /*��ֹ Ӳ��ƽ�� ���� */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* ʹ���ⲿVERFH VREFL ��Ϊģ���ѹ�ο� */
    ADC_Init(&ADC_InitStruct1);
    
    /* ����ת������ж����� */
    ADC_CallbackInstall(HW_ADC0, ADC_ISR);
    ADC_ITDMAConfig(HW_ADC0, kADC_MuxA, kADC_IT_EOF);
    
    /* ��ʼ����Ӧ���� */
    /* DM0����Ϊר�ŵ�ģ������ ADCʱ �������ø���  DM0Ҳ�޷�������ͨ���������� */
    
    /* ����ADCת�� */
    ADC_StartConversion(HW_ADC0, 19, kADC_MuxA);
    
    while(1)
    {
        printf("ADC:%4d\r", ADC_Value);
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(50);
    }
}


