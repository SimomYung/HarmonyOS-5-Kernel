/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK pm sysif
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 14 10:03:27 2023
 */

#include <libstrict/strict.h>
#include <libsysif/devhost/server.h>
#include <libdevhost/devhost.h>
#include <libhmsync/raw_rwlock.h>

#include <devhost/pm.h>
#include <udk/power.h>

#include "pm.h"
#include "fusion.h"

static struct raw_rwlock g_fusion_pm_lock = RAW_RDLOCK_INITIALIZER;

static vatomic32_t g_hibernate_atomic = VATOMIC_INIT(0);

static inline void udk_fusion_pm_wrlock(void)
{
	(void)raw_rwlock_wrlock(&g_fusion_pm_lock);
}

void udk_fusion_pm_rdlock(void)
{
	(void)raw_rwlock_rdlock(&g_fusion_pm_lock);
}

void udk_fusion_pm_unlock(void)
{
	(void)raw_rwlock_unlock(&g_fusion_pm_lock);
}

static bool udk_hibernate_acquire(void)
{
	return !vatomic32_cmpxchg(&g_hibernate_atomic, 0, 1);
}

static void udk_hibernate_release(void)
{
	vatomic32_write(&g_hibernate_atomic, 0);
}

const struct devhost_plat_pm_ops *pmops __read_mostly;
int udk_fusion_pm_init(const struct devhost_plat_pm_ops *plat_pm_ops)
{
	if (pmops != NULL) {
		return E_HM_OBJEXIST;
	}

	pmops = plat_pm_ops;
	return 0;
}

static int __udk_pm_suspend_prepare(bool force)
{
	UNUSED(force);

	return 0;
}

static int __udk_pm_suspend(void)
{
	int ret = 0;

	if ((pmops != NULL) && (pmops->suspend != NULL)) {
		udk_fusion_pm_wrlock();
		ret = pmops->suspend(UDK_PM_SUSPEND);
		if (ret != 0) {
			udk_fusion_pm_unlock();
		}
	}

	return ret;
}

static int __udk_pm_resume(void)
{
	int ret = 0;

	if ((pmops != NULL) && (pmops->suspend != NULL)) {
		ret = pmops->suspend(UDK_PM_RESUME);
		udk_fusion_pm_unlock();
	}

	return ret;
}

static int __udk_pm_resume_finish(void)
{
	return 0;
}

static int __udk_pm_syscore_shutdown(void)
{
	if ((pmops != NULL) && (pmops->syscore_shutdown != NULL)) {
		pmops->syscore_shutdown();
	}

	return 0;
}

static int __udk_pm_dev_shutdown(void)
{
	udk_fusion_pm_wrlock();
	udk_pm_dev_shutdown();

	return 0;
}

static int __udk_pm_hibernate_prepare(void)
{
	int err;

	if (!udk_hibernate_acquire()) {
		return -EBUSY;
	}

	err = udk_pm_notifier_call_robust(UDK_PM_HIBERNATE_PREPARE, UDK_PM_HIBERNATE_POST);
	err = UDK_NOTIFY_TO_ERR(err);
	if (err) {
		udk_hibernate_release();
	}

	return err;
}

static int __udk_pm_hibernate_post(void)
{
	udk_hibernate_release();
	return udk_pm_notifier_call(UDK_PM_HIBERNATE_POST);
}

static int __udk_pm_hibernate_restore_prepare(void)
{
	int err;

	if (!udk_hibernate_acquire()) {
		return -EBUSY;
	}

	err = udk_pm_notifier_call_robust(UDK_PM_HIBERNATE_RESTORE_PREPARE, UDK_PM_HIBERNATE_POST_RESTORE);
	err = UDK_NOTIFY_TO_ERR(err);
	if (err) {
		udk_hibernate_release();
	}

	return err;
}

static int __udk_pm_hibernate_post_restore(void)
{
	udk_hibernate_release();
	return udk_pm_notifier_call(UDK_PM_HIBERNATE_POST_RESTORE);
}

