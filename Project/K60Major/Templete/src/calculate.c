#include "calculate.h"

#include "DataScope_DP.h"

#include "filter.h"

const int SPEED_CONTROL_PERIOD = 100; // 100ms
const int SPEED_MAX = 0;
const int SPEED_MIN = -0;
// WOW
const int SPEED_SET = 0;

// Balance control
float ANGLE_P = 100;
float ANGLE_D = 0;
// Speed control
float SPEED_P = 0;
float SPEED_I = 0;
// Turn control
float TURN_P = 0;
float TURN_D = 0;

float LRS = 0;
float LRA = 0;

//int spd_per = 0; // ?
//int dir_per = 0; // ?

float AngleFilter(float angle, float gyro, int way)
{
    float angle_balance;
    
    if (way == 1)
        angle_balance =  Kalman_Filter(angle, gyro);
    else if (way == 2)
        angle_balance = Complementary_Filter(angle, gyro);
    
    return angle_balance;
}



float Balance_PID(float angle, float gyro)
{
    float balance;
        
    balance = angle * ANGLE_P + gyro * ANGLE_D;
    
    return balance;
}

//extern int speed_l, speed_r;
extern int balance_pwm ,speed_pwm;
float speed_new = 0, speed_old = 0;
float speed_outn = 0, speed_outo = 0;

double speed_integral = 0;

float Speed_PID(int speed_l, int speed_r)
{
    float speed = 0;
    
    speed_new = (speed_l + speed_l) / 2;
    speed = speed_new * 0.7 + speed_old * 0.3;
    speed_old = speed_new;
    
    //speed *= 1000 / 1 / 100  //单位换算 从电机到光电码盘 车轮直径6.3cm? 红外码盘100线 车轮齿轮 104?
    
    float speed_delta = 0;

    if (balance_pwm >= 50) {
        speed_delta = -speed;
    } else if (balance_pwm <= 19) {
        speed_delta = SPEED_SET * (20 - balance_pwm) / 20 - speed
                 - 0.03 * LRS * LRS / LRA;
    }
        
    float speed_p, speed_i;
    speed_p = speed_delta * SPEED_P;
    speed_i = speed_delta * SPEED_I;
    speed_integral += speed_i;
    
    if (speed_integral > SPEED_MAX)
        speed_integral = SPEED_MAX;
    if (speed_integral < SPEED_MIN)
        speed_integral = SPEED_MIN;
    
    speed_outo = speed_outn;
    speed_outn = speed_p + speed_integral;
    
    return (int)speed_outn;
}

int spd_per = 0;

void Speed_Control(void)
{
  float delta = 0; 
  
  delta = speed_outn - speed_outo;
  
  // 速度分段逐步递加
  speed_pwm = (int)(delta * spd_per / SPEED_CONTROL_PERIOD + speed_outo);
}

void DIR_VS(void)
{
    ;
}

int dir_per = 0;

float Turn_PID(int encoder_l, int encoder_r, float gyro)
{
    float turn;
    
    turn = 0;
    
    return turn;
}