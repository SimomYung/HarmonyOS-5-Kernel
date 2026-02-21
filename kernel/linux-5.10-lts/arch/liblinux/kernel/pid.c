// SPDX-License-Identifier: GPL-2.0-only
#include <linux/pid.h>

#include <liblinux/kapi.h>

LIBLINUX_KAPI_DIFF
void put_pid(struct pid *pid)
{
	(void)pid;
}
EXPORT_SYMBOL_GPL(put_pid);
