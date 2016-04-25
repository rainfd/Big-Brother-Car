#include "8700_2100.h"
#include "i2c.h"
#include "common.h"
#include "math.h"
#include "uart.h"

/*
    模块平放 前后：pitch 左右：yaw
       竖放 前后：pitch 左右：roll
    Turn = Gyro_y;
    Balance = Gyro_x, accel_y
*/

#define ALPHA_G 0.0078125
#define ALPHA_A 0.000244
#define PI      3.141592

#define GYROSCOPE_RATIO   0.0078125 //128.0//0.0078125//128 (pow 2 7)
#define GRAVITY_RATIO   0.244/1000//0.000244//4096.0//0.000244//4096.0//* 0.000244
#define SENSITIVITY_2G  (4096.0)
#define SENSITIVITY_250 (128.0)

int GYROSCOPE_OFFSET        = -855;//(-401)
int GRAVITY_OFFSET          = 3780;//(14233)
int TURN_OFFSET             = 0;

uint32_t instance;

void GyroAccel_Init(void)
{
    //uint32_t instance;
    instance = I2C_QuickInit(I2C1_SCL_PE01_SDA_PE00,1000);
    
    // gyro 2100
    // I2C_WriteSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t addr, uint8_t data);
    I2C_WriteSingleRegister(instance, Fxas21oo, CTRL_REG0_2100, 0x03);  // High-pass filter disabled, +/-250 dps range -> 7.8125 mdps/LSB = 128 LSB/dps 
    I2C_WriteSingleRegister(instance, Fxas21oo, CTRL_REG1_2100, 0x16);  // ODR = 25Hz, Active mode
    // accel 8700
    I2C_WriteSingleRegister(instance, Fxod8700, XYZ_DATA_CFG, 0x0);     // 0.244mg/LSB +2g
    I2C_WriteSingleRegister(instance, Fxod8700, CTRL_REG2_8700, 0x02);  // High Resolution mode
    I2C_WriteSingleRegister(instance, Fxod8700, CTRL_REG1_8700, 0x25);  // ODR = 25Hz, Reduced noise, Active mode  
    
// OFFSET auto
    /*
    int16_t gyro[3], accel[3];
    int data[6] = {0};
    int i = 0;
    for (i = 0; i < 20; i++) {
        I2CData_Read(Fxas21oo, gyro, 16);
        I2CData_Read(Fxod8700, accel, 14);
        
        data[0] += gyro[0];  // turn
        data[1] += gyro[1];  // gyro
        //data[2] += gyro[2];
        //data[3] += accel[0];
        //data[4] += accel[1];
        data[5] += accel[2]; // angle
    }
    
    TURN_OFFSET = data[0] / 20;
    GYROSCOPE_OFFSET = data[1] / 20;
    GRAVITY_OFFSET = data[5] / 20;
    */
}

void I2CData_Read(uint8_t chipAddr, int16_t *data, int bit)
{

    uint8_t raw[6];
//    I2C_BurstRead(uint32_t instance ,uint8_t chipAddr, uint32_t addr, uint32_t addrLen, uint8_t *buf, uint32_t len);
    I2C_BurstRead(instance, chipAddr, OUT_X_MSB_REG, 1, raw, 6); 
    
    data[0] = (raw[0]<<8) | raw[1];
    data[1] = (raw[2]<<8) | raw[3];
    data[2] = (raw[4]<<8) | raw[5];
    
    if (bit == 14) {
        data[0] = data[0] >> 2;
        data[1] = data[1] >> 2;
        data[2] = data[2] >> 2;
    } else if (bit == 14) {
    
    }
    else {
       return;
    }
}

void GyroAccel_Raw(int16_t *data)
{
    int16_t gyro[3], accel[3];
    
    I2CData_Read(Fxas21oo, gyro, 16);
    I2CData_Read(Fxod8700, accel, 14);
    
    // gyro[0] x gyro[1] y gyro[2] z
    // accel[0] x accel[1] y accel[2] z
    data[0] = gyro[0];
    data[1] = gyro[1];
    data[2] = gyro[2];
    data[3] = accel[0];
    data[4] = accel[1];
    data[5] = accel[2];
}

void GyroAccel_Read(float *data)
{
    int16_t raw[6];
    
    GyroAccel_Raw(raw);
    
//    data[0] = raw[0] / SENSITIVITY_250;
//    data[1] = raw[1] / SENSITIVITY_250;
//    data[2] = raw[2] / SENSITIVITY_250;
//    
//    data[3] = raw[3] / SENSITIVITY_2G;
//    data[4] = raw[4] / SENSITIVITY_2G;
//    data[5] = raw[5] / SENSITIVITY_2G; 
    
//    data[0] = raw[5];
//    data[1] = raw[1];
//    data[2] = raw[0];

    data[0] = (raw[5] - GRAVITY_OFFSET) / SENSITIVITY_2G;
    data[1] = (raw[1] - GYROSCOPE_OFFSET) / SENSITIVITY_250;
    data[2] = (raw[0] - 0) / SENSITIVITY_250;

}


float AngleCal(int raw, int way)
{
    float result;
    
    /* 1 accel 2 gyro 3 turn*/
    if (way == 1)
        result = (raw - GRAVITY_OFFSET) / SENSITIVITY_2G; 
    else if (way == 2)
        result = (raw - GYROSCOPE_OFFSET) / SENSITIVITY_250;
    else if (way == 3)
        result = (raw - TURN_OFFSET) / SENSITIVITY_250;
    
    
    return result;
}