/*
 * Copyright 2016, yichip Semiconductor(shenzhen office)
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
 /** @file
 *
 * mempool support for application
 */

 #ifndef _MP_H_
 #define _MP_H_
#include <stdio.h>
#include "yc11xx.h"
#include "ycdef.h"


/**
 * This function will init a mempool form a memory object and allocate the memory pool from
 * heap.
 *
 * @param mp pool object to init
 * @param start memory object to init
 * @param block_count the count of blocks in memory pool
 * @param block_size the size for each block
 *
 * @return the created mempool object
 */
error_t MP_Init(mp_t mp,
                    void              *start,
                    uint8_t          size,
                    uint16_t          block_size);

/**
 * This function will init a mempool object and allocate the memory pool from
 * heap.
 *
 * @param mp pool object to init
 * @param block_count the count of blocks in memory pool
 * @param block_size the size for each block
 *
 * @return the created mempool object
 */
error_t MP_Create(mp_t mp,
	uint8_t block_ount, uint16_t block_size);

/**
 * This function will allocate a block from memory pool
 *
 * @param mp the memory pool object
 *
 * @return the allocated memory block or RT_NULL on allocated failed
 */
void *MP_Alloc(mp_t mp);

/**
 * This function will release a memory block
 *
 * @param block the address of memory block to be released
 */
void MP_Free(void* block);


#endif /*_MP_H_*/


