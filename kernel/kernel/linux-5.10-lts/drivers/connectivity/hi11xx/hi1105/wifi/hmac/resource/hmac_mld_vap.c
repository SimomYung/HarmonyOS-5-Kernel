/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : HMAC MLD VAP配置操作
 * 创建日期   : 2023年10月13日
 */

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "hmac_vap.h"
#include "hmac_config.h"
#include "hmac_chan_mgmt.h"
#include "hmac_fsm.h"
#include "hmac_cali_mgmt.h"
#include "wlan_chip_i.h"
#include "mac_frame_ml.h"
#include "hmac_mld_ext.h"
#include "hmac_p2p.h"
#include "hmac_emlsr.h"
#include "dpe_wlan_vap.h"
#include "dpe_wlan_tid.h"
#include "hmac_roam_alg.h"
#include "hmac_roam_mld.h"
#ifdef CONFIG_HISI_WIFI7_MLO
#include "oal_cfg80211.h"
#endif
#include "hmac_emlsr.h"
#ifdef _PRE_WLAN_FEATURE_11AX
#include "hmac_11ax.h"
#endif
#ifdef _PRE_WLAN_FEATURE_WUR
#include "hmac_wur.h"
#endif
#include "hmac_mgmt_classifier.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MLD_VAP_C

#ifdef _PRE_WLAN_FEATURE_ML

#define HMAC_EMLSR_PADDING_DELAY 4 // 默认EMLSR能力值
#define HMAC_EMLSR_TRANSITION_DELAY 5 // 默认EMLSR能力值

hmac_mld_manage_stru g_hmac_mld_vap_manage;
uint8_t g_mld_assoc_mode = MLD_ASSOC_MODE_MLSR;

/* 功能描述: 根据ini配置, 判断是否使能MLD 单link关联 */
uint8_t hmac_mld_enable_single_link(void)
{
    return ((g_expand_feature_switch_bitmap & CUSTOM_MLD_ENBALE_SINGLE_LINK) != 0);
}

/* 功能描述: 判断ini是否支持 emlsr能力 */
uint8_t hmac_get_emlsr_switch(void)
{
    return g_pst_mac_device_capability[0].switch_emlsr;
}

/* 功能描述: 判断ini是否支持 关联后主动进入emlsr，该定制化使能后 mlsr入网/vap状态变化时主动检测是否可进入emlsr */
uint8_t hmac_get_emlsr_default_enter_switch(void)
{
    return g_st_mac_device_custom_cfg.custom_cfg_11be.emlsr_enter_switch == OAL_TRUE;
}

/* 功能描述: 判断ini是否支持 emlsr私有对通，该定制化使能后只支持emlsr端管协同 */
uint8_t hmac_get_emlsr_hisi_switch(void)
{
    return g_st_mac_device_custom_cfg.custom_cfg_11be.emlsr_hisi_switch == OAL_TRUE;
}

/* 功能描述: 根据ini配置, 判断是否使能识别对端WIFI7端管后关闭自身MLO能力 */
uint8_t hmac_mld_enable_eht_double_wifi_disbale_mlo_cap(void)
{
    return ((g_expand_feature_switch_bitmap & CUSTOM_EHT_DOUBLE_WIFI_DISABLE_MLO_CAP) != 0);
}

uint8_t hmac_get_mlo_work_state(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);
    uint8_t mlo_work_state;

    if (hmac_vap == NULL) {
        return MLO_WORK_STATE_BUTT;
    }

    if (mld_vap_manage == NULL) {
        return (hmac_vap->st_vap_base_info.en_protocol == WLAN_EHT_MODE ?
            MLO_WORK_STATE_LEGACY_WIFI7 : MLO_WORK_STATE_DEFAULT);
    }

    mlo_work_state = ((mld_vap_manage->mld_link_cnt == WLAN_MAX_ML_LINK_NUM) ?
        (mld_vap_manage->mld_assoc_mode == MLD_ASSOC_MODE_MLSR ?
        MLO_WORK_STATE_MLSR : MLO_WORK_STATE_STR) : MLO_WORK_STATE_SINGLE_LINK);

    return mlo_work_state;
}

uint8_t hmac_check_work_in_emlsr(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE || mld_vap_manage == NULL) {
        return OAL_FALSE;
    }

    return (mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_OPENED ? OAL_TRUE : OAL_FALSE);
}

int32_t hmac_emlsr_str_get_station_change_query_info(hmac_vap_stru *hmac_vap,
    const uint8_t *mac_addr, uint8_t *real_macaddr, oal_net_device_stru **temp_net_dev, hmac_vap_stru **temp_hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);
    hmac_vap_stru *link_hmac_vap = NULL;
    mac_user_stru *link_mac_user = NULL;
    uint8_t link_id;

    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_emlsr_str_get_station_change_query_info::mld_vap_manage is null");
        return OAL_FAIL;
    }

    oam_warning_log3(0, OAM_SF_ASSOC, "{hmac_emlsr_str_get_station_change_query_info:: bssid [%x:xx:xx:xx:%x:%x]",
        mac_addr[MAC_ADDR_0], mac_addr[MAC_ADDR_4], mac_addr[MAC_ADDR_5]);
    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_hmac_vap = mld_vap_manage->link_vap[link_id];
        if (link_hmac_vap == NULL) {
            oam_error_log1(0, OAM_SF_MLD, "{hmac_emlsr_str_get_station_change_query_info::link[%d] vap null", link_id);
            continue;
        }
        link_mac_user = mac_res_get_mac_user(link_hmac_vap->st_vap_base_info.us_assoc_vap_id);
        if (link_mac_user == NULL) {
            oam_error_log1(link_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
                "{hmac_emlsr_str_get_station_change_query_info::link[%d] user null", link_id);
            continue;
        }

        if (oal_memcmp(link_mac_user->auc_user_mac_addr, mac_addr, WLAN_MAC_ADDR_LEN) == 0) {
            memcpy_s(real_macaddr, WLAN_MAC_ADDR_LEN, mac_addr, WLAN_MAC_ADDR_LEN);
            *temp_hmac_vap = link_hmac_vap;
            *temp_net_dev = link_hmac_vap->pst_net_device;
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}

/* 功能描述: 获取当前MLD关联模式为MLSR or STR */
uint8_t hmac_get_mld_assoc_mode(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage;

    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_get_mld_assoc_mode::hamc_vap or mld_vap_manage is null");
        return MLD_ASSOC_MODE_BUTT;
    }
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage->mld_link_cnt != WLAN_MAX_ML_LINK_NUM) {
        return MLD_ASSOC_MODE_BUTT;
    }

    return mld_vap_manage->mld_assoc_mode;
}

void hmac_mld_set_mld_assoc_mode(hmac_vap_stru *hmac_vap, uint8_t new_mld_assoc_mode)
{
    uint8_t old_mld_assoc_mode;
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_MLD,
            "{hmac_mld_set_mld_assoc_mode::hmac_vap NULL");
        return;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mld_set_mld_assoc_mode::mld_vap_manage NULL");
        return;
    }
    if (mld_vap_manage->mld_assoc_mode == new_mld_assoc_mode) {
        return;
    }
    old_mld_assoc_mode = mld_vap_manage->mld_assoc_mode;
    mld_vap_manage->mld_assoc_mode = new_mld_assoc_mode;
    dpe_wlan_vap_mld_assoc_mode_set(hmac_vap->st_vap_base_info.uc_vap_id, new_mld_assoc_mode);
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
        "hmac_mld_set_mld_assoc_mode:mld_assoc_mode from %d to %d.[0:MLSR, 1:STR]",
        old_mld_assoc_mode, new_mld_assoc_mode);
}

void hmac_vap_set_mld_mode(hmac_vap_stru *hmac_vap, uint8_t new_mld_mode)
{
    uint8_t cur_mld_mode = hmac_vap->mld_vap_link_info.mld_mode;

    hmac_vap->mld_vap_link_info.mld_mode = new_mld_mode;
    dpe_wlan_vap_mld_mode_set(hmac_vap->st_vap_base_info.uc_vap_id, new_mld_mode);
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_UM,
        "{hmac_vap_set_mld_mode::mld_mode[%d]->[%d]}", cur_mld_mode, new_mld_mode);
}

uint8_t hmac_vap_get_mld_mode(hmac_vap_stru *hmac_vap)
{
    return hmac_vap->mld_vap_link_info.mld_mode;
}

uint8_t hmac_mlo_check_can_enter_state(mac_vap_stru *joining_mac_vap, mac_vap_stru *other_vap)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_vap_stru *other_hmac_vap = NULL;

    hmac_vap = mac_res_get_hmac_vap(joining_mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_mlo_check_can_enter_state::hmac_vap is null}");
        return OAL_TRUE;
    }

    other_hmac_vap = mac_res_get_hmac_vap(other_vap->uc_vap_id);
    if (other_hmac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_mlo_check_can_enter_state::other hmac_vap is null}");
        return OAL_TRUE;
    }

    /* 当前join vap与另一vap均为mld模式时，为MLD双link入网场景，不拦截 */
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_FALSE &&
        hmac_vap_is_legacy_mld_mode(other_hmac_vap) == OAL_FALSE) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

