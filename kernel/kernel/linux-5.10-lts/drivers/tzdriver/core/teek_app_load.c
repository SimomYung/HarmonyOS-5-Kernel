/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "teek_app_load.h"
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <securec.h>
#include "session_manager.h"
#include "ko_adapt.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"

void teek_free_app(bool load_app_flag, char **file_buf)
{
	if (load_app_flag && file_buf != NULL && *file_buf != NULL) {
		vfree(*file_buf);
		*file_buf = NULL;
	}
}

int32_t teek_get_app(const char *ta_path, char **file_buf, uint32_t *file_len)
{
	int32_t ret = teek_read_app(ta_path, file_buf, file_len);
	if (ret != TEEC_SUCCESS)
		tloge("teec load app error %d\n", ret);

	return ret;
}

#if defined(TA_PATH_LOADED_BY_TZDRIVER) || defined(CONFIG_ENABLE_TA_CTRL)
struct file_load_struct {
	const char *file_path;
	char **file_buffer;
	uint32_t *file_size;
	struct completion load_complete;
};

static int file_load_thread_fn(void *arg)
{
	struct file_load_struct *file_load_arg = arg;
	if (file_load_arg == NULL || file_load_arg->file_buffer == NULL ||
		file_load_arg->file_size == NULL || file_load_arg->file_path == NULL) {
		tloge("invalid params\n");
		return -1;
	}

	int32_t ret = teek_get_app(file_load_arg->file_path, file_load_arg->file_buffer, file_load_arg->file_size);
	if (ret != 0) {
		tloge("get file buff failed, file name=%s\n", file_load_arg->file_path);
		complete(&(file_load_arg->load_complete));
		return -1;
	}

	complete(&(file_load_arg->load_complete));
	return 0;
}

bool tzdriver_load_file(const char *file_path, char **file_buffer, uint32_t *file_size)
{
	if (file_buffer == NULL || file_path == NULL || file_size == NULL) {
		tloge("invalid params\n");
		return false;
	}
	struct task_struct *load_file_thread = NULL;
	struct file_load_struct file_load_arg = { NULL, NULL, NULL, { 0 } };
	file_load_arg.file_path = file_path;
	file_load_arg.file_buffer = file_buffer;
	file_load_arg.file_size = file_size;
	init_completion(&(file_load_arg.load_complete));
	load_file_thread = kthread_create(file_load_thread_fn, &file_load_arg, "file_load_fn");
	if (unlikely(IS_ERR_OR_NULL(load_file_thread))) {
		tloge("load app create kthread failed\n");
		return false;
	}

	wake_up_process(load_file_thread);
	wait_for_completion(&(file_load_arg.load_complete));
	if (file_buffer != NULL && *file_buffer != NULL) {
		return true;
	}
	return false;
}
#endif
 
#ifdef TA_PATH_LOADED_BY_TZDRIVER
#define MAX_PATH_LEN (256)
bool load_ta_from_default_path(struct tc_ns_client_context *context, bool *is_kernal_load)
{
	if (context == NULL || is_kernal_load == NULL || context->uuid == NULL) {
		tloge("invalid params\n");
		return false;
	}

	struct tc_uuid *uuid = (struct tc_uuid *)context->uuid;
	char file_path[MAX_PATH_LEN] = { 0 };
	int32_t ret = snprintf_s(file_path, MAX_PATH_LEN, MAX_PATH_LEN - 1,
		"%s/%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x.sec",
		TA_PATH_LOADED_BY_TZDRIVER,
		uuid->time_low, uuid->time_mid, uuid->timehi_and_version,
		uuid->clockseq_and_node[0], uuid->clockseq_and_node[1],
		uuid->clockseq_and_node[2], uuid->clockseq_and_node[3],
		uuid->clockseq_and_node[4], uuid->clockseq_and_node[5],
		uuid->clockseq_and_node[6], uuid->clockseq_and_node[7]);
	if (ret < 0) {
		tloge("get file path failed\n");
		return false;
	}

	bool load_flag = tzdriver_load_file(file_path, &(context->file_buffer), &(context->file_size));
	if (context->file_buffer != NULL) {
		context->memref.file_addr = (uint32_t)(uintptr_t)context->file_buffer;
		context->memref.file_h_addr = (uint32_t)(((uint64_t)(uintptr_t)context->file_buffer) >> ADDR_TRANS_NUM);
		*is_kernal_load = true;
		return true;
	}
	return false;
}
#else
bool load_ta_from_default_path(struct tc_ns_client_context *context, bool *is_kernal_load)
{
	(void)context;
	*is_kernal_load = false;
	return false;
}
#endif
