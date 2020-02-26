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
  *@file error.h
  *@brief error support for application.
  */
#ifndef YC_ERROR_H
#define YC_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#define YC_ERROR_BASE_NUM      (0x00)       /*!< Global error base */

/**
  *@brief Log type.
  */
#define YC_SUCCESS                              (YC_ERROR_BASE_NUM + 0)		/*!< Successful command */
#define YC_ERROR_NOT_FOUND                      (YC_ERROR_BASE_NUM + 1)		/*!< Not found error */
#define YC_ERROR_OPENED                         (YC_ERROR_BASE_NUM + 2)		/*!< Opened error */
#define YC_ERROR_DATA_TOO_LOOG                  (YC_ERROR_BASE_NUM + 3)		/*!< Data too loog error */



#ifdef __cplusplus
}
#endif

#endif

