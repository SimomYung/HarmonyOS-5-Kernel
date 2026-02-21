/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
#include "partial_update_bigdata.h"
#include <linux/sort.h>
#include "dksm_utils.h"

#define AREA_BUF_SIZE 300
#define FORBID_BUF_SIZE 400
#define MAX_DIV_VALUE 8
#define DIV_NUM 2
#define DIV_BASE_NUM 2
#define SUPPORT_DIRTY_MAX_NUM 5

static struct partial_update_bigdata g_bigdata[PANEL_ID_MAX_NUM];
static struct partial_update_multi_dirty_bigdata g_multi_dirty_bigdata[PANEL_ID_MAX_NUM];
static struct partial_update_bigdata_work g_multi_dirty_work[PANEL_ID_MAX_NUM];

static void clear_bigdata(struct partial_update_bigdata *bigdata)
{
	bigdata->frame_count = 0;
	memset_s(&bigdata->dirty_rect, sizeof(bigdata->dirty_rect), 0, sizeof(bigdata->dirty_rect));
	memset_s(bigdata->area_count, sizeof(bigdata->area_count), 0, sizeof(bigdata->area_count));
	memset_s(bigdata->forbid_reasons, sizeof(bigdata->forbid_reasons), 0, sizeof(bigdata->forbid_reasons));
}

static ssize_t get_bigdata(char *buf, int panel_id)
{
	ssize_t ret = 0;
	uint64_t frame_count = 0;
	uint32_t area_count[AREA_INTERVAL_SIZE];
	uint32_t forbid_count[FORBID_PARTIAL_REASON_SIZE];
	char area_buf[AREA_BUF_SIZE];
	char forbid_buf[FORBID_BUF_SIZE];

	if (!buf)
		return 0;

	if (panel_id >= PANEL_ID_MAX_NUM)
		return 0;

	mutex_lock(&g_bigdata[panel_id].lock);
	frame_count = g_bigdata[panel_id].frame_count;
	memcpy_s(area_count, sizeof(area_count), g_bigdata[panel_id].area_count, sizeof(area_count));
	memcpy_s(forbid_count, sizeof(forbid_count), g_bigdata[panel_id].forbid_reasons, sizeof(forbid_count));
	clear_bigdata(&g_bigdata[panel_id]);
	mutex_unlock(&g_bigdata[panel_id].lock);

	ret = snprintf_s(area_buf,
		AREA_BUF_SIZE,
		AREA_BUF_SIZE - 1,
		"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
		area_count[0], area_count[1], area_count[2],
		area_count[3], area_count[4], area_count[5],
		area_count[6], area_count[7], area_count[8],
		area_count[9], area_count[10], area_count[11],
		area_count[12], area_count[13], area_count[14],
		area_count[15], area_count[16], area_count[17],
		area_count[18], area_count[19], area_count[20],
		area_count[21], area_count[22], area_count[23],
		area_count[24]);
	if (ret == -1)
		dpu_pr_warn("area info fault");

	ret = snprintf_s(forbid_buf,
		FORBID_BUF_SIZE,
		FORBID_BUF_SIZE - 1,
		"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
		forbid_count[0], forbid_count[1], forbid_count[2],
		forbid_count[3], forbid_count[4], forbid_count[5],
		forbid_count[6], forbid_count[7], forbid_count[8],
		forbid_count[9], forbid_count[10], forbid_count[11],
		forbid_count[12], forbid_count[13], forbid_count[14],
		forbid_count[15], forbid_count[16], forbid_count[17],
		forbid_count[18], forbid_count[19], forbid_count[20],
		forbid_count[21], forbid_count[22], forbid_count[23],
		forbid_count[24], forbid_count[25], forbid_count[26],
		forbid_count[27], forbid_count[28], forbid_count[29],
		forbid_count[30], forbid_count[31]);
	if (ret == -1)
		dpu_pr_warn("forbid info fault");

	ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d|%s|%s\n", frame_count, area_buf, forbid_buf);
	return ret;
}

