/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Power management support
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 12 21:09:12 2019
 */
#include "devhost_pm.h"

#include <lib/dlist.h>
#include <lib/dlist_ext.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/wakelock.h>
#include <libhmpm/wl_stat.h>
#include <libhmpm/pmqos.h>
#include <libhmpm/pm_init_event.h>
#include <libhmpm/dfc.h>
#include <libsysif/pwrmgr/api.h>
#include <libsysif/freqmgr/api.h>
#include <librb/shm_ring_buffer.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>
#include <libpreempt/preempt.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_sysctrl.h>

#include "types.h"
#include "devhost.h"
#include "devhost_api.h"

#ifdef CONFIG_PM_WAKELOCK
#define DH_PM_WAKELOCK_NAME_MAX		17
static char devhost_pm_wakelock_name[DH_PM_WAKELOCK_NAME_MAX];
#endif

struct dev_pm_entry {
	struct dlist_node node;
	struct device_info *dev;
};

static DLIST_HEAD(dev_pm_list);
static struct raw_mutex devhost_pm_mutex = RAW_MUTEX_INITIALIZER;
static const struct devhost_plat_pm_ops *devhost_plat_pm_ops;
static rref_t pwrmgr_rref = 0ULL;
static rref_t freqmgr_rref = 0ULL;

static __thread bool preempt_bypass = false;

#ifdef CONFIG_DEVHOST_PM_PLUGIN
PUBLIC_SYMBOL
int devhost_register_plat_pm(const struct devhost_plat_pm_ops *plat_pm_ops)
{
	if (plat_pm_ops == NULL) {
		return -EINVAL;
	}
	/* platform pm ops should be registered only once */
	if (devhost_plat_pm_ops != NULL) {
		return -EEXIST;
	}

	devhost_plat_pm_ops = plat_pm_ops;

	return 0;
}

#ifdef CONFIG_PM_WAKELOCK
static void wakelock_init(void);
#endif

PUBLIC_SYMBOL
int devhost_pm_init(const struct devhost_plat_pm_ops *plat_pm_ops)
{
#ifdef CONFIG_PM_WAKELOCK
	wakelock_init();
#endif
	return devhost_register_plat_pm(plat_pm_ops);
}

#else

PUBLIC_SYMBOL
int devhost_register_plat_pm(const struct devhost_plat_pm_ops *plat_pm_ops)
{
	UNUSED(plat_pm_ops);
	return E_HM_OK;
}

PUBLIC_SYMBOL
int devhost_pm_init(const struct devhost_plat_pm_ops *plat_pm_ops)
{
	UNUSED(plat_pm_ops);
	return E_HM_OK;
}
#endif /* CONFIG_DEVHOST_PM_PLUGIN */

int devhost_pm_add_device(struct device_info *dev)
{
	struct dev_pm_entry *entry = NULL;

	if (dev == NULL) {
		hm_error("invalid device info\n");
		return E_HM_INVAL;
	}
	entry = (struct dev_pm_entry *)malloc(sizeof(struct dev_pm_entry));
	if (entry == NULL) {
		hm_error("malloc dev_pm_entry failed\n");
		return E_HM_NOMEM;
	}
	mem_zero_s(*entry);
	entry->dev = dev;
	RAW_MUTEX_GUARD(_, &devhost_pm_mutex);
	/*
	 * since power dependancy may exist between devices, the devices
	 * pm list order is relied on the devices power tree, root power
	 * node device will be added firstly, until tip power node device.
	 */
	(void)devinfo_get(dev);
	dlist_insert_tail(&dev_pm_list, &entry->node);

	return E_HM_OK;
}

int devhost_pm_remove_device(const struct device_info *dev)
{
	struct dev_pm_entry *pos = NULL;
	struct dev_pm_entry *n = NULL;
	bool found = false;

	if (dev == NULL) {
		hm_error("invalid device info\n");
		return E_HM_INVAL;
	}
	RAW_MUTEX_GUARD(_, &devhost_pm_mutex);
	dlist_for_each_entry_safe(pos, n, &dev_pm_list,
				  struct dev_pm_entry, node) {
		if ((pos != NULL) && (pos->dev == dev)) {
			dlist_delete(&pos->node);
			devinfo_put(pos->dev);
			free(pos);
			found = true;
			break;
		}
	}
	if (!found) {
		return E_HM_NODEV;
	}

	return E_HM_OK;
}

