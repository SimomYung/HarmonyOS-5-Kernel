/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Arch exception information management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 09 09:48:17 2020
 */
#ifndef AARCH64_MAPI_ASM_EXCEPTION_H
#define AARCH64_MAPI_ASM_EXCEPTION_H

#include <uapi/hmasm/exception.h>

#ifndef __ASSEMBLY__
static inline
void arch_exception_info_set_dabort_status(struct __arch_exception_info *info,
					   u64 esr)
{
	info->abort_status = esr;
}
#endif

#endif
