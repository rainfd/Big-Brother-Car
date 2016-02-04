#ifndef _FILTER_H
#define _FILTER_H

float Kalman_Filter(float Accel,float Gyro);
float Complementary_Filter(float angle_m, float gyro_m);
float Complementary_Filter2(float angle_m, float gyro_m);

#endif