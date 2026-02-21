/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : connect新增命令特性相关
 * 作    者 :
 * 创建日期 : 2024年2月6日
 */
#include "wal_linux_vendor.h"
#include "oam_ext_if.h"
#include "wal_linux_ioctl.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_VENDOR_CONNECT_C

static int32_t wal_vendor_blacklist_set(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len, uint32_t blkwhtlit_handle)
{
    int32_t ret;
    uint16_t hdr_len = 0;
    uint8_t *para_mac_address = NULL;
    wal_msg_write_stru write_msg;
    mac_blacklist_stru *blklst_stru = NULL;

    if (oal_any_null_ptr3(net_dev, priv_cmd, cmd)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_blacklist_set::parameter is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (wal_ioctl_judge_input_param_length(*priv_cmd, cmd_name_len, OAL_MAC_ADDR_LEN) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_blacklist_set::length is too short!}");
        return OAL_FAIL;
    }
    para_mac_address = cmd + cmd_name_len + 1; // 1 表示ioctl命令中 指令名称和指令内容之间的空格长度

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    memset_s((uint8_t *)&write_msg, sizeof(write_msg), 0, sizeof(write_msg));
    blklst_stru = (mac_blacklist_stru *)(write_msg.auc_value);
    if (memcpy_s(blklst_stru->auc_mac_addr, sizeof(blklst_stru->auc_mac_addr), para_mac_address,
        sizeof(blklst_stru->auc_mac_addr)) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_blacklist_set::memcpy_s error!}");
        return OAL_FAIL;
    }
    hdr_len = sizeof(mac_blacklist_stru);
    wal_write_msg_hdr_init(&write_msg, blkwhtlit_handle, hdr_len);

    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + hdr_len,
        (uint8_t *)&write_msg, OAL_FALSE, NULL);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log2(0, OAM_SF_CFG, "{wal_vendor_blacklist_set:wal_send_cfg_event handle[%d] return[%d].}",
            blkwhtlit_handle, ret);
        return ret;
    }
    return OAL_SUCC;
}

int32_t wal_vendor_blacklist_add(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len)
{
    return wal_vendor_blacklist_set(net_dev, priv_cmd, cmd, cmd_name_len, WLAN_CFGID_ADD_BLACK_LIST);
}

int32_t wal_vendor_blacklist_del(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len)
{
    return wal_vendor_blacklist_set(net_dev, priv_cmd, cmd, cmd_name_len, WLAN_CFGID_DEL_BLACK_LIST);
}

int32_t wal_vendor_blacklist_mode(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len)
{
    int32_t ret;
    uint16_t hdr_len = 0;
    uint8_t para_mode = 0;
    wal_msg_write_stru write_msg;

    if (oal_any_null_ptr3(net_dev, priv_cmd, cmd)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_blacklist_mode::parameter is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (wal_ioctl_judge_input_param_length(*priv_cmd, cmd_name_len, sizeof(para_mode)) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_blacklist_mode::length is too short!}");
        return OAL_FAIL;
    }
    para_mode = *(cmd + cmd_name_len + 1); // 1 表示ioctl命令中 指令名称和指令内容之间的空格长度

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    memset_s((uint8_t *)&write_msg, sizeof(write_msg), 0, sizeof(write_msg));
    write_msg.auc_value[0] = para_mode;
    hdr_len = sizeof(para_mode);
    wal_write_msg_hdr_init(&write_msg, WLAN_CFGID_BLACKLIST_MODE, hdr_len);

    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + hdr_len,
        (uint8_t *)&write_msg, OAL_FALSE, NULL);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_vendor_blacklist_del:wal_send_cfg_event return[%d].}", ret);
        return ret;
    }

    return OAL_SUCC;
}


int32_t wal_vendor_2040bss_enable(oal_net_device_stru *net_dev, wal_wifi_priv_cmd_stru *priv_cmd, uint8_t *cmd,
    uint32_t cmd_name_len)
{
    int32_t ret;
    uint8_t para_mode = 0;
    wal_msg_write_stru write_msg;

    if (oal_any_null_ptr3(net_dev, priv_cmd, cmd)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_2040bss_enable::parameter is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (wal_ioctl_judge_input_param_length(*priv_cmd, cmd_name_len, sizeof(para_mode)) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_vendor_2040bss_enable::length is too short!}");
        return OAL_FAIL;
    }
    para_mode = *(cmd + cmd_name_len + 1); // 1 表示ioctl命令中 指令名称和指令内容之间的空格长度

    /***************************************************************************
                                抛事件到wal层处理
    ***************************************************************************/
    memset_s((uint8_t *)&write_msg, sizeof(write_msg), 0, sizeof(write_msg));
    write_msg.auc_value[0] = para_mode;
    wal_write_msg_hdr_init(&write_msg, WLAN_CFGID_2040BSS_ENABLE, sizeof(uint8_t));

    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(uint8_t),
        (uint8_t *)&write_msg, OAL_FALSE, NULL);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_vendor_2040bss_enable:wal_send_cfg_event return[%d].}", ret);
        return ret;
    }

    return OAL_SUCC;
}
