#include "motor.h"

#include "ftm.h"
#include "common.h"
#include "gpio.h"

#define ENA PAout(11)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
#define ENB PAout(14)
#define IN1 PAout(9)
#define IN2 PAout(12)
#define IN3 PAout(10)
#define IN4 PAout(13)

void PWM_Init(int16_t duty)
{
    GPIO_QuickInit(HW_GPIOA, 11, kGPIO_Mode_OPP);   // ENA
    GPIO_QuickInit(HW_GPIOA, 12, kGPIO_Mode_OPP);   // IN2
    GPIO_QuickInit(HW_GPIOA, 13, kGPIO_Mode_OPP);   // IN4
    GPIO_QuickInit(HW_GPIOA, 14, kGPIO_Mode_OPP);   // ENB
    
    IN2 = 0;
    IN4 = 0;
    
    PWM_Enable();
    
#if 1
    /* 0-10000 对应 0-100% */
    FTM_PWM_QuickInit(FTM1_CH1_PA09, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM2_CH0_PA10, kPWM_EdgeAligned, duty);
#else
    GPIO_QuickInit(HW_GPIOA,  9, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOA, 10, kGPIO_Mode_OPP);
    IN1 = 1;
    IN3 = 1;
#endif
}

void PWM_Out(int16_t left, int16_t right)
{
    PWM_Enable();
    
    //  forward back
    if (left < 0) {
        left = -left;
        IN4 = 1;
    } else {
        IN2 = 0;
    }
    
    if (right < 0) {
        right = -right;
        IN4 = 1;
    } else { 
        IN4 = 0;
    }
    
    if (left > 10000)
        left = 10000;
    if (right > 10000)
        right = 10000;
        
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, left);
    FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, right);
}

void PWM_Enable(void)
{
    ENA = 1;
    ENB = 1;
}

void PWM_Disable(void)
{
    ENA = 0;
    ENB = 0;
}

/* 可用的FTM通道有: */
/*
 FTM0_CH4_PB12   // ftm0 模块 4通道 PB12引脚
 FTM0_CH5_PB13   
 FTM0_CH5_PA00   
 FTM0_CH6_PA01   
 FTM0_CH7_PA02   
 FTM0_CH0_PA03
 FTM0_CH1_PA04   
 FTM0_CH2_PA05   
 FTM0_CH3_PA06   
 FTM0_CH4_PA07   
 FTM0_CH0_PC01   
 FTM0_CH1_PC02   
 FTM0_CH2_PC03   
 FTM0_CH3_PC04   
 FTM0_CH4_PD04   
 FTM0_CH5_PD05   
 FTM0_CH6_PD06   
 FTM0_CH7_PD07   
 FTM1_CH0_PB12   
 FTM1_CH1_PB13   
 FTM1_CH0_PA08   
 FTM1_CH1_PA09   
 FTM1_CH0_PA12   
 FTM1_CH1_PA13   
 FTM1_CH0_PB00   
 FTM1_CH1_PB01   
 FTM2_CH0_PA10   
 FTM2_CH1_PA11   
 FTM2_CH0_PB18   
 FTM2_CH1_PB19   
*/