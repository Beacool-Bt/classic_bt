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
  *@file gpio.h
  *@brief GPIO support for application.
  */

#ifndef _GPIO_H_
#define _GPIO_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "yc11xx.h"

#define GPIO_NUM_MASK 0x1f

void GPIO_SetOut(GPIO_NUM gpio, bool BitVal);
void GPIO_SetInput(GPIO_NUM gpio);
void GPIO_SetOutput(GPIO_NUM gpio);
void GPIO_ClearWakeup(GPIO_NUM gpio);
void GPIO_SetGpioMultFunction(GPIO_NUM gpio, uint8_t Func);
bool gpioGetBit(uint8_t Num, uint32_t regBase);
bool GPIO_GetInputStatus(GPIO_NUM gpio);
void GPIO_SetWakeup(GPIO_NUM gpio);
void GPIO_SetWakeupByCurrentState(GPIO_NUM gpio);

/**
  *@brief Set gpio multple function.
  *@param Num gpio number.
  *@param Func gpio function,value correspond to function,
  *		-0  input
  *		-2  qspi_ncs
  *		-3  qspi_sck
  *		-4  qspi_io0
  *		-5  qspi_io1
  *		-6  qspi_io2
  *		-7  qspi_io3
  *		-8  uart_txd
  *		-9  uart_rxd
  *		-10 uart_rts
  *		-11 uart_cts
  *		-12 uartb_txd
  *		-13 uartb_rxd
  *		-14 uartb_rts
  *		-15 uartb_cts
  *		-16 pwm_out0
  *		-17 pwm_out1
  *		-18 pwm_out2
  *		-19 pwm_out3
  *		-20 pwm_out4
  *		-21 pwm_out5
  *		-22 pwm_out6
  *		-23 pwm_out7 
  *		-24 i2s_dout
  *		-25 i2s_lrckout
  *		-26 i2s_clkout
  *		-28 i2s_din
  *		-29 i2s_lrckin
  *		-30 i2s_clkin
  *		-31 spid_miso 
  *		-32 spid_ncs
  *		-33 spid_sck 
  *		-34 spid_mosi
  *		-35 spid_sdio 
  *		-36 jtag_swclk 
  *		-37 jtag_swdat 
  *		-38 qdec_x0
  *		-39 qdec_x1 
  *		-40 qdec_y0
  *		-41 qdec_y1 
  *		-42 qdec_z0
  *		-43 qdec_z1
  *		-44 iic_sda 
  *		-45 iic_scl
  *		-46 dac outp
  *		-47 dac outn 
  *		-60 swclk 
  *		-61 swdat
  *		-62 output_low
  *		-63 output_high  
  *@return None.
  */
//void GPIO_SetGpioMultFunction(uint8_t Num, uint8_t Func);

#endif


