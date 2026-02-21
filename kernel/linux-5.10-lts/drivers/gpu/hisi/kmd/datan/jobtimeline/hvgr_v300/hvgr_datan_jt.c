/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_datan_jt_interface.h"

#include <securec.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include "hvgr_ioctl_datan.h"
#include "hvgr_log_api.h"
#include "hvgr_mem_api.h"
#include "hvgr_kmd_defs.h"
#include "hvgr_dm_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_datan_api.h"
#include "hvgr_regmap_cq.h"

static int hvgr_datan_jt_fill_depinfo_entry(struct hvgr_ctx *ctx,
	struct hvgr_datan_timeline_dep_info *dep_info)
{
	struct hvgr_datan_timeline_dep_info *dep_entry = NULL;
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;

	if (dep_info == NULL || ctx == NULL)
		return -EINVAL;

	if ((dep_info->dst_num > HVGR_DATAN_MAX_DEP_CHAINID_NUM + 1) ||
		(dep_info->src_num > HVGR_DATAN_MAX_DEP_CHAINID_NUM + 1)) {
		hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u src num[%u] or dts num[%u]is invalid.",
			ctx->id, dep_info->src_num, dep_info->dst_num);
		return -EINVAL;
	}

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto exit;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_DEP_INFO;
	entry->tgid = ctx->tgid;
	dep_entry = &entry->timeline_info.dep_info;
	(void)memcpy_s(dep_entry, sizeof(*dep_entry), dep_info, sizeof(*dep_info));

	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);
exit:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return 0;
out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return -ENOMEM;
}

static int hvgr_datan_jt_fill_submit_time_entry(struct hvgr_ctx *ctx,
	struct hvgr_datan_timeline_submit_time *submit_time)
{
	struct hvgr_datan_timeline_submit_time *submit_time_entry = NULL;
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;

	if (ctx == NULL || submit_time == NULL)
		return -EINVAL;

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto exit;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_VK_SUBMIT;
	entry->tgid = ctx->tgid;

	submit_time_entry = &entry->timeline_info.submit_time;
	submit_time_entry->submit_id = submit_time->submit_id;
	submit_time_entry->submit_begin_time = submit_time->submit_begin_time;
	submit_time_entry->submit_end_time = submit_time->submit_end_time;
	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);
exit:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return 0;
out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return -ENOMEM;
}

static int hvgr_datan_jt_fill_submit_info_entry(struct hvgr_ctx *ctx,
	struct hvgr_datan_timeline_ctrl_submit_info *submit_info)
{
	struct hvgr_datan_timeline_submit_info *submit_info_entry = NULL;
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	struct hvgr_datan_timeline_cmd_buf *cmd_buf_tmp = NULL;
	uint32_t size;
	unsigned long flags;
	uint32_t i;
	uint32_t chain_num;

	if (submit_info == NULL || ctx == NULL)
		return -EINVAL;

	if (submit_info->cmd_buf_num > HVGR_DATAN_MAX_CMD_BUF_NUM ||
		submit_info->cmd_buf_num == 0 ||
		submit_info->cmd_buf == NULL) {
		hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u cmd buf is null.", ctx->id);
		return -EINVAL;
	}

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	size = sizeof(*cmd_buf_tmp) * submit_info->cmd_buf_num;
	cmd_buf_tmp = kzalloc(size, GFP_KERNEL);
	if (cmd_buf_tmp == NULL) {
		mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
		hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u Alloc cmd buf failed.", ctx->id);
		return -ENOMEM;
	}
	if (copy_from_user(cmd_buf_tmp, submit_info->cmd_buf, size) != 0) {
		kfree(cmd_buf_tmp);
		mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
		hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u Copy cmd_buffer from user failed.", ctx->id);
		return -ENOMEM;
	}

	for (i = 0; i < submit_info->cmd_buf_num; i++) {
		chain_num = cmd_buf_tmp[i].job_chain_num;
		if (chain_num > HVGR_DATAN_MAX_CHAIN_NUM) {
			kfree(cmd_buf_tmp);
			mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
			hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u chain[%d]=[%u] is invalid.", ctx->id, i, chain_num);
			return -EINVAL;
		}
	}
	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto exit;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_SUBMIT_INFO;
	entry->tgid = ctx->tgid;

