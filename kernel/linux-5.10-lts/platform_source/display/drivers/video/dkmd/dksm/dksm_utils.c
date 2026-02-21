/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "dksm_utils.h"
#include "dkmd_log.h"
int get_str_suffix_num(const char* source_str, const char* prefix)
{
	const char* num_str;
	size_t prefix_len = strlen(prefix);
	int str_num_index = 0;
	int res_index = 0;

	if (strlen(source_str) < prefix_len) {
		dpu_pr_err("invalid source_str %s!", source_str);
		return -1;
	}

	if (strlen(source_str) == prefix_len)
		return 0;

	num_str = source_str + prefix_len;
	while (num_str[str_num_index] != '\0') {
		if (num_str[str_num_index] < '0' || num_str[str_num_index] > '9') {
			dpu_pr_err("invalid source_str %s!", source_str);
			return -1;
		}
		res_index = res_index * 10 + (int)(num_str[str_num_index] - '0');
		str_num_index++;
	}
	return res_index;
}
