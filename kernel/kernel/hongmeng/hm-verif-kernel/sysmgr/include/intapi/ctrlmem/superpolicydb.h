/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Sysmgr handling superpolicydb which map policydb using supervs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 23 17:21:53 2025
 */

#ifndef INTAPI_CTRLMEM_SUPERPOLICYDB_H
#define INTAPI_CTRLMEM_SUPERPOLICYDB_H

#include <hmasm/memory.h>
#include <hmkernel/compiler.h>

#ifdef CONFIG_POLICYDB_USE_SUPERVS

void ctrlmem_superpolicydb_page_collect(__paddr_t pa);

void ctrlmem_superpolicydb_init(void);

#else

static inline void ctrlmem_superpolicydb_page_collect(__maybe_unused __paddr_t pa)
{ }

static inline void ctrlmem_superpolicydb_init(void)
{ }

#endif

#endif
