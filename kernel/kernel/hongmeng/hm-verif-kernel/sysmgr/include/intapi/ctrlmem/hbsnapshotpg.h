/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: APIs for hibernation snapshot page
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 24 11:31:23 2024
 */

#ifndef INTAPI_CTRLMEM_HBSNAPSHOTPG_H
#define INTAPI_CTRLMEM_HBSNAPSHOTPG_H

#include <hmasm/memory.h>

void hbsnapshotpg_register(__paddr_t pa);
void hbsnapshotpg_unregister(__paddr_t pa);

#endif
