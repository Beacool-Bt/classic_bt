 /**
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */

/** 
  *@file timer.h
  *@brief timer support for application.
  */
#ifndef DRIVERS_TIMER_YC_TIMER_H_
#define DRIVERS_TIMER_YC_TIMER_H_

#include <stdbool.h>

/**
  *@brief system tick.
  */
#define SYSTEM_CLOCK    (48000000UL)
#define HALFSYSTEM_CLOCK    (24000000UL)
extern uint32_t SYStick_count;

typedef void (*Timer_Expire_CB)(int params);

/**
  *@brief Timer_state.
  */
typedef enum
{
	TIMER_STOP,
	TIMER_START,
}TIMER_STATUS;

/**
  *@brief Timer_type.
  */
typedef enum
{
	TIMER_SINGLE,		/*!< */
	TIMER_CYCLE,		/*!< */
	TIMER_TYPE_BIT=0x80,
}TIMER_TYPE;

/**
  *@brief System timer type.
  */
typedef struct sTimerType
{
	uint32_t mTimerValue;		
	uint32_t mTick;					
	Timer_Expire_CB pfExpireCb;		
	struct sTimerType *pNextTimer;	
	int32_t cbParams;
	TIMER_STATUS mTimerStatus;		
	TIMER_TYPE mIsCycle;		
	uint8_t M0Tick;
}SYS_TIMER_TYPE;

/**
  *@brief Timer_number.
  */
typedef enum
{
	SYS_TIMER_0,
	SYS_TIMER_1,
	SYS_TIMER_2,
	SYS_TIMER_3,
	SYS_TIMER_4,
	SYS_TIMER_5,
	SYS_TIMER_6,
	SYS_TIMER_7,
	SYS_TIMER_8,
}SYS_TIMER_INDEX;



/**
  *@brief Timer initialization.
  *@param None.
  *@return None.
  */
void SYS_TimerInit();

/**
  *@brief This function can set a timer.
  *@param pTimer 
  *@param tick
  *@param type
  *@param pfExpire_CB
  *@retval true sucess.
  *@retval false failure.
  */
bool SYS_SetTimer(SYS_TIMER_TYPE *pTimer, int tick,TIMER_TYPE type,Timer_Expire_CB pfExpire_CB);

/**
  *@brief Timer polling.
  */
void SYS_timerPolling();

/**
  *@brief release timer.
  *@param pTimer .@ref SYS_TIMER_TYPE
  *@retval true success.
  *@retval false failure.
  */
bool SYS_ReleaseTimer(SYS_TIMER_TYPE *pTimer);

/**
  *@brief Timer expire default handle.
  *@param None.
  *@return None.
  */
void SYS_TimerExpireDefaultHandle();

/**
  *@brief release all timer.
  *@param None.
  *@return None.
  */
void SYS_ReleaseAllTimer();

/**
  *@brief  Check timer exist.
  *@param pTimer will be check exist or not.@ref SYS_TIMER_TYPE   
  *@retval true this timer is exist.
  *@retval false the timer is not exist.  
  */
bool SYS_TimerisExist(SYS_TIMER_TYPE *pTimer);

bool SYS_ResetTimer(SYS_TIMER_TYPE *pTimer);

void Bt_100ms_timer(uint8_t count);

void Bt_watchdog();

void SYS_delay_us(uint32_t nus);

void SYS_delay_ms(uint32_t nms);

#endif
