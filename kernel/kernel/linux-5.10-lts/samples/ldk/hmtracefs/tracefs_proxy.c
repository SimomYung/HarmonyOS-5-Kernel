// SPDX-License-Identifier: GPL-2.0
#include <stddef.h>
#include <libhmtrace_ng/hmtrace.h>
#include <hongmeng/errno.h>

int hmtrace_register_ldk_events(struct tracepoint_config *data, unsigned int num, unsigned int *tp_type)
{
	int ret = hm_trace_register_ldk_events(data, tp_type, num);
	if (ret < E_HM_OK)
		return -hmerrno2posix(ret);
	return ret;
}

int hmtrace_add_ldk_printk_format(char *str)
{
	int ret = hm_trace_add_ldk_printk_format(str);
	if (ret < E_HM_OK)
		return -hmerrno2posix(ret);
	return ret;
}
