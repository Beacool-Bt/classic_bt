/**
 * COPYRIGHT NOTICE
 *Copyright(c) 2014,YICHIP
 *
 * @file yc_RemoteCtr.h
 * @brief ...
 *
 * @version 1.0
 * @author  jingzou
 * @data    Jan 24, 2018
**/

#ifndef DRIVERS_KEYSCAN_YC_REMOTECTR_H_
#define DRIVERS_KEYSCAN_YC_REMOTECTR_H_

#include "yc_keypad.h"
#include "ipc.h"

//#define TWS_USER_INTERFACE_JIELI

typedef enum
{
	TWS_KEY_EVT_TYPE_TAP,
	TWS_KEY_EVT_TYPE_DOUBLE,
	TWS_KEY_EVT_TYPE_TRIPLE,
	TWS_KEY_EVT_TYPE_LONG,
}TWS_KEY_EVT_TYPE;

typedef enum
{
	KEY_FUNC_NONE,
	KEY_FUNC_MUSIC_PLAY_PAUSE,
	KEY_FUNC_MUSIC_FORWARD,
	KEY_FUNC_MUSIC_BACKWARD,
	KEY_FUNC_MUSIC_VOL_UP,
	KEY_FUNC_MUSIC_VOL_DOWN,
	KEY_FUNC_HFP_ACCEPT_CALL,
	KEY_FUNC_HFP_HANGUP,
	KEY_FUNC_HFP_REDIAL,
	KEY_FUNC_POWER_ON_OFF,
	KEY_FUNC_TWS_PAIR,
}KEY_SUPPORT_FUNC;

typedef struct{
	KEY_SUPPORT_FUNC noConnectPage;//
	KEY_SUPPORT_FUNC noConnectDiscovery;
	KEY_SUPPORT_FUNC connectedIdle;
	KEY_SUPPORT_FUNC connectedMusic;
	KEY_SUPPORT_FUNC connectedHfpIncoming;
	KEY_SUPPORT_FUNC connectedHfpOutgoing;
	KEY_SUPPORT_FUNC connectedHfpCalling;
}KEY_FUNCTION_REMAP;

typedef struct{
	KEY_FUNCTION_REMAP singleTap;
	KEY_FUNCTION_REMAP doublePress;
	KEY_FUNCTION_REMAP tripPress;
	KEY_FUNCTION_REMAP longPress;
}HEADSET_KEY_FUNCTION_MAP;

typedef struct{
	KEY_INDEX index;
	HEADSET_KEY_FUNCTION_MAP singleHeadset;
	HEADSET_KEY_FUNCTION_MAP doubleLeftHeadset;
	HEADSET_KEY_FUNCTION_MAP doubleRightHeadset;
}TWS_KEY_FUNCTION_MAP;

void keypad_keyPress_handle(KEY_INDEX index);

void keypad_keyDown_handle(KEY_INDEX index);

void KeyPad_Event_Handle(KEY_INDEX index,KEYPAD_EVT_TYPE key_evt);


void keypad_send_stop_record_command();

void keypad_send_start_record_command();

void keypad_mic_auto_testcase();

void keypad_send_update_param_command();

void keypad_sendLENty(IPC_DATA_FORMAT *packet,uint16_t handle,uint8_t len);

void keypad_keytap_handle(KEY_INDEX index);

void tws_key_func_handle(KEY_INDEX index, TWS_KEY_EVT_TYPE keyEvt, bool isExpireByRemote);
KEY_SUPPORT_FUNC Bt_SearchKeyFunc(KEY_INDEX index, TWS_KEY_EVT_TYPE keyEvt, bool isExpireByRemote);

void tws_user1_action();
void tws_user2_action();
void tws_user3_action();
void tws_user4_action();

extern uint8_t Bt_checkBRIsInDiscoverable(void);
extern bool Bt_A2dpCheckStart(void);

#endif /* DRIVERS_KEYSCAN_YC_REMOTECTR_H_ */
