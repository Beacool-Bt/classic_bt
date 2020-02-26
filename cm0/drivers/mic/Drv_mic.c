#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "Drv_mic.h"
#include "yc11xx.h"
#include "Drv_debug.h"
#include "ima_enc.h"
#include "Drv_adc.h"
#include "ipc.h"

#if CODEC_MSBC
#include "Drv_msbc.h"
#endif

tMicCallback gfMicCb;
unsigned char gadcEncodeBuf[ENCODE_OUTPUT_LEN+20];
IPC_DATA_FORMAT *gIpcDataPtr;
MIC_CUR_VARIABLE sMicCurVariable;
CodecState mg = {0,0};


void Drv_micDefaultCB(IPC_DATA_FORMAT * format,int len)
{
	return;
}

void Drv_setMicCB(tMicCallback miccb)
{
	gfMicCb = miccb;
}

void DRV_Mic_Init(unsigned char *adcBuf,int bufLen,unsigned char *enBuf,int enLen)
{
	DRV_ADC_CONFIG adcCfg;
	
	gIpcDataPtr = (IPC_DATA_FORMAT*)&gadcEncodeBuf;
	
	sMicCurVariable.mEndPtr =(unsigned char *)( ((int)adcBuf +  bufLen) );
	memset(adcBuf, 0, bufLen);
	sMicCurVariable.mMicEnable = MIC_DISABLE;
	sMicCurVariable.mReadBufPtr = adcBuf;
	sMicCurVariable.mReadPtr = adcBuf;

	adcCfg.mDmaBuf = adcBuf;
	adcCfg.mDmaBufLen = bufLen;
	//adcCfg.mAdcConfig = DRV_ADC_M0RAM_ENABLE;
	adcCfg.mAdcConfig = DRV_ADC_M0RAM_ENABLE|DRV_ADC_8K_FILTer;
	//adcCfg.mAdcConfig = DRV_ADC_M0RAM_ENABLE|DRV_ADC_FILTER_DISABLE;
	adcCfg.mChannelCfg = DRV_ADC_CHANNEL_DIFF_GPIO1213;
	adcCfg.mSampleDelay = 0;
	Drv_adcdma_init(&adcCfg);

	#if CODEC_MSBC
	gMsbc_params.mSrcBuf = adcBuf;
	gMsbc_params.mSrcBufLen = bufLen;
	gMsbc_params.mDstBuf = enBuf;
	gMsbc_params.mDstBufLen =  enLen;
	gMsbc_params.mRdPtr = enBuf;
	Drv_mSBC_init();
	#endif
}


void DRV_Mic_DeInit(void)
{
	int len;
	len = (int)(sMicCurVariable.mReadBufPtr - sMicCurVariable.mEndPtr);
	memset(sMicCurVariable.mReadBufPtr, 0, len);
	return;
}

void DRV_Mic_sendEnable()
{
	sMicCurVariable.mMicBleEnable= MIC_ENABLE;
}

void DRV_Mic_sendDisable()
{
	sMicCurVariable.mMicBleEnable= MIC_DISABLE;
}

void DRV_Mic_Enable(void)
{
	int len;
	if(sMicCurVariable.mMicEnable == MIC_ENABLE)
		return;

	//len = (int)(sMicCurVariable.mReadBufPtr - sMicCurVariable.mEndPtr);
	//memset(sMicCurVariable.mReadBufPtr, 0, len);

	sMicCurVariable.mMicEnable = MIC_ENABLE;
	sMicCurVariable.mReadPtr = sMicCurVariable.mReadBufPtr;

	#if CODEC_MSBC
	Drv_mSBC_enable();
	#endif
	Drv_adc_enable();

}

void DRV_Mic_Disable(void)
{
	sMicCurVariable.mMicEnable = MIC_DISABLE;
	DRV_Mic_sendDisable();
	Drv_adc_disable();
	#if CODEC_MSBC
	Drv_mSBC_disable();
	#endif
} 

MIC_STATUS_ENUM DRV_Mic_State()
{
	return sMicCurVariable.mMicEnable;
}

