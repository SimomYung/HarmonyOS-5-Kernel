/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Multi-Link特性处理
 * 作    者 : wifi
 * 创建日期 : 2023年11月9日
 */
#include "hmac_multilink.h"
#include "oal_ext_if.h"
#include "securec.h"
#include "mac_mib.h"
#include "mac_ie.h"
#include "mac_frame.h"
#include "mac_frame_inl.h"
#include "mac_function.h"
#include "mac_frame_11be.h"
#include "mac_frame_ml.h"
#include "hmac_encap_frame_sta.h"
#include "hmac_11ax.h"
#include "hmac_scan_bss.h"
#include "hmac_ml_scan.h"
#include "hmac_mgmt_sta.h"
#include "hmac_mld_ext.h"
#include "hmac_mgmt_sta_up.h"
#include "hmac_multilink_reconfig.h"
#include "hmac_multilink.h"
#include "hmac_roam_main.h"
#include "hmac_roam_connect.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MULTILINK_C

#ifdef _PRE_WLAN_FEATURE_ML
/* mld上报管理帧，sa和da替换为mld级别mac地址 */
void hmac_mld_rx_mgmt_change_addr(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf)
{
    mac_ieee80211_frame_stru *frame_hdr = NULL;

    if (oal_any_null_ptr2(hmac_vap, netbuf)) {
        return;
    }

    frame_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_header(netbuf);
    if (frame_hdr == NULL) {
        return;
    }

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        return;
    }

    /* TODO:aput需要把da替换成自己的mld mac地址，sa替换为user的mld mac地址 */
    if (is_sta(&hmac_vap->st_vap_base_info)) {
        memcpy_s(frame_hdr->auc_address1, WLAN_MAC_ADDR_LEN,
            hmac_vap->mld_vap_link_info.mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN);
        memcpy_s(frame_hdr->auc_address2, WLAN_MAC_ADDR_LEN,
            hmac_vap->mld_vap_link_info.mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN);
    }
}

/*
 * 函 数 名  : hmac_mld_tx_mgmt_change_addr
 * 功能描述  : supplicant组ML auth帧，驱动侧替换mac头的地址为link mac地址
 */
void hmac_mld_tx_mgmt_change_addr(mac_vap_stru *mac_vap, oal_netbuf_stru *netbuf)
{
    mac_ieee80211_frame_stru *frame_hdr = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_vap_stru *work_hmac_vap = NULL;
    mac_vap_stru *work_mac_vap = NULL;

    if (oal_any_null_ptr2(mac_vap, netbuf)) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_mld_tx_mgmt_change_addr:: argument is NULL.");
        return;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return;
    }

    /* 非mld vap发管理帧不用替换mac地址为link级别 */
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE ||
        hmac_vap->mld_vap_link_info.mld_vap_manage == NULL ||
        hmac_vap->mld_vap_link_info.mld_vap_manage->in_use != OAL_TRUE) {
        return;
    }

    frame_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_header(netbuf);

    oam_warning_log4(0, OAM_SF_TX, "hmac_mld_tx_mgmt_change_addr::old da=[%2X:%2X] sa=[%2X:%2X]",
        frame_hdr->auc_address1[BYTE_OFFSET_4], frame_hdr->auc_address1[BYTE_OFFSET_5],
        frame_hdr->auc_address2[BYTE_OFFSET_4], frame_hdr->auc_address2[BYTE_OFFSET_5]);
    oam_warning_log2(0, OAM_SF_TX, "hmac_mld_tx_mgmt_change_addr::old bssid=[%2X:%2X]",
        frame_hdr->auc_address3[BYTE_OFFSET_4], frame_hdr->auc_address3[BYTE_OFFSET_5]);

    /* 默认是当前vap，如果当前vap不工作，找到工作的vap */
    work_hmac_vap = (hmac_vap->mld_vap_link_info.work_flag == 1) ? hmac_vap :
        hmac_mlsr_get_cur_work_link_vap(hmac_vap->mld_vap_link_info.mld_vap_manage);
    if (work_hmac_vap == NULL) {
        return;
    }

    work_mac_vap = &work_hmac_vap->st_vap_base_info;
    /* TODO:aput需要把da替换成sta的mac地址，sa替换为自己的mac地址 */
    if (is_sta(work_mac_vap) == OAL_TRUE) {
        memcpy_s(frame_hdr->auc_address1, WLAN_MAC_ADDR_LEN, work_mac_vap->auc_bssid, WLAN_MAC_ADDR_LEN);
        memcpy_s(frame_hdr->auc_address2, WLAN_MAC_ADDR_LEN, mac_mib_get_StationID(work_mac_vap), WLAN_MAC_ADDR_LEN);
        memcpy_s(frame_hdr->auc_address3, WLAN_MAC_ADDR_LEN, work_mac_vap->auc_bssid, WLAN_MAC_ADDR_LEN);
    }

    oam_warning_log4(0, OAM_SF_TX, "hmac_mld_tx_mgmt_change_addr::new da=[%2X:%2X] sa=[%2X:%2X]",
        frame_hdr->auc_address1[BYTE_OFFSET_4], frame_hdr->auc_address1[BYTE_OFFSET_5],
        frame_hdr->auc_address2[BYTE_OFFSET_4], frame_hdr->auc_address2[BYTE_OFFSET_5]);
    oam_warning_log2(0, OAM_SF_TX, "hmac_mld_tx_mgmt_change_addr::new bssid=[%2X:%2X]",
        frame_hdr->auc_address3[BYTE_OFFSET_4], frame_hdr->auc_address3[BYTE_OFFSET_5]);
}

uint8_t hmac_set_auth_req_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *mgmt_frame)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint8_t mld_addr[WLAN_MAC_ADDR_LEN];

    if (oal_any_null_ptr2(hmac_vap, mgmt_frame)) {
        return 0;
    }

    mac_vap = &hmac_vap->st_vap_base_info;
    if (oal_any_null_ptr2(mac_vap, mac_vap->pst_mib_info)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_set_auth_req_basic_ml_ie::mac_vap or mib_info is null.}");
        return 0;
    }

    if ((mac_forbid_encap_eht_ie(&hmac_vap->st_vap_base_info) == OAL_TRUE) ||
        (mac_mib_get_MultiLinkActivated(mac_vap->pst_mib_info) != OAL_TRUE)) {
        return 0;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_set_auth_req_basic_ml_ie::get mld_vap_mgmt fail.");
        return 0;
    }
    if (memcpy_s(mld_addr, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return 0;
    }

    return mac_set_auth_basic_ml_ie(mac_vap, mgmt_frame, mld_addr, WLAN_MAC_ADDR_LEN);
}

uint8_t hmac_set_auth_resp_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *mgmt_frame, uint8_t is_ml_auth)
{
    /* auth rsp帧中的basic_ml_ie和auth req帧中的一致，复用接口 */
    return ((is_ml_auth == OAL_TRUE) ? hmac_set_auth_req_basic_ml_ie(hmac_vap, mgmt_frame) : 0);
}
/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_sta_profile
 * 功能描述   : 设置assoc req帧的sta profile字段
 */
OAL_STATIC uint8_t hmac_set_assoc_req_basic_ml_sta_profile(hmac_vap_stru *hmac_sta, uint8_t *sta_profile)
{
    mac_vap_stru *mac_vap = &hmac_sta->st_vap_base_info;
    uint8_t *tmp_profile = sta_profile;
    uint8_t ie_len = 0;

    if (mac_vap == NULL) {
        return 0;
    }

    /* 设置 capability information field */
    mac_set_cap_info_sta(mac_vap, tmp_profile);
    tmp_profile += MAC_CAP_INFO_LEN;

    /* 设置 Supported Rates IE */
    hmac_set_supported_rates_ie_asoc_req(hmac_sta, tmp_profile, &ie_len);
    tmp_profile += ie_len;

    /* 设置 Extended Supported Rates IE */
    hmac_set_exsup_rates_ie_asoc_req(hmac_sta, tmp_profile, &ie_len);
    tmp_profile += ie_len;

    /* 设置 Power Capability IE */
    mac_set_power_cap_ie((void *)mac_vap, tmp_profile, &ie_len);
    tmp_profile += ie_len;

    /* 设置 Supported channel IE */
    mac_set_supported_channel_ie((void *)mac_vap, tmp_profile, &ie_len);
    tmp_profile += ie_len;

    /* 设置 HT Capability IE */
    tmp_profile += mac_set_ht_capabilities_ie(mac_vap, tmp_profile);

    /* 设置 VHT Capability IE */
    mac_set_vht_capabilities_ie(mac_vap, tmp_profile, &ie_len);
    tmp_profile += ie_len;

    /* 设置 HE Capability IE */
    mac_set_he_capabilities_ie(mac_vap, tmp_profile, &ie_len);
    tmp_profile += ie_len;

#ifdef _PRE_WLAN_FEATURE_11BE
    tmp_profile += mac_set_eht_ie(mac_vap, tmp_profile, WLAN_ASSOC_REQ);
#endif

    return (tmp_profile - sta_profile);
}

/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_sta_info
 * 功能描述   : 设置assoc req帧的sta info字段
 */
