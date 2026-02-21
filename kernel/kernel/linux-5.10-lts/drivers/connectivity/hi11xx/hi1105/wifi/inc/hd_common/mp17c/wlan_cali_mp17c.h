/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : mp17c校准数据结构的头文件
 * 作    者 : wifi
 * 创建日期 : 2023年9月4日
 */

#ifndef WLAN_CALI_MP17C_H
#define WLAN_CALI_MP17C_H

/* 1 其他头文件包含 */
#include "wlan_types.h"

#define WLAN_WUR_RX_DC_LNA_LVL                    18

/* 07c新增不同VGA档位的插值计算，校准数据保存残留dc和斜率K */
typedef struct {
    int16_t intlna_valid_dc_i[WLAN_RX_DC_LNA_LVL];
    int16_t intlna_valid_dc_q[WLAN_RX_DC_LNA_LVL];
    int16_t intlna_valid_k_i[WLAN_RX_DC_LNA_LVL];
    int16_t intlna_valid_k_q[WLAN_RX_DC_LNA_LVL];
    int16_t extlna_valid_dc_i[WLAN_RX_DC_LNA_LVL];
    int16_t extlna_valid_dc_q[WLAN_RX_DC_LNA_LVL];
    int16_t extlna_valid_k_i[WLAN_RX_DC_LNA_LVL];
    int16_t extlna_valid_k_q[WLAN_RX_DC_LNA_LVL];
} wlan_cali_vga_rxdc_para_stru;

/* 单通道RXDC VGA校准数据结构体 */
typedef struct {
    wlan_cali_vga_rxdc_para_stru cali_data_2g_20m[WLAN_2G_CALI_BAND_NUM];
    wlan_cali_vga_rxdc_para_stru cali_data_5g_20m[WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_vga_rxdc_para_stru cali_data_5g_80m[WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_vga_rxdc_para_stru cali_data_5g_160m[WLAN_5G_160M_CALI_BAND_NUM];
} mp1x_wlan_cali_rf_vga_rxdc_para_stru;

/* 07c WUR通路RXDC */
typedef struct {
    // RX DC
    uint8_t analog_rxdc_cmp_lna_i[WLAN_WUR_RX_DC_LNA_LVL];
    uint8_t analog_rxdc_cmp_lna_q[WLAN_WUR_RX_DC_LNA_LVL];
    uint16_t digital_rxdc_cmp_i;
    uint16_t digital_rxdc_cmp_q;
} wlan_cali_wur_rxdc_para_stru;
typedef struct {
    wlan_cali_lodiv_para_stru lodiv_cali_data_5g[HD_EVENT_RF_NUM][WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_wur_rxdc_para_stru wur_cali_data[HD_EVENT_RF_NUM][WLAN_2G_CALI_BAND_NUM];
    wlan_cali_rxdc_para_stru lp_rxdc_cali_date_2g[WLAN_2G_CALI_BAND_NUM];
    wlan_cali_rxdc_para_stru lp_rxdc_cali_date_5g_20m[WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_rxdc_para_stru lp_rxdc_cali_date_5g_80m[WLAN_5G_20M_CALI_BAND_NUM];
    wlan_cali_rxdc_para_stru lp_rxdc_cali_date_5g_160m[WLAN_5G_160M_CALI_BAND_NUM];
    mp1x_wlan_cali_rf_vga_rxdc_para_stru rxdc_vga_cali_data[HD_EVENT_RF_NUM];
    mp1x_wlan_cali_rf_vga_rxdc_para_stru dbdc_rxdc_vga_cali_data[HD_EVENT_SLAVE_RF_NUM];
} mp1x_wlan_cali_common_ext_stru;

/* 单个带宽单个信道基准校准数据结构体(07c扩展) */
typedef struct {
    wlan_cali_vga_rxdc_para_stru  rxdc_cali_data;
} wlan_cali_basic_para_ext_stru;

/* dbdc单个带宽单信道基准校准数据结构体(07c扩展) */
typedef struct {
    wlan_cali_vga_rxdc_para_stru  rxdc_cali_data;
} wlan_cali_dbdc_basic_para_ext_stru;

/* 单信道校准数据结构体(07c扩展) */
typedef struct {
    wlan_cali_basic_para_ext_stru cali_data[HD_EVENT_RF_NUM];
    wlan_cali_dbdc_basic_para_ext_stru dbdc_cali_data[HD_EVENT_SLAVE_RF_NUM];
} wlan_cali_chn_ext_stru;
#endif /* end of wlan_cali_mp17c.h */
