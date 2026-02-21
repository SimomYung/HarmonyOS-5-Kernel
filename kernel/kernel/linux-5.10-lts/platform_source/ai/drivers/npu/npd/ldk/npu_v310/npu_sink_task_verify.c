/*
 * npu_sqe_fmt.c
 *
 * about npu rt task
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
#include "npu_sink_task_verify.h"

#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>

#include "npu_log.h"
#include "npu_user_common.h"
#include "npu_soc_defines.h"

static int verify_task_comm_field(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task, u32 *core_type)
{
	if (hwts_task->type == NPU_TASK_KERNEL_AICORE ||
		hwts_task->type == NPU_TASK_TYPE_KERNEL_AIV)
		*core_type |= 0x1;
	if (hwts_task->type == NPU_TASK_KERNEL_AIC_EFF)
		*core_type |= 0x2;
	if (hwts_task->type >= NPU_TASK_RESERVED) {
		npu_drv_err("invalid task type:%u, task_id:%u\n",
			hwts_task->type, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->stream_id < NPU_MAX_NON_SINK_STREAM_ID ||
		npu_proc_check_stream_id(proc_ctx, hwts_task->stream_id) != 0) {
		npu_drv_err("invalid task stream_id:%u, task_id:%u\n",
			hwts_task->stream_id, hwts_task->task_id);
		return -1;
	}
	if ((hwts_task->stream_id <
		NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_SINK_LONG_STREAM_ID
		&& hwts_task->task_id >= NPU_MAX_SINK_LONG_TASK_ID) ||
		(hwts_task->stream_id >=
			NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_SINK_LONG_STREAM_ID
		&& hwts_task->task_id >= NPU_MAX_SINK_TASK_ID)) {
		npu_drv_err("invalid task_id:%u, stream id:%u\n",
			hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	return 0;
}

static int verify_aicore_task(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->u.kernel_task.block_dim == 0) {
		npu_drv_err("invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.pc_start == 0) {
		npu_drv_err("invalid pc_start is 0, task_id:%u\n",
			hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.param_base == 0) {
		npu_drv_err("invalid param_base is 0, task_id:%u\n",
			hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.rd_cond == 1 &&
		hwts_task->u.kernel_task.block_dim != 1) {
		npu_drv_err("rd_cond is 1, invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	return 0;
}

static int verify_aiv_task(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->u.kernel_task.block_dim == 0) {
		npu_drv_err("invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.pc_start == 0) {
		npu_drv_err("invalid pc_start is 0, task_id:%u\n",
			hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.param_base == 0) {
		npu_drv_err("invalid param_base is 0, task_id:%u\n",
			hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.rd_cond == 1 &&
		hwts_task->u.kernel_task.block_dim != 1) {
		npu_drv_err("rd_cond is 1, invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	return 0;
}

static int verify_aicore_eff_task(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->u.kernel_task.block_dim == 0) {
		npu_drv_err("invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.pc_start == 0) {
		npu_drv_err("invalid pc_start is 0, task_id:%u\n",
			hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.param_base == 0) {
		npu_drv_err("invalid param_base is 0, task_id:%u\n",
			hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.kernel_task.rd_cond == 1 &&
		hwts_task->u.kernel_task.block_dim != 1) {
		npu_drv_err("rd_cond is 1, invalid block_dim:%u, task_id:%u\n",
			hwts_task->u.kernel_task.block_dim, hwts_task->task_id);
		return -1;
	}
	return 0;
}

static int verify_notify_record(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	if (npu_proc_check_notify_id(proc_ctx, hwts_task->u.notify_record_task.notify_id) != 0) {
		npu_drv_err("invalid notify_id:%u, task_id:%u\n",
			hwts_task->u.notify_record_task.notify_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

static int verify_notify_wait(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	if (npu_proc_check_notify_id(proc_ctx, hwts_task->u.notify_wait_task.notify_id) != 0) {
		npu_drv_err("invalid notify_id:%u, task_id:%u\n",
			hwts_task->u.notify_wait_task.notify_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

static int verify_hts_event_record(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->u.hts_event_record_task.event_id >= NPU_HTS_EVENT_MAX_ID) {
		npu_drv_err("invalid eventid:%u, task_id:%u\n",
			hwts_task->u.hts_event_record_task.event_id, hwts_task->task_id);
		return -1;
	}
	if (hwts_task->u.hts_event_record_task.dst_id >= NPU_HTS_EVENT_MAX_DST_ID) {
		npu_drv_err("invalid dst_id:%u, task_id:%u\n",
			hwts_task->u.hts_event_record_task.dst_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}

static int verify_hts_event_wait(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->u.hts_event_wait_task.event_id >= NPU_HTS_EVENT_MAX_ID) {
		npu_drv_err("invalid eventid:%u, task_id:%u\n",
			hwts_task->u.hts_event_wait_task.event_id, hwts_task->task_id);
		return -1;
	}
	return 0;
}


static int verify_ph_task(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	unused(hwts_task);
	return 0;
}

static int verify_label_switch_task(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->stream_id >=
		(NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_switch_task.true_label_idx >=
		NPU_MAX_HWTS_SQ_DEPTH) {
		npu_drv_err("invalid true_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_switch_task.true_label_idx,
			hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	if (hwts_task->stream_id <
		(NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_switch_task.true_label_idx >=
		NPU_MAX_LONG_HWTS_SQ_DEPTH) {
		npu_drv_err("invalid true_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_switch_task.true_label_idx,
			hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	return 0;
}

static int verify_label_goto_task(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *hwts_task)
{
	unused(proc_ctx);
	if (hwts_task->stream_id >=
		(NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_goto_task.label_idx >=
		NPU_MAX_HWTS_SQ_DEPTH) {
		npu_drv_err("invalid goto_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_goto_task.label_idx,
			hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	if (hwts_task->stream_id <
		(NPU_MAX_NON_SINK_STREAM_ID + NPU_MAX_LONG_HWTS_SQ_NUM) &&
		hwts_task->u.label_goto_task.label_idx >=
		NPU_MAX_LONG_HWTS_SQ_DEPTH) {
		npu_drv_err("invalid goto_label_idx:%u, task_id:%u, stream_id:%u\n",
			hwts_task->u.label_goto_task.label_idx,
			hwts_task->task_id, hwts_task->stream_id);
		return -1;
	}
	return 0;
}

verify_hwts_task_func verify_hwts_task_map[] = {
	/* 0                		1     					2                    	3 */
	verify_aicore_task, 		verify_aicore_eff_task, NULL, 					NULL,
	/* 4                		5                   	6      					7 */
	verify_ph_task,     		NULL, 					NULL,      				NULL,
	/* 8            			9       				10                      11 */
	NULL,   					NULL,   				verify_hts_event_record, verify_hts_event_wait,
	/* 12           			13      				14                      15 */
	NULL,      					NULL,   				NULL,     				NULL,
	/* 16           			17      				18                      19 */
	NULL,           			NULL,   				NULL,                   NULL,
	/* 20           			21                      22                      23 */
	verify_ph_task,   verify_label_switch_task,   verify_label_goto_task, 		NULL,
	/* 24           			25                      26                      27 */
	NULL,           		verify_aiv_task,          	verify_notify_record,    verify_notify_wait,
	/* 28           29                        30                      31 */
	NULL,			NULL,					  NULL,                   NULL,
	/* 32           33                        34                      35 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 36           37                        38                      39 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 40           41                        42                      43 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 44           45                        46                      47 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 48           49                        50                      51 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 52           53                        54                      55 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 56           57                        58                      59 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 60           61                        62                      63 */
	NULL,           NULL,                     NULL,                   NULL,
	/* 64           65                        66                      67 */
	NULL,           NULL,                     NULL,                   NULL,
};

