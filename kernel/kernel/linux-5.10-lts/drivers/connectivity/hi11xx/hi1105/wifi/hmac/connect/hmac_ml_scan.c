/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : ml扫描模块 hmac功能
 * 创建日期 : 2024年05月25日
 */

/* 1 头文件包含 */
#include "mac_frame_inl.h"
#include "wlan_chip_i.h"
#include "mac_frame_ml.h"
#include "hmac_sme_sta.h"
#include "hmac_scan.h"
#include "hmac_scan_bss.h"
#include "dpe_wlan_vap.h"
#include "hmac_ml_scan.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ML_SCAN_C

#ifdef _PRE_WLAN_FEATURE_ML
uint32_t g_hipriv_ml_scan = 0;
uint32_t hmac_set_hipriv_ml_scan(mac_vap_stru *mac_vap, uint32_t *params)
{
    g_hipriv_ml_scan = params[0];

    oam_warning_log1(0, 0, "{hmac_set_hipriv_ml_scan::ml_scan [0x%x]}", g_hipriv_ml_scan);
    return OAL_SUCC;
}
// hipriv控制，强制使能ml_scan
uint32_t hmac_get_hipriv_ml_scan(void)
{
    return g_hipriv_ml_scan & BIT0;
}
// hipriv控制，强制不封装Per-STA Profile的bss，用于打桩验证ml单播扫描结果中没有Per-STA Profile场景，此时应该退化成单link关联
uint32_t hmac_get_hipriv_ml_scan_no_bss(void)
{
    return g_hipriv_ml_scan & BIT1;
}

void hmac_ml_scan_comp_cb(void *record)
{
    hmac_scan_record_stru *scan_record = (hmac_scan_record_stru *)record;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_scan_rsp_stru scan_rsp = {0};

    /* 获取hmac vap */
    hmac_vap = mac_res_get_hmac_vap(scan_record->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_ml_scan_comp_cb::hmac_vap is null.");
        return;
    }

    /* 上报扫描结果结构体初始化 */
    memset_s(&scan_rsp, sizeof(scan_rsp), 0, sizeof(scan_rsp));
    scan_rsp.uc_result_code = scan_record->en_scan_rsp_status;

    /* 扫描结果发给sme，调用函数： hmac_handle_mld_scan_rsp_sta */
    hmac_send_rsp_to_sme_sta(hmac_vap, HMAC_SME_MLD_SCAN_RSP, (uint8_t *)&scan_rsp);

    return;
}
void hmac_prepare_ml_scan_req(hmac_vap_stru *hmac_vap, mac_scan_req_stru *scan_params)
{
    uint32_t ret;
    scan_params->p_fn_cb = hmac_ml_scan_comp_cb;
    // 指定ssid
    ret = memcpy_s(scan_params->ast_mac_ssid_set[0].auc_ssid, WLAN_SSID_MAX_LEN,
                   hmac_vap->ml_scan_param.ssid, WLAN_SSID_MAX_LEN);
    scan_params->uc_ssid_num = 1;
    // 指定bssid扫描
    oal_set_mac_addr(&scan_params->auc_bssid[0][0], hmac_vap->ml_scan_param.bssid);
    scan_params->uc_bssid_num = 1;

    ret += memcpy_s(&scan_params->ast_channel_list[0], sizeof(mac_channel_stru),
                    &hmac_vap->ml_scan_param.mac_channel, sizeof(mac_channel_stru));
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_prepare_ml_scan_req::memcpy mac_channel fail!");
    }
    /* 扫描参数带宽都是20M，从扫描结果中取出来的mac_channel信息已是根据扫描结果更新后的bandwidth，可能是40M 80M 160M等
       这里需要重新赋值20M
    */
    scan_params->ast_channel_list[0].en_bandwidth = WLAN_BAND_WIDTH_20M;
    scan_params->uc_channel_nums = 1;
    scan_params->mld_id = hmac_vap->ml_scan_param.mld_id;
    hmac_vap->ml_scan_param.scan_req_cnt++;
    oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
        "hmac_prepare_ml_scan_req::set ml scan params bssid[xx:xx:xx:xx:%02X:%02X], scan_req_cnt %d, scan_mode %d",
        scan_params->auc_bssid[0][BYTE_OFFSET_4],
        scan_params->auc_bssid[0][BYTE_OFFSET_5],
        hmac_vap->ml_scan_param.scan_req_cnt,
        scan_params->en_scan_mode);
}

void hmac_clear_ml_scan_params(hmac_vap_stru *hmac_vap)
{
    uint32_t ret;
    hmac_vap->ml_scan_param.vap_state_bak = MAC_VAP_STATE_INIT;
    hmac_vap->ml_scan_param.mld_id = 0;
    hmac_vap->ml_scan_param.scan_req_cnt = 0;
    ret = memset_s(hmac_vap->ml_scan_param.ssid, WLAN_SSID_MAX_LEN, 0, WLAN_SSID_MAX_LEN);
    ret += memset_s(hmac_vap->ml_scan_param.bssid, WLAN_MAC_ADDR_LEN, 0, WLAN_MAC_ADDR_LEN);
    ret += memset_s(hmac_vap->ml_scan_param.sta_mac_addr, WLAN_MAC_ADDR_LEN, 0, WLAN_MAC_ADDR_LEN);
    ret += memset_s(&hmac_vap->ml_scan_param.mac_channel, sizeof(mac_channel_stru), 0, sizeof(mac_channel_stru));
    if (ret != EOK) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, 0,
                       "{hmac_clear_ml_scan_params::clear params fail, ret[%d]}", ret);
    }
}

