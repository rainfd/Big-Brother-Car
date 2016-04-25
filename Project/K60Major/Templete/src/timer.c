#include "timer.h"

#include "pit.h"
#include "common.h"
#include "uart.h"
#include "adc.h"

#include "DataScope_DP.h"
#include "8700_2100.h"

#include "filter.h"
#include "calculate.h"
#include "counter.h"
#include "motor.h"
#include "ad.h"

#define AD 0

int pit_ctr = 0; 
int dir_ctr = 0;
int spd_ctr = 0;
extern int spd_per; // ?
extern int dir_per; // ?

float angle = 0, gyro, turn = 0;
int speed_l = 0, speed_r = 0;
float angle_balance = 0, gyro_balance = 0;
float balance_pwm = 0, speed_pwm = 0, turn_pwm = 0;

float motor_l, motor_r;

void Angle_Read(void);
void Angle_Merge(void);
void Speed_Read(void);
void Scope(void);
void Motor_Control(void);

void PIT_ISR(void)
{
    pit_ctr++;
    dir_ctr++;
    
    spd_per++;
    //Speed_Control();
    
    switch (pit_ctr) {
        case 1:
            Angle_Read();
            break;
        case 2: 
            Angle_Merge();
            balance_pwm = Balance_PID(angle_balance, gyro_balance);
            Motor_Control();
//            Motor_Out(3000, 3000);
            break;
        case 3: 
            spd_ctr++;
            if (spd_ctr > 20) {
                //speed_pwm = Speed_PID(speed_l, speed_r);
                spd_ctr = 0;
                spd_per = 0;
            }
            break;
        case 4: 
            dir_ctr++;
            //Dir_VS();
            //Scope();
            if (dir_ctr > 4) {
                //Dir_Control();
                dir_ctr = 0;
                dir_per = 0;
            }
            
            break;
        case 5: 
            Speed_Read();
            pit_ctr = 0;
            break;
        default: 
            printf("d");
            break;
    }
    
    //Spd_Control();
    //Dir_Contorl();
    Scope();

}

void Angle_Read()
{
    float data[3];
    
    GyroAccel_Read(data);
    
    angle = data[0];
    gyro = data[1];
    turn = data[2];
    
    // 角度归一化 从-1到1 变成 从-90到90 负号为前后修正
    angle = -angle * 90;
    gyro = -gyro;
    // 限幅
    if (angle > 90)
        angle = 90;
    if (angle < -90)
        angle = -90;
}

void Angle_Merge()
{
    angle_balance = AngleFilter(angle, gyro, 2); // 1 Kalman 2 Complementary
    gyro_balance = gyro;
}


void Speed_Read()
{
    if (motor_l > 0)
        speed_l = ChlValue[0];
    else 
        speed_l = -ChlValue[0];
    
    if (motor_r > 0)
        speed_r = ChlValue[1];
    else
        speed_r = -ChlValue[1];
}

void Motor_Control(void)
{
//    float motor_l, motor_r;
    //speed_Start+g_fSpeedControlOut- g_fDirectionControlOut;
    motor_l = balance_pwm + speed_pwm - turn_pwm;
    motor_r = balance_pwm + speed_pwm + turn_pwm;
    
    Motor_Out((int)motor_l, (int)motor_r);
}
    
void Scope(void)
{
    //scope_buf[0] = angle_raw;
    //scope_buf[1] = gyro_raw;
    //scope_buf[2] = turn_raw;
    int ad_data[5];
#if AD
    ad_data[0] = ADC_QuickReadValue(AD0);
    ad_data[1] = ADC_QuickReadValue(AD1);
    ad_data[2] = ADC_QuickReadValue(AD2);
    ad_data[3] = ADC_QuickReadValue(AD3);
    ad_data[4] = ADC_QuickReadValue(AD4);
    
    scope_buf[0] = ad_data[0];
    scope_buf[1] = ad_data[1];
    scope_buf[2] = ad_data[2];
    scope_buf[3] = ad_data[3];
    scope_buf[4] = ad_data[4];
    
#else
    scope_buf[3] = angle;
    scope_buf[4] = gyro;
    scope_buf[5] = angle_balance;
    scope_buf[6] = balance_pwm;
    
    scope_buf[8] = ChlValue[0];
    scope_buf[9] = ChlValue[1];
#endif
}

void Timer_Init(void)
{       
    PIT_QuickInit(HW_PIT_CH0, 5000); // 5ms周期
    
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR); // 注册PIT 中断回调函数 0号定时器的中断处理
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true); // 开启PIT0定时器中断
}