static void clear_multi_dirty_bigdata(struct partial_update_multi_dirty_bigdata *bigdata)
{
	bigdata->frame_count = 0;
	memset_s(bigdata->area_count, sizeof(bigdata->area_count), 0, sizeof(bigdata->area_count));
	memset_s(bigdata->area_buf, sizeof(bigdata->area_buf), 0, sizeof(bigdata->area_buf));
}

static ssize_t get_multi_dirty_bigdata(char *buf, int32_t panel_id)
{
	int32_t i = 0;
	ssize_t ret = 0;

	if (!buf)
		return 0;

	if (panel_id >= PANEL_ID_MAX_NUM)
		return 0;

	mutex_lock(&g_multi_dirty_bigdata[panel_id].lock);

	for (i = 0; i < MULTI_DIRTY_COMBINATION_SIZE; i++) {
		ret = snprintf_s(g_multi_dirty_bigdata[panel_id].area_buf[i],
			MULTI_DIRTY_AREA_BUF_SIZE,
			MULTI_DIRTY_AREA_BUF_SIZE - 1,
			"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
			g_multi_dirty_bigdata[panel_id].area_count[i][0],
			g_multi_dirty_bigdata[panel_id].area_count[i][1],
			g_multi_dirty_bigdata[panel_id].area_count[i][2],
			g_multi_dirty_bigdata[panel_id].area_count[i][3],
			g_multi_dirty_bigdata[panel_id].area_count[i][4],
			g_multi_dirty_bigdata[panel_id].area_count[i][5],
			g_multi_dirty_bigdata[panel_id].area_count[i][6],
			g_multi_dirty_bigdata[panel_id].area_count[i][7],
			g_multi_dirty_bigdata[panel_id].area_count[i][8],
			g_multi_dirty_bigdata[panel_id].area_count[i][9],
			g_multi_dirty_bigdata[panel_id].area_count[i][10],
			g_multi_dirty_bigdata[panel_id].area_count[i][11],
			g_multi_dirty_bigdata[panel_id].area_count[i][12],
			g_multi_dirty_bigdata[panel_id].area_count[i][13],
			g_multi_dirty_bigdata[panel_id].area_count[i][14],
			g_multi_dirty_bigdata[panel_id].area_count[i][15],
			g_multi_dirty_bigdata[panel_id].area_count[i][16],
			g_multi_dirty_bigdata[panel_id].area_count[i][17],
			g_multi_dirty_bigdata[panel_id].area_count[i][18],
			g_multi_dirty_bigdata[panel_id].area_count[i][19],
			g_multi_dirty_bigdata[panel_id].area_count[i][20],
			g_multi_dirty_bigdata[panel_id].area_count[i][21],
			g_multi_dirty_bigdata[panel_id].area_count[i][22],
			g_multi_dirty_bigdata[panel_id].area_count[i][23],
			g_multi_dirty_bigdata[panel_id].area_count[i][24],
			g_multi_dirty_bigdata[panel_id].frame_count);
		if (ret == -1)
			dpu_pr_warn("area info fault");
		ret = snprintf_s(buf,
			PAGE_SIZE,
			PAGE_SIZE - 1,
			"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
			g_multi_dirty_bigdata[panel_id].area_buf[0],
			g_multi_dirty_bigdata[panel_id].area_buf[1],
			g_multi_dirty_bigdata[panel_id].area_buf[2],
			g_multi_dirty_bigdata[panel_id].area_buf[3],
			g_multi_dirty_bigdata[panel_id].area_buf[4],
			g_multi_dirty_bigdata[panel_id].area_buf[5],
			g_multi_dirty_bigdata[panel_id].area_buf[6],
			g_multi_dirty_bigdata[panel_id].area_buf[7],
			g_multi_dirty_bigdata[panel_id].area_buf[8],
			g_multi_dirty_bigdata[panel_id].area_buf[9],
			g_multi_dirty_bigdata[panel_id].area_buf[10],
			g_multi_dirty_bigdata[panel_id].area_buf[11],
			g_multi_dirty_bigdata[panel_id].area_buf[12],
			g_multi_dirty_bigdata[panel_id].area_buf[13],
			g_multi_dirty_bigdata[panel_id].area_buf[14]);
	}
	clear_multi_dirty_bigdata(&g_multi_dirty_bigdata[panel_id]);
	mutex_unlock(&g_multi_dirty_bigdata[panel_id].lock);
	return ret;
}

