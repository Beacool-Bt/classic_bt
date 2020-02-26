#ifndef  _DRV_IPC_H_
#define _DRV_IPC_H_

#include <stdio.h>
#include "yc_timer.h"
#include "yc11xx.h"
#include "yc_keypad.h"
#include "yc_RemoteCtr.h"


#define TWS_USER_INTERFACE_HSC_I9S

#define SCO_48K_FIR

#define DEVICE_INFO_BASE 0x07f000
//#define DEVICE_INFO_BASE 0x1ff84
#define REG_B_TWS_ROLE mem_tws_role


#define REVERSE_3BYTE_DEFINE(a) ((a>>16)+(a&0xff00)+((a&0xff)<<16))  
//#define REVERSE_2BYTE_DEFINE(a,b)

#define CODE_START_FLASHADDR1 0x1003
#define CODE_START_FLASHADDR2 0X18003


#define	OTA_EVT_START  	0xc0
#define	OTA_EVT_DATA      0xc1
#define	OTA_EVT_END       0Xc2
#define	OTA_EVT_RESET   	0Xff

#define OTA_EVT 0xBA
#define OTA_CMD 0xAB
#define OTA_BLE_WRITE_HANDLE 0x0006
#define OTA_BLE_NOTIFY_HANDLE 0x0003
#define OTA_END_CHECK_OK 0X55
#define OTA_END_CHECK_FAIL 0XFF



/**
0 mem_nv_data
1 mem_tws_record_state
6 mem_tws_peer_lap
16 mem_tws_link_key
6 mem_tws_master_lap
6 mem_tws_ack_addr
1 mem_tws_role
**/
typedef struct{
	uint8_t sRecordState;
	uint8_t sPeerBDaddr[6];
	uint8_t sLinkKey[16];
	uint8_t sMasterBDaddr[6];
	uint8_t sTwsAckAddr[6];
	uint8_t sTwsrole;
	uint16_t sCRC;
}BT_REC_INFO;


#define  TWS_SET_NONE 0x00
#define TWS_SET_SLAVE 0x33
#define TWS_SET_MASTER 0x55

typedef enum
{
	BR_POWER_OFF=0,
	BR_WAIT_POWER_OFF,
	BR_CHARGER_IN,
	BR_WAIT_POWER_ON,
	BR_POWER_ON,
	BR_IDLE,

	BR_CONNECTABLE=6,
	BR_DISCOVERABLE,
	BR_CONNECTABLE_DISCOVERABLE,
	BR_CONNECTING,
	BR_CONNECTED,

	BR_HFP_INCOMMING=0x0b,
	BR_HFP_OUTGOING,
	BR_HFP_CALLACTIVE,
	BR_HFP_CALLACTIVE_WITHOUT_SCO,
	BR_HFP_CALLIMG,
	BR_HFP_CALLOGG,
	BR_HFP_CALMULTY,
	
	BR_TWS_PAIRING=0x12,
	BR_TWS_RECON,
	BR_TWS_WAIT_SLAVE=0x14,
	BR_TWS_WAIT_MASTER=0x15,
	BR_TWS_SLAVE_CONNECTED,
	BR_TWS_MASTER_CONNECTED=0x17,
	BR_STATE_NUM,
}BR_STATE_TYPE;

/**
#define AUTO_POWEROFF_WAKEUP 30 //uint20ms
#define AUTO_POWEROFF_IDLE    3000 // uint100ms
#define AUTO_POWEROFF_WAIT 50 //uint20ms
#define AUTO_POWEROFF_DISCOVERABLE 1800 // uint100
#define AUTO_POWEROFF_CONECTED 6000 // uint100
**/

#define AUTO_POWEROFF_WAKEUP 30 //uint20ms
#define AUTO_POWEROFF_IDLE    3000 // uint100ms
#define AUTO_POWEROFF_WAIT 50 //uint20ms
#ifdef TWS_USER_INTERFACE_HSC_I9S
#define AUTO_POWEROFF_DISCOVERABLE 2400 // uint100
#else
#define AUTO_POWEROFF_DISCOVERABLE 3000 // uint100
#endif
#define AUTO_POWEROFF_CONECTED 3000 // uint100

