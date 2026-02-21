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
#include <liblinux/pal.h>

int liblinux_pal_suspend_cnt_read(int *cnt)
{
	int err;
	int suspend_cnt;
	rref_t rref = 0ULL;

	rref = hm_module_open(__PM_CLASS_NAME);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		dh_error("module ioctl open failed! err is %s\n", hmstrerror(err));
		return -hmerrno2posix(err);
	}
	err = hm_module_ioctl(rref, __PM_SUSPEND_CNT_READ, NULL, 0UL,
			      (void *)&suspend_cnt, sizeof(int));
	if (err < 0) {
		dh_warn("module ioctl access failed! err is %s\n", hmstrerror(err));
		(void)hm_module_close(rref);
		return -hmerrno2posix(err);
	}
	*cnt = suspend_cnt;
	(void)hm_module_close(rref);

	return 0;
}
