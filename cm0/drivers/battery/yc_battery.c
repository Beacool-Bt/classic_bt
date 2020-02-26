#include "yc_battery.h"
#include "drv_bt.h"
#include "yc_led.h"
#include "yc_timer.h"
#include "yc11xx_gpio.h"
#include "yc_drv_common.h"
#include "yc_voiceprompt.h"
#include "yc_keypad.h"
#include "bt_hfp.h"
#include "drv_debug.h"
#include "yc11xx_qspi.h"


#define OTP_ADC_FLAG 0xaa55
SYS_TIMER_TYPE gCharger_Timer;
SYS_TIMER_TYPE gBatlevel_Timer;
SYS_TIMER_TYPE gReadBat_Timer;
SYS_TIMER_TYPE gChargerIn_timer;

//uint8_t gAuto_poweroff_count = 0;
uint8_t gSendBat_count = 0;
uint8_t gCheck_count=0;
uint8_t gLowPowerVoice_count=0;
uint8_t gCheckBatFull_count=0;

uint8_t gBatLevel[BAT_LEVEL_NUM] = 
{
	0x77, // 119-->4.19 level9
	0x6a, //level8-->4.06v
	0x62, //level7-->3.98v
	0x5c, //level6-->3.92v
	0x57, //level5-->3.87v
	0x52 , //level4-->3.82v
	0x4f, //level3-->3.79
	0x4b, //level2-->3.75
	0x46, //level 1-->3.70
	0x28, //level 0--->3.40
	0x14 //3.20,auto power off
};

//test variable
CHG_STATUS gChgStatus;
uint16_t gBat3vRef;
uint16_t gBat5vRef;
uint16_t gAdcVal[BAT_LEVEL_SAMPLE_ARRAY_NUM];
uint16_t gAdcAvr;
uint8_t CurAdcVal=0xff;
uint8_t adcVal,adcAalTemp=0;

void Bat_init()
{
    uint16_t opt;
	GPIO_SetInput(CHARGER_FULL_GPIO);
	Bat_setChargerParams();
	gChgStatus =CHG_CHARGING;
	opt = HREADW(mem_otp_adc_flag);
	if (opt == OTP_ADC_FLAG){
		gBat3vRef = HREADW(BAT_3V_REF_REG);
		gBat5vRef = HREADW(BAT_5V_REF_REG);
	}
	else{
		gBat3vRef = 0x4ec2;
		gBat5vRef = 0x5a80;
	}
	Bat_polling();
	gAdcAvr = HR_REG_16BIT(reg_map(BAT_ADC_VALUE_REG));
	Bat_CalLevel(0);
	HWRITE(mem_bat_level, CurAdcVal);
 	SYS_SetTimer(&gReadBat_Timer,BAT_READ_ADC_COUNT,
 		TIMER_CYCLE|TIMER_TYPE_BIT,Bat_CBLevelCheck);
}

void Bat_reInit()
{
	Bat_setChargerParams();
	GPIO_SetInput(CHARGER_FULL_GPIO);
}

void Bat_ClcGpio()
{
	// Â©µç
	GPIO_SetInput(CHARGER_FULL_GPIO|GPIO_ACTIVE_BIT);
}

void Bat_EnterChargerIn()
{
	DEBUG_LOG_2(LOG_LEVEL_CORE, "UI" ,"Bat_EnterChargerIn: 0x%04X", LOG_POINT_A200, gBRState.topState, gBRState.secondState);

	gBRState.topState = BR_CHARGER_IN;
//	gBRState.secondState = BR_IDLE;
//	gBRState.twsState = BR_IDLE;
	led_clear_led();
	Bt_ProcessLedFunc(LED_EVT_CHARGING);
	Lpm_LockLpm(CHARGER_LPM_FLAG);

//disable dac ,adc
	HWRITE(0x8980,0xff);
	HWRITE(0x897f,0x30);
	HWRITE(0x897e,0);
	HWRITE(0x897a,0x7f);
			
}


void Bat_ExitChargerIn()
{
	DEBUG_LOG_2(LOG_LEVEL_CORE, "UI" ,"Bat_ExitChargerIn: 0x%04X", LOG_POINT_A201, gBRState.topState, gBRState.secondState);
	SYS_ReleaseTimer(&gCharger_Timer);
	Lpm_unLockLpm(CHARGER_LPM_FLAG);
	led_clear_led();
	Bt_SndCmdPwroff();
}

void Bat_CbChargerFull(int params)
{
	uint8_t val;
	DEBUG_LOG_2(LOG_LEVEL_CORE, "UI" ,"Bat_CbChargerFull: 0x%04X", LOG_POINT_A202, gBRState.topState, gBRState.secondState);
	//charger full
	gChgStatus = CHG_FULL;
	
	val = HREAD(0x8976);
	if ((val & 0x07) != 0)
		val=val-3;
	HWRITE(0x8976,val);
	
	led_clear_led();
	Bt_ProcessLedFunc(LED_EVT_CHARGE_FULL);
//	led_send_bgevt(LED_BLUE_ALWAY_ON);
	SYS_SetTimer(&gCharger_Timer,CHARGER_FULL_COUNT,
		TIMER_SINGLE|TIMER_TYPE_BIT,Bat_CbEnterPowerOff);
}