OAL_STATIC uint8_t hmac_set_assoc_req_basic_ml_sta_info(hmac_vap_stru *hmac_sta, uint8_t *sta_info,
    mac_ml_sta_control_union sta_control)
{
    uint8_t      sta_info_len;
    uint8_t     *tmp_sta_info = sta_info;

    tmp_sta_info += 1; /* 偏移 1字节 的sta info长度 */

    if (sta_control.basic_ml_sta_control.sta_mac_address_present == 1) {
        if (memcpy_s(tmp_sta_info, WLAN_MAC_ADDR_LEN,
            mac_mib_get_StationID(&hmac_sta->st_vap_base_info), WLAN_MAC_ADDR_LEN) != EOK) {
            oam_error_log0(0, OAM_SF_ASSOC, "{hmac_set_assoc_req_basic_ml_sta_info::memcpy_s fail.}");
            return 0;
        }
        tmp_sta_info += WLAN_MAC_ADDR_LEN;
    }

    sta_info_len = tmp_sta_info - sta_info;
    sta_info[0] = sta_info_len;

    return sta_info_len;
}

/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_per_sta_profile
 * 功能描述   : 设置assoc req帧的per-STA Profile字段
 */
OAL_STATIC uint8_t hmac_set_assoc_req_basic_ml_per_sta_profile(hmac_vap_stru *hmac_sta, uint8_t *req_frame)
{
    uint8_t                       per_sta_len;
    uint8_t                       sta_info_len, sta_profile_len;
    mac_ml_per_sta_profiler_stru *per_sta_profile = NULL;
    uint8_t                      *req_frame_tmp = req_frame;

    per_sta_profile = (mac_ml_per_sta_profiler_stru *)req_frame;
    per_sta_profile->sub_eid = 0;

    /* 设置STA Control字段 */
    memset_s(&per_sta_profile->ml_control, STA_CONTROL_FIELD_LEN, 0, STA_CONTROL_FIELD_LEN);
    per_sta_profile->ml_control.basic_ml_sta_control.link_id = hmac_sta->mld_vap_link_info.link_ap_info.link_id;
    per_sta_profile->ml_control.basic_ml_sta_control.complete_profile = 1;
    per_sta_profile->ml_control.basic_ml_sta_control.sta_mac_address_present = 1;
    req_frame_tmp += sizeof(mac_ml_per_sta_profiler_stru); /* 指针偏移sub_id + sub_len + sta_control长度 */

    /* 设置STA Info */
    sta_info_len = hmac_set_assoc_req_basic_ml_sta_info(hmac_sta, req_frame_tmp, per_sta_profile->ml_control);
    req_frame_tmp += sta_info_len;

    /* 设置STA Profile */
    sta_profile_len = hmac_set_assoc_req_basic_ml_sta_profile(hmac_sta, req_frame_tmp);
    req_frame_tmp += sta_profile_len;

    per_sta_len = (uint8_t)(req_frame_tmp - req_frame);
    per_sta_profile->ie_len = (uint8_t)(per_sta_len - MAC_IE_HDR_LEN);

    return per_sta_len;
}

/*
 * 函 数 名   : hmac_set_assoc_rsp_basic_ml_per_sta_profile
 * 功能描述   : 设置assoc rsp帧的per-STA Profile字段
 */
OAL_STATIC uint8_t hmac_set_assoc_rsp_basic_ml_per_sta_profile(hmac_vap_stru *hmac_vap, uint8_t *frame)
{
    /* TODO:设置assoc rsp帧的per-STA Profile字段 */
    return 0;
}

/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_link_info
 * 功能描述   : 设置assoc req帧的link info字段，遍历link设置对应的per-STA Profile
 */
OAL_STATIC uint8_t hmac_set_assoc_req_basic_ml_link_info(hmac_vap_stru *hmac_sta, hmac_mld_manage_stru *mld_vap_manage,
    uint8_t *link_info)
{
    hmac_vap_stru *link_hmac_vap = NULL;
    uint8_t *tmp_link_info = link_info;
    uint8_t link_id;
    uint8_t per_sta_len;
    uint8_t sta_profile_len = 0;

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        /* 若不存在link vap 或 link vap为工作链路vap，不封装对应的per-STA Info字段 */
        if (link_hmac_vap == NULL ||
            link_hmac_vap->st_vap_base_info.uc_vap_id == hmac_sta->st_vap_base_info.uc_vap_id) {
            continue;
        }

        /* 设置Per-STA Info */
        per_sta_len = hmac_set_assoc_req_basic_ml_per_sta_profile(link_hmac_vap, tmp_link_info);
        sta_profile_len += per_sta_len;
        tmp_link_info += per_sta_len;
    }

    return sta_profile_len;
}

OAL_STATIC uint8_t hmac_set_assoc_rsp_basic_ml_link_info(hmac_vap_stru *hmac_vap, hmac_mld_manage_stru *mld_vap_manage,
    uint8_t *link_info)
{
    hmac_vap_stru *link_hmac_vap = NULL;
    uint8_t *tmp_link_info = link_info;
    uint8_t link_id;
    uint8_t per_sta_len;
    uint8_t sta_profile_len = 0;

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        /* 若不存在link vap 或 link vap为工作链路vap，不封装对应的per-STA Info字段 */
        if (link_hmac_vap == NULL ||
            link_hmac_vap->st_vap_base_info.uc_vap_id == hmac_vap->st_vap_base_info.uc_vap_id) {
            continue;
        }

        /* 设置Per-STA Info */
        per_sta_len = hmac_set_assoc_rsp_basic_ml_per_sta_profile(link_hmac_vap, tmp_link_info);
        sta_profile_len += per_sta_len;
        tmp_link_info += per_sta_len;
    }

    return sta_profile_len;
}
/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_common_info
 * 功能描述   : 设置ML IE的common info字段
 */
OAL_STATIC uint8_t hmac_set_assoc_req_basic_ml_common_info(hmac_vap_stru *hmac_sta,
    uint8_t *common_info, mac_ml_control_union ml_control)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint8_t *tmp_common_info = common_info;
    hmac_mld_vap_info_stru *mld_vap_link_info = &hmac_sta->mld_vap_link_info;
    uint8_t common_info_len;
    uint8_t max_simultaneous_link_cnt = 0;

    mld_vap_manage = hmac_sta->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_set_assoc_req_basic_ml_ie_check::get mld hmac vap fail.");
        return 0;
    }
    tmp_common_info += 1; /* 跳过IE长度的1字节 */

    /* 设置MLD MAC地址 */
    if (memcpy_s(tmp_common_info, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_set_assoc_req_basic_ml_common_info::memcpy_s fail.}");
        return 0;
    }
    tmp_common_info += WLAN_MAC_ADDR_LEN;

    /* eml capabilities 子字段 */
    if (ml_control.basic_ml_control.ml_eml_capabilities_present == 1) {
        mac_set_eml_capabilities_field(mld_vap_link_info, tmp_common_info);
        tmp_common_info += EML_CAPBILITIES_FIELD_LEN;
    }

    /* mld capabilities 子字段 */
    if (ml_control.basic_ml_control.ml_mld_capabilities_and_operations_present == 1) { /* 1表示字段存在 0表示不存在 */
        /* 认证要求：MLSR关联时最大同时支持链路数填1，STR关联时填2 */
        max_simultaneous_link_cnt = hmac_get_mld_assoc_mode(hmac_sta) == MLD_ASSOC_MODE_MLSR ?
            1 : mld_vap_manage->mld_link_cnt;
        mac_set_mld_capabilities_field(&hmac_sta->st_vap_base_info, tmp_common_info, max_simultaneous_link_cnt);
        tmp_common_info += MLD_CAPBILITIES_FIELD_LEN;
    }

    common_info_len = tmp_common_info - common_info;
    common_info[0] = common_info_len;
    return common_info_len;
}

/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_common_info
 * 功能描述   : 设置ML IE的common info字段
 */
OAL_STATIC uint8_t hmac_set_assoc_rsp_basic_ml_common_info(hmac_vap_stru *hmac_vap,
    uint8_t *common_info, mac_ml_control_union ml_control)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint8_t *tmp_common_info = common_info;
    uint8_t common_info_len;

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_set_assoc_rsp_basic_ml_common_info::get mld hmac vap fail.");
        return 0;
    }
    tmp_common_info += 1; /* 跳过IE长度的1字节 */

    /* 设置MLD MAC地址 */
    if (memcpy_s(tmp_common_info, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_set_assoc_rsp_basic_ml_common_info::memcpy_s fail.}");
        return 0;
    }
    tmp_common_info += WLAN_MAC_ADDR_LEN;

    /* Link ID Info子字段 */
    if (ml_control.basic_ml_control.ml_link_id_info_present == 1) {
        mac_set_ml_link_id_info_field(hmac_vap->mld_vap_link_info.link_id, tmp_common_info);
        tmp_common_info += ML_LINK_ID_INFO_FIELD_LEN;
    }

    /* BSS Parameters Change Count子字段 */
    if (ml_control.basic_ml_control.ml_bss_parameters_change_count_present == 1) {
        tmp_common_info[0] = hmac_vap->bss_critical_update.bss_parameters_change_count;
        tmp_common_info += BSS_PARAMETERS_CHANGE_COUNT_LEN;
    }

    /* eml capabilities 子字段 */
    if (ml_control.basic_ml_control.ml_eml_capabilities_present == 1) {
        mac_set_eml_capabilities_field(&(hmac_vap->mld_vap_link_info), tmp_common_info);
        tmp_common_info += EML_CAPBILITIES_FIELD_LEN;
    }

    /* mld capabilities 子字段 */
    if (ml_control.basic_ml_control.ml_mld_capabilities_and_operations_present == 1) { /* 1表示字段存在 0表示不存在 */
        mac_set_mld_capabilities_field(&hmac_vap->st_vap_base_info, tmp_common_info,
            (ml_control.basic_ml_control.ml_eml_capabilities_present == 1) ? 1 : mld_vap_manage->mld_link_cnt);
        tmp_common_info += MLD_CAPBILITIES_FIELD_LEN;
    }

    /* mld id 子字段 */
    if (ml_control.basic_ml_control.ml_mld_id_present == 1) {
        tmp_common_info[0] = mld_vap_manage->mld_id;
        tmp_common_info += MLD_ID_FIELD_LEN;
    }

    common_info_len = tmp_common_info - common_info;
    common_info[0] = common_info_len;
    return common_info_len;
}