static int devices_syscore_shutdown(void)
{
	if ((devhost_plat_pm_ops == NULL) ||
	    (devhost_plat_pm_ops->syscore_shutdown == NULL)) {
		return E_HM_OK;
	}

	hm_info("device syscore shutdown\n");
	devhost_plat_pm_ops->syscore_shutdown();

	return E_HM_OK;
}

static int devices_shutdown(void)
{
	struct dev_pm_entry *pos = NULL;
	int err = E_HM_OK;

	raw_mutex_lock(&devhost_pm_mutex);
	hm_info("device shutdown\n");
	/*
	 * since power dependancy may exist between devices, the devices pm
	 * list order is relied on the devices power tree, so reverse traverse
	 * devices pm list from tip power node device to root power node
	 * device to do shut down call back.
	 */
	pos = dlist_last_entry(&dev_pm_list, struct dev_pm_entry, node);
	while (&(pos->node) != &dev_pm_list) {
		struct device_info *dev = pos->dev;

		/*
		 * Since some device will be removed while shutdown, unlock pm_mutex
		 * and remove the devices from list to avoid deadlock.
		 */
		dlist_delete(&pos->node);
		free(pos);
		raw_mutex_unlock(&devhost_pm_mutex);

		if ((dev != NULL) && (dev->p->pmops != NULL) &&
		    (dev->p->pmops->shutdown != NULL)) {
			err = dev->p->pmops->shutdown(dev);
			if (err < 0) {
				err = posix2hmerrno(-err);
				hm_error("device shutdown failed, err=%s, "
					 "devid=%d\n",
					 hmstrerror(err), dev->devid);
			}
		}
		if (dev != NULL) {
			devinfo_put(dev);
		}

		raw_mutex_lock(&devhost_pm_mutex);
		pos = dlist_last_entry(&dev_pm_list, struct dev_pm_entry, node);
	}

	raw_mutex_unlock(&devhost_pm_mutex);
	return E_HM_OK;
}

static void devhost_preempt_bypass_enable(void)
{
	preempt_bypass = true;
	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->preempt_bypass != NULL)) {
		devhost_plat_pm_ops->preempt_bypass(true);
	}
}

static void devhost_preempt_bypass_disable(void)
{
	preempt_bypass = false;
	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->preempt_bypass != NULL)) {
		devhost_plat_pm_ops->preempt_bypass(false);
	}
}

#if defined(CONFIG_PM_SUSPEND) || defined(CONFIG_PM_HIBERNATE)
static int platform_wq_freeze(void)
{
	int ret = 0;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->wq_freeze != NULL)) {
		ret = devhost_plat_pm_ops->wq_freeze();
	}

	return posix2hmerrno(-ret);
}

static void platform_wq_thaw(void)
{
	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->wq_thaw != NULL)) {
		devhost_plat_pm_ops->wq_thaw();
	}
}
#endif

#ifdef CONFIG_PM_SUSPEND
static int platform_suspend(unsigned int action)
{
	int ret = 0;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->suspend != NULL)) {
		ret = devhost_plat_pm_ops->suspend(action);
	}

	return posix2hmerrno(-ret);
}

static int platform_suspend_preempt(unsigned int action)
{
	int ret = 0;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->suspend != NULL)) {
		devhost_preempt_bypass_enable();
		ret = devhost_plat_pm_ops->suspend(action);
		devhost_preempt_bypass_disable();
	}

	return posix2hmerrno(-ret);
}
#endif

static int platform_idle_prepare(void)
{
	/*
	 * touch the memory that the platform_idle_enter/exit will use,
	 * to avoid the platform_idle_enter/exit causing page fault
	 */
	devhost_preempt_bypass_enable();
	devhost_preempt_bypass_disable();

	return E_HM_OK;
}

static int platform_idle_enter(void)
{
	int ret = -ENOSYS;
	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->idle_enter != NULL)) {
		devhost_preempt_bypass_enable();
		ret = devhost_plat_pm_ops->idle_enter();
		devhost_preempt_bypass_disable();
	}

	return posix2hmerrno(-ret);
}

static int platform_idle_exit(void)
{
	int ret = -ENOSYS;
	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->idle_exit != NULL)) {
		devhost_preempt_bypass_enable();
		ret = devhost_plat_pm_ops->idle_exit();
		devhost_preempt_bypass_disable();
	}

	return posix2hmerrno(-ret);
}

