/**
 * COPYRIGHT NOTICE
 *Copyright(c) 2014,YICHIP
 *
 * @file yc_keypad.c
 * @brief ...
 *
 * @version 1.0
 * @author  jingzou
 * @data    Jan 23, 2018
**/
#include<string.h>
#include<stdio.h>
#include "yc_keypad.h"
#include "yc11xx.h"
#include "yc_timer.h"
#include "yc_queue.h"
#include "yc11xx_gpio.h"
#include "yc_drv_common.h"

#define KEY_EVT_QUEUE_MAX 10
KEY_EVT_ELE gQueueKeyEvt[KEY_EVT_QUEUE_MAX];
QUEUE_HEADER gEvtHeader;
KEYPAD_CONTROL gKeypadCtr;
KEYPAD_CONFIG  gKeypadConfig=
{
	10, //polling_timer
	{ //keypad_combokey_array
			{ //COMBKEY_CONFIG
					KEY_0_MASK|KEY_1_MASK,
					KEYPAD_PRESS_NONE,
					{0,0,TIMER_STOP,0,0,0,},
					{50,30,100,100},
			},
			{
					KEY_0_MASK|KEY_1_MASK,
					KEYPAD_PRESS_NONE,
					{0,0,TIMER_STOP,0,0,0,},
					{50,30,100,100},
			},
			/**
			{
					KEY_0_MASK|KEY_1_MASK,
					KEYPAD_PRESS_NONE,
					{0,0,TIMER_STOP,0,0,0,},
					{50,30,100,100},
			},
			{
					KEY_0_MASK|KEY_1_MASK,
					KEYPAD_PRESS_NONE,
					{0,0,TIMER_STOP,0,0,0,},
					{50,30,100,100},
			},
			**/

	},
	{//keypad_gpio_array
		{//KEY_CONFIG
				//GPIO_27|GPIO_ACTIVE_BIT, //0
				POWER_GPIO, //0
				KEYPAD_PRESS_NONE,
				{0,0,TIMER_STOP,0,0,0,},
				{50,15,60,150},
		},
		{
				//GPIO_28 |GPIO_ACTIVE_BIT,
				CHARGER_DETECT_GPIO,
				KEYPAD_PRESS_NONE,
				{0,0,TIMER_STOP,0,0,0,},
				{50,100,100,100},
		},
		{
				GPIO_MAX_NUM,
				KEYPAD_PRESS_NONE,
				{0,0,TIMER_STOP,0,0,0,},
				{50,100,100},
		},
		{
				GPIO_MAX_NUM, //3
				KEYPAD_PRESS_NONE,
				{0},
				{50,100,100},
		},
		{
				GPIO_MAX_NUM, //4
				KEYPAD_PRESS_NONE,
				{0},
				{50,100,100},
		},
		{
				GPIO_MAX_NUM, //5
				KEYPAD_PRESS_NONE,
				{0},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //6
				KEYPAD_PRESS_NONE,
				{0},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //7
				KEYPAD_PRESS_NONE,
				{0},
				{10,100,100},
		},
		/**
		{
				GPIO_MAX_NUM, //8
				KEYPAD_PRESS_NONE,
				{0},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //9
				KEYPAD_PRESS_NONE,
				{0},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //10
				KEYPAD_PRESS_NONE,
				{0,},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //11
				KEYPAD_PRESS_NONE,
				{0,},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //12
				KEYPAD_PRESS_NONE,
				{0,},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //13
				KEYPAD_PRESS_NONE,
				{0,},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //14
				KEYPAD_PRESS_NONE,
				{0,},
				{10,100,100},
		},
		{
				GPIO_MAX_NUM, //15
				KEYPAD_PRESS_NONE,
				{0,},
				{10,100,100},
		},
		**/
	},
	0, //comboKeyNum
	2, //keyNum
};

typedef union
{
	uint32_t value32;
	uint16_t value16[2];
	uint8_t  value8[4];
}UNION_32BIT;

SYS_TIMER_TYPE gKeypadPolling_Timer;
SYS_TIMER_TYPE gDelayLpm_timer;


