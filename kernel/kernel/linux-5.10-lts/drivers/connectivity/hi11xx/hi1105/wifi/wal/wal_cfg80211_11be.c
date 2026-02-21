/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : cfg80211 be接口
 * 作    者 : wifi7
 * 创建日期 : 2024年1月13日
 */

#include "wal_cfg80211_11be.h"
#include "wal_cfg_vap_ioctl.h"
#include "wal_linux_cfg80211.h"
#include "wal_linux_ioctl.h"
#include "wal_linux_ioctl_macro.h"
#include "wal_linux_netdev_ops.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID      OAM_FILE_ID_WAL_CFG80111_11BE_C

#ifdef _PRE_WLAN_FEATURE_11BE
#ifdef _PRE_WLAN_FEATURE_ML

oal_net_device_stru *wal_mld_get_netdev_by_ap_link_id(oal_net_device_stru *ml_netdev, int8_t ap_link_id)
{
    oal_netdev_priv_stru *ml_netdev_priv = NULL;
    oal_net_device_stru  *link_netdev = NULL;
    oal_netdev_priv_stru *link_netdev_priv = NULL;
    uint8_t idx;

    if (ml_netdev == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{wal_mld_get_netdev_by_ap_link_id::ml_netdev is NULL}");
        return NULL;
    }
    if (ap_link_id < 0) {
        oam_warning_log1(0, OAM_SF_MLD, "{wal_mld_get_netdev_by_ap_link_id::ap link id invalid:%d}", ap_link_id);
        return ml_netdev;
    }
    ml_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(ml_netdev);
    for (idx = 0; idx < WLAN_MAX_ML_LINK_NUM; idx++) {
        link_netdev = ml_netdev_priv->mlo_cfg_info.link_netdev[idx];
        if (link_netdev == NULL) {
            continue;
        }

        link_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);
        if (link_netdev_priv->mlo_cfg_info.ap_link_id == ap_link_id) {
            return link_netdev;
        }
    }

    if (ml_netdev_priv->mlo_cfg_info.ap_link_id == ap_link_id) {
        return ml_netdev;
    }

    return NULL;
}

static void wal_set_link_mac_addr(mac_device_stru *mac_device, oal_net_device_stru *link_netdev)
{
    uint8_t auc_primary_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 };
    oal_netdev_priv_stru *net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);

    if (oal_netdevice_mac_addr(mac_device->st_p2p_info.pst_primary_net_device) == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "wal_set_link_mac_addr::get primary_net_device mac addr fail!");
        return;
    }
    if (memcpy_s(auc_primary_mac_addr, WLAN_MAC_ADDR_LEN,
        oal_netdevice_mac_addr(mac_device->st_p2p_info.pst_primary_net_device), WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_MLD, "wal_set_link_mac_addr::memcpy fail!");
        return;
    }
    if (net_dev_priv->mlo_cfg_info.bind_with_ml_vap == OAL_FALSE) {
        auc_primary_mac_addr[net_dev_priv->mlo_cfg_info.link_id] ^= 0x80; // 0x80代表mac地址第0/2个数值
    }
    oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(link_netdev), auc_primary_mac_addr);
}

static void wal_cfg80211_init_link_info(oal_wiphy_stru *wiphy, mac_device_stru *mac_device,
    oal_net_device_stru *ml_net_device, mlo_netdev_cfg_param *cfg_param)
{
    uint8_t idx;
    oal_netdev_priv_stru *ml_net_dev_priv;
    oal_netdev_priv_stru *link_net_dev_priv;
    oal_net_device_stru  *link_netdev = wal_init_netdev(wiphy, LINK_NETDEV_NAME, NL80211_IFTYPE_STATION);
    if (link_netdev == NULL) {
        return;
    }
    link_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);
    link_net_dev_priv->mlo_cfg_info.link_id          = cfg_param->link_id;
    link_net_dev_priv->mlo_cfg_info.bind_with_ml_vap = cfg_param->bind_with_ml_netdev;
    link_net_dev_priv->mlo_cfg_info.ml_netdev        = ml_net_device;
    link_net_dev_priv->mlo_cfg_info.is_ml_netdev     = OAL_FALSE;
    link_net_dev_priv->mlo_cfg_info.is_link_netdev   = OAL_TRUE;
    link_net_dev_priv->mlo_cfg_info.ap_link_id       = 0xff;

    wal_set_link_mac_addr(mac_device, link_netdev);
    ml_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(ml_net_device);
    for (idx = 0; idx < WLAN_MAX_ML_LINK_NUM; idx++) {
        if (ml_net_dev_priv->mlo_cfg_info.link_netdev[idx]) {
            continue;
        }
        ml_net_dev_priv->mlo_cfg_info.link_netdev[idx] = link_netdev;
        break;
    }

    oam_warning_log1(0, OAM_SF_MLD, "{wal_cfg80211_init_link_info::link[%d]succ!}",
        link_net_dev_priv->mlo_cfg_info.link_id);
}

