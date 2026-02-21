/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_INTERNAL_H__
#define __LIBLINUX_INTERNAL_H__

struct task_struct;
void __init_current_mm_info(struct task_struct *p, unsigned long long vspace_key);

#endif
