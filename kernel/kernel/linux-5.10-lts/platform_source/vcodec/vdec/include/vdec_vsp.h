/*
 * vdec_vsp.h
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

#ifndef __VCODEC_VDEC_VSP_H__
#define __VCODEC_VDEC_VSP_H__
#include <linux/types.h>
#include <linux/dma-buf.h>

#define MAX_LAYER_NUM (3)
#define MAX_PAKET_NUM (2000)

struct _rx_slice_mgmt {
	uint32_t fix_len;
	uint32_t payload_offset;
	void *priv[MAX_LAYER_NUM];
	uint32_t last_len[MAX_LAYER_NUM];
	uint32_t packet_num[MAX_LAYER_NUM];
	uint64_t base_addr[MAX_LAYER_NUM][MAX_PAKET_NUM];
};

struct vcodec_vdec_callback {
	struct _rx_slice_mgmt *(*alloc_vdec_slice_buffer)(uint32_t);
	void (*wifi_rx_slice_done)(struct _rx_slice_mgmt*);
};
#endif