//rssi
#define RSSI_ADJUST_PHO 0x65
#define RSSI_LOW_MUTU_PHO 0x65
#define RSSI_LOW_MUTU_TWS_PHO 0x75

#define AUDIO_SYNC_DEF

typedef struct
{
	SYS_TIMER_TYPE autoPowerTimer;
	//power on/power off/charger in
	//uint8_t gStateFlag;
	//uint8_t gcount1;
	//uint8_t gcount2;
	//uint8_t gcount3;
	//uint8_t gcount4;
	uint8_t topState;
	//bluetooth state
	uint8_t secondState;
	uint8_t thirdlyState;
	uint8_t twsState;
	uint8_t lpmFlag;
}BR_STATE;


typedef enum
{
	BLE_STATE_INVALID,
	BLE_IDLE,
	BLE_ADV,
	BLE_CONNECTING,
	BLE_CONNECTED,
}BLE_STATE_TYPE;

typedef struct
{
	uint8_t topState;
	uint8_t secondState;
}BLE_STATE;

typedef struct
{
	uint16_t seq;
	uint32_t clk;
}TWS_PROCESS_AUDIO_INFO;

extern BR_STATE gBRState;
extern BLE_STATE gBLEState;


extern uint8_t g_tws_reset_flag;
extern uint8_t gHfpWorkingState;

//#define	CORE_CLKN									0x8300
#define	INVALID_CLK									0xF0000000
#define	DAC_BUFFER_EMPTY_DETECT_CLK_DIFF		0x20//uint: 312.5us   0x20=10ms
#define	DAC_BUFFER_EMPTY_RECOVER_CLK_DIFF		0x1E0//uint: 312.5us  0x1E0=150ms


typedef enum
{
TWS_RESET_FLAG_NONE=0,
TWS_RESET_FLAG_NEED_RESET,
TWS_RESET_FLAG_PROCESS_RESET,
}TWS_RESET_FLAG;

typedef enum
{
AVD_AUDIO_FORBID=0,
AVD_AUDIO_ALLOW
}AVD_AUDIO_STATE;
typedef enum
{
HFP_CIEVIND_SERVICE=0,
HFP_CIEVIND_CALL,
HFP_CIEVIND_CALLSETTUP,
HFP_CIEVIND_CALLHELD,
HFP_CIEVIND_SIGNAL,
HFP_CIEVIND_ROAM,
HFP_CIEVIND_BATTCHA,
}HFP_IND_INDEX;

typedef enum
{
 	HFP_NONO=0,
	HFP_INCOMMING,
	HFP_OUTGOING,
	HFP_CALLACTIVE,
	HFP_CALLIMG,
	HFP_CALLOGG,
	HFP_CALMULTY,
}HFP_CALL_STATE;


typedef enum{
	HSP_L = 0x01,
	HSP_R = 0x02,
}HSP_ROLE;


void Bt_init();
void Bt_BleCallBack(uint8_t len,uint8_t *dataPtr);
void Bt_EvtCallBack(uint8_t len,uint8_t *dataPtr);
void Bt_HFPCallBack(uint8_t len,uint8_t *dataPtr);
void Bt_TWSCallBack(uint8_t len,uint8_t *dataPtr);
void Bt_A2DPCallBack(uint8_t len,uint8_t *dataPtr);
void Bt_SPPCallBack(uint8_t len,uint8_t *dataPtr);

