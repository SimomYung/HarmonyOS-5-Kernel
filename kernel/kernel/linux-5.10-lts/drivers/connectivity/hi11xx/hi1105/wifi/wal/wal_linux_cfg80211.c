/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Linux cfg80211接口
 * 作    者 :
 * 创建日期 : 2012年11月8日
 */

#include "securec.h"
#include "securectype.h"
#include "oal_kernel_file.h"
#include "oal_cfg80211.h"
#include "hisi_customize_wifi_mp16.h"

#include "mac_ie.h"
#include "hmac_config.h"
#include "hmac_p2p.h"
#include "hmac_roam_main.h"
#include "hmac_chan_mgmt.h"
#include "hmac_tx_data.h"
#include "hmac_dfx.h"
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_mgmt.h"
#include "hmac_chba_frame.h"
#endif

#include "wal_config_base.h"
#include "wal_linux_cfg80211.h"
#include "wal_linux_cfg80211_band.h"
#include "wal_linux_event.h"
#include "wal_linux_cfgvendor.h"
#include "wal_cfg_ioctl.h"
#include "wal_linux_netdev_ops.h"
#include "wal_regdb.h"
#include "wal_main.h"
#include "host_hal_tas.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID              OAM_FILE_ID_WAL_LINUX_CFG80211_C

#define HI1151_A_RATES      (g_hi1151_rates + 4)
#define HI1151_A_RATES_SIZE 8
#define HI1151_G_RATES      (g_hi1151_rates + 0)
#define HI1151_G_RATES_SIZE 12

/* 设备支持的加密套件 */
OAL_STATIC const uint32_t g_wlan_supported_cipher_suites_without_bip_gmac_128[] = {
    WLAN_CIPHER_SUITE_WEP40,
    WLAN_CIPHER_SUITE_WEP104,
    WLAN_CIPHER_SUITE_TKIP,
    WLAN_CIPHER_SUITE_CCMP,
    WLAN_CIPHER_SUITE_GCMP,
    WLAN_CIPHER_SUITE_GCMP_256,
    WLAN_CIPHER_SUITE_CCMP_256,
    WLAN_CIPHER_SUITE_AES_CMAC,
    WLAN_CIPHER_SUITE_BIP_CMAC_256,
    WLAN_CIPHER_SUITE_SMS4,
    WLAN_CIPHER_SUITE_BIP_GMAC_256,
#ifdef _PRE_WLAN_FEATURE_PWL
    WLAN_CIPHER_SUITE_PWL_CMAC_128,
    WLAN_CIPHER_SUITE_PWL_GCM_128,
#endif
};

OAL_STATIC const uint32_t g_wlan_supported_cipher_suites[] = {
    WLAN_CIPHER_SUITE_WEP40,
    WLAN_CIPHER_SUITE_WEP104,
    WLAN_CIPHER_SUITE_TKIP,
    WLAN_CIPHER_SUITE_CCMP,
    WLAN_CIPHER_SUITE_GCMP,
    WLAN_CIPHER_SUITE_GCMP_256,
    WLAN_CIPHER_SUITE_CCMP_256,
    WLAN_CIPHER_SUITE_AES_CMAC,
    WLAN_CIPHER_SUITE_BIP_CMAC_256,
    WLAN_CIPHER_SUITE_SMS4,
    WLAN_CIPHER_SUITE_BIP_GMAC_128,
    WLAN_CIPHER_SUITE_BIP_GMAC_256,
#ifdef _PRE_WLAN_FEATURE_PWL
    WLAN_CIPHER_SUITE_PWL_CMAC_128,
    WLAN_CIPHER_SUITE_PWL_GCM_128,
#endif
};

#define IEEE80211_IFACE_TYPE_STA_MAX_LIMIT 2
#define IEEE80211_IFACE_TYPE_P2P_GO_CLI_MAX_LIMIT 2
#define IEEE80211_IFACE_TYPE_P2P_DEV_MAX_LIMIT 1
OAL_STATIC oal_ieee80211_iface_limit g_sta_p2p_limits[] = {
    {
        .max = IEEE80211_IFACE_TYPE_STA_MAX_LIMIT,
        .types = BIT(NL80211_IFTYPE_STATION),
    },
    {
        .max = IEEE80211_IFACE_TYPE_P2P_GO_CLI_MAX_LIMIT,
        .types = BIT(NL80211_IFTYPE_P2P_GO) | BIT(NL80211_IFTYPE_P2P_CLIENT),
    },
    {
        .max = IEEE80211_IFACE_TYPE_P2P_DEV_MAX_LIMIT,
        .types = BIT(NL80211_IFTYPE_P2P_DEVICE),
    },
};

#define IEEE80211_IFACE_COMBINATION_STA_P2P_MAX_INTERFACES 3
OAL_STATIC oal_ieee80211_iface_combination g_sta_p2p_iface_combinations[] = {
    {
        .max_interfaces = IEEE80211_IFACE_COMBINATION_STA_P2P_MAX_INTERFACES,
        .limits = g_sta_p2p_limits,
        .n_limits = oal_array_size(g_sta_p2p_limits),
    },
};

/* There isn't a lot of sense in it, but you can transmit anything you like */
static const struct ieee80211_txrx_stypes
    g_wal_cfg80211_default_mgmt_stypes[NUM_NL80211_IFTYPES] = {
        [NL80211_IFTYPE_ADHOC] = {
            .tx = 0xffff,
            .rx = BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4)
        },
        [NL80211_IFTYPE_STATION] = {
            .tx = 0xffff, .rx = BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4)
            | BIT(IEEE80211_STYPE_PROBE_REQ >> BIT_OFFSET_4)
#ifdef _PRE_WLAN_FEATURE_SAE
            | BIT(IEEE80211_STYPE_AUTH >> BIT_OFFSET_4)
#endif
        },
        [NL80211_IFTYPE_AP] = {
            .tx = 0xffff,
            .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_PROBE_REQ >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_DISASSOC >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_AUTH >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_DEAUTH >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4)
        },
        [NL80211_IFTYPE_AP_VLAN] = { /* copy AP */
            .tx = 0xffff,
            .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_PROBE_REQ >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_DISASSOC >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_AUTH >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_DEAUTH >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4)
        },
        [NL80211_IFTYPE_P2P_CLIENT] = {
            .tx = 0xffff,
            .rx = BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_PROBE_REQ >> BIT_OFFSET_4) |
                  BIT(IEEE80211_STYPE_AUTH >> BIT_OFFSET_4)
        },
        [NL80211_IFTYPE_P2P_GO] = {
            .tx = 0xffff,
            .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_REASSOC_REQ >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_PROBE_REQ >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_DISASSOC >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_AUTH >> BIT_OFFSET_4) |
            BIT(IEEE80211_STYPE_DEAUTH >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4)
        },
        [NL80211_IFTYPE_P2P_DEVICE] = {
            .tx = 0xffff,
            .rx = BIT(IEEE80211_STYPE_ACTION >> BIT_OFFSET_4) | BIT(IEEE80211_STYPE_PROBE_REQ >> BIT_OFFSET_4)
        },
};

uint8_t g_uc_cookie_array_bitmap = 0; /* 每个bit 表示cookie array 中是否使用，1 - 已使用；0 - 未使用 */
cookie_arry_stru g_cookie_array[WAL_COOKIE_ARRAY_SIZE];

static void wal_cfg80211_init_ieee80211_iface_combination(void)
{
    uint32_t i;
    for (i = 0; i < oal_array_size(g_sta_p2p_iface_combinations); i++) {
        g_sta_p2p_iface_combinations[i].num_different_channels = g_wlan_spec_cfg->num_different_channels;
    }
}

/*
 * 功能描述  : 是否为隐藏ssid
 * 1.日    期  : 2015年8月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 mac_is_hide_ssid(uint8_t *puc_ssid_ie, uint8_t uc_ssid_len)
{
    return (oal_bool_enum_uint8)((puc_ssid_ie == NULL) || (uc_ssid_len == 0) || (puc_ssid_ie[0] == '\0'));
}

void wal_set_locally_generated(uint8_t vap_id, uint8_t value)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_vap = mac_res_get_hmac_vap(vap_id);
    if (hmac_vap == NULL) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{wal_set_locally_generated::hmac vap has already been del!}");
        return;
    }
    hmac_vap->bit_locally_generated = value;
}

uint8_t wal_get_locally_generated(uint8_t vap_id)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_vap = mac_res_get_hmac_vap(vap_id);
    if (hmac_vap == NULL) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{wal_get_locally_generated::hmac vap has already been del!}");
        return 0;
    }
    return hmac_vap->bit_locally_generated;
}

/*
 * 函 数 名  : wal_cfg80211_vowifi_report
 * 功能描述  : 触发上报vowifi状态切换请求
 * 1.日    期  : 2016年4月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_cfg80211_vowifi_report(frw_event_mem_stru *pst_event_mem)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
    frw_event_stru *pst_event;
    hmac_vap_stru *pst_hmac_vap;
    oal_net_device_stru *netdev = NULL;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_vowifi_report::pst_event_mem is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_vowifi_report:: dfr or s3s4 is processing!}");
        return OAL_FAIL;
    }
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_hmac_vap = mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (pst_hmac_vap == NULL) {
        oam_error_log1(0, OAM_SF_TX, "{wal_cfg80211_vowifi_report::pst_hmac_vap null.vap_id[%d]}",
            pst_event->st_event_hdr.uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    netdev = hmac_vap_get_net_device(pst_hmac_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_vowifi_report::pst_net_device is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 上报vowifi切换申请 */
    oal_cfg80211_vowifi_report(netdev, GFP_KERNEL);
