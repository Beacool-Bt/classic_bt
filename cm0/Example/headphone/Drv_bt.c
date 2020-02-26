#include <stdio.h>
#include "Drv_bt.h"
#include "ipc.h"
#include "yc_RemoteCtr.h"
#include "yc_timer.h"
#include "yc_keypad.h"
#include "yc11xx.h"
#include "yc_led.h"
#include "yc11xx_gpio.h"
#include "yc11xx_qspi.h"
#include "yc_voiceprompt.h"
#include "yc_battery.h"
#include "fir_8to48.h"
#include "fir_8to48.h"
#include "bt_hfp.h"
#include "drv_debug.h"
#include "drv_media.h"

#define DEFAULT_DISCOVERY_WORK_TIMEOUT 250

//#define SYNC_POWER_OFF
SYS_TIMER_TYPE gSwithMasterTimer;
BR_STATE gBRState;
BLE_STATE gBLEState;
SYS_TIMER_TYPE gReconTimer;
SYS_TIMER_TYPE gCONLedTimer;
SYS_TIMER_TYPE gSniffMulTimer;
SYS_TIMER_TYPE gEnterSniffTimer;
SYS_TIMER_TYPE gMAXPowerTimer;
SYS_TIMER_TYPE gOTATimer;
//SYS_TIMER_TYPE gResetTimer;
SYS_TIMER_TYPE gClosePaDelayTimer;
SYS_TIMER_TYPE gTWSWaitSlaveTimer;
SYS_TIMER_TYPE gBtReconWorkTimer;
#define SNIFF_MUL_TIMEOUT 40
#define SNIFF_ENTER_DELAY 150
#define CLOSE_PA_DELAY       60
uint8_t gReconCount;
uint16_t gReconWorkTimeout;
uint16_t gRSSICount;
uint16_t gRSSIMuteCount;

extern SYS_TIMER_TYPE gChargerIn_timer;

#ifdef AUDIO_SYNC_DEF
uint8_t g_is_need_send_slave_reset_audio_cmd;
uint8_t gHfpWorkingState;
#endif //AUDIO_SYNC_DEF


BT_REC_INFO * gpRecinfo = (BT_REC_INFO *)reg_map(mem_nv_data);
//uint8_t *M0_LPM_REG = (uint8_t *)reg_map(mem_m0_lpm_flag);
//uint8_t *gpTwsRole  = (uint8_t *)reg_map(mem_tws_role);

//uint8_t *gpAGIndicatorMap=(uint8_t *) reg_map(mem_AG_indicator_map);
//uint8_t *gpAGIndicator=(uint8_t *) reg_map(mem_AG_indicator);
uint8_t *gpHFPCallState =(uint8_t *) reg_map( mem_hf_call_state);

//void Bt_CBM0ClkReset(int params)
//{
//	IPC_TxControlCmd(BT_CMD_RESET_M0_CLK);	
//}

void Bt_init()
{
	gBRState.topState = BR_WAIT_POWER_ON;
	gBRState.secondState=BR_IDLE;
	gBRState.thirdlyState = BR_IDLE;
	gBRState.twsState = BR_IDLE;
	//gBRState.gStateFlag = 0xff;
	//Bt_SetAutoPwroffTimer(AUTO_POWEROFF_WAKEUP);

}


void Bt_CBMaxPower(int params)
{
	Bt_SndCmdSndIncPwr();
}

void Bt_SetIncPwrTimer()
{
	if (HREAD(REG_B_TWS_ROLE) != TWS_SET_SLAVE)
		SYS_SetTimer(&gMAXPowerTimer, 1200, TIMER_CYCLE|TIMER_TYPE_BIT, Bt_CBMaxPower);
}



void Bt_CBConnectedLed(int params)
{
	if (Bt_checkBRIsConnected()){
		Bt_ProcessLedFunc(LED_EVT_CONNECTED);
	}
}

void Bt_SndCmdExitTWS()
{
	// For function good, teminate close.
	return;
	IPC_TxControlCmd(BT_CMD_TWS_NONE);
}

void Bt_CBWaitSlaveTimeout(int params){
	if (gBRState.twsState == BR_TWS_WAIT_SLAVE){
		Bt_SndCmdExitTWS();
	}
}

void Bt_CBSniffMul(int prams)
{
	//UI_STATE_BT_SNIFF,check is in sniff mode
	uint8_t state = HREAD(mem_ui_state_map);
	if ((0x20 & state)  == 0x20)
	{
		//Bt_SndCmdEnableSniffMul();
	}
}

void Bt_SetSniffMulTimer()
{
	SYS_SetTimer(&gSniffMulTimer, SNIFF_MUL_TIMEOUT,
		TIMER_TYPE_BIT|TIMER_SINGLE, Bt_CBSniffMul);
}

void Bt_CBEnterSniff(int prams)
{
	if(HREAD(mem_a2dp_state) == AVDTP_IDENTIFIER_START)
	{
		return;
	}
	Bt_SndCmdEnterSniff();
}

void Bt_SetEnterSniffTimer()
{
	if(HREAD(mem_a2dp_state) == AVDTP_IDENTIFIER_START)
	{
		return;
	}
	SYS_SetTimer(&gEnterSniffTimer, SNIFF_ENTER_DELAY,
		TIMER_TYPE_BIT|TIMER_SINGLE, Bt_CBEnterSniff);
}

void Bt_CBClosePaDelay()
{
	if (7 == HREAD(mem_a2dp_state))
		return;
	VP_snd_voiceEvt(VP_POPUP);
}

void Bt_ClosePaDelay()
{
	SYS_SetTimer(&gClosePaDelayTimer, CLOSE_PA_DELAY,
		TIMER_TYPE_BIT|TIMER_SINGLE, Bt_CBClosePaDelay);
}

void Bt_RelasePaDelay()
{
	SYS_ReleaseTimer(&gClosePaDelayTimer);
}

void Bt_Reset()
{
			Bt_init();	
			SYS_TimerInit();
			keypad_init(KeyPad_Event_Handle);
			//QSPI_Init();
			#ifdef SCO_48K_FIR
			sco_fir_init();
			#endif// SCO_48K_FIR
			VP_init();
			led_init();
			Bat_init();
			FarrowFilterInit();
			//SYS_SetTimer(&gResetTimer,
			//	100,
			//	TIMER_CYCLE|TIMER_TYPE_BIT,
			//	Bt_CBM0ClkReset);
}

void Bt_PowerOn()
{

			gBRState.topState = BR_POWER_ON;
			#ifdef AUDIO_SYNC_DEF
			g_is_need_send_slave_reset_audio_cmd = 0;
			gHfpWorkingState = 0;
			#endif //AUDIO_SYNC_DEF
			gReconWorkTimeout = DEFAULT_DISCOVERY_WORK_TIMEOUT;

			//VP_snd_voiceEvt(VP_POWERON);
			Bt_ProcessLedFunc(LED_EVT_POWER_ON);
			Bt_SetAutoPwroffTimer(AUTO_POWEROFF_IDLE,false,Bt_CBIdleTimeout);
			Bt_LoadReconnectInfo();
			Bt_checkReconInfoCRC();
			// Make sure nv info is right
			if(((HREAD(mem_tws_record_state)&0xf0) == 0x50)
				||((HREAD(mem_tws_record_state)&0x0f) == 0x05)){
				Bt_SndCmdLoadInfo();
			}
}

void Bt_PowerOnKeyRelease(void)
{
	if (gBRState.topState == BR_POWER_ON)
	{
		//Bt_CheckReconnectAction();
		if((HREAD(mem_tws_record_state)&0xf0) == 0x50){
			Bt_SndCmdStartReconTws();
		}
		else{
			VP_snd_voiceEvt(VP_POWERON);
			Bt_CheckReconnectAction();
		}
	}
}

