/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_datan.h"

#ifdef CONFIG_HISI_GPU_AI_FENCE_INFO
#include <platform_include/hvgrgpu/linux/hvgr_gpu_hook.h>
#endif
#include <linux/types.h>
#include <linux/fs.h>
#include <securec.h>

#include "hvgr_datan_switch.h"
#include "hvgr_ioctl_datan.h"
#include "hvgr_datan_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_defs.h"
#ifdef CONFIG_HVGR_DFX_MNTN_REPORT
#include "hvgr_mntn.h"
#endif

#ifdef CONFIG_HISI_GPU_AI_FENCE_INFO

static int hvgr_sync_fence_info(struct kmd_fence_info *fence)
{
	struct hvgr_device *gdev = NULL;

	if (fence == NULL)
		return -EINVAL;

	gdev = hvgr_get_device();
	if (gdev == NULL)
		return -ENODEV;

	if (gdev->ai_freq_game_pid != fence->game_pid)
		gdev->ai_freq_signaled_seqno = 0;

	gdev->ai_freq_game_pid = fence->game_pid;
	fence->signaled_seqno = gdev->ai_freq_signaled_seqno;

	return 0;
}

int perf_ctrl_get_hvgr_gpu_fence(void __user *uarg)
{
	struct kmd_fence_info gpu_fence = {};
	int ret;

	if (uarg == NULL)
		return -EFAULT;

	if (copy_from_user(&gpu_fence, uarg, sizeof(struct kmd_fence_info))) {
		pr_err("get_gpu_fence copy_from_user fail.\n");
		return -EFAULT;
	}
	ret = hvgr_sync_fence_info(&gpu_fence);
	if (ret) {
		pr_err("get_gpu_fence hvgr fail:%d\n", ret);
		return -EFAULT;
	}
	if (copy_to_user(uarg, &gpu_fence, sizeof(struct kmd_fence_info))) {
		pr_err("get_gpu_fence copy_to_user fail.\n");
		return -EFAULT;
	}

	return 0;
}
EXPORT_SYMBOL(perf_ctrl_get_hvgr_gpu_fence);

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
int hvgr_api_get_ai_fence_info(struct hvgr_ctx *ctx, struct hvgr_ioctl_ai_fence_info *para)
{
	void __user *uarg = u64_to_ptr(para->fence_info_addr);

	return perf_ctrl_get_hvgr_gpu_fence(uarg);
}
#endif /* CONFIG_LIBLINUX && CONFIG_LIBLINUX_CDC */

/*
 * This function get lb_enable flag from AI freq schedule service
 * When enter games, AI freq will set lb_enable=1 to enable LB
 * When exit games, AI freq will set lb_enable=0 to bypass LB
 */
#endif /* CONFIG_HISI_GPU_AI_FENCE_INFO */

static unsigned int hvgr_get_hwc_buffer_size(void)
{
	size_t dump_size = hvgr_api_ct_get_dump_size() / GROUP_SIZE_BY_UINT;

	return (unsigned int)dump_size;
}

int perf_ctrl_get_hvgr_gpu_buffer_size(void __user *uarg)
{
	unsigned int gpu_buffer_size;

	if (uarg == NULL)
		return -EINVAL;

	gpu_buffer_size = hvgr_get_hwc_buffer_size();
	if (copy_to_user(uarg, &gpu_buffer_size, sizeof(unsigned int))) {
		pr_err("%s: copy_to_user fail\n", __func__);
		return -EFAULT;
	}

	return 0;
}
#ifdef CONFIG_HISI_GPU_AI_FENCE_INFO
EXPORT_SYMBOL(perf_ctrl_get_hvgr_gpu_buffer_size);
#endif /* CONFIG_HISI_GPU_AI_FENCE_INFO */

int hvgr_api_get_ai_buffer_size(struct hvgr_ctx *ctx, struct hvgr_ioctl_ai_buffer_info *para)
{
	void __user *uarg = u64_to_ptr(para->buffer_size_addr);

	return perf_ctrl_get_hvgr_gpu_buffer_size(uarg);
}

/*
 * @brief it is called on cmodel where counter trace is not compiled,
 *        and it is overriden by conter trace if embedded env
 */
long __attribute__((weak)) hvgr_api_ct_alloc(struct hvgr_ctx *ctx,
	union hvgr_ioctl_ct_alloc *const para)
{
	return 0;
}

/*
 * @brief it is called on cmodel where counter trace is not compiled,
 *        and it is overriden by conter trace if embedded env
 */
int __attribute__((weak)) hvgr_api_ct_enable(struct hvgr_ctx *ctx,
	struct hvgr_ioctl_ct_single_dump_enable *enable)
{
	return 0;
}

/*
 * @brief it is called on cmodel where counter trace is not compiled,
 *        and it is overriden by conter trace if embedded env
 */
int __attribute__((weak)) hvgr_api_ct_dump(struct hvgr_ctx *ctx)
{
	return 0;
}

