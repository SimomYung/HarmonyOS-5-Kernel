/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network physical memory
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 20:06:36 2019
 */

#ifndef __HMNET_PHY_MEM__
#define __HMNET_PHY_MEM__
#include <libmem/utils.h>
#include <libhmshmalloc/shm_allocator.h>

#include "ux_netdev.h"

extern void *phy_mem_alloc(hmshm_mgr_t *shmmgr, unsigned int *offset);
extern void phy_mem_free(hmshm_mgr_t *shmmgr, const void *addr, unsigned int cell_offset);
extern void *get_buf_addr_by_offset(hmshm_mgr_t *shmmgr, int cell_pool_offset);
extern void dump_phy_mem_count(void);

#endif