/*
 * 函 数 名   : hmac_set_assoc_frame_basic_ml_ie_check
 * 功能描述   : 检查是否需要填充ml ie，并判断当前vap是否支持mld
 */
OAL_STATIC uint32_t hmac_set_basic_ml_ie_check(hmac_vap_stru *hmac_vap)
{
    mac_vap_stru *mac_vap = NULL;

    mac_vap = &hmac_vap->st_vap_base_info;
    if (oal_any_null_ptr2(mac_vap, mac_vap->pst_mib_info)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_set_basic_ml_ie_check::mac_vap or mib_info is null.}");
        return OAL_FAIL;
    }

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        return OAL_FAIL;
    }

    if ((mac_forbid_encap_eht_ie(&hmac_vap->st_vap_base_info) == OAL_TRUE) ||
        (mac_mib_get_MultiLinkActivated(mac_vap->pst_mib_info) != OAL_TRUE)) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_set_basic_ml_ie_check::not support multilink!}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名   : hmac_set_assoc_req_basic_ml_ie
 * 功能描述   : 设置assoc req帧中的ML IE
 */
uint8_t hmac_set_assoc_req_basic_ml_ie(hmac_vap_stru *hmac_sta, uint8_t *req_frame)
{
    uint8_t                     common_info_len, link_info_len;
    mac_eht_ml_ie_header_stru  *ml_ie_hdr = NULL;
    uint8_t                    *req_frame_tmp = req_frame;
    hmac_mld_manage_stru       *mld_vap_manage;

    if (oal_any_null_ptr2(hmac_sta, req_frame)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_set_assoc_req_basic_ml_ie::argument is null.}");
        return 0;
    }

    if (hmac_set_basic_ml_ie_check(hmac_sta) != OAL_SUCC) {
        return 0;
    }

    mld_vap_manage = hmac_sta->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_set_assoc_req_basic_ml_ie::get mld hmac vap fail.");
        return 0;
    }

    /* 设置 Multi-Link IE header */
    ml_ie_hdr = (mac_eht_ml_ie_header_stru *)req_frame_tmp;
    ml_ie_hdr->eid = MAC_EID_MULTILINK;
    ml_ie_hdr->eid_extension = MAC_EID_EXT_EHT_ML;

    /* 设置 Multi-Link Control */
    mac_set_basic_ml_ie_control(hmac_sta, &ml_ie_hdr->ml_control);
    req_frame_tmp += MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN;

    /* 设置 common info */
    common_info_len = hmac_set_assoc_req_basic_ml_common_info(hmac_sta, req_frame_tmp, ml_ie_hdr->ml_control);
    req_frame_tmp += common_info_len;

    /* 设置Link Info 信息 */
    link_info_len = hmac_set_assoc_req_basic_ml_link_info(hmac_sta, mld_vap_manage, req_frame_tmp);
    req_frame_tmp += link_info_len;

    ml_ie_hdr->ie_len = (req_frame_tmp - req_frame) - MAC_IE_HDR_LEN;

    return (req_frame_tmp - req_frame);
}

/*
 * 函 数 名   : hmac_set_assoc_rsp_basic_ml_ie
 * 功能描述   : 设置assoc rsp帧中的ML IE
 */
uint8_t hmac_set_assoc_rsp_basic_ml_ie(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *rsp_frame)
{
    uint8_t common_info_len, link_info_len;
    uint8_t *rsp_frame_tmp = rsp_frame;
    mac_eht_ml_ie_header_stru *ml_ie_hdr = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    uint8_t link_id;
    uint8_t is_need_ml_ie = OAL_FALSE;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, rsp_frame)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_set_assoc_rsp_basic_ml_ie::argument is null.}");
        return 0;
    }

    if (hmac_set_basic_ml_ie_check(hmac_vap) != OAL_SUCC) {
        return 0;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (mld_vap_manage == NULL || mld_user_manage == NULL) {
        return 0;
    }

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        if (mld_user_manage->link_assoc_info[link_id].link_assoc_is_valid == OAL_TRUE) {
            is_need_ml_ie = OAL_TRUE;
            break;
        }
    }

    if (is_need_ml_ie == OAL_FALSE) {
        return 0;
    }

    /* 设置 Multi-Link IE header */
    ml_ie_hdr = (mac_eht_ml_ie_header_stru *)rsp_frame_tmp;
    ml_ie_hdr->eid = MAC_EID_MULTILINK;
    ml_ie_hdr->eid_extension = MAC_EID_EXT_EHT_ML;

    /* 设置 Multi-Link Control */
    mac_set_basic_ml_ie_control(hmac_vap, &ml_ie_hdr->ml_control);
    rsp_frame_tmp += MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN;

    /* 设置 common info */
    common_info_len = hmac_set_assoc_rsp_basic_ml_common_info(hmac_vap, rsp_frame_tmp, ml_ie_hdr->ml_control);
    rsp_frame_tmp += common_info_len;

    /* 设置Link Info 信息 */
    link_info_len = hmac_set_assoc_rsp_basic_ml_link_info(hmac_vap, mld_vap_manage, rsp_frame_tmp);
    rsp_frame_tmp += link_info_len;

    ml_ie_hdr->ie_len = (rsp_frame_tmp - rsp_frame) - MAC_IE_HDR_LEN;

    return (rsp_frame_tmp - rsp_frame);
}

OAL_STATIC uint32_t hmac_scan_update_bss_multi_link_info(mac_bss_dscr_stru *bss_dscr)
{
    bss_dscr->ml_info.ml_link_info[0].link_id = bss_dscr->ml_info.link_id;
    if (memcpy_s(bss_dscr->ml_info.ml_link_info[0].sta_mac_addr, WLAN_MAC_ADDR_LEN,
                 bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_bss_multi_link_info::memcpy fail!}");
        return OAL_FAIL;
    }

    if (memcpy_s(&(bss_dscr->ml_info.ml_link_info[0].st_channel), sizeof(mac_channel_stru),
                 &bss_dscr->st_channel, sizeof(mac_channel_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_bss_multi_link_info::memcpy fail!}");
        return OAL_FAIL;
    }
    bss_dscr->ml_info.ml_link_info[0].st_channel.en_bandwidth = bss_dscr->en_channel_bandwidth;
    bss_dscr->ml_info.ml_link_info[0].is_add_link_list = OAL_TRUE;
    bss_dscr->ml_info.ml_cap = OAL_TRUE;
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_assoc_rsp_basic_ml_sta_profile(hmac_vap_stru *hmac_vap,
    uint8_t *sta_profile, uint16_t sta_profile_len, ml_link_info_stru *ml_link_info)
{
    uint8_t                      *inheritance_ies = NULL;
    uint16_t                      inheritance_ie_len;
    mac_status_code_enum_uint16   asoc_status;

    if (sta_profile_len < MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN) {
        return OAL_FAIL;
    }

    asoc_status = mac_get_asoc_status(sta_profile);
    if (asoc_status != MAC_SUCCESSFUL_STATUSCODE) {
        ml_link_info->asoc_succ = 0;
        ml_link_info->inheritance_ies = NULL;
        ml_link_info->inheritance_ie_len = 0;

        return OAL_SUCC;
    }

    ml_link_info->asoc_succ = 1; /* 关联成功 */
    inheritance_ies = sta_profile + MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN;
    inheritance_ie_len = sta_profile_len - MAC_CAP_INFO_LEN - MAC_STATUS_CODE_LEN;

    if (inheritance_ie_len > 0) {
        ml_link_info->inheritance_ies = (uint8_t *)oal_memalloc(inheritance_ie_len);
        if (ml_link_info->inheritance_ies == NULL) {
            return OAL_FAIL;
        }
        memcpy_s(ml_link_info->inheritance_ies, inheritance_ie_len, inheritance_ies, inheritance_ie_len);
        ml_link_info->inheritance_ie_len = inheritance_ie_len;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名   : hmac_scan_alloc_frame_body
 * 功能描述   : 申请临时内存，保存sta-profile的IE信息
 */
OAL_STATIC uint8_t *hmac_scan_alloc_frame_body(uint16_t frame_len, uint8_t *sta_profile, uint8_t sta_profile_len)
{
    uint8_t *frame_body = oal_memalloc(frame_len);

    if (oal_unlikely(frame_body == NULL)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_alloc_frame_body::oal_memalloc fail.}");
        return NULL;
    }

    memset_s(frame_body, frame_len, 0, frame_len);
    if (memcpy_s(frame_body + frame_len - sta_profile_len, sta_profile_len, sta_profile, sta_profile_len) != EOK) {
        oal_free(frame_body);
        return NULL;
    }

    return frame_body;
}

hmac_vap_stru *hmac_mld_get_sta_vap_by_ap_link_id(hmac_vap_stru *hmac_vap, int8_t ap_link_id)
{
    uint8_t link_idx;
    hmac_vap_stru *hmac_link_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    /* 非mld场景，cfg80211接口下发命令的link id默认为-1，不处理直接返回原hmac_vap */
    if (ap_link_id == -1) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_mld_get_link_vap_by_link_id::not mld, return}");
        return hmac_vap;
    }

    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_get_link_vap_by_link_id::hmac_vap null}");
        return NULL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_get_link_vap_by_link_id::mld_vap_manage null}");
        return NULL;
    }

    for (link_idx = 0; link_idx < mld_vap_manage->mld_link_cnt; link_idx++) {
        hmac_link_vap = mld_vap_manage->link_vap[link_idx];
        if (hmac_link_vap == NULL) {
            continue;
        }
        if (hmac_link_vap->mld_vap_link_info.link_ap_info.link_id == ap_link_id) {
            return hmac_link_vap;
        }
    }
    return NULL;
}

