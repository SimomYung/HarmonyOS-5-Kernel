/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Swap intapi file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 11 14:52:53 2023
 */
#ifndef __SYSMGR_INTAPI_MEM_SWAP_H__
#define __SYSMGR_INTAPI_MEM_SWAP_H__

#include <hongmeng/compiler.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>
#include <sys/types.h>

#ifdef CONFIG_SWAPFILE
/* swap interface for hibernate */

uint64_t swapentry_encode(uint64_t index, uint64_t offset);
uint64_t swentry_index_of(uint64_t entry);
uint64_t swentry_offset_of(uint64_t entry);

/* Return value:
 *	<0: errno
 *  >=0: allocated block's num
 */
extern int swapfile_get_unused_entries(unsigned int index, unsigned int nr, uint64_t *entries);

/*
 * Try to get consecutive and physical aligned blocks.
 * If there are not consecutive blocks, alloc as swapfile_get_index_from_devid
 */
extern int swapfile_get_unused_aligned_entries(unsigned index, unsigned int nr, unsigned int aligned_mask,
	uint64_t *entries);

extern bool swapfile_enough_space(unsigned int index, uint64_t nr_pages);
extern int swapfile_offset_to_block(unsigned int index, uint64_t offset, uint64_t *block);
extern void swapfile_free_entries(unsigned int nr, uint64_t *entries);
extern int swapfile_get_index_from_devid(dev_t device, uint64_t block);
extern int swapfile_get_first_dev_index(dev_t *device, uint64_t *block);

#else

static inline uint64_t swapentry_encode(uint64_t index, uint64_t offset)
{
	UNUSED(index, offset);

	return 0ULL;
}

static inline uint64_t swentry_index_of(uint64_t entry)
{
	UNUSED(entry);

	return 0ULL;
}

static inline uint64_t swentry_offset_of(uint64_t entry)
{
	UNUSED(entry);

	return 0ULL;
}

static inline int swapfile_get_unused_entries(unsigned int index, unsigned int nr, uint64_t *entries)
{
	UNUSED(index, nr, entries);

	return E_HM_NOSYS;
}

static inline int swapfile_get_unused_aligned_entries(unsigned index, unsigned int nr, unsigned int aligned_mask,
	uint64_t *entries)
{
	UNUSED(index, nr, aligned_mask, entries);

	return E_HM_NOSYS;
}

static inline bool swapfile_enough_space(unsigned int index, uint64_t nr_pages)
{
	UNUSED(index, nr_pages);

	return false;
}

static inline int swapfile_offset_to_block(unsigned int index, uint64_t offset, uint64_t *block)
{
	UNUSED(index, offset, block);

	return E_HM_NOSYS;
}

static inline void swapfile_free_entries(unsigned int nr, uint64_t *entries)
{
	UNUSED(nr, entries);
}

static inline int swapfile_get_index_from_devid(dev_t device, uint64_t block)
{
	UNUSED(device, block);

	return E_HM_NOSYS;
}

static inline int swapfile_get_first_dev_index(dev_t *device, uint64_t *block)
{
	UNUSED(device, block);

	return E_HM_NOSYS;
}

#endif /* CONFIG_SWAPFILE */

#endif /* __SYSMGR_INTAPI_MEM_SWAP_H__ */
