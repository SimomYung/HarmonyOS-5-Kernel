/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_SYSFS_H__
#define __LIBLINUX_SYSFS_H__

#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
void flush_sysfs_wq_no_destroy(void);
#else
static inline void flush_sysfs_wq_no_destroy(void) {}
#endif

#endif	/* __LIBLINUX_SYSFS_H__ */
