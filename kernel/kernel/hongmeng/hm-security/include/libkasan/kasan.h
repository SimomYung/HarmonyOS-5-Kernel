/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Header file for kasan initialization
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
#ifndef __KASAN_H_
#define __KASAN_H_
#include <bits/alltypes.h>
#include <stdint.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>

#ifdef ENABLE_KASAN
int kasan_init(void);
int kasan_map_shadow_range(uintptr_t start, uintptr_t end);
void kasan_unmap_shadow_range(uintptr_t start, uintptr_t end);

static inline void kasan_map_for_shadow_range(int pre_ret, uintptr_t start, uintptr_t end)
{
	if (pre_ret == E_HM_OK) {
		(void)kasan_map_shadow_range(start, end);
	}
}

static inline void kasan_unmap_for_shadow_range(int pre_ret, uintptr_t start, uintptr_t end)
{
	if (pre_ret == E_HM_OK) {
		kasan_unmap_shadow_range(start, end);
	}
}
#else
static inline void kasan_map_for_shadow_range(int pre_ret, uintptr_t start, uintptr_t end)
{
	UNUSED(pre_ret, start, end);
}

static inline void kasan_unmap_for_shadow_range(int pre_ret, uintptr_t start, uintptr_t end)
{
	UNUSED(pre_ret, start, end);
}
#endif /* ENABLE_KASAN */
#endif
