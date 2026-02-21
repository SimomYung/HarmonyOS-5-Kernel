/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "dpu_config_utils.h"
#include "dpu_comp_smmu.h"
#include "dpu_comp_mgr.h"
#include "gfxdev_mgr.h"
#include "dpu_comp_maintain.h"
#include "ukmd_listener.h"
#include "dpu_comp_config_utils.h"

#define LTPO_90HZ_TE 90
#define MIN_STAT_DURATION_US 2000
#define PPU_MODE_TWO_REGION 1

static uint32_t g_block_bounadry_height[PANEL_PARTITION_NUMS + 1];

static struct frm_rate_gears valid_frm_rate[FRM_RATE_GEARS_NUM] = {{FRM_RATE_1HZ, 1, 1000000},
	{FRM_RATE_5HZ, 5, 200000}, {FRM_RATE_10HZ, 10, 100000}, {FRM_RATE_15HZ, 15, 66667}, {FRM_RATE_20HZ, 20, 50000},
	{FRM_RATE_24HZ, 24, 41667}, {FRM_RATE_30HZ, 30, 33333}, {FRM_RATE_36HZ, 36, 27778},
	{FRM_RATE_45HZ, 45, 22222}, {FRM_RATE_60HZ, 60, 16667}, {FRM_RATE_72HZ, 72, 13889},
	{FRM_RATE_90HZ, 90, 11111}, {FRM_RATE_120HZ, 120, 8333}, {FRM_RATE_144HZ, 144, 6945}};

static uint32_t find_closest_fps(uint64_t val) {
	uint32_t idx;
	uint32_t distans = UINT_MAX;
	uint32_t final_fps_type = 0;

	for (idx = 0; idx < FRM_RATE_GEARS_NUM; idx++) {
		if (abs(val - (uint64_t)valid_frm_rate[idx].period_us) < (uint64_t)distans) {
			distans = (uint32_t)abs(val - (uint64_t)valid_frm_rate[idx].period_us);
			final_fps_type = valid_frm_rate[idx].type;
		}
	}

	return final_fps_type;
}

static void update_safe_refresh_time(uint32_t self_refresh_cnt, struct dpu_comp_maintain *comp_maintain)
{
	if (comp_maintain->multi_block_index == MULTI_BLOCK_NUMS - 1) {
		comp_maintain->safe_refresh_timestamp = comp_maintain->curr_refresh_timestamp_hw;
	} else {
		comp_maintain->safe_refresh_timestamp += (ktime_t)(PERIOD_US_TO_NS * self_refresh_cnt *
		    comp_maintain->pre_self_refresh_period_us);
	}

	comp_maintain->pre_self_refresh_period_us = comp_maintain->self_refresh_period_us;
}

static void refresh_stat_count_ddic_dimming(uint64_t *delta_time_us, struct dpu_comp_maintain *comp_maintain)
{
	uint32_t dim_idx;
	uint32_t repeat_idx;
	uint32_t dim_seq_num;
	struct dimming_node curr_dimming_node;
	uint32_t curr_dimming_fps_type;
	uint32_t last_dimming_fps_type;
	struct dfr_info *dfr_info = dkmd_get_dfr_info(comp_maintain->dpu_comp->conn_info);

	if (!dfr_info) {
		dpu_pr_err("dfr_info is null\n");
		return;
	}

	if (dfr_info->oled_info.ltpo_info.dimming_mode != DIMMING_MODE_DDIC)
		return;

	last_dimming_fps_type = find_closest_fps((uint64_t)comp_maintain->self_refresh_period_us);

	dim_seq_num = dfr_info->oled_info.ltpo_info.dimming_sequence[DIMMING_NORMAL].dimming_seq_num;
	for (dim_idx = 0; dim_idx < dim_seq_num; dim_idx++) {
		curr_dimming_node = dfr_info->oled_info.ltpo_info.dimming_sequence[DIMMING_NORMAL].dimming_seq_list[dim_idx];

		if (curr_dimming_node.frm_rate == valid_frm_rate[last_dimming_fps_type].frm_rate)
			break;

		for (repeat_idx = 0; repeat_idx < curr_dimming_node.repeat_num; repeat_idx++) {
			curr_dimming_fps_type = find_closest_fps((uint64_t)(PERIOD_US_1HZ / curr_dimming_node.frm_rate));
			if (*delta_time_us > valid_frm_rate[curr_dimming_fps_type].period_us) {
				*delta_time_us = *delta_time_us - valid_frm_rate[curr_dimming_fps_type].period_us;
				comp_maintain->refresh_stat_info.refresh_cnt[curr_dimming_fps_type]++;
				comp_maintain->refresh_stat_info.block_refresh_cnt[comp_maintain->multi_block_index]++;
				dpu_pr_debug("dimming fps = %u Hz", valid_frm_rate[curr_dimming_fps_type].frm_rate);
			}
		}
	}

	return;
}

