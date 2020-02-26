#ifndef __DRV_LED_H
#define	__DRV_LED_H

#include "yc11xx.h"



#define CORE_BASE_ADDR 0x10000000
#define WRITE(REG,VALUE)  	*((volatile unsigned char *)(CORE_BASE_ADDR + REG)) = VALUE
#define READ(REG)			*((volatile unsigned char *)(CORE_BASE_ADDR + REG)) 

#define REG_CONFIG_GPIO(N)	0X8080 + (N)
#define REG_GPIO_IN(N)		0X811B + (N)

#define GPIO_OUTPUT_LOW	    62
#define GPIO_OUTPUT_HIGH	63
#define GPIO_INPUT_PULLUP	0X40


#define LED_GPIO_NUM	    15


#define led_on()          *((volatile unsigned char *)(CORE_BASE_ADDR + REG_CONFIG_GPIO(LED_GPIO_NUM))) = GPIO_OUTPUT_HIGH
#define led_off()         *((volatile unsigned char *)(CORE_BASE_ADDR + REG_CONFIG_GPIO(LED_GPIO_NUM))) = GPIO_OUTPUT_LOW


void led_init(void);



#endif
