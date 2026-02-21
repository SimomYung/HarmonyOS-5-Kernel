/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: kip protection header for kernel
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 17 16:48:30 2024
 */

#ifndef HMKERNEL_DRIVERS_HKIP_SEC_REGION_H
#define HMKERNEL_DRIVERS_HKIP_SEC_REGION_H

#ifdef CONFIG_HKIP_SEC_REGION

void hkip_sec_region_set_range(void);

void hkip_sec_region_get_range(paddr_t *start, paddr_t *end);

#else

static inline void hkip_sec_region_set_range(void) {}

static inline void hkip_sec_region_get_range(paddr_t *start, paddr_t *end)
{
	if (start != NULL) {
		*start = 0;
	}

	if (end != NULL) {
		*end = 0;
	}
}

#endif

#endif