void keypad_init(KeyPad_Evt_CB_Handle cb)
{
	uint8_t i;
	UNION_32BIT wakeup_mask;
	//clear keypad control information
	memset(&gKeypadCtr, 0, sizeof(gKeypadCtr));

	//set call back event handle
	gKeypadCtr.keypad_evt_callback = cb;

	for (i = 0; i < gKeypadConfig.keyNum; i++)
	{
		if (gKeypadConfig.keypad_gpio_array[i].gpio_ind != GPIO_MAX_NUM)
		{
			//if (gKeypadConfig.keypad_gpio_array[i].gpio_ind & GPIO_ACTIVE_BIT){
			//	HWRITE((CORE_GPIO_CONF+(gKeypadConfig.keypad_gpio_array[i].gpio_ind & 0x1f)), GPCFG_PULLDOWN|GPCFG_INPUT);
			//}
			//else{
			//	HWRITE((CORE_GPIO_CONF+(gKeypadConfig.keypad_gpio_array[i].gpio_ind)), GPCFG_PULLUP|GPCFG_INPUT);
			//}
			GPIO_SetInput(gKeypadConfig.keypad_gpio_array[i].gpio_ind);
			GPIO_SetWakeup(gKeypadConfig.keypad_gpio_array[i].gpio_ind);
			
			gKeypadCtr.gpio_key_mask |= (1 << i);
			gKeypadCtr.gpio_hid_key_mask |= (1 << i);
			wakeup_mask.value32 |= (1 << (gKeypadConfig.keypad_gpio_array[i].gpio_ind & 0x3f));
		}
	}

//	//enable low wakeup function
//	HWRITEW(CORE_GPIO_WAKEUP_LOW, wakeup_mask.value16[0]);
//	HWRITEW(CORE_GPIO_WAKEUP_LOW+2, wakeup_mask.value16[1]);

	SYS_SetTimer(&gKeypadPolling_Timer,4,TIMER_CYCLE,keypad_check_keypress);


	if (!queue_init(&gEvtHeader, (void *)&gQueueKeyEvt, sizeof(KEY_EVT_ELE), KEY_EVT_QUEUE_MAX))
	{
		//printf("Queue init fail\n");
	}
	
    //scan gpio first
	keypad_check_keypress(0);
}

void key_send_evt_to_Queue(KEY_INDEX index,KEYPAD_EVT_TYPE key_evt)
{
	KEY_EVT_ELE key_ele_temp;
	key_ele_temp.key_ind = index;
	key_ele_temp.evt_type = key_evt;
	//YC_LOG_INFO("KEYPAD send ind=%d ,type=%d\r\n",index,key_evt);
	if (!Insert_Queue(&gEvtHeader,(void *)&key_ele_temp))
	{
		//printf("put to queue fail\n");
		//put queue is error
		//TO DO
	}
}

