/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of hmtrace
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 18 07:06:44 2023
 */
#include <libhmtrace_ng/hmtrace.h>
#include <lib/trace/trace_misc.h>

#ifdef CONFIG_TRACEMGR
void hm_trace_set_taskinfo_save(unsigned int cpu, bool enable)
{
	BUG_ON(cpu >= HM_SAVED_CPUS);

	struct trace_saved_taskinfo_s *saved_taskinfo = (struct trace_saved_taskinfo_s *)hm_trace_shm_saved_taskinfo_addr_of();
	saved_taskinfo->taskinfo_save[cpu] = enable;
}
#else
void hm_trace_set_taskinfo_save(unsigned int cpu, bool enable)
{
	UNUSED(cpu, enable);
}
#endif /* CONFIG_TRACEMGR */
