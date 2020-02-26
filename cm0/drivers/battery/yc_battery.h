#ifndef DRIVERS_BAT_YC_BAT_H_
#define DRIVERS_BAT_YC_BAT_H_

#include "yc11xx.h"

typedef enum
{
	CHG_CHARGING=0,
	CHG_FULL,
	CHG_STOP
}CHG_STATUS;

#define CHARGER_FULL_GPIO GPIO_5
//#define CHARGER_REMAIND_COUNT 12000 //20min
#define CHARGER_FULL_COUNT 220
#define CHARGER_CHECK_VOL_COUNT 3000
#define CHARGER_DISCONNECT_COUNT 11


#define BAT_5V_REF_REG mem_5v_adc_hvin_data
#define BAT_3V_REF_REG mem_3v_adc_hvin_data
#define BAT_ADC_VALUE_REG mem_adc_current_value

#define BAT_READ_ADC_COUNT  10 //10 8s 

#define BAT_CHECK_ADC_COUNT_CHARGE  55 
#define BAT_CHECK_ADC_COUNT_SHORT  100 
#define BAT_CHECK_ADC_COUNT_LONG 2000 // 250

#define BAT_LEVEL_NUM 11

#define BAT_LEVEL_SAMPLE_ARRAY_NUM 8

void Bat_EnterChargerIn();
void Bat_ExitChargerIn();
void Bat_CBLevelCheck(int params);
void Bat_polling();
void Bat_CbEnterPowerOff(int params);
void Bat_CbChargerFull(int params);
void Bat_ReadAdc();
void Bat_init();
void Bat_reInit();
void Bat_ClcGpio();
void Bat_setChargerParams();
void Bat_CalLevel(int params);
void SendBatCalLevel();
uint16_t MedianFilter(uint16_t* pData,int nSize);
void Bat_CbChargerIn(int params);


#endif //DRIVERS_BAT_YC_BAT_H_
