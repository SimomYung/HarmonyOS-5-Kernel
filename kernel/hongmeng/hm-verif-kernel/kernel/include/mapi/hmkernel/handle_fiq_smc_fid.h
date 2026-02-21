/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Platform-specific definitions for handle fiq via smc to EL3
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar  3 11:56:46 2023
 */

#ifndef MAPI_HMKERNEL_HANDLE_FIQ_SMC_FID_H
#define MAPI_HMKERNEL_HANDLE_FIQ_SMC_FID_H

#include <hmkernel/types.h>

#define HANDLE_FIQ_SMC_FID	UL(0xc3000f13)

enum handle_fiq_smc_op {
	FIQ_SMC_OP_CHECK_SUPPORT,
	FIQ_SMC_OP_ACK,
	FIQ_SMC_OP_EOI, /* x2: fiq_id */
	FIQ_SMC_OP_GET_LASTWORD_FIQ_ID,
	FIQ_SMC_OP_RAISE_SGI_CPUSET, /* x2: running cpuset */
	FIQ_SMC_OP_GET_WDT_FIQ_ID,
	FIQ_SMC_OP_RAISE_SGI_MPIDR, /* x2: mpidr of a cpu */
	FIQ_SMC_OP_NR,
};

#endif