hmac_vap_stru *hmac_mld_get_link_vap(hmac_vap_stru *hmac_vap)
{
    uint8_t link_idx;
    hmac_vap_stru *hmac_link_vap;
    hmac_mld_manage_stru       *mld_vap_manage;

    if (hmac_vap_is_link_mode(hmac_vap) == OAL_TRUE) {
        return hmac_vap;
    }
    if (hmac_vap_is_ml_mode(hmac_vap) == OAL_FALSE) {
        return NULL;
    }
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return NULL;
    }
    for (link_idx = 0; link_idx < mld_vap_manage->mld_link_cnt; link_idx++) {
        hmac_link_vap = mld_vap_manage->link_vap[link_idx];
        if (hmac_link_vap != NULL) {
            if (hmac_vap_is_link_mode(hmac_link_vap)) {
                return hmac_link_vap;
            }
        }
    }
    return NULL;
}

hmac_vap_stru *hmac_mld_get_master_link_vap(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru       *mld_vap_manage;

    if (hmac_vap_is_ml_mode(hmac_vap) == OAL_TRUE) {
        return hmac_vap;
    }
    if (hmac_vap_is_link_mode(hmac_vap) == OAL_FALSE) {
        return NULL;
    }
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return NULL;
    }
    return mld_vap_manage->mld_vap;
}

hmac_vap_stru *hmac_mld_get_another_vap(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage;
    hmac_vap_stru *another_vap = NULL;
    uint8_t link_idx;

    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        return NULL;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    for (link_idx = 0; link_idx < mld_vap_manage->mld_link_cnt; link_idx++) {
        another_vap = mld_vap_manage->link_vap[link_idx];
        if (another_vap != hmac_vap) {
            return another_vap;
        }
    }

    return NULL;
}

hmac_mld_manage_stru *hmac_mld_get_mld_vap_manage(hmac_vap_stru *hmac_vap)
{
    if (hmac_vap == NULL) {
        return NULL;
    }

    return hmac_vap->mld_vap_link_info.mld_vap_manage;
}

uint8_t hmac_mld_get_link_id_by_macaddr(hmac_vap_stru *hmac_vap, uint8_t *mac_addr)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_vap_stru *link_vap = NULL;
    uint8_t link_id = WLAN_MAX_ML_LINK_NUM;
    uint8_t idx;

    mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_get_link_id_by_macaddr::mld_vap_manage is null}");
        return link_id;
    }

    for (idx = 0; idx < mld_vap_manage->mld_link_cnt; idx++) {
        link_vap = mld_vap_manage->link_vap[idx];
        if (link_vap == NULL) {
            continue;
        }

        if (oal_memcmp(link_vap->st_vap_base_info.auc_bssid, mac_addr, WLAN_MAC_ADDR_LEN) == 0) {
            link_id = link_vap->mld_vap_link_info.link_id;
            break;
        }
    }

    return link_id;
}

uint8_t hmac_mlsr_is_link_switching(void)
{
    return ((g_hmac_mld_vap_manage.in_use == OAL_TRUE) ?
        g_hmac_mld_vap_manage.link_switch_record.is_link_switching : OAL_FALSE);
}

/* 函数功能：初始化link切换统计信息及标记 */
static void hmac_mlsr_link_switch_record_clear(hmac_mld_manage_stru *mld_vap_manage)
{
    mld_vap_manage->link_switch_record.link_switch_res = OAL_FALSE;
    mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
}

/* 函数功能：link切换触发时 记录本次link切换的统计信息及标记 */
static uint32_t hmac_mlsr_link_switch_start_record_proc(hmac_vap_stru *cur_work_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    mld_vap_manage = cur_work_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_link_switch_start_record_proc::mld_vap_manage NULL}");
        return OAL_FAIL;
    }

    /* 置位link切换标记，正在link切换流程中 */
    mld_vap_manage->link_switch_record.is_link_switching = OAL_TRUE;

    return OAL_SUCC;
}

/* 函数功能：记录本次link切换完成后的统计信息及标记 */
static void hmac_mlsr_link_switch_complete_record_proc(hmac_vap_stru *hmac_vap, hmac_mld_manage_stru *mld_vap_manage)
{
    /* 记录当前已有link切换触发 */
    mld_vap_manage->link_switch_record.link_switch_res = OAL_TRUE;
    /* 记录本次link切换的完成时间 */
    mld_vap_manage->link_switch_record.link_switch_time_record = hmac_mld_link_switch_get_tsf_systime(hmac_vap);
    /* 清除link切换标记 */
    mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
}

OAL_STATIC void hmac_mld_vap_common_proc(hmac_vap_stru *hmac_vap, uint8_t mld_mode,
    hmac_mld_manage_stru *mld_vap_manage, uint8_t link_id)
{
    hmac_vap_set_mld_mode(hmac_vap, mld_mode);
    hmac_vap->mld_vap_link_info.mld_vap_manage = mld_vap_manage;
    hmac_vap->mld_vap_link_info.link_id = link_id;
}

static void hmac_ml_vap_init(hmac_vap_stru *hmac_vap, oal_net_device_stru *mld_net_dev, uint8_t mld_mode)
{
    hmac_mld_manage_stru  *mld_vap_manage = &g_hmac_mld_vap_manage;
    if (mld_vap_manage->in_use == OAL_TRUE) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{hmac_mld_vap_init::ml reuse mld mgm}");
        return;
    }
    memset_s(mld_vap_manage, sizeof(hmac_mld_manage_stru), 0, sizeof(hmac_mld_manage_stru));
    mld_vap_manage->mld_vap = hmac_vap;
    mld_vap_manage->mld_net_device = mld_net_dev;
    mld_vap_manage->in_use = OAL_TRUE;
    mld_vap_manage->mld_assoc_mode = g_mld_assoc_mode;
    mld_vap_manage->emlsr_cap.emlsr_support =
        (is_custom_open_emlsr_switch(&hmac_vap->st_vap_base_info) == OAL_TRUE) ? OAL_TRUE : OAL_FALSE;
    mld_vap_manage->emlsr_cap.emlsr_padding_delay = wlan_chip_get_emlsr_padding_delay();
    mld_vap_manage->emlsr_cap.emlsr_transition_delay = wlan_chip_get_emlsr_transation_delay();
    mld_vap_manage->emlsr_record_info.emlsr_state = EMLSR_CLOSED; // emlsr初始状态为close
    oal_spin_lock_init(&mld_vap_manage->ml_user_lock);
    oal_dlist_init_head(&(mld_vap_manage->ml_user_list_head));
    hmac_mld_vap_common_proc(hmac_vap, mld_mode, mld_vap_manage, 0xff);
}

static void hmac_link_vap_init(hmac_vap_stru *hmac_vap, oal_net_device_stru *mld_net_dev, uint8_t mld_mode)
{
    uint8_t link_id;
    mac_vap_stru *mld_mac_vap = NULL;
    hmac_vap_stru *hmac_mld_vap = NULL;
    hmac_mld_manage_stru  *mld_vap_manage = NULL;

    if (mld_net_dev == NULL) {
        return;
    }
    mld_mac_vap = oal_net_dev_priv(mld_net_dev);
    if (mld_mac_vap == NULL) {
        return;
    }
    hmac_mld_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mld_mac_vap->uc_vap_id);
    if (hmac_mld_vap == NULL) {
        return;
    }
    mld_vap_manage = hmac_mld_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage->mld_link_cnt >= WLAN_MAX_ML_LINK_NUM) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_link_vap_init::mld_link_cnt >= MAX[%d]}", mld_vap_manage->mld_link_cnt, WLAN_MAX_ML_LINK_NUM);
        return;
    }
    if (mld_mode == WLAN_ML_LINK_MODE) {
        link_id = MAC_TRX_LINK_ID_0;
    } else {
        link_id = MAC_TRX_LINK_ID_1;
    }
    hmac_mld_vap_common_proc(hmac_vap, mld_mode, mld_vap_manage, link_id);
    mld_vap_manage->link_vap[mld_vap_manage->mld_link_cnt] = hmac_vap;
    mld_vap_manage->mld_link_cnt++;
}

static void hmac_mld_proc_vap_init(hmac_vap_stru *hmac_vap, uint8_t mld_mode, oal_net_device_stru  *mld_net_dev)
{
    if (mld_mode == WLAN_ML_MODE) {
        hmac_ml_vap_init(hmac_vap, mld_net_dev, mld_mode);
    } else if (mld_mode == WLAN_LINK_MODE) {
        hmac_link_vap_init(hmac_vap, mld_net_dev, mld_mode);
    } else if (mld_mode == WLAN_ML_LINK_MODE) {
        hmac_ml_vap_init(hmac_vap, mld_net_dev, mld_mode);
        hmac_link_vap_init(hmac_vap, mld_net_dev, mld_mode);
    }
}

