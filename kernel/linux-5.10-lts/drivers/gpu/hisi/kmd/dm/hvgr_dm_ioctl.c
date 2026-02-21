/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include <linux/ktime.h>
#include <linux/delay.h>
#ifdef CONFIG_LIBLINUX
#include <linux/dmabuf/dmabuf_swap.h>
#endif

#include "hvgr_dm_api.h"
#include "hvgr_version.h"
#include "hvgr_mem_api.h"
#include "hvgr_dm_driver_base.h"
#include "hvgr_log_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_sch_api.h"
#include "hvgr_platform_api.h"
#include "hvgr_ctx_sched.h"
#include "hvgr_sch_dfx.h"
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
#include "hvgr_2d_mcu.h"
#endif
#include "hvgr_mem_context_switchover.h"
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common.h"
#endif

long hvgr_ioctl_get_driver_info(struct hvgr_ctx * const ctx, hvgr_ioctl_para_version * const para)
{
	struct hvgr_device * const gdev = ctx->gdev;

	para->gpu_id = gdev->dm_dev.dev_reg.gpu_id;
	para->gpu_version = gdev->dm_dev.dev_reg.gpu_version;
	para->l2_features = gdev->dm_dev.dev_reg.l2_features;
	para->gpc_present = gdev->dm_dev.dev_reg.gpc_present;
	para->gpc_nums = hweight32(para->gpc_present);
	para->mmu_features = gdev->dm_dev.dev_reg.mmu_features;
	para->thread_features = gdev->dm_dev.dev_reg.thread_features;
	para->thread_max_workgroup_size = gdev->dm_dev.dev_reg.thread_max_workgroup_size;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	para->gpu_available_memory_size = (uint64_t)totalram_pages << PAGE_SHIFT;
#else
	para->gpu_available_memory_size = totalram_pages() << PAGE_SHIFT;
#endif
	return 0;
}

long hvgr_ioctl_cfg_driver(struct hvgr_ctx * const ctx, hvgr_ioctl_para_drv_cfg * const para)
{
	struct hvgr_mem_cfg_para mem_cfg_para;

	mutex_lock(&ctx->ctx_lock);

	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_DYING)) {
		mutex_unlock(&ctx->ctx_lock);
		hvgr_err(ctx->gdev, HVGR_DM, "not allow reconfig after deconfig drv");
		return -EINVAL;
	}

	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_CFGED)) {
		mutex_unlock(&ctx->ctx_lock);
		hvgr_info(ctx->gdev, HVGR_DM, "already config drv");
		return 0;
	}

	if (para->wq_max_wait_cnt > WQ_WAIT_MAX) {
		hvgr_err(ctx->gdev, HVGR_DM, "wq configure FAIL");
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}

	mem_cfg_para.mem_replay_size = para->mem_replay_size;
	mem_cfg_para.jit_large_size = para->jit_large_size;
	mem_cfg_para.jit_large_cnt = para->jit_large_cnt;
	mem_cfg_para.jit_medium_size = para->jit_medium_size;
	mem_cfg_para.jit_medium_cnt = para->jit_medium_cnt;
	mem_cfg_para.jit_small_size = para->jit_small_size;
	mem_cfg_para.jit_small_cnt = para->jit_small_cnt;
	mem_cfg_para.jit_default_size = para->jit_default_size;
	if (hvgr_mem_config_proc(ctx, &mem_cfg_para) != 0) {
		hvgr_err(ctx->gdev, HVGR_DM, "memory configure FAIL");
		mutex_unlock(&ctx->ctx_lock);
		return -ENOMEM;
	}

#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	/* v500 normal buffer init */
	hvgr_allocate_normal_ctx_buffer(ctx);
	hvgr_init_normal_ctx_buffer(ctx);
	hvgr_init_mcu_page_table(ctx);
#endif

	ctx->wq_wait_max_cnt = para->wq_max_wait_cnt;
#ifdef CONFIG_HVGR_DFX_SH
	hvgr_dmd_config_ctx_gaf(ctx, para->gaf_flag);
#endif
	hvgr_ctx_flag_set(ctx, HVGR_CTX_FLAG_CFGED);

#if hvgr_version_ge(HVGR_V350)
	hvgr_ctx_alloc_static_asid_and_ssid(ctx);
#endif

#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	if (hvgr_2d_mcu_load_fm(ctx) != 0) {
		hvgr_err(ctx->gdev, HVGR_DM, "2d mcu load firmware FAIL");
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}
#endif

	mutex_unlock(&ctx->ctx_lock);
	return 0;
}

