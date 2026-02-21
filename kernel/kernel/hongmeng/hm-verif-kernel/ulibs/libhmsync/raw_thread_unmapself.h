/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Interfaces of raw_thread_unmapself
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 12 16:34:42 2022
 */

#ifndef RAW_THREAD_UNMAPSELF_H
#define RAW_THREAD_UNMAPSELF_H

#include <stddef.h>

void raw_thread_unmapself(void *base, size_t size);

#endif