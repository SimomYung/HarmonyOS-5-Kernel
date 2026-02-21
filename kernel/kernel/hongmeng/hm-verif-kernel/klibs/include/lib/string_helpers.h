/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Header file for string formatting
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 19 08:32:35 2025
 */

#ifndef KLIBS_STRING_HELPERS_H
#define KLIBS_STRING_HELPERS_H

#include <stddef.h>

#define A_OFFSET_ZERO		10
#define BITS_OF_HEX		4
#define MAX_LEN_STR		(~(size_t)0)

static int hex_to_binary(unsigned char c)
{
	int ret;
	if (c >= '0' && c <= '9') { /* if c <= 9 && c >= 0, convert to int directly */
		ret = (int)((unsigned char)c - (unsigned char)'0');
	} else if (c >= 'a' && c <= 'f') {
		ret = (int)((unsigned char)c - (unsigned char)'a') + A_OFFSET_ZERO;
	} else if (c >= 'A' && c <= 'Z') {
		ret = (int)((unsigned char)c - (unsigned char)'A') + A_OFFSET_ZERO;
	} else {
		ret = -1;
	}
	return ret;
}

/* Traverse the hexadecimal char and convert it into binary */
static __bool convert_hex_char(char **str, char **dst)
{
	char *n = *str;
	unsigned char ret;
	int num_h, num_l;

	if (*n++ != 'x') {
		return __false;
	}
	num_h = hex_to_binary(*n++);
	if (num_h < 0) {
		return __false;
	}
	num_l = hex_to_binary(*n);
	if (num_l >= 0) {
		n++;
		ret = (num_h << BITS_OF_HEX) | num_l;
	} else {
		ret = num_h;
	}
	**dst = ret;
	*dst += 1;
	*str = n;
	return __true;
}

/* Traverse the hexadecimal string and convert it into binary */
int convert_hex_string(char *src, char *dst, size_t len)
{
	char *cur = dst;

	if (!len)
		len = MAX_LEN_STR;

	while (*src && --len) {
		if (src[0] == '\\' && src[1] != '\0' && len > 1) {
			src++;
			len--;
			if (convert_hex_char(&src, &cur)) {
				continue;
			}
			*cur++ = '\\';
		}
		*cur++ = *src++;
	}
	*cur = '\0';

	return cur - dst;
}
#endif
