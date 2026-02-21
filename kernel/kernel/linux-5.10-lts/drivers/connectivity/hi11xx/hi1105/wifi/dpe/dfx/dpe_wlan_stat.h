/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_stat.c的头文件
 * 作    者 :
 * 创建日期 : 2024年11月22日
 */

#ifndef DPE_WLAN_STAT_H
#define DPE_WLAN_STAT_H

/* 1 头文件包含 */
#include "dpe_wlan_stat_stru.h"
#include "dpe_wlan_device.h"
#include "dpe_wlan_vap.h"
#include "dpe_wlan_user.h"

dpe_wlan_device_stat_stru *dpe_wlan_device_stat_get(void);
void dpe_wlan_stat_init_device_stat(dpe_wlan_device_stru *device);
dpe_wlan_vap_stat_stru *dpe_wlan_vap_stat_get(dpe_wlan_vap_stru *vap);
dpe_wlan_user_stat_stru *dpe_wlan_user_stat_get(dpe_wlan_user_stru *user);
uint32_t dpe_wlan_vap_stat_sync(uint8_t vap_idx, uint8_t *param, uint32_t len);
uint32_t dpe_wlan_user_stat_sync(uint16_t user_idx, uint8_t *param, uint32_t len);
void dpe_wlan_stat_user_rx_netbuf(uint16_t user_idx, oal_netbuf_stru *netbuf);
void hmac_stat_user_tx_netbuf(uint16_t user_idx, oal_netbuf_stru *netbuf);

OAL_STATIC OAL_INLINE void hmac_stat_device_tx_msdu(uint32_t msdu_len)
{
    dpe_wlan_device_stat_stru *device_stats = dpe_wlan_device_stat_get();
    oal_atomic_add(&(device_stats->tx_packets), 1);
    oal_atomic_add(&(device_stats->tx_bytes), msdu_len);
}

OAL_STATIC OAL_INLINE void hmac_stat_device_rx_msdu(uint32_t msdu_len)
{
    dpe_wlan_device_stat_stru *device_stats = dpe_wlan_device_stat_get();
    oal_atomic_add(&(device_stats->rx_packets), 1);
    oal_atomic_add(&(device_stats->rx_bytes), msdu_len);
}

OAL_STATIC OAL_INLINE void hmac_chr_get_rx_checksum_stat(uint32_t *rx_tcp_checksum_fail, uint32_t *rx_ip_checksum_fail)
{
    dpe_wlan_device_stat_stru *device_stats = dpe_wlan_device_stat_get();
    *rx_tcp_checksum_fail = (uint32_t)oal_atomic_read(&device_stats->rx_tcp_checksum_fail);
    *rx_ip_checksum_fail = (uint32_t)oal_atomic_read(&device_stats->rx_ip_checksum_fail);
}

OAL_STATIC OAL_INLINE void dpe_device_rx_checksum_stat(mac_rx_ctl_stru *rx_ctrl)
{
    dpe_wlan_device_stat_stru *device_stats = dpe_wlan_device_stat_get();
    if (rx_ctrl->bit_ptlcs_valid) {
        if (rx_ctrl->bit_ptlcs_pass) {
            oal_atomic_add(&(device_stats->rx_tcp_checksum_pass), 1);
        } else {
            oal_atomic_add(&(device_stats->rx_tcp_checksum_fail), 1);
        }
    }
    if (rx_ctrl->bit_ipv4cs_valid) {
        if (rx_ctrl->bit_ipv4cs_pass) {
            oal_atomic_add(&(device_stats->rx_ip_checksum_pass), 1);
        } else {
            oal_atomic_add(&(device_stats->rx_ip_checksum_fail), 1);
        }
    }
}

/*
 * 更新接收netbuffer统计信息
 */
OAL_STATIC OAL_INLINE void hmac_stat_vap_rx_netbuf(uint8_t vap_idx, oal_netbuf_stru *netbuf)
{
    dpe_wlan_vap_stat_stru  *vap_stats = NULL;
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);

    if ((dpe_vap == NULL) || (netbuf == NULL)) {
        return;
    }
    vap_stats = dpe_wlan_vap_stat_get(dpe_vap);

    /* 数据帧统计 */
    vap_stats->rx_packets = vap_stats->rx_packets + 1;
    vap_stats->rx_bytes = vap_stats->rx_bytes + oal_netbuf_get_len(netbuf);
}

/*
 * 更新发送netbuffer统计信息
 */
OAL_STATIC OAL_INLINE void hmac_stat_vap_tx_netbuf(uint8_t vap_idx, oal_netbuf_stru *netbuf)
{
    dpe_wlan_vap_stat_stru  *vap_stats = NULL;
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);

    if ((dpe_vap == NULL) || (netbuf == NULL)) {
        return;
    }
    vap_stats = dpe_wlan_vap_stat_get(dpe_vap);

    /* 数据帧统计 */
    vap_stats->tx_packets = vap_stats->tx_packets + 1;
    vap_stats->tx_bytes = vap_stats->tx_bytes + oal_netbuf_get_len(netbuf);
}

#endif  /* DPE_WLAN_STAT_H */
