#ifndef __FLYCONTROL_H
#define __FLYCONTROL_H

#include "config.h"

#define FLYANGLE_MAX 350  //最大飞行倾角35度

enum {
    PIDROLL,
    PIDPITCH,
    PIDYAW,
		PIDVELX,
		PIDVELY,
    PIDVELZ,
    PIDALT,
    PIDLEVEL,
    PIDMAG,
		PIDITEMS
};


void FlyControl_Init(void);

	//姿态外环控制
void Attitude_Outter_Loop(void);

	//姿态内环控制
void Attitude_Inner_Loop(void);

extern   pid_data_t pid[PIDITEMS];

#endif























