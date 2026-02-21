/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "utils/comm_utils.h"
#include "dkmd_log.h"

#define MAX_UEVENT_BUFFER_LENGTH 120

static struct workqueue_struct *g_dptx_all_event_handle_wq = NULL;

void dptx_create_workqueue(void)
{
	if (g_dptx_all_event_handle_wq == NULL) {
		g_dptx_all_event_handle_wq = create_singlethread_workqueue("dptx_all_event_handle_wq");
		if (g_dptx_all_event_handle_wq == NULL)
			dpu_pr_err("[DP] create dptx_all_event_handle_wq failed");
	}
	return;
}
void dptx_destory_workqueue(void)
{
	if (g_dptx_all_event_handle_wq != NULL) {
		destroy_workqueue(g_dptx_all_event_handle_wq);
		g_dptx_all_event_handle_wq = NULL;
	}
}

void dptx_work_queue_handle(struct dp_ctrl *dptx, enum dptx_work_queue_event event_type)
{
	dpu_check_and_no_retval(!dptx || !g_dptx_all_event_handle_wq,  err, "[DP] NULL Pointer");
	switch (event_type) {
	case DPTX_GPIO_PLUG_EVENT:
		queue_work(g_dptx_all_event_handle_wq, &dptx->dptx_pluggable_work);
		break;
	case DPTX_PSR2_UEVENT_EVENT:
		queue_work(g_dptx_all_event_handle_wq, &dptx->dptx_psr2_uevent_work);
		break;
	default:
		dpu_pr_err("[DP] dptx_work_queue_handle event type %u is error", event_type);
		break;
	}
}
void switch_notification_event_unchange_state(struct switch_dev *sdev, uint32_t state)
{
	char *envp[3];
	char name_buffer[MAX_UEVENT_BUFFER_LENGTH] = {0};
	char state_buffer[MAX_UEVENT_BUFFER_LENGTH] = {0};
	int length;

	if (sdev == NULL) {
		dpu_pr_err("[DP] sdev is NULL!\n");
		return;
	}

	length = snprintf_s(name_buffer, sizeof(name_buffer), sizeof(name_buffer) - 1, "SWITCH_NAME=%s", sdev->name);
	if (length < 0 || length >= MAX_UEVENT_BUFFER_LENGTH) {
		dpu_pr_err("[DP] fatal: snprintf_s name_buffer fail, length=%d\n", length);
		return;
	}

	length = snprintf_s(state_buffer, sizeof(state_buffer), sizeof(state_buffer) - 1, "SWITCH_STATE=%u", state);
	if (length < 0 || length >= MAX_UEVENT_BUFFER_LENGTH) {
		dpu_pr_err("[DP] fatal: snprintf_s state_buffer fail, length=%d\n", length);
		return;
	}

	envp[0] = name_buffer;
	envp[1] = state_buffer;
	envp[2] = NULL;
	kobject_uevent_env(&sdev->dev->kobj, KOBJ_CHANGE, envp);

	dpu_pr_info("[DP] Notification: %s, %s.\n", name_buffer, state_buffer);
}