/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_datan_jt_interface.h"

#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#include "hvgr_log_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_mem_api.h"
#include "hvgr_kmd_defs.h"

void hvgr_datan_jt_update_ring_buffer_write_idx(struct hvgr_jt_ring_buffer_control *ring_buf_control)
{
	struct hvgr_device *gdev = NULL;
	uint16_t new_write_idx;

	gdev = hvgr_get_device();

	if (ring_buf_control == NULL) {
		hvgr_err(gdev, HVGR_DATAN, "ring_buf_control is NULL.");
		return;
	}

	new_write_idx = (ring_buf_control->write_idx + 1) & (JOB_TIMELINE_MAX_BUF_LEN - 1);
	if (new_write_idx == ring_buf_control->read_idx) {
		ring_buf_control->is_full = true;
		return;
	}
	ring_buf_control->write_idx = new_write_idx;
	hvgr_info(gdev, HVGR_DATAN, "update write idx success.");
}

struct hvgr_datan_timeline_entry *hvgr_datan_jt_get_available_entry(
	struct hvgr_jt_ring_buffer_control *ring_buf_control)
{
	struct hvgr_device *gdev = NULL;
	struct hvgr_datan_timeline_entry *entry = NULL;

	gdev = hvgr_get_device();
	if (gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE)
		return NULL;

	if (ring_buf_control->is_full) {
		hvgr_err(gdev, HVGR_DATAN, "time line ring buffer is full.");
		return NULL;
	}

	entry = &ring_buf_control->timeline_entry[ring_buf_control->write_idx];
	return entry;
}

static int hvgr_datan_jt_ring_buffer_init(struct hvgr_device *gdev)
{
	if (gdev == NULL)
		return -1;

	if (gdev->datan_dev.timeline_buf != NULL) {
		hvgr_info(gdev, HVGR_DATAN, "time line is already enable.");
		return 0;
	}

	gdev->datan_dev.timeline_buf = vzalloc(sizeof(struct hvgr_jt_ring_buffer_control));
	if (gdev->datan_dev.timeline_buf == NULL) {
		hvgr_err(gdev, HVGR_DATAN, "time line alloc ring buffer failed.");
		return -1;
	}
	return 0;
}

static void hvgr_datan_jt_ring_buffer_term(struct hvgr_device *gdev)
{
	if (gdev == NULL)
		return;

	if (gdev->datan_dev.timeline_buf == NULL)
		return;

	vfree(gdev->datan_dev.timeline_buf);
	gdev->datan_dev.timeline_buf = NULL;
}

int hvgr_datan_jt_config_status(struct hvgr_ctx *ctx, union kmd_ioctl_timeline_set *timeline_set)
{
	unsigned long flags;

	if (ctx == NULL || ctx->gdev == NULL)
		return -EPERM;

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	if ((timeline_set->in.set_flag != HVGR_DATAN_TIMLINE_ENABLE) &&
		(timeline_set->in.set_flag != HVGR_DATAN_TIMLINE_DISABLE))
		goto exit;

	if (timeline_set->in.set_flag == HVGR_DATAN_TIMLINE_ENABLE) {
		/*
		 * Notes: hvgr_datan_jt_ring_buffer_init -> jt_enable_flag
		 */
		if (hvgr_datan_jt_ring_buffer_init(ctx->gdev) != 0) {
			hvgr_err(ctx->gdev, HVGR_DATAN, "time line ring buffer enable failed.");
			goto exit;
		}
		hvgr_datan_jt_calc_soc_freq(ctx->gdev);
		hvgr_datan_jt_config_entry_irq(ctx->gdev, true);

		spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		ctx->gdev->datan_dev.timeline_buf->is_full = false;
		ctx->gdev->datan_dev.jt_enable_flag = HVGR_DATAN_TIMLINE_ENABLE;
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		hvgr_info(ctx->gdev, HVGR_DATAN, "time line enable success.");
	} else {
		spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		ctx->gdev->datan_dev.jt_enable_flag = HVGR_DATAN_TIMLINE_DISABLE;
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		hvgr_datan_jt_config_entry_irq(ctx->gdev, false);
		/*
		 * Notes: jt_enable_flag -> hvgr_datan_jt_ring_buffer_term
		 */
		hvgr_datan_jt_ring_buffer_term(ctx->gdev);
		hvgr_info(ctx->gdev, HVGR_DATAN, "time line disable success.");
	}
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	hvgr_info(ctx->gdev, HVGR_DATAN, "time line config success.");
	return 0;
exit:
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return -EPERM;
}

void hvgr_datan_jt_probe(struct hvgr_device *gdev)
{
	gdev->datan_dev.jt_enable_flag = HVGR_DATAN_TIMLINE_DISABLE;
	spin_lock_init(&gdev->datan_dev.jt_ring_buffer_lock);
	mutex_init(&gdev->datan_dev.jt_lock);
	hvgr_info(gdev, HVGR_DATAN, "time line probe success.");
}

