/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明   : HMAC MLSR LINK SWITCH处理
 * 创建日期   : 2024年8月31日
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
#include "hmac_mgmt_classifier.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MLSR_LINK_SWITCH_C

#ifdef _PRE_WLAN_FEATURE_ML
#define MLSR_LINK_SWITCH_PROTECT_DURATION 60000 /* 静默60s */

typedef struct {
    uint8_t src_vap_id;
    uint8_t dst_vap_id;
    uint8_t reason;
    uint8_t sub_reason;
    uint32_t timestamp;
    uint32_t protect_duration;
} mlsr_link_record_stru;

/* 上一次成功切换的记录 */
mlsr_link_record_stru g_mlsr_link_switch_record = {0};

OAL_STATIC mlsr_link_record_stru *hmac_get_mlsr_link_record_entry(void)
{
    return &g_mlsr_link_switch_record;
}

/* 通知算法静默一段时间 */
OAL_STATIC uint32_t hmac_mlsr_link_switch_get_protect_duration(uint8_t reason)
{
    uint32_t duration_by_reason[LINK_SWITCH_REASON_BUTT] = {
        0,                                      /* LINK_SWITCH_FOR_HIPRIV */
        MLSR_LINK_SWITCH_PROTECT_DURATION,      /* LINK_SWITCH_FOR_IOCTL */
        0,                                      /* LINK_SWITCH_FOR_ALG */
        MLSR_LINK_SWITCH_PROTECT_DURATION,      /* LINK_SWITCH_FOR_RECONFIG */
        MLSR_LINK_SWITCH_PROTECT_DURATION,      /* LINK_SWITCH_FOR_TTLM */
        MLSR_LINK_SWITCH_PROTECT_DURATION,      /* LINK_SWITCH_FOR_ROAM */
    };

    if (reason >= LINK_SWITCH_REASON_BUTT) {
        return 0;
    }

    return duration_by_reason[reason];
}

/* 功能：记录本次切换的原因 */
OAL_STATIC void hmac_mlsr_link_switch_record(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    uint32_t cur_timestamp = oal_time_get_stamp_ms();
    mlsr_link_record_stru *record = hmac_get_mlsr_link_record_entry();

    record->src_vap_id = cur_work_vap->st_vap_base_info.uc_vap_id;
    record->dst_vap_id = next_work_vap->st_vap_base_info.uc_vap_id;
    record->reason = reason;
    record->sub_reason = sub_reason;
    record->timestamp = cur_timestamp;
    record->protect_duration = hmac_mlsr_link_switch_get_protect_duration(reason);
    /* 通知device 模块静默切换duration */
    if (record->protect_duration != 0) {
        hmac_mld_sync_link_switch_cap(cur_work_vap, OAL_FALSE, record->protect_duration);
    }
}

/* 功能：判断当前时间是否在上一次切换保护期内 */
OAL_STATIC oal_bool_enum_uint8 hmac_mlsr_link_switch_in_protect_duration(mlsr_link_record_stru *record)
{
    uint32_t cur_timestamp;
    if (record->protect_duration == 0) {
        return OAL_FALSE;
    }
    cur_timestamp = oal_time_get_stamp_ms();
    /* 此处uint32_t允许翻转减法，减出来的还是时间差 */
    if (cur_timestamp - record->timestamp < record->protect_duration) {
        return OAL_TRUE;
    }
    /* 一旦本次保护时间已过，record->protect_duration置0 */
    record->protect_duration = 0;
    return OAL_FALSE;
}