int __attribute__((weak)) hvgr_api_ct_close_ctx(struct hvgr_ctx *ctx)
{
	return 0;
}

void __attribute__((weak)) hvgr_api_ct_open_ctx(struct hvgr_ctx *ctx) {}

long hvgr_ioctl_datan(struct hvgr_ctx * const ctx, unsigned int cmd, unsigned long arg)
{
	long ret = 0;

	switch (cmd) {
	case HVGR_IOCTL_AI_BUFFER_SIZE_GET:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_AI_BUFFER_SIZE_GET, hvgr_api_get_ai_buffer_size,
			arg, struct hvgr_ioctl_ai_buffer_info);
		break;
	case HVGR_IOCTL_CT_ALLOC:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_CT_ALLOC, hvgr_api_ct_alloc, arg,
			union hvgr_ioctl_ct_alloc);
		break;

	case HVGR_IOCTL_CT_ENABLE:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_CT_ENABLE,
			hvgr_api_ct_enable,
			arg, struct hvgr_ioctl_ct_single_dump_enable);
		break;
	case HVGR_IOCTL_CT_DUMP:
		hvgr_ioctl_handle(ctx, HVGR_IOCTL_CT_DUMP,
			hvgr_api_ct_dump);
		break;
#ifdef HVGR_DATAN_JOB_TIMELINE
	case HVGR_IOCTL_TIMELINE_SUBMIT:
	case HVGR_IOCTL_TIMELINE_SET:
	case HVGR_IOCTL_TIMELINE_GET:
	case HVGR_IOCTL_TIMELINE_GET_CHAIN_ID_BUF:
		ret = hvgr_datan_jt_ioctl(ctx, cmd, arg);
		break;
#endif
#ifdef CONFIG_HISI_GPU_AI_FENCE_INFO
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	case HVGR_IOCTL_AI_FENCE_GET:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_AI_FENCE_GET, hvgr_api_get_ai_fence_info,
			arg, struct hvgr_ioctl_ai_fence_info);
		break;
#endif /* CONFIG_LIBLINUX && CONFIG_LIBLINUX_CDC */
#endif /* CONFIG_HISI_GPU_AI_FENCE_INFO */
	default:
		hvgr_err(ctx->gdev, HVGR_DATAN, "Datan ioctl cmd 0x%x is INVALID", cmd);
		ret = -EINVAL;
		break;
	}

exit:
	return ret;
}

int hvgr_datan_init_proc(struct hvgr_ctx *ctx)
{
#ifdef HVGR_DATAN_JOB_TIMELINE
	ctx->datan_ctx.jt_ctx.jt_chain_enable = HVGR_DATAN_TIMLINE_DISABLE;
#endif
	hvgr_api_ct_open_ctx(ctx);
	return 0;
}

void hvgr_datan_term_proc(struct hvgr_ctx *ctx)
{

#ifdef HVGR_DATAN_JOB_TIMELINE
	hvgr_datan_jt_buffers_free(ctx);
#endif

	hvgr_api_ct_close_ctx(ctx);
}

/*
 * @brief it is called on cmodel where debug bus is not compiled,
 *        and it is overriden by debug bus if embedded env
 */
void __attribute__((weak)) kdatan_bus_dump_thread_init(struct hvgr_device * const gdev) {}

/*
 * @brief it is called on cmodel where debug bus is not compiled,
 *        and it is overriden by debug bus if embedded env
 */
void __attribute__((weak)) kdatan_bus_dump_thread_term(struct hvgr_device * const gdev) {}

/*
 * @brief it is called on cmodel where debug bus is not compiled,
 *        and it is overriden by debug bus if embedded env
 */
void __attribute__((weak)) kdatan_debug_bus_start_dump(struct hvgr_device * const gdev) {}

/*
 * @brief it is called on cmodel where debug bus is not compiled,
 *        and it is overriden by debug bus if embedded env
 */
void __attribute__((weak)) kdatan_dump_chain_state(struct hvgr_device * const gdev) {}

/*
 * @brief it is called on cmodel where debug bus is not compiled,
 *        and it is overriden by debug bus if embedded env
 */
void __attribute__((weak)) kdatan_debug_bus_cbit_config(struct hvgr_device * const gdev) {}

/*
 * @brief it is called on cmodel where mem dump is not compiled,
 *        and it is overriden by ct if embedded env
 */
void __attribute__((weak)) kdatan_mem_dump(struct hvgr_ctx * const ctx) {}

/*
 * @brief it is called on cmodel where ct is not compiled,
 *        and it is overriden by ct if embedded env
 */
void __attribute__((weak)) hvgr_api_ct_init_by_dev(struct hvgr_device * const gdev) {}

/*
 * @brief it is called on cmodel where ct is not compiled,
 *        and it is overriden by ct if embedded env
 */
void __attribute__((weak)) hvgr_api_ct_term_by_dev(struct hvgr_ct_context *ct_ctx) {}

