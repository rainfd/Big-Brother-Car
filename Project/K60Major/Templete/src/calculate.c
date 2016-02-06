#include "calculate.h"

#include "DataScope_DP.h"

#include "filter.h"

#define GRAVITY_OFFSET          0
#define GYROSCOPE_OFFSET        0

#define GRAVITY_RATIO     0.0078125
#define GYROSCOPE_RATIO   4096.0//* 0.000244

#define SCOPE

// Balance control
float ANGLE_P = 0;
float ANGLE_D = 0;
// Speed control
float SPEED_P = 0;
float SPEED_I = 0;
// Turn control
float TURN_P = 0;
float TURN_D = 0;

float AngelCal(int angle, int gyro, int way)
{
    float angle_balance, gyro_balance;
    
    angle_balance = (angle - GRAVITY_OFFSET) * GRAVITY_RATIO;
    gyro_balance = (gyro - GYROSCOPE_OFFSET) / GYROSCOPE_RATIO;
    
    float result;
    
    if (way == 1)
        result =  Kalman_Filter(angle_balance, gyro_balance);
    else if (way == 2)
        result = Complementary_Filter(angle_balance, gyro_balance);
    
#ifdef SCOPE
    scope_buf[7] = angle_balance;
    scope_buf[8] = gyro_balance;
    scope_buf[9] = result;
#endif
    
    return result;

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