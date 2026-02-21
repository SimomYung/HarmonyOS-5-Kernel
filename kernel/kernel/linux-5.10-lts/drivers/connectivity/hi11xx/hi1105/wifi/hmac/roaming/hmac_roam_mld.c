/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : MLD ROAM特性处理
 * 作    者 : wifi
 * 创建日期 : 2024年05月09日
 */

#include "oal_ext_if.h"
#include "securec.h"
#include "mac_mib.h"
#include "mac_ie.h"
#include "mac_frame_11be.h"
#include "hmac_11be.h"
#include "hmac_roam_alg.h"
#include "hmac_mld_ext.h"
#include "hmac_config.h"
#include "mac_frame_ml.h"
#include "hmac_scan.h"
#include "hmac_mgmt_sta.h"
#include "hmac_roam_alg.h"
#include "hmac_roam_scan.h"
#include "hmac_roam_mld.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ROAM_MLD_C

#ifdef _PRE_WLAN_FEATURE_ML
/*
 * 功能描述  : 判断目标bss dscr是否为已关联link之一
 */
uint8_t hmac_roam_mld_check_target_bss_is_link_vap(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    hmac_vap_stru *another_link_vap = NULL;
    if (bss_dscr->ml_info.ml_cap == OAL_FALSE) {
        return OAL_FALSE;
    }
    another_link_vap = hmac_mld_get_another_vap(hmac_vap);
    if (another_link_vap == NULL) {
        return OAL_FALSE;
    }

    if (oal_memcmp(bss_dscr->ml_info.ml_mac_addr, hmac_get_mld_bssid(hmac_vap), WLAN_MAC_ADDR_LEN) == 0 &&
        oal_memcmp(bss_dscr->auc_bssid, another_link_vap->st_vap_base_info.auc_bssid, WLAN_MAC_ADDR_LEN) == 0) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_mld_check_target_bss_is_link_vap::same mld_ap link}");
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

uint8_t hmac_roam_mld_bssid_check(hmac_roam_info_stru *roam_info, hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        return OAL_FALSE;
    }
    if (roam_info->cur_mld_link_bss_policy == ROAM_SAME_MLD_LINK_IGNORE) {
        return hmac_roam_mld_check_target_bss_is_link_vap(hmac_vap, bss_dscr);
    }
    return OAL_FALSE;
}

static void hmac_roam_save_user_old_mld_params(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    uint8_t                         idx, link_idx;
    uint8_t                         link_vap_id;
    hmac_vap_stru                  *hmac_link_vap;
    hmac_roam_info_stru            *link_roam_info;
    hmac_mld_connect_info          *mld_connect_info;
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;
    hmac_mld_user_manage_stru      *mld_user_manage    = mld_user_link_info->mld_user_manage;
    if (mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_save_user_old_mld_params::mld_user_manage is NULL}");
        return;
    }
    for (idx = 0; idx < mld_user_manage->mld_link_cnt; idx++) {
        if (mld_user_manage->record_link_info[idx].link_is_valid == OAL_FALSE) {
            oam_warning_log0(idx, OAM_SF_ROAM, "{hmac_roam_save_user_old_mld_params::link_is_valid}");
            continue;
        }
        link_vap_id = mld_user_manage->record_link_info[idx].link_vap_id;
        hmac_link_vap = mac_res_get_hmac_vap(link_vap_id);
        if (hmac_link_vap == NULL) {
            oam_error_log0(link_vap_id, OAM_SF_ROAM, "{hmac_roam_save_user_old_mld_params::hmac_link_vap is NULL}");
            continue;
        }
        link_roam_info = (hmac_roam_info_stru *)hmac_link_vap->pul_roam_info;
        if (link_roam_info == NULL) {
            oam_warning_log0(hmac_link_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_roam_save_user_old_mld_params::link_roam_info is NULL}");
            continue;
        }
        mld_connect_info = &(link_roam_info->st_old_bss.mld_connect_info);
        for (link_idx = 0; link_idx < WLAN_MAX_ML_LINK_NUM; link_idx++) {
            memcpy_s(&(mld_connect_info->record_link_info[link_idx]), sizeof(hmac_mld_record_link_info_stru),
                &(mld_user_manage->record_link_info[link_idx]), sizeof(hmac_mld_record_link_info_stru));
        }
    }
}

static void hmac_roam_restore_user_old_mld_params(hmac_roam_info_stru *roam_info)
{
    uint8_t link_id;
    hmac_mld_connect_info          *old_mld_connect_info;
    hmac_user_stru                 *hmac_user = roam_info->pst_hmac_user;
    hmac_mld_user_link_info_stru   *mld_user_link_info;
    hmac_mld_user_manage_stru      *mld_user_manage;
    if (hmac_user == NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_restore_user_old_mld_params::hmac_user is NULL}");
        return;
    }
    mld_user_link_info = &hmac_user->mld_user_link_info;
    mld_user_manage    = mld_user_link_info->mld_user_manage;
    if (mld_user_manage == NULL) {
        oam_warning_log0(0, OAM_SF_ROAM, "{hmac_roam_restore_user_old_mld_params::mld_user_manage is NULL}");
        return;
    }
    old_mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);
    memcpy_s(mld_user_manage->mld_addr, WLAN_MAC_ADDR_LEN, old_mld_connect_info->mld_bssid, WLAN_MAC_ADDR_LEN);
    for (link_id = 0; link_id < old_mld_connect_info->mld_link_cnt; link_id++) {
        memcpy_s(&(mld_user_manage->record_link_info[link_id]), sizeof(hmac_mld_record_link_info_stru),
            &(old_mld_connect_info->record_link_info[link_id]), sizeof(hmac_mld_record_link_info_stru));
    }
}

