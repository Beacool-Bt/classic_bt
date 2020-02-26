/**
 * COPYRIGHT NOTICE
 *Copyright(c) 2014,YICHIP
 *
 * @file yc_RemoteCtr.c
 * @brief ...
 *
 * @version 1.0
 * @author  jingzou
 * @data    Jan 24, 2018
**/

#include <stdio.h>
#include "yc_RemoteCtr.h"
#include "yc_keypad.h"
#include "Drv_bt.h"
#include "ipc.h"
#include "yc11xx.h"
#include "yc_led.h"
#include "yc_voiceprompt.h"
#include "yc_battery.h"
#include "drv_debug.h"

extern uint8_t *gpHFPCallState;



#ifdef TWS_USER_INTERFACE_HSC_I9S
static const TWS_KEY_FUNCTION_MAP gTwsKeyFuncArray[] = {
	{
		KEY_0,
		{//singleHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_HFP_REDIAL/*connectedIdle*/,KEY_FUNC_HFP_REDIAL/*connectedMusic*/
			, KEY_FUNC_HFP_REDIAL/*connectedHfpIncoming*/, KEY_FUNC_HFP_REDIAL/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_REDIAL/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_NONE/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_POWER_ON_OFF/*connectedHfpOutgoing*/
			,KEY_FUNC_POWER_ON_OFF/*connectedHfpCalling*/},
		},
		{//doubleLeftHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_VOL_DOWN/*connectedIdle*/,KEY_FUNC_MUSIC_VOL_DOWN/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_MUSIC_BACKWARD/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
		},
		{//doubleRightHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_VOL_UP/*connectedIdle*/,KEY_FUNC_MUSIC_VOL_UP/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_MUSIC_FORWARD/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
		},
	},
};

#elif TWS_USER_INTERFACE_JIELI
static const TWS_KEY_FUNCTION_MAP gTwsKeyFuncArray[] = {
	{
		KEY_0,
		{//singleHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_HFP_REDIAL/*connectedIdle*/,KEY_FUNC_HFP_REDIAL/*connectedMusic*/
			, KEY_FUNC_HFP_REDIAL/*connectedHfpIncoming*/, KEY_FUNC_HFP_REDIAL/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_REDIAL/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_NONE/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_POWER_ON_OFF/*connectedHfpOutgoing*/
			,KEY_FUNC_POWER_ON_OFF/*connectedHfpCalling*/},
		},
		{//doubleLeftHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_VOL_DOWN/*connectedIdle*/,KEY_FUNC_MUSIC_VOL_DOWN/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_MUSIC_BACKWARD/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
		},
		{//doubleRightHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_VOL_UP/*connectedIdle*/,KEY_FUNC_MUSIC_VOL_UP/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_MUSIC_FORWARD/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
		},
	},
};
#else
static const TWS_KEY_FUNCTION_MAP gTwsKeyFuncArray[] = {
	{
		KEY_0,
		{//singleHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_HFP_REDIAL/*connectedIdle*/,KEY_FUNC_HFP_REDIAL/*connectedMusic*/
			, KEY_FUNC_HFP_REDIAL/*connectedHfpIncoming*/, KEY_FUNC_HFP_REDIAL/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_REDIAL/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_NONE/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_POWER_ON_OFF/*connectedHfpOutgoing*/
			,KEY_FUNC_POWER_ON_OFF/*connectedHfpCalling*/},
		},
		{//doubleLeftHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_HFP_REDIAL/*connectedIdle*/,KEY_FUNC_HFP_REDIAL/*connectedMusic*/
			, KEY_FUNC_HFP_REDIAL/*connectedHfpIncoming*/, KEY_FUNC_HFP_REDIAL/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_REDIAL/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_NONE/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_POWER_ON_OFF/*connectedHfpOutgoing*/
			,KEY_FUNC_POWER_ON_OFF/*connectedHfpCalling*/},
		},
		{//doubleRightHeadset
			//singleTap
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedIdle*/,KEY_FUNC_MUSIC_PLAY_PAUSE/*connectedMusic*/
			, KEY_FUNC_HFP_ACCEPT_CALL/*connectedHfpIncoming*/, KEY_FUNC_HFP_HANGUP/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_HANGUP/*connectedHfpCalling*/},
			//doublePress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_TWS_PAIR/*noConnectDiscovery*/
			, KEY_FUNC_HFP_REDIAL/*connectedIdle*/,KEY_FUNC_HFP_REDIAL/*connectedMusic*/
			, KEY_FUNC_HFP_REDIAL/*connectedHfpIncoming*/, KEY_FUNC_HFP_REDIAL/*connectedHfpOutgoing*/
			,KEY_FUNC_HFP_REDIAL/*connectedHfpCalling*/},
			//tripPress
			{KEY_FUNC_NONE/*noConnectPage*/, KEY_FUNC_NONE/*noConnectDiscovery*/
			, KEY_FUNC_NONE/*connectedIdle*/,KEY_FUNC_NONE/*connectedMusic*/
			, KEY_FUNC_NONE/*connectedHfpIncoming*/, KEY_FUNC_NONE/*connectedHfpOutgoing*/
			,KEY_FUNC_NONE/*connectedHfpCalling*/},
			//longPress
			{KEY_FUNC_POWER_ON_OFF/*noConnectPage*/, KEY_FUNC_POWER_ON_OFF/*noConnectDiscovery*/
			, KEY_FUNC_POWER_ON_OFF/*connectedIdle*/,KEY_FUNC_POWER_ON_OFF/*connectedMusic*/
			, KEY_FUNC_HFP_HANGUP/*connectedHfpIncoming*/, KEY_FUNC_POWER_ON_OFF/*connectedHfpOutgoing*/
			,KEY_FUNC_POWER_ON_OFF/*connectedHfpCalling*/},
		},
	},
};
#endif