int npu_verify_sink_tasks(struct npu_proc_ctx *proc_ctx,
	void *stream_buf_addr, u64 ts_sq_len, u32 *core_type)
{
	u64 ts_sqe_num = ts_sq_len / NPU_RT_TASK_SIZE;
	verify_hwts_task_func verify_func = NULL;
	u8 *stream_buf_addr_base = stream_buf_addr;
	npu_rt_task_t *hwts_task = NULL;
	int ret = 0;
	u32 i;

	if (stream_buf_addr == NULL) {
		npu_drv_err("stream buf addr is null\n");
		return -1;
	}

	npu_drv_debug("stream_buf_addr:0x%pK, ts_sq_len:%llu, ts_sqe_num:%llu",
		(uintptr_t)stream_buf_addr, ts_sq_len, ts_sqe_num);
	for (i = 0; i < ts_sqe_num; i++) {
		hwts_task =
			(npu_rt_task_t *)rt_task_entry(stream_buf_addr_base, i);

		ret = verify_task_comm_field(proc_ctx, hwts_task, core_type);
		if (ret != 0) {
			npu_drv_err("verify task comm field failed, ret:%d, task_id:%u, type:%u\n",
				ret, hwts_task->task_id, hwts_task->type);
			return -1;
		}

		if (hwts_task->type >= NPU_TASK_RESERVED) {
			npu_drv_err("invalid task type:%u, task_id:%u\n",
				hwts_task->type, hwts_task->task_id);
			return -1;
		}
		verify_func = verify_hwts_task_map[hwts_task->type];
		if (verify_func == NULL) {
			npu_drv_err("invalid stream_id:%u, task_id:%u, type:%u, total:%llu, index:%u.\n",
				hwts_task->stream_id, hwts_task->task_id, hwts_task->type,
				ts_sqe_num, i);
			return -1;
		}
		ret = verify_func(proc_ctx, hwts_task);
		if (ret != 0) {
			npu_drv_err("verify hwts sqe failed, ret:%d, task_id:%u, type:%u\n",
				ret, hwts_task->task_id, hwts_task->type);
			return -1;
		}
	}

	npu_drv_debug("verify hwts sqe end\n");
	return 0;
}
