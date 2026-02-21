/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: Update and load independent calibration data
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#include "plat_cali.h"
#include "plat_debug.h"
#include "bfgx_dev.h"
#include "plat_pm.h"
#include "bfgx_data_parse.h"
#include "pcie_linux.h"
#include "cali_data_ops_default.h"
#include "cali_data_ops_mp17c.h"
#include "cali_data_ops_mp12.h"


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
#ifndef _PRE_NO_HISI_NVRAM
#define HISI_NVRAM_SUPPORT
#endif
#endif

/* 保存校准数据的buf */
STATIC uint8_t *g_cali_data_buf = NULL; /* wifi校准数据(仅SDIO使用) */
STATIC uint8_t *g_cali_data_buf_dma = NULL; /* wifi校准数据(仅PCIE使用) */
STATIC uint64_t g_cali_data_buf_phy_addr = 0; /* 06 wifi校准数据DMA物理地址 */
uint8_t g_netdev_is_open = OAL_FALSE;
uint32_t g_cali_update_channel_info = 0;

#ifdef _PRE_DUAL_CHIP
/* 用于chip2的校准数据 */
STATIC uint8_t *g_cali_data_buf_dev1 = NULL;
STATIC uint8_t *g_cali_data_buf_dma_dev1 = NULL;
STATIC uint64_t g_cali_data_buf_phy_addr_dev1 = 0;
STATIC uint32_t g_cali_update_channel_info_dev1 = 0;

static void cali_data_buf_free_dev1(void);
static int32_t wifi_cali_buf_malloc_dev1(void);
static void *get_cali_data_buf_addr_by_dev(uint32_t dev_id);
#endif

/* add for MP13 bfgx */
STATIC struct completion g_cali_recv_done;
uint8_t *g_bfgx_cali_data_buf = NULL;

#ifdef HISI_NVRAM_SUPPORT
STATIC int32_t nvram_post_process(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify);
STATIC int32_t ext_nvram_post_process(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify);
STATIC int32_t nvram_update_ini_process(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify);
#endif

int32_t g_cali_buffer_debug = 0;
oal_debug_module_param(g_cali_buffer_debug, int, S_IRUGO | S_IWUSR);

/* 定义不能超过BFGX_BT_CUST_INI_SIZE/4 (128) 最后使用 { NULL, 0 } 结尾 */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_mp13[MPXX_BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                       0 },
    { "bt_edrpow_offset",                  0 },
    { "bt_blepow_offset",                  0 },
    { "bt_cali_txpwr_pa_ref_num",          0 },
    { "bt_cali_txpwr_pa_ref_band1",        0 },
    { "bt_cali_txpwr_pa_ref_band2",        0 },
    { "bt_cali_txpwr_pa_ref_band3",        0 },
    { "bt_cali_txpwr_pa_ref_band4",        0 },
    { "bt_cali_txpwr_pa_ref_band5",        0 },
    { "bt_cali_txpwr_pa_ref_band6",        0 },
    { "bt_cali_txpwr_pa_ref_band7",        0 },
    { "bt_cali_txpwr_pa_ref_band8",        0 },
    { "bt_cali_txpwr_pa_fre1",             0 },
    { "bt_cali_txpwr_pa_fre2",             0 },
    { "bt_cali_txpwr_pa_fre3",             0 },
    { "bt_cali_txpwr_pa_fre4",             0 },
    { "bt_cali_txpwr_pa_fre5",             0 },
    { "bt_cali_txpwr_pa_fre6",             0 },
    { "bt_cali_txpwr_pa_fre7",             0 },
    { "bt_cali_txpwr_pa_fre8",             0 },
    { "bt_cali_bt_tone_amp_grade",         0 },
    { "bt_rxdc_band",                      0 },
    { "bt_dbb_scaling_saturation",         0 },
    { "bt_productline_upccode_search_max", 0 },
    { "bt_productline_upccode_search_min", 0 },
    { "bt_dynamicsarctrl_bt",              0 },
    { "bt_powoffsbt",                      0 },
    { "bt_elna_2g_bt",                     0 },
    { "bt_rxisobtelnabyp",                 0 },
    { "bt_rxgainbtelna",                   0 },
    { "bt_rxbtextloss",                    0 },
    { "bt_elna_on2off_time_ns",            0 },
    { "bt_elna_off2on_time_ns",            0 },
    { "bt_hipower_mode",                   0 },
    { "bt_fem_control",                    0 },
    { "bt_feature_32k_clock",              0 },
    { "bt_feature_log",                    0 },
    { "bt_cali_swtich_all",                0 },
    { "bt_ant_num_bt",                     0 },
    { "bt_power_level_control",            0 },
    { "bt_country_code",                   0 },
    { "bt_reserved1",                      0 },
    { "bt_reserved2",                      0 },
    { "bt_reserved3",                      0 },
    { "bt_dedicated_antenna",              0 },
    { "bt_reserved5",                      0 },
    { "bt_reserved6",                      0 },
    { "bt_reserved7",                      0 },
    { "bt_reserved8",                      0 },
    { "bt_reserved9",                      0 },
    { "bt_reserved10",                     0 },

    { NULL, 0 }
};

/* 定义不能超过BFGX_BT_CUST_INI_SIZE/4 (128) 最后使用 { NULL, 0 } 结尾 */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_mp15[MPXX_BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                       0 },
    { "bt_edrpow_offset",                  0 },
    { "bt_blepow_offset",                  0 },
    { "bt_cali_txpwr_pa_ref_num",          0 },
    { "bt_cali_txpwr_pa_ref_band1",        0 },
    { "bt_cali_txpwr_pa_ref_band2",        0 },
    { "bt_cali_txpwr_pa_ref_band3",        0 },
    { "bt_cali_txpwr_pa_ref_band4",        0 },
    { "bt_cali_txpwr_pa_ref_band5",        0 },
    { "bt_cali_txpwr_pa_ref_band6",        0 },
    { "bt_cali_txpwr_pa_ref_band7",        0 },
    { "bt_cali_txpwr_pa_ref_band8",        0 },
    { "bt_cali_txpwr_pa_fre1",             0 },
    { "bt_cali_txpwr_pa_fre2",             0 },
    { "bt_cali_txpwr_pa_fre3",             0 },
    { "bt_cali_txpwr_pa_fre4",             0 },
    { "bt_cali_txpwr_pa_fre5",             0 },
    { "bt_cali_txpwr_pa_fre6",             0 },
    { "bt_cali_txpwr_pa_fre7",             0 },
    { "bt_cali_txpwr_pa_fre8",             0 },
    { "bt_cali_bt_tone_amp_grade",         0 },
    { "bt_rxdc_band",                      0 },
    { "bt_dbb_scaling_saturation",         0 },
    { "bt_productline_upccode_search_max", 0 },
    { "bt_productline_upccode_search_min", 0 },
    { "bt_dynamicsarctrl_bt",              0 },
    { "bt_powoffsbt",                      0 },
    { "bt_elna_2g_bt",                     0 },
    { "bt_rxisobtelnabyp",                 0 },
    { "bt_rxgainbtelna",                   0 },
    { "bt_rxbtextloss",                    0 },
    { "bt_elna_on2off_time_ns",            0 },
    { "bt_elna_off2on_time_ns",            0 },
    { "bt_hipower_mode",                   0 },
    { "bt_fem_control",                    0 },
    { "bt_feature_32k_clock",              0 },
    { "bt_feature_log",                    0 },
    { "bt_cali_switch_all",                0 },
    { "bt_ant_num_bt",                     0 },
    { "bt_power_level_control",            0 },
    { "bt_country_code",                   0 },
    { "bt_power_idle_voltage",             0 },
    { "bt_power_tx_voltage",               0 },
    { "bt_power_rx_voltage",               0 },
    { "bt_power_sleep_voltage",            0 },
    { "bt_power_idle_current",             0 },
    { "bt_power_tx_current",               0 },
    { "bt_power_rx_current",               0 },
    { "bt_power_sleep_current",            0 },
    { "bt_20dbm_txpwr_cali_num",           0 },
    { "bt_20dbm_txpwr_cali_freq_ref1",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref2",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref3",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref4",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref5",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref6",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref7",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref8",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref9",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref10",    0 },
    { "bt_20dbm_txpwr_cali_freq_ref11",    0 },
    { "bt_20dbm_txpwr_cali_freq_ref12",    0 },
    { "bt_20dbm_txpwr_cali_freq_ref13",    0 },
    { "bt_reserved1",                      0 },
    { "bt_20dbm_txpwr_adjust",             0 },
    { "bt_feature_config",                 0 },
    { "bt_dedicated_antenna",              0 },
    { "bt_reserved5",                      0 },
    { "bt_reserved6",                      0 },
    { "bt_reserved7",                      0 },
    { "bt_reserved8",                      0 },
    { "bt_reserved9",                      0 },
    { "bt_reserved10",                     0 },

    { NULL, 0 }
};

/* 定义不能超过BFGX_BT_CUST_INI_SIZE/4 (128) 最后使用 { NULL, 0 } 结尾 */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_mp16[MPXX_BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                0 },
    { "bt_edrpow_offset",           0 },
    { "bt_blepow_offset",           0 },
    { "bt_power_level_control",     0 },
    { "rf_trx_features",            0 },
    { "bt_dedicated_antenna",       0 },
    { "bt_elna_2g_bt",              0 },
    { "bt_feature_config",          0 },
    { "bt_power_idle_voltage",      0 },
    { "bt_power_tx_voltage",        0 },
    { "bt_power_rx_voltage",        0 },
    { "bt_power_sleep_voltage",     0 },
    { "bt_power_idle_current",      0 },
    { "bt_power_tx_current",        0 },
    { "bt_power_rx_current",        0 },
    { "bt_power_sleep_current",     0 },
    { "bt_priv_config",             0 },
    { "bt_ce_adaptive",             0 },
    { "bt_cali_switch_all",         0 },
    { "bt_powermgmt",               0 },
    { "bt_reserved1",               0 },
    { "bt_reserved2",               0 },
    { "bt_reserved3",               0 },

    { NULL, 0 }
};

