#include "Drv_dac.h"
#include "yc11xx.h"

DRV_DAC_PARM gDrvAdcParm =
{
	0,	//uint8_t *mDacBufferPtr;
	0,	//int mDacBufferLen;
	6,	//uint8_t mDacOutP;
	7,	//uint8_t mDacOutN;
	DRV_DAC_SEL_MRAM| DRV_DAC_8KSAMPLE	//uint8_t mDacConfig;
};


void Drv_dac_init(uint8_t * dacBuffer,int bufferLen)
{
	unsigned char temp;
	HWRITE(0x896c,0x2d);
	HWRITE(0x896e,0x94);
	
	HWRITE(0x8978,0x80);
	HWRITE(0x8979,0xE0);
	HWRITE(0x897a,0x68);
	HWRITE(0x897b,0xa8);
	HWRITE(0x897c,0x55);

	HWRITE(0x897d,0x18);

	//HWRITE(0x897e,0xF0);
	//HWRITE(0x897f,0xF0);
	//HWRITE(0x8980,0xF0);
	
	
	temp = HREAD(0x897e);
	temp = temp |0xF0;
	HWRITE(0x897e,temp);
	
	HWRITE(0x897f,0x1F);

	HWRITE(0x8980,0x00);
	HWRITE(0x8981,0x30);

	gDrvAdcParm.mDacBufferPtr = dacBuffer;
	gDrvAdcParm.mDacBufferLen = bufferLen;

	HWRITEW(CORE_DAC_SADDR, dacBuffer);
	HWRITEW(CORE_DAC_LEN, bufferLen-1);

	HWRITE(0x8108,80);
	HWRITE(0x8109,80);
	HWRITE(0x8107,0x0d);
	HWRITE(0x8106,0x07);
	
}

void Drv_dac_enable(){
	HWRITE(CORE_DAC_CTRL,gDrvAdcParm.mDacConfig| DRV_DAC_ENABLE);
}

void Drv_dac_disable()
{
	HWRITE(CORE_DAC_CTRL, gDrvAdcParm.mDacConfig);
}

void Drv_dac_set_memtype(uint8_t memtype) 
{
	if (memtype != DRV_DAC_SEL_MRAM)
	{
		gDrvAdcParm.mDacConfig = gDrvAdcParm.mDacConfig & (~DRV_DAC_SEL_MRAM);
	}
	else
	{
		gDrvAdcParm.mDacConfig = gDrvAdcParm.mDacConfig | DRV_DAC_SEL_MRAM;
	}
}

void Drv_dac_set_DACSample(uint8_t dacSamplee)
{
	if (dacSamplee != DRV_DAC_8KSAMPLE)
	{
		gDrvAdcParm.mDacConfig = gDrvAdcParm.mDacConfig & (~DRV_DAC_8KSAMPLE);
	}
	else
	{
		gDrvAdcParm.mDacConfig = gDrvAdcParm.mDacConfig | DRV_DAC_8KSAMPLE;
	}
}