void keypad_gpio_mapping_process(KEY_KEYMAP keyBitmap_L , KEY_KEYMAP keyBitmap_H )
{
	uint8_t i;
	KEY_KEYMAP keyBitmap_Local;

	keyBitmap_Local.keymap_u16 = 0;

	gKeypadCtr.key_mapping_result_old.keymap_u16 =	gKeypadCtr.key_mapping_result.keymap_u16;//save old for next time to compare
	gKeypadCtr.key_hid_mapping_result_old.keymap_u16 = gKeypadCtr.key_hid_mapping_result.keymap_u16;//save old for next time to compare

	//for( i = 0 ; i < KEY_MAX_NUM ; i++  )
	for(i = 0; i < gKeypadConfig.keyNum; i++)
	{
		//if (gKeypadConfig.keypad_gpio_array[i].gpio_ind & 0x80 != 0x80)
		//		if (gKeypadConfig.keypad_gpio_array[i].gpio_ind & 0x80 != 0x80)	
		if (GPIO_ACTIVE_BIT != (gKeypadConfig.keypad_gpio_array[i].gpio_ind & GPIO_ACTIVE_BIT))	
		{
			switch( gKeypadConfig.keypad_gpio_array[i].gpio_ind & 0x3f)
			{
				case GPIO_0:
					if( keyBitmap_L.keymap_u8[0] & 0x01 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_1:
					if( keyBitmap_L.keymap_u8[0] & 0x02 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_2:
					if( keyBitmap_L.keymap_u8[0] & 0x04 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_3:
					if( keyBitmap_L.keymap_u8[0] & 0x08 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_4:
					if( keyBitmap_L.keymap_u8[0] & 0x10 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_5:
					if( keyBitmap_L.keymap_u8[0] & 0x20 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_6:
					if( keyBitmap_L.keymap_u8[0] & 0x40 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_7:
					if( keyBitmap_L.keymap_u8[0] & 0x80 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;

				case GPIO_8:
					if( keyBitmap_L.keymap_u8[1] & 0x01 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_9:
					if( keyBitmap_L.keymap_u8[1] & 0x02 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_10:
					if( keyBitmap_L.keymap_u8[1] & 0x04 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_11:
					if( keyBitmap_L.keymap_u8[1] & 0x08 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_12:
					if( keyBitmap_L.keymap_u8[1] & 0x10 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_13:
					if( keyBitmap_L.keymap_u8[1] & 0x20 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_14:
					if( keyBitmap_L.keymap_u8[1] & 0x40 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_15:
					if( keyBitmap_L.keymap_u8[1] & 0x80 )//
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;

				case GPIO_16:
					if( keyBitmap_H.keymap_u8[0] & 0x01 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_17:
					if( keyBitmap_H.keymap_u8[0] & 0x02 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_18:
					if( keyBitmap_H.keymap_u8[0] & 0x04 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_19:
					if( keyBitmap_H.keymap_u8[0] & 0x08 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_20:
					if( keyBitmap_H.keymap_u8[0] & 0x10 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_21:
					if( keyBitmap_H.keymap_u8[0] & 0x20 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_22:
					if( keyBitmap_H.keymap_u8[0] & 0x40 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_23:
					if( keyBitmap_H.keymap_u8[0] & 0x80 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;

				case GPIO_24:
					if( keyBitmap_H.keymap_u8[1] & 0x01 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_25:
					if( keyBitmap_H.keymap_u8[1] & 0x02 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_26:
					if( keyBitmap_H.keymap_u8[1] & 0x04 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_27:
					if( keyBitmap_H.keymap_u8[1] & 0x08 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_28:
					if( keyBitmap_H.keymap_u8[1] & 0x10 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_29:
					if( keyBitmap_H.keymap_u8[1] & 0x20 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_30:
					if( keyBitmap_H.keymap_u8[1] & 0x40 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
				case GPIO_31:
					if( keyBitmap_H.keymap_u8[1] & 0x80 )
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					else
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					break;
			}
		}
		else{
			switch( gKeypadConfig.keypad_gpio_array[i].gpio_ind & 0x3f)
			{
				case GPIO_0:
					if( keyBitmap_L.keymap_u8[0] & 0x01 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_1:
					if( keyBitmap_L.keymap_u8[0] & 0x02 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_2:
					if( keyBitmap_L.keymap_u8[0] & 0x04 )//gpio0 = 1
					{
						
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_3:
					if( keyBitmap_L.keymap_u8[0] & 0x08 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_4:
					if( keyBitmap_L.keymap_u8[0] & 0x10 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_5:
					if( keyBitmap_L.keymap_u8[0] & 0x20 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						

						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_6:
					if( keyBitmap_L.keymap_u8[0] & 0x40 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						

						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_7:
					if( keyBitmap_L.keymap_u8[0] & 0x80 )//gpio0 = 1
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;

				case GPIO_8:
					if( keyBitmap_L.keymap_u8[1] & 0x01 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_9:
					if( keyBitmap_L.keymap_u8[1] & 0x02 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);	
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_10:
					if( keyBitmap_L.keymap_u8[1] & 0x04 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_11:
					if( keyBitmap_L.keymap_u8[1] & 0x08 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_12:
					if( keyBitmap_L.keymap_u8[1] & 0x10 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);	
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_13:
					if( keyBitmap_L.keymap_u8[1] & 0x20 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_14:
					if( keyBitmap_L.keymap_u8[1] & 0x40 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_15:
					if( keyBitmap_L.keymap_u8[1] & 0x80 )//
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;

				case GPIO_16:
					if( keyBitmap_H.keymap_u8[0] & 0x01 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_17:
					if( keyBitmap_H.keymap_u8[0] & 0x02 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_18:
					if( keyBitmap_H.keymap_u8[0] & 0x04 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{	
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_19:
					if( keyBitmap_H.keymap_u8[0] & 0x08 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
						
					}
					break;
				case GPIO_20:
					if( keyBitmap_H.keymap_u8[0] & 0x10 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_21:
					if( keyBitmap_H.keymap_u8[0] & 0x20 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_22:
					if( keyBitmap_H.keymap_u8[0] & 0x40 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_23:
					if( keyBitmap_H.keymap_u8[0] & 0x80 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;

				case GPIO_24:
					if( keyBitmap_H.keymap_u8[1] & 0x01 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_25:
					if( keyBitmap_H.keymap_u8[1] & 0x02 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_26:
					if( keyBitmap_H.keymap_u8[1] & 0x04 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_27:
					if( keyBitmap_H.keymap_u8[1] & 0x08 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_28:
					if( keyBitmap_H.keymap_u8[1] & 0x10 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_29:
					if( keyBitmap_H.keymap_u8[1] & 0x20 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);	
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_30:
					if( keyBitmap_H.keymap_u8[1] & 0x40 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
				case GPIO_31:
					if( keyBitmap_H.keymap_u8[1] & 0x80 )
					{
						keyBitmap_Local.keymap_u16 |= (1<<i);
					}
					else
					{
						keyBitmap_Local.keymap_u16 &= (~(1<<i));
					}
					break;
			}
		}

	}

	//YC_LOG_INFO("local bitmap %x ,%x in\r\n",keyBitmap_Local.keymap_u16,gKeypadCtr.gpio_key_mask);
	gKeypadCtr.gpio_mapping_result.keymap_u16 = keyBitmap_Local.keymap_u16;
	gKeypadCtr.key_mapping_result.keymap_u16 = (gKeypadCtr.gpio_mapping_result.keymap_u16 & gKeypadCtr.gpio_key_mask );
	gKeypadCtr.key_hid_mapping_result.keymap_u16 = (gKeypadCtr.gpio_mapping_result.keymap_u16 & gKeypadCtr.gpio_hid_key_mask );
	if(gKeypadCtr.key_hid_mapping_result_old.keymap_u16 ^ gKeypadCtr.key_hid_mapping_result.keymap_u16 )//key change happen
	{
		gKeypadCtr.Key_Change_happen = 1;
	}
}

void keypad_keyExpired(int keyIndex)
{
	switch(gKeypadConfig.keypad_gpio_array[keyIndex].press_type)
	{
	case KEYPAD_PRESS_MULTI:
		if (gKeypadCtr.key_mapping_result.keymap_u16 & (1 << keyIndex))
		{
			if (gKeypadCtr.key_multi_count == 0)
			{
				gKeypadConfig.keypad_gpio_array[keyIndex].press_type = KEYPAD_PRESS_PRESS;
				gKeypadConfig.keypad_gpio_array[keyIndex].key_timer.cbParams = keyIndex;
				SYS_SetTimer(&gKeypadConfig.keypad_gpio_array[keyIndex].key_timer,
						gKeypadConfig.keypad_gpio_array[keyIndex].key_timer_cfg.press_timer,
						TIMER_SINGLE,
						keypad_keyExpired);
			}
			else
			{
				if (gKeypadCtr.key_multi_count > 3)
					key_send_evt_to_Queue(keyIndex,KEYPAD_TRIPLE);
				else
					key_send_evt_to_Queue(keyIndex,gKeypadCtr.key_multi_count);
				gKeypadCtr.key_multi_count = 0;
			}
		}
		else
		{
			if (gKeypadCtr.key_multi_count > 3)
				key_send_evt_to_Queue(keyIndex,KEYPAD_TRIPLE);
			else
				key_send_evt_to_Queue(keyIndex,gKeypadCtr.key_multi_count);
			keypad_delayLpm(KEY_LPM_FLAG);
			gKeypadCtr.key_multi_count = 0;
			gKeypadConfig.keypad_gpio_array[keyIndex].press_type = KEYPAD_PRESS_NONE;
		}
		//YC_LOG_INFO("multi timer %d\r\n",keyIndex);
		break;

	case KEYPAD_PRESS_PRESS:
		gKeypadConfig.keypad_gpio_array[keyIndex].press_type = KEYPAD_PRESS_LONG_PRESS;
		gKeypadConfig.keypad_gpio_array[keyIndex].key_timer.cbParams = keyIndex;
		SYS_SetTimer(&gKeypadConfig.keypad_gpio_array[keyIndex].key_timer,
				gKeypadConfig.keypad_gpio_array[keyIndex].key_timer_cfg.long_press_timer,
				TIMER_SINGLE,
				keypad_keyExpired);
		key_send_evt_to_Queue(keyIndex,KEYPAD_PRESS);
		//YC_LOG_INFO("press timer %d\r\n",keyIndex);
		break;

	case KEYPAD_PRESS_LONG_PRESS:
		gKeypadConfig.keypad_gpio_array[keyIndex].press_type = KEYPAD_PRESS_LONGLONG_PRESS;
		gKeypadConfig.keypad_gpio_array[keyIndex].key_timer.cbParams = keyIndex;
		SYS_SetTimer(&gKeypadConfig.keypad_gpio_array[keyIndex].key_timer,
				gKeypadConfig.keypad_gpio_array[keyIndex].key_timer_cfg.longlong_press_timer,
				TIMER_SINGLE,
				keypad_keyExpired);
		key_send_evt_to_Queue(keyIndex,KEYPAD_LONG_PRESS);
		//YC_LOG_INFO("long press timer %d\r\n",keyIndex);
		break;

	case KEYPAD_PRESS_LONGLONG_PRESS:
		gKeypadConfig.keypad_gpio_array[keyIndex].press_type = KEYPAD_PRESS_EXTREME_LONG_PESS;
		gKeypadConfig.keypad_gpio_array[keyIndex].key_timer.cbParams = keyIndex;
		SYS_SetTimer(&gKeypadConfig.keypad_gpio_array[keyIndex].key_timer,
				gKeypadConfig.keypad_gpio_array[keyIndex].key_timer_cfg.longlong_press_timer,
				TIMER_CYCLE,
				SYS_TimerExpireDefaultHandle);
		key_send_evt_to_Queue(keyIndex,KEYPAD_LONGLONG_PRESS);
		//YC_LOG_INFO("long long timer %d\r\n",keyIndex);
		break;
	case KEYPAD_PRESS_EXTREME_LONG_PESS:
		break;
	}
}

void keypad_comboKeyExpired(int keyIndex)
{
	switch(gKeypadConfig.keypad_combokey_array[keyIndex].press_type)
	{
		case KEYPAD_PRESS_PRESS:
			gKeypadConfig.keypad_combokey_array[keyIndex].press_type = KEYPAD_PRESS_LONG_PRESS;
			gKeypadConfig.keypad_combokey_array[keyIndex].key_timer.cbParams = keyIndex;
			SYS_SetTimer(&gKeypadConfig.keypad_combokey_array[keyIndex].key_timer,
					gKeypadConfig.keypad_combokey_array[keyIndex].key_timer_cfg.long_press_timer,
					TIMER_SINGLE,
					keypad_comboKeyExpired);
			key_send_evt_to_Queue(keyIndex+COMB_KEY1,KEYPAD_PRESS);
			//YC_LOG_INFO("press timer %d\r\n",keyIndex);
			break;

		case KEYPAD_PRESS_LONG_PRESS:
			gKeypadConfig.keypad_combokey_array[keyIndex].press_type = KEYPAD_PRESS_LONGLONG_PRESS;
			gKeypadConfig.keypad_combokey_array[keyIndex].key_timer.cbParams = keyIndex;
			SYS_SetTimer(&gKeypadConfig.keypad_combokey_array[keyIndex].key_timer,
					gKeypadConfig.keypad_combokey_array[keyIndex].key_timer_cfg.longlong_press_timer,
					TIMER_SINGLE,
					keypad_comboKeyExpired);
			key_send_evt_to_Queue(keyIndex+COMB_KEY1,KEYPAD_LONG_PRESS);
			//YC_LOG_INFO("long press timer %d\r\n",keyIndex);
			break;

		case KEYPAD_PRESS_LONGLONG_PRESS:
			gKeypadConfig.keypad_combokey_array[keyIndex].press_type = KEYPAD_PRESS_EXTREME_LONG_PESS;
			gKeypadConfig.keypad_combokey_array[keyIndex].key_timer.cbParams = keyIndex;
			SYS_SetTimer(&gKeypadConfig.keypad_combokey_array[keyIndex].key_timer,
					gKeypadConfig.keypad_combokey_array[keyIndex].key_timer_cfg.longlong_press_timer,
					TIMER_CYCLE,
					SYS_TimerExpireDefaultHandle);
			key_send_evt_to_Queue(keyIndex+COMB_KEY1,KEYPAD_LONGLONG_PRESS);
			//YC_LOG_INFO("long long timer %d\r\n",keyIndex);
			break;
		case KEYPAD_PRESS_EXTREME_LONG_PESS:
			break;
	}
}

void keypad_keyevtScan()
{
	uint8_t i;
	if (gKeypadCtr.Key_Change_happen == 1)
	{
		gKeypadCtr.Key_Change_happen =0;
//		printf("key is change = d %d !\r\n",gKeypadCtr.key_mapping_result.keymap_u16);
	}

	//check combo key
	for(i = 0; i < gKeypadConfig.comboKeyNum; i++)
	{
		if ((gKeypadCtr.key_mapping_result.keymap_u16
				& gKeypadConfig.keypad_combokey_array[i].comkKeyMask) == gKeypadConfig.keypad_combokey_array[i].comkKeyMask)
		{
			// key press
			if ((gKeypadCtr.key_mapping_result_old.keymap_u16
							& gKeypadConfig.keypad_combokey_array[i].comkKeyMask) != gKeypadConfig.keypad_combokey_array[i].comkKeyMask)
			{
				if (gKeypadConfig.keypad_combokey_array[i].press_type == KEYPAD_PRESS_NONE)
				{
					gKeypadConfig.keypad_combokey_array[i].press_type = KEYPAD_PRESS_PRESS;
					gKeypadConfig.keypad_combokey_array[i].key_timer.cbParams = i;
					SYS_SetTimer(&gKeypadConfig.keypad_combokey_array[i].key_timer,
							gKeypadConfig.keypad_combokey_array[i].key_timer_cfg.press_timer,
							TIMER_SINGLE,
							keypad_comboKeyExpired);
//					printf("combo key down\n",i);
				}
			}
		}
		else
		{
			//release key
			if ((gKeypadCtr.key_mapping_result_old.keymap_u16
							& gKeypadConfig.keypad_combokey_array[i].comkKeyMask) == gKeypadConfig.keypad_combokey_array[i].comkKeyMask)
			{
				SYS_ReleaseTimer(&gKeypadConfig.keypad_combokey_array[i].key_timer);
				switch(gKeypadConfig.keypad_combokey_array[i].press_type)
				{
					case KEYPAD_PRESS_PRESS:
						key_send_evt_to_Queue(i+COMB_KEY1,KEYPAD_TAP);
						break;
					case KEYPAD_PRESS_LONG_PRESS:
						key_send_evt_to_Queue(i+COMB_KEY1,KEYPAD_PRESS_RELEASE);
						break;
					case KEYPAD_PRESS_LONGLONG_PRESS:
						key_send_evt_to_Queue(i+COMB_KEY1,KEYPAD_LONG_RELEASE);
						break;
					case KEYPAD_PRESS_EXTREME_LONG_PESS:
						key_send_evt_to_Queue(i+COMB_KEY1,KEYPAD_LONGLONG_RELEASE);
						break;
					case KEYPAD_PRESS_NONE:
						break;
				}
				gKeypadConfig.keypad_combokey_array[i].press_type = KEYPAD_PRESS_NONE;
			}
		}
	}//end for

	//check single key
	for(i = 0; i < gKeypadConfig.keyNum; i++)
	{
		if (gKeypadCtr.key_mapping_result.keymap_u16 & (1 << i))
		{
			if (!(gKeypadCtr.key_mapping_result_old.keymap_u16 & (1 << i)))
			{
				if (SYS_TimerisExist(&gDelayLpm_timer))
				{
					SYS_ReleaseTimer(&gDelayLpm_timer);
				}
				Lpm_LockLpm(KEY_LPM_FLAG);
				key_send_evt_to_Queue(i,KEYPAD_DOWN);
				if (gKeypadConfig.keypad_gpio_array[i].press_type == KEYPAD_PRESS_NONE)
				{
					gKeypadConfig.keypad_gpio_array[i].press_type = KEYPAD_PRESS_MULTI;
					gKeypadConfig.keypad_gpio_array[i].key_timer.cbParams = i;

					SYS_SetTimer(&gKeypadConfig.keypad_gpio_array[i].key_timer,
							gKeypadConfig.keypad_gpio_array[i].key_timer_cfg.multi_timer,
							TIMER_SINGLE,
							keypad_keyExpired);
				}
			}
		}
		else
		{
			if ((gKeypadCtr.key_mapping_result_old.keymap_u16 & (1 << i)))
			{
				//send release event;
//				printf("key %d is up\r\n",i);
				key_send_evt_to_Queue(i,KEYPAD_UP);

				if (gKeypadConfig.keypad_gpio_array[i].press_type == KEYPAD_PRESS_MULTI)
				{
					if (gKeypadConfig.keypad_gpio_array[i].key_timer.mTimerStatus == TIMER_START)
					{
						gKeypadCtr.key_multi_count++;
					}
					else
					{
						keypad_delayLpm(KEY_LPM_FLAG);
						gKeypadConfig.keypad_gpio_array[i].press_type = KEYPAD_PRESS_NONE;
					}
				}
				else
				{
					SYS_ReleaseTimer(&gKeypadConfig.keypad_gpio_array[i].key_timer);
					switch(gKeypadConfig.keypad_gpio_array[i].press_type)
					{
						case KEYPAD_PRESS_PRESS:
							key_send_evt_to_Queue(i,KEYPAD_TAP);
							break;
						case KEYPAD_PRESS_LONG_PRESS:
							key_send_evt_to_Queue(i,KEYPAD_PRESS_RELEASE);
							break;
						case KEYPAD_PRESS_LONGLONG_PRESS:
							key_send_evt_to_Queue(i,KEYPAD_LONG_RELEASE);
							break;
						case KEYPAD_PRESS_EXTREME_LONG_PESS:
							key_send_evt_to_Queue(i,KEYPAD_LONGLONG_RELEASE);
							break;
						case KEYPAD_PRESS_NONE:
							break;
					}
					keypad_delayLpm(KEY_LPM_FLAG);
					gKeypadConfig.keypad_gpio_array[i].press_type = KEYPAD_PRESS_NONE;
				}//end else
			}
		}//end else
	}//end for
}

//scan key pad
void keypad_check_keypress(int params)
{
	KEY_KEYMAP key_bitmap1,key_bitmap2;
	KEY_KEYMAP tempkey_bitmap1,tempkey_bitmap2;
	int while1;

	key_bitmap1.keymap_u16 = HREADW(CORE_GPIO_IN);
	key_bitmap2.keymap_u16 = HREADW(CORE_GPIO_IN+2);
	while1 = 100;
	while(while1--);
	tempkey_bitmap1.keymap_u16 = HREADW(CORE_GPIO_IN);
	tempkey_bitmap2.keymap_u16 = HREADW(CORE_GPIO_IN+2);
	
	if (tempkey_bitmap1.keymap_u16 != key_bitmap1.keymap_u16)
		return;


	//YC_LOG_ERROR("output %x,%x \n",key_bitmap1.keymap_u16,key_bitmap2.keymap_u16);
	keypad_gpio_mapping_process(key_bitmap1,key_bitmap2);
	keypad_keyevtScan();
}

void keypad_polling_keyQueue()
{
	KEY_EVT_ELE *evt_ele;
	if (!queue_is_empty(&gEvtHeader))
	{
		evt_ele = (KEY_EVT_ELE *)Delete_Queue(&gEvtHeader);
		//YC_LOG_INFO("key index %d, key evt %d\r\n",evt_ele->key_ind,evt_ele->evt_type);
		gKeypadCtr.keypad_evt_callback(evt_ele->key_ind,evt_ele->evt_type);
	}
}

void keypad_Cblpm(int params)
{
	Lpm_unLockLpm(KEY_LPM_FLAG);
}

void keypad_delayLpm()
{
	if (SYS_TimerisExist(&gDelayLpm_timer))
	{
		SYS_ResetTimer(&gDelayLpm_timer);
	}
	else
	{
		SYS_SetTimer(&gDelayLpm_timer,15,TIMER_CYCLE,keypad_Cblpm);
	}
}

/**
byte __attribute__((noinline, aligned(32))) HREAD(word reg)
{
    return (*(volatile uint8_t *)(reg_map(reg)));
}
**/