static void refresh_stat_count_safe_refresh(uint64_t *delta_time_us, struct dpu_comp_maintain *comp_maintain)
{
	uint32_t final_fps_type = 0;
	uint32_t self_refresh_cnt = 0;
	uint64_t self_refresh_duration_us = 0;

	if (unlikely(comp_maintain->safe_refresh_timestamp > comp_maintain->curr_refresh_timestamp_hw)) {
		dpu_pr_warn("Invaild self refresh timestamp = %d", ktime_to_us(comp_maintain->safe_refresh_timestamp));
		return;
	}

	self_refresh_duration_us = (uint64_t)(ktime_to_us(comp_maintain->curr_refresh_timestamp_hw) -
	    ktime_to_us(comp_maintain->safe_refresh_timestamp));
	self_refresh_cnt = (uint32_t)(self_refresh_duration_us / (uint64_t)comp_maintain->pre_self_refresh_period_us);

	if (self_refresh_cnt > 0) {
		final_fps_type = find_closest_fps((uint64_t)comp_maintain->pre_self_refresh_period_us);
		comp_maintain->refresh_stat_info.refresh_cnt[final_fps_type] += self_refresh_cnt;
		comp_maintain->refresh_stat_info.block_refresh_cnt[MULTI_BLOCK_NUMS - 1] += self_refresh_cnt;
		comp_maintain->real_frame_rate = (uint32_t)(PERIOD_US_1HZ / comp_maintain->pre_self_refresh_period_us);
		*delta_time_us = self_refresh_duration_us - self_refresh_cnt * comp_maintain->pre_self_refresh_period_us;
		dpu_pr_debug("self refresh period = %u us, self refresh cnt = %u",
			comp_maintain->pre_self_refresh_period_us, self_refresh_cnt);
	}

	update_safe_refresh_time(self_refresh_cnt, comp_maintain);
}

static void refresh_stat_count_platform_refresh(uint64_t delta_time_us, struct dpu_comp_maintain *comp_maintain)
{
    uint32_t final_fps_type;

	if (delta_time_us > MIN_STAT_DURATION_US) {
		final_fps_type = find_closest_fps(delta_time_us);
		comp_maintain->refresh_stat_info.refresh_cnt[final_fps_type]++;
		comp_maintain->refresh_stat_info.block_refresh_cnt[comp_maintain->multi_block_index]++;
		comp_maintain->real_frame_rate = (uint32_t)(PERIOD_US_1HZ / delta_time_us);
		dpu_pr_debug("approximate fps = %u Hz, final fps = %u Hz", comp_maintain->real_frame_rate,
			valid_frm_rate[final_fps_type].frm_rate);
	}
}

static int32_t refresh_stat_count(uint64_t delta_time_us, struct dpu_comp_maintain *comp_maintain)
{
	/* self refresh rate is 90Hz when in 90Hz TE case */
	if (comp_maintain->cur_te_rate == LTPO_90HZ_TE)
		comp_maintain->pre_self_refresh_period_us = PERIOD_US_1HZ / comp_maintain->cur_te_rate;

	if (comp_maintain->pre_self_refresh_period_us == 0) {
		dpu_pr_err("self refresh period == 0");
		return -1;
	}

	/* handle the self-refresh situation */
	refresh_stat_count_ddic_dimming(&delta_time_us, comp_maintain);
	refresh_stat_count_safe_refresh(&delta_time_us, comp_maintain);

	/* handle the non self-refresh situation */
	refresh_stat_count_platform_refresh(delta_time_us, comp_maintain);

	return 0;
}

