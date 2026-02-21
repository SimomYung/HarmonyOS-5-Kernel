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

#ifndef DKMD_COMP_H
#define DKMD_COMP_H

#include <linux/ktime.h>
#include <linux/semaphore.h>
#include "dkmd_object.h"
#include "ukmd_sysfs.h"

#define DPU_COMPOSER_HOLD_TIME (1000UL * 3600 * 24 * 30) // 30 day

struct dkmd_base_frame;
struct dkmd_network;
struct product_config;
struct alsc_info;
struct dkmd_ffd_cfg;
struct disp_present_fence;

enum dpu_comp_type {
	DEVICE_COMP_PRIMARY_ID    = 0,
	DEVICE_COMP_BUILTIN_ID    = 1,
	DEVICE_COMP_DP_ID         = 2,
	DEVICE_COMP_HDMI_ID       = 3,
	DEVICE_COMP_VIRTUAL_ID    = 4,
	DEVICE_COMP_DP_ID_1       = 5,
	DEVICE_COMP_DP_ID_2       = 6,
	DEVICE_COMP_DP_ID_3       = 7,
	DEVICE_COMP_DP_ID_4       = 8,
	DEVICE_COMP_DP_ID_5       = 9,
	DEVICE_COMP_MAX_COUNT     = 10,
};

enum composer_off_mode {
	COMPOSER_OFF_MODE_BLANK = 0,
	COMPOSER_OFF_MODE_SUSPEND,
	COMPOSER_OFF_MODE_FAKE,
	COMPOSER_OFF_MODE_DOZE_SUSPEND,
	COMPOSER_OFF_MODE_DPU_OFF,
	COMPOSER_OFF_MODE_INIT,
};

enum composer_on_mode {
	COMPOSER_ON_MODE_UNBLANK = 0,
	COMPOSER_ON_MODE_RESUME,
	COMPOSER_ON_MODE_DOZE,
	COMPOSER_ON_MODE_DPU_ON,
	COMPOSER_ON_MODE_DOZE2ON,
	COMPOSER_ON_MODE_INIT,
};

struct composer {
	struct dkmd_object_info base;
	uint32_t index;
	uint32_t comp_frame_index;
	uint32_t sw_present_frame_index;
	uint32_t hw_present_frame_index;
	uint32_t fastboot_display_enabled;
	uint32_t comp_err_status;

	ktime_t power_on_timestamp;
	ktime_t power_off_timestamp;

	bool power_on;
	uint8_t power_off_mode;
	uint8_t power_on_mode;
	uint8_t reserved[1];
	struct semaphore blank_sem;
	void *device_data;

	int32_t (*set_fastboot)(struct composer *comp);
	int32_t (*on)(struct composer *comp, uint8_t on_mode /* on_mode */);
	int32_t (*off)(struct composer *comp, uint8_t off_mode /* off_mode */);
	void (*release)(struct composer *comp);

	int32_t (*wait_for_blank)(struct composer *comp);
	void (*restore_fast_unblank_status)(struct composer *comp);
	int32_t (*blank_peri_handle)(struct composer *comp, int32_t blank_mode);

	int32_t (*create_fence)(struct composer *comp, struct disp_present_fence *fence);
	int32_t (*present)(struct composer *comp, void *frame);
	int32_t (*release_fence)(struct composer *comp, int32_t fd);

	int32_t (*get_product_config)(struct composer *comp, struct product_config *out_config);
	int32_t (*get_ext_info)(struct composer *comp, struct product_ext_config *out_config);
	int32_t	(*get_link_info)(struct composer *comp, struct dpu_dp_hdmi_link_info *out_link_info);
	int32_t (*get_sysfs_attrs)(struct composer *comp, struct ukmd_attr **out_attr);
	int32_t (*set_display_timing)(struct composer *comp, struct dp_display_timing *out_timing);
    int32_t (*set_display_active_region)(struct composer *comp, struct dkmd_rect_coord *region);
	int32_t (*get_hdr_mean)(struct composer *comp, uint32_t *hdr_mean);
	int32_t (*get_hdr_statistic)(struct composer *comp, void __user* argp);

	int32_t (*register_alsc)(struct composer *comp, uint32_t index, uint32_t dpp_offset);
	int32_t (*unregister_alsc)(struct composer *comp);
	int32_t (*get_alsc_info)(struct composer *comp, struct alsc_info *info);

	int32_t (*get_fusa_info)(struct composer *comp, struct dkmd_ffd_cfg *info);
	int32_t (*get_tui_level1_layer_info)(struct composer *comp, struct tui_level1_layer_info *info);

	int32_t (*effect_hiace_get_hist)(struct composer *comp, void __user* argp);
	int32_t (*effect_hiace_set_lut)(struct composer *comp, const void __user* argp);
	int32_t (*effect_wake_up_hiace_hist)(struct composer *comp);
	int32_t (*set_safe_frm_rate)(struct composer *comp, uint32_t safe_frm_rate);

	int32_t (*set_active_rect)(struct composer *comp, uint32_t ppcConfigId);
	void (*get_online_crc)(struct composer *comp, uint32_t *crc_val);

	int32_t (*update_hardware_cursor)(struct composer *comp, struct hardware_cursor_info *info);
	int32_t (*enable_hardware_cursor_stats)(struct composer *comp, int32_t scene_id, bool enable);
    int32_t (*get_hardware_cursor_stats)(
        struct composer *comp, int32_t scene_id, uint32_t *frame_count, uint32_t *vsync_count);
	int32_t (*tunnel_present)(struct composer *comp, struct tunnel_layer_data *data);
	int32_t (*send_pq_data)(struct composer *comp, uint8_t *data, uint32_t len);
};

#endif
