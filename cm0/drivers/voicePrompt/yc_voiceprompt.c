#include "yc11xx.h"
#include "yc_voiceprompt.h"
#include "yc_queue.h"
#include "yc_drv_common.h"
#include "yc11xx_qspi.h"
#include "drv_bt.h"
#include "fir_8to48.h"
#include "drv_debug.h"

#define   PCM_IN_BUFFER_ADDR1 0x11000
#define   PCM_OUT_BUFFER_ADDR1 0x11970
#define   CVSD_OUT_BUFFER_ADDR1  0x122e0
#define   CVSD_IN_BUFFER_ADDR1 0x127a0
#define   CVSD_IN_MAX_LEN1 1200

uint8_t gVP_type = 0;
extern short gDacIndex;

#define VP_EVT_QUEUE_MAX 14
VP_INDEX gQueueVpEvt[VP_EVT_QUEUE_MAX];
QUEUE_HEADER gVpEvtHeader;


#define VP_DAC_LENGTH 0x2000
#define VP_DAC_START_ADDR0 0x10011000
#define VP_DAC_START_ADDR1 0x10012000
VP_PARAMS gVpparams;

int16_t gVpPopOffset=6750;
//int16_t *  vp_array_ptr = (int16_t *)((uint32_t)VP_DAC_START_ADDR0) ;
int16_t * vp_array_ptr = (uint16_t *)(0x10011000);

extern short * gPcmout; 
extern short * gDacout;
#define DAC_SPI_BUFF_START 0x122e0
short * gpSPI  = (short *)(reg_map_m0(DAC_SPI_BUFF_START));
#define  VP_ELE_8K_NUM 8
#define VP_ELE_8K_LEN 300
QUEUE_HEADER gVp8KDataHeader;


void VP_init()
{
	VP_ADAC_reg_init();
	HWRITE(VP_ENABLE,VP_STOP);
	if (!queue_init(&gVpEvtHeader, (void *)&gQueueVpEvt, sizeof(VP_INDEX), VP_EVT_QUEUE_MAX))
	{
		error_handle();
	}

	if (!queue_init(&gVp8KDataHeader, (void *)gPcmout, VP_ELE_8K_LEN, VP_ELE_8K_NUM))
	{
		error_handle();
	}

	gVpPopOffset = HREADW(mem_dc_otp_data);
	if(gVpPopOffset <= 3000)
	{
	     gVpPopOffset =6750;
	}
	
         if((gVpPopOffset>6800) &&  (gVpPopOffset < 7300))
	{
	      gVpPopOffset = 6800;
	}
	
	if((gVpPopOffset>=7300) &&  (gVpPopOffset <7800) )
	{
	      gVpPopOffset = 7800;
	}
}

void VP_snd_voiceEvt(VP_INDEX ind)
{
	if (gBRState.topState < BR_POWER_ON && ind != VP_POPUP)
		return;
	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"VP_snd_voiceEvt: 0x%04X", LOG_POINT_A001, ind);
         Bt_RelasePaDelay();
	if (ind == VP_POWEROFF)
	{
		queue_clear(&gVpEvtHeader);
		if (HREAD(0x8980) == 0x85){
			VP_play_voice(VP_POWEROFF);
			ind = VP_POPUP;
			//return;
		}
		else{
			VP_play_voice(VP_POPDOWN);
		}
	}
	else if(ind != VP_POPUP && ind != VP_POPDOWN){
		if (HREAD(0x8980) != 0x85){
			VP_INDEX tempind;
			if (!queue_is_empty(&gVpEvtHeader)){
				tempind = *(VP_INDEX *)Header_Queue(&gVpEvtHeader);
			}
			else{
				tempind == 0xff;
			}
			if (tempind != VP_POPDOWN)
			{
				tempind =  VP_POPDOWN;
				if (!Insert_Queue(&gVpEvtHeader,(void *)&tempind))
				{
					//put queue is error
					//TO DO
					error_handle();
				}
			}
		}
	}
	
	if (!Insert_Queue(&gVpEvtHeader,(void *)&ind))
	{
		//put queue is error
		//TO DO
		error_handle();
	}	
}

