/*
 * npu_sink_sqe_fmt.c
 *
 * about npu hwts sqe format
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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
#include "npu_sink_sqe_fmt.h"

#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include "npu_log.h"
#include "npu_sink_task_verify.h"
#include "npu_rt_task.h"
#include "npu_hwts_plat.h"
#include "npu_soc_defines.h"
#include "npu_pm_framework.h"
#ifdef CONFIG_FFRT_UNIFORM_ID
#include "uniform_id.h"
#endif

#ifdef CONFIG_NPU_OHOS
#include "npu_udk.h"
#endif
#include "hwts_driver.h"
#ifdef CONFIG_LIBLINUX
#include <liblinux/pal.h>
#endif

typedef void (*format_hwts_sqe_func)(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task, npu_model_desc_info_t *model_desc_info);

static void format_aicore_aiv_sqe(void *hwts_sqe_addr, npu_rt_task_t *hwts_task,
	unsigned int type)
{
	struct hwts_kernel_sqe *kernel_sqe = (struct hwts_kernel_sqe *)hwts_sqe_addr;
	kernel_sqe->type = type;
	kernel_sqe->l1_lock = 0;
	kernel_sqe->l1_unlock = 0;
	kernel_sqe->ie = 0;
	kernel_sqe->pre_p = 0;
	kernel_sqe->post_p = 0;
	kernel_sqe->wr_cqe = 0;
	kernel_sqe->rd_cond = hwts_task->u.kernel_task.rd_cond;
	kernel_sqe->res0 = 0;
	kernel_sqe->block_dim = hwts_task->u.kernel_task.block_dim;
	kernel_sqe->stream_id = hwts_task->stream_id;
	kernel_sqe->task_id = hwts_task->task_id;
	kernel_sqe->res1 = 0;
	kernel_sqe->res2 = 0;
	kernel_sqe->kernel_credit = AIC_AIV_SQE_KERNEL_CREDIT; // (Kernel_Credit +1) *2^({ip}_task_runtime_limit_exp)
	kernel_sqe->res3 = 0;

	kernel_sqe->schem = 0;
	kernel_sqe->res4 = 0;
	kernel_sqe->icache_prefetch_cnt = 1;
	kernel_sqe->wrr_ratio_wr = 0; // 1:1
	kernel_sqe->wrr_ratio_rd = 0;
	kernel_sqe->res5 = 0;
	kernel_sqe->res6 = 0;
	kernel_sqe->stack_phy_base_low = (u32)(hwts_task->u.kernel_task.stack_buff_addr);
	kernel_sqe->stack_phy_base_high =
		(u32)((hwts_task->u.kernel_task.stack_buff_addr) >> 32);

	kernel_sqe->task_start_pc_low = (u32)(hwts_task->u.kernel_task.pc_start);
	kernel_sqe->task_start_pc_high = (u16)((hwts_task->u.kernel_task.pc_start) >> 32);
	kernel_sqe->res7 = 0;
	kernel_sqe->task_param_ptr_low = (u32)(hwts_task->u.kernel_task.param_base);
	kernel_sqe->task_param_ptr_high =
		(u32)((hwts_task->u.kernel_task.param_base) >> 32);

	kernel_sqe->res8 = 0;
	kernel_sqe->res9 = 0;
	kernel_sqe->res10 = 0;
	kernel_sqe->res11 = 0;

	npu_drv_debug("kernel_sqe= %pK, struct size= %d, stream_id= %u, task_id= %u, "
		"pc_start= 0x%pK, param_base= 0x%pK, rd_cond= %u, task_type = %u\n",
		kernel_sqe,
		sizeof(struct hwts_kernel_sqe),
		hwts_task->stream_id,
		hwts_task->task_id,
		hwts_task->u.kernel_task.pc_start,
		hwts_task->u.kernel_task.param_base,
		hwts_task->u.kernel_task.rd_cond,
		kernel_sqe->type);
}

static void format_aicore_sqe(void *hwts_sqe_addr, npu_rt_task_t *hwts_task,
	npu_model_desc_info_t *model_desc_info)
{
	unused(model_desc_info);
	format_aicore_aiv_sqe(hwts_sqe_addr, hwts_task, NPU_HWTS_SQE_AICORE);
}

static void format_aiv_sqe(void *hwts_sqe_addr, npu_rt_task_t *hwts_task,
		npu_model_desc_info_t *model_desc_info)
{
	unused(model_desc_info);
	format_aicore_aiv_sqe(hwts_sqe_addr, hwts_task, NPU_HWTS_SQE_VECTOR_CORE);
}

static void format_aicore_eff_sqe(void *hwts_sqe_addr, npu_rt_task_t *hwts_task,
	npu_model_desc_info_t *model_desc_info)
{
	unused(model_desc_info);
	format_aicore_aiv_sqe(hwts_sqe_addr, hwts_task, NPU_HWTS_SQE_AICORE_EFF);
}

static void format_ph_sqe(void *hwts_sqe_addr, npu_rt_task_t *hwts_task,
	npu_model_desc_info_t *model_desc_info)
{
	struct hwts_ph_sqe *ph_sqe = (struct hwts_ph_sqe *)hwts_sqe_addr;
	unused(model_desc_info);

	ph_sqe->type = NPU_HWTS_SQE_PLACE_HOLDER;
	ph_sqe->ie = 0;
	ph_sqe->pre_p = 0;
	ph_sqe->post_p = 0;
	ph_sqe->wr_cqe = 0;
	ph_sqe->res0 = 0;
	ph_sqe->task_type = NPU_HWTS_PH_SQE_NORMAL;

	ph_sqe->stream_id = hwts_task->stream_id;
	ph_sqe->task_id   = hwts_task->task_id;

	npu_drv_debug("ph_sqe->task_id= %u\n", ph_sqe->task_id);
}

static void format_label_switch_sqe(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task,
	npu_model_desc_info_t *model_desc_info)
{
	struct hwts_ph_sqe *ph_sqe = (struct hwts_ph_sqe *)hwts_sqe_addr;
	unused(model_desc_info);

	ph_sqe->type = NPU_HWTS_SQE_PLACE_HOLDER;
	ph_sqe->ie = 0;
	ph_sqe->pre_p = 1;
	ph_sqe->post_p = 0;
	ph_sqe->wr_cqe = 0;
	ph_sqe->res0 = 0;
	ph_sqe->task_type = NPU_HWTS_PH_SQE_LABEL_SWITCH;

	ph_sqe->stream_id = hwts_task->stream_id;
	ph_sqe->task_id   = hwts_task->task_id;

	ph_sqe->u.label_switch.condition =
		hwts_task->u.label_switch_task.condition;
	ph_sqe->u.label_switch.right = hwts_task->u.label_switch_task.right;
	ph_sqe->u.label_switch.true_label_idx =
		hwts_task->u.label_switch_task.true_label_idx;

	npu_drv_debug("ph_sqe->task_id= %u, label_idx= %u, right= %llu, condition= %u\n",
		ph_sqe->task_id,
		hwts_task->u.label_switch_task.true_label_idx,
		hwts_task->u.label_switch_task.right,
		hwts_task->u.label_switch_task.condition);
}

static void format_label_goto_sqe(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task, npu_model_desc_info_t *model_desc_info)
{
	struct hwts_ph_sqe *ph_sqe = (struct hwts_ph_sqe *)hwts_sqe_addr;
	unused(model_desc_info);

	ph_sqe->type = NPU_HWTS_SQE_PLACE_HOLDER;
	ph_sqe->ie = 0;
	ph_sqe->pre_p = 1;
	ph_sqe->post_p = 0;
	ph_sqe->wr_cqe = 0;
	ph_sqe->res0 = 0;
	ph_sqe->task_type = NPU_HWTS_PH_SQE_LABEL_GOTO;

	ph_sqe->stream_id = hwts_task->stream_id;
	ph_sqe->task_id   = hwts_task->task_id;

	ph_sqe->u.label_goto.label_idx = hwts_task->u.label_goto_task.label_idx;

	npu_drv_debug("ph_sqe->task_id= %u, label_idx= %u\n",
		ph_sqe->task_id, hwts_task->u.label_goto_task.label_idx);
}

static void add_model_notify_id(npu_model_desc_info_t *model_desc_info, u16 notify_id)
{
	// uint64_t bit map
	u8 group = (u8)(notify_id / 64);
	u64 offset = notify_id % 64;

	uint64_t notify_map = (uint64_t)0x1 << offset;
	model_desc_info->notify_info[group] |= notify_map;
}

static void format_notify_rec_task(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task, npu_model_desc_info_t *model_desc_info)
{
	struct hwts_notify_sqe *notify_rec_sqe =
		(struct hwts_notify_sqe *)hwts_sqe_addr;

	notify_rec_sqe->type = NPU_HWTS_SQE_NOTIFY_RECORD;
	notify_rec_sqe->ie = 0;
	notify_rec_sqe->pre_p = 0;
	notify_rec_sqe->post_p = 0;
	notify_rec_sqe->wr_cqe = 0;

	notify_rec_sqe->stream_id = hwts_task->stream_id;
	notify_rec_sqe->task_id = hwts_task->task_id;
	notify_rec_sqe->notify_id = hwts_task->u.notify_record_task.notify_id;

	add_model_notify_id(model_desc_info, notify_rec_sqe->notify_id);
	npu_drv_debug("notify_rec_sqe->task_id= %u, notify_id= %u\n",
		notify_rec_sqe->task_id, notify_rec_sqe->notify_id);
}

static void format_wait_notify_task(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task, npu_model_desc_info_t *model_desc_info)
{
	struct hwts_notify_sqe *notify_wait_sqe =
		(struct hwts_notify_sqe *)hwts_sqe_addr;
	unused(model_desc_info);

	notify_wait_sqe->type = NPU_HWTS_SQE_NOTIFY_WAIT;
	notify_wait_sqe->ie = 0;
	notify_wait_sqe->pre_p = 0;
	notify_wait_sqe->post_p = 0;
	notify_wait_sqe->wr_cqe = 0;

	notify_wait_sqe->stream_id = hwts_task->stream_id;
	notify_wait_sqe->task_id = hwts_task->task_id;
	notify_wait_sqe->notify_id = hwts_task->u.notify_wait_task.notify_id;
	notify_wait_sqe->swapout_credit = 2;
	npu_drv_debug("notify_wait_sqe->task_id= %u, notify_id= %u\n",
		notify_wait_sqe->task_id, notify_wait_sqe->notify_id);
}

static void format_hts_evt_record(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task, npu_model_desc_info_t *model_desc_info)
{
#ifdef CONFIG_FFRT_UNIFORM_ID
	struct hwts_hts_event_sqe *hts_event_sqe =
		(struct hwts_hts_event_sqe *)hwts_sqe_addr;
	pid_t pid = (pid_t)(hwts_task->u.hts_event_record_task.pid);
	unused(model_desc_info);

	hts_event_sqe->type = NPU_HWTS_SQE_HTS_EVENT_RECORD;
	hts_event_sqe->ie = 0;
	hts_event_sqe->pre_p = 0;
	hts_event_sqe->post_p = 0;
	hts_event_sqe->wr_cqe = 0;

	hts_event_sqe->stream_id = hwts_task->stream_id;
	hts_event_sqe->task_id = hwts_task->task_id;
	hts_event_sqe->event_id = hwts_task->u.hts_event_record_task.event_id;
	hts_event_sqe->dst_id = hwts_task->u.hts_event_record_task.dst_id;
#ifdef CONFIG_LIBLINUX
	pid = liblinux_pal_thread_vtid_to_root_tid(pid);
#endif
	hts_event_sqe->uid = ffrt_get_uniform_id(pid);

	npu_drv_debug("hts_event_sqe->task_id= %u, event_id= %u\n",
		hts_event_sqe->task_id, hts_event_sqe->event_id);
#else
	npu_drv_err("not define micro CONFIG_FFRT_UNIFORM_ID");
#endif
}

static void format_hts_evt_wait(void *hwts_sqe_addr,
	npu_rt_task_t *hwts_task, npu_model_desc_info_t *model_desc_info)
{
	struct hwts_hts_event_sqe *hts_event_sqe =
		(struct hwts_hts_event_sqe *)hwts_sqe_addr;
	struct npu_dev_ctx *dev_ctx = NULL;
	unused(model_desc_info);
	dev_ctx = get_dev_ctx_by_id(0);

	hts_event_sqe->type = NPU_HWTS_SQE_HTS_EVENT_WAIT;
	hts_event_sqe->ie = 0;
	hts_event_sqe->pre_p = 0;
	hts_event_sqe->post_p = 0;
	hts_event_sqe->wr_cqe = 0;
	hts_event_sqe->swapout_credit = 1;

	hts_event_sqe->stream_id = hwts_task->stream_id;
	hts_event_sqe->task_id = hwts_task->task_id;
	hts_event_sqe->event_id = hwts_task->u.hts_event_wait_task.event_id;

#ifdef CONFIG_NPU_FFRT
	npu_pm_safe_call(dev_ctx, NPU_IP_NON_TOP,
		(void)npu_clear_hts_event_tbl(hwts_task->u.hts_event_wait_task.event_id));

	npu_drv_debug("hts_event_sqe->task_id= %u, event_id= %u\n",
		hts_event_sqe->task_id, hts_event_sqe->event_id);
#else
	npu_drv_debug("not support ffrt.\n");
#endif
}

format_hwts_sqe_func format_hwts_sqe_map[] = {
/*	0								1								2							3			*/
	format_aicore_sqe,				format_aicore_eff_sqe,			NULL,						NULL,