/* 函数功能：link切换流程需要检查是否mlsr模式、vap state和mld mode */
uint8_t hmac_mlsr_link_switch_check_vap_mld_state(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap)
{
    mac_vap_stru *cur_work_mac_vap = NULL;
    mac_vap_stru *next_work_mac_vap = NULL;

    if (cur_work_vap == NULL || next_work_vap == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_check_vap_mld_state::hmac_vap is null}");
        return OAL_FALSE;
    }

    cur_work_mac_vap = &cur_work_vap->st_vap_base_info;
    if (hmac_get_mld_assoc_mode(cur_work_vap) != MLD_ASSOC_MODE_MLSR) {
        oam_warning_log1(cur_work_mac_vap->uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_link_switch_check_vap_mld_state::mld_assoc_mode is [%d]}",
            hmac_get_mld_assoc_mode(cur_work_vap));
        return OAL_FALSE;
    }

    next_work_mac_vap = &next_work_vap->st_vap_base_info;
    if (cur_work_mac_vap->en_vap_state != MAC_VAP_STATE_UP ||
        next_work_mac_vap->en_vap_state != MAC_VAP_STATE_STA_LINK_DOWN) {
        oam_warning_log4(cur_work_mac_vap->uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_link_switch_check_vap_mld_state::vap[%d] state[%d], vap[%d] state[%d]}",
            cur_work_mac_vap->uc_vap_id, cur_work_mac_vap->en_vap_state,
            next_work_mac_vap->uc_vap_id, next_work_mac_vap->en_vap_state);
        return OAL_FALSE;
    }

    if (cur_work_vap->mld_vap_link_info.mld_mode != WLAN_ML_LINK_MODE ||
        next_work_vap->mld_vap_link_info.mld_mode != WLAN_LINK_MODE) {
        oam_warning_log4(cur_work_mac_vap->uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_link_switch_check_vap_mld_state::vap[%d] mld_mode[%d], vap[%d] mld_mode[%d]}",
            cur_work_mac_vap->uc_vap_id, cur_work_vap->mld_vap_link_info.mld_mode,
            next_work_mac_vap->uc_vap_id, next_work_vap->mld_vap_link_info.mld_mode);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

void hmac_mlsr_link_switch_wake_all_queues(hmac_vap_stru *hmac_vap)
{
    oal_net_device_stru *netdev = NULL;

    if (hmac_vap == NULL) {
        return;
    }

    netdev = hmac_vap_get_net_device(hmac_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_wake_all_queues::netdev NULL}");
        return;
    }

    /* 异常场景，link切换post事件及去关联时，需恢复发送队列 */
    oal_net_tx_wake_all_queues(netdev);
    oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
        "{hmac_mlsr_link_switch_wake_all_queues::wake all queues succ}");
}

void hmac_mlsr_link_switch_deinit(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    if (hmac_vap == NULL) {
        return;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return;
    }

    if (mld_vap_manage->link_switch_record.is_link_switching == OAL_FALSE) {
        return;
    }
    mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;

    hmac_mlsr_link_switch_wake_all_queues(hmac_vap);
}

static void hmac_proc_mld_vap_deinit(hmac_vap_stru *hmac_vap, uint8_t mld_mode)
{
    uint8_t idx;
    hmac_mld_manage_stru   *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_proc_mld_vap_deinit::mld_vap_manage NULL}");
        return;
    }
    if (hmac_vap_is_ml_mode(hmac_vap)) {
        mld_vap_manage->mld_net_device = NULL;
        mld_vap_manage->in_use = OAL_FALSE;
    }

    hmac_mld_destroy_mld_related_timer(hmac_vap);
    hmac_vap_state_change_notify_emlsr(hmac_vap, NULL, COFEATURE_MLD_DEINIT_EVENT, OAL_FALSE);
    hmac_vap_set_mld_mode(hmac_vap, mld_mode);

    memset_s(&hmac_vap->mld_vap_link_info, sizeof(hmac_mld_vap_info_stru), 0, sizeof(hmac_mld_vap_info_stru));
    if (mld_vap_manage->mld_link_cnt <= 0) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
            "{hmac_proc_mld_vap_deinit::mld_vap_manage->mld_link_cnt already zero}");
        return;
    }
    for (idx = 0; idx < mld_vap_manage->mld_link_cnt; idx++) {
        if (mld_vap_manage->link_vap[idx] == hmac_vap) {
            mld_vap_manage->link_vap[idx] = NULL;
            /* 如果不是最后一个vap，则把最后一个vap移动到这个位置，使得该数组是紧凑的 */
            if (idx < (mld_vap_manage->mld_link_cnt - 1)) {
                mld_vap_manage->link_vap[idx] = mld_vap_manage->link_vap[mld_vap_manage->mld_link_cnt - 1];
                break;
            }
        }
    }
    mld_vap_manage->mld_link_cnt--;
    /* mld deinit流程，清除link切换统计信息 */
    hmac_mlsr_link_switch_record_clear(mld_vap_manage);
    if (mld_vap_manage->mld_link_cnt == 0) {
        memset_s(mld_vap_manage, sizeof(hmac_mld_manage_stru), 0, sizeof(hmac_mld_manage_stru));
    }
}

void hmac_vap_legacy_to_ml_mode_change(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t mld_change_fsm_event)
{
    uint8_t new_mld_mode = WLAN_ML_MODE;
    mac_vap_mld_mode_change_stru change_param = {0};

    hmac_vap->mld_vap_link_info.work_flag = OAL_TRUE;
    hmac_ml_vap_init(hmac_vap, hmac_vap->pst_net_device, new_mld_mode);

    change_param.mld_vap_id = 0;
    change_param.work_flag = OAL_TRUE;
    change_param.mld_mode_change_event = MLD_CHANGE_EVENT_CHANGE_TO_MLD;
    change_param.new_mld_mode = new_mld_mode;
    change_param.link_id = 0xff;

    hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_CHANGE_VAP_MLD_MODE,
        sizeof(mac_vap_mld_mode_change_stru), (uint8_t *)&change_param);
    hmac_wlan_vap_emlsr_state_sync_event(hmac_vap->mld_vap_link_info.mld_vap_manage);
    dpe_wlan_vap_mld_assoc_mode_set(hmac_vap->st_vap_base_info.uc_vap_id, g_mld_assoc_mode);
}

void hmac_proc_vap_link_mode_change(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t new_mld_mode)
{
    uint8_t work_flag;   /* link是否工作 */
    mac_vap_mld_mode_change_stru change_param = {0};
    hmac_mld_manage_stru   *mld_vap_manage = NULL;
    hmac_vap_stru *hmac_mld_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mld_vap_id);
    if (hmac_mld_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_proc_vap_link_mode_change::mld vap is null}");
        return;
    }
    mld_vap_manage = hmac_mld_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_proc_vap_link_mode_change::mld vap manage is null}");
        return;
    }
    if (new_mld_mode == WLAN_ML_LINK_MODE) {
        work_flag = OAL_TRUE;
    } else {
        work_flag = mld_vap_manage->mld_assoc_mode == MLD_ASSOC_MODE_MLSR ? OAL_FALSE : OAL_TRUE;
    }
    hmac_vap->mld_vap_link_info.work_flag = work_flag;
    hmac_link_vap_init(hmac_vap, hmac_mld_vap->pst_net_device, new_mld_mode);

    change_param.mld_mode_change_event = MLD_CHANGE_EVENT_CHANGE_TO_LINK;
    change_param.mld_vap_id = mld_vap_id;
    change_param.work_flag = work_flag;
    change_param.new_mld_mode = new_mld_mode;
    change_param.link_id = hmac_vap->mld_vap_link_info.link_id;

    hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_CHANGE_VAP_MLD_MODE,
        sizeof(mac_vap_mld_mode_change_stru), (uint8_t *)&change_param);
}

void hmac_vap_legacy_to_link_mode_change(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t mld_change_fsm_event)
{
    uint8_t new_mld_mode = WLAN_LINK_MODE;

    hmac_proc_vap_link_mode_change(hmac_vap, mld_vap_id, new_mld_mode);
}

void hmac_vap_ml_to_mpl_mode_change(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t mld_change_fsm_event)
{
    uint8_t new_mld_mode = WLAN_ML_LINK_MODE;

    hmac_proc_vap_link_mode_change(hmac_vap, mld_vap_id, new_mld_mode);
}

void hmac_vap_ml_to_legacy_mode_change(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t mld_change_fsm_event)
{
    uint8_t new_mld_mode = WLAN_LEGACY_MLD_MODE;
    mac_vap_mld_mode_change_stru change_param = {0};

    change_param.mld_vap_id = 0;
    change_param.work_flag = OAL_FALSE;
    change_param.new_mld_mode = new_mld_mode;
    change_param.link_id = 0xff;
    change_param.mld_mode_change_event = MLD_CHANGE_EVENT_CHANGE_TO_LEGACY;

    hmac_vap->mld_vap_link_info.work_flag = OAL_FALSE;
    hmac_proc_mld_vap_deinit(hmac_vap, new_mld_mode);
    hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_CHANGE_VAP_MLD_MODE,
        sizeof(mac_vap_mld_mode_change_stru), (uint8_t *)&change_param);
}

void hmac_mld_vap_mode_change_null(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t mld_change_fsm_event)
{
    uint8_t cur_mld_mode = hmac_vap_get_mld_mode(hmac_vap);
    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{hmac_mld_vap_mode_change_null::%d mode event%d no func",
        cur_mld_mode, mld_change_fsm_event);
}

