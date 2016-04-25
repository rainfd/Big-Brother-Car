#ifndef _CALCULATE_H
#define _CALCULATE_H

float AngleFilter(float angle, float gyro, int way);
float Balance_PID(float angle, float gyro);
float Speed_PID(int speed_l, int speed_r);
void Speed_Control(void);
float Turn_PID(int encoder_l, int encoder_r, float gyro);

#endif