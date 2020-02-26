#include "Drv_cvsd.h"
#include "yc11xx.h"

DRV_CVSD_PARAM gCVSDParam = 
{
	0,	//int mCVSDLen;
	0,	//uint8_t * mCvsdOutputPtr;
	0,  //uint8_t * mCvsdInputPtr;
	0,	//uint8_t * mPcminputdataPtr;
	0,	//uint8_t * mPcmOutputdataPtr;
	DRV_CVSD_DMA_ENABLE| DRV_CVSD_SEL_MRAM,	//uint8_t mCVSDCtrl;
};

void Drv_cvsd_init()
{
	HWRITEW(CORE_PCMIN_SADDR, gCVSDParam.mPcminputdataPtr);//input,pcm sour
	HWRITEW(CORE_CVSDOUT_SADDR, gCVSDParam.mCvsdOutputPtr); //output,cvsd dest

	HWRITEW(CORE_CVSDIN_SADDR, gCVSDParam.mCvsdOutputPtr); //in,cvsd source
	//HWRITEW(CORE_CVSDIN_SADDR, gCVSDParam.mCvsdInputPtr); //in,cvsd source
	HWRITEW(CORE_PCMOUT_SADDR, gCVSDParam.mPcmOutputdataPtr);//output,pcm dest
	HWRITEW(CORE_CVSD_LEN, gCVSDParam.mCVSDLen-1); //len
}

void Drv_cvsd_enable()
{
	HWRITE(CORE_CVSD_CTRL, gCVSDParam.mCVSDCtrl | DRV_CVSD_ENABLE);
}

void Drv_cvsd_disable()
{
	HWRITE(CORE_CVSD_CTRL, gCVSDParam.mCVSDCtrl);
}

uint8_t * Drv_cvsd_get_pcmdata_ptr()
{
	return 0;
}

uint8_t * Drv_cvsd_get_cvsddata_ptr()
{
	return 0;
}

