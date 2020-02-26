
#include "control.h"


pid_data_t pid[PIDITEMS];
static	uint8_t yawRate;
static	int32_t RateError[3];

void FlyControl_Init()
{
	yawRate = 100;
	//重置PID参数
    PID_Set(&pid[PIDROLL],0.26, 0.15, 0.01, 200);
	PID_Set(&pid[PIDPITCH],0.26, 0.15, 0.01, 200);
	PID_Set(&pid[PIDYAW],0.8 , 0.35, 0, 200);
	PID_Set(&pid[PIDLEVEL],3.0, 0, 0, 0);
	PID_Set(&pid[PIDMAG],0.35, 0, 0, 0);
}


//飞行器姿态外环控制
void Attitude_Outter_Loop(void)
{
	int32_t	errorAngle[2];
	Vector3f_t Gyro;
	
	//计算角度误差值
	errorAngle[ROLL] = constrain_int32((rc_data.Command[ROLL] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu_data.angle.x * 10; 
	errorAngle[PITCH] = constrain_int32((rc_data.Command[PITCH] * 2) , -((int)FLYANGLE_MAX), +FLYANGLE_MAX) - imu_data.angle.y * 10; 
	
	//获取角速度
	Gyro.x = imu_data.Gyro.x/4.0f;
	Gyro.y = imu_data.Gyro.y/4.0f;
	Gyro.z = imu_data.Gyro.z/4.0f;
	
	//得到外环PID输出,作为内环的期望值，减去角速度测量值，得到内环PID的输入
	RateError[ROLL] = PID_GetP(&pid[PIDLEVEL],errorAngle[ROLL]) - Gyro.x;
	RateError[PITCH] = PID_GetP(&pid[PIDLEVEL],errorAngle[PITCH]) + Gyro.y;
	RateError[YAW] = ((int32_t)(yawRate) * rc_data.Command[YAW]) / 32 - Gyro.z;		
}

//飞行器姿态内环控制
void Attitude_Inner_Loop(void)
{
	uint8_t i = 0;
	int32_t PIDTerm[3];
	
	for(i=0; i<3;i++)
	{
		//当油门低于检查值时积分清零
		if ((rc_data.rawData[THROTTLE]) < RC_MINCHECK)	
		{
			PID_ResetI(&pid[i]);
		}
		//得到内环PID输出
		PIDTerm[i] = PID_GetPID(&pid[i],RateError[i], PID_INNER_LOOP_TIME*1e-6);
	}
	
	PIDTerm[YAW] = -constrain_int32(PIDTerm[YAW], -500 - abs(rc_data.Command[YAW]), +500 + abs(rc_data.Command[YAW]));	
		
	//PID输出转为电机控制量
	Motor_WriteMotor(rc_data.Command[THROTTLE], PIDTerm[ROLL], PIDTerm[PITCH], PIDTerm[YAW]);
}	
	
