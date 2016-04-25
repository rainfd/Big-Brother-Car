#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "adc.h"

/* ���õ� ADCͨ�������� - 144P*/
/*
 ADC0_SE0_DP0        
 ADC0_SE1_DP1        
 ADC0_SE3_DP3        
 ADC0_SE4B_PC2       
 ADC0_SE5B_PD1       
 ADC0_SE6B_PD5       
 ADC0_SE7B_PD6       
 ADC0_SE8_PB0        
 ADC0_SE9_PB1        
 ADC0_SE12_PB2       
 ADC0_SE13_PB3       
 ADC0_SE14_PC0       
 ADC0_SE15_PC1       
 ADC0_SE17_E24       
 ADC0_SE18_E25       
 ADC0_SE19_DM0       
 ADC0_SE20_DM1       
 ADC0_SE26_TEMP      
 ADC1_SE0_DP0        
 ADC1_SE1_DP1        
 ADC1_SE3_DP3        
 ADC1_SE4_PE0        
 ADC1_SE5_PE1        
 ADC1_SE6_PE2        
 ADC1_SE7_PE3        
 ADC1_SE4B_PC8       
 ADC1_SE5B_PC9       
 ADC1_SE6B_PC10      
 ADC1_SE7B_PC11      
 ADC1_SE8_PB0        
 ADC1_SE9_PB1        
 ADC1_SE14_PB10      
 ADC1_SE15_PB11      
 ADC1_SE17_PB117     
 ADC1_SE19_DM0       
 ADC1_SE20_DM1       
 ADC1_SE26_TEMP      
*/

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("ADC test\r\n");
    
    /* ��ʼ��ADCģ�� ADC0_SE19_BM0 */
    ADC_InitTypeDef ADC_InitStruct1;
    ADC_InitStruct1.instance = HW_ADC0;
    ADC_InitStruct1.clockDiv = kADC_ClockDiv2; /* ADC����ʱ��2��Ƶ */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff10or11;
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* �������ת�� */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*����ģʽ */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* ��������ת�� ת��һ�κ� �Զ���ʼ��һ��ת��*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverageDisable; /*��ֹ Ӳ��ƽ�� ���� */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* ʹ���ⲿVERFH VREFL ��Ϊģ���ѹ�ο� */
    ADC_Init(&ADC_InitStruct1);
    
    /* ��ʼ����Ӧ���� */
    /* DM0����Ϊר�ŵ�ģ������ ADCʱ �������ø���  DM0Ҳ�޷�������ͨ���������� */
    
    /* ����һ��ADCת�� */
    ADC_StartConversion(HW_ADC0, 19, kADC_MuxA);
    
    while(1)
    {
        /* ���ADCת����� */
        if(ADC_IsConversionCompleted(HW_ADC0, kADC_MuxA) == 0)
        {
            /* ��ȡADC��ֵ  kADC_MuxA��ÿ��ADCͨ����ת���� Ĭ�϶��� kADC_MuxA  MuxB һ�㲻������������� */
            printf("ADC[19]:%04d\r", ADC_ReadValue(HW_ADC0, kADC_MuxA));
        }
        DelayMs(5);
    }
}


