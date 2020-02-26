#ifndef __ANO_MOTOR_H
#define __ANO_MOTOR_H

#include "config.h"

#define MOTORS_NUM_MAX  4

#define MINTHROTTLE 1100
#define MAXTHROTTLE 2000


	void Motor_WriteMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw);
	
	void Motor_GetPWM(uint16_t* pwm);



#endif