OAL_STATIC void hmac_sta_up_parse_probe_rsp_basic_ml_sta_profile(hmac_vap_stru *hmac_vap, mac_ies_info *ies_info,
    ml_link_info_stru *ml_link_info)
{
    hmac_vap_stru *link_vap = NULL;
    hmac_user_stru *link_user = NULL;

    if (hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) {
        return;
    }
    link_vap = hmac_mld_get_sta_vap_by_ap_link_id(hmac_vap, ml_link_info->link_id);
    if (link_vap == NULL) {
        return;
    }

    link_user = mac_res_get_hmac_user(link_vap->st_vap_base_info.us_assoc_vap_id);
    if (link_user == NULL) {
        return;
    }

    /* 更新edca参数,按beacon逻辑处理 */
    hmac_sta_up_update_edca_params(ies_info, link_vap, WLAN_BEACON, link_user);

#ifdef _PRE_WLAN_FEATURE_11AX
    /* 按beacon逻辑处理 */
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_sta_up_update_he_edca_params(ies_info, link_vap, WLAN_BEACON, link_user);
    }
#endif
}
/*
 * 函 数 名   : hmac_parse_probe_rsp_basic_ml_sta_profile
 * 功能描述   : 解析per-STA profile字段携带的link IE信息
 */
OAL_STATIC uint32_t hmac_parse_probe_rsp_basic_ml_sta_profile(hmac_vap_stru *hmac_vap, uint8_t *sta_profile,
    uint16_t sta_profile_len, ml_link_info_stru *ml_link_info)
{
    uint8_t *frame_body = NULL;
    mac_device_stru *mac_device = NULL;
    uint8_t complete_profile = ml_link_info->complete_profile;
    uint16_t frame_len = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + sta_profile_len;
    mac_bss_dscr_stru tmp_bss_dscr = {0};
    mac_ies_info ies_info = {0};
    uint8_t channel = 0xff;

    ies_info.frame_ies = sta_profile + MAC_CAP_INFO_LEN;
    ies_info.frame_len = sta_profile_len - MAC_CAP_INFO_LEN;
    /* 解析per-STA profile中HT operation IE或 DSSS Param set ie指示的信道 */
    channel = mac_ie_get_chan_num(ies_info.frame_ies, ies_info.frame_len, 0, channel, 0);
    if (channel == 0xff) {
        oam_warning_log0(0, OAM_SF_SCAN, "{hmac_parse_probe_rsp_basic_ml_sta_profile::get frame channel fail.}");
        return OAL_SUCC;
    }

    ml_link_info->st_channel.uc_chan_number = channel;
    ml_link_info->st_channel.en_band = mac_get_band_by_channel_num(channel);

    /* 解析 STA Profile 中的协议相关的信息元素 */
    if ((frame_body = hmac_scan_alloc_frame_body(frame_len, sta_profile, sta_profile_len)) == NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_parse_probe_rsp_basic_ml_sta_profile::alloc frame fail.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    tmp_bss_dscr.st_channel.uc_chan_number = channel;
    tmp_bss_dscr.st_channel.en_band = ml_link_info->st_channel.en_band;
    mac_device = mac_res_get_dev(hmac_vap->st_vap_base_info.uc_chip_id);
    if (oal_unlikely(mac_device != NULL)) {
        /* 解析per-STA profile携带的速率信息 */
        hmac_scan_update_bss_list_rates(&tmp_bss_dscr, frame_body, frame_len, mac_device);
        ml_link_info->uc_num_supp_rates = tmp_bss_dscr.uc_num_supp_rates;
        if (memcpy_s(ml_link_info->auc_supp_rates, WLAN_MAX_SUPP_RATES,
            tmp_bss_dscr.auc_supp_rates, tmp_bss_dscr.uc_num_supp_rates) != EOK) {
            oal_free(frame_body);
            return OAL_FAIL;
        }
    }

    tmp_bss_dscr.en_channel_bandwidth = WLAN_BAND_WIDTH_20M;
    /* 解析per-STA profile携带的协议IE信息 */
    hmac_scan_update_bss_list_protocol(hmac_vap, &tmp_bss_dscr, frame_body, frame_len);
    /* per-STA profile不携带EHT IE, */
    if (tmp_bss_dscr.eht_capable != OAL_TRUE) {
        if (complete_profile == 1) {
            /* 使用继承特性 */
        } else {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_probe_rsp_basic_ml_sta_profile::don't support EHT.}");
            oal_free(frame_body);
            return OAL_FAIL;
        }
    }

    ml_link_info->st_channel.en_bandwidth = tmp_bss_dscr.en_channel_bandwidth;
    oal_free(frame_body);

    /* vap up状态处理sta-profile */
    hmac_sta_up_parse_probe_rsp_basic_ml_sta_profile(hmac_vap, &ies_info, ml_link_info);

    return OAL_SUCC;
}

/*
 * 函 数 名   : hmac_sta_parse_basic_ml_sta_profile
 * 功能描述   : 解析sta profile字段
 */
uint32_t hmac_sta_parse_basic_ml_sta_profile(hmac_vap_stru *hmac_vap, uint8_t *sta_profile,
    uint16_t sta_profile_len, ml_link_info_stru *ml_link_info, uint8_t frame_type)
{
    /* beacon/probe rsp解析 和 assoc rsp解析公共接口，判断帧类型走不同处理分支 */
    if (frame_type == WLAN_ASSOC_RSP || frame_type == WLAN_REASSOC_RSP) {
        return hmac_parse_assoc_rsp_basic_ml_sta_profile(hmac_vap, sta_profile, sta_profile_len, ml_link_info);
    } else {
        return hmac_parse_probe_rsp_basic_ml_sta_profile(hmac_vap, sta_profile, sta_profile_len, ml_link_info);
    }
}
/*
 * 函 数 名   : hmac_sta_parse_link_info_process
 * 功能描述   : 去分片后，解析sta info和sta profile字段
 */
uint32_t hmac_sta_parse_link_info_process(hmac_vap_stru *hmac_vap, uint8_t *sub_ie, uint16_t link_info_len,
    uint16_t offset, uint16_t sub_ie_len, ml_info_stru *ap_mld_info, uint8_t link_id, uint8_t frame_type)
{
    uint8_t  *sta_profile = NULL;
    uint16_t sta_info_len;
    uint16_t sta_profile_len;
    uint32_t ret = OAL_SUCC;

    sta_info_len = mac_count_basic_ml_sta_info_length(&sub_ie[ML_STA_CONTROL_OFFSET]);
    if (link_info_len < (offset + ML_STA_INFO_OFFSET + sta_info_len) ||
        sub_ie_len < (STA_CONTROL_FIELD_LEN + sta_info_len)) {
        return OAL_FAIL;
    }

    if ((ret = mac_sta_parse_basic_ml_sta_info(&sub_ie[ML_STA_INFO_OFFSET], &sub_ie[ML_STA_CONTROL_OFFSET],
        &ap_mld_info->ml_link_info[link_id])) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 如果子IE的总长度小于 sta control字段+sta info字段长度，sta profile长度会小于0，返回失败 */
    if (STA_CONTROL_FIELD_LEN + sta_info_len > sub_ie_len) {
        oam_warning_log2(0, 0, "{hmac_sta_parse_link_info_process::ie length error, sub_ie_len[%d], sta_info_len[%d].}",
            sub_ie_len, sta_info_len);
        return OAL_FAIL;
    }

    sta_profile_len = sub_ie_len - STA_CONTROL_FIELD_LEN - sta_info_len;
    /* STA profile字段协议规定是可变字段可能不会携带，不携带则不进行下面解析流程 */
    if (sta_profile_len == 0) {
        return OAL_SUCC;
    }
    sta_profile = &sub_ie[ML_STA_INFO_OFFSET + sta_info_len];
    if ((ret = hmac_sta_parse_basic_ml_sta_profile(hmac_vap, sta_profile, sta_profile_len,
        &ap_mld_info->ml_link_info[link_id], frame_type)) != OAL_SUCC) {
        return OAL_FAIL;
    }

    return ret;
}
/*
 * 函 数 名   : hmac_sta_parse_basic_ml_link_info
 * 功能描述   : 解析link info字段
 */
OAL_STATIC uint32_t hmac_sta_parse_basic_ml_link_info(hmac_vap_stru *hmac_vap, uint8_t *link_info,
    uint16_t link_info_len, ml_info_stru *ap_mld_info, uint8_t frame_type)
{
    uint8_t  *sub_ie = NULL;
    uint8_t  *defrag_sub_ie = NULL;
    uint16_t offset = 0;
    uint16_t sub_ie_len;
    uint16_t frag_ie_len;
    uint16_t defrag_sub_ie_len = link_info_len;
    uint8_t  link_id = 1;
    uint32_t ret = OAL_SUCC;

    /*********************************************************************************
     | sub ID | len | data | frag ID | len | data | ..... | frag ID | len  | data |
     |    0   | 255 |      |   254   | 255 |      |       |   254   | <255 |      |
    *********************************************************************************/
    while ((link_info_len > offset) && (link_id < WLAN_AP_MAX_ML_LINK_INFO_NUM)) {
        sub_ie = link_info + offset;
        if (sub_ie[0] != ML_SUBEID_PER_STA_PROFILE) {
            break;
        }

        sub_ie_len = sub_ie[1];
        frag_ie_len = sub_ie_len;
        /* 判断是否需要去分片 */
        if (mac_check_sub_ie_need_defragment(sub_ie, link_info_len)) {
            defrag_sub_ie = (uint8_t *)oal_memalloc(defrag_sub_ie_len); /* 申请内存保存去分片后的帧体信息 */
            ret = mac_per_sta_profile_subelement_defragment(&sub_ie, &sub_ie_len, &frag_ie_len,
                                                            defrag_sub_ie, defrag_sub_ie_len);
            if (ret != OAL_SUCC) {
                mac_free_sub_ie_defrag_buffer(defrag_sub_ie);
                return ret;
            }
            /* 去分片成功，将解析地址替换为去分片后的内存地址 */
            sub_ie = defrag_sub_ie;
        }

        /* 解析link info中的sta info和per-STA profile信息 */
        if (hmac_sta_parse_link_info_process(hmac_vap, sub_ie, link_info_len, offset,
            sub_ie_len, ap_mld_info, link_id, frame_type) != OAL_SUCC) {
            mac_free_sub_ie_defrag_buffer(defrag_sub_ie);
            break;
        }

        /* 帧体指针偏移 */
        offset += frag_ie_len + MAC_IE_HDR_LEN;

        mac_free_sub_ie_defrag_buffer(defrag_sub_ie);

        link_id++;
        ap_mld_info->ml_link_num++;
    }
    if ((link_info_len != offset) && (link_id < WLAN_AP_MAX_ML_LINK_INFO_NUM)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_parse_basic_ml_link_info::invalid link info length.}");
        ret = OAL_FAIL;
    }

    return ret;
}

/*
 * 函 数 名   : mac_count_basic_ml_common_info_length
 * 功能描述   : 计算 Basic Multi-Link IE 中 Common Info field 的长度
 */
uint8_t mac_count_basic_ml_common_info_length(mac_eht_ml_ie_header_stru *ml_ie_stru)
{
    mac_ml_control_union *ml_control = (mac_ml_control_union *)&ml_ie_stru->ml_control;
    uint8_t common_info_len = WLAN_MAC_ADDR_LEN + 1; /* 1字节 common info字段长度 + 6字节 mld mac地址长度 */

    if (ml_control->basic_ml_control.ml_link_id_info_present == 1) {
        common_info_len += ML_LINK_ID_INFO_FIELD_LEN;
    }
    if (ml_control->basic_ml_control.ml_bss_parameters_change_count_present == 1) {
        common_info_len += BSS_PARAMETERS_CHANGE_COUNT_LEN;
    }
    if (ml_control->basic_ml_control.ml_medium_synchronization_delay_information_present == 1) {
        common_info_len += MEDIUM_SYNCHRONIZATION_DELAY_INFORMATION_LEN;
    }
    if (ml_control->basic_ml_control.ml_eml_capabilities_present == 1) {
        common_info_len += EML_CAPBILITIES_FIELD_LEN;
    }
    if (ml_control->basic_ml_control.ml_mld_capabilities_and_operations_present == 1) {
        common_info_len += MLD_CAPBILITIES_FIELD_LEN;
    }
    if (ml_control->basic_ml_control.ml_mld_id_present == 1) {
        common_info_len += MLD_ID_FIELD_LEN;
    }

    return common_info_len;
}

uint32_t hmac_sta_parse_basic_ml_common_info(uint8_t *common_info, mac_eht_ml_ie_header_stru *ml_ie_stru,
    ml_info_stru *ap_mld_info, uint8_t common_info_len)
{
    uint8_t offset = 0;

    /* 1、common info字段声明的长度 与 根据ML control解析的长度相同
       2、ML IE的长度 不小于 ML EID 1字节 + ML Control 2字节 + common info字段声明长度 */
    if ((common_info[0] != common_info_len) ||
        (ml_ie_stru->ie_len < (1 + ML_CONTROL_FIELD_LEN + common_info[0]))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sta_parse_basic_ml_common_info::invalid common info length.}");
        return OAL_FAIL;
    }

    offset += 1; /* common info字段长度占 1字节 */
    if (memcpy_s(ap_mld_info->ml_mac_addr, WLAN_MAC_ADDR_LEN, &common_info[offset], WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sta_parse_basic_ml_common_info::memcpy_s fail.}");
        return OAL_FAIL;
    }
    offset += WLAN_MAC_ADDR_LEN;

    if (ml_ie_stru->ml_control.basic_ml_control.ml_link_id_info_present == 1) {
        ap_mld_info->link_id = common_info[offset];
        offset += ML_LINK_ID_INFO_FIELD_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_bss_parameters_change_count_present == 1) {
        ap_mld_info->bpcc = common_info[offset];
        offset += BSS_PARAMETERS_CHANGE_COUNT_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_medium_synchronization_delay_information_present == 1) {
        if (memcpy_s(&ap_mld_info->medium_sync_cap.medium_sync, MEDIUM_SYNCHRONIZATION_DELAY_INFORMATION_LEN,
            &common_info[offset], MEDIUM_SYNCHRONIZATION_DELAY_INFORMATION_LEN) != EOK) {
            return OAL_FAIL;
        }
        ap_mld_info->medium_sync_cap.medium_capable = OAL_TRUE;
        offset += MEDIUM_SYNCHRONIZATION_DELAY_INFORMATION_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_eml_capabilities_present == 1) {
        if (memcpy_s(&ap_mld_info->eml_cap, EML_CAPBILITIES_FIELD_LEN,
            &common_info[offset], EML_CAPBILITIES_FIELD_LEN) != EOK) {
            return OAL_FAIL;
        }
        offset += EML_CAPBILITIES_FIELD_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_mld_capabilities_and_operations_present == 1) {
        if (memcpy_s(&ap_mld_info->ml_capabilities, MLD_CAPBILITIES_FIELD_LEN,
            &common_info[offset], MLD_CAPBILITIES_FIELD_LEN) != EOK) {
            return OAL_FAIL;
        }
        offset += MLD_CAPBILITIES_FIELD_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_mld_id_present == 1) {
        ap_mld_info->mld_id = common_info[offset];
        offset += MLD_ID_FIELD_LEN;
    }

    return OAL_SUCC;
}

