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
  *@file ks.h
  *@brief key scan support for application.
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yc11xx.h"
#include "yc_drv_common.h"

#ifndef _KEYSCAN_
#define _KEYSCAN_

#define KS_BUFF_POLL_LEN 7
typedef struct {
	uint8_t ksPool[KS_BUFF_POLL_LEN];
}tKSEVENT;

/**
  *@brief This function initialize key scan hardware.
  *@param None. 
  *@return None.
  */
void KS_Initialize(uint8_t Col[], 
				uint8_t RowSize, 
				uint8_t ColSize, 
				uint8_t KsData[]);

/**
  *@brief This function Get Keyscan event message.
  *@param None.
  *@return Pointer of keyscan event.
  */
tKSEVENT *KS_GetCurrentEvt();

/**
  *@brief This function scan ks matrix. 
  *@param None.
  *@return None.
  */
void KS_ScanMatrix();

/**
  *@brief This function uninstall key scan hardware.
  *@param None. 
  *@return None.
  */
void KS_Unistall();
#endif
 