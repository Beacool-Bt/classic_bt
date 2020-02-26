#include "yc_led.h"
#include "yc11xx_gpio.h"
#include "yc11xx.h"
#include "yc_drv_common.h"
#include "yc_queue.h"
#include <string.h>
#include "Drv_bt.h"
#include "yc11xx_pwm.h"
#include "drv_debug.h"

LED_CFG gLedCfg =
{
	GPIO_14|GPIO_ACTIVE_BIT,//red led gpio
	GPIO_19|GPIO_ACTIVE_BIT,//blue led gpio
	//GPIO_12|GPIO_ACTIVE_BIT,//red led gpio
	//GPIO_13|GPIO_ACTIVE_BIT,//blue led gpio
};


#ifdef TWS_USER_INTERFACE_HSC_I9S
const TWS_LED_FUNCTION_MAP gTwsLedFunc = {
	{//singleHeadset
		//powerOn
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//powerOff
		{LED_RED_ONLY/*style*/, 20/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//connected
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//lowerPower
		{LED_RED_ONLY/*style*/, 4/*onTime*/, 4/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//chargeFull
		{LED_BLUE_ONLY/*style*/, 200/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//charging
		{LED_RED_ONLY/*style*/, 0/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPairing
		{LED_BLUE_RED_SWITCH/*style*/, 1/*onTime*/, 1/*offTime*/, 2/*blinkCount*/, 1/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPaired
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//noConnectPage
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//noConnectDiscovery
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
	},
	{//doubleLeftHeadset
		//powerOn
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//powerOff
		{LED_RED_ONLY/*style*/, 20/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//connected
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//lowerPower
		{LED_RED_ONLY/*style*/, 4/*onTime*/, 4/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//chargeFull
		{LED_BLUE_ONLY/*style*/, 200/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//charging
		{LED_RED_ONLY/*style*/, 0/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPairing
		{LED_BLUE_RED_SWITCH/*style*/, 1/*onTime*/, 1/*offTime*/, 2/*blinkCount*/, 1/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPaired
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//noConnectPage
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//noConnectDiscovery
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
	},
	{//doubleRightHeadset
		//powerOn
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//powerOff
		{LED_RED_ONLY/*style*/, 20/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//connected
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//lowerPower
		{LED_RED_ONLY/*style*/, 4/*onTime*/, 4/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//chargeFull
		{LED_BLUE_ONLY/*style*/, 200/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//charging
		{LED_RED_ONLY/*style*/, 0/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPairing
		{LED_BLUE_RED_SWITCH/*style*/, 1/*onTime*/, 1/*offTime*/, 2/*blinkCount*/, 1/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPaired
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//noConnectPage
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//noConnectDiscovery
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
	},
};
#else
const TWS_LED_FUNCTION_MAP gTwsLedFunc = {
	{//singleHeadset
		//powerOn
		{LED_BLUE_ONLY/*style*/, 6/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//powerOff
		{LED_RED_ONLY/*style*/, 2/*onTime*/, 3/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//connected
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//lowerPower
		{LED_RED_ONLY/*style*/, 4/*onTime*/, 4/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//chargeFull
		{LED_BLUE_ONLY/*style*/, 200/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//charging
		{LED_RED_ONLY/*style*/, 0/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPairing
		{LED_BLUE_RED_SWITCH/*style*/, 1/*onTime*/, 1/*offTime*/, 2/*blinkCount*/, 1/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPaired
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//noConnectPage
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 5/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//noConnectDiscovery
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//connectedIdle
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 5/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
	},
	{//doubleLeftHeadset
		//powerOn
		{LED_BLUE_ONLY/*style*/, 6/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//powerOff
		{LED_RED_ONLY/*style*/, 2/*onTime*/, 3/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//connected
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//lowerPower
		{LED_RED_ONLY/*style*/, 4/*onTime*/, 4/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//chargeFull
		{LED_BLUE_ONLY/*style*/, 200/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//charging
		{LED_RED_ONLY/*style*/, 0/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPairing
		{LED_BLUE_RED_SWITCH/*style*/, 1/*onTime*/, 1/*offTime*/, 2/*blinkCount*/, 1/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPaired
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//noConnectPage
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 5/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//noConnectDiscovery
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//connectedIdle
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 5/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
	},
	{//doubleRightHeadset
		//powerOn
		{LED_BLUE_ONLY/*style*/, 6/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//powerOff
		{LED_RED_ONLY/*style*/, 2/*onTime*/, 3/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//connected
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//disconnected
		{LED_INVALID_TYPE/*style*/, 0/*onTime*/, 0/*offTime*/, 0/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//lowerPower
		{LED_RED_ONLY/*style*/, 4/*onTime*/, 4/*offTime*/, 3/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//chargeFull
		{LED_BLUE_ONLY/*style*/, 200/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//charging
		{LED_RED_ONLY/*style*/, 0/*onTime*/, 0/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPairing
		{LED_BLUE_RED_SWITCH/*style*/, 1/*onTime*/, 1/*offTime*/, 2/*blinkCount*/, 1/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//twsPaired
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 0/*delayTime*/, LED_REP_MODE_SINGLE_SHOT/*repMode*/},
		//noConnectPage
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 5/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//noConnectDiscovery
		{LED_BLUE_RED_SWITCH/*style*/, 3/*onTime*/, 3/*offTime*/, 1/*blinkCount*/, 3/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
		//connectedIdle
		{LED_BLUE_ONLY/*style*/, 2/*onTime*/, 2/*offTime*/, 1/*blinkCount*/, 5/*delayTime*/, LED_REP_MODE_REPEAT/*repMode*/},
	},
};
#endif

LED_STATE gCurLedState;
LED_STATE gBackLedState;

#define LED_EVT_QUEUE_MAX 10
LED_EVT gQueueLedEvt[LED_EVT_QUEUE_MAX];
QUEUE_HEADER gLedEvtHeader;
SYS_TIMER_TYPE gLed_Timer;

void led_init()
{
	//led_set_inactive(gLedCfg.red_gpio);
	//led_set_inactive(gLedCfg.blue_gpio);
	GPIO_SetOutput(gLedCfg.red_gpio);
	GPIO_SetOutput(gLedCfg.blue_gpio);

	memset(&gCurLedState,0,sizeof(LED_STATE));
	memset(&gBackLedState,0,sizeof(LED_STATE));
	
	if (!queue_init(&gLedEvtHeader, (void *)&gQueueLedEvt, 1, LED_EVT_QUEUE_MAX))
	{
		error_handle();
	}
}

void led_clear_led()
{
	queue_clear(&gLedEvtHeader);
	led_init();
}

LED_EVT_PARAMS Bt_SearchLedFuncByLedEvt(HEADSET_LED_FUNCTION_MAP funcSubMap, LED_EVT ledEvt)
{
	LED_EVT_PARAMS ledFunc = {LED_INVALID_TYPE,0,0,0,0,0};
	switch(ledEvt)
	{
		case LED_EVT_POWER_ON:
			ledFunc = funcSubMap.powerOn;
			break;
		case LED_EVT_POWER_OFF:
			ledFunc = funcSubMap.powerOff;
			break;
		case LED_EVT_CONNECTED:
			ledFunc = funcSubMap.connected;
			break;
		case LED_EVT_DISCONNECTED:
			ledFunc = funcSubMap.disconnected;
			break;
		case LED_EVT_LOW_POWER:
			ledFunc = funcSubMap.lowerPower;
			break;
		case LED_EVT_CHARGE_FULL:
			ledFunc = funcSubMap.chargeFull;
			break;
		case LED_EVT_CHARGING:
			ledFunc = funcSubMap.charging;
			break;
		case LED_EVT_TWS_PARING:
			ledFunc = funcSubMap.twsPairing;
			break;
		case LED_EVT_PAGING:
			ledFunc = funcSubMap.noConnectPage;
			break;
		case LED_EVT_DISCOVERABLE:
			ledFunc = funcSubMap.noConnectDiscovery;
			break;
		case LED_EVT_CONNECTED_IDLE:
			ledFunc = funcSubMap.connectedIdle;
			break;
		default:
			break;
	}
	
	return ledFunc;
}
LED_EVT_PARAMS Bt_SearchLedFunc(LED_EVT ledEvt)
{
	LED_EVT_PARAMS func = {LED_INVALID_TYPE,0,0,0,0,0};
	
	DEBUG_LOG_2(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchLedFunc: 0x%04X", LOG_POINT_C101, gBRState.twsState, ledEvt);
	
	//Judge is tws slave
	if (HSP_R == HREAD(mem_hsp_role))
	{
		func = Bt_SearchLedFuncByLedEvt(gTwsLedFunc.doubleRightHeadset, ledEvt);
	}
	else if (HSP_L == HREAD(mem_hsp_role))
	{
		func = Bt_SearchLedFuncByLedEvt(gTwsLedFunc.doubleLeftHeadset, ledEvt);
	}
	else
	{
		func = Bt_SearchLedFuncByLedEvt(gTwsLedFunc.singleHeadset, ledEvt);
	}

	return func;
}


void Bt_ProcessLedFunc(LED_EVT ledEvt)
{
	LED_EVT_PARAMS func;
	func = Bt_SearchLedFunc(ledEvt);

	DEBUG_LOG_2(LOG_LEVEL_CORE, "TWS" ,"Bt_ProcessLedFunc: 0x%04X", LOG_POINT_C100, ledEvt, func.style);

	if(ledEvt == LED_EVT_POWER_OFF
		||ledEvt == LED_EVT_CONNECTED
		||ledEvt == LED_EVT_TWS_PARED
		||ledEvt == LED_EVT_TWS_SLAVE_CONNECTED)
	{
		led_clear_led();
	}
	
	if (func.style == LED_INVALID_TYPE)
	{
		return;
	}

	if(ledEvt == LED_EVT_CONNECTED_IDLE)
	{
		SYS_SetTimer(&gCONLedTimer,
			50, //5s
			TIMER_CYCLE|TIMER_TYPE_BIT,
			Bt_CBConnectedLed);
		return;
	}
	
	if(func.repMode == LED_REP_MODE_SINGLE_SHOT)
	{
		led_send_fgevt_to_queue(ledEvt);
	}
	else
	{
		led_send_bgevt(ledEvt);
	}
}

void led_send_fgevt_to_queue(LED_EVT event)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"led_send_fgevt_to_queue: 0x%04X", LOG_POINT_A101, event);
	if (!Insert_Queue(&gLedEvtHeader,(void *)&event))
	{
		//put queue is error
		//TO DO
		error_handle();
	}
}

void led_send_bgevt(LED_EVT event)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"led_send_bgevt: 0x%04X", LOG_POINT_A100, event);
	LED_EVT_PARAMS func = Bt_SearchLedFunc(event);
	if (gCurLedState.fgbg == LED_FG){
		gBackLedState.fgbg = LED_BG;
		gBackLedState.sled_switch = LED_OFF;
		gBackLedState.bCount = func.blinkCount;
		memcpy(&gBackLedState.led_param,&func,sizeof(LED_EVT_PARAMS));
	}
	else{
		gCurLedState.fgbg = LED_BG;
		led_set_inactive(gLedCfg.red_gpio);
		led_set_inactive(gLedCfg.blue_gpio);
				 
		if (SYS_TimerisExist(&gLed_Timer))
		{
			SYS_ReleaseTimer(&gLed_Timer);
		}
		
		memcpy(&gCurLedState.led_param,&func,sizeof(LED_EVT_PARAMS));
		if (gCurLedState.led_param.style != LED_INVALID_TYPE)
		{
			Lpm_LockLpm(LED_LPM_FLAG);
			gCurLedState.bCount = func.blinkCount;
			gCurLedState.sled_switch = LED_OFF ;
			led_timer_callback(0);
		}
		else
		{
			Lpm_unLockLpm(LED_LPM_FLAG);
			gCurLedState.sled_switch = LED_OFF ;
			SYS_ReleaseTimer(&gLed_Timer);
			led_light_led();
		}
	}
	
}


void led_light_ledpwm(GPIO_NUM gpio)
{
//	if (gBRState.topState == BR_CHARGER_IN)
//	{
	if(gpio == gLedCfg.red_gpio)
		led_set_pwm(gpio,PWM_CH0);
	else
		if(gpio == gLedCfg.blue_gpio)
			led_set_pwm(gpio,PWM_CH1);
/*	}else
	{
	if(gpio == gLedCfg.red_gpio)
		led_set_active(gpio);
	else
		if(gpio == gLedCfg.blue_gpio)
			led_set_active(gpio);
	}	
*/
}

void led_light_led()
{
	if (gCurLedState.sled_switch == LED_OFF)
	{
		switch (gCurLedState.led_param.style)
		{
			case LED_BLUE_RED_SWITCH:
				led_set_inactive(gLedCfg.blue_gpio);
				led_light_ledpwm(gLedCfg.red_gpio);
				break;
			case LED_BLUE_ONLY:
				led_set_inactive(gLedCfg.blue_gpio);
				break;
			case LED_RED_ONLY:
				led_set_inactive(gLedCfg.red_gpio);
				break;
			case LED_BLUE_RED_SYNC:
			default:
				 led_set_inactive(gLedCfg.red_gpio);
				 led_set_inactive(gLedCfg.blue_gpio);
				 break;
		}
	}
	else
	{
		switch (gCurLedState.led_param.style)
		{
			case LED_BLUE_RED_SWITCH:
				led_light_ledpwm(gLedCfg.blue_gpio);
				led_set_inactive(gLedCfg.red_gpio);
				break;
			case LED_BLUE_ONLY:
				led_light_ledpwm(gLedCfg.blue_gpio);
				break;
			case LED_RED_ONLY:
				led_light_ledpwm(gLedCfg.red_gpio);
				break;
			case LED_BLUE_RED_SYNC:
				led_light_ledpwm(gLedCfg.red_gpio);
				led_light_ledpwm(gLedCfg.blue_gpio);
				 break;
			default:
				break;
		}
	}
}

void led_timer_callback(int params)
{
	if (gCurLedState.sled_switch == LED_ON)
	{
		gCurLedState.sled_switch = LED_OFF;
		led_light_led();
		if (gCurLedState.led_param.offTimer)
			SYS_SetTimer(&gLed_Timer,gCurLedState.led_param.offTimer,TIMER_SINGLE|TIMER_TYPE_BIT,led_timer_callback);
	}
	else
	{
		if (gCurLedState.bCount == 0)
		{
			if (gCurLedState.fgbg == LED_FG){
			
				Lpm_unLockLpm(LED_LPM_FLAG);
				memcpy(&gCurLedState,&gBackLedState,sizeof(LED_STATE)) ;

				if (gCurLedState.led_param.style == LED_INVALID_TYPE)
					return;

				led_light_led();
				Lpm_LockLpm(LED_LPM_FLAG);
				if (gCurLedState.sled_switch == LED_ON){
					if (gCurLedState.led_param.onTimer)
						SYS_SetTimer(&gLed_Timer,gCurLedState.led_param.onTimer,TIMER_SINGLE|TIMER_TYPE_BIT,led_timer_callback);
				}
				else{
					if (gCurLedState.led_param.offTimer)
						SYS_SetTimer(&gLed_Timer,gCurLedState.led_param.offTimer,TIMER_SINGLE|TIMER_TYPE_BIT,led_timer_callback);
				}
				return;
			}
			else{
				gCurLedState.bCount = gCurLedState.led_param.blinkCount;
				//SYS_SetTimer(&gLed_Timer,gCurLedState.led_param.delayTimer,TIMER_SINGLE,led_timer_callback);
				if (gCurLedState.led_param.delayTimer)
					SYS_SetTimer(&gLed_Timer,gCurLedState.led_param.delayTimer,TIMER_SINGLE|TIMER_TYPE_BIT,led_timer_callback);
				led_set_inactive(gLedCfg.red_gpio);
				led_set_inactive(gLedCfg.blue_gpio);
				return;
			}
		}
		gCurLedState.bCount--;
		gCurLedState.sled_switch = LED_ON;
		if (gCurLedState.led_param.onTimer)
			SYS_SetTimer(&gLed_Timer,gCurLedState.led_param.onTimer,TIMER_SINGLE|TIMER_TYPE_BIT,led_timer_callback);
		led_light_led();
	}
}


void led_polling_Queue()
{
	LED_EVT *evt_ele;
	
	if (gCurLedState.fgbg == LED_FG)
		return;

	if (!queue_is_empty(&gLedEvtHeader))
	{
		evt_ele = (LED_EVT *)Delete_Queue(&gLedEvtHeader);
		
		LED_EVT_PARAMS func = Bt_SearchLedFunc(*evt_ele);
		if (func.style == LED_INVALID_TYPE)
			return;
		
		memcpy(&gBackLedState,&gCurLedState,sizeof(LED_STATE)) ;
		memcpy(&gCurLedState.led_param,&func,sizeof(LED_EVT_PARAMS));
		gCurLedState.bCount = gCurLedState.led_param.blinkCount;

		//start blink
		gCurLedState.sled_switch = LED_OFF;
		gCurLedState.fgbg = LED_FG;
		led_timer_callback(0);
		Lpm_LockLpm(LED_LPM_FLAG);
		
	}
}


void led_set_inactive(GPIO_NUM gpio)
{
	uint16_t addr;
	if (gpio == GPIO_MAX_NUM)
		return;

	GPIO_SetOut(gpio,false);
	Lpm_unLockLpm(PWM_LPM_FLAG);
	//if (gpio & 0x80){
	//	addr = CORE_GPIO_CONF+(gpio & 0x3f);	
	//	HWRITE(addr,62);
	//}
	//else {
	//	gpio = CORE_GPIO_CONF+(gpio & 0x3f);	
	//	HWRITE(addr,63);
	//}
}

void led_set_active(GPIO_NUM gpio)
{
	uint16_t addr;
	if (gpio == GPIO_MAX_NUM)
		return;

	GPIO_SetOut(gpio,true);
	//if (gpio & 0x80){
	//	addr = CORE_GPIO_CONF+(gpio & 0x3f);	
	//	HWRITE(addr,63);
	//}
	//else {
	//	addr = CORE_GPIO_CONF+(gpio & 0x3f);	
	//	HWRITE(addr,62);
	//}
}

void led_set_pwm(GPIO_NUM gpio,uint8_t channel)
{
	struct PWM_CTRL_BITS led_pwm;
	Lpm_LockLpm(PWM_LPM_FLAG);
	led_pwm.ENABLE = PWM_DISABLE;
	led_pwm.HIGHF =PWM_HIGHF_DISABLE;
	led_pwm.SYNC =PWM_SYNC_DEFAULT;
	led_pwm.VAL = 7;
	PWM_Init(channel,&led_pwm);
	PWM_SetPnCnt(channel,0,0x6);
	PWM_Start(channel);
	if((HREAD(CORE_CLKOFF+1) & 0x20) )
		HWCOR(CORE_CLKOFF+1,0X20);		
	 PWM_SetGpio( channel,gpio);

}


