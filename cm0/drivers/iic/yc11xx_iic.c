#include "yc11xx_iic.h"

#define IS_IIC_TXLen(TxLen)   	(TxLen != 0)
#define IS_IIC_RXLen(RxLen)    	(RxLen != 0)
#define IS_IICD_CLOCKSPEED(CLOCKSPEED)  ((CLOCKSPEED>=IICD_CLOCKSPEED1MHZ) && (CLOCKSPEED<=IICD_CLOCKSPEED200KHZ))

typedef struct
{
	uint8_t Ctrl;
	uint8_t Scll;
	uint8_t Sclh;
	uint8_t Stsu;
	uint8_t Sthd;
	uint8_t Sosu;
	uint8_t Dtsu;
	uint8_t Dthd;
	uint16_t TxLen;
	uint16_t TxAddr;
	uint16_t RxAddr;
	uint16_t RxLen;
} __packed I2C_RegDef;

void I2C_Init(I2C_InitTypeDef* I2C_InitStruct)
{
#define AUTO_DMA_AFTER_RW_BIT 		0
#define RESTART_BETWEEN_WR_BIT 	1
#define SELECT_RAM_BIT 				2
#define IICD_CTRL_VAL  ((1<<AUTO_DMA_AFTER_RW_BIT)|\
						(1<<RESTART_BETWEEN_WR_BIT)|\
						(1<<SELECT_RAM_BIT))

	I2C_RegDef* I2cAdr = NULL;
	uint16_t* Ptr = NULL;
	
	/*check parameter*/
	_ASSERT(I2C_InitStruct != NULL);
	_ASSERT(IS_IICD_CLOCKSPEED(I2C_InitStruct->I2C_ClockSpeed));
	_ASSERT(IS_IIC_TXLen(I2C_InitStruct->I2C_TXLen));
	_ASSERT(IS_IIC_RXLen(I2C_InitStruct->I2C_RXLen));

	I2cAdr = (I2C_RegDef*)(reg_map(CORE_IICD_CTRL));
	HW_REG_8BIT((uint32_t)&I2cAdr->Ctrl, IICD_CTRL_VAL);
	
	/*set iic clock speed*/
	I2cAdr->Scll = I2C_InitStruct->I2C_ClockSpeed-1;
	I2cAdr->Sclh = I2C_InitStruct->I2C_ClockSpeed;
	I2cAdr->Stsu = I2C_InitStruct->I2C_ClockSpeed;
	I2cAdr->Sthd = I2C_InitStruct->I2C_ClockSpeed;
	I2cAdr->Sosu = I2C_InitStruct->I2C_ClockSpeed;
	I2cAdr->Dtsu = I2C_InitStruct->I2C_ClockSpeed-1;

	/*set iic tx buff*/
	Ptr = malloc(I2C_InitStruct->I2C_TXLen);
	HW_REG_16BIT((uint32_t)&I2cAdr->TxAddr, (uint32_t)Ptr);
	_ASSERT(Ptr != NULL);
	HW_REG_16BIT((uint32_t)&I2cAdr->TxLen, 0);
	Ptr = NULL;
	
	/*set iic rx buff*/
	Ptr = malloc(I2C_InitStruct->I2C_RXLen);
	HW_REG_16BIT((uint32_t)&I2cAdr->RxAddr,(uint32_t) Ptr);
	_ASSERT(Ptr != NULL);	
	HW_REG_16BIT((uint32_t)&I2cAdr->RxLen, 0);
}

void I2C_DeInit(void)
{
	I2C_RegDef* I2cAdr = NULL;
	I2cAdr = (I2C_RegDef *)(reg_map(CORE_IICD_CTRL));
	free((void *)((uint32_t)I2cAdr->RxAddr));
	free((void *)((uint32_t)I2cAdr->TxAddr));
}

#define START_IIC_DMA 			0x04
#define CHECK_IIC_DMA_DONE		0x20

#define IIC_TXLEN_4BYTES		 4
#define IIC_TXLEN_3BYTES		 3
#define IIC_TXLEN_2BYTES		 2

void I2C_SendData(uint32_t DevAddr, uint32_t RegAddr, uint16_t Data)
{
	I2C_RegDef* I2cAdr = NULL;
	uint8_t * I2cTxPtr = NULL;
	
	_ASSERT(IS_I2C_REGADDR(RegAddr));

	I2cAdr = (I2C_RegDef *)(reg_map(CORE_IICD_CTRL));
	I2cTxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)&I2cAdr->TxAddr));

	DevAddr = DevAddr<<0x01;
	*I2cTxPtr++ = DevAddr & 0xff;
	if(IS_I2C_REGADDR16BITS(RegAddr)) {
		*I2cTxPtr++ = RegAddr>> 8;
		*I2cTxPtr++ = RegAddr & 0xff;
		*I2cTxPtr++ = Data & 0xff;
		HW_REG_16BIT((uint32_t )&I2cAdr->TxLen, IIC_TXLEN_4BYTES);
	}else {
		*I2cTxPtr++ = RegAddr & 0xff;
		*I2cTxPtr++ = Data & 0xff;
		HW_REG_16BIT((uint32_t )&I2cAdr->TxLen, IIC_TXLEN_3BYTES);
	}
	HW_REG_16BIT((uint32_t)&I2cAdr->RxLen, 0);
	
	HWRITE(CORE_DMA_START,START_IIC_DMA);	
	while(!(HREAD(CORE_DMA_STATUS) & (CHECK_IIC_DMA_DONE)));
}

