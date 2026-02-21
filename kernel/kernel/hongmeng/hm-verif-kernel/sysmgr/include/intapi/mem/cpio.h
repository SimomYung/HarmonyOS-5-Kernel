/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of cpio
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 17 16:50:23 2020
 */

#ifndef SYSMGR_MEM_CPIO_H
#define SYSMGR_MEM_CPIO_H

#include <stdint.h>
#include <lib/cpio.h>

int cpio_reclaim(uintptr_t _start, uintptr_t _end);
void cpio_init(uintptr_t standalone_uapps_start, uintptr_t standalone_uapps_end);
struct cpio_header *cpio_get(void);
void *cpio_end_get(void);

#endif
