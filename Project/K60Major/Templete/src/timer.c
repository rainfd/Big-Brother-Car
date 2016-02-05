#include "timer.h"

#include "pit.h"
#include "common.h"
//#include "adc.h"

#include "DataScope_DP.h"
#include "8700_2100.h"

#include "filter.h"

void Timer_Init(void)
{       
    PIT_QuickInit(HW_PIT_CH0, 5000); // 5ms周期
    
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR); // 注册PIT 中断回调函数 0号定时器的中断处理
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true); // 开启PIT0定时器中断
}

float xi = 0, yi = 0, zi = 0;

void PIT_ISR(void)
{

    
//    int16_t gyro_x = gyroaccel[0], gyro_y = gyroaccel[1], gyro_z =  gyroaccel[2],
//            accel_x = gyroaccel[3], accel_y = gyroaccel[4], accel_z = gyroaccel[5];
    
    
#if 1

    float gyroaccel[6];
    
    GyroAccel_Read(gyroaccel);
        
    scope_buf[0] = gyroaccel[0];
    scope_buf[1] = gyroaccel[1];
    scope_buf[2] = gyroaccel[2];
    scope_buf[3] = gyroaccel[3];
    scope_buf[4] = gyroaccel[4];
    scope_buf[5] = gyroaccel[5];
 
//    scope_buf[6] = yi;
 
    scope_buf[7] = Kalman_Filter(gyroaccel[5], gyroaccel[1]);	
    scope_buf[8] = Complementary_Filter(gyroaccel[5], gyroaccel[1]);
    scope_buf[9] = Complementary_Filter2(gyroaccel[5], gyroaccel[1]);
    
#endif

}