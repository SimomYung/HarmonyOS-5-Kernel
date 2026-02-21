/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Internal interfaces of reserved memory
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 08 17:10:53 2025
 */

#ifndef ULIBS_INCLUDE_LIBMEM_RESERV_MEM_H
#define ULIBS_INCLUDE_LIBMEM_RESERV_MEM_H

#include <hmkernel/types.h>

int hm_reserved_memory_read_config(unsigned int type, __u64 *paddr, __u32 *size);

#endif
