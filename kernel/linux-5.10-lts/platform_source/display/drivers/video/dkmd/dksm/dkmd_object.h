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

#ifndef _DKMD_OBJECT_H_
#define _DKMD_OBJECT_H_

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <dkmd_dpu.h>
#include "dkmd_log.h"
#include "dkmd_lcd_interface.h"
#include "ukmd_utils.h"

#ifndef is_bit_enable
#define is_bit_enable(val, i) (!!((val) & (1 << (i))))
#endif

#define HZ_1M   1000000
#define SIZE_1K 1024
#define FAST_UNLOCK_RET (-3)
#define DEFAULT_OPR_MASK 0xFFFFFFFFFFFFFFFF

#define SPLIT_SCREEN_MAX 4

#define SW_DVFS_FRM_RATE_LIMIT 120
#define EDID_DISPLAY_INFO_NUM 20

/* panel type list */
enum panel_type {
	PANEL_NO   =  BIT(0),      /* No Panel */
	PANEL_LCDC =  BIT(1),      /* internal LCDC type */
	PANEL_HDMI =  BIT(2),      /* HDMI TV */
	PANEL_MIPI_VIDEO = BIT(3), /* MIPI */
	PANEL_MIPI_CMD   = BIT(4), /* MIPI */
	PANEL_DUAL_MIPI_VIDEO  = BIT(5),  /* DUAL MIPI */
	PANEL_DUAL_MIPI_CMD    = BIT(6),  /* DUAL MIPI */
	PANEL_DP               = BIT(7),  /* DisplayPort */
	PANEL_MIPI2RGB         = BIT(8),  /* MIPI to RGB */
	PANEL_RGB2MIPI         = BIT(9),  /* RGB to MIPI */
	PANEL_OFFLINECOMPOSER  = BIT(10), /* offline composer */
	PANEL_WRITEBACK        = BIT(11), /* Wifi display */
	PANEL_EXTERNAL         = BIT(12), /* external panel on fold panel */
	PANEL_PRIMARY          = BIT(13), /* primary panel */
};

/* composer device type list */
#define DEV_NAME_FB0 "fb0"
#define DEV_NAME_OFFLINE "gfx_offline"
#define DEV_NAME_DP "gfx_dp"
#define DEV_NAME_HDMI "gfx_hdmi"

enum {
	PANEL_ID_PRIMARY,
	PANEL_ID_BUILTIN,
	PANEL_ID_MAX_NUM,
};

enum {
	PIPE_SW_PRE_ITFCH0 = 0,
	PIPE_SW_PRE_ITFCH1,
	PIPE_SW_PRE_ITFCH2,
	PIPE_SW_PRE_ITFCH3,
	PIPE_SW_PRE_ITFCH_MAX
};

enum VSYNC_IDLE_TYPE {
	VSYNC_IDLE_NONE = 0x0,
	VSYNC_IDLE_ISR_OFF = BIT(0),
	VSYNC_IDLE_MIPI_ULPS = BIT(1),
	VSYNC_IDLE_CLK_OFF = BIT(2),
	VSYNC_IDLE_VCC_OFF = BIT(3),
	VSYNC_IDLE_RESET_VOTE = BIT(4),
	VSYNC_IDLE_SHUT_DOWN = BIT(5),
	VSYNC_IDLE_PSR2_CMD_MODE = BIT(6)
};

enum DP_CONNECT_STATUS {
	DP_DISCONNECTED,
	DP_CONNECTED,
	DP_SHORTPLUG,
};

enum HDMITX_CONNECT_STATUS {
	HDMITX_DISCONNECTED,
	HDMITX_CONNECTED,
};

struct display_info {
	uint32_t xres;
	uint32_t yres;
};

struct edid_support_timing {
	uint64_t pixel_clock;
	uint16_t pixel_repetition_input;
	uint16_t hactive;
	uint16_t hblanking;
	uint16_t hsize;
	uint16_t hsync_offset;
	uint16_t hsync_pulse_width;
	uint16_t vactive;
	uint16_t vblanking;
	uint16_t vsize;
	uint16_t vsync_offset;
	uint16_t vsync_pulse_width;
	uint16_t interlaced;
	uint16_t fps;
	uint8_t hsync_polarity;
	uint8_t vsync_polarity;
};

/* Basic data structure, Shared access for all modules */
struct dkmd_object_info {
	/* for gfx device name, such as offline or dp */
	const char *name;
	const char *lcd_name;

	/* cmd or video mode, single or dual-mipi, online or offline */
	uint32_t type;

