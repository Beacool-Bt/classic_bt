#ifndef __ANO_IMU_H
#define __ANO_IMU_H

#include "config.h"

typedef struct ANO_IMU
{
	
	//欧拉角表示的飞行器姿态
	Vector3f_t angle;
	
	Vector3i_t Gyro, Acc; 
	Vector3f_t Gyrof, Accf; 
    Vector3f_t acc_Lpf, gyro_Lpf;

  	Vector3f_t gyroAccel;
	Vector3f_t vectorRollPitch;
	Vector3f_t vectorYaw; 	

	float accMag, accMagderi;	
	
	
    LPF2ndData_t Acc_lpf_2nd;
	LPF2ndData_t Gyro_lpf_2nd;
	float gyro_cf;	
	float magHold, headFreeHold;
	uint8_t initAlignment;
}imu_data_t;



void IMU_Init(void);
	
	//更新传感器数据
void IMU_UpdateSensor(void);	
	
	//计算飞行器姿态
void IMU_GetAttitude(void);

void IMU_Update(Vector3f_t acc, Vector3f_t gyro, float dt);

void IMU_DCM(Vector3i_t gyro, Vector3f_t acc);

extern imu_data_t imu_data;

#endif

