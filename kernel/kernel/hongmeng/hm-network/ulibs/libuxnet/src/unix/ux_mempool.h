/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Mempool header files for unix socket
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 28 02:39:31 PM 2023
 */

#ifndef UX_MEMPOOL_H
#define UX_MEMPOOL_H

#include <sys/types.h>

#include "osal.h"

#ifdef CONFIG_UX_MEMPOOL
void ux_mempool_init(void);

void *ux_mempool_alloc(size_t size);
void ux_mempool_free(void *ptr, size_t size);
#else
static inline void ux_mempool_init(void)
{}

static inline void *ux_mempool_alloc(size_t size)
{
	return osal_calloc(1, size);
}

static inline void ux_mempool_free(void *ptr, size_t size)
{
	UNUSED(size);
	osal_free(ptr);
}

#endif

#endif
