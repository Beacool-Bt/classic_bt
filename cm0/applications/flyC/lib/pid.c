#include "pid.h"

// Examples for _filter:
// f_cut = 10 Hz -> _filter = 15.9155e-3
// f_cut = 15 Hz -> _filter = 10.6103e-3
// f_cut = 20 Hz -> _filter =  7.9577e-3
// f_cut = 25 Hz -> _filter =  6.3662e-3
// f_cut = 30 Hz -> _filter =  5.3052e-3

/// 微分项的低通滤波器截止频率
static const float  _filter = 7.9577e-3; // 由 "1 / ( 2 * PI * f_cut )"这个公式计算得来;

void PID_Set(pid_data_t *pid,const float p,const float i,const float d,const float  imaxval)
{
  pid->kP = p;
	pid->kI = i;
	pid->kD = d;
	pid->imax = imaxval;
}
int32_t PID_GetP(pid_data_t *pid,int32_t error)
{
    return (float)error * pid->kP;
}

int32_t PID_GetI(pid_data_t *pid,int32_t error, float dt)
{
    if((pid->kI != 0) && (dt != 0)) {
        pid->integrator += ((float)error * pid->kI) * dt;
				//积分限幅
				pid->integrator = constrain_float(pid->integrator, -pid->imax, +pid->imax);		
				
        return pid->integrator;
    }
    return 0;
}

void PID_ResetI(pid_data_t *pid)
{
	pid->integrator = 0;
}

int32_t PID_GetD(pid_data_t *pid,int32_t error, float dt)
{
	if ((pid->kD != 0) && (dt != 0)) {
			float derivative;
	if (isnan(pid->last_derivative)) {		
		derivative = 0;
		pid->last_derivative = 0;
	} else {
		derivative = (error - pid->last_error) / dt;
	}

	// 一阶离散低通滤波器，降低高频噪声对控制器的干扰
	derivative = pid->last_derivative +
								(dt / ( _filter + dt)) * (derivative - pid->last_derivative);

	pid->last_error	= error;
	pid->last_derivative = derivative;

	return pid->kD * derivative;
	}
    return 0;
}

int32_t PID_GetPI(pid_data_t *pid,int32_t error, float dt)
{
    return PID_GetP(pid,error) + PID_GetI(pid,error, dt);
}

int32_t PID_GetPID(pid_data_t *pid,int32_t error, float dt)
{
    return PID_GetP(pid,error) + PID_GetI(pid,error, dt) + PID_GetD(pid,error, dt);
}

