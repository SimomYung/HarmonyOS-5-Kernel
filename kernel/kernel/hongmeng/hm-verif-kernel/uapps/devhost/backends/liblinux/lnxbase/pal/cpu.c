/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: liblinux PAL implement for cpu related operations.
 * Author: Huawei OS Kernel Lab
 * Create: Fue Apr 2 19:54:52 2024
 */

#include <devhost/log.h>

#include <hmkernel/power/power_module.h>
#include <hongmeng/errno.h>

#include <lib/errno.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <liblinux/pal.h>
#include <libsysif/sysmgr/api.h>

int liblinux_pal_get_cpu_total_inst(uint64_t *cpu_inst, uint32_t cpu_nr)
{
	int err;
	rref_t rref = 0ULL;
	rref = hm_module_open(__PM_CLASS_NAME);
	if (IS_REF_ERR(rref)) {
		dh_error("module ioctl open failed! err is %s\n", hmstrerror(err));
		return -hmerrno2posix(REF_TO_ERR(rref));
	}
	err = hm_module_ioctl(rref, __PM_POWER_TRACK_CPU_TOTAL_INST, NULL, 0UL,
			      (void *)cpu_inst, cpu_nr * sizeof(uint64_t));
	if (err < 0) {
		dh_warn("module ioctl access failed! err is %s\n", hmstrerror(err));
		(void)hm_module_close(rref);
		return -hmerrno2posix(err);
	}
	(void)hm_module_close(rref);

	return 0;
}

int liblinux_pal_get_max_capacity_cpu(int in_suspend, unsigned int *cpuid)
{
	int err;
	rref_t rref = 0ULL;

	if (cpuid == NULL) {
		dh_warn("invalid cpuid\n");
		return -hmerrno2posix(E_HM_INVAL);
	}
	if (in_suspend != 0) {
		rref = hm_module_open(__PM_CLASS_NAME);
		if (IS_REF_ERR(rref)) {
			dh_error("module ioctl open failed! err is %s\n", hmstrerror(err));
			return -hmerrno2posix(REF_TO_ERR(rref));
		}
		err = hm_module_ioctl(rref, __PM_FAST_CPU_READ, NULL, 0UL,
				      (void *)cpuid, sizeof(unsigned int));
		if (err < 0) {
			dh_warn("module ioctl access failed! err is %s\n", hmstrerror(err));
			(void)hm_module_close(rref);
			return -hmerrno2posix(err);
		}
		(void)hm_module_close(rref);
	} else {
		err = actvcall_hmcall_pwrmgr_resume_fast_cpu_read(cpuid);
		if (err < 0) {
			dh_warn("fast cpu acquire failed! err is %s\n", hmstrerror(err));
			return -hmerrno2posix(err);
		}
	}

	return 0;
}
