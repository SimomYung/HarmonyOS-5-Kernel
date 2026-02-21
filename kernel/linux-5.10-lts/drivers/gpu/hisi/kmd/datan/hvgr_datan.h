/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_H
#define HVGR_DATAN_H

#include <linux/version.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>

#include "hvgr_version.h"

#ifdef HVGR_DATAN_JOB_TIMELINE
#include "jobtimeline/hvgr_datan_jt_interface.h"

struct hvgr_jt_ring_buffer_control;
#endif

#include "hvgr_dm_api.h"

enum hvgr_datan_ct_state {
	HVGR_DATAN_CT_STATE_CLOSED = 0,
	HVGR_DATAN_CT_STATE_IDLE,
	HVGR_DATAN_CT_STATE_DUMPING,
	HVGR_DATAN_CT_STATE_FAULT
};

struct hvgr_ctx;
struct hvgr_datan_ct_dev {
	/*
	 * The ct_lock for operation perf counter register
	 */
	spinlock_t ct_lock;
	struct hvgr_ctx *ctx;
	u64 dump_addr;
	bool int_done;
	wait_queue_head_t wait;
	int triggered;
	bool ct_config;
	bool ct_closed;
	enum hvgr_datan_ct_state state;
};

#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
#include "hvgr_regmap_debug_bus.h"

/*
 * @brief debug bus struct
 */
struct hvgr_debugbus_dev {
	/* Debugbus workqueue */
	struct workqueue_struct *gpu_debugbus_wait_wq;
	/* Debugbus work struct */
	struct work_struct gpu_debugbus_wait_work;
	/* Debugbus data space */
	char *debugbus_buf;
	int32_t debugbus_cnt;
	size_t buffer_offset;
	size_t buffer_size;

	struct mutex debug_bus_mutex;
	spinlock_t hwaccess_lock;
	struct hvgr_datan_dev *global_datan_dev;
	/* gpu fault type */
	hvgr_gpu_fault_id gpu_fault_type;
};
#endif

struct hvgr_gpu_fault_msg {
	hvgr_gpu_fault_id gpu_fault_id;
	atomic_t gpu_fault_flag;
	char gpu_fault_log[MAX_DMD_LOG_LENTH];
};

#ifdef CONFIG_HVGR_DFX_DATAN
#define DATAN_PATH_MAX_LEN (256)

#if hvgr_version_ge(HVGR_V350)
struct hvgr_jobtrace_worker {
	struct workqueue_struct *jobtrace_wq;
	struct work_struct cd_worker;
	struct work_struct cmdq_worker;
};
#endif

struct hvgr_soft_q;
struct trace_callback_data {
	struct work_struct trace_worker;
	int len;
	char *buffer;
	struct datan_jobtrace_ctx *djctx;
	struct hvgr_soft_q *qctx;
	struct hvgr_ctx *ctx;
};

struct datan_jobtrace_ctx {
	struct mutex lock;
	/* Enable or Disable JobTrace */
	bool flag;
	char outpath[DATAN_PATH_MAX_LEN];
	int32_t softq_id;
	int32_t entry_size;
	atomic_t counter;
	struct workqueue_struct *trace_callback_wq;
	struct trace_callback_data trace_data;

#if hvgr_version_ge(HVGR_V350)
	struct hvgr_jobtrace_worker worker;
	uint32_t cd_current_id;
	uint32_t cmdq_current_id;
	struct {
		uint32_t first_idx;
		uint32_t current_idx;
	} cmdq_prod_reg_helper;
#endif
};
#endif

struct hvgr_datan_dev {
#ifdef CONFIG_HVGR_DFX_DEBUG_BUS_DUMP
	struct hvgr_debugbus_dev datan_bus_dev;
#endif
	struct hvgr_datan_ct_dev datan_ct_dev;
	struct mutex dump_dir_lock;
#ifdef HVGR_DATAN_JOB_TIMELINE
	uint32_t jt_enable_flag;
	spinlock_t jt_ring_buffer_lock;
	struct mutex jt_lock;
	struct hvgr_jt_ring_buffer_control *timeline_buf;
#endif

	struct dentry *datan_dir;

	struct hvgr_gpu_fault_msg gpu_fault_msg;
#ifdef CONFIG_DFX_DEBUG_FS
	bool mem_dump_enable;
#endif

#if hvgr_version_eq(HVGR_V350)
	atomic_t tool_client_ref;
#endif
};

struct hvgr_datan_jt_ctx {
	uint32_t jt_chain_enable;

	/* Job chain id buffer area */
	struct hvgr_mem_area *chain_id_area;
	/* chain id buffer size */
	uint32_t chain_id_size;

	/* Job chain start timestamp buffer area */
	struct hvgr_mem_area *start_timeline_area;
	/* start time buffer size */
	uint32_t start_timeline_size;

	/* Job chain emd timestamp buffer area */
	struct hvgr_mem_area *end_timeline_area;
	/* end time buffer size */
	uint32_t end_timeline_size;
};

struct hvgr_datan_ctx {
#ifdef CONFIG_HVGR_DFX_DATAN
	struct datan_jobtrace_ctx jobtrace_ctx;
#else
	int pad;
#endif
	struct hvgr_datan_jt_ctx jt_ctx;
};

#endif /* HVGR_DATAN_H END */
