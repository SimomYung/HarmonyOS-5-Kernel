/*
 * drv_venc_thread.c
 *
 * This is for wireless low latency solution.
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "drv_venc_thread.h"
#include "drv_venc_osal.h"

static int32_t venc_drv_thread_process(void *data)
{
	int32_t ret = 0;
	struct venc_kthread *thread = (struct venc_kthread *)data;

	while (!kthread_should_stop()) {
		if (!thread || IS_ERR_OR_NULL(thread->task))
			break;

		wait_event(thread->wait_queue, thread->condition > 0 || kthread_should_stop());

		if (kthread_should_stop()) {
			VCODEC_WARN_VENC("stop thread");
			break;
		}

		if (thread->condition && thread->handler) {
			VENC_SET_EVENT(thread->lock, thread->condition--);
			ret = thread->handler(thread->data); // venc_thread_send_data_handler
			if (ret)
				VCODEC_WARN_VENC("thread process failed ret %d", ret);
		}
	}
	return 0;
}

int32_t venc_drv_create_thread(struct venc_kthread *thread, const char *name)
{
	thread->task = kthread_create(venc_drv_thread_process, thread, name);
	if (IS_ERR_OR_NULL(thread->task)) {
		VCODEC_ERR_VENC("create %s thread failed", name);
		return -EFAULT;
	}
	wake_up_process(thread->task);
	return 0;
}

void venc_drv_destroy_thread(struct venc_kthread *thread)
{
	if (thread->task) {
		kthread_stop(thread->task);
		thread->task = NULL;
	}
}