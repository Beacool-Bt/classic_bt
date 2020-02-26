/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 #include "yc11xx_spi.h"
#include <stdlib.h>
#include <string.h>


#define IS_SPI_TXLen(TxLen)       (TxLen != 0)
#define IS_SPI_RXLen(RxLen)         (RxLen != 0)


#define YC1121B
typedef struct 
{
	uint8_t Ctrl;
	uint8_t Dealy;
	uint16_t TxLen;
	uint16_t TxAddr;
	uint16_t RxAddr;
	uint16_t RxLen;
} __packed Spix_RegDef;

void SPI_Init(SPI_InitTypeDef* SPI_InitStruct)
{
#define SPI_AUTO_INCR_ADDR		((uint8_t)1<<6)
	register uint16_t SpixCtrl = 0;
	Spix_RegDef * SpiAdr = NULL;
	void *Ptr = NULL;

	/*check parameter*/
	_ASSERT(SPI_InitStruct != NULL);
	_ASSERT(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
	_ASSERT(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
	_ASSERT(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
	_ASSERT(IS_SPI_BAUDSPEED(SPI_InitStruct->SPI_BaudSpeed));
	_ASSERT(IS_SPI_TXLen(SPI_InitStruct->SPI_TXLen));
	_ASSERT(IS_SPI_RXLen(SPI_InitStruct->SPI_RXlen));
	
	SpiAdr = (Spix_RegDef *)(reg_map(CORE_SPID_CTRL));

	/*set spi control*/
	SpixCtrl = SPI_InitStruct->SPI_BaudSpeed | \
			SPI_InitStruct->SPI_CPOL | \
			SPI_InitStruct->SPI_CPHA | \
			SPI_AUTO_INCR_ADDR;
	HWRITE((uint32_t)&SpiAdr->Ctrl, SpixCtrl);

#ifdef YC1121B
	/*set spi delay between read and write*/
	HWRITE((uint32_t)&SpiAdr->Dealy, 0x0a);

	/*init spi tx buff*/
	HW_REG_16BIT((uint32_t)&SpiAdr->TxAddr, 0x4f00);
	HW_REG_16BIT((uint32_t)&SpiAdr->TxLen, 0);

	/*init spi tx buff*/
	HW_REG_16BIT((uint32_t)&SpiAdr->RxAddr, 0x4f80);
	HW_REG_16BIT((uint32_t)&SpiAdr->RxLen, 0);

#else
	/*set spi delay between read and write*/
	HWRITE(SpiAdr->Dealy, 0x8a);

	/*init spi tx buff*/
	Ptr = malloc(SPI_InitStruct->SPI_TXLen);
	HW_REG_16BIT((uint32_t)&SpiAdr->TxAddr, Ptr);
	_ASSERT(Ptr != NULL);	
	HW_REG_16BIT((uint32_t)&SpiAdr->TxLen, 0);
	Ptr = NULL;

	/*init spi tx buff*/
	Ptr = malloc(SPI_InitStruct->SPI_RXlen);
	HW_REG_16BIT((uint32_t)&SpiAdr->RxAddr, Ptr);
	_ASSERT(Ptr != NULL);	
	HW_REG_16BIT((uint32_t)&SpiAdr->RxLen, 0);
#endif
}


void SPI_DeInit(void)
{
	Spix_RegDef * SpiAdr = NULL;

	SpiAdr = (Spix_RegDef *)(reg_map(CORE_SPID_CTRL));
	free((void *)((uint32_t)SpiAdr->RxAddr));
	free((void *)((uint32_t)SpiAdr->TxAddr));
}

#define START_SPI_DMA			0x02	//start spi dma
#define CHECK_SPI_DMA_DONE		0x40	//check spi dma is done

void SPI_SendData(uint16_t Data)
{
	Spix_RegDef * SpiAdr = NULL;
	uint8_t * SpiTxPtr = NULL;

	SpiAdr = (Spix_RegDef *)(reg_map(CORE_SPID_CTRL));
#ifdef YC1121B
	SpiTxPtr = (uint8_t *) reg_map(HR_REG_16BIT((uint32_t)&SpiAdr->TxAddr));
#else
	SpiTxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)SpiAdr->TxAddr));
#endif
	*SpiTxPtr = Data;

	HW_REG_16BIT((uint32_t)&SpiAdr->TxLen, 1);
	HW_REG_16BIT((uint32_t)&SpiAdr->RxLen, 0);
	HWRITE(CORE_DMA_START, START_SPI_DMA);
	while(!(HREAD(CORE_DMA_STATUS) & CHECK_SPI_DMA_DONE));
}