static void hmac_roam_save_vap_old_mld_params(hmac_vap_stru *hmac_vap)
{
    uint8_t link_id;
    hmac_vap_stru *link_hmac_vap;
    hmac_mld_connect_info *old_mld_connect_info = NULL;
    hmac_roam_info_stru *link_roam_info;
    hmac_mld_manage_stru *mld_vap_manage;
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_save_old_mld_params::mld_vap_manage null");
        return;
    }
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL) {
            continue;
        }
        link_roam_info = (hmac_roam_info_stru *)link_hmac_vap->pul_roam_info;
        if (link_roam_info == NULL) {
            continue;
        }
        old_mld_connect_info = &(link_roam_info->st_old_bss.mld_connect_info);
        memset_s(old_mld_connect_info, sizeof(hmac_mld_connect_info), 0, sizeof(hmac_mld_connect_info));
        old_mld_connect_info->mld_connect = OAL_TRUE;
        old_mld_connect_info->mld_link_cnt = mld_vap_manage->mld_link_cnt;
        old_mld_connect_info->ap_link_cnt = mld_vap_manage->ap_link_cnt;
        old_mld_connect_info->mld_mode = link_hmac_vap->mld_vap_link_info.mld_mode;
        memcpy_s(old_mld_connect_info->mld_addr, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN);
        memcpy_s(old_mld_connect_info->mld_bssid, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN);
        memcpy_s(old_mld_connect_info->mac_addr, WLAN_MAC_ADDR_LEN,
            mac_mib_get_StationID(&(link_hmac_vap->st_vap_base_info)), WLAN_MAC_ADDR_LEN);
        memcpy_s(&old_mld_connect_info->link_ap_info, sizeof(ml_link_info_stru),
            &link_hmac_vap->mld_vap_link_info.link_ap_info, sizeof(ml_link_info_stru));
    }
}

static void hmac_roam_restore_vap_old_mld_params(hmac_roam_info_stru *roam_info)
{
    hmac_mld_connect_info *mld_connect_info = NULL;
    hmac_vap_stru *hmac_vap = roam_info->pst_hmac_vap;
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_restore_old_mld_params::mld_vap_manage null");
        return;
    }
    mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);
    mld_vap_manage->ap_link_cnt = mld_connect_info->ap_link_cnt;
    memcpy_s(mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN, mld_connect_info->mld_addr, WLAN_MAC_ADDR_LEN);
    memcpy_s(mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN, mld_connect_info->mld_bssid, WLAN_MAC_ADDR_LEN);
    memcpy_s(&hmac_vap->mld_vap_link_info.link_ap_info, sizeof(ml_link_info_stru),
        &mld_connect_info->link_ap_info, sizeof(ml_link_info_stru));
    hmac_config_sta_h2d_mld_params_update(&hmac_vap->st_vap_base_info, mld_vap_manage);
}

void hmac_roam_start_clear_old_mld_connet_info(hmac_roam_info_stru *roam_info)
{
    memset_s(&(roam_info->st_old_bss.mld_connect_info), sizeof(hmac_mld_connect_info),
        0, sizeof(hmac_mld_connect_info));
}

static void hmac_roam_save_old_mld_params(hmac_roam_info_stru *roam_info, uint8_t cur_link_cnt)
{
    hmac_vap_stru *hmac_vap = roam_info->pst_hmac_vap;
    hmac_user_stru *hmac_user = roam_info->pst_hmac_user;
    if (cur_link_cnt == 0) {
        hmac_roam_start_clear_old_mld_connet_info(roam_info);
        return;
    }
    hmac_roam_save_vap_old_mld_params(hmac_vap);
    hmac_roam_save_user_old_mld_params(hmac_vap, hmac_user);
}

static void hmac_roam_restore_old_mld_params(hmac_roam_info_stru *roam_info)
{
    hmac_roam_restore_vap_old_mld_params(roam_info);
    hmac_roam_restore_user_old_mld_params(roam_info);
}

static void hmac_mld_roam_info_update(hmac_roam_info_stru  *link_roam_info, hmac_roam_info_stru  *roam_info,
    mac_bss_dscr_stru *other_link_dscr)
{
    uint8_t link_id;
    hmac_mld_connect_info          *link_mld_connect_info = &(link_roam_info->st_old_bss.mld_connect_info);
    hmac_mld_connect_info          *roam_mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);
    mac_bss_dscr_stru *save_bss_dscr_info = &(link_roam_info->save_bss_dscr_info);

    link_roam_info->uc_invalid_scan_cnt =  roam_info->uc_invalid_scan_cnt;
    link_roam_info->st_static.scan_end_timetamp = roam_info->st_static.scan_end_timetamp;
    link_roam_info->st_config.uc_scan_orthogonal = roam_info->st_config.uc_scan_orthogonal;
    link_roam_info->current_bss_policy = roam_info->current_bss_policy; /* false表示漫游到自己 */
    link_roam_info->en_roam_trigger = roam_info->en_roam_trigger;
#ifdef _PRE_WLAN_FEATURE_11R
    link_roam_info->st_connect.uc_ft_force_air = roam_info->st_connect.uc_ft_force_air;
    link_roam_info->st_connect.uc_ft_failed = roam_info->st_connect.uc_ft_failed;
#endif
    oal_set_mac_addr(link_roam_info->auc_target_bssid, roam_info->auc_target_bssid);
    memcpy_s(save_bss_dscr_info, sizeof(mac_bss_dscr_stru), other_link_dscr, sizeof(mac_bss_dscr_stru));

    for (link_id = 0; link_id < roam_mld_connect_info->mld_link_cnt; link_id++) {
        memcpy_s(&(link_mld_connect_info->record_link_info[link_id]), sizeof(hmac_mld_record_link_info_stru),
            &(roam_mld_connect_info->record_link_info[link_id]), sizeof(hmac_mld_record_link_info_stru));
    }
}

static void hmac_mld_link_roam_prepare(hmac_vap_stru *link_hmac_vap, ml_link_info_stru *other_mld_link_info,
    mac_bss_dscr_stru *other_link_dscr)
{
    hmac_mld_update_vap_link_info(link_hmac_vap, other_mld_link_info);
    hmac_roam_renew_privacy(link_hmac_vap, other_link_dscr);
    hmac_check_capability_mac_phy_supplicant(&link_hmac_vap->st_vap_base_info, other_link_dscr);
}

