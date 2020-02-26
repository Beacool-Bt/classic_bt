#ifndef __DRV_MEDIA__ 
#define __DRV_MEDIA__ 

#include "drv_bt.h"

#define	TWS_DROP_PROCESS_STATE_NONE    			0x00
#define	TWS_DROP_PROCESS_STATE_INPROCESS    	0x17
#define	TWS_DROP_PROCESS_STATE_WAIT_DROP    	0x53

typedef struct{
	uint16_t test1;
	uint16_t test2;
}TestEnum; 

extern TestEnum gTttt;
extern uint8_t gIsDropInProcess;

typedef struct
{
	int16_t lData;
	int16_t rData;
}MEDIA_ELEM;


void Drv_media_polling();
void FarrowFilterInit();

extern void Bt_SndCmdTwsDropOnePacket(void);


#endif //__DRV_MEDIA__ 