void WDT_Kick(void);
void Bt_EvtCallBack(uint8_t len,uint8_t *dataPtr)
{	
	if (*dataPtr >BT_EVT_100MS_UINT)
	{
		Bt_100ms_timer((*dataPtr)&(0x0F));
		return;
	}

	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"Bt_EvtCallBack: 0x%04X", LOG_POINT_9001, *dataPtr);
	DEBUG_LOG_2(LOG_LEVEL_CORE, "IPC" ,"Bt_EvtCallBack State Info: 0x%04X", LOG_POINT_9002, gBRState.topState, gBRState.secondState);
	
	if (gBRState.topState <= BR_CHARGER_IN&&
		(*dataPtr != BT_EVT_RESET) &&(*dataPtr != BT_EVT_RESTART)
		&&  (*dataPtr != BT_EVT_CLOSE_ADAC) )
		return;
	
	switch(*dataPtr)
	{
		case BT_EVT_CLOSE_ADAC:
			//VP_snd_voiceEvt(VP_POPUP);
			Bt_ClosePaDelay();
			break;
		case BT_EVT_MAX_POWER:
			SYS_ReleaseTimer(&gMAXPowerTimer);
			break;
		case BT_EVT_EXIT_SNIFF:
			SYS_ReleaseTimer(&gSniffMulTimer);
			Bt_SndCmdDisableSniffMul();
			Bt_SetEnterSniffTimer();
			break;
		
		case BT_EVT_ENTER_SNIFF:
			SYS_ReleaseTimer(&gEnterSniffTimer);
			Bt_SetSniffMulTimer();
			break;
		
		case BT_EVT_RECONN_FAILED:
		case BT_EVT_RECONN_PAGE_TIMEOUT:
			if (gBRState.topState >= BR_POWER_ON)
			{
				Bt_SndCmdStartDiscovery();
				if (gReconCount !=0)
				{
					gReconCount--;
					Bt_StartReconnectPhoneTimer();
					gReconWorkTimeout += 50;//add 2s
				}
				else
				{
					gReconWorkTimeout = DEFAULT_DISCOVERY_WORK_TIMEOUT;
				}
			}
			break;
		case BT_EVT_DEV_UPDATE_INFO:
			Bt_calReconInfoCRC();
			Bt_UpdateDeviceInfo();
			break;

		case BT_EVT_BB_DISCONNECTED:
			SYS_ReleaseTimer(&gMAXPowerTimer);
			SYS_ReleaseTimer(&gSniffMulTimer);
			SYS_ReleaseTimer(&gEnterSniffTimer);
			SYS_ReleaseTimer(&gCONLedTimer);
			if (gBRState.thirdlyState != BR_DISCOVERABLE)
			{	
				Bt_SetAutoPwroffTimer(AUTO_POWEROFF_IDLE,false,Bt_CBIdleTimeout);
			}
			Lpm_unLockLpm(LINK_LPM_FLAG);
			gBRState.secondState = BR_IDLE;
			HWRITE(mem_rssi_low,0);
			break;
		case BT_EVT_BB_CONNECTED:
			gReconCount = 0;
			Bt_SndCmdStopDiscovery();
		 	Bt_SndCmdWaitSlave();
			break;
		case BT_EVT_PROFILE_DISCONNECT:
			if (SYS_TimerisExist(&gChargerIn_timer))
				return;
			if (gBRState.topState >= BR_POWER_ON)
			{
				VP_snd_voiceEvt(VP_DISCONNECT);
				Bt_ProcessLedFunc(LED_EVT_DISCONNECTED);
			}
			break;
		case BT_EVT_ABNORMAL_DISCONNECT:
			if (SYS_TimerisExist(&gChargerIn_timer))
				return;
			gReconWorkTimeout = DEFAULT_DISCOVERY_WORK_TIMEOUT;
			gReconCount = 6;
			Bt_SndCmdReconnect();
			break;
		
		case BT_EVT_ENTER_DISCOVERABLE:
			if (SYS_TimerisExist(&gChargerIn_timer))
			{
				Bt_SndCmdStopDiscovery();
				return;
			}
			if (gBRState.topState == BR_POWER_ON)
			{
				Bt_SetAutoPwroffTimer(AUTO_POWEROFF_DISCOVERABLE,false,Bt_CBIdleTimeout);
				Bt_ProcessLedFunc(LED_EVT_DISCOVERABLE);
				gBRState.thirdlyState = BR_DISCOVERABLE;
			}
			break;
		case BT_EVT_EXIT_DISCOVERABLE:
			gBRState.thirdlyState = BR_IDLE;
			break;
			
		case BT_EVT_ENC_CONNECTED:
			gReconCount = 0;
			//Bt_SetAutoPwroffTimer(AUTO_POWEROFF_CONECTED,false,Bt_CBIdleTimeout);
			SYS_ReleaseTimer(&gBRState.autoPowerTimer);
			VP_snd_voiceEvt(VP_CONNECTED);
			Bt_ProcessLedFunc(LED_EVT_CONNECTED);
			Bt_ProcessLedFunc(LED_EVT_CONNECTED_IDLE);
			gBRState.secondState = BR_CONNECTED;
			break;
		case BT_EVT_RESET:
			Bt_Reset();		
			break;
		case BT_EVT_RESTART:
			Bt_Reset();
			WDT_Kick();
			SYS_delay_ms(500);
			WDT_Kick();
			SYS_delay_ms(500);
			WDT_Kick();
			SYS_delay_ms(100);
			Bt_PowerOn();
			Bat_polling();
			if(gBRState.topState != BR_CHARGER_IN  )
			{
				if((HREAD(mem_tws_record_state)&0xf0) == 0x50)
					Bt_SndCmdStartReconTws();
				else
				{
					VP_snd_voiceEvt(VP_POWERON);
					Bt_CheckReconnectAction();
				}
			}
			break;


		case BT_EVT_RECONNECT_RSP:
			Bt_ProcessLedFunc(LED_EVT_PAGING);
			break;
		case BT_EVT_WAKEUP:
			VP_ADAC_reg_init();
			Bat_reInit();
			SysTick_Config(SYSTEM_CLOCK/100); //each  systick interrupt is 10ms
			keypad_check_keypress(0);
			OS_EXIT_CRITICAL();
			break;
		case BT_EVT_ALL_PROFILE_CONNECT:
			Bt_SndCmdWaitSlave();
			break;
		//case BT_EVT_RESET_AUTO_TIMER:
		//	Bt_ResetAutoPwroffTimer();
		//	break;
			
		case BT_EVT_HFP_DISCONNECTED:
			BT_twsCheckAndUpdateProfileAfterProfileDisconnect();
			break;
	
		case BT_EVT_HFP_UP_TO_MAX_VLM:
			//VP_snd_voiceEvt(VP_VLM_MAX);
			break;
		case BT_EVT_PHONE_NUM:
			{
				if (!VP_Check_Queue_Is_empty())
					return;
				uint8_t temp = HREAD(mem_phone_num_len);
				for (uint8_t i=0;i<temp;i++)
				{
					uint8_t num=HREAD((mem_phone_number+i));
					if (num <0 || num >9)
					{
						error_handle();
					}
					VP_snd_voiceEvt(VP_DIGITAL+num);
				}
			}
			break;
		case BT_EVT_LE_CONNECTED:
			//Bt_SetAutoPwroffTimer(AUTO_POWEROFF_CONECTED,false,Bt_CBIdleTimeout);
			SYS_ReleaseTimer(&gBRState.autoPowerTimer);
			break;
		case BT_EVT_LE_DISCONNECTED:
			if (gBRState.thirdlyState== BR_DISCOVERABLE)	
				Bt_SndCmdStartDiscovery();
			break;
		default:
			break;
	}

	return;
}

uint8_t testFlag;


uint16_t OTA_packetid;
uint32_t OTA_flash_start_addr;
uint32_t OTA_datasum,OTA_datalen;

//uint iii;

void Bt_SndCmdBLE( uint8_t *data,uint8_t len)
{
	IPC_TxCommon(IPC_TYPE_BLE,data,len);
}


void Bt_SndCmdSPP(uint8_t *data,uint8_t len)
{
	IPC_TxCommon(IPC_TYPE_SPP,data,len);
}

void OTA_Init()
{
 		OTA_packetid = 0;
 		OTA_datasum = 0;
 		OTA_flash_start_addr = 0;
 		OTA_datalen = 0;
}

void Bt_CBChangeCode()
{
	uint8_t datatemp[3]={0};
	if(OTA_flash_start_addr != CODE_START_FLASHADDR1 
	 	&& OTA_flash_start_addr!= CODE_START_FLASHADDR2)
	 	return;
	 
//	temp = REVERSE_3BYTE_DEFINE(OTA_flash_start_addr);
	QSPI_Init();
	OS_ENTER_CRITICAL();
	SetLockQSPI( );
//	setRamType(XRAM);
	datatemp[0] = OTA_flash_start_addr>>16;
	datatemp[1] = (OTA_flash_start_addr&0xff00)>>8;
	datatemp[2] = OTA_flash_start_addr;
	QSPI_SectorEraseFlash(DEVICE_INFO_BASE);
	QSPI_SectorEraseFlash(0);
	QSPI_WriteFlashData(0,3,datatemp);
	SetReleaseQSPI( );
	OS_EXIT_CRITICAL();
	HWRITE(CORE_RESET,0x03);
	while(1);
}


