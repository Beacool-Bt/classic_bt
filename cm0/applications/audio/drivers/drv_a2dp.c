/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 #include <string.h>
#include "ipc.h"
#include "delayqueue.h"

#define TIME_STAMP_OFFSET 4

#define AUDIO_INTERVAL_48K	1000/48 
#define AUDIO_INTERVAL_44K	1000/44.1

tDELAYQUEUE* entrya2dp = NULL;

void a2dp_rx(uint8_t* pa2dp, uint16_t size)
{
	register uint8_t *ptstamp = pa2dp + TIME_STAMP_OFFSET;
	register uint32_t tstamp = 0;
	BE_STREAM_TO_UINT32(tstamp,ptstamp);
	
	entrya2dp = DQ_ApplyForNewNode(tstamp*AUDIO_INTERVAL_48K);
	if (entrya2dp != NULL) {
		entrya2dp->size = size;
		memcpy(entrya2dp->pnode, pa2dp, size);
	}
}

void ad2p_control(uint8_t cmd)
{
	
}

tIPCControlBlock a2dp_control_block = 
{
	ad2p_control,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	a2dp_rx
};

void a2dp_init()
{
	IPC_Initialize(&a2dp_control_block);
}


void a2dp_rx_data()
{
	if (!DQ_CheckFull()) {
		IPC_DealSingleStep();
	}
}

uint32_t testtimer = 0;
void a2dp_test_rx()
{
	if (!DQ_CheckFull()) {
		testtimer += 640*AUDIO_INTERVAL_48K;
		DQ_ApplyForNewNode(testtimer);
	}
}
 