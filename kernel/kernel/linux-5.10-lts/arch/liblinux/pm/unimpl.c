// SPDX-License-Identifier: GPL-2.0-only
#include <linux/err.h>
#include <liblinux/unimpl.h>
#include <linux/workqueue.h>

/* kernel/power/main.c */
struct workqueue_struct *pm_wq;

static int __init pm_init(void)
{
#ifdef CONFIG_PM_WQ_UNBOUND
	pm_wq = alloc_workqueue("pm", WQ_FREEZABLE | WQ_UNBOUND | WQ_HIGHPRI, 0);
#else
	pm_wq = alloc_workqueue("pm", WQ_FREEZABLE, 0);
#endif

	return pm_wq ? 0 : -ENOMEM;
}
core_initcall(pm_init);

#ifdef CONFIG_SUSPEND
unsigned int pm_suspend_global_flags;
LIBLINUX_GPL_EXPORTED_UNIMPL(s2idle_wake)
#endif

/* drivers/base/power/wakeirq.c */
LIBLINUX_UNIMPL(dev_pm_arm_wake_irq)
LIBLINUX_GPL_EXPORTED_UNIMPL(dev_pm_clear_wake_irq)
LIBLINUX_UNIMPL(dev_pm_disable_wake_irq_check)
LIBLINUX_UNIMPL(dev_pm_disarm_wake_irq)
LIBLINUX_UNIMPL(dev_pm_enable_wake_irq_check)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(dev_pm_set_dedicated_wake_irq, int, 0)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(dev_pm_set_wake_irq, int, 0)

/* drivers/base/power/common.c */
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(dev_pm_domain_attach, int, 0)
LIBLINUX_GPL_EXPORTED_UNIMPL(dev_pm_domain_detach)
