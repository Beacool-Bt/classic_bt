/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
/**
  *@file ipc.h
  *@brief ipc support for application.
  */
#ifndef _YC_IPC_H_
#define _YC_IPC_H_
#include <stdio.h>

#define BUG_FIX
#ifdef BUG_FIX
#define FIX_ENTER_LPM	0x01
#define FIX_ENTER_HIBERNATE	0x03

#endif


/**
  *@brief MCU state.
  */
#define IPC_MCU_STATE_RUNNING 0
#define IPC_MCU_STATE_HIBERNATE 1 
#define IPC_MCU_STATE_LMP 2
#define IPC_MCU_STATE_STOP 3

/**
  *@brief MCU phase type.
  */
#define IPC_MCU_PHASE_IDLE	0
#define IPC_MCU_PHASE1_NACK	1
#define IPC_MCU_PHASE1_ACK	2
#define IPC_MCU_PHASE2_NACK	3
#define IPC_MCU_PHASE2_ACK	4

/**
  *@brief IPC buffer address.
  */
#define IPC_TX_BUF_START_ADDR IPC_TO_BT_BUF_START_ADDR
#define IPC_TX_BUF_END_ADDR	IPC_TO_BT_BUF_END_ADDR
#define IPC_TX_WPTR_ADDR IPC_TO_BT_WPTR_ADDR
#define IPC_TX_RPTR_ADDR  IPC_TO_BT_RPTR_ADDR
#define IPC_RX_WPTR_ADDR IPC_TO_M0_WPTR_ADDR
#define IPC_RX_RPTR_ADDR  IPC_TO_M0_RPTR_ADDR
#define IPC_RX_BUF_START_ADDR IPC_TO_M0_BUF_START_ADDR
#define IPC_RX_BUF_END_ADDR	IPC_TO_M0_BUF_END_ADDR
#define IPC_MCU_PHASE mem_ipc_mcu_phase
#define IPC_MCU_STATE mem_ipc_mcu_state

/**
  *@brief IPC buffer length.
  */
#define IPC_TX_BUF_LEN (IPC_TX_BUF_END_ADDR-IPC_TX_BUF_END_ADDR)
#define IPC_RX_BUF_LEN (IPC_RX_BUF_END_ADDR-IPC_RX_BUF_END_ADDR)

/**
  *@brief IPC tx hardware address.
  */
#define IPC_TX_HEAD	IPC_TX_BUF_START_ADDR
#define IPC_TX_END	IPC_TX_BUF_END_ADDR
#define IPC_TX_WRITE_PTR	IPC_TX_WPTR_ADDR
#define IPC_TX_READ_PTR		IPC_TX_RPTR_ADDR

/**
  *@brief IPC rx hardware address.
  */
#define IPC_RX_HEAD	IPC_RX_BUF_START_ADDR
#define IPC_RX_END	IPC_RX_BUF_END_ADDR
#define IPC_RX_WRITE_PTR	IPC_RX_WPTR_ADDR
#define IPC_RX_READ_PTR		IPC_RX_RPTR_ADDR



/**
  *@brief IPC event type enum.
  */
typedef enum
{
 	HFP_EVT_PLUS_CIEV=1,
 	HFP_EVT_PLUS_CLIP,
 	HFP_EVT_RING,
 	HFP_EVT_PLUS_VGS,
 	HPF_EVT_PLUS_VGM,
 	HFP_EVT_CIND_INDICATOR,
}HFP_EVT_TYPE;


/**
  *@brief BLE format.
  */
typedef struct
{
	unsigned short mhandle;
	unsigned char data;
}IPC_BLE_FORMAT;

/**
  *@brief SPP format.
  */
typedef struct
{
	unsigned char data;
}IPC_SPP_FORMAT;

/**
  *@brief IPC data format.
  */
typedef struct
{
	unsigned char ipctype;
	unsigned char len;
	union
	{
		IPC_BLE_FORMAT uBleData;
	} ipcUnion;
}IPC_DATA_FORMAT;

/**
  *@brief IPC type enum.
  */
#define IPC_TYPE_START 0
#define IPC_TYPE_NUM 0xd

typedef void (*tIPCHandleCb)(uint8_t,uint8_t *);
typedef tIPCHandleCb (*tIPCHandleCbArray)[IPC_TYPE_NUM];

/**
  *@brief IPC EVT callback function type.
  */
typedef void (*tIPCEventCb)(uint8_t);

/**
  *@brief IPC SPP callback function type.
  */
typedef void (*tIPCSppCb)(uint8_t*, uint16_t);

/**
  *@brief IPC BLE callback function type.
  */
typedef void (*tIPCBleCb)(uint8_t*, uint16_t);

/**
  *@brief IPC timer(1s) callback function type.
  */
typedef void (*tIPCTimer)(void);

/**
  *@brief IPC Hid callback function type.
  */
typedef void (*tIPCHidCb)(uint8_t*, uint16_t);

/**
  *@brief IPC Hid callback function type.
  */
typedef void (*tIPCMeshCb)(uint8_t, uint8_t*, uint16_t);

/**
  *@brief IPC a2dp callback function type.
  */
typedef void (*tIPCa2dpCb)(uint8_t*, uint16_t);

/**
  *@brief IPC control block type.
  */
