#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "adc.h"
#include "pit.h"

#include "LQ_OLED.h"
#include "DataScope_DP.h"
#include "8700_2100.h"

#include "counter.h"
#include "filter.h"
#include "timer.h"

#define LED_ON PEout(26) = 0
#define LED_OFF PEout(26) = 1

uint32_t uart_instance;
    
extern uint32_t ChlValue[2];

Data fin;


// Turn = Gyro_y;
// Balance = Gyro_x, accel_y
    
int main(void)
{   
    DelayInit();

    GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_OPP);   

    uart_instance = UART_QuickInit(UART4_RX_PE25_TX_PE24, 115200);
    
    LCD_Init();
    LCD_Print(20, 2, "Big Brother");
    
//    LED_ON;  // 查看初始化时间
//
//    LED_ON;   // 
    
    GyroAccel_Init();
    
    CounterInit();
    Timer_Init();
    
    while(1)
    {

//        printf("[CH%d]:%4dHz [CH%d]:%4dHz\r\n", 0, ChlValue[0], 1, ChlValue[1]);


#if 0
        fin = GyroAccel_Pro();
        
        scope_buf[0] = fin.gyro_x;
        scope_buf[1] = fin.gyro_y;
        scope_buf[2] = fin.gyro_z;
        scope_buf[3] = fin.accel_x;
        scope_buf[4] = fin.accel_y;
        scope_buf[5] = fin.accel_z;
        scope_buf[6] = fin.pitch;
        scope_buf[7] = fin.roll;
        scope_buf[8] = fin.yaw;
        
        DataScope(uart_instance, scope_buf);
#elseif 0
        int16_t accle[3];
        GyroAccel_Read(Fxod8700, accle);
        
        printf("%d %d %d", accle[0], accle[1], accle[2]);
        DelayMs(100);
#endif
    }
}


