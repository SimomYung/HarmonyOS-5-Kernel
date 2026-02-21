/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Stdlib
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 01:36:17 2018
 */
#ifndef KLIBS_STDLIB_H
#define KLIBS_STDLIB_H

#include <stddef.h>

extern long int
strtol(const char *s, char **t, int b);

extern unsigned long int
strtoul(const char *s, char **t, int b);

typedef int (*cmp_func_t)(const void *a, const void *b);
void *bsearch(const void *key, const void *base, size_t num, size_t size, cmp_func_t cmp);

#endif