#ifdef CONFIG_PM_HIBERNATE
static int platform_hibernate(unsigned int action)
{
	int ret = 0;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->hibernate!= NULL)) {
		ret = devhost_plat_pm_ops->hibernate(action);
	}

	return posix2hmerrno(-ret);
}
#endif

int devhost_pm_action(unsigned int action)
{
	int err = E_HM_OK;

	switch (action) {
	case DH_PM_DEV_SHUTDOWN:
		err = devices_shutdown();
		break;
	case DH_PM_SYSCORE_SHUTDOWN:
		err = devices_syscore_shutdown();
		break;
	case DH_PM_IDLE_PREPARE:
		err = platform_idle_prepare();
		break;
	case DH_PM_IDLE_ENTER:
		err = platform_idle_enter();
		break;
	case DH_PM_IDLE_EXIT:
		err = platform_idle_exit();
		break;
#if defined(CONFIG_PM_SUSPEND) || defined(CONFIG_PM_HIBERNATE)
	case DH_PM_WQ_FREEZE:
		err = platform_wq_freeze();
		break;
	case DH_PM_WQ_THAW:
		platform_wq_thaw();
		break;
#endif
#ifdef CONFIG_PM_SUSPEND
	case DH_PM_SUSPEND_PREPARE_FORCE:
		err = platform_suspend(DH_PM_EVENT_SUSPEND_PREPARE_FORCE);
		break;
	case DH_PM_SUSPEND_PREPARE_NORMAL:
		err = platform_suspend(DH_PM_EVENT_SUSPEND_PREPARE_NORMAL);
		break;
	case DH_PM_SUSPEND:
		err = platform_suspend(DH_PM_EVENT_SUSPEND);
		break;
	case DH_PM_RESUME:
		err = platform_suspend(DH_PM_EVENT_RESUME);
		break;
	case DH_PM_RESUME_FINISH:
		err = platform_suspend(DH_PM_EVENT_RESUME_FINISH);
		break;
	case DH_PM_SUSPEND_LATE:
		err = platform_suspend_preempt(DH_PM_EVENT_SUSPEND_LATE);
		break;
	case DH_PM_RESUME_EARLY:
		err = platform_suspend_preempt(DH_PM_EVENT_RESUME_EARLY);
		break;
	case DH_PM_ULSR_ENTER:
		err = platform_suspend(DH_PM_EVENT_ULSR_ENTER);
		break;
	case DH_PM_ULSR_LEAVE:
		err = platform_suspend(DH_PM_EVENT_ULSR_LEAVE);
		break;
#endif
#ifdef CONFIG_PM_HIBERNATE
	case DH_PM_SUSPEND_DISK_PREPARE:
		err = platform_hibernate(DH_PM_EVENT_PREPARE);
		break;
	case DH_PM_SUSPEND_DISK_POST:
		err = platform_hibernate(DH_PM_EVENT_POST);
		break;
	case DH_PM_SUSPEND_DISK_RESTORE_PREPARE:
		err = platform_hibernate(DH_PM_EVENT_RESTORE_PREPARE);
		break;
	case DH_PM_SUSPEND_DISK_POST_RESTORE:
		err = platform_hibernate(DH_PM_EVENT_POST_RESTORE);
		break;
	case DH_PM_SUSPEND_DISK_FREEZE:
		err = platform_hibernate(DH_PM_EVENT_FREEZE);
		break;
	case DH_PM_SUSPEND_DISK_QUIESCE:
		err = platform_hibernate(DH_PM_EVENT_QUIESCE);
		break;
	case DH_PM_SUSPEND_DISK_THAW:
		err = platform_hibernate(DH_PM_EVENT_THAW);
		break;
	case DH_PM_SUSPEND_DISK_RECOVER:
		err = platform_hibernate(DH_PM_EVENT_RECOVER);
		break;
	case DH_PM_SUSPEND_DISK_RESTORE:
		err = platform_hibernate(DH_PM_EVENT_RESTORE);
		break;
#endif
	default:
		err = E_HM_INVAL;
		hm_warn("invalid pm action=%u\n", action);
		break;
	}

	return err;
}