static int __udk_pm_hibernate(int action)
{
	if ((pmops != NULL) && (pmops->syscore_shutdown != NULL)) {
		pmops->hibernate(action);
	}
	return 0;
}

static int __udk_pm_action(unsigned int action)
{
	switch (action) {
	case DH_PM_SUSPEND_PREPARE_FORCE:
		return __udk_pm_suspend_prepare(true);
	case DH_PM_SUSPEND_PREPARE_NORMAL:
		return __udk_pm_suspend_prepare(false);
	case DH_PM_SUSPEND:
		return __udk_pm_suspend();
	case DH_PM_RESUME:
		return __udk_pm_resume();
	case DH_PM_RESUME_FINISH:
		return __udk_pm_resume_finish();
	case DH_PM_SYSCORE_SHUTDOWN:
		return __udk_pm_syscore_shutdown();
	case DH_PM_DEV_SHUTDOWN:
		return __udk_pm_dev_shutdown();
	case DH_PM_SUSPEND_DISK_PREPARE:
		return __udk_pm_hibernate_prepare();
	case DH_PM_SUSPEND_DISK_POST:
		return __udk_pm_hibernate_post();
	case DH_PM_SUSPEND_DISK_RESTORE_PREPARE:
		return __udk_pm_hibernate_restore_prepare();
	case DH_PM_SUSPEND_DISK_POST_RESTORE:
		return __udk_pm_hibernate_post_restore();
	case DH_PM_SUSPEND_DISK_FREEZE:
	case DH_PM_SUSPEND_DISK_QUIESCE:
		return __udk_pm_hibernate(UDK_PM_HIBERNATE_FREEZE);
	case DH_PM_SUSPEND_DISK_THAW:
	case DH_PM_SUSPEND_DISK_RECOVER:
		return __udk_pm_hibernate(UDK_PM_HIBERNATE_THAW);
	case DH_PM_SUSPEND_DISK_RESTORE:
		return  __udk_pm_hibernate(UDK_PM_HIBERNATE_RESTORE);
	default:
		/* do noting, return success */
		return 0;
	}
}

int udk_pm_action(unsigned int action)
{
	int err = __udk_pm_action(action);

	return posix2hmerrno(-err);
}

static int __udk_cpu_add(unsigned int cpu)
{
	int ret = 0;

	if ((pmops != NULL) && (pmops->cpu_add != NULL)) {
		ret = pmops->cpu_add(cpu);
	}

	return posix2hmerrno(-ret);
}


static int __udk_cpu_remove(unsigned int cpu)
{
	int ret = 0;

	if ((pmops != NULL) && (pmops->cpu_remove != NULL)) {
		ret = pmops->cpu_remove(cpu);
	}

	return posix2hmerrno(-ret);
}

static int __udk_pm_cpuhp_action(unsigned int cpu, unsigned int action)
{
	int err = E_HM_INVAL;

	switch (action) {
	case DH_PM_CPU_ADD:
		err = __udk_cpu_add(cpu);
		break;
	case DH_PM_CPU_REMOVE:
		err = __udk_cpu_remove(cpu);
		break;
	default:
		hm_warn("invalid pm cpu action=%u\n", action);
		break;
	}

	return err;
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_pm_action,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, action)
{
	UNUSED(sender, credential);
	if (!udk_fusion_cnode_is_devmgr()) {
		return E_HM_PERM;
	}

	return __udk_pm_action(action);
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_pm_cpuhp_action,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, cpu, unsigned int, action)
{
	UNUSED(sender, credential);
	if (!udk_fusion_cnode_is_devmgr()) {
		return E_HM_PERM;
	}

	return __udk_pm_cpuhp_action(cpu, action);
}

bool udk_fusion_cnode_is_devmgr(void)
{
	return sysif_actv_src_cnode_idx() == libdh_get_devmgr_cnode_idx();
}
