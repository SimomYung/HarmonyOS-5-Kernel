/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : WIFI 芯片差异接口文件
 * 作    者 :
 * 创建日期 : 2020年6月19日
 */

#ifndef WLAN_CHIP_CUSTOM_H
#define WLAN_CHIP_CUSTOM_H

#include "wlan_chip.h"
#include "oneimage.h"
#include "oal_main.h"
#include "hd_event.h"

OAL_STATIC OAL_INLINE uint32_t wlan_chip_custom_cali(void)
{
    if (g_wlan_chip_ops->custom_cali != NULL) {
        return g_wlan_chip_ops->custom_cali();
    }
    return OAL_FAIL;
}
OAL_STATIC OAL_INLINE void wlan_chip_custom_host_cali_data_init(void)
{
    if (g_wlan_chip_ops->custom_cali_data_host_addr_init != NULL) {
        g_wlan_chip_ops->custom_cali_data_host_addr_init();
    }
}
OAL_STATIC OAL_INLINE void wlan_chip_custom_default_init(void)
{
    if (g_wlan_chip_ops->custom_default_init != NULL) {
        g_wlan_chip_ops->custom_default_init();
    }
}
OAL_STATIC OAL_INLINE uint32_t wlan_chip_custom_host_read_cfg_init(void)
{
    if (g_wlan_chip_ops->custom_host_read_cfg_init != NULL) {
        return g_wlan_chip_ops->custom_host_read_cfg_init();
    }
    return OAL_FAIL;
}
OAL_STATIC OAL_INLINE uint32_t wlan_chip_hcc_customize_h2d_data_cfg(void)
{
    if (g_wlan_chip_ops->hcc_customize_h2d_data_cfg != NULL) {
        return g_wlan_chip_ops->hcc_customize_h2d_data_cfg();
    }
    return OAL_FAIL;
}
OAL_STATIC OAL_INLINE void wlan_chip_show_customize_info(void)
{
    if (g_wlan_chip_ops->show_customize_info != NULL) {
        g_wlan_chip_ops->show_customize_info();
    }
}
OAL_STATIC OAL_INLINE void wlan_chip_show_cali_data_info(void)
{
    if (g_wlan_chip_ops->show_cali_data_info != NULL) {
        g_wlan_chip_ops->show_cali_data_info();
    }
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_send_cali_matrix_data(mac_vap_stru *mac_vap)
{
    if (g_wlan_chip_ops->send_cali_matrix_data != NULL) {
        return g_wlan_chip_ops->send_cali_matrix_data(mac_vap);
    }
    return OAL_FAIL;
}
#ifdef HISI_CONN_NVE_SUPPORT
OAL_STATIC OAL_INLINE void wlan_chip_get_nv_dpd_data(void)
{
    if (g_wlan_chip_ops->get_nv_dpd_data) {
        return g_wlan_chip_ops->get_nv_dpd_data();
    }
}
#endif
OAL_STATIC OAL_INLINE void wlan_chip_send_cali_data(oal_net_device_stru *cfg_net_dev)
{
    if (g_wlan_chip_ops->send_cali_data != NULL) {
        return g_wlan_chip_ops->send_cali_data(cfg_net_dev);
    }
}

OAL_STATIC OAL_INLINE void wlan_chip_send_20m_all_chn_cali_data(oal_net_device_stru *cfg_net_dev)
{
    if (g_wlan_chip_ops->send_20m_all_chn_cali_data != NULL) {
        return g_wlan_chip_ops->send_20m_all_chn_cali_data(cfg_net_dev);
    }
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_save_cali_event(frw_event_mem_stru *event_mem)
{
    if (g_wlan_chip_ops->save_cali_event != NULL) {
        return g_wlan_chip_ops->save_cali_event(event_mem);
    }
    return OAL_FAIL;
}
OAL_STATIC OAL_INLINE void wlan_chip_update_cur_chn_cali_data(cali_data_req_stru *cali_data_get)
{
    if (g_wlan_chip_ops->update_cur_chn_cali_data != NULL) {
        g_wlan_chip_ops->update_cur_chn_cali_data(cali_data_get);
    }
}
OAL_STATIC OAL_INLINE void wlan_chip_set_chn_cali_data(wlan_cali_band_enum_uint8 wlan_cali_band,
    uint8_t subband_idx, wlan_bandwidth_type_enum_uint8 bandwidth)
{
    if (g_wlan_chip_ops->set_chn_cali_data != NULL) {
        g_wlan_chip_ops->set_chn_cali_data(wlan_cali_band, subband_idx, bandwidth);
    }
}
OAL_STATIC OAL_INLINE uint8_t *wlan_chip_get_cali_data_send_addr(h2d_cali_trans_stru *h2d_cali_trans)
{
    if (g_wlan_chip_ops->get_cali_data_send_addr != NULL) {
        return g_wlan_chip_ops->get_cali_data_send_addr(h2d_cali_trans);
    }
    return NULL;
}
OAL_STATIC OAL_INLINE int32_t wlan_chip_get_cali_data_send_len(h2d_cali_trans_stru *h2d_cali_trans)
{
    if (g_wlan_chip_ops->get_cali_data_send_len != NULL) {
        return g_wlan_chip_ops->get_cali_data_send_len(h2d_cali_trans);
    }
    return 0;
}
OAL_STATIC OAL_INLINE uint32_t wlan_chip_get_cali_data_size(wlan_d2h_cali_data_type_enum_uint8 data_type)
{
    if (g_wlan_chip_ops->get_cali_data_size != NULL) {
        return g_wlan_chip_ops->get_cali_data_size(data_type);
    }
    return 0;
}
OAL_STATIC OAL_INLINE uint16_t *wlan_chip_save_cali_event_get_cali_times(uint8_t *mem_addr)
{
    if (g_wlan_chip_ops->save_cali_event_get_cali_times != NULL) {
        return g_wlan_chip_ops->save_cali_event_get_cali_times(mem_addr);
    }
    return NULL;
}

OAL_STATIC OAL_INLINE uint32_t wlan_chip_get_reduce_pwr_channel_2g_bitmap(void)
{
    if (g_wlan_chip_ops->get_reduce_pwr_channel_2g_bitmap != NULL) {
        return g_wlan_chip_ops->get_reduce_pwr_channel_2g_bitmap();
    }
    return 0;
}

#endif /* end of wlan_chip_i.h */