int devhost_pm_dubai_hwlog(struct pm_dubai_ctrl_log *log_info)
{
	int ret = -ENOSYS;

	if (log_info == NULL) {
		hm_error("invalid dubai log info\n");
		return E_HM_INVAL;
	}
	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->dubai_hwlog != NULL)) {
		ret = devhost_plat_pm_ops->dubai_hwlog(log_info->log_lv, log_info->tag, log_info->msg);
	}

	return posix2hmerrno(-ret);
}

int devhost_pm_prepare(unsigned int type, char *cmd)
{
	int ret = E_HM_OK;

	if ((devhost_plat_pm_ops == NULL) ||
	    (devhost_plat_pm_ops->prepare == NULL)) {
		return E_HM_OK;
	}

	ret = devhost_plat_pm_ops->prepare(type, cmd);

	return posix2hmerrno(-ret);
}

int devhost_plat_power_off(void)
{
	int ret = E_HM_OK;
	/*
	 * If devhost can't handle power_off, return E_HM_OK directly.
	 * Tppmgr will look for other ways to handle power_off.
	 */
	if ((devhost_plat_pm_ops == NULL) ||
	    (devhost_plat_pm_ops->power_off == NULL)) {
		return E_HM_OK;
	}
	ret = devhost_plat_pm_ops->power_off();

	return posix2hmerrno(-ret);
}

int devhost_plat_reboot(const char *cmd)
{
	int ret = E_HM_OK;
	/*
	 * If devhost can't handle reboot, return E_HM_OK directly.
	 * Tppmgr will look for other ways to handle reboot.
	 */
	if ((devhost_plat_pm_ops == NULL) ||
	    (devhost_plat_pm_ops->reboot == NULL)) {
		return E_HM_OK;
	}
	ret = devhost_plat_pm_ops->reboot(cmd);

	return posix2hmerrno(-ret);
}

static int devhost_pm_normal_uref_register(rref_t pwrmgr_rref,
					   struct sysarg_cnode_statrref *stat, uref_t *dh_uref)
{
	int ret;

