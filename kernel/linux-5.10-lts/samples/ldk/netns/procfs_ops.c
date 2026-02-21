// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mount.h>
#include <linux/file.h>
#include <linux/nsproxy.h>
#include <linux/cred.h>
#include <linux/netdevice.h>
#include <net/net_namespace.h>
#include <trace/hooks/liblinux.h>

#include "../../../fs/proc/internal.h"
#include "net_procfs.h"
#include "../sysctl.h"
#include "../procfs.h"
#include "securec.h"

#define ROOT_NET_NSID 8189
static unsigned int g_sys_nsid = ROOT_NET_NSID;
static unsigned int g_proc_nsid = ROOT_NET_NSID;

bool sysctl_need_create_batch(void *head, const char *path)
{
	struct ctl_table_header *header = (struct ctl_table_header *)head;
	struct net *netns = container_of(header->set, struct net, sysctls);;
	unsigned int conf_len = sizeof("net/ipv4/conf") - 1;
	unsigned int neigh_len = sizeof("net/ipv4/neigh") - 1;
	struct net_device *dev;
	char *devname;
	bool ret;

	if (strncmp(path, "net/ipv4/conf", conf_len) &&
	    strncmp(path, "net/ipv4/neigh", neigh_len) &&
	    strncmp(path, "net/ipv6/conf", conf_len) &&
	    strncmp(path, "net/ipv6/neigh", neigh_len)) {
		return false;
	}

	devname = strrchr(path, '/');
	dev = dev_get_by_name(netns, devname + 1);
	if (dev == NULL) {
		return false;
	}

	ret = !!(dev->dev.kobj.batch_create);
	dev_put(dev);
	return ret;
}

