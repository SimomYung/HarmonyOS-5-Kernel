/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __LIBLINUX_LINUX_PRINTK_H__
#define __LIBLINUX_LINUX_PRINTK_H__

#define kptr_restrict __kptr_restrict
#include_next <linux/printk.h>
#undef kptr_restrict

extern int liblinux_pal_kptr_restrict(void);
#define kptr_restrict liblinux_pal_kptr_restrict()

#endif /* __LIBLINUX_LINUX_PRINTK_H__ */
