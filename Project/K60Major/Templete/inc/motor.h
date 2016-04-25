#ifndef _MOTOR_H
#define _MOTOR_H

void Motor_Init(void);
void PWM_Limit(int* pwm);
void PWM_Out(int l1, int l2, int r1, int r2);
void Motor_Out(int left, int right);



#endif