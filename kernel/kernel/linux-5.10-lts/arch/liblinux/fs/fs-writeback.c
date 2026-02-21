// SPDX-License-Identifier: GPL-2.0-only
#include <linux/writeback.h>

#include <liblinux/kapi.h>

LIBLINUX_KAPI_DIFF
void inode_wait_for_writeback(struct inode *inode)
{
	(void)inode;
}

LIBLINUX_KAPI_DIFF
void __mark_inode_dirty(struct inode *inode, int flags)
{
	(void)inode;
	(void)flags;
}
EXPORT_SYMBOL(__mark_inode_dirty);
