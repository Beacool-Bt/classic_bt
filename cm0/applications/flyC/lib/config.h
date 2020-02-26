#ifndef __ANO_CONFIG_H
#define __ANO_CONFIG_H

#include <stdlib.h>
#include "board.h"
#include "pid.h"
#include "filter.h"
#include "imu.h"
#include "scheduler.h"
#include "anolink.h"
#include "motor.h"
#include "rc.h"
#include "control.h"
#include "param.h"
#include "drv_wireless.h"

typedef int32_t  vs32;
typedef  int16_t  vs16;
typedef  int8_t   vs8;

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000)

#define IMU_LOOP_TIME		    2000	//单位为uS
#define PID_INNER_LOOP_TIME		2000	//单位为us
#define PID_OUTER_LOOP_TIME		5000	//单位为us

#define GYRO_CF_TAU 8.5f

#define ACC_LPF_CUT 30.0f	//加速度低通滤波器截止频率
#define GYRO_LPF_CUT 25.0f		//陀螺仪低通滤波器截止频率

//#define GYRO_CF_TAU 1.2f
/*---------------------------------------------*/

/*-------------------无线数据发送方式选择-----------------*/
//#define ANO_DT_USE_Bluetooth
//#define ANO_DT_USE_NRF24l01
/*--------------------------------------------------------*/


typedef struct 
{

	struct Factor{	
			float acc_lpf;		
			float gyro_cf;		
	}factor;

	struct Flag{
			uint8_t ARMED;
			uint8_t failsafe;
	}f;
	
}config_t;
	//指示灯
void Pilot_Light(void);

extern config_t config;

#endif

