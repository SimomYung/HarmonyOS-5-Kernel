// SPDX-License-Identifier: GPL-2.0-only
#include <linux/list.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <liblinux_device.h>
#include <trace/hooks/liblinux.h>

extern struct kset *devices_kset;
/*
 * lock for:
 * 1. `device_chain` read vs write
 * 2. `platform_notify_remove` vs `__call_device_notifier`
 */
static DEFINE_MUTEX(dev_ch_mutex);
static RAW_NOTIFIER_HEAD(device_chain);

static int __call_device_notifier(struct notifier_block *nb)
{
	int err = 0;
	struct device *dev = NULL;

	if (devices_kset != NULL) {
		spin_lock(&devices_kset->list_lock);
		list_for_each_entry(dev, &devices_kset->list, kobj.entry) {
			err = nb->notifier_call(nb, LIBLINUX_DEVICE_ADD, dev);
			err = notifier_to_errno(err);
			/*
			 * disable devhost dma_ops. enable `dma_alloc_direct`
			 * make sure the cpu_addr of `dma_alloc_attrs` is dma coherent
			 */
			dev->dma_ops = NULL;
			if (err) {
				/* just warn? */
				pr_warn("notifier_call failed for dev %s, err=%d\n", dev_name(dev), err);
			}
		}
		spin_unlock(&devices_kset->list_lock);
	}
	return 0;
}

int liblinux_register_device_notifier(struct notifier_block *nb)
{
	int err = 0;

	if (nb == NULL) {
		return -EINVAL;
	}
	mutex_lock(&dev_ch_mutex);
	err = raw_notifier_chain_register(&device_chain, nb);
	if (err) {
		goto unlock;
	}

	/* notify all devices to nb */
	err = __call_device_notifier(nb);
unlock:
	mutex_unlock(&dev_ch_mutex);
	return err;
}
EXPORT_SYMBOL(liblinux_register_device_notifier);

static int __liblinux_device_ev(struct device *dev, unsigned long action)
{
	int err;

	if (!rcu_access_pointer(device_chain.head)) {
		return 0;
	}

	mutex_lock(&dev_ch_mutex);
	err = raw_notifier_call_chain(&device_chain, action, dev);
	dev->dma_ops = NULL;
	mutex_unlock(&dev_ch_mutex);
	err = notifier_to_errno(err);
	return err;
}

#ifdef CONFIG_LIBLINUX_DEVFS_WORKAROUND
static void __liblinux_device_notify(void *data, struct device *dev, unsigned int ev, int *ret)
{
	int err;

	err = __liblinux_device_ev(dev, ev);
	if (err)
		dev_warn(dev, "liblinux device %s notify failed\n", ev == LIBLINUX_VH_DEVICE_ADD ? "add" : "del");
	if (ret)
		*ret = err;
}

static int __init liblinux_device_core_init(void)
{
	return register_trace_ldk_rvh_device_notify(__liblinux_device_notify, NULL);
}
#else
static int __liblinux_notify(struct device *dev)
{
	return __liblinux_device_ev(dev, LIBLINUX_DEVICE_ADD);
}

static int __liblinux_notify_remove(struct device *dev)
{
	return __liblinux_device_ev(dev, LIBLINUX_DEVICE_DEL);
}

static int __init liblinux_device_core_init(void)
{
	if (platform_notify || platform_notify_remove) {
		pr_err("notify platform of device has been registered\n");
		return -EEXIST;
	}

	platform_notify = __liblinux_notify;
	platform_notify_remove = __liblinux_notify_remove;
	return 0;
}
#endif
core_initcall(liblinux_device_core_init);
