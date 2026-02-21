/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Memmgr interfaces of Feedback Directed Optimize between sysmgr and fs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 11 08:46:13 2023
 */
#ifndef AAPI_HMSYSMGR_FDO_H
#define AAPI_HMSYSMGR_FDO_H

#include <hmasm/types.h>
#include <hmasm/memory.h>

struct vnode_map_info_s {
	uintptr_t fs_vnode;
	uint64_t file_size;

	uintptr_t vstart;
	uintptr_t vend;
};

int fdofs_vnodes_fetch_pages(uintptr_t fs_vnode, uintptr_t *rpages,
			     unsigned int pg_start, unsigned int pg_cnt);
#endif
