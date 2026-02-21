/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __ARCH_LIBLINUX_POWER_H__
#define __ARCH_LIBLINUX_POWER_H__

int pm_notifier_call_chain_robust(unsigned long val_up, unsigned long val_down);
int pm_notifier_call_chain(unsigned long val);

#endif
