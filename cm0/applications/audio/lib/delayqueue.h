/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
#include "yc11xx.h"
#include "ycdef.h"
#include <stdbool.h>
#ifndef DELAY_QUEUE_H
#define DELAY_QUEUE_H

#define DELAY_ZERO 0
#define DELAY_INFINITY -1

//#define DEBUG_DQ

typedef struct
{
	uint32_t timerRemaining;
	list_t list;
	uint16_t size;
	uint8_t pnode[];
}tDELAYQUEUE;

typedef void (*tDQAlarmCb)(uint8_t *pnode, uint16_t Num);

error_t DQ_Init();
bool DQ_CheckEmpty();
bool DQ_CheckFull();
tDELAYQUEUE* DQ_ApplyForNewNode(uint32_t timeStamp);
tDELAYQUEUE* DQ_GetHead();
uint32_t DQ_GetTime2NextAlarm();
void DQ_HandleAlarm(tDQAlarmCb alarmcb);
void DQ_RemoveNode(tDELAYQUEUE* node);

 
 #endif