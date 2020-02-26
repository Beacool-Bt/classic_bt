#include "yc11xx_qspi.h"

void __attribute__((noinline))   FillSendCMD(uint8_t *buf, uint8_t cmd, int flash_addr);
void __attribute__((noinline,aligned(32)))  ReadFlashSPI(uint8_t *txdata,int txlen,uint8_t *rxbuf,int rxlen);
void  __attribute__((noinline,aligned(32)))  FlashWriteEnable();
void __attribute__((noinline,aligned(32)))   WaitFlashFinish();

uint8_t *readreg; // 2 byte
uint8_t *temp2;
uint8_t *temp;
uint8_t *readtemp;

MRAM_TYPE gMRAM_type;
uint8_t gReadreg[2];
uint8_t gTemp2[4];
uint8_t gTemp[8];
uint8_t gReadTemp[260];


void __attribute__((noinline))   SetLockQSPI( )
{
		HWRITE(mem_lock_qspi, 1);
}

void __attribute__((noinline))   SetReleaseQSPI( )
{
		HWRITE(mem_lock_qspi, 0);	
}


void __attribute__((noinline))   setRamType(MRAM_TYPE type )
{
	gMRAM_type = type;
	if (type ==XRAM)
	{
	 	readreg = (uint8_t *)(RXBUF_ADDR); // 2 byte
		temp2 = (uint8_t *)(RXBUF_ADDR+2); 
		temp = (uint8_t *)(RXBUF_ADDR+6); 
		readtemp =  (uint8_t *)(RXBUF_ADDR+16); 
	}
	else
	{
		readreg = gReadreg;
		temp2 = gTemp2;
		temp = gTemp;
		readtemp = gReadTemp;
	}
}

void __attribute__((noinline)) Start()
{
	__ASM("nop");
}

void __attribute__((noinline))   FillSendCMD(uint8_t *buf, uint8_t cmd, int flash_addr)
{
	uint8_t j;
	buf[0] = cmd;
	for(j = 0;j < 3;j++);
//	QSPI_Delay(1);
	buf[1] = flash_addr >> 16;
	for(j = 0;j < 3;j++);
//	QSPI_Delay(1);
	buf[2] = flash_addr >> 8;
	for(j = 0;j < 3;j++);
//	QSPI_Delay(1);
	buf[3] = flash_addr;
}


void  ReadFlashDSPI(uint8_t cammond,int flash_addr, int rxlen, uint8_t *rxbuf)
{
//	int addr;
	volatile int i, j;
	uint8_t mode, delay;
	FillSendCMD(temp, cammond, flash_addr);
	mode = HREAD(CORE_QSPI_CTRL);
	delay = HREAD(CORE_QSPI_DELAY);
	
	if (gMRAM_type == MRAM)
		HWRITE(CORE_QSPI_CTRL  , 0x45);//mode&0xb8 |0x05
	else
		HWRITE(CORE_QSPI_CTRL  , 0x05);//mode&0xb8 |0x05

		HWRITE(CORE_QSPI_DELAY, 8);
		HWRITEW(CORE_QSPI_TXADDR , (int)temp);
		HWRITEW(CORE_QSPI_TXLEN , 4);
		HWRITEW(CORE_QSPI_RXADDR , (int)rxbuf);
		HWRITEW(CORE_QSPI_RXLEN , rxlen);
		
		HWRITE(CORE_DMA_START , 8);
		while(!(HREAD(CORE_DMA_STATUS) & 8) );
		HWRITE(CORE_QSPI_CTRL, mode);
		HWRITE(CORE_QSPI_DELAY , delay);
}

void __attribute__((noinline,aligned(32))) QSPI_ReadFlashData(int flash_addr, int len, uint8_t *rxbuf)
{
	int i=0,j=0,x=0;

	for(i = len,j=0;i>0;i = i -0x40,j = j+0x40){
		if(i>0x40)
			ReadFlashDSPI(FlashCMD_ReadDual,flash_addr+j,0x40,rxbuf+j);
		else
			ReadFlashDSPI(FlashCMD_ReadDual,flash_addr+j,i,rxbuf+j);	
	}
}