#endif /* (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34)) */

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_CAC_REPORT
static uint32_t wal_cfg80211_cac_msg(oal_net_device_stru *net_dev, hmac_cac_event_stru *cac_event,
    oal_cfg80211_chan_def *chandef)
{
    struct ieee80211_channel *chan = NULL;
    mac_device_stru *mac_device = mac_res_get_dev(0);
    oal_wiphy_stru *wiphy = mac_device->pst_wiphy;
    if (wiphy == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_cac_msg::wiphy is NULL}");
        return OAL_FAIL;
    }
    /* 获取wiphy下的chan */
    chan = oal_ieee80211_get_channel(wiphy, cac_event->freq);
    if (chan == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_cac_msg::master_center_freq[%d]}", cac_event->freq);
        return OAL_FAIL;
    }
    chandef->chan = chan;
    if (oal_any_null_ptr2(net_dev, net_dev->ieee80211_ptr)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_cac_msg::ieee80211_ptr or chandef is NULL}");
        return OAL_FAIL;
    }
    /* 主信道中心频率 */
    chan->center_freq = cac_event->freq;
    if (cac_event->en_bw_mode <= WLAN_BAND_WIDTH_20M) {
        chandef->width = NL80211_CHAN_WIDTH_20;
    } else if (cac_event->en_bw_mode <= WLAN_BAND_WIDTH_40MINUS) {
        chandef->width = NL80211_CHAN_WIDTH_40;
    } else if (cac_event->en_bw_mode <= WLAN_BAND_WIDTH_80MINUSMINUS) {
        chandef->width = NL80211_CHAN_WIDTH_80;
    } else if (cac_event->en_bw_mode <= WLAN_BAND_WIDTH_160MINUSMINUSMINUS) {
        chandef->width = NL80211_CHAN_WIDTH_160;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_cac_msg::error chan width type[%d]}", cac_event->en_bw_mode);
        return OAL_FAIL;
    }
    /* 拓展信道后的中心频率 */
    chandef->center_freq1 = mac_get_center_freq1_from_freq_and_bandwidth(cac_event->freq, cac_event->en_bw_mode);
    chandef->center_freq2 = 0;
    if (memcpy_s(&(net_dev->ieee80211_ptr->chandef), sizeof(oal_cfg80211_chan_def),
        chandef, sizeof(oal_cfg80211_chan_def)) != EOK) {
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_cac_report
 * 功能描述  : 触发上报CAC事件
 * 1.日    期  : 2016年11月7日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_cfg80211_cac_report(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_cac_event_stru *cac_event = NULL;
    uint32_t ret;
    enum nl80211_radar_event event_type;
    oal_cfg80211_chan_def chandef = {0};
    oal_net_device_stru *netdev = NULL;

    if (event_mem == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    event = frw_get_event_stru(event_mem);
    hmac_vap = mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (oal_any_null_ptr2(hmac_vap, hmac_vap->pst_net_device)) {
        oam_error_log0(event->st_event_hdr.uc_vap_id, OAM_SF_ANY, "wal_cfg80211_cac_report:hmac_vap or net_dev null");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 获取上报事件类型 */
    cac_event = (hmac_cac_event_stru *)(event->auc_event_data);
    if (cac_event->en_type == HMAC_CAC_STARTED) {
        event_type = NL80211_RADAR_CAC_STARTED;
    } else if (cac_event->en_type == HMAC_CAC_FINISHED) {
        event_type = NL80211_RADAR_CAC_FINISHED;
    } else if (cac_event->en_type == HMAC_CAC_ABORTED) {
        event_type = NL80211_RADAR_CAC_ABORTED;
    } else {
        return OAL_FAIL;
    }
    netdev = hmac_vap_get_net_device(hmac_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
            "{wal_cfg80211_cac_report::net dev is null}");
        return OAL_FAIL;
    }
    /* 填cfg80211_chan_def结构体 */
    ret = wal_cfg80211_cac_msg(netdev, cac_event, &chandef);
    if (ret != OAL_SUCC) {
        return OAL_FAIL;
    }
    /* 内核CAC上报接口 */
    cfg80211_cac_event(netdev, &chandef, event_type, GFP_ATOMIC);
    oam_warning_log3(0, OAM_SF_ANY, "wal_cfg80211_cac_report:[DFS][type:%d][freq:%d][bw:%d]",
        cac_event->en_type, cac_event->freq, cac_event->en_bw_mode);
    return OAL_SUCC;
}
#else
uint32_t wal_cfg80211_cac_report(frw_event_mem_stru *event_mem)
{
    return OAL_SUCC;
}
#endif  /* _PRE_WLAN_FEATURE_CAC_REPORT */
/*
 * 函 数 名  : wal_p2p_stop_roc
 * 功能描述  : 停止p2p remain on channel
 * 1.日    期  : 2017年07月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
int32_t wal_p2p_stop_roc(mac_vap_stru *pst_mac_vap, oal_net_device_stru *pst_netdev)
{
    hmac_vap_stru *pst_hmac_vap = NULL;
    hmac_device_stru *pst_hmac_device;
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
            "{wal_p2p_stop_roc:: pst_hmac_device[%d] null!}\r\n", pst_mac_vap->uc_device_id);
        return -OAL_EFAIL;
    }

    /* tx mgmt roc 优先级低,可以被自己的80211 roc以及80211 scan打断 */
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
        if (pst_hmac_device->st_scan_mgmt.en_is_scanning != OAL_TRUE) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
                "{wal_p2p_stop_roc::not in scan state but vap is listen state!}");
            return OAL_SUCC;
        }

        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
        if (oal_unlikely(pst_hmac_vap == NULL)) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{wal_p2p_stop_roc:: pst_hmac_vap null!}\r\n");
            return -OAL_EFAIL;
        }
        pst_hmac_vap->en_wait_roc_end = OAL_TRUE;
        oal_init_completion(&(pst_hmac_vap->st_roc_end_ready));
        wal_force_scan_complete(pst_netdev, OAL_TRUE);
        if (oal_wait_for_completion_timeout(&(pst_hmac_vap->st_roc_end_ready),
            (uint32_t)oal_msecs_to_jiffies(200)) == 0) {       /* 200ms */
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{wal_p2p_stop_roc::cancel old roc timeout!}");
            return -OAL_EFAIL;
        }
    }
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_unregister_netdev
 * 功能描述  : 内核去注册指定类型的net_device,用于需要解mutex lock的应用
 * 1.日    期  : 2015年7月24日
  *   修改内容  : 新生成函数
 */
void wal_cfg80211_unregister_netdev(oal_net_device_stru *pst_net_dev)
{
    uint8_t uc_rollback_lock = OAL_FALSE;

    if (rtnl_is_locked()) {
        rtnl_unlock();
        uc_rollback_lock = OAL_TRUE;
    }
    /* 去注册netdev */
    oal_net_unregister_netdev(pst_net_dev);

    if (uc_rollback_lock) {
        rtnl_lock();
    }
}

/*
 * 函 数 名  : wal_cfg80211_mgmt_tx_cancel_wait
 * 功能描述  : 取消发送管理帧等待
 * 1.日    期  : 2014年11月22日
  *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_cfg80211_mgmt_tx_cancel_wait(oal_wiphy_stru *pst_wiphy,
    oal_wireless_dev_stru *pst_wdev, uint64_t ull_cookie)
{
    oal_net_device_stru *pst_netdev = NULL;
    mac_vap_stru *pst_mac_vap = NULL;

    if (pst_wdev == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx_cancel_wait::pst_wdev is Null!}");
        return -OAL_EFAIL;
    }

    pst_netdev = pst_wdev->netdev;
    if (pst_netdev == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx_cancel_wait::pst_netdev is Null!}");
        return -OAL_EFAIL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    if (pst_mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG,
            "{wal_cfg80211_mgmt_tx_cancel_wait::can't get mac vap from netdevice priv data!}");
        return -OAL_EFAIL;
    }
    /* 如果当前同信道普通监听标记置位, 则将其状态还原 */
    hmac_reset_same_chan_remain_on_chan(pst_mac_vap);

    return wal_p2p_stop_roc(pst_mac_vap, pst_netdev);
}

/*
 * 函 数 名  : wal_check_cookie_timeout
 * 功能描述  : 删除cookie 列表中超时的cookie
 * 1.日    期  : 2015年1月6日
  *   修改内容  : 新生成函数
 */
void wal_check_cookie_timeout(cookie_arry_stru *pst_cookie_array, uint8_t *puc_cookie_bitmap, uint32_t current_time)
{
    uint8_t uc_loops = 0;
    cookie_arry_stru *pst_tmp_cookie = NULL;

    oam_warning_log0(0, OAM_SF_CFG, "{wal_check_cookie_timeout::time_out!}\r\n");
    for (uc_loops = 0; uc_loops < WAL_COOKIE_ARRAY_SIZE; uc_loops++) {
        pst_tmp_cookie = &pst_cookie_array[uc_loops];
        if (oal_time_after32(OAL_TIME_JIFFY,
            pst_tmp_cookie->record_time + oal_msecs_to_jiffies(WAL_MGMT_TX_TIMEOUT_MSEC))) {
            /* cookie array 中保存的cookie 值超时 */
            /* 清空cookie array 中超时的cookie */
            pst_tmp_cookie->record_time = 0;
            pst_tmp_cookie->ull_cookie = 0;
            /* 清除占用的cookie bitmap位 */
            oal_bit_clear_bit_one_byte(puc_cookie_bitmap, uc_loops);
        }
    }
}

/*
 * 函 数 名  : wal_del_cookie_from_array
 * 功能描述  : 删除指定idx 的cookie
 * 1.日    期  : 2015年1月6日
  *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_del_cookie_from_array(cookie_arry_stru *pst_cookie_array,
                                              uint8_t *puc_cookie_bitmap, uint8_t uc_cookie_idx)
{
    cookie_arry_stru *pst_tmp_cookie = NULL;

    /* 清除对应cookie bitmap 位 */
    oal_bit_clear_bit_one_byte(puc_cookie_bitmap, uc_cookie_idx);

    /* 清空cookie array 中超时的cookie */
    pst_tmp_cookie = &pst_cookie_array[uc_cookie_idx];
    pst_tmp_cookie->ull_cookie = 0;
    pst_tmp_cookie->record_time = 0;
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_add_cookie_to_array
 * 功能描述  : 添加cookie 到cookie array 中
 * 1.日    期  : 2015年1月6日
  *   修改内容  : 新生成函数
 */
uint32_t wal_add_cookie_to_array(cookie_arry_stru *pst_cookie_array,
    uint8_t *puc_cookie_bitmap, uint64_t *pull_cookie, uint8_t *puc_cookie_idx)
{
    uint8_t uc_idx;
    cookie_arry_stru *pst_tmp_cookie = NULL;

    if (*puc_cookie_bitmap == WAL_COOKIE_FULL_MASK) {
        /* cookie array 满，返回错误 */
        oam_warning_log0(0, OAM_SF_CFG, "{wal_add_cookie_to_array::array full!}\r\n");
        return OAL_FAIL;
    }

    /* 将cookie 添加到array 中 */
    uc_idx = oal_bit_find_first_zero_one_byte(*puc_cookie_bitmap);
    oal_bit_set_bit_one_byte(puc_cookie_bitmap, uc_idx);

    pst_tmp_cookie = &pst_cookie_array[uc_idx];
    pst_tmp_cookie->ull_cookie = *pull_cookie;
    pst_tmp_cookie->record_time = OAL_TIME_JIFFY;

    *puc_cookie_idx = uc_idx;
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_check_cookie_from_array
 * 功能描述  : 从cookie array 中查找相应cookie index
 * 1.日    期  : 2015年7月31日
  *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_check_cookie_from_array(uint8_t *puc_cookie_bitmap, uint8_t uc_cookie_idx)
{
    /* 从cookie bitmap中查找相应的cookie index，如果位图为0，表示已经被del */
    if (*puc_cookie_bitmap & (BIT(uc_cookie_idx))) {
        return OAL_SUCC;
    }
    /* 找不到则返回FAIL */
    return OAL_FAIL;
}

OAL_STATIC oal_netbuf_stru *wal_mgmt_tx_prepare_netbuf(const uint8_t *buff, size_t len)
{
    oal_netbuf_stru *netbuf;
    int32_t ret;

    /*  申请netbuf 空间 */
    netbuf = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, len, OAL_NETBUF_PRIORITY_MID);
    /* Reserved Memory pool tried for high priority frames */
    if (netbuf == NULL && len <= WLAN_MEM_NETBUF_SIZE2) {
        netbuf = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
            WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    }

    if (netbuf == NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{wal_mgmt_tx_prepare_netbuf::alloc netbuf failed, len %d.}", len);
        return NULL;
    }

    /* 填充netbuf */
    ret = memcpy_s(oal_netbuf_header(netbuf), len, buff, len);
    if (ret != EOK) {
        oam_error_log1(0, OAM_SF_P2P, "{wal_mgmt_tx_prepare_netbuf::memcpy fail!errno[%d]}", ret);
        oal_netbuf_free(netbuf);
        return NULL;
    }

    oal_netbuf_put(netbuf, len);
    oal_mem_netbuf_trace(netbuf, OAL_TRUE);
    return netbuf;
}

/*
 * 功能描述  : 管理帧发送状态打印
 * 1.日    期  : 2020年8月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_mgmt_do_tx_status_report(uint32_t wait, oal_mgmt_tx_stru *mgmt_tx)
{
    signed long i_leftime;

    /*  增加打印，以看到超时时间 */
    i_leftime = oal_wait_event_interruptible_timeout_m(mgmt_tx->st_wait_queue,
        (oal_bool_enum_uint8)(mgmt_tx->mgmt_tx_complete == OAL_TRUE),
        (uint32_t)oal_msecs_to_jiffies(wait));  //  改成和上一级函数的等待时间一致
    if (i_leftime == 0) {
        /* 定时器超时 */
        oam_warning_log0(0, OAM_SF_ANY, "{wal_mgmt_do_tx_status_report::mgmt tx timeout!}");
        return OAL_FAIL;
    } else if (i_leftime < 0) {
        /* 定时器内部错误 */
        oam_warning_log0(0, OAM_SF_ANY, "{wal_mgmt_do_tx_status_report::mgmt tx timer error!}");
        return OAL_FAIL;
    } else {
        /* 正常结束  */
        /* . 正常发送结束，返回发送完成状态 */
        return mgmt_tx->mgmt_tx_status;
    }
}