/*	4								5								6							7			*/
	format_ph_sqe,					NULL,							NULL,						NULL,
/*	8								9								10							11			*/
	NULL,			        		NULL,							format_hts_evt_record,		format_hts_evt_wait,
/*	12								13								14							15			*/
	NULL,							NULL,							NULL,						NULL,
/*	16								17								18							19			*/
	NULL,							NULL,							NULL,						NULL,
/*	20								21								22							23			*/
	format_ph_sqe,					format_label_switch_sqe,		format_label_goto_sqe,		NULL,
/*	24								25								26							27			*/
	NULL,							format_aiv_sqe,					format_notify_rec_task,		format_wait_notify_task,
/*	28								29								30							31			*/
	NULL,							NULL,							NULL,						NULL,
/*	32								33								34							35			*/
	NULL,							NULL,							NULL,						NULL,
/*	36								37								38							39			*/
	NULL,							NULL,							NULL,						NULL,
/*	40								41								42							43			*/
	NULL,							NULL,							NULL,						NULL,
/*	44								45								46							47			*/
	NULL,							NULL,							NULL,						NULL,
/*	48								49								50							51			*/
	NULL,							NULL,							NULL,						NULL,
/*	52								53								54							55			*/
	NULL,							NULL,							NULL,						NULL,