static uint32_t hmac_start_double_to_double_link_roam_connect(hmac_roam_info_stru  *roam_info,
    mac_bss_dscr_stru *cur_bss_dscr, mac_bss_dscr_stru *other_link_dscr)
{
    uint32_t ret;
    uint8_t link_id;
    hmac_vap_stru *link_hmac_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_roam_info_stru  *link_roam_info = NULL;
    hmac_vap_stru *hmac_vap = roam_info->pst_hmac_vap;
    ml_link_info_stru *other_mld_link_info;

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 1、根据扫描结果，更新漫游vap的mld vap user mld参数 */
    hmac_vap_update_mld_params(hmac_vap, cur_bss_dscr);
    /* 2、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_user_update_mld_params(hmac_vap, roam_info->pst_hmac_user);
    /* 3、非入网link先执行漫游connect */
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL || link_hmac_vap == hmac_vap) {
            continue;
        }
        other_mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, cur_bss_dscr);
        hmac_mld_link_roam_prepare(link_hmac_vap, other_mld_link_info, other_link_dscr);
        link_roam_info = (hmac_roam_info_stru *)link_hmac_vap->pul_roam_info;
        if (link_roam_info == NULL) {
            oam_error_log0(link_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_start_double_to_double_link_mld_roam_connect::link_roam_info null");
            continue;
        }
        hmac_mld_roam_info_update(link_roam_info, roam_info, other_link_dscr);
        /* 非漫游link漫游     主状态机手动切到扫描状态 */
        hmac_roam_main_change_state(link_roam_info, ROAM_MAIN_STATE_SCANING);
        ret = hmac_roam_main_fsm_action(link_roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT, (void *)other_link_dscr);
        if (ret != OAL_SUCC) {
            return ret;
        }
    }
    /* 4、入网link后执行 */
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static void hmac_roam_start_other_link_no_fsm_proc(hmac_roam_info_stru *roam_info)
{
    uint8_t link_id;
    mac_bss_dscr_stru *cur_bss_dscr;
    hmac_vap_stru *link_hmac_vap = NULL;
    ml_link_info_stru *other_mld_link_info;
    mac_bss_dscr_stru *other_link_dscr;
    hmac_vap_stru *hmac_vap = roam_info->pst_hmac_vap;
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return;
    }
    cur_bss_dscr = &(roam_info->save_bss_dscr_info);
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL || link_hmac_vap == hmac_vap) {
            continue;
        }
        other_link_dscr = &(roam_info->save_other_bss_dscr_info);
        other_mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, cur_bss_dscr);
        if (other_mld_link_info == NULL) {
            oam_error_log0(link_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_roam_start_other_link_no_fsm_proc::other_mld_link_info null");
            continue;
        }
        hmac_mld_link_roam_prepare(link_hmac_vap, other_mld_link_info, other_link_dscr);
        hmac_roam_handle_start_join(link_hmac_vap, other_link_dscr);
        hmac_add_link_user(link_hmac_vap);
        break;
    }
}

static mac_bss_dscr_stru *hmac_mld_roam_get_link_dscr(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *cur_bss_dscr)
{
    ml_link_info_stru *other_mld_link_info;
    mac_bss_dscr_stru *other_link_dscr = NULL;

    other_mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, cur_bss_dscr);
    if (other_mld_link_info == NULL) {
        return NULL;
    }
    other_link_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
        other_mld_link_info->sta_mac_addr);
    return other_link_dscr;
}

static uint32_t hmac_legacy_to_legacy_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static uint32_t hmac_legacy_to_double_link_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    mac_bss_dscr_stru *cur_bss_dscr;
    hmac_mld_manage_stru *mld_vap_manage;

    /* 1、vap模式切换 */
    hmac_change_all_vap_to_mld_mode(hmac_vap, roam_link_cnt);
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    cur_bss_dscr = &(roam_info->save_bss_dscr_info);
    /* 2、根据扫描结果，更新漫游vap的mld vap user mld参数 */
    hmac_vap_update_mld_params(hmac_vap, cur_bss_dscr);
    /* 3、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_user_mld_mode_change(hmac_vap, roam_info->pst_hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_MLD);
    /* 4、非入网link先执行漫游connect */
    hmac_roam_start_other_link_no_fsm_proc(roam_info);
    /* 5、入网link后执行 */
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static uint32_t hmac_legacy_to_single_link_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    /* 1、当前vap模式切换 */
    hmac_change_vap_mld_mode(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_MLD);
    /* 2、根据扫描结果，更新漫游vap的mld vap user mld参数 */
    hmac_vap_update_mld_params(hmac_vap, &(roam_info->save_bss_dscr_info));
    /* 3、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_user_mld_mode_change(hmac_vap, roam_info->pst_hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_MLD);
    /* 4、入网link后执行 */
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static uint32_t hmac_single_link_to_legacy_mld_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    hmac_user_stru *hmac_user = roam_info->pst_hmac_user;
    if (hmac_user == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_single_link_to_legacy_mld_roam_start::hmac_user null");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user_mld_mode_change(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    hmac_change_vap_mld_mode(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);

    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static uint32_t hmac_single_to_single_link_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    /* 1、根据扫描结果，更新漫游vap的mld vap user mld参数 */
    hmac_vap_update_mld_params(hmac_vap, &(roam_info->save_bss_dscr_info));
    /* 2、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_user_update_mld_params(hmac_vap, roam_info->pst_hmac_user);
    /* 3、入网link后执行 */
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static uint32_t hmac_single_to_double_link_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    /* 1、另一个vap模式切换 */
    hmac_change_other_link_vap_mode(hmac_vap, roam_link_cnt, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    /* 2、根据扫描结果，更新漫游vap的mld vap user mld参数 */
    hmac_vap_update_mld_params(hmac_vap, &(roam_info->save_bss_dscr_info));
    /* 3、user模式切换&参数更新依赖更新到vap mld参数 */
    hmac_user_update_mld_params(hmac_vap, roam_info->pst_hmac_user);
    /* 4、非入网link先执行漫游connect */
    hmac_roam_start_other_link_no_fsm_proc(roam_info);
    /* 5、入网link后执行 */
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static void hmac_mld_roam_start_switch_mac_addr(hmac_vap_stru *hmac_vap)
{
    uint8_t link_id;
    hmac_vap_stru *link_hvap;
    hmac_roam_info_stru *roam_info;
    hmac_mld_connect_info *mld_connect_info;
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info);
    hmac_mld_manage_stru  *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_mld_roam_switch_mac_addr::mld_vap_manage null");
        return;
    }
    /* mib与mld地址一致，说明未做过link切换 */
    if (oal_memcmp(mld_vap_manage->mld_addr, mac_mib_get_StationID(mac_vap), WLAN_MAC_ADDR_LEN) == 0) {
        return;
    }
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_hvap = mld_vap_manage->link_vap[link_id];
        if (link_hvap == NULL) {
            continue;
        }
        roam_info = (hmac_roam_info_stru *)link_hvap->pul_roam_info;
        if (roam_info == NULL) {
            oam_error_log0(link_hvap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_mld_roam_start_switch_mac_addr::roam_info null");
            continue;
        }
        mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);
        mld_connect_info->switch_mac_addr = OAL_TRUE;
        hmac_update_vap_mac_addr(link_hvap, oal_netdevice_mac_addr(link_hvap->pst_net_device), WLAN_MAC_ADDR_LEN);
    }
}