	submit_info_entry = &entry->timeline_info.submit_info;
	submit_info_entry->submit_id = submit_info->submit_id;
	submit_info_entry->cmd_buf_num = submit_info->cmd_buf_num;
	if (memcpy_s(&submit_info_entry->cmd_buf[0], size, cmd_buf_tmp, size) != EOK)
		hvgr_info(ctx->gdev, HVGR_DATAN, "ctx_%u copy submit cmd buf to entry failed.", ctx->id);

	hvgr_info(ctx->gdev, HVGR_DATAN, "ctx_%u copy submit info to entry success.", ctx->id);
	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);

exit:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	kfree(cmd_buf_tmp);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return 0;

out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	kfree(cmd_buf_tmp);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return -ENOMEM;
}

static int hvgr_datan_jt_fill_cmd_buf_build_entry(struct hvgr_ctx *ctx,
	struct hvgr_datan_timeline_cmd_buf_build *cmd_buf_build)
{
	struct hvgr_datan_timeline_cmd_buf_build *cmd_buf_build_entry = NULL;
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;

	if (ctx == NULL)
		return -EINVAL;

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto exit;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_CMD_BUF_BUILD;
	entry->tgid = ctx->tgid;

	cmd_buf_build_entry = &entry->timeline_info.cmd_buf_build;
	cmd_buf_build_entry->cmd_buf_id = cmd_buf_build->cmd_buf_id;
	cmd_buf_build_entry->cmd_buf_begin_time = cmd_buf_build->cmd_buf_begin_time;
	cmd_buf_build_entry->cmd_buf_end_time = cmd_buf_build->cmd_buf_end_time;
	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);

exit:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return 0;

out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return -ENOMEM;
}

void hvgr_datan_jt_record_phychn_status(struct hvgr_ctx *ctx, uint32_t channel_id,
	uint16_t bind_state, uint32_t queue_id)
{
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_datan_timeline_phychn_state *phychn_entry = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;

	if (ctx == NULL)
		return;

	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto out;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_PHYCHN_STATE;
	entry->tgid = ctx->tgid;

	phychn_entry = &entry->timeline_info.phychn_state;
	phychn_entry->channel_id = channel_id;
	phychn_entry->bind_state = bind_state;
	phychn_entry->queue_id = queue_id;
	phychn_entry->phychn_timestamp = (uint64_t)ktime_to_ns(ktime_get());
	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);
out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
}

uint32_t hvgr_datan_jt_parse_svd_token(uint32_t svd_token)
{
	return HVGR_TIMELINE_JOB_CHAIN_COUNT;
}

void hvgr_datan_jt_record_entry_boundary(struct hvgr_ctx *ctx, uint16_t type, uint32_t chain_type,
	uint32_t token, uint32_t queue_id)
{
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_datan_timeline_chain_boundary *boundary_entry = NULL;
	uint32_t *chain_id_buf = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;
	uint32_t buf_valuel;

	if (ctx == NULL)
		return;

	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	hvgr_info(ctx->gdev, HVGR_DATAN, "%s ctx_%u token=%u", __func__, ctx->id, token);
	token &= (CQ_SYNC_TOKEN_NUMS - 1);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE ||
		ctx->datan_ctx.jt_ctx.jt_chain_enable == HVGR_DATAN_TIMLINE_DISABLE)
		goto out;

	chain_id_buf = u64_to_ptr(ctx->datan_ctx.jt_ctx.chain_id_area->kva);
	if (chain_id_buf != NULL && chain_id_buf[token] == HVGR_INVALID_CHAIN_ID) {
		hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u token is invalid", ctx->id);
		goto out;
	}
	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_SVD;
	entry->tgid = ctx->tgid;

	boundary_entry = &entry->timeline_info.entry_boundary;
	boundary_entry->boundary = type;
	boundary_entry->chain_type = chain_type;
	boundary_entry->entry_id = token;
	boundary_entry->boundary_timestamp = (uint64_t)ktime_to_ns(ktime_get());
	if (chain_id_buf != NULL) {
		buf_valuel = chain_id_buf[token];
		boundary_entry->chain_id = hvgr_datan_jt_get_chain_id(buf_valuel);
		boundary_entry->sub_q = hvgr_datan_jt_get_sub_q(buf_valuel);
	} else {
		boundary_entry->chain_id = 0;
		boundary_entry->sub_q = 0;
	}
	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);
