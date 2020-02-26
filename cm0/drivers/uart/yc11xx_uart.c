#include "yc11xx_uart.h"
#include "Drv_debug.h"
#include "kdevice.h"
#include <stdlib.h>


#define IS_USART_TXLen(TxLen)       (TxLen != 0) 
  
#define IS_USART_RXLen(RxLen)         (RxLen != 0)

#define IS_UARTE_BAUDRATE(BAUDRATE) ((BAUDRATE > 0) && (BAUDRATE< UARTE_BAUDRATE_BAUDRATE_Baud1M))

#define IS_USART_WORD_LENGTH(LENGTH) 		(((LENGTH) == USART_WordLength_8b) || \
                                    	((LENGTH) == USART_WordLength_9b))

#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_ENABLE))
typedef struct 
{
	uint16_t Baudrate;
	uint16_t RxSadr;
	uint16_t RxEadr;
	uint16_t RxRptr;
	uint16_t TxSadr;
	uint16_t TxEadr;
	uint16_t TxWptr;
}UartxRegDef;

typedef struct
{
	UartxRegDef rbu;
	uint8_t cbu;
}UartxRegControlBackup;

#define UART_PER_NUM 2

UartxRegControlBackup regBeck[UART_PER_NUM];


static void usartInitByBackup(USART_TypeDef USARTx)
{
	uint16_t UartxCtrlAdr = 0;
	UartxRegDef *UartAdr = NULL;
	/*set uart control*/
	if(USARTx == UARTA) {
		UartxCtrlAdr = CORE_UART_CTRL;
		UartAdr = (UartxRegDef *)(reg_map(CORE_UART_BAUD));
	}else if(USARTx == UARTB) {
		UartxCtrlAdr = CORE_UARTB_CTRL;
		UartAdr = (UartxRegDef *)(reg_map(CORE_UARTB_BAUD));
	}

	/*init all reg by backup*/
	HW_REG_16BIT(((uint32_t)(&UartAdr->Baudrate)), regBeck[USARTx].rbu.Baudrate);
	HW_REG_16BIT(((uint32_t)(&UartAdr->TxSadr)), (uint32_t)regBeck[USARTx].rbu.TxSadr);
	HW_REG_16BIT(((uint32_t)(&UartAdr->TxEadr)), (uint32_t)regBeck[USARTx].rbu.TxEadr);
	HW_REG_16BIT(((uint32_t)(&UartAdr->TxWptr)), (uint32_t)regBeck[USARTx].rbu.TxSadr);
	HW_REG_16BIT(((uint32_t)(&UartAdr->RxSadr)), (uint32_t)regBeck[USARTx].rbu.RxSadr);
	HW_REG_16BIT(((uint32_t)(&UartAdr->RxEadr)), (uint32_t)regBeck[USARTx].rbu.RxEadr);
	HW_REG_16BIT(((uint32_t)(&UartAdr->RxRptr)), (uint32_t)regBeck[USARTx].rbu.RxSadr);
	HWOR(UartxCtrlAdr, regBeck[USARTx].cbu);
}

static error_t usartAReinit()
{
	usartInitByBackup(UARTA);
	return SUCCESS;
}

static error_t usartBReinit()
{
	usartInitByBackup(UARTB);
	return SUCCESS;
}


