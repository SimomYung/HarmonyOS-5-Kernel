/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implementation of xvm lsycall interface in hm adapter
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 15 20:03:04 2024
 */

#ifndef __ULIBS_LIBHMPSF_XVM_ADAPTER_LSYSCALL_H
#define __ULIBS_LIBHMPSF_XVM_ADAPTER_LSYSCALL_H

#include <hongmeng/hm_lsyscall.h>
#include <hmasm/lsyscall.h>

static inline int xvm_lsyscall(unsigned int cmd, unsigned long arg)
{
	int ret = (int)lsyscall_syscall2(__NR_xvm, cmd, arg);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}
	return ret;
}

#endif
