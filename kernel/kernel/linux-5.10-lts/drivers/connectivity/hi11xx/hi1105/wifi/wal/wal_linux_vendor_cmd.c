/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : linux ioctl配置命令
 * 创建日期 : 2024年1月19日
 */
#include "wal_linux_vendor_cmd.h"
#include "wal_linux_ioctl.h"
#include "oam_ext_if.h"
#include "hmac_vap.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_VENDOR_CMD_C

const wal_vendor_cmd_entry_stru g_vendor_cmd_table[] = {
#ifdef _PRE_WLAN_FEATURE_ISOLATION
    { "set_isolation", wal_vendor_set_ap_isolation },
#endif
#ifdef _PRE_WLAN_FEATURE_PF_SCH_USER
    { CMD_ATF_SET_USER_PARAM, wal_vendor_set_atf_user_val },
#endif
    { CMD_SET_EDCA_PARAM, wal_vendor_set_edca_param },
    { "blkwhtlst_add", wal_vendor_blacklist_add },
    { "blkwhtlst_del", wal_vendor_blacklist_del },
    { "blkwhtlst_ctl", wal_vendor_blacklist_mode },
#ifdef _PRE_WLAN_FEATURE_QUERY_INFO
    { "query_vap_info", wal_vendor_query_vap_info },
    { "query_user_info", wal_vendor_query_user_info },
#if defined(CONFIG_COMPAT) && defined(_PRE_HISI_HERTZ)
    { "query_all_user_info", wal_vendor_query_all_user_info },
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
    { CMD_SET_VSP_CFG_PARAM, wal_vsp_set_cfg_param },
    { CMD_VSP_SET_NAV_EDCA, wal_vsp_set_nav_edca },
#endif
    { "2040bss_enable", wal_vendor_2040bss_enable },
#ifdef _PRE_WLAN_FEATURE_RADAR_BLOCK_CHECK
    { "get_radar_chn_block_status", wal_vendor_get_radar_block_status },
#endif
    { CMD_SET_AI_CSMA_EDCA_PARAM, wal_vendor_set_ai_csma_edca_param },
#ifdef _PRE_WLAN_FEATURE_GET_STATION_INFO_EXT
    { "SET_REPORT_STATION_INFO_CDF", wal_vendor_set_station_info_ext_cdf_th },
#endif
#ifdef _PRE_WLAN_CHBA_MGMT
    { "SERVICE_ASK_CHBA_USER_ALL_AWAKE", wal_ioctl_chba_service_ask_user_all_awake },
#endif
};

int32_t wal_ioctl_cmd_process(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd)
{
    int32_t result;
    uint32_t cmd_name_len = 0;
    uint32_t i;

    if (oal_any_null_ptr3(net_dev, priv_cmd, cmd)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (i = 0; i < oal_array_size(g_vendor_cmd_table); i++) {
        if (oal_strncasecmp(cmd, g_vendor_cmd_table[i].ioctl_cmd, strlen(g_vendor_cmd_table[i].ioctl_cmd)) != 0) {
            continue;
        }
        /* 只跳过命令头，若命令头后面有空格，需要自己跳过 */
        cmd_name_len = strlen(g_vendor_cmd_table[i].ioctl_cmd);
        result = g_vendor_cmd_table[i].func(net_dev, priv_cmd, cmd, cmd_name_len);
        if (result != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_ANY, "{wal_ioctl_cmd_process::ioctl_cmd_exec_failed[%d], idx[%d]!}", result, i);
        }
        return result;
    }

    oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_cmd_process::ioctl_cmd_entry_not_found!}");
    /* 找不到返回成功，和原来保持一致 */
    return OAL_SUCC;
}