typedef void (*hmac_mld_vap_mode_change)(hmac_vap_stru *hmac_vap, uint8_t mld_vap_id, uint8_t mld_change_fsm_event);
OAL_STATIC hmac_mld_vap_mode_change g_hmac_mld_vap_mode_change_func[WLAN_MLD_MODE_BUTT][MLD_CHANGE_FSM_EVENT_BUTT] = {
    [WLAN_LEGACY_MLD_MODE] =
         /* CHANGE_TO_LEGACY                    CHANGE_TO_MLD                        CHANGE_TO_LINK */
        {hmac_mld_vap_mode_change_null, hmac_vap_legacy_to_ml_mode_change, hmac_vap_legacy_to_link_mode_change},
    [WLAN_ML_MODE] =
        {hmac_vap_ml_to_legacy_mode_change, hmac_mld_vap_mode_change_null, hmac_vap_ml_to_mpl_mode_change},
    [WLAN_LINK_MODE] =
        {hmac_vap_ml_to_legacy_mode_change, hmac_mld_vap_mode_change_null, hmac_mld_vap_mode_change_null},
    [WLAN_ML_LINK_MODE] =
        {hmac_vap_ml_to_legacy_mode_change, hmac_mld_vap_mode_change_null, hmac_mld_vap_mode_change_null},
    };

void hmac_vap_mld_mode_change(hmac_vap_stru *hmac_vap, mld_change_fsm_event_type_enum mld_event, uint8_t mld_vap_id)
{
    uint8_t new_mld_mode;
    uint8_t cur_mld_mode = hmac_vap_get_mld_mode(hmac_vap);
    g_hmac_mld_vap_mode_change_func[cur_mld_mode][mld_event](hmac_vap, mld_vap_id, mld_event);
    new_mld_mode = hmac_vap_get_mld_mode(hmac_vap);
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{hmac_vap_mld_mode_change::[%d]->[%d] mode event[%d]", cur_mld_mode, new_mld_mode, mld_event);
}

void hmac_vap_mld_init(hmac_vap_stru *hmac_vap, mac_cfg_add_vap_param_stru *param)
{
    hmac_mld_manage_stru           *mld_vap_manage;
    oal_net_device_stru            *mld_net_dev;
    mac_cfg_add_mld_vap_param_stru *add_mld_vap_param = &param->mld_vap_param;
    uint8_t                         mld_mode = add_mld_vap_param->mld_mode;

    if (mld_mode == WLAN_LEGACY_MLD_MODE || mld_mode >= WLAN_MLD_MODE_BUTT) {
        return;
    }
    mld_net_dev = param->mld_net_device;
    hmac_mld_proc_vap_init(hmac_vap, mld_mode, mld_net_dev);

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL || mld_vap_manage->mld_vap == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG, "{hmac_vap_mld_init::mld init fail}");
        return;
    }
    add_mld_vap_param->link_id = hmac_vap->mld_vap_link_info.link_id;
    /* 保存mld vap id供device link->mld的绑定 */
    add_mld_vap_param->mld_vap_id = mld_vap_manage->mld_vap->st_vap_base_info.uc_vap_id;
}

void hmac_mld_vap_deinit(hmac_vap_stru *hmac_vap)
{
    uint8_t mld_mode = hmac_vap_get_mld_mode(hmac_vap);
    if (mld_mode == WLAN_LEGACY_MLD_MODE) {
        return;
    }
    hmac_proc_mld_vap_deinit(hmac_vap, mld_mode);
}

void hmac_config_sta_h2d_mld_params_update(mac_vap_stru *mac_vap, hmac_mld_manage_stru *mld_vap_manage)
{
    h2d_mld_vap_param_stru h2d_mld_vap_param = {{0}};

    memcpy_s(h2d_mld_vap_param.mld_addr, WLAN_MAC_ADDR_LEN,
        mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN);
    memcpy_s(h2d_mld_vap_param.mld_bssid, WLAN_MAC_ADDR_LEN,
        mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN);

    /* mld vap manage下保存对端AP MLD支持的最大链路个数 */
    h2d_mld_vap_param.ap_link_cnt = mld_vap_manage->ap_link_cnt;
    h2d_mld_vap_param.mld_assoc_mode = mld_vap_manage->mld_assoc_mode;

    /* 抛事件到dmac，设置dmac_mld_vap_manage下的mld_addr */
    hmac_config_send_event(mac_vap, WLAN_CFGID_SET_MLD_VAP_PARAM,
        sizeof(h2d_mld_vap_param_stru), (uint8_t *)&h2d_mld_vap_param);
    dpe_wlan_vap_mld_assoc_mode_set(mac_vap->uc_vap_id, mld_vap_manage->mld_assoc_mode);
    dpe_wlan_vap_mld_addr_set(mac_vap->uc_vap_id, mld_vap_manage->mld_addr);
}

void hmac_config_ap_h2d_mld_params_update(mac_vap_stru *mac_vap, hmac_mld_manage_stru *mld_vap_manage)
{
    h2d_mld_vap_param_stru h2d_mld_vap_param = {{0}};

    h2d_mld_vap_param.mld_assoc_mode = MLD_ASSOC_MODE_MLSR;
    memcpy_s(&h2d_mld_vap_param.mld_addr, WLAN_MAC_ADDR_LEN,
        mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN);
    memcpy_s(&h2d_mld_vap_param.mld_bssid, WLAN_MAC_ADDR_LEN,
        mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN);

    /* 抛事件到dmac，设置dmac_mld_vap_manage下的mld_addr */
    hmac_config_send_event(mac_vap, WLAN_CFGID_SET_MLD_VAP_PARAM,
        sizeof(h2d_mld_vap_param_stru), (uint8_t *)&h2d_mld_vap_param);
    dpe_wlan_vap_mld_assoc_mode_set(mac_vap->uc_vap_id, mld_vap_manage->mld_assoc_mode);
    dpe_wlan_vap_mld_addr_set(mac_vap->uc_vap_id, mld_vap_manage->mld_addr);
}

void hmac_mld_destroy_mld_related_timer(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    if (hmac_vap == NULL) {
        return;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return;
    }

    if (mld_vap_manage->link_switch_record.link_switch_wait_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(mld_vap_manage->link_switch_record.link_switch_wait_timer));
    }

    /* 去关联流程删除emlsr退出的omn重传定时器 */
    if (mld_vap_manage->emlsr_record_info.omn_retry_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(mld_vap_manage->emlsr_record_info.omn_retry_timer));
    }

    /* 去关联流程删除emlsr退出的omn重传定时器 */
    if (mld_vap_manage->emlsr_record_info.emlsr_open_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(mld_vap_manage->emlsr_record_info.emlsr_open_timer));
    }
}

