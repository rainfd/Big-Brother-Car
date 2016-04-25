#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "pit.h"
#include "ftm.h"

#include "LQ_OLED.h"
#include "DataScope_DP.h"
#include "8700_2100.h"

#include "ad.h"
#include "counter.h"
#include "filter.h"
#include "timer.h"
#include "motor.h"
#include "bluetooth.h"

//#define LED_ON PEout(26) = 0
//#define LED_OFF PEout(26) = 1
#define BTN4 PCin(6)
#define BTN3 PCin(8)
#define BTN2 PCin(10)
#define BTN1 PCin(12)

uint32_t uart_instance;

extern int left_pwm, right_pwm;
extern int speed_l, speed_r;
extern float ANGLE_P;
extern float ANGLE_D;
extern float angle_balance, gyro_balance;
extern float balance_pwm, speed_pwm, turn_pwm;
extern float motor_l, motor_r;

int main(void)
{
    DelayInit();
// LED
    GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_OPP);
// BTN
    GPIO_QuickInit(HW_GPIOC, 6, kGPIO_Mode_IPD);
    

    LCD_Init();
    LCD_Print(20, 0, "Big Brother");
    uart_instance = UARTB_Init(115200);
    GyroAccel_Init();
    CounterInit();
    AD_Init();
    Timer_Init();
    Motor_Init();
//    PWM_Out(3000, 0, 3000, 0);

    printf("Init OK");

    while(1)
    {
// LCD 128*64 font:8*16

// ANGLE 
        LCD_Print(0, 2, "G: ");
        LCD_Print_Num(20, 2, (int)angle_balance, 4);
        LCD_Print(64, 2, "A: ");
        LCD_Print_Num(84, 2, (int)gyro_balance, 4);        
// PID value 
        LCD_Print(0, 4, "P: ");
        LCD_Print_Num(20, 4, (int)ANGLE_P, 4);
        LCD_Print(64, 4, "D: ");
        LCD_Print_Num(84, 4, (int)ANGLE_D, 4);

        if (PCin(6) == 0) {         
// PWMout
            LCD_Print(0, 6, "L: ");
            LCD_Print_Num(30, 6, (int)motor_l, 4);
            LCD_Print(64, 6, "R: ");
            LCD_Print_Num(84, 6, (int)motor_r, 4);
        } else {                   
// Counter
            LCD_Print(0, 6, "L: ");
            LCD_Print_Num(30, 6, (int)speed_l, 4);
            LCD_Print(64, 6, "R: ");
            LCD_Print_Num(84, 6, (int)speed_r, 4);
        }
        
//        LCD_Print(0, 6, "Y: ");
//        LCD_Print_Num(20, 6, GYROSCOPE_OFFSET, 4);
//        LCD_Print(64, 6, "A: ");
//        LCD_Print_Num(84, 6, GRAVITY_OFFSET, 4);

        DataScope(uart_instance, scope_buf);

    }
}
