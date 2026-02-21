/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Proc_affinity info get interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 27 15:46:31 2021
 */
#include <sched.h>
#include <libsysif/sysmgr/api.h>
#include <hongmeng/types.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_proc_affinity.h>

/*
 * get process's proc_affinity, pid = 0 for self process
 *
 * Note: Only system service is supported
 */
int hm_proc_affinity_get(pid_t pid, struct proc_affinity_info_s *proc_affinity_info)
{
	int err;
	struct __actvret_hmcall_proc_affinity_get ret;

	mem_zero_s(ret);

	if (proc_affinity_info == NULL) {
		return E_HM_INVAL;
	}

	err = actvcall_hmcall_proc_affinity_get(pid, &ret);
	if (err == 0) {
		*proc_affinity_info = ret.proc_affinity_info;
	}

	return err;
}

void hm_proc_set_default_affinity(pid_t pid)
{
	int ret;
	struct proc_affinity_info_s proc_affinity_info;

	mem_zero_s(proc_affinity_info);

	ret = hm_proc_affinity_get(pid, &proc_affinity_info);
	if (ret == E_HM_OK) {
		cpu_set_t cpuset = {0};
		cpuset.__bits[0] = (unsigned long)proc_affinity_info.default_affinity.__bits;
		ret = hm_thread_sched_setaffinity(0, sizeof(cpuset), &cpuset);
		if (ret != E_HM_OK) {
			hm_warn("Failed to set process to default affinity 0x%x, err is %s\n",
				 proc_affinity_info.default_affinity.__bits, hmstrerror(ret));
		}
	} else {
		hm_warn("Failed to get proc affinity, err is %s\n", hmstrerror(ret));
	}
}
