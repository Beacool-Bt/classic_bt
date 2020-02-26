#include <stdlib.h>
#include "fir_8to48.h"
#include "yc11xx.h"
#include "yc_drv_common.h"
#include "Drv_debug.h"
#include "yc_voiceprompt.h"

typedef enum
{
	DAC_OPEN,
	DAC_CLOSE,
}DAC_STATE;
DAC_STATE dacState;

#define DAC_VALUE_CHECK

#define ONE_PACKET_LEN 60
#define HANDLE_BUFFER_LEN 1200

#define AVERGER_LEN 256
#define AVERGER_PWR 8


short filterData[FILTER_COL]={0};

short filterParams[FILTER_ROW][FILTER_COL]=
{-9,-281,1420,4058,824,-236,
-26,-278,2088,3815,347,-171,
-59,-191,2761,3362,10,-108,
-108,10,3362,2761,-191,-59,
-171,347,3815,2088,-278,-26,
-236,824,4058,1420,-281,-9};

short * gPcmout  = (short *)(reg_map_m0(PCM_OUT_START));
short * gDacout  = (short *)(reg_map_m0(DAC_48K_START_ADDR));

uint8_t indexarray=0;
short gDacIndex;
int gPcmReadPtr;

int gPreIndex;
int32_t gDacSum;
int16_t gDacThreshold = 300;
int32_t gTempAvg;

int8_t gDacFlag;
int8_t gDacCount;

//int gTett;

void insert_48K(short data)
{
	int sum ;

	filterData[indexarray] = data;
	for(int row =0; row < FILTER_ROW; row++)
	{
		sum = 0;
		for (int col=0,ind=indexarray ; col < FILTER_COL; col++)
		{
			sum += filterParams[row][col]*filterData[ind];
			if(ind == 0)
				ind = FILTER_COL-1;
			else
				ind--;
		}

		gDacout[gDacIndex++] =  (sum>>13);
	}
	
	if ((++indexarray) == FILTER_COL)
		indexarray = 0;
}


void sco_fir_init()
{
	gPcmReadPtr=0;
	gDacIndex = 0;
}

#define PTR_AUTO_INC() 	{	\
	if (gDacIndex >= DAC_48K_LEN )	\
		gDacIndex = 0;	\
	gPcmReadPtr+=MAX_PCK_LEN;		\
	if (gPcmReadPtr == PCM_OUT_END)		\
		gPcmReadPtr = PCM_OUT_START;	\
	HWRITEW(CORE_DAC_WPTR,DAC_48K_START_ADDR+(gDacIndex<<2));		\
}

void check_dac_value(int ind)
{
	OS_ENTER_CRITICAL();
	for (int j = 0;j<(MAX_PCK_LEN>>1);j++)
	{
		if ((HREAD(CORE_CVSD_CTRL) & 0x01)!= 0x01)
		{
			OS_EXIT_CRITICAL();
			return;
		}
		#ifdef DAC_VALUE_CHECK
		gDacSum += abs(gPcmout[j+ind]);
		#endif //DAC_VALUE_CHECK
		insert_48K(gPcmout[j+ind]);
	}
	OS_EXIT_CRITICAL();

	//DEBUG_PRINT2((char *)&gPcmout[ind],MAX_PCK_LEN);

	if (gDacFlag == 2)
	{	
		for (int j = 0;j<(MAX_PCK_LEN>>1);j++)
		{
			gDacSum  -=abs(gPcmout[gPreIndex+j]);
		}
		gPreIndex+=(MAX_PCK_LEN>>1);
		if (gPreIndex >=PCM_OUT_LEN)
			gPreIndex = 0;

		gTempAvg = gDacSum>>AVERGER_PWR;
		//DEBUG_PRINT2((char *)&gTempAvg,2);
		if ((gTempAvg) > gDacThreshold){
			//dacState = DAC_OPEN; //DAC_OPEN
			gDacCount = 0;
			if (((1 != HREAD(mem_special_cid)) || (30 != HREAD(mem_sco_rx_packet_len))) )
				HWRITE(mem_mic_mute, 1);
		}
		else{
			gDacCount++;
			if (gDacCount >=15){
				HWRITE(mem_mic_mute, 0);
				//dacState = DAC_CLOSE; //DAC_CLOSE
				gDacCount = 0;
			}
		}
			
	}

	if (ind+(MAX_PCK_LEN>>1) >= AVERGER_LEN)
	{
		gDacFlag = 2;
	}
	
	PTR_AUTO_INC();
}

uint8_t sco_fir_polling()
{
	int PcmOutPtr;
	#ifdef AUDIO_SYNC_DEF
	if ((HREAD(CORE_CVSD_CTRL) & 0x01)== 0x01
		&& (HREAD(mem_enable_hfp_start_sync) != 1))
	#else
	if ((HREAD(CORE_CVSD_CTRL) & 0x01)== 0x01)
	#endif //AUDIO_SYNC_DEF
	{
		PcmOutPtr = HR_REG_16BIT(reg_map(0x8364));
		if (PcmOutPtr != gPcmReadPtr)
		{
			if (PcmOutPtr > gPcmReadPtr)
			{
				if (PcmOutPtr - gPcmReadPtr >= MAX_PCK_LEN)
				{						
					int index1 = (gPcmReadPtr-PCM_OUT_START)>>1;
					check_dac_value(index1);
					return 1;
				}
			}
			else
			{
				int index1 = (gPcmReadPtr-PCM_OUT_START)>>1;
				int len = PCM_OUT_END - gPcmReadPtr;
				if (len < MAX_PCK_LEN)
				{
					while(1);
				}
				check_dac_value(index1);
				return 1;
			}
			return 0;
		}
	}
	else
	{
		if (HREAD(mem_vp_enable) == VP_START)
			return 0;
		
		gPcmReadPtr = PCM_OUT_START;
		gDacIndex = 0;
		
		gDacSum = 0;
		gPreIndex = 0;
		gDacFlag = 0;
		return 0;
	}
}