uint16_t SPI_ReadRegister(uint16_t RegAdr)
{
	Spix_RegDef * SpiAdr = NULL;
	uint8_t * SpiRxPtr = NULL;
	uint8_t * SpiTxPtr = NULL;

	SpiAdr = (Spix_RegDef *)(reg_map(CORE_SPID_CTRL));
#ifdef YC1121B
	SpiRxPtr = (uint8_t *) reg_map(HR_REG_16BIT((uint32_t)&SpiAdr->RxAddr));
	SpiTxPtr = (uint8_t *) reg_map(HR_REG_16BIT((uint32_t)&SpiAdr->TxAddr));
#else
	SpiRxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)SpiAdr->RxAddr));
	SpiTxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)SpiAdr->TxAddr));
#endif
	*SpiTxPtr =RegAdr;
	
	HW_REG_16BIT((uint32_t)&SpiAdr->TxLen, 1);
	HW_REG_16BIT((uint32_t)&SpiAdr->RxLen, 1);
	HWRITE(CORE_DMA_START, START_SPI_DMA);
	while(!(HREAD(CORE_DMA_STATUS) & CHECK_SPI_DMA_DONE));

	return *SpiRxPtr;

}

void SPI_SendDataFromBuff(uint8_t *Buff, uint16_t Len)
{
	Spix_RegDef * SpiAdr = NULL;
	uint8_t * SpiTxPtr = NULL;

	_ASSERT(Len);
	_ASSERT(Buff != NULL);

	SpiAdr = (Spix_RegDef *)(reg_map(CORE_SPID_CTRL));
#ifdef YC1121B
	SpiTxPtr = (uint8_t *) reg_map(HR_REG_16BIT((uint32_t)&SpiAdr->TxAddr));
#else
	SpiTxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)SpiAdr->TxAddr));
#endif
	memcpy(SpiTxPtr, Buff, Len);

	HW_REG_16BIT((uint32_t)&SpiAdr->TxLen, Len);
	HW_REG_16BIT((uint32_t)&SpiAdr->RxLen, 0);
	HWRITE(CORE_DMA_START, START_SPI_DMA);
	while(!(HREAD(CORE_DMA_STATUS) & CHECK_SPI_DMA_DONE));
}

void SPI_ReceiveDataToBuff(uint8_t *TxBuff, 
									uint16_t TxLen, 
									uint8_t *RxBuff, 
									uint16_t RxLen)
{
	Spix_RegDef * SpiAdr = NULL;
	uint8_t * SpiTxPtr = NULL;
	uint8_t * SpiRxPtr = NULL;

	_ASSERT(TxLen);
	_ASSERT(TxBuff != NULL);
	_ASSERT(RxBuff != NULL);

	SpiAdr = (Spix_RegDef *)(reg_map(CORE_SPID_CTRL));
#ifdef YC1121B
	SpiTxPtr = (uint8_t *) reg_map(HR_REG_16BIT((uint32_t)&SpiAdr->TxAddr));
	SpiRxPtr = (uint8_t *) reg_map(HR_REG_16BIT((uint32_t)&SpiAdr->RxAddr));
#else
	SpiTxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)SpiAdr->TxAddr));
	SpiRxPtr = (uint8_t *) reg_map_m0(HR_REG_16BIT((uint32_t)SpiAdr->RxAddr));
#endif
	memcpy(SpiTxPtr, TxBuff, TxLen);

	HW_REG_16BIT((uint32_t)&SpiAdr->TxLen, TxLen);
	HW_REG_16BIT((uint32_t)&SpiAdr->RxLen, RxLen);
	HWRITE(CORE_DMA_START, START_SPI_DMA);
	while(!(HREAD(CORE_DMA_STATUS) & CHECK_SPI_DMA_DONE));
	
	memcpy(RxBuff, SpiRxPtr, RxLen);
}


 
