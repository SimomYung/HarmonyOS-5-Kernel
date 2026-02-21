/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : wifi定制化函数实现
 * 作者       : wifi
 * 创建日期   : 2020年5月20日
 */

/* 头文件包含 */
#include "hisi_customize_wifi_mp17c.h"
#include "hmac_cali_mgmt.h"


#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HISI_CUSTOMIZE_WIFI_MP17_C

/*
 * 函 数 名  : hmac_set_scan_dbdc_cali_data
 * 功能描述  : 获取20m(扫描)下发DBDC校准数据
 */
void hmac_set_scan_dbdc_cali_data_mp17c(void)
{
    uint8_t *mem_addr = get_cali_data_buf_addr();
    mp1x_wlan_cali_data_para_stru *cali_data = (mp1x_wlan_cali_data_para_stru *)mem_addr;
    uint8_t rf_idx;
    uint8_t chn_idx;
    wlan_cali_band_enum_uint8 wlan_cali_band;
    uint8_t rf_slave_idx = hmac_get_slave_scan_rf_start_idx();
    uint8_t rf_slave_scan_num = hmac_get_slave_scan_rf_num();
    wlan_cali_20m_basic_para_stru *scan_cali_data = NULL;
    wlan_cali_dbdc_basic_para_stru *dbdc_cali_data = NULL;
    uint8_t chn_num[] = {WLAN_2G_CALI_BAND_NUM, WLAN_5G_20M_CALI_BAND_NUM, WLAN_6G_20M_CALI_BAND_NUM};

    for (rf_idx = rf_slave_idx; rf_idx < rf_slave_idx + rf_slave_scan_num; rf_idx++) {
        for (wlan_cali_band = WLAN_CALI_BAND_2G; wlan_cali_band < HAL_MAX_BAND_NUM; wlan_cali_band++) {
            for (chn_idx = 0; chn_idx < chn_num[wlan_cali_band]; chn_idx++) {
                scan_cali_data = hmac_get_scan_basic_cali_data_addr(rf_idx, wlan_cali_band, chn_idx);
                dbdc_cali_data = hmac_get_dbdc_basic_cali_data_addr(cali_data, rf_idx, wlan_cali_band, chn_idx);
                hmac_set_scan_dbdc_basic_cali_data(scan_cali_data, dbdc_cali_data);
            }
        }
    }
}

void wal_send_cali_data_mp17c(oal_net_device_stru *cfg_net_dev)
{
    h2d_cali_trans_stru h2d_scan_cali_trans = {0};
    h2d_cali_trans_stru h2d_scan_cali_trans_slave = {0};
    h2d_cali_trans_stru h2d_comm_cali_trans = {0};

    hmac_set_scan_cali_data(oal_net_dev_priv(cfg_net_dev));
    h2d_scan_cali_trans.cali_data_type = WLAN_SCAN_ALL_CHN;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_scan_cali_trans);
    /* 下发辅路扫描校准数据 */
    hmac_set_scan_dbdc_cali_data_mp17c();
    h2d_scan_cali_trans_slave.cali_data_type = WLAN_SCAN_ALL_CHN_SLAVE;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_scan_cali_trans_slave);
    h2d_comm_cali_trans.cali_data_type = WLAN_CALI_CTL;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_comm_cali_trans);
    h2d_comm_cali_trans.cali_data_type = WLAN_CALI_CTL_EXT;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_comm_cali_trans);
}
