// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mm.h>

#include <liblinux/kapi.h>

LIBLINUX_KAPI_DIFF
void truncate_setsize(struct inode *inode, loff_t newsize)
{
	(void)inode;
	(void)newsize;
}
EXPORT_SYMBOL(truncate_setsize);

LIBLINUX_KAPI_DIFF
void truncate_inode_pages_final(struct address_space *mapping)
{
	(void)mapping;
}
EXPORT_SYMBOL(truncate_inode_pages_final);
