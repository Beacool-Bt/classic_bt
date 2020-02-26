#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "noise_suppression.h"
#include "signal_processing_library.h"

NsHandle gNS_inst;


void WebRtcNS8KSample(char *szFileIn,char *szFileOut,int nSample,int nMode)
{
	int nRet = 0;
	

	FILE *fpIn = NULL;
	FILE *fpOut = NULL;

	char *pInBuffer =NULL;
	char *pOutBuffer = NULL;

	do
	{
		int i = 0;
		int nFileSize = 0;
		int nTime = 0;
		WebRtcNs_Create(&gNS_inst);
		WebRtcNs_Init(&gNS_inst,nSample);
		WebRtcNs_set_policy(&gNS_inst,nMode);

		fpIn = fopen(szFileIn, "rb");
		fpOut = fopen(szFileOut, "wb");
		if (NULL == fpIn || NULL == fpOut)
		{
			printf("open file err \n");
			break;
		}

		fseek(fpIn,0,SEEK_END);
		nFileSize = ftell(fpIn); 
		fseek(fpIn,0,SEEK_SET); 

		pInBuffer = (char*)malloc(nFileSize);
		memset(pInBuffer,0,nFileSize);
		fread(pInBuffer, sizeof(char), nFileSize, fpIn);

		pOutBuffer = (char*)malloc(nFileSize);
		memset(pOutBuffer,0,nFileSize);


		for (i = 0;i < nFileSize;i+=160)
		{
			if (nFileSize - i >= 160)
			{
				short shBufferIn[80] = {0};
				short shOutL[80] = {0};
				memcpy(shBufferIn,(char*)(pInBuffer+i),80*sizeof(short));
				if (0 == WebRtcNs_Process(&gNS_inst ,shBufferIn  ,NULL ,shOutL , NULL))
				{
					memcpy(pOutBuffer+i,shOutL,80*sizeof(short));
				}
			}	
		}

		fwrite(pOutBuffer, sizeof(char), nFileSize, fpOut);
	} while (0);

	WebRtcNs_Free(&gNS_inst);
	fclose(fpIn);
	fclose(fpOut);
	free(pInBuffer);
	free(pOutBuffer);
}

void main()
{
	WebRtcNS8KSample("test_1C_16bit_8K.pcm","test_1C_16bit_8K_ns.pcm",8000,1);

	printf("Ωµ‘ÎΩ· ¯...\n");

	getchar();
	return;
}
