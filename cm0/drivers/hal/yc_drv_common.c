/*************************************************************************
*
* Yichip  
*
*************************************************************************/
#include "yc_drv_common.h"

/*************************COMMON FUNC***************************/
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))




void HW_REG_16BIT(uint32_t reg, uint16_t word)
{
    HW_REG_8BIT(reg, word & 0x00FF);
    hw_delay();
    HW_REG_8BIT(reg + 1,(word >> 8));
}
uint16_t HR_REG_16BIT(uint32_t reg)
{
	uint16_t return_data = 0;
	uint16_t H_data = 0;
	hw_delay();
	return_data = HR_REG_8BIT(reg);
	hw_delay();
	H_data = HR_REG_8BIT(reg + 1);
	return_data = (return_data | ((H_data << 8) & 0xFF00));
	return return_data;
}

uint32_t HR_REG_24BIT(uint32_t reg)
{
	uint32_t return_data = 0;
	hw_delay();
	return_data = HR_REG_8BIT(reg);
	hw_delay();
	return_data = return_data |(HR_REG_8BIT(reg + 1)<<8);
	hw_delay();
	return_data = return_data | (HR_REG_8BIT(reg + 2)<<16);
	
	return return_data;
}

void xmemcpy(uint8_t* dest,const uint8_t* src, uint16_t len) 
{
	while(len--)
		*dest++ = *src++;
	return;
}

uint16_t xstrlen(const char *s)
{
	int n;  
    	for(n=0;*s!='\0';s++) {  
       		 n++;  
  	}  
    	return n;  
}

void error_handle()
{
	while(1);
}

void Lpm_LockLpm(uint16_t lpmNum)
{
	uint16_t temp;
	temp = HR_REG_16BIT(reg_map(M0_LPM_REG))|lpmNum;
	HW_REG_16BIT(reg_map(M0_LPM_REG),temp);
}


void Lpm_unLockLpm(uint16_t lpmNum)
{
	uint16_t temp;
	temp = HR_REG_16BIT(reg_map(M0_LPM_REG)) &(~lpmNum);
	HW_REG_16BIT(reg_map(M0_LPM_REG),temp);
}

bool Lpm_CheckLpmFlag()
{
	return NONE_LPM_FLAG == HR_REG_16BIT(reg_map(M0_LPM_REG));
}

void whileDelay(int delayValue)
{
	int delayMS;
	for (int k=0; k < delayValue; k++){
		delayMS = 20000;
		while(delayMS--);
	}
}
void whileDelayshort(int delayValue)
{
	int delayMS;
	for (int k=0; k < delayValue; k++){
		delayMS = 2000;
		while(delayMS--);
	}
}

