/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: einj module
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 23 15:03:33 CST 2023
 */

#ifndef UAPI_HMKERNEL_EINJ_MODULE_H
#define UAPI_HMKERNEL_EINJ_MODULE_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/einj/einj.h>

enum __einj_cmd {
	__EINJ_CMD_FUNC_CALL = 0,
	__EINJ_CMD_READ_MEM,
	__EINJ_CMD_WRITE_MEM,
	__EINJ_CMD_LOOKUP_SYMBOL,
	__EINJ_CMD_PROBE,
	__EINJ_CMD_MAX,
};

struct __einj_module_funcall_input_s {
	struct __einj_symbol_info_s symbol_info;
	struct __einj_func_arg_s func_arg;
};

struct __einj_mem_arg_s {
	struct __einj_symbol_info_s symbol_info;
	size_t data_len;
	void *data;
};

#endif