void USART_Init(USART_TypeDef USARTx, USART_InitTypeDef* USART_InitStruct)
{	
#define UARTC_BIT_ENABLE (1<<0)
#define BAUD_USE_SETTING (1<<7)
	HWRITE(CORE_UART_CLKSEL, 1);
	HWCOR(CORE_CLKOFF + 1, 0x80);


	void *Ptr = NULL;
	uint8_t CtrValue = 0;

	/*check parameter*/
	_ASSERT(USART_InitStruct != NULL);
	_ASSERT(IS_USARTAB(USARTx));
	_ASSERT(IS_UARTE_BAUDRATE(USART_InitStruct->USART_BaudRate));
	_ASSERT(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
	_ASSERT(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
	_ASSERT(IS_USART_PARITY(USART_InitStruct->USART_Parity));
	_ASSERT(IS_USART_MODE(USART_InitStruct->USART_Mode));
	_ASSERT(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
	_ASSERT(IS_USART_TXLen(USART_InitStruct->USART_TXLen));
	_ASSERT(IS_USART_RXLen(USART_InitStruct->USART_RXLen));

	/*init baud backup*/
	regBeck[USARTx].rbu.Baudrate = USART_InitStruct->USART_BaudRate;
	
	/*init tx ring buffer backup*/
	Ptr = malloc(USART_InitStruct->USART_TXLen);
	regBeck[USARTx].rbu.TxSadr = (uint32_t)Ptr;

	_ASSERT(Ptr != NULL);	
	regBeck[USARTx].rbu.TxEadr = ((uint32_t)Ptr) + USART_InitStruct->USART_TXLen;
	Ptr = NULL;
	
	/*init rx ring buffer backup*/
	Ptr = malloc(USART_InitStruct->USART_RXLen);
	regBeck[USARTx].rbu.RxSadr = (uint32_t)Ptr;
	_ASSERT(Ptr != NULL);	
	regBeck[USARTx].rbu.RxEadr = ((uint32_t)Ptr) + USART_InitStruct->USART_RXLen;

	CtrValue =  USART_InitStruct->USART_Mode | USART_InitStruct->USART_HardwareFlowControl |\
			  USART_InitStruct->USART_Parity |USART_InitStruct->USART_StopBits| \
			  USART_InitStruct->USART_WordLength|BAUD_USE_SETTING|UARTC_BIT_ENABLE;
	
	regBeck[USARTx].cbu = CtrValue;

	/*init rx ring buffer regs*/
	if(USARTx == UARTA) {
		usartInitByBackup(UARTA);
		//DEV_Register(Device_Id_UartA, usartAReinit, NULL);
	}
	else {
		usartInitByBackup(UARTB);
		//DEV_Register(Device_Id_UartB, usartBReinit, NULL);
	}

}


void USART_DeInit(USART_TypeDef USARTx)
{
	UartxRegDef *UartAdr = NULL;
	_ASSERT(IS_USARTAB(USARTx));
	
	if(USARTx == UARTA) {
		HWOR(reg_map(CORE_UART_CTRL), (1<<0));
		HWOR(reg_map(CORE_UART_CTRL), (0<<0));
		UartAdr = (UartxRegDef *)(reg_map(CORE_UART_BAUD));
	}else {
		HWOR(reg_map(CORE_UARTB_CTRL), (1<<0));
		HWOR(reg_map(CORE_UARTB_CTRL), (0<<0));
		UartAdr = (UartxRegDef *)(reg_map(CORE_UARTB_BAUD));
	}
	free((void *)((uint32_t)(UartAdr->TxSadr)));
	free((void *)((uint32_t)(UartAdr->RxSadr)));
}



void USART_SendData(USART_TypeDef USARTx, uint16_t Data)
{
	UartxRegDef * UartAdr = NULL;
	uint16_t  WPtr = 0; 
	_ASSERT(IS_USARTAB(USARTx));
	
	if(USARTx == UARTA) {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UART_BAUD));	
	}else {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UARTB_BAUD));
	}
	_ASSERT((&UartAdr->TxSadr  != NULL));
	
	WPtr = HR_REG_16BIT((uint32_t)(&UartAdr->TxWptr));
	if(USARTx == UARTA) {
		while(HREAD(CORE_UART_TX_ITEMS) > 0);
	}else {
		while(HREAD(CORE_UARTB_TX_ITEMS) > 0);
	}	
	HW_REG_8BIT(WPtr|M0_MEMORY_BASE,Data);	
	RB_UPDATE_PTR(WPtr, HR_REG_16BIT((uint32_t)(&UartAdr->TxSadr)),  HR_REG_16BIT((uint32_t)(&UartAdr->TxEadr)));	
	HW_REG_16BIT((uint32_t)(&UartAdr->TxWptr),  WPtr);                                                                                                		
}



