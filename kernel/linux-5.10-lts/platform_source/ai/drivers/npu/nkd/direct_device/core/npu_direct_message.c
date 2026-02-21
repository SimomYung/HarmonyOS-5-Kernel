/*
 * npu_message.c
 *
 * about npu sq/cq msg
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#include <linux/mutex.h>
#include <linux/list.h>

#include "npu_common.h"
#include "npu_log.h"
#include "npu_shm.h"
#include "npu_pm_framework.h"
#include "npu_direct_message.h"

static int npu_compose_task_check_stream(struct npu_proc_ctx *proc_ctx,
	u32 *stream_id)
{
	struct npu_id_entity *stream_sub = NULL;
	struct npu_stream_info *stream_info = NULL;
	int ret = 0;

	if (list_empty_careful(&proc_ctx->stream_list) == 0) {
		stream_sub = list_first_entry(&proc_ctx->stream_list, struct npu_id_entity, list);
		stream_info = npu_calc_stream_info(proc_ctx->devid, stream_sub->id);
		cond_return_error(stream_info == NULL, -ENODATA,
			"illegal npu stream info is null\n");
		*stream_id = stream_info->id;
		return ret;
	}

	ret = npu_proc_alloc_stream(proc_ctx, &stream_info, STREAM_STRATEGY_ASYNC, 0);
	if (ret != 0) {
		npu_drv_err("npu alloc stream failed\n");
		return -ENOKEY;
	}
	bitmap_set(proc_ctx->stream_bitmap, stream_info->id, 1);

	*stream_id = stream_info->id;
	return ret;
}

int npu_compose_task_for_request(struct npu_proc_ctx *proc_ctx, u32 model_id,
	npu_rt_task_t *task)
{
	npu_model_desc_sub_t model_desc_sub;
	u32 stream_id = 0;
	int ret;

	ret = npu_proc_get_model_info(proc_ctx, model_id, &model_desc_sub);
	cond_return_error(ret != 0, ret,
		"npu_proc_get_model_info error, ret= %d\n", ret);

	mutex_lock(&proc_ctx->stream_mutex);
	ret = npu_compose_task_check_stream(proc_ctx, &stream_id);
	mutex_unlock(&proc_ctx->stream_mutex);
	cond_return_error(ret != 0, ret,
		"npu_compose_task_check_stream error, ret = %d\n", ret);
	npu_drv_info("proc alloc stream success, stream id = %d\n", stream_id);

	task->stream_id = (u16)stream_id;
	task->next_task_id = 0;
	task->type = (model_desc_sub.sched_path == NPU_SCHED_HTS) ? NPU_TASK_MODEL_LOOP_EXECUTE : NPU_TASK_MODEL_EXECUTE;
	task->next_stream_id = 0;
	task->u.model_execute_task.model_id = (u16)model_id;
	task->u.model_execute_task.pid = current->tgid;
	task->u.model_execute_task.execute_pid = current->tgid;
	task->u.model_execute_task.priority = (u8)model_desc_sub.priority;
	task->task_flag.task_prof_enable = (u8)model_desc_sub.profiling;

	return ret;
}

int npu_compose_stop_task(struct npu_proc_ctx *proc_ctx, u32 model_id,
	npu_rt_task_t *task)
{
	npu_model_desc_sub_t model_desc_sub;
	u32 stream_id = 0;
	int ret;

	ret = npu_proc_get_model_info(proc_ctx, model_id, &model_desc_sub);
	cond_return_error(ret != 0, ret,
		"npu_proc_get_model_info error, ret= %d\n", ret);
	if (model_desc_sub.execute_pid != (u32)(current->tgid)) {
		npu_drv_err("current pid not consistent with model load id , model load id: %u, current id: %d\n",
		model_desc_sub.execute_pid, current->tgid);
		return -EFAULT;
	}
	if (model_desc_sub.sched_path != NPU_SCHED_HTS)
		return ENOMSG;
	mutex_lock(&proc_ctx->stream_mutex);
	ret = npu_compose_task_check_stream(proc_ctx, &stream_id);
	mutex_unlock(&proc_ctx->stream_mutex);
	cond_return_error(ret != 0, ret,
		"npu_compose_task_check_stream error, ret = %d\n", ret);
	npu_drv_info("proc alloc stream success, stream id = %d\n", stream_id);

	task->stream_id = (u16)stream_id;
	task->next_task_id = 0;
	task->type = NPU_TASK_MODEL_STOP;
	task->next_stream_id = 0;
	task->u.model_stop_task.model_id = (u16)model_id;

	return ret;
}