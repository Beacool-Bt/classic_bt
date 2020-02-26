/**
 * COPYRIGHT NOTICE
 *Copyright(c) 2014,YICHIP
 *
 * @file yc_timer.c
 * @brief ...
 *
 * @version 1.0
 * @author  jingzou
 * @data    Jan 23, 2018
**/

#include <stdbool.h>
#include <stdio.h>
#include "yc_timer.h"
#include "yc11xx.h"
#include "drv_debug.h"

#define ENABLE_BT_100MS_CLK
#define BT_WATCHDOG 500
SYS_TIMER_TYPE *pheader;
SYS_TIMER_TYPE header_Timer;
uint32_t sys_tick_count=0;
uint32_t bt_100ms_count=0;
uint32_t bt_watch_dog=0;
uint8_t  sys_Timer_Check_Flag;

void SYS_TimerExpireDefaultHandle(int params)
{
	// YC_LOG_ERROR("default timer expire !\r\n");
	while(0);
}

void SYS_TimerInit()
{
	//initial timer lists
	pheader = &header_Timer;
	pheader->pNextTimer = NULL;
	pheader->pfExpireCb = SYS_TimerExpireDefaultHandle;
	pheader->mTimerStatus = TIMER_START;
	pheader->mTimerValue = 0;

	SysTick_Config(SYSTEM_CLOCK/100); //each  systick interrupt is 10ms
}


//union is 20ms
bool SYS_SetTimer(SYS_TIMER_TYPE *pTimer, int tick,TIMER_TYPE isSingle,Timer_Expire_CB pfExpire_CB)
{
	SYS_TIMER_TYPE *pTemp;
	if(pTimer == NULL)
	{
		return false;
	}

	if (pfExpire_CB == NULL)
	{
		pTimer->pfExpireCb = SYS_TimerExpireDefaultHandle;
	}
	else
	{
		pTimer->pfExpireCb = pfExpire_CB;
	}

	OS_ENTER_CRITICAL();
#ifdef ENABLE_BT_100MS_CLK
	if (isSingle & TIMER_TYPE_BIT)
	{
		pTimer->M0Tick = 0;
		pTimer->mTimerValue = bt_100ms_count+tick;
	}
	else
#endif //ENABLE_BT_100MS_CLK
	{
		pTimer->M0Tick = 1;
		pTimer->mTimerValue = sys_tick_count+tick;
	}
	
	pTimer->mTick = tick;
	
	pTimer->mTimerStatus = TIMER_START;
	pTimer->mIsCycle = isSingle &0x3f;

	if (SYS_TimerisExist(pTimer))
	{
		OS_EXIT_CRITICAL();
		return true;
	}
	
	//insert to list
	pTemp = pheader;
	pheader = pTimer;
	pheader->pNextTimer = pTemp;
	OS_EXIT_CRITICAL();

	//YC_LOG_INFO("set timer %d \r\n",pTimer->cbParams);

	return true;
}

bool SYS_TimerisExist(SYS_TIMER_TYPE *pTimer)
{
	SYS_TIMER_TYPE *pTemp;
	if (pTimer == NULL)
	{
		return false;
	}
	for(pTemp = pheader; pTemp->pNextTimer != NULL; pTemp = pTemp->pNextTimer)
	{
		if (pTimer == pTemp)
		{
			return true;
		}
	}
	return false;
}


bool SYS_ResetTimer(SYS_TIMER_TYPE *pTimer)
{
	SYS_TIMER_TYPE *pTemp;
	if (pTimer == NULL)
	{
		return false;
	}
	for(pTemp = pheader; pTemp->pNextTimer != NULL; pTemp = pTemp->pNextTimer)
	{
		if (pTimer == pTemp)
		{
			if (pTimer->M0Tick == 1)
				pTimer->mTimerValue = sys_tick_count+(pTimer->mTick);
			else
				pTimer->mTimerValue = bt_100ms_count+(pTimer->mTick);
			return true;
		}
	}
	return false;
}


void SYS_TimerTest()
{
	SYS_TIMER_TYPE *pTimer;
	int i = 0;
	for (pTimer = pheader; pTimer->pNextTimer != NULL; pTimer = pTimer->pNextTimer)
	{
		i++;
	}
	//YC_LOG_INFO("timer count %d \r\n",i);
}

