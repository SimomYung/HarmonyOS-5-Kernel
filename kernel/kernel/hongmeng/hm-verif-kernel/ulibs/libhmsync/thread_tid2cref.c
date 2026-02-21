/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Get thread cref by tid
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 21 17:43:48 2020
 */
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/thread.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>

cref_t thread_tid2cref(int tid)
{
	int err;
	cref_t cref;
	struct __actvret_hmcall_thread_tid2cref call_ret;

	mem_zero_s(call_ret);
	err = actvcall_hmcall_thread_tid2cref(tid, &call_ret);
	if (err != E_HM_OK) {
		cref = ERR_TO_REF(err);
	} else {
		cref = call_ret.tcb_cref;
	}
	return cref;
}
