/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Adapted interfaces for xvm map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 16:09:26 2024
 */
#ifndef __ULIBS_LIBHMPSF_XVM_ADAPTER_MAP_H
#define __ULIBS_LIBHMPSF_XVM_ADAPTER_MAP_H

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <libhmpsf/adapter/lsyscall.h>
#include "../../../xvm/std/map/map_lsyscall.h"

static inline int xvm_syscall_map_release_global_shared(unsigned int id)
{
	int err = E_HM_OK;

	if (xvm_is_using_lsyscall()) {
		err = xvm_lsyscall_map_release_global_shared(id);
	} else {
		err = actvcall_hmcall_hmpsf_map_release_global_shared(id);
	}
	return err;
}
#endif
