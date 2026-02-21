/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_stat.c
 * 作    者 :
 * 创建日期 : 2024年11月22日
 */

/* 1 头文件包含 */
#include "dpe_wlan_stat.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DPE_WLAN_STAT_C

/* device级别统计量，不区分主辅路 */
dpe_wlan_device_stat_stru g_dpe_wlan_device_stats;

dpe_wlan_device_stat_stru *dpe_wlan_device_stat_get(void)
{
    return &g_dpe_wlan_device_stats;
}

dpe_wlan_vap_stat_stru *dpe_wlan_vap_stat_get(dpe_wlan_vap_stru *vap)
{
    return (&vap->vap_stats);
}

void dpe_wlan_stat_init_device_stat(dpe_wlan_device_stru *device)
{
    device->device_stats = &g_dpe_wlan_device_stats;
    memset_s(device->device_stats, sizeof(dpe_wlan_device_stru), 0, sizeof(dpe_wlan_device_stru));
}

dpe_wlan_user_stat_stru *dpe_wlan_user_stat_get(dpe_wlan_user_stru *user)
{
    return (&user->user_stats);
}

uint32_t dpe_wlan_vap_stat_sync(uint8_t vap_idx, uint8_t *param, uint32_t len)
{
    dpe_wlan_vap_stru *dpe_vap;
    dpe_wlan_vap_stat_stru *vap_stats;

    if (param == NULL || len < sizeof(dpe_wlan_vap_stat_stru)) {
        oam_error_log0(0, 0, "{dpe_wlan_user_stat_sync::param is invalid!}");
        return OAL_FAIL;
    }

    dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return OAL_FAIL;
    }
    vap_stats = dpe_wlan_vap_stat_get(dpe_vap);
    if (memcpy_s(param, sizeof(dpe_wlan_vap_stat_stru), vap_stats, sizeof(dpe_wlan_vap_stat_stru)) != EOK) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t dpe_wlan_user_stat_sync(uint16_t user_idx, uint8_t *param, uint32_t len)
{
    dpe_wlan_user_stru *dpe_user;
    dpe_wlan_user_stat_stru *user_stats;

    if (param == NULL || len < sizeof(dpe_wlan_user_stat_stru)) {
        oam_error_log0(0, 0, "{dpe_wlan_user_stat_sync::param is invalid!}");
        return OAL_FAIL;
    }

    dpe_user = dpe_wlan_user_get(user_idx);
    if (dpe_user == NULL) {
        return OAL_FAIL;
    }
    user_stats = dpe_wlan_user_stat_get(dpe_user);
    if (memcpy_s(param, sizeof(dpe_wlan_user_stat_stru), user_stats, sizeof(dpe_wlan_user_stat_stru)) != EOK) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

void dpe_wlan_stat_user_rx_netbuf(uint16_t user_idx, oal_netbuf_stru *netbuf)
{
    dpe_wlan_user_stru *user = NULL;
    dpe_wlan_user_stat_stru *user_stats = NULL;

    user = dpe_wlan_user_get(user_idx);
    if (user == NULL) {
        return;
    }
    user_stats = dpe_wlan_user_stat_get(user);

    /* 数据帧统计 */
    user_stats->rx_packets = user_stats->rx_packets + 1;
    user_stats->rx_bytes = user_stats->rx_bytes + oal_netbuf_get_len(netbuf);
}

void hmac_stat_user_tx_netbuf(uint16_t user_idx, oal_netbuf_stru *netbuf)
{
    dpe_wlan_user_stru *user = NULL;
    dpe_wlan_user_stat_stru *user_stats = NULL;
    user = dpe_wlan_user_get(user_idx);
    if (user == NULL) {
        return;
    }
    user_stats = dpe_wlan_user_stat_get(user);

    /* 数据帧统计 */
    user_stats->tx_packets = user_stats->tx_packets + 1;
    user_stats->tx_bytes = user_stats->tx_bytes + oal_netbuf_get_len(netbuf);
}