/* 定义不能超过BFGX_BT_CUST_INI_SIZE/4 (128) 最后使用 { NULL, 0 } 结尾 */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_mp16c[MPXX_BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                    0 },
    { "bt_edrpow_offset",               0 },
    { "bt_blepow_offset",               0 },
    { "bt_power_level_control",         0 },
    { "rf_trx_features",                0 },
    { "bt_dedicated_antenna",           0 },
    { "bt_elna_2g_bt",                  0 },
    { "bt_feature_config",              0 },
    { "bt_power_idle_voltage",          0 },
    { "bt_power_tx_voltage",            0 },
    { "bt_power_rx_voltage",            0 },
    { "bt_power_sleep_voltage",         0 },
    { "bt_power_idle_current",          0 },
    { "bt_power_tx_current",            0 },
    { "bt_power_rx_current",            0 },
    { "bt_power_sleep_current",         0 },
    { "bt_priv_config",                 0 },
    { "bt_ce_adaptive",                 0 },
    { "bt_cali_switch_all",             0 },
    { "bt_fem_mod_config",              0 },

    { "bt_tx_pwr_c_offset_high_0",      0 },
    { "bt_tx_pwr_c_offset_high_1",      0 },
    { "bt_tx_pwr_c_offset_high_2",      0 },
    { "bt_tx_pwr_c_offset_high_3",      0 },
    { "bt_tx_pwr_c_offset_high_4",      0 },
    { "bt_tx_pwr_c_offset_high_5",      0 },
    { "bt_tx_pwr_c_offset_high_6",      0 },
    { "bt_tx_pwr_c_offset_high_7",      0 },

    { "bt_tx_pwr_c_offset_norm_ad_0",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_1",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_2",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_3",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_4",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_5",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_6",   0 },
    { "bt_tx_pwr_c_offset_norm_ad_7",   0 },

    { "bt_tx_pwr_c_offset_norm_a0_0",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_1",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_2",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_3",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_4",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_5",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_6",   0 },
    { "bt_tx_pwr_c_offset_norm_a0_7",   0 },

    { "gle_tx_pwr_c_offset_0",          0 },
    { "gle_tx_pwr_c_offset_1",          0 },
    { "gle_tx_pwr_c_offset_2",          0 },
    { "gle_tx_pwr_c_offset_3",          0 },
    { "gle_tx_pwr_c_offset_4",          0 },
    { "gle_tx_pwr_c_offset_5",          0 },
    { "gle_tx_pwr_c_offset_6",          0 },
    { "gle_tx_pwr_c_offset_7",          0 },

    { "bt_spxt_ctrl_config",            0 },

    { "bt_powermgmt",                   0 },

    { "bt_sar_pwrctrl_en",              0 },

    { "bt_20dbm_mixbuf_code",           0 },
    { "bt_glepow_offset",               0 },
    { "gle_dpsk_pwr",                   0 },

    { "bt_wlcoex_ctrl_config",          0 },
    { "gle_off_find",                   0 },
    { "slem_cali_offset",               0 },
    { "bt_reserved2",                   0 },
    { "bt_reserved3",                   0 },

    { NULL, 0 }
};