/* 功能：保护上一次的切换，防止pingpang切换 */
OAL_STATIC uint32_t hmac_mlsr_link_switch_arbitration(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    mlsr_link_record_stru *record = hmac_get_mlsr_link_record_entry();

    /* case1:上次由ALG切换，本次允许直接切换 */
    if (record->reason == LINK_SWITCH_FOR_ALG) {
        return OAL_SUCC;
    }

    /* case2:本次切换时ALG触发，上次切换有保护周期 */
    if (reason == LINK_SWITCH_FOR_ALG && hmac_mlsr_link_switch_in_protect_duration(record)) {
        oam_warning_log3(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_arbitration::last_reason[%u] time[%u] protect[%u]}",
            record->reason, record->timestamp, record->protect_duration);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t hmac_mlsr_link_switch_post_sync_event(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    uint32_t ret;
    ml_link_switch_info_stru link_switch = {
        .curr_vap_id = cur_work_vap->st_vap_base_info.uc_vap_id,
        .new_vap_id = next_work_vap->st_vap_base_info.uc_vap_id,
        .reason = reason,
        .sub_reason = sub_reason,
    };
    ret = hmac_config_send_event(&(cur_work_vap->st_vap_base_info), WLAN_CFGID_MLSR_WROK_LINK_SWITCH_POST_EVENT,
        sizeof(link_switch), (uint8_t *)&link_switch);

    return ret;
}

uint32_t hmac_mlsr_link_switch_timeout(void *arg)
{
    hmac_vap_stru *cur_work_vap = NULL;
    hmac_vap_stru *next_work_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    ml_link_switch_info_stru *link_switch_info = (ml_link_switch_info_stru *)arg;
    hmac_user_stru *cur_work_user = NULL;

    if (link_switch_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    cur_work_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(link_switch_info->curr_vap_id);
    if (cur_work_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_timeout::cur_work_vap[%d] null.}",
            link_switch_info->curr_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    next_work_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(link_switch_info->new_vap_id);
    if (next_work_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_mlsr_link_switch_timeout::next_work_vap[%d] null.}",
            link_switch_info->new_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_vap_manage = cur_work_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_timeout::mld_vap_manage NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_vap_manage->link_switch_record.is_link_switching = OAL_FALSE;
    hmac_mlsr_link_switch_wake_all_queues(cur_work_vap);
    /* 抛事件清除device标记 */
    hmac_mlsr_link_switch_post_sync_event(cur_work_vap, next_work_vap, LINK_SWITCH_TIMEOUT, 0);

    cur_work_user = mac_res_get_hmac_user(cur_work_vap->st_vap_base_info.us_assoc_vap_id);
    hmac_vap_state_change_notify_feature(cur_work_vap, cur_work_user,
        COFEATURE_LINK_SWITCH_EVENT, COFEATURE_ENTER_FAIL);

    oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_timeout::link_switch send null_frame timeout!}");

    if (cur_work_user && cur_work_user->is_recv_deauth_disassoc) {
        /* 接收到过deauth，上报上层应用断联 */
        hmac_proc_disasoc_inform(cur_work_vap, MAC_DEAUTH_LV_SS, OAL_TRUE);
    }
    return OAL_SUCC;
}

/* 函数功能：link切换触发前检查所有vap状态，有处于入网、漫游状态的vap则不执行切换 */
uint32_t hmac_mlsr_link_switch_check_all_vap_state(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    hmac_mld_manage_stru *mld_vap_manage)
{
    mac_vap_stru *mac_vap = &cur_work_vap->st_vap_base_info;

    if (oal_any_null_ptr3(cur_work_vap, next_work_vap, mld_vap_manage)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 要切换的2个vap，其vap state、mld_mode、mld工作模式等不正确，不执行link切换 */
    if (hmac_mlsr_link_switch_check_vap_mld_state(cur_work_vap, next_work_vap) != OAL_TRUE) {
        return OAL_FAIL;
    }

    /* 如果已经处于emlsr模式，不执行link切换 */
    if (mld_vap_manage->emlsr_record_info.emlsr_state >= EMLSR_OPENED &&
        mld_vap_manage->emlsr_record_info.emlsr_state <= EMLSR_WAIT_CLOSE) {
        oam_error_log0(0, OAM_SF_CFG,
            "{hmac_mlsr_link_switch_check_all_vap_state::link_switch is reject for emlsr}");
        return OAL_FAIL;
    }

    return hmac_check_all_vap_state_allow_mld_feature(mac_vap);
}

/* 功能：mlsr link切换处理函数 */
OAL_STATIC uint32_t hmac_mlsr_link_switch_proc(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    oal_net_device_stru *netdev = NULL;
    netdev = hmac_vap_get_net_device(cur_work_vap->st_vap_base_info.uc_vap_id);
    if (netdev == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch_proc::netdev NULL}");
        return OAL_FAIL;
    }

    /* 进行切换仲裁 */
    if (hmac_mlsr_link_switch_arbitration(cur_work_vap, next_work_vap, reason, sub_reason) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 执行切换动作 */
    hmac_mlsr_link_switch_action(cur_work_vap, next_work_vap, netdev, reason, sub_reason);

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_mlsr_link_switch_post_proc_for_mlo_feature(
    hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap, uint8_t reason)
{
    mac_vap_stru *mac_vap = &(cur_work_vap->st_vap_base_info);
    hmac_user_stru *cur_work_user = NULL;
    uint32_t ret = OAL_SUCC;

    cur_work_user = mac_res_get_hmac_user(mac_vap->us_assoc_vap_id);
    if (cur_work_user == NULL) {
        return OAL_FAIL;
    }

    switch (reason) {
        case LINK_SWITCH_FOR_RECONFIG:
            oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_MLD,
                "{hmac_mlsr_link_switch::reconfig del user}");
            ret = hmac_user_del(&cur_work_vap->st_vap_base_info, cur_work_user);
            break;

        default:
            break;
    }

    return ret;
}


static void hmac_mlsr_link_switch_send_cali_data(hmac_vap_stru *next_work_vap)
{
    uint32_t up_vap_num;
    uint8_t work_cali_data_sub_type;
    mac_device_stru *mac_device = mac_res_get_mac_dev();
    up_vap_num = mac_device_calc_up_vap_num(mac_device);
    if (up_vap_num == g_wlan_spec_cfg->max_work_vap_num) {
        /* link切换前，已经满规格vap工作，先下发vap的部分校准数据,避免device 切信道无校准数据，
         * 待device上报可link切换后，补发vap全部校准数据 */
        work_cali_data_sub_type = CALI_DATA_CSA_TYPE;
    } else {
        /* 非满规格VAP场景，下发vap需要的全部校准数据 */
        work_cali_data_sub_type = CALI_DATA_NORMAL_JOIN_TYPE;
    }
    hmac_cali_send_work_channel_cali_data(&next_work_vap->st_vap_base_info,
        &next_work_vap->st_vap_base_info.st_channel, work_cali_data_sub_type);
}

OAL_STATIC void hmac_mlsr_link_switch_ps_sw_timer_proc(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap)
{
    if (cur_work_vap->st_ps_sw_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(cur_work_vap->st_ps_sw_timer));
        frw_timer_create_timer_m(&(next_work_vap->st_ps_sw_timer), hmac_set_ipaddr_timeout,
            HMAC_SWITCH_STA_PSM_PERIOD, (void *)next_work_vap, OAL_FALSE,
            OAM_MODULE_ID_HMAC, next_work_vap->st_vap_base_info.core_id);
        next_work_vap->us_check_timer_pause_cnt = 0;
    }
}

uint32_t hmac_mlsr_link_switch_post_proc(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    hmac_user_stru *cur_work_user = NULL;

    /* 切换前需要先下发校准数据 */
    hmac_mlsr_link_switch_send_cali_data(next_work_vap);

    /* 切换前，把当前工作link的fastsleep 置为非工作模式 */
    hmac_fastsleep_stop(&cur_work_vap->st_vap_base_info);

    /* 抛事件到device执行link切换 */
    hmac_mlsr_link_switch_post_sync_event(cur_work_vap, next_work_vap, reason, sub_reason);

    /* 执行切换后的mld参数切换 */
    hmac_mlsr_link_switch_post_action(cur_work_vap, next_work_vap,
        cur_work_vap->mld_vap_link_info.mld_vap_manage);

    if (hmac_mlsr_link_switch_post_proc_for_mlo_feature(cur_work_vap, next_work_vap, reason) != OAL_SUCC) {
        oam_error_log0(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
            "{hmac_mlsr_link_switch::post proc for mlo feature failed!}");
    }

    /* 如果切之前的link开低功耗定时器还在，在切换后的link上重新启动该定时器 */
    hmac_mlsr_link_switch_ps_sw_timer_proc(cur_work_vap, next_work_vap);

    /* 记录切换原因 */
    hmac_mlsr_link_switch_record(cur_work_vap, next_work_vap, reason, sub_reason);

    cur_work_user = mac_res_get_hmac_user(cur_work_vap->st_vap_base_info.us_assoc_vap_id);
    hmac_vap_state_change_notify_feature(cur_work_vap, cur_work_user,
        COFEATURE_LINK_SWITCH_EVENT, COFEATURE_ENTER_SUCC);

    /* 如果正在进行长期同信道监听, link切换后检查能否继续同信道监听, 如果不行则abort监听 */
    hmac_abort_long_same_chan_remain_on_chan(&next_work_vap->st_vap_base_info, OAL_TRUE,
        MAC_ABORT_SAME_CHAN_REMAIN_ON_CHAN_LINK_SWITCH);

    oam_warning_log4(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
        "{hmac_mlsr_link_switch::succ link_switch,cur_work_vap[%d] link_id[%d], next_work_vap[%d] link_id[%d]}",
        cur_work_vap->st_vap_base_info.uc_vap_id, cur_work_vap->mld_vap_link_info.link_id,
        next_work_vap->st_vap_base_info.uc_vap_id, next_work_vap->mld_vap_link_info.link_id);
    oam_warning_log2(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
        "{hmac_mlsr_link_switch::succ link_switch,reason[%u] subreason[%u]}", reason, sub_reason);

    return OAL_SUCC;
}

/* 功能：mlsr link切换函数,供其他模块调用 */
uint32_t hmac_mlsr_link_switch(hmac_vap_stru *cur_work_vap, hmac_vap_stru *next_work_vap,
    uint8_t reason, uint8_t sub_reason)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    mac_vap_stru *cur_work_mac_vap = NULL;
    mac_vap_stru *next_work_mac_vap = NULL;

    if (oal_any_null_ptr3(cur_work_vap, next_work_vap, cur_work_vap->pst_net_device)) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch::param is null!}");
        return OAL_FAIL;
    }

    oam_warning_log2(cur_work_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLD,
        "{hmac_mlsr_link_switch::link_switch reason[%d] \
        (0:hipriv 1:ioctl 2:alg 3:reconfig 4:ttlm 5:roam) sub_reason[%u]}",
        reason, sub_reason);

    mld_vap_manage = cur_work_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch::mld_vap_manage NULL}");
        return OAL_FAIL;
    }

    if (hmac_mlsr_link_switch_check_all_vap_state(cur_work_vap, next_work_vap, mld_vap_manage) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 当前已有link切换正在执行过程中，不接收本次切换 */
    if (hmac_mlsr_is_link_switching() == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch::is link_switching now!!!}");
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    /* 如果预期工作link为当前工作link，不切换 */
    if (next_work_vap == hmac_mlsr_get_cur_work_link_vap(mld_vap_manage)) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_mlsr_link_switch::expect vap is cur_work_vap, not need switch}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    cur_work_mac_vap = &(cur_work_vap->st_vap_base_info);
    next_work_mac_vap = &(next_work_vap->st_vap_base_info);
    /* 仅STA模式支持MLSR链路切换 */
    if (is_legacy_sta(cur_work_mac_vap) != OAL_TRUE ||
        is_legacy_sta(next_work_mac_vap) != OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch::vap is not STA!}");
        return OAL_FAIL;
    }

    /* 判断当前vap共存场景是否支持MLSR链路切换  区别 */
    if (hmac_mld_link_switch_channel_valid(cur_work_mac_vap, next_work_mac_vap) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_MLD, "{hmac_mlsr_link_switch::channel not valid!}");
        return OAL_FAIL;
    }

    return hmac_mlsr_link_switch_proc(cur_work_vap, next_work_vap, reason, sub_reason);
}

uint32_t hmac_mld_link_switch_get_tsf_systime(hmac_vap_stru *hmac_vap)
{
    uint32_t cur_time = 0;

    hal_host_get_tsf_lo(hal_get_host_device(hmac_vap->hal_dev_id), hmac_vap->hal_vap_id, &cur_time);
    cur_time = cur_time >> 10;    // 右移10位转化为ms

    return cur_time;
}

uint8_t hmac_mld_rx_deauth_need_reassoc(hmac_vap_stru *hmac_vap)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    uint32_t cur_timestamp;

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        return OAL_FALSE;
    }

    mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);
    if (mld_vap_manage == NULL) {
        return OAL_FALSE;
    }

    /* 没有触发过link切换就收到deauth 或者 收deauth距离前一次link切换的时间超过1.5分钟，不重关联 */
    cur_timestamp = hmac_mld_link_switch_get_tsf_systime(hmac_vap);
    if (mld_vap_manage->link_switch_record.link_switch_res != OAL_TRUE ||
        cur_timestamp - mld_vap_manage->link_switch_record.link_switch_time_record >= ML_LINK_SWITCH_WAIT_DEAUTH_TIME) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}
#endif