uint32_t hmac_save_connect_param(hmac_vap_stru *hmac_vap, hmac_conn_param_stru *connect_param)
{
    uint32_t ret;
    uint8_t *connect_ie = NULL;
    uint8_t *wep_key = NULL;
    ret = memcpy_s(&hmac_vap->ml_scan_param.connect_param, sizeof(hmac_conn_param_stru),
                   connect_param, sizeof(hmac_conn_param_stru));

    if (connect_param->ie_len != 0 && connect_param->puc_ie != NULL) {
        connect_ie = oal_memalloc(connect_param->ie_len);
        if (connect_ie == NULL) {
            oam_error_log1(0, OAM_SF_CFG, "{hmac_save_connect_param::alloc connect_ie fail, ie_len[%d]}",
                connect_param->ie_len);
            return OAL_FAIL;
        }
        ret += memcpy_s(connect_ie, connect_param->ie_len, connect_param->puc_ie, connect_param->ie_len);
    }

    if (connect_param->uc_wep_key_len != 0 && connect_param->puc_wep_key != NULL) {
        wep_key = oal_memalloc(connect_param->uc_wep_key_len);
        if (wep_key == NULL) {
            oal_free(connect_ie);
            oam_error_log1(0, OAM_SF_CFG, "{hmac_save_connect_param::alloc wep_key fail, wep_key_len[%d]}",
                connect_param->uc_wep_key_len);
            return OAL_FAIL;
        }
        ret += memcpy_s(wep_key, connect_param->uc_wep_key_len, connect_param->puc_wep_key,
                        connect_param->uc_wep_key_len);
    }
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_save_connect_param::copy connect ie/wep_key fail}");
        if (connect_ie != NULL) {
            oal_free(connect_ie);
        }
        if (wep_key != NULL) {
            oal_free(wep_key);
        }
        return OAL_FAIL;
    }
    hmac_vap->ml_scan_param.connect_param.puc_ie = connect_ie;
    hmac_vap->ml_scan_param.connect_param.puc_wep_key = wep_key;
    return OAL_SUCC;
}
uint8_t hmac_check_link_bss_exist(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    ml_link_info_stru *mld_link_info;
    mac_bss_dscr_stru *bss_dscr_tmp = NULL;

    /* 只有一个link 或者 同频段双link，不需要mld scan */
    mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, bss_dscr);
    if (mld_link_info == NULL) {
        return OAL_FALSE;
    }

    bss_dscr_tmp = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
        mld_link_info->sta_mac_addr);
    if (bss_dscr_tmp == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_check_link_bss_exist::can't get mld link bss}");
        return OAL_FALSE;
    }

    if ((bss_dscr_tmp->ml_info.ml_cap == OAL_FALSE) ||
        (memcmp(bss_dscr_tmp->ml_info.ml_mac_addr, bss_dscr->ml_info.ml_mac_addr, WLAN_MAC_ADDR_LEN) != 0)) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_check_link_bss_exist::another link bss not support mlo}");
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

uint8_t hmac_sta_check_start_ml_scan(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    if (hmac_get_bss_mld_link_info(hmac_vap, bss_dscr) == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_sta_check_start_ml_scan::mld_link_info is NULL, no need ml scan}");
        return OAL_FALSE;
    }

    /* mld scan次数是否到达上限 */
    if (hmac_check_ml_scan_time(hmac_vap) != OAL_TRUE) {
        return OAL_FALSE;
    }

    /* 如果hipriv下发执行mld scan，无论是否扫描到另一个link 都要执行mld scan */
    if (hmac_get_hipriv_ml_scan() == OAL_TRUE) {
        return OAL_TRUE;
    }

    /* 如果AP MLD支持两个link 且 bss链表缺少另一个link的bss dscr，则需要执行mld scan */
    if (hmac_check_link_bss_exist(hmac_vap, bss_dscr) == OAL_FALSE) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}
