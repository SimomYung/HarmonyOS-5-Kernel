/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: APIs for hibernation snapshot page
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 29 16:17:34 2024
 */

#ifndef INTAPI_CTRLMEM_HBSNAPSHOTPG_H
#define INTAPI_CTRLMEM_HBSNAPSHOTPG_H

#include <hmasm/memory.h>

int ctrlmem_pool_page_init(__paddr_t pa);
int ctrlmem_pool_page_finalize(__paddr_t pa);

#endif
