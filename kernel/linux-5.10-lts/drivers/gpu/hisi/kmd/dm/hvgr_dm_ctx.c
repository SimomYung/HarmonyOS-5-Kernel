/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_dm_ctx.h"

#include <linux/thread_info.h>
#include <securec.h>
#include <linux/kref.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/sched/task.h>
#include "hvgr_version.h"
#include "hvgr_mem_api.h"
#include "hvgr_mmu_api.h"

#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_sch_api.h"
#include "hvgr_datan_api.h"
#include "hvgr_protect_mode.h"
#include "hvgr_platform_api.h"
#include "hvgr_defs.h"
#include "hvgr_regmap.h"
#include "hvgr_fcp.h"
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
#include "hvgr_2d_mcu.h"
#endif
#include "hvgr_mem_context_switchover.h"
#include "hvgr_hts_api.h"
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common.h"
#endif

#if hvgr_version_ge(HVGR_V350)
#include "hvgr_pm_fast_power.h"
#define FCP_LOADER_CTX_ID		1
#else
#define FCP_LOADER_CTX_ID		0
#endif

#if defined(CONFIG_LIBLINUX)
#include <liblinux/pal.h>
#endif



#if defined(CONFIG_LIBLINUX)  && !defined(CONFIG_DOUBLEOS_CDC_RDA)
static bool is_ubt_task(void)
{
	bool is_ubt_task = false;
	const char *ubt_path = "/system/bin/ubt_a32a64";
	int ret;
	char *buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (buf == NULL)
		return is_ubt_task;

	(void)memset_s(buf, PATH_MAX, 0, PATH_MAX);
	ret = liblinux_pal_get_current_exec_path(buf, PATH_MAX);
	if (ret == 0 && strcmp(buf, ubt_path) == 0)
		is_ubt_task = true;

	kfree(buf);
	return is_ubt_task;
}
#endif

static void hvgr_ctx_set_group_name(struct hvgr_ctx *ctx, pid_t pid)
{
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	int main_tid = 0;

	liblinux_pal_query_main_thread(pid, &main_tid, ctx->group_name, TASK_COMM_LEN);
#else
	if (likely(ctx->leader != NULL))
		get_task_comm(ctx->group_name, ctx->leader);
#endif
}

struct hvgr_ctx *hvgr_open_context(struct hvgr_device * const gdev, struct file *kfile)
{
	struct hvgr_ctx *ctx = kvzalloc(sizeof(*ctx), GFP_KERNEL);

	if (ctx == NULL) {
		hvgr_err(gdev, HVGR_DM, "%s: alloc ctx failed", __func__);
		return NULL;
	}
	ctx->kfile = kfile;
	ctx->gdev = gdev;

	ctx->tgid = current->tgid;
	ctx->pid = current->pid;
	get_task_comm(ctx->process_name, current);
	ctx->leader = current->group_leader;
	hvgr_ctx_set_group_name(ctx, ctx->pid);

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	liblinux_pal_proc_lock_pid(ctx->tgid);
#else
	get_task_struct(ctx->leader);
#endif

	ctx->asid = HVGR_AS_NR_INVALID;
#if hvgr_version_ge(HVGR_V350)
	ctx->ssid = HVGR_SSID_INVALID;
#endif

	mutex_init(&ctx->softq_lock);
	mutex_init(&ctx->ctx_lock);
	init_waitqueue_head(&ctx->softq_wait);
	INIT_LIST_HEAD(&ctx->ctx_sw.swapped_list);
	mutex_init(&ctx->ctx_sw.swapped_list_mutex);
	atomic_set(&ctx->ctx_sw.same_va_total_pages, 0);
	atomic_set(&ctx->ctx_sw.ctx_foreground_flag, 0);

	ctx->softq_unused = HVGR_MAX_SOFTQ_BITMAP;
	kref_init(&ctx->ctx_kref);

	ctx->id = (u32)atomic_inc_return(&(gdev->ctx_num)) - 1;
	hvgr_info(gdev, HVGR_DM, "ctx_%u pid(%d) ctx_num(%d) %s %s.", ctx->id, ctx->pid,
		atomic_read(&(gdev->ctx_num)), ctx->process_name, ctx->group_name);

