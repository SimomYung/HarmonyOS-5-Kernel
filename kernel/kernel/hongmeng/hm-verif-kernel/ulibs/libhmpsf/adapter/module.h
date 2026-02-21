/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Adapted interfaces for xvm module
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 14:38:18 2024
 */
#ifndef __ULIBS_LIBHMPSF_XVM_ADAPTER_MODULE_H
#define __ULIBS_LIBHMPSF_XVM_ADAPTER_MODULE_H

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmpsf/adapter/lsyscall.h>
#include "../xvm/module_lsyscall.h"

static inline int xvm_syscall_module_load(struct hmpsf_module_attr_s *attr, unsigned char *code,
					  unsigned long code_size)
{
	int ret = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		ret = xvm_lsyscall_module_load(attr, code, code_size);
	} else {
		ret = actvcall_hmcall_hmpsf_load(*attr, code, code_size);
	}

	return ret;
}

static inline int xvm_syscall_module_release(unsigned int id)
{
	int ret = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		ret = xvm_lsyscall_module_release(id);
	} else {
		ret = actvcall_hmcall_hmpsf_release(id);
	}
	return ret;
}

#endif