static ssize_t partial_update_bigdata_gfx_primary_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return get_bigdata(buf, PANEL_ID_PRIMARY);
}

static ssize_t partial_update_bigdata_gfx_builtin_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return get_bigdata(buf, PANEL_ID_BUILTIN);
}

static ssize_t multi_dirty_bigdata_gfx_primary_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return get_multi_dirty_bigdata(buf, PANEL_ID_PRIMARY);
}

static ssize_t multi_dirty_bigdata_gfx_builtin_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return get_multi_dirty_bigdata(buf, PANEL_ID_BUILTIN);
}

static DEVICE_ATTR(partial_update_bigdata_gfx_primary, 0440, partial_update_bigdata_gfx_primary_show, NULL);
static DEVICE_ATTR(partial_update_bigdata_gfx_builtin, 0440, partial_update_bigdata_gfx_builtin_show, NULL);
static DEVICE_ATTR(multi_dirty_bigdata_gfx_primary, 0440, multi_dirty_bigdata_gfx_primary_show, NULL);
static DEVICE_ATTR(multi_dirty_bigdata_gfx_builtin, 0440, multi_dirty_bigdata_gfx_builtin_show, NULL);

void partial_update_bigdata_setup(struct dpu_composer *dpu_comp)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct ukmd_attr *attrs = &dpu_comp->attrs;
	dpu_check_and_no_retval(!attrs, err, "attrs is null pointer");

	if (pinfo->dirty_region_updt_support == 0) {
		dpu_pr_info("panel do not dirty_region_updt_support, do not set bigdata node");
		return;
	}

	if (unlikely(pinfo->base.id >= PANEL_ID_MAX_NUM)) {
		dpu_pr_info("setup fail , panel_id out of bounds");
		return;
	}
	dpu_pr_info("setup bigdata node");

	g_bigdata[pinfo->base.id].frame_count = 0;
	mutex_init(&g_bigdata[pinfo->base.id].lock);
	g_multi_dirty_bigdata[pinfo->base.id].frame_count = 0;
	mutex_init(&g_multi_dirty_bigdata[pinfo->base.id].lock);
	mutex_init(&g_multi_dirty_work[pinfo->base.id].lock);
	if (is_primary_panel(&pinfo->base)) {
		ukmd_sysfs_attrs_append(attrs, &dev_attr_partial_update_bigdata_gfx_primary.attr);
		ukmd_sysfs_attrs_append(attrs, &dev_attr_multi_dirty_bigdata_gfx_primary.attr);
	} else if (is_builtin_panel(&pinfo->base)) {
		ukmd_sysfs_attrs_append(attrs, &dev_attr_partial_update_bigdata_gfx_builtin.attr);
		ukmd_sysfs_attrs_append(attrs, &dev_attr_multi_dirty_bigdata_gfx_builtin.attr);
	}
}

