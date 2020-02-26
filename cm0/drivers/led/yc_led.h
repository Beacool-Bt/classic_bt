#ifndef DRIVERS_LED_YC_LED_H_
#define DRIVERS_LED_YC_LED_H_
#include "yc_keypad.h"

typedef enum
{
	LED_INVALID_TYPE,
	LED_BLUE_RED_SWITCH,
	LED_BLUE_RED_SYNC,
	LED_BLUE_ONLY,
	LED_RED_ONLY,
}LED_STYLE;

typedef enum
{
	LED_REP_MODE_SINGLE_SHOT,
	LED_REP_MODE_REPEAT,
}LED_REP_MODE;
typedef struct
{
	LED_STYLE style;
	uint8_t onTimer;
	uint8_t offTimer;
	uint8_t blinkCount;

	//not used for fg led
	uint8_t delayTimer;
	//repeat mode, 0: no rep, 1: repeat
	LED_REP_MODE repMode;
}LED_EVT_PARAMS;

typedef struct
{
	GPIO_NUM red_gpio;
	GPIO_NUM blue_gpio;
}LED_CFG;

typedef enum
{
	LED_OFF,
	LED_ON,
}LED_SWITCH;

typedef enum
{
	LED_BG,
	LED_FG,
}LED_FG_BG;

typedef struct
{
	LED_EVT_PARAMS led_param;
	LED_SWITCH sled_switch;
	LED_FG_BG fgbg;
	uint8_t bCount;
}LED_STATE;

extern LED_STATE gCurLedState;
extern LED_STATE gBackLedState;

extern SYS_TIMER_TYPE gCONLedTimer;
void Bt_CBConnectedLed(int params);

typedef enum
{
	LED_EVT_POWER_ON=0,
	LED_EVT_POWER_OFF,
	LED_EVT_CONNECTED,
	LED_EVT_DISCONNECTED,
	LED_EVT_LOW_POWER,
	LED_EVT_CHARGE_FULL,
	LED_EVT_CHARGING,
	LED_EVT_TWS_PARING,
	LED_EVT_TWS_PARED,
	LED_EVT_TWS_SLAVE_CONNECTED,
	LED_EVT_PAGING,
	LED_EVT_DISCOVERABLE,
	LED_EVT_CONNECTED_IDLE,
	LED_EVT_NUM,
}LED_EVT;
typedef struct{
	LED_EVT_PARAMS powerOn;
	LED_EVT_PARAMS powerOff;;
	LED_EVT_PARAMS connected;
	LED_EVT_PARAMS disconnected;
	LED_EVT_PARAMS lowerPower;
	LED_EVT_PARAMS chargeFull;
	LED_EVT_PARAMS charging;
	LED_EVT_PARAMS twsPairing;
	LED_EVT_PARAMS twsPaired;
	LED_EVT_PARAMS noConnectPage;
	LED_EVT_PARAMS noConnectDiscovery;
	LED_EVT_PARAMS connectedIdle;
	//LED_EVT_PARAMS connectedMusic;
	//LED_EVT_PARAMS connectedHfpIncoming;
	//LED_EVT_PARAMS connectedHfpOutgoing;
	//LED_EVT_PARAMS connectedHfpCalling;
}HEADSET_LED_FUNCTION_MAP;

typedef struct{
	HEADSET_LED_FUNCTION_MAP singleHeadset;
	HEADSET_LED_FUNCTION_MAP doubleLeftHeadset;
	HEADSET_LED_FUNCTION_MAP doubleRightHeadset;
}TWS_LED_FUNCTION_MAP;

void led_init();
void led_set_inactive(GPIO_NUM gpio);
void led_set_active(GPIO_NUM gpio);
void led_light_led();
void led_timer_callback(int params);
void led_send_bgevt(LED_EVT event);
void led_clear_led();
 void led_send_fgevt_to_queue(LED_EVT event);
 void led_polling_Queue();
 void led_set_pwm(GPIO_NUM gpio,uint8_t channel);

 void Bt_ProcessLedFunc(LED_EVT ledEvt);

#endif //DRIVERS_LED_YC_LED_H_