uint32_t hmac_sta_start_ml_scan(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr, mac_vap_state_enum vap_state,
    hmac_conn_param_stru *connect_param)
{
    uint8_t is_p2p0_scan = 0;
    uint32_t ret;
    ml_link_info_stru *mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, bss_dscr);
    if (mld_link_info == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_sta_start_ml_scan::mld_link_info is NULL}");
        return OAL_FAIL;
    }
    // 缓存单播扫描的参数、关联参数等
    mac_vap_state_change(&hmac_vap->st_vap_base_info, vap_state);
    hmac_vap->ml_scan_param.mld_id = bss_dscr->ml_info.mld_id;
    ret = memcpy_s(hmac_vap->ml_scan_param.ssid, WLAN_SSID_MAX_LEN, bss_dscr->ac_ssid, WLAN_SSID_MAX_LEN);
    ret += memcpy_s(hmac_vap->ml_scan_param.bssid, WLAN_MAC_ADDR_LEN, bss_dscr->auc_bssid, WLAN_MAC_ADDR_LEN);
    ret += memcpy_s(hmac_vap->ml_scan_param.sta_mac_addr, WLAN_MAC_ADDR_LEN,
                    mld_link_info->sta_mac_addr, WLAN_MAC_ADDR_LEN);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_sta_start_ml_scan::copy ssid bssid fail}");
        return OAL_FAIL;
    }
    ret = memcpy_s(&hmac_vap->ml_scan_param.mac_channel, sizeof(mac_channel_stru),
                   &bss_dscr->st_channel, sizeof(mac_channel_stru));
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_sta_start_ml_scan::copy mac_channel fail}");
        return OAL_FAIL;
    }
    if (hmac_save_connect_param(hmac_vap, connect_param) != OAL_SUCC) {
        return OAL_FAIL;
    }
    if (hmac_sta_initiate_scan(&hmac_vap->st_vap_base_info, sizeof(uint8_t), &is_p2p0_scan) != OAL_SUCC) {
        hmac_free_connect_param_resource(&hmac_vap->ml_scan_param.connect_param);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

// 保存扫描结果中的ml ie、per_sta_profile去分片地址，方便后续使用
typedef struct {
    uint8_t *ie;
    uint16_t ie_len;
    uint8_t is_alloc; // 标记是否是分片时申请的内存
} ie_info_stru;
typedef struct {
    ie_info_stru ml_ie;
    ie_info_stru per_sta_profile[WLAN_AP_MAX_ML_LINK_INFO_NUM];
} ml_defragment_stru;
ml_defragment_stru g_ml_defragment = {{ 0 }, {{ 0 }, { 0 }, { 0 }, { 0 }}};

void hmac_free_ml_defragment(void)
{
    uint8_t link_id;
    if (g_ml_defragment.ml_ie.ie != NULL && g_ml_defragment.ml_ie.is_alloc == OAL_TRUE) {
        oal_free(g_ml_defragment.ml_ie.ie);
        g_ml_defragment.ml_ie.is_alloc = OAL_FALSE;
    }
    g_ml_defragment.ml_ie.ie = NULL;
    g_ml_defragment.ml_ie.ie_len = 0;
    for (link_id = 0; link_id < WLAN_AP_MAX_ML_LINK_INFO_NUM; link_id++) {
        if (g_ml_defragment.per_sta_profile[link_id].ie != NULL &&
            g_ml_defragment.per_sta_profile[link_id].is_alloc == OAL_TRUE) {
            oal_free(g_ml_defragment.per_sta_profile[link_id].ie);
            g_ml_defragment.per_sta_profile[link_id].is_alloc = OAL_FALSE;
        }
        g_ml_defragment.per_sta_profile[link_id].ie = NULL;
        g_ml_defragment.per_sta_profile[link_id].ie_len = 0;
    }
}
void hmac_set_ml_defragemen_ml_ie(uint8_t *ie)
{
    g_ml_defragment.ml_ie.ie = ie;
}
uint8_t *hmac_get_ml_defragemen_ml_ie(void)
{
    return g_ml_defragment.ml_ie.ie;
}
void hmac_set_ml_defragemen_ml_ie_len(uint16_t ie_len)
{
    g_ml_defragment.ml_ie.ie_len = ie_len;
}
uint16_t hmac_get_ml_defragemen_ml_ie_len(void)
{
    return g_ml_defragment.ml_ie.ie_len;
}
void hmac_set_ml_defragemen_ml_ie_alloc_flag(void)
{
    g_ml_defragment.ml_ie.is_alloc = OAL_TRUE;
}
void hmac_set_ml_defragemen_per_sta_profile(uint8_t *ie, uint8_t link_id)
{
    g_ml_defragment.per_sta_profile[link_id].ie = ie;
}
uint8_t *hmac_get_ml_defragemen_per_sta_profile(uint8_t link_id)
{
    return g_ml_defragment.per_sta_profile[link_id].ie;
}
void hmac_set_ml_defragemen_per_sta_profile_len(uint16_t ie_len, uint8_t link_id)
{
    g_ml_defragment.per_sta_profile[link_id].ie_len = ie_len;
}
uint16_t hmac_get_ml_defragemen_per_sta_profile_len(uint8_t link_id)
{
    return g_ml_defragment.per_sta_profile[link_id].ie_len;
}
void hmac_set_ml_defragemen_per_sta_profile_alloc_flag(uint8_t link_id)
{
    g_ml_defragment.per_sta_profile[link_id].is_alloc = OAL_TRUE;
}
/*
 * 函 数 名   : hmac_sta_parse_link_info_process
 * 功能描述   : 去分片后，解析扫描结果中的sta info和sta profile字段
 */
uint32_t hmac_sta_scan_parse_link_info_process(hmac_vap_stru *hmac_vap, uint8_t *sub_ie,
    uint16_t sub_ie_len, ml_info_stru *ap_mld_info, uint8_t link_id, uint8_t frame_type)
{
    uint8_t  *sta_profile = NULL;
    uint16_t sta_info_len;
    uint16_t sta_profile_len;
    uint32_t ret = OAL_SUCC;

    sta_info_len = mac_count_basic_ml_sta_info_length(&sub_ie[ML_STA_CONTROL_OFFSET]);
    if (sub_ie_len < (STA_CONTROL_FIELD_LEN + sta_info_len)) {
        return OAL_FAIL;
    }

    if ((ret = mac_sta_parse_basic_ml_sta_info(&sub_ie[ML_STA_INFO_OFFSET], &sub_ie[ML_STA_CONTROL_OFFSET],
        &ap_mld_info->ml_link_info[link_id])) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 如果子IE的总长度小于 sta control字段+sta info字段长度，sta profile长度会小于0，返回失败 */
    if (STA_CONTROL_FIELD_LEN + sta_info_len > sub_ie_len) {
        oam_warning_log2(0, 0,
            "{hmac_sta_scan_parse_link_info_process::ie length error, sub_ie_len[%d], sta_info_len[%d].}",
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
 * 函 数 名   : hmac_sta_scan_parse_basic_ml_link_info
 * 功能描述   : 解析扫描结果中的link info字段
 */
uint32_t hmac_sta_scan_parse_basic_ml_link_info(hmac_vap_stru *hmac_vap, uint8_t *link_info,
    uint16_t link_info_len, ml_info_stru *ap_mld_info, uint8_t frame_type)
{
    uint8_t  *sub_ie = NULL;
    uint16_t sub_ie_len;
    uint8_t  link_id = 1;

    /*********************************************************************************
     | sub ID | len | data | frag ID | len | data | ..... | frag ID | len  | data |
     |    0   | 255 |      |   254   | 255 |      |       |   254   | <255 |      |
    *********************************************************************************/
    while ((link_id < WLAN_AP_MAX_ML_LINK_INFO_NUM) && hmac_get_ml_defragemen_per_sta_profile(link_id - 1) != NULL) {
        sub_ie = hmac_get_ml_defragemen_per_sta_profile(link_id - 1);
        if (sub_ie[0] != ML_SUBEID_PER_STA_PROFILE) {
            break;
        }

        sub_ie_len = hmac_get_ml_defragemen_per_sta_profile_len(link_id - 1);
        /* 解析link info中的sta info和per-STA profile信息 */
        if (hmac_sta_scan_parse_link_info_process(hmac_vap, sub_ie,
            sub_ie_len, ap_mld_info, link_id, frame_type) != OAL_SUCC) {
            break;
        }
        link_id++;
        ap_mld_info->ml_link_num++;
    }
    return OAL_SUCC;
}


uint32_t hmac_sta_parse_scan_result_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *ie, uint16_t ie_len,
    ml_info_stru *ap_mld_info, uint8_t frame_type)
{
    uint8_t  *ml_ie = NULL;
    uint8_t common_info_len = 0;
    uint16_t link_info_len;
    uint16_t ml_ie_len;
    uint32_t ret = OAL_SUCC;
    mac_ies_info ies_info = {0};

    ies_info.frame_ies = ie;
    ies_info.frame_len = ie_len;

    ml_ie = mac_find_ie_ext_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, &ies_info);
    if (ml_ie == NULL || ml_ie[1] <= MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN) {
        return OAL_SUCC;
    }

    ml_ie = hmac_get_ml_defragemen_ml_ie();
    ml_ie_len = hmac_get_ml_defragemen_ml_ie_len();
    if (ml_ie == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_parse_scan_result_basic_ml_ie::ml_defrag falut.}");
        return OAL_FAIL;
    }
    if (hmac_sta_parse_basic_ml_common_info_process(ml_ie, ie_len, ap_mld_info, &common_info_len) != OAL_SUCC) {
        return OAL_FAIL;
    }

    ap_mld_info->ml_link_num = 1; /* 上述ML IE解析成功，至少存在一个link */
    ml_ie += MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len;

    /* 解析 Link Info */
    if (ml_ie_len > (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len)) {
        link_info_len = ml_ie_len - (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len);
        ret = hmac_sta_scan_parse_basic_ml_link_info(hmac_vap, ml_ie, link_info_len, ap_mld_info, frame_type);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_parse_scan_result_basic_ml_ie::parse basic ml link info fail.}");
        }
    }
    return ret;
}