void keypad_keyPress_handle(KEY_INDEX index)
{
	/*
	switch(index)
	{
		case KEY_0:	
			tws_user2_action();
			break;
		default:
			break;
	}*/
}


void keypad_keyDown_handle(KEY_INDEX index)
{
	switch (index)
	{
		case KEY_0:
			//Lpm_LockLpm(KEY_LPM_FLAG);
			break;
		case KEY_1:
			//Bat_EnterChargerIn();
			break;
		default:
			break;
	}
}


void keypad_keyUp_handle(KEY_INDEX index)
{
	switch (index)
	{
		case KEY_0:
			//Lpm_unLockLpm(KEY_LPM_FLAG);
			break;
		case KEY_1:
			//Bat_ExitChargerIn();
			break;
		default:
			break;
	}
}

void tws_user3_action()
{
	if (Bt_checkBRIsConnected())
	{
		if (HREAD(REG_B_TWS_ROLE) != TWS_SET_SLAVE){
			Bt_SndCmdRedial();
		}
		else{
			Bt_SndCmdTwsUser3();
		}
	}		
}

void tws_user4_action()
{
	if (HREAD(REG_B_TWS_ROLE) != TWS_SET_SLAVE)
	{	
		Bt_SndCmdHangup();
	}
	else
	{
		Bt_SndCmdTwsUser4();
	}
}

void tws_user2_action()
{
	// hung up call
	 if (*gpHFPCallState >= HFP_OUTGOING)
	{
		if (HREAD(REG_B_TWS_ROLE) != TWS_SET_SLAVE){
			Bt_SndCmdHangup();
		}
		else{
			Bt_SndCmdTwsUser2();
		}
	}
}
void tws_user1_action()
{
	if (!Bt_checkBRIsConnected())
		return;
		
	if (HREAD(REG_B_TWS_ROLE) != TWS_SET_SLAVE)
	{
			
		if (*gpHFPCallState == HFP_NONO)
		{
			Bt_SndCmdPlayPause();
		}
		else if (*gpHFPCallState == HFP_INCOMMING)
		{
			Bt_SndCmdAcceptCall();
		}
		else
		{
			Bt_SndCmdHangup();
		}
	}
	else{
		Bt_SndCmdTwsUser1();
	}
}

void keypad_keytap_handle(KEY_INDEX index)
{
	tws_key_func_handle(index, TWS_KEY_EVT_TYPE_TAP, false);
}

void keypad_key_double_handle(KEY_INDEX index)
{
	tws_key_func_handle(index, TWS_KEY_EVT_TYPE_DOUBLE, false);
}

void keypad_key_triple_handle(KEY_INDEX index)
{
	tws_key_func_handle(index, TWS_KEY_EVT_TYPE_TRIPLE, false);
}


void keypad_key_longPress_release_handle(KEY_INDEX index)
{
	if (index == KEY_0)
	{
		if (gBRState.topState == BR_POWER_ON)
		{
			//Bt_CheckReconnectAction();
			if((HREAD(mem_tws_record_state)&0xf0) == 0x50){
				Bt_SndCmdStartReconTws();
			}
			else{
				Bt_CheckReconnectAction();
			}
		}
	}
}

