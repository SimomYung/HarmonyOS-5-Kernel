/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 * Description: Definition of libhmutils
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 31 19:46:33 2021
 */

#ifndef ULIBS_LIBHMUTILS__H
#define ULIBS_LIBHMUTILS__H

#include <stddef.h>
#include <hmkernel/types.h>
#include <lib/utils.h>

static inline char *trans_to_hex(char *_str, char in_val, const char *hex_map)
{
	char *str = _str;
	/* 4 is number of shift, which is used to get high half of byte */
	*str++ = hex_map[((unsigned char)(in_val) & 0xf0) >> 4];
	*str++ = hex_map[((unsigned char)(in_val) & 0x0f)];
	return str;
}

static inline char *trans_to_hex_upper(char *_str, char in_val)
{
	const char hex_upper_array[] = "0123456789ABCDEF";
	return trans_to_hex(_str, in_val, hex_upper_array);
}

static inline char *trans_to_hex_lower(char *_str, char in_val)
{
	const char hex_lower_array[] = "0123456789abcdef";
	return trans_to_hex(_str, in_val, hex_lower_array);
}

/*
 * Notice: The caller must ensure that the parameters are correct
 * @str_size should be greater than or equal to (@array_size * 2 + 1)
 */
static inline void hex_to_str(char *str, size_t str_size, const uint8_t *array, size_t array_size)
{
	for (unsigned int i = 0; i < array_size && ((i << 1) + 1) < str_size; i++) {
		(void)trans_to_hex_lower(str + (i << 1), array[i]);
	}
	str[str_size - 1] = '\0';
}

#endif /* ULIBS_LIBHMUTILS__H */