/* 函数功能：检查当前是否有vap处于入网流程中，不允许mlsr link切换或emlsr进入 */
uint32_t hmac_check_all_vap_state_allow_mld_feature(mac_vap_stru *mac_vap)
{
    mac_device_stru *mac_device = NULL;
    hmac_vap_stru *temp_hmac_vap = NULL;
    mac_vap_stru *temp_vap = NULL;
    uint8_t vap_num;
    uint8_t vap_idx;

    /*
     * 2.1 检查其他vap 状态，判断输入事件优先级是否比vap 状态优先级高
     * 如果输入事件优先级高，则可以执行输入事件
     */
    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == NULL) {
        oam_warning_log1(0, OAM_SF_CFG,
            "{hmac_mlsr_link_switch_check_all_vap_state::mac_res_get_dev fail.device_id = %u}", mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    vap_num = mac_device->uc_vap_num;

    for (vap_idx = 0; vap_idx < vap_num; vap_idx++) {
        temp_hmac_vap = mac_res_get_hmac_vap(mac_device->auc_vap_id[vap_idx]);
        if (temp_hmac_vap == NULL) {
            continue;
        }

        temp_vap = &temp_hmac_vap->st_vap_base_info;
        if (temp_vap == NULL) {
            oam_warning_log1(0, OAM_SF_CFG,
                "{hmac_mlsr_link_switch_check_all_vap_state::get_mac_vap fail.vap_idx = %u}",
                mac_device->auc_vap_id[vap_idx]);
            continue;
        }

        /* 当GO或者AP在进行关联时，不进行扫描业务 */
        if (hmac_sta_is_connecting(temp_vap->en_vap_state) ||
            (hmac_enter_state_check_vap_connect_state(temp_vap, 0, 0) != OAL_FALSE) ||
            (hmac_check_roam_main_state_is_roaming(temp_hmac_vap) == OAL_TRUE)) {
            oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_MLD,
                "{hmac_check_all_vap_state_allow_mld_feature::vap[%d]state[%d] is connecting}",
                temp_vap->uc_vap_id, temp_vap->en_vap_state);
            return OAL_ERR_CODE_CONFIG_BUSY;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC uint8_t *hmac_mld_get_mld_macaddr(hmac_mld_manage_stru *mld_vap_manage)
{
    return oal_netdevice_mac_addr(mld_vap_manage->mld_net_device);
}


void hmac_vap_update_mld_param(mac_vap_stru *mac_vap,
    hmac_mld_manage_stru *mld_vap_manage, mac_bss_dscr_stru *bss_dscr)
{
    if (bss_dscr != NULL) {
        /* 设置mld vap manage下的mld_addr mld_bssid */
        memcpy_s(mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN,
            hmac_mld_get_mld_macaddr(mld_vap_manage), WLAN_MAC_ADDR_LEN);
        memcpy_s(mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN,
            bss_dscr->ml_info.ml_mac_addr, WLAN_MAC_ADDR_LEN);
        /* mld vap manage下保存对端AP MLD支持的最大链路个数 */
        mld_vap_manage->ap_link_cnt = bss_dscr->ml_info.ml_link_num;
        hmac_config_sta_h2d_mld_params_update(mac_vap, mld_vap_manage);
    } else {
        /* 设置mld vap manage下的mld_addr mld_bssid */
        memcpy_s(mld_vap_manage->mld_addr, WLAN_MAC_ADDR_LEN,
            mac_mib_get_StationID(mac_vap), WLAN_MAC_ADDR_LEN);
        memcpy_s(mld_vap_manage->mld_bssid, WLAN_MAC_ADDR_LEN,
            mac_mib_get_StationID(mac_vap), WLAN_MAC_ADDR_LEN);
        hmac_config_ap_h2d_mld_params_update(mac_vap, mld_vap_manage);
    }
}

void hmac_change_other_link_vap_mode(hmac_vap_stru *hmac_vap, uint32_t ml_link_num,
    mld_change_fsm_event_type_enum mld_change_event)
{
    uint8_t idx;
    oal_netdev_priv_stru *link_net_dev_priv;
    oal_net_device_stru *link_netdev;
    mac_vap_stru *link_mac_vap;
    hmac_vap_stru *link_hmac_vap = NULL;
    oal_net_device_stru *net_dev = hmac_vap->pst_net_device;
    oal_netdev_priv_stru *net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(net_dev);
    oal_net_device_stru *ml_net_dev = NULL;
    oal_netdev_priv_stru *ml_net_dev_priv = NULL;

    if (net_dev_priv == NULL) {
        return;
    }
    ml_net_dev = net_dev_priv->mlo_cfg_info.ml_netdev;
    if (ml_net_dev == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "hmac_change_other_link_vap_mode::ml_net_dev is NULL");
        return;
    }
    ml_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(ml_net_dev);
    if (ml_net_dev_priv == NULL) {
        return;
    }

    for (idx = 0; idx < oal_min(WLAN_MAX_ML_LINK_NUM, ml_link_num); idx++) {
        link_netdev = ml_net_dev_priv->mlo_cfg_info.link_netdev[idx];
        if (link_netdev == NULL) {
            continue;
        }
        link_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);
        if (link_net_dev_priv->mlo_cfg_info.bind_with_ml_vap == OAL_TRUE) {
            link_mac_vap = oal_net_dev_priv(link_net_dev_priv->mlo_cfg_info.ml_netdev);
        } else {
            /* 对端AP只支持一个link 或 定制化只使能一个link，则不配置另一个vap的mld mode */
            if (hmac_mld_enable_single_link() == OAL_TRUE) {
                link_mac_vap = NULL;
            } else {
                link_mac_vap = oal_net_dev_priv(link_netdev);
            }
        }
        if (link_mac_vap == NULL || link_mac_vap->uc_vap_id == hmac_vap->st_vap_base_info.uc_vap_id) {
            continue;
        }
        link_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(link_mac_vap->uc_vap_id);
        if (link_hmac_vap != NULL) {
            hmac_vap_mld_mode_change(link_hmac_vap, mld_change_event, hmac_vap->st_vap_base_info.uc_vap_id);
        }
    }
}

void hmac_change_vap_mld_mode(hmac_vap_stru *hmac_vap, mld_change_fsm_event_type_enum mld_change_event)
{
    if (mld_change_event == MLD_CHANGE_EVENT_CHANGE_TO_MLD) {
        hmac_vap_mld_mode_change(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_MLD, hmac_vap->st_vap_base_info.uc_vap_id);
        hmac_vap_mld_mode_change(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_LINK, hmac_vap->st_vap_base_info.uc_vap_id);
    } else {
        hmac_vap_mld_mode_change(hmac_vap, mld_change_event, hmac_vap->st_vap_base_info.uc_vap_id);
    }
}

void hmac_change_all_vap_to_mld_mode(hmac_vap_stru *hmac_vap, uint8_t ml_link_num)
{
    hmac_change_vap_mld_mode(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_MLD);
    if (ml_link_num > WLAN_ML_SINGLE_RADIO) {
        hmac_change_other_link_vap_mode(hmac_vap, ml_link_num, MLD_CHANGE_EVENT_CHANGE_TO_LINK);
    }
}

void hmac_change_all_vap_to_legacy_mode(hmac_vap_stru *hmac_vap, uint8_t ml_link_num)
{
    hmac_change_vap_mld_mode(hmac_vap, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    if (ml_link_num > WLAN_ML_SINGLE_RADIO) {
        hmac_change_other_link_vap_mode(hmac_vap, ml_link_num, MLD_CHANGE_EVENT_CHANGE_TO_LEGACY);
    }
}

uint8_t hmac_mld_connect_reset_link_cnt(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    ml_link_info_stru *mld_link_info = NULL;
    uint8_t connect_link_cnt = 0;
    if (bss_dscr == NULL) {
        return connect_link_cnt;
    }
    connect_link_cnt = bss_dscr->ml_info.ml_link_num;

    if (hmac_mld_enable_single_link() == OAL_TRUE ||
        hmac_check_link_bss_exist(hmac_vap, bss_dscr) == OAL_FALSE) {
        /* 找不到其他link的扫描结果，只关联1个link */
        connect_link_cnt = 1;
        return connect_link_cnt;
    }

    mld_link_info = hmac_get_bss_mld_link_info(hmac_vap, bss_dscr);
    if (mld_link_info == NULL) {
        return connect_link_cnt;
    }
    if (g_mld_assoc_mode == MLD_ASSOC_MODE_STR &&
        mld_link_info->st_channel.en_band == bss_dscr->st_channel.en_band) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_mld_connect_reset_link_cnt:: 2link is same band, str -> mlsr");
        g_mld_assoc_mode = MLD_ASSOC_MODE_MLSR;
    }

    return connect_link_cnt;
}

void hmac_vap_change_to_mld_process(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *bss_dscr)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint8_t real_connect_link_cnt;
    uint8_t ml_link_num = WLAN_ML_SINGLE_RADIO;

    if (hmac_vap == NULL) {
        return;
    }
    if (bss_dscr != NULL) {
        real_connect_link_cnt = hmac_mld_connect_reset_link_cnt(hmac_vap, bss_dscr);
        ml_link_num = oal_min(real_connect_link_cnt, bss_dscr->ml_info.ml_link_num);
    }
    hmac_change_all_vap_to_mld_mode(hmac_vap, ml_link_num);

    /* 对端mld mac地址存在mld vap manage下 */
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_vap_mode_change_process::mld_vap_mgmt is null");
        return;
    }

    /* 根据扫描结果，更新mld vap manage下的参数 */
    hmac_vap_update_mld_param(&hmac_vap->st_vap_base_info, mld_vap_manage, bss_dscr);
}

hmac_vap_stru *hmac_mlsr_get_cur_work_link_vap(hmac_mld_manage_stru *mld_vap_manage)
{
    hmac_vap_stru *link_vap = NULL;
    uint8_t link_id;

    for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
        link_vap = mld_vap_manage->link_vap[link_id];
        if (link_vap == NULL) {
            continue;
        }
        if (link_vap->mld_vap_link_info.work_flag == 1) {
            return link_vap;
        }
    }

    return NULL;
}

void hmac_change_tid_ba_info(uint16_t cur_work_user_idx, uint16_t next_work_user_idx)
{
    uint8_t tid;
    hmac_ba_stru *tid_ba = NULL; /* dpe和hmac分开，封装函数 */
    hmac_user_stru *next_work_user = mac_res_get_hmac_user(next_work_user_idx);
    if (next_work_user == NULL) {
        return;
    }
    for (tid = 0; tid < WLAN_TID_MAX_NUM; tid++) {
        tid_ba = hmac_get_ba_info(next_work_user, tid);
        if (tid_ba == NULL) {
            continue;
        }

        tid_ba->st_ba_tx_info.st_alarm_data.us_mac_user_idx = next_work_user_idx;
        tid_ba->st_ba_tx_info.st_alarm_data.uc_vap_id = next_work_user->st_user_base_info.uc_vap_id;
    }
}

void hmac_mld_change_vap_user_mld_param(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    hmac_mld_manage_stru *mld_vap_manage)
{
    hmac_user_stru *cur_work_user = NULL;
    hmac_user_stru *next_work_user = NULL;
    hmac_mld_user_manage_stru    *mld_user_manage = NULL;

    if (next_work_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mld_change_vap_user_mld_param::next_work_vap null");
        return;
    }
    /* 切换vap的mld参数 */
    cur_work_vap->mld_vap_link_info.work_flag = 0;
    hmac_vap_set_mld_mode(cur_work_vap, WLAN_LINK_MODE);
    next_work_vap->mld_vap_link_info.work_flag = 1;
    hmac_vap_set_mld_mode(next_work_vap, WLAN_ML_LINK_MODE);

    /* 切换user的mld参数 */
    cur_work_user = mac_res_get_hmac_user(cur_work_vap->st_vap_base_info.us_assoc_vap_id);
    next_work_user = mac_res_get_hmac_user(next_work_vap->st_vap_base_info.us_assoc_vap_id);
    if (cur_work_user == NULL || next_work_user == NULL) {
        return;
    }
    cur_work_user->mld_user_link_info.mld_mode = WLAN_LINK_MODE;
    next_work_user->mld_user_link_info.mld_mode = WLAN_ML_LINK_MODE;

    hmac_change_tid_ba_info(cur_work_user->st_user_base_info.us_assoc_id,
        next_work_user->st_user_base_info.us_assoc_id);
    dpe_change_tid_ba_info(cur_work_user->st_user_base_info.us_assoc_id,
        next_work_user->st_user_base_info.us_assoc_id);

    /* 切换vap、user的mld参数 */
    mld_vap_manage->mld_vap = next_work_vap;
    mld_user_manage = next_work_user->mld_user_link_info.mld_user_manage;
    if (mld_user_manage != NULL) {
        mld_user_manage->ml_user_id = next_work_user->st_user_base_info.us_assoc_id;
    }
}