int  DRV_Mic_Cycle_Mic_check()
{
	int audioLen,framelen,frame_len;
	int delaycount;
	int result = 0;
	if(sMicCurVariable.mMicEnable == MIC_DISABLE)
		return result;

	int audioReadPtr = HREADW(CORE_ADCD_ADDR) ;
	
	if (audioReadPtr >= TO_16BIT_ADDR(sMicCurVariable.mReadPtr))
	{
		while((audioReadPtr -  TO_16BIT_ADDR(sMicCurVariable.mReadPtr)) >= ENCODE_PACKETS_LEN)
		{
			//DEBUG_PRINT2(sMicCurVariable.mReadPtr,MIC_ONE_PACKET_LEN);
			result =  1;
			sMicCurVariable.mReadPtr+=ENCODE_PACKETS_LEN;
		}
	}
	else
	{
		while(TO_16BIT_ADDR(sMicCurVariable.mReadPtr) != TO_16BIT_ADDR(sMicCurVariable.mReadBufPtr))
		{
			//DEBUG_PRINT2(sMicCurVariable.mReadPtr,MIC_ONE_PACKET_LEN);
			result = 1;
			sMicCurVariable.mReadPtr+=ENCODE_PACKETS_LEN;
			if (sMicCurVariable.mReadPtr == sMicCurVariable.mEndPtr)
				sMicCurVariable.mReadPtr = sMicCurVariable.mReadBufPtr;
			
		}
	}
	return result;
}

#if CODEC_MSBC
uint8_t msbcChg;
uint8_t msbc_test;
#endif
int metest;
void DRV_Mic_Encode_ADC()
{
	int i;

	

	//if (sMicCurVariable.mMicBleEnable == MIC_DISABLE)
	//	return;
	
	#if CODEC_MSBC
	if (Drv_CheckMsbcCoding_packet(&gIpcDataPtr->ipcUnion.uBleData.data))
	{			
		(*gfMicCb)(gIpcDataPtr,20);
		Drv_CheckMsbcCoding_packet(&gIpcDataPtr->ipcUnion.uBleData.data);
		(*gfMicCb)(gIpcDataPtr,20);
		Drv_CheckMsbcCoding_packet(&gIpcDataPtr->ipcUnion.uBleData.data);
		(*gfMicCb)(gIpcDataPtr,17);
	}
	#endif
	
	//if(sMicCurVariable.mMicEnable == MIC_DISABLE)
	//	return;

	int audioReadPtr = HREADW(CORE_ADCD_ADDR) ;

	
	
	if (audioReadPtr >= TO_16BIT_ADDR(sMicCurVariable.mReadPtr))
	{
		while((audioReadPtr -  TO_16BIT_ADDR(sMicCurVariable.mReadPtr)) >= ENCODE_PACKETS_LEN)
		{
			for (i =0; i<ENCODE_PACKET_NUM;i++)
			{	
				metest++;
				#if CODEC_ADPCM
				encode(&mg, (s16*)sMicCurVariable.mReadPtr, (ENCODE_INPUT_LEN/2), 
					&gIpcDataPtr->ipcUnion.uBleData.data);
				(*gfMicCb)(gIpcDataPtr,ENCODE_OUTPUT_LEN);
				//DEBUG_PRINT2(sMicCurVariable.mReadPtr,ENCODE_INPUT_LEN);
				#elif CODEC_MSBC
				Drv_mSBC_RefreshWptr(ENCODE_INPUT_LEN);
				//DEBUG_PRINT2(sMicCurVariable.mReadPtr,ENCODE_INPUT_LEN);
				#elif CODEC_DUMP_ADC
				//DEBUG_PRINT2(sMicCurVariable.mReadPtr,ENCODE_INPUT_LEN);
				#endif
				sMicCurVariable.mReadPtr+=ENCODE_INPUT_LEN;
			}
			
		}
	}
	else
	{
		while(TO_16BIT_ADDR(sMicCurVariable.mReadPtr) != TO_16BIT_ADDR(sMicCurVariable.mReadBufPtr))
		{
			for (i =0; i<ENCODE_PACKET_NUM;i++)
			{	
				#if CODEC_ADPCM
				encode(&mg, (s16*)sMicCurVariable.mReadPtr, (ENCODE_INPUT_LEN/2), 
					&gIpcDataPtr->ipcUnion.uBleData.data);
				(*gfMicCb)(gIpcDataPtr,ENCODE_OUTPUT_LEN);
				//DEBUG_PRINT2(sMicCurVariable.mReadPtr,ENCODE_INPUT_LEN);
				#elif CODEC_MSBC
				Drv_mSBC_RefreshWptr(ENCODE_INPUT_LEN);
				#elif CODEC_DUMP_ADC
				//DEBUG_PRINT2(sMicCurVariable.mReadPtr,ENCODE_INPUT_LEN);
				#endif
				sMicCurVariable.mReadPtr+=ENCODE_INPUT_LEN;
			}
			
			if (sMicCurVariable.mReadPtr == sMicCurVariable.mEndPtr)
			{	
				sMicCurVariable.mReadPtr = sMicCurVariable.mReadBufPtr;
			}
		}
	}
}

