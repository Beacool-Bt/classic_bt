/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
/** 
  *@file keypad.h
  *@brief keypad support for application
  */
#ifndef DRIVERS_KEYSCAN_YC_KEYPAD_H_
#define DRIVERS_KEYSCAN_YC_KEYPAD_H_
#include <stdint.h>
#include "yc11xx.h"
#include "yc_timer.h"


//#define KEY_DEFINE_TOUCH_PAD

#define CHARGER_DETECT_GPIO (GPIO_28|GPIO_ACTIVE_BIT)
#ifdef KEY_DEFINE_TOUCH_PAD
#define POWER_GPIO (GPIO_27)
#else
#define POWER_GPIO (GPIO_27|GPIO_ACTIVE_BIT)
#endif

#define KEYPAD_COMBO_KEY_MAX 2

/**
  *@brief Key index.
  */
typedef enum
{
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_10,
	KEY_11,
	KEY_12,
	KEY_13,
	KEY_14,
	KEY_15,
	KEY_MAX_NUM, /*!< don't change this value */
	COMB_KEY1,
	COMB_KEY2,
	COMB_KEY3,
	COMB_KEY4,
}KEY_INDEX;

/**
  *@brief Key index mask.
  */
typedef enum
{
	KEY_0_MASK = 0x0001,
	KEY_1_MASK = 0x0002,
	KEY_2_MASK = 0x0004,
	KEY_3_MASK = 0x0008,
	KEY_4_MASK = 0x0010,
	KEY_5_MASK = 0x0020,
	KEY_6_MASK = 0x0040,
	KEY_7_MASK = 0x0080,
	KEY_8_MASK = 0x0100,
	KEY_9_MASK = 0x0200,
	KEY_10_MASK = 0x0400,
	KEY_11_MASK = 0x0800,
	KEY_12_MASK = 0x1000,
	KEY_13_MASK = 0x2000,
	KEY_14_MASK = 0x4000,
	KEY_15_MASK = 0x8000,
	KEY_INVALID_MASK = 0x0000,
}KEY_INDEX_MASK;

/**
  *@brief Event type.
  */
typedef enum
{
	KEYPAD_NON,
	KEYPAD_TAP,		
	KEYPAD_DOUBLE,	
	KEYPAD_TRIPLE,	
	KEYPAD_DOWN,	
	KEYPAD_UP,		
	KEYPAD_PRESS,	
	KEYPAD_PRESS_RELEASE,
	KEYPAD_LONG_PRESS,
	KEYPAD_LONG_RELEASE,
	KEYPAD_LONGLONG_PRESS,
	KEYPAD_LONGLONG_RELEASE,
}KEYPAD_EVT_TYPE;

/**
  *@brief Press type.
  */
typedef enum
{
	KEYPAD_PRESS_NONE,
	KEYPAD_PRESS_MULTI,
	KEYPAD_PRESS_PRESS,
	KEYPAD_PRESS_LONG_PRESS,
	KEYPAD_PRESS_LONGLONG_PRESS,
	KEYPAD_PRESS_EXTREME_LONG_PESS,
}KEYPAD_PRESS_TYPE;

/**
  *@brief Key timer.
  */
typedef struct
{
	uint32_t multi_timer;
	uint32_t press_timer;  /*!< unit 10ms */
	uint32_t long_press_timer;  /*!< unit 10ms */
	uint32_t longlong_press_timer;  /*!<unit 10ms */
}KEYPAD_KEYTIMER;

/**
  *@brief Key configure. 
  */
typedef struct
{
	GPIO_NUM gpio_ind;
	KEYPAD_PRESS_TYPE press_type;
	SYS_TIMER_TYPE key_timer;
	KEYPAD_KEYTIMER key_timer_cfg;
}KEY_CONFIG;

/**
  *@brief Combkey configure.
  */
typedef struct
{
	uint16_t comkKeyMask;
	KEYPAD_PRESS_TYPE press_type;
	SYS_TIMER_TYPE key_timer;
	KEYPAD_KEYTIMER key_timer_cfg;
}COMBKEY_CONFIG;

#define KEYPAD_FLAG_SUPPORT_MULTI_KEY 0x0001
#define KEYPAD_FLAG_SUPPORT_COMBO_KEY 0x0002

/**
  *@brief Keypad configure.
  */
typedef struct
{
	uint32_t polling_timer; /*!< unit 10ms*/
	COMBKEY_CONFIG keypad_combokey_array[KEYPAD_COMBO_KEY_MAX];
	//KEY_CONFIG keypad_gpio_array[KEY_MAX_NUM];
	KEY_CONFIG keypad_gpio_array[8];
	uint8_t comboKeyNum; /*!< the number combo key configured*/
	uint8_t keyNum; /*!< the number key configured*/
	uint16_t kepadFlag;
}KEYPAD_CONFIG;

/**
  *@brief Keypad event callback function.
  */
typedef void (* KeyPad_Evt_CB_Handle)(KEY_INDEX,KEYPAD_EVT_TYPE);

/**
  *@brief Key map.
  */
typedef union
{
	uint16_t keymap_u16;
	uint8_t  keymap_u8[2];
}KEY_KEYMAP;

/**
  *@brief Driver key map.
  */
typedef union
{
	uint16_t keymap_u16;
	uint8_t  keymap_u8[2];
} DRIVER_KEY_KEYMAP;

/**
  *@brief Keypad control.
  */
typedef struct
{
	uint16_t gpio_key_mask;
	uint16_t gpio_hid_key_mask;
	uint8_t Key_Change_happen;
	uint32_t keypad_history_value;
	uint32_t keypapd_current_value;
	KeyPad_Evt_CB_Handle keypad_evt_callback;

	DRIVER_KEY_KEYMAP gpio_mapping_result; 			/*!< all input result */
	DRIVER_KEY_KEYMAP key_mapping_result; 			/*!< only key input result */
	DRIVER_KEY_KEYMAP key_mapping_result_old;		/*!< last key input */
	DRIVER_KEY_KEYMAP key_hid_mapping_result;		/*!< only key hid input result */
	DRIVER_KEY_KEYMAP key_hid_mapping_result_old;	/*!< last key input */
	uint8_t key_multi_count;
}KEYPAD_CONTROL;

/**
  *@brief Key event.
  */
typedef struct
{
	KEY_INDEX key_ind;
	KEYPAD_EVT_TYPE evt_type;
}KEY_EVT_ELE;

/**
  *@brief Keypad initialization.
  *@param cb Callback function.
  *@return None.
  */
void keypad_init(KeyPad_Evt_CB_Handle cb);

/**
  *@brief Scan key pad.
  *@param None.
  *@return None.
  */
void keypad_check_keypress();

/**
  *@brief Key expired.
  *@param keyIndex key number.
  *@return None.
  */
void keypad_keyExpired(int keyIndex);

/**
  *@brief scan key pad.
  *@param index key index value.@ref KEY_INDEX
  *@param key_evt key event.@ref KEYPAD_EVT_TYPE
  *@return None.
  */
void key_send_evt_to_Queue(KEY_INDEX index,KEYPAD_EVT_TYPE key_evt);

/**
  *@brief Polling key queue.
  *@param None.
  *@return None.
  */
void keypad_polling_keyQueue();


void keypad_delayLpm();

#endif
