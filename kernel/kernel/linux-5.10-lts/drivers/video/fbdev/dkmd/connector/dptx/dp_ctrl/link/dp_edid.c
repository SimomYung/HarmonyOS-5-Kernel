/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "dp_edid.h"
#include "dp_drv.h"
#include "dp_dmd_report.h"
#include <securec.h>
#include "dp_dtd_helper.h"
#include "dkmd_object.h"

static const uint8_t edid_v1_other_descriptor_flag[2] = {0x00, 0x00};

static struct list_head *dptx_hdmi_list;
struct dptx_hdmi_vic {
	struct list_head list_node;
	uint32_t vic_id;
};
static uint8_t g_hdmi_vic_len;
static uint8_t g_hdmi_vic_real_len;

static const struct image_ration_t image_aspect_ration[] = {
	{.h_ratio = 16, .v_ratio = 10},
	{.h_ratio = 4, .v_ratio = 3},
	{.h_ratio = 5, .v_ratio = 4},
	{.h_ratio = 16, .v_ratio = 9},
};

#define IMAGE_H_RATIO(x)	image_aspect_ration[IMAGE_ASPECT_RATIO_INDEX(x)].h_ratio
#define IMAGE_V_RATIO(x)	image_aspect_ration[IMAGE_ASPECT_RATIO_INDEX(x)].v_ratio

static bool is_valid_standard_timing(uint8_t hdisplay_size, uint8_t freq_aspect)
{
	if ((hdisplay_size == 0x00 && freq_aspect == 0x00) ||
		(hdisplay_size == 0x01 && freq_aspect == 0x01))
		return false;

	return true;
}

static void add_timing_node(struct edid_video *vid_info, struct dtd *edid_timing)
{
	struct timing_info *node = NULL;

	if (vid_info->dptx_timing_list == NULL) {
		vid_info->dptx_timing_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
		if (vid_info->dptx_timing_list == NULL) {
			dpu_pr_err("[DP] vid_info->dptx_timing_list is NULL");
			return;
		}
		INIT_LIST_HEAD(vid_info->dptx_timing_list);

		vid_info->main_vcount = 0;
		/* Initial Max Value */
		vid_info->max_hpixels = edid_timing->h_active;
		vid_info->max_vpixels = edid_timing->v_active;
		vid_info->max_pixel_clock = edid_timing->pixel_clock / 10;
		dpu_pr_info("[DP] max_pixel_clock 1 is %llu", vid_info->max_pixel_clock);
	}

	/* Get Max Value by comparing all values */
	if ((vid_info->max_hpixels < edid_timing->h_active) && (vid_info->max_vpixels <= edid_timing->v_active) &&
		(vid_info->max_pixel_clock < edid_timing->pixel_clock / 10)) {
		vid_info->max_hpixels = edid_timing->h_active;
		vid_info->max_vpixels = edid_timing->v_active;
		vid_info->max_pixel_clock = edid_timing->pixel_clock / 10;
		dpu_pr_info("[DP] max_pixel_clock 2 is %llu", vid_info->max_pixel_clock);
	}

	node = kzalloc(sizeof(struct timing_info), GFP_KERNEL);
	if (node != NULL) {
		node->hactive_pixels = edid_timing->h_active;
		node->hblanking = edid_timing->h_blanking;
		node->hsync_offset = edid_timing->h_sync_offset;
		node->hsync_pulse_width = edid_timing->h_sync_pulse_width;
		node->hsize = edid_timing->h_image_size;

		node->vactive_pixels = edid_timing->v_active;
		node->vblanking = edid_timing->v_blanking;
		node->vsync_offset = edid_timing->v_sync_offset;
		node->vsync_pulse_width = edid_timing->v_sync_pulse_width;
		node->vsize = edid_timing->v_image_size;

		node->pixel_clock = edid_timing->pixel_clock / 10;
		node->interlaced = edid_timing->interlaced;
		node->hsync_polarity = edid_timing->h_sync_polarity;
		node->vsync_polarity = edid_timing->v_sync_polarity;

		vid_info->main_vcount += 1;
		list_add_tail(&node->list_node, vid_info->dptx_timing_list);
	} else {
		dpu_pr_err("[DP] kzalloc struct dptx_hdmi_vic fail!\n");
		return;
	}

	dpu_pr_info("[DP] The timinginfo %u: hactive_pixels is %u, vactive_pixels is %u, pixel clock=%llu\n",
		vid_info->main_vcount, node->hactive_pixels, node->vactive_pixels, node->pixel_clock);
}

static int skip_same_resolution(struct dkmd_connector_info *pinfo, struct timing_info *dptx_timing_node, uint32_t cnt)
{
	int ret = 0;
	uint32_t i = 0;

	dpu_check_and_return(!pinfo, -EINVAL, err, "[DP] pinfo is NULL!\n");
	dpu_check_and_return(!dptx_timing_node, -EINVAL, err, "[DP] dptx_timing_node is NULL!\n");

	for (i = 0; i < cnt; i++) {
		if (pinfo->base.edid_timing[i].hactive == dptx_timing_node->hactive_pixels &&
				pinfo->base.edid_timing[i].vactive == dptx_timing_node->vactive_pixels) {
			dpu_pr_warn("[DP] the same resolution, skip this timing node");
			ret = -EINVAL;
			break;
		}
	}
	return ret;
}

static void edid_timing_from_node_to_pinfo(struct dkmd_connector_info *dst_timing, struct timing_info *src_timing,
												uint32_t index, uint16_t fps)
{
	dst_timing->base.edid_timing[index].hactive = src_timing->hactive_pixels;
	dst_timing->base.edid_timing[index].hblanking = src_timing->hblanking;
	dst_timing->base.edid_timing[index].hsync_offset = src_timing->hsync_offset;
	dst_timing->base.edid_timing[index].hsync_pulse_width = src_timing->hsync_pulse_width;
	dst_timing->base.edid_timing[index].hsize = src_timing->hsize;
	dst_timing->base.edid_timing[index].hsync_polarity = src_timing->hsync_polarity;

	dst_timing->base.edid_timing[index].vactive = src_timing->vactive_pixels;
	dst_timing->base.edid_timing[index].vblanking = src_timing->vblanking;
	dst_timing->base.edid_timing[index].vsync_offset = src_timing->vsync_offset;
	dst_timing->base.edid_timing[index].vsync_pulse_width = src_timing->vsync_pulse_width;
	dst_timing->base.edid_timing[index].vsize = src_timing->vsize;
	dst_timing->base.edid_timing[index].vsync_polarity = src_timing->vsync_polarity;

	dst_timing->base.edid_timing[index].pixel_clock = src_timing->pixel_clock;
	dst_timing->base.edid_timing[index].interlaced = src_timing->interlaced;
	dst_timing->base.edid_timing[index].fps = fps;
}

static void copy_edid_info_to_comp(struct dp_ctrl *dptx)
{
	int ret = 0;
	uint32_t i = 0, fps = 0;
	struct timing_info *dptx_timing_node = NULL;
	struct timing_info *_node_ = NULL;
	struct dkmd_connector_info *pinfo = NULL;

	dpu_pr_info("[DP] copy edid supported display info to composer");
	pinfo = dptx->connector->conn_info;

	if (dptx->edid_info.video.dptx_timing_list != NULL) {
		list_for_each_entry_safe(dptx_timing_node, _node_, dptx->edid_info.video.dptx_timing_list, list_node) {
			ret = skip_same_resolution(pinfo, dptx_timing_node, i);
			if (ret)
				continue;
			ret = dptx_calc_fps(dptx_timing_node, &fps);
			if (ret) {
				dpu_pr_warn("[DP] fps calc error, skip this timing node");
				continue;
			}
			edid_timing_from_node_to_pinfo(pinfo, dptx_timing_node, i, (uint16_t)fps);
			dpu_pr_info("[DP] copy display info to composer: h:%hu, v:%hu, f:%hu", pinfo->base.edid_timing[i].hactive,
				pinfo->base.edid_timing[i].vactive, pinfo->base.edid_timing[i].fps);
			i++;
		}
	}
	pinfo->base.edid_timing_num = i;
	dpu_pr_info("[DP] display info num to composer is: %d", pinfo->base.edid_timing_num);
}

static void check_factory_info_change(struct dp_ctrl *dptx)
{
	int edid_index = 0;
	int factory_index = 0;
	dpu_pr_info("[DP] check_factory_info_change enter");
	dptx->is_edid_change = false;
	if (dptx->is_first_read_edid) {
		dptx->is_first_read_edid = false;
		dpu_pr_info("[DP] is first read edid");
		return;
	}
 
	for (edid_index = EDID_FACTORY_START, factory_index = 0; edid_index <= EDID_FACTORY_END;
		++edid_index, ++factory_index) {
		if (factory_index < FACTORY_EDID_MAX_LEN && dptx->edid[edid_index] != dptx->factory_edid_info[factory_index]) {
			dptx->is_edid_change = true;
			dpu_pr_warn("[DP] is_edid_change is true, edid_index = %d, edid[edid_index] = %x,\
				factory_edid_info[factory_index] =%x", edid_index,
				dptx->edid[edid_index], dptx->factory_edid_info[factory_index]);
			break;
		}
	}
}