int32_t partial_update_bigdata_update(struct dpu_composer *dpu_comp, struct disp_frame *frame)
{
	uint64_t reason = 0;
	uint32_t dirty_area = 0;
	int32_t area_index = -1;
	int32_t panel_id = -1;
	uint32_t fullscreen_area = 0;
	int32_t i = 0;
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;

	if (pinfo->dirty_region_updt_support == 0)
		return -1;

	panel_id = (int32_t)pinfo->base.id;
	if (panel_id >= PANEL_ID_MAX_NUM || panel_id < 0) {
		dpu_pr_err("panel id is error: %d", panel_id);
		return -1;
	}

	reason = frame->forbid_partial_update_reason;
	if ((reason & (1 << (FORBID_PARTIAL_DISENABLE_INDEX - 1))) != 0)
		return 0;

	mutex_lock(&g_bigdata[panel_id].lock);
	g_bigdata[panel_id].frame_count++;
	if (reason == 0) {
		g_bigdata[panel_id].dirty_rect = frame->disp_rect;
		dirty_area = g_bigdata[panel_id].dirty_rect.w * g_bigdata[panel_id].dirty_rect.h;
		fullscreen_area = pinfo->base.xres * pinfo->base.yres;

		if (fullscreen_area == 0) {
			dpu_pr_err("fullscreen_area is zero");
			mutex_unlock(&g_bigdata[panel_id].lock);
			return -1;
		}
		/*
		* round(int / int)
		* index = (AREA_INTERVAL_SIZE - 1) * dirty_area / fullscreen_area
		*/
		area_index = (int32_t)((10 * (AREA_INTERVAL_SIZE - 1) * dirty_area / fullscreen_area + 5) / 10);
		if (area_index < 0 || area_index > AREA_INTERVAL_SIZE - 1) {
			dpu_pr_err("area_index %d calc error (0 <= area_index <=%d) panel_id %d  dirty area %d fullscreen area %d ",
				area_index, AREA_INTERVAL_SIZE - 1, panel_id, dirty_area, fullscreen_area);
		} else {
			g_bigdata[panel_id].area_count[area_index]++;
		}
	} else if (reason > 0) {
		for (i = 0; i < FORBID_PARTIAL_REASON_SIZE; i++) {
			if ((reason & (1 << i)) != 0) {
				g_bigdata[panel_id].forbid_reasons[i]++;
				break;
			}
		}
	}
	mutex_unlock(&g_bigdata[panel_id].lock);
	return 0;
}

void partial_update_bigdata_thread_init(struct dpu_composer *dpu_comp)
{
	uint32_t panel_id = dpu_comp->conn_info->base.id;

	if (unlikely(panel_id >= PANEL_ID_MAX_NUM)) {
		dpu_pr_info("panel_id out of bounds");
		return;
	}

	dpu_pr_debug("muti_dirty : is_init : %d", g_multi_dirty_work[panel_id].multi_dirty_is_init);
	mutex_lock(&g_multi_dirty_work[panel_id].lock);
	if (!g_multi_dirty_work[panel_id].multi_dirty_is_init) {
		kthread_init_worker(&g_multi_dirty_work[panel_id].bigdata_worker_queue);
		kthread_init_work(&g_multi_dirty_work[panel_id].kwork, partial_update_bigdata_func);
		g_multi_dirty_work[panel_id].bigdata_task_thread = kthread_run(kthread_worker_fn,
			&g_multi_dirty_work[panel_id].bigdata_worker_queue,
			panel_id == PANEL_ID_PRIMARY ? "cal_primary" : "cal_else");
		g_multi_dirty_work[panel_id].multi_dirty_is_init = true;
	}
	mutex_unlock(&g_multi_dirty_work[panel_id].lock);
}

void partial_update_bigdata_thread_deinit(struct dpu_composer *dpu_comp)
{
	uint32_t panel_id = dpu_comp->conn_info->base.id;
	
	if (unlikely(panel_id >= PANEL_ID_MAX_NUM)) {
		dpu_pr_info("panel_id out of bounds");
		return;
	}

	dpu_pr_debug("muti_dirty : can_deinit : %d", g_multi_dirty_work[panel_id].multi_dirty_is_init);
	mutex_lock(&g_multi_dirty_work[panel_id].lock);
	if (g_multi_dirty_work[panel_id].multi_dirty_is_init) {
		kthread_stop(g_multi_dirty_work[panel_id].bigdata_task_thread);
		kthread_destroy_worker(&g_multi_dirty_work[panel_id].bigdata_worker_queue);
		g_multi_dirty_work[panel_id].multi_dirty_is_init = false;
	}
	mutex_unlock(&g_multi_dirty_work[panel_id].lock);
}

