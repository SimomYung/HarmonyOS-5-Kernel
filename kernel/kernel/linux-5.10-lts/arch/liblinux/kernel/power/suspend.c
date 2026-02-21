// SPDX-License-Identifier: GPL-2.0-only
#include <asm/timex.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/freezer.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/stop_machine.h>
#include <linux/workqueue.h>
#include <liblinux/pal.h>

#include <base/power/power.h>

#include "suspend_fail_notifier.h"
#include "freezer.h"
#include "power.h"

#ifdef CONFIG_PM_SLEEP
int pm_async_enabled = 1;
#endif

#ifdef CONFIG_SUSPEND
suspend_state_t pm_suspend_target_state;
EXPORT_SYMBOL_GPL(pm_suspend_target_state);
#endif

#ifdef CONFIG_SUSPEND
static const struct platform_suspend_ops dummy_ops = {
	.begin = NULL,
	.prepare = NULL,
	.prepare_late = NULL,
	.wake = NULL,
	.finish = NULL,
	.end = NULL,
	.recover = NULL,
};
static const struct platform_suspend_ops *suspend_ops = &dummy_ops;
static const struct platform_s2idle_ops *s2idle_ops;

static int liblinux_suspend_state = LIBLINUX_PM_SUSPEND_DEFAULT;

enum {
	DEV_RESUME_NOIRQ,
	DEV_RESUME_EARLY,
	DEV_RESUME_END,
	PLAT_RESUME_NOIRQ,
	PLAT_RESUME_FINISH,
	PLAT_RESUME_END,
	PLAT_RECOVER,
	PLAT_SUSPEND_PREPARE,
	PLAT_SUSPEND_NOIRQ,
	PLAT_SUSPEND_BEGIN,
};

/* default implementation */
void __weak arch_suspend_disable_irqs(void)
{
	local_irq_disable();
}

/* default implementation */
void __weak arch_suspend_enable_irqs(void)
{
	local_irq_enable();
}

/* Routines for PM-transition notifications */

static BLOCKING_NOTIFIER_HEAD(pm_chain_head);

int register_pm_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&pm_chain_head, nb);
}
EXPORT_SYMBOL_GPL(register_pm_notifier);

int unregister_pm_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&pm_chain_head, nb);
}
EXPORT_SYMBOL_GPL(unregister_pm_notifier);

int pm_notifier_call_chain_robust(unsigned long val_up, unsigned long val_down)
{
	int ret;

	ret = blocking_notifier_call_chain_robust(&pm_chain_head, val_up, val_down, NULL);

	return notifier_to_errno(ret);
}

int pm_notifier_call_chain(unsigned long val)
{
	return blocking_notifier_call_chain(&pm_chain_head, val, NULL);
}

static int platform_suspend(suspend_state_t state, unsigned int step)
{
	int ret = 0;
	if (state != PM_SUSPEND_TO_IDLE) {
		switch (step) {
		case (unsigned int)PLAT_SUSPEND_PREPARE:
			if (suspend_ops->prepare != NULL) {
				ret = suspend_ops->prepare();
			}
			break;
		case (unsigned int)PLAT_SUSPEND_NOIRQ:
			if (suspend_ops->prepare_late != NULL) {
				ret = suspend_ops->prepare_late();
			}
			break;
		case (unsigned int)PLAT_SUSPEND_BEGIN:
			if (suspend_ops->begin != NULL) {
				ret = suspend_ops->begin(state);
			}
			break;
		default:
			break;
		}
	} else {
		if (step == (unsigned int)PLAT_SUSPEND_BEGIN) {
			if (s2idle_ops && s2idle_ops->begin) {
				return s2idle_ops->begin();
			}
		}
	}
	return ret;
}

static void platform_resume(suspend_state_t state, unsigned int step)
{
	if (state != PM_SUSPEND_TO_IDLE) {
		switch (step) {
		case (unsigned int)PLAT_RESUME_NOIRQ:
			if (suspend_ops->wake != NULL) {
				suspend_ops->wake();
			}
			break;
		case (unsigned int)PLAT_RESUME_FINISH:
			if (suspend_ops->finish != NULL) {
				suspend_ops->finish();
			}
			break;
		case (unsigned int)PLAT_RESUME_END:
			if (suspend_ops->end != NULL) {
				suspend_ops->end();
			}
			break;
		case (unsigned int)PLAT_RECOVER:
			if (suspend_ops->recover != NULL) {
				suspend_ops->recover();
			}
			break;
		default:
			break;
		}
	} else {
		if (step == (unsigned int)PLAT_RESUME_END) {
			if (s2idle_ops && s2idle_ops->end) {
				return s2idle_ops->end();
			}
		}
	}
}

