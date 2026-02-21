/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_PROTECT_DATA_H
#define HVGR_PROTECT_DATA_H

#include <linux/types.h>

struct hvgr_cq_ctx;

enum protect_flag {
	CQ_PROTECT_MODE_IN       = 0,
	CQ_PROTECT_MODE_OUT      = 1,
	CQ_PROTECT_MODE_INVALID  = 2,
};

enum cq_enter_protected_state {
	HVGR_CQ_ENTER_PROTECTED_CHECK = 0,
	HVGR_CQ_ENTER_PROTECTED_WAIT_IDLE,
	/* Write reg to enter protect mode. */
	HVGR_CQ_ENTER_PROTECTED_CONFIG,
	/* End state: Prepare coherency change. */
	HVGR_CQ_ENTER_PROTECTED_FINISHED,
};

enum cq_exit_protected_state {
	HVGR_CQ_EXIT_PROTECTED_CHECK = 0,
	HVGR_CQ_EXIT_PROTECTED_WAIT_IDLE,
	/* Wait for the L2 to become idle in preparation for the reset. */
	/* Issue the protected reset. */
	HVGR_CQ_EXIT_PROTECTED_RESET,
	/* End state: Wait for the reset to complete. */
	HVGR_CQ_EXIT_PROTECTED_RESET_WAIT,
};

struct cq_protect_mode {
	bool protected_mode;
	bool protected_mode_transition;
	u32 protected_mode_refcount;

	/* record the queue need running in protect. */
	struct hvgr_cq_ctx *running_queue;
	/* When we received two hold_en at the same time, we also need save the second queue. */
	struct hvgr_cq_ctx *bak_running_queue[CQ_CHANNEL_NUM - 1];
	uint32_t get_index;  /* record running queue get_index when into protect mode. */
	uint32_t get_index_bak;  /* record bak running queue get_index when into protect mode. */
	union {
		enum cq_enter_protected_state enter;
		enum cq_exit_protected_state exit;
	} protected_state;
};

struct cq_cmdq_inline_head_protect {
	uint32_t sync : 1;
	uint32_t in_line : 1;
	uint32_t reserve1 : 30;
};

struct cq_cmdq_protect_info {
	struct cq_cmdq_inline_head_protect head;

	uint32_t opcode1 : 1;
	uint32_t opcode2 : 10;
	uint32_t i_mm : 1;
	uint32_t sub_channel_enable : 1;
	uint32_t sub_channel_type : 3;
	uint32_t reserve : 16;

	uint32_t sem_addr_low;
	uint32_t sem_addr_high : 16;
	uint32_t reserve1 : 16;

	uint32_t reserve2[4];
};

struct cq_cmdq_inline_notify_protect {
	struct cq_cmdq_inline_head_protect head;

	uint32_t opcode1 : 1;
	uint32_t opcode2 : 10;
	uint32_t i_mm : 1;
	uint32_t sub_channel_enable : 1;
	uint32_t sub_channel_type : 3;
	uint32_t signal_op : 3;
	uint32_t condition : 13;

	uint32_t reserve0;

	uint32_t reserve1 : 16;
	uint32_t irq_en : 1;
	uint32_t hold_en : 1;
	uint32_t token : 14;

	uint32_t reserve[4];
};

struct base_jcd {
	uint64_t reserve[8];
};

#endif