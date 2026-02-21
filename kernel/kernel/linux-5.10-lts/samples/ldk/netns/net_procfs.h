/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_NET_PROCFS_H__
#define __LIBDH_LINUX_NET_PROCFS_H__
#include "../kernfs.h"

bool sysctl_need_create_batch(void *head, const char *path);
int procfs_create_net_file(unsigned int nsid, const char *name,
					struct liblinux_pal_kernfs_ops *ops,
					void *ctx, unsigned int mode);
int procfs_create_net_dir(unsigned int nsid, const char *name, unsigned int mode);
int procfs_delete_net_data_specific(const char *name, unsigned int nsid);
int procfs_create_net_entries(const void *parent, unsigned int size,
				struct liblinux_pal_kernfs_ops *ops);
void liblinux_proc_create_sysnet_entries_ops(struct liblinux_procfs_create_ops **create_ops,
					struct liblinux_pal_kernfs_ops **ops);
void liblinux_proc_create_procnet_entries_ops(struct liblinux_procfs_create_ops **create_ops,
					struct liblinux_pal_kernfs_ops **ops);
#endif /* !__LIBDH_LINUX_NET_PROCFS_H__ */