void keypad_key_longPress_handle(KEY_INDEX index)
{
	tws_key_func_handle(index, TWS_KEY_EVT_TYPE_LONG, false);
}


void tws_key_func_handle(KEY_INDEX index, TWS_KEY_EVT_TYPE keyEvt, bool isExpireByRemote)
{
	//Other func KEY_SUPPORT_FUNC
	KEY_SUPPORT_FUNC keyFunc = Bt_SearchKeyFunc(index, keyEvt
		, isExpireByRemote);
	bool isNeedSendActionToRemote = false;
	
	DEBUG_LOG_2(LOG_LEVEL_CORE, "TWS" ,"tws_key_func_handle: 0x%04X", LOG_POINT_C001, index, keyEvt);
	DEBUG_LOG_2(LOG_LEVEL_CORE, "TWS" ,"tws_key_func_handle: 0x%04X", LOG_POINT_C002, gBRState.twsState, isExpireByRemote);
	
	
	//Judge is tws slave or not
	if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
	{
		if(keyFunc == KEY_FUNC_POWER_ON_OFF)
		{
			Bt_ProcessKeyFunc(keyFunc);
			return;
		}
		else if(keyFunc == KEY_FUNC_MUSIC_VOL_UP
			||keyFunc == KEY_FUNC_MUSIC_VOL_DOWN)
		{
			Bt_ProcessKeyFunc(keyFunc);
		}
		if(!isExpireByRemote)
		{
			isNeedSendActionToRemote = true;
		}
	}
	else
	{
		Bt_ProcessKeyFunc(keyFunc);

		if(!isExpireByRemote
			&& (gBRState.twsState == BR_TWS_MASTER_CONNECTED))
		{
			if(keyFunc == KEY_FUNC_MUSIC_VOL_UP
				||keyFunc == KEY_FUNC_MUSIC_VOL_DOWN)
			{
				isNeedSendActionToRemote = true;
			}
		}
	}

	if(isNeedSendActionToRemote)
	{
		switch(keyEvt)
		{
			case TWS_KEY_EVT_TYPE_TAP:
				Bt_SndCmdTwsUser1();
				break;
			case TWS_KEY_EVT_TYPE_DOUBLE:
				Bt_SndCmdTwsUser2();
				break;
			case TWS_KEY_EVT_TYPE_TRIPLE:
				Bt_SndCmdTwsUser3();
				break;
			case TWS_KEY_EVT_TYPE_LONG:
				Bt_SndCmdTwsUser4();
				break;
		}

	}
}