	*dh_uref = libdh_ap_export(stat->src_cnode_idx);
	if (IS_REF_ERR(*dh_uref)) {
		ret = REF_TO_ERR(*dh_uref);
		hm_error("export actv pool to pwrmgr failed: %s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvcapcall_pmcall_dh_register(pwrmgr_rref, *dh_uref, DH_UREF_NORMAL);
	if (ret != E_HM_OK) {
		hm_error("export uref to pwrmgr failed: %s\n", hmstrerror(ret));
		(void)hm_ucap_revoke(0, *dh_uref);
		return ret;
	}

	return E_HM_OK;
}

static int devhost_pm_populated_uref_register(rref_t pwrmgr_rref, struct sysarg_cnode_statrref *stat)
{
	int ret;
	cref_t dh_pm_ap = 0uLL;
	uref_t dh_pmap_uref;
	struct __actvpool_grant grant;

	dh_pm_ap = dh_pm_ap_cref_of();
	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;

	dh_pmap_uref = hm_ucap_export(0, (uint32_t)stat->src_cnode_idx,
				      /* flags= */
				      CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
				      CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX,
				      dh_pm_ap,
				      /* rights= */
				      CRIGHT_ACTVPOOL_CALL,
				      /* args= */
				      ptr_to_ulong(&grant));
	if (IS_REF_ERR(dh_pmap_uref)) {
		ret = REF_TO_ERR(dh_pmap_uref);
		hm_error("export actv pool to pwrmgr failed: %s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvcapcall_pmcall_dh_register(pwrmgr_rref, dh_pmap_uref, DH_UREF_POPULATED);
	if (ret != E_HM_OK) {
		hm_error("export pm ap uref to pwrmgr failed: %s\n", hmstrerror(ret));
		(void)hm_ucap_revoke(0, dh_pmap_uref);
		return ret;
	}

	return E_HM_OK;
}

int devhost_pm_register(void)
{
	int ret;
	uref_t dh_uref = 0ULL;
	struct sysarg_cnode_statrref stat = { 0 };

	if (pwrmgr_rref == 0ULL) {
		ret = hm_path_acquire(PWRMGR_PATH, &pwrmgr_rref);
		if (ret != E_HM_OK) {
			hm_error("acquire pwrmgr path failed\n");
			return ret;
		}
	}

	ret = sysfast_cnode_stat_rref(&stat, pwrmgr_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret != E_HM_OK) {
		(void)hm_path_release(PWRMGR_PATH, pwrmgr_rref);
		hm_error("stat pwrmgr rref failed: %s\n", hmstrerror(ret));
		return ret;
	}

	ret = devhost_pm_normal_uref_register(pwrmgr_rref, &stat, &dh_uref);
	if (ret != E_HM_OK) {
		(void)hm_path_release(PWRMGR_PATH, pwrmgr_rref);
		return ret;
	}

	ret = devhost_pm_populated_uref_register(pwrmgr_rref, &stat);
	if (ret != E_HM_OK) {
		(void)hm_path_release(PWRMGR_PATH, pwrmgr_rref);
		(void)hm_ucap_revoke(0, dh_uref);
		return ret;
	}

	return E_HM_OK;
}

bool is_pwrmgr_ready(void)
{
	int err;
	rref_t pwrmgr_path_rref;

	err = hm_path_acquire(PWRMGR_PATH, &pwrmgr_path_rref);
	if (err == E_HM_OK) {
		(void)hm_path_release(PWRMGR_PATH, pwrmgr_path_rref);
		return true;
	} else {
		hm_debug("pwrmgr path acquire failed: %s\n", hmstrerror(err));
		return false;
	}
}

#ifdef CONFIG_PM_WAKELOCK

static inline char *get_wakelock_name(void)
{
	if (devhost_pm_wakelock_name[0] == '\0') {
		hm_error("invalid wake lock name\n");
		return NULL;
	}

	return devhost_pm_wakelock_name;
}

PUBLIC_SYMBOL
int devhost_wakelock_acquire(void)
{
	char *wakelock_name = NULL;

	wakelock_name = get_wakelock_name();
	if (wakelock_name == NULL) {
		return E_HM_INVAL;
	}

	return hm_wake_lock_acquire(wakelock_name);
}

PUBLIC_SYMBOL
int devhost_wakelock_release(void)
{
	char *wakelock_name = NULL;

	wakelock_name = get_wakelock_name();
	if (wakelock_name == NULL) {
		return E_HM_INVAL;
	}

	return hm_wake_lock_release(wakelock_name);
}

static void wakelock_init(void)
{
	int err;
	__u32 idx;

	/* init wakelock name */
	mem_zero_a(devhost_pm_wakelock_name);
	idx = hm_ucap_self_cnode_idx();
	err = snprintf_s(devhost_pm_wakelock_name, DH_PM_WAKELOCK_NAME_MAX,
			 DH_PM_WAKELOCK_NAME_MAX, "%s:%08x", WL_DEVHOST_NAME, idx);
	BUG_ON(err <= 0);

	/* init wake lock */
	err = hm_wake_lock_add(devhost_pm_wakelock_name);
	if (err != E_HM_OK) {
		hm_error("hm_wake_lock_add failed: %s\n", hmstrerror(err));
		return;
	}
}

static int wl_stat_enqueue(struct wl_stat_queue *queue, const struct wl_stat *stat)
{
	int ret;
	struct wl_stat *wl_stat = NULL;

	if (queue->total_cnt >= queue->size) {
		/* could not push any more */
		hm_error("the size of wl stat queue is not enough\n");
		return E_HM_OVERFLOW;
	}

	wl_stat = (struct wl_stat *)ptr_add(queue->base_addr,
					    sizeof(struct wl_stat) * queue->total_cnt);
	ret = memcpy_s(wl_stat, sizeof(struct wl_stat), stat, sizeof(struct wl_stat));
	if (ret != 0) {
		hm_error("memcpy wl stat failed\n");
		return E_HM_INVAL;
	}

	ret = sring_buf_push(queue->sring, wl_stat);
	if (ret != E_HM_OK) {
		hm_error("wl stat enqueue failed\n");
		return ret;
	}
	queue->total_cnt++;

	return 0;
}

PUBLIC_SYMBOL
int devhost_wakelock_stat_enqueue(const struct wl_stat *stat, void *args)
{
	struct wl_stat_queue *queue = (struct wl_stat_queue *)args;

	if (stat == NULL || queue == NULL) {
		hm_warn("wakelock stat enqueue parameters is invalid\n");
		return E_HM_INVAL;
	}

	return wl_stat_enqueue(queue, stat);
}

static int platform_wl_stat(struct wl_stat_queue *queue)
{
	int ret = E_HM_NOSYS;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->wakelock_stat != NULL)) {
		ret = devhost_plat_pm_ops->wakelock_stat((void *)queue);
	}

	return ret;
}

static int wl_stat_queue_init(struct wl_stat_queue *queue,
			      struct wl_stat_queue_info *queue_info)
{
	int ret;
	struct sring_buf *srbuf = NULL;

	srbuf = sring_buf_create_from_fd(queue_info->ring_key, true,
					 queue_info->cnode_idx);
	if (srbuf == NULL) {
		hm_error("create sring buf from ring key failed\n");
		return E_HM_INVAL;
	}

	ret = sring_buf_connect(srbuf, true);
	if (ret != E_HM_OK) {
		hm_error("connect srbuf failed\n");
		sring_buf_destroy(srbuf, true, false);
		free(srbuf);
		return ret;
	}
	queue->sring = srbuf;
	queue->base_addr = srbuf->data_vaddr;

	return E_HM_OK;
}

static struct wl_stat_queue *wl_stat_queue_create(struct wl_stat_queue_info *queue_info,
						  size_t cnt)
{
	int ret;
	struct wl_stat_queue *queue = NULL;

	queue = (struct wl_stat_queue *)malloc(sizeof(struct wl_stat_queue));
	if (queue == NULL) {
		hm_error("malloc devhost wl stat queue failed\n");
		return NULL;
	}
	queue->size = queue_info->size;
	queue->total_cnt = cnt;

	ret = wl_stat_queue_init(queue, queue_info);
	if (ret != E_HM_OK) {
		free(queue);
		return NULL;
	}

	return queue;
}

static int wl_stat_enqueue_dh_info(struct wl_stat_queue *queue)
{
	int ret;
	struct wl_stat dh_info;

	mem_zero_s(dh_info);
	ret = memcpy_s(dh_info.name, sizeof(dh_info.name),
		       devhost_pm_wakelock_name, DH_PM_WAKELOCK_NAME_MAX);
	if (ret != 0) {
		hm_error("memcpy dh wakelock name failed\n");
		return E_HM_INVAL;
	}

	return wl_stat_enqueue(queue, &dh_info);
}

static void wl_stat_queue_destroy(struct wl_stat_queue *queue)
{
	sring_buf_disconnect(queue->sring, true);
	sring_buf_destroy(queue->sring, true, false);
	free(queue->sring);
	free(queue);
}

int devhost_pm_wl_stat(struct wl_stat_queue_info *queue_info, size_t cnt,
		       size_t *enqueue_cnt)
{
	int ret;
	struct wl_stat_queue *queue = NULL;

	queue = wl_stat_queue_create(queue_info, cnt);
	if (queue == NULL) {
		return E_HM_INVAL;
	}
	/* enqueue a devhost stat as a mark */
	ret = wl_stat_enqueue_dh_info(queue);
	if (ret != E_HM_OK) {
		goto out;
	}
	ret = platform_wl_stat(queue);
	if (ret != E_HM_OK) {
		goto out;
	}
	/* enqueue_cnt is the number of wl_stat pushed in this devhost */
	*enqueue_cnt = queue->total_cnt - cnt;
out:
	wl_stat_queue_destroy(queue);

	return ret;
}

#else

PUBLIC_SYMBOL
int devhost_wakelock_acquire(void)
{
	return 0;
}

PUBLIC_SYMBOL
int devhost_wakelock_release(void)
{
	return 0;
}

PUBLIC_SYMBOL
int devhost_wakelock_stat_enqueue(const struct wl_stat *stat, void *args)
{
	UNUSED(stat, args);
	return 0;
}

int devhost_pm_wl_stat(struct wl_stat_queue_info *queue_info, size_t cnt,
		       size_t *enqueue_cnt)
{
	UNUSED(queue_info, cnt, enqueue_cnt);
	return 0;
}

#endif /* CONFIG_PM_WAKELOCK */

static int platform_cpu_add(unsigned int cpu)
{
	int ret = -ENOSYS;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->cpu_add != NULL)) {
		ret = devhost_plat_pm_ops->cpu_add(cpu);
	}

