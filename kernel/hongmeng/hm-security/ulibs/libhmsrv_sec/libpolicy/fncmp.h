/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: String pattern matching
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:14 2019
 */
#ifndef __FNCMP_H_
#define __FNCMP_H_
#include <stdbool.h>
#include <stddef.h>

bool fncmp(const char *pattern, size_t psz, const char *str, size_t ssz);

#endif