void hmac_mlsr_change_netdev_vap_relation(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap)
{
    oal_net_device_stru *netdev_master = NULL;
    oal_net_device_stru *netdev_link = NULL;
    mac_vap_stru *cur_vap = NULL;
    mac_vap_stru *next_vap = NULL;

    if (cur_work_vap == NULL || next_work_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_mlsr_change_netdev_vap_relation::hmac_vap is null!!");
        return;
    }

    cur_vap = &cur_work_vap->st_vap_base_info;
    next_vap = &next_work_vap->st_vap_base_info;
    netdev_master = cur_work_vap->pst_net_device;
    netdev_link = next_work_vap->pst_net_device;

    /* 如果当前next vap已经是UP的主link，那么不应该将wlan0 挂接到cur vap上 */
    if ((hmac_vap_is_ml_mode(cur_work_vap) == OAL_TRUE || hmac_vap_is_link_mode(next_work_vap) == OAL_TRUE) ||
        (cur_vap->en_vap_state == MAC_VAP_STATE_UP || next_vap->en_vap_state == MAC_VAP_STATE_STA_LINK_DOWN)) {
        oam_error_log4(cur_vap->uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_change_netdev_vap_relation::cur_vap mld_mode[%d] state[%d] next_vap mld_mode[%d] state[%d]",
            cur_work_vap->mld_vap_link_info.mld_mode, cur_vap->en_vap_state,
            next_work_vap->mld_vap_link_info.mld_mode, next_vap->en_vap_state);
        return;
    }

    oal_net_dev_priv(netdev_master) = next_vap;
    oal_net_dev_priv(netdev_link) = cur_vap;
    cur_work_vap->pst_net_device = netdev_link;
    next_work_vap->pst_net_device = netdev_master;
    dpe_wlan_mlsr_change_netdev_vap_relation(cur_work_vap->st_vap_base_info.uc_vap_id,
        next_work_vap->st_vap_base_info.uc_vap_id, netdev_link, netdev_master);
}

static uint32_t hmac_mld_post_link_switch(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    uint32_t ret;
    ml_link_switch_info_stru link_switch = {
        .curr_vap_id = cur_work_vap->st_vap_base_info.uc_vap_id,
        .new_vap_id = next_work_vap->st_vap_base_info.uc_vap_id,
        .reason = reason,
        .sub_reason = sub_reason,
    };
    ret = hmac_config_send_event(&(cur_work_vap->st_vap_base_info), WLAN_CFGID_MLSR_WROK_LINK_SWITCH,
        sizeof(link_switch), (uint8_t *)&link_switch);
    return ret;
}

void hmac_mlsr_link_switch_action(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    oal_net_device_stru *netdev, uint8_t reason, uint8_t sub_reason)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint32_t pedding_data = 0;
    hmac_user_stru *cur_work_user = NULL;

    if (oal_any_null_ptr3(cur_work_vap, next_work_vap, netdev)) {
        return;
    }

    mld_vap_manage = cur_work_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch::mld_vap_manage NULL}");
        return;
    }

    /* link切换开始 记录本次link切换信息 */
    if (hmac_mlsr_link_switch_start_record_proc(cur_work_vap) != OAL_SUCC) {
        return;
    }

    /* 进信道切换流程前停掉扫描 */
    hmac_config_scan_abort(&cur_work_vap->st_vap_base_info, sizeof(uint32_t), (uint8_t *)&pedding_data);
#ifdef _PRE_WLAN_FEATURE_11AX
    /* mlsr切换，退出omi */
    hmac_omi_force_close_process();
#endif

    cur_work_user = mac_res_get_hmac_user(cur_work_vap->st_vap_base_info.us_assoc_vap_id);

    hmac_vap_state_change_notify_feature(cur_work_vap, cur_work_user, COFEATURE_LINK_SWITCH_EVENT, COFEATURE_ENTER);

    oal_net_tx_stop_all_queues(netdev); /* 暂停发送队列 */

    hmac_mld_post_link_switch(cur_work_vap, next_work_vap, reason, sub_reason);

    mld_vap_manage->link_switch_record.link_switch_info.curr_vap_id = cur_work_vap->st_vap_base_info.uc_vap_id;
    mld_vap_manage->link_switch_record.link_switch_info.new_vap_id = next_work_vap->st_vap_base_info.uc_vap_id;
    mld_vap_manage->link_switch_record.link_switch_info.reason = reason;
    mld_vap_manage->link_switch_record.link_switch_info.sub_reason = sub_reason;
    frw_timer_create_timer_m(&(mld_vap_manage->link_switch_record.link_switch_wait_timer),
        hmac_mlsr_link_switch_timeout, ML_LINK_SWITCH_TIMEOUT,
        (void *)&mld_vap_manage->link_switch_record.link_switch_info,
        OAL_FALSE, OAM_MODULE_ID_HMAC, cur_work_vap->st_vap_base_info.core_id);
}

void hmac_mlsr_link_switch_post_action(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    hmac_mld_manage_stru *mld_vap_manage)
{
    oal_net_device_stru *netdev = NULL;

    netdev = hmac_vap_get_net_device(cur_work_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_proc::netdev NULL}");
        return;
    }

    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_proc::mld_vap_manage NULL}");
        /* null睡眠帧发送失败，device未执行link切换，只恢复发送队列 */
        oal_net_tx_wake_all_queues(netdev);
        return;
    }

    /* 允许link 切换，下发vap 的全带宽校准数据 */
    hmac_cali_send_work_channel_cali_data(&next_work_vap->st_vap_base_info,
        &next_work_vap->st_vap_base_info.st_channel, CALI_DATA_NORMAL_JOIN_TYPE);

    /* 切换vap、user的mld参数 */
    hmac_mld_change_vap_user_mld_param(cur_work_vap, next_work_vap, mld_vap_manage);
    /* 配置之前work vap state LINK_DOWN */
    mac_vap_state_change(&cur_work_vap->st_vap_base_info, MAC_VAP_STATE_STA_LINK_DOWN);
    /* 配置当前work vap state UP */
    mac_vap_state_change(&next_work_vap->st_vap_base_info, MAC_VAP_STATE_UP);
    /* 交换netdev<--->vap指针关系 */
    hmac_mlsr_change_netdev_vap_relation(cur_work_vap, next_work_vap);
    /* 恢复发送队列 */
    oal_net_tx_wake_all_queues(netdev);
    /* 记录更新link切换后的统计信息及标记 */
    hmac_mlsr_link_switch_complete_record_proc(cur_work_vap, mld_vap_manage);
    /* 通知上层mlsr工作link变化 */
    hmac_mld_work_link_change_notify(next_work_vap);
}

uint32_t hmac_config_mlsr_link_switch(hmac_vap_stru *hmac_vap, uint8_t *param)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_vap_stru *next_work_vap = NULL;
    hmac_vap_stru *cur_work_vap = NULL;
    mac_link_switch_param_stru *link_switch_param = (mac_link_switch_param_stru *)param;
    uint8_t work_link_id = *param;
    uint32_t ret = OAL_SUCC;

    /* 非MLD或预期工作link的link id不合法，不执行切换 */
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE ||
        link_switch_param->enable_link_id >= WLAN_MAX_ML_LINK_NUM ||
        link_switch_param->trigger_reason > LINK_SWITCH_FOR_IOCTL) {
        oam_error_log1(0, OAM_SF_CFG, "{hmac_config_mlsr_link_switch::not mld or link_id[%d] invalid}", work_link_id);
        return ret;
    }

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_config_mlsr_link_switch::mld_vap_manage NULL}");
        return ret;
    }

    /* 获取当前工作link的vap */
    cur_work_vap = hmac_mlsr_get_cur_work_link_vap(mld_vap_manage);
    if (cur_work_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_config_mlsr_link_switch::no link work!}");
        return ret;
    }

    /* 获取预期工作link的vap */
    next_work_vap = mld_vap_manage->link_vap[link_switch_param->enable_link_id];
    if (next_work_vap == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_config_mlsr_link_switch::next_work_vap is null!}");
        return ret;
    }

    return hmac_mlsr_link_switch(cur_work_vap, next_work_vap, link_switch_param->trigger_reason, 0);
}

