/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interfaces for implement procfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 14 14:24:43 2024
 */
#include <linux/errno.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/init_syscalls.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/mutex.h>
#include <linux/pid_namespace.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <net/net_namespace.h>
#include <securec.h>
#include <trace/hooks/liblinux.h>
#include <uapi/linux/mount.h>
#include <linux/netdevice.h>
#include "../../fs/proc/internal.h"
#include "file.h"
#include "netns/net_procfs.h"
#include "procfs_net.h"
#include "sysctl.h"
#include "procfs.h"
#define ROOT_NET_NSID 8189
#define PROC_TOTAL_SIZE(num) \
	(sizeof(struct proc_path_list) + num * sizeof(struct procfs_entry_node))

static int proc_net_traverse(struct proc_dir_entry *dir, struct proc_path_list *entries,
				char *name_table, struct vfsmount *proc_mnt);
static int sysctl_net_traverse(struct ctl_dir *dir, struct proc_path_list *entries,
				char *name_table);

static unsigned int proc_get_nsid_by_entry(struct proc_dir_entry *head_dp)
{
	struct net *net = (struct net *)(head_dp->data);
	unsigned int cookie = (unsigned int)atomic64_read(&net->net_cookie);
	return cookie & NET_NSID_MASK;
}

static int proc_entries_expansion(struct proc_path_list **entries)
{
	unsigned int max_num = INT_MAX / sizeof(struct procfs_entry_node) - 1;
	unsigned int realloc_num = (*entries)->max_len * 2;
	struct proc_path_list *new_path = NULL;
	unsigned int origin_size = PROC_TOTAL_SIZE((*entries)->len);

	if (realloc_num > max_num)
		return -EINVAL;

	new_path = vmalloc(PROC_TOTAL_SIZE(realloc_num));
	if (new_path == NULL)
		return -ENOMEM;

	new_path->nsid = (*entries)->nsid;
	new_path->max_len = realloc_num;
	new_path->len = 0;
	vfree(*entries);
	*entries = new_path;

	return 0;
}

static void release_proc_inode(struct proc_path_list *entries)
{
	for (int idx = 0; idx < entries->len; idx++) {
		if(S_ISREG(entries->path[idx].mode)) {
			struct inode *inode = (struct inode *)entries->path[idx].ctx;
			if (inode != NULL)
				iput(inode);
		}
	}
}

static void get_parent_dir(char** str, char ch)
{
	char* found = strrchr(*str, ch);
	if (found) {
		*found = '\0';
	} else {
		**str = '\0';
	}
}

static int proc_net_dir_init(struct proc_dir_entry *dir, struct proc_path_list *entries,
				char *name_table, struct vfsmount *proc_mnt)
{
	struct procfs_entry_node *path = &entries->path[entries->len];
	int rc;

	rc = strcat_s(name_table, PROCFS_ENTRY_MAX_NUM, "/");
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = strcat_s(name_table, PROCFS_ENTRY_MAX_NUM, dir->name);
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = memcpy_s(path->path, PROCFS_PATH_NAME_MAX, name_table, strlen(name_table) + 1);
	if (rc != EOK) {
		return -EINVAL;
	}
	path->mode = dir->mode;
	entries->len++;

	return proc_net_traverse(dir, entries, name_table, proc_mnt);
}

static int proc_net_file_init(struct proc_dir_entry *dir, struct proc_path_list *entries,
				char *name_table, struct vfsmount *proc_mnt)
{
	struct procfs_entry_node *path = &entries->path[entries->len];
	struct inode *inode = NULL;
	int rc;

	pde_get(dir);
	inode = proc_get_inode(proc_mnt->mnt_sb, dir);
	if (inode == NULL) {
		pr_err("failed to get inode of %s\n", dir->name);
		return -ENOMEM;
	}

	path->ctx = (void *)inode;
	path->mode = dir->mode;

	rc = memcpy_s(path->path, PROCFS_PATH_NAME_MAX, name_table, strlen(name_table) + 1);
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = strcat_s(path->path, PROCFS_PATH_NAME_MAX, "/");
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = strcat_s(path->path, PROCFS_PATH_NAME_MAX, dir->name);
	if (rc != EOK) {
		return -EINVAL;
	}
	entries->len++;

	return 0;
}

