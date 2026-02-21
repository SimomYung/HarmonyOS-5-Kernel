/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: devhost iaware notify operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 28 12:15:54 2023
 */

#include <asm/uaccess.h>
#include <asm/ioctls.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/version.h>

#include "iaware_notify_api.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
#include <chipset_common/cpu_netlink/cpu_netlink.h>
#else
#include <cpu_netlink/cpu_netlink.h>
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("iaware notify ops");
MODULE_VERSION("0.01");

static int devhost_send_to_cpu_netlink(void *buffer)
{
	struct cpu_netlink_event_s *event = (struct cpu_netlink_event_s*)buffer;

	send_to_user(event->sock_no, event->len, event->data);
	return 0;
}

static struct devhost_iaware_notify_ops g_iaware_notify_ops __read_mostly = {
	.send_to_cpu_netlink = devhost_send_to_cpu_netlink,
};

static int __init devhost_iaware_notify_init(void)
{
	int ret;

	/* register socket poll ops */
	ret = devhost_iaware_notify_ops_init(&g_iaware_notify_ops);
	if (ret < 0) {
		pr_err("devhost_iaware_notify_ops init failed, err=%d\n", ret);
	}

	return ret;
}

module_init(devhost_iaware_notify_init);
