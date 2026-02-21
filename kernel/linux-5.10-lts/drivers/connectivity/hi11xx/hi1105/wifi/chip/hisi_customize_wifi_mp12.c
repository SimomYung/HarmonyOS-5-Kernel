/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : wifi定制化函数实现
 * 作者       : wifi
 * 创建日期   : 2024年8月02日
 */

/* 头文件包含 */
#include "hisi_customize_wifi_mp12.h"
#include "hmac_cali_mgmt.h"


#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HISI_CUSTOMIZE_WIFI_MP12_C

void wal_send_cali_data_mp12(oal_net_device_stru *cfg_net_dev)
{
    h2d_cali_trans_stru h2d_scan_cali_trans = {0};
    h2d_cali_trans_stru h2d_comm_cali_trans = {0};

    hmac_set_scan_cali_data_mp12(oal_net_dev_priv(cfg_net_dev));
    h2d_scan_cali_trans.cali_data_type = WLAN_SCAN_ALL_CHN;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_scan_cali_trans);
    h2d_comm_cali_trans.cali_data_type = WLAN_CALI_CTL;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_comm_cali_trans);
    h2d_comm_cali_trans.cali_data_type = WLAN_CALI_CTL_EXT;
    hmac_send_cali_data_mp16(oal_net_dev_priv(cfg_net_dev), &h2d_comm_cali_trans);
}