/*
 * 函 数 名   : hmac_sta_parse_basic_ml_link_info
 * 功能描述   : 解析link info字段
 */
OAL_STATIC uint32_t hmac_sta_defrage_per_sta_profile(hmac_vap_stru *hmac_vap, uint8_t *link_info,
    uint16_t link_info_len, uint8_t frame_type, uint16_t frame_len)
{
    uint8_t  *sub_ie = NULL;
    uint8_t  *defrag_sub_ie = NULL;
    uint16_t offset = 0;
    uint16_t sub_ie_len;
    uint16_t frag_ie_len;
    uint16_t defrag_sub_ie_len = frame_len;
    uint8_t  link_id = 0;
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
            /* 申请内存保存去分片后的帧体信息 按beacon帧frame_len的长度申请内存，防止去分片时申请的内存长度不足导致截断 */
            defrag_sub_ie = (uint8_t *)oal_memalloc(defrag_sub_ie_len);
            ret = mac_per_sta_profile_subelement_defragment(&sub_ie, &sub_ie_len, &frag_ie_len,
                                                            defrag_sub_ie, defrag_sub_ie_len);
            if (ret != OAL_SUCC) {
                mac_free_sub_ie_defrag_buffer(defrag_sub_ie);
                return ret;
            }
            hmac_set_ml_defragemen_per_sta_profile_alloc_flag(link_id);
            /* 去分片成功，将解析地址替换为去分片后的内存地址 */
            sub_ie = defrag_sub_ie;
        }
        hmac_set_ml_defragemen_per_sta_profile(sub_ie, link_id);
        hmac_set_ml_defragemen_per_sta_profile_len(sub_ie_len, link_id);
        /* 帧体指针偏移 */
        offset += frag_ie_len + MAC_IE_HDR_LEN;
        link_id++;
    }

    if ((link_info_len != offset) && (link_id < WLAN_AP_MAX_ML_LINK_INFO_NUM)) {
        oam_error_log3(0, OAM_SF_ASSOC,
                       "{hmac_sta_defrage_per_sta_profile::link_id[%d], link_info_len[%d] != offset[%d].}",
                       link_id, link_info_len, offset);
        ret = OAL_FAIL;
    }

    return ret;
}

uint32_t hmac_sta_defrag_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *frame_body, uint16_t frame_len)
{
    uint8_t  *ml_ie = NULL;
    mac_ies_info ies_info = {0};
    ml_info_stru ap_mld_info = { 0 }; // 复用接口使用临时变量，无实际用途
    uint16_t ml_ie_len;
    uint8_t common_info_len = 0;
    uint16_t link_info_len;
    uint32_t ret = OAL_SUCC;
    uint8_t *defrag_ie = NULL;
    uint16_t defrag_ie_len  = frame_len;

    ies_info.frame_ies = frame_body;
    ies_info.frame_len = frame_len;

    ml_ie = mac_find_ie_ext_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, &ies_info);
    if (ml_ie == NULL || ml_ie[1] <= MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN) {
        return OAL_FAIL;
    }
    ml_ie_len = ml_ie[1];
    if (mac_check_ml_ie_need_defragment(ml_ie, frame_len)) {
        /* ML IE需要去分片，申请内存，按beacon帧frame_len的长度申请内存，防止去分片时申请的内存长度不足导致截断 */
        defrag_ie = (uint8_t *)oal_memalloc(defrag_ie_len);
        /* 去分片流程，将实际数据拷贝到申请的内存中 */
        ret = mac_basic_ml_element_defragment(&ml_ie, &ml_ie_len, defrag_ie, defrag_ie_len);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_defrag_ml_ie::element_defragment fail.}");
            mac_free_ie_defrag_buffer(defrag_ie);
            return ret;
        }
        hmac_set_ml_defragemen_ml_ie_alloc_flag();
        /* 去分片解析数据成功，后续ML IE解析在去分片结果上进行 */
        ml_ie = defrag_ie;
    }
    hmac_set_ml_defragemen_ml_ie(ml_ie);
    hmac_set_ml_defragemen_ml_ie_len(ml_ie_len);

    if (hmac_sta_parse_basic_ml_common_info_process(ml_ie, frame_len, &ap_mld_info, &common_info_len) != OAL_SUCC) {
        mac_free_ie_defrag_buffer(defrag_ie);
        hmac_free_ml_defragment();
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_sta_defrag_ml_ie::parse ml_common_info fail.}");
        return OAL_FAIL;
    }
    ml_ie += MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len;

    /* 解析 Link Info */
    if (ml_ie_len > (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len)) {
        link_info_len = ml_ie_len - (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len);
        ret = hmac_sta_defrage_per_sta_profile(hmac_vap, ml_ie, link_info_len, WLAN_MGMT_SUBTYPE_BUTT, frame_len);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_defrag_ml_ie::defrage_per_sta_profile fail.}");
            mac_free_ie_defrag_buffer(defrag_ie);
            hmac_free_ml_defragment();
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}

uint32_t hmac_sta_per_sta_profile_copy_cb(oal_netbuf_stru *netbuf, oal_netbuf_stru *new_netbuf)
{
    int32_t ret;
    hal_rx_ctl_stru *rx_ctrl = NULL;
    hal_rx_ctl_stru *new_rx_ctrl = NULL;

    rx_ctrl = (hal_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    /* copy cb 字段 */
    new_rx_ctrl = (hal_rx_ctl_stru *)oal_netbuf_cb(new_netbuf);
    ret = memcpy_s(new_rx_ctrl, sizeof(hal_rx_ctl_stru), rx_ctrl, sizeof(hal_rx_ctl_stru));
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_sta_per_sta_profile_copy_cb::memcpy fail!");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

uint32_t hmac_sta_per_sta_profile_encap_mac_header(oal_netbuf_stru *netbuf, uint16_t buf_len,
    oal_netbuf_stru *new_netbuf, uint8_t *per_sta_profile, uint16_t per_sta_profile_len)
{
    uint8_t *frame = oal_netbuf_data(netbuf);
    uint8_t *new_frame = oal_netbuf_data(new_netbuf);
    mac_ieee80211_frame_stru *new_header = (mac_ieee80211_frame_stru *)new_frame;
    uint16_t offset = ML_STA_INFO_OFFSET + ML_STA_INFO_LENGTH;
    uint8_t *sta_mac_addr = NULL;
    uint32_t ret;

    if (new_header == NULL) {
        return OAL_FAIL;
    }
    // 拷贝mac header内容
    ret = memcpy_s(new_frame, buf_len, frame, sizeof(mac_ieee80211_frame_stru));
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_mac_header::copy mac header fail}");
        return OAL_FAIL;
    }
    // 修改mac header中的TA bssid
    if (per_sta_profile_len < (WLAN_MAC_ADDR_LEN + offset)) {
        oam_error_log1(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_mac_header::per_sta_profile_len[%d]}",
                       per_sta_profile_len);
        return OAL_FAIL;
    }
    sta_mac_addr = per_sta_profile + offset;
    ret = memcpy_s(new_header->auc_address2, WLAN_MAC_ADDR_LEN, sta_mac_addr, WLAN_MAC_ADDR_LEN);
    ret += memcpy_s(new_header->auc_address3, WLAN_MAC_ADDR_LEN, sta_mac_addr, WLAN_MAC_ADDR_LEN);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_mac_header::copy mac addr fail}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