static int proc_net_traverse(struct proc_dir_entry *dir, struct proc_path_list *entries,
				char *name_table, struct vfsmount *proc_mnt)
{
	struct proc_dir_entry *subdir = NULL;
	struct rb_node *node = NULL;
	int err = 0;

	if (dir == NULL)
		return 0;

	node = rb_first(&dir->subdir);
	while (node != NULL) {
		if (entries->len >= entries->max_len) {
			return -E2BIG;
		}

		subdir = rb_entry(node, struct proc_dir_entry, subdir_node);
		if (S_ISDIR(subdir->mode))
			err = proc_net_dir_init(subdir, entries, name_table, proc_mnt);
		else
			err = proc_net_file_init(subdir, entries, name_table, proc_mnt);

		if (err < 0)
			return err;

		node = rb_next(node);
	}

	/* Return to the upper-level directory. */
	get_parent_dir(&name_table, '/');

	return 0;
}

static int liblinux_procfs_create_proc_net(struct proc_dir_entry *dp,
						unsigned int nsid)
{
	struct liblinux_procfs_create_ops *create_ops = NULL;
	char name_table[PROCFS_ENTRY_MAX_NUM] = "net";
	struct liblinux_pal_kernfs_ops *ops = NULL;
	struct proc_path_list *entries;
	struct vfsmount *proc_mnt;
	unsigned int size;
	int ret = 0;

	if (dp == NULL)
		return -EINVAL;

	entries = vmalloc(PROC_TOTAL_SIZE(PROCFS_ENTRY_MAX_NUM));
	if (entries == NULL) {
		pr_err("netns: create entries alloc failed\n");
		return -ENOMEM;
	}

	proc_mnt = proc_mnt_get_locked();
	if (proc_mnt == NULL) {
		ret = -ENOMEM;
		goto err_free;
	}

	entries->nsid = nsid;
	entries->max_len = PROCFS_ENTRY_MAX_NUM;
	ret = proc_net_traverse(dp, entries, name_table, proc_mnt);
	if (ret == -E2BIG) {
		ret = proc_entries_expansion(&entries);
		if (ret == 0)
			ret = proc_net_traverse(dp, entries, name_table, proc_mnt);
		else
			pr_warn("procfs fail to expansion list, len:%u", entries->len);
	}
	if (ret < 0)
		goto err_release;

	liblinux_proc_create_procnet_entries_ops(&create_ops, &ops);
	size = PROC_TOTAL_SIZE(entries->len);
	ret = create_ops->create_file((void *)entries, ops, NULL, size);
	if ((ret < 0) && (ret != -EEXIST)) {
		pr_err("netns: create procfs failed");
		goto err_release;
	} else {
		ret = 0;
		goto err_unlock;
	}


err_release:
	release_proc_inode(entries);
err_unlock:
	proc_mnt_unlock();
err_free:
	vfree(entries);

	return ret;
}

static int sysctl_net_dir_init(struct ctl_table_header *header, struct ctl_table *table,
				struct proc_path_list *entries, char *name_table)
{
	struct ctl_dir *subdir = container_of(header, struct ctl_dir, header);
	struct procfs_entry_node *path = &entries->path[entries->len];
	int rc;

	path->mode = table->mode;
	rc = strcat_s(name_table, PROCFS_ENTRY_MAX_NUM, "/");
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = strcat_s(name_table, PROCFS_ENTRY_MAX_NUM, table->procname);
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = memcpy_s(path->path, PROCFS_PATH_NAME_MAX, name_table, strlen(name_table) + 1);
	if (rc != EOK) {
		return -EINVAL;
	}

	entries->len++;

	return sysctl_net_traverse(subdir, entries, name_table);
}

static int sysctl_net_file_init(struct rb_node *node, struct ctl_table *table,
		struct proc_path_list *entries, char *name_table)
{
	struct procfs_entry_node *path = &entries->path[entries->len];
	int rc;

	rc = memcpy_s(path->path, PROCFS_PATH_NAME_MAX, name_table, strlen(name_table) + 1);
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = strcat_s(path->path, PROCFS_PATH_NAME_MAX, "/");
	if (rc != EOK) {
		return -EINVAL;
	}

	rc = strcat_s(path->path, PROCFS_PATH_NAME_MAX, table->procname);
	if (rc != EOK) {
		return -EINVAL;
	}
	path->mode = (umode_t)S_IFREG | table->mode;
	path->ctx = (void *)node;
	entries->len++;

	return 0;
}