KEY_SUPPORT_FUNC Bt_SearchKeyFuncByKeyEvt(HEADSET_KEY_FUNCTION_MAP funcSubMap, TWS_KEY_EVT_TYPE keyEvt)
{
	KEY_FUNCTION_REMAP keyRemap;
	KEY_SUPPORT_FUNC keyFunc = KEY_FUNC_NONE;
	switch(keyEvt)
	{
		case TWS_KEY_EVT_TYPE_TAP:
			keyRemap = funcSubMap.singleTap;
			break;
		case TWS_KEY_EVT_TYPE_DOUBLE:
			keyRemap = funcSubMap.doublePress;
			break;
		case TWS_KEY_EVT_TYPE_TRIPLE:
			keyRemap = funcSubMap.tripPress;
			break;
		case TWS_KEY_EVT_TYPE_LONG:
			keyRemap = funcSubMap.longPress;
			break;
	}
	if(!Bt_checkBRIsConnected())
	{
 		if(!Bt_checkBRIsInDiscoverable())//KEY_SUPPORT_FUNC noConnectPage;
 		{
			keyFunc = keyRemap.noConnectPage;
 		}
  		else // KEY_SUPPORT_FUNC noConnectDiscovery;
 		{
			keyFunc = keyRemap.noConnectDiscovery;
 		}
	}
	else
	{
		if(*gpHFPCallState != HFP_NONO)
		{
			if(*gpHFPCallState == HFP_INCOMMING)
			{
				keyFunc = keyRemap.connectedHfpIncoming;
			}
			else if(*gpHFPCallState == HFP_OUTGOING)
			{
				keyFunc = keyRemap.connectedHfpOutgoing;
			}
			else// if(*gpHFPCallState == HFP_CALLACTIVE)
			{
				keyFunc = keyRemap.connectedHfpCalling;
			}
		}
		else
		{
			//
			if(Bt_A2dpCheckStart())
			{
				keyFunc = keyRemap.connectedMusic;
			}
			else
			{
				keyFunc = keyRemap.connectedIdle;
			}
		}
		
	}

	return keyFunc;
}
KEY_SUPPORT_FUNC Bt_SearchKeyFunc(KEY_INDEX index, TWS_KEY_EVT_TYPE keyEvt, bool isExpireByRemote)
{
	KEY_SUPPORT_FUNC func = KEY_FUNC_NONE;
	TWS_KEY_FUNCTION_MAP funcMap;
	uint8_t arraySize = sizeof(gTwsKeyFuncArray);
	uint8_t i;
	for(i = 0; i < arraySize; i ++)
	{
		if(gTwsKeyFuncArray[i].index == index)
		{
			break;
		}
	}
	// check find or not
	if(i >= arraySize)
	{
		return func;
	}
	funcMap = gTwsKeyFuncArray[i];
	DEBUG_LOG_2(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchKeyFunc: 0x%04X", LOG_POINT_C003, index, keyEvt);
	DEBUG_LOG_2(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchKeyFunc: 0x%04X", LOG_POINT_C004, gBRState.twsState, isExpireByRemote);
	
	//Judge is tws slave
	if (HSP_R == HREAD(mem_hsp_role))
	{
		if(isExpireByRemote)
		{
			func = Bt_SearchKeyFuncByKeyEvt(funcMap.doubleLeftHeadset, keyEvt);
		}
		else
		{
			func = Bt_SearchKeyFuncByKeyEvt(funcMap.doubleRightHeadset, keyEvt);
		}
	}
	else if (HSP_L == HREAD(mem_hsp_role))
	{
		if(isExpireByRemote)
		{
			func = Bt_SearchKeyFuncByKeyEvt(funcMap.doubleRightHeadset, keyEvt);
		}
		else
		{
			func = Bt_SearchKeyFuncByKeyEvt(funcMap.doubleLeftHeadset, keyEvt);
		}
		
	}
	else
	{
		func = Bt_SearchKeyFuncByKeyEvt(funcMap.singleHeadset, keyEvt);
	}

	return func;
}

//void keypad_key_longlongPress_handle(KEY_INDEX index)
//{
	//if (index == KEY_0)
	//{
		//Bt_SndCmdStartDiscovery();
	//}
//}

void KeyPad_Event_Handle(KEY_INDEX index,KEYPAD_EVT_TYPE key_evt)
{
	//YC_LOG_INFO("key index %d, key evt %d\r\n",index,key_evt);
	DEBUG_LOG_2(LOG_LEVEL_CORE, "IPC" ,"KeyPad_Event_Handle index: 0x%02X, key_evt: 0x%02X", LOG_POINT_A300, index, key_evt);
	switch (key_evt)
	{
		case KEYPAD_DOWN:	//0
			//printf("key index %d, key down\r\n",index);
			keypad_keyDown_handle(index);
			Bt_ResetAutoPwroffTimer();
			break;
		case KEYPAD_UP:	//1
			//printf("key index %d, key up\r\n",index);
			keypad_keyUp_handle(index);
			Bt_ResetAutoPwroffTimer();
			break;
		case KEYPAD_TAP:		//2
			//printf("key index %d, key tap\r\n",index);
			keypad_keytap_handle(index);
			break;
		case KEYPAD_DOUBLE:	//3
			//printf("key index %d, key double\r\n",index);
			keypad_key_double_handle(index);
			break;
		case KEYPAD_TRIPLE:	//4
			//printf("key index %d, key triple\r\n",index);
			keypad_key_triple_handle(index);
			break;
		case KEYPAD_PRESS:	//5
			//printf("key index %d, key press\r\n",index);
			keypad_keyPress_handle(index);
			break;
		case KEYPAD_PRESS_RELEASE:
			//printf("key index %d, key press release \r\n",index);
			break;
		case KEYPAD_LONG_PRESS:	//6
			//printf("key index %d, key long press\r\n",index);
			keypad_key_longPress_handle(index);
			break;
		case KEYPAD_LONG_RELEASE:	//7
			//printf("key index %d, key long release\r\n",index);
			//keypad_key_longPress_release_handle(index);
			break;
		case KEYPAD_LONGLONG_PRESS:	//8
			//printf("key index %d, key long long press\r\n",index);
			//keypad_key_longlongPress_handle(index);
			break;
		case KEYPAD_LONGLONG_RELEASE://9
			//printf("key index %d, key longlong release\r\n",index);
			break;
	}
}


