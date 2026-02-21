/*
 * 
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

#ifndef __DRV_VENC_WIRELESS_H__
#define __DRV_VENC_WIRELESS_H__
#include "venc_vsp.h"
#include "drv_common.h"
#include "wireless_lowdelay_info.h"

#define MSDU_QOS_START_OFFSET 78
#define MSDU_TYPE_START_OFFSET 76

typedef union {
	struct {
		uint16_t type : 1;
		uint16_t layer_idx : 3;
		uint16_t slice_idx : 4;
		uint16_t is_last_package : 1;
		uint16_t max_layer : 3;
		uint16_t version :4;
	} bits;
	uint16_t u16;
} u_qos_layer_info;

int32_t venc_drv_wireless_init(unsigned int cmd, struct venc_file_private *file_private);
void venc_drv_wireless_deinit(void);
void venc_wireless_init_entry(void);
void venc_wireless_deinit_entry(void);
void venc_drv_wireless_send_head(struct encode_info *encode_info);
int32_t venc_drv_wireless_check_and_map(struct encode_info *encode_info);
int32_t venc_drv_wireless_get_feedback_info(struct venc_file_private *file_private,
    struct wifi_feedback_info *feedback_info);
int32_t venc_drv_wireless_check_file_private(struct venc_file_private *file_private);
void venc_drv_wireless_push_sendbuf(struct encode_done_info *info);
void venc_drv_unmap_wireless(int32_t shared_fd);
bool __attribute__((weak)) wifi_tx_venc_pkg(struct _tx_layer_ctrl *ctrl);
void __attribute__((weak)) venc_set_callback(struct vcodec_venc_callback *callback);
uint32_t __attribute__((weak)) hmac_vsp_source_map_dma_buf(struct dma_buf *dmabuf, uintptr_t *iova);
uint32_t __attribute__((weak)) hmac_vsp_source_unmap_dma_buf(struct dma_buf *dmabuf);
#ifdef VENC_DEBUG_ENABLE
void set_venc_wifi_mock(int32_t flag);
#endif
#endif
