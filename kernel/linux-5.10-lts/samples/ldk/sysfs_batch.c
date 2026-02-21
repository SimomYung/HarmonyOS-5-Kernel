// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/mount.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/vmalloc.h>
#include <uapi/linux/mount.h>
#include <linux/init_syscalls.h>
#include <net/net_namespace.h>
#include <linux/list.h>
#include <linux/netdevice.h>
#include <linux/kobject.h>
#include <linux/timekeeping.h>
#include <securec.h>
#include <linux/hash.h>
#include <securec.h>
#include <../../drivers/base/base.h>
#include <trace/hooks/liblinux.h>
#include <linux/netdevice.h>
#include "kernfs.h"
#include "sysfs_batch.h"
#include "procfs_net.h"

bool is_need_create_in_batch(const struct net *ns)
{
	unsigned int cookie = (unsigned int)atomic64_read(&ns->net_cookie);
	return !!(cookie & BATCH_FLAG);
}

static bool dev_can_create_async(void)
{
#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	return liblinux_driver_is_async();
#endif
	return false;
}

/* get sys/class/net node */
static struct kobject *device_get_class_obj(struct device *dev)
{
	if (!dev->class)
		return NULL;

	return &dev->class->p->subsys.kobj;
}

static int kernfs_get_batch_kns(struct kernfs_node *kn, void *ns,
				struct kernfs_node **kns, int *cnt)
{
	const void *kn_ns = NULL;
	int index = *cnt;

	if (index >= SYSFS_CREATE_BATCH_MAX) {
		pr_err("create batch too many, cnt %d\n", index);
		return -EINVAL;
	}

	kn_ns = liblinux_get_ns_info_by_kn(kn);
	if (kn_ns == ns) {
		kns[index++] = kn;
	}

	*cnt = index;
	return 0;
}

static int kernfs_child_node_traverse(struct kernfs_node *kn, void *ns,
				      struct kernfs_node **kns, int *cnt)
{
	int ret = 0;
	struct rb_node *rbn = NULL;
	struct rb_node *tmp_rbn = NULL;
	struct kernfs_node *pos = NULL;
	int tmp_cnt = *cnt;

	if (kernfs_type(kn) != KERNFS_DIR) {
		return ret;
	}

	rbn = rb_first(&kn->dir.children);
	if (!rbn) {
		return ret;
	}
	pos = rb_to_kn(rbn);

	while (pos) {
		if (kernfs_type(pos) == KERNFS_DIR) {
			ret = kernfs_get_batch_kns(pos, ns, kns, &tmp_cnt);
			if (ret == 0) {
				ret = kernfs_child_node_traverse(pos, ns, kns, &tmp_cnt);
			}
		} else {
			ret = kernfs_get_batch_kns(pos, ns, kns, &tmp_cnt);
		}

		if (ret != 0) {
			break;
		}

		tmp_rbn = rb_next(&pos->rb);
		if (!tmp_rbn) {
			pos = NULL;
		} else {
			pos = rb_to_kn(tmp_rbn);
		}
	}

	*cnt = tmp_cnt;
	return ret;
}

/* create sysfs batch */
static void liblinux_sysfs_set_dir_entry(ldk_sysfs_entry_t *entry)
{
	entry->type = KERNFS_DIR;
}

static int liblinux_sysfs_set_file_entry(ldk_sysfs_entry_t *entry, struct kernfs_node *kn)
{
	int err;
	int tmp_id;

	err = liblinux_sysfs_get_file_id(kn, &tmp_id);
	if (err != 0) {
		return err;
	}

	entry->id = tmp_id;
	entry->size = kn->attr.size;
	entry->type = KERNFS_FILE;

	return err;
}

static int liblinux_sysfs_set_symlink_entry(ldk_sysfs_entry_t *entry,
					    struct kernfs_node *kn)
{
	int ret;
	char targetpath[SYSFS_PATH_NAME_MAX] = { 0 };
	struct kernfs_node *target_kn = kn->symlink.target_kn;

	ret = kernfs_get_target_path(kn->parent, target_kn, targetpath);
	if (ret < 0) {
		pr_err("failed to get symlink target path.\n");
		return ret;
	}

	ret = strcpy_s(entry->linkpath, SYSFS_PATH_NAME_MAX, targetpath);
	if (ret != 0) {
		return ret;
	}
	entry->type = KERNFS_LINK;

	return ret;
}

