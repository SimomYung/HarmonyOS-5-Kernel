/*
 * drv_venc_queue.c
 *
 * This is for wireless lowdelay solution.
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

#include "drv_venc_osal.h"
#include "venc_vsp.h"
#include "drv_common.h"
#include "drv_venc_queue.h"

#define FEEDBACK_TIMEOUT_MS 500

static DEFINE_SPINLOCK(g_venc_wireless_spinlock);

static void venc_drv_free_ctrl_queue(struct venc_ctrl_queue *ctrl_queue)
{
	if (ctrl_queue) {
		free_queue(ctrl_queue);
		destory_queue(ctrl_queue);
		ctrl_queue = NULL;
	}
}

static int32_t venc_drv_alloc_ctrl_queue(struct venc_wireless_entry *entry)
{
	int32_t i;
	struct venc_ctrl_queue *ctrl_queue = NULL;

	ctrl_queue = create_queue(struct venc_ctrl_queue);
	if (!ctrl_queue) {
		VCODEC_ERR_VENC("create ctrl queue failed ");
		return -ENOMEM;
	}

	if (alloc_queue(ctrl_queue, MAX_QUEUE_SIZE) != 0) {
		VCODEC_ERR_VENC("alloc ctrl queue failed");
		destory_queue(ctrl_queue);
		return -ENOMEM;
	}

	for (i = 0; i < MAX_QUEUE_SIZE; i++)
		push(ctrl_queue, &entry->ctrl_pool[i]);

	if (queue_data_len(ctrl_queue) != MAX_QUEUE_SIZE) {
		VCODEC_FATAL_VENC("ctrl_queue size error, size %d", queue_data_len(ctrl_queue));
		venc_drv_free_ctrl_queue(ctrl_queue);
		return -ENOMEM;
	}

	entry->ctrl_queue = ctrl_queue;
	return 0;
}

static struct venc_fifo_buffer *venc_drv_create_fifo_buffer_queue(void)
{
	struct venc_fifo_buffer *queue = create_queue(struct venc_fifo_buffer);
	if (!queue) {
		VCODEC_ERR_VENC("create venc fifo buffer failed");
		return NULL;
	}

	if (alloc_queue(queue, MAX_QUEUE_SIZE) != 0) {
		VCODEC_ERR_VENC("alloc venc fifo buffer failed");
		destory_queue(queue);
		return NULL;
	}

	return queue;
}

static void venc_drv_destory_fifo_buffer_queue(struct venc_fifo_buffer *queue)
{
	if (queue) {
		free_queue(queue);
		destory_queue(queue);
		queue = NULL;
	}
}

int32_t venc_drv_wireless_create_queue(struct venc_wireless_entry *entry, struct venc_fifo_buffer *encdone_queue)
{
	int32_t ret;
	unsigned long flags;
	struct venc_fifo_buffer *send_queue = NULL;
	struct venc_fifo_buffer *backup_queue = NULL;

	send_queue = venc_drv_create_fifo_buffer_queue();
	if (!send_queue) {
		VCODEC_FATAL_VENC("create encode send queue failed");
		ret = -ENOMEM;
		return ret;
	}

	backup_queue = venc_drv_create_fifo_buffer_queue();
	if (!backup_queue) {
		VCODEC_FATAL_VENC("create channel info backup queue failed");
		ret = -ENOMEM;
		goto backup_queue_error;
	}

	ret = venc_drv_alloc_ctrl_queue(entry);
	if (ret) {
		VCODEC_FATAL_VENC("alloc ctrl queue failed");
		goto ctrl_queue_error;
	}

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	entry->send_queue = send_queue;
	entry->backup_queue = backup_queue;
	entry->encdone_queue = encdone_queue;
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);

	return 0;

ctrl_queue_error:
	venc_drv_destory_fifo_buffer_queue(backup_queue);
backup_queue_error:
	venc_drv_destory_fifo_buffer_queue(send_queue);

	return ret;
}

void venc_drv_wireless_destroy_queue(struct venc_wireless_entry *entry)
{
	unsigned long flags;
	struct venc_ctrl_queue *ctrl_queue = NULL;
	struct venc_fifo_buffer *send_queue = NULL;
	struct venc_fifo_buffer *backup_queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	ctrl_queue = entry->ctrl_queue;
	backup_queue = entry->backup_queue;
	send_queue = entry->send_queue;
	entry->ctrl_queue = NULL;
	entry->backup_queue = NULL;
	entry->send_queue = NULL;
	entry->encdone_queue = NULL;
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
	venc_drv_free_ctrl_queue(ctrl_queue);
	venc_drv_destory_fifo_buffer_queue(backup_queue);
	venc_drv_destory_fifo_buffer_queue(send_queue);
}

struct _tx_layer_ctrl *venc_drv_get_idle_ctrl(struct venc_wireless_entry *entry)
{
	unsigned long flags;
	struct _tx_layer_ctrl *ctrl = NULL;
	struct venc_ctrl_queue *queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	queue = entry->ctrl_queue;
	if (queue == NULL) {
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return NULL;
	}

	if (pop(queue, &ctrl)) {
		VCODEC_ERR_VENC("get ctrl buf failed");
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return NULL;
	}

	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
	return ctrl;
}

void venc_drv_release_idle_ctrl(struct venc_wireless_entry *entry, struct _tx_layer_ctrl *head)
{
	unsigned long flags;
	struct _tx_layer_ctrl *ctrl = head;
	struct venc_ctrl_queue *queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	queue = entry->ctrl_queue;
	if (queue == NULL) {
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return;
	}

	while (ctrl && queue) {
		push(queue, &ctrl);
		ctrl = (struct _tx_layer_ctrl *)(uintptr_t)ctrl->next;
	}
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
}

int32_t wait_queue_filled(struct venc_feedback_queue *queue)
{
	int32_t ret;
	ret = OSAL_WAIT_EVENT_TIMEOUT(&queue->event,
		!queue_is_empty(queue), FEEDBACK_TIMEOUT_MS);
	if (ret)
		VCODEC_ERR_VENC("wait feedback queue filled timeout");

	return ret;
}

void venc_drv_fill_feedback(struct venc_feedback_queue *queue, struct wifi_feedback_info *info)
{
	if (queue == NULL) {
		VCODEC_ERR_VENC("invalid queue");
		return;
	}

	push(queue, info);
	venc_drv_osal_give_event(&queue->event);
}

void venc_drv_fill_encode_done(struct venc_wireless_entry *entry, struct encode_done_info *info)
{
	unsigned long flags;
	struct venc_fifo_buffer *queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	queue = entry->encdone_queue;
	if (queue == NULL) {
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return;
	}

	push(queue, info);
	venc_drv_osal_give_event(&queue->event);
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
}

void venc_drv_get_backup_info(struct venc_wireless_entry *entry, struct encode_done_info *info)
{
	unsigned long flags;
	struct venc_fifo_buffer *queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	queue = entry->backup_queue;
	if (queue == NULL || queue_is_empty(queue)) {
		VCODEC_ERR_VENC("backup queue is empty");
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return;
	}

	pop(queue, info);
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
}

void venc_drv_push_backup_info(struct venc_wireless_entry *entry, struct encode_done_info *info)
{
	unsigned long flags;
	struct venc_fifo_buffer *queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	queue = entry->backup_queue;
	if (queue == NULL) {
		VCODEC_ERR_VENC("backup queue is NULL");
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return;
	}

	push(queue, info);
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
}

void venc_drv_push_send_info(struct venc_wireless_entry *entry, struct encode_done_info *info)
{
	unsigned long flags;
	struct venc_fifo_buffer *queue = NULL;

	spin_lock_irqsave(&g_venc_wireless_spinlock, flags);
	queue = entry->send_queue;
	if (queue == NULL) {
		VCODEC_ERR_VENC("send queue is NULL");
		spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
		return;
	}

	push(queue, info);
	spin_unlock_irqrestore(&g_venc_wireless_spinlock, flags);
}