static bool check_wakeup_count(void)
{
	bool no_wk_lock = false;
	unsigned int wakeup_count = 0;

	no_wk_lock = pm_get_wakeup_count(&wakeup_count, false);
	if (!no_wk_lock) {
		pr_err("there is wake lock in progress\n");
#ifdef CONFIG_POWER_DUBAI
		pm_get_active_wakeup_sources();
#endif
		return no_wk_lock;
	}
	no_wk_lock = pm_save_wakeup_count(wakeup_count);
	if (!no_wk_lock) {
		pr_err("save wakeup count failed\n");
#ifdef CONFIG_POWER_DUBAI
		pm_get_active_wakeup_sources();
#endif
		return no_wk_lock;
	}

	return true;
}
#endif

#if defined (CONFIG_SUSPEND) || defined(CONFIG_LIBLINUX_HIBERNATE)

void liblinux_freeze_proc(void)
{
	suspend_system_proc_freeze(true);
}
EXPORT_SYMBOL(liblinux_freeze_proc);

void liblinux_thaw_proc(void)
{
	suspend_system_proc_freeze(false);
}
EXPORT_SYMBOL(liblinux_thaw_proc);

#define FREEZE_WORKQUEUE_WAIT_MS		20000U
#define FREEZE_WORKQUEUE_SLEEP_US		1000U

int liblinux_freeze_wqs(void)
{
	bool wakeup = false;
	bool wq_busy = true;
	unsigned long due_time;

	due_time = jiffies + msecs_to_jiffies(FREEZE_WORKQUEUE_WAIT_MS);
	freeze_workqueues_begin();
	while (true) {
		wq_busy = freeze_workqueues_busy();
		if (!wq_busy || time_after(jiffies, due_time)) {
			break;
		}
		if (pm_wakeup_pending()) {
			wakeup = true;
			break;
		}
		usleep_range(FREEZE_WORKQUEUE_SLEEP_US, FREEZE_WORKQUEUE_SLEEP_US);
	}
	if (wq_busy || wakeup) {
		pr_err("freeze ldk workqueues failed, ldk wqs categorized as %sbusy%s\n",
		       wq_busy ? "" : "not ",
		       wakeup ? ", aborted by wakeup source" : "");
		thaw_workqueues();
		return -EBUSY;
	}

	return 0;
}
EXPORT_SYMBOL(liblinux_freeze_wqs);

void liblinux_thaw_wqs(void)
{
	thaw_workqueues();
}
EXPORT_SYMBOL(liblinux_thaw_wqs);
#endif

int liblinux_suspend_prepare(int force)
{
#ifdef CONFIG_SUSPEND
	int error;

	pm_suspend_clear_flags();
	error = pm_notifier_call_chain_robust(PM_SUSPEND_PREPARE, PM_POST_SUSPEND);
	if (error) {
		return error;
	}
	if (!force) {
		if (!check_wakeup_count()) {
			(void)pm_notifier_call_chain(PM_POST_SUSPEND);
			return -EBUSY;
		}
	}
	pm_wakeup_clear(true);
	liblinux_inform_suspend();
	liblinux_suspend_state_update(LIBLINUX_PM_SUSPEND_MEM);

	return 0;
#else
	return 0;
#endif
}
EXPORT_SYMBOL(liblinux_suspend_prepare);

void liblinux_resume_finish(void)
{
#ifdef CONFIG_SUSPEND
	(void)pm_notifier_call_chain(PM_POST_SUSPEND);
	events_check_enabled = false;
	liblinux_inform_resume();
	liblinux_suspend_state_update(LIBLINUX_PM_SUSPEND_DEFAULT);
#endif
}
EXPORT_SYMBOL(liblinux_resume_finish);