/*
 * 功能描述  : WAL 层发送从wpa_supplicant  接收到的管理帧
 * 1.日    期  : 2015年8月29日
  *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_mgmt_do_tx(oal_net_device_stru *netdev, hmac_mgmt_frame_stru *mgmt_tx_param, bool offchan,
                                   uint32_t wait, const uint8_t *buff, size_t len)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    oal_mgmt_tx_stru *mgmt_tx = NULL;
    int32_t ret;
    mac_same_chan_remain_on_chan_info_stru *remain_on_chan_info = hmac_get_same_chan_remain_on_chan_info();

    mac_vap = oal_net_dev_priv(netdev);
    if (mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_mgmt_do_tx::can't get mac vap from netdevice priv data.}");
        return OAL_FAIL;
    }

    //  如果是Go, 仍然继续发送(比如Go Device Discoverability Request)
    if (!is_p2p_go(mac_vap)) {
        // 如果不是Go，则做如下判断
        /* 需要offchanel但驱动已经不在roc状态了,此action帧不需要发送 */
        if ((offchan == OAL_TRUE) && ((mac_vap->en_vap_state != MAC_VAP_STATE_STA_LISTEN) &&
            (mac_normal_same_remain_on_chan_is_running(remain_on_chan_info) == OAL_FALSE))) {
            oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                "{wal_mgmt_do_tx::pst_mac_vap state[%d]not in listen!}", mac_vap->en_vap_state);
            return OAL_ERR_CODE_WRONG_CHANNEL;  // 软件不再重传,告诉上层此次tx mgmt失败结束
        }
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_mgmt_do_tx::pst_hmac_vap ptr is null!}");
        return OAL_FAIL;
    }

    mgmt_tx_param->frame = wal_mgmt_tx_prepare_netbuf(buff, len);
    if (mgmt_tx_param->frame == NULL) {
        return OAL_FAIL;
    }

    mgmt_tx = &(hmac_vap->st_mgmt_tx);
    mgmt_tx->mgmt_tx_complete = OAL_FALSE;
    mgmt_tx->mgmt_tx_status = OAL_FAIL;

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    ret = wal_cfg80211_start_req(netdev, mgmt_tx_param, sizeof(hmac_mgmt_frame_stru),
                                 WLAN_CFGID_CFG80211_MGMT_TX, OAL_FALSE);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_netbuf_free(mgmt_tx_param->frame); // need rsp参数传入false，返回fail说明事件没有抛成功，需要释放netbuf。
        mgmt_tx_param->frame = NULL;
        oam_warning_log1(0, OAM_SF_ANY, "{wal_mgmt_do_tx::wal_send_cfg_event return err code %d!}", ret);
        return OAL_FAIL;
    }

    return wal_mgmt_do_tx_status_report(wait, mgmt_tx);
}

OAL_STATIC int32_t wal_cfg80211_mgmt_tx_bss_ap_need_offchan(hmac_vap_stru *pst_hmac_vap,
    hmac_mgmt_frame_stru *pst_mgmt_tx, mac_device_stru *pst_mac_device,
    oal_wireless_dev_stru *pst_roc_wireless_dev, bool *pen_need_offchan)
{
    // 此处应该是channel number，而不是channel index
    oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{wal_cfg80211_mgmt_tx::p2p mode[%d](0=Legacy, 1=Go, 2=Dev, 3=Gc), tx mgmt vap chan[%d], mgmt tx chan[%d]",
        pst_hmac_vap->st_vap_base_info.en_p2p_mode, pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number,
        pst_mgmt_tx->channel);

    if ((pst_hmac_vap->st_vap_base_info.st_channel.uc_chan_number != pst_mgmt_tx->channel) &&
        is_p2p_go(&pst_hmac_vap->st_vap_base_info)) {
        if (pst_mac_device->st_p2p_info.pst_p2p_net_device == NULL) {
            oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::go,p2p dev null");
            return -OAL_EINVAL;
        }
        // 使用p2p0 roc
        pst_roc_wireless_dev = oal_netdevice_wdev(pst_mac_device->st_p2p_info.pst_p2p_net_device);
        *pen_need_offchan = OAL_TRUE;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_set_net_info
 * 功能描述  : 设置 net dev mac vap信息
 * 1.日    期  : 2020年11月09日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_cfg80211_set_net_info(oal_wireless_dev_stru *wdev, oal_net_device_stru **netdev,
    mac_vap_stru **mac_vap, mac_device_stru **mac_device, hmac_vap_stru **hmac_vap)
{
    /* 通过net_device 找到对应的mac_device_stru 结构 */
    *netdev = wdev->netdev;
    if (*netdev == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx::netdev ptr is null!}\r\n");
        return OAL_FAIL;
    }

    *mac_vap = oal_net_dev_priv(*netdev);
    if (*mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx::can't get mac vap from netdevice priv data!}\r\n");
        return OAL_FAIL;
    }

    *mac_device = (mac_device_stru *)mac_res_get_dev((*mac_vap)->uc_device_id);
    if (*mac_device == NULL) {
        return OAL_FAIL;
    }

    *hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap((*mac_vap)->uc_vap_id);
    if (*hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx::hmac_vap ptr is null!}\r\n");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_set_channel_msg
 * 功能描述  : 设置 net channel 信息
 * 1.日    期  : 2020年11月09日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_cfg80211_set_channel_msg(oal_ieee80211_channel *chan_info, mac_vap_stru *mac_vap,
    const oal_ieee80211_mgmt *mgmt, oal_ieee80211_channel *chan, size_t len)
{
    uint8_t band_80211;
#ifdef _PRE_WLAN_FEATURE_SAE
    if (chan_info == NULL) {
        if (is_ap(mac_vap)) {
            /* AP直接取VAP信道 */
            band_80211 = hmac_get_80211_band_type(&mac_vap->st_channel);
            chan->band = band_80211;
            chan->center_freq = (uint16_t)oal_ieee80211_channel_to_frequency(mac_vap->st_channel.uc_chan_number,
                band_80211);
            chan->hw_value = mac_vap->st_channel.uc_chan_number;

            oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_SAE,
                "{wal_cfg80211_mgmt_tx:: AP pst_chan ptr is null! vap_mode[%d], p2p_mode [%d]. use vap_channel [%d]}",
                mac_vap->en_vap_mode, mac_vap->en_p2p_mode, mac_vap->st_channel.uc_chan_number);
        } else {
            /* STA取扫描结果信道 */
            mac_bss_dscr_stru *bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(mac_vap, mgmt->da);
            /* SAE发送auth commit/comfirm 帧，下发的pst_channel入参是NULL,
             * 需要根据目的MAC在扫描结果中查找信道信息 */
            oam_warning_log0(0, OAM_SF_SAE, "{wal_cfg80211_mgmt_tx::STA tx frame, find channel from scan result}");
            /* 判断传入的帧长度是否够长 */
            if (len < MAC_80211_FRAME_LEN) {
                oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_SAE,
                    "{wal_cfg80211_mgmt_tx::frame length is too short to send. %d}", len);
                return -OAL_EINVAL;
            }

            if (bss_dscr == NULL) {
                oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_SAE,
                    "{wal_cfg80211_mgmt_tx::can not find [%02X:XX:XX:XX:%02X:%02X] from scan result.}",
                    mgmt->da[MAX_ADDR_INDEX_0], mgmt->da[MAX_ADDR_INDEX_4], mgmt->da[MAX_ADDR_INDEX_5]);
                return -OAL_EINVAL;
            }
            band_80211 = hmac_get_80211_band_type(&bss_dscr->st_channel);
            chan->band = band_80211;
            chan->center_freq =
                (uint16_t)oal_ieee80211_channel_to_frequency(bss_dscr->st_channel.uc_chan_number, band_80211);
            chan->hw_value = bss_dscr->st_channel.uc_chan_number;
        }
    } else {
        *chan = *chan_info;
    }
#else
    if (chan_info == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_tx:: chan_info ptr is null!}");
        return OAL_FAIL;
    }
    *chan = *chan_info;
#endif /* _PRE_WLAN_FEATURE_SAE */
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_set_vap_state
 * 功能描述  : 设置 vap 状态信息
 * 1.日    期  : 2020年11月09日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void wal_cfg80211_set_vap_state(hmac_vap_stru *hmac_vap, uint8_t vap_state, uint8_t last_vap_state)
{
    mac_vap_stru *base_info = &hmac_vap->st_vap_base_info;
    /* 发送失败，处理超时帧的bitmap */
    wal_check_cookie_timeout(g_cookie_array, &g_uc_cookie_array_bitmap, OAL_TIME_JIFFY);

    oam_warning_log1(base_info->uc_vap_id, OAM_SF_CFG,
        "{wal_cfg80211_mgmt_tx::vap status[%d], tx mgmt timeout}\r\n", vap_state);

    /* ,失败超时需要恢复原vap状态，并取消device侧remain on channel */
    if (vap_state == MAC_VAP_STATE_STA_LISTEN) {
        if (base_info->en_p2p_mode != WLAN_LEGACY_VAP_MODE) {
            mac_vap_state_change(base_info, last_vap_state);
        } else {
            mac_vap_state_change(base_info, base_info->en_last_vap_state);
        }

        /* 抛事件到DMAC ，返回监听信道 */
        hmac_p2p_send_listen_expired_to_device(hmac_vap);
    }
}

/*
 * 函 数 名  : wal_cfg80211_swithchover_offchan
 * 功能描述  : 设置 offchan 状态信息
 * 1.日    期  : 2020年11月09日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void wal_cfg80211_check_sta_need_offchan(uint32_t flags, mac_vap_stru *base_info, bool *need_offchan,
    int32_t channel, bool offchan)
{
    if ((offchan == OAL_TRUE) && (flags & WIPHY_FLAG_OFFCHAN_TX)) {
        *need_offchan = OAL_TRUE;
    }
    if ((base_info->en_p2p_mode == WLAN_LEGACY_VAP_MODE) &&
        (base_info->en_vap_state == MAC_VAP_STATE_UP) &&
        (channel == base_info->st_channel.uc_chan_number)) {
        *need_offchan = OAL_FALSE;
    }
}

/* 功能描述: 发送管理帧时获取对应的tx roc类型 */
OAL_STATIC wlan_ieee80211_roc_type_uint8 wal_cfg80211_mgmt_tx_get_roc_type(uint8_t *buff, size_t buff_len)
{
#ifdef _PRE_WLAN_CHBA_MGMT
    uint8_t priority_flag;
    uint8_t *payload = NULL;
    uint16_t payload_len;
    const struct ieee80211_mgmt *mgmt_info = (const struct ieee80211_mgmt *)buff;

    /* chba nego action视为最高优先级发送 */
    if ((wal_cfg80211_tx_buff_is_action(mgmt_info->frame_control) == OAL_TRUE) && (buff_len > MAC_80211_FRAME_LEN)) {
        payload = buff + MAC_80211_FRAME_LEN;
        payload_len = buff_len - MAC_80211_FRAME_LEN;
        if (hmac_is_chba_negotiation_action(payload, payload_len) == OAL_TRUE) {
            priority_flag = hmac_chba_frame_get_negotiation_priority_flag(payload, payload_len);
            if (priority_flag == CHBA_NEGOACTION_HIGH_PRIORITY) {
                return IEEE80211_ROC_TYPE_CHBA_HIGH_PRIORITY_MGMT_TX; /* 高优先级chba action帧, 如建链前引导、pkt协商 */
            } else {
                return IEEE80211_ROC_TYPE_CHBA_LOW_PRIORITY_MGMT_TX; /* 低优先级chba发现action帧 */
            }
        }
    }
#endif
    return IEEE80211_ROC_TYPE_MGMT_TX; /* 普通管理帧收发 */
}

