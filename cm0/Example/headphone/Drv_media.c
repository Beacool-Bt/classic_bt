#include <stdbool.h>
#include "yc11xx.h"
#include "drv_media.h"
#include "yc_voiceprompt.h"
#include "drv_debug.h"

//#define FILTER_BYPASS

uint8_t gdacflag;
uint16_t gFullqueue;

//test
uint8_t gSBCErrCount;
uint16_t gEmptyCount,gEmptyFlag;
uint16_t gLossMedia;
TestEnum gTttt;

#define	A2DP_FILTER_DELTA_UK_DEFAULT_VALUE    0
#define	A2DP_FILTER_COMPENSATION_FREQ_VALUE    0x05


const int32_t acc_uk = 963379;
int32_t delta_uk = A2DP_FILTER_DELTA_UK_DEFAULT_VALUE;
const int16_t CoefB0[4] = {0,0,2,0};
const int16_t CoefB1[4] = {-1,3,-1,-1};
const int16_t CoefB2[4] = {1,-1,-1,1};

#define DAC_BUFFER_LEN (A2DP_DAC_BUFFER_SIZE>>1) //dac buffer³¤¶È
#define DAC_START_ADDR (reg_map_m0(HF_DAC_START_ADDRESS1))
#define FILTER_STATE_LEN 4
int16_t *filter_out = (int16_t *)DAC_START_ADDR ;
volatile int32_t *sbcSrc=(int32_t *)(reg_map_m0(SBC_DECODE_BUFFER_START));
int16_t src_state[FILTER_STATE_LEN] = {0};
int32_t uk_tmp = 0;
int32_t cnt = 0;
uint8_t cntAdjust = 0;
uint8_t gIsDropInProcess = 0;

void FarrowFilter(int16_t din_point);

void FarrowFilterInit()
{
	for(int i=0; i < FILTER_STATE_LEN; i++)
		src_state[i] = 0;
	uk_tmp = 0;
	cnt=0;
	cntAdjust = 0;
	gIsDropInProcess = TWS_DROP_PROCESS_STATE_NONE;
	delta_uk = A2DP_FILTER_DELTA_UK_DEFAULT_VALUE;
}


bool Drv_checkSBCBusy()
{
	uint16_t addr1 = HREADW(CORE_SBC_SWP);
	uint16_t addr2 = HREADW(CORE_SBC_SRP);
	if (addr1 != addr2)
		return true;
	return false;
}


void Drv_checkDacEmpty()
{
	uint16_t wptr,rptr,len;

	if (0 == HREAD(mem_audio_allow))
		return;

	if (0 != HREAD(mem_media_cur_len))
		return;

	//if (HREAD(mem_media_start) == 1)
	//	return;
	
	wptr = HREADW(CORE_DAC_WPTR);
	rptr = HREADW(CORE_DAC_RPTR);

	if (wptr == rptr){
		HWRITE(mem_media_start,1);
	}

}

//sbc_hw_check_err
void Drv_checkSBCError()
{
	uint8_t temp;
	temp = HREAD(CORE_SBC_STATUS);
	if (0 == (temp & 0xc0))
		return;
	HWRITE(CORE_SBC_CLR,0x20);
	temp = 5;
	while(temp--);
	HWRITE(CORE_SBC_CLR,0);
	HWRITE(CORE_SBC_CTRL,0x08);
	HWRITE(mem_media_sbc_flag,0);
	gSBCErrCount++;
	if (Drv_checkSBCBusy())
	{
		//branch hp_stero_init
		 if (0== HREAD(mem_init_a2dp_dac))
			HWRITE(mem_init_a2dp_dac,1);
	}	
}


bool Drv_checkDacBufferEnough(uint16_t length)
{
	uint16_t wptr,rptr,len;

	//wptr = HREADW(CORE_DAC_WPTR);
	wptr = DAC_START_ADDR + (cnt<<1);
	rptr = HREADW(CORE_DAC_RPTR);
	len = HREADW(CORE_DAC_LEN);

	if (wptr == rptr)
	{
		if (gEmptyFlag == 0){
			gEmptyCount++;
			gEmptyFlag=1;
			//gdacflag = 2;
		}
	}
	
	if (wptr - rptr > 0){
		gEmptyFlag=0;
		if  ((len - (wptr - rptr)) > length)
		{
			return true;
		}
		return false;
	}
	else if (wptr - rptr < 0){
		gEmptyFlag=0;
		if ((rptr - wptr) > length)
		{
			return true;
		}
		return false;
	}else{
		return true;
	}
}

