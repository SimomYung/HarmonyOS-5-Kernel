/*
 * npu_pm_interframe.c
 *
 * inter-frame power management for NPU, separated from npu_pm_framework.c
 * during the refactoring project in 2023
 *
 * Copyright (c) 2012-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

#include "npu_pm_framework.h"
#include "npu_pm_interframe.h"
#include "bbox/npu_dfx_black_box.h"
#include "npu_log.h"
#include "npu_platform.h"
#include "npu_shm.h"
#include "npu_autofs.h"
#include "npu_powercapping.h"
#include "npu_message.h"
#include "npu_heart_beat.h"
#include "npu_io.h"
#include "npu_hwts_plat.h"
#include "npu_reg.h"
#include "npu_task_message.h"
#include "npu_manager.h"
#include "npu_svm.h"
#include "profiling/npu_dfx_profiling.h"
#include "npu_stream.h"
#include "npd_interface.h"
#include "npu_syscache.h"

static inline struct npu_pm_work_strategy *npu_pm_interframe_get_work_strategy(
	struct npu_power_mgr *handle, u32 work_mode)
{
	struct npu_pm_work_strategy *strategy = NULL;
	struct npu_interframe_idle_manager *interframe_idle_manager =
		&handle->interframe_idle_manager;

	strategy = (struct npu_pm_work_strategy *)(uintptr_t)(
		interframe_idle_manager->strategy_table_addr +
		sizeof(struct npu_pm_work_strategy) * (work_mode));

	return strategy;
}

static bool is_support_interframe_power_down(struct npu_power_mgr *power_mgr, u32 work_mode)
{
	struct npu_pm_work_strategy *work_strategy = NULL;
	work_strategy = npu_pm_interframe_get_work_strategy(power_mgr, work_mode);
	return work_strategy->is_idle_support;
}

static bool is_interframe_power_down_enabled(struct npu_power_mgr *power_mgr)
{
	struct npu_platform_info *plat_info = npu_plat_get_info();
	
	if ((plat_info->dts_info.feature_switch[NPU_FEATURE_INTERFRAME_IDLE_POWER_DOWN] != 0) &&
		(atomic_read(&power_mgr->interframe_idle_manager.enable) == 1))
		return true;
 
	return false;
}

// npu_pm_interframe_idle_get_powerup_delta_subip
static u32 npu_interframe_get_delta_up_subip(
	struct npu_power_mgr *handle, u32 work_mode)
{
	struct npu_pm_work_strategy *strategy = NULL;
	u32 delta_subip;
	u32 cur_subip;

	strategy = npu_pm_interframe_get_work_strategy(handle, work_mode);
	delta_subip = strategy->subip_set.data;
	cur_subip = handle->cur_subip_set;
	delta_subip &= (~cur_subip);

	return delta_subip;
}
// npu_pm_interframe_idle_get_powerdown_delta_subip
static u32 npu_interframe_get_delta_down_subip(
	struct npu_power_mgr *handle, u32 work_mode)
{
	struct npu_pm_work_strategy *strategy = NULL;
	u32 mode_idx;
	u32 idle_subip;
	u32 delta_subip;
	u32 used_subip = 0;

	for (mode_idx = 0; mode_idx < NPU_WORKMODE_MAX; mode_idx++) {
		if (mode_idx == work_mode)
			continue;
		// subip will be added into used_subip if it satisfies the following two conditions:
		// 1. the workmode which devices voted contains this ip
		// 2. the workmode is not in an idle_state(powered up but no tasks)
		if (npu_bitmap_get(handle->work_mode, mode_idx) == 0x1 &&
			npu_bitmap_get(handle->work_mode_idle, mode_idx) == 0x0) {
			strategy = npu_pm_interframe_get_work_strategy(
				handle, mode_idx);
			used_subip |= strategy->subip_set.data;
		}
	}

	strategy = npu_pm_interframe_get_work_strategy(handle, work_mode);
	idle_subip = strategy->subip_set.data;
	delta_subip = idle_subip & (~used_subip);

	return delta_subip;
}
// npu_pm_interframe_idle_get_delta_subip
static u32 npu_pm_interframe_get_delta_subip(
	struct npu_power_mgr *handle, u32 work_mode, int pm_ops)
{
	u32 delta_subip;

	if (pm_ops == POWER_UP)
		delta_subip = npu_interframe_get_delta_up_subip(handle, work_mode);
	else
		delta_subip = npu_interframe_get_delta_down_subip(handle, work_mode);

	return delta_subip;
}

static int npu_pm_interframe_idle_powerdown(struct npu_power_mgr *power_mgr, u32 work_mode)
{
	int ret;
	u32 delta_subip;

	delta_subip = npu_pm_interframe_get_delta_subip(power_mgr,
		work_mode, POWER_DOWN);
	npu_drv_warn("interframe idle power down enter, delta_subip= 0x%x\n", delta_subip);
	ret = npu_pm_powerdown_proc(power_mgr, work_mode, delta_subip);

	return ret;
}

static int npu_pm_interframe_powerup(struct npu_power_mgr *power_mgr, u32 workmode)
{
	int ret;
	u32 delta_subip;

	delta_subip = npu_pm_interframe_get_delta_subip(power_mgr, workmode, POWER_UP);
	npu_drv_warn("interframe idle power up enter, delta_subip= 0x%x\n", delta_subip);
	ret = npu_pm_powerup_proc(power_mgr, workmode, delta_subip);
	cond_return_error(ret != 0, ret, "power down fail: ret = %d\n", ret);

	npu_drv_info("interframe power up\n");

	return ret;
}

int npu_handle_interframe_powerup(struct npu_power_mgr *power_mgr, u32 workmode)
{
	int ret = 0;
	struct npu_platform_info *plat_info = npu_plat_get_info();

	if (plat_info == NULL ||
		plat_info->dts_info.feature_switch[NPU_FEATURE_INTERFRAME_IDLE_POWER_DOWN] == 0)
		return ret;

	if (is_support_interframe_power_down(power_mgr, workmode)) {
#ifdef CONFIG_NPU_AUTOFS
			if ((plat_info->dts_info.feature_switch[NPU_FEATURE_NPU_AUTOFS] == 1) &&
				(power_mgr->power_stage == NPU_PM_UP))
				npu_autofs_disable();
#endif
			ret = npu_pm_interframe_powerup(power_mgr, workmode);
	}

	return ret;
}

static void npu_pm_interframe_idle_work_by_mode(struct npu_interframe_idle_manager *idle_manager,
	u32 work_mode)
{
	struct npu_dev_ctx *dev_ctx = NULL;
	struct npu_platform_info *plat_info = npu_plat_get_info();
	int work_state;

	cond_return_void(plat_info == NULL, "plat_info is null\n");
	dev_ctx = container_of(idle_manager,
		struct npu_dev_ctx, pm.interframe_idle_manager);
	mutex_lock(&dev_ctx->pm.npu_power_mutex);
	if (dev_ctx->pm.wm_cnt[work_mode] != 0) {
		atomic_cmpxchg(&idle_manager->wq_processing[work_mode], WORK_ADDED, WORK_IDLE);
		mutex_unlock(&dev_ctx->pm.npu_power_mutex);
		return;
	}
	work_state = atomic_read(&idle_manager->wq_processing[work_mode]);
	if (work_state == WORK_ADDED) {
		(void)npu_pm_interframe_idle_powerdown(&dev_ctx->pm, work_mode);
#ifdef CONFIG_NPU_AUTOFS
		if ((plat_info->dts_info.feature_switch[NPU_FEATURE_NPU_AUTOFS] == 1) &&
			(npu_bitmap_get(dev_ctx->pm.work_mode, NPU_ISPNN_SEPARATED) == 0) &&
			(npu_bitmap_get(dev_ctx->pm.work_mode, NPU_ISPNN_SHARED) == 0) &&
			(dev_ctx->pm.power_stage == NPU_PM_UP))
			npu_autofs_enable();
#endif
		atomic_set(&idle_manager->wq_processing[work_mode], WORK_IDLE);
	} else {
		npu_drv_warn("work state = %d\n", work_state);
	}
	mutex_unlock(&dev_ctx->pm.npu_power_mutex);
}

static void npu_pm_interframe_idle_work(struct work_struct *interframe_idle_work)
{
	struct npu_interframe_idle_manager *idle_manager = NULL;
	
	npu_drv_info("interframe idle timer nonsec work enter\n");
	cond_return_void(interframe_idle_work == NULL,
		"interframe_idle_work is null\n");
	idle_manager = container_of(interframe_idle_work,
		struct npu_interframe_idle_manager, work[NPU_NONSEC].work);
	(void)npu_pm_interframe_idle_work_by_mode(idle_manager, NPU_NONSEC);
}

static void npu_pm_interframe_gene_idle_work(struct work_struct *interframe_idle_work)
{
	struct npu_interframe_idle_manager *idle_manager = NULL;

	npu_drv_info("interframe idle timer nonsec gene work enter\n");
	cond_return_void(interframe_idle_work == NULL,
		"interframe_idle_work is null\n");
	idle_manager = container_of(interframe_idle_work,
		struct npu_interframe_idle_manager, work[NPU_NONSEC_GE].work);
	npu_pm_interframe_idle_work_by_mode(idle_manager, NPU_NONSEC_GE);
}

static void npu_pm_interframe_eff_idle_work(struct work_struct *interframe_idle_work)
{
	struct npu_interframe_idle_manager *idle_manager = NULL;

	npu_drv_info("interframe idle timer nonsec eff work enter\n");
	cond_return_void(interframe_idle_work == NULL,
		"interframe_idle_work is null\n");
	idle_manager = container_of(interframe_idle_work,
		struct npu_interframe_idle_manager, work[NPU_NONSEC_EFF].work);
	npu_pm_interframe_idle_work_by_mode(idle_manager, NPU_NONSEC_EFF);
}

int npu_enable_interframe_ilde(struct npu_proc_ctx *proc_ctx, uint32_t enable)
{
	struct npu_dev_ctx *dev_ctx = NULL;

	dev_ctx = get_dev_ctx_by_id(proc_ctx->devid);
	cond_return_error(dev_ctx == NULL, -1, "invalid dev ctx\n");

	atomic_set(&dev_ctx->pm.interframe_idle_manager.enable, (int)enable);
	npu_drv_debug("interframe enable = %u",
		atomic_read(&dev_ctx->pm.interframe_idle_manager.enable));

	return 0;
}

static void (*g_npu_pm_interframe_idle_work[NPU_WORKMODE_MAX])(
	struct work_struct *interframe_idle_work) = {
	npu_pm_interframe_idle_work,
	NULL,
	NULL,
	NULL,
	NULL,
	npu_pm_interframe_gene_idle_work,
	npu_pm_interframe_eff_idle_work,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

void npu_pm_interframe_delay_work_init(struct npu_power_mgr *power_mgr)
{
	u32 mode_idx;
	struct npu_pm_work_strategy *strategy = NULL;
	struct npu_interframe_idle_manager *idle_manager = &power_mgr->interframe_idle_manager;
	struct npu_platform_info *plat_info = npu_plat_get_info();
	cond_return_void(plat_info == NULL, "plat_info is null\n");

	if (plat_info->dts_info.feature_switch[NPU_FEATURE_INTERFRAME_IDLE_POWER_DOWN]) {
		idle_manager->idle_time_out = NPU_INTERFRAME_IDLE_TIME_OUT_DEFAULT_VALUE;
		for (mode_idx = 0; mode_idx < NPU_WORKMODE_MAX; mode_idx++) {
			strategy = npu_pm_interframe_get_work_strategy(power_mgr, mode_idx);
			if (strategy->is_idle_support) {
				cond_return_void(g_npu_pm_interframe_idle_work[mode_idx] == NULL,
					"work_mode %u func is null\n", mode_idx);
				atomic_set(&idle_manager->wq_processing[mode_idx], WORK_IDLE);
				INIT_DELAYED_WORK(&idle_manager->work[mode_idx],
					g_npu_pm_interframe_idle_work[mode_idx]);
			}
		}
	}
}

// npu_open_interframe_power_down
// npu_plan_interframe_power_down
void npu_schedule_interframe_power_down(struct npu_power_mgr *power_mgr, u32 work_mode)
{
	int work_state;
	struct npu_interframe_idle_manager *idle_manager = &power_mgr->interframe_idle_manager;
	cond_return_void(idle_manager == NULL, "idle_manager is null\n");

	if(!is_interframe_power_down_enabled(power_mgr) ||
	    !is_support_interframe_power_down(power_mgr, work_mode))
		return;

	work_state = atomic_cmpxchg(&idle_manager->wq_processing[work_mode], WORK_IDLE, WORK_ADDING);
	if (work_state == WORK_IDLE) {
		schedule_delayed_work(&idle_manager->work[work_mode], msecs_to_jiffies(idle_manager->idle_time_out));
		atomic_set(&idle_manager->wq_processing[work_mode], WORK_ADDED);
	} else if (work_state != WORK_ADDED) {
		npu_drv_warn("work state = %d\n", work_state);
	}
}
// npu_close_interframe_power_down
void npu_cancel_interframe_power_down(struct npu_power_mgr *power_mgr, u32 work_mode)
{
	int work_state;
	struct npu_interframe_idle_manager *idle_manager = &power_mgr->interframe_idle_manager;
	cond_return_void(idle_manager == NULL, "idle_manager is null\n");

	if(!is_interframe_power_down_enabled(power_mgr) ||
	    !is_support_interframe_power_down(power_mgr, work_mode))
		return;

	work_state = atomic_cmpxchg(&idle_manager->wq_processing[work_mode], WORK_ADDED, WORK_CANCELING);
	if (work_state == WORK_ADDED) {
		cancel_delayed_work_sync(&idle_manager->work[work_mode]);
		atomic_set(&idle_manager->wq_processing[work_mode], WORK_IDLE);
	} else if (work_state != WORK_IDLE) {
		npu_drv_warn("work state = %d\n", work_state);
	}
}

void npu_pm_interframe_delay_work_cancel(struct npu_power_mgr *power_mgr)
{
	u32 mode_idx;
	struct npu_pm_work_strategy *strategy = NULL;
	struct npu_platform_info *plat_info = npu_plat_get_info();
	cond_return_void(plat_info == NULL, "plat_info is null\n");

	if (plat_info->dts_info.feature_switch[NPU_FEATURE_INTERFRAME_IDLE_POWER_DOWN]) {
		for (mode_idx = 0; mode_idx < NPU_WORKMODE_MAX; mode_idx++) {
			strategy = npu_pm_interframe_get_work_strategy(power_mgr, mode_idx);
			if (strategy->is_idle_support)
				npu_cancel_interframe_power_down(power_mgr, mode_idx);
		}
	}
}