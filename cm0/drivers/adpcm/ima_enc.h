
#ifndef _IMA_ADPCM_H_
#define _IMA_ADPCM_H_


typedef unsigned int uint;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;


typedef struct 
{
	int valprev;
	int index;
}CodecState;

void encode(CodecState* state, s16* input, int numSamples, u8* output);
void decode(CodecState* state, u8* input, int numSamples, s16* output);

#endif //_IMA_ADPCM_H_