/* 功能描述: 配置mgmt_tx触发remain_on_chan的参数 */
OAL_STATIC void wal_cfg80211_set_remain_on_chan_params(wal_mgmt_tx_remain_on_chan_params_stru *remain_on_chan_params,
    uint32_t *duration, uint8_t *buff, size_t buff_len)
{
    remain_on_chan_params->continue_wait_after_tx = OAL_FALSE;
    remain_on_chan_params->tx_roc = wal_cfg80211_mgmt_tx_get_roc_type(buff, buff_len);
    if (*duration == 0) {
        if (remain_on_chan_params->tx_roc == IEEE80211_ROC_TYPE_CHBA_LOW_PRIORITY_MGMT_TX) {
            *duration = WLA_CHBA_LOW_PRIORITY_MGMT_TX_TIMEOUT_MSEC;
        } else {
            *duration = WAL_MGMT_TX_TIMEOUT_MSEC;
        }
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx::wait time is 0, set to %d ms}", *duration);
    }
    remain_on_chan_params->duration = *duration;
}

OAL_STATIC int32_t wal_tx_mgmt_same_chan_remain_on_chan(oal_net_device_stru *net_dev, mac_vap_stru *mac_work_vap,
    oal_bool_enum_uint8 remian_on_chan_need_wait, uint32_t wait_time)
{
    int ret;
    wal_msg_stru *rsp_msg = NULL;
    uint32_t err_code;
    wal_msg_write_stru msg = { 0 };
    mac_same_chan_remain_on_chan_cfg *remain_on_chan_cfg = (mac_same_chan_remain_on_chan_cfg *)(msg.auc_value);

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    wal_write_msg_hdr_init(&msg, WLAN_CFGID_TX_START_SAME_CHAN_REMAIN_ON_CHAN,
        sizeof(mac_same_chan_remain_on_chan_cfg));

    remain_on_chan_cfg->mac_work_vap = mac_work_vap;
    remain_on_chan_cfg->wait_time = wait_time;
    remain_on_chan_cfg->remian_on_chan_need_wait = remian_on_chan_need_wait;
    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_same_chan_remain_on_chan_cfg), (uint8_t *)&msg, OAL_TRUE, &rsp_msg);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_start_same_chan_long_remain_on_chan::fail to start, error[%d]}", ret);
        return -OAL_EFAIL;
    }

    /* 读取返回的错误码 */
    err_code = wal_check_and_release_msg_resp(rsp_msg);
    if (err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_start_same_chan_long_remain_on_chan:msg resp err:[%u]!}", err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

/* 功能描述: 根据roc类型判断该mgmt发送完成后是继续保持remain_on_chan还是立即结束 */
oal_bool_enum_uint8 wal_cfg80211_mgmt_tx_get_continue_wait_after_tx_flag(mac_vap_stru *mac_vap,
    wlan_ieee80211_roc_type_uint8 tx_roc)
{
    oal_bool_enum_uint8 continue_wait_after_tx = OAL_TRUE;

    switch (tx_roc) {
        case IEEE80211_ROC_TYPE_MGMT_TX:
            continue_wait_after_tx = OAL_TRUE;
            break;
        case IEEE80211_ROC_TYPE_CHBA_LOW_PRIORITY_MGMT_TX:
        case IEEE80211_ROC_TYPE_CHBA_HIGH_PRIORITY_MGMT_TX:
            continue_wait_after_tx = OAL_FALSE;
            break;
        default:
            oam_error_log1(mac_vap->uc_vap_id, OAM_SF_SCAN,
                "{wal_cfg80211_mgmt_tx_get_continue_wait_after_tx_flag:invalid roc[%d]}", tx_roc);
            break;
    }
    return continue_wait_after_tx;
}

OAL_STATIC oal_bool_enum_uint8 wal_cfg80211_is_support_same_chan_mgmt_tx(oal_net_device_stru *netdev,
    mac_vap_stru *mac_vap, uint8_t mgmt_tx_channel)
{
    /* 不支持同信道remain_on_chan时, 固定通过扫描发起remain_on_chan */
    if (g_wlan_spec_cfg->feature_same_chan_remain_on_chan_support == OAL_FALSE) {
        return OAL_FALSE;
    }

    /* p2p_dev与p2p_gc共用mac vap时需保证p2p_dev mgmt_tx发送不干扰p2p_gc配置, 如果有影响则走扫描mgmt_tx */
    if ((g_wlan_spec_cfg->p2p_device_gc_use_one_vap == OAL_TRUE) && (wal_is_p2p_device(netdev) == OAL_TRUE)) {
        if ((is_p2p_cl(mac_vap) == OAL_TRUE) && (mgmt_tx_channel != mac_vap->st_channel.uc_chan_number)) {
            return OAL_FALSE; /* p2p_gc已开始工作, p2p_dev只能通过扫描进行gc异信道发送 */
        }
    }
    return OAL_TRUE;
}

/* 功能描述: tx_mgmt触发的remain_on_chan处理 */
OAL_STATIC int32_t wal_cfg80211_mgmt_tx_remain_on_channel(oal_wireless_dev_stru *wdev,
    struct ieee80211_channel *chan, wal_mgmt_tx_remain_on_chan_params_stru *chan_param, uint64_t *cookie)
{
    int32_t ret;
    oal_net_device_stru *netdev = wdev->netdev;
    mac_vap_stru *mac_vap = NULL;
    mac_vap_stru *mac_work_vap = NULL;
    mac_device_stru *mac_device = NULL;
    uint8_t channel_num;

    /* 检查同信道监听参数 */
    if (wal_remain_on_chan_param_check(netdev, chan->center_freq) == OAL_FALSE) {
        oam_error_log0(0, OAM_SF_P2P, "{wal_cfg80211_mgmt_tx_remain_on_channel:param is invalid!}");
        return -OAL_EINVAL;
    };
    mac_vap = oal_net_dev_priv(netdev);
    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == NULL) {
        return -OAL_EFAIL;
    }
    channel_num = (uint8_t)oal_ieee80211_frequency_to_channel((int32_t)chan->center_freq);

    /* 根据roc类型判断该帧发送完成后是否需要继续保持一段时间监听 */
    chan_param->continue_wait_after_tx =
        wal_cfg80211_mgmt_tx_get_continue_wait_after_tx_flag(mac_vap, chan_param->tx_roc);
    /* 发送前打断扫描与同信道监听 */
    ret = wal_cfg80211_try_scan_abort_before_remain_on_chan(netdev, mac_device, mac_vap, chan_param->tx_roc);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx_remain_on_channel:fail to scan abort}");
        return ret;
    }

    /* 不支持同信道remain_on_chan发送时, 通过扫描发起remain_on_chan */
    if (wal_cfg80211_is_support_same_chan_mgmt_tx(netdev, mac_vap, channel_num) == OAL_FALSE) {
        return wal_drv_remain_on_channel(wdev, chan, chan_param->duration, cookie, chan_param->tx_roc);
    }
    /* dbac模式下避免信道切换导致的管理帧发送失败, 统一采用扫描模式进行发送 */
    if (mac_is_dbac_running(mac_device) == OAL_TRUE) {
        return wal_drv_remain_on_channel(wdev, chan, chan_param->duration, cookie, chan_param->tx_roc);
    }

    /* 如果同信道存在工作vap则进行同信道监听, 否则通过普通扫描remain_on_chan监听 */
    mac_work_vap = mac_find_listen_chan_work_vap(mac_device, mac_vap, OAL_TRUE, channel_num);
    if (mac_work_vap == NULL) {
        return wal_drv_remain_on_channel(wdev, chan, chan_param->duration, cookie, chan_param->tx_roc);
    } else {
        return wal_tx_mgmt_same_chan_remain_on_chan(netdev, mac_work_vap,
            chan_param->continue_wait_after_tx, chan_param->duration);
    }
}

/* 功能描述: 发送结束后处理 */
OAL_STATIC void wal_cfg80211_mgmt_tx_process_after_tx(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
    oal_bool_enum_uint8 continue_wait_after_tx, uint64_t cookie)
{
    /* 发送完成后无需继续等待的, 立刻结束tx_mgmt引起的remain_on_chna(包括扫描类型与同信道监听类型) */
    if (continue_wait_after_tx == OAL_FALSE) {
        wal_cfg80211_mgmt_tx_cancel_wait(wiphy, wdev, cookie);
    }
}

OAL_STATIC void wal_cfg80211_mgmt_tx_status_debug(hmac_vap_stru *hmac_vap, uint8_t retry, int32_t ret)
{
    if (retry > WAL_MGMT_TX_RETRY_CNT) {
        oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{wal_cfg80211_mgmt_tx::retry count[%d]>max[%d],tx status[%d],stop tx mgmt}",
            retry, WAL_MGMT_TX_RETRY_CNT, ret);
    }
}

