/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: einj module
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 23 15:03:33 CST 2023
 */
#ifndef UAPI_HMKERNEL_EINJ_H
#define UAPI_HMKERNEL_EINJ_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmasm/einj.h>

#define __EINJ_MAX_SYMBOL_NAME_LEN (64U - sizeof(unsigned int))
#define __EINJ_MAX_MEM_READ_LEN (16 * 1024)
#define __EINJ_MAX_MEM_WRITE_LEN (16 * 1024)

struct __einj_func_ret_s {
	unsigned long ret;
};

struct __einj_func_arg_s {
	unsigned long args[__EINJ_FUNCALL_MAX_ARG_COUNT];
};

enum __einj_symbol_type {
	__EINJ_SYMBOL_TYPE_NAME = 0,
	__EINJ_SYMBOL_TYPE_ADDR,
	__EINJ_SYMBOL_TYPE_MAX,
	__EINJ_SYMBOL_TYPE_INVALID = __EINJ_SYMBOL_TYPE_MAX,
};

struct __einj_symbol_name_s {
	char name[__EINJ_MAX_SYMBOL_NAME_LEN];
};

struct __einj_symbol_info_s {
	unsigned int type; /* type of einj_symbol_type */
	union {
		__uptr_t addr;
		struct __einj_symbol_name_s name;
	};
};

#endif