void __attribute__((noinline,aligned(32))) ReadFlashSPI(uint8_t *txdata,int txlen,uint8_t *rxbuf,int rxlen)
{
	volatile int i, j;
	uint8_t mode, delay;
//	byte temp[8]={0,0,0,0,0,0,0,0};
//	for(i = 0;i < txlen;i++)  
//		temp[i] = txdata[i];
		mode = HREAD(CORE_QSPI_CTRL);
		delay = HREAD(CORE_QSPI_DELAY);
		if (gMRAM_type == MRAM)
			HWRITE(CORE_QSPI_CTRL , 0x44);// 0x40  mode&0xb8 |0x00
		else
			HWRITE(CORE_QSPI_CTRL , 0x04);// 0x40  mode&0xb8 |0x00
		
		HWRITE(CORE_QSPI_DELAY , 0x0);
		HWRITEW(CORE_QSPI_TXADDR ,(int)txdata );
		HWRITEW(CORE_QSPI_TXLEN , txlen);
		HWRITEW(CORE_QSPI_RXADDR , (int)rxbuf);
		HWRITEW(CORE_QSPI_RXLEN , rxlen);
		HWRITE(CORE_DMA_START , 8);
		while(!(HREAD(CORE_DMA_STATUS) & 8) );
		HWRITE(CORE_QSPI_CTRL , mode);
		HWRITE(CORE_QSPI_DELAY , delay);
}

void __attribute__((noinline,aligned(32))) QSPI_ReadFlashRDID(uint8_t *rxbuf)
{
	temp[0] = FlashCMD_RDID;
	ReadFlashSPI(temp,1,rxbuf,3);
}

void __attribute__((noinline,aligned(32))) QSPI_ReadFlashReg(uint8_t *rxbuf)
{
	temp2[0] = FlashCMD_ReadRegister1;
	ReadFlashSPI(temp2,1,rxbuf,1); //s7-s0
	temp2[0] = FlashCMD_ReadRegister2;
	ReadFlashSPI(temp2,1,rxbuf+1,1);	//s15-s8
}

void __attribute__((noinline,aligned(32))) QSPI_ReadFlashREMS(uint8_t *rxbuf)
{
	temp[0] = FlashCMD_REMS;
	ReadFlashSPI(temp,4,rxbuf,2); //s7-s0
}

void __attribute__((noinline,aligned(32))) QSPI_ReadFlashUID(uint8_t *rxbuf)
{
	temp[0] = FlashCMD_UID;
	ReadFlashSPI(temp,5,rxbuf,8); //s7-s0
}

void  WaitFlashFinish()
{
	uint8_t j;
	do{
		for(j = 0;j < 40;j++);
		QSPI_ReadFlashReg(readreg);
	}	while((readreg[0]&0x01));
}

void WriteFlashData( int txlen, uint8_t *txdata)
{
	volatile int i, j;
	uint8_t mode, delay;
	FlashWriteEnable();
	mode = HREAD(CORE_QSPI_CTRL);
	delay = HREAD(CORE_QSPI_DELAY);
//	FillSendCMD(qspi_txbuf, 0x02, flash_addr);
//	for(i = 0;i < txlen;i++)  
//		qspi_txbuf[i+4] = txdata[i];
	if (gMRAM_type == MRAM)
		HWRITE(CORE_QSPI_CTRL ,0x44);//mode&0xf8 |0x40
	else
		HWRITE(CORE_QSPI_CTRL ,0x04);//mode&0xf8 |0x40

		HWRITE(CORE_QSPI_DELAY , 0x0);
		HWRITEW(CORE_QSPI_TXADDR , (int)txdata);
		HWRITEW(CORE_QSPI_TXLEN , txlen );
		HWRITEW(CORE_QSPI_RXLEN , 0);
		HWRITE(CORE_DMA_START , 8);
		while(!(HREAD(CORE_DMA_STATUS) & 8) );
		HWRITE(CORE_QSPI_CTRL , mode);
		HWRITE(CORE_QSPI_DELAY , delay);
		WaitFlashFinish();
}