/*
 * 函 数 名  : wal_cfg80211_mgmt_tx
 * 功能描述  : 发送管理帧
 * 1.日    期  : 2014年11月21日
  *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_cfg80211_mgmt_tx(oal_wiphy_stru *wiphy, oal_wireless_dev_stru *wdev,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    struct cfg80211_mgmt_tx_params *params,
#else
    oal_ieee80211_channel *chan_info, bool offchan, uint32_t wait, const uint8_t *buf, size_t len,
    bool no_cck, bool dont_wait_for_ack,
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0)) */
    uint64_t *pull_cookie)
{
    oal_net_device_stru *netdev = NULL;
    mac_device_stru *mac_device = NULL;
    mac_vap_stru *mac_vap = NULL;
    const oal_ieee80211_mgmt *mgmt = NULL;
    int32_t ret;
    uint32_t rst;
    hmac_mgmt_frame_stru mgmt_frame;
    uint8_t cookie_idx;
    uint8_t retry;
    uint8_t max_retry_cnt = WAL_MGMT_TX_RETRY_CNT;
    mac_p2p_info_stru *p2p_info = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    oal_mgmt_tx_stru *mgmt_tx = NULL;
    oal_wireless_dev_stru *roc_wireless_dev = wdev;
    unsigned long start_time_stamp;
    bool need_offchan = OAL_FALSE;
    uint32_t temp_flag;
    oal_ieee80211_channel chan = { 0 };
    wal_mgmt_tx_remain_on_chan_params_stru chan_param = { 0 };
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    oal_ieee80211_channel *chan_info;
    const uint8_t *buf;
    oal_size_t len;
    uint32_t wait;
    bool offchan;
    if (params == NULL) {
        return -OAL_EINVAL;
    }
    chan_info = params->chan;
    buf = params->buf;
    len = params->len;
    offchan = params->offchan;
    wait = params->wait;
    /* mp16 FPGA信道切换时间长，需要增加监听时间以保证管理帧发送时间,回片后可删除 */
    wait = wlan_chip_update_cfg80211_mgmt_tx_wait_time(wait);
#endif

    /* 1.1 入参检查 */
    if (oal_any_null_ptr4(wiphy, wdev, pull_cookie, buf)) {
        oam_error_log0(0, 0, "{wal_cfg80211_mgmt_tx::para null:wiphy/wdev/pull_cookie/buf is null}");
        return -OAL_EINVAL;
    }
    if (wal_cfg80211_set_net_info(wdev, &netdev, &mac_vap, &mac_device, &hmac_vap) != OAL_SUCC) {
        return -OAL_EINVAL;
    }
    p2p_info = &mac_device->st_p2p_info;
    *pull_cookie = p2p_info->ull_send_action_id++; /* cookie值上层调用需要判断是否是这次的发送导致的callback */
    if (*pull_cookie == 0) {
        *pull_cookie = p2p_info->ull_send_action_id++;
    }

    if (len < MAC_80211_FRAME_LEN) {
        return -OAL_EINVAL;
    }
    mgmt = (const struct ieee80211_mgmt *)buf;

    temp_flag = oal_ieee80211_is_probe_resp(mgmt->frame_control);
    temp_flag = !hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH) ? temp_flag :
        (temp_flag && (mac_vap->st_probe_resp_ctrl.en_probe_resp_status == MAC_PROBE_RESP_MODE_ACTIVE));
    if (temp_flag) {
        *pull_cookie = 0; /* set cookie default value */
        /* host should not send PROE RESPONSE, device will send immediately when receive probe request packet */
        oal_cfg80211_mgmt_tx_status(wdev, *pull_cookie, buf, len, OAL_TRUE, GFP_KERNEL);
        return OAL_SUCC;
    }
    rst = wal_cfg80211_set_channel_msg(chan_info, mac_vap, mgmt, &chan, len);
    if (rst != OAL_SUCC) {
        return -OAL_EINVAL;
    }
    /* 2.1 消息参数准备 */
    memset_s(&mgmt_frame, sizeof(mgmt_frame), 0, sizeof(mgmt_frame));
    mgmt_frame.channel = oal_ieee80211_frequency_to_channel(chan.center_freq);

    /* 若检测发现cookie空间用完，则清除最旧的一个cookie */
    if (g_uc_cookie_array_bitmap == WAL_COOKIE_FULL_MASK) {
        cookie_idx = wal_find_oldest_cookie(g_cookie_array);
        wal_del_cookie_from_array(g_cookie_array, &g_uc_cookie_array_bitmap, cookie_idx);
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx::cookies is [0x%x] after clear}",
            g_uc_cookie_array_bitmap);
    }

    ret = wal_add_cookie_to_array(g_cookie_array, &g_uc_cookie_array_bitmap, pull_cookie, &cookie_idx);
    if (ret != OAL_SUCC) {
        oam_warning_log1(mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::Fail to add cooki, ret[%d]!}", ret);
        return -OAL_EINVAL;
    } else {
        mgmt_frame.mgmt_frame_id = cookie_idx;
    }

    mgmt_tx = &(hmac_vap->st_mgmt_tx);
    mgmt_tx->mgmt_tx_complete = OAL_FALSE;
    mgmt_tx->mgmt_tx_status = OAL_FAIL;

    /* APUT模式不能roc */
    switch (hmac_vap->st_vap_base_info.en_vap_mode) {
        case WLAN_VAP_MODE_BSS_AP:
            if (wal_cfg80211_mgmt_tx_bss_ap_need_offchan(hmac_vap, &mgmt_frame, mac_device,
                roc_wireless_dev, &need_offchan) == -OAL_EINVAL) {
                return -OAL_EINVAL;
            }
            break;
        case WLAN_VAP_MODE_BSS_STA: /* P2P CL DEV */
            wal_cfg80211_check_sta_need_offchan(wiphy->flags, &hmac_vap->st_vap_base_info, &need_offchan,
                mgmt_frame.channel, offchan);
            break;
        default:
            break;
    }

    if ((need_offchan == OAL_TRUE) && (chan.center_freq == 0)) {
        oam_error_log1(mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::need offchan but chan null} ", offchan);
        return -OAL_EINVAL;
    }

    oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{wal_cfg80211_mgmt_tx::offchan[%d].chan[%d]vap state[%d],wait[%d]}",
        need_offchan, mgmt_frame.channel, hmac_vap->st_vap_base_info.en_vap_state, wait);

    /* 需要offchannel,按照入参切到相应的信道XXms */
    if (need_offchan == OAL_TRUE) {
        wal_cfg80211_set_remain_on_chan_params(&chan_param, &wait, (uint8_t *)buf, len);
        ret = wal_cfg80211_mgmt_tx_remain_on_channel(roc_wireless_dev, &chan, &chan_param, pull_cookie);
        if (ret != OAL_SUCC) {
            oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{wal_cfg80211_mgmt_tx::wal_drv_remain_on_channel[%d]!!!offchannel[%d].channel[%d],vap state[%d]}\r\n",
                ret, need_offchan, mgmt_frame.channel, hmac_vap->st_vap_base_info.en_vap_state);
            oal_cfg80211_mgmt_tx_status(wdev, *pull_cookie, buf, len, OAL_FALSE, GFP_KERNEL);
            return OAL_SUCC;
        }
    }

    /* 无需切离信道场景，发送管理帧时也需要等待一定时间 */
    if (wait == 0) {
        wait = WAL_MGMT_TX_TIMEOUT_MSEC;
        oam_warning_log1(mac_vap->uc_vap_id, 0, "{wal_cfg80211_mgmt_tx::wait time is 0, set to %d ms}", wait);
    }
    start_time_stamp = OAL_TIME_JIFFY;

    /* 发错信道,软件不能一直重传,加入软件重传次数限制 */
    retry = 0;
    // 解决WFA 4.2.3 APUT Anti-clogging test
    if (is_ap(mac_vap) && (g_optimized_feature_switch_bitmap & BIT(CUSTOM_OPTIMIZE_FEATURE_CERTIFY_MODE))) {
        max_retry_cnt = 0;
    }
    /* 发送失败，则尝试重传 */
    do {
        ret = (int32_t)wal_mgmt_do_tx(netdev, &mgmt_frame, need_offchan, wait, buf, len);
        retry++;
    } while (oal_value_ne_all2(ret, OAL_SUCC, OAL_ERR_CODE_WRONG_CHANNEL) && (retry <= max_retry_cnt) &&
        time_before(OAL_TIME_JIFFY, start_time_stamp + oal_msecs_to_jiffies(wait)));
    //  请留意，如果是Go端(通过p2p0)在一次会话中第二次(第一次发送已成功)
    // 给第三方发送Device Discoverability Response等帧时，可能会由于对方不再侦听而超时失败。
    wal_cfg80211_mgmt_tx_status_debug(hmac_vap, retry, ret);

    if (ret != OAL_SUCC) {
        wal_cfg80211_set_vap_state(hmac_vap, mac_vap->en_vap_state, mac_device->st_p2p_info.en_last_vap_state);
        oal_cfg80211_mgmt_tx_status(wdev, *pull_cookie, buf, len, OAL_FALSE, GFP_KERNEL);
    } else {
        /* 正常结束  */
        *pull_cookie = g_cookie_array[mgmt_tx->mgmt_frame_id].ull_cookie;
        wal_del_cookie_from_array(g_cookie_array, &g_uc_cookie_array_bitmap, mgmt_tx->mgmt_frame_id);
        oal_cfg80211_mgmt_tx_status(wdev, *pull_cookie, buf, len, OAL_TRUE, GFP_KERNEL);
    }

    //  请留意，如果是Go端(通过p2p0)在一次会话中第二次(第一次发送已成功)
    // 给第三方发送Device Discoverability Response等帧时，可能会由于对方不再侦听而超时失败。
    oam_warning_log3(0, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx::tx status[%d], retry cnt[%d]}, delta_time[%d]",
        ret, retry, oal_jiffies_to_msecs(OAL_TIME_JIFFY - start_time_stamp));

    wal_cfg80211_mgmt_tx_process_after_tx(wiphy, wdev, chan_param.continue_wait_after_tx, *pull_cookie);
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_mgmt_tx_status
 * 功能描述  : HMAC抛mgmt tx status到WAL, 唤醒wait queue
 * 1.日    期  : 2014年1月6日
  *   修改内容  : 新生成函数
 */
uint32_t wal_cfg80211_mgmt_tx_status(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = NULL;
    dmac_crx_mgmt_tx_status_stru *pst_mgmt_tx_status_param = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_mgmt_tx_stru *pst_mgmt_tx = NULL;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_mgmt_tx_status::pst_event_mem is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_hmac_vap = mac_res_get_hmac_vap(pst_event->st_event_hdr.uc_vap_id);
    if (pst_hmac_vap == NULL) {
        oam_error_log1(0, OAM_SF_TX, "{wal_cfg80211_mgmt_tx_status::pst_hmac_vap null.vap_id[%d]}",
            pst_event->st_event_hdr.uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_tx_status_param = (dmac_crx_mgmt_tx_status_stru *)(pst_event->auc_event_data);
    pst_mgmt_tx = &(pst_hmac_vap->st_mgmt_tx);
    pst_mgmt_tx->mgmt_tx_complete = OAL_TRUE;
    pst_mgmt_tx->mgmt_tx_status = pst_mgmt_tx_status_param->uc_dscr_status;
    pst_mgmt_tx->mgmt_frame_id = pst_mgmt_tx_status_param->mgmt_frame_id;

    /* 找不到相应的cookie值，说明已经超时被处理，不需要再唤醒 */
    if (wal_check_cookie_from_array(&g_uc_cookie_array_bitmap, pst_mgmt_tx->mgmt_frame_id) == OAL_SUCC) {
        /* 让编译器优化时保证OAL_WAIT_QUEUE_WAKE_UP在最后执行 */
        oal_smp_mb();
        oal_wait_queue_wake_up_interrupt(&pst_mgmt_tx->st_wait_queue);
    }

    return OAL_SUCC;
}

/* P2P 补充缺失的CFG80211接口 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
void wal_cfg80211_mgmt_frame_register(struct wiphy *wiphy, struct wireless_dev *wdev, struct mgmt_frame_regs *upd)
#else
void wal_cfg80211_mgmt_frame_register(struct wiphy *wiphy, struct wireless_dev *wdev, uint16_t frame_type, bool reg)
#endif
{
    if (wiphy == NULL || wdev == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_mgmt_frame_register::wiphy or wdev is null}");
        return;
    }
    return;
}

int32_t wal_cfg80211_set_bitrate_mask(struct wiphy *wiphy, struct net_device *dev, const uint8_t *peer,
                                      const struct cfg80211_bitrate_mask *mask)
{
    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_start_p2p_device
 * 功能描述  : 启动P2P_DEV
 * 1.日    期  : 2014年11月22日
  *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_cfg80211_start_p2p_device(oal_wiphy_stru *pst_wiphy, oal_wireless_dev_stru *pst_wdev)
{
    return -OAL_EFAIL;
}

/*
 * 函 数 名  : wal_cfg80211_stop_p2p_device
 * 功能描述  : 停止P2P_DEV
 * 1.日    期  : 2014年11月22日
  *   修改内容  : 新生成函数
 */
OAL_STATIC void wal_cfg80211_stop_p2p_device(oal_wiphy_stru *pst_wiphy, oal_wireless_dev_stru *pst_wdev)
{
    return;
}

/*
 * 函 数 名  : wal_cfg80211_set_power_mgmt
 * 功能描述  : 开关低功耗
 * 1.日    期  : 2015年07月22日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_cfg80211_set_power_mgmt(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    bool enabled, int32_t timeout)
{
    wal_msg_write_stru st_write_msg = {0};
    wal_msg_write_stru write_msg_powersaving = { 0 };
    mac_cfg_ps_open_stru *pst_sta_pm_open = NULL;
#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    mac_rx_listen_ps_switch_stru *rx_listen_ps_switch = NULL;
#endif
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_wiphy, pst_netdev))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_set_power_mgmt::pst_wiphy or pst_wdev is null!}");
        return -OAL_EINVAL;
    }

    wal_write_msg_hdr_init(&st_write_msg, WLAN_CFGID_SET_STA_PM_ON, sizeof(mac_cfg_ps_open_stru));
    wal_write_msg_hdr_init(&write_msg_powersaving, WLAN_CFGID_RX_LISTEN_PS_SWITCH,
        sizeof(mac_rx_listen_ps_switch_stru));

    pst_mac_vap = oal_net_dev_priv(pst_netdev);
    /* P2P dev不下发 */
    if ((oal_unlikely(pst_mac_vap == NULL)) || is_p2p_dev(pst_mac_vap)) {
        oam_warning_log0(0, OAM_SF_PWR, "{wal_cfg80211_set_power_mgmt::mac vap null or p2p dev.}");
        return OAL_SUCC;
    }

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
        "{wal_cfg80211_set_power_mgmt::vap mode[%d]p2p mode[%d]set pm:[%d]}",
        pst_mac_vap->en_vap_mode, pst_mac_vap->en_p2p_mode, enabled);
    pst_sta_pm_open = (mac_cfg_ps_open_stru *)(st_write_msg.auc_value);
    /* MAC_STA_PM_SWITCH_ON / MAC_STA_PM_SWITCH_OFF */
    pst_sta_pm_open->uc_pm_enable = enabled;
    pst_sta_pm_open->uc_pm_ctrl_type = MAC_STA_PM_CTRL_TYPE_HOST;

    l_ret = wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_cfg_ps_open_stru), (uint8_t *)&st_write_msg, OAL_FALSE, NULL);