void SYS_timerPolling()
{
	SYS_TIMER_TYPE *pTimer;
	if (sys_Timer_Check_Flag)
	{
		sys_Timer_Check_Flag = false;
		SYS_TimerTest();
		for (pTimer = pheader; pTimer->pNextTimer != NULL; pTimer = pTimer->pNextTimer)
		{
			if (pTimer->mTimerStatus == TIMER_START)
			{
				//YC_LOG_INFO("timer %d in\r\n",pTimer->cbParams);
				if (pTimer->M0Tick == 1)
				{
					if (pTimer->mTimerValue <= sys_tick_count)
					{
						if (pTimer->mIsCycle == TIMER_SINGLE)
						{
							//YC_LOG_INFO("timer release %d in\r\n",pTimer->cbParams);
							SYS_ReleaseTimer(pTimer);
						}
						else
						{
							//YC_LOG_INFO("timer cycle in\r\n");
							//YC_LOG_INFO("timer release %d in\r\n",pTimer->cbParams);
							pTimer->mTimerValue = sys_tick_count+(pTimer->mTick);
						}
						pTimer->pfExpireCb(pTimer->cbParams);
					}
				}
				else
				{
					if (pTimer->mTimerValue <= bt_100ms_count)
					{
						if (pTimer->mIsCycle == TIMER_SINGLE)
						{
							//YC_LOG_INFO("timer release %d in\r\n",pTimer->cbParams);
							SYS_ReleaseTimer(pTimer);
						}
						else
						{
							//YC_LOG_INFO("timer cycle in\r\n");
							//YC_LOG_INFO("timer release %d in\r\n",pTimer->cbParams);
							pTimer->mTimerValue = bt_100ms_count+(pTimer->mTick);
						}
						pTimer->pfExpireCb(pTimer->cbParams);
					}
				}
			}
		}
	}
}

bool SYS_ReleaseTimer(SYS_TIMER_TYPE *pTimer)
{
	SYS_TIMER_TYPE *pTemp, *pPre;
	if (pTimer == NULL)
	{
		return false;
	}

	if (pTimer == &header_Timer)
	{
		return false;
	}

	for(pTemp = pPre = pheader; pTemp->pNextTimer != NULL; pPre = pTemp,pTemp = pTemp->pNextTimer)
	{
		if (pTimer == pTemp)
		{
			pTemp->mTimerStatus = TIMER_STOP;
			if (pPre == pTemp) //delete the first one element
			{
				pheader = pTemp->pNextTimer;
			}
			else
			{
				pPre->pNextTimer = pTemp->pNextTimer;
			}

			//YC_LOG_INFO("release timer %d,%d \r\n",pPre->cbParams,pTimer->cbParams);
			return true;
		}
	}
	return false;
}

void SYS_ReleaseAllTimer()
{
	SYS_TIMER_TYPE *pTemp;
	for(pTemp = pheader; pTemp->pNextTimer != NULL; pTemp = pTemp->pNextTimer)
	{
		pTemp->mTimerStatus = TIMER_STOP;
	}
	pheader = &header_Timer;
}

void SYStick_handle()
{
	sys_tick_count++; //10
	sys_Timer_Check_Flag = true;
	bt_watch_dog++;
}

void Bt_100ms_timer(uint8_t count)
{
	bt_100ms_count+=count;
	bt_watch_dog=0;
	sys_Timer_Check_Flag = true;
}

void Bt_watchdog()
{
	return;
	if (bt_watch_dog > BT_WATCHDOG)
	{
		OS_ENTER_CRITICAL();
		DEBUG_LOG(LOG_LEVEL_CORE, "Error" ,"Bt_watchdog: 0x%04X", LOG_POINT_FFF9, ERROR_CODE_WATCH_DOG);
		//HWRITE(CORE_RESET,0x03);
		while(1);
	}
}

void SYS_delay_ms(uint32_t nms)
{
	SYS_delay_us(nms*1000);
}

void SYS_delay_us(uint32_t nus){
	uint32_t ticks;    
	uint32_t told,tnow,tcnt=0;    
	uint32_t reload=SysTick->LOAD;    //LOAD的值               
	ticks=nus*(HALFSYSTEM_CLOCK/1000000);             //需要的节拍数       fac_us=时钟频率/1000000
	tcnt=0;    
	told=SysTick->VAL;            //刚进入时的计数器值    
	while(1){        
		tnow=SysTick->VAL;            
		if(tnow!=told){                    
			if(tnow<told)
				tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.            
			else 
				tcnt+=reload-tnow+told;                    
			told=tnow;            

			if(tcnt>=ticks)
				break;//时间超过/等于要延迟的时间,则退出.        
			}      
	};                                         
}

