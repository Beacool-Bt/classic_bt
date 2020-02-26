﻿
#include "drv_i2c.h"
#include "yc11xx_iic.h"
#include "yc11xx_gpio.h"

#define IIC_SCL_GPIO_NUM					22	//gpio 22
#define IIC_SDA_GPIO_NUM					21	//gpio 21

void i2c_init(void)
{
	I2C_InitTypeDef mem_test;	//����I2C��ʼ���ṹ��
	mem_test.I2C_RXLen = 20;	//���ζԳ�ʼ����Ϣ��ֵ
	mem_test.I2C_TXLen = 20;
	mem_test.I2C_ClockSpeed = IICD_CLOCKSPEED400KHZ;
	I2C_Init(&mem_test);		//���ó�ʼ������
	
	GPIO_SetGpioMultFunction(IIC_SCL_GPIO_NUM,  GPCFG_IIC_SCL);	//����GPIO  22ΪSCL����
	GPIO_SetGpioMultFunction(IIC_SDA_GPIO_NUM, GPCFG_IIC_SDA );	//����GPIO 21ΪSDA����
}