static void stat_clear_case_process(struct dpu_comp_maintain *comp_maintain)
{
	int32_t clear_flag;

	// timeout clear
	if (comp_maintain->refresh_stat_info.total_count_time_us > MAX_RECODE_DURATION_US) {
		dpu_pr_info("total_count_time_us > MAX_RECODE_DURATION_US, total_count_time_us = %llu us",
			comp_maintain->refresh_stat_info.total_count_time_us);

		comp_maintain->refresh_stat_info.total_count_time_us = 0;
		memset_s(&comp_maintain->refresh_stat_info, sizeof(comp_maintain->refresh_stat_info),
			0, sizeof(comp_maintain->refresh_stat_info));
		return;
	}

	// clear flag clear
	clear_flag = pipeline_next_ops_handle(comp_maintain->dpu_comp->conn_info->conn_device,
		comp_maintain->dpu_comp->conn_info, GET_STATISTIC_CLEAR_FLAG, &comp_maintain->refresh_stat_info);
	if (clear_flag) {
		comp_maintain->refresh_stat_info.total_count_time_us = 0;
		memset_s(&comp_maintain->refresh_stat_info, sizeof(comp_maintain->refresh_stat_info),
			0, sizeof(comp_maintain->refresh_stat_info));

		dpu_pr_info("refresh_stat_info clear");
		return;
	}

	return;
}

static void find_cloest_lower_bound(uint32_t *lower_bound_index, uint32_t rect_side)
{
	uint32_t middle = 0;
	uint32_t upper_bound = *lower_bound_index - 1;
	uint32_t lower_bound = PANEL_PARTITION_NUMS;

	while (lower_bound - upper_bound > 1) {
		middle = (upper_bound + lower_bound) / 2;
		if (g_block_bounadry_height[middle] < rect_side) {
			upper_bound = middle;
		} else if (g_block_bounadry_height[middle] > rect_side){
			lower_bound = middle;
		} else {
			*lower_bound_index = middle + 1;
			return;
		}
	}

	*lower_bound_index = upper_bound + 1;
}

static bool cover_block_area_over_half(uint32_t *lower_bound_index, struct dkmd_rect *panel_refresh_rect, uint32_t rect_side)
{
	uint32_t max_upper_bound = 0;
	uint32_t min_lower_bound = 0;
	uint32_t side_to_block_size = 0;
	uint32_t block_upper_bound = 0;
	uint32_t block_lower_bound = 0;
	uint32_t top = (uint32_t)(panel_refresh_rect->y);
    uint32_t bottom = (uint32_t)(panel_refresh_rect->y) + panel_refresh_rect->h - 1;

    find_cloest_lower_bound(lower_bound_index, rect_side);

	block_upper_bound = g_block_bounadry_height[*lower_bound_index - 1];
	block_lower_bound = g_block_bounadry_height[*lower_bound_index];

	max_upper_bound = (block_upper_bound >= top ? block_upper_bound: top);
	min_lower_bound = (block_lower_bound <= bottom ? block_lower_bound : bottom);

	side_to_block_size = block_lower_bound - block_upper_bound;

	/* block is counted only when it is half covered by dirty rect */
	return (min_lower_bound - max_upper_bound) >= (side_to_block_size / 2);
}

static uint32_t find_bottom_to_lbound_index(uint32_t *lower_bound_index, struct dkmd_rect *panel_refresh_rect)
{
    uint32_t bottom = (uint32_t)(panel_refresh_rect->y) + panel_refresh_rect->h - 1;

    if (!cover_block_area_over_half(lower_bound_index, panel_refresh_rect, bottom)) {
        return *lower_bound_index - 1;
    }

	return *lower_bound_index;
}

static uint32_t find_top_to_lbound_index(uint32_t *lower_bound_index, struct dkmd_rect *panel_refresh_rect)
{
    uint32_t top = (uint32_t)(panel_refresh_rect->y);

    if (!cover_block_area_over_half(lower_bound_index, panel_refresh_rect, top)) {
        return *lower_bound_index + 1;
    }

	return *lower_bound_index;
}

