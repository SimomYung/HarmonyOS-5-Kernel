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
#include <linux/module.h>
#include "dkmd_log.h"

uint32_t g_dkmd_log_level = UKMD_LOG_LVL_INFO;
uint32_t g_debug_fence_timeline = 0;
uint32_t g_ldi_data_gate_en = 1;
uint32_t g_debug_present_hold;
uint32_t g_debug_vsync_dump = 0;
uint32_t g_debug_force_update = 0;
uint32_t g_debug_vsync_delay_time = 0;
uint32_t g_debug_dpu_clear_enable = 1;
uint32_t g_debug_idle_enable = 1;
uint32_t g_debug_dimming_enable = 1;
uint32_t g_debug_m1_qic_enable = 0;
uint32_t g_debug_dpu_qos_enable = 1;
uint32_t g_debug_dpu_send_dcs_cmds = 0;
uint32_t g_debug_dpu_perf_level = 0;
int32_t g_debug_underflow_dump_enable = 1;
uint32_t g_debug_dump_reg = 0;
int32_t g_debug_pc_dfr_backlight_with_frame = 0;
uint32_t g_debug_dmd_report_vact_end_miss = 0;
// Set this type in screen off, in order to switch to other dvfs type
// 0: hw channel dvfs 1: sw channel dvfs 2: sw regulator dvfs
uint32_t g_debug_dvfs_type = 0;
uint32_t g_debug_dvfs_isr_enable = 0;
uint32_t g_debug_dpu_safe_frm_rate = 0;
uint32_t g_pan_display_frame_index;
uint32_t g_debug_dpu_frm_rate = 0;
bool g_dpu_complete_start;
uint32_t g_vsync_vactive_time_gap = 2000; // us
uint32_t g_debug_underflow_itr_enable = 0;
uint64_t g_debug_perfstat_init_data[6] = {10, 0x7FF00000300C00C0, 0x00000000000E8018, 77, 2, 1};
uint64_t g_debug_perfstat_init_data_cpu_dmc[6] = {10, 0x0000000F00000000, 0x0, 19, 2, 2};
int g_debug_perfstat_init_data_len = 0;
int g_debug_perfstat_init_data_len_cpu_dmc = 0;
int32_t g_debug_underflow_fail = 0;
uint32_t g_debug_ltpo_by_mcu = 0;
uint32_t g_offline_wait_time = 0;
uint32_t g_debug_mipi_dsi_clk_upt = 0;
int32_t g_enable_dpu_doze = 0;
uint32_t g_debug_online_crc_enable = 0;
uint32_t g_debug_tui_level_disable = 0; // 0-both enable; 1-level0 disable; 2-level1 disable; 3-both disable
uint32_t g_debug_dpu_reset_hardware = 0;
uint32_t g_enable_esd_recovery = 0;
int32_t g_debug_wait_vactive_timeout_ms = 300;
int32_t g_debug_dsi_lp_power_mode = 0;
uint32_t g_debug_dss_clk_lp_mode = 1;
uint32_t g_debug_panel_refresh_rect = 0;
int32_t g_debug_partial_tx_support = -1;
int32_t g_debug_partial_refresh_support = -1;
uint32_t g_debug_idle_shut_down_enable = 1;
uint32_t g_debug_ops_deep_sleep_enable = 0;
uint32_t g_debug_dvfs_dump_timer = 3000;
uint32_t g_debug_dvfs_frame_idle = 0;
uint32_t g_debug_multi_dirty_rects_support = 1;
uint32_t g_debug_dvfs_auto_test_enable = 0;
uint32_t g_debug_mntn_rdr_dump_enable = 0;
uint32_t g_debug_doze2_enable = 1;
uint32_t g_debug_doze1_intra_flag_enable = 0;
uint32_t g_debug_doze1_inter_flag_enable = 1;

