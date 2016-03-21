#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "adc.h"
#include "pit.h"
#include "ftm.h"

#include "LQ_OLED.h"
#include "DataScope_DP.h"
#include "8700_2100.h"

#include "counter.h"
#include "filter.h"
#include "timer.h"
#include "motor.h"
#include "bluetooth.h"

#define LED_ON PEout(26) = 0
#define LED_OFF PEout(26) = 1

uint32_t uart_instance;

extern int left_pwm, right_pwm;
extern float ANGLE_P;
extern float ANGLE_D;

int main(void)
{   
    DelayInit();

    GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_OPP);
    
    LED_OFF;

    uart_instance = UARTB_Init(115200);
    GyroAccel_Init();
    CounterInit();
    Timer_Init();
    Motor_Init(0);
    PWM_Out(5000,0,5000,0);
    LCD_Init();
    LCD_Print(20, 0, "Big Brother"); 
    
    printf("Init OK");

    LED_ON;
    
    while(1)
    {
// LCD 128*64 font:8*16
#if 0   
// Counter
        LCD_Print(0, 2, "L: ");
        LCD_Print_Num(20, 2, ChlValue[0], 4);
        LCD_Print(64, 2, "R: ");
        LCD_Print_Num(84, 2, ChlValue[1], 4);
#elif 1
// PID value
        LCD_Print(0, 2, "P: ");
        LCD_Print_Num(20, 2, (int)ANGLE_P, 4);
        LCD_Print(64, 2, "D: ");
        LCD_Print_Num(84, 2, (int)ANGLE_D, 4);
#endif
        
        /* pwm取值整十， 负数最后一位加一 */
        int l, r;
        l = left_pwm/10;
        r = right_pwm/10;

        
        LCD_Print(0, 4, "L: ");
        LCD_Print_Num(20, 4, l, 4);
        LCD_Print(64, 4, "R: ");
        LCD_Print_Num(84, 4, r, 4);
        
        DataScope(uart_instance, scope_buf);

    }
}