/* 定义不能超过BFGX_BT_CUST_INI_SIZE/4 (128) 最后使用 { NULL, 0 } 结尾 */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_mp17c[MP17C_BFGX_BT_CUST_INI_SIZE / 4] = {
    { "fem_mode_map_2g1",               0 },
    { "fem_mode_map_2g2",               0 },
    { "fem_mode_map_5g1",               0 },
    { "fem_mode_map_5g2",               0 },

    { "bt_ant_switch_mask",             0 },
    { "bt_lna_switch_mask",             0 },
    { "bt_lna",                         0 },
    { "bt_wl_path_en",                  0 },
    { "bt_rssi_amend",                  0 },
    { "bt_2g_cali_cust_mask",           0 },
    { "bt_2g_comp_cust_mask",           0 },
    { "bt_5g_cali_cust_mask",           0 },
    { "bt_5g_comp_cust_mask",           0 },
    { "multi_mac_rxpd_set",             0 },

    { "bt_maxpower",                    0 },
    { "bt_edrpow_offset",               0 },
    { "bt_blepow_offset",               0 },
    { "rf_trx_features",                0 },
    { "bt_feature_config",              0 },
    { "bt_priv_config",                 0 },
    { "bt_ce_adaptive",                 0 },
    { "bt_online_cali_switch",          0 },
    /* 校准后保存到nv中的ini，次序不要变更 */
    { "bt_tx_pwr_c0_coeff_a_norm",      0 },
    { "bt_tx_pwr_c1_coeff_a_norm",      0 },
    { "bt_tx_pwr_c0_coeff_a_high",      0 },
    { "bt_tx_pwr_c1_coeff_a_high",      0 },
    { "bt_tx_pwr_c0_coeff_b_norm",      0 },
    { "bt_tx_pwr_c1_coeff_b_norm",      0 },
    { "bt_tx_pwr_c0_coeff_b_high",      0 },
    { "bt_tx_pwr_c1_coeff_b_high",      0 },

    { "bt_tx_pwr_c0_coeff_c_norm_0",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_1",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_2",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_3",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_4",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_5",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_6",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_7",    0 },

    { "bt_tx_pwr_c1_coeff_c_norm_0",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_1",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_2",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_3",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_4",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_5",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_6",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_7",    0 },

    { "bt_tx_pwr_c0_coeff_c_high_0",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_1",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_2",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_3",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_4",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_5",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_6",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_7",    0 },

    { "bt_tx_pwr_c1_coeff_c_high_0",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_1",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_2",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_3",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_4",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_5",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_6",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_7",    0 },

    { "bt_wl_pwr_c0_coeff_a_2g",    12 },
    { "bt_wl_pwr_c0_coeff_b_2g",    34 },
    { "bt_tx_pwr_c0_coeff_c_2g",    -74 },
    { "bt_wl_pwr_c1_coeff_a_2g",    12 },
    { "bt_wl_pwr_c1_coeff_b_2g",    34 },
    { "bt_tx_pwr_c1_coeff_c_2g",    -74 },
    { "bt_wl_pwr_c0_dpn_0_2g",    0 },
    { "bt_wl_pwr_c0_dpn_1_2g",    0 },
    { "bt_wl_pwr_c1_dpn_0_2g",    0 },
    { "bt_wl_pwr_c1_dpn_1_2g",    0 },
    { "bt_wl_pwr_c0_coeff_a_5g",    12 },
    { "bt_wl_pwr_c0_coeff_b_5g",    34 },
    { "bt_tx_pwr_c0_coeff_c_5g",    -74 },
    { "bt_wl_pwr_c1_coeff_a_5g",    12 },
    { "bt_wl_pwr_c1_coeff_b_5g",    34 },
    { "bt_tx_pwr_c1_coeff_c_5g",    -74 },
    /* 校准后保存到nv中的ini，次序不要变更 */
    { "bt_wl_pwr_c0_dpn_0_5g",    0 },
    { "bt_wl_pwr_c1_dpn_0_5g",    0 },

    { "bt_spxt_ctrl_config",            0 },

    { "bt_powermgmt",                   0 },
    { "bt_sar_pwrctrl_en",              0 },
    { "bt_glepow_offset",               0 },
    { "gle_dpsk_pwr",                   0 },
    { "bt_wlcoex_wlan_siso",            0 },
    { "bt_wlcoex_hpower_mode",          0 },
    { "slem_cali_offset",               0 },
    { "gle_5g_enable",                  0 },
    { "txbf_power_backoff_2g",          0 },
    { "txbf_power_backoff_5g",          0 },
    { "mod_pwr_gfsk_c0_2g",             0 },
    { "mod_pwr_gfsk_c1_2g",             0 },
    { "mod_pwr_bpsk_c0_2g",             0 },
    { "mod_pwr_bpsk_c1_2g",             0 },
    { "mod_pwr_qpsk_c0_2g",             0 },
    { "mod_pwr_qpsk_c1_2g",             0 },
    { "mod_pwr_8psk_c0_2g",             0 },
    { "mod_pwr_8psk_c1_2g",             0 },
    { "mod_pwr_4m8psk_c0_2g",           0 },
    { "mod_pwr_4m8psk_c1_2g",           0 },
    { "mod_pwr_polar_bpsk_c0_2g",       0 },
    { "mod_pwr_polar_bpsk_c1_2g",       0 },
    { "mod_pwr_polar_qpsk_c0_2g",       0 },
    { "mod_pwr_polar_qpsk_c1_2g",       0 },
    { "mod_pwr_polar_8psk_c0_2g",       0 },
    { "mod_pwr_polar_8psk_c1_2g",       0 },
    { "mod_pwr_16qam_c0_2g",            0 },
    { "mod_pwr_16qam_c1_2g",            0 },
    { "mod_pwr_4m_16qam_c0_2g",         0 },
    { "mod_pwr_4m_16qam_c1_2g",         0 },
    { "wl_5g_pwr_dec",                  0 },

    { NULL, 0 }
};

STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_mp12[MP12_BFGX_BT_CUST_INI_SIZE / 4] = {
    { "fem_mode_map_2g1",               0 },
    { "fem_mode_map_2g2",               0 },
    { "fem_mode_map_5g1",               0 },
    { "fem_mode_map_5g2",               0 },

    { "bt_ant_switch_mask",             0 },
    { "bt_lna_switch_mask",             0 },
    { "bt_lna",                         0 },
    { "bt_wl_path_en",                  0 },
    { "bt_rssi_amend",                  0 },
    { "bt_2g_cali_cust_mask",           0 },
    { "bt_2g_comp_cust_mask",           0 },
    { "bt_5g_cali_cust_mask",           0 },
    { "bt_5g_comp_cust_mask",           0 },
    { "multi_mac_rxpd_set",             0 },

    { "bt_maxpower",                    0 },
    { "bt_edrpow_offset",               0 },
    { "bt_blepow_offset",               0 },
    { "rf_trx_features",                0 },
    { "bt_feature_config",              0 },
    { "bt_priv_config",                 0 },
    { "bt_ce_adaptive",                 0 },
    { "bt_cali_switch_all",             0 },
    { "bt_tx_pwr_c0_coeff_a_norm",      0 },
    { "bt_tx_pwr_c1_coeff_a_norm",      0 },
    { "bt_tx_pwr_c0_coeff_a_high",      0 },
    { "bt_tx_pwr_c1_coeff_a_high",      0 },
    { "bt_tx_pwr_c0_coeff_b_norm",      0 },
    { "bt_tx_pwr_c1_coeff_b_norm",      0 },
    { "bt_tx_pwr_c0_coeff_b_high",      0 },
    { "bt_tx_pwr_c1_coeff_b_high",      0 },

    { "bt_tx_pwr_c0_coeff_c_norm_0",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_1",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_2",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_3",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_4",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_5",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_6",    0 },
    { "bt_tx_pwr_c0_coeff_c_norm_7",    0 },

    { "bt_tx_pwr_c1_coeff_c_norm_0",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_1",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_2",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_3",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_4",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_5",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_6",    0 },
    { "bt_tx_pwr_c1_coeff_c_norm_7",    0 },

    { "bt_tx_pwr_c0_coeff_c_high_0",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_1",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_2",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_3",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_4",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_5",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_6",    0 },
    { "bt_tx_pwr_c0_coeff_c_high_7",    0 },

    { "bt_tx_pwr_c1_coeff_c_high_0",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_1",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_2",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_3",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_4",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_5",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_6",    0 },
    { "bt_tx_pwr_c1_coeff_c_high_7",    0 },
    { "bt_spxt_ctrl_config",            0 },

    { "bt_powermgmt",                   0 },
    { "bt_sar_pwrctrl_en",              0 },
    { "bt_glepow_offset",               0 },
    { "gle_dpsk_pwr",                   0 },
    { "bt_wlcoex_wlan_siso",            0 },
    { "bt_wlcoex_hpower_mode",          0 },
    { "bt_reserved1",                   0 },
    { "bt_reserved2",                   0 },
    { "bt_reserved3",                   0 },

    { NULL, 0 }
};
#ifdef HISI_NVRAM_SUPPORT
#ifndef _PRE_WINDOWS_SUPPORT
STATIC bfgx_nv_table_stru g_bfgx_nv_table[] = {
    { OAL_BT_NVRAM_NAME, 0, OAL_BT_NVRAM_DATA_LENGTH, OAL_BT_NVRAM_NUMBER, 0, nvram_post_process },
    // 扩展nv ram给业务使用，可以根据offset填充
    { OAL_BT_EXT_NVRAM_NAME, 0, BT_EXT_NVRAM_LENGTH, OAL_BT_EXT_NVRAM_NUMBER, 0, ext_nvram_post_process },
    // SLE功率校准数据，读取到NV中
    { OAL_BT_NVRAM_NAME, 0, OAL_SLE_NVRAM_DATA_LENGTH, OAL_BT_NVRAM_NUMBER, 0, nvram_post_process },
    // NV存储的ini曲线系数
    { OAL_SLE_CAL_NVRAM_NAME, OAL_SLE_COF_NVRAM_OFFSET1, OAL_SLE_FST_COF_SIZE,
        OAL_SLE_CAL_NVRAM_NUMBER, OAL_SLE_NVRAM_VERIFY, nvram_update_ini_process },
    { OAL_SLE_COF_NVRAM_NAME, OAL_SLE_COF_NVRAM_OFFSET2, OAL_SLE_SEC_COF_SIZE,
        OAL_SLE_COF_NVRAM_NUMBER, OAL_SLE_NVRAM_VERIFY, nvram_update_ini_process },
    { OAL_SLE_COE_NVRAM_NAME, OAL_SLE_COF_NVRAM_OFFSET3, OAL_SLE_THR_COF_SIZE,
        OAL_SLE_COE_NVRAM_NUMBER, OAL_SLE_NVRAM_VERIFY, nvram_update_ini_process },
};
#endif
#endif

/* 定义不能超过BFGX_BT_CUST_INI_SIZE/4 (128) */
STATIC int32_t g_bfgx_cust_ini_data[MPXX_BFGX_BT_CUST_INI_SIZE / 4] = {0};

bfgx_cali_data_ops_stru *g_cali_data_ops = &g_cali_data_ops_mpxx;

void bt_cali_data_ops_init(void)
{
    int32_t chip = get_mpxx_subchip_type();
    if (chip == BOARD_VERSION_MP17C) {
        g_cali_data_ops = &g_cali_data_ops_mp17c;
    } else {
        g_cali_data_ops = &g_cali_data_ops_mpxx;
    }
    return;
}

uint32_t get_cali_update_channel_info(void)
{
#ifdef _PRE_DUAL_CHIP
    if (hcc_get_main_dev() == HCC_EP_WIFI_DEV1) {
        return g_cali_update_channel_info_dev1;
    }
#endif
    return g_cali_update_channel_info;
}
EXPORT_SYMBOL(get_cali_update_channel_info);

void set_cali_update_channel_info(uint32_t value)
{
#ifdef _PRE_DUAL_CHIP
    if (hcc_get_main_dev() == HCC_EP_WIFI_DEV1) {
        g_cali_update_channel_info_dev1 = value;
        return;
    }
#endif
    g_cali_update_channel_info = value;
}
EXPORT_SYMBOL(set_cali_update_channel_info);

/*
 * 函 数 名  : get_cali_count
 * 功能描述  : 返回校准的次数，首次开机校准时为0，以后递增
 * 输入参数  : uint32 *count:调用函数保存校准次数的地址
 * 输出参数  : count:自开机以来，已经校准的次数
 * 返 回 值  : 0表示成功，-1表示失败
 */
int32_t get_cali_count(uint32_t *count)
{
    uint32_t cali_update_channel_info = get_cali_update_channel_info();

    if (count == NULL) {
        ps_print_err("count is NULL\n");
        return -EFAIL;
    }

    *count = cali_update_channel_info;

    ps_print_warning("cali update info is [%d]\r\n", cali_update_channel_info);

    return SUCC;
}

#ifdef HISI_NVRAM_SUPPORT
#ifndef _PRE_WINDOWS_SUPPORT
STATIC int32_t nvram_gen_post_process(uint8_t *nv_buffer, uint32_t offset, uint32_t length,
                                      void *(*process)(uint32_t *length))
{
    uint32_t len = 0;
    uint8_t *pst_buf = NULL;

    pst_buf = (uint8_t *)process(&len);
    if (pst_buf == NULL) {
        ps_print_err("get bfgx nv buf fail!");
        return INI_FAILED;
    }
    if (len < length || len < offset + length) {
        ps_print_err("get bfgx nv buf size %d, NV data size is %d!", len, length);
        return INI_FAILED;
    }
    if (memcpy_s(pst_buf + offset, len - offset, nv_buffer, length) != EOK) {
        ps_print_err("nvram_gen_post_process FAILED!");
        return INI_FAILED;
    }
    return INI_SUCC;
}

STATIC int32_t nvram_post_process(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify)
{
    uint8_t nv_buffer[MPXX_BFGX_NV_DATA_SIZE] = {0};
    if (read_conf_from_nvram(nv_buffer, OAL_BT_NVRAM_DATA_LENGTH, nv_number, nv_name) != INI_SUCC) {
        ps_print_err("nvram_post_process::BT read NV error!");
        // last byte of NV ram is used to mark if NV ram is failed to read.
        nv_buffer[OAL_BT_NVRAM_DATA_LENGTH - 1] = OAL_TRUE;
    } else {
        // last byte of NV ram is used to mark if NV ram is failed to read.
        nv_buffer[OAL_BT_NVRAM_DATA_LENGTH - 1] = OAL_FALSE;
    }
    if (verify != 0) {
        nv_buffer[OAL_BT_NVRAM_DATA_LENGTH - 1] = (*(uint32_t*)nv_buffer == verify) ? OAL_FALSE : OAL_TRUE;
    }
    return nvram_gen_post_process(nv_buffer, offset, length, g_cali_data_ops->bfgx_get_nv_data_buf_ops);
}

STATIC int32_t ext_nvram_post_process(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify)
{
    uint8_t nv_buffer[MPXX_BFGX_NV_DATA_SIZE] = {0};
    int32_t gle_off_find = 0;

    (void)get_cust_conf_int32(INI_MODU_DEV_BT, "gle_off_find", &gle_off_find);
    // 判断是否有gle_off_find特性，无此特性直接返回INI_SUCC
    if (((uint32_t)gle_off_find & BIT0) == 0) {
        return INI_SUCC;
    }
    // 避免影响有gle_off_find特性但未经过产线写扩展nv的手机正常启动，直接返回INI_SUCC
    if (read_conf_from_nvram(nv_buffer, OAL_BT_EXT_NVRAM_DATA_LENGTH, nv_number, nv_name) != INI_SUCC) {
        ps_print_info("ext_nvram_post_process nv_number:%d", nv_number);
        return INI_SUCC;
    }

    if (verify != 0) {
        nv_buffer[OAL_BT_NVRAM_DATA_LENGTH - 1] = (*(uint32_t*)nv_buffer == verify) ? OAL_FALSE : OAL_TRUE;
    }

    return nvram_gen_post_process(nv_buffer, offset, length, g_cali_data_ops->bfgx_get_ext_nv_data_buf_ops);
}

STATIC int32_t nvram_update_ini_with_verify(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify)
{
    uint32_t len = 0;
    uint8_t *ini_buf = g_cali_data_ops->bfgx_get_cust_ini_data_buf_ops(&len);
    uint8_t nv_buffer[MPXX_BFGX_NV_DATA_SIZE] = {0};

    if (read_conf_from_nvram(nv_buffer, OAL_BT_NVRAM_DATA_LENGTH, nv_number, nv_name) != INI_SUCC) {
        ps_print_err("nvram_update_ini_with_verify::BT read NV error!");
        // NV加载失败不能影响ini，返回SUCC
        return INI_SUCC;
    }
    // 数据校验,校验失败需要存储ini数据到NV
    if (verify != 0) {
        if (*(uint32_t*)nv_buffer != verify) {
            *(uint32_t*)nv_buffer = verify;
            if (memcpy_s(nv_buffer + OAL_SLE_VERIFY_NUM_SIZE, MPXX_BFGX_NV_DATA_SIZE - OAL_SLE_VERIFY_NUM_SIZE,
                ini_buf + offset, length) != EOK) {
                ps_print_err("nvram_update_ini_with_verify memcpy failed!");
                return INI_SUCC;
            }
            write_conf_to_nvram(nv_name, nv_number, nv_buffer, OAL_SLE_VERIFY_NUM_SIZE + length);
            ps_print_err("nvram_update_ini_with_verify save pow ini to nv!");
            return INI_SUCC;
        } else {
            return nvram_gen_post_process(nv_buffer + OAL_SLE_VERIFY_NUM_SIZE, offset, length,
                g_cali_data_ops->bfgx_get_cust_ini_data_buf_ops);
        }
    } else {
        return nvram_gen_post_process(nv_buffer, offset, length, g_cali_data_ops->bfgx_get_cust_ini_data_buf_ops);
    }
}
// BT ini保存到NV的数据，从NV加载后，刷新到ini处理函数
STATIC int32_t nvram_update_ini_process(uint32_t nv_number, const char *nv_name, uint32_t offset,
    uint32_t length, uint32_t verify)
{
    uint32_t len = 0;
    uint8_t *ini_buf = g_cali_data_ops->bfgx_get_cust_ini_data_buf_ops(&len);
    uint32_t cali_switch = (*(uint32_t*)(ini_buf + OAL_BT_CALI_SWITCH_OFFSET)) & OAL_BT_CALI_SWITCH_MASK;

    // ini开关控制
    if (cali_switch != 0) {
        return nvram_update_ini_with_verify(nv_number, nv_name, offset, length, verify);
    } else {
        return INI_SUCC;
    }
}
#endif

/*
 * 函 数 名  : bfgx_nv_data_init
 * 功能描述  : bt 校准NV读取
 */
STATIC int32_t bfgx_nv_data_init(void)
{
#ifdef _PRE_WINDOWS_SUPPORT
    return INI_SUCC;
#else
    int i;
    int32_t l_ret = INI_SUCC;
    ps_print_info("%s\n", __func__);

    for (i = 0; i < oal_array_size(g_bfgx_nv_table); i++) {
        if (g_bfgx_nv_table[i].post_process != NULL) {
            l_ret = g_bfgx_nv_table[i].post_process(g_bfgx_nv_table[i].nv_number, g_bfgx_nv_table[i].nv_name,
                                                    g_bfgx_nv_table[i].offset, g_bfgx_nv_table[i].length,
                                                    g_bfgx_nv_table[i].verify);
            if (l_ret != INI_SUCC) {
                return l_ret;
            }
            ps_print_info("bfgx_nv_data_init nv_number:%d SUCCESS", g_bfgx_nv_table[i].nv_number);
        }
    }
    return INI_SUCC;
#endif
}
#endif

/*
 * 函 数 名  : save_get_bfgx_cali_data
 * 功能描述  : 返回保存bfgx校准数据的内存首地址以及长度
 * 输入参数  : uint8  *buf:调用函数保存bfgx校准数据的首地址
 *            uint32_t buf_len:buf的长度
 * 返 回 值  : 0表示成功，-1表示失败
 */
int32_t save_get_bfgx_cali_data(uint8_t *buf, uint32_t buf_len)
{
    uint32_t bfgx_cali_data_len;

    ps_print_info("%s\n", __func__);

#ifdef HISI_NVRAM_SUPPORT
    if (bfgx_nv_data_init() != OAL_SUCC) {
        ps_print_err("bfgx nv data init fail!\n");
    }
#endif

    bfgx_cali_data_len = (uint32_t)g_cali_data_ops->bfgx_cali_data_len;
    if (buf_len < bfgx_cali_data_len) {
        ps_print_err("bfgx cali buf len[%d] is smaller than struct size[%d]\n", buf_len, bfgx_cali_data_len);
        return -EFAIL;
    }

    if (memcpy_s(buf, buf_len, g_bfgx_cali_data_buf, bfgx_cali_data_len) != EOK) {
        ps_print_err("bfgx cali data copy fail, buf len = [%d], bfgx_cali_data_len = [%d]\n",
                     buf_len, bfgx_cali_data_len);
        return -EFAIL;
    }

    return SUCC;
}

STATIC int32_t get_board_specify(uint32_t fw_addr, uint8_t *index)
{
    int32_t subchip_type = get_mpxx_subchip_type();
    if (subchip_type < BOARD_VERSION_MP13 || subchip_type >= BOARD_VERSION_MPXX_BUTT) {
        ps_print_err("subchip type invalid! subchip=%d\n", subchip_type);
        return -EFAIL;
    }

    if (subchip_type == BOARD_VERSION_MP16C) {
        if (fw_addr == 0x4472F800) {
            *index = BUART;
        } else if (fw_addr == 0x444D7800) { // G核蓝牙校准地址
            *index = GUART;
        } else {
            ps_print_err("cali address[0x%x] not adapter, pleas check cfg", fw_addr);
            return -EFAIL;
        }
        return SUCC;
    }

    *index = BUART;
    return SUCC;
}

/*
 * 函 数 名  : get_bfgx_cali_data
 * 功能描述  : 返回保存bfgx校准数据的内存首地址以及长度
 * 输入参数  : uint8  *buf:调用函数保存bfgx校准数据的首地址
 *            uint32_t buf_len:buf的长度
 *            uint32_t fw_addr 固件加载的地址
 * 返 回 值  : 0表示成功，-1表示失败
 */
int32_t get_bfgx_cali_data(uint8_t *buf, uint32_t buf_len, uint32_t fw_addr)
{
    int32_t ret;
    uint8_t uart_index;

    if (unlikely(g_bfgx_cali_data_buf == NULL)) {
        ps_print_err("g_bfgx_cali_data_buf is NULL\n");
        return -EFAIL;
    }

    ret = get_board_specify(fw_addr, &uart_index);
    if (ret < 0) {
        ps_print_err("get board cali fail\n");
        return ret;
    }

    ret = g_cali_data_ops->get_bfgx_cali_data_ops(buf, buf_len, uart_index);
    if (ret < 0) {
        ps_print_err("get cali data fail\n");
        return ret;
    }

    return SUCC;
}

#ifdef _PRE_PLAT_FEATURE_GF6X_PCIE
static uint8_t *gf6x_alloc_dma_buff(uint32_t buffer_len)
{
    oal_pcie_linux_res *res = NULL;
    res = oal_get_default_pcie_handler();
    if ((res != NULL) && (res->comm_res != NULL) && (res->comm_res->pcie_dev != NULL)) {
        return dma_alloc_coherent(&(res->comm_res->pcie_dev->dev), buffer_len,
            (dma_addr_t *)&g_cali_data_buf_phy_addr, GFP_KERNEL);
    }
    ps_print_err("gf61_alloc_dma_buff:: get_pcie_dev fail\n");
    return NULL;
}
#endif

uint32_t get_cali_buf_len(int32_t type)
{
    uint32_t buffer_len = (uint32_t)MP16_MIMO_CALI_DATA_STRU_LEN;

    if (type == BOARD_VERSION_MP17C ||
        ((type == BOARD_VERSION_MP16C) && (get_mpxx_subchip_version() == CHIP_VERSION_MPW))) {
        /* 07pilot/07mpw 校准数据内存长度 */
        buffer_len = MP17C_MIMO_CALI_DATA_STRU_LEN;
    } else if (type == BOARD_VERSION_MP12) {
        buffer_len = MP12_MIMO_CALI_DATA_STRU_LEN;
    }
    return buffer_len;
}

/*
 * 函 数 名  : get_cali_data_buf_addr
 * 功能描述  : 返回保存校准数据的内存地址
 */
void *get_cali_data_buf_addr(void)
{
    int32_t type = get_mpxx_subchip_type();
    uint8_t *buffer = NULL;
    uint32_t ul_buffer_len = get_cali_buf_len(type);

#ifdef _PRE_DUAL_CHIP
    if (hcc_get_main_dev() == HCC_EP_WIFI_DEV1) {
        return get_cali_data_buf_addr_by_dev(HCC_EP_WIFI_DEV1);
    }
#endif
    /* 仅针对MP16、MP16C、GF61 PCIE */
    if ((hcc_is_pcie() == OAL_FALSE) ||
        (type == BOARD_VERSION_MP13 || type == BOARD_VERSION_MP15)) {
        return g_cali_data_buf;
    }

    /* 已申请直接返回 */
    if (g_cali_data_buf_dma != NULL) {
        return g_cali_data_buf_dma;
    }

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
    if (oal_get_wifi_pcie_dev() != NULL) {
        buffer = dma_alloc_coherent(&oal_get_wifi_pcie_dev()->dev, ul_buffer_len,
            &g_cali_data_buf_phy_addr, GFP_KERNEL);
    }
#elif (defined _PRE_PLAT_FEATURE_GF6X_PCIE)
    buffer = gf6x_alloc_dma_buff(ul_buffer_len);
#endif
    if (buffer == NULL) {
        ps_print_err("get_cali_data_buf_addr:: malloc for g_cali_data_buf_dma fail\n");
        return NULL;
    }

    g_cali_data_buf_dma = buffer;
    memset_s(g_cali_data_buf_dma, ul_buffer_len, 0, ul_buffer_len);
    ps_print_info("get_cali_data_buf_addr:: buf(%p) size %d alloc succ", g_cali_data_buf_dma, ul_buffer_len);
    return g_cali_data_buf_dma;
}

/*
 * 函 数 名  : get_cali_data_buf_phy_addr
 * 功能描述  : 返回保存校准数据的DMA物理内存地址
 */
uint64_t get_cali_data_buf_phy_addr(uint32_t dev_id)
{
#ifdef _PRE_DUAL_CHIP
    if (dev_id == HCC_EP_WIFI_DEV1) {
        return g_cali_data_buf_phy_addr_dev1;
    }
#endif
    return g_cali_data_buf_phy_addr;
}

EXPORT_SYMBOL(get_cali_data_buf_phy_addr);
EXPORT_SYMBOL(get_cali_data_buf_addr);
EXPORT_SYMBOL(g_netdev_is_open);
EXPORT_SYMBOL(g_cali_update_channel_info);

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
STATIC int32_t wifi_cali_buf_malloc(void)
{
    uint8_t *buffer = NULL;
    uint32_t ul_buffer_len;
    int32_t type = get_mpxx_subchip_type();
    if ((type == BOARD_VERSION_MP16) || (type == BOARD_VERSION_MP16C) ||
        (type == BOARD_VERSION_GF61)) {
        if ((type == BOARD_VERSION_MP16C) && (get_mpxx_subchip_version() == CHIP_VERSION_MPW)) {
            /* 07mpw */
            ul_buffer_len = (uint32_t)MP17C_MIMO_CALI_DATA_STRU_LEN;
        } else {
            ul_buffer_len = (uint32_t)MP16_MIMO_CALI_DATA_STRU_LEN;
        }
    } else if (type == BOARD_VERSION_MP15) {
        ul_buffer_len = OAL_MIMO_CALI_DATA_STRU_LEN;
    } else if (type == BOARD_VERSION_MP13) {
        ul_buffer_len = OAL_DOUBLE_CALI_DATA_STRU_LEN;
    } else if (type == BOARD_VERSION_MP17C) {
        ul_buffer_len = MP17C_MIMO_CALI_DATA_STRU_LEN;
    } else if (type == BOARD_VERSION_MP12) {
        ul_buffer_len = MP12_MIMO_CALI_DATA_STRU_LEN;
    } else {
        // default max
        ps_print_info("can not get chip type, use default max size of cali data struct\n");
        ul_buffer_len = (uint32_t)DEFAULT_CALI_DATA_STRU_LEN;
    }

    if (g_cali_data_buf == NULL) {
        buffer = (uint8_t *)os_kzalloc_gfp(ul_buffer_len);
        if (buffer == NULL) {
            ps_print_err("malloc for g_cali_data_buf fail\n");
            return -ENOMEM;
        }
        g_cali_data_buf = buffer;
        memset_s(g_cali_data_buf, ul_buffer_len, 0, ul_buffer_len);
        ps_print_info("g_cali_data_buf(%p) size %d alloc in sysfs init", g_cali_data_buf, ul_buffer_len);
    } else {
        ps_print_info("g_cali_data_buf size %d alloc in sysfs init\n", ul_buffer_len);
    }

    return SUCC;
}
#endif

STATIC int32_t bfgx_cali_buf_malloc(void)
{
    uint8_t *buffer = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        buffer = (uint8_t *)os_kzalloc_gfp(g_cali_data_ops->bfgx_cali_data_len);
        if (buffer == NULL) {
            os_mem_kfree(g_cali_data_buf);
            g_cali_data_buf = NULL;
            ps_print_err("malloc for g_bfgx_cali_data_buf fail\n");
            return -ENOMEM;
        }
        g_bfgx_cali_data_buf = buffer;
    } else {
        ps_print_info("g_bfgx_cali_data_buf alloc in sysfs init\n");
    }

    return SUCC;
}

/*
 * 函 数 名  : cali_data_buf_malloc
 * 功能描述  : 分配保存校准数据的内存
 * 返 回 值  : 0表示分配成功，-1表示分配失败
 */
int32_t cali_data_buf_malloc(void)
{
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    if (wifi_cali_buf_malloc() < 0) {
        return -ENOMEM;
    }
#endif
#ifdef _PRE_DUAL_CHIP
    if (wifi_cali_buf_malloc_dev1() < 0) {
        return -ENOMEM;
    }
#endif

    if (bfgx_cali_buf_malloc() < 0) {
        return -ENOMEM;
    }

    return SUCC;
}

/*
 * 函 数 名  : cali_data_buf_free
 * 功能描述  : 释放保存校准数据的内存
 */
void cali_data_buf_free(void)
{
#if (defined(_PRE_PLAT_FEATURE_HI110X_PCIE) || defined(_PRE_PLAT_FEATURE_GF6X_PCIE))
    int32_t type = get_mpxx_subchip_type();
    uint32_t ul_buffer_len = get_cali_buf_len(type);
#endif

    if (g_cali_data_buf != NULL) {
        os_mem_kfree(g_cali_data_buf);
        g_cali_data_buf = NULL;
    }

    if (g_cali_data_buf_dma != NULL) {
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
        if (oal_get_wifi_pcie_dev() != NULL) {
            dma_free_coherent(&oal_get_wifi_pcie_dev()->dev,
                ul_buffer_len, g_cali_data_buf_dma, g_cali_data_buf_phy_addr);
        }
#elif (defined _PRE_PLAT_FEATURE_GF6X_PCIE)
        dma_free_coherent(&(oal_get_default_pcie_handler())->comm_res->pcie_dev->dev,
            ul_buffer_len, g_cali_data_buf_dma, g_cali_data_buf_phy_addr);
#endif
        g_cali_data_buf_dma = NULL;
    }

    g_cali_data_buf_phy_addr = 0;
    if (g_bfgx_cali_data_buf != NULL) {
        os_mem_kfree(g_bfgx_cali_data_buf);
        g_bfgx_cali_data_buf = NULL;
    }

#ifdef _PRE_DUAL_CHIP
    cali_data_buf_free_dev1();
#endif
}

/*
 * 函 数 名  : wait_bfgx_cali_data
 * 功能描述  : 等待接受device发送的校准数据
 * 返 回 值  : 0表示成功，-1表示失败
 */
STATIC int32_t wait_bfgx_cali_data(void)
{
#define WAIT_BFGX_CALI_DATA_TIME 2000
    unsigned long timeleft;

    timeleft = wait_for_completion_timeout(&g_cali_recv_done, msecs_to_jiffies(WAIT_BFGX_CALI_DATA_TIME));
    if (!timeleft) {
        ps_print_err("wait bfgx cali data timeout\n");
        return -ETIMEDOUT;
    }

    return 0;
}

STATIC bfgx_ini_cmd *bfgx_cust_get_ini(void)
{
    int32_t l_subchip_type = get_mpxx_subchip_type();
    bfgx_ini_cmd *p_bfgx_ini_cmd = NULL;

    switch (l_subchip_type) {
        case BOARD_VERSION_MP13:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_mp13;
            ps_print_dbg("bfgx load ini MP13");
            break;
        case BOARD_VERSION_MP15:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_mp15;
            ps_print_dbg("bfgx load ini MP15");
            break;
        case BOARD_VERSION_MP16:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_mp16;
            ps_print_dbg("bfgx load ini MP16");
            break;
        case BOARD_VERSION_MP16C:
        case BOARD_VERSION_GF61:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_mp16c;
            ps_print_dbg("bfgx load ini GF61&MP16C");
            break;
        case BOARD_VERSION_MP17C:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_mp17c;
            ps_print_dbg("bfgx load ini MP17C");
            break;
        case BOARD_VERSION_MP12:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_mp12;
            ps_print_dbg("bfgx load ini MP12");
            break;
        default:
            ps_print_err("bfgx load ini error: No corresponding chip was found, chiptype %d", l_subchip_type);
            break;
    }
    return p_bfgx_ini_cmd;
}

/*
 * 函 数 名  : bfgx_cust_ini_init
 * 功能描述  : bt校准定制化项初始化
 * 返 回 值  : 0表示成功，-1表示失败
 */
STATIC int32_t bfgx_cust_ini_init(void)
{
    int32_t i;
    int32_t l_ret = INI_FAILED;
    int32_t l_cfg_value = 0;
    int32_t l_ori_val;
    char *pst_buf = NULL;
    uint32_t len;
    bfgx_ini_cmd *p_bfgx_ini_cmd = bfgx_cust_get_ini();
    if (p_bfgx_ini_cmd == NULL) {
        return INI_FAILED;
    }

    for (i = 0; i < (MPXX_BFGX_BT_CUST_INI_SIZE / 4); i++) { /* 512/4转换为字节 */
        /* 定制化项目列表名称以 null 结尾 */
        if (p_bfgx_ini_cmd[i].name == NULL) {
            ps_print_info("bfgx ini load end count: %d", i);
            break;
        }

        l_ori_val = p_bfgx_ini_cmd[i].init_value;

        /* 获取ini的配置值 */
        l_ret = get_cust_conf_int32(INI_MODU_DEV_BT, p_bfgx_ini_cmd[i].name, &l_cfg_value);
        if (l_ret == INI_FAILED) {
            g_bfgx_cust_ini_data[i] = l_ori_val;
            ps_print_dbg("bfgx read ini file failed cfg_id[%d],default value[%d]!", i, l_ori_val);
            continue;
        }

        g_bfgx_cust_ini_data[i] = l_cfg_value;

        ps_print_info("bfgx ini init [id:%d] [%s] changed from [%d]to[%d]",
                      i, p_bfgx_ini_cmd[i].name, l_ori_val, l_cfg_value);
    }

    pst_buf = g_cali_data_ops->bfgx_get_cust_ini_data_buf_ops(&len);
    if (pst_buf == NULL) {
        ps_print_err("get cust ini buf fail!");
        return INI_FAILED;
    }

    l_ret = memcpy_s(pst_buf, len, g_bfgx_cust_ini_data, sizeof(g_bfgx_cust_ini_data));
    if (l_ret != EOK) {
        ps_print_err("data buff not enough\n");
        return INI_FAILED;
    }

    return INI_SUCC;
}

/*
 * 函 数 名  : bfgx_customize_init
 * 功能描述  : bfgx定制化项初始化，读取ini配置文件，读取nv配置项
 * 返 回 值  : 0表示成功，-1表示失败
 */
int32_t bfgx_customize_init(void)
{
    int32_t ret;

    /* 申请用于保存校准数据的buffer */
    ret = cali_data_buf_malloc();
    if (ret != OAL_SUCC) {
        ps_print_err("alloc cali data buf fail\n");
        return INI_FAILED;
    }

    init_completion(&g_cali_recv_done);

    ret = bfgx_cust_ini_init();
    if (ret != OAL_SUCC) {
        ps_print_err("bfgx ini init fail!\n");
        cali_data_buf_free();
        return INI_FAILED;
    }

#ifdef HISI_NVRAM_SUPPORT
    ret = bfgx_nv_data_init();
    if (ret != OAL_SUCC) {
        ps_print_err("bfgx nv data init fail!\n");
        cali_data_buf_free();
        return INI_FAILED;
    }
#endif

    return INI_SUCC;
}

int32_t ps_recv_bt_cali_data(struct ps_core_s *ps_core_d, uint8_t *buf_ptr)
{
    uint16_t rx_pkt_total_len;
    uint32_t bfgx_bt_cali_data_len;
    uint32_t copy_len;
    struct st_bfgx_data *pst_bfgx_data = NULL;

    if (ps_core_d == NULL || buf_ptr == NULL) {
        ps_print_err("ps_core_d or buf is NULL\n");
        return -EINVAL;
    }

    pst_bfgx_data = &ps_core_d->bfgx_info[BFGX_BT];
    if (pst_bfgx_data->cali_buf == NULL) {
        ps_print_err("bt cali buffer is null\n");
        return -EINVAL;
    }
    bfgx_bt_cali_data_len = g_cali_data_ops->bt_cali_data_len;
    rx_pkt_total_len = ps_core_d->rx_pkt_total_len -
                       (uint16_t)(sizeof(struct ps_packet_head) + sizeof(struct ps_packet_end));
    copy_len = (rx_pkt_total_len > bfgx_bt_cali_data_len) ? bfgx_bt_cali_data_len : rx_pkt_total_len;

    ps_print_info("[%s]recv bfgx cali data, rx_len=%d,copy_len=%d\n",
                  index2name(ps_core_d->pm_data->index), rx_pkt_total_len, copy_len);
    if (unlikely(memcpy_s(pst_bfgx_data->cali_buf, bfgx_bt_cali_data_len, buf_ptr, copy_len) != EOK)) {
        ps_print_err("memcopy_s error, destlen=%d, srclen=%d\n ", bfgx_bt_cali_data_len, copy_len);
    }

    complete(&g_cali_recv_done);

    return 0;
}

int32_t bt_cali_buffer_malloc(struct ps_core_s *ps_core_d)
{
    struct st_bfgx_data *pst_bfgx_data = NULL;

    pst_bfgx_data = &ps_core_d->bfgx_info[BFGX_BT];

    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_OPEN) {
        ps_print_warning("%s has opened! ignore bfgx cali!\n", pst_bfgx_data->name);
        return -EINVAL;
    }
    if (pst_bfgx_data->cali_buf != NULL) {
        ps_print_err("%s already cali\n", pst_bfgx_data->name);
        return -EINVAL;
    }

    pst_bfgx_data->cali_buf = (uint8_t *) os_kzalloc_gfp(g_cali_data_ops->bt_cali_data_len);
    if (pst_bfgx_data->cali_buf == NULL) {
        ps_print_err("cali buf malloc fail\n");
        return -ENOMEM;
    }

    return 0;
}

void bt_cali_buffer_free(struct ps_core_s *ps_core_d)
{
    struct st_bfgx_data *pst_bfgx_data = NULL;

    if (ps_core_d == NULL) {
        return;
    }

    pst_bfgx_data = &ps_core_d->bfgx_info[BFGX_BT];
    if (pst_bfgx_data != NULL) {
        os_mem_kfree(pst_bfgx_data->cali_buf);
    }
}


/*
 * 函 数 名  : bt_cali_data_init
 * 功能描述  : 开机打开bt，进行bt校准
 * 返 回 值  : 0表示成功，-1表示失败
 */
int32_t bt_cali_data_init(struct ps_core_s *ps_core_d)
{
    int32_t ret = 0;
    struct pm_top* pm_top_data = pm_get_top();

    ps_print_info("%s\n", __func__);

    mutex_lock(&(pm_top_data->host_mutex));

    ret = hw_bfgx_open(ps_core_d, BFGX_BT);
    if (ret != SUCC) {
        mutex_unlock(&(pm_top_data->host_mutex));
        ps_print_err("bfgx cali, open bt fail\n");
        return ret;
    }

    ret = wait_bfgx_cali_data();
    if (ret != SUCC) {
        ps_print_err("wait bfgx cali fail\n");
    }

    ret = hw_bfgx_close(ps_core_d, BFGX_BT);
    if (ret != SUCC) {
        ps_print_err("bfgx cali, clsoe bt fail\n");
    }
    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;
}

void bt_txbf_get_wl_cali_data_mp16c(void)
{
    unsigned stream, channel, level;
    bt_txbf_cali_param_stru *bt_txbf_cali_data = NULL;
    bt_txbf_cali_common_stru *common_cali_data = NULL;
    uint32_t cali_data_size;
    mp1x_wlan_cali_data_para_stru *wl_cali_data = (mp1x_wlan_cali_data_para_stru *)get_cali_data_buf_addr();

    if (wl_cali_data == NULL || (get_mpxx_subchip_type() != BOARD_VERSION_MP16 &&
        get_mpxx_subchip_type() != BOARD_VERSION_MP16C && get_mpxx_subchip_type() != BOARD_VERSION_GF61)) {
        return;
    }

    bt_txbf_cali_data = (bt_txbf_cali_param_stru *)wifi_get_bt_cali_data_buf(&cali_data_size);
    if (bt_txbf_cali_data == NULL) {
        return;
    }
    ps_print_warning("[BT TxBF]get required Tx calibration result from WLAN.");

    common_cali_data = &bt_txbf_cali_data->common_cali_data;

    // C0
    memcpy_s(&common_cali_data->rc_r_c_cali_data[0], sizeof(wlan_cali_rc_r_c_para_stru),
             &wl_cali_data->common_cali_data.rc_r_c_cali_data[WLAN_CALI_BAND_2G][0],
             sizeof(wlan_cali_rc_r_c_para_stru));
    memcpy_s(&common_cali_data->logen_cali_data[0], sizeof(wlan_cali_logen_para_stru),
             &wl_cali_data->common_cali_data.logen_cali_data_2g[0][WLAN_CALI_BAND_2G],
             sizeof(wlan_cali_logen_para_stru));

    // C1
    memcpy_s(&common_cali_data->rc_r_c_cali_data[1], sizeof(wlan_cali_rc_r_c_para_stru),
             &wl_cali_data->common_cali_data.rc_r_c_cali_data[WLAN_CALI_BAND_2G][1],
             sizeof(wlan_cali_rc_r_c_para_stru));
    memcpy_s(&common_cali_data->logen_cali_data[1], sizeof(wlan_cali_logen_para_stru),
             &wl_cali_data->common_cali_data.logen_cali_data_2g[1][WLAN_CALI_BAND_2G],
             sizeof(wlan_cali_logen_para_stru));

    for (stream = 0; stream < BT_TXBF_CALI_STREAMS; stream++) {
        bt_txbf_cali_stream_stru *current_stream = &bt_txbf_cali_data->stream_cali_data[stream];
        bt_txbf_cali_stream_extend_stru *current_stream_extend = &bt_txbf_cali_data->stream_cali_data_extend[stream];
        mp1x_wlan_cali_2g_20m_save_stru *wl_cali_2g = &wl_cali_data->rf_cali_data[stream].cali_data_2g_20m;

        for (channel = 0; channel < WLAN_2G_CALI_BAND_NUM; channel++) {
            bt_txbf_cali_channel_stru *current_channel = &current_stream->channel_cali_data[channel];
            bt_txbf_cali_channel_extend_stru *current_channel_extend =
                &current_stream_extend->channel_cali_data_extend[channel];
            mp1x_wlan_cali_basic_para_stru *wl_cali_band = &wl_cali_2g->cali_data[channel];

            // use WL calibration result of level 0 to compensate
            level = 0;
            current_channel->txdc_comp_i = wl_cali_band->txdc_cali_data.txdc_cmp_i[level];
            current_channel->txdc_comp_q = wl_cali_band->txdc_cali_data.txdc_cmp_q[level];
            memcpy_s(&current_channel->txiq_comp_siso, sizeof(wlan_cali_iq_coef_stru),
                     &wl_cali_band->txiq_cali_data.qmc_data[level].qmc_siso, sizeof(wlan_cali_iq_coef_stru));
            memcpy_s(&current_channel_extend->txpwr_cali_data, sizeof(wlan_cali_txpwr_para_stru),
                     &wl_cali_band->txpwr_cali_data, sizeof(wlan_cali_txpwr_para_stru));
        }
    }
}

uint8_t g_bt_5g_band_sel[] = { 1, 2, 6 }; // WIFI BAND映射BT BAND SEL关系
void bt_get_wl_cali_txpwr_data_mp17c(bt_wl_cali_data_stru *bt_cali_data, mp1x_wlan_cali_data_para_stru *wl_cali_data)
{
    uint8_t wl_band_sel;
    mp1x_wlan_cali_rf_para_stru *rf_cali_data = NULL;
    uint8_t rf_idx;
    uint8_t sub_band_idx;
    for (rf_idx = 0; rf_idx < FE_CALI_ALL_CHN_NUMS; rf_idx++) {
        rf_cali_data = &wl_cali_data->rf_cali_data[rf_idx];
        for (sub_band_idx = 0; sub_band_idx < BT_WL_CALI_SUB_BAND_NUM; sub_band_idx++) {
            // 2g
            bt_cali_data->txpwr_cali_data.txpwr_cali_2g[rf_idx][sub_band_idx].ppa_cap_cmp =
                rf_cali_data->cali_data_2g_20m.cali_data[sub_band_idx].txpwr_cali_data.ppa_cap_cmp;
            bt_cali_data->txpwr_cali_data.txpwr_cali_2g[rf_idx][sub_band_idx].ppa_cmp =
                rf_cali_data->cali_data_2g_20m.cali_data[sub_band_idx].txpwr_cali_data.ppa_cmp;
            // 5g
            wl_band_sel = g_bt_5g_band_sel[sub_band_idx];
            bt_cali_data->txpwr_cali_data.txpwr_cali_5g[rf_idx][sub_band_idx].ppa_cap_cmp =
                rf_cali_data->cali_data_5g_20m.cali_data[wl_band_sel].txpwr_cali_data.ppa_cap_cmp;
            bt_cali_data->txpwr_cali_data.txpwr_cali_5g[rf_idx][sub_band_idx].ppa_cmp =
                rf_cali_data->cali_data_5g_20m.cali_data[wl_band_sel].txpwr_cali_data.ppa_cmp;
            ps_print_warning("rf_idx=%d sub_band_idx=%d{ppa_cap_cmp ppa_cmp}=2g[0x%x 0x%x]5g[0x%x 0x%x]",
                rf_idx, sub_band_idx,
                bt_cali_data->txpwr_cali_data.txpwr_cali_2g[rf_idx][sub_band_idx].ppa_cap_cmp,
                bt_cali_data->txpwr_cali_data.txpwr_cali_2g[rf_idx][sub_band_idx].ppa_cmp,
                bt_cali_data->txpwr_cali_data.txpwr_cali_5g[rf_idx][sub_band_idx].ppa_cap_cmp,
                bt_cali_data->txpwr_cali_data.txpwr_cali_5g[rf_idx][sub_band_idx].ppa_cmp);
        }
    }
}

void bt_get_wl_lo_beamforming_mp17c(bt_wl_cali_data_stru *bt_cali_data, mp1x_wlan_cali_data_para_ext_stru *wl_cali_data)
{
    uint8_t rf_idx;
    uint8_t sub_band_idx;

    memcpy_s(bt_cali_data->lodiv_cali_data.lodiv_cali_data_2g,
        sizeof(wlan_cali_lodiv_para_stru) * FE_CALI_ALL_CHN_NUMS,
        &wl_cali_data->cali_data.common_cali_data.lodiv_cali_data,
        sizeof(wlan_cali_lodiv_para_stru) * FE_CALI_ALL_CHN_NUMS);

    for (rf_idx = 0; rf_idx < FE_CALI_ALL_CHN_NUMS; rf_idx++) {
        for (sub_band_idx = 0; sub_band_idx < BT_WL_CALI_SUB_BAND_NUM; sub_band_idx++) {
            memcpy_s(&bt_cali_data->lodiv_cali_data.lodiv_cali_data_5g[rf_idx][sub_band_idx],
                sizeof(wlan_cali_logen_para_stru),
                &wl_cali_data->common_cali_data_ext.lodiv_cali_data_5g[rf_idx][g_bt_5g_band_sel[sub_band_idx]],
                sizeof(wlan_cali_logen_para_stru));
            ps_print_warning("rf_idx=%d sub_band_idx=%d {lo_buf_tune lo_buf_tune}=2g[%d %d]5g[%d %d]",
                rf_idx, sub_band_idx,
                bt_cali_data->lodiv_cali_data.lodiv_cali_data_5g[rf_idx][sub_band_idx].tx_lo,
                bt_cali_data->lodiv_cali_data.lodiv_cali_data_5g[rf_idx][sub_band_idx].tx_dpd_lo_tx,
                bt_cali_data->lodiv_cali_data.lodiv_cali_data_5g[rf_idx][sub_band_idx].tx_dpd_lo_dpd,
                bt_cali_data->lodiv_cali_data.lodiv_cali_data_5g[rf_idx][sub_band_idx].rx_lo);
        }
    }
}

void bt_get_wl_fem_abnormal_flag_mp17c(bt_wl_cali_data_stru *bt_cali_data, mp1x_wlan_cali_data_para_stru *wl_cali_data)
{
    uint8_t rf_idx;
    bt_cali_data->fem_abnormal_flag = 0;
    for (rf_idx = 0; rf_idx < FE_CALI_ALL_CHN_NUMS; rf_idx++) {
        bt_cali_data->fem_abnormal_flag |=
            ((wl_cali_data->common_cali_data.rc_r_c_cali_data[WLAN_CALI_BAND_2G][rf_idx].elna_abnormal << rf_idx) |
            (wl_cali_data->common_cali_data.rc_r_c_cali_data[WLAN_CALI_BAND_5G][rf_idx].elna_abnormal <<
                (rf_idx + NUM_4_BITS)));
    }
    ps_print_warning("fem_abnormal_flag=0x%x", bt_cali_data->fem_abnormal_flag);
}


void bt_get_wl_cali_bw_data_mp17c(bt_wl_cali_data_stru *bt_cali_data, mp1x_wlan_cali_data_para_stru *wl_cali_data)
{
    uint8_t rf_idx;
    for (rf_idx = 0; rf_idx < FE_CALI_ALL_CHN_NUMS; rf_idx++) {
        bt_cali_data->rc_cali_data.rc_20m_cmp_code[rf_idx] =
            wl_cali_data->common_cali_data.rc_r_c_cali_data[WLAN_CALI_BAND_2G][rf_idx].rc_20m_cmp_code;
        ps_print_warning("rf_idx=%d {rc_20m_cmp_code}=[0x%x]",
            rf_idx, bt_cali_data->rc_cali_data.rc_20m_cmp_code[rf_idx]);
    }
}

void bt_get_wl_cali_logen_data_mp17c(bt_wl_cali_data_stru *bt_cali_data, mp1x_wlan_cali_data_para_stru *wl_cali_data)
{
    uint8_t rf_idx;
    uint8_t sub_band_idx;
    for (rf_idx = 0; rf_idx < FE_CALI_ALL_CHN_NUMS; rf_idx++) {
        for (sub_band_idx = 0; sub_band_idx < BT_WL_CALI_SUB_BAND_NUM; sub_band_idx++) {
            memcpy_s(&bt_cali_data->logen_cali_data.logen_cali_data_2g[rf_idx][sub_band_idx],
                sizeof(wlan_cali_logen_para_stru),
                &wl_cali_data->common_cali_data.logen_cali_data_2g[rf_idx][sub_band_idx],
                sizeof(wlan_cali_logen_para_stru));
            memcpy_s(&bt_cali_data->logen_cali_data.logen_cali_data_5g[rf_idx][sub_band_idx],
                sizeof(wlan_cali_logen_para_stru),
                &wl_cali_data->common_cali_data.logen_cali_data_5g[rf_idx][g_bt_5g_band_sel[sub_band_idx]],
                sizeof(wlan_cali_logen_para_stru));
            ps_print_warning("rf_idx=%d sub_band_idx=%d {lo_buf_tune lo_buf_tune}=2g[%d %d]5g[%d %d]",
                rf_idx, sub_band_idx,
                bt_cali_data->logen_cali_data.logen_cali_data_2g[rf_idx][sub_band_idx].lo_ssb_tune,
                bt_cali_data->logen_cali_data.logen_cali_data_2g[rf_idx][sub_band_idx].lo_vb_tune,
                bt_cali_data->logen_cali_data.logen_cali_data_5g[rf_idx][sub_band_idx].lo_ssb_tune,
                bt_cali_data->logen_cali_data.logen_cali_data_5g[rf_idx][sub_band_idx].lo_vb_tune);
        }
    }
}

void bt_get_wl_cali_lna_sat_data_mp17c(bt_wl_cali_data_stru *bt_cali_data, mp1x_wlan_cali_data_para_stru *wl_cali_data)
{
    uint8_t rf_idx;
    for (rf_idx = 0; rf_idx < FE_CALI_ALL_CHN_NUMS; rf_idx++) {
        bt_cali_data->lna_sat_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_2G][rf_idx] =
            wl_cali_data->common_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_2G][rf_idx];
        bt_cali_data->lna_sat_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_5G][rf_idx] =
            wl_cali_data->common_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_5G][rf_idx];
        ps_print_warning("lna_blk:2g_c0[0x%x]2g_c1[0x%x]5g_c0[0x%x]5g_c1[0x%x]",
            bt_cali_data->lna_sat_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_2G][WLAN_RF_CHANNEL_ZERO].fine_code,
            bt_cali_data->lna_sat_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_2G][WLAN_RF_CHANNEL_ONE].fine_code,
            bt_cali_data->lna_sat_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_5G][WLAN_RF_CHANNEL_ZERO].fine_code,
            bt_cali_data->lna_sat_cali_data.lna_blk_cali_data[WLAN_CALI_BAND_5G][WLAN_RF_CHANNEL_ONE].fine_code);
    }
}

