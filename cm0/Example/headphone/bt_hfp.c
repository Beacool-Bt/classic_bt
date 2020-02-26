#include <stdbool.h>
#include "bt_hfp.h"
#include "yc11xx.h"
#include "drv_bt.h"
#include "yc_voiceprompt.h"
#include "ipc.h"
#include <string.h>
#include "drv_debug.h"

uint8_t testCmd[]=
{
 0x0D ,0x0A ,0x2B ,0x42 ,0x52 ,0x53 ,0x46 ,0x3A ,0x20 ,0x39 ,0x39 ,0x0D 
,0x0A ,0x0D ,0x0A ,0x4F ,0x4B ,0x0D
/**
 0x0D,0x0A,0x2B,0x43,0x49,0x4E,0x44,0x3A,0x20,0x28,0x22,0x73,0x65,0x72,0x76,0x69,0x63
,0x65,0x22,0x2C,0x28,0x30,0x2D,0x31,0x29,0x29,0x2C,0x28,0x22,0x63,0x61,0x6C,0x6C,0x22
,0x2C,0x28,0x30,0x2D,0x31,0x29,0x29,0x2C,0x28,0x22,0x63,0x61,0x6C,0x6C,0x73,0x65,0x74
,0x75,0x70,0x22,0x2C,0x28,0x30,0x2D,0x33,0x29,0x29,0x2C,0x28,0x22,0x63,0x61,0x6C,0x6C
,0x68,0x65,0x6C,0x64,0x22,0x2C,0x28,0x30,0x2D,0x32,0x29,0x29,0x2C,0x28,0x22,0x73,0x69
,0x67,0x6E,0x61,0x6C,0x22,0x2C,0x28,0x30,0x2D,0x35,0x29,0x29,0x2C,0x28,0x22,0x72,0x6F
,0x61,0x6D,0x22,0x2C,0x28,0x30,0x2D,0x31,0x29,0x29,0x2C,0x28,0x22,0x62,0x61,0x74,0x74
,0x63,0x68,0x67,0x22,0x2C,0x28,0x30,0x2D,0x35,0x29,0x29,0x0D,0x0A,0x0D,0x0A,0x4F,0x4B
,0x0D
**/
};

uint8_t  const  HFP_Indicators[HFP_INDICATOR_ARRAY_SIZE][HFP_INDICATOR_MAX_LENGTH]=
{
	{"service\""},
	{"call\""},
	{"callsetup\""},
	{"callheld\""},
	{"signal\""},
	{"roam\""},
	{"battchg\""}
};

uint8_t  const HFP_result_code[HFP_RESULT_CODE_ARRAY_SIZE][HFP_RESULT_CODE_MAX_LENGTH] = 
{
	{"RING"},			//0
	{"+BRSF"},
	{"+CIND"},
	{"+CHLD"},
	{"+VGM"},
	{"+VGS"},			//5
	{"OK"},	
	{"ERROR"},
	{"+CIEV"},
	{"+BVRA"},
	{"+COPS"},			//10
	{"+BTRH"},
	{"+BSIR"},
	{"+CNUM"},
	{"+CME"},
	{"+CLCC"},			//15
	{"+CLIP"},
	{"+CCWA"},
	{"+BINP"},
	{"+XAPL"},
};

void HFP_test()
{
	HFP_process_rfcomm_packet(sizeof(testCmd),testCmd);
}

bool AT_CompareString (uint8_t * strPtr, uint8_t const  *c_ptr, uint16_t length)
{
	while (length-- && *c_ptr != 0x00)
	{
		switch (*strPtr)
		{
			case '_':
			case '-':
				strPtr++;
				break;

			default:
				if (*strPtr != *c_ptr) 
				{
				 	return false;
				}
				else
				{
				   	strPtr++;
					c_ptr++;	
				}
				break;
		}
	}

	return true;
}

uint8_t AT_FindMatchCode (uint8_t * ptr1)
{
	int i;
	for (i = 0; i < HFP_RESULT_CODE_ARRAY_SIZE; i++)
	{
		if (AT_CompareString (ptr1, &HFP_result_code[i][0], HFP_RESULT_CODE_MAX_LENGTH))
		{
			return i;
		}		
	}
	return AT_EOF;
}

void HFP_check_battery_state()
{
	switch(HREAD(mem_hfp_battery_state))
	{
		case 2:
			HWRITE(mem_hfp_battery_state,5);
			break;
		case 4:
			HWRITE(mem_hfp_battery_state,5);
			break;
		case 6:
			HWRITE(mem_hfp_battery_state,7);
			break;
		case 8:
			HWRITE(mem_hfp_battery_state,9);
			break;
		case 10:
			HWRITE(mem_hfp_battery_state,12);
			break;
		case 13:
			HWRITE(mem_hfp_battery_state,14);
			break;
		case 15:
			HWRITE(mem_hfp_battery_state,0);
			break;
		
	}
}