static void print_edid(uint8_t *edid, struct monitor_name_descriptor name_descriptor, uint32_t edid_len)
{
	uint32_t i;
	uint8_t data_sum = 0;
	uint32_t data_len = name_descriptor.product_end - name_descriptor.product_start;
	const uint8_t edid_col = 16;
	bool is_need_update_product_name;
	uint8_t data_tmp[EDID_FACTORY_END - EDID_FACTORY_START + 1];
	uint8_t data_name[MONTIOR_NAME_DESCRIPTION_SIZE];

	is_need_update_product_name = (name_descriptor.product_name != NULL) && (name_descriptor.product_start != 0) &&
		(name_descriptor.product_end != 0);

	for (i = EDID_FACTORY_START; i <= EDID_FACTORY_END; i++)
		data_tmp[i - EDID_FACTORY_START] = edid[i];

	if (memset_s(&edid[EDID_FACTORY_START], (EDID_FACTORY_END - EDID_FACTORY_START + 1), 0x0,
		(EDID_FACTORY_END - EDID_FACTORY_START + 1)) != 0) {
		dpu_pr_err("[DP] memset edid error!");
		return;
	}

	if (is_need_update_product_name) {
		dpu_pr_info("[DP] product_start = %u, product_end = %u data_len = %u\n", name_descriptor.product_start, 
			name_descriptor.product_end, data_len);

		if (data_len > MONTIOR_NAME_DESCRIPTION_SIZE) {
			dpu_pr_err("[DP] The value result is incorrect!");
			return;
		}

		for (i = name_descriptor.product_start; i < name_descriptor.product_end; i++) {
			data_name[i - name_descriptor.product_start] = name_descriptor.product_name[i];
			data_sum += name_descriptor.product_name[i];
		}

		if (memset_s(&name_descriptor.product_name[name_descriptor.product_start], data_len, 0x0, data_len) != 0) {
			dpu_pr_err("[DP] memset monitor name fail");
			return;
		}
		name_descriptor.product_name[name_descriptor.product_start] = data_sum;
	}
	
	for (i = 0; i < edid_len;) {
		if ((i % edid_col) == 0) {
			dpu_pr_info("EDID [%04x]:  %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \
%02x %02x %02x %02x %02x\n",
				i, edid[i], edid[i + 1], edid[i + 2], edid[i + 3],
				edid[i + 4], edid[i + 5], edid[i + 6], edid[i + 7],
				edid[i + 8], edid[i + 9], edid[i + 10], edid[i + 11],
				edid[i + 12], edid[i + 13], edid[i + 14], edid[i + 15]);
		}
		i += edid_col;
		if (i == EDID_BLOCK_LENGTH)
			dpu_pr_info("<<<-------------------------------------------------------------->>>\n");
	}

	for (i = EDID_FACTORY_START; i <= EDID_FACTORY_END; i++)
		edid[i] = data_tmp[i - EDID_FACTORY_START];

	if (is_need_update_product_name && memcpy_s(&name_descriptor.product_name[name_descriptor.product_start],
			data_len, data_name, data_len) != EOK) {
		dpu_pr_err("[DP] memcpy_s fail");
		return;
	}
}

static void parse_screen_size_or_aspect_ratio(struct dp_ctrl *dptx)
{
	uint8_t hscreen_size = 0;
	uint8_t vscreen_size = 0;
	uint8_t edid_version = 0;
	uint8_t edid_revision = 0;

	dpu_check_and_no_retval(!dptx->edid, err, "[DP] dptx->edid is NULL!");

	hscreen_size = dptx->edid[HORIZONTAL_AND_VERTICAL_SCREEN_SIZE];
	vscreen_size = dptx->edid[HORIZONTAL_AND_VERTICAL_SCREEN_SIZE + 1];
	edid_version = dptx->edid[EDID_STRUCT_VERSION];
	edid_revision = dptx->edid[EDID_STRUCT_REVISION];

	dptx->screen_ratio_size = 0;
	if (hscreen_size == 0 && vscreen_size == 0) {
		dpu_pr_info("[DP] the manufacturer does not specify the display’s screen size");
		return;
	}

	dpu_pr_info("[DP] the screen size is [%u * %u]", hscreen_size, vscreen_size);
	if (edid_version > 1 || (edid_version == 1 && edid_revision >= 4)) {
		if (hscreen_size != 0 && vscreen_size == 0) {
			dptx->screen_ratio_size = hscreen_size + 99;
			dpu_pr_info("[DP] the aspect ratio(h/v) of landscape orientation is:%u", dptx->screen_ratio_size);
		}
		if (hscreen_size == 0 && vscreen_size != 0) {
			dptx->screen_ratio_size = vscreen_size + 99;
			dpu_pr_info("[DP] the aspect ratio(v/h) of portrait orientation is:%u", dptx->screen_ratio_size);
		}
	}
	
	if (hscreen_size != 0 && vscreen_size != 0) {
		dptx->screen_ratio_size = (hscreen_size >= vscreen_size ? IMAGE_RATIO(hscreen_size, vscreen_size) :
									IMAGE_RATIO(vscreen_size, hscreen_size));
		dpu_pr_info("[DP] the aspect ratio size of screen is approximately %u", dptx->screen_ratio_size);
	}
}

int parse_edid(struct dp_ctrl *dptx, uint16_t len)
{
	int16_t i, ext_block_num;
	int ret, ext_timing_num;
	uint8_t *edid_t = NULL;
	uint32_t edid_factory_len = EDID_FACTORY_END - EDID_FACTORY_START + 1;

	dpu_pr_info("[DP] entry");

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");

	edid_t = dptx->edid;

	/* Check if data has any error */
	dpu_check_and_return(!edid_t, -EINVAL, err, "[DP] Raw Data is invalid!(NULL error)!\n");

	if (((len / EDID_LENGTH) > 5) || ((len % EDID_LENGTH) != 0) || (len < EDID_LENGTH)) {
		dpu_pr_err("[DP] Raw Data length is invalid(not the size of (128 x N , N = [1-5]) uint8_t!");
		return -EINVAL;
	}
	dptx->edid_len = len;
	check_factory_info_change(dptx);
	if (memcpy_s(dptx->factory_edid_info, edid_factory_len, &dptx->edid[EDID_FACTORY_START], edid_factory_len) != EOK)
		dpu_pr_err("[DP] memcpy edid buffer error!");

	/* Parse the EDID main part, check how many(count as ' ext_block_num ') Extension blocks there are to follow. */
	ext_block_num = parse_main(dptx);
	dpu_pr_debug("[DP] ext_block_num is %d, len is %hu", ext_block_num, len);

	dptx->connector->conn_info->dp_color_gamut_flag = true;

	if (unlikely(memcpy_s(dptx->connector->conn_info->color_gamut_info, EDID_COLOR_DATA_LEN,
		&dptx->edid[EDID_COLOR_GAMUT_START], EDID_COLOR_DATA_LEN) != EOK)) {
        dpu_pr_err("[DP] memcpy edid color gamut buffer error!");
		dptx->connector->conn_info->dp_color_gamut_flag = false;
    }

	if ((ext_block_num > 0) && (len == (uint16_t)((ext_block_num + 1) * EDID_LENGTH))) {
		dptx->edid_info.audio.ext_acount = 0;
		dptx->edid_info.audio.ext_speaker = 0;
		dptx->edid_info.audio.basic_audio = 0;
		dptx->edid_info.audio.spec = NULL;
		/* Parse all Extension blocks */
		for (i = 0; i < ext_block_num; i++) {
			if (edid_t[(EDID_LENGTH * (i + 1))] == EXTENSION_HEADER_TIMING)
				ret = parse_extension_v21(dptx, edid_t + (EDID_LENGTH * (i + 1)));
			else
				ret = parse_extension(dptx, edid_t + (EDID_LENGTH * (i + 1)));
			if (ret != 0) {
				dpu_pr_warn("[DP] Extension Parsing failed!Only returning the Main Part of this EDID!");
				continue;
			}
		}
	} else if (ext_block_num < 0) {
		dpu_pr_err("[DP] Error occurred while parsing, returning with NULL!");
		dp_add_error_log(dptx, "Err_par return NULL");
		return -EINVAL;
	}
	/* Parse the Established Timings in EDID Block0 */
	ext_timing_num = parse_established_timing(dptx);
	dpu_pr_info("total established timing num:%d", ext_timing_num);
	/* Parse the Standard Timings in EDID Block0 */
	ext_timing_num = parse_standard_timing(dptx);
	dpu_pr_info("total standard timing num:%d", ext_timing_num);
	/* Parse the horizontal and vertical screen size or aspect ratio */
	parse_screen_size_or_aspect_ratio(dptx);
	/* Delete non 60 fps resolution in dptx_timing_list when using Multi-screen */
	if (dptx->filter_resolution_enable)
		dptx->delete_invalid_timing_node(dptx);

	print_edid(dptx->edid, dptx->dp_monitor, len);
	copy_edid_info_to_comp(dptx);
	dpu_pr_info("[DP] -");

	return 0;
}

int dptx_calc_fps(struct timing_info *dptx_timing_node, uint32_t *fps)
{
	uint32_t pixels;

	pixels = ((dptx_timing_node->hactive_pixels + dptx_timing_node->hblanking) *
		(dptx_timing_node->vactive_pixels + dptx_timing_node->vblanking));
	if (pixels == 0) {
		dpu_pr_info("[DP] pixels cannot be zero");
		return -EINVAL;
	}
	*fps = (uint32_t)(dptx_timing_node->pixel_clock * 10000 / pixels);
	dpu_pr_debug("[DP] dptx_calc_fps fps:%d, pixels:%d, pixel_clock:%lu", *fps, pixels, dptx_timing_node->pixel_clock);

	return 0;
}

static int check_main_edid(struct dp_ctrl *dptx, uint8_t *edid)
{
	uint32_t checksum = 0;
	int32_t i;

	dpu_check_and_return((edid == NULL), -EINVAL, err, "[DP] edid is NULL!\n");
	/* Verify 0 checksum */
	for (i = 0; i < EDID_LENGTH; i++)
		checksum += edid[i];
	if (checksum & 0xFF) {
		dpu_pr_err("[DP] EDID checksum failed - data is corrupt. Continuing anyway.\n");
		dp_add_error_log(dptx, "EDIDchecksum failed");
		return -EINVAL;
	}

	/* Verify Header */
	for (i = 0; i < EDID_HEADER_END + 1; i++) {
		if (edid[i] != edid_v1_header[i]) {
			dpu_pr_info("[DP] first uint8_ts don't match EDID version 1 header\n");
			return -EINVAL;
		}
	}

	return 0;
}

