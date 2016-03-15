#ifndef _MOTOR_H
#define _MOTOR_H

void PWM_Init(int rate);
void PWM_Limit(int* l1, int* l2, int* r1, int* r2);
void PWM_Out(int l1, int l2, int r1, int r2);



#endif