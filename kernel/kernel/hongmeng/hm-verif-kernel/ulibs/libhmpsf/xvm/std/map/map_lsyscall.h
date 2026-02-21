/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interfaces for xvm lsyscall map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 15:54:48 2024
 */

#ifndef __ULIBS_LIBHMPSF_XVM_STD_MAP_LSYSCALL_H
#define __ULIBS_LIBHMPSF_XVM_STD_MAP_LSYSCALL_H

#include <libhmpsf/xvm/lsyscall.h>
#include "../../../adapter/lsyscall.h"

static inline int xvm_lsyscall_map_release_global_shared(unsigned int id)
{
	return xvm_lsyscall(XVM_SC_CMD_MAP_RELEASE_GS, (unsigned long)id);
}

#endif
