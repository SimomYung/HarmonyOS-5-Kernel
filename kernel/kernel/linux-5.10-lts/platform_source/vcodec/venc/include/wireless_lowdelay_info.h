/*
 * wireless_lowdelay_info.h
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

#ifndef __WIRLESS_LOWDELY_INFO_H__
#define __WIRLESS_LOWDELY_INFO_H__
#include "vcodec_type.h"

#define WIRELESS_LOWDELAY_MAX_SLICE 4
#define WIRELESS_LOWDELAY_MAX_LAYER 3
#define MAX_HARDWARE_ADDR_LEN (6)
struct layer_pack_info {
	uint32_t vcpi_pack_info; // 0-15:type 16-31:qos
	int32_t shared_fd;
	uint32_t reserve_l;
	uint32_t reserve_m;
	uint32_t reserve_h;
	uint8_t wireless_frame_no;
	uint32_t pack_slice_info_addr;
};

struct enhance_layer_header_info {
	uint32_t slc_hdr_part1;
	uint32_t slc_hdr_size_part1;
	uint32_t slc_hdr_size_part2;
	uint32_t cabac_slc_hdr_part2_seg[8];
	uint32_t cabac_nal_unit_head;
	uint32_t cabac_nal_unit_private_head;
	uint32_t first_slice_segment_in_pic_flag;
	uint32_t slice_segment_address;
	uint32_t valid_data_low_16bit;
	uint32_t valid_data_high_16bit;
};

struct enhance_layer_info {
	uint32_t rec_y_addr;
	uint32_t rec_c_addr;
	uint32_t rec_yh_addr;
	uint32_t rec_ch_addr;
	uint32_t ref_y_addr;
	uint32_t ref_c_addr;
	uint32_t ref_yh_addr;
	uint32_t ref_ch_addr;
	uint32_t stream_addr;
	uint32_t stream_buf_len;
	uint32_t frame_qp;
	struct enhance_layer_header_info enhance_layer_header;
	uint32_t pmv_poc[6];
};

struct slice_timestape {
	uint64_t start_encode_ts_us;
	uint64_t end_encode_ts_us;
	uint64_t start_wifi_send_ts_us;
};

struct stream_header_info {
	int32_t sharefd;
	uint32_t normal_pack_len;
	uint32_t last_pack_len;
	uint32_t pack_num;
};

struct wireless_lowdelay_info {
	int32_t is_wireless_lowdelay_enable;
	int32_t is_send_stream_header_to_wifi;
	int32_t is_eos;
	int32_t pack_enable;
	uint16_t slice_num;
	uint16_t layer_num;
	uint16_t cur_slice_idx;
	uint16_t cur_layer_idx;
	uint32_t width;
	uint32_t front_slice_height;
	uint32_t mac_ral;
	uint32_t mac_sal;
	uint32_t mac_rah_sah;
	struct stream_header_info stream_header;
	struct enhance_layer_info enhance_layer[WIRELESS_LOWDELAY_MAX_LAYER - 1];
	struct layer_pack_info layer_pack[WIRELESS_LOWDELAY_MAX_LAYER];
	struct slice_timestape slice_ts[WIRELESS_LOWDELAY_MAX_SLICE];
};

struct wireless_lowdelay_init_param {
	uint32_t enable_wireless_lowdelay;
};

struct wifi_feedback_info {
	uint32_t channel_id;
	uint16_t frame_idx;
	uint16_t slice_idx;
	uint16_t layer_idx;
	uint16_t is_tx_or_rx;
	uint16_t slice_layer0_max;
	uint16_t slice_layerall_max;
	uint32_t rtt_delay;
	uint32_t send_status;
};
#endif