void bt_get_wl_cali_data_mp17c(void)
{
    bt_wl_cali_data_stru *bt_cali_data = NULL;
    uint32_t cali_data_size;
    mp1x_wlan_cali_data_para_ext_stru *cali_data = (mp1x_wlan_cali_data_para_ext_stru *)get_cali_data_buf_addr();
    mp1x_wlan_cali_data_para_stru *wl_cali_data = NULL;
    if (cali_data == NULL) {
        return;
    }
    wl_cali_data = &cali_data->cali_data;
    bt_cali_data = (bt_wl_cali_data_stru *)g_cali_data_ops->wifi_get_bt_cali_data_buf_ops(&cali_data_size);
    if (bt_cali_data == NULL) {
        return;
    }
    ps_print_warning("start to get wl cali data");
    bt_get_wl_cali_lna_sat_data_mp17c(bt_cali_data, wl_cali_data);
    bt_get_wl_cali_bw_data_mp17c(bt_cali_data, wl_cali_data); // BW
    bt_get_wl_cali_logen_data_mp17c(bt_cali_data, wl_cali_data); // LOGEN
    bt_get_wl_cali_txpwr_data_mp17c(bt_cali_data, wl_cali_data); // TX POW
    bt_get_wl_fem_abnormal_flag_mp17c(bt_cali_data, wl_cali_data);
    bt_get_wl_lo_beamforming_mp17c(bt_cali_data, cali_data);
}