static inline uint32_t bound_to_block(uint32_t bound)
{
	return bound - 1;
}

static bool is_non_positive_rect(struct dkmd_rect *panel_refresh_rect)
{
	return ((panel_refresh_rect->x < 0) || (panel_refresh_rect->y < 0));
}

static bool is_zero_rect(struct dkmd_rect *panel_refresh_rect)
{
	return ((panel_refresh_rect->x == 0) && (panel_refresh_rect->y == 0) &&
        (panel_refresh_rect->w == 0) && (panel_refresh_rect->h == 0));
}

static bool is_invaild_dirty_rect(struct dkmd_rect *panel_refresh_rect)
{
	return (is_non_positive_rect(panel_refresh_rect) || is_zero_rect(panel_refresh_rect));
}

static uint32_t find_multiblock_index(uint32_t top_index, uint32_t bottom_index)
{
	uint32_t group_index = 0;
	uint32_t multiblock_start_index = 0;
	uint32_t multiblock_index = 0;

	/* summation formula of arithmetic sequence */
	group_index = bottom_index - top_index;
    multiblock_start_index = (group_index * PANEL_PARTITION_NUMS) - ((group_index * (group_index - 1)) / 2);
	multiblock_index = multiblock_start_index + top_index;

	return multiblock_index;
}

static uint32_t find_dirty_rect_to_block(struct dkmd_rect *panel_refresh_rect)
{
	uint32_t lower_bound_index = 1;
	uint32_t top_bound_index = 0;
	uint32_t top_block_index = 0;
    uint32_t bottom_bound_index = 0;
    uint32_t bottom_block_index = 0;
	uint32_t dirty_multiblock_index = 0;

	if (g_block_bounadry_height[PANEL_PARTITION_NUMS] == 0) {
		return MULTI_BLOCK_NUMS - 1;
	}

	if (is_invaild_dirty_rect(panel_refresh_rect)) {
		dpu_pr_warn("panel_refresh_rect is invalid, full-screen counted by default");
		return MULTI_BLOCK_NUMS - 1;
	}

	top_bound_index = find_top_to_lbound_index(&lower_bound_index, panel_refresh_rect);
    bottom_bound_index = find_bottom_to_lbound_index(&lower_bound_index, panel_refresh_rect);
	if (bottom_bound_index < top_bound_index) {
        --top_bound_index;
        ++bottom_bound_index;
    }

	top_block_index = bound_to_block(top_bound_index);
	bottom_block_index = bound_to_block(bottom_bound_index);
	dirty_multiblock_index = find_multiblock_index(top_block_index, bottom_block_index);
	if (dirty_multiblock_index >= MULTI_BLOCK_NUMS) {
		dirty_multiblock_index = MULTI_BLOCK_NUMS - 1;
		dpu_pr_warn("dirty_multiblock_index is invalid, full-screen counted by default");
	}

    return dirty_multiblock_index;
}

static uint32_t cal_block_average_size(uint32_t res, uint32_t align)
{
	uint32_t slice_nums = 0;
    uint32_t average_size = 0;
    uint32_t tail_size = 0;
    uint32_t head_size = 0;

	if (align == 0) {
		dpu_pr_err("divisor block align is zero!");
		return 0;
	}

    slice_nums = res / align;
    average_size = (slice_nums / (PANEL_PARTITION_NUMS - 1)) * align;
    tail_size = (slice_nums % (PANEL_PARTITION_NUMS - 1)) * align;
    head_size = average_size;

    /* Head block and tail block split size */
    if (tail_size == 0) {
        head_size = head_size / 2;
    }

    g_block_bounadry_height[1] = head_size;
    g_block_bounadry_height[PANEL_PARTITION_NUMS] = res;

    return average_size;
}

