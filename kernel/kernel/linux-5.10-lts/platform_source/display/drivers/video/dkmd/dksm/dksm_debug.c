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
#ifdef CONFIG_MDFX_KMD
int64_t g_dkmd_mdfx_id = -1;
#endif

#ifdef CONFIG_DKMD_DEBUG_ENABLE

module_param_named(debug_msg_level, g_dkmd_log_level, uint, 0640);
MODULE_PARM_DESC(debug_msg_level, "dpu msg level");

module_param_named(debug_fence_timeline, g_debug_fence_timeline, uint, 0640);
MODULE_PARM_DESC(debug_fence_timeline, "dpu fence timeline debug");

module_param_named(enable_ldi_data_gate, g_ldi_data_gate_en, uint, 0640);
MODULE_PARM_DESC(enable_ldi_data_gate, "dpu dsi ldi data gate enable");

module_param_named(debug_present_hold, g_debug_present_hold, uint, 0640);
MODULE_PARM_DESC(debug_present_hold, "dpu present hold debug");

module_param_named(debug_vsync_dump, g_debug_vsync_dump, uint, 0640);
MODULE_PARM_DESC(debug_vsync_dump, "debug vsync dump timestamp");

module_param_named(debug_force_update, g_debug_force_update, uint, 0640);
MODULE_PARM_DESC(debug_force_update, "debug force update");

module_param_named(debug_vsync_delay, g_debug_vsync_delay_time, uint, 0640);
MODULE_PARM_DESC(debug_vsync_delay, "debug vsync delay time");

module_param_named(debug_dpu_clear, g_debug_dpu_clear_enable, uint, 0640);
MODULE_PARM_DESC(debug_dpu_clear, "debug dpu composer clear config");

module_param_named(debug_idle_enable, g_debug_idle_enable, uint, 0640);
MODULE_PARM_DESC(debug_idle_enable, "dpu idle enable");

module_param_named(debug_dimming_enable, g_debug_dimming_enable, uint, 0640);
MODULE_PARM_DESC(debug_dimming_enable, "dpu dimming enable");

module_param_named(debug_m1_qic_enable, g_debug_m1_qic_enable, uint, 0640);
MODULE_PARM_DESC(debug_m1_qic_enable, "dpu m1 qic enable");

module_param_named(debug_dpu_qos_enable, g_debug_dpu_qos_enable, uint, 0640);
MODULE_PARM_DESC(debug_dpu_qos_enable, "dpu qos enable");

module_param_named(debug_dpu_send_dcs_cmds, g_debug_dpu_send_dcs_cmds, uint, 0640);
MODULE_PARM_DESC(debug_dpu_send_dcs_cmds, "dpu send dcs cmds");

module_param_named(debug_dpu_perf_level, g_debug_dpu_perf_level, uint, 0640);
MODULE_PARM_DESC(debug_dpu_perf_level, "dpu perf level");

module_param_named(debug_dump_reg, g_debug_dump_reg, uint, 0640);
MODULE_PARM_DESC(debug_dump_reg, "dpu dump reg");

module_param_named(debug_pc_dfr_backlight_with_frame, g_debug_pc_dfr_backlight_with_frame, int, 0640);
MODULE_PARM_DESC(debug_pc_dfr_backlight_with_frame, "debug pc dfr backlight with frame");

module_param_named(debug_dvfs_type, g_debug_dvfs_type, uint, 0640);
MODULE_PARM_DESC(debug_dvfs_type, "debug dvfs type");

module_param_named(debug_dvfs_isr_enable, g_debug_dvfs_isr_enable, uint, 0640);
MODULE_PARM_DESC(debug_dvfs_isr_enable, "enable dvfs isr");

module_param_named(debug_dpu_safe_frm_rate, g_debug_dpu_safe_frm_rate, uint, 0640);
MODULE_PARM_DESC(debug_dpu_safe_frm_rate, "dpu safe frm rate");

module_param_named(underflow_dump_enable, g_debug_underflow_dump_enable, int, 0600);
MODULE_PARM_DESC(underflow_dump_enable, "enable underflow dump");

module_param_named(dmd_report_vact_end_miss, g_debug_dmd_report_vact_end_miss, int, 0600);
MODULE_PARM_DESC(dmd_report_vact_end_miss, "enable dmd report vactive endmiss");
module_param_named(vsync_vactive_time_gap, g_vsync_vactive_time_gap, int, 0640);
MODULE_PARM_DESC(time_gap, "vsync time diff");

module_param_named(debug_dpu_frm_rate, g_debug_dpu_frm_rate, uint, 0640);
MODULE_PARM_DESC(debug_dpu_frm_rate, "dpu frm rate");
module_param_named(debug_underflow_itr_enable, g_debug_underflow_itr_enable, uint, 0640);
MODULE_PARM_DESC(debug_underflow_itr_enable, "dpu underflow itr enable");
module_param_array_named(debug_perfstat_init_data, g_debug_perfstat_init_data, ullong,
						&g_debug_perfstat_init_data_len, 0640);
module_param_array_named(debug_perfstat_init_data_cpu_dmc, g_debug_perfstat_init_data_cpu_dmc, ullong,
						&g_debug_perfstat_init_data_len_cpu_dmc, 0640);
