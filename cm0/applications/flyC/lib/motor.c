
#include "motor.h"

int16_t motorPWM[4];	

void Motor_WriteMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw)
{
	uint8_t i = 0;
	//����X��

	motorPWM[1] = throttle + pidTermRoll + pidTermPitch - pidTermYaw; //ǰ��
	motorPWM[2] = throttle - pidTermRoll + pidTermPitch + pidTermYaw; //ǰ��
	motorPWM[3] = throttle - pidTermRoll - pidTermPitch - pidTermYaw; //����
	motorPWM[0] = throttle + pidTermRoll - pidTermPitch + pidTermYaw; //����
	
	int16_t maxMotor = motorPWM[0];
	for (i = 1; i < MOTORS_NUM_MAX; i++)
	{
		if (motorPWM[i] > maxMotor)
					maxMotor = motorPWM[i];				
	}
	
	for (i = 0; i < MOTORS_NUM_MAX; i++) 
	{
		if (maxMotor > MAXTHROTTLE)    
			motorPWM[i] -= maxMotor - MAXTHROTTLE;	
		//���Ƶ��PWM����С�����ֵ
		motorPWM[i] = constrain_int16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);
	}	

	//���δ�������򽫵���������Ϊ���
	if(!config.f.ARMED || rc_data.rawData[THROTTLE] < 1100)	
		for(i=0; i< MOTORS_NUM_MAX ; i++)
			{
			motorPWM[i] = 1000;
		    }
	for (i = 0; i < MOTORS_NUM_MAX; i++)
		{
		motorPWM[i]-=1000;
		}

	//д����PWM
	motors_set_pwm(motorPWM);
	
}

void Motor_GetPWM(uint16_t* pwm)
{
	*(pwm) = motorPWM[0];
	*(pwm+1) = motorPWM[1];
	*(pwm+2) = motorPWM[2];
	*(pwm+3) = motorPWM[3];
}


