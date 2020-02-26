#include "yc11xx.h"
#include "Drv_debug.h"
#include "Drv_msbc.h"
#define MSBC_ONE_ENCODED_LEN 57
#define MSBC_ONE_PACKET_LEN 240

MSBC_PARAMS gMsbc_params=
{
0,
0,
0,
0,
0,
0,
//DRV_MSBC_DST_MRAM|DRV_MSBC_SRC_MRAM|DRV_MSBC_MODE_ENABLE|DRV_MSBC_GAIN_1
DRV_MSBC_DST_MRAM|DRV_MSBC_SRC_MRAM|DRV_MSBC_MODE_ENABLE|DRV_MSBC_GAIN_1
};

unsigned char lengthSBC;

void Drv_mSBC_init()
{
    //gMsbc_params.mDecPtr = gMsbc_params.mDstBuf;
    if (gMsbc_params.mSrcBufLen %MSBC_ONE_PACKET_LEN != 0)
    {
	while(1);
    }
    
    if (gMsbc_params.mDstBufLen %MSBC_ONE_ENCODED_LEN !=0)
    {
	while(1);
    }
    
    HWRITEW(CORE_SBC_SADDR, gMsbc_params.mSrcBuf);
    HWRITEW(CORE_SBC_DADDR, gMsbc_params.mDstBuf);
    HWRITEW(CORE_SBC_SLEN, gMsbc_params.mSrcBufLen);
    HWRITEW(CORE_SBC_DLEN, gMsbc_params.mDstBufLen);
    HWRITEW(CORE_SBC_SWP,0);
}

void Drv_mSBC_enable()
{
	gMsbc_params.mRdPtr = gMsbc_params.mDstBuf;
	HWRITEW(CORE_SBC_SWP,0);
	
	HWRITE(CORE_SBC_CTRL,gMsbc_params.mCtrFlag);
	HWRITE(CORE_SBC_CTRL,gMsbc_params.mCtrFlag|DRV_MSBC_ENABLE);

	HWRITE(CORE_SBC_CRL,0x86);
}

void Drv_mSBC_disable()
{
	HWRITE(CORE_SBC_CTRL,gMsbc_params.mCtrFlag);
}

void Drv_mSBC_PushPacket(unsigned char * src,int len)
{
	//Drv_mSBC_RefreshWptr(len);	
}

void Drv_mSBC_RefreshWptr(int len)
{
	int swp = HREADW(CORE_SBC_SWP);
	swp+=len;

	//DEBUG_PCHAR((char)swp);
	//DEBUG_PCHAR((char)(swp>>8));
	if (swp >= gMsbc_params.mSrcBufLen)
	{
		swp= swp-gMsbc_params.mSrcBufLen;
	}
	HWRITEW(CORE_SBC_SWP,swp);
	gMsbc_params.mMsbcCoding = 1;
}

int Drv_mSBC_CheckEncodeCmp()
{
	int swp = HREADW(CORE_SBC_SWP);
	int srp = HREADW(CORE_SBC_SRP);
	if (swp == srp )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Drv_CheckMsbcCoding(uint8_t *Coding)
{
	int i;
	if (gMsbc_params.mMsbcCoding == 1)
	{
		if(Drv_mSBC_CheckEncodeCmp())
		{	
			gMsbc_params.mMsbcCoding = 0;
			for(i = 0; i < MSBC_ONE_ENCODED_LEN; i++)
			{
				Coding[i]= gMsbc_params.mRdPtr[i];
			}
						
			gMsbc_params.mRdPtr += MSBC_ONE_ENCODED_LEN;
			if (gMsbc_params.mRdPtr >= (gMsbc_params.mDstBuf +gMsbc_params.mDstBufLen))
			{
				gMsbc_params.mRdPtr = gMsbc_params.mDstBuf;
			}
			return 1;
		}	
	}
	return 0;
}

int Drv_CheckMsbcCoding_packet(uint8_t *Coding)
{
	int i;
	if (gMsbc_params.mMsbcCoding == 1)
	{
		if(Drv_mSBC_CheckEncodeCmp())
		{	
			if (lengthSBC == 40)
			{
				gMsbc_params.mMsbcCoding = 0;
				for(i = 0; i < 17; i++)
				{
					Coding[i]= gMsbc_params.mRdPtr[i];
				}
				lengthSBC = 0;
				
				gMsbc_params.mRdPtr += 17;
				if (gMsbc_params.mRdPtr >= (gMsbc_params.mDstBuf +gMsbc_params.mDstBufLen))
				{
					gMsbc_params.mRdPtr = gMsbc_params.mDstBuf;
				}

			}
			else
			{
				for(i = 0; i < 20; i++)
				{
					Coding[i]= gMsbc_params.mRdPtr[i];
				}
				gMsbc_params.mRdPtr += 20;
				lengthSBC+=20;
				if (gMsbc_params.mRdPtr >= (gMsbc_params.mDstBuf +gMsbc_params.mDstBufLen))
				{
					gMsbc_params.mRdPtr = gMsbc_params.mDstBuf;
				}
			}
			return 1;
		}	
	}
	return 0;
}

void Drv_mSBC_enable_test()
{
	int sbcState;
	gMsbc_params.mRdPtr = gMsbc_params.mDstBuf;
	HWRITEW(CORE_SBC_SWP,0);
	
	HWRITE(CORE_SBC_CTRL,gMsbc_params.mCtrFlag);
	HWRITE(CORE_SBC_CTRL,gMsbc_params.mCtrFlag|DRV_MSBC_ENABLE);

	HWRITE(CORE_SBC_CRL,0x40);

	sbcState = HREAD(0x8308);
	while(sbcState & 0x40)
	{
		sbcState = HREAD(0x8308);
	}	
}

void Drv_mSBC_disable_test()
{
	HWRITEW(CORE_SBC_SWP,0);
	HWRITE(CORE_SBC_CTRL,gMsbc_params.mCtrFlag);
	HWRITE(CORE_SBC_CRL,0xff);
}



