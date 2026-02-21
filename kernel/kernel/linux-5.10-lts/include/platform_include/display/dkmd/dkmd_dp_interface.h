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

#ifndef __DKMD_DP_INTERFACE_H__
#define __DKMD_DP_INTERFACE_H__

#include <linux/types.h>
#include "dkmd_esd_panel_info.h"
#include "dkmd_dfr_info.h"
#include "dkmd_user_panel_info.h"
#include "dkmd_dubai_interface.h"
#include "dkmd_lcd_interface.h"

#define FACTORY_EDID_MAX_LEN 20
/* this struct expose for external include */

struct dp_panel_mgr {
	uint8_t esd_is_support;
	struct esd_panel_info esd_info;
	uint8_t fps_support;
	struct dfr_info dfr_info;
	uint32_t te_from_tcon_gpio;
	uint8_t adjust_htotal;
	struct user_panel_info dpu_pu_cfg_info; /* dpu partial update config info */
	int32_t (*panel_power_pre_on)(void);
	int32_t (*panel_power_after_on)(void);
	int32_t (*panel_power_pre_off)(void);
	int32_t (*panel_power_after_off)(void);
	void (*panel_power_set_backlight)(int port_id, uint32_t bl_level);
	int32_t (*esd_handle)(void);
	int32_t (*notify_sfr_info)(const struct sfr_info *sfr_info);
	struct panel_refresh_statistic_rslt* (*panel_get_refresh_statistic_data)(int port_id);
	int32_t (*panel_set_display_active_region)(struct dkmd_rect_coord *region);
	int32_t (*panel_power_no_need_dp_off)(void);
	int32_t (*power_on_sink_device)(int port_id); /* notify sink device power on */
};
struct dp_link_info {
	uint8_t *edid;
	uint16_t edid_len;
	uint8_t factory_edid_info[FACTORY_EDID_MAX_LEN];
	uint32_t xres;
	uint32_t yres;
	uint32_t fps;
};

struct dp_hook_ops {
	int (*save_dp_edid)(uint8_t *edid_buf, uint32_t buf_len, uint8_t port_id);
	bool (*is_valid_timing_info)(uint16_t hactive_pixels, uint16_t vactive_pixels, uint32_t fps, uint8_t port_id);
};

#ifdef CONFIG_DKMD_DPU_DP
void dpu_dp_hook_register(struct dp_hook_ops *ops);
#else
inline void dpu_dp_hook_register(struct dp_hook_ops *ops)
{
	return;
}
#endif

int32_t register_dp_panel_mgr(struct dp_panel_mgr *dp_panel_mgr, int port_id);
void aux_set_backlight(int port_id, uint32_t bl_level);
int dptx_read_dpcd_by_portid(int port_id, uint32_t reg_addr, uint8_t *val);

int get_dp_edid_info(int port_id, struct dp_link_info* out_data);
int get_dp_name(int port_id, char* out_dp_name);

#endif /* __DKMD_DP_INTERFACE_H__ */
