#include "8700_2100.h"
#include "i2c.h"
#include "common.h"
#include "math.h"
#include "uart.h"

#define ALPHA_G 0.0078125
#define ALPHA_A 0.000244
#define PI      3.141592

uint32_t instance;

void GyroAccel_Init(void)
{
//    uint32_t instance;
    
    instance = I2C_QuickInit(I2C1_SCL_PE01_SDA_PE00,1000);
    
    // gyro 2100
//    I2C_WriteSingleRegister(uint32_t instance, uint8_t chipAddr, uint8_t addr, uint8_t data);
    I2C_WriteSingleRegister(instance, Fxas21oo, 0x0d, 0x02); // 250 7.8125
    I2C_WriteSingleRegister(instance, Fxas21oo, CTRL_REG1_2100, 0x02); // active
    
    // accel 8700
    I2C_WriteSingleRegister(instance, Fxod8700, CTRL_REG1_8700, 0x05); // active
}

void I2CData_Read(uint8_t chipAddr, int16_t *data)
{

    uint8_t raw[6];
//    I2C_BurstRead(uint32_t instance ,uint8_t chipAddr, uint32_t addr, uint32_t addrLen, uint8_t *buf, uint32_t len);
    I2C_BurstRead(instance, chipAddr, OUT_X_MSB_REG, 1, raw, 6); 
    
    data[0] = (raw[0]<<8) | raw[1];
    data[1] = (raw[2]<<8) | raw[3];
    data[2] = (raw[4]<<8) | raw[5];
    
}

void GyroAccel_Read(int16_t *data)
{
    int16_t gyro[3], accel[3];
    
    I2CData_Read(Fxas21oo, gyro);
    I2CData_Read(Fxod8700, accel);
    
    data[0] = gyro[0];
    data[1] = gyro[1];
    data[2] = gyro[2];
    data[3] = accel[0];
    data[4] = accel[1];
    data[5] = accel[2];
}

//GyroAccel GyroAccel_Filter(const int16_t *gyro, const int16_t *accel)
//{
//    GyroAccel dps;
//    
//    dps.gyro_x = gyro[0] * ALPHA_G;
//    dps.gyro_y = gyro[1] * ALPHA_G;
//    dps.gyro_z = gyro[2] * ALPHA_G;
//    
//    dps.accel_x = accel[0] * ALPHA_A;
//    dps.accel_y = accel[1] * ALPHA_A;
//    dps.accel_z = accel[2] * ALPHA_A;
//    
//    return dps;
//}

Rotation Accel_Cal(const GyroAccel raw)
{
    double x, y, z;
    Rotation dps;
    
    x = raw.accel_x;
    y = raw.accel_y;
    z = raw.accel_z;
    
    dps.pitch = (atan2(x, sqrt(y*y + z*z)) * 180.0) / PI;
    dps.roll = (atan2(y, sqrt(x*x + z*z)) * 180.0) / PI;
    dps.yaw = (atan2(z, sqrt(x*x + y*y)) * 180.0) / PI;
                
    
    return dps;
}

//Fxas21oo
//Fxod8700
/*
Data GyroAccel_Pro(void)
{   
    int16_t gyro[3], accel[3];
    GyroAccel dps;
    Rotation data;
    Data fin;
    
//    GyroAccel_Init();
    
    GyroAccel_Read(Fxas21oo, gyro);
    GyroAccel_Read(Fxod8700, accel);
       
    dps = GyroAccel_Data(gyro, accel);
    data = Accel_Cal(dps);
    
    fin.gyro_x = dps.gyro_x;
    fin.gyro_y = dps.gyro_y;
    fin.gyro_z = dps.gyro_z;
    fin.accel_x = dps.accel_x;
    fin.accel_y = dps.accel_y;
    fin.accel_z = dps.accel_z;
    fin.pitch = data.pitch;
    fin.roll = data.roll;
    fin.yaw = data.yaw;
    
    return fin;
//    printf("x:%f y:%f z:%f pitch:%f roll:%f yaw:%f", dps.gyro_x, dps.gyro_y, dps.gyro_z, data.pitch, data.roll, data.yaw);
}
*/