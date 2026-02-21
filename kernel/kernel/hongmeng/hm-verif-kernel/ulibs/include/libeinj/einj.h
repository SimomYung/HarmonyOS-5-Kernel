/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file of libeinj
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 23 15:03:33 CST 2023
 */

#ifndef ULIBS_INCLUDE_LIBEINJ_EINJ_H
#define ULIBS_INCLUDE_LIBEINJ_EINJ_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/einj/einj.h>

#define EINJ_MAX_FUNCALL_DATA_LEN 4096U

enum einj_data_type {
	EINJ_TYPE_S8 = 0,
	EINJ_TYPE_U8,
	EINJ_TYPE_S16,
	EINJ_TYPE_U16,
	EINJ_TYPE_S32,
	EINJ_TYPE_U32,
	EINJ_TYPE_S64,
	EINJ_TYPE_U64,
	EINJ_TYPE_STR,
	EINJ_TYPE_PTR,
	EINJ_TYPE_PADDING,
};

struct einj_input_arg_s {
	/* argument number of the called function */
	unsigned int arg_count;
	unsigned int arg_data_len;
	unsigned int total_len;
	unsigned int padding;
	char arg_data[];
};

struct einj_funcall_input_s {
	struct __einj_symbol_info_s func_symbol;
	struct einj_input_arg_s func_arg;
};

#endif