	if (is_compat_task())
		hvgr_ctx_flag_set(ctx, HVGR_CTX_FLAG_COMPAT);

#if defined(CONFIG_EXAGEAR_BT) || (defined(CONFIG_LIBLINUX) && !defined(CONFIG_DOUBLEOS_CDC_RDA))
	if (is_ubt_task())
		hvgr_ctx_flag_set(ctx, HVGR_CTX_FLAG_COMPAT);
#endif

	if (hvgr_mem_init_proc(ctx) != 0) {
		hvgr_err(gdev, HVGR_DM, "%s: mem init failed", __func__);
		goto mem_init_fail;
	}

	if (hvgr_sch_ctx_create(ctx) != 0) {
		hvgr_err(gdev, HVGR_DM, "%s: cq ctx init failed", __func__);
		goto cq_ctx_init_fail;
	}

	if (ctx->id == FCP_LOADER_CTX_ID) {
		hvgr_info(gdev, HVGR_DM, "%s hvgr_fcp_loader_entry", __func__);
		hvgr_fcp_loader_entry(gdev);
#ifdef CONFIG_HVGR_DFX_SH
		hvgr_dmd_init_sh(gdev);
#endif
	}

	ctx->token_process_step = TOKEN_PROCESS_IDLE;
	mutex_lock(&gdev->dm_dev.ctx_list_mutex);
	list_add(&ctx->dev_link, &gdev->dm_dev.ctx_list);
	mutex_unlock(&gdev->dm_dev.ctx_list_mutex);

	(void)hvgr_datan_init_proc(ctx);
#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
	hvgr_hts_get_ssid(ctx);
#endif

	return ctx;

cq_ctx_init_fail:
	hvgr_mem_term_proc(ctx);
mem_init_fail:
	atomic_dec(&(gdev->ctx_num));
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	liblinux_pal_proc_unlock_pid(ctx->tgid);
#else
	put_task_struct(ctx->leader);
#endif
	kvfree(ctx);
	return NULL;
}

void hvgr_close_context(struct hvgr_ctx * const ctx)
{
	struct hvgr_ctx *tmp = NULL;
	struct hvgr_ctx *ctx_tmp = NULL;

	mutex_lock(&ctx->gdev->dm_dev.ctx_list_mutex);
	list_for_each_entry_safe(ctx_tmp, tmp, &ctx->gdev->dm_dev.ctx_list, dev_link) {
		if (ctx_tmp == ctx) {
			list_del(&ctx_tmp->dev_link);
			break;
		}
	}
	mutex_unlock(&ctx->gdev->dm_dev.ctx_list_mutex);

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	hvgr_switch_close_ctx(ctx);
#endif
	hvgr_datan_term_proc(ctx);
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	hvgr_2d_mcu_fm_free(ctx);
#endif
	hvgr_sch_ctx_destroy(ctx);

#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_common_destory_ctxid(ctx);
#endif
#if hvgr_version_lt(HVGR_V300)
	hvgr_mem_protect_jcd_term(ctx);
#endif
#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
	hvgr_hts_block_proc(ctx);
#endif
	hvgr_mem_term_proc(ctx);
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	liblinux_pal_proc_unlock_pid(ctx->tgid);
#else
	put_task_struct(ctx->leader);
#endif
	if (unlikely(atomic_read(&(ctx->mem_ctx.used_pt_pages)) != 0))
		hvgr_err(ctx->gdev, HVGR_DM, "%s ctx_%u pt pages is %u",
			__func__, ctx->id, (u32)atomic_read(&(ctx->mem_ctx.used_pt_pages)));

	kvfree(ctx);
}