	/* used for framebuffer device regitser */
	uint32_t xres;
	uint32_t yres;
	uint32_t width; /* mm */
	uint32_t height; /* mm */
	uint32_t fps;
	uint32_t dfr_fps[FPS_LEVEL_MAX];
	uint32_t fps_info_count;
	uint32_t fps_of_longv_for_dvfs;
	uint32_t sw_dvfs_frm_rate;

	uint32_t display_num;
	struct display_info display_info[SPLIT_SCREEN_MAX];

	/* used for dsc_info send to dumd */
	uint32_t dsc_out_width;
	uint32_t dsc_out_height;
	uint32_t dsc_en;
	uint32_t spr_en;

	uint32_t fpga_flag;
	uint32_t fake_panel_flag;

	uint32_t mode;
	uint32_t id;

	uint32_t enable_lbuf_reserve;

	/* used for scene and policy send to dumd */
	struct support_scene_info scene_info;
	uint64_t opr_policy;
	uint8_t compose_policy;

	uint8_t panel_partial_ctrl_support;
	uint8_t hs_pkt_discontin_support;
	uint8_t rsv[1];

	/* used for fold panel */
	uint32_t fold_type;
	uint32_t lcd_te_idx; /* 0: te0, 1: te1 */

	/* confirmed by the hardware, same with scene_id */
	uint32_t pipe_sw_itfch_idx;

	uint32_t enable_async_online;
	uint8_t *edid;
	uint16_t edid_len;

	uint32_t ppu_support;
	uint8_t dirty_region_updt_support;

	/* Always refresh is set to the pointer to the next device such as:
	 * fb->peri_device = composer_manager(online)
	 * composer_manager->peri_device = dpu_connector_manager(mipi_dsi)
	 * dpu_connector_manager->peri_device = panel
	 * panel->peri_device = null
	 *
	 * gfxdev->peri_device = composer_manager(offline)
	 * composer_manager->peri_device = dpu_connector_manager(offline_panel)
	 */
	struct platform_device *peri_device;

	/* update link relationships
	 * connector->prev is dpu_composer
	 * dpu_composer->next is connector
	 * used for peri_device unregist process, such as
	 * get composer struct by connector's obj_info
	 */
	struct dkmd_object_info *comp_obj_info;
	/*
	 * generic mutex for dptx and hdmitx disconnected and online present
	 */
	struct mutex *pluggable_connect_mutex;
	/*
	 * get supported display information from edid;
	 */
	struct edid_support_timing edid_timing[EDID_DISPLAY_INFO_NUM];
	uint32_t edid_timing_num;

	uint32_t connect_status;

	/* dp pluggable type such as 983 not support plug */
	uint64_t is_pluggable;

	bool is_primary_panel;
	bool is_hardware_cursor_support;
};

static inline bool is_mipi_video_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & (PANEL_MIPI_VIDEO | PANEL_DUAL_MIPI_VIDEO);
}

static inline bool is_mipi_cmd_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & (PANEL_MIPI_CMD | PANEL_DUAL_MIPI_CMD);
}

static inline bool is_dual_mipi_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & (PANEL_DUAL_MIPI_VIDEO | PANEL_DUAL_MIPI_CMD);
}

static inline bool is_dp_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & PANEL_DP;
}

static inline bool is_offline_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & (PANEL_OFFLINECOMPOSER | PANEL_WRITEBACK);
}

static inline bool is_fake_panel(const struct dkmd_object_info *pinfo)
{
	return (pinfo->fake_panel_flag == 1);
}

static inline bool is_hdmi_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & PANEL_HDMI;
}

static inline bool is_builtin_panel(const struct dkmd_object_info *pinfo)
{
	return pinfo->type & PANEL_EXTERNAL;
}

static inline bool is_dp_primary_panel(const struct dkmd_object_info *pinfo)
{
	return is_dp_panel(pinfo) && (pinfo->type & PANEL_PRIMARY) != 0;
}

static inline bool is_primary_panel(const struct dkmd_object_info *pinfo)
{
	return ((is_mipi_cmd_panel(pinfo) || is_mipi_video_panel(pinfo)) && (!is_builtin_panel(pinfo))) ||
		is_dp_primary_panel(pinfo);
}

static inline bool is_ppc_support(const struct dkmd_object_info *pinfo)
{
	return pinfo->panel_partial_ctrl_support == 1;
}

static inline bool is_ppu_support(const struct dkmd_object_info *pinfo)
{
	return pinfo->ppu_support != 0;
}

static inline bool is_dpu_pu_support(const struct dkmd_object_info *pinfo)
{
	return pinfo->dirty_region_updt_support == 1;
}

static inline bool is_force_update(const struct dkmd_object_info *pinfo)
{
	return ((g_debug_force_update == 1) || is_fake_panel(pinfo));
}

#endif /* DKMD_OBJECTS_H */
