/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 27 10:42:14 2019
 */
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/thermal.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/tick.h>
#include <linux/rcutree.h>
#include <linux/workqueue.h>
#include <linux/pm.h>

#include <lnxbase/lnxbase.h>
#ifdef CONFIG_HUAWEI_DUBAI
#include <log/hwlog_kernel.h>
#include <chipset_common/dubai/dubai.h>
#endif

#include <devhost/pm.h>
#include "pmqos.h"
#include "wakeup.h"
#include "devhost.h"

#include "linux_device.h"

#ifdef CONFIG_VENDOR_HOOKS
#include <trace/hooks/liblinux.h>
#endif
/* keep consistent with sysif */
#define SHUTDOWN_PREPARE	0U
#define REBOOT_PREPARE		1U
#define LIBLINUX_CPUHP_NONBOOT	0xffffffff

struct wl_async_work {
	struct work_struct work;
	bool is_activate;
};

static struct workqueue_struct *wakelock_wq = NULL;

static int device_pm_shutdown(struct device_info *info)
{
	struct device *dev = NULL;

	dev = linux_device_get_device(info);
	if (dev == NULL) {
		pr_err("device info get device failed\n");
		return -EINVAL;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	if (dev->class && dev->class->shutdown) {
		dev->class->shutdown(dev);
#else
	if (dev->class && dev->class->shutdown_pre) {
		dev->class->shutdown_pre(dev);
#endif
	} else if (dev->bus && dev->bus->shutdown) {
		dev->bus->shutdown(dev);
	} else if (dev->driver && dev->driver->shutdown) {
		dev->driver->shutdown(dev);
	} else {
		/* do nothing, ignored */
	}

	return 0;
}

struct devhost_dev_pm_ops dev_pm_ops __read_mostly = {
	.shutdown = device_pm_shutdown,
};

static int platform_prepare(unsigned int type, char *cmd)
{
	switch(type) {
	case SHUTDOWN_PREPARE:
		return blocking_notifier_call_chain(&reboot_notifier_list,
						    SYS_POWER_OFF, NULL);
	case REBOOT_PREPARE:
		return blocking_notifier_call_chain(&reboot_notifier_list,
						    SYS_RESTART, cmd);
	default:
		break;
	}

	return -EINVAL;
}

static int platform_power_off(void)
{
	machine_power_off();

	return -ENOSYS;
}

static int platform_reboot(const char *cmd)
{
	machine_restart((char *)cmd);

	return -ENOSYS;
}

static int platform_wq_freeze(void)
{
	return liblinux_freeze_wqs();
}

static void platform_wq_thaw(void)
{
	liblinux_thaw_wqs();
}

static void platform_syscore_shutdown(void)
{
	syscore_shutdown();
}

static int devices_platform_suspend(void)
{
#ifdef CONFIG_HUAWEI_DUBAI
	int ret;
	int pid;
	char name[RTC_OWNER_NAME_LEN] = {0};

	ret = lnxbase_rtc_read_pid_and_name(&pid, name, RTC_OWNER_NAME_LEN);
	if (ret == 0) {
		dubai_set_rtc_timer(name, pid);
		pr_info("suspend.dubai: pid=%d, name=%s\n", pid, name);
	}
#endif
	return liblinux_suspend();
}

static int devices_platform_suspend_prepare(bool force)
{
	return liblinux_suspend_prepare((int)force);
}

static int platform_suspend(unsigned int action)
{
	int err = 0;
	switch (action) {
	case DH_PM_EVENT_SUSPEND_PREPARE_NORMAL:
		err = devices_platform_suspend_prepare(false);
		break;
	case DH_PM_EVENT_SUSPEND_PREPARE_FORCE:
		err = devices_platform_suspend_prepare(true);
		break;
	case DH_PM_EVENT_SUSPEND:
		err = devices_platform_suspend();
		break;
	case DH_PM_EVENT_RESUME:
		liblinux_resume();
		break;
	case DH_PM_EVENT_RESUME_FINISH:
		liblinux_resume_finish();
		break;
	case DH_PM_EVENT_SUSPEND_LATE:
		err = liblinux_suspend_late();
		break;
	case DH_PM_EVENT_RESUME_EARLY:
		liblinux_resume_early();
		break;
	case DH_PM_EVENT_ULSR_ENTER:
#ifdef CONFIG_LIBLINUX_HAVE_ULSR
		liblinux_ulsr_enter();
#endif
		break;
	case DH_PM_EVENT_ULSR_LEAVE:
#ifdef CONFIG_LIBLINUX_HAVE_ULSR
		liblinux_ulsr_leave();
#endif
		break;
	default:
		err = -EINVAL;
	}
	return err;
}

static void platform_preempt_bypass(bool bypass)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(5,10,1)
	liblinux_preempt_bypass(bypass);
#endif
}

#if LINUX_VERSION_CODE <= KERNEL_VERSION(5,6,0)
#define remove_cpu(x)	cpu_down(x)
#define add_cpu(x)	cpu_up(x)
#endif

static int platform_cpu_add(unsigned int cpu)
{
#ifdef CONFIG_HOTPLUG_CPU
	return liblinux_add_cpu(cpu);
#else
	return -ENOSYS;
#endif
}

static int platform_cpu_remove(unsigned int cpu)
{
#ifdef CONFIG_HOTPLUG_CPU
	return liblinux_remove_cpu(cpu);
#else
	return -ENOSYS;
#endif
}

static int platform_idle_enter(void)
{
	/*
	 * Temporary Use below macros to represent platform kirin which enable
	 * deep idle, which shoudl be changed as soon as possible
	 */
#if defined(CONFIG_NO_HZ_IDLE) && ((LINUX_VERSION_CODE == KERNEL_VERSION(4,14,61)) \
				   || (LINUX_VERSION_CODE) == KERNEL_VERSION(5,10,97) \
				   || (LINUX_VERSION_CODE) == KERNEL_VERSION(5,10,43))
	int ret;
	int saved_preempt_cnt = preempt_count();
	unsigned int cpu = smp_processor_id();

	if (__preempt_disabled[cpu] > 0 || is_vtimer_dying(cpu)) {
		return -EAGAIN;
	}

	/* OH */
#if (LINUX_VERSION_CODE == KERNEL_VERSION(5,10,97) || (LINUX_VERSION_CODE) == KERNEL_VERSION(5,10,43))
	local_irq_disable();
	rcu_note_context_switch(false);
	local_irq_enable();
#endif

	preempt_count_set(saved_preempt_cnt + 1);
	tick_nohz_idle_enter();
	local_irq_disable();
	tick_nohz_idle_stop_tick();
	ret = tick_nohz_tick_stopped();
	rcu_idle_enter();
	/* notify rcu to exit idle when tick was not stoped,
	 * because idlemgr will not call idle_exit after with the ret as 0 */
	if (ret == 0) {
		rcu_idle_exit();
	}
	local_irq_enable();
	preempt_count_set(saved_preempt_cnt);
	return (ret == 0) ? -EAGAIN : 0;
#elif defined(CONFIG_LIBLINUX_NO_HZ)
	return liblinux_nohz_idle_enter();
#else
	return -EAGAIN;
#endif
}

