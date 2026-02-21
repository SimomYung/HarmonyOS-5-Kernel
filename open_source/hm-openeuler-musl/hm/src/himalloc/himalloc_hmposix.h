/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The header for hmposix
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 16:50:16 2022
 */
#ifndef HIMALLOC_HMPOSIX_H
#define HIMALLOC_HMPOSIX_H

#include <stddef.h>

int posix_memalign(void **res, size_t align, size_t len);
void *aligned_alloc(size_t align, size_t len);

#endif