void Bat_CbEnterPowerOff(int params)
{
	DEBUG_LOG_2(LOG_LEVEL_CORE, "UI" ,"Bat_CbEnterPowerOff: 0x%04X", LOG_POINT_A203, gBRState.topState, gBRState.secondState);
	Bt_SndCmdChargeFullPoweroff();
	GPIO_SetWakeupByCurrentState(CHARGER_DETECT_GPIO);
	Bat_ExitChargerIn();
}

void Bat_CbCheckVol(int params)
{
	if( (gBRState.topState == BR_CHARGER_IN) && ( gChgStatus != CHG_FULL)   )
	{
		if(adcVal - adcAalTemp >=2 )
		{
			adcAalTemp = adcVal;		
			gCheckBatFull_count = 0;
		}
		else
		{
			gCheckBatFull_count++;
			if(gCheckBatFull_count >= 4)
			{
				SYS_ReleaseTimer(&gCharger_Timer);
				Bat_CbChargerFull(0);
			}
		}
	}
	else
		gCheckBatFull_count = 0;	
}


void Bat_CbChargerIn(int params)
{
	Bat_EnterChargerIn();
	gCheckBatFull_count = 0;
	SYS_SetTimer(&gCharger_Timer,CHARGER_CHECK_VOL_COUNT,
			TIMER_CYCLE|TIMER_TYPE_BIT,Bat_CbCheckVol);
}

void Bat_polling()
{	
	volatile int i;
	if (SYS_TimerisExist(&gChargerIn_timer))
		return;
	
	if(gChgStatus != CHG_STOP )
	{
		if (GPIO_GetInputStatus(CHARGER_DETECT_GPIO)){
			//charge in
			if (gBRState.topState != BR_CHARGER_IN ){
				if(gBRState.topState == BR_POWER_ON)
				{
					if (gBRState.twsState == BR_TWS_MASTER_CONNECTED)
					{
						Lpm_LockLpm(CHARGER_LPM_FLAG);
						SYS_SetTimer(&gChargerIn_timer,125,
							TIMER_SINGLE,Bat_CbChargerIn);
						Bt_SndCmdTwsMasterSwitch();
						return;
					}
					else if (gBRState.twsState == BR_TWS_SLAVE_CONNECTED){
						Lpm_LockLpm(CHARGER_LPM_FLAG);
						SYS_SetTimer(&gChargerIn_timer,125,
							TIMER_SINGLE,Bat_CbChargerIn);
						Bt_SndSlavePwroff();
						return;
					}
					else if (gBRState.twsState != BR_TWS_MASTER_CONNECTED 
						&& gBRState.secondState >= BR_CONNECTING)
					{
						Bt_SndCmdDisconnect();
						Bat_CbChargerIn(0);
					}	
					else
					{
						Bt_SndCmdStopDiscovery();
						Bt_ReleaseReconTimer();
						Bat_CbChargerIn(0);
					}
				}
				else	
				{
					Bt_ReleaseReconTimer();
					Bt_SndCmdStopDiscovery();
					Bat_CbChargerIn(0);
				}
	
				if(gBRState.twsState == BR_TWS_RECON)
					Bt_SndCmdStopReconTws();
			} 

		if (gChgStatus == CHG_FULL)
			return;

		if (GPIO_GetInputStatus(CHARGER_FULL_GPIO))
		{
/*			if (!SYS_TimerisExist(&gCharger_Timer))
			{
				SYS_SetTimer(&gCharger_Timer,CHARGER_REMAIND_COUNT,
					TIMER_SINGLE|TIMER_TYPE_BIT,Bat_CbChargerFull);
			}
*/			
			//delay 300us
			 i=60;
			while(i)
			{
				if (!GPIO_GetInputStatus(CHARGER_FULL_GPIO))
						break;
					if(!GPIO_GetInputStatus(CHARGER_DETECT_GPIO))
					break;
				hw_delay();
				i--;
			}
			if (i == 0 && adcVal >= 110)
			{	//charger full
				SYS_ReleaseTimer(&gCharger_Timer);
				Bat_CbChargerFull(0);
			}
		}

		if(HREAD(mem_bat_level) <1 )
		{
			GPIO_SetOut(CHARGER_DETECT_GPIO,false);	
			for(i=30000;i>0;i--);
			HWRITE(0x8974,0x0f);		
			GPIO_SetInput(CHARGER_DETECT_GPIO);
		}
		else
			HWRITE(0x8974,0x0c);		

	}
	else{
		//charger exit
		if (gBRState.topState == BR_CHARGER_IN )
		{
				gChgStatus = CHG_STOP;
				Bt_SndCmdChargeDisconnect();	
				GPIO_SetWakeup(CHARGER_FULL_GPIO&GPIO_NUM_MASK);
				Bat_ExitChargerIn();
			}
		}
	}	
}