void Bt_SPPCallBack(uint8_t len,uint8_t *dataPtr)
{
	uint8_t j=0,data[5]={0},datatemp[32]={0};
	uint32_t i;
//	uint16_t temp;
//	testFlag = *dataPtr;
	QSPI_Init();
	OS_ENTER_CRITICAL();
	setRamType(MRAM);
	SetLockQSPI( );
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"Bt_SPPCallBack: 0x%04X", LOG_POINT_9004, *(dataPtr));
	switch(*dataPtr)
	{
 		case OTA_EVT_START:
   			if(len != 1)
 				break;
			OTA_Init();	
			QSPI_ReadFlashData(0,3,(uint8_t *)(&OTA_flash_start_addr));
	 		if(CODE_START_FLASHADDR1 == REVERSE_3BYTE_DEFINE(OTA_flash_start_addr))
 				OTA_flash_start_addr = CODE_START_FLASHADDR2;
 			else if(CODE_START_FLASHADDR2 == REVERSE_3BYTE_DEFINE(OTA_flash_start_addr))
  					OTA_flash_start_addr = CODE_START_FLASHADDR1;
  				else
  					break;	
  					
  			for(i=0;i<0x17;i++)
  				QSPI_SectorEraseFlash(OTA_flash_start_addr+(i<<12));	
 	 	//	QSPI_64kEraseFlash(OTA_flash_start_addr);
//while(1);
			data[0] = OTA_CMD;
 			data[1] = OTA_EVT_START;			
 			Bt_SndCmdSPP(data,2);
			break;
			
 		case OTA_EVT_DATA:
  // if(iii >10)
  // 	break;
 			if(OTA_packetid == 0 || (OTA_packetid +1) == (*(dataPtr+1) + (*(dataPtr+2) <<8)))
 				OTA_packetid = *(dataPtr+1) + (*(dataPtr+2) <<8);
			else
	 			break;

			if(((*(dataPtr+4) <<8) + *(dataPtr+3) ) >0)
			{
 			 	QSPI_WriteFlashData(OTA_flash_start_addr+OTA_datalen,(*(dataPtr+4) <<8) + *(dataPtr+3) ,dataPtr+5);
				OTA_datalen += (*(dataPtr+4) <<8)  +  *(dataPtr+3);
 		//		for(i=0;i<((*(dataPtr+4) <<8) + *(dataPtr+3) );i++)
		//			OTA_datasum += *(dataPtr+5+i);
			}
			else
				break;
// iii++;			
 			data[0] = OTA_CMD;
 			data[1] = OTA_EVT_DATA;
 			data[2] = *(dataPtr+1);
 			data[3] = *(dataPtr+2);
 			Bt_SndCmdSPP(data,4);
 			break;
 			
 		case OTA_EVT_END:
 			if(len != 9)
 				break;
	  		data[0] = OTA_CMD;
	 		data[1] = OTA_EVT_END;
	  		data[2] = *(dataPtr+1);
	  		data[3] = *(dataPtr+2);

			for(i=0; i<OTA_datalen; )
			{
				if(OTA_datalen-i>=32)
				{
					QSPI_ReadFlashData(OTA_flash_start_addr+i,32,datatemp);	
 		 			for(j=0;j<32;j++)
		 				OTA_datasum += datatemp[j];
		 			i += 32;
		 		}else
		 			{
					QSPI_ReadFlashData(OTA_flash_start_addr+i,OTA_datalen-i,datatemp);	
 		 			for(j=0;j<OTA_datalen-i;j++)
		 				OTA_datasum += datatemp[j];		
		 			i = OTA_datalen;
		 			}
			}			
 			if(OTA_datasum == *(dataPtr+3) + (*(dataPtr+4)<<8) + (*(dataPtr+5)<<16)
 				&& OTA_datalen  ==  *(dataPtr+6) + (*(dataPtr+7)<<8) + (*(dataPtr+8)<<16) ) 
				{
	  				data[4] = OTA_END_CHECK_OK;		
		 			SYS_SetTimer(&gOTATimer,
						11, 
						TIMER_SINGLE|TIMER_TYPE_BIT,
						Bt_CBChangeCode);
	  			}else
		  		{
		  			data[4] = OTA_END_CHECK_FAIL;	
		  			OTA_Init();
	 	 		}
	 	 		
 			Bt_SndCmdSPP(data,5);
 			break;
 			
 		case OTA_EVT_RESET:
  			if(len != 1)
 				break;
			OTA_Init();
			break;
			
		default:
			OTA_Init();
			break;
	}	
	SetReleaseQSPI( );
	OS_EXIT_CRITICAL();
}


void Bt_BleCallBack(uint8_t len,uint8_t *dataPtr)
{
	uint8_t j=0,data[5]={0},datatemp[32]={0};
	uint32_t i;
//	uint16_t temp;
//	testFlag = *dataPtr;
	setRamType(MRAM);
	if(*dataPtr +(*(dataPtr +1)<<8) == OTA_BLE_WRITE_HANDLE  
		&& len>3 && 	*(dataPtr+2) == OTA_EVT)
	{
	QSPI_Init();
	SetLockQSPI( );
	OS_ENTER_CRITICAL();
		DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"Bt_BleCallBack: 0x%04X", LOG_POINT_9003, *(dataPtr+3));

		switch(*(dataPtr+3))
		{
	 		case OTA_EVT_START:
	   			if(len != 4)
	 				break;
				OTA_Init();	
				QSPI_ReadFlashData(0,3,(uint8_t *)(&OTA_flash_start_addr));
		 		if(CODE_START_FLASHADDR1 == REVERSE_3BYTE_DEFINE(OTA_flash_start_addr))
	 				OTA_flash_start_addr = CODE_START_FLASHADDR2;
	 			else if(CODE_START_FLASHADDR2 == REVERSE_3BYTE_DEFINE(OTA_flash_start_addr))
	  					OTA_flash_start_addr = CODE_START_FLASHADDR1;
	  				else
	  					break;	
	  					
	  			for(i=0;i<0x17;i++)
	  				QSPI_SectorEraseFlash(OTA_flash_start_addr+(i<<12));	
	 	 	//	QSPI_64kEraseFlash(OTA_flash_start_addr);
	 	 		
				data[0] = OTA_BLE_NOTIFY_HANDLE;
				data[1] = OTA_BLE_NOTIFY_HANDLE>>8;
				data[2] = OTA_CMD;
	 			data[3] = OTA_EVT_START;			
	 			Bt_SndCmdBLE(data,4);
				break;
				
	 		case OTA_EVT_DATA:
//	    if(iii >2)
//	    	break;
	 			if(OTA_packetid == 0 || (OTA_packetid +1) == (*(dataPtr+4) + (*(dataPtr+5) <<8)))
	 				OTA_packetid = *(dataPtr+4) + (*(dataPtr+5) <<8);
				else
		 			break;

				if( *(dataPtr+6) >0)
				{
	 			 	QSPI_WriteFlashData(OTA_flash_start_addr+OTA_datalen, *(dataPtr+6),dataPtr+7);
					OTA_datalen +=  *(dataPtr+6);
//	 				for(i=0;i<*(dataPtr+6) ;i++)
//						OTA_datasum += *(dataPtr+7+i);
				}
				else
					break;		
				data[0] = OTA_BLE_NOTIFY_HANDLE;
				data[1] = OTA_BLE_NOTIFY_HANDLE>>8;
				data[2] = OTA_CMD;
	 			data[3] = OTA_EVT_DATA;
	 	 		data[4] = *(dataPtr+4);
	 			data[5] = *(dataPtr+5);
	 			Bt_SndCmdBLE(data,6);
	 			break;
	 			
	 		case OTA_EVT_END:
//	 			if(len != 9)
//	 				break;
				data[0] = OTA_BLE_NOTIFY_HANDLE;
				data[1] = OTA_BLE_NOTIFY_HANDLE>>8;
		  		data[2] = OTA_CMD;
		 		data[3] = OTA_EVT_END;
		  		data[4] = *(dataPtr+4);
		  		data[5] = *(dataPtr+5);

				for(i=0; i<OTA_datalen; )
				{
					if(OTA_datalen-i>=32)
					{
						QSPI_ReadFlashData(OTA_flash_start_addr+i,32,datatemp);	
	 		 			for(j=0;j<32;j++)
			 				OTA_datasum += datatemp[j];
			 			i += 32;
			 		}else
			 			{
						QSPI_ReadFlashData(OTA_flash_start_addr+i,OTA_datalen-i,datatemp);	
			 			for(j=0;j<OTA_datalen-i;j++)
			 				OTA_datasum += datatemp[j];	
			 			i = OTA_datalen;
			 			}
				}			
	 			if(OTA_datasum == *(dataPtr+6) + (*(dataPtr+7)<<8) + (*(dataPtr+8)<<16)
	 				&& OTA_datalen  ==  *(dataPtr+9) + (*(dataPtr+10)<<8) + (*(dataPtr+11)<<16) ) 
					{
		  				data[6] = OTA_END_CHECK_OK;		
			 			SYS_SetTimer(&gOTATimer,
							11, 
							TIMER_SINGLE|TIMER_TYPE_BIT,
							Bt_CBChangeCode);
 	  			}else
			  		{
			  			data[6] = OTA_END_CHECK_FAIL;	
			  			OTA_Init();
		 	 		}
		 	 		
	 			Bt_SndCmdBLE(data,7);
	 			break;
	 			
	 		case OTA_EVT_RESET:
//	  			if(len != 4)
//	 				break;
				OTA_Init();
				break;
				
			default:
				OTA_Init();
				break;
		}	
	SetReleaseQSPI( );
	OS_EXIT_CRITICAL();
	}
	else
	{
	
	}
	
	
}