int parse_main(struct dp_ctrl *dptx)
{
	int16_t i;
	int ret;
	uint8_t *block = NULL;
	uint8_t *edid_t = NULL;
	struct edid_video *vid_info = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!dptx->edid, -EINVAL, err, "[DP] dptx->edid is NULL!\n");

	dptx->edid_info.video.dptx_timing_list = NULL;
	dptx->edid_info.video.ext_timing = NULL;
	dptx->edid_info.video.dp_monitor_descriptor = NULL;
	dptx->edid_info.video.ext_vcount = 0;
	edid_t = dptx->edid;
	/* Initialize some fields */
	vid_info = &(dptx->edid_info.video);
	vid_info->main_vcount = 0;
	vid_info->max_pixel_clock = 0;

	if (check_main_edid(dptx, edid_t)) {
		dpu_pr_err("[DP] The main edid block is wrong");
		dp_add_error_log(dptx, "main block is wrong");
		return -EINVAL;
	}

	if (dptx->hook_ops && dptx->hook_ops->save_dp_edid)
		dptx->hook_ops->save_dp_edid(edid_t, DPTX_DEFAULT_EDID_BUFLEN, (uint8_t)dptx->port_id);

	/* Check EDID version (usually 1.3) */
	dpu_pr_info("[DP] EDID version %d revision %d", (int)edid_t[EDID_STRUCT_VERSION],
		(int)edid_t[EDID_STRUCT_REVISION]);

	/* Check Display Image Size(Physical) */
	vid_info->max_himage_size = (uint16_t)edid_t[H_MAX_IMAGE_SIZE];
	vid_info->max_vimage_size = (uint16_t)edid_t[V_MAX_IMAGE_SIZE];
	/* Alloc the name memory */
	if (vid_info->dp_monitor_descriptor == NULL) {
		vid_info->dp_monitor_descriptor =
			(char *)kzalloc(MONTIOR_NAME_DESCRIPTION_SIZE * sizeof(char), GFP_KERNEL);
		if (vid_info->dp_monitor_descriptor == NULL) {
			dpu_pr_err("[DP] dp_monitor_descriptor memory alloc fail\n");
			return -EINVAL;
		}
	}
	memset(vid_info->dp_monitor_descriptor, 0, MONTIOR_NAME_DESCRIPTION_SIZE);
	/* Parse the EDID Detailed Timing Descriptor */
	block = edid_t + DETAILED_TIMING_DESCRIPTIONS_START;
	/* EDID main part has a total of four Descriptor Block */
	for (i = 0; i < DETAILED_TIMING_DESCRIPTION_COUNT; i++, block += DETAILED_TIMING_DESCRIPTION_SIZE) {
		switch (block_type(block)) {
		case DETAILED_TIMING_BLOCK:
			ret = parse_timing_description(dptx, block);
			if (ret != 0) {
				dpu_pr_info("[DP] Timing Description Parsing failed!\n");
				return ret;
			}
			break;
		case MONITOR_LIMITS:
			ret = parse_monitor_limits(dptx, block);
			if (ret != 0) {
				dpu_pr_info("[DP] Parsing the monitor limit is failed!\n");
				return ret;
			}
			break;
		case MONITOR_NAME:
			ret = parse_monitor_name(dptx, block, DETAILED_TIMING_DESCRIPTION_SIZE);
			if (ret != 0) {
				dpu_pr_err("[DP] The monitor name parsing failed.\n");
				return ret;
			}
			break;
		default:
			break;
		}
	}

	dpu_pr_info("[DP] Extensions to follow:\t%d\n", (int)edid_t[EXTENSION_FLAG]);
	/* Return the number of following extension blocks */
	return (int)edid_t[EXTENSION_FLAG];
}

static int parse_establish_timing_description(struct dp_ctrl *dptx, const struct dtd *edid_timing)
{
	struct edid_video *vid_info = NULL;
	struct timing_info *node = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!edid_timing, -EINVAL, err, "[DP] edid_timing is NULL!\n");

	dpu_pr_debug("[DP] parse_establish_timing_description start");
	vid_info = &(dptx->edid_info.video);

	if (vid_info->dptx_timing_list == NULL) {
		vid_info->dptx_timing_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
		if (vid_info->dptx_timing_list == NULL) {
			dpu_pr_err("[DP] vid_info->dptx_timing_list is NULL");
			return -EINVAL;
		}
		INIT_LIST_HEAD(vid_info->dptx_timing_list);

		vid_info->main_vcount = 0;
		/* Initial Max Value */
		vid_info->max_hpixels = edid_timing->h_active;
		vid_info->max_vpixels = edid_timing->v_active;
		vid_info->max_pixel_clock = edid_timing->pixel_clock / 10;
		dpu_pr_info("[DP] max_pixel_clock 1 is %lu", vid_info->max_pixel_clock);
	}

	/* Get Max Value by comparing all values */
	if ((vid_info->max_hpixels < edid_timing->h_active) && (vid_info->max_vpixels <= edid_timing->v_active) &&
		(vid_info->max_pixel_clock < edid_timing->pixel_clock / 10)) {
		vid_info->max_hpixels = edid_timing->h_active;
		vid_info->max_vpixels = edid_timing->v_active;
		vid_info->max_pixel_clock = edid_timing->pixel_clock / 10;
		dpu_pr_info("[DP] max_pixel_clock 2 is %lu", vid_info->max_pixel_clock);
	}

	node = kzalloc(sizeof(struct timing_info), GFP_KERNEL);
	if (node != NULL) {
		node->hactive_pixels = edid_timing->h_active;
		node->hblanking = edid_timing->h_blanking;
		node->hsync_offset = edid_timing->h_sync_offset;
		node->hsync_pulse_width = edid_timing->h_sync_pulse_width;
		node->hsize = edid_timing->h_image_size;

		node->vactive_pixels = edid_timing->v_active;
		node->vblanking = edid_timing->v_blanking;
		node->vsync_offset = edid_timing->v_sync_offset;
		node->vsync_pulse_width = edid_timing->v_sync_pulse_width;
		node->vsize = edid_timing->v_image_size;

		node->pixel_clock = edid_timing->pixel_clock / 10;
		node->interlaced = edid_timing->interlaced;
		node->hsync_polarity = edid_timing->h_sync_polarity;
		node->vsync_polarity = edid_timing->v_sync_polarity;

		vid_info->main_vcount += 1;
		list_add_tail(&node->list_node, vid_info->dptx_timing_list);
	} else {
		dpu_pr_err("[DP] kzalloc struct dptx_hdmi_vic fail!\n");
		return -EINVAL;
	}

	dpu_pr_info("[DP] The timinginfo %hhu: hactive_pixels is %hu, vactive_pixels is %hu, pixel clock=%llu\n",
		vid_info->main_vcount, node->hactive_pixels, node->vactive_pixels, node->pixel_clock);

	return 0;
}

int parse_established_timing(struct dp_ctrl *dptx)
{
	int16_t i;
	int count = 0;
	uint8_t *edid_t = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!dptx->edid, -EINVAL, err, "[DP] dptx->edid is NULL!\n");

	edid_t = dptx->edid;

	for (i = 0; i < 8; i++) {
		if (edid_t[ESTABLISHED_TIMINGS_START] & ESTABLISHED_TIMINGS(i)) {
			parse_establish_timing_description(dptx, &edid_estab_timing[i]);
			count++;
		}
	}

	for (i = 0; i < 8; i++) {
		if (edid_t[ESTABLISHED_TIMINGS_START+1] & ESTABLISHED_TIMINGS(i)) {
			parse_establish_timing_description(dptx, &edid_estab_timing[i+8]);
			count++;
		}
	}

	if (edid_t[ESTABLISHED_TIMINGS_START+2] & ESTABLISHED_TIMINGS(7)) {
		parse_establish_timing_description(dptx, &edid_estab_timing[16]);
		count++;
	}

	return count;
}

static bool get_dmt_mode_timing(struct dtd *edid_timing, uint16_t fps)
{
	int i;
	uint16_t dmt_fps;
	uint64_t pixels;

    for (i = 1; i < ARRAY_SIZE(dmt_modes_dtd); i++) {
		if (dmt_modes_dtd[i].interlaced != 0)
			continue;
        if (edid_timing->h_active != dmt_modes_dtd[i].h_active)
			continue;
        if (edid_timing->v_active != dmt_modes_dtd[i].v_active)
            continue;

		pixels = (uint64_t)((dmt_modes_dtd[i].h_active + dmt_modes_dtd[i].h_blanking) *
			(dmt_modes_dtd[i].v_active + dmt_modes_dtd[i].v_blanking));
		dmt_fps = (uint16_t)(dmt_modes_dtd[i].pixel_clock * 1000 / pixels);
		dpu_pr_debug("[DP] index:%d, cal_fps:%u, pixel_clock:%llu", i, dmt_fps, dmt_modes_dtd[i].pixel_clock);
		if ((dmt_fps < fps - 1) || (dmt_fps > fps))
            continue;

		edid_timing->h_active = dmt_modes_dtd[i].h_active;
        edid_timing->h_blanking = dmt_modes_dtd[i].h_blanking;
		edid_timing->h_sync_offset = dmt_modes_dtd[i].h_sync_offset;
		edid_timing->h_sync_pulse_width = dmt_modes_dtd[i].h_sync_pulse_width;
		edid_timing->h_sync_polarity = dmt_modes_dtd[i].h_sync_polarity;
		edid_timing->h_image_size = dmt_modes_dtd[i].h_image_size;

		edid_timing->v_active = dmt_modes_dtd[i].v_active;
        edid_timing->v_blanking = dmt_modes_dtd[i].v_blanking;
        edid_timing->v_sync_offset = dmt_modes_dtd[i].v_sync_offset;
		edid_timing->v_sync_pulse_width = dmt_modes_dtd[i].v_sync_pulse_width;
        edid_timing->v_sync_polarity = dmt_modes_dtd[i].v_sync_polarity;
		edid_timing->v_image_size = dmt_modes_dtd[i].v_image_size;

		edid_timing->interlaced = dmt_modes_dtd[i].interlaced;
		edid_timing->pixel_clock = dmt_modes_dtd[i].pixel_clock;

		dpu_pr_info("[DP] Get dmt mode[%d][%u * %u @ %uHz]", i, edid_timing->h_active, edid_timing->v_active, dmt_fps);
		return true;
	}

	return false;
}

static void get_cvt_rbv2_timing(struct dtd *edid_timing, uint16_t fps)
{
	uint16_t h_blank_addition = 200;
	uint32_t h_period = 0;
	uint64_t pixels = 0;

	/* estimate times per line(horizontal period us) */
	h_period = (uint32_t)((1000000 - CVT_RBV2_MIN_V_BLANK * fps) / (edid_timing->v_active * fps));
	/* find vertical blanking lines */
	edid_timing->v_blanking = (uint16_t)(CVT_RBV2_MIN_V_BLANK / h_period + 1);
	if (edid_timing->v_blanking < (CVT_RBV2_V_SYNC + CVT_RBV2_MIN_V_FRONT_PORCH + CVT_RBV2_MIN_V_BACK_PORCH))
		edid_timing->v_blanking = CVT_RBV2_V_SYNC + CVT_RBV2_MIN_V_FRONT_PORCH + CVT_RBV2_MIN_V_BACK_PORCH;
	edid_timing->v_sync_offset = CVT_RBV2_MIN_V_FRONT_PORCH;
	edid_timing->v_sync_pulse_width = CVT_RBV2_V_SYNC;
	edid_timing->v_sync_polarity = CVT_RBV2_V_POLARITY;
	edid_timing->h_blanking = CVT_RBV2_MIN_H_BLANK + h_blank_addition;
	edid_timing->h_sync_offset = CVT_RBV2_H_FRONT_PORCH;
	edid_timing->h_sync_pulse_width = CVT_RBV2_H_SYNC;
	edid_timing->h_sync_polarity = CVT_RBV2_H_POLARITY;
	dpu_pr_info("[DP] Get cvt mode hblank is %u, vblank is %u", edid_timing->v_blanking, edid_timing->h_blanking);
	pixels = (uint64_t)((edid_timing->h_active + edid_timing->h_blanking) *
			(edid_timing->v_active + edid_timing->v_blanking));
	edid_timing->pixel_clock = (uint64_t)fps * pixels / 1000;
	/* non-interlaced scan */
	edid_timing->interlaced = 0;
	dpu_pr_info("[DP] Get cvt mode[%u * %u @ %uHz]", edid_timing->h_active, edid_timing->v_active, fps);
}