void bt_get_wl_cali_data(void)
{
    switch (get_mpxx_subchip_type()) {
        case BOARD_VERSION_MP17C:
            bt_get_wl_cali_data_mp17c();
            break;
        case BOARD_VERSION_MP16:
        case BOARD_VERSION_MP16C:
        case BOARD_VERSION_GF61:
        default:
            bt_txbf_get_wl_cali_data_mp16c();
            break;
    }
}

#ifdef _PRE_DUAL_CHIP
static uint8_t *get_g_cali_data_buf(uint32_t dev_id)
{
    if (dev_id == HCC_EP_WIFI_DEV1) {
        return g_cali_data_buf_dev1;
    }
    return g_cali_data_buf;
}

static uint8_t *get_g_cali_data_buf_dma(uint32_t dev_id)
{
    if (dev_id == HCC_EP_WIFI_DEV1) {
        return g_cali_data_buf_dma_dev1;
    }
    return g_cali_data_buf_dma;
}

static void set_g_cali_data_buf_dma(uint32_t dev_id, uint8_t *p_addr)
{
    if (dev_id == HCC_EP_WIFI_DEV1) {
        g_cali_data_buf_dma_dev1 = p_addr;
        return;
    }
    g_cali_data_buf_dma = p_addr;
}

static void set_g_cali_data_buf_phy_addr(uint32_t dev_id, uint64_t addr_value)
{
    if (dev_id == HCC_EP_WIFI_DEV1) {
        g_cali_data_buf_phy_addr_dev1 = addr_value;
        return;
    }
    g_cali_data_buf_phy_addr = addr_value;
}

