/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Devhost tracefs operation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 12:26:14 2023
 */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/printk.h>

#include "tracefs_api.h"
#include "tracefs.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("tracefs ops");
MODULE_VERSION("0.01");

static int enable_disable_ldk_event(unsigned tp_index, bool enable)
{
	return hmtrace_enable_disable_ldk_event(tp_index, enable);
}

static int ldk_event_format(unsigned tp_index, char *format)
{
	return hmtrace_ldk_event_format(tp_index, format);
}

static int print_ldk_event(unsigned int tp_index, void *entry, char *event_str)
{
	return hmtrace_print_ldk_event(tp_index, entry, event_str);
}

static struct devhost_tracefs_ops g_tracefs_ops = {
	.enable_disable_ldk_event = enable_disable_ldk_event,
	.ldk_event_format = ldk_event_format,
	.print_ldk_event = print_ldk_event
};

static int __init devhost_tracefs_init(void)
{
	int ret;

	/* register ops */
	ret = devhost_tracefs_ops_init(&g_tracefs_ops);
	if (ret < 0) {
		pr_err("devhost_tracefs_ops init failed, err=%d\n", ret);
	}
	return ret;
}

module_init(devhost_tracefs_init);