#if hvgr_version_ge(HVGR_V350)
int hvgr_ctx_common_ctx_init(struct hvgr_device * const gdev)
{
	struct hvgr_ctx *ctx = NULL;
	int ret;

	hvgr_info(gdev, HVGR_DM, "%s enter", __func__);
	ctx = vmalloc(sizeof(*ctx));
	if (ctx == NULL) {
		hvgr_err(gdev, HVGR_DM, "common ctx vmalloc fail");
		return -ENOMEM;
	}

	(void)memset_s(ctx, sizeof(*ctx), 0, sizeof(*ctx));
	ctx->asid = COMMON_USE_SSID_VAL;
	ctx->ssid = COMMON_USE_SSID_VAL;
	ctx->gdev = gdev;
	ret = hvgr_mem_init_proc(ctx);
	if (ret != 0) {
		vfree(ctx);
		hvgr_err(gdev, HVGR_DM, "common ctx init fail");
		return -ENOMEM;
	}

	atomic_inc(&(gdev->ctx_num));
	gdev->com_ctx.ctx = ctx;
	ctx->mem_ctx.zones[0].pmem = &ctx->mem_ctx;
	/* malloc other buffer used common ctx */
	ret = hvgr_pm_fast_pwr_malloc_buf(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "auto gpc malloc fail");
		goto common_ctx_init_fail;
	}
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	/* malloc pa slice buffer use common ctx */
	ret = hvgr_paslice_malloc_buf(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "pa slice buffer malloc fail");
		hvgr_pm_fast_pwr_release_buf(gdev);
		goto common_ctx_init_fail;
	}
	hvgr_first_pa_submit(gdev);
#endif
	return 0;
common_ctx_init_fail:
	hvgr_mem_term_proc(ctx);
	gdev->com_ctx.ctx = NULL;
	vfree(ctx);
	return -1;
}

void hvgr_ctx_common_ctx_term(struct hvgr_device * const gdev)
{
	struct hvgr_ctx *ctx = gdev->com_ctx.ctx;

	hvgr_info(gdev, HVGR_DM, "start in %s", __func__);
	/* free pa slice buffer */
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_release_pa_pages(ctx);
	hvgr_paslice_release_buf(gdev);
#endif
	/* free other buffer used common ctx */
	hvgr_pm_fast_pwr_release_buf(gdev);

	if (ctx == NULL)
		return;

	hvgr_info(gdev, HVGR_DM, "%s enter", __func__);
	hvgr_mem_term_proc(ctx);
	gdev->com_ctx.ctx = NULL;
	vfree(ctx);
	hvgr_info(gdev, HVGR_DM, "end up %s", __func__);
	return;
}

static int hvgr_ctx_config_common_ssid(struct hvgr_device *gdev)
{
	u32 value;
	u32 time = 1000;

	hvgr_write_reg(gdev, cq_control_reg(gdev, COMMON_USE_SSID), COMMON_USE_SSID_VAL);

	do {
		value = hvgr_read_reg(gdev, cq_control_reg(gdev, COMMON_USE_SSID_STATUS));
		time--;
	} while ((value != COMMON_USE_SSID_EN) && (time != 0));

	if (time == 0) {
		hvgr_err(gdev, HVGR_DM, "common ssid set timeout");
		return -1;
	}

	return 0;
}

int hvgr_ctx_common_ctx_enable(struct hvgr_device * const gdev)
{
	struct hvgr_ctx *ctx = gdev->com_ctx.ctx;

	if (ctx == NULL) {
		hvgr_err(gdev, HVGR_DM, "no common ctx");
		return -1;
	}

	hvgr_mmu_hal_enable(ctx);

	if (hvgr_ctx_config_common_ssid(gdev) != 0) {
		hvgr_err(gdev, HVGR_DM, "config common ssid failed");
		return -1;
	}

#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	if (ctx->itcm_2d_addr != 0) {
		hvgr_info(gdev, HVGR_DM, "%s start write reg 0x%lx value: 0x%lx",
			__func__, fcp_2d_mb_reg(gdev, FCP_2D_MB_MCUVECTOR_ADDR), ctx->itcm_2d_addr);

		hvgr_write_reg(gdev, fcp_2d_mb_reg(gdev, FCP_2D_MB_MCUVECTOR_ADDR), ctx->itcm_2d_addr);

		hvgr_info(gdev, HVGR_DM, "%s write reg 0x%lx value: 0x%lx, read value = 0x%lx",
			__func__, fcp_2d_mb_reg(gdev, FCP_2D_MB_MCUVECTOR_ADDR), ctx->itcm_2d_addr,
			hvgr_read_reg(gdev, fcp_2d_mb_reg(gdev, FCP_2D_MB_MCUVECTOR_ADDR)));
	}
#endif

	return 0;
}

#endif
