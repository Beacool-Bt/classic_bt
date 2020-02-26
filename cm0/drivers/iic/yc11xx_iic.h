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
  *@file iic.h
  *@brief IIC support for application.
  */
#ifndef _IIC_H_
#define _IIC_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yc11xx.h"
#include "yc_drv_common.h"

/**
  *@brief I2C_ClockSpeed.
  */ 
#define IICD_CLOCKSPEED200KHZ 26
#define IICD_CLOCKSPEED400KHZ 11
#define IICD_CLOCKSPEED800KHZ 4
#define IICD_CLOCKSPEED1MHZ 2

/** 
  *@brief I2C initialized structure definition.  
  */
typedef struct
{
  uint32_t I2C_ClockSpeed;          /*!< Specifies the clock frequency.
                                         This parameter must be set to a value lower than 400kHz */
                                         
  uint16_t I2C_TXLen; /*!< Specifies Tx DMA buff length */

  uint16_t I2C_RXLen; /*!< Specifies Rx DMA buff length */
  
}I2C_InitTypeDef;

/** 
  *@brief I2C_RegAddr.
  */
#define I2C_REGADDR8BITS(x) (0x01000000|x)
#define I2C_REGADDR16BITS(x) (0x10000000|x)

/** 
  *@brief I2C_RegAddr.
  */
#define IS_I2C_REGADDR(x) (0x11000000&x)
#define IS_I2C_REGADDR8BITS(x) (0x01000000&x)
#define IS_I2C_REGADDR16BITS(x) (0x10000000&x)

/**
  *@brief cancel IIC initialization.
  *@param None.
  *@return None.
  */
void I2C_DeInit(void);

/**
  *@brief IIC initialization.
  *@param I2C_InitStruct IIC initializing structure.@ref I2C_InitTypeDef
  *@return None.
  */
void I2C_Init(I2C_InitTypeDef* I2C_InitStruct);

/** 
  *@brief This function can send a byte data through IIC bus.
  *@param DevAddr target device address.
  *@param RegAddr target register address.  
  *@param Data the data will be send.
  *@return None.  
  */
void I2C_SendData(uint32_t DevAddr, uint32_t RegAddr, uint16_t Data);

/** 
  *@brief This function can receive a byte data from IIC bus.
  *@param DevAddr target device address.
  *@param RegAddr target register address.  
  *@return a byte data that read from IIC.
  */
uint32_t I2C_ReceiveData(uint32_t DevAddr, uint32_t RegAddr);

/** 
  *@brief This function can receive a set of data through IIC bus.
  *@param DevAddr target device address.
  *@param RegAddr target register address.  
  *@param RxBuff The buffer that the read data is stored.
  *@param RxLen the number of byte received.
  *@return None.  
  */
void I2C_ReadDatatoBuff(uint32_t DevAddr, uint32_t RegAddr, uint8_t* RxBuff, uint16_t RxLen);

/** 
  *@brief This function can send a set of data through IIC bus.
  *@param DevAddr target device address.
  *@param RegAddr target register address.  
  *@param TxBuff the data will be send.
  *@param RxLen the number of byte to be send.
  *@return None.  
  */
void I2C_SendDataFromBuff(uint32_t DevAddr, uint32_t RegAddr, uint8_t* TxBuff, uint16_t TxLen);

#endif