static int liblinux_sysfs_data_create_batch(struct kernfs_node **kns, int batch_cnt)
{
	int ret, i;
	char fast_path[SYSFS_PATH_NAME_MAX] = { 0 };
	char *buf = fast_path;
	char *path = NULL;
	const void *ns = NULL;
	ldk_sysfs_entry_t *ldk_entry = NULL;
	ldk_sysfs_entry_t *tmp_ldk_entry = NULL;
	unsigned int nsid;

	ldk_entry = kvmalloc(batch_cnt * sizeof(ldk_sysfs_entry_t), GFP_KERNEL);
	if (ldk_entry == NULL) {
		return -ENOMEM;
	}

	for (i = 0; i < batch_cnt; i++) {
		/* path process */
		BUG_ON(kns[i] == NULL);
		tmp_ldk_entry = ldk_entry + i;

		ret = kernfs_path(kns[i], buf, SYSFS_PATH_NAME_MAX);
		if (unlikely(ret < 0)) {
			break;
		}
		/* cann't handle path begin with '/' */
		path = buf + 1;

		ns = liblinux_get_ns_info_by_kn(kns[i]);
		nsid = liblinux_get_nsid_by_ns(ns);
		nsid &= ~BATCH_FLAG;
		tmp_ldk_entry->nsid = nsid;
		tmp_ldk_entry->mode = kns[i]->mode;
		ret = strcpy_s(tmp_ldk_entry->path, SYSFS_PATH_NAME_MAX, path);
		if (ret != 0) {
			break;
		}

		switch (kernfs_type(kns[i])) {
			case KERNFS_DIR:
				liblinux_sysfs_set_dir_entry(tmp_ldk_entry);
				break;
			case KERNFS_FILE:
				ret = liblinux_sysfs_set_file_entry(tmp_ldk_entry, kns[i]);
				break;
			case KERNFS_LINK:
				ret = liblinux_sysfs_set_symlink_entry(tmp_ldk_entry, kns[i]);
				break;
			default:
				break;
		}

		if (unlikely(ret < 0)) {
			break;
		}
	}

	if (ret == 0) {
		ret = liblinux_pal_sysfs_create_data_batch((void *)ldk_entry, batch_cnt);
	}

	kvfree(ldk_entry);
	return ret;
}

static void clear_net_batch_flag(struct net *net)
{
	/* init_net is not created in batch */
	if (!net_eq(net, &init_net)) {
		u64 res = atomic64_read(&net->net_cookie);
		res &= ~BATCH_FLAG;
		atomic64_set(&net->net_cookie, res);
	}
}

static void hook_setup_net(void *__data, void *ns, int *ret)
{
	struct net *netns = (struct net *)ns;
	struct kernfs_node **kns = NULL;
	struct kobject *class_kobj = NULL;
	int cnt = 0;

	if (net_eq(netns, &init_net)) {
		return;
	}

	if (netns->loopback_dev == NULL) {
		*ret = -EINVAL;
		return;
	}

	kns = kvmalloc(sizeof(struct kernfs_node *) * SYSFS_CREATE_BATCH_MAX, GFP_KERNEL);
	if (kns == NULL) {
		*ret = -ENOMEM;
		return;
	}

	/* traverse /sys/devices/virtual/net */
	*ret = kernfs_child_node_traverse(netns->loopback_dev->dev.kobj.sd->parent, (void *)netns, kns, &cnt);
	if (*ret != 0) {
		goto out_undo;
	}
	/* traverse /sys/class/net */
	class_kobj = device_get_class_obj(&netns->loopback_dev->dev);
	if (class_kobj == NULL) {
		goto out_undo;
	}
	*ret = kernfs_child_node_traverse(class_kobj->sd, (void *)netns, kns, &cnt);
	if (*ret != 0) {
		goto out_undo;
	}

	*ret = liblinux_sysfs_data_create_batch(kns, cnt);
	if (*ret != 0) {
		pr_err("sysfs create batch failed, err %d\n", *ret);
		goto out_undo;
	}

#ifdef CONFIG_NET_NS
	liblinux_create_procfs_in_batches(netns, ret);
	if (*ret != 0) {
		pr_err("procfs create batch failed, err %d\n", *ret);
		goto out_undo;
	}
#endif

	clear_net_batch_flag(netns);

out_undo:
	kvfree(kns);
}
INIT_VENDOR_HOOK(ldk_vh_setup_net, hook_setup_net);

static struct kernfs_node * find_dev_class_kn(struct kernfs_node *kn, char *devname)
{
	struct rb_node *rbn = NULL;
	struct rb_node *tmp_rbn = NULL;
	struct kernfs_node *pos = NULL;

	if (kernfs_type(kn) != KERNFS_DIR) {
		return NULL;
	}

	rbn = rb_first(&kn->dir.children);
	if (!rbn) {
		return NULL;
	}
	pos = rb_to_kn(rbn);

	while (pos) {
		if ((kernfs_type(pos) == KERNFS_LINK) && (strcmp(pos->name, devname) == 0)) {
			return pos;
		}

		tmp_rbn = rb_next(&pos->rb);
		if (!tmp_rbn) {
			pos = NULL;
		} else {
			pos = rb_to_kn(tmp_rbn);
		}
	}

