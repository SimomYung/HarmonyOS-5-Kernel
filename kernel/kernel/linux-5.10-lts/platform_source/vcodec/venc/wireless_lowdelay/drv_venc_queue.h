/*
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

#ifndef __DRV_VENC_QUEUE_H__
#define __DRV_VENC_QUEUE_H__

#include "drv_common.h"
#include "wireless_lowdelay_info.h"

int32_t venc_drv_wireless_create_queue(struct venc_wireless_entry *entry, struct venc_fifo_buffer *encdone_queue);
void venc_drv_wireless_destroy_queue(struct venc_wireless_entry *entry);

struct _tx_layer_ctrl *venc_drv_get_idle_ctrl(struct venc_wireless_entry *entry);
void venc_drv_release_idle_ctrl(struct venc_wireless_entry *entry, struct _tx_layer_ctrl *head);

int32_t wait_queue_filled(struct venc_feedback_queue *queue);
void venc_drv_fill_feedback(struct venc_feedback_queue *queue, struct wifi_feedback_info *info);

void venc_drv_fill_encode_done(struct venc_wireless_entry *entry, struct encode_done_info *info);
void venc_drv_get_backup_info(struct venc_wireless_entry *entry, struct encode_done_info *info);
void venc_drv_push_backup_info(struct venc_wireless_entry *entry, struct encode_done_info *info);
void venc_drv_push_send_info(struct venc_wireless_entry *entry, struct encode_done_info *info);

#endif