	return posix2hmerrno(-ret);
}

static int platform_cpu_remove(unsigned int cpu)
{
	int ret = -ENOSYS;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->cpu_remove != NULL)) {
		ret = devhost_plat_pm_ops->cpu_remove(cpu);
	}

	return posix2hmerrno(-ret);
}

PUBLIC_SYMBOL
bool devhost_preempt_bypassed(void)
{
	return preempt_bypass;
}

int devhost_pm_cpuhp_action(unsigned int cpu, unsigned int action)
{
	int err = E_HM_INVAL;

	switch (action) {
	case DH_PM_CPU_ADD:
		err = platform_cpu_add(cpu);
		break;
	case DH_PM_CPU_REMOVE:
		err = platform_cpu_remove(cpu);
		break;
	default:
		hm_warn("invalid pm cpu action=%u\n", action);
		break;
	}

	return err;
}

static inline bool is_type_valid(unsigned int type)
{
	if (type >= (unsigned int)PMQOS_TYPE_NR) {
		return false;
	}

	return true;
}

static inline bool is_request_valid(unsigned int type, struct hm_pmqos_request *request)
{
	if (!is_type_valid(type) || (request->id <= 0)) {
		hm_warn("invalid type or id\n");
		return false;
	}

	return true;
}

PUBLIC_SYMBOL
int devhost_pmqos_request_add(int type, int value, int *id)
{
	int ret;
	struct hm_pmqos_request request = {
		.type = type,
		.id = 0,
	};

	if ((id == NULL) || !is_type_valid((unsigned int)type)) {
		hm_warn("invalid type or id is NULL\n");
		return -hmerrno2posix(E_HM_INVAL);
	}
	ret = hm_pmqos_request_add(&request, value);
	if (ret == E_HM_NOSYS) {
		hm_debug("devhost_pmqos_request_add unsupported\n");
		ret = E_HM_OK;
	}
	if (ret == E_HM_OK) {
		*id = request.id;
	}

	return -hmerrno2posix(ret);
}