/*
 * Retry 100000 times(10ms).
 * When the ctx_kref decreases to 2, means no other ioctl is running.
 */
#define HVGR_DEFCFG_KREF_TRY_MAX_TIMES  10000u
#define HVGR_DEFCFG_KREF_VALUE          2u

static int hvgr_wait_other_ioctl_done(struct hvgr_ctx * const ctx)
{
	uint32_t try_times = HVGR_DEFCFG_KREF_TRY_MAX_TIMES;

	while (try_times > 0) {
		if (kref_read(&ctx->ctx_kref) == HVGR_DEFCFG_KREF_VALUE)
			return 0;
		udelay(1);
		try_times--;
	}

	return -EBUSY;
}

long hvgr_ioctl_decfg_driver(struct hvgr_ctx * const ctx, hvgr_ioctl_para_drv_cfg * const para)
{
	mutex_lock(&ctx->ctx_lock);
	if (!hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_CFGED)) {
		mutex_unlock(&ctx->ctx_lock);
		return 0;
	}

	hvgr_ctx_ioctl_record(ctx, ktime_to_ns(ktime_get()), HVGR_IOCTL_DECONFIGURE, 0);
	/*
	 * When decfg is called, ioctl cann't be called any more.
	 * The defcfg need to wait for other running ioctl done.
	 */
	hvgr_ctx_flag_set(ctx, HVGR_CTX_FLAG_DYING);
	hvgr_ctx_flag_clear(ctx, HVGR_CTX_FLAG_CFGED);
	hvgr_ctx_stop(ctx);
	if (hvgr_wait_other_ioctl_done(ctx) != 0)
		hvgr_err(ctx->gdev, HVGR_DM, "other ioctl still running, decfg may cause some probelem");
	hvgr_msync_term(ctx);
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	hvgr_2d_mcu_fm_free(ctx);
#endif
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_release_first_level_page_table(ctx);
	hvgr_normal_ctx_release_buf(ctx);
#endif
	if (hvgr_mem_deconfig_proc(ctx) != 0) {
		hvgr_err(ctx->gdev, HVGR_DM, "memory deconfigure FAIL");
		mutex_unlock(&ctx->ctx_lock);
		return -ENOMEM;
	}
	mutex_unlock(&ctx->ctx_lock);
	return 0;
}

long hvgr_ioctl_get_msb(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_msb * const para)
{
	para->out.msb = ctx->same_va_4g_base_page;
	return 0;
}

static long hvgr_dev_ioctl_softq_create(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_create_softq * const para)
{
	uint64_t uva = 0;
	int ret;

	/* Check soft queue direction and input entry size */
	if (para->in.direct > HVGR_SOFTQ_DIR_U_TO_K ||
		para->in.entry_nums > MAX_ENTRY_NUM ||
		para->in.entry_size > MAX_ENTRY_SIZE) {
		hvgr_err(ctx->gdev, HVGR_DM, "direct is %lu,entry_nums is %lu,entry_size is %lu",
			para->in.direct, para->in.entry_nums, para->in.entry_size);
		return -EINVAL;
	}

	if (((para->in.direct & HVGR_SOFTQ_DIR_K_TO_U) != 0) &&
		((para->in.direct & HVGR_SOFTQ_DIR_U_TO_K) != 0))
		return -EINVAL;

	if ((para->in.entry_nums & (para->in.entry_nums - 1)) != 0)
		return -EINVAL;

	ret = hvgr_softq_create(ctx, para->in.entry_size,
		para->in.entry_nums, para->in.direct, &uva);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_DM, "%s create softq fail", __func__);
		return ret;
	}

	para->out.ctrl_addr = uva;
	return 0;
}