void Bt_A2DPCallBack(uint8_t len,uint8_t *dataPtr)
{
	testFlag = *dataPtr;
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"Bt_A2DPCallBack: 0x%04X", LOG_POINT_9006, *dataPtr);
	switch(*dataPtr)
	{
 		case AVDTP_IDENTIFIER_OPEN:
 			Bt_SetEnterSniffTimer();

 			//HWRITE(mem_a2dp_state,AVDTP_IDENTIFIER_OPEN);
 			break;
 		case AVDTP_IDENTIFIER_START:
 			FarrowFilterInit();
 			Bt_SetIncPwrTimer();
 			SYS_ReleaseTimer(&gEnterSniffTimer);
 			SYS_ReleaseTimer(&gClosePaDelayTimer);
 			#ifdef AUDIO_SYNC_DEF
 			Bt_TwsResetFuncInit();
 			#endif //AUDIO_SYNC_DEF

 			VP_snd_voiceEvt(VP_POPDOWN);
			
 			//HWRITE(mem_a2dp_state,AVDTP_IDENTIFIER_START);
 			break;
 		case AVDTP_IDENTIFIER_CLOSE:
 			SYS_ReleaseTimer(&gMAXPowerTimer);
 			Bt_SetEnterSniffTimer();
			#ifdef AUDIO_SYNC_DEF
 			Bt_TwsResetFuncInit();
 			#endif //#ifdef AUDIO_SYNC_DEF
 			//HWRITE(mem_a2dp_state,AVDTP_IDENTIFIER_CLOSE);
 			break;
 		case AVDTP_IDENTIFIER_SUSPEND:
 			SYS_ReleaseTimer(&gMAXPowerTimer);
 			Bt_SetEnterSniffTimer();
 			#ifdef AUDIO_SYNC_DEF
 			Bt_TwsResetFuncInit();
 			#endif //#ifdef AUDIO_SYNC_DEF
 			//HWRITE(mem_a2dp_state,AVDTP_IDENTIFIER_SUSPEND);
 			break;
 			
 		case AVDTP_VOL_CHANGED:
 			DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"Bt_A2DPCallBack AVDTP_VOL_CHANGED: 0x%04X", LOG_POINT_900D, dataPtr[1]);
 			if (dataPtr[1] == 0x07){
				HWRITE(mem_audio_vol,0x03);//diff=3
 			}
 			else if(dataPtr[1] == 0x0f){
				HWRITE(mem_audio_vol,0x09);//diff=6
 			}
 			else if(dataPtr[1] == 0x17){
				HWRITE(mem_audio_vol,0x11);//diff=10
 			}
 			else if(dataPtr[1] == 0x1F){
				HWRITE(mem_audio_vol,0x1C);//diff=11
 			}
 			else{
				HWRITE(mem_audio_vol,dataPtr[1]);
 			}
 			break;
 		
		default:
			break;
	}	
}


void Bt_HFPCallBack(uint8_t len,uint8_t *dataPtr)
{
	uint16_t temp16;
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"Bt_HFPCallBack: 0x%04X", LOG_POINT_9007, *dataPtr);
	switch(*dataPtr)
	{
		case HFP_EVT_ESCO_START:
			temp16= HREADW(0x8050);
			//CLOCK_OFF_VOICE_FILTER,bit6
			temp16 = temp16&0xffbf;
			HWRITE(0x8050,temp16);
			#ifdef AUDIO_SYNC_DEF
			gHfpWorkingState = 1;
			#endif //AUDIO_SYNC_DEF

			Bt_SetIncPwrTimer();
			SYS_ReleaseTimer(&gEnterSniffTimer);
			SYS_ReleaseTimer(&gClosePaDelayTimer);
			VP_snd_voiceEvt(VP_POPDOWN);
			break;
		case HFP_EVT_ESCO_STOP:
			temp16= HREADW(0x8050);
			//CLOCK_OFF_VOICE_FILTER,bit6
			temp16 = temp16|0x0040;
			HWRITE(0x8050,temp16);
			#ifdef AUDIO_SYNC_DEF
			gHfpWorkingState = 0;
			#endif //#ifdef AUDIO_SYNC_DEF
			SYS_ReleaseTimer(&gMAXPowerTimer);
			Bt_SetEnterSniffTimer();
		 	break;
		default:
			break;
	}
	
	return;
}


void Bt_TWSCallBack(uint8_t len,uint8_t *dataPtr)
{
	DEBUG_LOG_2(LOG_LEVEL_CORE, "IPC" ,"Bt_TWSCallBack: 0x%04X", LOG_POINT_9008, *dataPtr, gBRState.twsState);
	DEBUG_LOG_2(LOG_LEVEL_CORE, "IPC" ,"Bt_TWSCallBack: 0x%04X", LOG_POINT_9009, gBRState.topState, gBRState.secondState);
	if (gBRState.topState <= BR_CHARGER_IN)
		return;

	switch(*dataPtr)
	{
		case TWS_EVT_TWS_NONE:
			gBRState.twsState = BR_IDLE;
			break;
		case TWS_EVT_ENTER_PAIR:
			Bt_ProcessLedFunc(LED_EVT_TWS_PARING);
			gBRState.twsState = BR_TWS_PAIRING;
			break;
			
		case TWS_EVT_EXIT_PAIR:
			gBRState.twsState = BR_IDLE;
			break;

		case TWS_EVT_PAIR_SUCCESS:
			VP_snd_voiceEvt(VP_TWS_CONNECTED);
			Bt_ProcessLedFunc(LED_EVT_TWS_PARED);
			if (HREAD(REG_B_TWS_ROLE) == TWS_SET_MASTER)
				Bt_CheckReconnectAction(); 
			break;

		case TWS_EVT_WAIT_SLAVE:
			Bt_ReleaseReconTimer();
			gBRState.twsState = BR_TWS_WAIT_SLAVE;
			SYS_SetTimer(&gTWSWaitSlaveTimer,
				1200, // 2min
				TIMER_SINGLE|TIMER_TYPE_BIT,
				Bt_CBWaitSlaveTimeout);
			break;

		case TWS_EVT_WAIT_MASTER:
			Bt_ReleaseReconTimer();
			gBRState.twsState = BR_TWS_WAIT_MASTER;
			break;
			
		case  TWS_EVT_SLAVE_CONNECTED:
			SYS_ReleaseTimer(&gBRState.autoPowerTimer);
			Bt_ProcessLedFunc(LED_EVT_CONNECTED);
			Bt_ProcessLedFunc(LED_EVT_CONNECTED_IDLE);
			gBRState.twsState = BR_TWS_SLAVE_CONNECTED;
			gBRState.secondState = BR_CONNECTED;

			#ifdef AUDIO_SYNC_DEF
			// Check is before power on play
			if (AVD_AUDIO_ALLOW == HREAD(mem_audio_allow))
			{
				Bt_SndCmdSlaveResetAudio();
			}
			else
			{
				g_is_need_send_slave_reset_audio_cmd = 1;
			}
			#endif //AUDIO_SYNC_DEF
			break;

		case  TWS_EVT_MASTER_CONNECTED:
			SYS_ReleaseTimer(&gTWSWaitSlaveTimer);
			HWRITE(mem_tws_record_state,0x55);
			Bt_calReconInfoCRC();
			Bt_UpdateDeviceInfo();
			
			gBRState.twsState = BR_TWS_MASTER_CONNECTED;
			#ifdef AUDIO_SYNC_DEF
 			Bt_TwsResetFuncInit();
 			#endif //Bt_TwsResetFuncInit
			break;

		case TWS_EVT_START_RECON:
			gBRState.twsState = BR_TWS_RECON;
			break;

		case TWS_EVT_SLAVE_FOUND:
			VP_snd_voiceEvt(VP_TWS_CONNECTED);
			Bt_ReleaseReconTimer();
			Bt_ReleaseReconnectPhoneTimer();
			if (gBRState.twsState == BR_TWS_RECON)
			{
				gBRState.twsState = BR_IDLE;
				Bt_CheckReconnectAction();
			}
			break;
			
		case TWS_EVT_MASTER_FOUND:
			VP_snd_voiceEvt(VP_TWS_CONNECTED);
			Bt_ReleaseReconTimer();
			Bt_SndCmdStopDiscovery();
			gBRState.twsState = BR_IDLE;
			Bt_SndCmdWaitMaster();
			Bt_ProcessLedFunc(LED_EVT_TWS_SLAVE_CONNECTED);
			break;


		case TWS_SYNC_EVT_POWR_OFF:
			if (gBRState.twsState == BR_TWS_MASTER_CONNECTED){
				Bt_SndCmdWaitTwsSlave();
				//gBRState.gcount1++;
			}
			else if (SYS_TimerisExist(&gChargerIn_timer)){
				//gBRState.gcount3++;
			//else if (gBRState.topState == BR_CHARGER_IN 
			//	&& gBRState.twsState == BR_TWS_SLAVE_CONNECTED){
				SYS_ReleaseTimer(&gChargerIn_timer);
				Bat_CbChargerIn(0);
				Bt_SndCmdAcceptTwsSwitch();
			}
			else{
				gBRState.twsState = BR_IDLE; 
				if (gBRState.secondState >= BR_CONNECTING)
				{
					Lpm_LockLpm(LINK_LPM_FLAG);
					Bt_SndCmdDisconnect();
				}
				Bt_SndCmdWaitPwroff();
			}
			break;
		case TWS_SYNC_EVT_USER1:
			tws_key_func_handle(KEY_0, TWS_KEY_EVT_TYPE_TAP, true);
			break;
		case TWS_SYNC_EVT_USER2:
			tws_key_func_handle(KEY_0, TWS_KEY_EVT_TYPE_DOUBLE, true);
			break;
		case TWS_SYNC_EVT_USER3:
			tws_key_func_handle(KEY_0, TWS_KEY_EVT_TYPE_TRIPLE, true);
			break;      
		case TWS_SYNC_EVT_USER4:
			tws_key_func_handle(KEY_0, TWS_KEY_EVT_TYPE_LONG, true);
			break;
		case TWS_SYNC_EVT_MASTER_SWITCH:  
			if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
			{
				//gBRState.gcount2++;
				SYS_SetTimer(&gSwithMasterTimer, 10, TIMER_SINGLE, CB_SwitchToMaster);
				//Bt_SndCmdTwsSwitchToMaster();	
			}
			else if (gBRState.twsState == BR_TWS_MASTER_CONNECTED){
				SYS_ReleaseTimer(&gChargerIn_timer);
				Bat_CbChargerIn(0);
			}
			break;
		#ifdef AUDIO_SYNC_DEF
		case TWS_SYNC_EVT_RESET_AUDIO:
			tws_user_reset_audio_action();
			break;
		case TWS_SYNC_EVT_SLAVE_RESET_AUDIO:
			// Need check need do or not
			if(g_tws_reset_flag != TWS_RESET_FLAG_NONE)
			{
				// Becouse we are in error buffer state, we will send reset audio message after while.
				DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"check and send reset audio message, error reset_flag: 0x%04X", LOG_POINT_B013, g_tws_reset_flag);
				
				return;
			}
			if(HREAD(mem_enable_start_sync) != 0)
			{
				// Becouse we are in start sync.
				DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"check and send reset audio message, error mem_enable_start_sync: 0x%04X", LOG_POINT_B014, HREAD(mem_enable_start_sync));
				return;
			}
			
			Bt_SndCmdResetAudio();
			break;
		case TWS_SYNC_EVT_UPDATE_AUDIO_PROCESS_INFO:
			tws_user_process_audio_info();
			break;
		case TWS_SYNC_EVT_AUDIO_PROCESS_INFO:
			tws_user_process_remote_audio_info();
			break;
		case TWS_SYNC_EVT_HFP_START_SYNC:
			tws_user_hfp_start_sync_action();
			break;
		case TWS_SYNC_EVT_ALL_PROFILE_DISCONNECTED:
			if(gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
			{
				if (gBRState.secondState >= BR_CONNECTING)
				{
					Bt_SndCmdDisconnect();
				}
			}
			else if(gBRState.twsState == BR_TWS_MASTER_CONNECTED)
			{
				Bt_SndCmdWaitTwsSlave();
			}
			break;
		case TWS_SYNC_EVT_DROP_ONE_PACKET:
			gIsDropInProcess = TWS_DROP_PROCESS_STATE_WAIT_DROP;
			break;
		case TWS_EVT_SLAVE_CHANGE_MASTER_NO_CON:
			gBRState.twsState = BR_IDLE;
			Bt_CheckReconnectAction();
			break;
		#endif //AUDIO_SYNC_DEF
		default:
			break;
	}
}

