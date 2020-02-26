#ifndef DRIVERS_VP_YC_VP_H_
#define DRIVERS_VP_YC_VP_H_
#include <stdbool.h>

/**
3 mem_vp_poweron_addr
3 mem_vp_poweroff_addr
3 mem_vp_connected_addr
3 mem_vp_disconnect_addr
3 mem_vp_lowbat_addr
3 mem_vp_vlm_max
3 mem_vp_vlm_min
3 mem_vp_0
3 mem_vp_1
3 mem_vp_2
3 mem_vp_3
3 mem_vp_4
3 mem_vp_5
3 mem_vp_6
3 mem_vp_7
3 mem_vp_8
3 mem_vp_9
**/
typedef enum
{
	VP_POWERON=0,
	VP_POWEROFF,
	VP_CONNECTED,
	VP_DISCONNECT,
	VP_LOWBAT,
	VP_VLM_MAX,
	VP_TWS_CONNECTED,
	VP_DIGITAL,
	VP_0 = VP_DIGITAL,
	VP_1,
	VP_2,
	VP_3,
	VP_4,
	VP_5,
	VP_6,
	VP_7,
	VP_8,
	VP_9,
	VP_RING,
	VP_POPDOWN,
	VP_POPUP,
	VP_NUM,
}VP_INDEX;

typedef enum
{
	VP_STOP=0,
	VP_START,
}VP_PLAY_STATE;

typedef struct
{
	uint32_t sVpDataLen;
	uint32_t sVpDataOffset;
	uint32_t sVpDataAddr;
	uint8_t sMemBlock;
	uint8_t sStopNext;
}VP_PARAMS;


#define AUDIO_ALLOW mem_audio_allow
#define VP_ENABLE mem_vp_enable
#define VP_ADDR_ARRAY mem_vp_addr_array

void VP_init();

void VP_snd_voiceEvt(VP_INDEX ind);

void VP_play_voice(VP_INDEX ind);

void VP_polling_Queue();

void VP_CheckGetData();
void VP_dac_enable();

bool VP_Check_Queue_Is_empty();
void VP_ADAC_reg_init();
void VP_cvsd_init();

#endif //DRIVERS_VP_YC_VP_H_