void  __attribute__((noinline,aligned(32))) QSPI_WriteFlashData(int flash_addr, int txlen, uint8_t *txdata)
{
	int i=0,j=0,x=0, offset=0,addr =0;
	if((flash_addr%Flash_PageLength + txlen) >Flash_PageLength)
	{
		offset = flash_addr%Flash_PageLength;
		QSPI_ReadFlashData(flash_addr-offset,offset,readtemp+4);
		QSPI_PageEraseFlash(flash_addr);
		
		FillSendCMD(readtemp, FlashCMD_PageProgram, flash_addr-offset);
		for(x = 0;x < Flash_PageLength-offset;x++)  
			readtemp[x+4+offset] = txdata[x];
		WriteFlashData(Flash_PageLength + 4,readtemp);

		i = txlen -(Flash_PageLength -offset);
		addr =  flash_addr- offset +Flash_PageLength;
		while(i>0)
		{
			j =  i>=Flash_PageLength?  Flash_PageLength : i ;
	//		QSPI_PageEraseFlash(addr);
			FillSendCMD(readtemp, FlashCMD_PageProgram, addr);
			for(x = 0;x < j ;x++)  
				readtemp[x+4] = txdata[x+txlen-i];
			WriteFlashData( j +4,readtemp);
			addr += Flash_PageLength;
			i -= j;
		}
	}	
	else
	{
		offset = flash_addr%Flash_PageLength;
		QSPI_ReadFlashData(flash_addr-offset,offset,readtemp+4);
		QSPI_PageEraseFlash(flash_addr);

		FillSendCMD(readtemp, FlashCMD_PageProgram, flash_addr-offset);
		for(x = 0;x < txlen ;x++)  
			readtemp[x+offset+4] = txdata[x];
		WriteFlashData(txlen + offset + 4,readtemp);	
	}
	
}
#if 0
void __attribute__((noinline,aligned(32))) QSPI_WriteFlashDataAuto(uint8_t block, int txlen, uint8_t *txdata)
{
	 int i,j,x=0;
	int addr_offset;
	uint8_t temp ;
	for(i=0;i<0x1000;i = i+0x20)
	{
		QSPI_ReadFlashData(0x10000*block+i,0x20,rx_temp);
		for(j=0;j<0x20;j++)
			if(rx_temp[j] != 0)
				break;
		if(j != 0x20)
			break;
	}
	
//USART_SendDataFromBuff(UARTB,rx_temp,0x40);	
//USART_SendDataFromBuff(UARTB,&i ,1);	
//USART_SendDataFromBuff(UARTB,&j ,1);	
	
	addr_offset = i+j;//bytes
	if((i == 0x1000) ||(addr_offset+(txlen/16) > 0x1000))
	{
		QSPI_64kEraseFlash(0x10000*block);
		QSPI_WriteFlashData(0x10000*block,txlen,txdata);
	}
	else
	{
		if(rx_temp[j] == 0xff)
		{
			x = 0;
			temp = 0;
		}
		else
		{
			temp =1;
			for(x=1;x<=8;x++)
			{
				if((rx_temp[j] | temp) == 0xff)
					break;
				temp = (temp<<1) +1;
			}
		}

		QSPI_WriteFlashData(0x10000*block+0x1000+((addr_offset)*8+x)*16,txlen,txdata);			
	}
	if( txlen%16==0 )
		i = addr_offset*8 + txlen/16 +x;//all bits
	else	
		i = addr_offset*8 + txlen/16 +1+x;//all bits	
	j=0;
	x=0;
	for(;i>0;i = i  -8)
	{
//		rx_temp[j] = 0;
		j++; //bytes
		x = i;//bits
	}
	temp =0xfe;	
	for(;x>1;x = x-1)	
		temp = (temp<<1) ;
	
	rx_temp[j-1] = temp;
	for(i=addr_offset;i<j-1;i++)
		rx_temp[i] = 0;
	

	QSPI_WriteFlashData(0x10000*block+addr_offset,j-addr_offset,rx_temp+addr_offset);					

USART_SendDataFromBuff(UARTB,&j ,1);	
USART_SendDataFromBuff(UARTB,&addr_offset ,1);	
USART_SendDataFromBuff(UARTB,&temp ,1);	
USART_SendDataFromBuff(UARTB,rx_temp+addr_offset ,1);	
	
}


void __attribute__((noinline,aligned(32))) QSPI_ReadFlashDataAuto(uint8_t block, int len, uint8_t *rxbuf)
{
	 int i,j,x=0;
	uint8_t temp ;
	int addr_offset;
	for(i=0;i<0x1000;i = i+0x20)
	{
		QSPI_ReadFlashData(0x10000*block+i,0x20,rx_temp);
		for(j=0;j<0x20;j++)
			if(rx_temp[j] != 0)
				break;
		if(j != 0x20)
			break;
	}
	addr_offset = i+j;//bytes
	
	if(rx_temp[j] == 0xff)
	{
		x = 0;
		temp = 0;
	}
	else
	{
		temp =1;
		for(x=1;x<=8;x++)
		{
			if((rx_temp[j] | temp) == 0xff)
				break;
			temp = (temp<<1) +1;
		}
	}
	
	if(len/16)
		temp = len/16;
	else
		temp = (len/16+1);
			
	QSPI_ReadFlashData(0x10000*block+0x1000+((addr_offset)*8+x-temp-1)*16,len,rxbuf);
	
}
#endif
void __attribute__((noinline,aligned(32))) SendFlashWithoutRx(uint8_t *txdata,int txlen)
{
	volatile int i, j;
	uint8_t mode, delay;
//	byte temp[8]={0,0,0,0,0,0,0,0};
//	for(i = 0;i < txlen;i++)  
//		temp[i] = txdata[i];
	mode = HREAD(CORE_QSPI_CTRL);
	delay = HREAD(CORE_QSPI_DELAY);

	if (gMRAM_type == MRAM)
		HWRITE(CORE_QSPI_CTRL,0x44 );// 0x40
	else
		HWRITE(CORE_QSPI_CTRL,0x04 );// 0x40

		HWRITE(CORE_QSPI_DELAY , 0x0);
		HWRITEW(CORE_QSPI_TXADDR , (int)txdata);
		HWRITEW(CORE_QSPI_TXLEN, txlen);
		HWRITEW(CORE_QSPI_RXLEN , 0);
		HWRITE(CORE_DMA_START , 8);
		while(!(HREAD(CORE_DMA_STATUS) & 8) );
		HWRITE(CORE_QSPI_CTRL , mode);
		HWRITE(CORE_QSPI_DELAY , delay);
		WaitFlashFinish();

}


