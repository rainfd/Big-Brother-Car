#include "ad.h"

#include <stdlib.h>

#include "adc.h"
#include "common.h"

//#define AD0 ADC0_SE1_DP1
//#define AD1 ADC0_SE20_DM1
//#define AD2 ADC1_SE1_DP1
//#define AD3 ADC1_SE20_DM1
//#define AD4 ADC0_SE0_DP0

//        ADC0_SE1_DP1        2     
//        ADC0_SE20_DM1       3
//        ADC1_SE1_DP1        4
//        ADC1_SE20_DM1       5
//        ADC0_SE0_DP0        6 
//        ADC0_SE19_DM0       7
//        ADC1_SE0_DP0        8
//        ADC1_SE19_DM0       9

//ADC_0 SE20 SE1 SE19 SE0
//ADC_1 SE20 SE1 SE19 SE0

void ADC_ISR(void)
{
    //ADC_Value = ADC_ReadValue(HW_ADC0, kADC_MuxA);
}

void AD_Init(void)
{
    /* 初始化ADC模块 ADC0_SE19_DM0 */
    ADC_InitTypeDef ADC_InitStruct1; //申请一个结构体
    ADC_InitStruct1.instance = HW_ADC0; //使用ADC0模块
    ADC_InitStruct1.clockDiv = kADC_ClockDiv8; /* ADC采样时钟8分频 */
    ADC_InitStruct1.resolutionMode = kADC_SingleDiff12or13; //设置12位精度
    ADC_InitStruct1.triggerMode = kADC_TriggerSoftware; /* 软件触发转换 */
    ADC_InitStruct1.singleOrDiffMode = kADC_Single; /*单端模式 */
    ADC_InitStruct1.continueMode = kADC_ContinueConversionEnable; /* 启动连续转换 转换一次后 自动开始下一次转换*/
    ADC_InitStruct1.hardwareAveMode = kADC_HardwareAverageDisable; /*禁止 硬件平均 功能 */
    ADC_InitStruct1.vref = kADC_VoltageVREF;                       /* 使用外部VERFH VREFL 作为模拟电压参考 */
    ADC_Init(&ADC_InitStruct1);
    
    ADC_InitStruct1.instance = HW_ADC1;
    ADC_Init(&ADC_InitStruct1);
    
    
    /* 开启转换完成中断配置 */
    //ADC_CallbackInstall(HW_ADC0, ADC_ISR);
    //ADC_ITDMAConfig(HW_ADC0, kADC_MuxA, kADC_IT_EOF);
    
    /* 初始化对应引脚 */
    /* DM0引脚为专门的模拟引脚 ADC时 无需设置复用  DM0也无法当做普通的数字引脚 */
    
    /* 启动ADC转换 */
    ADC_StartConversion(HW_ADC0, 0, kADC_MuxA);
    ADC_StartConversion(HW_ADC0, 1, kADC_MuxA);
    ADC_StartConversion(HW_ADC0, 19, kADC_MuxA);
    ADC_StartConversion(HW_ADC0, 20, kADC_MuxA);
    ADC_StartConversion(HW_ADC1, 0, kADC_MuxA);
//    ADC_StartConversion(HW_ADC1, 1, kADC_MuxA);
//    ADC_StartConversion(HW_ADC1, 19, kADC_MuxA);
//    ADC_StartConversion(HW_ADC1, 20, kADC_MuxA);
    
//    while(1)
//    {
//        /* 如果ADC转换完成 */
//        if(ADC_IsConversionCompleted(HW_ADC0, kADC_MuxA) == 0)
//        {
//            /* 读取ADC的值  kADC_MuxA是每个ADC通道的转换器 默认都是 kADC_MuxA  MuxB 一般不能用于软件触发 */
//            printf("ADC[19]:%04d\r", ADC_ReadValue(HW_ADC0, kADC_MuxA));
//        }
//        DelayMs(5);
//    }
}

int cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void AD_Get(void)
{
    int ad_data[4][10] = {0};
    int ad_sum[4] = {0};
    float ad_avr[4] = {0};
    
    int i = 0, j = 0;
    for (i = 0; i < 10; i++) {
//        ad_data[0][i] = ADC_QuickReadValue(AD0);
//        ad_data[1][i] = ADC_QuickReadValue(AD1);
//        ad_data[2][i] = ADC_QuickReadValue(AD2);
//        ad_data[3][i] = ADC_QuickReadValue(AD3);
//        ad_data[4][i] = ADC_QuickReadValue(AD4);
    }
    // sort
    for (j = 0; j < 5; j++)  // min to max
        qsort(ad_data[j], 10, sizeof(ad_data[j]), cmp);
    // 去除最大最小值 求和
    for (j = 0; j < 5; j++)
        for (i = 1; i < 9; i++)
            ;
//            add_sum[j] += add_data[j][i];
    // 取平均
    for (j = 0; j < 5; j++)
        ;
//        ad_avr[j] = avr_sum[j] / 8.0;

    
    // 冲出赛道检测
    // 
}
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