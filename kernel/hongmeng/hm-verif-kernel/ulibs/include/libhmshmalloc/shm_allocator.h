/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define and prototypes for share memory allocator
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 19 18:19:32 2019
 */
#ifndef ULIBS_LIBSHM_ALLOCATOR_H
#define ULIBS_LIBSHM_ALLOCATOR_H

#include <stdint.h>
#include <libmem/types.h>

/* use one page to store head info */
#define SHARE_MEM_HEAD_LEN PAGE_SIZE

struct share_mem_ctrl_info;

struct shm_version {
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
	uint16_t build;
};

void shmalloc_version_get(struct shm_version *version);
int shmalloc_version_check(const struct shm_version *expected);

/*
 * shmalloc_init - initialize the share memory control information
 * @buffer: the share memory start virtual address
 * @total_len: the toatl length of share memory
 * @unit_size: size of alloc or free each time
 *
 * RETURNS:
 * buffer virtual address on success, NULL on failure
 *
 * This function initialize the share memory control information. Usually we first
 * map the share memory with cell or other methods, than call sharemem_allocator_init
 * to manage this memory. The total_len should be aligned to page size.
 * Now we use one page to store control information and the left
 * for units. Units num is equal to (total_len - page_size) / unit_size.
 * The max units num should no more than 1<<12.
 */
struct share_mem_ctrl_info *
shmalloc_init(void *buffer, size_t total_len, size_t unit_size);

/* shmalloc_unit_alloc - alloc a unit from the share memory pool
 * @pmem_ctrl_info: the share memory start virtual address
 * @va: the unit virtual address of alloced share memory
 *
 * RETURNS:
 * 0 on success, errcode on failure.
 */
int
shmalloc_unit_alloc(struct share_mem_ctrl_info *pmem_ctrl_info,
		    uintptr_t *va);

/* shmalloc_unit_free - free a unit to the share memory pool
 * @pmem_ctrl_info: the share memory start virtual address
 * @offset: the unit offset to share memory start address
 *
 * RETURNS:
 * 0 on success, errcode on failure.
 *
 * The offset should be in the region which starts after share memory control information
 * and ends with the share mem size.
 */
int
shmalloc_unit_free(struct share_mem_ctrl_info *pmem_ctrl_info,
		   unsigned int offset);

/* shmalloc_get_unit_free_num - get free unit number in the share memory pool
 * @pmem_ctrl_info: the share memory start virtual address
 *
 * RETURNS:
 * free unit number.
 */
unsigned int
shmalloc_get_unit_free_num(struct share_mem_ctrl_info *pmem_ctrl_info);

#endif
