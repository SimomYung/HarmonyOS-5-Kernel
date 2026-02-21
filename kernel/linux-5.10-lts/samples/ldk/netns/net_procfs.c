// SPDX-License-Identifier: GPL-2.0
#include <stddef.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <hmnet/hmnet_procfs.h>

int procfs_create_net_file(unsigned int nsid, const char *name,
					struct liblinux_pal_kernfs_ops *ops,
					void *ctx, unsigned int mode)
{
	return hmnet_procfs_create_ldk_entry(nsid, name, ops, ctx, mode);
}

int procfs_create_net_dir(unsigned int nsid, const char *name, unsigned int mode)
{
	return hmnet_procfs_create_ldk_entry(nsid, name, NULL, NULL, mode);
}

int procfs_delete_net_data_specific(const char *name, unsigned int nsid)
{
	return hmnet_procfs_delete_ldk_entry_specific(name, nsid);
}

int procfs_create_net_entries(void *parent, unsigned int size,
				struct liblinux_pal_kernfs_ops *ops)
{
	return hmnet_procfs_create_ldk_entries(parent, size, ops);
}
