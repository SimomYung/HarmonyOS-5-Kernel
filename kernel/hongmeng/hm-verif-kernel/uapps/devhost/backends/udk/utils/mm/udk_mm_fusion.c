/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Memory manager interfaces of UDK only enabled under fusion mode
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:35:13 2019
 */
#include <limits.h>

#include <hongmeng/types.h>
#include <libdevhost/mm.h>
#include <libmem/types.h>

#include <intapi/mem/map.h>
#include <intapi/mem/sysmgr_page.h>

#include <udk/errno.h>
#include <udk/log.h>
#include <udk/mm_fusion.h>
#include <udk/bitops.h>

uint64_t udk_alloc_pages(uint32_t flags, uint32_t order)
{
	return sysmgr_alloc_pages(flags, order);
}

void udk_free_pages(uint64_t pa)
{
	sysmgr_free_pages(pa);
}