static uint8_t hmac_get_link_id_from_per_sta_profile(uint8_t per_sta_profile_idx)
{
    uint8_t *per_sta_profile = hmac_get_ml_defragemen_per_sta_profile(per_sta_profile_idx);
    uint16_t per_sta_profile_len = hmac_get_ml_defragemen_per_sta_profile_len(per_sta_profile_idx);
    mac_ml_sta_control_union *sta_ctrl = NULL;
    uint8_t ap_link_id = 0xff;

    if (per_sta_profile == NULL || per_sta_profile[0] != ML_SUBEID_PER_STA_PROFILE) {
        return ap_link_id;
    }

    if (per_sta_profile_len < sizeof(mac_ml_sta_control_union)) {
        return ap_link_id;
    }

    sta_ctrl = (mac_ml_sta_control_union *)(per_sta_profile + ML_STA_CONTROL_OFFSET);
    ap_link_id = sta_ctrl->basic_ml_sta_control.link_id;

    return ap_link_id;
}

void hmac_add_ml_ie_update_link_param(uint8_t *new_frame_body, uint8_t ap_link_id, uint8_t copy_len)
{
    uint8_t *ml_ie = new_frame_body;
    uint8_t *common_info = new_frame_body + MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN;

    if (ml_ie == NULL) {
        return;
    }
    /* 该处不需要检查越界，前面流程保证了 最小拷贝长度 > common info的偏移 */
    ml_ie[1] = copy_len - MAC_IE_HDR_LEN;
    common_info += WLAN_MAC_ADDR_LEN + 1;
    common_info[0] = ap_link_id;
}

static uint8_t hmac_per_sta_profile_add_basic_ml_ie(uint8_t *new_frame_body, uint16_t left_netbuf_len, uint8_t link_id)
{
    uint8_t *ml_ie = NULL;
    uint16_t ml_ie_len = 0;
    ml_info_stru ap_mld_info = {0};
    uint8_t common_info_len = 0;
    uint8_t copy_len = 0;
    uint8_t ap_link_id;
    uint8_t ml_ie_min_len = MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + ML_COMMON_INFO_LEN +
        WLAN_MAC_ADDR_LEN + ML_LINK_ID_INFO_FIELD_LEN + BSS_PARAMETERS_CHANGE_COUNT_LEN + MLD_CAPBILITIES_FIELD_LEN;
    int32_t ret;

    ml_ie = hmac_get_ml_defragemen_ml_ie();
    ml_ie_len = hmac_get_ml_defragemen_ml_ie_len();
    /* 只拷贝ml control和common info字段，最短长度场景为 ml_control字段 + common_info字段只携带mld_cap可变字段 */
    if (ml_ie == NULL || ml_ie_len < ml_ie_min_len) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_per_sta_profile_add_basic_ml_ie::ml_ie is null or too short!}");
        return 0;
    }

    if (hmac_sta_parse_basic_ml_common_info_process(ml_ie, ml_ie_len, &ap_mld_info, &common_info_len) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_per_sta_profile_add_basic_ml_ie::parse common_info fail!}");
        return 0;
    }

    copy_len = MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len;
    if (copy_len > ml_ie_len || copy_len > left_netbuf_len || copy_len < ml_ie_min_len) {
        oam_error_log3(0, OAM_SF_SCAN,
            "{hmac_per_sta_profile_add_basic_ml_ie::copy_len[%d] > ml_ie_len[%d] left_len[%d] invalid!}",
            copy_len, ml_ie_len, left_netbuf_len);
        return 0;
    }

    /* 查找待封装link的link id */
    ap_link_id = hmac_get_link_id_from_per_sta_profile(link_id);
    if (ap_link_id > WLAN_MAX_AP_LINK_ID) {
        oam_error_log1(0, OAM_SF_SCAN, "{hmac_get_link_id_from_per_sta_profile::link_id[%d] invalid!!}", link_id);
        return 0;
    }

    /* 拷贝ml ie */
    ret = memcpy_s(new_frame_body, left_netbuf_len, ml_ie, copy_len);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_per_sta_profile_add_comm_ie::copy ie fail}");
        return 0;
    }

    /* 更新拷贝的ml ie中common info字段的link id */
    hmac_add_ml_ie_update_link_param(new_frame_body, ap_link_id, copy_len);

    return copy_len;
}

static uint8_t hmac_per_sta_profile_add_vendor_ie(mac_ies_info *ies_info,
    uint8_t *new_frame_body, uint16_t left_netbuf_len)
{
    mac_comm_vendor_ie_info vendor_ie_list[] = {{MAC_HUAWEI_VENDER_IE, MAC_HISI_CAP_IE}};
    uint8_t ie_index;
    uint8_t *ie = NULL;
    uint8_t ie_len = 0;
    uint16_t add_ie_len = 0;
    int32_t ret;

    for (ie_index = 0; ie_index < (sizeof(vendor_ie_list) / sizeof(mac_comm_vendor_ie_info)); ie_index++) {
        /* 查找帧体中是否存在查询的IE */
        ie = mac_find_vendor_ie(
            vendor_ie_list[ie_index].oui, vendor_ie_list[ie_index].oui_type, ies_info);
        if (ie == NULL) {
            continue; /* 找不到当前ie，继续查找下一个 */
        }

        /* 计算ie长度，拷贝到新组的帧体中 */
        ie_len = ie[1] + MAC_IE_HDR_LEN;
        ret = memcpy_s(new_frame_body, left_netbuf_len, ie, ie_len);
        if (ret != EOK) {
            oam_error_log1(0, OAM_SF_SCAN, "{hmac_per_sta_profile_add_comm_ie::copy ie fail ret[%d]}", ret);
            return add_ie_len;
        }
        new_frame_body += ie_len;
        add_ie_len += ie_len;
        left_netbuf_len -= ie_len; /* 新申请的帧体内存 长度递减 */
    }

    return add_ie_len;
}

