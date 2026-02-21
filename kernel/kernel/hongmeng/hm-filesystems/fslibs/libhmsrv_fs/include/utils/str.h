/*
* Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of str
* Author: Huawei OS Kernel Lab
* Create: Mon Jul 22 14:26:43 2019
*/

#ifndef LIBVFS_UTILS_STR_H
#define LIBVFS_UTILS_STR_H

#include <limits.h>
#include <vfs.h>
#include <fslib_common.h>
#include <stddef.h>
#include <stdint.h>

#define STR_ERR '?'

#define STR_SIZE_MAX  ((size_t) -1)

#define UTF16_MAX	0x0010FFFF
#define CONT_SHIFT1	6
#define CONT_SHIFT2	12
#define CONT_SHIFT3	18

#define UTF8_CONT_MASK		0xC0
#define UTF8_CONT_VALUE		0x80
#define UTF8_1B_MASK		0x80

#define UTF8_2B_MASK		0xE0
#define UTF8_2B_VALUE		0xC0
#define UTF8_2B_MIN		0x80
#define UTF8_SHIFT1_MASK	0x1F

#define UTF8_3B_MASK		0xF0
#define UTF8_3B_VALUE		0xE0
#define UTF8_3B_MIN		0x800
#define UTF8_3B_MAX		0x10000
#define UTF8_SHIFT2_MASK	0x0F

#define UTF8_4B_MASK		0xF8
#define UTF8_4B_VALUE		0xF0
#define UTF8_SHIFT3_MASK	0x0F
#define UTF8_4B_MIN		0x10000
#define UTF8_1W_MASK		0xFC00
#define UTF8_1W_PRE		0xD800
#define UTF8_2W_PRE		0xDC00
#define UTF8_WCH_MASK		0x3FF
#define UTF8_CH_MASK		0x3F

#define UTF8_NEED_1B 1
#define UTF8_NEED_2B 2
#define UTF8_NEED_3B 3

#define UTF_SHIFT	10U

#define STR_SIZE(len)  ((len) << 2)

extern wchar_t to_wchr(const char *, size_t *, size_t);

extern int utf8_to_utf16(const char *src, size_t slen, uint16_t *dest, size_t dlen);
extern size_t utf16_len(const uint16_t *, size_t);
extern int utf16_to_utf8(const uint16_t *src, char *dst, size_t len);


extern int fs_strtoi(const char *str, int *val, int bs);
extern int fs_strtol(const char *str, long *val, int bs);
extern int fs_strtoui(const char *str, unsigned int *val, int bs);
extern int fs_strtoul(const char *str, unsigned long *val, int bs);
extern int fs_strtoull(const char *str, unsigned long long *val, int bs);

#endif
