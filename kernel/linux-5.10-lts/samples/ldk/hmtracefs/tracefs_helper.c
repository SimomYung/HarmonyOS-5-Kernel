// SPDX-License-Identifier: GPL-2.0
#include "tracefs_helper.h"

#include <libhmactv/actv.h>
#include <libhmtrace_ng/hmtrace.h>

int hmtracefs_tid_of(void)
{
	return hm_actv_local_thread_tid_of();
}

void hmtracefs_set_taskinfo_save(int enable)
{
	hm_trace_set_taskinfo_save(hm_trace_current_cpu_of(), enable != 0);
}