uint32_t hmac_sta_parse_basic_ml_common_info_process(uint8_t  *ml_ie, uint16_t ie_len,
    ml_info_stru *ap_mld_info, uint8_t *ret_common_info_len)
{
    mac_eht_ml_ie_header_stru *ml_ie_stru = (mac_eht_ml_ie_header_stru *)ml_ie;
    uint8_t common_info_len = 0;
    if (ml_ie_stru->ml_control.basic_ml_control.ml_control_type != BASIC_MULTI_LINK) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_sta_parse_basic_ml_common_info_process::invalid ml_control_type [%d].}",
            ml_ie_stru->ml_control.basic_ml_control.ml_control_type);
        return OAL_FAIL;
    }
    /* 解析ML control字段，计算common info长度 */
    common_info_len = mac_count_basic_ml_common_info_length(ml_ie_stru);
    if (MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len > ie_len) {
        return OAL_FAIL;
    }
    ml_ie += (MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN);

    /* 解析 Common Info */
    if (hmac_sta_parse_basic_ml_common_info(ml_ie, ml_ie_stru, ap_mld_info, common_info_len) != OAL_SUCC) {
        return OAL_FAIL;
    }
    *ret_common_info_len = common_info_len;
    return OAL_SUCC;
}

/*
 * 函 数 名   : mac_check_ml_ie_need_defragment
 * 功能描述   : 检查Multi-Link element是否分片(包含per-STA profile时)
 */
uint8_t mac_check_ml_ie_need_defragment(uint8_t *ml_ie, uint16_t max_len)
{
    uint16_t next_frag_offset = MAC_IE_EXT_HDR_LEN + EXT_IE_FRAGMENT_THRESHOLD;

    if ((ml_ie[1] == IE_FRAGMENT_THRESHOLD) && (next_frag_offset < max_len) &&
        (ml_ie[next_frag_offset] == MAC_EID_FRAGMENT)) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}
