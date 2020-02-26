#include "yc11xx_ks.h"
#include "yc11xx_gpio.h"

#define KS_KEY_MAP_LEN	158
#define KS_SEL_MAP_LEN	20
#define KS_RAW_MAX	 	8

#define	KB_KSCAN_COL_NUM				20
#define	KB_KSCAN_ROW_NUM				7
#define	KB_DELAY_COUNT				5

#define GET_RAW_KEY_SEL(y, n) HWRITE(CORE_GPIO_CONF + n, GPCFG_OUTPUT_LOW); \
	hw_delay();\
	y = ~ HREAD(CORE_GPIO_IN);	\
	hw_delay();\
	HWRITE(CORE_GPIO_CONF + n, GPCFG_PULLUP);\
	hw_delay();
	

tKSEVENT ksEvtPool = {0};
uint8_t *ksMap;
uint8_t kscurrmult;

uint8_t ksSelMapOld[KS_SEL_MAP_LEN] = {0};
uint8_t ksSelMapCheck[KS_SEL_MAP_LEN] = {0};
uint8_t ksSelMapCurr[KS_SEL_MAP_LEN] = {0};

uint8_t *col;
uint8_t ghost = 0;

uint8_t gRowSize = 0;
uint8_t gColSize = 0;

#define MAX(a,b) ((a)>(b)?(a):(b))

static uint8_t bit_count(uint8_t v)
{
	unsigned char c;
	for (c = 0; v; c++) {
    		v &= v - 1;
    	}
    	return c;
}

static void keyIn(uint8_t key)
{
	uint8_t i = 0;

	for (i=0 ; i<KS_BUFF_POLL_LEN ; i++) {
		if (ksEvtPool.ksPool[i] == key)
			break;
		if (ksEvtPool.ksPool[i] == 0) {
			ksEvtPool.ksPool[i] = key;
			break;
		}
	}
}

static void keyOut(uint8_t key)
{
	uint8_t i = 0 ;
	uint8_t found = 0;
	
	for (i=0 ; i<KS_BUFF_POLL_LEN ; i++) {
		if (found == 0) {
			if (ksEvtPool.ksPool[i] == key) {
				if (i == KS_BUFF_POLL_LEN-1) {
					ksEvtPool.ksPool[i] = 0;
				}
				found = 1;
			}
		}
		else {
			ksEvtPool.ksPool[i-1] = ksEvtPool.ksPool[i];
			if (ksEvtPool.ksPool[i] == 0)
				break;
			if (i == KS_BUFF_POLL_LEN-1) {
				ksEvtPool.ksPool[i] = 0;
			}
		}
	}
}

static void ksRawInitialize(uint8_t RowSize)
{
	uint8_t i = 0;
	
	for (i=0; i<RowSize; i++) {
		GPIO_SetInput(i, 0);
	}
}

static void ksColInitialize(uint8_t Col[], 
	uint8_t ColSize)
{
	uint8_t i = 0;
	for (i=0; i < ColSize; i++) {
		GPIO_SetInput(col[i], 0);
	}
}

void KS_Initialize(uint8_t Col[],
	uint8_t RowSize,
	uint8_t ColSize,
	uint8_t KsData[])
{
	_ASSERT(RowSize <= KS_RAW_MAX);
	_ASSERT(ColSize <= KS_SEL_MAP_LEN);
	ksMap = KsData;
	col = Col;
	gRowSize = RowSize;
	gColSize = ColSize;
	ksRawInitialize(RowSize);
	ksColInitialize(Col, ColSize);
}
/*
void KS_Unistall()
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t tgroup = 0;
	uint8_t tgpionum = 0;
	for (i=0; col[i]!=0; i++) {
		tgroup = col[i]  >> 3;
		tgpionum = 1 << (col[i] & 7);
		REG_GPIO_PUP (tgroup) &= ~tgpionum;
		REG_GPIO_OE(tgroup) |= tgpionum;
		REG_GPIO_OUT(tgroup) &= ~tgpionum;
	}
	temp = REG_GPIO_IN(0);
	YC_GPIOWakeupL[0] = temp;
	YC_GPIOWakeupH[0] = ~temp;
}
*/

void KS_ScanMatrix()
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t temp = 0;
	uint8_t check = 0;
	uint8_t rawSel = 0;
	for (i=0; i<gColSize; i++) {
		GET_RAW_KEY_SEL(ksSelMapCurr[i], col[i]);
	}
	return;
	for (i=0; i<gColSize; i++) {
		GET_RAW_KEY_SEL(ksSelMapCheck[i], col[i]);
	}

	for (i=0; i<gColSize; i++) {
		if (ksSelMapCheck[i]!=ksSelMapCurr[i]) {
			ksSelMapCurr[i] &= ksSelMapCheck[i];
		}
	}
}


tKSEVENT *KS_GetCurrentEvt()
{
	//
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t temp = 0;
	uint8_t flag = 0;
	uint8_t offset = 0;
	uint8_t state = 0;
	uint8_t colCounttemp = 0;
	uint8_t rawSel =0;
	uint8_t keyCounttemp = 0;
	uint8_t rawCount = 0;
	uint8_t colCount = 0;
	uint8_t keyCount = 0;
	uint8_t keyinCount = 0;
	uint8_t keyoutCount = 0;

	KS_ScanMatrix();


	for (i=0; i<gColSize; i++) {
		temp = ksSelMapCurr[i];
		if (temp!=0) {
			colCounttemp++;
			rawSel = temp|rawSel;
			keyCounttemp += bit_count(temp);
		}
	}
	keyCount = keyCounttemp;
	colCount = colCounttemp;
	rawCount = bit_count(rawSel);
	if (keyCount >2 && keyCount > MAX(colCount, rawCount)) {
		ghost = 1;
	}
	else {
		ghost = 0;
	}
	
	for (i=0; i<gColSize; i++) {
		temp = ksSelMapCurr[i] ^ ksSelMapOld[i];
		state = ksSelMapCurr[i];
		if (temp == 0) {
			continue;
		}
		else {
			if (ghost) 
				return NULL;
			ksSelMapOld[i] = ksSelMapCurr[i];
			for (j = 0 ; j<gRowSize ; j++) {
				if (temp&1) {
					offset = i<<3 |j;
					if (state & 1) {
						keyinCount++;
						keyIn(ksMap[offset]);
					}
					else {
						keyoutCount++;
						keyOut(ksMap[offset]);
					}
				}
				temp = temp >>1;
				state = state>>1;
			}
			flag = 1;
		}
	}

	if (ksEvtPool.ksPool[0] ==0) {
		//scan for same row
	}
	else {
		//scan for same row
	}
	
	if (flag) {
		return &ksEvtPool;
	}
	else {
		return NULL;
	}
	

}


