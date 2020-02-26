#include "app_pwm.h"

error_t app_pwm_init(tPWM_CONTROL_BLOCK* bc)
{
	uint16_t pcnt = 0;
	uint16_t ncnt = 0;
	uint32_t acnt = 0;
	if (bc->ch >= PWM_CH_COUNT)
		return ERR_ILLEGAL_PARAM;
	acnt = (24*1000/((bc->Freq)) );
	pcnt = (acnt*bc->duty)/1000;
	ncnt = (acnt*(1000-bc->duty))/1000;
	PWM_SetPnCnt((1 << (bc->ch)), pcnt, ncnt);
	PWM_Start((1 << (bc->ch)));
}

error_t app_pwm_set_duty(tPWM_CONTROL_BLOCK *bc, uint16_t duty)
{
	uint32_t pcnt = 0;
	uint32_t ncnt = 0;
	uint32_t acnt = 0;
	
	if (bc->property&APP_PWM_PROP_ENABLE == 0) 
		return ERR_DEVICE_CLOSED;
	acnt = (24*1000/((bc->Freq)) );
	pcnt = (acnt*bc->duty)/1000;
	ncnt = (acnt*(1000-bc->duty))/1000;
	PWM_SetPnCnt((1 << (bc->ch)), pcnt, ncnt);
	bc->duty = duty;
	return SUCCESS;
}