static bool get_cvt_mode_timing(struct dtd *edid_timing, uint16_t fps, enum cvt_version cvt)
{
	bool ret = false;
	/*
	Standard CRT-based Timing and CVT Reduced Blanking Timing Version 1 are removed from CVT2.1,
	if cvt_rbv 1 is required, please refer to CVT1.2 standard and add corresponding formula to 
	this function. cvt_rbv 3 is used for adptive-sync operation, if cvt_rbv 3 is required, please
	refer to CVT2.1 standard and add corresponding formula to this function.
	*/
	switch (cvt) {
		case CVT_CRT:
			dpu_pr_info("[DP] CVT_CRT version is not required yet");
			break;
		case CVT_RBV1:
			dpu_pr_info("[DP] CVT_RBV1 version is not required yet");
			break;
		case CVT_RBV2:
			get_cvt_rbv2_timing(edid_timing, fps);
			ret = true;
			break;
		case CVT_RBV3:
			dpu_pr_info("[DP] CVT_RBV3 version is not required yet");
			break;
        default:
			dpu_pr_warn("[DP] Invalid CVT mode");
			return false;
	}

	return ret;
}

static bool get_gtf_mode_timing(struct edid_video *vid_info, struct dtd *edid_timing, uint16_t fps, enum gtf_version gtf)
{
	uint16_t gtf_m;
    uint16_t gtf_c;
    uint16_t gtf_k;
	uint16_t gtf_j;
	uint16_t gtf_c_prime;
	uint16_t gtf_m_prime;
	uint16_t total_hactive = 0;
	uint16_t total_hpixels = 0;
	uint16_t vback_porch = 0;
	uint16_t ideal_hb_duty_cycle = 0;
	uint32_t h_period = 0;
	uint64_t pixels = 0;

	if (gtf == GTF_DEFAULT) {
		gtf_m = GTF_DEFAULT_M;
		gtf_c = GTF_DEFAULT_C;
		gtf_k = GTF_DEFAULT_K;
        gtf_j = GTF_DEFAULT_J;
		edid_timing->h_sync_polarity = NEGATIVE;
		edid_timing->v_sync_polarity = POSITIVE;
    } else if (gtf == GTF_SECONDARY) {
		gtf_m = vid_info->gtf2_m;
		gtf_c = vid_info->gtf2_c / 2;
		gtf_k = vid_info->gtf2_k;
		gtf_j = vid_info->gtf2_j / 2;
		edid_timing->h_sync_polarity = POSITIVE;
		edid_timing->v_sync_polarity = NEGATIVE;
	} else {
		dpu_pr_warn("[DP] Invalid GTF mode");
		return false;
	}

	/* estimate times per line(horizontal period us) */
	h_period = (uint32_t)(1000000 - GTF_MIN_V_SYNC_PLUS_BP * fps) / ((edid_timing->v_active + GTF_MIN_V_FRONT_PORCH) * fps);
	/* find the number of lines of the vertical back porch */
	vback_porch = (uint16_t)(GTF_MIN_V_SYNC_PLUS_BP / h_period - GTF_V_SYNC);
	/* get vertical blanking parameters */
	edid_timing->v_blanking = GTF_MIN_V_FRONT_PORCH + GTF_V_SYNC + vback_porch;
	edid_timing->v_sync_offset = GTF_MIN_V_FRONT_PORCH;
	edid_timing->v_sync_pulse_width = GTF_V_SYNC;
	dpu_pr_info("[DP] v_blanking is %u, v_sync_offset is %u, v_sync_pulse_width is %u", edid_timing->v_blanking,
		edid_timing->v_sync_offset, edid_timing->v_sync_pulse_width);
	/*
	get the horizontal blanking duty cycle(%) which indicates the percentage of the horizontal blanking per line
	according to the Generalized Timing Formula(GTF) standard.
	*/
	gtf_c_prime = (gtf_c - gtf_j) * gtf_k / 256 + gtf_j;
	gtf_m_prime = gtf_k * gtf_m / 256;
	ideal_hb_duty_cycle = (uint16_t)(gtf_c_prime - gtf_m_prime * h_period / 1000);
	dpu_pr_info("[DP] [GTF_M*GTF_C*GTF_K*GTF_J]=[%u,%u,%u,%u], [GTF_M_PRIME*GTF_C_PRIME]=[%u,%u], ideal_hb_duty_cycle=%u",
		gtf_m, gtf_c, gtf_k, gtf_j, gtf_m_prime, gtf_c_prime, ideal_hb_duty_cycle);
	/*
	The number of horizontal pixels requested is first processed to ensure that it is divisible by the character size,
	by rounding it to the nearest character cell boundary. The assumed character cell granularity are 8 pixels.
	*/
	total_hactive = (edid_timing->h_active + GTF_CELL_GRAN / 2) / GTF_CELL_GRAN;
	total_hactive = total_hactive * GTF_CELL_GRAN;
	/* get the horizontal blanking(in pixels) */
	edid_timing->h_blanking = total_hactive * ideal_hb_duty_cycle / (100 - ideal_hb_duty_cycle);
	edid_timing->h_blanking = (edid_timing->h_blanking + GTF_CELL_GRAN) / (2 * GTF_CELL_GRAN);
	edid_timing->h_blanking = edid_timing->h_blanking * 2 * GTF_CELL_GRAN;
	/* get total number of the horizontal pixels */
	total_hpixels = total_hactive + edid_timing->h_blanking;
	/* get the horizontal sync width(in pixels) */
	edid_timing->h_sync_pulse_width = total_hpixels * GTF_H_SYNC_PERCENT / 100;
	edid_timing->h_sync_pulse_width = (edid_timing->h_sync_pulse_width + GTF_CELL_GRAN / 2) / GTF_CELL_GRAN;
	edid_timing->h_sync_pulse_width = edid_timing->h_sync_pulse_width * GTF_CELL_GRAN;
	/* get the horizontal front porch(in pixels) */
	edid_timing->h_sync_offset = edid_timing->h_blanking / 2 - edid_timing->h_sync_pulse_width;
	dpu_pr_info("[DP] h_blanking is %u, h_sync_offset is %u, h_sync_pulse_width is %u",
		edid_timing->h_blanking, edid_timing->h_sync_offset, edid_timing->h_sync_pulse_width);
	/* get the pixel clock */
	pixels = (uint64_t)((edid_timing->h_active + edid_timing->h_blanking) *
			(edid_timing->v_active + edid_timing->v_blanking));
	edid_timing->pixel_clock = (uint64_t)fps * pixels / 1000;
	/* non-interlaced scan */
	edid_timing->interlaced = 0;
	dpu_pr_info("[DP] Get gtf mode[%u * %u @ %uHz]", edid_timing->h_active, edid_timing->v_active, fps);

	return true;
}

static int parse_standard_timing_description(struct edid_video *vid_info, struct dtd *edid_timing, uint16_t fps, uint8_t mode)
{
	int ret = 0;
	dpu_pr_info("[DP] parse_standard_timing_description start");

	if (get_dmt_mode_timing(edid_timing, fps))
		return ret;

	switch (mode) {
		case VESA_DMT_MODE:
		case VESA_CVT_MODE:
			if (!get_cvt_mode_timing(edid_timing, fps, CVT_RBV2))
				ret = -EINVAL;
			break;
		case VESA_GTF_MODE:
			if (!get_gtf_mode_timing(vid_info, edid_timing, fps, GTF_DEFAULT))
				ret = -EINVAL;
            break;
		case VESA_GTF2_MODE:
            if (!get_gtf_mode_timing(vid_info, edid_timing, fps, GTF_SECONDARY))
				ret = -EINVAL;
            break;
		default:
			dpu_pr_warn("[DP] Invalid timing mode");
			return -EINVAL;
    }

	return ret;
}

static uint8_t get_standard_timing_mode(struct edid_video *vid_info, uint8_t *edid_t)
{
	vid_info->edid_feature_support = (edid_t[EDID_FEATURE_SUPPORT] & EDID_FEATURE_SUPPORT_TIMING_MODE);

	/*
	With EDID Structure version 1, revision 4, GTF has been Deprecated, (GTF is considered obsolete
	and in the process of being phased out) in favor of CVT. GTF has been retained in EDID Structure
	version 1, revision 4 for legacy support only and may be retired in a future release of the E-EDID
	Standard. VESA no longer recommends using GTF. However, GTF is probably still supported in some
	EDID v1.4 of manufacture's monitor, so GTF and GTF2 are also supported here in EDID v1.4. The priciple
	here that EDID v1.4 or higher version only support CVT may be applicated in the future.
	*/
	if (edid_t[EDID_STRUCT_VERSION] == 1 && edid_t[EDID_STRUCT_REVISION] >= 3 && vid_info->edid_feature_support != 0) {
		if (vid_info->video_timing_support_flag == DEFAULT_GTF_SUPPORTED)
			return VESA_GTF_MODE;
		if (vid_info->video_timing_support_flag == SECONDARY_GTF_SUPPORTED)
			return VESA_GTF2_MODE;
		if (edid_t[EDID_STRUCT_REVISION] >= 4)
			return VESA_CVT_MODE;
		if (edid_t[EDID_STRUCT_REVISION] == 3)
			return VESA_GTF_MODE;
	}

	return VESA_DMT_MODE;
}

int parse_standard_timing(struct dp_ctrl *dptx)
{
	int ret = 0;
	int i = 0;
	int count = 0;
	uint8_t *edid_t = NULL;
	uint8_t t_mode = 0;
	uint8_t hdisplay_size = 0;
	uint8_t freq_aspect = 0;
	uint16_t h_active = 0;
	uint16_t image_fps = 0;
	struct dtd standard_timing = {0};
	struct edid_video *vid_info = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!");
	dpu_check_and_return(!dptx->edid, -EINVAL, err, "[DP] dptx->edid is NULL!");
	
	vid_info = &(dptx->edid_info.video);
	edid_t = dptx->edid;
	if (check_main_edid(dptx, edid_t)) {
		dpu_pr_err("[DP] The main edid block is wrong");
		return -EINVAL;
	}

	t_mode = get_standard_timing_mode(vid_info, edid_t);
	dpu_pr_info("[DP] First get standard timing mode is %u", t_mode);

	for (i = 0; i <= 14; i += 2) {
		hdisplay_size = edid_t[STANDARD_TIMINGS_START + i];
		freq_aspect = edid_t[STANDARD_TIMINGS_START + i + 1];
		if (is_valid_standard_timing(hdisplay_size, freq_aspect)) {
			if (IMAGE_ASPECT_RATIO_INDEX(freq_aspect) == 0 && edid_t[EDID_STRUCT_REVISION] < 3) {
				standard_timing.h_active = standard_timing.v_active = (hdisplay_size + 31) * 8;
				standard_timing.h_image_size = standard_timing.v_image_size = 1;
			} else {
				standard_timing.h_active = h_active = (uint16_t)(hdisplay_size + 31) * 8;
				standard_timing.v_active = h_active * IMAGE_V_RATIO(freq_aspect) / IMAGE_H_RATIO(freq_aspect);
				standard_timing.h_image_size = (uint16_t)IMAGE_H_RATIO(freq_aspect);
				standard_timing.v_image_size = (uint16_t)IMAGE_V_RATIO(freq_aspect);
			}
			image_fps = (uint16_t)(EDID_BASE_FPS + REFRESH_RATE(freq_aspect));
			/* The range of fps in EDID standard timing is from 60Hz to 123Hz */
			if (image_fps > 123) {
				dpu_pr_err("[DP] invalid fps, igonre this timing");
				continue;
			}
			ret = parse_standard_timing_description(vid_info, &standard_timing, image_fps, t_mode);
			if (ret == 0) {
				add_timing_node(vid_info, &standard_timing);
				count++;
			}
		}
	}

	return count;
}

