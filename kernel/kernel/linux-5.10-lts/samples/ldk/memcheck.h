/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_MEMCHECK_H__
#define __LIBDH_MEMCHECK_H__

#include <errno.h>
#ifndef __KERNEL__
#include <hmkernel/types.h>
#else
#include <linux/types.h>
#endif

struct kernel_siginfo;
#ifdef CONFIG_DFX_MEMCHECK_STACK
int liblinux_send_memcheck_signal(int pid, int signum, struct kernel_siginfo *info);
#else
static inline int liblinux_send_memcheck_signal(int pid, int signum, struct kernel_siginfo *info)
{
	UNUSED(pid, signum, info);

	return -ENOSYS;
}
#endif

#endif /* __LIBDH_MEMCHECK_H__ */
