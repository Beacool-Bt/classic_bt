#ifndef __FLYCONTROL_H
#define __FLYCONTROL_H

#include "config.h"

#define FLYANGLE_MAX 350  //���������35��

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

	//��̬�⻷����
void Attitude_Outter_Loop(void);

	//��̬�ڻ�����
void Attitude_Inner_Loop(void);

extern   pid_data_t pid[PIDITEMS];

#endif