/* 函数功能：从帧体中查找mld公共IE，添加到新封装的上报帧体中 */
uint16_t hmac_per_sta_profile_add_comm_ie(oal_netbuf_stru *netbuf, uint16_t buf_len,
    oal_netbuf_stru *new_netbuf, uint16_t new_buf_len, uint8_t link_id)
{
    uint8_t *frame = (uint8_t *)oal_netbuf_data(netbuf);
    uint8_t *new_frame = (uint8_t *)oal_netbuf_data(new_netbuf);
    uint8_t *new_frame_body = new_frame + new_buf_len; /* 偏移到netbuf末尾，new_buf_len已经包含了MAC_80211_FRAME_LEN */
    uint16_t left_netbuf_len = buf_len - new_buf_len; /* 新申请内存剩余长度 */
    uint16_t offset = MAC_80211_FRAME_LEN + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    mac_ies_info ies_info = {0};
    uint8_t *ie = NULL;
    uint8_t ie_len = 0;
    uint16_t add_ie_len = 0;
    uint32_t ret;
    mac_comm_ie_info comm_ie_list[] = {{MAC_EID_RSN, 0}, {MAC_EID_RSNX, 0}};
    uint8_t ie_index;

    if (buf_len < offset) {
        return add_ie_len;
    }

    ies_info.frame_ies = frame + offset; /* netbuf偏移到tlv格式 */
    ies_info.frame_len = buf_len - offset; /* 剩余长度 */
    for (ie_index = 0; ie_index < (sizeof(comm_ie_list) / sizeof(mac_comm_ie_info)); ie_index++) {
        /* 查找帧体中是否存在查询的IE */
        ie = (comm_ie_list[ie_index].eid < MAC_EID_EXTENSION) ?
             (mac_find_ie(comm_ie_list[ie_index].eid, &ies_info)) :
             (mac_find_ie_ext_ie(comm_ie_list[ie_index].eid, comm_ie_list[ie_index].ext_eid, &ies_info));
        if (ie == NULL) {
            continue; /* 找不到当前ie，继续查找下一个 */
        }

        /* 计算ie长度，拷贝到新组的帧体中 */
        ie_len = (comm_ie_list[ie_index].eid < MAC_EID_EXTENSION) ?
                 (ie[1] + MAC_IE_HDR_LEN) : (ie[1] + MAC_IE_EXT_HDR_LEN);
        ret = (uint32_t)memcpy_s(new_frame_body, left_netbuf_len, ie, ie_len);
        if (ret != EOK) {
            oam_error_log0(0, OAM_SF_SCAN, "{hmac_per_sta_profile_add_comm_ie::copy ie fail}");
            return add_ie_len;
        }
        new_frame_body += ie_len;
        add_ie_len += ie_len;
        left_netbuf_len -= ie_len; /* 新申请的帧体内存 长度递减 */
    }

    /* 拷贝ml_ie时 只拷贝ml_control和common_info字段 */
    ie_len = hmac_per_sta_profile_add_basic_ml_ie(new_frame_body, left_netbuf_len, link_id);
    new_frame_body += ie_len;
    add_ie_len += ie_len;
    left_netbuf_len -= ie_len;

    /* 拷贝hisi_cap_ie */
    ie_len = hmac_per_sta_profile_add_vendor_ie(&ies_info, new_frame_body, left_netbuf_len);
    new_frame_body += ie_len;
    add_ie_len += ie_len;
    left_netbuf_len -= ie_len;

    oam_warning_log1(0, OAM_SF_SCAN, "{hmac_per_sta_profile_add_comm_ie::add_ie len[%d]}", add_ie_len);
    return add_ie_len;
}

uint32_t hmac_sta_per_sta_profile_encap_fix_params(oal_netbuf_stru *netbuf, uint16_t buf_len,
    oal_netbuf_stru *new_netbuf, uint8_t *per_sta_profile, uint16_t per_sta_profile_len)
{
    uint8_t *frame = (uint8_t *)oal_netbuf_data(netbuf);
    uint8_t *frame_body = frame + MAC_80211_FRAME_LEN;
    uint8_t *new_frame = (uint8_t *)oal_netbuf_data(new_netbuf);
    uint8_t *new_frame_body = new_frame + MAC_80211_FRAME_LEN;
    uint8_t *profile_beacon_interval = NULL;
    uint8_t sta_info_len = 0;
    uint8_t *profile_cap_info = NULL;
    uint16_t offset = ML_STA_INFO_OFFSET + ML_STA_INFO_LENGTH + WLAN_MAC_ADDR_LEN;
    uint32_t ret;

    // 拷贝time stamp
    ret = memcpy_s(new_frame_body, MAC_TIME_STAMP_LEN, frame_body, MAC_TIME_STAMP_LEN);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_fix_params::copy timestamp fail}");
        return OAL_FAIL;
    }
    new_frame_body += MAC_TIME_STAMP_LEN;
    if (per_sta_profile_len < (offset + MAC_BEACON_INTERVAL_LEN)) {
        oam_error_log1(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_fix_params::per_sta_profile_len[%d]}",
            per_sta_profile_len);
        return OAL_FAIL;
    }
    profile_beacon_interval = per_sta_profile + offset;
    // 拷贝beacon interval
    ret = memcpy_s(new_frame_body, MAC_BEACON_INTERVAL_LEN, profile_beacon_interval, MAC_BEACON_INTERVAL_LEN);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_fix_params::copy beacon interval fail}");
        return OAL_FAIL;
    }
    // 拷贝 cap info
    sta_info_len = mac_count_basic_ml_sta_info_length(&per_sta_profile[ML_STA_CONTROL_OFFSET]);
    if (per_sta_profile_len < (ML_STA_INFO_OFFSET + sta_info_len + MAC_CAP_INFO_LEN)) {
        oam_error_log2(0, OAM_SF_SCAN,
                       "{hmac_sta_per_sta_profile_encap_fix_params::per_sta_profile_len[%d], sta_info_len[%d]}",
                       per_sta_profile_len, sta_info_len);
        return OAL_FAIL;
    }
    profile_cap_info = per_sta_profile + ML_STA_INFO_OFFSET + sta_info_len;
    new_frame_body += MAC_BEACON_INTERVAL_LEN;
    ret = memcpy_s(new_frame_body, MAC_CAP_INFO_LEN, profile_cap_info, MAC_CAP_INFO_LEN);
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_encap_fix_params::copy cap_info fail}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

