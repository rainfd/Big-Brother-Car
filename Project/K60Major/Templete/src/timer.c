#include "timer.h"

#include "pit.h"
#include "common.h"
//#include "adc.h"

#include "mma7361.h"
#include "l3g4200d.h"
#include "DataScope_DP.h"

#include "filter.h"

void Timer_Init(void)
{       
    PIT_QuickInit(HW_PIT_CH0, 5000);
    
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR); // 注册PIT 中断回调函数 0号定时器的中断处理
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true); // 开启PIT0定时器中断
}

float xi = 0, yi = 0, zi = 0;

void PIT_ISR(void)
{
//    int16_t gyroaccel[6];
//    
//    GyroAccel_Read(gyroaccel);
    
    
    
#if 0

    yi += dps.y * 0.005;
    
    scope_buf[0] = rot.pitch;
    scope_buf[1] = dps.y;
//    
    scope_buf[6] = yi;
//    
    scope_buf[7] = Kalman_Filter(rot.pitch, dps.y);	
    scope_buf[8] = Complementary_Filter(rot.pitch, dps.y);
    scope_buf[9] = Complementary_Filter2(rot.pitch, dps.y);
#endif

}