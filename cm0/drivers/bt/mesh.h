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
  *@file mesh.h
  *@brief mesh support for application.
  */
#ifndef _MESH_NET_H
#define _MESH_NET_H
#include "yc11xx.h"
#include "ipc.h"

/**
  *@brief This function will set bluetooth adv data.
  *@param Dt the pointer of adv data.
  *@param Size the size of new adv data.
  *@return None.
  */
void MSH_SetAdvData(uint8_t* Dt, uint16_t Size);

/**
  *@brief This function will set bluetooth scan res.
  *@param Dt the pointer of scan response.
  *@param Size the size of new scan response.
  *@return None.
  */
void MSH_SetScanData(uint8_t* Dt, uint16_t Size);

/**
  *@brief This function will stop adv state.
  *@param None.
  *@return None.
  */
void MSH_StopAdv();

/**
  *@brief This function will start adv state.
  *@param None.
  *@return None.
  */
void MSH_StartAdv();

/**
  *@brief This function will start scan.
  *@param None.
  *@return None.
  */
void MSH_StartScan();

/**
  *@brief This function will stop scan.
  *@parma None.
  *@return None.
  */
void MSH_StopScan();

/**
  *@brief This function set bluetooth mac address.
  *@param Addr the new bluetooth mac to set.
  *@return None.
  */
void MSH_SetMacAddr(tBTADDR Addr);

/**
  *@brief adv type.
  */
enum advType{
	ADV_TYPE_NOMAL,
	ADV_TYPE_DIRECT,
	ADV_TYPE_NOCONNECT,
};

/**
  *@brief scan type.
  */
enum scanType{
	SCAN_TYPE_PASSIVE,
	SCAN_TYPE_ACTIVE,
};

/**
  *@brief This function set adv type.
  *@param Type the new adv type to set.@ref advType
  *@return None.
  */
void MSH_SetAdvType(enum advType Type);

/**
  *@brief This function set rsp type.
  *@param Type the new rsp type to set.@ref scanType
  *@return None.
  */
void MSH_SetScanRspType(enum scanType Type);

/**
  *@brief This function set Adv interval.
  *@param Type the new rsp type to set.
  *@return None.
  */
void MSH_SetAdvInterval(uint16_t Intv);

/**
  *@brief This function set scan interval.
  *@param Type the new rsp type to set.
  *@return None.
  */
void MSH_SetScanInterval(uint16_t Intv);

#endif

 
