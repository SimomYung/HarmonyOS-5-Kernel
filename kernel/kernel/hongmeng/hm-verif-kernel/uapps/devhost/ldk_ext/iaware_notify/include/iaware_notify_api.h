/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Prototypes and type definitions for iaware notify APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 28 12:15:54 2023
 */

#ifndef __DEVHOST_PLUGIN_IAWARE_NOTIFY_API_H__
#define __DEVHOST_PLUGIN_IAWARE_NOTIFY_API_H__

#if defined(MODULE)
#include <linux/types.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <udk/stdint.h>
#include <stddef.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

#include <devhost/device.h>

struct devhost_iaware_notify_ops {
	int (*send_to_cpu_netlink)(void *);
};

#define MAX_DATA_LEN 18
struct cpu_netlink_event_s {
	int sock_no;
	int len;
	int data[MAX_DATA_LEN];
};

/*
 * Interface for LDK to register it's socket ops and init devinfo.
 * Return 0 if LDK stack enabled in hm-netd and ops registered successfully,
 * return error code otherwise
 */
int devhost_iaware_notify_ops_init(struct devhost_iaware_notify_ops *ops);

#endif /* __DEVHOST_PLUGIN_IAWARE_NOTIFY_API_H__ */
