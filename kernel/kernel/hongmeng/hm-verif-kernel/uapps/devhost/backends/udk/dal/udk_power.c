/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Shutdown udk device
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 15:14:11 2019
 */
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/power.h>
#include <udk/mutex.h>
#include <udk/iolib.h>
#include <udk/lib/string.h>

#include <devhost/pm.h>
#include <devhost/interrupt.h>
#include <internal/init.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/types.h>
#include <libhwsecurec/securec.h>

static struct udk_blocking_notifier_list *g_reboot_notifier_list = NULL;
static struct udk_blocking_notifier_list *g_pm_notifier_list = NULL;

struct dev_pm_ops_pair {
	struct udk_device *dev;
	struct udk_dev_pm_ops *ops;
	struct dlist_node pm_list;
};

static DLIST_HEAD(udk_dev_pm_list);

static struct udk_mutex udk_pm_mutex;

static inline void udk_pm_mutex_init(void)
{
	udk_mutex_init(&udk_pm_mutex);
}

static inline void udk_pm_lock(void)
{
	udk_mutex_lock(&udk_pm_mutex);
}

static inline void udk_pm_unlock(void)
{
	udk_mutex_unlock(&udk_pm_mutex);
}

int udk_shutdown(void)
{
	return devhost_shutdown();
}

int udk_snapshot(void)
{
	return devhost_snapshot();
}

void udk_pm_dev_shutdown(void)
{
	struct dev_pm_ops_pair *pair = NULL;

	udk_info("udk_pm_dev_shutdown\n");

	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair == NULL || pair->ops == NULL || pair->dev == NULL ||
			pair->ops->shutdown == NULL) {
			continue;
		}

		udk_info("handle dev=%s shutdown\n", pair->dev->name);
		pair->ops->shutdown(pair->dev);
	}
	udk_pm_unlock();
}

static int dev_pmops_shutdown(struct device_info *dev)
{
	struct udk_device *udkdev = NULL;
	struct dev_pm_ops_pair *pair = NULL;

	if (dev == NULL) {
		udk_error("get NULL device_info\n");
		return -EINVAL;
	}

	udkdev = udk_device_of(dev);
	if (udkdev == NULL) {
		udk_error("Fail to get a udk_device\n");
		return -EINVAL;
	}

	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair == NULL || pair->ops == NULL || pair->dev != udkdev ||
			pair->ops->shutdown == NULL) {
			continue;
		}

		udk_info("handle dev=%s shutdown\n", udkdev->name);
		pair->ops->shutdown(udkdev);
		break;
	}
	udk_pm_unlock();

	return 0;
}

struct devhost_dev_pm_ops udk_dev_pmops = {
	.shutdown	= dev_pmops_shutdown,
	.pre_suspend	= NULL,
	.suspend	= NULL,
	.post_suspend	= NULL,
	.noirq_suspend	= NULL,
	.noirq_resume	= NULL,
	.pre_resume	= NULL,
	.resume		= NULL,
	.post_resume	= NULL,
};

