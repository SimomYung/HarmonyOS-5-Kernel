/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : 配置相关实现hmac接口实现源文件
 * 作    者 :
 * 创建日期 : 2013年1月8日
 */

#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "hmac_config.h"
#include "mac_device.h"
#include "hd_event_vap.h"
#include "hmac_scan.h"
#ifdef _PRE_WLAN_FEATURE_ML
#include "hmac_ml_scan.h"
#include "hmac_mld_vap.h"
#include "hmac_mld_ext.h"
#include "hmac_wifi7_self_cure.h"
#include "hmac_chan_mgmt.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CONFIG_FRAME_ML_C

#ifdef _PRE_WLAN_FEATURE_ML
typedef struct {
    int8_t bss_rssi;
    wlan_channel_bandwidth_enum_uint8 en_bandwidth;
    uint8_t max_rssi_idx;
    uint8_t max_bw_idx;
} hmac_mld_preferred_link_info_stru;

uint32_t hmac_check_is_mld_connect(mac_vap_stru *mac_vap, mac_bss_dscr_stru *bss_dscr)
{
    return mac_is_primary_legacy_vap(mac_vap) &&
            (mac_mib_get_MultiLinkActivated(mac_vap->pst_mib_info) == OAL_TRUE) &&
            (bss_dscr->ml_info.ml_cap == OAL_TRUE);
}
#define LINK_RSSI_NE80_DB (-80)
#define LINK_RSSI_NE78_DB (-78)
#define LINK_RSSI_DIFF_15_DB (15)
OAL_STATIC uint8_t hmac_get_preferred_bss_mld_link_info_idx(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *join_bss_dscr,
    mac_bss_dscr_stru *link_bss_dscr, uint8_t cur_link_idx, hmac_mld_preferred_link_info_stru *record_info)
{
    mac_bss_dscr_stru *max_bw_bss_dscr = NULL;
    mac_bss_dscr_stru *max_rssi_bss_dscr = NULL;
    /* 如果扫描结果找不到该link bss，则只记录当前idx */
    if (link_bss_dscr == NULL) {
        return cur_link_idx;
    }
    oam_warning_log3(0, 0,
        "hmac_get_preferred_bss_mld_link_info_idx::cur idx[%d], link bss rssi[%d], bw[%d]!",
        cur_link_idx, link_bss_dscr->c_rssi, link_bss_dscr->st_channel.en_bandwidth);
    if (record_info->bss_rssi == 0 || record_info->bss_rssi < link_bss_dscr->c_rssi) { /* 当前link bss rssi初始化 */
        record_info->bss_rssi = link_bss_dscr->c_rssi;
        record_info->max_rssi_idx = cur_link_idx;
    }
    if (record_info->en_bandwidth == WLAN_BAND_WIDTH_BUTT ||
        record_info->en_bandwidth < link_bss_dscr->st_channel.en_bandwidth) {
        record_info->en_bandwidth = link_bss_dscr->st_channel.en_bandwidth;
        record_info->max_bw_idx = cur_link_idx;
    }
    if (record_info->max_rssi_idx != record_info->max_bw_idx) {
        max_bw_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
            join_bss_dscr->ml_info.ml_link_info[record_info->max_bw_idx].sta_mac_addr);
        max_rssi_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
            join_bss_dscr->ml_info.ml_link_info[record_info->max_rssi_idx].sta_mac_addr);
        /* 参考漫游，2G小于-80或者5G小于-78不选带宽大的link bss */
        if (((max_bw_bss_dscr->c_rssi < LINK_RSSI_NE80_DB) && (max_bw_bss_dscr->st_channel.en_band == WLAN_BAND_2G)) ||
            ((max_bw_bss_dscr->c_rssi < LINK_RSSI_NE78_DB) && (max_bw_bss_dscr->st_channel.en_band == WLAN_BAND_5G))) {
            record_info->max_bw_idx = record_info->max_rssi_idx;
            record_info->en_bandwidth = max_rssi_bss_dscr->st_channel.en_bandwidth;
        } else if (record_info->bss_rssi > max_bw_bss_dscr->c_rssi + LINK_RSSI_DIFF_15_DB) {
            /* 当前最大带宽信号弱15db不选 */
            record_info->max_bw_idx = record_info->max_rssi_idx;
            record_info->en_bandwidth = max_rssi_bss_dscr->st_channel.en_bandwidth;
        }
    }
    if (record_info->max_bw_idx != 0) { // 存在最大带宽idx优先返回
        return record_info->max_bw_idx;
    } else {
        return record_info->max_rssi_idx;
    }
}

