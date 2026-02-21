// SPDX-License-Identifier: GPL-2.0-only
#include <linux/lsm_hooks.h>
#include <linux/fs.h>
#include <linux/mm.h>

void __override security_add_hooks(struct security_hook_list *hooks, int count,
				char *lsm)
{
}

int __override security_file_permission(struct file *file, int mask)
{
	return 0;
}

int __override security_sb_set_mnt_opts(struct super_block *sb,
				void *mnt_opts,
				unsigned long kern_flags,
				unsigned long *set_kern_flags)
{
	return 0;
}

int __override security_add_mnt_opt(const char *option, const char *val,
					int len, void **mnt_opts)
{
	return 0;
}

int __override security_file_open(struct file *file)
{
	return 0;
}

#ifdef CONFIG_SECURITY_SELINUX
void __override selinux_complete_init(void)
{
}
#endif