static int check_exten_edid(uint8_t *exten)
{
	uint32_t i, checksum;

	checksum = 0;

	dpu_check_and_return(!exten, -EINVAL, err, "[DP] exten is NULL!\n");

	for (i = 0; i < EDID_LENGTH; i++)
		checksum += exten[i];
	if (checksum & 0xFF) {
		dpu_pr_err("[DP] Extension Data checksum failed - data is corrupt. Continuing anyway.\n");
		return -EINVAL;
	}
	/* Check Extension Tag */
	/* ( Header Tag stored in the first uint8_t ) */
	if (exten[0] != EXTENSION_HEADER && exten[0] != EXTENSION_HEADER_TIMING) {
		dpu_pr_warn("[DP] Not CEA-EDID Timing Extension, Extension-Parsing will not continue!\n");
		return -EINVAL;
	}
	return 0;
}

static int parse_extension_timing_description(struct dp_ctrl *dptx, uint8_t *dtd_block, uint32_t dtd_begin,
	uint16_t dtd_total)
{
	uint32_t i;
	int ret;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!dtd_block, -EINVAL, err, "[DP] dtd_block is NULL!\n");

	if ((dtd_begin + 1 + dtd_total * DETAILED_TIMING_DESCRIPTION_SIZE) > EDID_LENGTH) {
		dpu_pr_err("[DP] The dtd total number 0x%x is out of the limit\n", dtd_total);
		return -EINVAL;
	}

	for (i = 0; i < (uint32_t)dtd_total; i++, dtd_block += DETAILED_TIMING_DESCRIPTION_SIZE) {
		switch (block_type(dtd_block)) {
		case DETAILED_TIMING_BLOCK:
			ret = parse_timing_description(dptx, dtd_block);
			if (ret != 0) {
				dpu_pr_err("[DP] Timing Description Parsing failed!");
				return ret;
			}
			break;
		case MONITOR_LIMITS:
			parse_monitor_limits(dptx, dtd_block);
			break;
		default:
			break;
		}
	}
	return 0;
}

int parse_extension(struct dp_ctrl *dptx, uint8_t *exten)
{
	int ret;
    uint8_t *dtd_block = NULL;
    uint8_t *cea_block = NULL;
	uint8_t dtd_start_byte = 0;
	uint8_t cea_data_block_collection = 0;
	uint16_t dtd_total = 0;
	struct edid_audio *aud_info = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!exten, -EINVAL, err, "[DP] exten is NULL!\n");

	aud_info = &(dptx->edid_info.audio);

	ret = check_exten_edid(exten);
	if (ret) {
		dpu_pr_warn("[DP] The check_exten_edid failed.\n");
		return ret;
	}
	/*
	 * Get uint8_t number (decimal) within this block where the 18-uint8_t DTDs begin.
	 * ( Number data stored in the third uint8_t )
	 */

	if (exten[2] == 0x00) {
		dpu_pr_info("[DP] There are no DTDs present in this extension block and no non-DTD data.\n");
		return -EINVAL;
	} else if (exten[2] == 0x04) {
		dtd_start_byte = 0x04;
	} else {
		cea_data_block_collection = 0x04;
		dtd_start_byte = exten[2];
	}
	/*
	 * Get Number of Native DTDs present, other Version 2+ information
	 * DTD Total stored in the fourth uint8_t )
	 */
	if (aud_info->basic_audio != 1)
		aud_info->basic_audio = (0x40 & exten[3]) >> 6;
	dp_imonitor_set_param(DP_PARAM_BASIC_AUDIO, &(aud_info->basic_audio));

	dtd_total = lower_nibble(exten[3]);
	// Parse DTD in Extension
	dtd_block = exten + dtd_start_byte;
	if (dtd_total != (EDID_LENGTH - dtd_start_byte - 1) / DETAILED_TIMING_DESCRIPTION_SIZE) {
		dpu_pr_info("[DP] The number of native DTDs is not equal the size\n");
		dtd_total = (EDID_LENGTH - dtd_start_byte - 1) / DETAILED_TIMING_DESCRIPTION_SIZE;
	}

	ret = parse_extension_timing_description(dptx, dtd_block, dtd_start_byte, dtd_total);
	if (ret) {
		dpu_pr_err("[DP] Parse the extension block timing information fail.\n");
		return ret;
	}

    /* Parse CEA Data Block Collection */
	if (cea_data_block_collection == 0x04) {
		cea_block = exten + cea_data_block_collection;
		ret = parse_cea_data_block(dptx, cea_block, dtd_start_byte);
		if (unlikely(ret != 0)) {
			dpu_pr_err("[DP] CEA data block Parsing failed!");
			return ret;
		}
    }
    
	return 0;
}
static int check_exten_edid_v21(const uint8_t *exten)
{
	uint16_t dtd_total = 0;
	uint32_t checksum = 0;
	uint32_t i = 0;

	if (exten[1] != 0x20) {
		dpu_pr_info("[DP] EDID is not V 2.0");
		return -EINVAL;
	} else if (exten[3] != 0x02) {
		dpu_pr_info("[DP] EDID is not Generic display");
		return -EINVAL;
	}

	dtd_total = exten[2];
	if (dtd_total != EDID_LENGTH - 2 - 5) {
		dpu_pr_err("[DP] dtd_total err, dtd_total: %hu", dtd_total);
		return -EINVAL;
	}

	for (i = 5; i < EDID_LENGTH - 1; i++)
		checksum += exten[i];
	if (checksum & 0xFF)
		dpu_pr_info("[DP] dtd_total: %d, checksum: %d", dtd_total, checksum);

	return 0;
}

int parse_extension_v21(struct dp_ctrl *dptx, uint8_t *exten)
{
	int ret;
	uint8_t dts_data_block_position = 0;
	uint16_t dtd_total = 0;

	dpu_pr_info("[DP] +");

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!");
	dpu_check_and_return(!exten, -EINVAL, err, "[DP] exten is NULL!");

	ret = check_exten_edid(exten);
	if (ret) {
		dpu_pr_warn("[DP] The check_exten_edid failed");
		return ret;
	}
	/*
	 * Get uint8_t number (decimal) within this block where the 18-uint8_t DTDs begin.
	 * ( Number data stored in the third uint8_t )
	 */
	ret = check_exten_edid_v21(exten);
	if (ret) {
		dpu_pr_warn("[DP] The check_exten_edid_v21 failed");
		return ret;
	}

	dts_data_block_position = 5;
	dtd_total = exten[2];

	while (dts_data_block_position < dtd_total + 5 - 2 && exten[dts_data_block_position] != 0) {
		switch (exten[dts_data_block_position]) {
		case 0x22:
			dpu_pr_info("[DP] parse start 0x%x", dts_data_block_position);
			parse_timing_description_v21(dptx, exten + dts_data_block_position);
			break;
		default:
			dpu_pr_warn("[DP] jump of 0x%x", exten[dts_data_block_position]);
			break;
		}
		if (exten[dts_data_block_position + 2] == 0x0) {
			dpu_pr_err("[DP] dts_data_block_position is end, P: %u", dts_data_block_position);
			break;
		}
		dpu_pr_info("[DP] continue dts_data_block_position: %u, size: 0x%x", dts_data_block_position,
			exten[dts_data_block_position + 2]);
		dts_data_block_position = dts_data_block_position + exten[dts_data_block_position + 2] + 3;
	}

	dpu_pr_info("[DP] -");
	return 0;
}

static void parse_priority_timing_description(struct edid_video *vid_info, struct timing_info *node,
	bool is_native)
{
	/* Add only the first resolution or the resolution marked as native. */
	if ((vid_info->main_vcount == 1 || is_native) && vid_info->pri_vcount < PRIORITY_TIMING_MAX) {
		vid_info->pri_timing[vid_info->pri_vcount].pri_hpixels = node->hactive_pixels;
		vid_info->pri_timing[vid_info->pri_vcount].pri_vpixels = node->vactive_pixels;
		dpu_pr_info("[DP] add priority timing pri_hpixels:%u, pri_vpixels:%u, pri_vcount:%u\n",
			vid_info->pri_timing[vid_info->pri_vcount].pri_hpixels,
				vid_info->pri_timing[vid_info->pri_vcount].pri_vpixels,
					vid_info->pri_vcount);
		vid_info->pri_vcount++;
	} else {
		dpu_pr_info("[DP] Not add priority timing main_vcount:%u, is_native:%d, pri_vcount:%u\n",
			vid_info->main_vcount, is_native, vid_info->pri_vcount);
	}
}

