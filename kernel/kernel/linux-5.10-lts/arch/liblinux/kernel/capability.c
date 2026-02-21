// SPDX-License-Identifier: GPL-2.0-only
#include <linux/capability.h>
#include <linux/mm.h>
#include <linux/export.h>
#include <asm/current.h>
#include <liblinux/pal.h>

struct inode;
bool capable_wrt_inode_uidgid(const struct inode *inode, int cap)
{
	return capable(cap);
}
EXPORT_SYMBOL(capable_wrt_inode_uidgid);

bool has_capability(struct task_struct *t, int cap)
{
	BUG_ON(t != current);
	return capable(cap);
}
EXPORT_SYMBOL(has_capability);

bool has_capability_noaudit(struct task_struct *t, int cap)
{
	BUG_ON(t != current);
	return !!liblinux_pal_capable(cap, LIBLINUX_CAP_OPT_NOAUDIT);
}

bool capable(int cap)
{
	return !!liblinux_pal_capable(cap, LIBLINUX_CAP_OPT_NONE);
}
EXPORT_SYMBOL(capable);

bool ns_capable(struct user_namespace *ns, int cap)
{
	return !!liblinux_pal_capable(cap, LIBLINUX_CAP_OPT_NONE);
}
EXPORT_SYMBOL(ns_capable);

bool ns_capable_noaudit(struct user_namespace *ns, int cap)
{
	return !!liblinux_pal_capable(cap, LIBLINUX_CAP_OPT_NOAUDIT);
}
EXPORT_SYMBOL(ns_capable_noaudit);