PUBLIC_SYMBOL
int devhost_pmqos_request_update(int type, int id, int value)
{
	struct hm_pmqos_request request = {
		.type = type,
		.id = id,
	};

	if (!is_request_valid((unsigned int)type, &request)) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	return -hmerrno2posix(hm_pmqos_request_update(&request, value));
}

PUBLIC_SYMBOL
int devhost_pmqos_request_remove(int type, int id)
{
	struct hm_pmqos_request request = {
		.type = type,
		.id = id,
	};

	if (!is_request_valid((unsigned int)type, &request)) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	return -hmerrno2posix(hm_pmqos_request_remove(&request));
}

static int cpufreq_limit_handle(unsigned int cpuid,
				unsigned int target_freq,
				unsigned int limit_type,
				enum dfc_constraint_reason reason)
{
	int err = E_HM_OK;

	if (freqmgr_rref == 0ULL) {
		err = hm_path_acquire(FREQMGR_PATH, &freqmgr_rref);
		if (err != E_HM_OK) {
			hm_error("acquire freqmgr path failed\n");
			return -hmerrno2posix(err);
		}
	}
	err = actvcapcall_fmcall_dfc_cpu_freq_limit(freqmgr_rref, cpuid, target_freq, limit_type, reason);
	if (err != E_HM_OK) {
		hm_error("failed to set target freq: %u to cpu: %u, reason: %u\n", target_freq, cpuid, reason);
	}

	return -hmerrno2posix(err);
}

PUBLIC_SYMBOL
int devhost_pm_set_min_cpufreq(unsigned int cpuid,
			       unsigned int target_freq,
			       int reason)
{
	return cpufreq_limit_handle(cpuid, target_freq, DFC_CPU_FREQ_LIMIT_MIN, (enum dfc_constraint_reason)reason);
}

PUBLIC_SYMBOL
int devhost_pm_set_max_cpufreq(unsigned int cpuid,
			       unsigned int target_freq,
			       int reason)
{
	return cpufreq_limit_handle(cpuid, target_freq, DFC_CPU_FREQ_LIMIT_MAX, (enum dfc_constraint_reason)reason);
}

PUBLIC_SYMBOL
int devhost_freq_driver_init_done(void)
{
	int err;

	err = hm_pm_init_notify(INIT_EVENT_FREQ_DRIVER);
	if (err != E_HM_OK) {
		hm_error("freq driver failed to notify tppmgr init\n");
	}

	return -hmerrno2posix(err);
}