out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
}

void hvgr_datan_jt_record_hold_cnt(struct hvgr_ctx *ctx, uint32_t cnt, uint32_t queue_id)
{
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_datan_timeline_hold_cnt *hold_cnt = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;

	if (ctx == NULL)
		return;

	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto out;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_IIMELINE_WQ_HOLD_CNT;
	entry->tgid = ctx->tgid;

	hold_cnt = &entry->timeline_info.hold_cnt;
	hold_cnt->queue_id = queue_id;
	hold_cnt->counter = cnt;
	hold_cnt->timestamp = (uint64_t)ktime_to_ns(ktime_get());

	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);
out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
}

void hvgr_datan_jt_record_fence(struct hvgr_ctx *ctx, uint32_t token)
{
	struct hvgr_datan_timeline_entry *entry = NULL;
	struct hvgr_datan_timeline_fence *fence_entry = NULL;
	struct hvgr_jt_ring_buffer_control *ring_buffer_control = NULL;
	unsigned long flags;

	if (ctx == NULL)
		return;

	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		goto out;

	ring_buffer_control = ctx->gdev->datan_dev.timeline_buf;
	entry = hvgr_datan_jt_get_available_entry(ring_buffer_control);
	if (entry == NULL)
		goto out;

	entry->msg_type = JOB_TIMELINE_FENCE;
	entry->tgid = ctx->tgid;

	fence_entry = &entry->timeline_info.fence;
	fence_entry->token_id = token;
	fence_entry->wait_fence_timestamp = (uint64_t)ktime_to_ns(ktime_get());
	hvgr_datan_jt_update_ring_buffer_write_idx(ring_buffer_control);
	hvgr_datan_jt_debug_dump(ctx, entry);
out:
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
}

void hvgr_datan_jt_config_entry_irq(struct hvgr_device * const gdev, bool enable)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	(void)hvgr_pm_request_gpu(gdev, false);

	if (enable)
		gcqctx->hw_cfg.cq_entry_mode |= CQ_ENTRY_BOUNDARY_EN;
	else
		gcqctx->hw_cfg.cq_entry_mode &= ~CQ_ENTRY_BOUNDARY_EN;
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_DFX_CONFIG), gcqctx->hw_cfg.cq_entry_mode);

	hvgr_pm_release_gpu(gdev);
}

static int hvgr_datan_jt_record_up_info(struct hvgr_ctx *ctx, struct hvgr_ioctl_timelime_submit *submit)
{
	int ret = 0;

	if (ctx == NULL || ctx->gdev == NULL || submit == NULL)
		return -EPERM;

	switch (submit->msg_type) {
	case JOB_TIMELINE_DEP_INFO:
		ret = hvgr_datan_jt_fill_depinfo_entry(ctx, &submit->timeline_msg.dep_info);
		break;
	case JOB_TIMELINE_VK_SUBMIT:
		ret = hvgr_datan_jt_fill_submit_time_entry(ctx, &submit->timeline_msg.submit_time);
		break;
	case JOB_TIMELINE_CMD_BUF_BUILD:
		ret = hvgr_datan_jt_fill_cmd_buf_build_entry(ctx, &submit->timeline_msg.cmd_buf_build);
		break;
	case JOB_TIMELINE_SUBMIT_INFO:
		ret = hvgr_datan_jt_fill_submit_info_entry(ctx, &submit->timeline_msg.submit_info);
		break;
	default:
		hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u Msg type is invalid.", ctx->id);
		return -EPERM;
	}
	return ret;
}

/*
 * @brief in order to keep code simple, some versions with the func body empty.
 */
void hvgr_datan_jt_calc_soc_freq(struct hvgr_device *gdev) {}