uint32_t hmac_sta_parse_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *ie, uint16_t ie_len,
    ml_info_stru *ap_mld_info, uint8_t frame_type)
{
    uint8_t  *ml_ie = NULL;
    uint8_t common_info_len = 0;
    uint16_t link_info_len;
    uint16_t ml_ie_len;
    uint32_t ret = OAL_SUCC;
    uint8_t *defrag_ie = NULL;
    uint16_t defrag_ie_len  = MAX_ML_IE_LEN;
    mac_ies_info ies_info = {0};

    ies_info.frame_ies = ie;
    ies_info.frame_len = ie_len;

    ml_ie = mac_find_ml_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, BASIC_MULTI_LINK, &ies_info);
    if (ml_ie == NULL || ml_ie[1] <= MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN) {
        return OAL_SUCC;
    }

    ap_mld_info->ml_ie = ml_ie; /* 记录帧体中ML IE的位置 */
    ml_ie_len = ml_ie[1];
    if (mac_check_ml_ie_need_defragment(ml_ie, ie_len)) {
        /* ML IE需要去分片，申请内存 */
        defrag_ie = (uint8_t *)oal_memalloc(MAX_ML_IE_LEN);
        /* 去分片流程，将实际数据拷贝到申请的内存中 */
        ret = mac_basic_ml_element_defragment(&ml_ie, &ml_ie_len, defrag_ie, defrag_ie_len);
        if (ret != OAL_SUCC) {
            mac_free_ie_defrag_buffer(defrag_ie);
            return ret;
        }
        /* 去分片解析数据成功，后续ML IE解析在去分片结果上进行 */
        ml_ie = defrag_ie;
    }
    if (hmac_sta_parse_basic_ml_common_info_process(ml_ie, ie_len, ap_mld_info, &common_info_len) != OAL_SUCC) {
        mac_free_ie_defrag_buffer(defrag_ie);
        return OAL_FAIL;
    }

    ap_mld_info->ml_link_num = 1; /* 上述ML IE解析成功，至少存在一个link */
    ml_ie += MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len;

    /* 解析 Link Info */
    if (ml_ie_len > (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len)) {
        link_info_len = ml_ie_len - (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len);
        ret = hmac_sta_parse_basic_ml_link_info(hmac_vap, ml_ie, link_info_len, ap_mld_info, frame_type);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_parse_basic_ml_ie::parse basic ml link info fail.}");
        }
    }
    mac_free_ie_defrag_buffer(defrag_ie);

    return ret;
}
uint8_t hmac_add_link_info_find_idx(ml_link_info_stru *ml_link_info, uint8_t *cur_link_addr,
    mac_bss_dscr_stru *bss_dscr)
{
    uint8_t link_info_idx;
    /* 当前link info数据idx 0的位置默认添加当前扫到的bss，link vap bss从1开始往后添加 */
    /* 当前link info已添加过，则返回当前idx直接覆盖即可；否则往后添加 */
    for (link_info_idx = 1; link_info_idx < WLAN_AP_MAX_ML_LINK_INFO_NUM; link_info_idx++) {
        if (oal_memcmp(ml_link_info[link_info_idx].sta_mac_addr, cur_link_addr, OAL_MAC_ADDR_LEN) == 0) {
            return link_info_idx;
        } else if (ml_link_info[link_info_idx].is_add_link_list == OAL_FALSE) {
            /* 没有添加过link才需要num增加 */
            bss_dscr->ml_info.ml_link_num++;
            return link_info_idx;
        }
    }
    oam_error_log2(0, OAM_SF_ASSOC, "{hmac_add_link_info_find_idx::link info too much, cur addr[%02x:%02x].}",
        cur_link_addr[BYTE_OFFSET_4], cur_link_addr[BYTE_OFFSET_5]);
    return 1;
}
OAL_STATIC uint32_t hmac_scan_parse_rnr_ie(uint8_t *neigh_rpt_ie, uint8_t ie_len, mac_bss_dscr_stru *bss_dscr)
{
    mac_mld_paramters_stru           *mld_params    = NULL;
    mac_tbtt_infomation_header_stru  *tbtt_info_hdr = NULL;
    mac_bss_parameters_subfield_stru *bss_params    = NULL;
    uint8_t *neigh_ap_info = neigh_rpt_ie + MAC_IE_HDR_LEN;
    uint16_t offset = 0;
    uint8_t idx = 1;
    uint8_t channel_num;

    while ((neigh_rpt_ie[1] > offset) && (idx < WLAN_AP_MAX_ML_LINK_INFO_NUM) &&
           (ie_len > offset + sizeof(mac_tbtt_infomation_header_stru))) {
        tbtt_info_hdr = (mac_tbtt_infomation_header_stru *)&neigh_ap_info[offset];

        /* 判断条件：
           1、tbtt info类型正确
           2、tbtt info长度为包含mld参数的长度
           3、剩余帧体长度不小于 含有mld参数的tbtt info长度
        */
        if (tbtt_info_hdr->tbtt_infomation_field_type == TBTT_INFO_TYPE0 &&
            tbtt_info_hdr->tbtt_infomation_length >= TBTT_INFO_INCLUDE_MLD_LEN_TYPE0 &&
            ie_len >= offset + MIN_REDUCED_NEIGHBOR_REPORT_IE_LEN) {
            bss_params = (mac_bss_parameters_subfield_stru *)&neigh_ap_info[offset + NEIGHBOR_AP_BSS_PARAM_OFFSET];
            mld_params = (mac_mld_paramters_stru *)&neigh_ap_info[offset + NEIGHBOR_AP_MLD_PARAMETERS_OFFSET];

            /* 能力位不符合MLD要求，不解析当前neighbor AP info字段 */
            if (bss_params->same_ssid != 1 || mld_params->mld_id == 0xff) {
                offset += NEIGHBOR_AP_TBTT_INFO_SET_OFFSET + tbtt_info_hdr->tbtt_infomation_length;
                continue;
            }
            idx = hmac_add_link_info_find_idx(bss_dscr->ml_info.ml_link_info,
                &neigh_ap_info[offset + NEIGHBOR_AP_BSSID_OFFSET], bss_dscr);
            if (memcpy_s(bss_dscr->ml_info.ml_link_info[idx].sta_mac_addr, WLAN_MAC_ADDR_LEN,
                &neigh_ap_info[offset + NEIGHBOR_AP_BSSID_OFFSET], WLAN_MAC_ADDR_LEN) != EOK) {
                break;
            }

            /* 解析neighbor AP info字段中指示的其他link的channel number */
            channel_num = neigh_ap_info[offset + NEIGHBOR_AP_CHANNEL_NUM_OFFSET];
            bss_dscr->ml_info.ml_link_info[idx].st_channel.uc_chan_number = channel_num;
            bss_dscr->ml_info.ml_link_info[idx].st_channel.en_band = mac_get_band_by_channel_num(channel_num);
            bss_dscr->ml_info.ml_link_info[idx].link_id = mld_params->link_id;
            bss_dscr->ml_info.ml_link_info[idx].is_add_link_list = OAL_TRUE;
            bss_dscr->ml_info.mld_id = mld_params->mld_id;
        }
        offset += NEIGHBOR_AP_TBTT_INFO_SET_OFFSET + tbtt_info_hdr->tbtt_infomation_length;
    }

    if (neigh_rpt_ie[1] != offset) {
        if (idx < WLAN_MAX_ML_LINK_NUM) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_scan_parse_reduced_neighbor_report_ie::invalid length.}");
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_scan_parse_rnr_ie_process(mac_bss_dscr_stru *bss_dscr,
    uint8_t *frame_body, uint16_t frame_body_len)
{
    uint8_t *neigh_rpt_ie = NULL;
    uint8_t ie_len;
    mac_ies_info ies_info = {0};

    ies_info.frame_ies = frame_body;
    ies_info.frame_len = frame_body_len;

    /* 解析 Reduced Neighbor Report element */
    neigh_rpt_ie = mac_find_ie(MAC_EID_REDUCED_NEIGHBOR_REPORT, &ies_info);
    if (neigh_rpt_ie == NULL) {
        return OAL_SUCC;
    }

    ie_len = neigh_rpt_ie[1];
    if (ie_len < MIN_REDUCED_NEIGHBOR_REPORT_IE_LEN) {
        oam_warning_log1(0, OAM_SF_SCAN, "{hmac_scan_parse_rnr_ie_process::invalid ie length [%u].",
            neigh_rpt_ie[1]);
        return OAL_FAIL;
    }

    /* 解析rnr ie中的multilink相关信息 */
    if (hmac_scan_parse_rnr_ie(neigh_rpt_ie, ie_len, bss_dscr) != OAL_SUCC) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t hmac_scan_update_multi_link_cap(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr,
    uint8_t *frame_body, uint16_t frame_len)
{
    uint32_t ret = OAL_FAIL;
    mac_ies_info ies_info = {0};

    ies_info.frame_ies = frame_body;
    ies_info.frame_len = frame_len;

    if (oal_any_null_ptr3(hmac_vap, bss_dscr, frame_body)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_multi_link_cap:: argument is NULL.");
        return OAL_FAIL;
    }

    /* 不支持EHT 或 找不到ML IE，不解析ML IE能力 */
    if (bss_dscr->eht_capable == OAL_FALSE ||
        mac_find_ml_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, BASIC_MULTI_LINK, &ies_info) == NULL) {
        return OAL_SUCC;
    }
    /* 解析 Multi-Link element */
    ret = hmac_sta_parse_scan_result_basic_ml_ie(hmac_vap, frame_body, frame_len,
                                                 &bss_dscr->ml_info, WLAN_MGMT_SUBTYPE_BUTT);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_multi_link_cap:: parse ml ie fail.");
        return ret;
    }

    /* 解析Reduced Neighbor Report element */
    ret = hmac_scan_parse_rnr_ie_process(bss_dscr, frame_body, frame_len);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_multi_link_cap:: parse rnr ie fail.");
        return ret;
    }

    /* 扫描结果更新 */
    ret = hmac_scan_update_bss_multi_link_info(bss_dscr);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_scan_update_multi_link_cap:: update bss_dscr fail.");
        return ret;
    }

    return ret;
}

