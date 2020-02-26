#ifndef _FIR_8TO48_H_
#define _FIR_8TO48_H_

#include "yc11xx.h"

#define DAC_48K_LEN 0x708 // 1800*2=3600,sizeof(short) = 2
#define DAC_48K_START_ADDR 0x3000

#define PCM_OUT_START 0x1970 //+1200
#define PCM_OUT_END 0x22d0
#define PCM_OUT_LEN 1200 //600*2=1200 bytes
#define MAX_PCK_LEN 120

#define FILTER_ROW 6
#define FILTER_COL 6

uint8_t sco_fir_polling();
void sco_fir_init();
void insert_48K(short data);

#endif //_FIR_8TO48_H_