/*	56								57								58							59			*/
	NULL,							NULL,							NULL,						NULL,
/*	60								61								62							63			*/
	NULL,							NULL,							NULL,						NULL,
/*	64								65								66							67			*/
	NULL,							NULL,							NULL,						NULL,
};

static inline void npu_format_head_sqe(struct hwts_sqe_head *hwts_sqe_head)
{
	unused(hwts_sqe_head);
	return;
}

static inline void npu_format_tail_sqe(struct hwts_sqe_head *hwts_sqe_head)
{
	hwts_sqe_head->wr_cqe = 1;
}

int npu_format_hw_sink_sqe(npu_model_desc_info_t *model_desc_info, void *stream_buf_addr,
	void *hwts_sq_addr, u64 ts_sq_len)
{
	u64 ts_sqe_num = ts_sq_len / NPU_RT_TASK_SIZE;
	format_hwts_sqe_func format_func = NULL;
	u8 *hwts_sq_base = hwts_sq_addr;
	u8 *stream_buf_addr_base = stream_buf_addr;
	u8 *hwts_sqe = NULL;
	npu_rt_task_t *hwts_task = NULL;
	u32 i;

	if (hwts_sq_addr == NULL) {
		npu_drv_err("hwts sq addr is null\n");
		return -1;
	}
	if (stream_buf_addr == NULL) {
		npu_drv_err("stream buf addr is null\n");
		return -1;
	}
	if (ts_sqe_num == 0) {
		npu_drv_err("ts_sqe_num is 0\n");
		return 0;
	}

	npu_drv_debug("hwts_sq_addr:0x%pK, stream_buf_addr:0x%pK, ts_sq_len:%llu, ts_sqe_num:%llu",
		(uintptr_t)hwts_sq_addr, (uintptr_t)stream_buf_addr, ts_sq_len,
		ts_sqe_num);
	for (i = 0; i < ts_sqe_num; i++) {
		hwts_task = (npu_rt_task_t *)rt_task_entry(
			stream_buf_addr_base, i);
		hwts_sqe = hwts_sqe_entry(hwts_sq_base, i);

		format_func = format_hwts_sqe_map[hwts_task->type];
		if (format_func == NULL) {
			npu_drv_err("invalid task_id:%u, type:%u\n",
				hwts_task->task_id, hwts_task->type);
			return -1;
		}
		format_func((void *)hwts_sqe, hwts_task, model_desc_info);
		npu_drv_debug("type = %u\n", hwts_task->type);
	}

	npu_format_head_sqe((struct hwts_sqe_head *)hwts_sqe_entry(
		hwts_sq_base, 0));
	npu_format_tail_sqe((struct hwts_sqe_head *)hwts_sqe_entry(
		hwts_sq_base, ts_sqe_num - 1));
	npu_drv_debug("end\n");
	return (int)ts_sqe_num;
}