static void hmac_mld_roam_back_switch_mac_addr(hmac_vap_stru *hmac_vap)
{
    uint8_t link_id;
    hmac_vap_stru *link_hmac_vap;
    hmac_roam_info_stru *roam_info;
    hmac_mld_connect_info *mld_connect_info;
    hmac_mld_manage_stru  *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_mld_roam_switch_mac_addr::mld_vap_manage null");
        return;
    }
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL) {
            continue;
        }
        roam_info = (hmac_roam_info_stru *)link_hmac_vap->pul_roam_info;
        if (roam_info == NULL) {
            oam_error_log0(link_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_mld_roam_back_switch_mac_addr::roam_info null");
            continue;
        }
        mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);
        if (mld_connect_info->switch_mac_addr == OAL_FALSE) {
            continue;
        }
        mld_connect_info->switch_mac_addr = OAL_FALSE;
        hmac_update_vap_mac_addr(link_hmac_vap, mld_connect_info->mac_addr, WLAN_MAC_ADDR_LEN);
    }
}

static uint32_t hmac_double_link_to_legacy_mld_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    hmac_user_stru        *hmac_user = roam_info->pst_hmac_user;
    hmac_mld_connect_info *mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);

    hmac_mld_roam_start_switch_mac_addr(hmac_vap);
    hmac_change_all_user_to_legacy_mode(hmac_vap, hmac_user, mld_connect_info->mld_link_cnt);
    hmac_change_all_vap_to_legacy_mode(hmac_vap, mld_connect_info->mld_link_cnt);

    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

static uint32_t hmac_double_to_double_link_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    mac_bss_dscr_stru *cur_bss_dscr = &(roam_info->save_bss_dscr_info);
    mac_bss_dscr_stru *other_link_dscr = &(roam_info->save_other_bss_dscr_info);

    hmac_mld_roam_start_switch_mac_addr(hmac_vap);
    return hmac_start_double_to_double_link_roam_connect(roam_info, cur_bss_dscr, other_link_dscr);
}

static uint32_t hmac_double_to_single_link_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt)
{
    hmac_user_stru        *hmac_user = roam_info->pst_hmac_user;
    hmac_mld_connect_info *mld_connect_info = &(roam_info->st_old_bss.mld_connect_info);

    hmac_mld_roam_start_switch_mac_addr(hmac_vap);
    hmac_change_other_link_user_mode(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    hmac_change_other_link_vap_mode(hmac_vap, mld_connect_info->mld_link_cnt, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    /* 1、根据扫描结果，更新漫游vap的mld vap user mld参数 */
    hmac_vap_update_mld_params(hmac_vap,  &(roam_info->save_bss_dscr_info));
    /* 2、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_user_update_mld_params(hmac_vap, roam_info->pst_hmac_user);
    /* 3、入网link后执行 */
    return hmac_roam_main_fsm_action(roam_info, ROAM_MAIN_FSM_EVENT_START_CONNECT,
        (void *)&roam_info->save_bss_dscr_info);
}

typedef uint32_t (*hmac_mld_roam_start_func)(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info,
    uint8_t roam_link_cnt);
hmac_mld_roam_start_func g_hmac_mld_roam_start_table[WLAN_MAX_ML_LINK_NUM + 1][WLAN_MAX_ML_LINK_NUM + 1] = {
        /* 0->0 1 2 */
        {hmac_legacy_to_legacy_roam_start,
            hmac_legacy_to_single_link_roam_start,
            hmac_legacy_to_double_link_roam_start},
        /* 1->0 1 2 */
        {hmac_single_link_to_legacy_mld_roam_start,
            hmac_single_to_single_link_roam_start,
            hmac_single_to_double_link_roam_start},
        /* 2->0 1 2 */
        {hmac_double_link_to_legacy_mld_roam_start,
            hmac_double_to_single_link_roam_start,
            hmac_double_to_double_link_roam_start},
    };

static uint32_t hmac_mld_roam_start(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info, uint8_t roam_link_cnt)
{
    uint32_t ret;
    uint8_t new_link_cnt;
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);
    if (cur_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1) || roam_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1)) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_mld_roam_start::cur_link_cnt %d, roam_link_cnt %d",
            cur_link_cnt, roam_link_cnt);
        return OAL_FAIL;
    }

    hmac_roam_save_old_mld_params(roam_info, cur_link_cnt);
    ret = g_hmac_mld_roam_start_table[cur_link_cnt][roam_link_cnt](hmac_vap, roam_info, roam_link_cnt);
    new_link_cnt = hmac_mld_get_link_num(hmac_vap);
    oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
        "{hmac_mld_roam_start::req[%d]->[%d]link,cur_result[%d]link,execution_result[%d]",
        cur_link_cnt, roam_link_cnt, new_link_cnt, ret);
    return ret;
}