OAL_STATIC ml_link_info_stru *hmac_preferred_bss_mld_link_info(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    ml_link_info_stru *mld_link_info = NULL;
    uint8_t link_index;
    mac_bss_dscr_stru *bss_dscr_tmp = NULL;
    hmac_mld_preferred_link_info_stru same_band_info = {0};
    hmac_mld_preferred_link_info_stru diff_band_info = {0};
    uint8_t same_band_idx = 0;
    uint8_t diff_band_idx = 0;
    same_band_info.en_bandwidth = WLAN_BAND_WIDTH_BUTT;
    diff_band_info.en_bandwidth = WLAN_BAND_WIDTH_BUTT;
    for (link_index = 1; link_index < WLAN_AP_MAX_ML_LINK_INFO_NUM; link_index++) {
        if (bss_dscr->ml_info.ml_link_info[link_index].is_add_link_list == OAL_FALSE) {
            continue;
        }
        oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, 0,
            "hmac_preferred_bss_mld_link_info::link bss[%02x:%02x]",
            bss_dscr->ml_info.ml_link_info[link_index].sta_mac_addr[BYTE_OFFSET_4],
            bss_dscr->ml_info.ml_link_info[link_index].sta_mac_addr[BYTE_OFFSET_5]);
        /* 扫描结果看是否能找到该bss */
        bss_dscr_tmp = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
            bss_dscr->ml_info.ml_link_info[link_index].sta_mac_addr);
        /* 优先选择异频作为link 关联; 如果存在多个异频link则优选rssi大的bss */
        if (bss_dscr->ml_info.ml_link_info[link_index].st_channel.en_band != bss_dscr->st_channel.en_band) {
            diff_band_idx = hmac_get_preferred_bss_mld_link_info_idx(hmac_vap, bss_dscr, bss_dscr_tmp, link_index,
                &diff_band_info);
            oam_warning_log1(0, 0, "hmac_preferred_bss_mld_link_info::diff_band_idx[%d]", diff_band_idx);
        } else {
            /* 同频场景，指定优选策略，当前选择rssi较大的bss */
            same_band_idx = hmac_get_preferred_bss_mld_link_info_idx(hmac_vap, bss_dscr, bss_dscr_tmp, link_index,
                &same_band_info);
            oam_warning_log1(0, 0, "hmac_preferred_bss_mld_link_info::same_band_idx[%d]", same_band_idx);
        }
    }
    if (diff_band_idx != 0) {
        mld_link_info = &bss_dscr->ml_info.ml_link_info[diff_band_idx];
    } else if (same_band_idx != 0) {
        mld_link_info = &bss_dscr->ml_info.ml_link_info[same_band_idx];
    } else {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, 0, "hmac_get_bss_mld_link_info::no match link bss!");
    }
    return mld_link_info;
}

ml_link_info_stru *hmac_get_bss_mld_link_info(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    ml_link_info_stru *mld_link_info = NULL;

    if (hmac_vap == NULL || bss_dscr == NULL || bss_dscr->ml_info.ml_cap != OAL_TRUE) {
        return NULL;
    }
    mld_link_info = hmac_preferred_bss_mld_link_info(hmac_vap, bss_dscr);
    if (mld_link_info == NULL) {
        return NULL;
    }
    if (mac_addr_is_zero(mld_link_info->sta_mac_addr)) {
        return NULL;
    }
    return mld_link_info;
}

void hmac_update_mld_link_bss_dscr(hmac_vap_stru *hmac_vap, ml_link_info_stru *mld_link_info, uint8_t *mld_mac)
{
    mac_bss_dscr_stru *bss_dscr = NULL;
    uint8_t *bssid = mld_link_info->sta_mac_addr;
    uint8_t rate_num;

    /* 根据对端link mac地址查找是否有对应的扫描结果 */
    bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info, bssid);
    if (bss_dscr == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_update_mld_link_bss_dscr::bss dscr is null}");
        return;
    }
    if (bss_dscr->ml_info.ml_cap == 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_update_mld_link_bss_dscr::not support mld}");
        return;
    }

    if (memcmp(bss_dscr->ml_info.ml_mac_addr, mld_mac, WLAN_MAC_ADDR_LEN) != 0) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_update_mld_link_bss_dscr::memcpy fail}");
        return;
    }

    /* 拷贝扫描结果中AP在该link的速率集 */
    rate_num = (bss_dscr->uc_num_supp_rates < WLAN_USER_MAX_SUPP_RATES) ?
        bss_dscr->uc_num_supp_rates : WLAN_USER_MAX_SUPP_RATES;
    if (memcpy_s(mld_link_info->auc_supp_rates, WLAN_USER_MAX_SUPP_RATES, bss_dscr->auc_supp_rates, rate_num) != EOK) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_update_mld_link_bss_dscr::memcpy fail}");
        return;
    }
    mld_link_info->uc_num_supp_rates = bss_dscr->uc_num_supp_rates;

    /* 拷贝扫描结果中AP在该link的信道参数 */
    if (memcpy_s(&mld_link_info->st_channel, sizeof(mac_channel_stru),
                 &bss_dscr->st_channel, sizeof(mac_channel_stru)) != EOK) {
        return;
    }
    mld_link_info->st_channel.en_bandwidth = bss_dscr->st_channel.en_bandwidth;
}