#ifdef CONFIG_SUSPEND
static void liblinux_resume_rollback(unsigned int step)
{
	suspend_state_t state = PM_SUSPEND_MEM;

	pr_info("liblinux resume rollback start, step: %u\n", step);
	switch (step) {
	case (unsigned int)PLAT_RESUME_NOIRQ:
		platform_resume(state, PLAT_RESUME_NOIRQ);
		/* fall through */
	case (unsigned int)DEV_RESUME_NOIRQ:
		dpm_resume_noirq(PMSG_RESUME);
		/* fall through */
	case (unsigned int)DEV_RESUME_EARLY:
		dpm_resume_early(PMSG_RESUME);
		/* fall through */
	case (unsigned int)PLAT_RESUME_FINISH:
		platform_resume(state, PLAT_RESUME_FINISH);
		/* fall through */
	case (unsigned int)DEV_RESUME_END:
		dpm_resume_end(PMSG_RESUME);
		/* fall through */
	case (unsigned int)PLAT_RESUME_END:
		platform_resume(state, PLAT_RESUME_END);
		break;
	default:
		break;
	}
	pr_info("liblinux resume rollback end\n");
}
#endif

static int g_suspend_cnt = 0;
static bool g_suspend_success = true;

bool liblinux_is_suspend_success(void)
{
	return g_suspend_success;
}

void liblinux_resume(void)
{
#ifdef CONFIG_SUSPEND
	liblinux_resume_rollback(PLAT_RESUME_NOIRQ);
#endif
}
EXPORT_SYMBOL(liblinux_resume);

static inline void liblinux_platform_recover(void)
{
	platform_resume(PM_SUSPEND_MEM, PLAT_RECOVER);
	liblinux_resume_rollback(DEV_RESUME_END);
}

int liblinux_suspend(void)
{
#ifdef CONFIG_SUSPEND
	int error;
	suspend_state_t state = PM_SUSPEND_MEM;

	pr_info("liblinux suspend start\n");

	if (!pm_suspend_should_fail(PM_TEST_PLAT_BEGIN, &error))
		error = platform_suspend(state, PLAT_SUSPEND_BEGIN);
	if (error != 0) {
		pr_err("platform failed to begin suspend, error=%d\n", error);
		liblinux_resume_rollback(PLAT_RESUME_END);
		goto exit;
	}

	if (!pm_suspend_should_fail(PM_TEST_DPM_START, &error))
		error = dpm_suspend_start(PMSG_SUSPEND);
	if (error != 0) {
		pr_err("Some devices failed to suspend, "
		       "or early wake event detected, error=%d\n", error);
		liblinux_platform_recover();
		goto exit;
	}

	if (!pm_suspend_should_fail(PM_TEST_PLAT_PRE, &error))
		error = platform_suspend(state, PLAT_SUSPEND_PREPARE);
	if (error != 0){
		pr_err("platform failed to prepare suspend, error=%d\n", error);
		liblinux_resume_rollback(PLAT_RESUME_FINISH);
		goto exit;
	}

	if (!pm_suspend_should_fail(PM_TEST_DPM_LATE, &error))
		error = dpm_suspend_late(PMSG_SUSPEND);
	if (error != 0) {
		pr_err("late suspend of devices failed, error=%d\n", error);
		liblinux_resume_rollback(PLAT_RESUME_FINISH);
		goto exit;
	}

	if (!pm_suspend_should_fail(PM_TEST_DPM_NOIRQ, &error))
		error = dpm_suspend_noirq(PMSG_SUSPEND);
	if (error != 0) {
		pr_err("noirq suspend of devices failed, error=%d\n", error);
		liblinux_resume_rollback(DEV_RESUME_EARLY);
		goto exit;
	}
	error = platform_suspend(state, PLAT_SUSPEND_NOIRQ);
	if (error != 0) {
		pr_err("pltaform failed to prepare noirq, error=%d\n", error);
		liblinux_resume_rollback(PLAT_RESUME_NOIRQ);
		goto exit;
	}

	pr_info("liblinux suspend end\n");
exit:
	return error;
#else
	return 0;
#endif
}
EXPORT_SYMBOL(liblinux_suspend);

static struct liblinux_suspend_core_ops *g_suspend_core_ops = NULL;

int liblinux_suspend_core_ops_register(struct liblinux_suspend_core_ops *ops)
{
	if (g_suspend_core_ops != NULL) {
		return -EEXIST;
	}
	if (ops == NULL) {
		return -EINVAL;
	}
	g_suspend_core_ops = ops;

	return 0;
}