void Drv_drop_media_packet(void)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchLedFunc: 0x%04X", LOG_POINT_B034, HREADW(mem_target_start_seq));
	gLossMedia++;
	HWRITE(mem_media_release,1);
	FarrowFilterInit();//reset filter
	return;
}

#define TWS_SYNC_INVALID_SEQ_DIFF 20


void Drv_tws_media_start_sync(void)
{
	//Wait clock ok
	if(HREAD(mem_enable_start_sync) != 0)
	{
		if(HREAD(mem_media_wait_clk) != 0)
		{
	
			while(HREAD(mem_media_wait_clk) != 0)
			{
				
			}

			HWRITEW(CORE_DAC_WPTR, DAC_START_ADDR + (cnt<<1));
			DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchLedFunc: 0x%04X", LOG_POINT_B033, (DAC_START_ADDR + (cnt<<1)));
		}
	}
}
uint16_t gtest1;
void Drv_media_polling()
{
	uint16_t temp,len;
	uint16_t addr,addr1;
	MEDIA_ELEM *element;
	uint16_t seq;
	uint16_t targetSeq;
	

	//if (HREAD(mem_vp_enable) == VP_START)
	//	return;

	Drv_checkSBCError();
	Drv_checkDacEmpty();

	if (0 == HREAD(mem_media_sbc_flag)){
		if (1 == HREAD(mem_media_release))
			return;

		temp = HREAD(mem_media_cur_len);
		
		if (temp>=3){
			if(temp >= 4)
			{
				HWRITE(mem_media_start,0);
				if (++gFullqueue > 50){
					gFullqueue=0;
					gLossMedia++;
					if (0x19 == HREAD(mem_tws_state) ||0x69 == HREAD(mem_tws_state))
					{
						if (gIsDropInProcess == TWS_DROP_PROCESS_STATE_NONE
							&& (0x19 == HREAD(mem_tws_state)))
						{
							gIsDropInProcess = TWS_DROP_PROCESS_STATE_INPROCESS;
							Bt_SndCmdTwsDropOnePacket();
						}
					}
					else
					{
						HWRITE(mem_media_release,1);
						DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Drv_media_polling drop more packet: 0x%04X", LOG_POINT_B036, seq);
						return;
					}
				}
			}
		}
		else if (temp > 0)
		{
			gFullqueue = 0;
			//if ( 1 == HREAD(mem_media_start))
			//	return;
		}
		else
		{
			return;
		}

		//drop one packet
		if(gIsDropInProcess == TWS_DROP_PROCESS_STATE_WAIT_DROP)
		{
			gIsDropInProcess = TWS_DROP_PROCESS_STATE_NONE;
			gLossMedia++;
			HWRITE(mem_media_release,1);
			DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Drv_media_polling drop more packet: 0x%04X", LOG_POINT_B036, seq);
			return;
		}
		
		//change uk
		if(temp >= 4)
		{
			delta_uk = 10;//be quickly
		}
		else if(temp >= 3)
		{
			delta_uk = 5;//be quickly
		}
		else if(temp <= 1)
		{
			delta_uk = -1;//slow down
		}
		else
		{
			delta_uk = 0;
		}
		
		//start decode new packet
		temp = HREADW(mem_media_rptr);
		//get packet start addr
		addr1= HREADW(temp);
		
		//get seq
		addr1=addr1+6;
		seq = HREAD(addr1 + 1) | (HREAD(addr1) << 8);
		HWRITEW(mem_a2dp_seq_num, seq);

		DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchLedFunc: 0x%04X", LOG_POINT_B030, seq);
		

		if (0 == HREAD(mem_audio_allow) || 0 != HREAD(mem_listen_master_timer)){
			gLossMedia++;
			HWRITE(mem_media_release,1);
			return;
		}

		if (1 == HREAD(mem_media_lock))
			return;

		//Start Sync
		if(HREAD(mem_enable_start_sync) != 0)
		{
			targetSeq = HREADW(mem_target_start_seq);
			temp = HREADW(mem_a2dp_seq_num);
			if(targetSeq > temp) 
			{
				temp = targetSeq - temp;//diff
				if(temp > TWS_SYNC_INVALID_SEQ_DIFF)// Check overflow
				{
					
				}
				else
				{
					Drv_drop_media_packet();
					return;
				}
			}
			else
			{
				temp = temp - targetSeq;//diff
				if(temp > TWS_SYNC_INVALID_SEQ_DIFF)// Check overflow
				{
					Drv_drop_media_packet();
					return;
				}
			}
			
		}

		if(HREAD(mem_first_buffer_cache) == 0x53)
		{
			DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchLedFunc: 0x%04X", LOG_POINT_B032, HREAD(mem_media_cur_len));
			FarrowFilterInit();//reset filter
			if(HREAD(mem_enable_start_sync) == 0)
			{
				// Step1: cache buffer in dac buffer

				// Step2: cache buffer in media buffer
				if(HREAD(mem_media_cur_len) < 4)
				{
					return;
				}
			}

			HWRITE(mem_first_buffer_cache, 0);
		}
		
		//start decode new packet
		temp = HREADW(mem_media_rptr);
		//get packet start addr
		addr1= addr = HREADW(temp);
		//addr1 = addr;
		//get packet length
		len = HREADW(addr);

		//check dac buffer is enough,6.5
		//if (Drv_checkDacBufferEnough(len*8)){
		{
			//get seq
			addr1=addr1+6;
			gTttt.test1 = HREADW(addr1);

			addr = addr+4+12;
			len = len -12;
			do{
				len--;
			}while(HREAD(addr++) == 0);				
			
			if (HREAD(addr) != 0x9c)
			{
				HWRITE(mem_media_release,1);
				while(1);
				return;
			}
			//gMediaSBCFlag = 1;	
			//HWRITE(0x8099,63);
			HWRITE(mem_media_sbc_flag,1);
			HWRITE(CORE_SBC_CTRL,0x08);

			HWRITEW(CORE_SBC_SWP,len);

			HWRITEW(CORE_SBC_SADDR,addr);

			HWRITEW(CORE_SBC_SLEN,0x400);
			HWRITE(CORE_SBC_CLR,6);
			temp = 5;
			while(temp--);
			HWRITE(CORE_SBC_CLR,0);
			HWRITE(CORE_SBC_CTRL,0x09);
			temp = HREAD(CORE_SBC_STATUS);
			if (0 == (temp & 0xc0))
				return;
			HWRITE(CORE_SBC_CLR,0x20);
			temp = 5;
			while(temp--);
			HWRITE(CORE_SBC_CLR,0);
			HWRITE(CORE_SBC_CTRL,0x08);
			//HWRITE(mem_media_sbc_flag,0);
			
		}
	}
	else{
		if (!Drv_checkSBCBusy()){
			//update wptr
			//sbc_update_dac_wptr
			gtest1 = addr = HREADW(CORE_SBC_DWP);

			Drv_tws_media_start_sync();
			
			if(HREAD(mem_enable_start_sync) != 0
				&& (cnt != 0))
			{
				return;
			}
			
			if(HREAD(mem_enable_start_sync) == 0)
			{
				HWRITEW(CORE_DAC_WPTR, DAC_START_ADDR + (cnt<<1));
			}
			int subLenth = (addr >>1); // becouse we will use limit buffer
			if (!Drv_checkDacBufferEnough(subLenth+(subLenth/10)))//becouse 48/44.1=1.088, use 1.1
			{
				return;
			}
				
			//gMediaSBCFlag = 0;
			//HWRITE(0x8099,62);
			HWRITE(mem_media_sbc_flag,0);
			
			if (addr&0x03!= 0)
				while(1);

			if (gdacflag != 0){
				gdacflag--;
				if (gdacflag !=0)
					return;
			}

			//HWRITE(0x8097,63);
			for (int i =0; i < (addr>>2); i++)
			{
				element = (MEDIA_ELEM *)&sbcSrc[i];
				//sample rate = 48K
				if (0x01== (0x01 & HREAD(mem_conf_sample_freq)))
				{
					if (0x19 == HREAD(mem_tws_state) ||0x69 == HREAD(mem_tws_state))
					{
						if (HSP_L == HREAD(mem_hsp_role))
							filter_out[cnt++] = (element->lData);
						else
							filter_out[cnt++] = element->rData;
					}
					else{
						filter_out[cnt++] = (element->lData+ element->rData)>>1;
					}
					
					if (cnt > DAC_BUFFER_LEN -1)
						cnt=0;
				}
				else{ //sample rate = 44.1K
					if (0x19 == HREAD(mem_tws_state) || 0x69 == HREAD(mem_tws_state))
					{
						if (HSP_L == HREAD(mem_hsp_role))
							FarrowFilter( element->lData);
						else
							FarrowFilter(element->rData);
					}
					else{
						FarrowFilter((element->lData+element->rData)>>1);
					}
				}
			
			}
			//HWRITE(0x8097,62);
			
			DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_SearchLedFunc: 0x%04X", LOG_POINT_B035, HREADW(mem_a2dp_seq_num));

			//release queue
			HWRITE(mem_media_release,1);
			
			if(HREAD(mem_enable_start_sync) != 0)
			{
				return;
			}
			HWRITEW(CORE_DAC_WPTR, DAC_START_ADDR + (cnt<<1));
			//HWRITE(0x8098,63);

			temp = HREAD(mem_dac_loss);
			if (0 != temp)
			{
				HWRITE(mem_dac_loss,--temp);
				if (temp != 0)
					return;
			}
							
			//update volume
			addr1= HREAD(CORE_DAC_LVOL);
			addr =HREAD(mem_audio_vol);
			if (addr1 < addr){
				addr1++;

			}
			else if (addr1 > addr){
				addr1--;
			}

			HWRITE(CORE_DAC_LVOL,addr1);
			HWRITE(CORE_DAC_RVOL,addr1);
			
		}
	}
}



