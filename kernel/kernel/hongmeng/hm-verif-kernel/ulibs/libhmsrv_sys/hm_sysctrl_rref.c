/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: sysctrl rref ops
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 11 16:37:37 2023
 */

#include <string.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <hongmeng/hongmeng.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsrv_sys/hm_sysctrl.h>

static rref_t g_sysctrl_rref;

static int do_sysctrl_require(rref_t *rref)
{
	int ret = E_HM_OK;
	struct __actvret_hmcall_sysctrl_require hmcall_ret;

	mem_zero_s(hmcall_ret);
	ret = actvcall_hmcall_sysctrl_require((unsigned int)0, (unsigned int)0xff, &hmcall_ret);
	if (ret != E_HM_OK) {
		hm_error("actvcall_hmcall_sysctrl_require failed, err=%s\n",
			 hmstrerror(ret));
	} else {
		*rref = hmcall_ret.rref;
	}

	return ret;
}

rref_t hm_sysctrl_rref_of(void)
{
	int err = E_HM_OK;
	rref_t rref;

	if (g_sysctrl_rref != 0ULL) {
		rref = g_sysctrl_rref;
	} else {
		err = do_sysctrl_require(&rref);
		if (err != E_HM_OK) {
			rref = ERR_TO_REF(err);
		} else if (!hm_fetch_fork_allowd_flag()) {
			g_sysctrl_rref = rref;
		} else {
			/* do nothing */
		}
	}

	return rref;
}
