#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include "MyMath.h"

typedef struct 
{
	
	//PID参数
	float kP;
	float kI;
	float kD;
	
	//积分上限
	float imax;
	//积分器的值
	float integrator; 
	//上一次的误差输入
	int32_t last_error;
	//上一次的微分值，低通滤波计算用
	float	last_derivative;   

} pid_data_t;


//返回PID计算的值
int32_t PID_GetPID(pid_data_t *pid,int32_t error, float dt);
int32_t PID_GetPI(pid_data_t *pid,int32_t error, float dt);
int32_t PID_GetP(pid_data_t *pid,int32_t error);
int32_t PID_GetI(pid_data_t *pid,int32_t error, float dt);
int32_t PID_GetD(pid_data_t *pid,int32_t error, float dt);

//积分控制器的值清零
void PID_ResetI(pid_data_t *pid);
	
void PID_Set(pid_data_t *pid,const float p,const float i,const float d,const float  imaxval);
	

#endif