module_param_named(debug_underflow_fail, g_debug_underflow_fail, int, 0600);
MODULE_PARM_DESC(debug_underflow_fail, "debug underflow fail");

module_param_named(debug_ltpo_by_mcu, g_debug_ltpo_by_mcu, uint, 0640);
MODULE_PARM_DESC(debug_ltpo_by_mcu, "debug ltpo by mcu");

module_param_named(debug_online_crc_enable, g_debug_online_crc_enable, uint, 0640);
MODULE_PARM_DESC(debug_online_crc_enable, "dpu crc config");

module_param_named(debug_offline_wait_time, g_offline_wait_time, uint, 0640);
MODULE_PARM_DESC(debug_offline_wait_time, "debug offline change wait time");

module_param_named(debug_mipi_dsi_clk_upt, g_debug_mipi_dsi_clk_upt, uint, 0640);
MODULE_PARM_DESC(debug_mipi_dsi_clk_upt, "external upt mipi dsi clk");

module_param_named(enable_dpu_doze, g_enable_dpu_doze, int, 0600);
MODULE_PARM_DESC(enable_dpu_doze, "enable dpu doze vote");

module_param_named(tui_level_disable, g_debug_tui_level_disable, uint, 0640);
MODULE_PARM_DESC(tui_level_disable, "tui level disable");
module_param_named(debug_dpu_reset_hardware, g_debug_dpu_reset_hardware, uint, 0640);
MODULE_PARM_DESC(debug_dpu_reset_hardware, "debug dpu reset hardware");

module_param_named(enable_esd_recovery, g_enable_esd_recovery, uint, 0640);
MODULE_PARM_DESC(enable_esd_recovery, "enable esd recovery");

module_param_named(debug_event_time_out, g_debug_wait_vactive_timeout_ms, int, 0640);
MODULE_PARM_DESC(debug_event_time_out, "dpu event timeout");

module_param_named(debug_dsi_lp_power_mode, g_debug_dsi_lp_power_mode, int, 0600);
MODULE_PARM_DESC(debug_dsi_lp_power_mode, "debug dsi lp power mode");

module_param_named(debug_dss_clk_lp_mode, g_debug_dss_clk_lp_mode, uint, 0640);
MODULE_PARM_DESC(debug_dss_clk_lp_mode, "dss clk lp mode");

module_param_named(debug_panel_refresh_rect, g_debug_panel_refresh_rect, uint, 0600);
MODULE_PARM_DESC(debug_panel_refresh_rect, "debug panel refresh rect");

module_param_named(debug_partial_tx_support, g_debug_partial_tx_support, int, 0600);
MODULE_PARM_DESC(debug_partial_tx_support, "debug partial tx support");

module_param_named(debug_partial_refresh_support, g_debug_partial_refresh_support, int, 0600);
MODULE_PARM_DESC(debug_partial_refresh_support, "debug partial refresh support");

module_param_named(debug_idle_shut_down_enable, g_debug_idle_shut_down_enable, uint, 0640);
MODULE_PARM_DESC(debug_idle_shut_down_enable, "debug idle shut down enable");

module_param_named(debug_ops_deep_sleep_enable, g_debug_ops_deep_sleep_enable, uint, 0640);
MODULE_PARM_DESC(debug_ops_deep_sleep_enable, "debug ops deep sleep enable");

module_param_named(debug_dvfs_dump_timer_enable, g_debug_dvfs_dump_timer, uint, 0640);
MODULE_PARM_DESC(debug_dvfs_dump_timer_enable, "debug dvfs dump timer enable");

module_param_named(debug_dvfs_frame_idle_enable, g_debug_dvfs_frame_idle, uint, 0640);
MODULE_PARM_DESC(debug_dvfs_dump_timer_enable, "debug frame dvfs idle");

module_param_named(debug_multi_dirty_rects_support, g_debug_multi_dirty_rects_support, uint, 0600);
MODULE_PARM_DESC(debug_multi_dirty_rects_support, "debug multi dirty rects support");

module_param_named(debug_dvfs_auto_test_enable, g_debug_dvfs_auto_test_enable, uint, 0640);
MODULE_PARM_DESC(debug_dvfs_auto_test_enable, "debug dvfs auto test enable");

module_param_named(debug_mntn_rdr_dump_enable, g_debug_mntn_rdr_dump_enable, uint, 0640);
MODULE_PARM_DESC(debug_mntn_rdr_dump_enable, "debug mntn rdr dump enable");

module_param_named(debug_doze2_enable, g_debug_doze2_enable, uint, 0640);
MODULE_PARM_DESC(debug_doze2_enable, "debug doze2 enable");

module_param_named(debug_doze1_intra_flag_enable, g_debug_doze1_intra_flag_enable, uint, 0640);
MODULE_PARM_DESC(debug_doze1_intra_flag_enable, "debug doze1 intra flag enable");

module_param_named(debug_doze1_inter_flag_enable, g_debug_doze1_inter_flag_enable, uint, 0640);
MODULE_PARM_DESC(debug_doze1_inter_flag_enable, "debug doze1 inter flag enable");
#endif