uint32_t hmac_config_al_rx_set_link_vap_param(oal_net_device_stru *net_dev, uint16_t len, uint8_t *param)
{
    oal_netdev_priv_stru *ml_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(net_dev);
    oal_netdev_priv_stru *link_net_dev_priv = NULL;
    oal_net_device_stru *link_netdev = NULL;
    mac_vap_stru *link_mac_vap = NULL;
    hmac_vap_stru *link_hmac_vap = NULL;
    uint8_t idx;
    uint32_t ret = OAL_SUCC;

    for (idx = 0; idx < WLAN_MAX_ML_LINK_NUM; idx++) {
        link_netdev = ml_net_dev_priv->mlo_cfg_info.link_netdev[idx];
        if (link_netdev == NULL) {
            continue;
        }
        link_net_dev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(link_netdev);
        if (link_net_dev_priv->mlo_cfg_info.bind_with_ml_vap == OAL_TRUE) {
            continue;
        }

        link_mac_vap = oal_net_dev_priv(link_netdev);
        if (link_mac_vap == NULL) {
            continue;
        }
        link_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(link_mac_vap->uc_vap_id);
        if (link_hmac_vap == NULL) {
            oam_error_log0(0, OAM_SF_CFG, "{hmac_config_al_rx_set_link_vap_param::link vap not add");
            return OAL_ERR_CODE_PTR_NULL;
        }

        ret = hmac_config_send_event(link_mac_vap, WLAN_CFGID_SET_LINK_VAP_ALWAYS_RX, len, param);
        if (oal_unlikely(ret != OAL_SUCC)) {
            oam_warning_log1(link_mac_vap->uc_vap_id, OAM_SF_CFG,
                "{hmac_config_al_rx_set_link_vap_param::send event fail[%d].}", ret);
            return ret;
        }
    }

    return ret;
}

/* 功能描述：获取mld 连接的两路link_vap */
uint32_t hmac_mld_get_mlsr_link_sta(hmac_vap_stru **hmac_link_vap_work, hmac_vap_stru **hmac_link_vap_down)
{
    hmac_mld_manage_stru *mld_vap_manage = &g_hmac_mld_vap_manage;

    if (mld_vap_manage->mld_assoc_mode != MLD_ASSOC_MODE_MLSR) {
        return OAL_FAIL;
    }

    if (mld_vap_manage->in_use != OAL_TRUE) {
        return OAL_FAIL;
    }

    *hmac_link_vap_work = mld_vap_manage->mld_vap;
    if (*hmac_link_vap_work == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    *hmac_link_vap_down = hmac_mld_get_link_vap(*hmac_link_vap_work);
    if (*hmac_link_vap_down == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    return OAL_SUCC;
}

/* 功能描述：同步link 切换是否使能标志到device
 * 入参描述：value取值 OAL_TRUE:link 切换使能；OAL_FALSE:link 切换不使能;
            duration取值 0：无效值  非0：配合OAL_FALSE表示禁止切换一段时间
 */
void hmac_mld_sync_link_switch_cap(hmac_vap_stru *hmac_vap, oal_bool_enum_uint8 value, uint32_t duration)
{
    uint32_t ret;
    mlsr_switch_enable_stru cfg;
    cfg.enable = value;
    cfg.duration = duration;

    ret = hmac_config_send_event(&(hmac_vap->st_vap_base_info), WLAN_CFGID_MLD_LINK_SWITCH_ENABLE,
        sizeof(cfg), (uint8_t *)(&cfg));

    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
        "{hmac_mld_sync_link_switch_cap::enable_link_switch %d, duration %u ret %d.}", value, duration, ret);
}

void hmac_mld_update_link_switch_flag(void)
{
    uint32_t ret;
    hmac_vap_stru *hmac_link_vap_work = NULL;
    hmac_vap_stru *hmac_link_vap_down = NULL;
    hmac_vap_stru *hmac_cfg_vap = NULL;
    mac_device_stru *mac_device = NULL;
    oal_bool_enum_uint8 link_switch_enable;

    if (g_wlan_spec_cfg->feature_ml_is_supported != OAL_TRUE) {
        return;
    }

    mac_device = mac_res_get_mac_dev();
    if (mac_device == NULL) {
        oam_error_log0(0, OAM_SF_MLD,
            "{hmac_mld_update_link_switch_flag::get device failed.}");
        return;
    }

    ret = hmac_mld_get_mlsr_link_sta(&hmac_link_vap_work, &hmac_link_vap_down);
    if (ret != OAL_SUCC || hmac_link_vap_work->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) {
        hmac_cfg_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_device->uc_cfg_vap_id);
        if (hmac_cfg_vap == NULL) {
            oam_error_log1(0, OAM_SF_MLD,
                "{hmac_mld_update_link_switch_flag::find cfg vap failed. %d}",
                mac_device->uc_cfg_vap_id);
            return;
        }
        hmac_mld_sync_link_switch_cap(hmac_cfg_vap, OAL_FALSE, 0);
        return;
    }

    link_switch_enable = hmac_mld_link_switch_channel_valid(
        &(hmac_link_vap_work->st_vap_base_info), &(hmac_link_vap_down->st_vap_base_info));
    hmac_mld_sync_link_switch_cap(hmac_link_vap_work, link_switch_enable, 0);
}

void hmac_mld_update_state(hmac_vap_stru *hmac_vap, uint8_t vap_state, uint8_t old_vap_state)
{
    if (vap_state == MAC_VAP_STATE_UP ||
        (old_vap_state == MAC_VAP_STATE_UP &&
        (vap_state == MAC_VAP_STATE_STA_FAKE_UP || vap_state == MAC_VAP_STATE_INIT))) {
        /* 更新link 切换标志：
         * （1）ap 创建/删除
         * （2）sta 关联/断联
         */

        /* 计算link切换是否可使能，并通知device */
        hmac_mld_update_link_switch_flag();
    }
}

uint8_t *hmac_get_mld_bssid(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        /* 非MLD，mld bssid配置与bssid一致 */
        return hmac_vap->st_vap_base_info.auc_bssid;
    }

    return mld_vap_manage->mld_bssid;
}

uint8_t hmac_mld_get_link_num(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage;
    if (hmac_vap == NULL || hmac_vap_is_legacy_mld_mode(hmac_vap)) {
        return 0;
    }
    if (hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
            "{hmac_mld_get_link_num::mld_vap_manage is null");
        return MLD_ASSOC_MODE_MLSR;
    }
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;

    return mld_vap_manage->mld_link_cnt;
}

OAL_STATIC uint32_t hmac_mld_str_del_link_proc(hmac_vap_stru *hmac_vap, hmac_vap_stru *another_hmac_vap)
{
    oal_net_device_stru *netdev = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    if (hmac_vap == NULL || hmac_vap->mld_vap_link_info.mld_vap_manage == NULL) {
        return OAL_FAIL;
    }
    netdev = hmac_vap->pst_net_device;
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;

    if (another_hmac_vap == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mld_str_del_link_proc::another_hmac_vap null.}");
        return OAL_FAIL;
    }

    /* 暂停发送队列 */
    oal_net_tx_stop_all_queues(netdev);
    /* 更新vap user的mld参数 */
    hmac_mld_change_vap_user_mld_param(hmac_vap, another_hmac_vap, mld_vap_manage);
    /* 交换netdev<--->vap指针关系 */
    hmac_mlsr_change_netdev_vap_relation(hmac_vap, another_hmac_vap);
    /* 启动发送队列 */
    oal_net_tx_wake_all_queues(netdev);

    return OAL_SUCC;
}

uint32_t hmac_mld_delete_link(mac_vap_stru *mac_vap)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_vap_stru *another_hmac_vap = NULL;
    uint32_t ret = OAL_SUCC;

    if (mac_vap == NULL) {
        return OAL_FAIL;
    }
    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    hmac_user = mac_res_get_hmac_user(mac_vap->us_assoc_vap_id);

    if (hmac_vap == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_MLD, "{hmac_mld_delete_link::hamc_vap is null");
        return OAL_FAIL;
    }

    oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_MLD, "{hmac_mld_delete_link::delete link now!");
    another_hmac_vap = hmac_mld_get_another_vap(hmac_vap);
    if (another_hmac_vap == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_MLD, "{hmac_mld_delete_link::another_hmac_vap is null");
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_EMLSR
    /* 删除link前，先退出emlsr */
    hmac_vap_state_change_notify_emlsr(another_hmac_vap, NULL, COFEATURE_RECONFIG_EVENT, OAL_FALSE);
#endif

    if (hmac_vap_is_ml_mode(hmac_vap) == OAL_FALSE) {
        return hmac_user_del(mac_vap, hmac_user);
    }

    if (hmac_mld_get_link_num(hmac_vap) == 0) {
        return hmac_user_del(mac_vap, hmac_user);
    }

    if (hmac_get_mld_assoc_mode(hmac_vap) != MLD_ASSOC_MODE_MLSR) {
        ret = hmac_mld_str_del_link_proc(hmac_vap, another_hmac_vap);
    } else {
        ret = hmac_mlsr_link_switch(hmac_vap, another_hmac_vap, LINK_SWITCH_FOR_RECONFIG, 0);
    }

    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_MLD, "{hmac_mld_delete_link::ret[%d]", ret);
    return ret;
}

