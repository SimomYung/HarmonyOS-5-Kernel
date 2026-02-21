/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_SYSCTL_H__
#define __LIBDH_LINUX_SYSCTL_H__

#include "procfs.h"

int liblinux_proc_sys_read(unsigned long long pos, unsigned long ubuf,
			 unsigned long size, void *ctx,
			 unsigned long *rsize /* out */);
int liblinux_proc_sys_write(unsigned long long pos, unsigned long ubuf,
			  unsigned long size, void *ctx,
			  unsigned long *wsize /* out */);

#endif /* __LIBDH_LINUX_SYSCTL_H__ */