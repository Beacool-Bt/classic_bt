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
 *@file mic.h
 *@brief microphone support for application.
 */
#ifndef _DRV_MIC_H_
#define _DRV_MIC_H_
#include <stdio.h>
#include "ipc.h"

/**
  *@brief Code type.
  */
#define CODEC_MSBC 	0
#define CODEC_ADPCM	0
#define CODEC_DUMP_ADC 1

/**
  *@brief Code length select.
  */
#if CODEC_MSBC
#define ENCODE_INPUT_LEN 240
#define ENCODE_OUTPUT_LEN 57
#define ENCODE_PACKET_NUM 1
#elif CODEC_ADPCM
#define ENCODE_INPUT_LEN 80
#define ENCODE_OUTPUT_LEN 20
#define ENCODE_PACKET_NUM 6
#elif CODEC_DUMP_ADC
#define ENCODE_INPUT_LEN 240
#define ENCODE_OUTPUT_LEN 0
#define ENCODE_PACKET_NUM 1
#endif
#define ENCODE_PACKETS_LEN (ENCODE_INPUT_LEN*ENCODE_PACKET_NUM)

/**
  *@brief Microphone state enum.
  */
typedef enum
{
	MIC_DISABLE,
	MIC_ENABLE
}MIC_STATUS_ENUM;

/**
  *@brief Microphone current variable.
  */
typedef struct
{
	unsigned char *mReadBufPtr;	/*!< the pointer of microphone read buffer start */
	unsigned char *mReadPtr;	/*!< Microphone read pointer*/
	unsigned char *mEndPtr;		/*!< the pointer of microphone read buffer ending*/
	MIC_STATUS_ENUM mMicEnable;	/*!< microphone enable */
	MIC_STATUS_ENUM mMicBleEnable;/*!< microphone ble enable*/
}MIC_CUR_VARIABLE;

typedef void (*tMicCallback)(IPC_DATA_FORMAT *,int);

/**    
  *@brief Microphone initialization.
  *@param adcBuf Analog-digital conversion source buffer.
  *@param bufLen Source buffer length.
  *@param enBuf Analog-digital conversion dest buffer.
  *@param enLen Dest buffer length.
  *@return None.
  */
//void DRV_Mic_Init(unsigned char *adcBuf,int bufLen );
void DRV_Mic_Init(unsigned char *adcBuf,int bufLen,unsigned char *enBuf,int enLen);

/**    
  *@brief Microphone cancel initialization.
  *@param None.
  *@return None.
  */
void DRV_Mic_DeInit(void);

/**    
  *@brief Microphone enable.
  *@param None.
  *@return None.
  */
void DRV_Mic_Enable(void);

/**   
  *@brief Microphone disable.
  *@param None.
  *@return None.
  */
void DRV_Mic_Disable(void);

/**   
  *@brief Microphone dump adc data.
  *@param None.
  *@return None.
  */
void DRV_Mic_dump_adc_data();

/**   
  *@brief Encode audio ADPCM.
  *@param None.
  *@return None.  
  */
void DRV_Mic_Encode_Audio_ADPCM();

/**   
  *@brief Circle check microphone. 
  *@param None.
  *@return Cycle times.
  */
int  DRV_Mic_Cycle_Mic_check();

/**   
  *@brief Encode ADC.
  *@param None.
  *@return None.
  */
void DRV_Mic_Encode_ADC();

/**   
  *@brief Setting call back function.
  *@param call back function.
  *@return None.
  */
void Drv_setMicCB(tMicCallback miccb);

/**   
  *@brief Getting microphone state.
  *@param None.
  *@return MIC_STATUS_ENUM microphone state.@ref MIC_STATUS_ENUM
  */
MIC_STATUS_ENUM DRV_Mic_State();

/**   
  *@brief Enable microphone send.
  *@param None.
  *@return None.
  */
void DRV_Mic_sendEnable();

/**   
  *@brief Disable microphone send.
  *@param None.
  *@return None.
  */
void DRV_Mic_sendDisable();

#endif