/* 判断当前关联bss是否为目标bss的其中一路link */
OAL_STATIC oal_bool_enum_uint8 hmac_roam_cur_bss_is_same_ap_other_link(hmac_vap_stru *hmac_vap,
    mac_bss_dscr_stru *bss_dscr)
{
    uint8_t link_index;
    mac_bss_dscr_stru *bss_dscr_tmp = NULL;
    for (link_index = 0; link_index < WLAN_AP_MAX_ML_LINK_INFO_NUM; link_index++) {
        if (bss_dscr->ml_info.ml_link_info[link_index].is_add_link_list == OAL_FALSE) {
            continue;
        }
        /* 扫描结果看是否能找到该bss */
        bss_dscr_tmp = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(&hmac_vap->st_vap_base_info,
            bss_dscr->ml_info.ml_link_info[link_index].sta_mac_addr);
        if (bss_dscr_tmp == NULL) {
            continue;
        }
        if (oal_memcmp(hmac_vap->st_vap_base_info.auc_bssid,
            bss_dscr->ml_info.ml_link_info[link_index].sta_mac_addr, OAL_MAC_ADDR_LEN) == 0) {
            oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, 0,
                "hmac_roam_cur_bss_is_same_ap_other_link::is same ap other link");
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

/* 漫游发起时自适应link cnt */
OAL_STATIC uint8_t hmac_mld_start_roam_connect_adjust_real_link_cnt(hmac_roam_info_stru *roam_info)
{
    mac_bss_dscr_stru *roam_bss_dscr = &(roam_info->save_bss_dscr_info);
    uint8_t roam_link_cnt = roam_bss_dscr->ml_info.ml_link_num;
    hmac_vap_stru *hmac_vap = roam_info->pst_hmac_vap;
    mac_bss_dscr_stru *other_link_dscr = NULL;
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);
    if (roam_link_cnt < WLAN_ML_SINGLE_RADIO) {
        return roam_link_cnt;
    }
    other_link_dscr = hmac_mld_roam_get_link_dscr(hmac_vap, roam_bss_dscr);
    if (other_link_dscr == NULL) {
        roam_link_cnt = WLAN_LEGACY_ROAM_MODE;
    } else {
        /* 当前如果是非双link连接，则漫游时判断是否是target ap其中一路link降为legacy漫游；否则继续以2->2漫游, 避免自愈降规格 */
        if (cur_link_cnt < WLAN_MAX_ML_LINK_NUM &&
            hmac_roam_cur_bss_is_same_ap_other_link(hmac_vap, roam_bss_dscr) == OAL_TRUE) {
            roam_link_cnt = WLAN_LEGACY_ROAM_MODE;
        } else {
            roam_link_cnt = WLAN_MAX_ML_LINK_NUM; // 最多只支持2link
            memcpy_s(&roam_info->save_other_bss_dscr_info, sizeof(mac_bss_dscr_stru),
                other_link_dscr, sizeof(mac_bss_dscr_stru));
        }
    }
    return roam_link_cnt;
}

uint32_t hmac_mld_start_roam_connect(hmac_roam_info_stru  *roam_info, uint8_t is_roam_mld_connect,
    uint8_t is_cur_mld_connect)
{
    uint8_t roam_link_cnt = 0;
    hmac_vap_stru *hmac_vap = NULL;
    if (roam_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_vap = roam_info->pst_hmac_vap;
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 1、TDO lzhq 如果没有另一个link的扫描信息或者对方是单link  ，暂且不用mld漫游 */
    /* 只有mld漫游才涉及计算link cnt，否则默认为legacy漫游 */
    if (is_roam_mld_connect == OAL_TRUE) {
        roam_link_cnt = hmac_mld_start_roam_connect_adjust_real_link_cnt(roam_info);
    }

    return hmac_mld_roam_start(hmac_vap, roam_info, roam_link_cnt);
}

uint32_t hmac_legacy_mld_roam_back_to_legacy_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

uint32_t hmac_legacy_mld_roam_back_to_single_link(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    /* 1、link vap user 切换为mld模式 */
    hmac_change_vap_mld_mode(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_MLD);
    hmac_roam_restore_vap_old_mld_params(roam_info);

    /* 2、user模式切换&参数更新依赖更新到vap mld里的参数 */
    hmac_user_mld_mode_change(hmac_vap, roam_info->pst_hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    /* 3、mac地址恢复 */
    hmac_mld_roam_back_switch_mac_addr(hmac_vap);
    /* 4、恢复漫游link的old bss */
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

void hmac_roam_back_other_link_user_del(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    uint8_t                         idx;
    uint8_t                         link_vap_id;
    uint8_t                         link_user_id;
    hmac_vap_stru                  *hmac_link_vap   = NULL;
    hmac_user_stru                 *hmac_link_user = NULL;
    hmac_mld_user_link_info_stru   *mld_user_link_info = &hmac_user->mld_user_link_info;
    hmac_mld_user_manage_stru      *mld_user_manage    = mld_user_link_info->mld_user_manage;
    if (mld_user_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_back_other_link_user_del::mld_user_manage is NULL}");
        return;
    }
    for (idx = 0; idx < mld_user_manage->mld_link_cnt; idx++) {
        if (mld_user_manage->record_link_info[idx].link_is_valid == OAL_FALSE) {
            continue;
        }
        link_vap_id = mld_user_manage->record_link_info[idx].link_vap_id;
        hmac_link_vap = mac_res_get_hmac_vap(link_vap_id);
        if (hmac_link_vap == NULL || hmac_vap->st_vap_base_info.uc_vap_id == link_vap_id) {
            continue;
        }
        link_user_id = mld_user_manage->record_link_info[idx].link_user_id;
        hmac_link_user = mac_res_get_hmac_user(link_user_id);
        if (hmac_link_user == NULL) {
            oam_error_log0(hmac_link_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_roam_back_other_link_user_del::hmac_link_user is NULL}");
            continue;
        }
        hmac_user_mld_mode_change(hmac_link_vap, hmac_link_user, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
        hmac_user_del(&(hmac_link_vap->st_vap_base_info), hmac_link_user);
    }
}

uint32_t hmac_legacy_mld_roam_back_to_double_link(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint8_t ml_link_num = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;

    /* 1、link vap user 切换为mld模式 */
    hmac_change_all_vap_to_mld_mode(hmac_vap, ml_link_num);
    hmac_roam_restore_vap_old_mld_params(roam_info);

    /* 2、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_change_all_user_to_mld_mode(hmac_vap, roam_info->pst_hmac_user, ml_link_num);
    /* 3、mac地址恢复 */
    hmac_mld_roam_back_switch_mac_addr(hmac_vap);
    /* 4、恢复漫游link的old bss */
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

static uint32_t hmac_mld_roam_back_to_legacy_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    hmac_user_stru *hmac_user = roam_info->pst_hmac_user;
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);
    /* 1、辅link del user, user 切换为legacy模式 */
    if (cur_link_cnt > WLAN_ML_SINGLE_RADIO) {
        hmac_roam_back_other_link_user_del(hmac_vap, hmac_user);
    }
    /* 2、当前link user 模式切换 */
    hmac_user_mld_mode_change(hmac_vap, hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    /* 3、link vap切换为legacy模式 */
    hmac_change_all_vap_to_legacy_mode(hmac_vap, cur_link_cnt);
    /* 4、恢复old bss参数 */
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

uint32_t hmac_single_link_roam_back_to_legacy_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return hmac_mld_roam_back_to_legacy_mld(hmac_vap, roam_info);
}

uint32_t hmac_single_link_roam_back_to_single_link(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    hmac_roam_restore_old_mld_params(roam_info);
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

uint32_t hmac_single_link_roam_back_to_double_link(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint8_t ml_link_num = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;

    /* 1、link vap user 切换为mld模式 */
    hmac_change_other_link_vap_mode(hmac_vap, ml_link_num, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    hmac_change_other_link_user_mode(hmac_vap, roam_info->pst_hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    /* 2、恢复old_mld参数 */
    hmac_roam_restore_old_mld_params(roam_info);
    /* 3、mac地址恢复 */
    hmac_mld_roam_back_switch_mac_addr(hmac_vap);
    /* 4、恢复漫游link的old bss */
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

uint32_t hmac_double_link_roam_back_to_legacy_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return hmac_mld_roam_back_to_legacy_mld(hmac_vap, roam_info);
}

uint32_t hmac_double_link_roam_back_to_double_link(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    hmac_roam_restore_old_mld_params(roam_info);
    /* 3、mac地址恢复 */
    hmac_mld_roam_back_switch_mac_addr(hmac_vap);
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

uint32_t hmac_double_link_roam_back_to_single_link(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    hmac_user_stru *hmac_user = roam_info->pst_hmac_user;
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);

    /* 1、辅link user 切换为legacy模式 del user */
    hmac_roam_back_other_link_user_del(hmac_vap, hmac_user);
    /* 2、vap切换为legacy模式 */
    hmac_change_other_link_vap_mode(hmac_vap, cur_link_cnt, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    /* 3、恢复old_mld参数 */
    hmac_roam_restore_old_mld_params(roam_info);
    /* 4、恢复old bss参数 */
    return hmac_proc_roam_to_old_bss(hmac_vap, roam_info);
}

typedef uint32_t (*hmac_mld_roam_back_func)(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info);
hmac_mld_roam_back_func g_hmac_mld_roam_back_table[WLAN_MAX_ML_LINK_NUM + 1][WLAN_MAX_ML_LINK_NUM + 1] = {
        /* 0->0 1 2 */
        {hmac_legacy_mld_roam_back_to_legacy_mld,
            hmac_legacy_mld_roam_back_to_single_link,
            hmac_legacy_mld_roam_back_to_double_link},
        /* 1->0 1 2 */
        {hmac_single_link_roam_back_to_legacy_mld,
            hmac_single_link_roam_back_to_single_link,
            hmac_single_link_roam_back_to_double_link},
        /* 2->0 1 2 */
        {hmac_double_link_roam_back_to_legacy_mld,
            hmac_double_link_roam_back_to_single_link,
            hmac_double_link_roam_back_to_double_link},
    };

uint32_t hmac_proc_mld_roam_to_old_bss(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint32_t ret;
    uint8_t new_link_cnt;
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);
    uint8_t old_link_cnt = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;
    if (cur_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1) || old_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1)) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_proc_mld_roam_to_old_bss::cur_link_cnt %d, old_link_cnt %d",
            cur_link_cnt, old_link_cnt);
        return OAL_FAIL;
    }
    ret = g_hmac_mld_roam_back_table[cur_link_cnt][old_link_cnt](hmac_vap, roam_info);
    new_link_cnt = hmac_mld_get_link_num(hmac_vap);
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
        "{hmac_proc_mld_roam_to_old_bss::req[%d]->[%d]link,result[%d]link",
        cur_link_cnt, old_link_cnt, new_link_cnt);
    return ret;
}

static uint32_t hmac_mld_roam_succ_process(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint32_t ret;
    uint8_t idx;
    uint8_t old_ap_link_cnt;
    uint8_t link_vap_id;
    uint16_t       link_user_id;
    hmac_user_stru *hmac_link_user;
    hmac_vap_stru  *hmac_link_vap;
    hmac_mld_connect_info *old_mld_connect_info;

    old_ap_link_cnt = roam_info->st_old_bss.mld_connect_info.ap_link_cnt;
    old_mld_connect_info = &roam_info->st_old_bss.mld_connect_info;
    for (idx = 0; idx < old_ap_link_cnt; idx++) {
        if (old_mld_connect_info->record_link_info[idx].link_is_valid == OAL_FALSE) {
            oam_error_log0(idx, OAM_SF_ROAM, "{hmac_mld_roam_succ_process::link invaid:");
            continue;
        }
        link_vap_id = old_mld_connect_info->record_link_info[idx].link_vap_id;
        if (hmac_vap->st_vap_base_info.uc_vap_id == link_vap_id) {
            continue;
        }
        hmac_link_vap = mac_res_get_hmac_vap(link_vap_id);
        if (hmac_link_vap == NULL) {
            oam_error_log0(0, OAM_SF_ROAM, "{hmac_mld_roam_succ_process::hmac_link_vap NULL:");
            continue;
        }
        link_user_id = old_mld_connect_info->record_link_info[idx].link_user_id;
        hmac_link_user = mac_res_get_hmac_user(link_user_id);
        if (hmac_link_user == NULL) {
            oam_error_log0(hmac_link_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
                "{hmac_mld_roam_succ_process::hmac_link_user NULL");
            continue;
        }
        ret = hmac_user_del(&(hmac_link_vap->st_vap_base_info), hmac_link_user);
        if (ret != OAL_SUCC) {
            return ret;
        }
    }
    return OAL_SUCC;
}

static void hmac_roam_update_another_link_ip_status(hmac_vap_stru *hmac_vap)
{
    /* 0/1->2 link vap的ip获取标记更新 */
    hmac_vap_stru *hmac_link_vap = hmac_mld_get_link_vap(hmac_vap);
    if (hmac_link_vap != NULL) {
        hmac_roam_wpas_connect_state_notify_for_single_link(hmac_link_vap,
            WPAS_CONNECT_STATE_IPADDR_OBTAINED, OAL_TRUE);
    }
}

uint32_t hmac_legacy_to_legacy_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return OAL_SUCC;
}

uint32_t hmac_legacy_to_single_link_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return OAL_SUCC;
}

