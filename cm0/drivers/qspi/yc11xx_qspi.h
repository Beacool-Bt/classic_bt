/**
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
  
/** 
  *@file qspi.h
  *@brief Queued SPI support for application.
  */
#ifndef _QSPI_H_
#define _QSPI_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include<stdlib.h>
#include "Yc11xx.h"
#include"Ycdef.h"
#include "yc_drv_common.h"


#define RXBUF_ADDR 0x10001ee0 


typedef enum
{
	MRAM =0,
	XRAM =1,
}MRAM_TYPE;


/**
  *@brief QSPI register definition.
  */
typedef struct 
{
	uint8_t Ctrl; 		/*!< QSPI configure */
	uint8_t Delay;		/*!< Delay time */
	uint16_t TxLen;		/*!< QSPI transport data length */
	uint16_t TxAddr;	/*!< QSPI transport data address */
	uint16_t RxAddr;	/*!< QSPI receive data address */
	uint16_t RxLen;		/*!< QSPI receive data length */
}QSPI_RegDef;

#define Flash_PageLength	0x100


#define FlashCMD_ReadRegister1 	0x05
#define FlashCMD_ReadRegister2	0x35
#define FlashCMD_WriteRegister 	0x01
#define FlashCMD_PageProgram 	0x02
#define FlashCMD_WriteEnable	0x06
#define FlashCMD_WriteDisable	0x04
#define FlashCMD_ReadDual	0x3b
#define FlashCMD_RDID 	0x9f
#define FlashCMD_REMS	0x90
#define FlashCMD_UID		0x4b
#define FlashCMD_PageErase   0x81
#define FlashCMD_SectorErase   0x20
#define FlashCMD_32kErase	0x52
#define FlashCMD_64kErase	0xd8
#define FlashCMD_ChipErase	0x60
#define FlashCMD_DeepPowerDown	0xb9


/**
  *@brief QSPI initialization.
  *@param None.
  *@return None.
  */
void   QSPI_Init();
void  WaitFlashFinish();


/**
  *@brief Read the flash, One is less than 0x80.
  *@param flash_addr Flash address.
  *@param len Data length.
  *@param rxbuf Data buffer.
  *@return None.
  */
void __attribute__((noinline,aligned(32)))  QSPI_ReadFlashData(int flash_addr, int len, uint8_t *rxbuf);

/**
  *@brief Read flash RDID.
  *@param rxbuf Receive RDID buffer.
  *@return None.
  */
void __attribute__((noinline,aligned(32)))  QSPI_ReadFlashRDID(uint8_t *rxbuf);

/**
  *@brief Read flash register.
  *@param rxbuf Receive register value buffer.
  *@return None.
  */
void __attribute__((noinline,aligned(32)))  QSPI_ReadFlashReg(uint8_t *rxbuf);

/**
  *@brief Read flash REMS.
  *@param rxbuf Receive REMS buffer. 
  *@return None.
  */
void  __attribute__((noinline,aligned(32)))  QSPI_ReadFlashREMS(uint8_t *rxbuf);

/**
  *@brief Read flash UID.
  *@param rxbuf Receive UID buffer. 
  *@return None.
  */
void __attribute__((noinline,aligned(32)))  QSPI_ReadFlashUID(uint8_t *rxbuf);

/**
  *@brief Write flash.
  *@param flash_addr Flash address where send command to.
  *@param len Write flash data length.
  *@param txdata Write flash data buffer.
  *@return None.
  */
void    __attribute__((noinline,aligned(32))) QSPI_WriteFlashData(int flash_addr, int len, uint8_t *txdata);

/**
  *@brief Erasure by page ,256byte at a time,size range 0~0xFF.
  *@param flash_addr The flash address will be erasure.
  *@return None.
  */
void __attribute__((noinline,aligned(32))) QSPI_PageEraseFlash(int flash_addr);

/**
  *@brief Erasure by sector ,2KB at a time,size range 0~0xFFF.
  *@param flash_addr The flash address will be erasure.
  *@return None.
  */
void  __attribute__((noinline,aligned(32)))  QSPI_SectorEraseFlash(int flash_addr);

/**
  *@brief Erasure 32KB at a time,size range 0~0x7FFF.
  *@param flash_addr The flash address will be erasure.
  *@return None.  
  */
void  __attribute__((noinline,aligned(32)))  QSPI_32kEraseFlash(int flash_addr);

/**
  *@brief Erasure 32KB at a time,size range 0~0xFFFF.
  *@param flash_addr The flash address will be erasure.
  *@return None.  
  */
void  __attribute__((noinline,aligned(32)))  QSPI_64kEraseFlash(int flash_addr);

/**
  *@brief Erasure all.
  *@return None.
  */
void  __attribute__((noinline,aligned(32)))  QSPI_ChipEraseFlash();

/**
  *@brief Enable Quadruple SPI model.
  *@param on is a non-zero positive integer the model on , on is zero represent the model off.
  *@return None.
  */
void   __attribute__((noinline,aligned(32)))  QSPI_FlashQuadEnable(uint8_t on);

/**
  *@brief Enter DEEP POWER DOWN model, can not read and write.
  *@return None.
  */
void  __attribute__((noinline,aligned(32)))  QSPI_FlashDeepPowerDown();

void __attribute__((noinline))   setRamType(MRAM_TYPE type );
void __attribute__((noinline))   SetLockQSPI( );
void __attribute__((noinline))   SetReleaseQSPI( );

#endif
 