static void update_panel_block_size(struct dkmd_connector_info *conn_info)
{
	struct ppu_config_info ppu_cfg_info = {0};
    struct dkmd_object_info *pinfo = &(conn_info->base);

	uint32_t align = 1;
	uint32_t index = 2;
    uint32_t average_size = 0;
	uint32_t res = pinfo->yres;

    dpu_check_and_no_retval((PANEL_PARTITION_NUMS <= 0), err,
	    "PANEL_PARTITION_NUMS = %d is validate!", PANEL_PARTITION_NUMS);

    if ((!is_ppu_support(pinfo)) || (PANEL_PARTITION_NUMS == 1)) {
        return;
    }

	if (conn_info->get_ppu_config_info) {
        conn_info->get_ppu_config_info(conn_info, &ppu_cfg_info);

		if (ppu_cfg_info.h_min > 0) {
		    align = (uint32_t)(ppu_cfg_info.h_min);
		}
	}

    average_size = cal_block_average_size(res, align);
	dpu_pr_info("panel_block_average_size is %d", average_size);

    while (index < PANEL_PARTITION_NUMS) {
        g_block_bounadry_height[index] = g_block_bounadry_height[index - 1] + average_size;
		index++;
    }
}

static void refresh_stat_isr_handle_work(struct kthread_work *work)
{
	uint64_t power_off_duration_us;
	uint64_t refresh_duration_us;
	uint64_t delta_time_us;
	uint32_t multiple_cnt;
	uint32_t te_duration_us;
	struct dpu_comp_maintain *comp_maintain = NULL;
	struct dkmd_connector_info *conn_info = NULL;
	struct composer *comp = NULL;

	comp_maintain = container_of(work, struct dpu_comp_maintain, isr_handle_work);
	dpu_check_and_no_retval(!comp_maintain, err, "comp_maintain is null pointer");
	conn_info = comp_maintain->dpu_comp->conn_info;
	dpu_check_and_no_retval(!conn_info, err, "conn_info is null pointer");
	comp = &comp_maintain->dpu_comp->comp;
	dpu_check_and_no_retval(!comp, err, "comp is null pointer");

	mutex_lock(&comp_maintain->maintain_lock);
	if (unlikely(comp_maintain->first_count_flag)) {
		comp_maintain->pre_refresh_timestamp = comp_maintain->curr_refresh_timestamp;
		comp_maintain->pre_refresh_timestamp_hw = comp_maintain->curr_refresh_timestamp_hw;
		comp_maintain->safe_refresh_timestamp = comp_maintain->curr_refresh_timestamp_hw;
		comp_maintain->pre_self_refresh_period_us = comp_maintain->self_refresh_period_us;
		comp_maintain->first_count_flag = false;
		update_panel_block_size(conn_info);
		mutex_unlock(&comp_maintain->maintain_lock);
		dpu_pr_info("first count case");
		return;
	}
	dpu_pr_debug("ppu refresh_type = %d, ctrl_mode = %d", comp_maintain->ppu_refresh_type, comp_maintain->ppu_ctrl_mode);
	comp_maintain->multi_block_index = find_dirty_rect_to_block(&(comp_maintain->panel_refresh_rect));
	
	refresh_duration_us = (uint64_t)(ktime_to_us(comp_maintain->curr_refresh_timestamp) -
		ktime_to_us(comp_maintain->pre_refresh_timestamp));
	if (comp->power_on_timestamp > comp->power_off_timestamp) {
		if (comp->power_off_timestamp > comp_maintain->pre_refresh_timestamp) {
			power_off_duration_us = (uint64_t)(ktime_to_us(comp->power_on_timestamp) -
				ktime_to_us(comp->power_off_timestamp));
			dpu_pr_debug("power off duration = %llu us", power_off_duration_us);
			delta_time_us = refresh_duration_us - power_off_duration_us;
			comp_maintain->safe_refresh_timestamp = comp_maintain->curr_refresh_timestamp_hw;
		} else if (comp_maintain->pre_refresh_timestamp > comp->power_on_timestamp) {
			delta_time_us = (uint64_t)abs(ktime_to_us(comp_maintain->curr_refresh_timestamp_hw) -
				ktime_to_us(comp_maintain->pre_refresh_timestamp_hw));
		} else {
			power_off_duration_us = (uint64_t)(ktime_to_us(comp->power_on_timestamp) -
				ktime_to_us(comp_maintain->pre_refresh_timestamp));
			dpu_pr_debug("power off duration = %llu us", power_off_duration_us);
			delta_time_us = refresh_duration_us - power_off_duration_us;
			comp_maintain->safe_refresh_timestamp = comp_maintain->curr_refresh_timestamp_hw;
		}
	} else {
		dpu_pr_debug("power_on_timestamp = %lld ms, power_off_timestamp = %lld ms",
			comp->power_on_timestamp, comp->power_off_timestamp);
		delta_time_us = refresh_duration_us;
	}

	dpu_pr_debug("delta time = %llu us", delta_time_us);
	comp_maintain->refresh_stat_info.total_count_time_us += delta_time_us;

	if (comp_maintain->cur_te_rate > 0) {
		te_duration_us = PERIOD_US_1HZ / comp_maintain->cur_te_rate;
		/* multiple_cnt need round off */
		multiple_cnt = (uint32_t)(delta_time_us / (uint64_t)te_duration_us);
		if ((delta_time_us - multiple_cnt * te_duration_us) > te_duration_us / 2)
			multiple_cnt += 1;

		delta_time_us = multiple_cnt * te_duration_us;
	}

	if (refresh_stat_count(delta_time_us, comp_maintain) != 0) {
		mutex_unlock(&comp_maintain->maintain_lock);
		dpu_pr_err("calculation encountered an error");
		return;
	}

	// set refresh count
	pipeline_next_ops_handle(comp_maintain->dpu_comp->conn_info->conn_device,
		comp_maintain->dpu_comp->conn_info, SET_REFRESH_STATISTIC, &comp_maintain->refresh_stat_info);

	stat_clear_case_process(comp_maintain);

	comp_maintain->pre_refresh_timestamp = comp_maintain->curr_refresh_timestamp;
	comp_maintain->pre_refresh_timestamp_hw = comp_maintain->curr_refresh_timestamp_hw;

	mutex_unlock(&comp_maintain->maintain_lock);
}

