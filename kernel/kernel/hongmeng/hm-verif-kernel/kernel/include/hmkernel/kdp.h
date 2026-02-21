/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: kdp header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 12 19:24:14 UTC 2024
 */

#ifndef HMKERNEL_KDP_H
#define HMKERNEL_KDP_H

#include <hmasm/types.h>

#ifndef CONFIG_KDP_LOWMEMSZ_NOADJUST
u64 lowmem_size_adjust(void);
#else
static inline u64 lowmem_size_adjust(void)
{
	return 0ULL;
}
#endif

#endif