void FarrowFilter(int16_t din_point)
{
    int32_t interp_out;
    int32_t uk=0;
    int64_t temp0,temp1,temp2;
    int32_t uk_alter_tmp;

    uk_tmp = uk_tmp + acc_uk + delta_uk 
    	+ ((cntAdjust == 0)?1:0);// + (((cnt&0x10) == 0x10)?1:0);//0.2
    if(uk_tmp >= 1<<20)
        uk_tmp = uk_tmp - (1<<20);
    
    src_state[0] = din_point;

    temp0 = src_state[2]*2;
    temp1 = - src_state[0] + src_state[1]*3 - src_state[2] - src_state[3];
    temp2 = src_state[0] - src_state[1] - src_state[2] + src_state[3];

    for (int i=2;i>-1;i--)
	    src_state[i+1] = src_state[i];

    temp0 = temp0>>4;
    temp1 = temp1>>4;
    temp2 = temp2>>4;

    uk = uk_tmp>>12;

    interp_out = temp2*uk;
    interp_out = interp_out>>8;

    interp_out = (interp_out+temp1)*uk;
    interp_out = interp_out>>8;

    interp_out = interp_out+temp0;

    if(interp_out > 8191)
        interp_out = 8191;
    else if(interp_out < -8191)
        interp_out = -8191;

    filter_out[cnt++] = interp_out<<2;
    if (cnt > DAC_BUFFER_LEN-1)
    	cnt=0;
    cntAdjust++;
    if(cntAdjust >= A2DP_FILTER_COMPENSATION_FREQ_VALUE)
    {
         cntAdjust = 0;
    }
    uk_alter_tmp = uk_tmp + acc_uk + delta_uk 
    	+ ((cntAdjust == 0)?1:0);// + (((cnt&0x10) == 0x10)?1:0);// + aaa;// + (cnt%5 == 0?1:0);

    if(uk_alter_tmp < 1<<20) {

        uk = uk_alter_tmp>>12;

        interp_out = temp2*uk;
        interp_out = interp_out>>8;

        interp_out = (interp_out+temp1)*uk;
        interp_out = interp_out>>8;

        interp_out = interp_out+temp0;

        if(interp_out > 8191) interp_out = 8191;
        else if(interp_out < -8191) interp_out = -8191;

        filter_out[cnt++] = interp_out<<2;
        if (cnt > DAC_BUFFER_LEN-1)
        	cnt=0;

        cntAdjust++;
        if(cntAdjust >= A2DP_FILTER_COMPENSATION_FREQ_VALUE)
        {
             cntAdjust = 0;
        }

        uk_tmp = uk_alter_tmp;
    }
}



