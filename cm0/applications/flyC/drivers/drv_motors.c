#include "drv_motors.h"

tPWM_CONTROL_BLOCK pb0 = {
		0,
		 0,
		APP_PWM_PROP_HIGH_FIRST|APP_PWM_PROP_ENABLE,
		16,
	};
tPWM_CONTROL_BLOCK pb1 = {
		0,
		 1,
		APP_PWM_PROP_HIGH_FIRST|APP_PWM_PROP_ENABLE,
		16,
	};
tPWM_CONTROL_BLOCK pb2 = {
		0,
		 2,
		APP_PWM_PROP_HIGH_FIRST|APP_PWM_PROP_ENABLE,
		16,
	};
tPWM_CONTROL_BLOCK pb3 = {
		0,
		 3,
		APP_PWM_PROP_HIGH_FIRST|APP_PWM_PROP_ENABLE,
		16,
	};

//初始化电机pwm输出，频率为16khz
void motors_hw_init(void)
{
    HWRITE(CORE_GPIO_CONF + 23, GPCFG_PWM_OUT0); //设置 gpio23 为pwm0
    HWRITE(CORE_GPIO_CONF + 24, GPCFG_PWM_OUT1); //设置 gpio24 为pwm1
    HWRITE(CORE_GPIO_CONF + 25, GPCFG_PWM_OUT2); //设置 gpio25 为pwm2
    HWRITE(CORE_GPIO_CONF + 26, GPCFG_PWM_OUT3); //设置 gpio26 为pwm3
    HWCOR(CORE_CLKOFF + 1, 0x20);
    
    app_pwm_init(&pb0);
    app_pwm_init(&pb1);
    app_pwm_init(&pb2);
    app_pwm_init(&pb3);
}
//设置4个电机的pwm占空比输出
void motors_set_pwm(int16_t *pwm)
{
  //设置电机1的pwm占比输出，pwm[0]的值为0-1000，换算成对应的pwm占空比输出
  app_pwm_set_duty(&pb0,pwm[0]);
  //设置电机2的pwm占比输出，pwm[1]的值为0-1000，换算成对应的pwm占空比输出
  app_pwm_set_duty(&pb1,pwm[1]);
  //设置电机3的pwm占比输出，pwm[2]的值为0-1000，换算成对应的pwm占空比输出
  app_pwm_set_duty(&pb2,pwm[2]);
  //设置电机4的pwm占比输出，pwm[3]的值为0-1000，换算成对应的pwm占空比输出
  app_pwm_set_duty(&pb3,pwm[3]);
}
