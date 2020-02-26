/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
 /** @file
 *
 * device support for application
 */
#ifndef ___KDEVICE_H__
#define ___KDEVICE_H__
#include <stdio.h>
#include "ycdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function registers a device driver with specified name.
 *
 * @param dev the pointer of device driver structure
 * @param flags the flag of device
 *
 * @return the error code.
 */
error_t DEV_Register(enum device_id Devid, tReinit R, tEnterlpm E);


/**
 * This function removes a previously registered device driver
 *
 * @param dev the pointer of device driver structure
 *
 * @return the error code.
 */
error_t DEV_UnRegister(enum device_id Devid);

/**
 * This function will prepare reinit all device when low power mode wake.
 *
 * @return none
 */
void DEV_RestartAll(void);

/**
 * This function will prepare all device to wait enter low power mode. 
 *
 * @return none
 */
void DEV_EnterLpmMode(void);

/**
 * This function will create device pool
 *
 * @return none
 */
void DEV_PoolEnable(uint8_t Count);

/*@}*/
#ifdef __cplusplus
}
#endif

#endif