#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    rx_listen_ps_switch = (mac_rx_listen_ps_switch_stru *)write_msg_powersaving.auc_value;
    rx_listen_ps_switch->rx_listen_enable = enabled;
    rx_listen_ps_switch->rx_listen_ctrl_type = MAC_RX_LISTEN_CFG80211_POWER_MGMT;
    l_ret += wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_rx_listen_ps_switch_stru),
        (uint8_t *)&write_msg_powersaving, OAL_FALSE, NULL);
#endif
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_set_power_mgmt::fail to send pm cfg msg, error[%d]}", l_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_11R

/*
 * 函 数 名  : wal_cfg80211_update_ft_ies
 * 功能描述  : 停止P2P_DEV
 * 1.日    期  : 2015年8月13日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_cfg80211_update_ft_ies(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    oal_cfg80211_update_ft_ies_stru *pst_fties)
{
    wal_msg_write_stru st_write_msg = {0};
    mac_cfg80211_ft_ies_stru *pst_mac_ft_ies;
    wal_msg_stru *pst_rsp_msg;
    uint32_t err_code;
    int32_t l_ret;

    if (oal_any_null_ptr3(pst_wiphy, pst_netdev, pst_fties)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_update_ft_ies::param is null.}\r\n");

        return -OAL_EINVAL;
    }

    if ((pst_fties->ie == NULL) || (pst_fties->ie_len == 0) || (pst_fties->ie_len >= MAC_MAX_FTE_LEN)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_update_ft_ies::unexpect ie or len[%d].}\r\n", pst_fties->ie_len);

        return -OAL_EINVAL;
    }

    /***************************************************************************
        抛事件到wal层处理
    ***************************************************************************/
    st_write_msg.en_wid = WLAN_CFGID_SET_FT_IES;
    st_write_msg.us_len = sizeof(mac_cfg80211_ft_ies_stru);

    pst_mac_ft_ies = (mac_cfg80211_ft_ies_stru *)st_write_msg.auc_value;
    pst_mac_ft_ies->us_mdid = pst_fties->md;
    pst_mac_ft_ies->us_len = pst_fties->ie_len;
    if (memcpy_s(pst_mac_ft_ies->auc_ie, MAC_MAX_FTE_LEN, pst_fties->ie, pst_fties->ie_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_cfg80211_update_ft_ies::memcpy fail!");
        return -OAL_EFAIL;
    }

    l_ret = wal_send_cfg_event(pst_netdev, WAL_MSG_TYPE_WRITE, \
        WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_cfg80211_ft_ies_stru), \
        (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_cfg80211_update_ft_ies::wal_send_cfg_event fail err code %d!}\r\n", l_ret);
        return -OAL_EFAIL;
    }
    err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_update_ft_ies:: fail return err code:[%u]!}\r\n", err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_11R

/*
 * 函 数 名  : wal_cfg80211_dump_survey
 * 功能描述  : report channel stat to kernel
 * 1.日    期  : 2016年6月1日
  *   修改内容  : 新生成函数
 */
int32_t wal_cfg80211_dump_survey(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
                                 int32_t l_idx, oal_survey_info_stru *pst_info)
{
    if (wal_cfg80211_dfr_and_s3s4_param_check() == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_dump_survey:: dfr or s3s4 is processing!}");
        return -OAL_EFAIL;
    }

    return hmac_cfg80211_dump_survey(pst_wiphy, pst_netdev, l_idx, pst_info);
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
OAL_STATIC void wal_cfg80211_abort_scan(oal_wiphy_stru *pst_wiphy, oal_wireless_dev_stru *pst_wdev)
{
    oal_net_device_stru *pst_netdev;

    /* 1.1 入参检查 */
    if (oal_any_null_ptr2(pst_wiphy, pst_wdev)) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_abort_scan::wiphy or wdev is null!}");
        return;
    }

    pst_netdev = pst_wdev->netdev;
    if (pst_netdev == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_cfg80211_abort_scan::netdev is null!}");
        return;
    }
    oam_warning_log0(0, OAM_SF_CFG, "{wal_cfg80211_abort_scan::enter!}");
    wal_force_scan_abort_then_scan_comp(pst_netdev);
    return;
}
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#ifdef _PRE_WLAN_FEATURE_SAE
/*
 * 函 数 名  : wal_cfg80211_external_auth
 * 功能描述  : 执行内核下发 ext_auth 命令
 * 1.日    期  : 2019年11月2日
  *   修改内容  : 新生成函数
 */
OAL_STATIC int32_t wal_cfg80211_external_auth(oal_wiphy_stru *pst_wiphy, oal_net_device_stru *pst_netdev,
    oal_cfg80211_external_auth_stru *pst_external_auth_params)
{
    uint32_t ret;
    int32_t l_ret;
    hmac_external_auth_req_stru st_ext_auth;

    if ((pst_netdev == NULL) || (pst_external_auth_params == NULL)) {
        oam_error_log0(0, OAM_SF_SAE, "wal_cfg80211_external_auth::param is null");
        return -OAL_EFAIL;
    }
    memset_s(&st_ext_auth, sizeof(st_ext_auth), 0, sizeof(st_ext_auth));
    st_ext_auth.us_status = pst_external_auth_params->status;
    l_ret = memcpy_s(st_ext_auth.auc_bssid, WLAN_MAC_ADDR_LEN, pst_external_auth_params->bssid, WLAN_MAC_ADDR_LEN);
    st_ext_auth.st_ssid.uc_ssid_len = oal_min(pst_external_auth_params->ssid.ssid_len, OAL_IEEE80211_MAX_SSID_LEN);
    l_ret += memcpy_s(st_ext_auth.st_ssid.auc_ssid, OAL_IEEE80211_MAX_SSID_LEN,
                      pst_external_auth_params->ssid.ssid, st_ext_auth.st_ssid.uc_ssid_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SAE, "wal_cfg80211_external_auth::memcpy fail!");
        return -OAL_EFAIL;
    }

    oam_warning_log4(0, OAM_SF_SAE, "{wal_cfg80211_external_auth::status %d, bssid[%02X:XX:XX:XX:%02X:%02X]}",
        st_ext_auth.us_status, st_ext_auth.auc_bssid[MAC_ADDR_0], st_ext_auth.auc_bssid[MAC_ADDR_4],
        st_ext_auth.auc_bssid[MAC_ADDR_5]);

    ret = wal_cfg80211_do_external_auth(pst_netdev, &st_ext_auth);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_SAE, "{wal_cfg80211_external_auth::do external auth fail. ret %d}", ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif /* _PRE_WLAN_FEATURE_SAE */
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0) */

/* 不同操作系统函数指针结构体方式不同 */
OAL_STATIC oal_cfg80211_ops_stru g_wal_cfg80211_ops = {
    .scan = wal_cfg80211_scan,
    .connect = wal_cfg80211_connect,
    .disconnect = wal_cfg80211_disconnect,
    .add_key = wal_cfg80211_add_key,
    .get_key = wal_cfg80211_get_key,
    .del_key = wal_cfg80211_remove_key,
    .set_default_key = wal_cfg80211_set_default_key,
    .set_default_mgmt_key = wal_cfg80211_set_default_mgmt_key,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0))
    .set_default_beacon_key = wal_cfg80211_set_default_mgmt_key,
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0) */
    .set_wiphy_params = wal_cfg80211_set_wiphy_params,
    .change_beacon = wal_cfg80211_change_beacon,
    .start_ap = wal_cfg80211_start_ap,
    .stop_ap = wal_cfg80211_stop_ap,
    .change_bss = wal_cfg80211_change_bss,
    .sched_scan_start = wal_cfg80211_sched_scan_start,
    .sched_scan_stop = wal_cfg80211_sched_scan_stop,
    .change_virtual_intf = wal_cfg80211_change_virtual_intf,
    .add_station = wal_cfg80211_add_station,
    .del_station = wal_cfg80211_del_station,
    .change_station = wal_cfg80211_change_station,
    .get_station = wal_cfg80211_get_station,
    .dump_station = wal_cfg80211_dump_station,
    .dump_survey = wal_cfg80211_dump_survey,
    .set_pmksa = wal_cfg80211_set_pmksa,
    .del_pmksa = wal_cfg80211_del_pmksa,
    .flush_pmksa = wal_cfg80211_flush_pmksa,
    .remain_on_channel = wal_cfg80211_remain_on_channel,
    .cancel_remain_on_channel = wal_cfg80211_cancel_remain_on_channel,
    .mgmt_tx = wal_cfg80211_mgmt_tx,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    .update_mgmt_frame_registrations = wal_cfg80211_mgmt_frame_register,
#else
    .mgmt_frame_register = wal_cfg80211_mgmt_frame_register,
#endif
    .set_bitrate_mask = wal_cfg80211_set_bitrate_mask,
    .add_virtual_intf = wal_cfg80211_add_virtual_intf,
    .del_virtual_intf = wal_cfg80211_del_virtual_intf,
    .mgmt_tx_cancel_wait = wal_cfg80211_mgmt_tx_cancel_wait,
    .start_p2p_device = wal_cfg80211_start_p2p_device,
    .stop_p2p_device = wal_cfg80211_stop_p2p_device,
    .set_power_mgmt = wal_cfg80211_set_power_mgmt,
#ifdef _PRE_WLAN_FEATURE_11R
    .update_ft_ies = wal_cfg80211_update_ft_ies,
#endif  // _PRE_WLAN_FEATURE_11R
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 0))
    .abort_scan = wal_cfg80211_abort_scan,
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)) */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
#ifdef _PRE_WLAN_FEATURE_SAE
    .external_auth = wal_cfg80211_external_auth,
#endif /* _PRE_WLAN_FEATURE_SAE */
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0) */
};

/*
 * 函 数 名  : wal_cfg80211_reset_bands
 * 功能描述  : 重新初始化wifi wiphy的bands
 * 1.日    期  : 2015年12月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void wal_cfg80211_reset_bands(uint8_t uc_dev_id)
{
    int i;

    /*
     * 每次更新国家码,flags都会被修改,且上次修改的值不会被清除,
     * 相当于每次修改的国家码都会生效,因此更新国家需要清除flag标志
     */
    for (i = 0; i < g_st_supported_band_2ghz_info.n_channels; i++) {
        g_st_supported_band_2ghz_info.channels[i].flags = 0;
    }

    if (mac_device_band_is_support(uc_dev_id, MAC_DEVICE_CAP_5G) == OAL_TRUE) {
        for (i = 0; i < g_st_supported_band_5ghz_info.n_channels; i++) {
            g_st_supported_band_5ghz_info.channels[i].flags = 0;
        }
    }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 43)) && !defined (_PRE_WLAN_EXPORT)
    /* 6G信道flags reset适配 */
    if ((mac_device_band_is_support(uc_dev_id, MAC_DEVICE_CAP_6G_LOW_BAND) == OAL_TRUE) ||
        (mac_device_band_is_support(uc_dev_id, MAC_DEVICE_CAP_6G_ALL_BAND) == OAL_TRUE)) {
        for (i = 0; i < g_st_supported_band_6ghz_info.n_channels; i++) {
            g_st_supported_band_6ghz_info.channels[i].flags = 0;
        }
    }
