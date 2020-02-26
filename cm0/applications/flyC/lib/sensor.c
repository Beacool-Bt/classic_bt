#include "param.h"
#include "sensor.h"
#include "mpu6050.h"
#include "yc11xx_iic.h"

struct Sensor  sensor;

uint8_t buffer[20]; //接收数据缓存区
static Vector3i_t Acc_ADC,Gyro_ADC;
static Vector3f_t Gyro_dps;

	//加速度零偏矫正
static	void Sensor_CalOffsetAcc(void);
	//陀螺仪零偏矫正
static	void Sensor_CalOffsetGyro(void);
//MPU6050初始化，传入参数：采样率，低通滤波频率
void Sensor_Init(void)
{
	 //lsm6ds3_hw_init();
	// mpu6500_init();
	mpu6050_init();
}

//读取加速度和角速度
void Sensor_ReadData(void)
{    
	int16_t acc_temp[3];
	int16_t gyro_temp[3];
	
	//LSM6DS3_ReadData(buffer,12);
	I2C_ReadDatatoBuff(MPU6050_DEFAULT_ADDRESS, I2C_REGADDR8BITS(MPU_RA_ACCEL_XOUT_H), buffer, 14);
  acc_temp[0] = ((((int16_t)buffer[0]) << 8) | buffer[1])- sensor.Acc_Offset.x;  //加速度X轴
  acc_temp[1] = ((((int16_t)buffer[2]) << 8) | buffer[3])- sensor.Acc_Offset.y;  //加速度Y轴
  acc_temp[2] = ((((int16_t)buffer[4]) << 8) | buffer[5])- sensor.Acc_Offset.z;  //加速度Z轴
	
  gyro_temp[0] = ((((int16_t)buffer[8]) << 8) | buffer[9])- sensor.Gyro_Offset.x;  //加速度X轴
  gyro_temp[1] = ((((int16_t)buffer[10]) << 8) | buffer[11])- sensor.Gyro_Offset.y;  //加速度Y轴
  gyro_temp[2] = ((((int16_t)buffer[12]) << 8) | buffer[13])- sensor.Gyro_Offset.z;  //加速度Z轴

 Acc_ADC.x = acc_temp[0];
 Acc_ADC.y = acc_temp[1];
 Acc_ADC.z = acc_temp[2];
	
 Gyro_ADC.x = gyro_temp[0];
 Gyro_ADC.y = gyro_temp[1];
 Gyro_ADC.z = gyro_temp[2];

	Sensor_CalOffsetGyro();
	Sensor_CalOffsetAcc();
}

Vector3i_t Sensor_GetAcc(void)
{
 return Acc_ADC;	
}

Vector3i_t Sensor_GetGyro(void)
{
 return Gyro_ADC;		
}

Vector3f_t Sensor_GetGyro_in_dps(Vector3i_t gyro)
{
	Gyro_dps.x = radians(gyro.x * MPU6050G_S2000DPS);   // dps
	Gyro_dps.y = radians(gyro.y * MPU6050G_S2000DPS);   // dps
	Gyro_dps.z = radians(gyro.z * MPU6050G_S2000DPS);   // dps	
	
	return Gyro_dps;
}

//加速度零偏矫正
static void Sensor_CalOffsetAcc(void)
{
	if(sensor.Acc_CALIBRATED)
		{
				static Vector3f_t	tempAcc;
			static uint16_t cnt_a=0;

			if(cnt_a==0)
			{
				sensor.Acc_Offset.x = 0;
				sensor.Acc_Offset.y = 0;
				sensor.Acc_Offset.z = 0;
				tempAcc.x = 0;
				tempAcc.y = 0;
				tempAcc.z = 0;
				cnt_a = 1;
				return;
			}			
			tempAcc.x += Acc_ADC.x;
			tempAcc.y += Acc_ADC.y;
			tempAcc.z += Acc_ADC.z;
			if(cnt_a == CALIBRATING_ACC_CYCLES)
			{
				sensor.Acc_Offset.x = tempAcc.x/cnt_a;
				sensor.Acc_Offset.y = tempAcc.y/cnt_a;
				sensor.Acc_Offset.z = tempAcc.z/cnt_a - ACC_1G;
				cnt_a = 0;
				sensor.Acc_CALIBRATED = 0;
				Param_SaveAccelOffset();//保存数据
				return;
			}
			cnt_a++;		
		}	
	
}

//陀螺仪零偏矫正
static void Sensor_CalOffsetGyro(void)
{
	if(sensor.Gyro_CALIBRATED)
	{
		static Vector3f_t	tempGyro;
		static uint16_t cnt_g=0;
		if(cnt_g==0)
		{
			sensor.Gyro_Offset.x = 0;
           sensor.Gyro_Offset.y = 0;
			sensor.Gyro_Offset.z = 0;
			tempGyro.x = 0;
			tempGyro.y = 0;
			tempGyro.z = 0;
			cnt_g = 1;
			return;
		}
		tempGyro.x += Gyro_ADC.x;
		tempGyro.y += Gyro_ADC.y;
		tempGyro.z += Gyro_ADC.z;
		if(cnt_g == CALIBRATING_GYRO_CYCLES)
		{
			sensor.Gyro_Offset.x = tempGyro.x/cnt_g;
			sensor.Gyro_Offset.y = tempGyro.y/cnt_g;
			sensor.Gyro_Offset.z = tempGyro.z/cnt_g;
			cnt_g = 0;
			sensor.Gyro_CALIBRATED = 0;
			Param_SaveGyroOffset();//保存数据
			return;
		}
		cnt_g++;
	}
}