static int platform_idle_exit(void)
{
#if defined(CONFIG_NO_HZ_IDLE) && ((LINUX_VERSION_CODE == KERNEL_VERSION(4,14,61)) \
				   || (LINUX_VERSION_CODE) == KERNEL_VERSION(5,10,97) \
				   || (LINUX_VERSION_CODE) == KERNEL_VERSION(5,10,43))
	int saved_preempt_cnt = preempt_count();
	preempt_count_set(saved_preempt_cnt + 1);
	rcu_idle_exit();
	tick_nohz_idle_exit();
	preempt_count_set(saved_preempt_cnt);
#elif defined(CONFIG_LIBLINUX_NO_HZ)
	liblinux_nohz_idle_exit();
#endif
	return 0;
}

static int platform_hibernate(unsigned int action)
{
	int err = 0;
#ifdef CONFIG_LIBLINUX_HIBERNATE
	switch (action) {
	case DH_PM_EVENT_PREPARE:
		err = liblinux_hibernate_prepare();
		break;
	case DH_PM_EVENT_POST:
		liblinux_post_hibernate();
		break;
	case DH_PM_EVENT_RESTORE_PREPARE:
		err = liblinux_hibernate_restore_prepare();
		break;
	case DH_PM_EVENT_POST_RESTORE:
		liblinux_hibernate_post_restore();
		break;
	case DH_PM_EVENT_FREEZE:
		err = liblinux_hibernate(PM_EVENT_FREEZE);
		break;
	case DH_PM_EVENT_QUIESCE:
		err = liblinux_hibernate(PM_EVENT_QUIESCE);
		break;
	case DH_PM_EVENT_THAW:
		liblinux_hibernate_resume(PM_EVENT_THAW);
		break;
	case DH_PM_EVENT_RECOVER:
		liblinux_hibernate_resume(PM_EVENT_RECOVER);
		break;
	case DH_PM_EVENT_RESTORE:
		liblinux_hibernate_resume(PM_EVENT_RESTORE);
		break;
	default:
		err = -EINVAL;
	}
#endif
	return err;
}

static int platform_dubai_hwlog(unsigned int log_lv, char *tag, char *msg)
{
#ifdef CONFIG_HUAWEI_DUBAI
	return hwlog_to_write(log_lv, HW_LOG_ID_DUBAI, tag, msg);
#else
	return -ENOSYS;
#endif
}

#define THERMAL_DEV_NUM_MAX		10U

static struct thermal_zone_device *g_tz_devs[THERMAL_DEV_NUM_MAX];
static int g_therm_dev_index;