#endif
}

/*
 * 函 数 名  : wal_cfg80211_save_bands
 * 功能描述  : 保存wifi wiphy的bands
 * 1.日    期  : 2016年12月06日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void wal_cfg80211_save_bands(uint8_t uc_dev_id)
{
    int i;

    /* 每次更新国家码,flags都会被修改,且上次修改的值不会被清除,
       相当于每次修改的国家码都会生效,因此更新国家需要清除flag标志，
       每次设置国家码flag 后，保存当前设置到orig_flags中
    */
    for (i = 0; i < g_st_supported_band_2ghz_info.n_channels; i++) {
        g_st_supported_band_2ghz_info.channels[i].orig_flags = g_st_supported_band_2ghz_info.channels[i].flags;
    }

    if (mac_device_band_is_support(uc_dev_id, MAC_DEVICE_CAP_5G) == OAL_TRUE) {
        for (i = 0; i < g_st_supported_band_5ghz_info.n_channels; i++) {
            g_st_supported_band_5ghz_info.channels[i].orig_flags = g_st_supported_band_5ghz_info.channels[i].flags;
        }
    }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 43)) && !defined (_PRE_WLAN_EXPORT)
    /* 6G信道ori_flags save适配 */
    if ((mac_device_band_is_support(uc_dev_id, MAC_DEVICE_CAP_6G_LOW_BAND) == OAL_TRUE) ||
        (mac_device_band_is_support(uc_dev_id, MAC_DEVICE_CAP_6G_ALL_BAND) == OAL_TRUE)) {
        for (i = 0; i < g_st_supported_band_6ghz_info.n_channels; i++) {
            g_st_supported_band_6ghz_info.channels[i].orig_flags = g_st_supported_band_6ghz_info.channels[i].flags;
        }
    }
#endif
}

#ifdef _PRE_WLAN_FEATURE_MULTI_AP
OAL_STATIC void wal_init_multi_ap_netdev(oal_wiphy_stru *wiphy)
{
    uint8_t i;
    uint8_t num;
    uint32_t res;
    int8_t netdev_name[MAC_NET_DEVICE_NAME_LENGTH] = {0};
    uint8_t netdev_id[] = {2, 3}; /* 2、3:创建wlan2 wlan3 */

    num = sizeof(netdev_id) / sizeof(netdev_id[0]);
    for (i = 0; i < num; i++) {
        if (snprintf_s(netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1, "wlan%u",
            netdev_id[i]) < 0) {
            oal_io_print("{wal_init_multi_ap_netdev:: snprintf_s wlan%d fail.}\n", netdev_id[i]);
            return;
        }

        res = wal_init_wlan_netdev(wiphy, netdev_name);
        if (res != OAL_SUCC) {
            oal_io_print("wal_init_multi_ap_netdev:: wlan%u failed.res:%d\n", netdev_id[i], res);
            return;
        }
    }
}
#endif

OAL_STATIC uint32_t wal_init_dual_wlan_netdev(mac_device_stru *mac_device, oal_wiphy_stru *wiphy, uint8_t dev_id)
{
    const char *netdev_name = "wlan1";
    if (!g_wlan_spec_cfg->feature_dual_wlan_is_supported) {
        oal_io_print("{wal_init_dual_wlan_netdev::dual wlan unsupported, Not create hwlan netdev!}");
        return OAL_SUCC;
    }
#ifdef _PRE_DUAL_CHIP
    if (oal_bus_get_chip_num() == WLAN_CHIP_MAX_NUM_PER_BOARD) {
        oal_io_print("{wal_init_dual_wlan_netdev::dual chip case, wlan1 will be created for chip1!}");
        return OAL_SUCC;
    }
#endif
    return wal_init_wlan_netdev(wiphy, netdev_name);
}
OAL_STATIC void wal_cfg80211_init_netdev_fail(mac_device_stru *pst_device)
{
    wal_unregister_netdev_and_free_wdev(pst_device->st_p2p_info.pst_p2p_net_device);
    wal_unregister_netdev_and_free_wdev(pst_device->st_p2p_info.pst_primary_net_device);
}
/*
 * 函 数 名  : wal_cfg80211_init_netdev
 * 功能描述  : netdev加载初始化
 * 1.日    期  : 2019年11月26日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t wal_cfg80211_init_netdev(mac_device_stru *pst_device, oal_wiphy_stru *pst_wiphy,
    uint8_t dev_id)
{
    uint32_t l_return;
    int8_t netdev_name[MAC_NET_DEVICE_NAME_LENGTH] = {0};

    oal_io_print("wal_init_wlan_netdev wlan and p2p[%d].\n", dev_id);
    /* 主路辅路的netdev规格:主路wlan0 p2p0,辅路wlan1 p2p1 */
    if (snprintf_s(netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1, "wlan%u", dev_id) < 0) {
        return OAL_FAIL;
    }

    l_return = wal_init_wlan_netdev(pst_wiphy, netdev_name);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_wlan_netdev wlan[%d] failed.l_return:%d\n", dev_id, l_return);
        return (uint32_t)l_return;
    }

    if (snprintf_s(netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1, "p2p%u", dev_id) < 0) {
        return OAL_FAIL;
    }

    l_return = wal_init_wlan_netdev(pst_wiphy, netdev_name);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_wlan_netdev p2p[%d] failed.l_return:%d\n", dev_id, l_return);

        /* 释放wlan网络设备资源 */
        wal_unregister_netdev_and_free_wdev(pst_device->st_p2p_info.pst_primary_net_device);
        return (uint32_t)l_return;
    }
    l_return = wal_init_dual_wlan_netdev(pst_device, pst_wiphy, dev_id);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_dual_wlan_netdev wlan1 failed.l_return:%d\n", l_return);
        wal_cfg80211_init_netdev_fail(pst_device);
        return (uint32_t)l_return;
    }
#ifdef _PRE_WLAN_CHBA_MGMT
    /* 注册chba device */
    if (snprintf_s(netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1, "chba%u", dev_id) < 0) {
        return OAL_FAIL;
    }
    l_return = wal_init_chba_netdev(pst_wiphy, netdev_name);
    if (l_return != OAL_SUCC) {
        oal_io_print("wal_init_chba_netdev chba failed.l_return:%d\n", l_return);
        wal_cfg80211_init_netdev_fail(pst_device);
        return l_return;
    }
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
    /* 注册NAN netdev */
    wal_init_nan_netdev(pst_wiphy);
#endif
#ifdef _PRE_WLAN_FEATURE_MULTI_AP
    wal_init_multi_ap_netdev(pst_wiphy);
#endif
    return OAL_SUCC;
}
static void wal_cfg80211_init_scan_info(oal_wiphy_stru *wiphy, uint8_t dev_id)
{
    wiphy->max_remain_on_channel_duration = 5000; /* 5000ms */
    /* 使能驱动监听,使能offchannel发送防止发错信道 */
    wiphy->flags |= WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL | WIPHY_FLAG_OFFCHAN_TX;

    /* PNO相关 */
    wiphy->max_sched_scan_ssids = MAX_PNO_SSID_COUNT;
    wiphy->max_match_sets = MAX_PNO_SSID_COUNT;
    wiphy->max_sched_scan_ie_len = WAL_MAX_SCAN_IE_LEN;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 12, 0))
    wiphy->max_sched_scan_reqs = 1;
#else
    wiphy->flags |= WIPHY_FLAG_SUPPORTS_SCHED_SCAN;
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 43)) && !defined (_PRE_WLAN_EXPORT)
    /* 判断是否使能6G扫描， 适配split扫描模式：
       2G+5G信道扫描完成并上报内核，内核根据扫描结果单独下发6G信道扫描 */
    if (g_cust_cap.chn_radio_cap_6g != 0 &&
        (mac_device_band_is_support(dev_id, MAC_DEVICE_CAP_6G_LOW_BAND) == OAL_TRUE ||
        mac_device_band_is_support(dev_id, MAC_DEVICE_CAP_6G_ALL_BAND) == OAL_TRUE)) {
        wiphy->flags |= WIPHY_FLAG_SPLIT_SCAN_6GHZ;
    }
#endif

    wiphy->max_scan_ssids = WLAN_SCAN_REQ_MAX_SSID;
    wiphy->max_scan_ie_len = WAL_MAX_SCAN_IE_LEN;
}
static void wal_cfg80211_init_band_info(oal_wiphy_stru *wiphy, uint8_t dev_id)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 43)) && !defined (_PRE_WLAN_EXPORT)
    if (g_cust_cap.chn_radio_cap_6g != 0 &&
        (mac_device_band_is_support(dev_id, MAC_DEVICE_CAP_6G_LOW_BAND) == OAL_TRUE ||
        mac_device_band_is_support(dev_id, MAC_DEVICE_CAP_6G_ALL_BAND) == OAL_TRUE)) {
        /* 6G定制化chann_radio_cap_6g开启后需要向内核注册6G能力 */
        wiphy->bands[NL80211_BAND_6GHZ] = &g_st_supported_band_6ghz_info; /* 支持的频带信息 6G */
    }
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    wiphy->bands[NL80211_BAND_2GHZ] = &g_st_supported_band_2ghz_info; /* 支持的频带信息 2.4G */
    if (mac_device_band_is_support(dev_id, MAC_DEVICE_CAP_5G) == OAL_TRUE) {
        wiphy->bands[NL80211_BAND_5GHZ] = &g_st_supported_band_5ghz_info; /* 支持的频带信息 5G */
    }
#else
    wiphy->bands[IEEE80211_BAND_2GHZ] = &g_st_supported_band_2ghz_info; /* 支持的频带信息 2.4G */
    if (mac_device_band_is_support(dev_id, MAC_DEVICE_CAP_5G) == OAL_TRUE) {
        wiphy->bands[IEEE80211_BAND_5GHZ] = &g_st_supported_band_5ghz_info; /* 支持的频带信息 5G */
    }
#endif
}
static void wal_cfg80211_init_cipher_info(oal_wiphy_stru *wiphy)
{
    if (g_wlan_spec_cfg->support_bip_gmac_128 != OAL_TRUE) {
        wiphy->cipher_suites = g_wlan_supported_cipher_suites_without_bip_gmac_128;
        wiphy->n_cipher_suites =
            (int32_t)(sizeof(g_wlan_supported_cipher_suites_without_bip_gmac_128) / sizeof(uint32_t));
    } else {
        wiphy->cipher_suites = g_wlan_supported_cipher_suites;
        wiphy->n_cipher_suites = (int32_t)(sizeof(g_wlan_supported_cipher_suites) / sizeof(uint32_t));
    }
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0))
void oal_wiphy_ext_feature_set(oal_wiphy_stru *wiphy, enum nl80211_ext_feature_index index)
{
    if (oal_unlikely(oal_any_null_ptr1(wiphy))) {
        return;
    }

    wiphy_ext_feature_set(wiphy, index);
}
#endif
static void wal_cfg80211_init_wiphy_feature(oal_wiphy_stru *wiphy)
{
#ifdef _PRE_WLAN_FEATURE_SAE
    wiphy->features |= NL80211_FEATURE_SAE; /* 驱动支持SAE 特性 */
#endif
    wiphy->features |= NL80211_FEATURE_SCAN_RANDOM_MAC_ADDR;
    wiphy->features |= NL80211_FEATURE_SCHED_SCAN_RANDOM_MAC_ADDR;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0))
#ifdef _PRE_WLAN_FEATURE_MULTI_AP
    oal_wiphy_ext_feature_set(wiphy, NL80211_EXT_FEATURE_DFS_OFFLOAD);
#endif
    oal_wiphy_ext_feature_set(wiphy, NL80211_EXT_FEATURE_LOW_POWER_SCAN);
    if ((g_expand_feature_switch_bitmap & CUSTOM_BEACON_PROTECTION) != 0) {
        oal_wiphy_ext_feature_set(wiphy, NL80211_EXT_FEATURE_BEACON_PROTECTION);
        oal_wiphy_ext_feature_set(wiphy, NL80211_EXT_FEATURE_BEACON_PROTECTION_CLIENT);
    }
