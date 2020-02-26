#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include "MyMath.h"

typedef struct 
{
	
	//PID����
	float kP;
	float kI;
	float kD;
	
	//��������
	float imax;
	//��������ֵ
	float integrator; 
	//��һ�ε��������
	int32_t last_error;
	//��һ�ε�΢��ֵ����ͨ�˲�������
	float	last_derivative;   

} pid_data_t;


//����PID�����ֵ
int32_t PID_GetPID(pid_data_t *pid,int32_t error, float dt);
int32_t PID_GetPI(pid_data_t *pid,int32_t error, float dt);
int32_t PID_GetP(pid_data_t *pid,int32_t error);
int32_t PID_GetI(pid_data_t *pid,int32_t error, float dt);
int32_t PID_GetD(pid_data_t *pid,int32_t error, float dt);

//���ֿ�������ֵ����
void PID_ResetI(pid_data_t *pid);
	
void PID_Set(pid_data_t *pid,const float p,const float i,const float d,const float  imaxval);
	

#endif