uint32_t I2C_ReceiveData(uint32_t DevAddr, uint32_t RegAddr)
{
	I2C_RegDef* I2cAdr = NULL;
	uint8_t * I2cTxPtr = NULL;
	uint8_t * I2cRxPtr = NULL;

	_ASSERT(IS_I2C_REGADDR(RegAddr));

	I2cAdr = (I2C_RegDef *)(reg_map(CORE_IICD_CTRL));
	I2cTxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)&I2cAdr->TxAddr));
	I2cRxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)&I2cAdr->RxAddr));

	DevAddr = DevAddr<<0x01;
	*I2cTxPtr++ = DevAddr & 0xff;
	if(IS_I2C_REGADDR16BITS(RegAddr)) {
		*I2cTxPtr++ = RegAddr >> 8;
		*I2cTxPtr++ = RegAddr & 0xff; 
		*I2cTxPtr++ = DevAddr | 0x01;
		HW_REG_16BIT((uint32_t)&I2cAdr->TxLen,
		IIC_TXLEN_4BYTES);
	}else {
		*I2cTxPtr++ = RegAddr & 0xff;
		*I2cTxPtr++ = DevAddr | 0x01;
		HW_REG_16BIT((uint32_t)&I2cAdr->TxLen, IIC_TXLEN_3BYTES);
	}

	HW_REG_16BIT((uint32_t )&I2cAdr->RxLen, 1);
	
	HWRITE(CORE_DMA_START, START_IIC_DMA);
	while(!(HREAD(CORE_DMA_STATUS) & (CHECK_IIC_DMA_DONE)));

	return (uint32_t)(*I2cRxPtr);
}

void  I2C_ReadDatatoBuff(uint32_t DevAddr, 
								uint32_t RegAddr, 
								uint8_t *RxBuff, 
								uint16_t RxLen)
{
	I2C_RegDef * I2cAdr = NULL;
	uint8_t * I2cTxPtr = NULL;
	uint8_t * I2cRxPtr = NULL;

	_ASSERT(IS_I2C_REGADDR(RegAddr));
	_ASSERT(IS_IIC_RXLen(RxLen));

	I2cAdr = (I2C_RegDef *)(reg_map(CORE_IICD_CTRL));
	I2cTxPtr = (uint8_t *)reg_map_m0(HR_REG_16BIT((uint32_t)&I2cAdr->TxAddr));
	I2cRxPtr = (uint8_t *)reg_map_m0(HR_REG_16BIT((uint32_t)&I2cAdr->RxAddr));

	DevAddr = DevAddr<<0x01;
	*I2cTxPtr++ = DevAddr & 0xff;
	if(IS_I2C_REGADDR16BITS(RegAddr)) {
		*I2cTxPtr++ = RegAddr >> 8;
		*I2cTxPtr++ = RegAddr & 0xff;
		*I2cTxPtr++ = DevAddr | 0x01;
		HW_REG_16BIT((uint32_t)&I2cAdr->TxLen, IIC_TXLEN_4BYTES);
	}else {
		*I2cTxPtr++ = RegAddr & 0xff;
		*I2cTxPtr++ = DevAddr | 0x01;
		HW_REG_16BIT((uint32_t)&I2cAdr->TxLen, IIC_TXLEN_3BYTES);
	}
	
	HW_REG_16BIT((uint32_t )&I2cAdr->RxLen, RxLen);
	HWRITE(CORE_DMA_START, START_IIC_DMA);	
	while(!(HREAD(CORE_DMA_STATUS) & (CHECK_IIC_DMA_DONE)));
	
	memcpy(RxBuff, I2cRxPtr, RxLen);	
}

void I2C_SendDataFromBuff(uint32_t DevAddr, 
										uint32_t RegAddr, 
										uint8_t *TxBuff, 
										uint16_t TxLen)
{
	I2C_RegDef * I2cAdr = NULL;
	uint8_t * I2cTxPtr = NULL;

	_ASSERT(IS_I2C_REGADDR(RegAddr));
	_ASSERT(IS_IIC_TXLen(TxLen));

	I2cAdr = (I2C_RegDef *)(reg_map(CORE_IICD_CTRL));
	I2cTxPtr = (uint8_t *)reg_map_m0(HR_REG_16BIT((uint32_t)&I2cAdr->TxAddr));

	DevAddr = DevAddr<<0x01;
	*I2cTxPtr++ = DevAddr & 0xff;
	if(IS_I2C_REGADDR16BITS(RegAddr)) {
		*I2cTxPtr++ = RegAddr >> 8;
		*I2cTxPtr++ = RegAddr & 0xff;
		memcpy(I2cTxPtr, TxBuff, TxLen);
		HW_REG_16BIT((uint32_t )&I2cAdr->TxLen, TxLen+IIC_TXLEN_3BYTES);
	}else {
		*I2cTxPtr++ = RegAddr & 0xff;
		memcpy(I2cTxPtr, TxBuff, TxLen);
		HW_REG_16BIT((uint32_t )&I2cAdr->TxLen, TxLen+IIC_TXLEN_2BYTES);
	}

	HW_REG_16BIT((uint32_t)&I2cAdr->RxLen, 0);
	HWRITE(CORE_DMA_START, START_IIC_DMA);
	while(!(HREAD(CORE_DMA_STATUS) & (CHECK_IIC_DMA_DONE)));
}