void setup_multi_dirty_bigdata_update(struct dpu_composer *dpu_comp, struct disp_frame *frame)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	int32_t i;
	uint32_t panel_id = pinfo->base.id;

	if (unlikely(panel_id >= PANEL_ID_MAX_NUM)) {
		dpu_pr_info("panel_id out of bounds");
		return;
	}

	mutex_lock(&g_multi_dirty_work[panel_id].lock);
	if (!g_multi_dirty_work[panel_id].multi_dirty_is_init) {
		dpu_pr_err("muti_dirty : not init");
		mutex_unlock(&g_multi_dirty_work[panel_id].lock);
		return;
	}

	if (frame->multi_dirty_rects_size < 0 || frame->multi_dirty_rects_size > DISP_RECT_MAX_DIRTY_NUM) {
		dpu_pr_err("muti_dirty : g_multi_dirty_work multi_dirty_rects_size :%d invalidate", 
		frame->multi_dirty_rects_size);
		mutex_unlock(&g_multi_dirty_work[panel_id].lock);
		return;
	}

	g_multi_dirty_work[panel_id].dirty_region_updt_support = pinfo->dirty_region_updt_support;
	g_multi_dirty_work[panel_id].common_data.xres = pinfo->base.xres;
	g_multi_dirty_work[panel_id].common_data.yres = pinfo->base.yres;
	g_multi_dirty_work[panel_id].common_data.panel_id = (int32_t)panel_id;
	g_multi_dirty_work[panel_id].forbid_partial_update_reason = frame->forbid_partial_update_reason;
	g_multi_dirty_work[panel_id].common_data.dirty_size = frame->multi_dirty_rects_size;
	for (i = 0; i < frame->multi_dirty_rects_size; i++) {
		dpu_pr_debug("muti_dirty : g_multi_dirty_work dirty_rect x :%d , y:%d , w:%u , h:%u", 
		frame->multi_dirty_rects[i].x, frame->multi_dirty_rects[i].y, frame->multi_dirty_rects[i].w, 
		frame->multi_dirty_rects[i].h);
		if (frame->multi_dirty_rects[i].x < 0 || frame->multi_dirty_rects[i].y < 0) {
			dpu_pr_debug("muti_dirty : dirty_rect x < 0 or y < 0");
			mutex_unlock(&g_multi_dirty_work[panel_id].lock);
			return;
		}
		g_multi_dirty_work[panel_id].multi_dirty_rects[i] = frame->multi_dirty_rects[i];
	}
	mutex_unlock(&g_multi_dirty_work[panel_id].lock);

	kthread_queue_work(&g_multi_dirty_work[panel_id].bigdata_worker_queue, &g_multi_dirty_work[panel_id].kwork);
}

static int32_t cmp_rect_top(const void *a, const void *b)
{
	struct dkmd_rect *rect1 = (struct dkmd_rect *)a;
	struct dkmd_rect *rect2 = (struct dkmd_rect *)b;
	return rect1->y - rect2->y;
}

static int32_t cmp_distance_dis(const void *a, const void *b)
{
	struct partial_update_rect_distance *d1 = (struct partial_update_rect_distance *)a;
	struct partial_update_rect_distance *d2 = (struct partial_update_rect_distance *)b;
	return d1->distance - d2->distance;
}

static int32_t cmp_distance_seq_id(const void *a, const void *b)
{
	struct partial_update_rect_distance *d1 = (struct partial_update_rect_distance *)a;
	struct partial_update_rect_distance *d2 = (struct partial_update_rect_distance *)b;
	return d1->seq_id - d2->seq_id;
}

static void union_to_first(struct dkmd_rect *first_rect, struct dkmd_rect *second_rect)
{
	int32_t first_r = first_rect->x + (int32_t)first_rect->w;
	int32_t first_b = first_rect->y + (int32_t)first_rect->h;
	int32_t second_r = second_rect->x + (int32_t)second_rect->w;
	int32_t second_b = second_rect->y + (int32_t)second_rect->h;
	int32_t r = max(first_r, second_r);
    int32_t b = max(first_b, second_b);
	first_rect->x = min(first_rect->x, second_rect->x);
    first_rect->y = min(first_rect->y, second_rect->y);
	first_rect->w = (uint32_t)(r - first_rect->x);
	first_rect->h = (uint32_t)(b - first_rect->y);
}