uint8_t Bt_checkBleIsConnected();
uint8_t Bt_checkBRIsConnected();
void Bt_CheckWaitPwroff();
void Bt_CBAutoPwroff(int paras);
void Bt_CBIdleTimeout(int prams);
void Bt_ResetAutoPwroffTimer();
void Bt_SetAutoPwroffTimer(uint32_t count,bool type,Timer_Expire_CB pfExpire_CB);
//void Lpm_LockLpm(uint8_t lpmNum);
//void Lpm_unLockLpm(uint8_t lpmNum);
//bool Lpm_CheckLpmFlag();
void Bt_LoadReconnectInfo();
void Bt_UpdateDeviceInfo();
void Bt_CheckReconnectAction();
void Bt_SndCmdPwroff();
void Bt_SndCmdWaitPwroff();
void Bt_SndCmdChargeFullPoweroff();
void Bt_SndCmdChargeDisconnect();
void Bt_SndCmdDisconnect();
void Bt_SndCmdStartDiscovery();
void Bt_SndCmdStopDiscovery();
void Bt_SndCmdReconnect();
void Bt_SndCmdStartTwsPair();
void Bt_SndCmdPlayPause();
void Bt_SndCmdAcceptCall();
void Bt_SndCmdHangup();
void Bt_SndCmdRejectCall();
void Bt_SndCmdRedial();
void Bt_SndCmdWaitMaster();
void Bt_SndCmdLoadInfo();
void Bt_SndCmdtwsPwroff();
void Bt_SndSlavePwroff();
void Bt_SndCmdRecoverAudio();
void Bt_SndCmdBatRep();
void Bt_SndCmdWaitSlave();
void Bt_SndCmdStartReconTws();
void Bt_SndCmdStopReconTws();
void BtCB_ReconnectPhoneTimeout(int params);
void Bt_StartReconnectPhoneTimer();
void Bt_ReleaseReconnectPhoneTimer();
void Bt_ReleaseReconTimer();
void Bt_SndCmdTwsUser1();
void Bt_SndCmdTwsUser2();
void Bt_SndCmdTwsUser3();
void Bt_SndCmdTwsUser4();
void Bt_SndCmdSndIncPwr();
void Bt_SndCmdAcceptTwsSwitch();
void Bt_SndCmdTwsSwitchToMaster();
void Bt_SndCmdTwsMasterSwitch();
void Bt_ActionBeforeLpm();
void Bt_ActionBeforeHibernate();

void Bt_SetEnterSniffTimer();
void Bt_CBEnterSniff(int prams);
void Bt_SetSniffMulTimer();
void Bt_CBSniffMul(int prams);
void Bt_SndCmdDisableSniffMul();
void Bt_SndCmdEnableSniffMul();
void Bt_SndCmdEnterSniff();
void Bt_SndCmdExitSniff();

bool Bt_CheckHFPIsIncomming();

void Bt_setIphoneAdvBdaddr();
void Bt_SndCmdUpdateHFPState();

bool Bt_CheckAVRCPConnected();
bool Bt_CheckA2DPConnected();
void BT_twsCheckProfileConnected();
void Bt_RelasePaDelay();
void Bt_DACBufferMonitor(void);
void Bt_L2CAPQueueBufferMonitor(void);
void Bt_RoundBufferMonitor(void);
void Bt_Reset();
void Bt_PowerOn();

void Drv_check_rssi();

void Bt_SndCmdSlaveResetAudio(void);
void Bt_SndCmdResetAudio(void);
void tws_user_reset_audio_action(void);
void tws_user_process_audio_info(void);
void tws_user_process_remote_audio_info(void);
void Bt_TwsResetFlagUpdate(uint8_t flag, uint32_t clockn);
void Bt_TwsResetFuncInit(void);
void Bt_TwsDetectAudioBufferEmpty(void);
void tws_user_hfp_start_sync_action(void);
void Bt_SndCmdHFPStartSync(void);
void Bt_SndCmdWaitTwsSlave();
void Bt_SndCmdTwsAllProfileDisconnected(void);
void BT_twsCheckAndUpdateProfileAfterProfileDisconnect(void);
void CB_SwitchToMaster(int params);

bool Bt_checkReconInfoCRC();
void Bt_calReconInfoCRC();

void Bt_SndCmdBackward();
void Bt_SndCmdForward();
void Bt_AdjustVol(bool isVolUp);
void Bt_ProcessKeyFunc(KEY_SUPPORT_FUNC func);
#endif //_DRV_IPC_H_
