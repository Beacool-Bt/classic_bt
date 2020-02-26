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
  *@file cvsd.h
  *@brief cvsd support for application.
  */
#ifndef CM0_COMPONENTS_DRIVER_MIC_DRV_CVSD_H_
#define CM0_COMPONENTS_DRIVER_MIC_DRV_CVSD_H_

#include <stdint.h>

#define DRV_CVSD_ENABLE 0x01
#define DRV_CVSD_SEL_MRAM 0x40
#define DRV_CVSD_DMA_ENABLE 0x80

/**
  *@brief CVSD parameter,input PCM format output CVSD format,input CVSD format output PCM format.
  */
typedef struct {
	int mCVSDLen;				/*!< CVSD length */
	uint8_t * mCvsdOutputPtr;	/*!< pointer of CVSD output */
	uint8_t * mCvsdInputPtr;	/*!< pointer of CVSD input */
	uint8_t * mPcminputdataPtr;	/*!< pointer of PCM input */
	uint8_t * mPcmOutputdataPtr;/*!< pointer of CVSD output */
	uint8_t mCVSDCtrl;			/*!< CVSD configure byte */
}DRV_CVSD_PARAM;

extern DRV_CVSD_PARAM gCVSDParam;

/**
  *@brief CVSD initialization.
  *@param None.
  *@return None.
  */
void Drv_cvsd_init();

/**
  *@brief CVSD disable.
  *@param None.
  *@return None.
  */
void Drv_cvsd_disable();

/**
  *@brief CVSD enable.
  *@param None.
  *@return None.
  */
void Drv_cvsd_enable();

#endif
