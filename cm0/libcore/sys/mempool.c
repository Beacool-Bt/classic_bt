#include "ycpool.h"
#include <stdlib.h>

error_t MP_Init(mp_t mp,
                    void              *start,
                    uint8_t          size,
                    uint16_t          block_size)
{
	uint8_t *block_ptr = NULL;
	uint8_t offset = 0;
	
	_ASSERT(mp!=NULL);
	/* here init obj*/
    
	mp->start_address = start;

	size = _ALIGN_UP(size, _ALIGN_SIZE);
	block_size = _ALIGN_UP(block_size, _ALIGN_SIZE);
	/* align to align size byte */
	mp->size = size;
	mp->block_size = block_size;
	
	/* align to align size byte */
	mp->block_total_count = mp->size / (mp->block_size + sizeof(uint8_t *)); 
	mp->block_free_count  = mp->block_total_count;
	
	/* initialize free block list */
	block_ptr = (uint8_t *)mp->start_address;
	for (offset = 0; offset < mp->block_total_count; offset ++) {
		*(uint8_t **)(block_ptr + offset * (block_size + sizeof(uint8_t *))) =
		(uint8_t *)(block_ptr + (offset + 1) * (block_size + sizeof(uint8_t *)));
	}
	
	*(uint8_t **)(block_ptr + (offset - 1) * (block_size + sizeof(uint8_t *)))  = NULL;
	mp->block_list = block_ptr;
	return 0;
}

error_t MP_Create(mp_t mp,
	uint8_t block_count, uint16_t block_size)
{
	uint8_t *block_ptr = NULL;
	uint8_t offset = 0;
	
	_ASSERT(mp!=NULL);
	/* here init obj*/

	block_size = _ALIGN_UP(block_size, _ALIGN_SIZE);
	mp->size = block_size;
	mp->block_size = block_count * (block_size + sizeof(uint8_t *));
	
	mp->start_address = malloc(mp->block_size);
	_ASSERT(mp->start_address!=NULL);

	/* align to align size byte */
	mp->block_total_count = block_count;
	mp->block_free_count  = block_count;

	/* initialize free block list */
	block_ptr = (uint8_t *)mp->start_address;
	for (offset = 0; offset < mp->block_total_count; offset ++) {
		*(uint8_t **)(block_ptr + offset * (block_size + sizeof(uint8_t *))) =
		(uint8_t *)(block_ptr + (offset + 1) * (block_size + sizeof(uint8_t *)));
	}
	
	*(uint8_t **)(block_ptr + (offset - 1) * (block_size + sizeof(uint8_t *)))  = NULL;
	mp->block_list = block_ptr;

	return 0;
}

void *MP_Alloc(mp_t mp)
{
	uint8_t *block_ptr;
    	register uint32_t level;
	if (mp->block_free_count == 0){
		return NULL;
	}
	/* memory block is available. decrease the free block counter */
	mp->block_free_count--;

	/* get block from block list */
	block_ptr = mp->block_list;
	_ASSERT(block_ptr != NULL);

	/* Setup the next free node. */
	mp->block_list = *(uint8_t **)block_ptr;

	/* point to memory pool */
	*(uint8_t **)block_ptr = (uint8_t *)mp;

	return (uint8_t *)(block_ptr + sizeof(uint8_t *));
}

void MP_Free(void *block)
{
	uint8_t **block_ptr;
	struct mempool *mp;

	/* get the control block of pool which the block belongs to */
	block_ptr = (uint8_t **)((uint8_t *)block - sizeof(uint8_t *));
	mp        = (struct mempool *)*block_ptr;

	/* increase the free block count */
	mp->block_free_count ++;

	/* link the block into the block list */
	*block_ptr = mp->block_list;
	mp->block_list = (uint8_t *)block_ptr;
}

