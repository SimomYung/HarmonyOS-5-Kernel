/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 16 05:10:41 2019
 */
#ifndef AARCH64_AAPCS_H
#define AARCH64_AAPCS_H

#include <hmkernel/types.h>
#include <hmkernel/uaccess.h>

#include <hmasm/ucontext.h>
#include <hmasm/cpu_local.h>
#include <mapi/hmasm/aapcs.h>

static inline int
aarch32_apcs_pull_u32(u32 *res, unsigned int pos, const struct arch_uctx *uctx,
		      bool is_fastpath, struct arch_cpu_local_s *cpu_local)
{
	int err;

	if (pos < 4U) {
		/* aarch32 still pass args like arm */
		*res = (u32)REG_NORMAL(&uctx->regs, pos);
		err = 0;
	} else if (!is_fastpath) {
		void __user *uptr = u64_to_uptr(REG_SP(&uctx->regs) + (size_t)(pos - 4U) * sizeof(u32));
		err = copy_from_user(res, uptr, sizeof(u32));
	} else {
		void __user *uptr = u64_to_uptr(REG_SP(&uctx->regs) + (size_t)(pos - 4U) * sizeof(u32));
		u32 *ptr = ptr_from_void(uptr, u32);
		fastpath_check_user_access(actv_kobjvs_of(actv_of_uctx(uctx)), UACCESS_DIRECTION_U2K,
					   cpu_local, ptr, sizeof(u32));
		*res = *ptr;
		err = 0;
	}
	return err;
}

#endif