static void update_panel_refresh_rect(struct dpu_comp_maintain *comp_maintain)
{
	struct panel_refresh_ctrl *displaying_refresh_ctrl = NULL;
	struct dpu_composer *dpu_comp = comp_maintain->dpu_comp;
	struct dkmd_object_info *pinfo = &(dpu_comp->conn_info->base);
	struct dkmd_rect full_refresh_rect = {0, 0, pinfo->xres, pinfo->yres};
    struct comp_online_present *present_data = (struct comp_online_present *)dpu_comp->present_data;
	struct dpu_comp_dfr_ctrl* dfr_ctrl = &(present_data->dfr_ctrl);

	if (unlikely(present_data == NULL)) {
		dpu_pr_err("online_present_data is null!");
		return;
	}

	if (unlikely(dfr_ctrl == NULL)) {
		dpu_pr_err("dfr_ctrl is null!");
		return;
	}

    if (!is_ppu_support(pinfo) || !(dfr_ctrl->mode == DFR_MODE_TE_SKIP_BY_MCU ||
	    dfr_ctrl->mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU)) {
        return;
    }

	comp_maintain->ppu_refresh_type = dpu_get_refresh_type(dpu_comp->comp_mgr->dpu_base);
	displaying_refresh_ctrl = &(present_data->frames[present_data->displaying_idx].in_frame.panel_refresh_ctrl);

    /* count refresh times of three-region in two-region mode */
	if (pinfo->ppu_support == PPU_MODE_TWO_REGION) {
		comp_maintain->panel_refresh_rect = displaying_refresh_ctrl->pre_extend_ctrl.panel_rect;
		comp_maintain->ppu_ctrl_mode = displaying_refresh_ctrl->pre_extend_ctrl.ppu_ctrl_mode;
	} else {
		comp_maintain->panel_refresh_rect = displaying_refresh_ctrl->panel_rect;
		comp_maintain->ppu_ctrl_mode = displaying_refresh_ctrl->ppu_ctrl_mode;
	}

	if ((!(comp_maintain->ppu_ctrl_mode == PPU_CTRL_MODE_MRRI)) &&
		((comp_maintain->ppu_refresh_type == PANEL_REFRESH_TYPE_NORMAL) ||
	    (comp_maintain->ppu_refresh_type == PANEL_REFRESH_TYPE_PPU_FULL))) {
		comp_maintain->panel_refresh_rect = full_refresh_rect;
		comp_maintain->refresh_stat_info.full_dimming_refresh_cnt++;
	}
}

