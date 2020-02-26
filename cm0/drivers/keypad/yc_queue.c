/**
 * COPYRIGHT NOTICE
 *Copyright(c) 2014,YICHIP
 *
 * @file yc_queue.c
 * @brief ...
 *
 * @version 1.0
 * @author  jingzou
 * @data    Jan 24, 2018
**/
#include "yc_queue.h"
#include<stdio.h>
#include<string.h>

bool queue_init(QUEUE_HEADER *pQheader, void *pMem, uint32_t unitLen, uint32_t len)
{
//	printf("Queue initial\r\n");
	if (pQheader == NULL || pMem == NULL || unitLen == 0 || len == 0)
	{
		return false;
	}

	pQheader->pRead = pMem;
	pQheader->pWrite = pMem;
	pQheader->pHead = pMem;
	pQheader->array_Len = len;
	pQheader->unit_Len = unitLen;
	pQheader->current_queue_len = 0;
	return true;
}

bool queue_clear(QUEUE_HEADER *pQheader)
{
	if (pQheader == NULL)
	{
		return false;
	}

	pQheader->pRead = pQheader->pHead;
	pQheader->pWrite = pQheader->pHead;
	pQheader->current_queue_len = 0;
	return true;
}

bool preinsert_Queue(QUEUE_HEADER *pQheader,void **pEle)
{
	uint8_t i;
	if (queue_is_full(pQheader))
	{
		return false;
	}

	//memcpy(pQheader->pWrite,pEle,pQheader->unit_Len);
	(*pEle) = pQheader->pWrite;
	pQheader->pWrite = (void *)(((uint8_t *)pQheader->pWrite)+pQheader->unit_Len);
	if ((uint8_t *)pQheader->pWrite == ((uint8_t *)pQheader->pHead) + (pQheader->unit_Len*pQheader->array_Len))
	{
		pQheader->pWrite = pQheader->pHead;
	}
	pQheader->current_queue_len++;
	//YC_LOG_INFO("put queue = %x,%d \r\n",(uint32_t)pQheader->pWrite,pQheader->current_queue_len);
	return true;

}

bool Insert_Queue(QUEUE_HEADER *pQheader,void *pEle)
{
	uint8_t i;
	//unsigned char * pTemp;
	if (queue_is_full(pQheader))
	{
		return false;
	}

	memcpy(pQheader->pWrite,pEle,pQheader->unit_Len);
	pQheader->pWrite = (void *)(((uint8_t *)pQheader->pWrite)+pQheader->unit_Len);
	if ((uint8_t *)pQheader->pWrite == ((uint8_t *)pQheader->pHead) + (pQheader->unit_Len*pQheader->array_Len))
	{
		pQheader->pWrite = pQheader->pHead;
	}
	pQheader->current_queue_len++;
	//YC_LOG_INFO("put queue = %x,%d \r\n",(uint32_t)pQheader->pWrite,pQheader->current_queue_len);
	return true;
}

void * Delete_Queue(QUEUE_HEADER *pQheader)
{
	void *pTemp;
	if (queue_is_empty(pQheader))
	{
		return NULL;
	}

	pTemp = pQheader->pRead;
	pQheader->pRead = (void *)(((unsigned char *)pQheader->pRead)+ pQheader->unit_Len);
	if ((uint8_t *)pQheader->pRead == ((uint8_t *)pQheader->pHead) + (pQheader->unit_Len*pQheader->array_Len))
	{
		pQheader->pRead = pQheader->pHead;
	}
	pQheader->current_queue_len--;
	//YC_LOG_INFO("get queue = %x,%d \r\n",(uint32_t)pTemp,pQheader->current_queue_len);
	return pTemp;
}

void * Header_Queue(QUEUE_HEADER *pQheader)
{
	void *pTemp;
	if (queue_is_empty(pQheader))
	{
		return NULL;
	}

	pTemp = pQheader->pRead;
	return pTemp;

}
bool queue_is_empty(QUEUE_HEADER *pQheader)
{
	if (pQheader->current_queue_len == 0)
	{
		return true;
	}
	return false;
}

bool queue_is_full(QUEUE_HEADER *pQheader)
{
	if (pQheader->current_queue_len == pQheader->array_Len)
	{
		return true;
	}
	return false;
}
uint8_t queue_ele_count(QUEUE_HEADER *pQheader)
{
	return pQheader->current_queue_len;
}
