/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_PROCFS_H__
#define __LIBDH_LINUX_PROCFS_H__

#include "kernfs.h"
/* Below should be moved from liblinux/pal.h to lnxbase and remove **pal** */

struct liblinux_procfs_create_ops {
	int (*create_file)(const char *name, struct liblinux_pal_kernfs_ops *ops,
				void *ctx, unsigned int mode);
	int (*create_dir)(const char *name, unsigned int mode);
};

int liblinux_pal_procfs_register_ops(struct liblinux_pal_kernfs_ops *ops);
int liblinux_pal_procfs_create_file_bypath(const char *name,
				    unsigned int mode);
int liblinux_pal_procfs_create_file(const char *name,
				    struct liblinux_pal_kernfs_ops *ops,
				    void *ctx, unsigned int mode);
int liblinux_pal_procfs_create_dir(const char *name, unsigned int mode);
int liblinux_pal_procfs_create_link(const char *name, unsigned int mode,
				    char *linkdata);
int liblinux_pal_procfs_delete(const char *name);
int liblinux_procfs_read(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *ctx,
				  unsigned long *rsize /* out */);
int liblinux_procfs_write(unsigned long long pos, unsigned long ubuf,
				   unsigned long size, void *ctx,
				   unsigned long *wsize /* out */);
struct vfsmount *proc_mnt_get_locked(void);
void proc_mnt_unlock(void);
#endif /* __LIBDH_LINUX_PROCFS_H__ */