OAL_STATIC void hmac_mld_set_ap_link_id_in_netdev_priv(hmac_vap_stru *hmac_vap, uint8_t link_id)
{
    oal_net_device_stru  *ml_net_device = hmac_vap->pst_net_device;
    oal_netdev_priv_stru *ml_net_dev_priv = NULL;

    if (ml_net_device == NULL) {
        return;
    }
    ml_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(ml_net_device);
    ml_net_dev_priv->mlo_cfg_info.ap_link_id = link_id;
}

uint32_t hmac_init_link_vap_bss(hmac_vap_stru *hmac_vap, hmac_vap_stru *link_hmac_vap,
    mac_bss_dscr_stru **link_bss_dscr, mac_bss_dscr_stru *bss_dscr)
{
    ml_link_info_stru *mld_link_info;
    mac_bss_dscr_stru *bss_dscr_tmp = NULL;

    mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, bss_dscr);
    if (mld_link_info == NULL) {
        return OAL_ERR_CODE_ML_MISMATCH_LINK_BSS;
    }
    /* 如果工作link的扫描结果中没有携带其他link的速率集，则查找是否有其他link的扫描结果 */
    if (mld_link_info->uc_num_supp_rates == 0) {
        hmac_update_mld_link_bss_dscr(hmac_vap, mld_link_info, bss_dscr->ml_info.ml_mac_addr);
    }
    bss_dscr_tmp = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
        mld_link_info->sta_mac_addr);
    if (bss_dscr_tmp == NULL) {
        return OAL_FAIL;
    }
    /* 检查是隐藏ssid，进行单link关联 */
    if (bss_dscr_tmp->ac_ssid[0] == '\0') {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{hmac_init_link_vap_bss::bss is hidden}");
        return OAL_FAIL;
    }
    /* 检查扫描结果不支持mlo，进行单link关联 */
    if ((bss_dscr_tmp->ml_info.ml_cap == OAL_FALSE) ||
        (memcmp(bss_dscr_tmp->ml_info.ml_mac_addr, bss_dscr->ml_info.ml_mac_addr, WLAN_MAC_ADDR_LEN) != 0)) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id,
            OAM_SF_CFG, "{hmac_init_link_vap_bss::link bss not support mlo}");
        return OAL_FAIL;
    }

    *link_bss_dscr = bss_dscr_tmp;
    /* 拷贝扫描结果中的ap mld信息到link vap下 */
    memcpy_s(&link_hmac_vap->mld_vap_link_info.link_ap_info, sizeof(ml_link_info_stru),
        mld_link_info, sizeof(ml_link_info_stru));
    hmac_mld_set_ap_link_id_in_netdev_priv(link_hmac_vap, link_hmac_vap->mld_vap_link_info.link_ap_info.link_id);

    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_mld_config_connect
 * 功能描述  : MLD关联时，配置connect参数流程
 * 1.日    期  : 2023年12月8日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_mld_config_connect(hmac_vap_stru *hmac_vap, hmac_conn_param_stru *connect_param,
    mac_bss_dscr_stru *bss_dscr, mac_conn_security_stru *conn_sec, oal_bool_enum_uint8 sta_connected)
{
    uint32_t ret = OAL_SUCC;
    uint8_t link_id;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_vap_stru *link_hmac_vap = NULL;
    mac_bss_dscr_stru *link_bss_dscr = NULL;
    mac_vap_stru *link_mac_vap = NULL;
    if (oal_any_null_ptr4(hmac_vap, connect_param, bss_dscr, conn_sec)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_config_connect::argument is null.}");
        return OAL_FAIL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_config_connect::get mld_vap_mgmt fail.");
        return OAL_FAIL;
    }

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL) {
            continue;
        }

        if (hmac_vap_is_ml_mode(link_hmac_vap) == OAL_TRUE) {
            continue;
        }
        // 初始化非工作link vap的bss_dscr
        ret = hmac_init_link_vap_bss(hmac_vap, link_hmac_vap, &link_bss_dscr, bss_dscr);
        if (ret != OAL_SUCC) {
            oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                "{hmac_mld_config_connect::init link bss dscr fail back to single radio, ret[%d]!", ret);
            hmac_vap_mld_mode_change(link_hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY,
                hmac_vap->st_vap_base_info.uc_vap_id);
            continue;
        }
        link_mac_vap = &link_hmac_vap->st_vap_base_info;
        /* 检查link vap入网信道是否合法，否则退化为single radio入网 */
        if (hmac_check_coex_channel_is_valid(link_mac_vap, &link_bss_dscr->st_channel) != OAL_TRUE) {
            oam_warning_log0(link_mac_vap->uc_vap_id, 0, "{hmac_mld_config_connect::link chan invalid back to single}");
            hmac_vap_mld_mode_change(link_hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY,
                hmac_vap->st_vap_base_info.uc_vap_id);
            continue;
        }
        ret = hmac_config_connect_process(link_hmac_vap, connect_param, link_bss_dscr, conn_sec, sta_connected);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_config_connect::config link vap fail!.");
            return OAL_FAIL;
        }
    }

    memcpy_s(&hmac_vap->mld_vap_link_info.link_ap_info, sizeof(ml_link_info_stru),
        &bss_dscr->ml_info.ml_link_info[0], sizeof(ml_link_info_stru));

    hmac_mld_set_ap_link_id_in_netdev_priv(hmac_vap, hmac_vap->mld_vap_link_info.link_ap_info.link_id);
    /* master link vap config */
    return hmac_config_connect_process(hmac_vap, connect_param, bss_dscr, conn_sec, sta_connected);
}

