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
  *@file dac.h
  *@brief DAC support for application.
  */
#ifndef CM0_COMPONENTS_DRIVER_MIC_DRV_DAC_H_
#define CM0_COMPONENTS_DRIVER_MIC_DRV_DAC_H_

#include <stdint.h>

#define DRV_DAC_ENABLE 0x01
#define DRV_DAC_SEL_MRAM 0x40
#define DRV_DAC_8KSAMPLE 0x20

/**
  *@brief Digital-analog conversion structure definition.
  */
typedef struct
{
	uint8_t *mDacBufferPtr;	/*!< DAC buffer pointer */
	int mDacBufferLen;		/*!< DAC buffer length */
	uint8_t mDacOutP;		/*!< DAC output gpio1 */
	uint8_t mDacOutN;		/*!< DAC output gpio2 */
	uint8_t mDacConfig;		/*!< DAC configure byte */
} DRV_DAC_PARM;

extern DRV_DAC_PARM gDrvAdcParm;

/**
  *@brief DAC initialization.
  *@param dacBuffer DAC buffer pointer.
  *@param bufferLen DAC buffer length.
  *@return None.
  */
void Drv_dac_init(uint8_t * dacBuffer, int bufferLen);

/**
  *@brief DAC enable.
  *@param None.
  *@return None.
  */
void Drv_dac_enable();

/**
  *@brief DAC disable.
  *@param None.
  *@return None.
  */
void Drv_dac_disable();

#endif