void VP_play_voice(VP_INDEX ind)
{
	unsigned char temp;
	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"VP_play_voice: 0x%04X", LOG_POINT_A000, ind);
	
	if (ind >=VP_0 && ind <= VP_9)
	{
		if (!Bt_CheckHFPIsIncomming())
			return;
	}

	//todo close dac
	Lpm_LockLpm(VP_LPM_FLAG);
	HWRITE(VP_ENABLE,VP_START);
	HWRITE(AUDIO_ALLOW,0);

	HWRITE(mem_media_sbc_flag,0);
	HWRITE(CORE_SBC_CTRL,0x00);

         temp = HREAD(0x8106);
         temp &= ~1;
	//stop dac
	HWRITE(0x8106,temp);
	
	//stop adc
	HWRITE(0x803b,0x50);
	//stop cvsd
	HWRITE(0x80f5,0);

	queue_clear(&gVp8KDataHeader);
	gDacIndex = 0;
	
	if (ind != VP_POPDOWN && ind != VP_POPUP){
		gVpparams.sVpDataAddr = HR_REG_24BIT(reg_map(VP_ADDR_ARRAY+ind+ind+ind));
		gVpparams.sVpDataOffset =0;
		gVpparams.sMemBlock =0;
		
		//get vp data length
		QSPI_Init();
		OS_ENTER_CRITICAL();
		SetLockQSPI( );
		setRamType(MRAM);
		QSPI_ReadFlashData(gVpparams.sVpDataAddr,0x3,(uint8_t *)&gVpparams.sVpDataLen);
		SetReleaseQSPI( );
		OS_EXIT_CRITICAL();
		//increase 3 for length
		gVpparams.sVpDataAddr+=3;
	}
	
	//set 48K
	HWRITE(0x896c,0x2d);
	HWRITE(0x896e,0x94);
	temp=HREAD(0x894d);
	temp = temp|0x01;
	HWRITE(0x894d,temp);
	
	HWRITE(0x8978,0x80);
	HWRITE(0x8979,0xE0);
	HWRITE(0x897c,0x55);
	HWRITE(0x897d,0x08);
	HWRITE(0x8981,0x30);

	temp = HREAD(0x897e);
	temp = temp |0xF0;
	HWRITE(0x897e,temp);
	
	HWRITE(0x897f,0x1f);
	HWRITE(0x897a,0x61);
	
	//HWRITE(0x8106,0x2);
	//hw_delay();
	//HWRITE(0x8106,0x0);

	if (ind == VP_POPDOWN || ind == VP_POPUP){
		HWRITEW(0x8100, VP_DAC_START_ADDR0);
		HWRITEW(0x8104,VP_DAC_START_ADDR0);
		HWRITEW(0x8102, VP_DAC_LENGTH-1);
	}
	else{
		HWRITEW(0x8100, 0x13000);
		HWRITEW(0x8104,0x13000);
		HWRITEW(0x8102, 3599);
	}
	/*
	HWRITE(0x8106,0x18);
	hw_delay();
	HWRITE(0x8106,0x08);
	*/
	//volume
	HWRITE(0x8108,80);
	gVpparams.sStopNext = 1;
		
	if (ind == VP_POPDOWN){
		gVP_type = 5;
		HWRITE(0x8114,0x66);
		HWRITE(0x8115,0x15);
		HWRITE(0x8107,0x05);
		if (HREAD(0x8980) == 0x85)	
		     return;
		HWRITE(0x8106,0x02);

		for (int i = 0; i<0x1000;i++)
		{
			vp_array_ptr[i] = gVpPopOffset;
		}
		//update dac wptr
		HWRITEW(0x8104,0x3000-2);

		//start digital dac
		HWRITE(0x8106,0x03);
		//whileDelay(80);
		whileDelay(7);

		HWRITE(0x8980,0xc5);
		whileDelayshort(1);
		//whileDelay(2);
		HWRITE(0x8980,0x85);
		//whileDelay(40);
		whileDelay(10);

		int gvaluetemp;
		gvaluetemp = gVpPopOffset;
		for (int i = 0; i<(0x1000-2);i++)
		{
			if (gvaluetemp <= 0)
			{
				vp_array_ptr[i] = 0;
			}
			else
			{
				if (gVpPopOffset > 8000)
					gvaluetemp = gvaluetemp-3;
				else
					gvaluetemp = gvaluetemp-2;
				vp_array_ptr[i] = gvaluetemp;
			}	
		}
		//vp_array_ptr[4096] = gVpPopOffset;
		//vp_array_ptr[4095] = gVpPopOffset;
		//vp_array_ptr[4094] = gVpPopOffset;
		HWRITEW(0x8104,0x3000-4);
	}
	else if (ind == VP_POPUP){
		gVP_type = 6;
		HWRITE(0x8107,0x05);	
		HWRITE(0x8106,0x02);
		int gvaluetemp = 0;
		for (int i = 0; i<0x1000;i++)
		{
			if (gvaluetemp >=gVpPopOffset)
				vp_array_ptr[i] = gVpPopOffset;
			else{
				vp_array_ptr[i] = gvaluetemp;
				if (gVpPopOffset > 8000)
					gvaluetemp+=3;
				else
					gvaluetemp+=2;
			}
		}
		//update dac wptr
		HWRITEW(0x8104,0x3000-2);

		//start digital dac
		HWRITE(0x8106,0x03);
		whileDelay(20);
		HWRITE(0x897f,0x0e);
	         whileDelay(4);
		HWRITE(0x8980,0xff);
		whileDelay(4);
		
	}
	else{
		HWRITE(0x8107,0x05);
		//get data from flash
		VP_CheckGetData();
		HWRITE(0x8106,0x02);
		HWRITE(0x8106,0x03);
		HWRITE(0x8980,0x85);
	}

}

