#include<string.h>
#include<stdio.h>
#include <math.h>
#include "yc_timer.h"
#include "yc_keypad.h"
#include "yc_RemoteCtr.h"
#include "yc11xx.h"
#include "ipc.h"
#include "Drv_bt.h"
#include "yc_drv_common.h"
#include "yc_led.h"
#include "yc11xx_gpio.h"
#include "yc11xx_qspi.h"
#include "yc_voiceprompt.h"
#include "yc_battery.h"
#include "drv_debug.h"
#include "fir_8to48.h"
#include "bt_hfp.h"
#include "yc11xx_wdt.h"
#include "drv_media.h"

//#define CHECK_ADC_DAC
#define HW_WATCH_DOG_FUNC

tIPCHandleCb gTIPCHandleCb[IPC_TYPE_NUM]=
{
	0,
	IpcDefaultCallBack,//cmd
	Bt_EvtCallBack,//evt
	IpcDefaultCallBack,//hid
	//Bt_SPPCallBack,//spp
	IpcDefaultCallBack,
	Bt_BleCallBack,//ble
	IpcDefaultCallBack,//24g
	Bt_RFCOMMCallBack,//mesh
	IpcDefaultCallBack,//mesh
	IpcDefaultCallBack,//mesh
	Bt_A2DPCallBack,//a2dp
	Bt_HFPCallBack,//hfp
	Bt_TWSCallBack,//tws
};

#ifdef QSPI_TEST
uint8_t array[40];
void Qspi_test()
{
	//uint8_t *array = (uint8_t *)reg_map(mem_nv_data);;
	for (int i =0;i < 30;i++)
	{
		array[i] = 0x88;
	}
	
	QSPI_Init();
	setRamType(MRAM);
	//Bt_UpdateDeviceInfo();
	QSPI_SectorEraseFlash(DEVICE_INFO_BASE);
	QSPI_WriteFlashData(DEVICE_INFO_BASE,sizeof(BT_REC_INFO),(uint8_t *)array);
	for (int i =0;i < sizeof(BT_REC_INFO);i++)
	{
		array[i] = 0;
	}
	QSPI_ReadFlashData(DEVICE_INFO_BASE,sizeof(BT_REC_INFO),(uint8_t *)array);
	while(1);
}
#endif


#ifdef VP_TEST
void vp_test()
{
	QSPI_Init();
	VP_init();
	VP_snd_voiceEvt(VP_POWERON);
	VP_snd_voiceEvt(VP_POWERON);
	VP_snd_voiceEvt(VP_POWERON);
	while(1)
	{	
		VP_polling_Queue();
	}
}
#endif //VP_TEST

void check_adc_value();

void check_adc_init()
{
	uint8_t tem;
	tem = HREAD(mem_adac_init);
	if (tem == 1)
	{
		tem = HREAD(0x897f);
		tem = tem|0x01;
		HWRITE(0x897f,tem);
	
		HWRITE(0x897e,0xf3);	
		HWRITE(0x897e,0xE3);
		for (int j =100;j>0;j--)
		{
			int i = 10000;
			while(i--);
		}
		HWRITE(0x897e,0xf3);
		HWRITE(mem_adac_init,2);
	}
}

void main()
{	
	//ipc init
	IPC_init(&gTIPCHandleCb);

	//debug info
	DEBUG_INIT();
#ifdef HW_WATCH_DOG_FUNC
	//Watch Dog
	WDT_Enable();
#endif
	//hfp test
	//HFP_test();
	//while(1);

	//HWRITE(0x8097,62);

	while (1) {
		//For respin log print
		DEBUG_POLLING_PRINT();
#ifdef HW_WATCH_DOG_FUNC
		WDT_Kick();
#endif
		Bt_RoundBufferMonitor();
		switch (HREAD(IPC_MCU_STATE))
		{
			case IPC_MCU_STATE_RUNNING:
				//always first init bt
				IPC_HandleRxPacket();
				//Bt_watchdog();
				if (gBRState.topState != BR_POWER_OFF){
					SYS_timerPolling();
					keypad_polling_keyQueue();
					led_polling_Queue();
					VP_polling_Queue();
					Bt_CheckWaitPwroff();
					BT_twsCheckProfileConnected();
					#ifdef SCO_48K_FIR
					sco_fir_polling();
					#endif //SCO_48K_FIR
					Bat_polling();
					Drv_check_rssi();
					check_adc_init();
					Drv_media_polling();
				}
				Lpm_unLockLpm(M0_LPM_FLAG);
				break;
			case IPC_MCU_STATE_LMP:
				if (IPC_IsTxBuffEmpty()
					&& DEBUG_LOG_BUFFER_IS_EMPTY()){
					OS_ENTER_CRITICAL();
					Bt_ActionBeforeLpm();
					HWRITE(IPC_MCU_STATE,IPC_MCU_STATE_STOP);
				}
				else{
					HWRITE(IPC_MCU_STATE,IPC_MCU_STATE_RUNNING);
				}
				break;

			case IPC_MCU_STATE_HIBERNATE:
				OS_ENTER_CRITICAL();
				Bt_ActionBeforeHibernate();
				HWRITE(IPC_MCU_STATE,IPC_MCU_STATE_STOP);
				break;
			case IPC_MCU_STATE_STOP:
				break;
		}
		//hw_delay();
	};
}






