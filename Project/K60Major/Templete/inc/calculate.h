#ifndef _CALCULATE_H
#define _CALCULATE_H

float AngelCal(int angle, int gyro, int way);
int Balance_PID(float angle, float gyro);
int Speed_PID(int encoder_l, int encoder_r);
int Turn_PID(int encoder_l, int encoder_r, float gyro);

#endif