/* lint -e429 */
int parse_timing_description(struct dp_ctrl *dptx, uint8_t *dtd)
{
	struct edid_video *vid_info = NULL;
	struct timing_info *node = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!dtd, -EINVAL, err, "[DP] dtd is NULL!\n");

	dpu_pr_info("[DP] parse_timing_description start");
	vid_info = &(dptx->edid_info.video);

	if (vid_info->dptx_timing_list == NULL) {
		vid_info->dptx_timing_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
		if (vid_info->dptx_timing_list == NULL) {
			dpu_pr_err("[DP] vid_info->dptx_timing_list is NULL");
			return -EINVAL;
		}
		INIT_LIST_HEAD(vid_info->dptx_timing_list);

		vid_info->main_vcount = 0;
		/* Initial Max Value */
		vid_info->max_hpixels = H_ACTIVE;
		vid_info->max_vpixels = V_ACTIVE;
		vid_info->max_pixel_clock = PIXEL_CLOCK;
		dpu_pr_info("[DP] max_pixel_clock 1 is %d", vid_info->max_pixel_clock);
	}

	/* Get Max Value by comparing all values */
	if ((vid_info->max_hpixels < H_ACTIVE) && (vid_info->max_vpixels <= V_ACTIVE) &&
		(vid_info->max_pixel_clock < PIXEL_CLOCK)) {
		vid_info->max_hpixels = H_ACTIVE;
		vid_info->max_vpixels = V_ACTIVE;
		vid_info->max_pixel_clock = PIXEL_CLOCK;
		dpu_pr_info("[DP] max_pixel_clock 2 is %d", vid_info->max_pixel_clock);
	}

	node = kzalloc(sizeof(struct timing_info), GFP_KERNEL);
	if (node != NULL) {
		node->hactive_pixels = H_ACTIVE;
		node->hblanking = H_BLANKING;
		node->hsync_offset = H_SYNC_OFFSET;
		node->hsync_pulse_width = H_SYNC_WIDTH;
		node->hborder = H_BORDER;
		node->hsize = H_SIZE;

		node->vactive_pixels = V_ACTIVE;
		node->vblanking = V_BLANKING;
		node->vsync_offset = V_SYNC_OFFSET;
		node->vsync_pulse_width = V_SYNC_WIDTH;
		node->vborder = V_BORDER;
		node->vsize = V_SIZE;

		node->pixel_clock = PIXEL_CLOCK;

		node->input_type = INPUT_TYPE; // need to modify later
		node->interlaced = VSCAN_MODE;
		node->vsync_polarity = V_SYNC_POLARITY;
		node->hsync_polarity = H_SYNC_POLARITY;
		node->sync_scheme = SYNC_SCHEME;
		node->scheme_detail = SCHEME_DETAIL;

		vid_info->main_vcount += 1;
		list_add_tail(&node->list_node, vid_info->dptx_timing_list);
		parse_priority_timing_description(vid_info, node, false);
	} else {
		dpu_pr_err("[DP] kzalloc struct dptx_hdmi_vic fail!\n");
		return -EINVAL;
	}

	dpu_pr_info("[DP] The timinginfo %hhu: hactive_pixels is %hu, vactive_pixels is %hu, pixel clock=%llu\n",
		vid_info->main_vcount, node->hactive_pixels, node->vactive_pixels, node->pixel_clock);

	return 0;
}

int parse_timing_description_v21(struct dp_ctrl *dptx, uint8_t *dtd)
{
	struct edid_video *vid_info = NULL;
	struct timing_info *node = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!");
	dpu_check_and_return(!dtd, -EINVAL, err, "[DP] dtd is NULL!");

	dpu_pr_info("[DP] parse_timing_description_v21 start");
	vid_info = &(dptx->edid_info.video);

	if (vid_info->dptx_timing_list == NULL) {
		vid_info->dptx_timing_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
		if (vid_info->dptx_timing_list == NULL) {
			dpu_pr_err("[DP] vid_info->dptx_timing_list is NULL");
			return -EINVAL;
		}
		INIT_LIST_HEAD(vid_info->dptx_timing_list);

		vid_info->main_vcount = 0;
		/* Initial Max Value */
		vid_info->max_hpixels = H_ACTIVE_V21;
		vid_info->max_vpixels = V_ACTIVE_V21;
		vid_info->max_pixel_clock = PIXEL_CLOCK_V21;
		dpu_pr_info("[DP] max_pixel_clock 1 is %llu", vid_info->max_pixel_clock);
	}

	/* Get Max Value by comparing all values */
	if ((vid_info->max_hpixels < H_ACTIVE_V21) && (vid_info->max_vpixels <= V_ACTIVE_V21) &&
		(vid_info->max_pixel_clock < PIXEL_CLOCK_V21)) {
		vid_info->max_hpixels = H_ACTIVE_V21;
		vid_info->max_vpixels = V_ACTIVE_V21;
		vid_info->max_pixel_clock = PIXEL_CLOCK_V21;
		dpu_pr_info("[DP] max_pixel_clock 2 is %d", vid_info->max_pixel_clock);
	}

	node = kzalloc(sizeof(struct timing_info), GFP_KERNEL);
	if (node != NULL) {
		node->hactive_pixels = H_ACTIVE_V21;
		node->hblanking = H_BLANKING_V21;
		node->hsync_offset = H_SYNC_OFFSET_V21;
		node->hsync_pulse_width = H_SYNC_WIDTH_V21;

		node->vactive_pixels = V_ACTIVE_V21;
		node->vblanking = V_BLANKING_V21;
		node->vsync_offset = V_SYNC_OFFSET_V21;
		node->vsync_pulse_width = V_SYNC_WIDTH_V21;

		node->pixel_clock = PIXEL_CLOCK_V21;

		vid_info->main_vcount += 1;
		list_add_tail(&node->list_node, vid_info->dptx_timing_list);
		parse_priority_timing_description(vid_info, node, false);
	} else {
		dpu_pr_err("[DP] kzalloc struct dptx_hdmi_vic fail!");
		return -EINVAL;
	}

	dpu_pr_info("[DP] The timinginfo %u: hactive_pixels is %hu, vactive_pixels is %hu, pixel clock=%llu",
		vid_info->main_vcount, node->hactive_pixels, node->vactive_pixels, node->pixel_clock);

	return 0;
}

static int parse_timing_description_by_vesaid(struct edid_video *vid_info, uint8_t vesa_id,
	bool is_native)
{
	struct dtd mdtd;
	struct timing_info *node = NULL;

	dpu_check_and_return(!vid_info, -EINVAL, err, "[DP] vid_info is NULL!");

	dpu_pr_info("[DP] video mode value %d",vesa_id);
	if (!convert_code_to_dtd(&mdtd, vesa_id, 60000, VCEA)) {
		dpu_pr_info("[DP] Invalid video mode value %u", vesa_id);
		return -EINVAL;
	}

	if (mdtd.interlaced) {
		dpu_pr_info("[DP] Don't Support interlace mode %d\n",
						vesa_id);
		return -EINVAL;
	}

	if (vid_info->dptx_timing_list == NULL) {
		vid_info->dptx_timing_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
		if (vid_info->dptx_timing_list == NULL) {
			dpu_pr_err("[DP] vid_info->dptx_timing_list is NULL");
			return -EINVAL;
		}
		INIT_LIST_HEAD(vid_info->dptx_timing_list);

		vid_info->main_vcount = 0;
		/* Initial Max Value */
		vid_info->max_hpixels = mdtd.h_active;
		vid_info->max_vpixels = mdtd.v_active;
		vid_info->max_pixel_clock = mdtd.pixel_clock / 10;
	}

	/* Get Max Value by comparing all values */
	if ((mdtd.h_active > vid_info->max_hpixels) && (mdtd.v_active >= vid_info->max_vpixels) &&
		((mdtd.pixel_clock / 10) > vid_info->max_pixel_clock)) {
		vid_info->max_hpixels = mdtd.h_active;
		vid_info->max_vpixels = mdtd.v_active;
		vid_info->max_pixel_clock = mdtd.pixel_clock / 10;
	}

	node = kzalloc(sizeof(struct timing_info), GFP_KERNEL);
	if (node != NULL) {
		node->hactive_pixels = mdtd.h_active;
		node->hblanking = mdtd.h_blanking;
		node->hsync_offset = mdtd.h_sync_offset;
		node->hsync_pulse_width = mdtd.h_sync_pulse_width;
		node->hsize = mdtd.h_image_size;

		node->vactive_pixels = mdtd.v_active;
		node->vblanking = mdtd.v_blanking;
		node->vsync_offset = mdtd.v_sync_offset;
		node->vsync_pulse_width = mdtd.v_sync_pulse_width;
		node->vsize = mdtd.v_image_size;

		node->pixel_clock = mdtd.pixel_clock / 10; // Edid pixel clock is 1/10 of dtd filled timing.
		node->interlaced = mdtd.interlaced;
		node->vsync_polarity = mdtd.v_sync_polarity;
		node->hsync_polarity = mdtd.h_sync_polarity;

		vid_info->main_vcount += 1;
		list_add_tail(&node->list_node, vid_info->dptx_timing_list);
		parse_priority_timing_description(vid_info, node, is_native);
	} else {
		dpu_pr_err("[DP] kzalloc struct dptx_hdmi_vic fail!\n");
		return -EINVAL;
	}

	dpu_pr_info("[DP] The timinginfo %d: hactive_pixels is %d, vactive_pixels is %d, pixel clock=%llu\n",
		vid_info->main_vcount, node->hactive_pixels, node->vactive_pixels, node->pixel_clock);

	return 0;
}

static int parse_hdmi_vic_id(uint8_t vic_id)
{
	struct dptx_hdmi_vic *node = NULL;

	if (g_hdmi_vic_real_len >= g_hdmi_vic_len) {
		dpu_pr_err("[DP] The g_hdmi_vic_real_len is more than g_hdmi_vic_len.\n");
		return -EINVAL;
	}

	if (dptx_hdmi_list == NULL) {
		dptx_hdmi_list = kzalloc(sizeof(struct list_head), GFP_KERNEL);
		if (dptx_hdmi_list == NULL) {
			dpu_pr_err("[DP] dptx_hdmi_list is NULL");
			return -EINVAL;
		}
		INIT_LIST_HEAD(dptx_hdmi_list);
	}

	node = kzalloc(sizeof(struct dptx_hdmi_vic), GFP_KERNEL);
	if (node != NULL) {
		node->vic_id = vic_id;
		list_add_tail(&node->list_node, dptx_hdmi_list);
		g_hdmi_vic_real_len++;
	} else {
		dpu_pr_err("[DP] kzalloc struct dptx_hdmi_vic fail!\n");
	}

	return 0;
}
/*lint +e429*/
static int parse_audio_spec_info(struct edid_audio *aud_info, struct edid_audio_info *spec_info,
	uint8_t *cea_data_block)
{
	if ((EXTEN_AUDIO_FORMAT <= 8) && (EXTEN_AUDIO_FORMAT >= 1)) {
		dpu_check_and_return((!aud_info || !spec_info), -EINVAL, err, "[DP] aud_info or spec_info is NULL!\n");
		/* Set up SAD fields */
		spec_info->format = EXTEN_AUDIO_FORMAT;
		spec_info->channels =  EXTEN_AUDIO_MAX_CHANNELS;
		spec_info->sampling = EXTEN_AUDIO_SAMPLING;
		if (EXTEN_AUDIO_FORMAT == 1)
			spec_info->bitrate = EXTEN_AUDIO_LPCM_BIT;
		else
			spec_info->bitrate = EXTEN_AUDIO_BITRATE;
		aud_info->ext_acount += 1;

		dpu_pr_info(
			"[DP] parse audio spec success. format(0x%x), channels(0x%x), sampling(0x%x), bitrate(0x%x)\n",
			spec_info->format, spec_info->channels, spec_info->sampling, spec_info->bitrate);
	}

	return 0;
}