static int32_t refresh_stat_isr_notify(struct notifier_block *self, unsigned long action, void *data)
{
	struct ukmd_listener_data *listener_data = (struct ukmd_listener_data *)data;
	struct dpu_comp_maintain *comp_maintain = (struct dpu_comp_maintain *)(listener_data->data);

	if (!comp_maintain->routine_enabled)
		return 0;

	comp_maintain->curr_refresh_timestamp = ktime_get();
	comp_maintain->curr_refresh_timestamp_hw = get_te_hw_timestamp_us(comp_maintain->dpu_comp->comp_mgr->dpu_base,
		(int32_t)comp_maintain->dpu_comp->conn_info->base.pipe_sw_itfch_idx);

	update_panel_refresh_rect(comp_maintain);

	dpu_pr_debug("timestamp = %llu us, timestamp_hw = %llu us", ktime_to_us(comp_maintain->curr_refresh_timestamp),
		ktime_to_us(comp_maintain->curr_refresh_timestamp_hw));

	kthread_queue_work(&comp_maintain->dpu_comp->handle_worker, &comp_maintain->isr_handle_work);
	return 0;
}

static struct notifier_block refresh_isr_notifier = {
	.notifier_call = refresh_stat_isr_notify,
};

void comp_mntn_refresh_stat_init(struct dpu_comp_maintain *comp_maintain)
{
	struct dkmd_object_info *pinfo = &(comp_maintain->dpu_comp->conn_info->base);

	init_waitqueue_head(&comp_maintain->wait);

	comp_maintain->routine_enabled = false;
	comp_maintain->first_count_flag = true;
	comp_maintain->panel_refresh_rect.x = 0;
	comp_maintain->panel_refresh_rect.y = 0;
	comp_maintain->panel_refresh_rect.w = pinfo->xres;
	comp_maintain->panel_refresh_rect.h = pinfo->yres;

	if (comp_maintain->dpu_comp->comp.index == DEVICE_COMP_PRIMARY_ID)
		comp_maintain->self_refresh_period_us = PRIMARY_SELF_REFRESH_PERIOD_US;

	if (comp_maintain->dpu_comp->comp.index == DEVICE_COMP_BUILTIN_ID)
		comp_maintain->self_refresh_period_us = BUILTIN_SELF_REFRESH_PERIOD_US;

	kthread_init_work(&comp_maintain->isr_handle_work, refresh_stat_isr_handle_work);

	return;
}

void register_comp_mntn_listener(struct dpu_comp_maintain *comp_maintain)
{
	struct ukmd_isr *isr_ctrl = NULL;
	struct dpu_composer *dpu_comp = comp_maintain->dpu_comp;
	struct comp_online_present *present = (struct comp_online_present *)dpu_comp->present_data;
	struct dpu_comp_dfr_ctrl* dfr_ctrl = &present->dfr_ctrl;

	switch (dfr_ctrl->mode) {
	case DFR_MODE_TE_SKIP_BY_MCU:
	case DFR_MODE_LONGH_TE_SKIP_BY_MCU:
		isr_ctrl = &dpu_comp->comp_mgr->mdp_isr_ctrl;
		break;
	case DFR_MODE_TE_SKIP_BY_ACPU:
		isr_ctrl = &dpu_comp->isr_ctrl;
		break;
	default:
		dpu_pr_warn("dfr mode %d, don't need register composer maintain listener", dfr_ctrl->mode);
		return;
	}

	ukmd_isr_register_listener(isr_ctrl, &refresh_isr_notifier, NOTIFY_REFRESH, &present->comp_maintain);
}

void comp_mntn_refresh_stat_deinit(struct ukmd_isr *isr, uint32_t listening_bit)
{
	ukmd_isr_unregister_listener(isr, &refresh_isr_notifier, listening_bit);
}