int metest = 0;
void VP_CheckGetData()
{	
	uint16_t wptr ;
	uint16_t rptr;
	uint16_t remaind;
	uint8_t temp;

	if (HREAD(VP_ENABLE) == VP_STOP)
		return;
		
	wptr = HREADW(0x8104);
	rptr = HREADW(0x836a);

	if (gVP_type == 5){
		if (wptr != rptr){
			return;
		}
		else{
			gVP_type = 0;
			gVpparams.sStopNext = 0;
			if (queue_is_empty(&gVpEvtHeader)){
				//dac stop
				Lpm_unLockLpm(VP_LPM_FLAG);
				HWRITE(VP_ENABLE,VP_STOP); 
				Bt_SndCmdRecoverAudio();
			}
			return;
		}
	}
	else if (gVP_type == 6){
		if (wptr != rptr){
			return;
		}
		else{
			gVP_type = 0;
			gVpparams.sStopNext = 0;
		         temp = HREAD(0x8106);
		         temp &= ~1;
			//stop dac
			HWRITE(0x8106,0);
			//stop adc
			HWRITE(0x803b,0x50);
			//stop cvsd
			HWRITE(0x80f5,0);

			HWRITE(0x8980,0xff);
			HWRITE(0x897f,0x30);
			HWRITE(0x897e,0);
			HWRITE(0x897a,0x7f);
			//dac stop
			Lpm_unLockLpm(VP_LPM_FLAG);
			HWRITE(VP_ENABLE,VP_STOP); 
			return;
	
		}
	}
	
	uint32_t addr;
	
	while(1){
		if ( gVpparams.sVpDataLen - gVpparams.sVpDataOffset < 300)
			break;
		else if (!queue_is_full(&gVp8KDataHeader)){
			if (preinsert_Queue(&gVp8KDataHeader, (void **)(&addr))){
				metest = addr;
				QSPI_Init();
				OS_ENTER_CRITICAL();
				SetLockQSPI( );
				setRamType(MRAM);
				QSPI_ReadFlashData(gVpparams.sVpDataAddr+gVpparams.sVpDataOffset
					,300,(uint8_t *)addr);
				SetReleaseQSPI( );
				OS_EXIT_CRITICAL();
				gVpparams.sVpDataOffset+=300;
			}
			else{
				//put queue is error
				//TO DO
				error_handle();
			}
		}
		else
			break;
		
	}	

	wptr = HREADW(0x8104);
	rptr = HREADW(0x836a);
	if (wptr>rptr)
	{
		remaind = wptr-rptr;
	}
	else if(rptr > wptr)
	{
		remaind = rptr-wptr;
		remaind = 3600-remaind;
	}
	else 
		remaind = 0x700;

	if (remaind > 0x700)
		return;
	
	if ( gVpparams.sVpDataLen - gVpparams.sVpDataOffset < 300)
	{
		if (queue_is_empty(&gVp8KDataHeader)){
			gVpparams.sStopNext = 0;
			if (queue_is_empty(&gVpEvtHeader))
			{
				//dac stop
				Lpm_unLockLpm(VP_LPM_FLAG);
				HWRITE(VP_ENABLE,VP_STOP); 
				
				extern uint8_t *gpHFPCallState;
		                  if (*gpHFPCallState == HFP_INCOMMING)
			         {
			                 return;
			         }
				if (gBRState.secondState < BR_CONNECTED)
				{
				        return;
			         }
			   
			        Bt_SndCmdRecoverAudio();
				 return;
			}
			else
				return;
		}
	}
	
	short * dacBuf  = (short *)Delete_Queue(&gVp8KDataHeader);
	for(int i=0; i < 150; i++)
	{
		insert_48K(dacBuf[i]);
	}
	if (gDacIndex >= 1800 )
		gDacIndex = 0;
	HWRITEW(CORE_DAC_WPTR,0x13000+(gDacIndex<<1));

}


