/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __THP_THREAD__
#define __THP_THREAD__

#define THP_CMD_QUEUE_SIZE 80

enum thp_timeout_flag {
	TS_TIMEOUT = 0,
	TS_NOT_TIMEOUT,
	TS_UNDEF = 255,
};

struct thp_cmd_param {
	union {
		struct thp_device *dev;
	} pub_params;
	void *prv_params;
	u32 panel_id; // Specify whitch screen resume or suspend.
	u32 thread_id; // Specify whitch thread to run in.
};

enum thp_cmd {
	TS_INT_PROCESS = 0,
	TS_CHIP_DETECT,
	TS_SUSPEND,
	TS_RESUME,
	THP_MUTIL_RESUME_THREAD,
	THP_MUTIL_SUSPEND_THREAD,
	TSKIT_MUTIL_RESUME_THREAD,
	TSKIT_MUTIL_SUSPEND_THREAD,
	TS_SCREEN_FOLD,
	TS_SCREEN_OFF_FOLD,
	TS_SCREEN_UNFOLD,
	DUAL_THP_PANEL_REVERT_FOLD,
	DUAL_THP_PANEL_REVERT_UNFOLD,
	TS_INVAILD_CMD = 255,
};

struct thp_cmd_sync {
	atomic_t timeout_flag;
	struct completion done;
};

struct thp_cmd_node {
	enum thp_cmd command;
	struct thp_cmd_sync *sync;
	struct thp_cmd_param cmd_param;
};

struct thp_proc_cmd_group {
	enum thp_cmd cmd;
	int (*cmd_func)(struct thp_cmd_node *proc_cmd, u32 panel_id);
};

struct thp_cmd_queue {
	int wr_index;
	int rd_index;
	int cmd_count;
	int queue_size;
	spinlock_t spin_lock;
	struct thp_cmd_node ring_buff[THP_CMD_QUEUE_SIZE];
};
#endif /* __THP_THREAD__ */
