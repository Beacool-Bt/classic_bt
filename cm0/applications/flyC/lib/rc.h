#ifndef __RC_H
#define __RC_H

#include "config.h"

#define RC_MID  			1500                     
#define RC_MINCHECK		1150                      
#define RC_MAXCHECK  	1850 
#define RC_MINTHROTTLE	1150                      
#define RC_MAXTHROTTLE 	1850 

enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
    AUX1,
    AUX2,
    AUX3,
    AUX4,
		AUX5,
		AUX6
};

typedef struct 
{
	uint16_t rawData[10];
	int16_t Command[4];
}rc_data_t;

//参数初始化
void RC_Init(void);
//遥控通道数据处理
void RC_CalCommand(void);
//摇杆位置检测
void RC_CheckSticks(void);

extern rc_data_t rc_data;

#endif



