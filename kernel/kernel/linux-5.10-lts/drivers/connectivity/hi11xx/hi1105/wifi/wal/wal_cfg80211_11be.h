/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : wal_cfg80211_11be.c 的头文件
 * 作    者 : wifi7
 * 创建日期 : 2024年1月13日
 */

#ifndef WAL_CFG80211_11BE_H
#define WAL_CFG80211_11BE_H

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "oal_net.h"
#include "mac_device.h"

typedef struct {
    uint32_t bind_with_ml_netdev : 1;
    uint32_t link_id             : 8;
    uint32_t resv                : 23;
} mlo_netdev_cfg_param;

oal_net_device_stru *wal_mld_get_netdev_by_ap_link_id(oal_net_device_stru *ml_netdev, int8_t ap_link_id);
int32_t wal_init_mlo_link_vap(oal_net_device_stru *net_dev);
void wal_cfg80211_init_mlo_netdev(mac_device_stru *mac_device, oal_wiphy_stru *wiphy);
int32_t wal_deinit_mlo_link_vap(oal_net_device_stru *net_dev);
uint32_t wal_cofeature_state_notify(frw_event_mem_stru *event_mem);
#endif
