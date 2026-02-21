/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Collect pages for hibernation process
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 10 09:33:57 2024
 */

#ifndef HMKERNEL_PGSTR_HIBERNATE_H
#define HMKERNEL_PGSTR_HIBERNATE_H

#include <hmkernel/drivers/hibernate.h>

#ifdef CONFIG_KERNEL_PM_HIBERNATION

extern int pgstrtbl_hibernate_collect_kpages(struct __hibernate_memory_bitmap *kernel_bm);

#else

static inline int pgstrtbl_hibernate_collect_kpages(struct __hibernate_memory_bitmap *kernel_bm)
{
	UNUSED(kernel_bm);
	return E_HM_NOSYS;
}

#endif

#endif