void HFP_handle_result_ok()
{
	switch(HREAD(mem_hf_command))
	{
		case 1:
		case 2:
		case 3:
		case 4:
			HWRITE(mem_hf_command,0);
			break;
	}

	switch (HREAD(mem_hf_state))
	{
		case 0x08: //HF_STATE_WAITING_AG_UPDATE_STATUS_OK
			HWRITE(mem_hf_state,0x0a);
			break;
		case 0x0b: //HF_STATE_WAITING_AG_INFO
			HWRITE(mem_hf_state,0x0c);
			break;
		case 0x0d: //HF_STATE_WAITING_AT_CLIP
			HWRITE(mem_hf_state,0x0e);
			break;
		case 0x0f: //HF_STATE_WAITING_AT_CCWA
			HWRITE(mem_hf_state,0x09);
			HWRITE(mem_hfp_battery_state,0x01);
			IPC_TxControlCmd(BT_CMD_RECONNECT_AVDTP);	
			break;
		case 0x02: //HF_STATE_WAITING_AG_FEATURE_OK
			HWRITE(mem_hf_state,0x03);
			break;
		case 0x04: //HF_STATE_WAITING_AG_INDICATORS_OK
			HWRITE(mem_hf_state,0x05);
			break;
		case 0x06: //HF_STATE_WAITING_AG_STATUS_OK
			HWRITE(mem_hf_state,0x07);
			break;
		case 0x09:
			HFP_check_battery_state();
			break;
	}
}

uint8_t *str1;

 int32_t HFP_translate_ascii_to_hex()
 {
	int i=0,j;
	int oct=0,mul=1;
	
	while(1){
		if (str1[i] == 0x0d || str1[i] == 0x2c)
			break;
		i++;
	}

	j = i;
	
	while(i--){
		if (str1[i] == 0x3d || str1[i] == 0x3a || str1[i] == 0x2c)
			break;

		if (str1[i] == 0x20)
			continue;
		
		oct = (str1[i]-0x30)*mul+oct;
		mul=mul*10;
	}
	
	str1=str1+j+1;
	return oct;
 }


void HFP_handle_result_ciev(uint8_t *ptr1)
{
	int temp,temp1;
	
	str1 = ptr1;
	temp = HFP_translate_ascii_to_hex();
	
	temp1 = HREAD(mem_AG_indicator_map+temp-1);
	temp = HFP_translate_ascii_to_hex();
	HWRITE(mem_AG_indicator+temp1,temp);
	{
	      temp1 = HREAD(mem_AG_indicator+1);
	      if(temp1 ==1)    //CIEV_CALL_ACT
	      {
	            temp1 = HREAD(mem_AG_indicator+3);
	            if(temp1 == 0)  //CIEV_HELD_NO
	            {
	                  temp1 = HREAD(mem_AG_indicator+2);
	                  if(temp1 == 0) //CIEV_CALLST_NO
	                  {
	                      if (HREAD(VP_ENABLE) == VP_START)
	                       {
	                  
	                             temp = HREAD(0x8106);
                                      temp &= ~1;
	                             //stop dac
	                            HWRITE(0x8106,temp);
	                            extern VP_PARAMS gVpparams;
	                            memset(&gVpparams, 0, sizeof(VP_PARAMS));
			          HWRITE(VP_ENABLE,VP_STOP); 
			          VP_Dele_Queue();
	                       }
	                      Bt_SndCmdRecoverAudio();
	                  }
	            }
	      }
	      Bt_SndCmdUpdateHFPState();
	}
}


void HFP_handle_result_brsf(uint8_t *ptr1)
{
	uint16_t i;
	str1 = ptr1;
	i = (uint16_t)HFP_translate_ascii_to_hex();
	HWRITEW(mem_agoption,i);
}

void HFP_handle_result_vgs(uint8_t *ptr1)
{
	uint16_t i;
	str1 = ptr1;
	i = (uint16_t)HFP_translate_ascii_to_hex();
	HWRITE(mem_hf_vlm_speaker,i);
}

void HFP_handle_result_vgm(uint8_t *ptr1)
{
	uint16_t i;
	str1 = ptr1;
	i = (uint16_t)HFP_translate_ascii_to_hex();
	HWRITE(mem_hf_vlm_mic,i);
}

