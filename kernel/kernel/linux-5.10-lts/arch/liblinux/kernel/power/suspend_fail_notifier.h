/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __SUSPEND_FAIL_NOTIFIER_H__
#define __SUSPEND_FAIL_NOTIFIER_H__

/* suspend notifier events */
#define PM_TEST_PLAT_BEGIN	0x0001
#define PM_TEST_DPM_START	0x0002
#define PM_TEST_PLAT_PRE	0x0003
#define PM_TEST_DPM_LATE	0x0004
#define PM_TEST_DPM_NOIRQ	0x0005
#define PM_TEST_NONBOOT_CPU	0x0006

#ifdef CONFIG_SUSPEND_FAIL_NOTIFIER
extern int suspend_error_inject_register(struct notifier_block *nb);
extern int suspend_error_inject_unregister(struct notifier_block *nb);
extern int pm_suspend_should_fail(unsigned long val, int *error);
#else
static inline int suspend_error_inject_register(struct notifier_block *nb) {return 0;}
static inline int suspend_error_inject_unregister(struct notifier_block *nb) {return 0;}
static inline int pm_suspend_should_fail(unsigned long val, int *error) {return false;}
#endif /* CONFIG_SUSPEND_FAIL_NOTIFIER */

#endif /* __SUSPEND_FAIL_NOTIFIER_H__ */