static void cali_data_buf_free_dev1(void)
{
#if (defined(_PRE_PLAT_FEATURE_HI110X_PCIE) || defined(_PRE_PLAT_FEATURE_GF6X_PCIE))
    int32_t type = get_mpxx_subchip_type();
    uint32_t ul_buffer_len = get_cali_buf_len(type);
#endif

    if (g_cali_data_buf_dev1 != NULL) {
        os_mem_kfree(g_cali_data_buf_dev1);
        g_cali_data_buf_dev1 = NULL;
    }

    if (g_cali_data_buf_dma_dev1 != NULL) {
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
        if (oal_get_wifi_pcie_dev_by_id(HCC_EP_WIFI_DEV1) != NULL) {
            dma_free_coherent(&(oal_get_wifi_pcie_dev_by_id(HCC_EP_WIFI_DEV1)->dev),
                ul_buffer_len, g_cali_data_buf_dma_dev1, g_cali_data_buf_phy_addr_dev1);
        }
#endif
        g_cali_data_buf_dma_dev1 = NULL;
    }

    g_cali_data_buf_phy_addr_dev1 = 0;
}

/* 当前仅适配双05c，后续有其他芯片的需求再扩展 */
static int32_t wifi_cali_buf_malloc_dev1(void)
{
    uint8_t *buffer_dev1 = NULL;
    uint32_t ul_buffer_len;
    int32_t type = get_mpxx_subchip_type();
    if (type == BOARD_VERSION_MP15) {
        ul_buffer_len = OAL_MIMO_CALI_DATA_STRU_LEN;
    } else {
        // default max
        ps_print_info("can not get chip type, use default max size of cali data struct\n");
        ul_buffer_len = (uint32_t)DEFAULT_CALI_DATA_STRU_LEN;
    }

    if (g_cali_data_buf_dev1 == NULL) {
        buffer_dev1 = (uint8_t *)os_kzalloc_gfp(ul_buffer_len);
        if (buffer_dev1 == NULL) {
            ps_print_err("malloc for g_cali_data_buf_dev1 fail\n");
            return -ENOMEM;
        }
        g_cali_data_buf_dev1 = buffer_dev1;
        memset_s(g_cali_data_buf_dev1, ul_buffer_len, 0, ul_buffer_len);
        ps_print_info("g_cali_data_buf_dev1(%p) size %d alloc in sysfs init", g_cali_data_buf_dev1, ul_buffer_len);
    } else {
        ps_print_info("g_cali_data_buf_dev1 size %d alloc in sysfs init\n", ul_buffer_len);
    }

    return SUCC;
}