typedef struct IPCContolBlock {
	tIPCEventCb evtcb;		/*< ipc evt call back*/
	tIPCSppCb sppcb;		/*< ipc spp call back*/
	tIPCTimer timercb;		/*< ipc 1s timer call back*/
	tIPCTimer stimercb;		/*< ipc 100ms timer call back */
	tIPCBleCb blecb;		/*< ipc ble call back*/
	tIPCHidCb hidcb;		/*< ipc hid call back*/
	tIPCMeshCb meshcb;		/*< ipc mesh call back*/
	tIPCa2dpCb a2dpcb;		/*< ipc audio call back */
}tIPCControlBlock;

/**
  *@brief Define 6-bit address.
  */
#define BT_ADDR_SIZE	6
typedef uint8_t tBTADDR[BT_ADDR_SIZE];

/**
  *@brief This function rigist IPC Control Block.
  *@param cb the IPC control block. 
  *@return None.
  */
void IPC_Initialize(tIPCControlBlock *Cb);

/**
  *@brief This function deal IPC Single process, callback the rigisted IPC control function.
  *@param None.
  *@return None.
  */
void IPC_DealSingleStep(void);

/**
  *@brief This function tx control IPC command.
  *@param cmd IPC command want to send.
  *@return None.
  */
void IPC_TxControlCmd(uint8_t Cmd);

/**
  *@brief This function tx mult IPC data.
  *@param cmd IPC command want to send.
  *@return None.
  */
void IPC_TxCommon(uint8_t Type, uint8_t* Dt, uint8_t Len);

/**
  *@brief This function tx HID IPC command,Cpu will block when TxBuffer is not Empty. 
  *@param dt IPC HID data want to send,len length of hid data. 
  *@return None.
  */
#define IPC_TxHidData(dt, len)		IPC_TxCommon(IPC_HID_DATA, (dt), (len))

/**
  *@brief This function tx 24G IPC command,Cpu will block when TxBuffer is not Empty. 
  *@param dt IPC HID data want to send,len length of hid data. 
  *@return None.
  */
#define IPC_Tx24GData(dt, len)		IPC_TxCommon(IPC_24G_DATA, (dt), (len))

/**
  *@brief This function tx SPP IPC command,Cpu will block when TxBuffer is not Empty.
  *@param dt IPC SPP data want to send,len length of SPP data. 
  *@return None.
  */
#define IPC_TxSppData(dt, len)		IPC_TxCommon(IPC_SPP_DATA, (dt), (len))

/**
  *@brief This function tx BLE IPC command,Cpu will block when TxBuffer is not Empty. 
  *@param dt IPC BLE data want to send,len length of BLE data.
  *@return None.
  */
#define IPC_TxBleData(dt, len)		 IPC_TxCommon(IPC_BLE_DATA, (dt), (len))

/**
  *@brief This function will do nothing wait enter lpm. 
  *@param None.
  *@return None.
  */
void IPC_WaitLpm(void);

/**
  *@brief This function will do nothing abandon lpm this time.
  *@param None.
  *@return None.
  */
void IPC_AbandonLpm(void);

/**
  *@brief This function check if tx buffer empty. 
  *@param None.
  *@return None.
  */
uint8_t IPC_IsTxBuffEmpty(void);

/**
  *@brief This function check if rx buffer empty.
  *@param None. 
  *@return None.
  */
uint8_t IPC_IsRxBuffEmpty();

/**
  *@brief This function will check sdp server connect.
  *@param None.
  *@return None.
  */
uint8_t IPC_CheckServerConenct(void);

/**
  *@brief This function will disable lpm function.
  *@param None.
  *@return None.
  */
void IPC_DisableLpm();

/**
  *@brief This function will enable lpm function.
  *@param None.
  *@return None.
  */
void IPC_EnableLpm();

/**
  *@brief This function prevents IPC txbuffer overflow.
  *@param The length of the data send.
  *@return 1 represents the completion of sending data,return 0 represents txbuffer is empty.
  */
unsigned char IPC_TxBufferIsEnough(uint8_t Len);

/**
  *@brief This function waits to be written tx buffer.
  *@param The length of the data send.
  *@return 1 represent the completion of sending data.
  */
unsigned char IPC_WaitBufferEnough(uint8_t Len);

/**
  *@brief IPC default callback function.
  *@param len 
  *@param dataPtr
  *@return None.
  */
void IpcDefaultCallBack(uint8_t len,uint8_t *dataPtr);

/**
  *@brief Accept packages from the handle.
  *@param None.
  *@return None.
  */
void IPC_HandleRxPacket();

/**
  *@brief IPC initialization.
  *@param cbArrayPtr callback function.
  *@return None.
  */
void IPC_init(tIPCHandleCbArray cbArrayPtr);

/**
  *@brief IPC transport packet.
  *@param packet IPC data.@ref IPC_DATA_FORMAT
  *@return None.
  */
void IPC_TxPacket(IPC_DATA_FORMAT *packet);

/**
  *@brief IPC set ACK flag.
  *@param None.
  *@return None.
  */
void IPC_set_ack_flag();

/**
  *@brief IPC set NACK flag.
  *@param None.
  *@return None.
  */
void IPC_set_nack_flag();

/**
  *@brief IPC wait ACK.
  *@param None.
  *@return None.
  */
void IPC_wait_ack();

/**
  *@brief IPC clear flag.
  *@param None.
  *@return None.
  */
void IPC_clear_flag();

#endif
