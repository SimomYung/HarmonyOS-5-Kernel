// SPDX-License-Identifier: GPL-2.0-only
#include <asm/neon.h>
#include <linux/init.h>
#include <liblinux/pal.h>
#include <linux/export.h>
#include <linux/preempt.h>

void kernel_neon_begin(void)
{
	preempt_disable();
	liblinux_pal_fpu_save();
}
EXPORT_SYMBOL(kernel_neon_begin);

void kernel_neon_end(void)
{
	liblinux_pal_fpu_restore();
	preempt_enable();
}
EXPORT_SYMBOL(kernel_neon_end);
