/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Cpuhp related operates
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 07 11:53:32 2022
 */
#include <libhmpm/cpuhp.h>
#include <hongmeng/errno.h>
#include <libsysif/pwrmgr/api.h>
#include <libhmsrv_sys/hm_path.h>

#include "pm_sysif.h"

static int handle_cpuhp(unsigned int cpu, unsigned int event)
{
	int ret = 0;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		ret = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvcapcall_pmcall_cpuhp_action(rref, cpu, event);
	if (ret != E_HM_OK) {
		hm_error("failed to hotplug cpu: %u event: %u\n", cpu, event);
	}

	return ret;
}

int hm_cpu_up(unsigned int cpu)
{
	return handle_cpuhp(cpu, CPUHP_ADD_EVENT);
}

int hm_cpu_down(unsigned int cpu)
{
	return handle_cpuhp(cpu, CPUHP_REMOVE_EVENT);
}
