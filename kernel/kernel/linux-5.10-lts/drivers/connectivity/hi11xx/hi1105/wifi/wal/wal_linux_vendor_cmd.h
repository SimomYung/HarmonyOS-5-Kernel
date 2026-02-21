/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : wal_linux_ioctl_cmd.c 的头文件
 * 创建日期 : 2024年1月19日
 */
#ifndef WAL_LINUX_VENDOR_CMD_H
#define WAL_LINUX_VENDOR_CMD_H
#include "oal_types.h"
#include "oal_net.h"
#include "wal_linux_vendor.h"

/* ioctl命令表函数 */
typedef int32_t (*wal_ioctl_cmd_func)(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
typedef struct {
    int8_t *ioctl_cmd;
    wal_ioctl_cmd_func func;
} wal_vendor_cmd_entry_stru;

#ifdef _PRE_WLAN_FEATURE_RADAR_BLOCK_CHECK
int32_t wal_vendor_get_radar_block_status(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
#endif

int32_t wal_ioctl_cmd_process(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd);
int32_t wal_vendor_set_ap_isolation(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
int32_t wal_vendor_set_atf_user_val(oal_net_device_stru *net_dev,
    wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *command, uint32_t cmd_name_len);
int32_t wal_vendor_set_edca_param(oal_net_device_stru *net_dev,
    wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *command, uint32_t cmd_name_len);
int32_t wal_vendor_blacklist_add(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
int32_t wal_vendor_blacklist_del(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
int32_t wal_vendor_blacklist_mode(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
int32_t wal_vendor_2040bss_enable(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
#ifdef _PRE_WLAN_FEATURE_QUERY_INFO
int32_t wal_vendor_query_vap_info(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
int32_t wal_vendor_query_user_info(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
int32_t wal_vendor_query_all_user_info(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len);
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
int32_t wal_vsp_set_nav_edca(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *command,
    uint32_t cmd_name_len);
int32_t wal_vsp_set_cfg_param(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *command,
    uint32_t cmd_name_len);
#endif
#endif
