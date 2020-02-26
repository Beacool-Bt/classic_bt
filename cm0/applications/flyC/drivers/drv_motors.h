#ifndef __DRV_MOTORS_H
#define __DRV_MOTORS_H
#include "app_pwm.h"

typedef enum
{
 MOTOR_M1=0,
 MOTOR_M2,
 MOTOR_M3,
 MOTOR_M4,
 MOTOR_NUM_MAX,
}motor_t;
 
void motors_hw_init(void);
void motors_set_pwm(int16_t *pwm);

#endif
