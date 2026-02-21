/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Kasan sysmgr init functions header
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 15 20:05:35 2020
 */
#ifndef INTAPI_KASAN_H
#define INTAPI_KASAN_H

#include <hongmeng/types.h>

extern uintptr_t __sysmgr_linear_map_start;
extern uintptr_t __sysmgr_linear_map_end;

extern uintptr_t __sysmgr_linear_map_start_h;
extern uintptr_t __sysmgr_linear_map_end_h;

#ifdef CONFIG_SYSMGR_KASAN
int kasan_post_init(void);
#else
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
static inline int kasan_post_init(void)
{
	return E_HM_OK;
}
#endif

#endif
