/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: fdtable cap ops
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 26 10:02:06 2023
 */

#ifndef ULIBS_LIBHM_FDTABLE_H
#define ULIBS_LIBHM_FDTABLE_H

#include <stdint.h>
#include <hmkernel/capability.h>

#ifdef __cplusplus
extern "C" {
#endif

int hm_fdtable_require_grant(unsigned int cnode_idx, rref_t *result);

#ifdef __cplusplus
}
#endif

#endif
