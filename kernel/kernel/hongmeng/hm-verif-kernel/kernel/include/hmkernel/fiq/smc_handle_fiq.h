/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces of smc handle fiq
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar  3 11:56:46 2023
 */

#ifndef KERNEL_DRIVERS_SMC_HANDLE_FIQ_H
#define KERNEL_DRIVERS_SMC_HANDLE_FIQ_H

#include <hmkernel/const.h>
#include <hmkernel/smccc.h>

#include <hmkernel/handle_fiq_smc_fid.h>

#ifdef CONFIG_SMC_HANDLE_FIQ
static inline u32 fiq_smc_call(unsigned long op, unsigned long x2)
{
	struct smccc_res res = {};

	if (op >= (unsigned long)FIQ_SMC_OP_NR) {
		res.r0 = SMC_UNK;
	} else {
		smccc_smc_call(smccc_args(HANDLE_FIQ_SMC_FID, op, x2), &res);
	}

	return u_to_u32(res.r0);
}

static inline bool fiq_smc_supported(void)
{
	u32 ret = fiq_smc_call(FIQ_SMC_OP_CHECK_SUPPORT, 0UL);
	return (ret == (u32)SMC_OK);
}
#else /* !CONFIG_SMC_HANDLE_FIQ */
static inline u32 fiq_smc_call(unsigned long op, unsigned long x2)
{
	UNUSED(op, x2);
	return (u32)SMC_UNK;
}

static inline bool fiq_smc_supported(void)
{
	return false;
}
#endif

#endif