static int parse_extension_audio_tag(struct edid_audio *aud_info, uint8_t *cea_data_block, uint8_t temp_length)
{
	uint8_t i, xa;
	void *temp_ptr = NULL;

	dpu_check_and_return(!aud_info, -EINVAL, err, "[DP] aud_info is NULL!\n");
	dpu_check_and_return(!cea_data_block, -EINVAL, err, "[DP] cea_data_block is NULL!\n");
	dpu_check_and_return((temp_length < 1), -EINVAL, err, "[DP] The input param temp_length is wrong!\n");

	for (i = 0; i < (temp_length) / 3; i++) {
		xa = aud_info->ext_acount;

		if (xa == 0) {
			/* Initial audio part */
			if (aud_info->spec != NULL) {
				dpu_pr_err("[DP] The spec of audio is error.\n");
				return -EINVAL;
			}
			aud_info->spec = kzalloc(sizeof(struct edid_audio_info), GFP_KERNEL);
			if (aud_info->spec == NULL) {
				dpu_pr_err("[DP] malloc audio Spec part failed!\n");
				return -EINVAL;
			}
		} else {
			/* Add memory as needed with error handling */
			temp_ptr = kzalloc((xa + 1) * sizeof(struct edid_audio_info), GFP_KERNEL);
			if (temp_ptr == NULL) {
				dpu_pr_err("[DP] Realloc audio Spec part failed!\n");
				return -EINVAL;
			}
			if (aud_info->spec == NULL) {
				dpu_pr_err("[DP] The spec is NULL.\n");
				kfree(temp_ptr);
				temp_ptr = NULL;
				return -EINVAL;
			}
			memcpy(temp_ptr, aud_info->spec, xa * sizeof(struct edid_audio_info));
			kfree(aud_info->spec);
			aud_info->spec = NULL;
			aud_info->spec = temp_ptr;
		}
		if (parse_audio_spec_info(aud_info, &(aud_info->spec[xa]), cea_data_block)) {
			dpu_pr_err("[DP] parse the audio spec info fail.\n");
			return -EINVAL;
		}

		cea_data_block += 3;
	}
	return 0;
}

static int parse_extension_video_tag(struct edid_video *vid_info, uint8_t *cea_data_block, uint8_t length)
{
	uint8_t i;

	dpu_check_and_return(!vid_info, -EINVAL, err, "[DP] vid_info is NULL!\n");
	dpu_check_and_return(!cea_data_block, -EINVAL, err, "[DP] cea_data_block is NULL!\n");
	dpu_check_and_return((length < 1), -EINVAL, err, "[DP] The input param length is wrong!\n");

	vid_info->ext_timing = kzalloc(length * sizeof(struct ext_timing), GFP_KERNEL);
	if (vid_info->ext_timing == NULL) {
		dpu_pr_err("[DP] ext_timing memory alloc fail\n");
		return -EINVAL;
	}
	memset(vid_info->ext_timing, 0x0, length * sizeof(struct ext_timing));
	vid_info->ext_vcount = 0;

	for (i = 0; i < length; i++) {
		if (EXTEN_VIDEO_CODE != 0) {
			/* Set up SVD fields */
			vid_info->ext_timing[i].ext_format_code = EXTEN_VIDEO_CODE;
			vid_info->ext_vcount += 1;
			if (EXTEN_VIDEO_NATIVE == 1)
				parse_timing_description_by_vesaid(vid_info, vid_info->ext_timing[i].ext_format_code, true);
			else
				parse_timing_description_by_vesaid(vid_info, vid_info->ext_timing[i].ext_format_code, false);
		}
		cea_data_block += 1;
	}
	return 0;
}

static void parse_extension_vsdb_after_latency(uint8_t *cea_data_block, uint8_t length,
	uint8_t pos_after_latency)
{
	uint8_t i;
	bool b3dpresent = false;

	dpu_check_and_no_retval(!cea_data_block, err, "[DP] cea_data_block is NULL!\n");

	for (i = pos_after_latency; i < length; i++) {
		if (i == pos_after_latency) {
			b3dpresent = (cea_data_block[i] & 0x80) >> 7;
			dpu_pr_info("[DP] b3dpresent= %d \n", (uint32_t)b3dpresent);
		} else if (i == pos_after_latency + 1) {
			g_hdmi_vic_len = (cea_data_block[i] & 0xE0) >> 5;
			if (g_hdmi_vic_len == 0) {
				dpu_pr_info("[DP] This EDID don't include HDMI additional video format (2).\n");
				return;
			}
			g_hdmi_vic_real_len = 0;
		} else if (i <= pos_after_latency + 1 + g_hdmi_vic_len) {
			parse_hdmi_vic_id(cea_data_block[i]);
		} else {
			return;
		}
	}
}

static void parse_hdmi_vsdb_timing_list(struct edid_video *vid_info)
{

	dpu_check_and_no_retval(!vid_info, err, "[DP] vid_info is NULL!\n");
	dpu_check_and_no_retval(!dptx_hdmi_list, err, "[DP] dptx_hdmi_list is NULL!\n");

	kfree(dptx_hdmi_list);
	dptx_hdmi_list = NULL;
}

static int parse_extension_vsdb_tag(struct edid_video *vid_info, uint8_t *cea_data_block, uint8_t length)
{
	uint8_t i;
	uint32_t ieee_flag;
	uint8_t max_tmds_clock;
	uint8_t latency_fields;
	uint8_t interlaced_latency_fields;
	uint8_t hdmi_video_present;
	bool support_ai = false;

	g_hdmi_vic_real_len = 0;
	g_hdmi_vic_len = 0;
	latency_fields = 0;
	interlaced_latency_fields = 0;

	dpu_check_and_return(!vid_info, -EINVAL, err, "[DP] vid_info is NULL!\n");
	dpu_check_and_return(!cea_data_block, -EINVAL, err, "[DP] cea_data_block is NULL!\n");
	dpu_check_and_return((length < 5), -EINVAL, err, "[DP] VSDB length isn't correct!\n");

	ieee_flag = (cea_data_block[0]) | (cea_data_block[1] << 8) | (cea_data_block[2] << 16);

	if (ieee_flag != 0x000c03) {
		dpu_pr_info("[DP] This block isn't belong to HDMI block: %x.\n", ieee_flag);
		return 0;
	}

	for (i = 5; i < min(length, (uint8_t)8); i++) {
		switch (i) {
		case 5:
			support_ai = (cea_data_block[i] & 0x80) >> 7;
			dpu_pr_info("[DP] support_ai= %u \n", (uint32_t)support_ai);
			break;
		case 6:
			max_tmds_clock = cea_data_block[i];
			dpu_pr_info("[DP] max_tmds_clock= %u \n", (uint32_t)max_tmds_clock);
			break;
		case 7:
			latency_fields = (cea_data_block[i] & 0x80) >> 7;
			interlaced_latency_fields = (cea_data_block[i] & 0x40) >> 6;
			hdmi_video_present = (cea_data_block[i] & 0x20) >> 5;
			if (hdmi_video_present == 0) {
				dpu_pr_info("[DP] This EDID don't include HDMI additional video format (1).\n");
				return 0;
			}
			break;
		default:
			break;
		}
	}

	if (2 * LATENCY_PRESENT + 8 < length)
		parse_extension_vsdb_after_latency(cea_data_block, length, 2 * LATENCY_PRESENT + 8);

	if (g_hdmi_vic_len == 0)
		return 0;
	dpu_pr_info("[DP] vic_id real length =%u , vic length=%u !\n", g_hdmi_vic_real_len, g_hdmi_vic_len);

	parse_hdmi_vsdb_timing_list(vid_info);

	return 0;
}

int parse_cea_data_block(struct dp_ctrl *dptx, uint8_t *cea_data, uint8_t dtd_start)
{
	uint8_t total_length, block_length;
	uint8_t *cea_data_block = cea_data;
	struct edid_video *vid_info = NULL;
	struct edid_audio *aud_info = NULL;
	/* exTlist *extlist; */
	/* Initialize some fields */

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!cea_data, -EINVAL, err, "[DP] cea_data is NULL!\n");
	dpu_check_and_return((dtd_start > (EDID_LENGTH - 1)), -EINVAL, err,
		"[DP] The start of dtd is out of limit!\n");

	total_length = 4;
	vid_info = &(dptx->edid_info.video);
	aud_info = &(dptx->edid_info.audio);
	/* Parse CEA Data Block Collection */
	while (total_length < dtd_start) {
		/* Get length(total number of following uint8_ts of a certain tag) */
		block_length = get_cea_data_block_len(cea_data_block);
		/* Get tag code */
		switch (get_cea_data_block_tag(cea_data_block)) {
		case EXTENSION_AUDIO_TAG:
			/* Block type tag combined with data length takes 1 uint8_t */
			cea_data_block += 1;
			/* Each Short audio Descriptor(SAD) takes 3 uint8_ts */
			if (parse_extension_audio_tag(aud_info, cea_data_block, block_length)) {
				dpu_pr_err("[DP] parse_extension_audio_tag fail.\n");
				return -EINVAL;
			}
			cea_data_block += block_length;
			break;
		case EXTENSION_VIDEO_TAG:
			/* Block type tag combined with data length takes 1 uint8_t */
			cea_data_block += 1;
			/* Each Short video Descriptor(SVD) takes 1 uint8_t */
			if (parse_extension_video_tag(vid_info, cea_data_block, block_length)) {
				dpu_pr_err("[DP] parse_extension_video_tag fail.\n");
				return -EINVAL;
			}
			cea_data_block += block_length;
			break;
		case EXTENSION_VENDOR_TAG:
			cea_data_block += 1;
			if (parse_extension_vsdb_tag(vid_info, cea_data_block, block_length)) {
				dpu_pr_err("[DP] parse_extension_vsdb_tag fail.\n");
				return -EINVAL;
			}
			cea_data_block += block_length;
			break;
		case EXTENSION_SPEAKER_TAG:
			/*
			 * If the extension has Speaker Allocation Data,
			 * there should only be one
			 */
			cea_data_block += 1;
			aud_info->ext_speaker = EXTEN_SPEAKER;
			/* Speaker Allocation Data takes 3 uint8_ts */
			cea_data_block += 3;
			break;
		default:
			cea_data_block += block_length + 1;
			break;
		}
		total_length = total_length + block_length + 1;
	}

	return 0;
}