void VP_polling_Queue()
{
	VP_INDEX *evt_ele;

	VP_CheckGetData();

	if (gVpparams.sStopNext)
		return;
	
	if (!queue_is_empty(&gVpEvtHeader))
	{
		evt_ele = (VP_INDEX *)Delete_Queue(&gVpEvtHeader);
		VP_play_voice(*evt_ele);
	}
}


bool VP_Check_Queue_Is_empty()
{
	return queue_is_empty(&gVpEvtHeader);
}

bool VP_Dele_Queue()
{
	while(!queue_is_empty(&gVpEvtHeader))
	{
		 (VP_INDEX *)Delete_Queue(&gVpEvtHeader);
	}
}

void VP_ADAC_reg_init(){
	HWRITE(0x8978,0x80);
	HWRITE(0x897d,0x08);
	HWRITE(0x897a,0x7f);
	HWRITE(0x897b,0xbf);
}



void VP_cvsd_init()
{
	HWRITE(CORE_CVSD_CTRL,0x00);
	
	HWRITEW(CORE_PCMIN_SADDR, PCM_IN_BUFFER_ADDR1);//input,pcm sour

	HWRITEW(CORE_CVSDOUT_SADDR, CVSD_OUT_BUFFER_ADDR1); //output,cvsd dest
	HWRITEW(mem_cvsd_out_addr_ptr,CVSD_OUT_BUFFER_ADDR1);
	HWRITEW(mem_cvsd_out_addr_end,(CVSD_OUT_BUFFER_ADDR1+CVSD_IN_MAX_LEN1));

	HWRITEW(CORE_CVSDIN_SADDR, CVSD_IN_BUFFER_ADDR1); //in,cvsd source
	HWRITEW(mem_cvsd_in_addr_ptr,CVSD_IN_BUFFER_ADDR1);
	HWRITEW(mem_cvsd_in_addr_end,(CVSD_IN_BUFFER_ADDR1+CVSD_IN_MAX_LEN1));
	
	HWRITEW(CORE_PCMOUT_SADDR,PCM_OUT_BUFFER_ADDR1);//output,pcm dest
	HWRITEW(CORE_CVSD_LEN, (CVSD_IN_MAX_LEN1-1)); //len
}

