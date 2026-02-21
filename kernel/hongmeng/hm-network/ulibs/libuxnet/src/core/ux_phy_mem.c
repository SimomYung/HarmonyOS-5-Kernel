/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network physical memory
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 20:06:36 2019
 */
#include <stdio.h>
#include <libmem/utils.h>
#include <libhmshmalloc/shm_allocator.h>
#include <libhmucap/ucap.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>

#include "ux_netlog.h"
#include "ux_phy_mem.h"

static unsigned long long total_phy_mem_alloc_success = 0;
static unsigned long long total_phy_mem_alloc_fail = 0;
static unsigned long long total_phy_mem_free = 0;

void *phy_mem_alloc(hmshm_mgr_t *shmmgr, unsigned int *offset)
{
	uintptr_t va;
	int err;

	err = shmalloc_unit_alloc(shmmgr->netif_mpool, &va);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "shmalloc unit alloc failed, err:%d\n", err);
		total_phy_mem_alloc_fail++;
		return NULL;
	}

	total_phy_mem_alloc_success++;

	if (offset != NULL) {
		*offset = (unsigned int)(va - (uintptr_t)shmmgr->netif_mpool);
	}

	return (void *)va;
}

void phy_mem_free(hmshm_mgr_t *shmmgr, const void *addr, unsigned int cell_offset)
{
	int ret;

	(void)addr;

	ret = shmalloc_unit_free(shmmgr->netif_mpool, cell_offset);

	total_phy_mem_free++;
	if (ret != 0) {
		net_error(HMNET_CORE, "ret:%d, cell_offset:%u.\n", ret, cell_offset);
	}
}

void *get_buf_addr_by_offset(hmshm_mgr_t *shmmgr, int cell_pool_offset)
{
	unsigned int unit_size = shmmgr->unit_size;
	unsigned int pool_size = shmmgr->pool_size;

	if (((unsigned int)cell_pool_offset > pool_size) ||
	    (unit_size > (pool_size - (unsigned int)cell_pool_offset))) {
		return NULL;
	}

	return (char *)shmmgr->netif_mpool + cell_pool_offset;
}

void dump_phy_mem_count(void)
{
	net_info(HMNET_CORE, "phy_mem_count: alloc_success:%llu, alloc_fail:%llu"
		"free:%llu\n", total_phy_mem_alloc_success,
		total_phy_mem_alloc_fail, total_phy_mem_free);
}