void hvgr_datan_jt_buffers_free(struct hvgr_ctx * const ctx)
{
	unsigned long flags;
	long ret = -EINVAL;

	if (ctx == NULL || ctx->gdev == NULL)
		return;
	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	/*
	 * When the fd has closed, we need to check ring buffer is free or not.
	 */
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_ENABLE) {
		ctx->gdev->datan_dev.jt_enable_flag = HVGR_DATAN_TIMLINE_DISABLE;
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		hvgr_datan_jt_config_entry_irq(ctx->gdev, false);
		/*
		 * Notes: jt_enable_flag -> hvgr_datan_jt_ring_buffer_term
		 */
		hvgr_datan_jt_ring_buffer_term(ctx->gdev);
	} else {
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	}

	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	ctx->datan_ctx.jt_ctx.jt_chain_enable = HVGR_DATAN_TIMLINE_DISABLE;
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	/*
	 * When the fd has closed, we need to check chain id buffer is free or not.
	 */
	(void)hvgr_datan_jt_chain_buffer_term(ctx);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
}

struct hvgr_mem_area *hvgr_datan_jt_mem_allocate(struct hvgr_ctx *ctx,
	uint64_t property, uint64_t attribute, uint64_t bytes)
{
	struct hvgr_mem_alloc_para para;

	para.property = property;
	para.attribute = attribute;
	para.pages = (uint32_t)hvgr_page_nums(bytes);
	para.max_pages = para.pages;

	return hvgr_mem_allocate(ctx, &para, NULL, false);
}

long hvgr_datan_jt_copy_entries_to_user(struct hvgr_ctx *ctx, union kmd_ioctl_timeline_get *timeline_get)
{
	struct hvgr_jt_ring_buffer_control *timeline_buf = NULL;
	unsigned long flags;
	uint32_t read_idx;
	uint32_t write_idx;
	uint32_t msg_num;
	uint32_t msg_size;
	int ret;
	bool is_full = false;
	uint32_t idx;

	if (ctx == NULL || ctx->gdev == NULL || timeline_get == NULL)
		return -EPERM;

	mutex_lock(&ctx->gdev->datan_dev.jt_lock);
	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	if (ctx->gdev->datan_dev.jt_enable_flag == HVGR_DATAN_TIMLINE_DISABLE) {
		timeline_get->out.msg_num = 0;
		spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
		mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
		return 0;
	}
	timeline_buf = ctx->gdev->datan_dev.timeline_buf;
	read_idx = timeline_buf->read_idx;
	write_idx = timeline_buf->write_idx;
	is_full = timeline_buf->is_full;
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);

	if (is_full)
		msg_num = 1;
	else
		msg_num = 0;

	if (read_idx < write_idx) {
		msg_num += (write_idx - read_idx);
		msg_size = sizeof(struct hvgr_datan_timeline_entry) * msg_num;
		if (timeline_get->in.size < msg_size)
			goto exit;

		ret = copy_to_user(u64_to_user_ptr(timeline_get->in.addr),
			&timeline_buf->timeline_entry[read_idx], msg_size);
		if (ret != 0)
			goto exit;
	} else if (read_idx > write_idx) {
		msg_num += (JOB_TIMELINE_MAX_BUF_LEN - read_idx + write_idx);
		msg_size = sizeof(struct hvgr_datan_timeline_entry) * msg_num;
		if (timeline_get->in.size < msg_size)
			goto exit;

		msg_size = sizeof(struct hvgr_datan_timeline_entry) * (JOB_TIMELINE_MAX_BUF_LEN - read_idx);
		ret = copy_to_user(u64_to_user_ptr(timeline_get->in.addr), &timeline_buf->timeline_entry[read_idx], msg_size);
		if (ret != 0)
			goto exit;

		ret = copy_to_user(u64_to_user_ptr(timeline_get->in.addr + msg_size), &timeline_buf->timeline_entry[0],
			sizeof(struct hvgr_datan_timeline_entry) * (is_full ? (write_idx + 1) : write_idx));
		if (ret != 0)
			goto exit;
	} else {
		timeline_get->out.msg_num = 0;
		mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
		return 0;
	}
	timeline_get->out.msg_num = msg_num;

	spin_lock_irqsave(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	timeline_buf->read_idx = write_idx;
	timeline_buf->is_full = false;
	spin_unlock_irqrestore(&ctx->gdev->datan_dev.jt_ring_buffer_lock, flags);
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	return 0;
exit:
	mutex_unlock(&ctx->gdev->datan_dev.jt_lock);
	hvgr_err(ctx->gdev, HVGR_DATAN, "time line get entry failed.");
	return -EINVAL;
}
