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

#ifdef TA_PATH_LOADED_BY_TZDRIVER
#define MAX_PATH_LEN (256)

struct ta_load_struct {
	unsigned char uuid[UUID_LEN];
	char **file_buffer;
	uint32_t *file_size;
	struct completion load_complete;
};
 
static int ta_load_thread_fn(void *arg)
{
	struct ta_load_struct *ta_load_arg = arg;
	char file_path[MAX_PATH_LEN] = { 0 };
	if (ta_load_arg == NULL || ta_load_arg->uuid == NULL ||
		ta_load_arg->file_buffer == NULL || ta_load_arg->file_size == NULL) {
		tloge("invalid params\n");
		return -1;
	}

	struct tc_uuid *uuid = (struct tc_uuid *)ta_load_arg->uuid;

	int32_t ret = snprintf_s(file_path, MAX_PATH_LEN, MAX_PATH_LEN - 1,
		"%s/%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x.sec",
		TA_PATH_LOADED_BY_TZDRIVER,
		uuid->time_low, uuid->time_mid,
		uuid->timehi_and_version,
		uuid->clockseq_and_node[0], uuid->clockseq_and_node[1],
		uuid->clockseq_and_node[2], uuid->clockseq_and_node[3],
		uuid->clockseq_and_node[4], uuid->clockseq_and_node[5],
		uuid->clockseq_and_node[6], uuid->clockseq_and_node[7]);
	if (ret < 0) {
		tloge("get file path failed\n");
		complete(&(ta_load_arg->load_complete));
		return -1;
	}

	ret = teek_get_app((const char *)file_path, ta_load_arg->file_buffer, ta_load_arg->file_size);
	if (ret != 0) {
		tloge("get file buff failed\n");
		complete(&(ta_load_arg->load_complete));
		return -1;
	}

	complete(&(ta_load_arg->load_complete));
	return 0;
}
 
bool load_ta_from_default_path(struct tc_ns_client_context *context, bool *is_kernal_load)
{
	if (context == NULL || is_kernal_load == NULL) {
		tloge("invalid params\n");
		return false;
	}
	struct task_struct *ta_load_thread = NULL;
	struct ta_load_struct ta_load_arg = { { 0 }, NULL, NULL, { 0 } };
	errno_t err = memcpy_s(ta_load_arg.uuid, sizeof(ta_load_arg.uuid), context->uuid, sizeof(context->uuid));
	if (err != 0) {
		tloge("copy uuid fail %d\n", err);
		return false;
	}
	ta_load_arg.file_buffer = &(context->file_buffer);
	ta_load_arg.file_size = &(context->file_size);
 
	init_completion(&(ta_load_arg.load_complete));
	ta_load_thread = kthread_create(ta_load_thread_fn, &ta_load_arg, "ta_load_fn");
	if (unlikely(IS_ERR_OR_NULL(ta_load_thread))) {
		tloge("load app create kthread failed\n");
		return false;
	}
 
	wake_up_process(ta_load_thread);
	wait_for_completion(&(ta_load_arg.load_complete));
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