uint32_t hmac_legacy_to_double_link_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    hmac_roam_update_another_link_ip_status(hmac_vap);
    return OAL_SUCC;
}

uint32_t hmac_single_link_to_legacy_mld_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return OAL_SUCC;
}

uint32_t hmac_single_link_to_single_link_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return OAL_SUCC;
}

uint32_t hmac_single_link_to_double_link_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    hmac_roam_update_another_link_ip_status(hmac_vap);
    return OAL_SUCC;
}

uint32_t hmac_double_link_to_legacy_mld_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    /* 删除辅link user */
    return hmac_mld_roam_succ_process(hmac_vap, roam_info);
}

uint32_t hmac_double_link_to_single_link_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return hmac_mld_roam_succ_process(hmac_vap, roam_info);
}

uint32_t hmac_double_link_to_double_link_roam_complete(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    return OAL_SUCC;
}
typedef uint32_t (*hmac_mld_roam_connect_comp_func)(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info);
hmac_mld_roam_connect_comp_func g_hmac_mld_roam_succ_table[WLAN_MAX_ML_LINK_NUM + 1][WLAN_MAX_ML_LINK_NUM + 1] = {
        /* 0->0 1 2 */
        {hmac_legacy_to_legacy_roam_complete,
            hmac_legacy_to_single_link_roam_complete,
            hmac_legacy_to_double_link_roam_complete},
        /* 1->0 1 2 */
        {hmac_single_link_to_legacy_mld_roam_complete,
            hmac_single_link_to_single_link_roam_complete,
            hmac_single_link_to_double_link_roam_complete},
        /* 2->0 1 2 */
        {hmac_double_link_to_legacy_mld_roam_complete,
            hmac_double_link_to_single_link_roam_complete,
            hmac_double_link_to_double_link_roam_complete},
    };

