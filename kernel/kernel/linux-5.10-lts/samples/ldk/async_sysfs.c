// SPDX-License-Identifier: GPL-2.0
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/smp.h>
#include <linux/kobject.h>
#include <linux/rwsem.h>
#include <linux/llist.h>

#include <liblinux/sysfs.h>

#include "kernfs.h"
#include "sysfs_batch.h"
enum sysfs_op_type {
	SYS_OP_CREATE_FILE = 0,
	SYS_OP_CREATE_DIR,
	SYS_OP_CREATE_LINK,
	SYS_OP_DELETE,
	SYS_OP_NOTIFY,
	SYS_OP_RENAME,
	SYS_OP_REGISTER_DEV,
	SYS_OP_KOBJECT_UEVENT,
};

struct sysfs_work {
	struct llist_node lnode;
	struct work_struct work;
	int op;
	mode_t mode;
	unsigned long long size;
	char *name;

	/* file create*/
	struct liblinux_pal_kernfs_ops *fops;
	void *ctx;

	/* create link */
	char *linkdata;

	/* rename */
	char *old_name;
	char *new_name;

	/* for namespace isolation */
	unsigned int nsid;
	unsigned int new_nsid; /* for rename ops */

	/* for netdev */
	void *data;
};

static DECLARE_RWSEM(sysfs_lock);
static bool async_delay = true;
static bool driver_async = false;
static LLIST_HEAD(delayed_list);
static struct workqueue_struct *sysfs_wq;
static void sysfs_work(struct work_struct *work);

extern void liblinux_get_cpus_allowed(struct cpumask *allowed_mask,
				      const struct cpumask *new_mask);

bool liblinux_sysfs_is_sync(void)
{
	void *ret = NULL;

	down_read(&sysfs_lock);
	ret = sysfs_wq;
	up_read(&sysfs_lock);

	if (ret != NULL)
		return false;
	return true;
}

bool liblinux_sysfs_is_selinux(const char *name)
{
	return !strcmp(name, "fs/selinux");
}

bool liblinux_driver_is_async(void)
{
	bool ret = false;

	down_read(&sysfs_lock);
	ret = driver_async;
	up_read(&sysfs_lock);

	return ret;
}

void liblinux_driver_set_async(bool flag)
{
	down_write(&sysfs_lock);
	driver_async = flag;
	up_write(&sysfs_lock);
}

/*
 * Try not to choose default_affinity_cpus and boot_cpu to avoid competing with
 * other kworkers or the main thread for cpu resources.
 */
static int get_sysfs_cpu(void)
{
	static int sysfs_cpu = -1U;
	struct cpumask non_boot_mask;
	struct cpumask default_affinity_mask;
	struct cpumask mask;
	int boot_cpu;
	int ret;

	if (sysfs_cpu != -1U)
		return sysfs_cpu;

	boot_cpu = get_boot_cpu_id();
	cpumask_copy(&non_boot_mask, cpu_online_mask);
	cpumask_clear_cpu(boot_cpu, &non_boot_mask);
	/* Fallback to boot_cpu if non_boot_mask is empty */
	if (cpumask_empty(&non_boot_mask)) {
		sysfs_cpu = boot_cpu;
		return sysfs_cpu;
	}

	liblinux_get_cpus_allowed(&default_affinity_mask, &non_boot_mask);
	ret = cpumask_andnot(&mask, &non_boot_mask, &default_affinity_mask);
	/* Fallback to default_affinity_cpus if mask is empty */
	if (ret == 0)
		sysfs_cpu = cpumask_any(&non_boot_mask);
	else
		sysfs_cpu = cpumask_any(&mask);

	return sysfs_cpu;
}

static struct sysfs_work *alloc_work(int op)
{
	struct sysfs_work *w;

	w = kzalloc(sizeof(*w), GFP_KERNEL);
	if (w == NULL) {
		 return ERR_PTR(-ENOMEM);
	}

	INIT_WORK(&w->work, sysfs_work);
	w->op = op;
	return w;
}

static void free_work(struct sysfs_work *w)
{
	if (w) {
		 kfree(w->name);
		 kfree(w->new_name);
		 kfree(w->old_name);
		 kfree(w->linkdata);
		 kfree(w);
	}
}

static int cnt_worker = 0, cnt_delay = 0;
static void do_work(struct sysfs_work *w)
{
	down_read(&sysfs_lock);
	if (sysfs_wq) {
		bool ret = true;

		cnt_worker++;
		if (async_delay) {
			llist_add(&w->lnode, &delayed_list);
			cnt_delay++;
		}
		else
			ret = queue_work_on(get_sysfs_cpu(), sysfs_wq, &w->work);
		if (!ret)
			panic("file name=%s, old_name=%s do op %d ,cpu=%d, ret=%d\n", w->name, w->new_name, w->op, get_sysfs_cpu(), ret);
	} else {
		sysfs_work(&w->work);
	}
	up_read(&sysfs_lock);
}