uint8_t *hmac_sta_get_per_sta_profile_ie_postion(uint8_t *per_sta_profile, uint16_t per_sta_profile_len,
    uint16_t *left_len)
{
    uint8_t sta_info_len = mac_count_basic_ml_sta_info_length(&per_sta_profile[ML_STA_CONTROL_OFFSET]);
    uint16_t offset = ML_STA_INFO_OFFSET + sta_info_len + MAC_CAP_INFO_LEN;
    uint8_t *profile_ie = NULL;
    // offset包含Per-STA Profile ie和ie_len俩字节，而per_sta_profile_len不包含ie头
    if ((per_sta_profile_len + MAC_IE_HDR_LEN) <= offset) {
        *left_len = 0;
        return NULL;
    }
    *left_len = (per_sta_profile_len + MAC_IE_HDR_LEN) - offset;
    profile_ie = per_sta_profile + offset; // 计算per_sta_profile中第一个ie位置
    return profile_ie;
}

uint32_t hmac_sta_per_sta_profile_copy_ie(oal_netbuf_stru *netbuf, uint16_t buf_len,
    oal_netbuf_stru *new_netbuf, uint8_t *per_sta_profile, uint16_t per_sta_profile_len, uint16_t *new_buf_len_tmp)
{
    uint32_t ret = EOK;
    uint8_t *frame = (uint8_t *)oal_netbuf_data(netbuf);
    uint8_t *frame_body = frame + MAC_80211_FRAME_LEN;
    uint8_t *new_frame = (uint8_t *)oal_netbuf_data(new_netbuf);
    uint8_t *new_frame_body = new_frame + MAC_80211_FRAME_LEN;
    uint16_t offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    uint16_t frame_body_len = buf_len - wlan_chip_get_scaned_payload_extend_len() - MAC_80211_FRAME_LEN;
    uint8_t *ssid = NULL;
    uint8_t ssid_len = 0;
    uint8_t *new_frame_ssid = new_frame_body + offset;
    uint8_t *ie = NULL;
    uint16_t left_len = 0;
    uint8_t *profile_ie = hmac_sta_get_per_sta_profile_ie_postion(per_sta_profile, per_sta_profile_len, &left_len);

    new_frame_ssid[0] = MAC_EID_SSID; // tag num
    ssid = mac_get_ssid(frame_body, (int32_t)frame_body_len, &ssid_len);
    if ((ssid != NULL) && (ssid_len != 0)) {
        /* 将查找到的ssid保存到bss描述结构体中 */
        ret += memcpy_s(new_frame_ssid + MAC_IE_HDR_LEN, WLAN_SSID_MAX_LEN, ssid, ssid_len);
    }
    new_frame_ssid[1] = ssid_len;
    ie = new_frame_ssid + MAC_IE_HDR_LEN + ssid_len;
    if (left_len != 0 && profile_ie != NULL) {
        ret += memcpy_s(ie, frame_body_len - offset - MAC_IE_HDR_LEN - ssid_len, profile_ie, left_len);
    }
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_copy_ie::copy ie fail}");
        return OAL_FAIL;
    }
    *new_buf_len_tmp += (MAC_IE_HDR_LEN + ssid_len + left_len);
    return OAL_SUCC;
}

uint32_t hmac_sta_per_sta_profile_copy_scan_extend_info(oal_netbuf_stru *netbuf, uint16_t buf_len,
    oal_netbuf_stru *new_netbuf)
{
    mac_scanned_result_extend_info_stru *scan_result_info = NULL;
    mac_scanned_result_extend_info_stru *new_scan_result_info = NULL;
    uint16_t frame_len = buf_len - wlan_chip_get_scaned_payload_extend_len();
    uint32_t ret;

    scan_result_info = wlan_chip_get_scaned_result_extend_info(netbuf, frame_len);
    new_scan_result_info = wlan_chip_get_scaned_result_extend_info(new_netbuf, frame_len);
    ret = memcpy_s(new_scan_result_info, sizeof(mac_scanned_result_extend_info_stru),
        scan_result_info, sizeof(mac_scanned_result_extend_info_stru));
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_per_sta_profile_copy_scan_extend_info::copy extend info fail}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

