/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Freq-Info hide feature.
 * Author: Huawei HISI Kirin
 * Create: Fri Sep 13 15:48:19 2024
 */

#include <stddef.h>
#include <stdint.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_freq_hide.h>
#include <libsysif/sysmgr/api.h>
#include <libhmlog/hmlog.h>

bool hm_freq_privileged_user(void)
{
	struct __actvret_hmcall_procfs_freq_privileged_user actv_ret;
	bool ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_procfs_freq_privileged_user(&actv_ret);
	if (rc == E_HM_OK) {
		ret = actv_ret.flag;
	} else {
		ret = false;
	}
	return ret;
}