static void *get_cali_data_buf_addr_by_dev(uint32_t dev_id)
{
    int32_t type = get_mpxx_subchip_type();
    uint8_t *buffer = NULL;
    uint32_t ul_buffer_len = get_cali_buf_len(type);
    uint8_t *cali_data_buf_dma_t = NULL;
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
    uint64_t cali_data_buf_phy_addr_t = 0;
#endif

    if (dev_id >= HCC_CHIP_MAX_DEV) {
        ps_print_err("[E]get_cali_data_buf_addr_by_dev:: dev[%d] is exceed!\n", dev_id);
        return NULL;
    }

    /* 仅针对MP16、MP16C、GF61 PCIE */
    if ((hcc_is_pcie() == OAL_FALSE) ||
        (type == BOARD_VERSION_MP13 || type == BOARD_VERSION_MP15)) {
        return get_g_cali_data_buf(dev_id);
    }

    cali_data_buf_dma_t = get_g_cali_data_buf_dma(dev_id);
    /* 已申请直接返回 */
    if (cali_data_buf_dma_t != NULL) {
        return get_g_cali_data_buf_dma(dev_id);
    }

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
    if (oal_get_wifi_pcie_dev_by_id(dev_id) != NULL) {
        buffer = (uint8_t *)dma_alloc_coherent(&(oal_get_wifi_pcie_dev_by_id(dev_id)->dev), ul_buffer_len,
            &cali_data_buf_phy_addr_t, GFP_KERNEL);
    }
#endif
    if (buffer == NULL) {
        ps_print_err("[E]get_cali_data_buf_addr_by_dev:: malloc for dev[%d] g_cali_data_buf_dma fail\n", dev_id);
        return NULL;
    }
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
    set_g_cali_data_buf_phy_addr(dev_id, cali_data_buf_phy_addr_t);
#endif
    memset_s(buffer, ul_buffer_len, 0, ul_buffer_len);
    set_g_cali_data_buf_dma(dev_id, buffer);
    ps_print_info("get_cali_data_buf_addr_by_dev:: dev[%d] buf(%p) size %d alloc succ",
                  dev_id, cali_data_buf_dma_t, ul_buffer_len);
    return get_g_cali_data_buf_dma(dev_id);
}
#endif