static uint32_t hmac_ml_scan_check_new_scan_result_channel_valid(oal_netbuf_stru *netbuf, uint16_t buf_len)
{
    uint8_t channel = 0xff;
    uint8_t *frame = oal_netbuf_data(netbuf);
    uint16_t offset = MAC_80211_FRAME_LEN + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    channel = mac_ie_get_chan_num(frame, buf_len, offset, channel, OAL_FALSE);
    if (channel == 0xff) {
        oam_warning_log0(0, OAM_SF_SCAN,
            "{hmac_ml_scan_check_new_scan_result_channel_valid:invalid ml scan channel}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

oal_netbuf_stru *hmac_sta_encap_per_sta_profile_netbuf(hmac_vap_stru *hmac_vap,
    oal_netbuf_stru *netbuf, uint16_t buf_len, uint8_t link_id, uint16_t *new_buf_len)
{
    uint16_t new_buf_len_temp = 0;
    oal_netbuf_stru *new_netbuf = NULL;

    /***********************************per_sta_profile_ie******************************************
     | sub ID | len | sta_control |                  sta info              | cap info | tag xxx |
     |    0   |     |             |length|sta mac|beacon interval|  xxxx   |          |         |
     | 1byte  |1byte|    2byte    |1byte | 6byte |     2byte     | variable|   2byte  |         |
    ***********************************************************************************************/
    uint8_t *per_sta_profile = hmac_get_ml_defragemen_per_sta_profile(link_id);
    uint16_t per_sta_profile_len = hmac_get_ml_defragemen_per_sta_profile_len(link_id);

    if (per_sta_profile[0] != ML_SUBEID_PER_STA_PROFILE) {
        return NULL;
    }
    // 1. 申请netbuf内存
    new_netbuf = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, buf_len, OAL_NETBUF_PRIORITY_MID);
    if (new_netbuf == NULL) {
        oam_error_log1(0, OAM_SF_SCAN, "{Allco netbuf_len=[%d] fail}", buf_len);
        return NULL;
    }

    // 2. 封装cb字段
    if (hmac_sta_per_sta_profile_copy_cb(netbuf, new_netbuf) != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return NULL;
    }
    // 3. 封装mac_header
    if (hmac_sta_per_sta_profile_encap_mac_header(netbuf, buf_len, new_netbuf,
                                                  per_sta_profile, per_sta_profile_len) != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return NULL;
    }
    new_buf_len_temp += MAC_80211_FRAME_LEN;
    // 4. 封装fix_params
    if (hmac_sta_per_sta_profile_encap_fix_params(netbuf, buf_len, new_netbuf,
                                                  per_sta_profile, per_sta_profile_len) != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return NULL;
    }
    new_buf_len_temp += (MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN);
    // 5. 封装ie字段
    if (hmac_sta_per_sta_profile_copy_ie(netbuf, buf_len, new_netbuf, per_sta_profile,
                                         per_sta_profile_len, &new_buf_len_temp) != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return NULL;
    }

    new_buf_len_temp += hmac_per_sta_profile_add_comm_ie(netbuf, buf_len, new_netbuf, new_buf_len_temp, link_id);
    if (hmac_ml_scan_check_new_scan_result_channel_valid(new_netbuf, new_buf_len_temp) != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return NULL;
    }

    // 6. 封装扩展信息
    if (hmac_sta_per_sta_profile_copy_scan_extend_info(netbuf, buf_len, new_netbuf) != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return NULL;
    }
    new_buf_len_temp += wlan_chip_get_scaned_payload_extend_len();
    *new_buf_len = new_buf_len_temp;
    return new_netbuf;
}

static uint32_t hmac_sta_add_ml_scan_per_sta_profile_bss(hmac_vap_stru *hmac_vap,
    oal_netbuf_stru *netbuf, uint16_t buf_len)
{
    uint8_t link_id = 0;
    oal_netbuf_stru *new_netbuf = NULL;
    uint16_t new_buf_len = 0;
    mac_multi_bssid_frame_info_stru mbss_frame_info;

    for (link_id = 0; link_id < WLAN_AP_MAX_ML_LINK_INFO_NUM; link_id++) {
        if (hmac_get_ml_defragemen_per_sta_profile(link_id) == NULL) {
            continue;
        }
        new_netbuf = hmac_sta_encap_per_sta_profile_netbuf(hmac_vap, netbuf, buf_len, link_id, &new_buf_len);
        if (new_netbuf == NULL) {
            return OAL_FAIL;
        }

        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "hmac_sta_add_ml_scan_per_sta_profile_bss::link_id[%d] netbuf goto add bss",
            link_id);
        memset_s((uint8_t *)&mbss_frame_info, sizeof(mbss_frame_info), 0, sizeof(mbss_frame_info));
        hmac_scan_proc_scan_result_handle(hmac_vap, new_netbuf, new_buf_len, &mbss_frame_info);
    }
    return OAL_SUCC;
}
uint32_t hmac_ml_ucast_scan_result_handle(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf, uint16_t buf_len)
{
    uint8_t scan_abort = 1;
    uint8_t *mgmt_frame = (uint8_t *)oal_netbuf_data(netbuf);
    mac_ieee80211_frame_stru *frame_header = (mac_ieee80211_frame_stru *)mgmt_frame;
    if (hmac_get_hipriv_ml_scan_no_bss()) { // hipriv命令打桩验证扫描结果中没有Per-STA Profile 字段，退化成单link关联
        return OAL_SUCC;
    }
    // ml单播扫描结果中有per-STA Profile字段，需要封装成netbuf添加bss
    if ((frame_header != NULL) && ((frame_header->st_frame_control.bit_sub_type != WLAN_PROBE_RSP) ||
        (oal_memcmp(hmac_vap->ml_scan_param.bssid, frame_header->auc_address3, WLAN_MAC_ADDR_LEN) != 0))) {
        return OAL_SUCC;
    }
    hmac_sta_add_ml_scan_per_sta_profile_bss(hmac_vap, netbuf, buf_len);
    return hmac_config_scan_abort(&hmac_vap->st_vap_base_info, sizeof(uint8_t), &scan_abort);
}
uint32_t hmac_ml_scan_result_handle(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf, uint16_t buf_len)
{
    uint8_t *mgmt_frame = (uint8_t *)oal_netbuf_data(netbuf);
    uint8_t *frame_body = NULL;
    uint16_t frame_body_len;
    uint16_t mgmt_filed_len;
    uint32_t ret;
    hmac_scan_stru *scan_mgmt = NULL;
    /* 对dmac上报的netbuf内容进行解析，内容如下所示 */
    /***********************************************************************************************/
    /*            netbuf data域的上报的扫描结果的字段的分布                                        */
    /* ------------------------------------------------------------------------------------------  */
    /* beacon/probe rsp body  |     帧体后面附加字段(mac_scanned_result_extend_info_stru)          */
    /* -----------------------------------------------------------------------------------------   */
    /* 收到的beacon/rsp的body | rssi(4字节) | channel num(1字节)| band(1字节)|bss_tye(1字节)|填充  */
    /* ------------------------------------------------------------------------------------------  */
    /*                                                                                             */
    /***********************************************************************************************/
    /* 管理帧的长度等于上报的netbuf的长度减去尾部上报的扫描结果的扩展字段的长度 */
    if (buf_len < (MAC_80211_FRAME_LEN + wlan_chip_get_scaned_payload_extend_len())) {
        oam_warning_log1(0, OAM_SF_SCAN, "hmac_ml_scan_result_handle::buf_len[%u]", buf_len);
        return OAL_FAIL;
    }

    frame_body_len = buf_len - MAC_80211_FRAME_LEN - wlan_chip_get_scaned_payload_extend_len();
    mgmt_filed_len = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;

    if (frame_body_len <= mgmt_filed_len) {
        return OAL_FAIL;
    }
    if (mgmt_frame == NULL) {
        return OAL_FAIL;
    }

    /* ie 开始位置 */
    frame_body  = mgmt_frame + MAC_80211_FRAME_LEN + mgmt_filed_len;
    frame_body_len -= mgmt_filed_len;
    // 有ml ie时，做去分片操作
    ret = hmac_sta_defrag_ml_ie(hmac_vap, frame_body, frame_body_len);
    if (ret != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 漫游MLD扫描处理 */
    scan_mgmt = hmac_get_scam_mgmt(hmac_vap);
    // ml单播扫描结果处理流程
    if (hmac_vap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_STA_WAIT_MLD_SCAN ||
        (scan_mgmt != NULL && scan_mgmt->st_scan_record_mgmt.en_scan_mode == WLAN_SCAN_MODE_MLD_SCAN)) {
        ret = hmac_ml_ucast_scan_result_handle(hmac_vap, netbuf, buf_len);
    }
    return ret;
}
#endif
