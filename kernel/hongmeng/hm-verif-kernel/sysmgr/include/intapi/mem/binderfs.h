/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: binderfs shm populate header file
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 06 11:14:17 2024
 */

#ifndef FS_BINDERFS_H
#define FS_BINDERFS_H

#include <stdint.h>

int binderfs_shm_populate(__u32 cnode_idx, uintptr_t user_vstart, uintptr_t binder_vstart, size_t size);

#endif /* FS_BINDERFS_H */
