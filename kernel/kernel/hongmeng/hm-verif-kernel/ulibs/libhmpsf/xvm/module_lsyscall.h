/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interfaces for xvm lsyscall module
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 11:39:13 2024
 */

#ifndef __ULIBS_LIBHMPSF_XVM_MODULE_LSYSCALL_H
#define __ULIBS_LIBHMPSF_XVM_MODULE_LSYSCALL_H

#include <libhmpsf/xvm/lsyscall.h>
#include "../adapter/lsyscall.h"

static inline int xvm_lsyscall_module_load(struct hmpsf_module_attr_s *attr, unsigned char *code,
					   unsigned long code_size)
{
	struct xvm_sc_module_load_args_s args = {
		.attr = *attr,
		.code = code,
		.code_size = code_size,
	};
	return xvm_lsyscall(XVM_SC_CMD_MODULE_LOAD, (unsigned long)&args);
}

static inline int xvm_lsyscall_module_release(unsigned int id)
{
	return xvm_lsyscall(XVM_SC_CMD_MODULE_RELEASE, (unsigned long)id);
}

#endif