void wal_cfg80211_init_mlo_netdev(mac_device_stru *mac_device, oal_wiphy_stru *wiphy)
{
    oal_net_device_stru  *ml_net_device = mac_device->st_p2p_info.pst_primary_net_device;
    oal_netdev_priv_stru *ml_net_dev_priv;
    mlo_netdev_cfg_param cfg_param;
    uint8_t link_id;
    uint8_t link_num;

    if (ml_net_device == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "wal_cfg80211_init_mlo_netdev::ml_net_device null!");
        return;
    }
    ml_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(ml_net_device);
    ml_net_dev_priv->mlo_cfg_info.is_ml_netdev   = OAL_TRUE;
    ml_net_dev_priv->mlo_cfg_info.is_link_netdev = OAL_FALSE;
    ml_net_dev_priv->mlo_cfg_info.ap_link_id     = 0xff;
    ml_net_dev_priv->mlo_cfg_info.ml_netdev      = ml_net_device;

    link_num = (g_st_mac_device_custom_cfg.custom_cfg_11be.single_radio_switch) ? 1 : WLAN_MAX_ML_LINK_NUM;
    for (link_id = MAC_TRX_LINK_ID_0; link_id < link_num; link_id++) {
        cfg_param.link_id = link_id;
        cfg_param.bind_with_ml_netdev = link_id == MAC_TRX_LINK_ID_0 ? OAL_TRUE : OAL_FALSE;
        wal_cfg80211_init_link_info(wiphy, mac_device, ml_net_device, &cfg_param);
    }
}

int32_t wal_init_mlo_link_vap(oal_net_device_stru *net_dev)
{
    uint8_t idx;
    int32_t ret = OAL_SUCC;
    oal_net_device_stru *link_netdev;
    oal_netdev_priv_stru *link_net_dev_priv;
    oal_netdev_priv_stru *ml_net_dev_priv  = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(net_dev);

    for (idx = 0; idx < WLAN_MAX_ML_LINK_NUM; idx++) {
        link_netdev = ml_net_dev_priv->mlo_cfg_info.link_netdev[idx];
        if (link_netdev == NULL) {
            continue;
        }
        link_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);
        /* TDO 与ml_net_dev绑定的link需要做什么 */
        if (link_net_dev_priv->mlo_cfg_info.bind_with_ml_vap == OAL_FALSE) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_init_mlo_link_vap::link[%d]start!}",
                link_net_dev_priv->mlo_cfg_info.link_id);
            ret = wal_init_wlan_vap(link_netdev);
            if (ret != OAL_SUCC) {
                oam_error_log1(0, OAM_SF_MLD, "{wal_init_mlo_link_vap::wal_init_wlan_vap%d.}", ret);
                return -OAL_EFAIL;
            }
            if (wal_netdev_start_vap(link_netdev) != OAL_SUCC) {
                oam_warning_log0(0, OAM_SF_ANY, "{wal_init_mlo_link_vap::wal_netdev_start_vap fail}");
                return -OAL_EFAIL;
            }
            oam_warning_log1(0, OAM_SF_MLD, "{wal_init_mlo_link_vap::link[%d]end!}",
                link_net_dev_priv->mlo_cfg_info.link_id);
        }
    }
    return OAL_SUCC;
}

int32_t wal_deinit_mlo_link_vap(oal_net_device_stru *net_dev)
{
    uint8_t idx;
    int32_t ret = OAL_SUCC;
    oal_net_device_stru *link_netdev;
    oal_netdev_priv_stru *link_net_dev_priv;
    oal_netdev_priv_stru *ml_net_dev_priv  = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(net_dev);
    mac_vap_stru *mac_vap = NULL;
    for (idx = 0; idx < WLAN_MAX_ML_LINK_NUM; idx++) {
        link_netdev = ml_net_dev_priv->mlo_cfg_info.link_netdev[idx];
        if (link_netdev == NULL) {
            continue;
        }
        link_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);
        /* TDO 与ml_net_dev绑定的link需要做什么 */
        if (link_net_dev_priv->mlo_cfg_info.bind_with_ml_vap == OAL_FALSE) {
            /* 如果link vap已经为空，则不需要down和del操作 */
            mac_vap = oal_net_dev_priv(link_netdev);
            if (oal_unlikely(mac_vap == NULL)) {
                continue;
            }
            oam_warning_log1(0, OAM_SF_MLD, "{wal_deinit_mlo_link_vap::link[%d]start!}",
                link_net_dev_priv->mlo_cfg_info.link_id);
            if (wal_netdev_down_vap(link_netdev) != OAL_SUCC) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_deinit_mlo_link_vap::down vap fail}");
            }
            ret = wal_deinit_wlan_vap(link_netdev);
            if (ret != OAL_SUCC) {
                oam_error_log1(0, OAM_SF_MLD, "{wal_deinit_mlo_link_vap::wal_init_wlan_vap%d.}", ret);
            }
            oam_warning_log1(0, OAM_SF_MLD, "{wal_deinit_mlo_link_vap::link[%d]end!}",
                link_net_dev_priv->mlo_cfg_info.link_id);
        }
    }
    return ret;
}
#endif
#endif
