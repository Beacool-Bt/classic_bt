#include <stdio.h>
#include <string.h>
#include "ipc.h"

uint8_t ssp_data[50];
uint16_t ssp_len = 0;
uint8_t tempLen = 0;

void ws_control(uint8_t cmd)
{
	
}

#define WS_HEAD_SIZE 4
void ws_rx_spp_cb(uint8_t *p, uint16_t size)
{
	uint8_t alllen;
	
	if (p[0]==0xaa&&p[1]==0xaf&&tempLen == 0)
	{
		alllen = p[3];
		if (alllen+WS_HEAD_SIZE ==  size) {
			memcpy(ssp_data,p,size);
			ssp_len = size;
			tempLen = 0;
		}
		else {
			memcpy(ssp_data,p,size);
			tempLen = size;
		}
	}
	else {
		if (tempLen!=20)
			while(1);
		memcpy(ssp_data+tempLen,p,size);
		ssp_len = tempLen+size;
		tempLen = 0;
	}
		
		
}

uint16_t  ws_rx_data(uint8_t *rx)
{
	uint16_t tsize = ssp_len;
	if (tsize==0) return 0;
	memcpy(rx, ssp_data, tsize);
	ssp_len = 0;
	return tsize;
}

tIPCControlBlock ws_control_block = 
{
	ws_control,
	ws_rx_spp_cb,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

void ws_init()
{
	IPC_Initialize(&ws_control_block);
}


void ws_rx_data_single()
{
	if (ssp_len!=0)
		return;
	IPC_DealSingleStep();
}

void ws_tx_data(uint8_t *p,uint8_t len)
{
	if (!IPC_IsTxBuffEmpty()) return;
	IPC_TxSppData(p, len);
}

