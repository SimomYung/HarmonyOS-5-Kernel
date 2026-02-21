/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: liblinux base sysctl APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 20 09:40:43 2022
 */
#include <lnxbase/lnxbase.h>

#include <hmkernel/sysconf.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_sysctrl.h>

/* sysinfo helper */
int lnxbase_sysinfo_get(struct lnxbase_sysinfo *info)
{
	struct __actvret_hmcall_sysctrl_sysinfo_native info_ret;
	int ret;

	if (info == NULL) {
		return -EINVAL;
	}

	/* try to get sysinfo from sysmgr */
	mem_zero_s(info_ret);
	ret = actvcall_hmcall_sysctrl_sysinfo_native(&info_ret);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	info->memstart = (unsigned long long)info_ret.info.ramstart;
	info->memend = (unsigned long long)info_ret.info.ramend;
	info->total_ram = (unsigned long long)info_ret.info.total_ram;

	return 0;
}

int lnxbase_max_cspace_slots_get(unsigned long *max)
{
	int ret;
	unsigned long size;

	ret = hm_sysctrl_read_conf(__SYSCONF_CSPACE_SLOTS, (void *)&size, sizeof(unsigned long));
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	if (max != NULL) {
		*max = size;
	}
	return 0;
}