static int sysctl_net_traverse(struct ctl_dir *dir, struct proc_path_list *entries,
					char *name_table)
{
	struct rb_node *node;
	int err;

	if (dir == NULL)
		return 0;

	node = rb_first(&dir->root);
	while (node != NULL) {
		struct ctl_table_header *header;
		struct ctl_node *ctl_node;
		struct ctl_table *table;

		if (entries->len >= entries->max_len) {
			return -E2BIG;
		}

		ctl_node = rb_entry(node, struct ctl_node, node);
		header = ctl_node->header;
		table = &header->ctl_table[ctl_node - header->node];
		if (S_ISDIR(table->mode)) {
			/* read dir */
			err = sysctl_net_dir_init(header, table, entries, name_table);
			if (err < 0)
				return err;
		} else {
			err = sysctl_net_file_init(node, table, entries, name_table);
			if (err < 0) {
				return err;
			}
		}

		node = rb_next(node);
	}

	get_parent_dir(&name_table, '/');

	return 0;
}

static int liblinux_procfs_create_proc_sysnet(struct ctl_dir *dir,
					unsigned int nsid)
{
	struct liblinux_procfs_create_ops *create_ops = NULL;
	struct liblinux_pal_kernfs_ops *ops = NULL;
	char name_table[PROCFS_ENTRY_MAX_NUM] = "sys";
	struct proc_path_list *entries;
	unsigned int size;
	int ret = 0;

	if (dir == NULL)
		return -EINVAL;

	entries = vmalloc(PROC_TOTAL_SIZE(PROCFS_SYS_MAX_NUM));
	if (entries == NULL)
		return -ENOMEM;

	liblinux_proc_create_sysnet_entries_ops(&create_ops, &ops);
	entries->nsid = nsid;
	entries->max_len = PROCFS_SYS_MAX_NUM;
	ret = sysctl_net_traverse(dir, entries, name_table);
	if (ret == -E2BIG) {
		ret = proc_entries_expansion(&entries);
		if (ret == 0)
			ret = sysctl_net_traverse(dir, entries, name_table);
		else
			pr_warn("proc/sys fail to expansion list, len:%u", entries->len);
	}
	if (ret < 0)
		goto err;

	size = PROC_TOTAL_SIZE(entries->len);
	ret = create_ops->create_file((void *)entries, ops, NULL, size);
	if ((ret < 0) && (ret != -EEXIST)) {
		pr_err("netns: create procfs sys/net failed");
		goto err;
	}

	ret = 0;

err:
	vfree(entries);

	return ret;
}

void liblinux_create_procfs_in_batches(struct net *net, int *ret)
{
	unsigned int nsid;

	if (net == NULL)
		return;

	nsid = proc_get_nsid_by_entry(net->proc_net);
	if (nsid == ROOT_NET_NSID)
		return;

	*ret = liblinux_procfs_create_proc_net(net->proc_net, nsid);
	if (*ret < 0) {
		pr_err("create proc/net failed: ret:%d\n", *ret);
		return;
	}

	*ret = liblinux_procfs_create_proc_net(net->proc_net_stat, nsid);
	if (*ret < 0) {
		pr_err("create proc/net/stat failed, ret:%d\n",*ret);
		return;
	}

	*ret = liblinux_procfs_create_proc_sysnet(&net->sysctls.dir, nsid);
	if (*ret < 0) {
		pr_err("create proc/sys/net failed, ret:%d\n", *ret);
		return;
	}
}

static int name_cmp(const char *name1, int len1, const char *name2, int len2)
{
	int minlen;
	int cmp;

	minlen = len1;
	if (minlen > len2)
		minlen = len2;

	cmp = memcmp(name1, name2, minlen);
	if (cmp == 0)
		cmp = len1 - len2;

	return cmp;
}

static struct ctl_dir *find_subdir(struct ctl_dir *dir, const char *name,
				int namelen)
{
	struct ctl_table *entry, *ret_entry = NULL;
	struct rb_node *node = dir->root.rb_node;
	struct ctl_table_header *head;

	while (node)
	{
		struct ctl_node *ctl_node;
		const char *procname;
		int cmp;

		ctl_node = rb_entry(node, struct ctl_node, node);
		head = ctl_node->header;
		entry = &head->ctl_table[ctl_node - head->node];
		procname = entry->procname;

		cmp = name_cmp(name, namelen, procname, strlen(procname));
		if (cmp < 0)
			node = node->rb_left;
		else if (cmp > 0)
			node = node->rb_right;
		else {
			ret_entry = entry;
			break;
		}
	}

	if (!ret_entry)
		return ERR_PTR(-ENOENT);

	if (!S_ISDIR(ret_entry->mode))
		return ERR_PTR(-ENOTDIR);

	return container_of(head, struct ctl_dir, header);
}