void HFP_handle_result_cind(uint8_t *ptr1)
{
	
	while(*ptr1 != 0x3a){
		ptr1++;
	}
	
	//ptr1=ptr1+2;
	ptr1++;
	while(*ptr1 == 0x20)
		ptr1++;

	if (*ptr1 == 0x28)
	{
		for (int i = 0; i < 7;i++)
		{
			//start ''
			while(*ptr1 != 0x22)
			{
				ptr1++;
			}
			ptr1++;
			
			for (int j = 0; j < HFP_INDICATOR_ARRAY_SIZE; j++)
			{
				if (AT_CompareString (ptr1, &HFP_Indicators[j][0], HFP_INDICATOR_MAX_LENGTH))
				{
					HWRITE(mem_AG_indicator_map+i, j);
					break;
				}		
			}

			//end '
			while(*ptr1 != 0x22)
			{
				ptr1++;
			}
			ptr1++;
		}
	}
	else
	{
		int temp,temp2;
		str1 = ptr1;
		
		for (int i = 0; i < 7; i++)
		{
			temp = (uint16_t)HFP_translate_ascii_to_hex();
			temp2 = HREAD(mem_AG_indicator_map+i);
			HWRITE(mem_AG_indicator+temp2,temp);
		}
		Bt_SndCmdUpdateHFPState();
	}
}

void HFP_get_phone_num(uint8_t *ptr1)
{
	int i=0,j=0;
	
	while(0x22 != *ptr1)
	{
		++ptr1;
	}

	while(*(++ptr1) != 0x22)
	{
		j = *ptr1 + 0x30;
		HWRITE(mem_phone_number+(i++),j);
	}
	HWRITE(mem_phone_num_len,i);
}

void HPF_handle_result_code(HFP_RESULT_CODE_IND ind,uint8_t *ptr1)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"HPF_handle_result_code: 0x%04X", LOG_POINT_9005, ind);
	switch(ind){
		case HFP_RESULT_CODE_RING:
			//VP_snd_voiceEvt(VP_RING);
			break;
		case HFP_RESULT_CODE_PLUS_BRSF:
			HFP_handle_result_brsf(ptr1);
			break;

		case HFP_RESULT_CODE_PLUS_CIND:	
			HFP_handle_result_cind(ptr1);
			break;
		case HFP_RESULT_CODE_PLUS_CHLD:
			break;
		case HFP_RESULT_CODE_PLUS_VGM:
			HFP_handle_result_vgm(ptr1);
			break;

		case HFP_RESULT_CODE_PLUS_VGS:
			HFP_handle_result_vgs(ptr1);
			break;
		
		case HFP_RESULT_CODE_OK:	
			HFP_handle_result_ok();
			break;
		case HFP_RESULT_CODE_ERROR:	
		case HFP_RESULT_CODE_PLUS_XAPL:	
			 if (0x09== HREAD(mem_hf_state))
			 {
				HFP_check_battery_state();
			 }
			break;
		case HFP_RESULT_CODE_CIEV:	
			HFP_handle_result_ciev(ptr1);
			break;
		case HFP_RESULT_CODE_PLUS_BVRA:
			//get recognition status;
			break;
		case HFP_RESULT_CODE_PLUS_COPS:		
		case HFP_RESULT_CODE_PLUS_BTRH:					
		case HFP_RESULT_CODE_PLUS_BSIR:	
		case HFP_RESULT_CODE_PLUS_CNUM:	
		case HFP_RESULT_CODE_PLUS_CME:	
		case HFP_RESULT_CODE_PLUS_CLCC:	
			break;
		case HFP_RESULT_CODE_PLUS_CLIP:	
		case HFP_RESULT_CODE_PLUS_CCWA:	
			HFP_get_phone_num(ptr1);
			break;
		case HFP_RESULT_CODE_PLUS_BINP:
		case HFP_RESULT_CODE_PLUS_BCS:
			break;
		default:
			break;
	}
}

void HFP_process_rfcomm_packet(uint16_t len,uint8_t *pck)
{
	int i;
	HFP_RESULT_CODE_IND ind;
	
	//start
	for (i = 0; i<len;i++)
	{
		if (pck[i] == AT_CR && pck[i+1] == AT_LF)
			break;		
	}
	if (i == len) {
		return;
	}
	
	ind = AT_FindMatchCode(&pck[i+2]);
	HPF_handle_result_code(ind,&pck[i+2]);

	//end
	for (i += 2; i < len;i++)
	{
		if (pck[i] == AT_CR && pck[i+1] == AT_LF)
			break;		
	}
	if (i == len) {
		return;
	}

	if (len - (i+2) >=4)
		HFP_process_rfcomm_packet(len-(i+2),&pck[i+2]);

}

void Bt_RFCOMMCallBack(uint8_t len,uint8_t *dataPtr)
{
	if (*dataPtr == 0xba)
		Bt_SPPCallBack(len-1,dataPtr+1);
	else
		HFP_process_rfcomm_packet(len,dataPtr);
}

void Bt_ReportBattery()
{
	if (0x09 == HREAD(mem_hf_state)){
		HWRITE(mem_hfp_battery_state,12); // HF_STATE_WAITING_ANDROID	
	}
	
}

