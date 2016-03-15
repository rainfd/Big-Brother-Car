#include "motor.h"

#include "ftm.h"
#include "common.h"
#include "gpio.h"

#define MAX_DUTY 10000
#define MIN_DUTY 0
#define LIM_DUTY 10000

// PA8 9 10 11
#define IN1 PAout(8)
#define IN2 PAout(9)
#define IN3 PAout(10)
#define IN4 PAout(11)

void PWM_Init(int duty)
{   
    //PWM_Lim(duty, 0, 0, 0);
    if (duty > MAX_DUTY)
        duty = LIM_DUTY;
    
    /* 0-10000 对应 0-100% */
    FTM_PWM_QuickInit(FTM1_CH0_PA08, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM1_CH1_PA09, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM2_CH0_PA10, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM2_CH1_PA11, kPWM_EdgeAligned, duty);
}

void PWM_Limit(int* l1, int* l2, int* r1, int* r2)
{
    if (*l1 > MAX_DUTY)
        *l1 = LIM_DUTY;
    if (*l2 > MAX_DUTY)
        *l2 = LIM_DUTY;
    if (*r1 > MAX_DUTY)
        *r1 = LIM_DUTY;
    if (*r2 > MAX_DUTY)
        *r2 = LIM_DUTY;
    
    if (*l1 < MIN_DUTY)
        *l1 = MIN_DUTY;
    if (*l2 < MIN_DUTY)
        *l2 = MIN_DUTY;
    if (*r1 < MIN_DUTY)
        *r1 = MIN_DUTY;
    if (*r2 < MIN_DUTY)
        *r2 = MIN_DUTY;
}

void PWM_Out(int l1, int l2, int r1, int r2)
{       
    PWM_Limit(&l1, &l2, &r1, &r2);
    
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0, l1);
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, l2);
    FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, r1);
    FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, r2);
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