int block_type(uint8_t *block)
{
	dpu_check_and_return(!block, -EINVAL, err, "[DP] block is NULL!\n");

	if ((block[0] == 0) && (block[1] == 0)) {
		/* Other descriptor */
		if ((block[2] != 0) || (block[4] != 0))
			return UNKNOWN_DESCRIPTOR;
		return block[3];
	}
	/* Detailed timing block */
	return DETAILED_TIMING_BLOCK;
}

int parse_monitor_limits(struct dp_ctrl *dptx, uint8_t *block)
{
	struct edid_video *vid_info = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!block, -EINVAL, err, "[DP] block is NULL!\n");

	vid_info = &(dptx->edid_info.video);
	/* Set up limit fields */
	vid_info->max_hfreq = H_MAX_RATE;
	vid_info->min_hfreq = H_MIN_RATE;
	vid_info->max_vfreq = V_MAX_RATE;
	vid_info->min_vfreq = V_MIN_RATE;
	vid_info->max_pixel_clock = (int)MAX_PIXEL_CLOCK;
	vid_info->video_timing_support_flag = VIDEO_TIMING_SUPPORT_FLAG;
	if (vid_info->video_timing_support_flag == SECONDARY_GTF_SUPPORTED) {
		dpu_pr_info("[DP] video_timing_support_flag is gtf2");
		vid_info->gtf2_start_freq = GTF2_START_FREQ;
		vid_info->gtf2_c = GTF2_C;
		vid_info->gtf2_m = GTF2_M;
		vid_info->gtf2_k = GTF2_K;
		vid_info->gtf2_j = GTF2_J;
	}

	dpu_pr_info("[DP] parse_monitor_limits max_pixel_clock is %llu", vid_info->max_pixel_clock);

	return 0;
}

/*
 * EDID Display Descriptors[7]
 * Bytes	Description
 * 0-1	0, indicates Display Descriptor (cf. Detailed Timing Descriptor).
 * 2	0, reserved
 * 3	Descriptor type. FA-FF currently defined. 00-0F reserved for vendors.
 * 4	0, reserved, except for Display Range Limits Descriptor.
 * 5-17	Defined by descriptor type. If text, code page 437 text,
 * terminated (if less than 13 bytes) with LF and padded with SP.
 */
int parse_monitor_name(struct dp_ctrl *dptx, uint8_t *blockname, uint32_t size)
{
	uint32_t i;
	uint32_t str_start = 5;
	uint32_t str_end = 0;
	uint8_t data_tmp = 0;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");
	dpu_check_and_return(!blockname, -EINVAL, err, "[DP] blockname is NULL!\n");
	dpu_check_and_return((size != DETAILED_TIMING_DESCRIPTION_SIZE), -EINVAL, err,
		"[DP] The length of monitor name is wrong!\n");

	for (i = str_start; i < (size - 1); i++) {
		if ((blockname[i] == 0x0A) && (blockname[i + 1] == 0x20)) {
			str_end = i;
			break;
		}
	}

	if (str_end == 0)
		str_end = size;

	if (str_end < str_start) {
		dpu_pr_err("[DP] The length of monitor name is wrong\n");
		return -EINVAL;
	}

	dptx->dp_monitor.product_name = blockname;
	dptx->dp_monitor.product_start = str_start;
	dptx->dp_monitor.product_end = str_end;

	if (((str_end - str_start) < MONTIOR_NAME_DESCRIPTION_SIZE) &&
		(dptx->edid_info.video.dp_monitor_descriptor != NULL)) {
		memcpy(dptx->edid_info.video.dp_monitor_descriptor, &(blockname[str_start]), (str_end - str_start));
		dptx->edid_info.video.dp_monitor_descriptor[str_end - str_start] = '\0';
	} else {
		dpu_pr_err("[DP] The length of monitor name is wrong\n");
		return -EINVAL;
	}

	return 0;
}

int release_edid_info(struct dp_ctrl *dptx)
{
	struct timing_info *dptx_timing_node = NULL;
	struct timing_info *_node_ = NULL;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");

	if (dptx->edid_info.video.dptx_timing_list != NULL) {
		list_for_each_entry_safe(dptx_timing_node, _node_, dptx->edid_info.video.dptx_timing_list, list_node) {
			list_del(&dptx_timing_node->list_node);
			kfree(dptx_timing_node);
		}

		kfree(dptx->edid_info.video.dptx_timing_list);
		dptx->edid_info.video.dptx_timing_list = NULL;
	}

	if (dptx->edid_info.video.ext_timing != NULL) {
		kfree(dptx->edid_info.video.ext_timing);
		dptx->edid_info.video.ext_timing = NULL;
	}

	if (dptx->edid_info.video.dp_monitor_descriptor != NULL) {
		kfree(dptx->edid_info.video.dp_monitor_descriptor);
		dptx->edid_info.video.dp_monitor_descriptor = NULL;
	}

	if (dptx->edid_info.audio.spec != NULL) {
		kfree(dptx->edid_info.audio.spec);
		dptx->edid_info.audio.spec = NULL;
	}

	memset(&(dptx->edid_info), 0, sizeof(struct edid_information));

	dpu_pr_info("[DP] release_edid_info need to read edid");
	atomic_set(&(dptx->edid_reads), NEED_READ_EDID);

	return 0;
}

struct vfp_data {
	uint32_t refresh_rate;
	uint16_t v_blanking;
	uint16_t v_sync_offset;
};

struct vary_vfp_data {
	uint8_t code;
	uint8_t valid_count;

	struct vfp_data data[2];
};

struct vary_vfp_data vary_vfp_tlb[] = {
	{  8, 1, {{59940, 22, 4}, {    0,  0, 0}} },
	{  9, 1, {{59940, 22, 4}, {    0,  0, 0}} },
	{ 12, 1, {{60054, 22, 4}, {    0,  0, 0}} },
	{ 13, 1, {{60054, 22, 4}, {    0,  0, 0}} },
	{ 23, 2, {{50080, 24, 2}, {49920, 25, 3}} },
	{ 24, 2, {{50080, 24, 2}, {49920, 25, 3}} },
	{ 27, 2, {{50080, 24, 2}, {49920, 25, 3}} },
	{ 28, 2, {{50080, 24, 2}, {49920, 25, 3}} }
};

static void vary_vfp(struct dtd *dtd_by_code, uint8_t code, uint32_t refresh_rate)
{
	uint8_t i, j;

	for (i = 0; i < (uint8_t)ARRAY_SIZE(vary_vfp_tlb); i++) {
		if (code == vary_vfp_tlb[i].code) {
			/* valid_count must be 1 or 2, so < ARRAYSIZE(vary_vfp_tlb[i].data) */
			for (j = 0; j < vary_vfp_tlb[i].valid_count; j++) {
				if (refresh_rate == vary_vfp_tlb[i].data[j].refresh_rate) {
					dtd_by_code->v_blanking = vary_vfp_tlb[i].data[j].v_blanking;
					dtd_by_code->v_sync_offset = vary_vfp_tlb[i].data[j].v_sync_offset;
					return;
				}
			}
			if (j > 0) {
				j--;
				dtd_by_code->v_blanking = vary_vfp_tlb[i].data[j].v_blanking + 1;
				dtd_by_code->v_sync_offset = vary_vfp_tlb[i].data[j].v_sync_offset + 1;
			}
		}
	}
}

bool convert_code_to_dtd(struct dtd *mdtd, uint8_t code, uint32_t refresh_rate, uint8_t video_format)
{
	struct dtd_info {
		uint8_t size;
		const struct dtd *support_modes_dtd;
	} support_dtd_info[] = {
		{ .size = (uint8_t)dtd_array_size(cea_modes_dtd), .support_modes_dtd = cea_modes_dtd },
		{ .size = (uint8_t)dtd_array_size(cvt_modes_dtd), .support_modes_dtd = cvt_modes_dtd },
		{ .size = (uint8_t)dtd_array_size(dmt_modes_dtd), .support_modes_dtd = dmt_modes_dtd }
	}, *handle_dtd = NULL;

	dpu_check_and_return(!mdtd, false, err, "[DP] mdtd is NULL!");
	memset(mdtd, 0, sizeof(*mdtd));

	if (video_format > DMT) {
		dpu_pr_info("[DP] video_format=%d is error", video_format);
		return false;
	}

	handle_dtd = &support_dtd_info[video_format];
	if (code == 0 || code >= handle_dtd->size) {
		dpu_pr_info("[DP] code error, code is %u", code);
		return false;
	}

	*mdtd = handle_dtd->support_modes_dtd[code];
	dpu_pr_info("[DP] convert_code_to_dtd, pixel_clock is %lu, code: %u, ha: %hu, va: %hu",
		mdtd->pixel_clock, code, mdtd->h_active, mdtd->v_active);
	if (mdtd->pixel_clock == 0) {
		dpu_pr_info("[DP] Empty Timing");
		return false;
	}

	/*
	 * For certain VICs the spec allows the vertical
	 * front porch to vary by one or two lines.
	 */
	vary_vfp(mdtd, code, refresh_rate);

	return true;
}

bool convert_code_to_dtd_by_user(struct dp_ctrl *dptx, struct dtd *mdtd, uint8_t *fps)
{
	uint32_t index = 0;
	struct dkmd_connector_info *pinfo = NULL;

	pinfo = dptx->connector->conn_info;
	index = pinfo->base.edid_using_timing_id;

	mdtd->h_active = pinfo->base.edid_timing[index].hactive;
	mdtd->h_blanking = pinfo->base.edid_timing[index].hblanking;
	mdtd->h_sync_offset = pinfo->base.edid_timing[index].hsync_offset;
	mdtd->h_sync_pulse_width = pinfo->base.edid_timing[index].hsync_pulse_width;
	mdtd->h_image_size = pinfo->base.edid_timing[index].hsize;
	mdtd->h_sync_polarity = pinfo->base.edid_timing[index].hsync_polarity;
	mdtd->v_active = pinfo->base.edid_timing[index].vactive;
	mdtd->v_blanking = pinfo->base.edid_timing[index].vblanking;
	mdtd->v_sync_offset = pinfo->base.edid_timing[index].vsync_offset;
	mdtd->v_sync_pulse_width = pinfo->base.edid_timing[index].vsync_pulse_width;
	mdtd->v_image_size = pinfo->base.edid_timing[index].vsize;
	mdtd->v_sync_polarity = pinfo->base.edid_timing[index].vsync_polarity;
	mdtd->pixel_clock = pinfo->base.edid_timing[index].pixel_clock * 10;
	mdtd->interlaced = pinfo->base.edid_timing[index].interlaced;
	mdtd->pixel_repetition_input = 0;

	dpu_pr_info("[DP] convert_code_to_dtd by user, pixel_clock:[%lu], fps:[%u], ha:[%hu], va[%hu]",
		mdtd->pixel_clock, *fps, mdtd->h_active, mdtd->v_active);

	return true;
}
