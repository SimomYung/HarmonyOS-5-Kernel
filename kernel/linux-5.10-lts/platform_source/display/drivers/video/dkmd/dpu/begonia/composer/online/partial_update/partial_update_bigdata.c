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
#define AREA_BUF_SIZE 300
#define FORBID_BUF_SIZE 400

static struct partial_update_bigdata g_bigdata[PANEL_ID_MAX_NUM];

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

	ret = snprintf_s(area_buf, AREA_BUF_SIZE, AREA_BUF_SIZE - 1,
		"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u", area_count[0], area_count[1],
		area_count[2], area_count[3], area_count[4], area_count[5], area_count[6], area_count[7], area_count[8],
		area_count[9], area_count[10], area_count[11], area_count[12], area_count[13], area_count[14], area_count[15],
		area_count[16], area_count[17], area_count[18], area_count[19], area_count[20], area_count[21], area_count[22],
		area_count[23], area_count[24]);
	if (ret == -1)
		dpu_pr_warn("area info fault");

	ret = snprintf_s(forbid_buf, FORBID_BUF_SIZE, FORBID_BUF_SIZE - 1,
		"%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u",
		forbid_count[0], forbid_count[1], forbid_count[2], forbid_count[3], forbid_count[4], forbid_count[5],
		forbid_count[6], forbid_count[7], forbid_count[8], forbid_count[9], forbid_count[10], forbid_count[11],
		forbid_count[12], forbid_count[13], forbid_count[14], forbid_count[15], forbid_count[16], forbid_count[17],
		forbid_count[18], forbid_count[19], forbid_count[20], forbid_count[21], forbid_count[22], forbid_count[23],
		forbid_count[24], forbid_count[25], forbid_count[26], forbid_count[27], forbid_count[28], forbid_count[29],
		forbid_count[30], forbid_count[31]);
	if (ret == -1)
		dpu_pr_warn("forbid info fault");

	ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d|%s|%s\n", frame_count, area_buf, forbid_buf);
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

static DEVICE_ATTR(partial_update_bigdata_gfx_primary, 0440, partial_update_bigdata_gfx_primary_show, NULL);
static DEVICE_ATTR(partial_update_bigdata_gfx_builtin, 0440, partial_update_bigdata_gfx_builtin_show, NULL);

void partial_update_bigdata_setup(struct dpu_composer *dpu_comp)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct ukmd_attr *attrs = &dpu_comp->attrs;
	dpu_check_and_no_retval(!attrs, err, "attrs is null pointer");
	if (pinfo->dirty_region_updt_support == 0) {
		dpu_pr_info("panel do not dirty_region_updt_support, do not set bigdata node");
		return;
	}
	dpu_pr_info("setup bigdata node\n");

	g_bigdata[pinfo->base.id].frame_count = 0;
	mutex_init(&g_bigdata[pinfo->base.id].lock);
	if (is_primary_panel(&pinfo->base))
		ukmd_sysfs_attrs_append(attrs, &dev_attr_partial_update_bigdata_gfx_primary.attr);
	else if (is_builtin_panel(&pinfo->base))
		ukmd_sysfs_attrs_append(attrs, &dev_attr_partial_update_bigdata_gfx_builtin.attr);
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
		dpu_pr_err("panel id is error: %d\n", panel_id);
		return -1;
	}

	reason = frame->forbid_partial_update_reason;
	if ((reason & (1 << (FORBID_PARTIAL_DISENABLE_INDEX -1))) != 0)
		return 0;

	mutex_lock(&g_bigdata[panel_id].lock);
	g_bigdata[panel_id].frame_count++;
	if (reason == 0) {
		g_bigdata[panel_id].dirty_rect = frame->disp_rect;
		dirty_area = g_bigdata[panel_id].dirty_rect.w * g_bigdata[panel_id].dirty_rect.h;
		fullscreen_area = pinfo->base.xres * pinfo->base.yres;

		if (fullscreen_area == 0) {
			dpu_pr_err("fullscreen_area is zero\n");
			mutex_unlock(&g_bigdata[panel_id].lock);
			return -1;
		}
		/*
		* round(int / int)
		* index = (AREA_INTERVAL_SIZE - 1) * dirty_area / fullscreen_area
		*/
		area_index = (int32_t)((10 * (AREA_INTERVAL_SIZE - 1) * dirty_area / fullscreen_area + 5) / 10);
		if (area_index < 0 || area_index > AREA_INTERVAL_SIZE - 1) {
			dpu_pr_err(
				"area_index %d calc error (0 <= area_index <=%d) panel_id %d  dirty area %d fullscreen area %d \n",
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