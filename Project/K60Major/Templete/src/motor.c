#include "motor.h"

#include "ftm.h"
#include "common.h"
#include "gpio.h"

#define TEST 1

#define MAX_DUTY 10000
#define MIN_DUTY 0
#define LIM_DUTY 10000

// PA8 9 10 11
#define IN1 PAout(8)
#define IN2 PAout(9)
#define IN3 PAout(10)
#define IN4 PAout(11)

void Motor_Init(int duty)
{   
    PWM_Limit(&duty);
    
#if TEST
    /* 0-10000 ��Ӧ 0-100% */
    FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM0_CH4_PA07, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM1_CH0_PA08, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM1_CH1_PA09, kPWM_EdgeAligned, duty);
#else
    FTM_PWM_QuickInit(FTM1_CH0_PA08, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM1_CH1_PA09, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM2_CH0_PA10, kPWM_EdgeAligned, duty);
    FTM_PWM_QuickInit(FTM2_CH1_PA11, kPWM_EdgeAligned, duty);
#endif
}

void PWM_Limit(int* pwm)
{
    if (*pwm > MAX_DUTY)
        *pwm = LIM_DUTY;

}

void PWM_Out(int l1, int l2, int r1, int r2)
{       
    PWM_Limit(&l1);
    PWM_Limit(&l2);
    PWM_Limit(&r1);
    PWM_Limit(&r2);
#if TEST
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, r1);
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH4, r2);
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0, l1);
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, l2);
#else
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0, l1);
    FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH1, l2);
    FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH0, r1);
    FTM_PWM_ChangeDuty(HW_FTM2, HW_FTM_CH1, r2);
#endif
}

void Motor_Out(int left, int right)
{
    int l1 = 0, l2 = 0;
    int r1 = 0, r2 = 0;
    
    if (left > 0)
        l1 = left;
    else
        l2 = -left;
    
    if (right > 0)
        r1 = right;
    else
        r2 = -right;
    
    PWM_Out(l1, l2, r1, r2);
}
/* ���õ�FTMͨ����: */
/*
 FTM0_CH4_PB12   // ftm0 ģ�� 4ͨ�� PB12����
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