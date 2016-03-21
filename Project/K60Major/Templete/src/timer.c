#include "timer.h"

#include "pit.h"
#include "common.h"

#include "DataScope_DP.h"
#include "8700_2100.h"

#include "filter.h"
#include "calculate.h"
#include "counter.h"
#include "motor.h"

int left_pwm, right_pwm;

void Timer_Init(void)
{       
    PIT_QuickInit(HW_PIT_CH0, 5000); // 5ms����
    
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR); // ע��PIT �жϻص����� 0�Ŷ�ʱ�����жϴ���
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true); // ����PIT0��ʱ���ж�
}

void PIT_ISR(void)
{
// counter 
    //scope_buf[8] = ChlValue[0];
    //scope_buf[9] = ChlValue[1];
    
// gyro accel
    int16_t raw[6];
    int16_t angle_raw, gyro_raw, turn_raw;
    float angle, gyro, turn;
    float balance[2];
    float angle_balance, gyro_balance;
    
    GyroAccel_Raw(raw);
    
    /* raw 0-2 gyro 3-5 accel */
    angle_raw = raw[5];
    gyro_raw = raw[1];
    turn_raw = raw[0];
    angle = AngleCal(angle_raw, 2);
    gyro = AngleCal(gyro_raw, 1);
    turn = AngleCal(turn_raw, 1);
    
    //scope_buf[0] = angle_raw;
    //scope_buf[1] = gyro_raw;
    //scope_buf[2] = turn_raw;
    

    /* ��ԭʼ����ֱ���˲� */
    //angle_balance = AngleFilter(angle_raw, gyro_raw, 1);
    
    angle_balance = AngleFilter(angle, gyro, 2); // 1 Kalman 2 Complementary
    gyro_balance = gyro;
    
    //scope_buf[3] = angle;
    //scope_buf[4] = gyro;
    //scope_buf[5] = angle_balance;
        
    
// PID
    static int balance_pwm = 0, bp_for = 0;
    
    
    balance_pwm = Balance_PID(angle_balance, gyro_balance);
    
    left_pwm = (balance_pwm + bp_for) / 2;
    right_pwm = (balance_pwm + bp_for) / 2;
    
    bp_for = balance_pwm;
    
    Motor_Out(left_pwm, right_pwm);
    
    
    //scope_buf[6] = balance_pwm;

    scope_buf[0] = angle_raw;
    scope_buf[1] = gyro_raw;
    scope_buf[2] = turn_raw;
    scope_buf[3] = angle;
    scope_buf[4] = gyro;
    scope_buf[5] = angle_balance;
    scope_buf[6] = balance_pwm;
    
    scope_buf[8] = ChlValue[0];
    scope_buf[9] = ChlValue[1];

}