uint8_t Bt_checkBleIsConnected()
{
	if(gBLEState.topState == BLE_CONNECTED)
		return 1;
	return 0;
}

uint8_t Bt_checkBRIsConnected()
{
	if(gBRState.topState == BR_POWER_ON && gBRState.secondState == BR_CONNECTED)
		return 1;
	return 0;
}

uint8_t Bt_checkBRIsInDiscoverable(void)
{
	if(gBRState.topState == BR_POWER_ON && gBRState.thirdlyState == BR_DISCOVERABLE)
		return 1;
	return 0;
}

void Bt_CheckWaitPwroff()
{
	if (Lpm_CheckLpmFlag())
	{
		//no detecte key action,then enter hibernate
		if(gBRState.topState == BR_WAIT_POWER_ON ||
			gBRState.topState == BR_WAIT_POWER_OFF)
		{
			SYS_ReleaseTimer(&gBRState.autoPowerTimer);
			Bt_SndCmdPwroff();
		}
	}
}

void Bt_CBIdleTimeout(int prams)
{
  	Bt_SndCmdWaitPwroff();
}

void Bt_CBAutoPwroff(int paras)
{
	if (Lpm_CheckLpmFlag())
	{
		if (gBRState.topState == BR_WAIT_POWER_ON ||
			gBRState.topState == BR_WAIT_POWER_OFF){
			Bt_SndCmdPwroff();
		}
		else if (gBRState.topState == BR_POWER_ON){
			Bt_SndCmdWaitPwroff();
		}
	}
	else
	{
		//for disconect link/enter hibernate delay some time
		uint16_t lpm=(uint16_t)( ~IGNORE_LPM_FLAG);
		if (lpm & HR_REG_16BIT(reg_map(M0_LPM_REG)))
		{
			SYS_SetTimer(&gBRState.autoPowerTimer,
				3,
				TIMER_SINGLE,
				Bt_CBAutoPwroff);
		}
		else
		{
			Bt_SndCmdPwroff();
		}
	}
}

void Bt_ResetAutoPwroffTimer()
{
	if (gBRState.topState >= BR_WAIT_POWER_ON)
	{
		if (gBRState.secondState != BR_CONNECTED)
			SYS_ResetTimer(&gBRState.autoPowerTimer);
	}
}

void Bt_SetAutoPwroffTimer(uint32_t count,bool type,Timer_Expire_CB pfExpire_CB)
{	
	//Bt_CBAutoPwroff
	if (gBRState.topState >= BR_WAIT_POWER_ON)
	{
		if (type){
			SYS_SetTimer(&gBRState.autoPowerTimer,
				count,
				TIMER_SINGLE,
				pfExpire_CB);
		}
		else{
			SYS_SetTimer(&gBRState.autoPowerTimer,
				count,
				TIMER_SINGLE|TIMER_TYPE_BIT,
				pfExpire_CB);
		}
	}
}

bool Bt_checkReconInfoCRC()
{
	uint16_t i,crc_value,sum=0;
	uint16_t addr = mem_nv_data;
	
	//for(i = 0; i< sizeof(BT_REC_INFO)-2; i++)
	for(i = 0; i < 36; i++)
		sum = sum + HREAD(addr+i);
		

	crc_value = HREADW(addr+i);
	if (crc_value == sum)
		return true;

	//for(i = 0; i < sizeof(BT_REC_INFO); i++)
	for(i = 0; i < 36+2; i++)
		HWRITE(addr+i,0xff);
	return false;
}

void Bt_calReconInfoCRC()
{
	uint16_t i, crc_value,sum=0;
	uint16_t addr = mem_nv_data;
	
	for(i = 0; i < sizeof(BT_REC_INFO)-2; i++)
	//for(i = 0; i < 36; i++)
		sum = sum + HREAD(addr+i);

	HWRITEW((addr+i),sum);
}

void Bt_LoadReconnectInfo()
{
	//todo
	QSPI_Init();
	
	OS_ENTER_CRITICAL();
	SetLockQSPI( );
	setRamType(XRAM);
	QSPI_ReadFlashData(DEVICE_INFO_BASE,sizeof(BT_REC_INFO),(uint8_t *)gpRecinfo);
	SetReleaseQSPI( );
	OS_EXIT_CRITICAL();
}

void Bt_UpdateDeviceInfo()
{
	QSPI_Init();
	//todo
	OS_ENTER_CRITICAL();
	SetLockQSPI( );
	setRamType(XRAM);
	QSPI_SectorEraseFlash(DEVICE_INFO_BASE);
	QSPI_WriteFlashData(DEVICE_INFO_BASE,sizeof(BT_REC_INFO),(uint8_t *)gpRecinfo);
	SetReleaseQSPI( );
	OS_EXIT_CRITICAL();
}

void Bt_CheckReconnectAction()
{
	/**
	//todo
	if((HREAD(mem_tws_record_state)&0xf0) == 0x50
		&& gpRecinfo->sTwsrole == TWS_SET_SLAVE){
		//todo tws slave feature
		Bt_SndCmdWaitMaster();
		return;
	}
	**/

	if((HREAD(mem_tws_record_state)&0x0f) == 0x05)
	{
		gReconCount = 2;
		Bt_SndCmdReconnect();
	}
	else{
		Bt_SndCmdStartDiscovery();
	}

	return;
}


void Bt_SndCmdPwroff()
{
	//disable interrupt,will disable keyscan
	OS_ENTER_CRITICAL();
	gBRState.topState = BR_POWER_OFF;
	IPC_TxControlCmd(BT_CMD_ENTER_HIBERNATE);
	
	while(1){
	 	uint8_t temp = HREAD(IPC_MCU_STATE);
		if (temp == IPC_MCU_STATE_HIBERNATE)
		{	
			Bt_ActionBeforeHibernate();
			HWRITE(IPC_MCU_STATE,IPC_MCU_STATE_STOP);
			while(1);
		}
		else if (temp == IPC_MCU_STATE_LMP){
			HWRITE(IPC_MCU_STATE,IPC_MCU_STATE_RUNNING);
		}
		hw_delay();
	}
}

void Bt_ActionBeforeLpm()
{
	//gpio leakage of electricity
	Bat_ClcGpio();
/**
	uint8_t i;
	uint16_t regaddr=0x8080; //gpio reg
	for(i=0; i<32;i++)
	{
		switch (HREAD(regaddr))
		{
  			//case GPCFG_QSPI_SCK:     
  			//case GPCFG_QSPI_IO0:        
  			//case GPCFG_QSPI_IO1:       
  			//case GPCFG_QSPI_IO3:  
  			case GPCFG_IIC_SCL:
  			case GPCFG_IIC_SDA:
  			case GPCFG_PULLUP:
  				HWRITE(regaddr,GPCFG_PULLDOWN);
  				break;
  			default:
  				break;
		}
		regaddr++;
	}
**/
}

void Bt_ActionBeforeHibernate()
{
	//gpio leakage of electricity
	Bat_ClcGpio();
}

void Bt_SndCmdWaitPwroff()
{
//	return;

	if (gBRState.topState != BR_POWER_ON)
		return;

	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"Bt_SndCmdWaitPwroff: 0x%04X", LOG_POINT_A207, gBRState.topState);
	Bt_ReleaseReconTimer();
	
	//led_send_bgevt(LED_ALL_OFF);
	led_clear_led();
	Bt_ProcessLedFunc(LED_EVT_POWER_OFF);
	VP_snd_voiceEvt(VP_POWEROFF);
	
	//temp code
	Lpm_LockLpm(HIBER_LPM_FLAG);

	Bt_SetAutoPwroffTimer(AUTO_POWEROFF_WAIT,true,Bt_CBAutoPwroff);
	gBRState.topState = BR_WAIT_POWER_OFF;
	