static void cal_dirty_rect_width(struct dkmd_rect *cal_dirty_width_rects, struct dkmd_rect *dirty_rects,
	struct multi_dirty_cal_param *cal_param, int32_t dirty_rects_size)
{
	int32_t tmp_width = 0;
	int32_t k;
	int32_t j;
	int32_t i;
	int32_t div_widths[MAX_DIV_VALUE + 1];

	if (cal_param->div != 0) {
		for (i = 0; i <= cal_param->div; i++) {
			div_widths[i] = i * (int32_t)(cal_param->common_data.xres) / cal_param->div;
		}
	}

	for (k = 0; k < dirty_rects_size; k++) {
		cal_dirty_width_rects[k] = dirty_rects[k];

		if (cal_param->div == 0) {
			dpu_pr_debug("muti_dirty : div is 0!!!");
			continue;
		}

		tmp_width = 0;
		for (j = 0; j <= cal_param->div; j++) {
			if (cal_dirty_width_rects[k].x >= div_widths[j]) {
				tmp_width = div_widths[j];
			}

			if ((cal_dirty_width_rects[k].x + (int32_t)cal_dirty_width_rects[k].w) <= div_widths[j]) {
				cal_dirty_width_rects[k].w = (uint32_t)(div_widths[j] - tmp_width);
				break;
			}
		}
		cal_dirty_width_rects[k].x = tmp_width;
	}

	sort(cal_dirty_width_rects, dirty_rects_size, sizeof(struct dkmd_rect), cmp_rect_top, NULL);
}

static void cal_dirty_num_less_support_num(uint32_t area, struct multi_dirty_cal_param *cal_param)
{
	int32_t condition = 0;
	uint32_t index = 0;
	int32_t j;

	if (cal_param->fullscreen_area == 0) {
		dpu_pr_err("muti_dirty : screen area == 0");
		return;
	}

	for (j = 0; j < cal_param->common_data.dirty_size; j++) {
		area += cal_param->need_cal_dirty_rects[j].w * cal_param->need_cal_dirty_rects[j].h;
	}

	if (area == 0) {
		dpu_pr_err("muti_dirty : area == 0");
		return;
	}

	condition = cal_param->div_num * SUPPORT_DIRTY_MAX_NUM + cal_param->support_dirty_count - 1;

	index = ((area - 1) * AREA_INTERVAL_SIZE + cal_param->fullscreen_area / 2) /
			cal_param->fullscreen_area;
	
	index = (index == 0) ? 0 : index - 1;

	if (index >= AREA_INTERVAL_SIZE) {
		dpu_pr_err("index = %u is >= 25", index);
		return;
	}
	dpu_pr_debug("muti_dirty : less condition = %d index = %u ", condition, index);

	g_multi_dirty_bigdata[cal_param->common_data.panel_id].area_count[condition][index]++;
}

static void set_dirty_rect_distance(
	struct partial_update_rect_distance *distances, struct multi_dirty_cal_param *cal_param, int32_t distances_size)
{
	struct partial_update_rect_distance distance;
	int32_t dirty_rect_idx;
	int32_t pre_dirty_rect_bottom;

	for (dirty_rect_idx = 1; dirty_rect_idx <= distances_size; dirty_rect_idx++) {
		pre_dirty_rect_bottom = cal_param->need_cal_dirty_rects[dirty_rect_idx - 1].y +
								(int32_t)cal_param->need_cal_dirty_rects[dirty_rect_idx - 1].h;
		distance.distance = cal_param->need_cal_dirty_rects[dirty_rect_idx].y - pre_dirty_rect_bottom;
		distance.seq_id = dirty_rect_idx;
		distances[dirty_rect_idx - 1] = distance;
	}

	sort(distances, distances_size, sizeof(struct partial_update_rect_distance), cmp_distance_dis, NULL);
}

