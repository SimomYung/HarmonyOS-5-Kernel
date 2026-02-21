/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_JOB_TIMELINE_DEFS_H
#define HVGR_JOB_TIMELINE_DEFS_H

#include "linux/types.h"

typedef enum {
	JOB_TIMELINE_DEP_INFO = 0,
	JOB_TIMELINE_SPECIAL_NOTIFY,
	JOB_TIMELINE_PHYCHN_STATE,
	JOB_TIMELINE_SVD,
	JOB_TIMELINE_FENCE,
	JOB_TIMELINE_VK_SUBMIT,
	JOB_TIMELINE_CMD_BUF_BUILD,
	JOB_TIMELINE_SUBMIT_INFO,
	JOB_IIMELINE_WQ_HOLD_CNT,
	JOB_TIMELINE_END,
} job_timeline_msg;

#define HVGR_DATAN_MAX_DEP_CHAINID_NUM       8
#define HVGR_DATAN_MAX_CHAIN_NUM             512
#define HVGR_DATAN_MAX_CMD_BUF_NUM           32

struct hvgr_datan_timeline_dep_info {
	uint32_t src_chain_id[HVGR_DATAN_MAX_DEP_CHAINID_NUM];
	uint32_t dest_chain_id[HVGR_DATAN_MAX_DEP_CHAINID_NUM];
	uint32_t src_num;
	uint32_t dst_num;
};

struct hvgr_datan_timeline_submit_time {
	uint32_t submit_id;
	uint64_t submit_begin_time;
	uint64_t submit_end_time;
};

struct hvgr_datan_timeline_cmd_buf {
	uint32_t cmd_buf_id;
	uint32_t job_chain_num;
	uint32_t job_chains[HVGR_DATAN_MAX_CHAIN_NUM];
};

struct hvgr_datan_timeline_submit_info {
	uint32_t submit_id;
	uint32_t cmd_buf_num;
	struct hvgr_datan_timeline_cmd_buf cmd_buf[HVGR_DATAN_MAX_CMD_BUF_NUM];
};

struct hvgr_datan_timeline_ctrl_submit_info {
	uint32_t submit_id;
	uint32_t cmd_buf_num;
	struct hvgr_datan_timeline_cmd_buf *cmd_buf;
};

struct hvgr_datan_timeline_cmd_buf_build {
	uint32_t cmd_buf_id;
	uint64_t cmd_buf_begin_time;
	uint64_t cmd_buf_end_time;
};

struct hvgr_datan_timeline_phychn_state {
	uint16_t channel_id;
	uint16_t bind_state;
	uint32_t queue_id;
	uint64_t phychn_timestamp;
};

struct hvgr_datan_timeline_chain_boundary {
	uint16_t boundary;
	uint16_t chain_type;
	uint32_t entry_id;
	uint32_t chain_id;
	uint32_t sub_q;
	uint64_t boundary_timestamp;
};

struct hvgr_datan_timeline_fence {
	uint32_t token_id;
	uint64_t wait_fence_timestamp;
};

struct hvgr_datan_timeline_hold_cnt {
	uint32_t queue_id;
	uint32_t counter;
	uint64_t timestamp;
};

struct hvgr_ioctl_timelime_submit {
	job_timeline_msg msg_type;
	union {
		struct hvgr_datan_timeline_dep_info dep_info;
		struct hvgr_datan_timeline_submit_time submit_time;
		struct hvgr_datan_timeline_ctrl_submit_info submit_info;
		struct hvgr_datan_timeline_cmd_buf_build cmd_buf_build;
	} timeline_msg;
};

struct hvgr_datan_timeline_entry {
	uint32_t msg_type;
	uint64_t tgid;
	union {
		struct hvgr_datan_timeline_dep_info dep_info;
		struct hvgr_datan_timeline_submit_time submit_time;
		struct hvgr_datan_timeline_submit_info submit_info;
		struct hvgr_datan_timeline_cmd_buf_build cmd_buf_build;
		struct hvgr_datan_timeline_phychn_state phychn_state;
		struct hvgr_datan_timeline_chain_boundary entry_boundary;
		struct hvgr_datan_timeline_fence fence;
		struct hvgr_datan_timeline_hold_cnt hold_cnt;
	} timeline_info;
};

struct kmd_ioctl_timeline_buf {
	uint32_t chain_buf_size;
	uint64_t chain_buf_addr;
};

#endif