#ifdef SYNC_POWER_OFF
	if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED 
		|| gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		Bt_SndCmdtwsPwroff();
	}
	else if (gBRState.secondState >= BR_CONNECTING)
	{
		Lpm_LockLpm(LINK_LPM_FLAG);
		Bt_SndCmdDisconnect();
	}
#else
	if (gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		Bt_SndCmdTwsMasterSwitch();
	}
	else if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED){
		Bt_SndCmdtwsPwroff();
	}
	else if (gBRState.twsState != BR_TWS_MASTER_CONNECTED 
		&& gBRState.secondState >= BR_CONNECTING)
	{
		Lpm_LockLpm(LINK_LPM_FLAG);
		Bt_SndCmdDisconnect();
	}	
#endif //SYNC_POWER_OFF
}

void Bt_SndCmdChargeFullPoweroff()
{
	IPC_TxControlCmd(BT_CMD_CHARGE_FULL_POWEROFF);
}

void Bt_SndCmdChargeDisconnect()
{
	IPC_TxControlCmd(BT_CMD_CHARGE_DISCONNECT);
}

void Bt_SndCmdDisconnect()
{
	IPC_TxControlCmd(BT_CMD_DISCONNECT);
}

void Bt_SndCmdStartDiscovery()
{
	IPC_TxControlCmd(BT_CMD_START_DISCOVERY);
}

void Bt_SndCmdStopDiscovery()
{
	Bt_ReleaseReconnectPhoneTimer();
	IPC_TxControlCmd(BT_CMD_STOP_DISCOVERY);
}


void Bt_SndCmdReconnect()
{
	// To workaround mem_plap will clear by some case
	Bt_SndCmdLoadInfo();
	IPC_TxControlCmd(BT_CMD_RECONNECT);
}

void Bt_SndCmdStartTwsPair()
{
	IPC_TxControlCmd(BT_CMD_ENTER_TWS_PAIR);
}

void Bt_SndCmdPlayPause()
{
	if (!Bt_CheckAVRCPConnected())
		return;
	IPC_TxControlCmd(BT_CMD_PLAY_PAUSE);
}

void Bt_SndCmdAcceptCall()
{
	IPC_TxControlCmd(BT_CMD_ACCEPT_CALL);
}

void Bt_SndCmdHangup()
{
	IPC_TxControlCmd(BT_CMD_HANG_UP);
}

void Bt_SndCmdRejectCall()
{
	IPC_TxControlCmd(BT_CMD_REJECT_CALL);
}

void Bt_SndCmdRedial()
{
	IPC_TxControlCmd(BT_CMD_REDIAL);
}

void Bt_SndCmdWaitMaster()
{
	IPC_TxControlCmd(BT_CMD_START_WAIT_MASTER);
}

void Bt_SndCmdTwsUser1()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_USER1);
}

void Bt_SndCmdTwsUser2()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_USER2);
}

void Bt_SndCmdTwsUser3()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_USER3);
}

void Bt_SndCmdTwsUser4()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_USER4);
}

void Bt_SndCmdtwsPwroff()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_POWR_OFF);
}

void Bt_SndCmdLoadInfo()
{
	IPC_TxControlCmd(BT_CMD_LOAD_INFO);
}

void Bt_SndCmdRecoverAudio()
{
	VP_cvsd_init();
	IPC_TxControlCmd(BT_CMD_RECOVER_AUDIO);
	#ifdef AUDIO_SYNC_DEF
	if(g_is_need_send_slave_reset_audio_cmd != 0)
	{
		g_is_need_send_slave_reset_audio_cmd = 0;
		Bt_SndCmdSlaveResetAudio();
	}
	else
	{
		if(0 != gHfpWorkingState)
		{
			Bt_SndCmdHFPStartSync();
		}
		else
		{
			Bt_SndCmdResetAudio();
		}
	}
	#endif //AUDIO_SYNC_DEF

}

void Bt_SndCmdBatRep()
{
	IPC_TxControlCmd(BT_CMD_REPORT_BAT);
}

void Bt_SndCmdWaitSlave()
{
	IPC_TxControlCmd(BT_CMD_START_WAIT_SLAVE);
}

void Bt_SndCmdEnableSniffMul()
{
	IPC_TxControlCmd(BT_CMD_ENABLE_SNIFF_MUL);
}

void Bt_SndCmdDisableSniffMul()
{
	IPC_TxControlCmd(BT_CMD_DISABLE_SNIFF_MUL);
}

void Bt_SndCmdEnterSniff()
{
	IPC_TxControlCmd(BT_CMD_ENTER_SNIFF);
}

void Bt_SndCmdExitSniff()
{
	IPC_TxControlCmd(BT_CMD_EXIT_SNIFF);
}

void Bt_SndCmdUpdateHFPState()
{
	IPC_TxControlCmd(BT_CMD_HFP_INDICATORS_UPDATE);
}

void TwsCB_Recon_Timeout(int params)
{
	VP_snd_voiceEvt(VP_POWERON);
	Bt_SndCmdStopReconTws();
	Bt_CheckReconnectAction();
}

void Bt_SndCmdStartReconTws()
{
	SYS_SetTimer(&gReconTimer, 150, TIMER_SINGLE, TwsCB_Recon_Timeout);
	IPC_TxControlCmd(BT_CMD_START_RECON_TWS);
}

void Bt_SndCmdSndIncPwr()
{
	IPC_TxControlCmd(BT_CMD_INC_POWER);
}

void Bt_ReleaseReconTimer()
{
	SYS_ReleaseTimer(&gReconTimer);
}


void BtCB_ReconnectPhoneTimeout(int params)
{
	Bt_SndCmdStopDiscovery();
	Bt_SndCmdReconnect();
}
void Bt_StartReconnectPhoneTimer()
{
	SYS_SetTimer(&gBtReconWorkTimer, gReconWorkTimeout, TIMER_SINGLE, BtCB_ReconnectPhoneTimeout);
}
void Bt_ReleaseReconnectPhoneTimer()
{
	SYS_ReleaseTimer(&gBtReconWorkTimer);
}

void Bt_SndCmdStopReconTws()
{
	IPC_TxControlCmd(BT_CMD_STOP_RECON_TWS);
}

void CB_SwitchToDisconnect(int params)
{
	Bt_SndCmdAcceptTwsSwitch();
}

void CB_SwitchToMaster(int params)
{
	Bt_SndCmdTwsSwitchToMaster();
}

void Bt_SndCmdAcceptTwsSwitch()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_ACCPET_SWITCH);
}

void Bt_SndSlavePwroff()
{
	SYS_SetTimer(&gSwithMasterTimer, 44, TIMER_SINGLE, CB_SwitchToDisconnect);
	Bt_SndCmdtwsPwroff();
}
	

void Bt_SndCmdTwsMasterSwitch()
{
	SYS_SetTimer(&gSwithMasterTimer, 44, TIMER_SINGLE, CB_SwitchToDisconnect);
	IPC_TxControlCmd(TWS_SYNC_CMD_MASTER_SWITCH);
}

void Bt_SndCmdTwsSwitchToMaster()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_SWITCH_TO_MASTER);
}

bool Bt_CheckHFPIsIncomming()
{
	return (*gpHFPCallState == HFP_INCOMMING);
}



void Bt_setIphoneAdvBdaddr()
{
	uint8_t temp;
	uint16_t adv_lap_addr= mem_le_adv_data+9;
	for(uint8_t i=0;i<6;i++)
	{
		temp = HREAD(mem_lap+i);
		HWRITE(adv_lap_addr+(5-i),temp);
	}
}

void Bt_setIphoneBoxClose()
{
	HWRITE(mem_le_adv_data+19,0x09);
}

void Bt_setIphoneBoxOPen()
{
	HWRITE(mem_le_adv_data+19,0x01);
}

bool Bt_CheckAVRCPConnected()
{
	uint16_t temp;
	uint8_t temp1;
	temp =  HREADW(mem_avr_l2capch_ptr);
	if (temp == 0)
		return false;
	temp+=1;
	temp1 = HREAD(temp);
	if (temp1 != 0x3f)
		return false;
	return true;
}

bool Bt_CheckA2DPConnected()
{
	uint16_t temp;
	uint8_t temp1;
	temp =  HREADW(mem_avdtpmedia_l2capch_ptr);
	if (temp == 0)
		return false;
	temp+=1;
	temp1 = HREAD(temp);
	if (temp1 != 0x3f)
		return false;
	return true;
}

void BT_twsCheckProfileConnected()
{
	if ((0 == HREAD(mem_tws_profile_disconnected_after_connected))
		&& (0 != HREAD(mem_tws_profile_connected)))
		return;

	if (0x09 != HREAD(mem_hf_state))
		return;

	if(!Bt_CheckA2DPConnected())
		return;

	if(!Bt_CheckAVRCPConnected())
		return;

	HWRITE(mem_tws_profile_disconnected_after_connected,0);

	if (0 != HREAD(mem_tws_profile_connected))
		return;
	HWRITE(mem_tws_profile_connected,1);

	if (0 != HREAD(mem_tws_state))
		return;

	Bt_SndCmdWaitSlave();
}

