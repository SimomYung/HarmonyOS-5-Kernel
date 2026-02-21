/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 :
 * 创建日期 : 2022年11月19日
 */

#ifndef WAL_VAP_CFG_IOCTL_H
#define WAL_VAP_CFG_IOCTL_H
#include "mac_vap.h"
#include "oal_ext_if.h"

#ifdef WIFI_DEBUG_ENABLE

uint32_t wal_hipriv_reset_device(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_reset_operate(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_vap_classify_en(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_vap_classify_tid(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
uint32_t wal_ioctl_set_beacon_interval(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_start_vap(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
#endif
oal_net_device_stru *wal_init_netdev(oal_wiphy_stru *pst_wiphy, const char *dev_name, enum nl80211_iftype en_type);
int32_t wal_set_random_mac_to_mib_event(oal_net_device_stru *net_dev, mac_vap_stru *mac_vap,
    wlan_p2p_mode_enum_uint8 en_p2p_mode);
#endif