	return NULL;
}

static void dev_set_batch_flag(struct net_device *dev)
{
	dev->dev.kobj.batch_create = 1;
}

static void dev_clear_batch_flag(struct net_device *dev)
{
	dev->dev.kobj.batch_create = 0;
}

static void hook_pre_register_netdev(void *__data, struct net_device *dev)
{
	struct net *netns = dev_net(dev);

	if (is_need_create_in_batch(netns)) {
		return;
	}

	if ((strcmp(dev->name, "lo") == 0) ||
	    (strcmp(dev->name, "default") == 0) ||
	    (strcmp(dev->name, "all") == 0)) {
		return;
	}

#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	if (!liblinux_sysfs_is_sync()) {
		return;
	}
#endif
	dev_set_batch_flag(dev);
}
INIT_VENDOR_HOOK(ldk_vh_pre_register_netdev, hook_pre_register_netdev);

int liblinux_register_netdev(void *data)
{
	struct net_device *dev = (struct net_device *)data;
	struct net *netns = dev_net(dev);
	struct kernfs_node **kns = NULL;
	struct kernfs_node *class_kn = NULL;
	int ret, cnt = 0;

#ifdef CONFIG_NET_NS
	ret = liblinux_create_netdev_procfs_batch(dev);
	if (ret != 0) {
		pr_err("procfs create batch failed, err %d\n", ret);
		goto out;
	}
#endif

	kns = kvmalloc(sizeof(struct kernfs_node *) * SYSFS_CREATE_BATCH_MAX, GFP_KERNEL);
	if (kns == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	ret = kernfs_get_batch_kns(dev->dev.kobj.sd, (void *)netns, kns, &cnt);
	if (ret != 0) {
		goto out_undo;
	}

	/* traverse /sys/devices/virtual/net */
	ret = kernfs_child_node_traverse(dev->dev.kobj.sd, (void *)netns, kns, &cnt);
	if (ret != 0) {
		goto out_undo;
	}

	/* traverse /sys/class/net */
	class_kn = find_dev_class_kn(dev->dev.class->p->subsys.kobj.sd, dev->name);
	if (class_kn == NULL) {
		ret = -ENOENT;
		goto out_undo;
	}

	ret = kernfs_get_batch_kns(class_kn, (void *)netns, kns, &cnt);
	if (ret != 0) {
		goto out_undo;
	}

	ret = kernfs_child_node_traverse(class_kn, (void *)netns, kns, &cnt);
	if (ret != 0) {
		goto out_undo;
	}

	ret = liblinux_sysfs_data_create_batch(kns, cnt);
	if (ret != 0) {
		pr_err("dev:%s sysfs create batch failed, err %d\n", dev->name, ret);
		goto out_undo;
	}

	dev_clear_batch_flag(dev);

out_undo:
	kvfree(kns);
out:
	dev_put(dev);
	return ret;
}

static void hook_post_register_netdev(void *__data, struct net_device *dev, int *ret)
{
	if (!(dev->dev.kobj.batch_create) || (*ret != 0)) {
               return;
	}

	dev_hold(dev);
	if (!dev_can_create_async()) {
		*ret = liblinux_register_netdev((void *)dev);
		return;
	}

#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	*ret = liblinux_register_netdev_async((void *)dev);
	if (*ret) {
		dev_put(dev);
	}
#endif
}
INIT_VENDOR_HOOK(ldk_vh_post_register_netdev, hook_post_register_netdev);

int liblinux_kobject_add_uevent(void *data)
{
	struct kobject *kobj = (struct kobject *)data;
	int ret = kobject_uevent(kobj, KOBJ_ADD);

	kobject_put(kobj);
	return ret;
}

static void hook_kobject_add_uevent(void *__data, struct device *dev, struct kobject *kobj)
{
	kobject_get(kobj);
#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	if (dev->kobj.batch_create && dev_can_create_async()) {
		int ret = liblinux_kobject_add_uevent_async((void *)kobj);
		if (ret) {
			kobject_put(kobj);
		}
		return;
	}
#endif
	liblinux_kobject_add_uevent((void *)kobj);
}
INIT_VENDOR_HOOK(ldk_vh_kobject_add_uevent, hook_kobject_add_uevent);

static void hook_pre_init_module(void *__data, char *driver)
{
#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	if (strcmp(driver, "modem_driver") == 0) {
		liblinux_driver_set_async(true);
	}
#endif
}
INIT_VENDOR_HOOK(ldk_vh_pre_init_module, hook_pre_init_module);

static void hook_post_init_module(void *__data, char *driver)
{
#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	if (strcmp(driver, "modem_driver") == 0) {
		liblinux_driver_set_async(false);
	}
#endif
}
INIT_VENDOR_HOOK(ldk_vh_post_init_module, hook_post_init_module);