static void sysfs_work(struct work_struct *work)
{
	int rc;
	struct sysfs_work *w = container_of(work, struct sysfs_work, work);

	switch (w->op) {
	case SYS_OP_CREATE_DIR:
		 rc = liblinux_pal_sysfs_create_dir_ns(w->name, w->mode, w->nsid);
		 break;
	case SYS_OP_CREATE_FILE:
		 rc = liblinux_pal_sysfs_create_file_ex(w->name, w->mode, w->size, w->fops, w->ctx, w->nsid);
		 break;
	case SYS_OP_CREATE_LINK:
		 rc = liblinux_pal_sysfs_create_link_ns(w->name, w->mode, w->linkdata, w->nsid);
		 break;
	case SYS_OP_DELETE:
		 rc = liblinux_pal_sysfs_delete_ns(w->name, w->nsid);
		 break;
	case SYS_OP_NOTIFY:
		 rc = liblinux_pal_sysfs_notify(w->ctx, w->name);
		 break;
	case SYS_OP_RENAME:
		 rc = liblinux_pal_sysfs_rename_link_ns(w->old_name, w->new_name, w->nsid, w->new_nsid);
		 break;
	case SYS_OP_REGISTER_DEV:
		 rc = liblinux_register_netdev(w->data);
		 break;
	case SYS_OP_KOBJECT_UEVENT:
		 rc = liblinux_kobject_add_uevent(w->data);
		 break;
	default:
		 panic("unsupported op %d\n", w->op);
	}

	if (rc < 0)
		 pr_err("  file name=%s, old_name=%s do op %d failed %d\n", w->name, w->new_name, w->op, rc);

	free_work(w);
}

#ifdef CONFIG_HIBERNATION
static bool liblinux_sysfs_in_whitelist(const char *name)
{
	if (!strstr(name, "devices/platform"))
		return false;

	if (!strcmp(name, "devices/platform") ||
		!strcmp(name, "devices/platform/b0000000.hi_pcie") ||
		!strcmp(name, "devices/platform/b0000000.hi_pcie/pci0000:00") ||
		!strcmp(name, "devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0") ||
		!strcmp(name, "devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0/0000:01:00.0"))
		return true;

	return false;
}
#endif