void hmac_roam_connect_comp_postdo_notify_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info, uint32_t result)
{
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);
    uint8_t old_link_cnt = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;
    if (cur_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1) || old_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1)) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_connect_complete_postdo_notify_mld::cur_link_cnt %d, old_link_cnt %d",
            cur_link_cnt, old_link_cnt);
        return;
    }

    roam_info->is_link_down_vap = OAL_FALSE;
    oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
        "{hmac_roam_connect_complete_postdo_notify_mld::clear link_down_vap flag");
    /* 成功在此处处理，失败的需要回退的在hmac roam to old bss 处理 */
    if (result == OAL_SUCC) {
        g_hmac_mld_roam_succ_table[old_link_cnt][cur_link_cnt](hmac_vap, roam_info);
    }
}

OAL_STATIC void hmac_mld_assoc_ml_link_info_print(hmac_mld_manage_stru *mld_vap_manage)
{
    hmac_vap_stru *ml_hmac_vap = NULL;
    mac_vap_stru *ml_mac_vap = NULL;

    ml_hmac_vap = mld_vap_manage->mld_vap;
    if (ml_hmac_vap == NULL) {
        return;
    }

    ml_mac_vap = &ml_hmac_vap->st_vap_base_info;
    oam_warning_log2(ml_mac_vap->uc_vap_id, OAM_SF_ASSOC,
        "{hmac_mld_assoc_ml_link_info_print::ml_link chan[%d] bandwidth[%d]}",
        ml_mac_vap->st_channel.uc_chan_number, ml_mac_vap->st_channel.en_bandwidth);
}

static void hmac_link_sta_wait_asoc_rx_succ_post_handle(hmac_user_stru *hmac_link_user, hmac_vap_stru *link_hmac_vap,
    ml_link_info_stru  *link_ap_info, hmac_mld_manage_stru *mld_vap_manage, uint8_t vap_id)
{
    mac_vap_state_enum_uint8 en_new_vap_state = MAC_VAP_STATE_UP;

    if (mld_vap_manage->mld_assoc_mode == MLD_ASSOC_MODE_MLSR) {
        en_new_vap_state = MAC_VAP_STATE_STA_LINK_DOWN;
        oam_warning_log0(vap_id, OAM_SF_ASSOC, "{hmac_sta_wait_asoc_rx_succ_handle::mlsr assoc succ");
    } else if (mld_vap_manage->mld_assoc_mode == MLD_ASSOC_MODE_STR) {
        /* TDO lzhq str是不能直接切up */
        en_new_vap_state =  MAC_VAP_STATE_UP;
        oam_warning_log0(vap_id, OAM_SF_ASSOC, "{hmac_sta_wait_asoc_rx_succ_handle::str assoc succ");
    }
    hmac_sta_wait_asoc_rx_succ_handle_single_link(hmac_link_user, link_hmac_vap,
        en_new_vap_state, link_ap_info->sta_mac_addr, hmac_link_user->st_user_base_info.us_assoc_id);
}

void hmac_link_sta_wait_asoc_rx_succ_handle(hmac_vap_stru *hmac_sta)
{
    uint8_t vap_id;
    uint8_t link_id;
    hmac_vap_stru        *link_hvap = NULL;
    ml_link_info_stru    *ap_link_info = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    ml_link_info_stru    *link_ap_info;
    hmac_user_stru       *hmac_link_user = NULL;

    if (hmac_vap_is_legacy_mld_mode(hmac_sta)) {
        return;
    }
    mld_vap_manage = hmac_sta->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_link_sta_wait_asoc_rx_succ_handle::get mld hmac vap fail.");
        return;
    }

    /* 打印主link的信道、带宽信息 */
    hmac_mld_assoc_ml_link_info_print(mld_vap_manage);

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hvap = mld_vap_manage->link_vap[link_id];
        if (link_hvap == NULL || hmac_vap_is_ml_mode(link_hvap) == OAL_TRUE) {
            continue;
        }
        vap_id = link_hvap->st_vap_base_info.uc_vap_id;
        ap_link_info = &link_hvap->mld_vap_link_info.link_ap_info;
        if (ap_link_info->asoc_succ == 0) {
            oam_warning_log1(vap_id, OAM_SF_ASSOC, "{hmac_link_sta_wait_asoc_rx_succ_handle::asoc[%d].",
                ap_link_info->asoc_succ);
            continue;
        }
        link_ap_info = &link_hvap->mld_vap_link_info.link_ap_info;
        hmac_link_user = mac_vap_get_hmac_user_by_addr(&(link_hvap->st_vap_base_info), link_ap_info->sta_mac_addr,
            OAL_MAC_ADDR_LEN);
        if (hmac_link_user == NULL) {
            oam_error_log0(vap_id, OAM_SF_ASSOC,
                "{hmac_link_sta_wait_asoc_rx_succ_handle::mac_vap_get_hmac_user_by_addr fail.");
            continue;
        }
        if (link_hvap->st_vap_base_info.en_vap_state == MAC_VAP_STATE_ROAMING) {
            oam_warning_log0(vap_id, OAM_SF_ASSOC,
                "{hmac_link_sta_wait_asoc_rx_succ_handle::roaming not change vap state");
            hmac_user_add_notify_alg(&(link_hvap->st_vap_base_info), hmac_link_user->st_user_base_info.us_assoc_id);
            continue;
        }
        hmac_link_sta_wait_asoc_rx_succ_post_handle(hmac_link_user, link_hvap, link_ap_info, mld_vap_manage, vap_id);
    }
}

