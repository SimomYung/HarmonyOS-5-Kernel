/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define udk power headfile
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 16:28:40 2019
 */

#ifndef __UDK_POWER_H__
#define __UDK_POWER_H__

#include <udk/notifier.h>
#include <udk/device.h>

/* keep consistent with libsysif */
#define UDK_REBOOT_NOTIFER_EVENT_SHUTDOWN	0UL
#define UDK_REBOOT_NOTIFER_EVENT_REBOOT		1UL

#define UDK_PM_HIBERNATE_PREPARE		0U
#define UDK_PM_HIBERNATE_POST			1U
#define UDK_PM_HIBERNATE_RESTORE_PREPARE	2U
#define UDK_PM_HIBERNATE_POST_RESTORE		3U
#define UDK_PM_HIBERNATE_FREEZE			4U
#define UDK_PM_HIBERNATE_THAW			5U
#define UDK_PM_HIBERNATE_RESTORE		6U
#define UDK_PM_SUSPEND				7U
#define UDK_PM_RESUME				8U

struct udk_device;

struct udk_dev_pm_ops {
	int (*suspend)(struct udk_device* dev);
	void (*resume)(struct udk_device* dev);
	void (*shutdown)(struct udk_device* dev);
	int (*freeze)(struct udk_device* dev);
	void (*thaw)(struct udk_device* dev);
	void (*restore)(struct udk_device* dev);
};

int udk_device_pm_register(struct udk_device* udkdev, struct udk_dev_pm_ops *pm_ops);
int udk_device_pm_unregister(struct udk_device* udkdev);
void udk_pm_dev_shutdown(void);

int udk_shutdown(void);
int udk_snapshot(void);

int udk_reboot_notifier_register(struct udk_notifier *notifier);
int udk_reboot_notifier_unregister(struct udk_notifier *notifier);

int udk_pm_notifier_register(struct udk_notifier *notifier);
int udk_pm_notifier_unregister(struct udk_notifier *notifier);
int udk_pm_notifier_call(unsigned int event);
int udk_pm_notifier_call_robust(unsigned int event, unsigned int revent);

#endif /* __DEVHOST_UDK_POWER_H__ */
