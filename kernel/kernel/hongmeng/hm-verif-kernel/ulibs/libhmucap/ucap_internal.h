/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Ucap internal header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 14 10:54:16 2023
 */

#ifndef __LIBHMUCAP_UCAP_INTERNAL__
#define __LIBHMUCAP_UCAP_INTERNAL__

#include <hongmeng/types.h>
#include <hmkernel/capability.h>

static inline size_t min(size_t a, size_t b)
{
	return a > b ? b : a;
}

int kobj_check_captype(cref_t cnode, cref_t cref, enum hmobj captype);

#endif
