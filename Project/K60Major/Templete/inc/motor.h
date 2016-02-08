#ifndef _MOTOR_H
#define _MOTOR_H

#include "common.h"

void PWM_Init(int16_t rate);
void PWM_Out(int16_t left, int16_t right);
void PWM_Enable(void);
void PWM_Disable(void);

#endif