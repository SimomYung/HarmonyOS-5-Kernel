/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_KERNFS_H__
#define __LIBDH_LINUX_KERNFS_H__

#include <linux/kernfs.h>

/*
 * the valid nsid is from 0 to 8192,
 * use 8193 to identify nodes that do not distinguish nsid.
 */
#define KERNFS_COMMON_NSID 8193
#define NET_NSID_MASK 0x3FFF

/* Rough statistics show that almost all sysfs & procfs file paths are less than 160,
 * but there may be exceptions that are outside the scope of our statistics,
 * and we will also ensure that they can work correctly.
 */

#define LDK_FAST_PATH_BUF_MAX 160

struct liblinux_pal_kernfs_ops {
	int (*read)(unsigned long long pos, unsigned long ubuf, unsigned long size,
		    void *ctx, unsigned long *rsize /* out */);
	int (*write)(unsigned long long pos, unsigned long ubuf, unsigned long size,
		     void *ctx, unsigned long *wsize /* out */);
	int (*poll)(int (*callback)(void *ctx, int pollable), int id, int pollable);
	void *(*open)(void *ctx, unsigned int flags, unsigned int mode);
	void (*close)(void *filep);
	void (*flush)(void);
	int (*enter)(void);
	void (*exit)(void);
	void (*unlink)(void *ctx);
};

int kernfs_get_target_path(struct kernfs_node *parent,
			   struct kernfs_node *target, char *path);

/* Below should be moved from pal to lnxbase */
enum liblinux_fs_type {
	LIBLINUX_SYSFS = 1,
	MAX_FS_TYPE, /* always be the last one */
};
int liblinux_pal_require_frange(int type, int *min, int *max);
int liblinux_pal_sysfs_register_ops_ex(void *ops);
int liblinux_pal_sysfs_register_ops_ex_ns(void *ops);

int liblinux_pal_sysfs_create_dir_ns(const char *name, unsigned int mode, unsigned int nsid);
int liblinux_pal_sysfs_create_file_ex(const char *name, unsigned int mode, unsigned long long size,
				   struct liblinux_pal_kernfs_ops *fops,
				   void *ctx, unsigned int nsid);
int liblinux_pal_sysfs_create_link_ns(const char *name, unsigned int mode,
				   const char *linkdata, unsigned int nsid);
int liblinux_pal_sysfs_delete_ns(const char *name, unsigned int nsid);
int liblinux_pal_sysfs_notify(void *ctx, const char *name);
int liblinux_pal_sysfs_rename_link_ns(const char *old_name, const char *new_name,
								   unsigned int old_nsid, unsigned int new_nsid);
int liblinux_pal_sysfs_create_data_batch(void *ldk_entry, int cnt);

int liblinux_sysfs_get_file_id(struct kernfs_node *kn, int *id);

const void *liblinux_get_ns_info_by_kn(struct kernfs_node *kn);

unsigned int liblinux_get_nsid_by_ns(const struct net *ns);

#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
bool liblinux_sysfs_is_sync(void);
bool liblinux_driver_is_async(void);
void liblinux_driver_set_async(bool flag);
int liblinux_register_netdev_async(void *data);
int liblinux_kobject_add_uevent_async(void *data);
int liblinux_sysfs_create_dir(const char *name, unsigned int mode, unsigned int nsid);
int liblinux_sysfs_create_file(const char *name, unsigned int mode, unsigned long long size,
					     struct liblinux_pal_kernfs_ops *fops,
					     void *ctx, unsigned int nsid);

int liblinux_sysfs_create_link(const char *name, unsigned int mode,
					     const char *linkdata, unsigned int nsid);

int liblinux_sysfs_delete(const char *name, unsigned int nsid);

int liblinux_sysfs_notify(void *ctx, const char *name);

int liblinux_sysfs_rename_link(const char *old_name, const char *new_name,
							   unsigned int old_nsid, unsigned int new_nsid);
void flush_sysfs_wq(void);

#else /* !CONFIG_LIBLINUX_ASYNC_SYSFS */
static inline int liblinux_sysfs_create_dir(const char *name, unsigned int mode, unsigned int nsid)
{
	return liblinux_pal_sysfs_create_dir_ns(name, mode, nsid);
}

static inline int liblinux_sysfs_create_file(const char *name, unsigned int mode, unsigned long long size,
					     struct liblinux_pal_kernfs_ops *fops,
					     void *ctx, unsigned int nsid)
{
	return liblinux_pal_sysfs_create_file_ex(name, mode, size, fops, ctx, nsid);
}

static inline int liblinux_sysfs_create_link(const char *name, unsigned int mode,
					     const char *linkdata, unsigned int nsid)
{
	return liblinux_pal_sysfs_create_link_ns(name, mode, linkdata, nsid);
}

static inline int liblinux_sysfs_delete(const char *name, unsigned int nsid)
{
	return liblinux_pal_sysfs_delete_ns(name, nsid);
}

static inline void flush_sysfs_wq(void) {}

static inline int liblinux_sysfs_notify(void *ctx, const char *name)
{
	return liblinux_pal_sysfs_notify(ctx, name);
}

static inline int liblinux_sysfs_rename_link(const char *old_name, const char *new_name,
											 unsigned int old_nsid, unsigned int new_nsid)
{
	return liblinux_pal_sysfs_rename_link_ns(old_name, new_name, old_nsid, new_nsid);
}
#endif /* CONFIG_LIBLINUX_ASYNC_SYSFS */
#endif