static void hook_proc_net_register(void *__data, struct net *net, struct proc_dir_entry *netd, int *err)
{
	unsigned int nsid = liblinux_get_nsid_by_ns(net);
	int rc;

	*err = 0;
	netd->parent = netd;
	rc = procfs_create_net_dir(nsid, "net", S_IRUGO | S_IXUGO | S_IFDIR);
	if (rc < 0 && rc != -EEXIST) {
		pr_err("failed to create net dir, err:%d\n", rc);
		*err = rc;
		return;
	}

	rc = procfs_create_net_dir(nsid, "sys/net", S_IRUGO | S_IXUGO | S_IFDIR);
	if (rc < 0 && rc != -EEXIST) {
		(void)procfs_delete_net_data_specific("", nsid);
		pr_err("failed to create sys/net dir, err:%d\n", rc);
		*err = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_proc_net_register, hook_proc_net_register);

static void hook_proc_net_unregister(void *__data, struct net *net)
{
	unsigned int nsid = liblinux_get_nsid_by_ns(net);
	(void)procfs_delete_net_data_specific("", nsid);
	pr_info("net procfs data deleted, nsid %u \n", nsid);
}
INIT_VENDOR_HOOK(ldk_vh_proc_net_unregister, hook_proc_net_unregister);

static int __liblinux_net_proc_sys_read(unsigned long long pos, unsigned long ubuf,
			 unsigned long size, void *ctx,
			 unsigned long *rsize /* out */)
{
	mm_segment_t fs;
	int ret;

	fs = get_fs();
	set_fs(KERNEL_DS);
	ret = liblinux_proc_sys_read(pos, ubuf, size, ctx, rsize);
	set_fs(fs);

	return ret;
}

static int __liblinux_net_proc_sys_write(unsigned long long pos, unsigned long ubuf,
			  unsigned long size, void *ctx,
			  unsigned long *wsize /* out */)
{
	mm_segment_t fs;
	int ret;

	fs = get_fs();
	set_fs(KERNEL_DS);
	ret = liblinux_proc_sys_write(pos, ubuf, size, ctx, wsize);
	set_fs(fs);

	return ret;
}

static int __liblinux_net_proc_sys_enter(void)
{
	struct net *net = NULL;

	trace_ldk_vh_devhost_net_socket_get_netns(&net);
	if (net == NULL) {
		return -ENOENT;
	}
	current->nsproxy->net_ns = net;

	return 0;
}

static void __liblinux_net_proc_sys_exit(void)
{
	struct task_struct *tsk = current;
	if (tsk->nsproxy->net_ns) {
		put_net(tsk->nsproxy->net_ns);
	}
	tsk->nsproxy->net_ns = &init_net;
	tsk->cred = &init_cred;
}

static struct liblinux_pal_kernfs_ops liblinux_net_proc_sys_ops = {
	.read = __liblinux_net_proc_sys_read,
	.write = __liblinux_net_proc_sys_write,
	.enter = __liblinux_net_proc_sys_enter,
	.exit = __liblinux_net_proc_sys_exit,
};

static unsigned int get_nsid_by_head(struct ctl_table_header *head)
{
	struct net *net = container_of(head->set, struct net, sysctls);
	return liblinux_get_nsid_by_ns(net);
}

static bool get_nsid_need_create_by_head(struct ctl_table_header *head)
{
	struct net *net = container_of(head->set, struct net, sysctls);
	unsigned int cookie = (unsigned int)atomic64_read(&net->net_cookie);

	return !(cookie & BATCH_FLAG);
}

static int liblinux_proc_sys_net_create_file(const char *path,
				    struct liblinux_pal_kernfs_ops *ops,
				    void *ctx, unsigned int mode)
{
	return procfs_create_net_file(g_sys_nsid, path, ops, ctx, (unsigned int)mode);
}

static int liblinux_proc_sys_net_create_dir(const char *path, unsigned int mode)
{
	return procfs_create_net_dir(g_sys_nsid, path, (unsigned int)mode);
}

static struct liblinux_procfs_create_ops liblinux_proc_sys_create_ops_net = {
	.create_file = liblinux_proc_sys_net_create_file,
	.create_dir = liblinux_proc_sys_net_create_dir,
};

static void hook_register_net_sysctl_table(void *__data, struct ctl_table_header *head,
				struct liblinux_procfs_create_ops **create_ops,
				struct liblinux_pal_kernfs_ops **ops,  bool *need_create)
{
	g_sys_nsid = get_nsid_by_head(head);
	*need_create = get_nsid_need_create_by_head(head);
	*create_ops = &liblinux_proc_sys_create_ops_net;
	*ops = &liblinux_net_proc_sys_ops;
}
INIT_VENDOR_HOOK(ldk_vh_register_net_sysctl_table, hook_register_net_sysctl_table);

static int __liblinux_net_procfs_read(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *ctx,
				  unsigned long *rsize /* out */)
{
	mm_segment_t fs;
	int ret;

	fs = get_fs();
	set_fs(KERNEL_DS);
	ret = liblinux_procfs_read(pos, ubuf, size, ctx, rsize);
	set_fs(fs);

	return ret;
}

static int __liblinux_net_procfs_write(unsigned long long pos, unsigned long ubuf,
				   unsigned long size, void *ctx,
				   unsigned long *wsize /* out */)
{
	mm_segment_t fs;
	int ret;

	fs = get_fs();
	set_fs(KERNEL_DS);
	ret = liblinux_procfs_write(pos, ubuf, size, ctx, wsize);
	set_fs(fs);

	return ret;
}

static void __liblinux_net_procfs_unlink(void *ctx)
{
	struct inode *inode = (struct inode *)ctx;

	if (inode == NULL) {
		return;
	}

	iput(inode);
}

static struct liblinux_pal_kernfs_ops liblinux_net_procfs_ops = {
	.read = __liblinux_net_procfs_read,
	.write = __liblinux_net_procfs_write,
	.enter = __liblinux_net_proc_sys_enter,
	.exit = __liblinux_net_proc_sys_exit,
	.unlink = __liblinux_net_procfs_unlink,
};

static unsigned int get_nsid_by_entry(struct proc_dir_entry *head_dp)
{
	struct net *net = (struct net *)(head_dp->data);
	return liblinux_get_nsid_by_ns(net);
}

static bool get_nsid_need_create_by_entry(struct proc_dir_entry *head_dp)
{
	struct net *net = (struct net *)(head_dp->data);
	unsigned int cookie = (unsigned int)atomic64_read(&net->net_cookie);

	return !(cookie & BATCH_FLAG);
}

static int procfs_net_mk_path(char **ret_path, const char *path)
{
	int ret;

	char *net_pathname = kzalloc(PATH_MAX, GFP_KERNEL);
	if (net_pathname == NULL) {
		pr_err("liblinux: proc kzalloc buf failed\n");
		return -ENOMEM;
	}
	ret = snprintf_s(net_pathname, PATH_MAX, PATH_MAX - 1, "net/%s", path);
	if (ret < 0) {
		kfree(net_pathname);
		return ret;
	}

	*ret_path = net_pathname;
	return 0;
}

static int liblinux_proc_net_create_file(const char *path,
				    struct liblinux_pal_kernfs_ops *ops,
				    void *ctx, unsigned int mode)
{
	int ret;
	char *net_pathname = NULL;

	ret = procfs_net_mk_path(&net_pathname, path);
	if (ret < 0) {
		return ret;
	}

	ret = procfs_create_net_file(g_proc_nsid, net_pathname, ops, ctx, mode);
	kfree(net_pathname);
	return ret;
}

static int liblinux_proc_net_create_dir(const char *path, unsigned int mode)
{
	int ret;
	char *net_pathname = NULL;

	ret = procfs_net_mk_path(&net_pathname, path);
	if (ret < 0) {
		return ret;
	}

	ret = procfs_create_net_dir(g_proc_nsid, net_pathname, mode);
	kfree(net_pathname);
	return ret;
}

static struct liblinux_procfs_create_ops liblinux_proc_create_ops_net = {
	.create_file = liblinux_proc_net_create_file,
	.create_dir = liblinux_proc_net_create_dir,
};

static void hook_liblinux_proc_create_net_data(void *__data, struct proc_dir_entry *head_dp,
				struct liblinux_procfs_create_ops **create_ops,
				struct liblinux_pal_kernfs_ops **ops, bool *need_create)
{
	g_proc_nsid = get_nsid_by_entry(head_dp);
	*need_create = get_nsid_need_create_by_entry(head_dp);
	*create_ops = &liblinux_proc_create_ops_net;
	*ops = &liblinux_net_procfs_ops;
}
INIT_VENDOR_HOOK(ldk_vh_liblinux_proc_create_net_data, hook_liblinux_proc_create_net_data);

static int liblinux_proc_net_create_entries(const char *parent,
			struct liblinux_pal_kernfs_ops *ops,
			void *ctx, unsigned int size)
{
	return procfs_create_net_entries((void *)parent, size, ops);
}

static struct liblinux_procfs_create_ops liblinux_proc_create_ops_entries = {
	.create_file = liblinux_proc_net_create_entries,
};

void liblinux_proc_create_procnet_entries_ops(struct liblinux_procfs_create_ops **create_ops,
						struct liblinux_pal_kernfs_ops **ops)
{
        *create_ops = &liblinux_proc_create_ops_entries;
        *ops = &liblinux_net_procfs_ops;
}

void liblinux_proc_create_sysnet_entries_ops(struct liblinux_procfs_create_ops **create_ops,
						struct liblinux_pal_kernfs_ops **ops)
{
        *create_ops = &liblinux_proc_create_ops_entries;
        *ops = &liblinux_net_proc_sys_ops;
}
