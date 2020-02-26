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
  *@file spi.h
  *@brief spi support for application.
  */
#ifndef _SPI_H_
#define _SPI_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "yc11xx.h"
#include "yc_drv_common.h"

/**
  *@brief SPI_mode. 
  */ 
#define SPI_Mode_Four_Wire                 	4
#define SPI_Mode_Three_Wire			3
#define SPI_Mode_Two_Wire			2

#define IS_SPI_MODE(MODE) 		(((MODE) == SPI_Mode_Two_Wire) || \
                                    	((MODE) == SPI_Mode_Two_Wire)) || \
                                    	((MODE) == SPI_Mode_Three_Wire)

/**
  *@brief SPI_CPOL.
  */  
#define SPI_CPOL_Active_High                 	((uint8_t)0<<4)
#define SPI_CPOL_Active_Low			((uint8_t)1<<4)

#define IS_SPI_CPOL(CPOL) 		(((CPOL) == SPI_CPOL_Active_High) || \
                                    	((CPOL) == SPI_CPOL_Active_Low))


/**
  *@brief SPI_CPHA.
  */ 
#define SPI_CPHA_First_Edge			((uint8_t)0<<5)
#define SPI_CPHA_Second_Edge		((uint8_t)1<<5)

#define IS_SPI_CPHA(CPHA) 		(((CPHA) == SPI_CPHA_First_Edge) || \
                                    	((CPHA) == SPI_CPHA_Second_Edge))

										
/**
  *@brief SPI_BaudSpeed. 
  */
 #define SPI_BAUDSPEED_2MHz			4
#define SPI_BAUDSPEED_750KHz		5
#define SPI_BAUDSPEED_375KHz		6
#define SPI_BAUDSPEED_187KHz		7
#define IS_SPI_BAUDSPEED(BAUDSPEED) 		(((BAUDSPEED) >=SPI_BAUDSPEED_2MHz)  &&  ((BAUDSPEED) <= SPI_BAUDSPEED_187KHz))

/** 
  *@brief SPI initialized structure definition.  
  */
typedef struct
{
  uint16_t SPI_Mode;                /*!< Specifies the SPI operating mode.
                                         This parameter can be a value of @ref SPI_mode */
                                         
   uint16_t SPI_CPOL;               /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

  uint16_t SPI_CPHA;                /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */
  
  uint16_t SPI_BaudSpeed;   		/*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock.
                                         This parameter can be a value of @ref SPI_BaudRate_Prescaler.
                                         @note The communication clock is derived from the master
                                               clock. The slave clock does not need to be set. */
  uint16_t SPI_TXLen; 				/*!< Specifies Tx DMA buff length */
  
  uint16_t SPI_RXlen; 				/*!< Specifies Rx DMA buff length */ 
}SPI_InitTypeDef;

/**
  *@brief SPI initialization.
  *@param SPI_InitStruct SPI initializing structure.@ref SPI_InitTypeDef
  *@return None.
  */
void SPI_Init(SPI_InitTypeDef* SPI_InitStruct);

/**
  *@brief cancel SPI initialization.
  *@param None.
  *@return None.
  */
void SPI_DeInit(void);

/**
  *@brief This function can send data through SPI bus.
  *@param Data the data will be send.
  *@return None.  
  */
void SPI_SendData(uint16_t Data);

/**
  *@brief The function is used to read the value of the SPI register.
  *@param RegAdr address of the register to read.
  *@return SPI current read pointer.
  */
uint16_t SPI_ReadRegister(uint16_t RegAdr);

/** 
  *@brief This function can send a set of data through SPI bus.
  *@param Buff the pointer of data that will be send.
  *@param Len the number of byte to be send.
  *@return None.  
  */
void SPI_SendDataFromBuff(uint8_t *Buff, uint16_t Len);

/** 
  *@brief This function can receive a set of data through SPI bus.
  *@param TxBuff the pointer of data that will be send.
  *@param TxLen the number of byte to be send.
  *@param RxBuff the pointer that the read data is stored.
  *@param RxLen the number of byte received.
  *@return None.
  */
void SPI_ReceiveDataToBuff(uint8_t *TxBuff, uint16_t TxLen, uint8_t *RxBuff, uint16_t RxLen);

#endif
 
