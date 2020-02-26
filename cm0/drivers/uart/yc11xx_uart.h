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
  *@file uart.h
  *@brief uart support for application
  */
#ifndef _UART_H_
#define _UART_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "yc11xx.h"
#include "yc_drv_common.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
  *@brief Baudrate definition.
  */
#define UARTE_BAUDRATE_BAUDRATE_Baud1200        (0x804F) /*!< 1200 baud  */
#define UARTE_BAUDRATE_BAUDRATE_Baud2400        (0x809D) /*!< 2400 baud  */
#define UARTE_BAUDRATE_BAUDRATE_Baud4800        (0x813B) /*!< 4800 baud  */
#define UARTE_BAUDRATE_BAUDRATE_Baud9600        (0x8275) /*!< 9600 baud  */
#define UARTE_BAUDRATE_BAUDRATE_Baud14400       (0x83AF) /*!< 14400 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud19200       (0x84EA) /*!< 19200 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud28800       (0x875C) /*!< 28800 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud31250       (0x8800) /*!< 31250 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud38400       (0x89D0) /*!< 38400 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud56000       (0x8E50) /*!< 56000 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud57600       (0x8EB0) /*!< 57600 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud76800       (0x83A9) /*!< 76800 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud115200      (0x81A0) /*!< 115200 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud230400      (0x803B) /*!< 230400 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud250000      (0x8040) /*!< 250000 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud460800      (0x8074) /*!< 460800 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud921600      (0x8034) /*!< 921600 baud */
#define UARTE_BAUDRATE_BAUDRATE_Baud1M              (0x9000) /*!< 1Mega baud */


/**
  *@brief USART_Word_Length.
  */ 
#define USART_WordLength_8b                  (0<<2)
#define USART_WordLength_9b                  (1<<2)

/**
  *@brief USART_Stop_Bits. 
  */
#define USART_StopBits_1                 (0<<3)
#define USART_StopBits_2                 (1<<3)
#define IS_USART_STOPBITS(STOPBITS)		 (((STOPBITS) == USART_StopBits_1) ||  \
							((STOPBITS) == USART_StopBits_2) )

/**
  *@brief USART_Parity.
  */ 
#define USART_Parity_Even                    (0<<1)
#define USART_Parity_Odd                     (1 << 1) 
#define IS_USART_PARITY(PARITY)		 ( ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

/**
  *@brief USART_Mode. 
  */
#define USART_Mode_Single_Line        (1<<6)
#define USART_Mode_duplex                      (0<<6)
#define IS_USART_MODE(MODE) 		(((MODE) == USART_Mode_Single_Line) ||\
				((MODE) == USART_Mode_duplex))

/**
  *@brief USART_Hardware_Flow_Control. 
  */ 
#define USART_HardwareFlowControl_None       (0<<4)
#define USART_HardwareFlowControl_ENABLE       (1<<4)

/** 
  *@brief  USART channel define.
  */ 
typedef enum
{
	UARTA = 0,
	UARTB,
}USART_TypeDef;

#define IS_USARTAB(USARTx)         (USARTx == UARTA)||\
						(USARTx == UARTB)

/** 
  *@brief USART initialized Structure definition.  
  */ 
typedef struct
{
  uint32_t USART_BaudRate;            	/*!< This member configures the USART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            -USART_BaudRate = (48M/baudrate)|(1<<31) */

  uint16_t USART_WordLength;          	/*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref USART_Word_Length */

  uint16_t USART_StopBits;            	/*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_Stop_Bits */

  uint16_t USART_Parity;              	/*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
 
  uint16_t USART_Mode;                	/*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_Mode */

  uint16_t USART_HardwareFlowControl; 	/*!< Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref USART_Hardware_Flow_Control */
                                           
  uint16_t USART_TXLen; 				/*!< Specifies Tx DMA buff length */
  
  uint16_t USART_RXLen; 				/*!< Specifies Rx DMA buff length */
} USART_InitTypeDef;

/**
  *@brief Uart initialization.
  *@param USARTx USART channel select.
  *@param USART_InitStruct Pointer of initializing uart structure.
  *@return None.
  */
void USART_Init(USART_TypeDef USARTx, USART_InitTypeDef* USART_InitStruct);

/**
  *@brief Cancle uart initialization.
  *@param USARTx USART channel select.
  *@return None.
  */
void USART_DeInit(USART_TypeDef USARTx);

/**
  *@brief Sending a byte data by UART.
  *@param USARTx USART channel select.
  *@param Data The data to be send.
  *@return None.
  */
void USART_SendData(USART_TypeDef USARTx, uint16_t Data);

/**
  *@brief Receive a byte data from UART.
  *@param USARTx USART channel select.
  *@returns A 16-bit data what read from UART . 
  */
uint16_t USART_ReceiveData(USART_TypeDef USARTx);

/**
  *@brief Gets the position of the read pointer of UART.
  *@param USARTx USART channel select.
  *@returns The current read pointer position of the UART.
  */
uint16_t USART_GetRxCount(USART_TypeDef USARTx);

/**
  *@brief Receive data from UART.
  *@param USARTx USART channel select.
  *@param RxBuff The pointer that stores data read from the UART.
  *@param RxSize The number of byte read from UART.
  *@retval 0 The length of the read data is not enought of does not read the data;
  *@retval non-zero The actual number of byte read from UART.
  */
uint16_t USART_ReadDatatoBuff(USART_TypeDef USARTx, uint8_t* RxBuff, uint16_t RxSize);

/**
  *@brief Sending data by UART.
  *@param USARTx USART channel select.
  *@param TxBuff The pointer of the data to be send.
  *@param TxLen The number of byte of the data to be send.
  *@return The actual number of byte send to UART.
  */
uint16_t USART_SendDataFromBuff(USART_TypeDef USARTx, uint8_t* TxBuff, uint16_t TxLen);

#ifdef __cplusplus
}
#endif

#endif