ml_link_info_stru *hmac_get_ap_link_info(hmac_vap_stru *hmac_vap, ml_info_stru *ap_mld_info)
{
    ml_link_info_stru *ap_link_info = &hmac_vap->mld_vap_link_info.link_ap_info;
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    hmac_user_stru *mld_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    uint8_t link_id;

    if (mld_vap_manage == NULL || mld_user == NULL) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_get_ap_link_info::mld_vap_manage or mld_user is NULL.");
        return NULL;
    }

    for (link_id = 1; link_id <= ap_mld_info->ml_link_num && link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        if (ap_mld_info->ml_link_info[link_id].link_id == ap_link_info->link_id &&
            memcmp(ap_mld_info->ml_link_info[link_id].sta_mac_addr, ap_link_info->sta_mac_addr,
            WLAN_MAC_ADDR_LEN) == 0) {
            ap_link_info->asoc_succ = ap_mld_info->ml_link_info[link_id].asoc_succ;
            ap_link_info->inheritance_ies = ap_mld_info->ml_link_info[link_id].inheritance_ies;
            ap_link_info->inheritance_ie_len = ap_mld_info->ml_link_info[link_id].inheritance_ie_len;
            ap_link_info->complete_profile = ap_mld_info->ml_link_info[link_id].complete_profile;
            memcpy_s(mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN, ap_mld_info->ml_mac_addr, WLAN_MAC_ADDR_LEN);
            mld_user->mld_user_link_info.bpcc = ap_mld_info->ml_link_info[link_id].bpcc;

            return &ap_mld_info->ml_link_info[link_id];
        }
    }
    return NULL;
}

OAL_STATIC uint32_t hmac_mld_link_process_assoc_rsp(hmac_vap_stru *hmac_sta, uint8_t *mac_hdr,
    uint8_t *payload, uint16_t msg_len)
{
    uint16_t                          payload_len;
    mac_vap_stru                     *mac_vap = &(hmac_sta->st_vap_base_info);
    hmac_user_stru                   *hmac_user = NULL;
    mac_ies_info                      ies_info = {0};
    ml_link_info_stru                *link_ap_info = &hmac_sta->mld_vap_link_info.link_ap_info;
    uint8_t                          *payload_tmp = payload;
    uint32_t ret = OAL_SUCC;
    /* 帧体长度检查 */
    if (msg_len < MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN) {
        oam_error_log2(0, OAM_SF_ASSOC, "{hmac_mld_link_process_assoc_rsp:: us_msg_len %d < %d.}",
                       msg_len, MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN);
        return OAL_FAIL;
    }
    hmac_user = mac_vap_get_hmac_user_by_addr(mac_vap, link_ap_info->sta_mac_addr, OAL_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        return OAL_FAIL;
    }

    hmac_process_assoc_rsp_set_aid(mac_vap, payload_tmp);

    payload_len = msg_len;
    payload_tmp += (MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN);
    msg_len -= (MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN);

    ies_info.frame_ies = payload_tmp;
    ies_info.frame_len = msg_len;
    ies_info.inheritance_ies = link_ap_info->inheritance_ies;
    ies_info.inheritance_ie_len = link_ap_info->inheritance_ie_len;

    hmac_process_assoc_rsp_set_feature(hmac_sta, hmac_user, mac_hdr, &ies_info);

    hmac_process_assoc_rsp_parse_ie(hmac_sta, hmac_user, mac_hdr,  &ies_info);
    if (hmac_sta->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING) {
        hmac_sta_wait_asoc_rx_rpt_user_chan_check(&hmac_sta->st_vap_base_info, &ret, hmac_sta,
            hmac_user->st_user_base_info.us_assoc_id);
    }
    return OAL_SUCC;
}

OAL_STATIC void hmac_free_ap_mld_info(ml_info_stru *ap_mld_info)
{
    ml_link_info_stru *link_info = NULL;
    uint8_t link_id;

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_info = &ap_mld_info->ml_link_info[link_id];
        if (link_info->inheritance_ies != NULL) {
            oal_free(link_info->inheritance_ies);
            link_info->inheritance_ies = NULL;
        }
    }
}

uint32_t hmac_parse_assoc_rsp_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *payload, uint16_t msg_len,
    ml_info_stru *ap_mld_info)
{
    uint8_t *ie = NULL;
    uint16_t offset = MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN;
    uint16_t ie_len;

    if (hmac_vap == NULL || payload == NULL || ap_mld_info == NULL || msg_len < offset) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_parse_assoc_rsp_basic_ml_ie::invalid argument.}");
        return OAL_FAIL;
    }
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_parse_assoc_rsp_basic_ml_ie::hmac vap is not mld link vap.}");
        return OAL_SUCC;
    }

    ie = payload + offset;
    ie_len = msg_len - offset;

    return hmac_sta_parse_basic_ml_ie(hmac_vap, ie, ie_len, ap_mld_info, WLAN_ASSOC_RSP);
}

OAL_STATIC void hmac_mld_del_asoc_fail_link_user(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru     *link_hmac_vap = NULL;
    ml_link_info_stru *ap_link_info = NULL;
    hmac_user_stru    *hmac_user = NULL;
    mac_vap_stru      *mac_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint8_t link_id;

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_mld_process_asoc_rsp::get mld hmac vap fail.");
        return;
    }

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL || hmac_vap_is_ml_mode(link_hmac_vap) == OAL_TRUE) {
            continue;
        }

        ap_link_info = &link_hmac_vap->mld_vap_link_info.link_ap_info;
        if (ap_link_info->asoc_succ == 1) {
            continue;
        }

        mac_vap = &link_hmac_vap->st_vap_base_info;
        hmac_user = mac_vap_get_hmac_user_by_addr(mac_vap, ap_link_info->sta_mac_addr, OAL_MAC_ADDR_LEN);
        if (hmac_user == NULL) {
            continue;
        }
        if (hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING &&
            hmac_is_pmf_enable_temp_reject(hmac_vap) == OAL_TRUE) {
            oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_mld_del_asoc_fail_link_user::roam temp reject no need del user.");
            continue;
        }
        hmac_user_del(mac_vap, hmac_user);
        memset_s(ap_link_info, sizeof(ml_link_info_stru), 0, sizeof(ml_link_info_stru));
    }
}

uint32_t hmac_mld_update_mld_user_info(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, ml_info_stru *ap_ml_info)
{
    uint32_t ret;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;

    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (mld_user_manage == NULL) {
        return OAL_FAIL;
    }

    /* 删除关联失败的user */
    hmac_mld_del_asoc_fail_link_user(hmac_vap);

    /* 更新eml cap参数到mld user manage下 */
    memcpy_s(&mld_user_manage->eml_cap, sizeof(mac_eml_capabilities_stru),
        &ap_ml_info->eml_cap, sizeof(mac_eml_capabilities_stru));

    /* 更新mld param参数到mld user manage下 */
    memcpy_s(&mld_user_manage->mld_cap, sizeof(mac_mld_capabilities_stru),
        &ap_ml_info->ml_capabilities, sizeof(mac_mld_capabilities_stru));

    /* 更新medium sync参数到mld user manage下 */
    memcpy_s(&mld_user_manage->medium_cap, sizeof(mac_medium_sync_info_stru),
        &ap_ml_info->medium_sync_cap, sizeof(mac_medium_sync_info_stru));

    /* 初始化bpcc */
    hmac_user->mld_user_link_info.bpcc = ap_ml_info->bpcc;
    ret =  hmac_config_user_mld_cap_syn(&(hmac_vap->st_vap_base_info), &(hmac_user->st_user_base_info));
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_mld_update_mld_user_info::update mld user info fail.}");
        return ret;
    }
    return OAL_SUCC;
}

uint32_t hmac_mld_process_asoc_rsp(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *mac_hdr, uint8_t *payload, uint16_t msg_len)
{
    hmac_vap_stru        *link_hmac_vap = NULL;
    ml_link_info_stru    *ap_link_info = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    ml_info_stru ap_mld_info = {0};
    uint8_t link_id;
    uint32_t ret = OAL_SUCC;

    if (oal_any_null_ptr3(hmac_vap, payload, mac_hdr)) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_mld_process_asoc_rsp_check::argument is null.}");
        return OAL_FAIL;
    }

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        return OAL_SUCC;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_mld_process_asoc_rsp::get mld hmac vap fail.");
        return OAL_FAIL;
    }

    memcpy_s(ap_mld_info.ml_mac_addr, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN);
    // reconfig rsp复用此流程
    if ((mac_get_frame_sub_type(mac_hdr) == WLAN_FC0_SUBTYPE_ACTION) && (payload[MAC_ACTION_OFFSET_ACTION] ==
        MAC_PROTECTED_EHT_ACTION_LINK_RECONFIGURATION_RESPONSE)) {
        ret = hmac_parse_reconfig_rsp_basic_ml_ie(hmac_vap, payload, msg_len, &ap_mld_info);
    } else {
        ret = hmac_parse_assoc_rsp_basic_ml_ie(hmac_vap, payload, msg_len, &ap_mld_info);
    }
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_mld_process_asoc_rsp::parse multi-link ie fail.}");
        hmac_free_ap_mld_info(&ap_mld_info);
        return OAL_FAIL;
    }

    // 设置工作link关联成功标志
    hmac_vap->mld_vap_link_info.link_ap_info.asoc_succ = 1;

    // 更新 mld link user info
    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL || hmac_vap_is_ml_mode(link_hmac_vap) == OAL_TRUE) {
            continue;
        }

        ap_link_info = hmac_get_ap_link_info(link_hmac_vap, &ap_mld_info);
        if (ap_link_info == NULL || ap_link_info->asoc_succ == 0) {
            oam_warning_log1(0, OAM_SF_ASSOC, "{hmac_mld_process_asoc_rsp::ap link info%d.}", ap_link_info == NULL);
            continue;
        }
        if (hmac_mld_link_process_assoc_rsp(link_hmac_vap, mac_hdr, payload, msg_len) != OAL_SUCC) {
            ap_link_info->asoc_succ = 0;
        }
    }
    ret = hmac_mld_update_mld_user_info(hmac_vap, hmac_user, &ap_mld_info);
    hmac_free_ap_mld_info(&ap_mld_info);
    return ret;
}