int udk_device_pm_register(struct udk_device* udkdev, struct udk_dev_pm_ops *pm_ops)
{
	struct dev_pm_ops_pair *pair = NULL;
	struct dev_pm_ops_pair *next = NULL;

	if (udkdev == NULL || pm_ops == NULL) {
		udk_warn("udkdev or pm_ops is NULL!\n");
		return -EINVAL;
	}

	udk_pm_lock();
	dlist_for_each_entry_safe(pair, next, &udk_dev_pm_list,
				  struct dev_pm_ops_pair, pm_list) {
		if (pair->dev == udkdev) {
			pair->ops = pm_ops;
			udk_pm_unlock();
			return 0;
		}
	}
	udk_pm_unlock();

	pair = (struct dev_pm_ops_pair *)udk_malloc(sizeof(*pair));
	if (pair == NULL) {
		udk_error("malloc pair failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*pair);

	udk_device_capable_pm(udkdev, &udk_dev_pmops);
	pair->dev = udkdev;
	pair->ops = pm_ops;
	dlist_init(&pair->pm_list);

	udk_pm_lock();
	dlist_insert_tail(&udk_dev_pm_list, &pair->pm_list);
	udk_pm_unlock();

	return 0;
}

int udk_device_pm_unregister(struct udk_device* udkdev)
{
	int err;
	struct dev_pm_ops_pair *pos = NULL;
	struct dev_pm_ops_pair *next = NULL;

	if (udkdev == NULL) {
		udk_warn("udkdev is NULL!\n");
		return -EINVAL;
	}
	err = -ENOENT;

	udk_pm_lock();
	dlist_for_each_entry_safe(pos, next, &udk_dev_pm_list,
				  struct dev_pm_ops_pair, pm_list) {
		if (pos->dev == udkdev) {
			dlist_delete(&pos->pm_list);
			pos->dev = NULL;
			pos->ops = NULL;
			udk_free(pos);
			err = 0;
			break;
		}
	}
	udk_pm_unlock();

	return err;
}

static int platform_reboot_init(void)
{
	struct udk_blocking_notifier_list *notifier_list = NULL;

	notifier_list = udk_blocking_notifier_list_alloc();
	if (notifier_list == NULL) {
		return -ENOMEM;
	}
	g_reboot_notifier_list = notifier_list;

	return 0;
}

static int platform_pm_init(void)
{
	g_pm_notifier_list = udk_blocking_notifier_list_alloc();
	return g_pm_notifier_list == NULL ? -ENOMEM : 0;
}

static int platform_reboot_prepare(unsigned int type, char *cmd)
{
	unsigned long event;

	switch (type) {
	case DH_PM_SHUTDOWN_PREPARE:
		event = UDK_REBOOT_NOTIFER_EVENT_SHUTDOWN;
		break;
	case DH_PM_REBOOT_PREPARE:
		event = UDK_REBOOT_NOTIFER_EVENT_REBOOT;
		break;
	default:
		udk_error("invalid prepare type: %u\n", type);
		return -EINVAL;
	}

	return udk_blocking_notifier_list_call(g_reboot_notifier_list,
					       event, (void *)cmd);
}

static void platform_syscore_shutdown(void)
{
	udk_info("platform_syscore_shutdown\n");
	/* Release udk framework resources here, instead of
	 * traversing the shutdown callback of udk drivers */
}

/* rollback all success pm suspend operation (from last success pm pair to head) */
static void pmops_suspend_rollback(struct dev_pm_ops_pair *last)
{
	struct dev_pm_ops_pair *pair = last;

	if (pair == NULL) {
		return;
	}

	udk_info("pmops suspend rollback start\n");
	dlist_for_each_entry_reverse_continue(pair, &udk_dev_pm_list,
					      struct dev_pm_ops_pair, pm_list) {
		if (pair == NULL || pair->ops == NULL || pair->dev == NULL ||
			pair->ops->resume == NULL) {
			continue;
		}
		pair->ops->resume(pair->dev);
	}
	udk_info("pmops suspend rollback end\n");
}

static int platform_pmops_suspend(void)
{
	udk_info("platform_pmops_suspend\n");
	struct dev_pm_ops_pair *pair = NULL;
	int err = 0;

	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair == NULL || pair->ops == NULL || pair->dev == NULL ||
			pair->ops->suspend == NULL) {
			continue;
		}

		err = pair->ops->suspend(pair->dev);
		if (err < 0) {
			udk_warn("udk dev suspend failed: name=%s, err=%s\n",
				pair->dev->name, strerror(-err));
			break;
		}
	}
	if (err < 0) {
		pmops_suspend_rollback(pair);
	}
	udk_pm_unlock();

	return err;
}

static void platform_pmops_resume(void)
{
	udk_info("platform_pmops_resume\n");
	struct dev_pm_ops_pair *pair = NULL;

	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair == NULL || pair->ops == NULL || pair->dev == NULL ||
			pair->ops->resume == NULL) {
			continue;
		}

		pair->ops->resume(pair->dev);
	}
	udk_pm_unlock();
}

static int platform_suspend(unsigned int action)
{
	int err = 0;

	switch (action) {
	case UDK_PM_SUSPEND:
		err = platform_pmops_suspend();
		break;
	case UDK_PM_RESUME:
		platform_pmops_resume();
		break;
	default:
		err = 0;
	}

	return err;
}

/* rollback all success pm hibernate freeze operation (from last success pm pair to head) */
static void pmops_hibernate_freeze_rollback(struct dev_pm_ops_pair *last)
{
	struct dev_pm_ops_pair *pair = last;

	if (pair == NULL) {
		return;
	}

	udk_info("pmops hibernate freeze rollback start\n");
	dlist_for_each_entry_reverse_continue(pair, &udk_dev_pm_list,
					      struct dev_pm_ops_pair, pm_list) {
		if (pair == NULL || pair->ops == NULL || pair->dev == NULL ||
		    pair->ops->thaw == NULL) {
			continue;
		}
		pair->ops->thaw(pair->dev);
	}
	udk_info("pmops hibernate freeze rollback end\n");
}

