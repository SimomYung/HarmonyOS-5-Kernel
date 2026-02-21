/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_datan_jt_interface.h"

#include "hvgr_log_api.h"

static void hvgr_datan_jt_dump_job_boundary(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_chain_boundary *boundary = NULL;

	boundary = &entry->timeline_info.entry_boundary;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Job boundary entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "boundary boundary = %u", boundary->boundary);
	hvgr_info(ctx->gdev, HVGR_DATAN, "boundary chain_type = %u", boundary->chain_type);
	hvgr_info(ctx->gdev, HVGR_DATAN, "boundary entry_id = %u", boundary->entry_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "boundary chain_id = %u", boundary->chain_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "boundary sub_q = %u", boundary->sub_q);
	hvgr_info(ctx->gdev, HVGR_DATAN, "boundary timestamp = %lu", boundary->boundary_timestamp);
}

static void hvgr_datan_jt_dump_phychn_status(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_phychn_state *phychn = NULL;

	phychn = &entry->timeline_info.phychn_state;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Physical status entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "phychn channel id = %u", phychn->channel_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "phychn bind state = %u", phychn->bind_state);
	hvgr_info(ctx->gdev, HVGR_DATAN, "phychn queue id = %u", phychn->queue_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "phychn timestamp = %lu", phychn->phychn_timestamp);
}

static void hvgr_datan_jt_dump_submit_time(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_submit_time *submit = NULL;

	submit = &entry->timeline_info.submit_time;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Submit_t time entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "submit_t id = %lu", submit->submit_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "submit_t begin time = %lu", submit->submit_begin_time);
	hvgr_info(ctx->gdev, HVGR_DATAN, "submit_t end time = %lu", submit->submit_end_time);
}

static void hvgr_datan_jt_dump_cmd_buf_build(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_cmd_buf_build *cmd = NULL;

	cmd = &entry->timeline_info.cmd_buf_build;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Cmd buf build entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "cmd buf id = %u", cmd->cmd_buf_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "cmd buf begin time = %lu", cmd->cmd_buf_begin_time);
	hvgr_info(ctx->gdev, HVGR_DATAN, "cmd buf end time = %lu", cmd->cmd_buf_end_time);
}

static void hvgr_datan_jt_dump_submit_info(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_submit_info *submit = NULL;
	uint32_t i;
	uint32_t j;

	submit = &entry->timeline_info.submit_info;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Submit info entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "submit_i id = %lu", submit->submit_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "submit_i cmd_buf_num = %lu", submit->cmd_buf_num);
	for (i = 0; i < submit->cmd_buf_num; i++) {
		hvgr_info(ctx->gdev, HVGR_DATAN, "  submit_i cmd buf %d info:", i);
		hvgr_info(ctx->gdev, HVGR_DATAN, "  submit_i cmd_buf_id = %u", submit->cmd_buf[i].cmd_buf_id);
		hvgr_info(ctx->gdev, HVGR_DATAN, "  submit_i job_chain_num = %u", submit->cmd_buf[i].job_chain_num);
		for (j = 0; j < submit->cmd_buf[i].job_chain_num; j++)
			hvgr_info(ctx->gdev, HVGR_DATAN, "    submit_i job chain[%d] = %u:", i,
			submit->cmd_buf[i].job_chains[j]);
	}
}

static void hvgr_datan_jt_dump_dep_info(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_dep_info *dep = NULL;
	uint32_t i;

	dep = &entry->timeline_info.dep_info;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Dep info entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "dep src_num = %u", dep->src_num);
	for (i = 0; i < HVGR_DATAN_MAX_DEP_CHAINID_NUM; i++)
		hvgr_info(ctx->gdev, HVGR_DATAN, "  dep src_chain_id[%u]=%u", i, dep->src_chain_id[i]);

	hvgr_info(ctx->gdev, HVGR_DATAN, "dep dst_num = %lu", dep->dst_num);
	for (i = 0; i < HVGR_DATAN_MAX_DEP_CHAINID_NUM; i++)
		hvgr_info(ctx->gdev, HVGR_DATAN, "  dep dst_chain_id[%u]=%u", i, dep->dest_chain_id[i]);
}

static void hvgr_datan_jt_dump_hold_cnt(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	struct hvgr_datan_timeline_hold_cnt *hold_cnt = NULL;

	hold_cnt = &entry->timeline_info.hold_cnt;
	hvgr_info(ctx->gdev, HVGR_DATAN, "Hold cnt entry:");
	hvgr_info(ctx->gdev, HVGR_DATAN, "hold queue_id = %u", hold_cnt->queue_id);
	hvgr_info(ctx->gdev, HVGR_DATAN, "hold counter = %u", hold_cnt->counter);
	hvgr_info(ctx->gdev, HVGR_DATAN, "hold timestamp = %u", hold_cnt->timestamp);
}

void hvgr_datan_jt_debug_dump(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry)
{
	switch (entry->msg_type) {
	case JOB_TIMELINE_SVD:
		hvgr_datan_jt_dump_job_boundary(ctx, entry);
		break;
	case JOB_TIMELINE_PHYCHN_STATE:
		hvgr_datan_jt_dump_phychn_status(ctx, entry);
		break;
	case JOB_TIMELINE_VK_SUBMIT:
		hvgr_datan_jt_dump_submit_time(ctx, entry);
		break;
	case JOB_TIMELINE_CMD_BUF_BUILD:
		hvgr_datan_jt_dump_cmd_buf_build(ctx, entry);
		break;
	case JOB_TIMELINE_SUBMIT_INFO:
		hvgr_datan_jt_dump_submit_info(ctx, entry);
		break;
	case JOB_TIMELINE_DEP_INFO:
		hvgr_datan_jt_dump_dep_info(ctx, entry);
		break;
	case JOB_IIMELINE_WQ_HOLD_CNT:
		hvgr_datan_jt_dump_hold_cnt(ctx, entry);
		break;
	default:
		break;
	};
}