static struct ctl_dir *get_dev_dir(struct ctl_dir *proc_dir,
					const char *path)
{
	const char *name, *nextname;
	struct ctl_dir *dir = proc_dir;

	for (name = path; name; name = nextname) {
		int namelen;
		nextname = strchr(name, '/');
		if (nextname) {
			namelen = nextname - name;
			nextname++;
		} else {
			namelen = strlen(name);
		}

		if (namelen == 0)
			continue;

		dir = find_subdir(dir, name, namelen);
		if (IS_ERR(dir))
			return NULL;
	}

	return dir;
}

static int netdev_sysctl_traverse(struct net_device *dev, const char *flag,
				struct proc_path_list *entries)
{
	char *path_name, *name_table;
	struct net *net = dev_net(dev);
	struct ctl_table_header *header;
	struct ctl_table *table;
	struct ctl_dir *dir;
	int ret;

	path_name = kcalloc(1, PROCFS_PATH_NAME_MAX, GFP_KERNEL);
	if (path_name == NULL)
		return -ENOMEM;

	name_table = kcalloc(1, PROCFS_PATH_NAME_MAX, GFP_KERNEL);
	if (name_table == NULL) {
		ret = -ENOMEM;
		goto free_path;
	}

	if (snprintf(path_name, PROCFS_PATH_NAME_MAX, "net/%s/%s",
		     flag, dev->name) >= PROCFS_ENTRY_MAX_NUM) {
		ret = -ENAMETOOLONG;
		goto free_table;
	}

	dir = get_dev_dir(&net->sysctls.dir, path_name);
	if (dir == NULL) {
		ret = -ENOENT;
		goto free_table;
	}

	if (snprintf(name_table, PROCFS_PATH_NAME_MAX,
		     "sys/net/%s", flag) >= PROCFS_ENTRY_MAX_NUM) {
		ret = -ENAMETOOLONG;
		goto free_table;
	}

	header = &dir->header;
	table = dir->header.ctl_table;

	ret = sysctl_net_dir_init(header, table, entries, name_table);

free_table:
	kfree(name_table);
free_path:
	kfree(path_name);

	return ret;
}

static int liblinux_procfs_create_netdev(struct net_device *dev,
					unsigned int nsid)
{
	struct liblinux_procfs_create_ops *create_ops = NULL;
	struct liblinux_pal_kernfs_ops *ops = NULL;
	struct proc_path_list *entries;
	unsigned int size;
	int ret = 0;

	const char *async_sysctl_list[] = {
		"ipv4/conf",
		"ipv4/neigh",
		"ipv6/conf",
		"ipv6/neigh",
	};

	entries = vmalloc(PROC_TOTAL_SIZE(PROCFS_SYS_MAX_NUM));
	if (entries == NULL)
		return -ENOMEM;

	liblinux_proc_create_sysnet_entries_ops(&create_ops, &ops);
	entries->nsid = nsid;
	entries->max_len = PROCFS_SYS_MAX_NUM;
	for(int i = 0; i < ARRAY_SIZE(async_sysctl_list); i++) {
		ret = netdev_sysctl_traverse(dev, async_sysctl_list[i], entries);
		if (ret == -E2BIG) {
			ret = proc_entries_expansion(&entries);
			if (ret == 0 )
				ret = netdev_sysctl_traverse(dev, async_sysctl_list[i], entries);
			else
				pr_warn("netdev fail to expansion list, len:%u", entries->len);
		}
		if (ret < 0)
			goto err;
	}

	size = PROC_TOTAL_SIZE(entries->len);
	ret = create_ops->create_file((void *)entries, ops, NULL, size);
	if ((ret < 0) && (ret != -EEXIST)) {
		pr_err("netns: create dev:%s procfs sys/net failed", dev->name);
		goto err;
	}

	ret = 0;

err:
	vfree(entries);
	return ret;
}

int liblinux_create_netdev_procfs_batch(struct net_device *dev)
{
	struct net *net = dev_net(dev);
	unsigned int nsid;
	int ret;

	nsid = proc_get_nsid_by_entry(net->proc_net);
	ret = liblinux_procfs_create_netdev(dev, nsid);
		if (ret < 0) {
		pr_err("dev:%s create proc/sys/net failed, ret:%d\n", dev->name, ret);
	}

	return ret;
}
