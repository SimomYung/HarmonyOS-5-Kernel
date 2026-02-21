/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Printf
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifndef KLIBS_PRINTF_H
#define KLIBS_PRINTF_H

#include <stdarg.h>
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>

extern int __printf(3, 0)
raw_vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

extern int __printf(3, 4)
raw_snprintf(char *buf, size_t size, const char *fmt, ...);

extern int __printf(3, 0)
vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

extern int __printf(3, 4)
snprintf(char *buf, size_t size, const char *fmt, ...);

#ifdef __HOST_LLT__
extern int __printf(4, 0)
vsnprintf_s(char *buf, size_t size_max, size_t size, const char *fmt, va_list args);

extern int __printf(4, 5)
snprintf_s(char *buf, size_t size_max, size_t size, const char *fmt, ...);
#endif

#endif