static void hvgr_gpu_fault_msg_init(struct hvgr_device * const gdev)
{
	struct hvgr_gpu_fault_msg * const gpu_fault_msg = &gdev->datan_dev.gpu_fault_msg;

	hvgr_info(gdev, HVGR_DATAN, "%s IN", __func__);
	atomic_set(&gpu_fault_msg->gpu_fault_flag, 0);
	hvgr_info(gdev, HVGR_DATAN, "%s OUT", __func__);
}

int hvgr_datan_init(struct hvgr_device * const gdev)
{
#if defined(CONFIG_HVGR_DFX_MEM_DUMP) || defined(CONFIG_HVGR_DFX_DEBUG_BUS_DUMP)
	mutex_init(&gdev->datan_dev.dump_dir_lock);
#endif

#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
	kdatan_bus_dump_thread_init(gdev);
#endif

#ifdef HVGR_DATAN_JOB_TIMELINE
	hvgr_datan_jt_probe(gdev);
#endif

	hvgr_api_ct_init_by_dev(gdev);

	hvgr_gpu_fault_msg_init(gdev);

#ifdef CONFIG_HVGR_DFX_MNTN_REPORT
	if (hvgr_mntn_init(gdev) != 0)
		hvgr_err(gdev, HVGR_DATAN, "hvgr mntn init failed!\n");
#endif


#if hvgr_version_eq(HVGR_V350)
	atomic_set(&gdev->datan_dev.tool_client_ref, 0);
#endif

	return 0;
}

void hvgr_datan_term(struct hvgr_device *gdev)
{
#ifdef CONFIG_HVGR_DFX_MNTN_REPORT
	hvgr_mntn_term(gdev);
#endif

#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
	kdatan_bus_dump_thread_term(gdev);
#endif

	hvgr_api_ct_term_by_dev(gdev->ct_dev.ct_ctx);
}

void hvgr_datan_dump_debug_bus(struct hvgr_device *gdev)
{
#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
	kdatan_debug_bus_start_dump(gdev);
#endif
}

void hvgr_datan_dump_mem(struct hvgr_ctx * const ctx)
{
}

void hvgr_datan_dump_chain_state(struct hvgr_device * const gdev)
{
#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
	kdatan_dump_chain_state(gdev);
#endif
}

void hvgr_datan_cbit_config(struct hvgr_device * const gdev)
{
#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
	kdatan_debug_bus_cbit_config(gdev);
#endif
}

void __attribute__((weak)) hvgr_api_ct_collect_and_disable(struct hvgr_device *gdev, bool dump_ct) {}

int __attribute__((weak)) hvgr_api_ct_enable_and_go_on(struct hvgr_device *gdev)
{
	return 0;
}

uint32_t __attribute__((weak)) hvgr_api_ct_get_dump_size(void)
{
	return 0;
}

void hvgr_datan_ct_enable_by_power(struct hvgr_device *gdev)
{
	if (hvgr_api_ct_enable_and_go_on(gdev) != 0)
		hvgr_info(gdev, HVGR_DATAN, "%s enable perf counter error.", __func__);
}

void hvgr_datan_ct_disable_by_power(struct hvgr_device *gdev, bool dump_ct)
{
	hvgr_api_ct_collect_and_disable(gdev, dump_ct);
}

void hvgr_datan_gpu_fault_type_set(struct hvgr_device *gdev, hvgr_gpu_fault_id gpu_fault_id,
	const char *gpu_fault_log)
{
	struct hvgr_gpu_fault_msg *gpu_fault_msg = NULL;
	errno_t err;

	if (gdev == NULL || gpu_fault_log == NULL)
		return;

	gpu_fault_msg = &gdev->datan_dev.gpu_fault_msg;

	if (atomic_cmpxchg(&gpu_fault_msg->gpu_fault_flag, 0, 1) != 0) {
		hvgr_debug(gdev, HVGR_DATAN, "hvgr gpu fault flag error: set %u when %u is reporting",
			gpu_fault_id, gpu_fault_msg->gpu_fault_id);
		return;
	}

	err = strncpy_s(gpu_fault_msg->gpu_fault_log,
		MAX_DMD_LOG_LENTH, gpu_fault_log, strlen(gpu_fault_log));
	if (err != EOK)
		hvgr_err(gdev, HVGR_DATAN, "hvgr set gpu_fault_log failed!");

	gpu_fault_msg->gpu_fault_id = gpu_fault_id;
	hvgr_debug(gdev, HVGR_DATAN, "hvgr gpu fault set fault type %u", gpu_fault_id);
}

void hvgr_datan_gpu_fault_type_reset(struct hvgr_device *gdev)
{
	struct hvgr_gpu_fault_msg *gpu_fault_msg = NULL;
	gpu_fault_msg = &gdev->datan_dev.gpu_fault_msg;
	atomic_set(&gpu_fault_msg->gpu_fault_flag, 0);
}