long hvgr_datan_jt_chain_buffer_term(struct hvgr_ctx * const ctx)
{
	if (ctx == NULL)
		return 0;
	if (ctx->datan_ctx.jt_ctx.chain_id_area != NULL) {
		hvgr_mem_free_kernel_area(ctx, ctx->datan_ctx.jt_ctx.chain_id_area);
		ctx->datan_ctx.jt_ctx.chain_id_area = NULL;
	}

	return 0;
}

static long hvgr_datan_jt_chain_buffer_init(struct hvgr_ctx * const ctx)
{
	uint32_t chain_id_size;
	struct hvgr_mem_area *chain_id_area = NULL;

	if (ctx->datan_ctx.jt_ctx.chain_id_area == NULL) {
		chain_id_size = sizeof(uint32_t) * CQ_SYNC_TOKEN_NUMS;
		chain_id_area = hvgr_datan_jt_mem_allocate(ctx, HVGR_MEM_PROPERTY_DRIVER,
			HVGR_MEM_ATTR_CPU_RD | HVGR_MEM_ATTR_CPU_WR | HVGR_MEM_ATTR_KMD_ACCESS, chain_id_size);
		if (chain_id_area == NULL) {
			hvgr_err(ctx->gdev, HVGR_DATAN, "ctx_%u alloc chain id buffer failed.", ctx->id);
			return -1;
		}
		ctx->datan_ctx.jt_ctx.chain_id_area = chain_id_area;
		ctx->datan_ctx.jt_ctx.chain_id_size = chain_id_size;
		hvgr_info(ctx->gdev, HVGR_DATAN, "ctx_%u alloc chain id buffer success.", ctx->id);
	}
	return 0;
}

static long hvgr_datan_jt_get_chain_id_buf(struct hvgr_ctx * const ctx,
	struct kmd_ioctl_timeline_buf *chain_id_buf_para)
{
	long ret = -EINVAL;
	unsigned long flags;

	if (ctx == NULL || chain_id_buf_para == NULL)
		return ret;

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	if (chain_id_buf_para->chain_buf_addr == 0) {
		ret = hvgr_datan_jt_chain_buffer_init(ctx);
		spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		ctx->datan_ctx.jt_ctx.jt_chain_enable = HVGR_DATAN_TIMLINE_ENABLE;
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		if (ret == 0) {
			chain_id_buf_para->chain_buf_addr = ctx->datan_ctx.jt_ctx.chain_id_area->gva;
			chain_id_buf_para->chain_buf_size = ctx->datan_ctx.jt_ctx.chain_id_size;
		}
	} else {
		spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		ctx->datan_ctx.jt_ctx.jt_chain_enable = HVGR_DATAN_TIMLINE_DISABLE;
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		ret = hvgr_datan_jt_chain_buffer_term(ctx);
	}
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return ret;
}

long hvgr_datan_jt_ioctl(struct hvgr_ctx * const ctx, unsigned int cmd, unsigned long arg)
{
	long ret = -EINVAL;

	switch (cmd) {
	case HVGR_IOCTL_TIMELINE_SUBMIT:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_TIMELINE_SUBMIT,
			hvgr_datan_jt_record_up_info, arg, struct hvgr_ioctl_timelime_submit);
		break;
	case HVGR_IOCTL_TIMELINE_SET:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_TIMELINE_SET,
			hvgr_datan_jt_config_status, arg, union kmd_ioctl_timeline_set);
		break;
	case HVGR_IOCTL_TIMELINE_GET:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_TIMELINE_GET,
			hvgr_datan_jt_copy_entries_to_user, arg, union kmd_ioctl_timeline_get);
		break;
	case HVGR_IOCTL_TIMELINE_GET_CHAIN_ID_BUF:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_TIMELINE_GET_CHAIN_ID_BUF,
			hvgr_datan_jt_get_chain_id_buf, arg, struct kmd_ioctl_timeline_buf);
		break;
	default:
		hvgr_err(ctx->gdev, HVGR_DATAN, "jt ctx_%u ioctl cmd 0x%x is INVALID", ctx->id, cmd);
		break;
	}
exit:
	return ret;
}
