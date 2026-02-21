/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_IOCTL_DATAN_H
#define HVGR_IOCTL_DATAN_H

#include "linux/types.h"
#include "apicmd/hvgr_ioctl.h"
#include "apicmd/hvgr_job_timeline_defs.h"

#define HVGR_DATAN_JOBTRACE_CMD_ENABLE (1u)
#define HVGR_DATAN_JOBTRACE_CMD_DISABLE (2u)

#define HVGR_CT_ALLOC_CMD (3u)

#define HVGR_CT_SINGLE_DUMP_ENABLE_CMD (4u)
#define HVGR_CT_SINGLE_DUMP_DATA_CMD (5u)
#define HVGR_CT_SINGLE_DUMP_CLEAR_CMD (6u)

#define HVGR_IOCTL_CMD_TIMELINE_SUBMIT (7u)
#define HVGR_IOCTL_CMD_TIMELINE_SET (8u)
#define HVGR_IOCTL_CMD_TIMELINE_GET (9u)
#define HVGR_IOCTL_CMD_TIMELINE_GET_CHAIN_ID_BUFFER (10u)

#define HVGR_IOCTL_CMD_AI_FENCE (11u)
#define HVGR_IOCTL_CMD_AI_BUFFER_SIZE (12u)
#define HVGR_IOCTL_CMD_ENABLE_SC (13u)

#define DATAN_PATH_MAX_LEN (256)
#define DATAN_JOBTRACE_MAX_LEN (1<<12)

struct hvgr_datan_jobtrace_para_enable {
	struct {
		char outpath[DATAN_PATH_MAX_LEN];
		int32_t entry_size;
		int32_t softq_id;
	} in;
	struct {
		int64_t hvgr_ctx;
	} out;
};

struct hvgr_datan_jobtrace_para_disable {
	struct {
		int32_t softq_id;
	} in;
};

struct hvgr_ct_queue_user_ctrl { /* only UMD can modify. */
	uint32_t rd; /* Read index */
};

/* This value must match the strcut `hvgr_ct_queue_data_ctrl` */
#define HVGR_CT_QUEUE_DATA_CTRL_HEAD_SIZE 16
struct hvgr_ct_queue_data_ctrl {
	uint64_t timestamp;
	uint64_t event_id;
	char data[1];
} __attribute__((packed));

struct hvgr_ct_queue_ctrl { /* only KMD can modify. */
	/* ct_raw_base point to hvgr_ct_queue_data_ctrl */
	uint64_t ct_raw_base;

	/* user_ctrl_addr point to hvgr_ct_queue_user_ctrl */
	uint64_t user_ctrl_addr;

	uint32_t wr; /* Write index */
};

union hvgr_ioctl_ct_alloc {
	struct {
		uint32_t ring_num; /* Ring Buffer num */
		uint32_t fcm_bm;
		uint32_t gpc_bm;
		uint32_t btc_bm;
		uint32_t mmu_l2_bm;
		uint32_t bvh_bm;
		uint32_t set_id;
	} in;
	struct {
		int fd;
		/* Return hvgr_ct_queue_ctrl address. */
		uint64_t ctrl_addr;
	} out;
};

struct hvgr_ioctl_ct_single_dump_enable {
	uint64_t dump_buffer;
	uint32_t fcm_bm;
	uint32_t gpc_bm;
	uint32_t btc_bm;
	uint32_t mmu_l2_bm;
	uint32_t bvh_bm;
	uint32_t set_id;
};

#define HVGR_IOCTL_CT_ENABLE \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_CT_SINGLE_DUMP_ENABLE_CMD, \
		struct hvgr_ioctl_ct_single_dump_enable)

#define HVGR_IOCTL_CT_DUMP \
	_IO(HVGR_IOCTL_MODULE_DATAN, HVGR_CT_SINGLE_DUMP_DATA_CMD)

#define HVGR_DATAN_JOBTRACE_ENABLE \
	_IOWR(HVGR_IOCTL_MODULE_DATAN, HVGR_DATAN_JOBTRACE_CMD_ENABLE, \
		struct hvgr_datan_jobtrace_para_enable)

#define HVGR_DATAN_JOBTRACE_DISABLE \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_DATAN_JOBTRACE_CMD_DISABLE, \
		struct hvgr_datan_jobtrace_para_disable)

#define HVGR_IOCTL_CT_ALLOC \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_CT_ALLOC_CMD, union hvgr_ioctl_ct_alloc)

#define HVGR_IOCTL_TIMELINE_GET_CHAIN_ID_BUF \
	_IOR(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_TIMELINE_GET_CHAIN_ID_BUFFER, \
		struct kmd_ioctl_timeline_buf)

#define HVGR_IOCTL_TIMELINE_SUBMIT \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_TIMELINE_SUBMIT, \
		struct hvgr_ioctl_timelime_submit)

union kmd_ioctl_timeline_set {
	struct {
		uint8_t set_flag;
	} in;
};
#define HVGR_IOCTL_TIMELINE_SET \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_TIMELINE_SET, union kmd_ioctl_timeline_set)

union kmd_ioctl_timeline_get {
	struct {
		uint32_t size;
		uint64_t addr;
	} in;
	struct {
		uint32_t msg_num;
	} out;
};
#define HVGR_IOCTL_TIMELINE_GET \
	_IOWR(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_TIMELINE_GET, union kmd_ioctl_timeline_get)

struct hvgr_ioctl_ai_fence_info {
	uint64_t fence_info_addr;
};

struct hvgr_ioctl_ai_buffer_info {
	uint64_t buffer_size_addr;
};

struct hvgr_ioctl_enable_sc_info {
	uint64_t enable_sc_addr;
};

#define HVGR_IOCTL_AI_FENCE_GET \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_AI_FENCE, struct hvgr_ioctl_ai_fence_info)

#define HVGR_IOCTL_AI_BUFFER_SIZE_GET \
	_IOW(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_AI_BUFFER_SIZE, struct hvgr_ioctl_ai_buffer_info)

#define HVGR_IOCTL_ENABLE_SC_GET \
	_IOR(HVGR_IOCTL_MODULE_DATAN, HVGR_IOCTL_CMD_ENABLE_SC, struct hvgr_ioctl_enable_sc_info)

#endif

