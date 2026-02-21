/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of fs malloc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 05 12:28:08 2020
 */

#ifndef LIBHMSRV_UTILS_MALLOC_H
#define LIBHMSRV_UTILS_MALLOC_H

#include <stddef.h>

void *fs_zmalloc(size_t size);
void fs_free(void *buf);

#endif
