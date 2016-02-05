#ifndef _8700_2100_H
#define _8700_2100_H

#define Fxas21oo              0x20
#define CTRL_REG1_2100        0x13
#define Fxod8700              0x1e
#define CTRL_REG1_8700        0x2a

#define OUT_X_MSB_REG         0x01
#define OUT_X_LSB_REG         0x02
#define OUT_Y_MSB_REG         0x03
#define OUT_Y_LSB_REG         0x04
#define OUT_Z_MSB_REG         0x05
#define OUT_Z_LSB_REG         0x06

#include "common.h"

typedef struct {
    double gyro_x;
    double gyro_y;
    double gyro_z;
    double accel_x;
    double accel_y;
    double accel_z;
} GyroAccel;

typedef struct {
    double pitch;
    double roll;
    double yaw;
} Rotation;

typedef struct {
    double gyro_x;
    double gyro_y;
    double gyro_z;
    double accel_x;
    double accel_y;
    double accel_z;
    double pitch;
    double roll;
    double yaw;
} Data;

//uint32_t instance;

void GyroAccel_Init(void);
void GyroAccel_Read(float *data);

//Data GyroAccel_Pro(void);

#endif
