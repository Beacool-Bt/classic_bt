#include "Drv_debug.h"
#include "delayqueue.h"
#include "ipc.h"
#include "sbc.h"
#include "drv_a2dp.h"
#include "systick.h"
#include <stdio.h>

#define SBC_OFFSET 13
#define PCM_BUFFER_SIZE 1024
uint8_t pcmBuff[PCM_BUFFER_SIZE];

void sbcCallback(uint8_t *psbc,uint16_t size)
{
	uint32_t timer = systick_get_us();
	static uint32_t lasttimer;
	printf("t:%d\r\n",timer-lasttimer);
	lasttimer = timer;

	/*sbc decode */
	{
		int curPcmSize = 0;
		int currSbcFrameSize = 0;
		uint8_t *psbcFrame = psbc + SBC_OFFSET;
		size -= SBC_OFFSET;
		while (size) {
			curPcmSize = PCM_BUFFER_SIZE;
			currSbcFrameSize = sbcDecode(psbcFrame, size, pcmBuff, &curPcmSize);
			psbc += currSbcFrameSize;
			_ASSERT(currSbcFrameSize > 0);
			size -= currSbcFrameSize;
		}
	}
	
}

tDELAYQUEUE* puffpoint[5] = {NULL};

void main()
{
	error_t ret;
	
	log_init();
	
	ret = DQ_Init();

	printf("DQ init ret:%d\r\n", ret);
	
	systick_init();

	printf("systick init\r\n");

	a2dp_init();

	printf("a2dp init\r\n");

	sbcInitDecoder();

	printf("sbc decoder init\r\n");
	
	while(1) {
		a2dp_rx_data();
		DQ_HandleAlarm(sbcCallback);
	}
	
}