static int platform_pmops_hibernate_freeze(void)
{
	int err = 0;
	struct dev_pm_ops_pair *pair = NULL;

	udk_info("platform_pmops_hibernate freeze\n");
	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair->ops == NULL || pair->ops->freeze == NULL || pair->dev == NULL) {
			continue;
		}
		err = pair->ops->freeze(pair->dev);
		if (err < 0) {
			udk_warn("udk dev hibernate freeze failed: name=%s, err=%s\n",
				pair->dev->name, strerror(-err));
			break;
		}
	}
	if (err < 0) {
		pmops_hibernate_freeze_rollback(pair);
	}
	udk_pm_unlock();

	return err;
}

static void platform_pmops_hibernate_thaw(void)
{
	struct dev_pm_ops_pair *pair = NULL;

	udk_info("platform_pmops_hibernate thaw\n");
	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair->ops == NULL || pair->ops->thaw == NULL || pair->dev == NULL) {
			continue;
		}
		pair->ops->thaw(pair->dev);
	}
	udk_pm_unlock();
}

static void platform_pmops_hibernate_restore(void)
{
	struct dev_pm_ops_pair *pair = NULL;

	udk_info("platform_pmops_hibernate restore\n");
	udk_pm_lock();
	dlist_for_each_entry(pair, &udk_dev_pm_list, struct dev_pm_ops_pair, pm_list) {
		if (pair->ops == NULL || pair->ops->restore == NULL || pair->dev == NULL) {
			continue;
		}
		pair->ops->restore(pair->dev);
	}
	udk_pm_unlock();
}

static int platform_hibernate(unsigned int action)
{
	int err = 0;

	udk_info("platform_pmops_hibernate action %d\n", action);
	switch (action) {
	case UDK_PM_HIBERNATE_FREEZE:
		err = platform_pmops_hibernate_freeze();
		break;
	case UDK_PM_HIBERNATE_THAW:
		platform_pmops_hibernate_thaw();
		break;
	case UDK_PM_HIBERNATE_RESTORE:
		platform_pmops_hibernate_restore();
		break;
	default:
		err = 0;
	}

	return err;
}

static int platform_cpu_remove(unsigned int cpu)
{
	devhost_migrate_all_irq_of_cpu(cpu);
	return 0;
}

static const struct devhost_plat_pm_ops udk_plat_pm_ops = {
	.power_off = NULL,
	.reboot = NULL,
	.prepare = platform_reboot_prepare,
	.syscore_shutdown = platform_syscore_shutdown,
	.wq_freeze = NULL,
	.wq_thaw = NULL,
	.suspend = platform_suspend,
	.cpu_add = NULL,
	.cpu_remove = platform_cpu_remove,
	.wakelock_stat = NULL,
	.idle_enter = NULL,
	.idle_exit = NULL,
	.hibernate = platform_hibernate,
	.dubai_hwlog = NULL,
	.thermal_device_acquire = NULL,
	.thermal_device_temp_read = NULL,
};

int udk_reboot_notifier_register(struct udk_notifier *notifier)
{
	if (notifier == NULL) {
		return -EINVAL;
	}

	return udk_blocking_notifier_list_insert(g_reboot_notifier_list, notifier);
}

int udk_reboot_notifier_unregister(struct udk_notifier *notifier)
{
	if (notifier == NULL) {
		return -EINVAL;
	}

	return udk_blocking_notifier_list_delete(g_reboot_notifier_list, notifier);
}

int udk_pm_notifier_register(struct udk_notifier *notifier)
{
	return udk_blocking_notifier_list_insert(g_pm_notifier_list, notifier);
}

int udk_pm_notifier_unregister(struct udk_notifier *notifier)
{
	return udk_blocking_notifier_list_delete(g_pm_notifier_list, notifier);
}

int udk_pm_notifier_call(unsigned int event)
{
	return udk_blocking_notifier_list_call(g_reboot_notifier_list, event, NULL);
}

int udk_pm_notifier_call_robust(unsigned int event, unsigned int revent)
{
	return udk_blocking_notifier_list_call_robust(g_reboot_notifier_list, event, revent, NULL);
}

static int udk_pm_init(void)
{
	int err;

	udk_pm_mutex_init();

	err = platform_reboot_init();
	if (err < 0) {
		udk_error("udk platform reboot init failed: %s\n", strerror(-err));
		return err;
	}

	err = platform_pm_init();
	if (err < 0) {
		udk_error("udk platform pm init failed: %s\n", strerror(-err));
		return err;
	}

	return devhost_pm_init(&udk_plat_pm_ops);
}
udk_init_call(udk_pm_init);
