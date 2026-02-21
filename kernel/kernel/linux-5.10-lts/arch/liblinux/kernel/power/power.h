/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __ARCH_LIBLINUX_POWER_H__
#define __ARCH_LIBLINUX_POWER_H__

#define LIBLINUX_PM_SUSPEND_DEFAULT	0
#define LIBLINUX_PM_SUSPEND_MEM		1
#define LIBLINUX_PM_SUSPEND_DISK	2

int pm_notifier_call_chain_robust(unsigned long val_up, unsigned long val_down);
int pm_notifier_call_chain(unsigned long val);

void liblinux_suspend_state_update(int suspend_state);
int liblinux_suspend_state_of(void);

#endif