void BT_twsCheckAndUpdateProfileAfterProfileDisconnect(void)
{
	if (0 == HREAD(mem_tws_profile_connected))
		return;

	if (0x09 == HREAD(mem_hf_state))
		return;

	if(Bt_CheckA2DPConnected())
		return;

	if(Bt_CheckAVRCPConnected())
		return;

	HWRITE(mem_tws_profile_disconnected_after_connected,1);

	if(gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		if (gBRState.secondState >= BR_CONNECTING)
		{
			Bt_SndCmdTwsAllProfileDisconnected();
		}
	}
}
/**
 *    Use to monitor DAC buffer work state
 */
void Bt_DACBufferMonitor(void)
{
	uint8_t index = DEBUG_ROUND_BUFFER_INDEX_00;
	uint32_t start_addr = reg_map(0x11010);// DAC start addr = 0x11010
	uint32_t end_addr = reg_map(0x11010) + A2DP_DAC_BUFFER_SIZE;
	uint32_t write_addr = reg_map(HREADW(CORE_DAC_WPTR));
	uint32_t read_addr = reg_map(HREADW(CORE_DAC_RPTR));
	DEBUG_LOG_PRINT_ROUND_BUFFER(index, start_addr, end_addr, write_addr, read_addr);
}

void Bt_L2CAPQueueBufferMonitor(void)
{
	uint8_t index = DEBUG_ROUND_BUFFER_INDEX_01;
	uint32_t start_addr = 0;
	uint32_t end_addr = 4;
	uint32_t write_addr = HREAD(mem_current_queue_len);
	uint32_t read_addr = 0;
	DEBUG_LOG_PRINT_ROUND_BUFFER(index, start_addr, end_addr, write_addr, read_addr);
}


void Bt_RoundBufferMonitor(void)
{	
	//uint8_t temp;
	//temp = HREAD(0x4ffe);
	//if (temp ==0 )
	//	return;
	//while(1);
	//Bt_DACBufferMonitor();
	//Bt_L2CAPQueueBufferMonitor();
}

//#define RSSI_ADJUST_PHO 0x65
//#define RSSI_LOW_MUTU_PHO 0x85
//#define RSSI_LOW_MUTU_TWS_PHO 0x75
uint16_t testSum;
void Drv_check_rssi()
{
	uint16_t sum;
	uint8_t level;

	//check rssi is updated
	level = HREAD(mem_rssi_update);
	if (level == 0)
		return;
	HWRITE(mem_rssi_update,0);
	
	if (gBRState.secondState >= BR_CONNECTING)
	{
		sum = HREADW(mem_rssi_sum);
		sum = sum >>3;

		//adjust tx power
		if (sum <= RSSI_ADJUST_PHO){
			gRSSICount++;
			if (gRSSICount >=1000)
			{
				gRSSICount=0;
				level = HREAD(mem_tx_power);
				switch(level)
				{
					case TX_POWER_3DB:
						HWRITE(mem_tx_power,TX_POWER_0DB);
						break;
					default:
						HWRITE(mem_tx_power,TX_POWER_f5DB);
						break;
				}
			}
		}
		else{
			HWRITE(mem_tx_power,TX_POWER_3DB);
			gRSSICount = 0;
		}

		testSum = sum;
		if (sum >= RSSI_LOW_MUTU_PHO){
			gRSSIMuteCount++;
			if (gRSSIMuteCount >=60)
			{
				gRSSIMuteCount = 0;
				HWRITE(mem_rssi_low,1);
			}
		}
		else{
			HWRITE(mem_rssi_low,0);
			gRSSIMuteCount=0;
		}
	}
}

#ifdef AUDIO_SYNC_DEF
void Bt_SndCmdResetAudio(void)
{
	if (!Bt_checkBRIsConnected())
	{
		return;
	}
	if (0x07 != HREAD(mem_a2dp_state))//AVDTP_IDENTIFIER_START
	{
		return;
	}
	if(0 != gHfpWorkingState)
	{
		return;
	}
	if(HREAD(mem_enable_start_sync) != 0)
	{
		return;
	}
	if(gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		uint32_t clocke = HREADL(mem_start_play_clk);
		uint32_t seq = HREADW(mem_a2dp_seq_num);
		uint32_t currentQueueSize = HREAD(mem_media_cur_len);
		
		//Becouse our double ack function, and our master-slave transaction machine, here we need 3 buffer for safe.
		//HWRITEW(mem_target_start_seq, seq + currentQueueSize + 3);
		//HWRITE(mem_enable_start_sync, 1);
		IPC_TxControlCmd(TWS_SYNC_CMD_RESET_AUDIO);
	}
}

void Bt_SndCmdHFPStartSync(void)
{
	if (!Bt_checkBRIsConnected())
	{
		return;
	}
	if (1 != gHfpWorkingState)
	{
		return;
	}
	if(gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		IPC_TxControlCmd(TWS_SYNC_CMD_HFP_START_SYNC);
	}
}

void Bt_SndCmdSlaveResetAudio(void)
{
	if (!Bt_checkBRIsConnected())
	{
		return;
	}	
	if (0x07 != HREAD(mem_a2dp_state))//AVDTP_IDENTIFIER_START
	{
		return;
	}
	if(0 != gHfpWorkingState)
	{
		return;
	}
	if(gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
	{
		IPC_TxControlCmd(TWS_SYNC_CMD_SLAVE_RESET_AUDIO);
	}
}

void Bt_SndCmdMasterAudioProcessInfo(void)
{
	if (!Bt_checkBRIsConnected())
		return;
		
	if (0x07 != HREAD(mem_a2dp_state))//AVDTP_IDENTIFIER_START
		return;
	if(gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		IPC_TxControlCmd(TWS_SYNC_CMD_AUDIO_PROCESS_INFO);
	}
}



void tws_user_reset_audio_action(void)
{
	uint32_t clockn = HREADL(CORE_CLKN);
	uint32_t clocke = HREADL(mem_start_play_clk);
	uint32_t seq = HREADW(mem_target_start_seq);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_reset_audio_action: 0x%04X", LOG_POINT_B004, seq );
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_reset_audio_action: 0x%04X", LOG_POINT_B005, clocke >> 16);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_reset_audio_action: 0x%04X", LOG_POINT_B006, clocke >> 0);
	if (gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		Bt_TwsResetFlagUpdate(TWS_RESET_FLAG_NONE, clockn);
	}
	else if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
	{
	
	}
}

void tws_user_hfp_start_sync_action(void)
{
	uint32_t clocke = HREADL(mem_hfp_start_sync_clk);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_hfp_start_sync_action: 0x%04X", LOG_POINT_B020, clocke >> 16);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_hfp_start_sync_action: 0x%04X", LOG_POINT_B021, clocke >> 0);
	if (gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		
	}
	else if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
	{
	
	}
}
#define TWS_MEDIA_MISS_MATCH_LIMIT        (40)// unit 312.5 if clock diff big to 10ms, there may be something wrong, need request reset
uint8_t gClkMissMatch;
void tws_slave_check_process_clk_info(uint32_t clkLocal, uint32_t clkRemote) 
{
	uint32_t diffClk = 0;
	if(clkRemote > clkLocal)
	{
		diffClk = clkRemote - clkLocal;
	}
	else
	{
		diffClk = clkLocal - clkRemote;
	}

	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_slave_check_process_clk_info diffClk: 0x%04X", LOG_POINT_B015, diffClk);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_slave_check_process_clk_info diffClk: 0x%04X", LOG_POINT_B016, gClkMissMatch);
	
	if(diffClk > 1000)
	{
		// Clk overflow we didn't care
	}
	else if(diffClk > TWS_MEDIA_MISS_MATCH_LIMIT)
	{
		gClkMissMatch = 0;
		Bt_SndCmdSlaveResetAudio();
	}
	else if(diffClk > (TWS_MEDIA_MISS_MATCH_LIMIT/2))
	{
		gClkMissMatch += 5;
	}
	else if(diffClk > (TWS_MEDIA_MISS_MATCH_LIMIT/4))
	{
		gClkMissMatch += 1;
	}
	else
	{
		gClkMissMatch = 0;
	}

	if(gClkMissMatch > 20)
	{
		gClkMissMatch = 0;
		Bt_SndCmdSlaveResetAudio();
	}
}

void tws_user_process_audio_info(void)
{
	uint16_t seqLocal = HREADW(mem_process_a2dp_seq_num);
	uint32_t clkLocal = HREADL(mem_process_clk);
	uint16_t seqRemote = HREADW(mem_process_receive_a2dp_seq_num);
	uint32_t clkRemote = HREADL(mem_process_receive_clk);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_audio_info: 0x%04X", LOG_POINT_B007, seqLocal );
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_audio_info: 0x%04X", LOG_POINT_B008, clkLocal >> 16);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_audio_info: 0x%04X", LOG_POINT_B009, clkLocal >> 0);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_audio_info: 0x%04X", LOG_POINT_B00A, seqRemote);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_audio_info: 0x%04X", LOG_POINT_B00B, clkRemote >> 16);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_audio_info: 0x%04X", LOG_POINT_B00C, clkRemote >> 0);
	if (gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		Bt_SndCmdMasterAudioProcessInfo();
	}
	else if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
	{
		if(seqLocal == seqRemote) 
		{
			tws_slave_check_process_clk_info(clkLocal, clkRemote);
		}
	}
}