int liblinux_sysfs_create_dir(const char *name, unsigned int mode, unsigned int nsid)
{
	struct sysfs_work *w;

#ifdef CONFIG_HIBERNATION
	if (liblinux_sysfs_is_sync() || liblinux_sysfs_in_whitelist(name) ||
		liblinux_sysfs_is_selinux(name)) {
#else
	if (liblinux_sysfs_is_sync() || liblinux_sysfs_is_selinux(name)) {
#endif
		return liblinux_pal_sysfs_create_dir_ns(name, mode, nsid);
	}

	w = alloc_work(SYS_OP_CREATE_DIR);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->name = kstrdup(name, GFP_KERNEL);
	if (w->name == NULL)
		goto failed;

	w->mode = mode;
	w->nsid = nsid;
	do_work(w);

	return 0;
failed:
	free_work(w);
	return -ENOMEM;
}

int liblinux_sysfs_create_file(const char *name, unsigned int mode, unsigned long long size,
				struct liblinux_pal_kernfs_ops *fops,
				void *ctx, unsigned int nsid)
{
	struct sysfs_work *w;

	if (liblinux_sysfs_is_sync()) {
		return liblinux_pal_sysfs_create_file_ex(name, mode, size, fops, ctx, nsid);
	}

	w = alloc_work(SYS_OP_CREATE_FILE);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->name = kstrdup(name, GFP_KERNEL);
	if (w->name == NULL)
		goto failed;

	w->fops = fops;
	w->ctx = ctx;
	w->mode = mode;
	w->size = size;
	w->nsid = nsid;
	do_work(w);

	return 0;
failed:
	free_work(w);
	return -ENOMEM;
}

int liblinux_sysfs_create_link(const char *name, unsigned int mode,
				const char *linkdata, unsigned int nsid)
{
	struct sysfs_work *w;

	if (liblinux_sysfs_is_sync()) {
		return liblinux_pal_sysfs_create_link_ns(name, mode, linkdata, nsid);
	}

	w = alloc_work(SYS_OP_CREATE_LINK);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->name = kstrdup(name, GFP_KERNEL);
	if (w->name == NULL)
		goto failed;

	w->linkdata = kstrdup(linkdata, GFP_KERNEL);
	if (w->linkdata == NULL)
		goto failed;

	w->mode = mode;
	w->nsid = nsid;
	do_work(w);

	return 0;
failed:
	free_work(w);
	return -ENOMEM;
}

int liblinux_sysfs_delete(const char *name, unsigned int nsid)
{
	struct sysfs_work *w;

	if (liblinux_sysfs_is_sync()) {
		return liblinux_pal_sysfs_delete_ns(name, nsid);
	}

	w = alloc_work(SYS_OP_DELETE);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->nsid = nsid;
	w->name = kstrdup(name, GFP_KERNEL);
	if (w->name == NULL)
		goto failed;

	do_work(w);
	return 0;
failed:
	free_work(w);
	return -ENOMEM;
}

int liblinux_sysfs_notify(void *ctx, const char *name)
{
	struct sysfs_work *w;

	if (liblinux_sysfs_is_sync()) {
		return liblinux_pal_sysfs_notify(ctx, name);
	}

	w = alloc_work(SYS_OP_NOTIFY);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->name = kstrdup(name, GFP_KERNEL);
	if (w->name == NULL)
		goto failed;


	w->ctx = ctx;
	do_work(w);

	return 0;
failed:
	free_work(w);
	return -ENOMEM;
}
int liblinux_sysfs_rename_link(const char *old_name, const char *new_name,
							   unsigned int old_nsid, unsigned int new_nsid)
{
	struct sysfs_work *w;

	if (liblinux_sysfs_is_sync()) {
		return liblinux_pal_sysfs_rename_link_ns(old_name, new_name, old_nsid, new_nsid);
	}

	w = alloc_work(SYS_OP_RENAME);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->old_name = kstrdup(old_name, GFP_KERNEL);
	if (w->old_name == NULL)
		goto failed;

	w->new_name = kstrdup(new_name, GFP_KERNEL);
	if (w->new_name == NULL)
		goto failed;

	w->nsid = old_nsid;
	w->new_nsid = new_nsid;
	do_work(w);

	return 0;
failed:
	free_work(w);
	return -ENOMEM;
}

static int register_netdev_work(void *data, enum sysfs_op_type type)
{
	struct sysfs_work *w;

	w = alloc_work(type);
	if (IS_ERR(w)) {
		return PTR_ERR(w);
	}

	w->data = data;
	schedule_work(&w->work);

	return 0;
}

int liblinux_kobject_add_uevent_async(void *data)
{
	return register_netdev_work(data, SYS_OP_KOBJECT_UEVENT);
}

int liblinux_register_netdev_async(void *data)
{
	return register_netdev_work(data, SYS_OP_REGISTER_DEV);
}

/* should be called with sysfs_lock write lock help */
static void flush_delayed_sysfs(void)
{
	struct sysfs_work *w, *t;
	struct llist_node *head;
	bool ret = true;

	async_delay = false;
	head = llist_del_all(&delayed_list);
	if (head) {
		head = llist_reverse_order(head);
		llist_for_each_entry_safe(w, t, head, lnode) {
			ret = queue_work_on(get_sysfs_cpu(), sysfs_wq, &w->work);
			if (!ret)
				panic("[%s:%d]  file name=%s, old_name=%s do op %d ,cpu=%d, ret=%d\n", __func__, __LINE__,  w->name, w->new_name, w->op, get_sysfs_cpu(), ret);
		}
	}
}

void flush_sysfs_wq_no_destroy(void)
{
	down_write(&sysfs_lock);
	if (sysfs_wq) {
		pr_info("flush async sysfs, total cnt=%d, delay=%d\n", cnt_worker, cnt_delay);
		flush_delayed_sysfs();
		flush_workqueue(sysfs_wq);
	}
	up_write(&sysfs_lock);
}
EXPORT_SYMBOL_GPL(flush_sysfs_wq_no_destroy);

void flush_sysfs_wq(void)
{
	down_write(&sysfs_lock);
	if (sysfs_wq) {
		pr_info("flush async sysfs, total cnt=%d, delay=%d\n", cnt_worker, cnt_delay);
		flush_delayed_sysfs();
		flush_workqueue(sysfs_wq);

		destroy_workqueue(sysfs_wq);
		sysfs_wq = NULL;
		pr_info("async sysfs flushed \n");
	}
	up_write(&sysfs_lock);
}

#define SYSFS_ATTR_WO(_name) \
	struct kobj_attribute _name##_attr = __ATTR_WO(_name)

static ssize_t flush_store(struct kobject *kobj, struct kobj_attribute *attr,
			   const char *buf, size_t count)
{
	flush_sysfs_wq();
	return count;
}
SYSFS_ATTR_WO(flush);

static struct attribute * sysfs_attrs[] = {
	&flush_attr.attr,
	NULL
};

static const struct attribute_group sysfs_attr_group = {
	.attrs = sysfs_attrs,
};

static void liblinux_sysfs_async_init(void)
{
	int ret;
	struct kobject *sysfs_obj;

	sysfs_obj = kobject_create_and_add("sysfs", fs_kobj);
	if (!sysfs_obj) {
		return;
	}

	ret = sysfs_create_group(sysfs_obj, &sysfs_attr_group);
	if (ret < 0) {
		kobject_put(sysfs_obj);
		return;
	}

	if (num_online_cpus() > 1U) {
		down_write(&sysfs_lock);
		sysfs_wq = create_singlethread_workqueue("async_sysfs");
		up_write(&sysfs_lock);
	}
}

void smp_cpus_done(unsigned int max_cpus)
{
	liblinux_sysfs_async_init();
}

static int __init liblinux_sysfs_async_start(void)
{
	down_write(&sysfs_lock);
	if (sysfs_wq)
		flush_delayed_sysfs();
	up_write(&sysfs_lock);
	return 0;
}
pure_initcall(liblinux_sysfs_async_start);