uint32_t hmac_config_set_mld_sta_pm_on(hmac_vap_stru *hmac_vap, mac_cfg_ps_mode_param_stru *ps_mode_param,
    uint8_t pm_en, uint16_t len, uint8_t *param)
{
    uint32_t ret = OAL_SUCC;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    mac_vap_stru *link_mac_vap = NULL;
    hmac_vap_stru *hmac_vap_tmp = NULL;
    uint8_t link_id;
    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;

    if (mld_vap_manage == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 若处于非STR模式，则只需要使能net dev对应的vap的低功耗使能 */
    if ((mld_vap_manage->mld_assoc_mode != MLD_ASSOC_MODE_STR) &&
        (mld_vap_manage->emlsr_record_info.emlsr_state != EMLSR_OPENED)) {
        return hmac_config_sync_set_sta_pm_on(&hmac_vap->st_vap_base_info, ps_mode_param, len, param);
    }

    /* 若处于STR模式，则遍历mld vap管理结构体下的link vap */
    for (link_id = 0; link_id < mld_vap_manage->mld_link_cnt; link_id++) {
        link_mac_vap = &(mld_vap_manage->link_vap[link_id]->st_vap_base_info);
        hmac_vap_tmp = mld_vap_manage->link_vap[link_id];
        if ((hmac_vap_tmp->st_ps_sw_timer.en_is_registerd == OAL_TRUE) &&
            (pm_en > MAC_STA_PM_SWITCH_OFF)) {
            frw_timer_immediate_destroy_timer_m(&(hmac_vap_tmp->st_ps_sw_timer));
        }
        ret = hmac_config_sync_set_sta_pm_on(link_mac_vap, ps_mode_param, len, param);
        if (ret != OAL_SUCC) {
            return ret;
        }
    }
    return ret;
}

void hmac_mld_update_vap_link_info(hmac_vap_stru *hmac_vap, ml_link_info_stru *ml_link_info)
{
    memcpy_s(&hmac_vap->mld_vap_link_info.link_ap_info, sizeof(ml_link_info_stru), ml_link_info,
        sizeof(ml_link_info_stru));
}

void hmac_vap_update_mld_params(hmac_vap_stru *hmac_vap, mac_bss_dscr_stru *cur_bss_dscr)
{
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        return;
    }
    hmac_vap_update_mld_param(&hmac_vap->st_vap_base_info, mld_vap_manage, cur_bss_dscr);
    hmac_mld_update_vap_link_info(hmac_vap, &cur_bss_dscr->ml_info.ml_link_info[0]);
}

/* 函数功能：null帧发送完成后，检查当前全部vap状态是否支持link切换 */
static uint8_t hmac_mlsr_check_allow_link_switch(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    ml_link_switch_post_event_stru *mlsr_event)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    mld_vap_manage = cur_work_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_check_allow_link_switch::mld_vap_manage NULL}");
        return OAL_FALSE;
    }

    if (mlsr_event->allow_link_switch == OAL_FALSE) {
        oam_warning_log0(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_link_switch::fail link_switch, null frame send fail}");
        /* 记录更新link切换后的标记 */
        mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
        /* null睡眠帧发送失败，device未执行link切换，只恢复发送队列 */
        return OAL_FALSE;
    }

    if (mld_vap_manage->link_switch_record.is_link_switching == OAL_FALSE) {
        oam_warning_log0(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_mlsr_check_allow_link_switch::not in link_switching, return}");
        return OAL_FALSE;
    }

    if (hmac_mlsr_link_switch_check_vap_mld_state(cur_work_vap, next_work_vap) != OAL_TRUE) {
        mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
        return OAL_FALSE;
    }

    /* 判断当前vap共存场景是否支持MLSR链路切换  区别 */
    if (hmac_mld_link_switch_channel_valid(
        &cur_work_vap->st_vap_base_info, &next_work_vap->st_vap_base_info) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_check_allow_link_switch::channel not valid!}");
        mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
        return OAL_FALSE;
    }

    if (hmac_mlsr_link_switch_check_all_vap_state(cur_work_vap, next_work_vap, mld_vap_manage) != OAL_SUCC) {
        mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

void hmac_mld_work_link_change_notify(hmac_vap_stru *hmac_vap)
{
    oal_net_device_stru *netdev = NULL;

    if (hmac_vap == NULL) {
        return;
    }

    netdev = hmac_vap_get_net_device(hmac_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mld_work_link_change_notify::netdev NULL}");
        return;
    }

    hmac_report_channel_switch(hmac_vap, &(hmac_vap->st_vap_base_info.st_channel));
#ifdef CONFIG_HISI_WIFI7_MLO
    oal_inform_assoc_link_id(netdev, LINK_SWITCH_EVENT);
#endif
}

#endif /* end of _PRE_WLAN_FEATURE_ML */

/* 函数功能：link切换算法 上报link_switch 事件处理 */
uint32_t hmac_mlsr_link_switch_event_process(frw_event_mem_stru *event_mem)
{
#ifdef _PRE_WLAN_FEATURE_ML
    frw_event_stru *event = NULL; /* 事件结构体 */
    ml_link_switch_info_stru *mlsr_event = NULL;
    hmac_vap_stru *cur_work_vap;
    hmac_vap_stru *next_work_vap;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    mlsr_event = (ml_link_switch_info_stru *)event->auc_event_data;
    cur_work_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mlsr_event->curr_vap_id);
    if (cur_work_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_event_process::cur_work_vap[%d] null.}",
            mlsr_event->curr_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    next_work_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mlsr_event->new_vap_id);
    if (next_work_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_event_process::cur_work_vap[%d] null.}",
            mlsr_event->new_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (cur_work_vap->ipaddr_obtained == OAL_FALSE || next_work_vap->ipaddr_obtained == OAL_FALSE) {
        oam_warning_log4(0, OAM_SF_MLD,
            "{hmac_mlsr_link_switch_event_process::refused.work vap id %d, ipaddr_obtained %d, \
            link vap id %d, ipaddr_obtained %d}",
            cur_work_vap->st_vap_base_info.uc_vap_id, cur_work_vap->ipaddr_obtained,
            next_work_vap->st_vap_base_info.uc_vap_id, next_work_vap->ipaddr_obtained);
        return OAL_ERR_CODE_ML_LINK_SWITCH_REFUSE;
    }

    return hmac_mlsr_link_switch(cur_work_vap, next_work_vap, mlsr_event->reason, mlsr_event->sub_reason);
#else
    return OAL_SUCC;
#endif
}

static uint32_t hmac_mlsr_link_switch_get_vap(ml_link_switch_info_stru *switch_info,
    hmac_vap_stru **cur_work_vap, hmac_vap_stru **next_work_vap)
{
    *cur_work_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(switch_info->curr_vap_id);
    if (*cur_work_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_post_event_process::cur_work_vap[%d] null.}",
            switch_info->curr_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    *next_work_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(switch_info->new_vap_id);
    if (*next_work_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_post_event_process::cur_work_vap[%d] null.}",
            switch_info->new_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    return OAL_SUCC;
}

/* 函数功能：link切换NULL帧发送完成（成功/失败/超时），上报host link切换事件后续处理
 * NULL_DATA发送成功，执行link 切换；NULL_DATA发送失败，不执行link切换
 */
uint32_t hmac_mlsr_link_switch_post_event_process(frw_event_mem_stru *event_mem)
{
#ifdef _PRE_WLAN_FEATURE_ML
    frw_event_stru *event = NULL; /* 事件结构体 */
    ml_link_switch_post_event_stru *mlsr_event = NULL;
    ml_link_switch_info_stru *switch_info = NULL;
    hmac_vap_stru *cur_work_vap = NULL;
    hmac_vap_stru *next_work_vap = NULL;
    hmac_user_stru *cur_work_user = NULL;
    uint32_t ret;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_post_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    mlsr_event = (ml_link_switch_post_event_stru *)event->auc_event_data;
    switch_info = &mlsr_event->switch_info;
    if (hmac_mlsr_link_switch_get_vap(switch_info, &cur_work_vap, &next_work_vap) != SUCC) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log2(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
        "{hmac_mlsr_link_switch_post_event_process::reason[%d] null_data_state[%d]}",
        switch_info->reason, mlsr_event->null_send_state);
    /* device通知host link切换的null帧发送成功，取消100ms等待定时器 */
    hmac_mld_destroy_mld_related_timer(cur_work_vap);

    cur_work_user = mac_res_get_hmac_user(cur_work_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_mlsr_check_allow_link_switch(cur_work_vap, next_work_vap, mlsr_event) == OAL_FALSE) {
        /* 抛事件到device通知不执行link切换 */
        hmac_mlsr_link_switch_post_sync_event(cur_work_vap, next_work_vap, LINK_SWITCH_NO_SWITCH, 0);
        hmac_mlsr_link_switch_wake_all_queues(cur_work_vap);
        /* link切换失败，通知wur */
        hmac_vap_state_change_notify_feature(cur_work_vap, cur_work_user,
            COFEATURE_LINK_SWITCH_EVENT, COFEATURE_ENTER_FAIL);

        if (cur_work_user && cur_work_user->is_recv_deauth_disassoc) {
            /* 接收到过deauth，上报上层应用断联 */
            hmac_proc_disasoc_inform(cur_work_vap, MAC_DEAUTH_LV_SS, OAL_TRUE);
        }
        return OAL_FAIL;
    }

    ret = hmac_mlsr_link_switch_post_proc(cur_work_vap, next_work_vap, switch_info->reason, switch_info->sub_reason);

    if (cur_work_user && cur_work_user->is_recv_deauth_disassoc) {
        /* 接收过deauth，执行漫游重关联 */
        if (hmac_mld_rx_deauth_do_reassoc(next_work_vap) != OAL_SUCC) {
            /* 漫游重关联失败，上报断联 */
            hmac_proc_disasoc_inform(next_work_vap, MAC_DEAUTH_LV_SS, OAL_TRUE);
        }
    }

    return ret;
#else
    return OAL_SUCC;
#endif
}