static int platform_thermal_device_acquire(const char *type, int *id)
{
	struct thermal_zone_device *tz_dev = NULL;

	if (g_therm_dev_index >= THERMAL_DEV_NUM_MAX) {
		pr_warn("thermal device number is out of range\n");
		return -EFAULT;
	}
	tz_dev = thermal_zone_get_zone_by_name(type);
	if (IS_ERR(tz_dev)) {
		pr_err("get thermal zone %s failed: %d\n", type, PTR_ERR(tz_dev));
		return PTR_ERR(tz_dev);
	}
	g_tz_devs[g_therm_dev_index] = tz_dev;
	*id = g_therm_dev_index;
	g_therm_dev_index++;

	return 0;
}

static int platform_thermal_device_temp_read(int id, int *temp)
{
	int tmp, err;
	struct thermal_zone_device *tz_dev = NULL;

	if (id >= THERMAL_DEV_NUM_MAX || g_tz_devs[id] == NULL || temp == NULL) {
		pr_warn("invalid thermal device id or temp\n");
		return -EINVAL;
	}
	tz_dev = g_tz_devs[id];
	err = thermal_zone_get_temp(tz_dev, &tmp);
	if (err < 0) {
		pr_warn("get thermal zone temp failed\n");
		return err;
	}
	*temp = tmp;

	return 0;
}

static const struct devhost_plat_pm_ops plat_pm_ops = {
	.power_off = platform_power_off,
	.reboot = platform_reboot,
	.prepare = platform_prepare,
	.syscore_shutdown = platform_syscore_shutdown,
	.wq_freeze = platform_wq_freeze,
	.wq_thaw = platform_wq_thaw,
	.suspend = platform_suspend,
	.cpu_add = platform_cpu_add,
	.cpu_remove = platform_cpu_remove,
	.wakelock_stat = platform_wakelock_stat,
	.idle_enter = platform_idle_enter,
	.idle_exit = platform_idle_exit,
	.hibernate = platform_hibernate,
	.dubai_hwlog = platform_dubai_hwlog,
	.thermal_device_acquire = platform_thermal_device_acquire,
	.thermal_device_temp_read = platform_thermal_device_temp_read,
	.preempt_bypass = platform_preempt_bypass,
};

static void __handle_wl_async_work(struct work_struct *work)
{
	struct wl_async_work *awork = NULL;

	if (work == NULL) {
		pr_err("wakelock: invalid work\n");
		return;
	}
	awork = container_of(work, struct wl_async_work, work);

	if (awork->is_activate) {
		if (devhost_wakelock_acquire() != 0) {
			pr_err("acquire devhost wakelock failed\n");
		}
	} else {
		if (devhost_wakelock_release() != 0) {
			pr_err("release devhost wakelock failed\n");
		}
	}
	kfree(awork);

	return;
}

void wl_async_schedule(bool is_activate)
{
	struct wl_async_work *awork = NULL;

	awork = (struct wl_async_work *)kmalloc(sizeof(*awork), GFP_ATOMIC);
	if (awork == NULL) {
		pr_err("wakelock: alloc async work failed\n");
		return;
	}

	INIT_WORK(&awork->work, __handle_wl_async_work);
	awork->is_activate = is_activate;

	queue_work(wakelock_wq, &awork->work);

	return;
}
#if defined(CONFIG_VENDOR_HOOKS) && defined(CONFIG_PM_SLEEP)
static void hook_ldk_rvh_wakeup_source_activate(void *__data, bool is_activate, split_counters_t split_counters_f)
{
	unsigned int cnt = 0;
	unsigned int inpr = 0;

	split_counters_f(&cnt, &inpr);
	if (is_activate && inpr == 1) {
		wl_async_schedule(is_activate);
	}

	if (!is_activate && inpr == 0) {
		wl_async_schedule(is_activate);
	}
}
#endif

int devhost_wakelock_wq_init(void)
{
	wakelock_wq = alloc_ordered_workqueue("wakelock_rq", WQ_HIGHPRI);
	if (wakelock_wq == NULL) {
		return -ENOMEM;
	}

	return 0;
}

int devhost_power_init(void)
{
	int ret;
#if defined(CONFIG_VENDOR_HOOKS) && defined(CONFIG_PM_SLEEP)
	int rc = 0;

	rc = register_trace_ldk_rvh_wakeup_source_activate(hook_ldk_rvh_wakeup_source_activate, NULL);
	if (rc) {
		pr_warn("hooks, liblinux: register_trace_ldk_rvh_wakeup_source failed, rc=%d\n", rc);
	}
#endif
	ret = devhost_pmqos_init();
	if (ret < 0) {
		pr_warn("devhost pmqos init failed, err=%d\n", ret);
		return ret;
	}
	ret = devhost_wakelock_wq_init();
	if (ret < 0) {
		pr_warn("devhost wakelock workqueue init failed\n");
		return ret;
	}

	return devhost_pm_init(&plat_pm_ops);
}
