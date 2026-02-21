/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: String
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 01:36:17 2018
 */
#ifndef _LIBFDT_STRING_H
#define _LIBFDT_STRING_H

#include <hmkernel/types.h>

extern void *memmove(void *to, const void *from, size_t len);
extern void *memcpy(void *dst, const void *src, size_t len);
extern void *memchr(const void *s, int c, size_t n);
extern char *strchr(const char *s, int c);
extern char *strrchr(const char *s, int c);
extern size_t strnlen(const char *s, size_t maxlen);
extern void *memset(void *dst, int c, size_t len);

#endif
