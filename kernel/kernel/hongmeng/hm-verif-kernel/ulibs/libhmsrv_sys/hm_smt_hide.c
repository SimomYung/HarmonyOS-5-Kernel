/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: SMT hide feature.
 * Author: Huawei HISI Kirin
 * Create: Fri Nov 03 19:03:19 2023
 */

#include <stddef.h>
#include <stdint.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_smt_hide.h>
#include <libsysif/sysmgr/api.h>
#include <libhmlog/hmlog.h>

bool hm_smt_privileged_user(void)
{
	struct __actvret_hmcall_procfs_smt_privileged_user actv_ret;
	bool ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_procfs_smt_privileged_user(&actv_ret);
	if (rc == E_HM_OK) {
		ret = actv_ret.flag;
	} else {
		ret = true;
	}

	return ret;
}