static void merge_distance_cal_area(uint32_t *area, struct partial_update_rect_distance *merge_distances,
	struct multi_dirty_cal_param *cal_param, int32_t merge_distance_size)
{
	int32_t findFlags[DISP_RECT_MAX_DIRTY_NUM] = {0};
	struct dkmd_rect uni_dirty_rect = {0, 0, 0, 0};
	int32_t merge_distance_idx;
	int32_t flag_idx;
	int32_t pre_seq_id = -1;

	for (merge_distance_idx = 0; merge_distance_idx < merge_distance_size; merge_distance_idx++) {
		if (merge_distances[merge_distance_idx].seq_id != pre_seq_id + 1) {
			(*area) += uni_dirty_rect.w * uni_dirty_rect.h;
			uni_dirty_rect = cal_param->need_cal_dirty_rects[merge_distances[merge_distance_idx].seq_id - 1];
		}
		union_to_first(&uni_dirty_rect, &cal_param->need_cal_dirty_rects[merge_distances[merge_distance_idx].seq_id]);
		findFlags[merge_distances[merge_distance_idx].seq_id - 1] = 1;
		findFlags[merge_distances[merge_distance_idx].seq_id] = 1;
		pre_seq_id = merge_distances[merge_distance_idx].seq_id;
	}

	(*area) += uni_dirty_rect.w * uni_dirty_rect.h;

	for (flag_idx = 0; flag_idx < cal_param->common_data.dirty_size; flag_idx++) {
		if (findFlags[flag_idx] == 0) {
			(*area) += cal_param->need_cal_dirty_rects[flag_idx].w * cal_param->need_cal_dirty_rects[flag_idx].h;
		}
	}
}

static void merge_dirty_rect_distance(
	uint32_t *area, struct partial_update_rect_distance *distances, struct multi_dirty_cal_param *cal_param)
{
	int32_t merge_distance_size = cal_param->common_data.dirty_size - cal_param->support_dirty_count;
	struct partial_update_rect_distance merge_distances[DISP_RECT_MAX_DIRTY_NUM];
	int32_t idx;

	for (idx = 0; idx < merge_distance_size; idx++) {
		merge_distances[idx] = distances[idx];
	}

	sort(merge_distances, merge_distance_size, sizeof(struct partial_update_rect_distance), cmp_distance_seq_id, NULL);

	merge_distance_cal_area(area, merge_distances, cal_param, merge_distance_size);
}

static void cal_dirty_num_more_support_num(uint32_t area, struct multi_dirty_cal_param *cal_param)
{
	struct partial_update_rect_distance distances[DISP_RECT_MAX_DIRTY_NUM];
	int32_t condition = 0;
	uint32_t index = 0;
	int32_t distances_size = cal_param->common_data.dirty_size - 1;

	if (cal_param->fullscreen_area == 0) {
		dpu_pr_err("muti_dirty : screen area == 0");
		return;
	}

	set_dirty_rect_distance(distances, cal_param, distances_size);

	merge_dirty_rect_distance(&area, distances, cal_param);

	if (area == 0) {
		dpu_pr_err("muti_dirty : area == 0");
		return;
	}

	condition = cal_param->div_num * SUPPORT_DIRTY_MAX_NUM + cal_param->support_dirty_count - 1;
	if (condition < 0 || condition >= MULTI_DIRTY_COMBINATION_SIZE) {
		dpu_pr_err("condition = %d is invalid", condition);
		return;
	}

	index = ((area - 1) * AREA_INTERVAL_SIZE + cal_param->fullscreen_area / 2) /
			cal_param->fullscreen_area;
	
	index = (index == 0) ? 0 : index - 1;
	if (index >= AREA_INTERVAL_SIZE) {
		dpu_pr_err("index = %u is >= 25", index);
		return;
	}
	dpu_pr_debug("muti_dirty : more condition = %d index = %u ", condition, index);

	g_multi_dirty_bigdata[cal_param->common_data.panel_id].area_count[condition][index]++;
}

static void multi_dirty_cal(
	struct multi_dirty_cal_param *cal_param, struct dkmd_rect *dirty_rects, int32_t dirty_rects_size)
{
	int32_t div_num;
	int32_t support_dirty_count;
	uint32_t area;

