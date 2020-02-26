/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
/** 
  *@file yc_queue.h
  *@brief USB support for application.
  */
#ifndef DRIVERS_KEYSCAN_YC_QUEUE_H_
#define DRIVERS_KEYSCAN_YC_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

/**
  *@brief Queue header.
  */
typedef struct
{
	void *pHead;		/*!< Header pointer*/
	void *pWrite;		/*!< Writing queue pointer*/
	void *pRead;		/*!< Reading queue pointer*/
	uint32_t unit_Len;	/*!< size of element */
	uint32_t array_Len;	/*!< the length of array */
	uint8_t current_queue_len;/*!< Entire queue length */
}QUEUE_HEADER;

/**
  *@brief Queue initialization.
  *@param pQheader point queue header structure.@ref QUEUE_HEADER
  *@param pMem empty pointer.
  *@param unitLen unit length.
  *@param len array length.
  *@retval true  initializing success.
  *@retval false  initializing failure.
  */
bool queue_init(QUEUE_HEADER *pQheader, void *pMem, uint32_t unitLen, uint32_t len);

/**
  *@brief Insert queue.
  *@param pQheader inserted queue pointer.@ref QUEUE_HEADER
  *@param pEle inserted element pointer.
  *@retval True operating success.
  *@retval false operating failure.
  */
bool Insert_Queue(QUEUE_HEADER *pQheader,void *pEle);

/**
  *@brief Delete queue.
  *@param pQheader will be delete element.@ref QUEUE_HEADER
  *@return Empty pointer.
  */
void * Delete_Queue(QUEUE_HEADER *pQheader);

/**
  *@brief Check queue is empty.
  *@param pQheader The target queue.@ref QUEUE_HEADER
  *@retval True the queue is empty.
  *@retval false the queue is not empty. 
  */
bool queue_is_empty(QUEUE_HEADER *pQheader);

/**
  *@brief Getting queue current length.
  *@param pQheader The target queue.@ref QUEUE_HEADER
  *@return The queue length.
  */
uint8_t queue_ele_count(QUEUE_HEADER *pQheader);

/**
  *@brief Check queue is full.
  *@param pQheader The target queue.@ref QUEUE_HEADER
  *@retval True queue is full.
  *@retval false queue is not full. 
  */
bool queue_is_full(QUEUE_HEADER *pQheader);

/**
  *@brief Clear queue.
  *@param pQheader The target queue.@ref QUEUE_HEADER 
  *@retval True represent clear success.
  *@retval false represent clear failure.
  */
bool queue_clear(QUEUE_HEADER *pQheader);

void * Header_Queue(QUEUE_HEADER *pQheader);
bool preinsert_Queue(QUEUE_HEADER *pQheader,void **pEle);

#endif /* DRIVERS_KEYSCAN_YC_QUEUE_H_ */