uint16_t USART_ReceiveData(USART_TypeDef USARTx)
{	
	UartxRegDef *UartAdr = NULL;
	uint16_t  RPtr = 0;
	uint16_t  RdData = 0;
	_ASSERT(IS_USARTAB(USARTx)); 

	if(USARTx == UARTA) {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UART_BAUD));	
	}else {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UARTB_BAUD));
	}
	RPtr = HR_REG_16BIT((uint32_t)(&UartAdr->RxRptr));
	RdData = HR_REG_16BIT(RPtr|M0_MEMORY_BASE);
	RB_UPDATE_PTR(RPtr, HR_REG_16BIT((uint32_t)(&UartAdr->RxSadr)), HR_REG_16BIT((uint32_t)(&UartAdr->RxEadr)));
	HW_REG_16BIT((uint32_t)(&UartAdr->RxRptr), RPtr);	
	return RdData;
}


uint16_t USART_GetRxCount(USART_TypeDef USARTx)
{
	_ASSERT(IS_USARTAB(USARTx));
	if(USARTx == UARTA) {
		return HR_REG_16BIT(reg_map(CORE_UART_RX_ITEMS));
	}else {
		return HR_REG_16BIT(reg_map(CORE_UARTB_RX_ITEMS));
	}
}


uint16_t USART_ReadDatatoBuff(USART_TypeDef USARTx, uint8_t* RxBuff, uint16_t RxSize)
{
	uint16_t RxLen = 0;	
    	uint16_t RPtr = 0;
    	uint16_t RdataLen = 0;
   	uint32_t RxITEMS = 0;
	UartxRegDef *UartAdr = NULL;
    	_ASSERT(IS_USARTAB(USARTx));
	_ASSERT(RxBuff != NULL);
	
	if(USARTx == UARTA) {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UART_BAUD));
		RxITEMS = reg_map(CORE_UART_RX_ITEMS);
	}else {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UARTB_BAUD));
		RxITEMS = reg_map(CORE_UARTB_RX_ITEMS);
	}
	RxLen = HR_REG_16BIT(reg_map(RxITEMS)); 
	if (RxSize!=0) {
		if (RxLen < RxSize) return 0; else RxLen = RxSize;
	}
	 if (0 == RxLen) {
        		return 0;
    	} else {
		RPtr = HR_REG_16BIT((uint32_t)(&UartAdr->RxRptr));
		for(RdataLen = 0; RdataLen<RxLen; RdataLen++ )
		{
                			
        				RxBuff[RdataLen] = HR_REG_8BIT(RPtr|M0_MEMORY_BASE);   
        				RB_UPDATE_PTR(RPtr, HR_REG_16BIT((uint32_t)(&UartAdr->RxSadr)), HR_REG_16BIT((uint32_t)(&UartAdr->RxEadr)));
		}
    	}
	HW_REG_16BIT((uint32_t)(&UartAdr->RxRptr), (RPtr));
	return RdataLen;
}


uint16_t USART_SendDataFromBuff(USART_TypeDef USARTx, uint8_t* TxBuff, uint16_t TxLen)
{
	uint16_t  WPtr = 0;
	uint16_t  SDataLen = 0;	
	UartxRegDef *UartAdr = NULL;
	_ASSERT(IS_USARTAB(USARTx));
	_ASSERT(TxBuff != 0);
	_ASSERT(TxLen > 0);
	
	if(USARTx == UARTA) {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UART_BAUD));	
	}else {
		UartAdr = (UartxRegDef *)(reg_map(CORE_UARTB_BAUD));
	}
   	WPtr = HR_REG_16BIT((uint32_t)(&UartAdr->TxWptr));
   	for (uint16_t i=0; i<TxLen; i++)  {	
		
		HW_REG_8BIT(WPtr|M0_MEMORY_BASE,TxBuff[i]);	
		RB_UPDATE_PTR(WPtr, HR_REG_16BIT((uint32_t)(&UartAdr->TxSadr)),  HR_REG_16BIT((uint32_t)(&UartAdr->TxEadr)));	   
		SDataLen++;	
	}
	HW_REG_16BIT((uint32_t)(&UartAdr->TxWptr),  WPtr);  
	return SDataLen;
}