	if (dirty_rects_size == 0) {
		dpu_pr_debug("muti_dirty : dirty_size is zero");
		return;
	}

	for (div_num = 0; div_num <= DIV_NUM; div_num++) {
		cal_param->div_num = div_num;
		cal_param->div = int32_pow(DIV_BASE_NUM, cal_param->div_num);

		cal_dirty_rect_width(cal_param->need_cal_dirty_rects, dirty_rects, cal_param, dirty_rects_size);

		for (support_dirty_count = 1; support_dirty_count <= SUPPORT_DIRTY_MAX_NUM; support_dirty_count++) {
			cal_param->support_dirty_count = support_dirty_count;
			cal_param->fullscreen_area = cal_param->common_data.xres * cal_param->common_data.yres;
			area = 0;

			if (cal_param->fullscreen_area == 0) {
				dpu_pr_err("fullscreen_area is zero");
				return;
			}

			if (dirty_rects_size <= cal_param->support_dirty_count) {
				cal_dirty_num_less_support_num(area, cal_param);
			} else {
				cal_dirty_num_more_support_num(area, cal_param);
			}
		}
	}
}

void partial_update_bigdata_func(struct kthread_work *work)
{
    struct partial_update_bigdata_work *bigdata_work = container_of(work, struct partial_update_bigdata_work, kwork);
	uint8_t dirty_region_updt_support;
	uint64_t reason;
	struct dkmd_rect dirty_rects[DISP_RECT_MAX_DIRTY_NUM];
	struct multi_dirty_cal_param cal_param;
	int32_t i;
	int32_t panel_id = bigdata_work->common_data.panel_id;

	if (panel_id >= PANEL_ID_MAX_NUM || panel_id < 0) {
		dpu_pr_err("muti_dirty : panel id is error: %d", panel_id);
		return;
	}

	mutex_lock(&g_multi_dirty_work[panel_id].lock);
	dirty_region_updt_support = bigdata_work->dirty_region_updt_support;
	reason = bigdata_work->forbid_partial_update_reason;
	cal_param.common_data = bigdata_work->common_data;
	dpu_pr_debug("muti_dirty : dirty_size --- %d", cal_param.common_data.dirty_size);
	memcpy_s(
		dirty_rects, sizeof(dirty_rects), bigdata_work->multi_dirty_rects, sizeof(bigdata_work->multi_dirty_rects));
	mutex_unlock(&g_multi_dirty_work[panel_id].lock);

	for (i = 0; i < cal_param.common_data.dirty_size; i++) {
		dpu_pr_debug("muti_dirty : dirty_rect x :%d , y:%d , w:%u , h:%u", dirty_rects[i].x, dirty_rects[i].y, dirty_rects[i].w, dirty_rects[i].h);
		if (dirty_rects[i].x < 0 || dirty_rects[i].y < 0 || ((uint32_t)dirty_rects[i].x + dirty_rects[i].w) > cal_param.common_data.xres ||
		 ((uint32_t)dirty_rects[i].y + dirty_rects[i].h) > cal_param.common_data.yres) {
			dpu_pr_debug("muti_dirty : dirty_rect x , y , w , h invalidate!!!");
			return;
		}
	}

	if (dirty_region_updt_support == 0) {
		dpu_pr_debug("muti_dirty : dirty_region_updt_support %u", dirty_region_updt_support);
		return;
	}

	dpu_pr_debug("muti_dirty : frame.forbid_partial_update_reason %lu", reason);
	if ((reason & (1 << (FORBID_PARTIAL_DISENABLE_INDEX - 1))) != 0) {
		return;
	}

	mutex_lock(&g_multi_dirty_bigdata[panel_id].lock);
	g_multi_dirty_bigdata[panel_id].frame_count++;

	if (reason == 0) {
		multi_dirty_cal(&cal_param, dirty_rects, cal_param.common_data.dirty_size);
	}

	mutex_unlock(&g_multi_dirty_bigdata[panel_id].lock);
}