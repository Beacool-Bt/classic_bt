/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */

  /** @file
 *
 * pwm support for application
 */
 #ifndef _APP_PWM_H_
#define _APP_PWM_H_
#include "yc11xx_pwm.h"
#include "app_util_platform.h"

#define APP_PWM_PROP_SYNC_PWM0 (1<<3)
#define APP_PWM_PROP_HIGH_FIRST	(1<<4)
#define APP_PWM_PROP_ENABLE		(1<<5)

typedef struct{
	uint16_t duty;		//0~1000
	uint8_t ch;		//ch
	uint8_t property;
	uint16_t Freq;		//khz
}tPWM_CONTROL_BLOCK;


error_t app_pwm_init(tPWM_CONTROL_BLOCK* bc);
error_t app_pwm_set_duty(tPWM_CONTROL_BLOCK *bc, uint16_t duty);

#endif
