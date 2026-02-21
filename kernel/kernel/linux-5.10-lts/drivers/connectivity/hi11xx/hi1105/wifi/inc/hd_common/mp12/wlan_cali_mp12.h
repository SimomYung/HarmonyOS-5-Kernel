/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : mp12校准数据结构的头文件
 * 作    者 : wifi
 * 创建日期 : 2023年9月4日
 */

#ifndef WLAN_CALI_MP12_H
#define WLAN_CALI_MP12_H

/* 1 其他头文件包含 */
#include "wlan_types.h"

/* 2 宏定义 */
/* 1112天线数 */
#define MP12_HD_EVENT_RF_NUM 2

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */

/* 校准策略/控制参数 */
typedef struct {
    mp16_update_cali_channel_stru cali_update_info;
    uint32_t check_hw_status[MP12_HD_EVENT_RF_NUM];

    uint8_t last_cali_fail_status;
    uint8_t fem_error_band : 3,
            pll1_err_flag : 3,
            curr_bt_on_flag : 1,
            curr_gle_on_flag : 1;
    uint16_t lna_code_abnormal_cnt;
} mp12_wlan_cali_ctl_para_stru;

/* 单通道RXDC VGA校准数据结构体 */
typedef struct {
    wlan_cali_vga_rxdc_para_stru cali_data_2g_20m[WLAN_2G_CALI_BAND_NUM];
    wlan_cali_vga_rxdc_para_stru cali_data_5g_20m[WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_vga_rxdc_para_stru cali_data_5g_80m[WLAN_5G_20M_CALI_BAND_NUM];
} mp12_wlan_cali_rf_vga_rxdc_para_stru;

typedef struct {
    wlan_cali_lodiv_para_stru lodiv_cali_data_5g[MP12_HD_EVENT_RF_NUM][WLAN_5G_20M_CALI_BAND_NUM];
    mp12_wlan_cali_rf_vga_rxdc_para_stru rxdc_vga_cali_data[MP12_HD_EVENT_RF_NUM];
} mp12_wlan_cali_common_ext_stru;

/* 非通道相关(或者非带宽相关)校准数据结构体 */
typedef struct {
    wlan_cali_rc_r_c_para_stru rc_r_c_cali_data[HAL_MAX_BAND_NUM][MP12_HD_EVENT_RF_NUM];
    wlan_cali_lna_blk_para_stru lna_blk_cali_data[HAL_MAX_BAND_NUM][MP12_HD_EVENT_RF_NUM];
    wlan_cali_lodiv_para_stru lodiv_cali_data[MP12_HD_EVENT_RF_NUM];
    wlan_cali_ppf_para_stru ppf_cali_data_5g[MP12_HD_EVENT_RF_NUM][WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_logen_para_stru logen_cali_data_2g[MP12_HD_EVENT_RF_NUM][WLAN_2G_CALI_BAND_NUM];
    wlan_cali_logen_para_stru logen_cali_data_5g[MP12_HD_EVENT_RF_NUM][WLAN_5G_20M_CALI_BAND_NUM];
    uint8_t  pa_current_data_5g[MP12_HD_EVENT_RF_NUM][WLAN_5G_20M_CALI_BAND_NUM];
    mp12_wlan_cali_ctl_para_stru cali_ctl_data;
} mp12_wlan_cali_common_para_stru;

/* 扫描(20m)单个通道校准数据结构体 */
typedef struct {
    wlan_cali_2g_20m_para_stru cali_data_2g;
    wlan_cali_5g_20m_para_stru cali_data_5g;
} mp12_wlan_cali_rf_20m_para_stru;

/* 扫描(20m)comm数据结构体(不区分通道) */
typedef struct {
    mp12_wlan_cali_common_para_stru common_cali_data;
    uint8_t open_wifi_cali;
    uint8_t cali_cnt;
    uint16_t fac_cali_chain;
} mp12_wlan_cali_data_20m_common_para_stru;

/* 单通道校准数据结构体 */
typedef struct {
    wlan_cali_2g_20m_save_stru cali_data_2g_20m;
    wlan_cali_5g_20m_save_stru cali_data_5g_20m;
    wlan_cali_5g_80m_save_stru cali_data_5g_80m;
} mp12_wlan_cali_rf_para_stru;

/* host校准数据结构体 */
typedef struct {
    mp12_wlan_cali_rf_para_stru rf_cali_data[MP12_HD_EVENT_RF_NUM];
    mp12_wlan_cali_common_para_stru common_cali_data;
} mp12_wlan_cali_data_para_stru;

/* 单信道校准数据结构体 */
typedef struct {
    wlan_cali_basic_para_stru cali_data[MP12_HD_EVENT_RF_NUM];
} mp12_wlan_cali_chn_stru;

#endif /* end of wlan_cali_mp12.h */
