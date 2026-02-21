/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define interface of suspend
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 08 11:56:35 CST 2024
 */

#include <devhost/log.h>

#include <hmkernel/power/power_module.h>
#include <hongmeng/errno.h>

#include <lib/errno.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libsysif/sysmgr/api.h>
#include <liblinux/pal.h>

int liblinux_pal_suspend_cnt_read(int *cnt)
{
	int err;
	int suspend_cnt;
	rref_t rref = 0ULL;

	rref = liblinux_pal_pm_class_rref_acquire();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		dh_error("pm calss rref invalid, err is %s\n", hmstrerror(err));
		return -hmerrno2posix(err);
	}
	err = hm_module_ioctl(rref, __PM_SUSPEND_CNT_READ, NULL, 0UL,
			      (void *)&suspend_cnt, sizeof(int));
	if (err < 0) {
		dh_warn("module ioctl access failed! err is %s\n", hmstrerror(err));
		return -hmerrno2posix(err);
	}
	*cnt = suspend_cnt;

	return 0;
}

int liblinux_pal_set_pm_crypt_info(uint32_t key_index, bool clear)
{
	int ret;
	struct crypt_info crypt_info = {0};

	crypt_info.key_len = MAX_CI_KEY_LEN;
	crypt_info.key_index = key_index;
	ret = actvcall_hmcall_pwrmgr_set_crypt_info(crypt_info, clear);
	if (ret != E_HM_OK) {
		hm_error("%s crypt info failed, err=%s\n", clear ? "clear" : "set", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}
