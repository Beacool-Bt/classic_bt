
#ifndef _DRV_MSBC_H_

#define _DRV_MSBC_H_

//#define DRV_MSBC_ENABLE 	0x01
//#define DRV_MSBC_BITNEED_MODE	0x02
//#define DRV_MSBC_SRC_MRAM 	0x04
//#define DRV_MSBC_DST_MRAM  	0x08
//#define DRV_MSBC_MODE_ENABLE 0x10
#define DRV_MSBC_ENABLE 	0x01
#define DRV_MSBC_BITNEED_MODE	0x02
#define DRV_MSBC_SRC_MRAM 	0x04
#define DRV_MSBC_DST_MRAM  	0x08
#define DRV_MSBC_GAIN_0	0x00
#define DRV_MSBC_GAIN_1	0x10
#define DRV_MSBC_GAIN_2	0x20
#define DRV_MSBC_GAIN_3	0x30
#define DRV_MSBC_GAIN_4	0x40
#define DRV_MSBC_GAIN_5	0x50
#define DRV_MSBC_GAIN_6	0x60
#define DRV_MSBC_GAIN_7	0x70
#define DRV_MSBC_MODE_ENABLE 0x80


typedef struct
{
	unsigned char *mDecPtr;
	unsigned char *mSrcBuf;
	unsigned char *mDstBuf;
	unsigned char *mRdPtr;
	int mSrcBufLen;
	int mDstBufLen;
	unsigned char mCtrFlag;
	unsigned char mMsbcCoding;
	
}MSBC_PARAMS;

void Drv_mSBC_init();

void Drv_mSBC_enable();

void Drv_mSBC_disable();

int Drv_mSBC_CheckEncodeCmp();

void Drv_mSBC_RefreshWpr(int len);

int Drv_CheckMsbcCoding(uint8_t *Coding);
int Drv_CheckMsbcCoding_packet(uint8_t *Coding);

extern MSBC_PARAMS gMsbc_params;

void Drv_mSBC_RefreshWptr(int len);

#endif //_DRV_MSBC_H_
