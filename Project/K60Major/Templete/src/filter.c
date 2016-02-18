#include "filter.h"


// һ,���׻����˲����� �Լ��ٶȼ�ȡֵ��Ȩ��
const float K = 0.1;//0.02;
const float K1 = 0.05; 
const float K2 = 0.2;
	
// �������˲�����
float Q_angle = 0.001;        // ����������Э����   һ��һ�����еľ���
float Q_gyro = 0.003;         // ����������Э����   һ��һ�����еľ���
float R_angle = 0.5;          // ����������Э���� ������ƫ��)

const float dt = 0.005;             //  �������� 5ms

float angle, angle_dot;      // Kalman
float angle_com, angle_com2;             // Complementary

float x1, x2, y1;

char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = {0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };


float Kalman_Filter(float Accel,float Gyro)		
{   
	angle += (Gyro - Q_bias) * dt;              // �������
	Pdot[0] = Q_angle - PP[0][1] - PP[1][0];    // Pk ����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;
	PP[0][0] += Pdot[0] * dt;                   // Pk ����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;                   // ����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - angle;	                // Zk �������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		                // ����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angle += K_0 * Angle_err;	                // �������
	Q_bias += K_1 * Angle_err;	                // �������
	angle_dot = Gyro - Q_bias;	                // ���ֵ(�������)��΢��=���ٶ�
    
    return angle;
}

// һ�׻����˲�
float Complementary_Filter(float angle_m, float gyro_m)
{
    
    angle_com = (angle_com + gyro_m * dt) * (1 - K) + K * angle_m;
    
    return angle_com;
}

// ���׻����˲�
float Complementary_Filter2(float angle_m, float gyro_m)
{   
    x1 = (angle_m - angle_com2) * (1 - K2) * (1 - K2);
    y1 += x1 * dt;
    x2 = y1 + 2 * (1 - K2) * (angle_m - angle_com2) + gyro_m;
    angle_com2 = angle_com + x2 * dt;
    
    return angle_com2;
}
/*
float Complementary_Filter(float angle_m, float gyro_m)
{
    
    angle_com = (angle_com + gyro_m * 0.005) * (1 - K) + K * angle_m;
    
    return angle_com;
}
*/