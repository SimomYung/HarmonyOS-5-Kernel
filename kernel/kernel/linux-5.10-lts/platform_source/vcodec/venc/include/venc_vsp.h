/*
 * venc_vsp.h
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

#ifndef __VCODEC_VENC_VSP_H__
#define __VCODEC_VENC_VSP_H__

#include <linux/types.h>

#define MAX_LAYER_NUM (3)
#define MAX_HARDWARE_ADDR_LEN (6)
#define VSP_MAX_CB_LEN (48)
#define RESERVE_LENGTH (40)

struct _tx_layer_ctrl {
	uint64_t next;
	char cb[VSP_MAX_CB_LEN];
	uint8_t mac_ra_address[MAX_HARDWARE_ADDR_LEN];
	uint8_t mac_sa_address[MAX_HARDWARE_ADDR_LEN];
	uint16_t qos_type;
	uint64_t data_addr;
	uint64_t iova_addr;
	uint16_t version;
	char reserve[RESERVE_LENGTH];
	uint16_t layer_number;
	uint16_t last_paket_len;
	uint16_t normal_pack_length;
	uint16_t paket_number;
};

struct _send_result {
	struct _tx_layer_ctrl *slice_layer;
	uint16_t len;
	uint16_t slice_info;
	uint16_t layer_suc_num[MAX_LAYER_NUM];
	uint16_t slice_layer0_max;
	uint16_t slice_layerall_max;
	char *vendor_spec;
};

struct vcodec_venc_callback {
	void (*wifi_tx_pkg_done)(struct _send_result*);
	void (*wifi_tx_send_done)(struct _send_result*);
};

#endif