#endif
}
static void wal_cfg80211_init_iface(oal_wiphy_stru *wiphy)
{
    wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) | BIT(NL80211_IFTYPE_AP) | BIT(NL80211_IFTYPE_P2P_CLIENT) |
        BIT(NL80211_IFTYPE_P2P_GO) | BIT(NL80211_IFTYPE_P2P_DEVICE);
    wiphy->iface_combinations = g_sta_p2p_iface_combinations;
    wiphy->n_iface_combinations = (int32_t)oal_array_size(g_sta_p2p_iface_combinations);
}
static void wal_cfg80211_init_wiphy(oal_wiphy_stru *wiphy, uint8_t dev_id)
{
    wal_cfg80211_init_iface(wiphy);
    wal_cfg80211_init_scan_info(wiphy, dev_id);
    wal_cfg80211_init_cipher_info(wiphy);
    wal_cfg80211_init_wiphy_feature(wiphy);
    wiphy->mgmt_stypes = g_wal_cfg80211_default_mgmt_stypes;
    /*  解决GO Beacon register失败问题 */
    wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME; /* 设置GO 能力位 */
    /* 不使能节能 */
    wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;

    /* BEGIN  roaming failure between different encryption methods ap. */
    /* wifi 驱动上报支持FW_ROAM,关联时(cfg80211_connect)候使用bssid_hint 替代bssid。 */
    wiphy->flags |= WIPHY_FLAG_SUPPORTS_FW_ROAM;
    /* END  roaming failure between different encryption methods ap. */
#ifdef _PRE_WLAN_FEATURE_DFS_OFFLOAD
    wiphy->flags |= 0x200;
#endif

    /* linux 3.14 版本升级，管制域重新修改 */
    wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;

    wal_cfg80211_init_band_info(wiphy, dev_id);

    wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;

    oal_wiphy_apply_custom_regulatory(wiphy, &g_st_default_regdom);
    wal_cfgvendor_init(wiphy);
}

/*
 * 函 数 名  : wal_cfg80211_init
 * 功能描述  : wal_linux_cfg80211加载初始化
 * 1.日    期  : 2013年8月28日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_cfg80211_init(void)
{
    uint8_t chip, uc_device, uc_dev_id;
    int32_t l_return;
    mac_device_stru *pst_device = NULL;
    mac_board_stru *pst_hmac_board = NULL;
    oal_wiphy_stru *pst_wiphy = NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = NULL;

    hmac_board_get_instance(&pst_hmac_board);
    wal_cfg80211_init_ieee80211_iface_combination();
    for (chip = 0; chip < oal_bus_get_chip_num(); chip++) {
        for (uc_device = 0; uc_device < pst_hmac_board->ast_chip[chip].uc_device_nums; uc_device++) {
            /* 获取device_id */
            uc_dev_id = pst_hmac_board->ast_chip[chip].auc_device_id[uc_device];
            pst_device = mac_res_get_dev(uc_dev_id);
            if (oal_unlikely(pst_device == NULL)) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::mac_res_get_dev,pst_dev null!}\r\n");
                return OAL_FAIL;
            }
            pst_device->pst_wiphy = oal_wiphy_new(&g_wal_cfg80211_ops, sizeof(mac_wiphy_priv_stru));
            if (pst_device->pst_wiphy == NULL) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::oal_wiphy_new failed!}\r\n");
                return OAL_FAIL;
            }

            /* 初始化wiphy 结构体内容 */
            pst_wiphy = pst_device->pst_wiphy;
            wal_cfg80211_init_wiphy(pst_wiphy, uc_dev_id);

            l_return = oal_wiphy_register(pst_wiphy);
            if (l_return != 0) {
                oal_wiphy_free(pst_device->pst_wiphy);
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::oal_wiphy_register failed!}\r\n");
                return (uint32_t)l_return;
            }

            /* P2P add_virtual_intf 传入wiphy 参数，在wiphy priv 指针保存wifi 驱动mac_devie_stru 结构指针 */
            pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wiphy));
            pst_wiphy_priv->pst_mac_device = pst_device;

            l_return = (int32_t)wal_cfg80211_init_netdev(pst_device, pst_wiphy, uc_dev_id);
            if (l_return != OAL_SUCC) {
                return (uint32_t)l_return;
            }
            if (g_wlan_spec_cfg->feature_ml_is_supported) {
        #ifdef _PRE_WLAN_FEATURE_ML
                wal_cfg80211_init_mlo_netdev(pst_device, pst_wiphy);
        #endif
            }
        }
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : wal_cfg80211_exit
 * 功能描述  : 卸载wihpy
 * 1.日    期  : 2013年9月5日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void wal_cfg80211_exit(void)
{
    uint32_t chip;
    uint8_t uc_device;
    uint8_t uc_dev_id;
    mac_device_stru *pst_device = NULL;
    uint32_t chip_max_num;
    mac_board_stru *pst_hmac_board = NULL;

    hmac_board_get_instance(&pst_hmac_board);

    chip_max_num = oal_bus_get_chip_num(); /* 这个地方待确定 */

    for (chip = 0; chip < chip_max_num; chip++) {
        for (uc_device = 0; uc_device < pst_hmac_board->ast_chip[chip].uc_device_nums; uc_device++) {
            /* 获取device_id */
            uc_dev_id = pst_hmac_board->ast_chip[chip].auc_device_id[uc_device];

            pst_device = mac_res_get_dev(uc_dev_id);
            if (pst_device == NULL) {
                oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_init::mac_res_get_dev pst_device is null!}\r\n");
                return;
            }
            wal_cfgvendor_deinit(pst_device->pst_wiphy);

            /* 注销注册 wiphy device */
            oal_wiphy_unregister(pst_device->pst_wiphy);

            /* 卸载wiphy device */
            oal_wiphy_free(pst_device->pst_wiphy);
        }
    }

    return;
}

/*
 * 函 数 名  : wal_cfg80211_init_evt_handle
 * 功能描述  : host初始化完dev信息之后，抛给wal来处理
 * 1.日    期  : 2012年11月6日
  *   修改内容  : 新生成函数
 */
uint32_t wal_cfg80211_init_evt_handle(frw_event_mem_stru *pst_event_mem)
{
    uint32_t ret;

    ret = wal_cfg80211_init();
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_cfg80211_init_evt_handle::wal_cfg80211_init return err code[%d]!}", ret);
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/*
 * 函 数 名  : wal_cfg80211_tas_rssi_access_report
 * 功能描述  : 触发上报tas测量事件
 * 1.日    期  : 2018年7月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t wal_cfg80211_tas_rssi_access_report(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event;
    hmac_vap_stru *pst_hmac_vap;
    uint8_t uc_vap_idx;
    dmac_tas_rssi_notify_stru *pst_tas_rssi_comp_status;
    int32_t l_tas_state;
    oal_net_device_stru *netdev = NULL;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_cfg80211_tas_rssi_access_report::pst_event_mem is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* 获取配置vap */
    uc_vap_idx = pst_event->st_event_hdr.uc_vap_id;
    pst_hmac_vap = mac_res_get_hmac_vap(uc_vap_idx);
    if (pst_hmac_vap == NULL) {
        oam_error_log0(uc_vap_idx, OAM_SF_ANY, "{wal_cfg80211_tas_rssi_access_report::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_tas_rssi_comp_status = (dmac_tas_rssi_notify_stru *)(pst_event->auc_event_data);

    l_tas_state = hal_get_wifi_tas_gpio_state();
    oam_warning_log3(uc_vap_idx, OAM_SF_ANY, "{wal_cfg80211_tas_rssi_access_report::core[%d] c_ant%d_rssi[%d].}",
                     pst_tas_rssi_comp_status->l_core_idx, l_tas_state, pst_tas_rssi_comp_status->l_rssi);

    netdev = hmac_vap_get_net_device(pst_hmac_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(uc_vap_idx, OAM_SF_M2S,
            "{wal_cfg80211_tas_rssi_access_report::net dev is null}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 上报内核 */
    oal_cfg80211_tas_rssi_access_report(netdev, GFP_KERNEL, (uint8_t *)pst_tas_rssi_comp_status,
                                        sizeof(dmac_tas_rssi_notify_stru));

    return OAL_SUCC;
}
#endif

#define POWER_OFF_READY_WAIT_TIME (2 * OAL_TIME_HZ)
OAL_STATIC uint32_t wal_power_off_ready_get_event_timeout_valid(void)
{
    signed long i_leftime;

    i_leftime = oal_wait_event_interruptible_timeout_m(g_power_off_ready_wait_queue,
        (hmac_check_power_off_ready() == OAL_TRUE), POWER_OFF_READY_WAIT_TIME);

    oam_warning_log2(0, OAM_SF_ANY,
        "{wal_power_off_ready_get_event_timeout_valid::query info left_time %ld ms, timeout threshold %ld ms!}",
        ((i_leftime * WAL_HIPRIV_MS_TO_S) / OAL_TIME_HZ),
        ((POWER_OFF_READY_WAIT_TIME * WAL_HIPRIV_MS_TO_S) / OAL_TIME_HZ));
    if (i_leftime <= 0) {
        /* i_leftime为0超时还没有上报扫描结束, i_leftime小于0定时器内部错误 */
        oam_error_log0(0, OAM_SF_ANY,
            "{wal_power_off_ready_get_event_timeout_valid::Waiting timed out!}");
        return -OAL_EINVAL;
    } else {
        return OAL_SUCC;
    }
}

int32_t wal_del_vap_try_wlan_pm_close(uint32_t dev_id)
{
    mac_chip_stru *chip = NULL;
    mac_device_stru *mac_device = NULL;
    uint8_t vap_num;

    if (dev_id >= HCC_CHIP_MAX_DEV) {
        oam_error_log1(0, OAM_SF_ANY, "wal_del_vap_try_wlan_pm_close:dev[%d] exceed", dev_id);
        return OAL_FAIL;
    }

    mac_device = mac_res_get_dev(dev_id);
    if (mac_device == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "wal_del_vap_try_wlan_pm_close:mac_device[%d] null", dev_id);
        return OAL_FAIL;
    }

    /*  DFR流程不能进行下电操作 */
    if (g_st_dfr_info.bit_device_reset_process_flag == OAL_TRUE) {
        return OAL_SUCC;
    }

    chip = hmac_res_get_mac_chip(mac_device->uc_chip_id);
    if (chip == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "wal_del_vap_try_wlan_pm_close:chip_id=%d", mac_device->uc_chip_id);
        return OAL_FAIL;
    }
    vap_num = hmac_get_chip_vap_num(chip);
    if (vap_num != 0) {
        return OAL_SUCC;
    }

    if (g_wlan_spec_cfg->power_off_ready_support) {
        wal_power_off_ready_get_event_timeout_valid();
    }

    wal_wake_lock();
    if (wlan_pm_close_dev(dev_id) != OAL_ERR_CODE_FOBID_CLOSE_DEVICE) {
        wal_wake_unlock();
        oam_warning_log1(0, OAM_SF_ANY, "{wal_del_vap_try_wlan_pm_close:wlan_pm_close dev[%d] succ", dev_id);
#ifdef _PRE_WINDOWS_SUPPORT
        return hcc_bus_power_action(hcc_get_bus(dev_id), HCC_BUS_POWER_DOWN);
#endif
    }
    wal_wake_unlock();
    return OAL_SUCC;
}

void wal_unregister_netdev_and_free_wdev(oal_net_device_stru *netdev)
{
    /* 先去注册netdev再释放wireless_device内存 */
    oal_wireless_dev_stru *wdev = oal_netdevice_wdev(netdev);
    oal_net_unregister_netdev(netdev);
    oal_mem_free_m(wdev, OAL_TRUE);
}
