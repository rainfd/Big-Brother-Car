#include "calculate.h"

#include "DataScope_DP.h"

#include "filter.h"

#define GRAVITY_OFFSET          0
#define GYROSCOPE_OFFSET        0

#define GYROSCOPE_RATIO   0.0078125 //128.0//0.0078125//128 (pow 2 7)
#define GRAVITY_RATIO   0.000244//4096.0//0.000244//4096.0//* 0.000244

#define SCOPE

//float angle_balance, gyro_balance

// Balance control
float ANGLE_P = 1250;
float ANGLE_D = 0;
// Speed control
float SPEED_P = 0;
float SPEED_I = 0;
// Turn control
float TURN_P = 0;
float TURN_D = 0;


float AngleCal(int raw, int way)
{
    float result;
 
    /* 1 gyro 2 accel */

    if (way == 1)
        result = (raw - GYROSCOPE_OFFSET) * GYROSCOPE_RATIO; 
    else if (way == 2)
        result = (raw - GRAVITY_OFFSET) * GRAVITY_RATIO;
    
    
    return result;
}

float AngleFilter(float angle, float gyro, int way)
{
    float angle_balance;
    
    if (way == 1)
        angle_balance =  Kalman_Filter(angle, gyro);
    else if (way == 2)
        angle_balance = Complementary_Filter(angle, gyro);
    
#ifdef SCOPE
    scope_buf[0] = angle;
    scope_buf[1] = gyro;
    scope_buf[2] = angle_balance;
#endif
    
    return angle_balance;
}



int Balance_PID(float angle, float gyro)
{
    float balance;
        
    balance = angle * ANGLE_P + gyro * ANGLE_D;
    
    return (int)balance;
}

int Speed_PID(int encoder_l, int encoder_r)
{
    int speed;
    
    speed = 0;
    
    return speed;
}

int Turn_PID(int encoder_l, int encoder_r, float gyro)
{
    int turn;
    
    turn = 0;
    
    return turn;
}