void tws_user_process_remote_audio_info(void)
{
	uint16_t seqLocal = HREADW(mem_process_a2dp_seq_num);
	uint32_t clkLocal = HREADL(mem_process_clk);
	uint16_t seqRemote = HREADW(mem_process_receive_a2dp_seq_num);
	uint32_t clkRemote = HREADL(mem_process_receive_clk);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_remote_audio_info: 0x%04X", LOG_POINT_B00D, seqLocal );
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_remote_audio_info: 0x%04X", LOG_POINT_B00E, clkLocal >> 16);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_remote_audio_info: 0x%04X", LOG_POINT_B00F, clkLocal >> 0);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_remote_audio_info: 0x%04X", LOG_POINT_B010, seqRemote);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_remote_audio_info: 0x%04X", LOG_POINT_B011, clkRemote >> 16);
	DEBUG_LOG(LOG_LEVEL_CORE, "IPC" ,"tws_user_process_remote_audio_info: 0x%04X", LOG_POINT_B012, clkRemote >> 0);
	if (gBRState.twsState == BR_TWS_MASTER_CONNECTED)
	{
		
	}
	else if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
	{
		if(seqLocal == seqRemote) 
		{
			tws_slave_check_process_clk_info(clkLocal, clkRemote);
		}
	}
}
uint8_t g_tws_reset_flag;
uint32_t g_tws_reset_last_detect_buffer_empty_clock;
uint32_t g_last_detect_buffer_empty_clock;
bool Bt_CheckIsAudioBufferEmpty(void)
{
	uint32_t wptr = HREADW(CORE_DAC_WPTR);
	uint32_t rptr = HREADW(CORE_DAC_RPTR);
	
	// Becouse when dac buffer detect empty, it not realy meanings empty
	uint32_t clockn = HREADL(CORE_CLKN);

	if(0 != HREAD(mem_first_buffer_cache)
		|| 0 != HREAD(mem_enable_start_sync)
		) 
	{
		return false;
	}
	if((wptr - rptr) == 0) 
	{
		DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_CheckIsAudioBufferEmpty: 0x%04X", LOG_POINT_B001, clockn);
		if(g_tws_reset_flag != TWS_RESET_FLAG_NONE)
		{
			return true;
		}
		if(g_last_detect_buffer_empty_clock == INVALID_CLK)
		{
			g_last_detect_buffer_empty_clock = clockn;
		}
		else
		{
			// TODO: Think clk overflow
			if((clockn - g_last_detect_buffer_empty_clock) > DAC_BUFFER_EMPTY_DETECT_CLK_DIFF)
			{
				//g_last_detect_buffer_empty_clock = INVALID_CLK;
				return true;
			}
		}
	}
	else
	{
		g_last_detect_buffer_empty_clock = INVALID_CLK;
	}
	return false;
}

void Bt_TwsResetFlagUpdate(uint8_t flag, uint32_t clockn)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_TwsResetFlagUpdate: 0x%04X", LOG_POINT_B002, flag);
	DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_TwsResetFlagUpdate: 0x%04X", LOG_POINT_B003, clockn);
	g_tws_reset_flag = flag;
	g_tws_reset_last_detect_buffer_empty_clock = clockn;
}
void Bt_TwsResetFuncInit(void)
{
	uint32_t clockn = HREADL(CORE_CLKN);
	DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_TwsResetFlagUpdate: 0x%04X", LOG_POINT_B000, clockn);
	g_last_detect_buffer_empty_clock = INVALID_CLK;
	Bt_TwsResetFlagUpdate(TWS_RESET_FLAG_NONE, INVALID_CLK);
}
void Bt_TwsDetectAudioBufferEmpty(void)
{
	if (!Bt_CheckAVRCPConnected())
	{
		return;
	}
	if (0x07 != HREAD(mem_a2dp_state))//AVDTP_IDENTIFIER_START
	{
		return;
	}
	if(AVD_AUDIO_ALLOW != HREAD(mem_audio_allow)) 
	{
		//Bt_TwsResetFlagUpdate(TWS_RESET_FLAG_NONE, INVALID_CLK);
		return;
	}
	else
	{
		uint32_t clockn = HREADL(CORE_CLKN);
		// TODO: judge clk overflow
		uint32_t clk_diff = clockn - g_tws_reset_last_detect_buffer_empty_clock;
		// To work
		// Only master check buffer empty
		if(gBRState.twsState == BR_TWS_MASTER_CONNECTED)
		//if (HREAD(REG_B_TWS_ROLE) == TWS_SET_MASTER)
		{
			if(!Bt_CheckIsAudioBufferEmpty())
			{
				switch(g_tws_reset_flag) 
				{
					case TWS_RESET_FLAG_NEED_RESET:
						if(clk_diff > DAC_BUFFER_EMPTY_RECOVER_CLK_DIFF)
						{
							Bt_TwsResetFlagUpdate(TWS_RESET_FLAG_PROCESS_RESET, clockn);
							// Throw all local media packet, or do nothing
							Bt_SndCmdResetAudio();
						}
						break;
				
					case TWS_RESET_FLAG_PROCESS_RESET:
					case TWS_RESET_FLAG_NONE:
					default:
						break;
				}
			}
			else
			{
				switch(g_tws_reset_flag) 
				{
					case TWS_RESET_FLAG_NONE:
					case TWS_RESET_FLAG_NEED_RESET:
						Bt_TwsResetFlagUpdate(TWS_RESET_FLAG_NEED_RESET, clockn);
						break;
					default:
					case TWS_RESET_FLAG_PROCESS_RESET:
						break;
				}
			}
		}
		else
		if(gBRState.twsState == BR_TWS_SLAVE_CONNECTED)
		//if(HREAD(REG_B_TWS_ROLE) == TWS_SET_SLAVE)
		{

		}
	}

}

#endif //#ifdef AUDIO_SYNC_DEF

void Bt_SndCmdWaitTwsSlave()
{
	IPC_TxControlCmd(TWS_SYNC_CMD_WAIT_TWS_SLAVE);
}

void Bt_SndCmdTwsAllProfileDisconnected(void)
{
	IPC_TxControlCmd(TWS_SYNC_CMD_ALL_PROFILE_DISCONNECTED);
}
void Bt_SndCmdTwsDropOnePacket(void)
{
	IPC_TxControlCmd(TWS_SYNC_CMD_DROP_ONE_PACKET);
}


void Bt_SndCmdBackward()
{
	IPC_TxControlCmd(BT_CMD_BACKWARD);
}

void Bt_SndCmdForward()
{
	IPC_TxControlCmd(BT_CMD_FORWARD);
}

void Bt_AdjustVol(bool isVolUp)
{
	uint8_t temp;
	temp = HREAD(mem_audio_vol) /8;
	if(isVolUp)//Volume up
	{
		if (temp < 16)
		{
			temp++;
			HWRITE(mem_audio_vol,temp*8);
			//Bt_SndCmdAdjustVlm();
		}
		else
		{
		       temp = 16;
		       HWRITE(mem_audio_vol,temp*8);
		       //Bt_SndCmdAdjustVlm();
		       VP_play_voice(VP_VLM_MAX);
			//max vlm
		}
	}
	else
	{
		if (temp > 0)
		{
			temp--;
			HWRITE(mem_audio_vol,temp*8);
			//SSBt_SndCmdAdjustVlm();
		}
	}
}

bool Bt_A2dpCheckStart(void)
{
	//Check a2dp work state
	if (!Bt_CheckAVRCPConnected())
	{
		return false;
	}
	if (0x07 != HREAD(mem_a2dp_state))//AVDTP_IDENTIFIER_START
	{
		return false;
	}

	return true;
}


void Bt_ProcessKeyFunc(KEY_SUPPORT_FUNC func)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "TWS" ,"Bt_ProcessKeyFunc: 0x%04X", LOG_POINT_C000, func);
	if((gBRState.topState != BR_POWER_ON)
		&& (func != KEY_FUNC_POWER_ON_OFF))
	{
		return;
	}
	switch(func)
	{
		case KEY_FUNC_MUSIC_PLAY_PAUSE:
			Bt_SndCmdPlayPause();
			break;
		case KEY_FUNC_MUSIC_FORWARD:
			Bt_SndCmdForward();
			break;
		case KEY_FUNC_MUSIC_BACKWARD:
			Bt_SndCmdBackward();
			break;
		case KEY_FUNC_MUSIC_VOL_UP:
			Bt_AdjustVol(true);
			break;
		case KEY_FUNC_MUSIC_VOL_DOWN:
			Bt_AdjustVol(false);
			break;
		case KEY_FUNC_HFP_ACCEPT_CALL:
			Bt_SndCmdAcceptCall();
			break;
		case KEY_FUNC_HFP_HANGUP:
			Bt_SndCmdHangup();
			break;
		case KEY_FUNC_HFP_REDIAL:
			Bt_SndCmdRedial();
			break;
		case KEY_FUNC_POWER_ON_OFF:
			if (gBRState.topState == BR_WAIT_POWER_ON)
			{
				Bt_PowerOn();
				Bt_PowerOnKeyRelease();
			}
			else if (gBRState.topState == BR_POWER_ON)
			{
				Bt_SndCmdWaitPwroff();
			}
			break;
		case KEY_FUNC_TWS_PAIR:
			Bt_SndCmdStartTwsPair();
			break;
		default:
			break;
	}
}