static int liblinux_suspend_core(void)
{
	int err = 0;
	int state = liblinux_suspend_state_of();

	switch (state) {
	case LIBLINUX_PM_SUSPEND_MEM:
		if (g_suspend_core_ops != NULL &&
		    g_suspend_core_ops->suspend != NULL) {
			err = g_suspend_core_ops->suspend();
		}
		break;
	case LIBLINUX_PM_SUSPEND_DISK:
		if (g_suspend_core_ops != NULL &&
		    g_suspend_core_ops->freeze != NULL) {
			err = g_suspend_core_ops->freeze();
		}
		break;
	default:
		pr_warn("invalid state of sr: %d\n", state);
		err = -EINVAL;
		break;
	}

	return err;
}

static void liblinux_resume_core(void)
{
	int state = liblinux_suspend_state_of();

	switch (state) {
	case LIBLINUX_PM_SUSPEND_MEM:
		if (g_suspend_core_ops != NULL &&
		    g_suspend_core_ops->resume != NULL) {
			g_suspend_core_ops->resume();
		}
		break;
	case LIBLINUX_PM_SUSPEND_DISK:
		if (g_suspend_core_ops != NULL &&
		    g_suspend_core_ops->thaw != NULL) {
			g_suspend_core_ops->thaw();
		}
		break;
	default:
		pr_warn("invalid state of sr: %d\n", state);
		break;
	}
}

int liblinux_suspend_late(void)
{
#ifdef CONFIG_SUSPEND
	int err;

	if (pm_wakeup_pending()) {
		return -EBUSY;
	}
	arch_suspend_disable_irqs();
	err = syscore_suspend();
	if (err != 0) {
		pr_err("syscore suspend failed, error=%d\n", err);
		arch_suspend_enable_irqs();
		return err;
	}
	err = liblinux_suspend_core();
	if (err != 0) {
		pr_err("syscore suspend late failed, error=%d\n", err);
		syscore_resume();
		arch_suspend_enable_irqs();
		return err;
	}
	arch_suspend_enable_irqs();

	return 0;
#else
	return 0;
#endif
}
EXPORT_SYMBOL(liblinux_suspend_late);

void liblinux_resume_early(void)
{
#ifdef CONFIG_SUSPEND
	int err;
	int ori_cnt = g_suspend_cnt;

	err = liblinux_pal_suspend_cnt_read(&g_suspend_cnt);
	if (err == 0) {
		g_suspend_success = (ori_cnt != g_suspend_cnt);
	} else {
		pr_warn("suspend cnt read failed\n");
		g_suspend_success = false;
	}
	arch_time_sync_early();
	arch_suspend_disable_irqs();
	liblinux_resume_core();
	syscore_resume();
	arch_suspend_enable_irqs();
	/* In syscore_resume, timekeeping_resume would incorrectly rewrite the time, causing
	 * ldk's time to be imcompatible with kernel's time. The following re-sync operation
	 * correct the result of timekeeping_resume. Note that, it's not safe to simply
	 * remove the calling of timekeeping_resume, as timekeeping_resume also performs other
	 * adjustment, such as DFX. */
	arch_time_sync_early();
#endif
}
EXPORT_SYMBOL(liblinux_resume_early);

void liblinux_suspend_state_update(int suspend_state)
{
#ifdef CONFIG_SUSPEND
	liblinux_suspend_state = suspend_state;
#endif
}

int liblinux_suspend_state_of(void)
{
#ifdef CONFIG_SUSPEND
	return liblinux_suspend_state;
#else
	return LIBLINUX_PM_SUSPEND_DEFAULT;
#endif
}

static void suspend_lock_devhp(void)
{
	if (liblinux_suspend_state_of() != LIBLINUX_PM_SUSPEND_DISK) {
		lock_device_hotplug();
	}
}

static void suspend_unlock_devhp(void)
{
	if (liblinux_suspend_state_of() != LIBLINUX_PM_SUSPEND_DISK) {
		unlock_device_hotplug();
	}
}

int liblinux_add_cpu(unsigned int cpu)
{
	int ret;

	suspend_lock_devhp();
	ret = device_online(get_cpu_device(cpu));
	suspend_unlock_devhp();

	return ret;
}
EXPORT_SYMBOL(liblinux_add_cpu);

int liblinux_remove_cpu(unsigned int cpu)
{
	int ret;

	suspend_lock_devhp();
	ret = device_offline(get_cpu_device(cpu));
	suspend_unlock_devhp();

	return ret;
}
EXPORT_SYMBOL(liblinux_remove_cpu);