static int cpufreq_query_handle(unsigned int cpuid,
				unsigned int query_type, unsigned int *freq)
{
	int err = E_HM_OK;
	struct __actvret_fmcall_dfc_cpu_freq_query pret;

	if (freq == NULL) {
		hm_warn("freq is NULL\n");
		return -hmerrno2posix(E_HM_INVAL);
	}
	if (freqmgr_rref == 0ULL) {
		err = hm_path_acquire(FREQMGR_PATH, &freqmgr_rref);
		if (err != E_HM_OK) {
			hm_error("acquire freqmgr path failed\n");
			return -hmerrno2posix(err);
		}
	}
	err = actvcapcall_fmcall_dfc_cpu_freq_query(freqmgr_rref, cpuid, query_type, &pret);
	if (err != E_HM_OK) {
		hm_error("failed to get cpu freq. cpu: %u, ret: %d\n", cpuid, err);
		return -hmerrno2posix(err);
	}
	*freq = pret.v;

	return -hmerrno2posix(err);
}

PUBLIC_SYMBOL
int devhost_pm_cpufreq_query_cur(unsigned int cpuid, unsigned int *freq)
{
	return cpufreq_query_handle(cpuid, DFC_CPU_FREQ_QUERY_CUR, freq);
}

PUBLIC_SYMBOL
int devhost_pm_cpufreq_query_max(unsigned int cpuid, unsigned int *freq)
{
	return cpufreq_query_handle(cpuid, DFC_CPU_FREQ_QUERY_MAX, freq);
}

PUBLIC_SYMBOL
int devhost_pm_cpufreq_query_hw_max(unsigned int cpuid, unsigned int *freq)
{
	return cpufreq_query_handle(cpuid, DFC_CPU_HW_FREQ_QUERY_MAX, freq);
}

PUBLIC_SYMBOL
unsigned long long devhost_pm_get_cpu_idle_time(unsigned int cpu, unsigned long long *wall, int io_busy)
{
	int err;
	struct __sysctrl_cpu_stat stat;
	struct timespec now;
	unsigned long long idle_tocks;

	mem_zero_s(stat);
	err = hm_clock_gettime(CLOCK_MONOTONIC, &now);
	if (err < E_HM_OK) {
		hm_debug("hm clock gettime failed, %s\n", hmstrerror(err));
		return 0ULL;
	}
	err = hm_sysctl_cpu_stat_of(cpu, &stat);
	if (err < E_HM_OK) {
		hm_error("cpustat failed err: %s\n", hmstrerror(err));
		return 0ULL;
	}
	if (io_busy != 0) {
		idle_tocks = stat.idle_time;
	} else {
		idle_tocks = stat.idle_time + stat.iowait_time;
	}
	if (wall != NULL) {
		*wall = now.tv_sec * USEC_PER_SEC + now.tv_nsec / NSEC_PER_USEC;
	}

	return tocks_to_ns(idle_tocks) / NSEC_PER_USEC;
}

static int thermal_device_acquire(struct thermal_action_arg *arg, int *action_ret)
{
	int ret = -ENOSYS;
	int id;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->thermal_device_acquire != NULL)) {
		ret = devhost_plat_pm_ops->thermal_device_acquire(arg->type, &id);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
		*action_ret = id;
	}

	return posix2hmerrno(-ret);
}

static int thermal_device_temp_read(struct thermal_action_arg *arg, int *action_ret)
{
	int ret = -ENOSYS;
	int temp;

	if ((devhost_plat_pm_ops != NULL) &&
	    (devhost_plat_pm_ops->thermal_device_acquire != NULL)) {
		ret = devhost_plat_pm_ops->thermal_device_temp_read(arg->id, &temp);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
		*action_ret = temp;
	}

	return posix2hmerrno(-ret);
}

PUBLIC_SYMBOL
int devhost_pm_thermal_action(unsigned int action, struct thermal_action_arg *arg, int *action_ret)
{
	int ret = E_HM_OK;

	if (arg == NULL || action_ret == NULL) {
		return E_HM_INVAL;
	}
	switch (action) {
		case DH_PM_THERMAL_DEVICE_ACQUIRE:
			ret = thermal_device_acquire(arg, action_ret);
			if (ret != E_HM_OK) {
				hm_warn("acquire thermal device failed: %s\n", hmstrerror(ret));
			}
			break;
		case DH_PM_THERMAL_DEVICE_TEMP_READ:
			ret = thermal_device_temp_read(arg, action_ret);
			if (ret < 0) {
				hm_warn("read thermal device temp failed: %s\n", hmstrerror(ret));
			}
			break;
		default:
			hm_warn("invalid action = %u\n", action);
			ret = E_HM_INVAL;
			break;
	}

	return ret;
}
