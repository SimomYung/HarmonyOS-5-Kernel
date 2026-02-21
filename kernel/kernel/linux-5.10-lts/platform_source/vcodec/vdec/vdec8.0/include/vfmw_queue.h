/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: vfmw queue defination
 */

#ifndef VFMW_QUEUE_H
#define VFMW_QUEUE_H

#include "vcodec_types.h"
#include "vcodec_vdec.h"

#define MAX_EVENT_CACHE_SIZE       (20U * 1024U)
#define MAX_OUTPUT_EVENT_SIZE      (2U * 1024U)
#define MAX_EVENT_CNT              (60U)
#define MAX_INPIUT_MSG_SIZE        (256U)
#define MAX_MSG_CNT                (2048U)

typedef struct {
	uint8_t is_seek_pending;
	uint32_t flags;
	uint32_t buf_len;
	uint32_t cfg_width;
	uint32_t cfg_height;
} vfmw_raw_data_ext;

typedef struct {
	uint8_t valid;
	uint8_t is_not_last_packet_flag;
	uint8_t is_stream_end_flag;
	uint8_t is_extern_eop_flag;
	UADDR phy_addr;
	uint32_t length;
	uint32_t index;
	uint32_t discontinue_count;
	uint32_t disp_enable_flag;
	uint32_t disp_frame_distance;
	uint32_t distance_before_first_frame;
	uint32_t gop_num;
	uint64_t vir_addr;
	uint64_t user_tag;
	uint64_t disp_time;
	int64_t pts;
	uint64_t time_stamp;
	vfmw_raw_data_ext vfmw_raw_ext;
	uint8_t dv_dual_layer; /* 0: SL 1:DL */
	int32_t share_fd;
	uint32_t buffer_id;
} vfmw_stream_data;

typedef struct {
	uint32_t chanId;
	vfmw_stream_data data;
} stream_ioctl;

typedef struct {
	uint32_t image_id;
	int32_t share_fd;
} vfmw_release_image_info;

typedef struct {
	uint32_t chan_id;
	vfmw_release_image_info data;
} image_ioctl;

typedef struct {
	uint8_t  upstream_buffer[MAX_EVENT_CACHE_SIZE];
	uint32_t event_count;
} event_output;

typedef struct {
	event_output event_output_buffer;
	struct vdec_chan_list *chan_list;
} event_output_list;

typedef struct {
	int32_t chan_id;
	int32_t type;
	uint32_t param_len;
	volatile bool used;
	uint8_t msg[MAX_OUTPUT_EVENT_SIZE];
} event_node;

typedef struct {
	event_node node_list[MAX_EVENT_CNT]; // 128K / 2 = 64
	uint32_t read;
	uint32_t write;
	uint32_t size;
} event_queue;

typedef struct {
	int32_t cmd;
	uint32_t size;
	uint8_t msg[MAX_INPIUT_MSG_SIZE];
	volatile bool used;
} msg_node;

typedef struct {
	uint32_t read;
	uint32_t write;
	uint32_t size;
	msg_node msgs[MAX_MSG_CNT];
} msg_queue;

#endif