void Bat_CalLevel(int params)
{
	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"Bat_CalLevel: 0x%04X", LOG_POINT_A205, gAdcAvr);
	if (gAdcAvr < gBat3vRef)
	{
		//auto power off battery
//		gAuto_poweroff_count++;
//		if (gAuto_poweroff_count<2)
//			return;
		CurAdcVal = 0;
		if (gBRState.topState != BR_CHARGER_IN)
			//Bt_SndCmdWaitPwroff();
			return;
	}
	else
	{
		adcVal = ((gAdcAvr-gBat3vRef)*200)/(gBat5vRef - gBat3vRef);

		for(int i=0;i<BAT_LEVEL_NUM-1;i++)
		{
			if (adcVal > gBatLevel[i]){
//				gAuto_poweroff_count = 0;
				CurAdcVal = BAT_LEVEL_NUM-2-i;
				SendBatCalLevel();
				gLowPowerVoice_count = 0;
				return;
			}
		}
	}
	
	CurAdcVal = 0;
	SendBatCalLevel();
	if (gBRState.topState == BR_CHARGER_IN)
		return;
		
	if (gBRState.topState != BR_POWER_ON )
		return;

	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"Bat_CalLevel error state: 0x%04X", LOG_POINT_A206, adcVal);
	if (adcVal <= gBatLevel[BAT_LEVEL_NUM-2] && adcVal >=gBatLevel[BAT_LEVEL_NUM-1])
	{
//		gAuto_poweroff_count =0;
		//low_bat
		if(gLowPowerVoice_count == 0)
		{
			gLowPowerVoice_count = 7;   //8*BAT_LEVEL_SAMPLE_ARRAY_NUM*BAT_READ_ADC_COUNT; 
			VP_snd_voiceEvt(VP_LOWBAT);
		}
		else
			gLowPowerVoice_count --;
		Bt_ProcessLedFunc(LED_EVT_LOW_POWER);
		//Bt_SndCmdBatRep();
	}
	else if(adcVal <gBatLevel[BAT_LEVEL_NUM-1])
	{
//		gAuto_poweroff_count++;
//		if (gAuto_poweroff_count<2)
//			return;
		//auto power off level
		//Bt_SndCmdWaitPwroff();
//		while(1);
	}
}


void SendBatCalLevel()
{
	uint8_t LastAdcVal;
	LastAdcVal = HREAD(mem_bat_level);
	DEBUG_LOG_2(LOG_LEVEL_CORE, "UI" ,"SendBatCalLevel error state: 0x%04X", LOG_POINT_A208, LastAdcVal, CurAdcVal);
	if(LastAdcVal != CurAdcVal) 
	{
		gSendBat_count++;
		if (gSendBat_count<3)
			return;
		gSendBat_count=0;
		HWRITE(mem_bat_level, CurAdcVal);
		Bt_ReportBattery();
	}
}


void Bat_CBLevelCheck(int params)
{
	uint16_t adc_value = HR_REG_16BIT(reg_map(BAT_ADC_VALUE_REG));
	if(gCheck_count  >= BAT_LEVEL_SAMPLE_ARRAY_NUM)
	{
		gCheck_count = 0;
	}
	gAdcVal[gCheck_count] = adc_value;
//	if (gBRState.topState == BR_CHARGER_IN)
//		return;
	DEBUG_LOG(LOG_LEVEL_CORE, "UI" ,"Bat_CBLevelCheck: 0x%04X", LOG_POINT_A204, adc_value);
	gCheck_count++;
	if(gCheck_count  < BAT_LEVEL_SAMPLE_ARRAY_NUM)
	{
		return;
	}

 	
	gAdcAvr = MedianFilter(gAdcVal, BAT_LEVEL_SAMPLE_ARRAY_NUM);
	gCheck_count = 0;

	Bat_CalLevel(0);

}



void Bat_setChargerParams()
{
	uint8_t gc,con;
	gc = HREAD(mem_charge_otp_data_gc);
	HWRITE(0x8974,0x0c);		
	if (gc != 0 &&  gc <= 7){
		con = HREAD(0x8976);
		con=con&0xf8;
		con=con|gc;
		HWRITE(0x8976,con);
		gc = HREAD(mem_charge_otp_data_con);
		HWRITE(0x8975,gc);		
	}
	else{
		con = HREAD(0x8976);
		con=con&0xf8;
		con=con|5;
		HWRITE(0x8976,con);
		HWRITE(0x8975,10);
	}
}

/**
*  Median Average Filtering
* @param pData:Data without filtering
* @param nSize:Data Size
* @return:filter value
*/
uint16_t MedianFilter(uint16_t* pData,int nSize)
{
    uint16_t max,min;
    int sum;
    if(nSize>2)
    {
        max = pData[0];
        min = max;
        sum = 0;
        for(int i=0;i<nSize;i++)
        {
            sum += pData[i];            
            if(pData[i]>max)
            {
                max = pData[i];   //get max
            }

            if(pData[i]<min)
            {
                min = pData[i];   //get min
            }
        }

        sum = sum-max-min;       //sub min and max
        return sum/(nSize-2); //judge ave value   
    }

    return 0;
}