static long hvgr_dev_ioctl_softq_flush(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_softq_enqueue * const para)
{
	struct hvgr_soft_q *qctx = NULL;

	if (para->softq_id >= HVGR_MAX_NR_SOFTQ) {
		hvgr_err(ctx->gdev, HVGR_DM, "softq_id is %lu", para->softq_id);
		return -EINVAL;
	}
	mutex_lock(&ctx->ctx_lock);
	qctx = ctx->id_to_qctx[para->softq_id];
	if (qctx == NULL) {
		hvgr_err(ctx->gdev, HVGR_DM, "%s qctx is NULL", __func__);
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}

	hvgr_softq_do_flush(qctx);
	mutex_unlock(&ctx->ctx_lock);
	return 0;
}

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
long hvgr_ioctl_context_switch(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_ctx_sw * const para)
{
	uint32_t cmd = para->in.cmd;
	long ret = -1;
	unsigned long size;

	switch (cmd) {
	case HVGR_CTX_SW_TO_FRONT:
		hvgr_info(ctx->gdev, HVGR_DM, "[ctx_sw] ftont cmd:%d, tgid:%d",
			cmd, para->in.tgid);
		ret = hvgr_switch_to_frontground(ctx, para->in.tgid);
		break;
	case HVGR_CTX_SW_TO_BACKGROUND:
		hvgr_info(ctx->gdev, HVGR_DM, "[ctx_sw] back cmd:%d, tgid:%d ratio:%u",
			cmd, para->in.tgid, para->in.ratio);
		ret = hvgr_switch_to_background(ctx, para);
		break;
	case HVGR_CTX_SW_INIT_GPU_SIZE:
		size = (unsigned long)para->in.size;
		ret = mm_gpu_page_swap_space_init(size);
		if (ret != 0)
			hvgr_err(ctx->gdev, HVGR_DM, "[ctx_sw]swap_space_init failed,size:0x%lx", size);
		else
			hvgr_debug(ctx->gdev, HVGR_DM, "[ctx_sw]swap_space_init success,size:0x%lx", size);
		break;
	default:
		hvgr_err(ctx->gdev, HVGR_DM, "invalid sw value:%u,tgid:%u", cmd, para->in.tgid);
		break;
	}
	return ret;
}

long hvgr_ioctl_get_ctx_pages(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_get_ctx_pages * const para)
{
	long ret = 0;
	struct hvgr_device *gdev = NULL;
	struct hvgr_ctx *tmp = NULL;
	struct hvgr_ctx *ctx_tmp = NULL;
	uint32_t ctx_pages = 0;
	uint32_t tgid = para->in.tgid;
	uint32_t tmp_pages;

	gdev = ctx->gdev;
	mutex_lock(&gdev->dm_dev.ctx_list_mutex);
	list_for_each_entry_safe(ctx_tmp, tmp, &gdev->dm_dev.ctx_list, dev_link) {
		if (ctx_tmp->tgid == tgid) {
			tmp_pages = hvgr_switch_get_ctx_pages(ctx_tmp);
			if (tmp_pages > ctx_pages)
				ctx_pages = tmp_pages;
		}
	}
	mutex_unlock(&gdev->dm_dev.ctx_list_mutex);

	if (ctx_pages == 0)
		ret = -1;

	para->out.ctx_pages = ctx_pages;

	return ret;
}
#endif

#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
long hvgr_dev_ioctl_set_uni_render(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_set_uni_render * const para)
{
	if (para->flag != MALEOON_2D_MCU_ENABLE) {
		hvgr_err(ctx->gdev, HVGR_DM, "flag is %lu invalid", para->flag);
		return -EINVAL;
	}
	ctx->set_uni_render = MALEOON_2D_MCU_ENABLE;
	return 0;
}
#endif

long hvgr_ioctl_dm(struct hvgr_ctx * const ctx, unsigned int cmd, unsigned long arg)
{
	long ret = -EINVAL;

	switch (cmd) {
	case HVGR_IOCTL_CREATE_SOFTQ:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_CREATE_SOFTQ,
			hvgr_dev_ioctl_softq_create, arg, hvgr_ioctl_para_create_softq);
		break;
	case HVGR_IOCTL_SOFT_ENQUEUE:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_SOFT_ENQUEUE,
			hvgr_dev_ioctl_softq_flush, arg, hvgr_ioctl_para_softq_enqueue);
		break;
	case HVGR_IOCTL_GET_MSB:
		hvgr_ioctl_handle_r(ctx, HVGR_IOCTL_GET_MSB,
			hvgr_ioctl_get_msb, arg, union hvgr_ioctl_para_msb);
		break;
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	case HVGR_IOCTL_CONTEXT_SWITCH:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_CONTEXT_SWITCH,
			hvgr_ioctl_context_switch, arg, hvgr_ioctl_para_ctx_sw);
		break;
	case HVGR_IOCTL_GET_CTX_PAGES:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_GET_CTX_PAGES,
			hvgr_ioctl_get_ctx_pages, arg, hvgr_ioctl_para_get_ctx_pages);
		break;
#endif
	default:
		hvgr_err(ctx->gdev, HVGR_DM, "DM ioctl cmd %x is INVALID", cmd);
		break;
	}

exit:
	return ret;
}