OAL_STATIC uint32_t hmac_ap_parse_basic_ml_link_info(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *link_info, uint16_t link_info_len)
{
    /* TODO: ap解析assoc req帧中ml ie的link info，有的话另一个link需要add user，暂时只支持单link */
    return OAL_SUCC;
}

/* ap解析sta的ml ie中的common info，目前仅使用eml cap和mld cap */
OAL_STATIC uint32_t hmac_ap_parse_basic_ml_common_info(hmac_user_stru *hmac_user,
    uint8_t *common_info, mac_eht_ml_ie_header_stru *ml_ie_stru, uint8_t common_info_len)
{
    uint8_t offset = 0;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;

    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (mld_user_manage == NULL) {
        return OAL_FAIL;
    }

    /* 1、common info字段声明的长度 与 根据ML control解析的长度相同
       2、ML IE的长度 不小于 ML EID 1字节 + ML Control 2字节 + common info字段声明长度 */
    if ((common_info[0] != common_info_len) ||
        (ml_ie_stru->ie_len < (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info[0]))) {
        oam_error_log3(0, OAM_SF_ANY,
            "{hmac_ap_parse_basic_ml_common_info::invalid common info length. \
            common_info %d, common_info_len %d, ml_ie_len %d}",
            common_info[0], common_info_len, ml_ie_stru->ie_len);
        return OAL_FAIL;
    }

    offset += 1; /* common info字段长度占 1字节 */
    if (memcpy_s(mld_user_manage->mld_addr, WLAN_MAC_ADDR_LEN, &common_info[offset], WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ap_parse_basic_ml_common_info::copy mld_addr fail.}");
        return OAL_FAIL;
    }
    offset += WLAN_MAC_ADDR_LEN;

    if (ml_ie_stru->ml_control.basic_ml_control.ml_link_id_info_present == 1) {
        offset += ML_LINK_ID_INFO_FIELD_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_bss_parameters_change_count_present == 1) {
        offset += BSS_PARAMETERS_CHANGE_COUNT_LEN;
    }
    if (ml_ie_stru->ml_control.basic_ml_control.ml_medium_synchronization_delay_information_present == 1) {
        offset += MEDIUM_SYNCHRONIZATION_DELAY_INFORMATION_LEN;
    }

    // EML Cap 字段解析
    if (ml_ie_stru->ml_control.basic_ml_control.ml_eml_capabilities_present == 1) {
        if (memcpy_s(&mld_user_manage->eml_cap, EML_CAPBILITIES_FIELD_LEN,
            &common_info[offset], EML_CAPBILITIES_FIELD_LEN) != EOK) {
            return OAL_FAIL;
        }
        offset += EML_CAPBILITIES_FIELD_LEN;
    }

    // MLD Cap 字段解析
    if (ml_ie_stru->ml_control.basic_ml_control.ml_mld_capabilities_and_operations_present == 1) {
        if (memcpy_s(&mld_user_manage->mld_cap, MLD_CAPBILITIES_FIELD_LEN,
            &common_info[offset], MLD_CAPBILITIES_FIELD_LEN) != EOK) {
            return OAL_FAIL;
        }
        offset += MLD_CAPBILITIES_FIELD_LEN;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ap_parse_basic_ml_common_info_process(hmac_user_stru *hmac_user,
    mac_eht_ml_ie_header_stru *ml_ie_stru, uint8_t *ml_ie, uint16_t ie_len)
{
    uint8_t common_info_len = 0;

    /* 解析ML control字段，计算common info长度 */
    common_info_len = mac_count_basic_ml_common_info_length(ml_ie_stru);
    if (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len > ie_len) {
        oam_error_log2(0, OAM_SF_ASSOC,
            "{hmac_ap_parse_basic_ml_common_info_process::len invalid.ie_len %d, common_info_len %d}",
            ie_len, common_info_len);
        return 0;
    }
    ml_ie += (MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN); // 执行ML COMMON_INFO

    /* 解析 Common Info */
    if (hmac_ap_parse_basic_ml_common_info(hmac_user, ml_ie, ml_ie_stru, common_info_len) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ASSOC,
            "{hmac_ap_parse_basic_ml_common_info::failed.}");
        return 0;
    }

    return common_info_len;
}

static uint32_t hmac_ap_process_assoc_req_basic_ml_ie(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *ml_ie)
{
    mac_eht_ml_ie_header_stru *basic_ml_ie = (mac_eht_ml_ie_header_stru *)ml_ie;
    uint8_t common_info_len;
    uint16_t link_info_len;
    uint16_t ml_ie_len = ml_ie[1];
    uint32_t ret;
    /******************************************************************************/
    /*               Association Resquest Frame - Basic Multi-Link IE             */
    /* -------------------------------------------------------------------------- */
    /* | EID |IE LEN | EID Ext | ML Control |             Common Info           | */
    /* | --- |------ | ------- | ML Control | Len | MLD MAC | MLD Cap | EML Cap | */
    /* -------------------------------------------------------------------------- */
    /* |  1  |   1   |    1    |     2      |  1  |    6    |    2    |    2    | */
    /* -------------------------------------------------------------------------- */
    /* |         Per-STA Profile            |        Per-STA Profile            | */
    /* | EID | IE LEN | CAP |    IE         | EID | IE LEN | CAP |    IE        | */
    /* -------------------------------------------------------------------------- */
    /* |  1  |   2    |  2  |    可变       |   1 |   2    |  2  |    可变       | */
    /******************************************************************************/
    if (basic_ml_ie->ml_control.basic_ml_control.ml_control_type != BASIC_MULTI_LINK) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ap_parse_assoc_req_basic_ml_ie::invalid ml_control_type.}");
        return OAL_FAIL;
    }

    /* 解析 Common Info */
    common_info_len = hmac_ap_parse_basic_ml_common_info_process(hmac_user, basic_ml_ie, ml_ie, ml_ie_len);
    if (common_info_len == 0) {
        return OAL_FAIL;
    }

    /* 该link上user置为关联成功 */
    hmac_mld_user_save_link_assoc_status(hmac_user, hmac_vap->mld_vap_link_info.link_id, MAC_SUCCESSFUL_STATUSCODE);

    ml_ie += MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len; // 指向ML COMMON_INFO

    /* 解析 Link Info */
    if (ml_ie_len > (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len)) {
        link_info_len = ml_ie_len - (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len);
        ret = hmac_ap_parse_basic_ml_link_info(hmac_vap, hmac_user, ml_ie, link_info_len);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_parse_basic_ml_ie::parse basic ml link info fail.}");
        }
    }

    return OAL_SUCC;
}

uint32_t hmac_ap_parse_assoc_req_basic_ml_ie(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len)
{
    uint32_t ret;

    uint16_t ml_ie_len;
    uint8_t *ml_ie = NULL;
    uint8_t *defrag_ie = NULL;
    uint16_t defrag_ie_len  = MAX_ML_IE_LEN;
    mac_ies_info ies_info = {0};

    if (oal_any_null_ptr3(hmac_vap, hmac_user, payload)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ap_parse_assoc_req_basic_ml_ie::argument is null}");
        return OAL_FAIL;
    }

    /* 非ML 模式vap， 不用处理ml ie */
    if (hmac_vap_is_ml_mode(hmac_vap) != OAL_TRUE) {
        return OAL_SUCC;
    }

    ies_info.frame_ies = payload;
    ies_info.frame_len = payload_len;
    ml_ie = mac_find_ml_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, BASIC_MULTI_LINK, &ies_info);
    if (ml_ie == NULL) {
        return OAL_SUCC;
    }

    ml_ie_len = ml_ie[1];
    if (mac_check_ml_ie_need_defragment(ml_ie, payload_len)) {
        /* ML IE需要去分片，申请内存 */
        defrag_ie = (uint8_t *)oal_memalloc(MAX_ML_IE_LEN);
        /* 去分片流程，将实际数据拷贝到申请的内存中 */
        ret = mac_basic_ml_element_defragment(&ml_ie, &ml_ie_len, defrag_ie, defrag_ie_len);
        if (ret != OAL_SUCC) {
            oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                "{hmac_ap_parse_assoc_req_basic_ml_ie::defragment fail}");
            mac_free_ie_defrag_buffer(defrag_ie);
            return ret;
        }
        /* 去分片解析数据成功，后续ML IE解析在去分片结果上进行 */
        ml_ie = defrag_ie;
    }

    ret = hmac_ap_process_assoc_req_basic_ml_ie(hmac_vap, hmac_user, ml_ie);

    mac_free_ie_defrag_buffer(defrag_ie);

    return ret;
}
#endif