uint32_t hmac_mld_connect_process(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr,
    hmac_conn_param_stru *connect_param, mac_conn_security_stru *conn_sec, oal_bool_enum_uint8 sta_connected)
{
    mac_vap_state_enum_uint8 vap_state = hmac_vap->st_vap_base_info.en_vap_state;
    uint32_t ret;

    if (hmac_sta_check_start_ml_scan(hmac_vap, bss_dscr) == OAL_TRUE) {
        if (!(vap_state == MAC_VAP_STATE_STA_FAKE_UP || vap_state == MAC_VAP_STATE_STA_SCAN_COMP)) {
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_mld_connect_process::vap_state[%d].", vap_state);
        }
        hmac_vap->ml_scan_param.vap_state_bak = vap_state;
        ret = hmac_sta_start_ml_scan(hmac_vap, bss_dscr, MAC_VAP_STATE_STA_WAIT_MLD_SCAN, connect_param);
        if (ret != OAL_SUCC) {
            hmac_vap->st_vap_base_info.en_vap_state = hmac_vap->ml_scan_param.vap_state_bak;
        }
        return ret;
    }
    hmac_clear_ml_scan_params(hmac_vap);

    /* 切换master link、slave link对应vap的mld mode */
    hmac_vap_change_to_mld_process(hmac_vap, bss_dscr);

    /* MLD模式 遍历Link配置对应VAP */
    return hmac_mld_config_connect(hmac_vap, connect_param, bss_dscr, conn_sec, sta_connected);
}

void hmac_wifi7_get_connect_flag_cmd(mac_vap_stru *mac_vap, hmac_conn_param_stru *connect_param,
    mac_bss_dscr_stru *bss_dscr)
{
#ifdef CONFIG_HISI_WIFI7_MLO
    uint8_t connect_flag_cmd;
    if (g_wlan_spec_cfg->feature_ml_is_supported != OAL_TRUE
        || g_wlan_spec_cfg->feature_11be_is_open != OAL_TRUE) {
        return;
    }
    if (mac_vap == NULL || connect_param == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_wifi7_get_connect_flag_cmd::nulll ptr]}");
        return;
    }
    connect_flag_cmd = (uint8_t)(connect_param->flags & ASSOC_REQ_DISABLE_EHT);
    if (connect_flag_cmd || hmac_wifi7_self_cure_mac_is_wifi7_blacklist_type(connect_param->auc_bssid)) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_wifi7_get_connect_flag_cmd::wifi7 connect disable}");
        mac_mib_set_EHTOptionImplemented(mac_vap->pst_mib_info, OAL_FALSE);
        mac_mib_set_MultiLinkActivated(mac_vap->pst_mib_info, OAL_FALSE);
        return;
    }
    connect_flag_cmd = (uint8_t)(connect_param->flags & CONNECT_REQ_MLO_SUPPORT);
    if (!connect_flag_cmd || hmac_wifi7_self_cure_mac_is_mld_blacklist_type(connect_param->auc_bssid) ||
        bss_dscr->supp_eht_double_wifi == OAL_TRUE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_wifi7_get_connect_flag_cmd::mld connect disable}");
        mac_mib_set_MultiLinkActivated(mac_vap->pst_mib_info, OAL_FALSE);
        return;
    }
#ifdef _PRE_WLAN_FEATURE_11BE
    mac_vap_init_mib_mlo(mac_vap);
#endif
#endif
}


#else
uint32_t hmac_check_is_mld_connect(mac_vap_stru *mac_vap, mac_bss_dscr_stru *bss_dscr)
{
    return OAL_FALSE;
}
#endif
