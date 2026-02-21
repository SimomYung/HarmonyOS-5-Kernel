/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Sysif of signal mask
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 19 06:29:03 2019
 */
#include <errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <libsysif/utils.h>
#include <libmem/utils.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>

__thread struct sec_cred sysif_callinfo = {
	.sender = 0ULL,
	.credential = __ACTV_CRED_SELFCALL,
	.selfcall_type = __SEC_CRED_ACTV_SELFCALL,
	.cred_flags.selfcall_fwd_use_self = SEC_CRED_SELFCALL_FWD_USE_SELF,
	.cred_flags.fwd_base_cred_valid = SEC_CRED_INVALID,
	.cred_flags.fwd_sh_cred_valid = SEC_CRED_INVALID,
	.cred_flags.src_base_cred_valid = SEC_CRED_INVALID,
	.cred_flags.src_sh_cred_valid = SEC_CRED_INVALID,
	.self_cred_idx = 0ULL
};