uint32_t hmac_double_link_to_legacy_mld_roam_comp_predo(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint8_t ml_link_num = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;
    /* 1、link vap user 切换为mld模式 */
    hmac_change_all_vap_to_mld_mode(hmac_vap, ml_link_num);
    hmac_roam_restore_vap_old_mld_params(roam_info);

    /* 2、user模式切换&参数更新依赖更新到vap mld  里的参数 */
    hmac_change_all_user_to_mld_mode(hmac_vap, roam_info->pst_hmac_user, ml_link_num);
    return OAL_SUCC;
}

uint32_t hmac_double_link_to_single_link_roam_comp_predo(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint8_t ml_link_num = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;

    /* 1、link vap user 切换为mld模式 */
    hmac_change_other_link_vap_mode(hmac_vap, ml_link_num, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    hmac_change_other_link_user_mode(hmac_vap, roam_info->pst_hmac_user, MLD_CHANGE_EVENT_CHANGE_TO_LINK);

    hmac_roam_restore_vap_old_mld_params(roam_info);
    return OAL_SUCC;
}

hmac_mld_roam_connect_comp_func g_hmac_mld_roam_comp_predo_table[WLAN_MAX_ML_LINK_NUM + 1][WLAN_MAX_ML_LINK_NUM + 1] = {
        /* 0->0 1 2 */
        {NULL,
            NULL,
            NULL},
        /* 1->0 1 2 */
        {NULL,
            NULL,
            NULL},
        /* 2->0 1 2 */
        {hmac_double_link_to_legacy_mld_roam_comp_predo,
            hmac_double_link_to_single_link_roam_comp_predo,
            NULL},
    };

void hmac_roam_mld_recover_mlsr_relationship(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info)
{
    uint8_t cur_link_cnt = hmac_mld_get_link_num(hmac_vap);
    uint8_t old_link_cnt = roam_info->st_old_bss.mld_connect_info.mld_link_cnt;

    if (cur_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1) || old_link_cnt >= (WLAN_MAX_ML_LINK_NUM + 1)) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ROAM,
            "{hmac_roam_mld_recover_mlsr_relationship::cur_link_cnt %d, old_link_cnt %d",
            cur_link_cnt, old_link_cnt);
        return;
    }

    if (g_hmac_mld_roam_comp_predo_table[old_link_cnt][cur_link_cnt] != NULL) {
        g_hmac_mld_roam_comp_predo_table[old_link_cnt][cur_link_cnt](hmac_vap, roam_info);
    }
}

void hmac_roam_connect_comp_predo_notify_mld(hmac_vap_stru *hmac_vap, hmac_roam_info_stru *roam_info, uint32_t result)
{
    /* 握手失败直接删用户，但mld关系已被改变，需要先还原mld关系 */
    if (result == OAL_ERR_CODE_ROAM_HANDSHAKE_FAIL) {
        hmac_roam_mld_recover_mlsr_relationship(hmac_vap, roam_info);
    }
}

uint8_t hmac_roam_check_need_link_switch(hmac_roam_info_stru *roam_info, hmac_vap_stru *hmac_vap,
    mac_bss_dscr_stru *bss_dscr, uint8_t is_roam_mld_connect, uint8_t is_cur_mld_connect)
{
    if (roam_info == NULL || hmac_vap == NULL || bss_dscr == NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_roam_check_need_link_switch::ptr null");
        return OAL_FALSE;
    }
    if ((roam_info->cur_mld_link_bss_policy != ROAM_ONLY_SAME_MLD_LINK_SWITCH) &&
        (roam_info->cur_mld_link_bss_policy != ROAM_SAME_MLD_LINK_SWITCH)) {
        return OAL_FALSE;
    }
    /* 如果漫游选网选到已关联的另一link，则直接link切换，无需执行漫游流程 */
    if (is_roam_mld_connect == OAL_TRUE && is_cur_mld_connect == OAL_TRUE &&
        hmac_roam_mld_check_target_bss_is_link_vap(hmac_vap, bss_dscr) == OAL_TRUE) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

uint32_t hmac_mld_link_switch_deauth_force_reassoc(hmac_vap_stru *hmac_vap)
{
    hmac_roam_start_params roam_start_params = {0};
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_vap_stru *another_hmac_vap = NULL;

    mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ROAM, "{hmac_mld_link_switch_deauth_force_reassoc::mld_vap_mgmt null!");
        return OAL_FAIL;
    }

    /* 清除link切换标记，避免重关联流程直接被deauth进入死循环 */
    mld_vap_manage->link_switch_record.link_switch_res = OAL_FALSE;
    /* 找link切换前的vap */
    another_hmac_vap = hmac_mld_get_another_vap(hmac_vap);
    if (another_hmac_vap == NULL) {
        return OAL_FAIL;
    }

    /* 指定bssid漫游到切换前的link vap上 */
    hmac_roam_start_fill_info(&roam_start_params, ROAM_SCAN_CHANNEL_ORG_0,
        OAL_TRUE, another_hmac_vap->st_vap_base_info.auc_bssid, ROAM_TRIGGER_BSSID);
    roam_start_params.cur_mld_link_bss_policy = ROAM_SAME_MLD_LINK_REASSOC;
    return hmac_roam_start(hmac_vap, &roam_start_params);
}
#endif /* end of _PRE_WLAN_FEATURE_ML */