void  __attribute__((noinline,aligned(32)))  QSPI_FlashDeepPowerDown()
{
	int i;
	temp[0] = FlashCMD_DeepPowerDown;
	SendFlashWithoutRx(temp,1);
	for(i=0;i<5000;i++);
}

void  __attribute__((noinline,aligned(32)))  FlashWriteEnable()
{
	temp2[0] = FlashCMD_WriteEnable;
	SendFlashWithoutRx(temp2,1);
//	ReadFlashSPI(&temp,1,&temp+1,0);  
//	ReadFlashSPI(&temp,1,NULL,0);  
}

void  __attribute__((noinline,aligned(32)))  QSPI_FlashWriteDisable()
{
	temp[0] = FlashCMD_WriteDisable;
	SendFlashWithoutRx(temp,1);
}

void __attribute__((noinline,aligned(32))) QSPI_PageEraseFlash(int flash_addr)
{
	FillSendCMD(temp, FlashCMD_PageErase, flash_addr);
	FlashWriteEnable();	
	SendFlashWithoutRx(temp,4);
}

void __attribute__((noinline,aligned(32))) QSPI_SectorEraseFlash(int flash_addr)
{
	FillSendCMD(temp, FlashCMD_SectorErase, flash_addr);
	FlashWriteEnable();	
	SendFlashWithoutRx(temp,4);
}

void __attribute__((noinline,aligned(32)))  QSPI_32kEraseFlash(int flash_addr)
{
	FillSendCMD(temp, FlashCMD_32kErase, flash_addr);
	FlashWriteEnable();	
	SendFlashWithoutRx(temp,4);
}

void  __attribute__((noinline,aligned(32)))  QSPI_64kEraseFlash(int flash_addr)
{
	FillSendCMD(temp, FlashCMD_64kErase, flash_addr);
	FlashWriteEnable();	
	SendFlashWithoutRx(temp,4);
}


void __attribute__((noinline,aligned(32)))   QSPI_ChipEraseFlash()
{
	temp[0] = FlashCMD_ChipErase;
	FlashWriteEnable();	
	SendFlashWithoutRx(temp,1);
}


void   __attribute__((noinline,aligned(32)))  QSPI_WriteFlashReg(uint8_t *txbuf)
{
	temp[0] = FlashCMD_WriteRegister;
	temp[1] = txbuf[0];
	temp[2] = txbuf[1];
	FlashWriteEnable();
	SendFlashWithoutRx(temp,3);	//s15-s8
}

void   __attribute__((noinline,aligned(32)))   QSPI_FlashQuadEnable(uint8_t on)
{
	QSPI_ReadFlashReg(temp);
	if(on)
		temp[1] = temp[1] |0x02;	
	else
		temp[1] = temp[1] &0xfd;
	QSPI_WriteFlashReg(temp);
}


void __attribute__((noinline,aligned(32)))    End()
{
	__ASM("nop");
}

void __attribute__((noinline,aligned(32)))   prefetch(int start_addr, int end_addr)
{
	int addr= 0;
	for(addr = (int)start_addr & (~0x1f);addr < (int)end_addr + 32;addr += 32)
	{
		__ASM("ldr r0,[%0]": : "r"(addr) :"%r0");
//			__ASM("ldr r0,[%0]": : "r"(addr) :);
	}
}

void QSPI_Init()
{
	int a,b;
	a = (int)Start;
	b = (int)End;
	if(a<b)
		prefetch(a,b);
	else
		prefetch(b,a);
}




