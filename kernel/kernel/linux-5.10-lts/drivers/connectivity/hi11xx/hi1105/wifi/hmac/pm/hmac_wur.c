/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : wur特性
 * 作    者 : q30033932
 * 创建日期 : 2024年2月19日
 */

#include "hmac_wur.h"
#include "oal_ext_if.h"
#include "securec.h"
#include "mac_mib.h"
#include "mac_ie.h"
#include "mac_frame_wur_ie.h"
#include "mac_frame_wur.h"
#include "mac_user.h"
#include "mac_frame_inl.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_hcc_adapt.h"
#include "wlan_chip_i.h"
#include "hmac_config.h"
#ifdef _PRE_WLAN_FEATURE_STA_PM
#include "plat_pm_wlan.h"
#endif
#include "hmac_vap.h"
#include "hmac_mld_ext.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WUR_C

#ifdef _PRE_WLAN_FEATURE_WUR
uint32_t g_wur_disable_bitmap = 0x1; /* bitmap有置位不进入wur，wur_notify_event_enum */

uint32_t calc_crc32(uint8_t *data, uint16_t data_len_input)
{
    uint32_t crc_value = 0xffffffff; /* crc初始值全1 */
    uint32_t poly = 0xedb88320; /* crc32生成多项式0x04c11db7的逆bit序 */
    uint8_t value;
    uint8_t i;
    uint16_t data_len = data_len_input;
    while (data_len--) {
        value = *(data++);
        crc_value ^= value;
        /* 8个bit依次处理 */
        for (i = 0; i < 8; i++)  {
            crc_value = (crc_value >> 1) ^ ((crc_value & 0x1) ? poly : 0);
        }
    }
    return (~crc_value);
}

/*
 * 功能描述  : 判断wur参数是否已经协商过了，wur setup action帧交互完成后，置位协商完成。
 */
uint8_t hmac_wur_is_negotiated(hmac_vap_stru *hmac_vap)
{
    if (hmac_vap == NULL) {
        return OAL_FALSE;
    }
    return (wlan_chip_get_wur_switch() == OAL_TRUE && hmac_vap->wur_info.cfg_info.negotiated == OAL_TRUE);
}

OAL_STATIC uint32_t hmac_wur_check_bitmap(void)
{
    return (g_wur_disable_bitmap == 0) ? OAL_SUCC : OAL_FAIL;
}

OAL_STATIC void hmac_wur_set_enabled(hmac_vap_stru *hmac_vap, uint8_t val)
{
    hmac_vap->wur_info.is_enabled = val;
}

OAL_STATIC uint8_t hmac_wur_is_enabled(hmac_vap_stru *hmac_vap)
{
    return hmac_vap->wur_info.is_enabled;
}

OAL_STATIC void hmac_wur_set_wur_disable_bitmap_bit(uint8_t event)
{
    g_wur_disable_bitmap |= BIT(event);
}

OAL_STATIC void hmac_wur_clear_wur_disable_bitmap_bit(uint8_t event)
{
    g_wur_disable_bitmap &= ~BIT(event);
}

/* 找到协商过wur的sta所在hmac_vap */
hmac_vap_stru *hmac_find_up_wur_vap(void)
{
    uint8_t vap_idx;
    hmac_vap_stru *hmac_vap = NULL;
    mac_device_stru *mac_device = NULL;

    mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return NULL;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        hmac_vap = mac_res_get_hmac_vap(mac_device->auc_vap_id[vap_idx]);
        if (hmac_vap == NULL) {
            continue;
        }
        if (hmac_wur_is_negotiated(hmac_vap) == OAL_TRUE) {
            return hmac_vap;
        }
    }

    return NULL;
}

/* 找到wur enabled的所在up hmac_vap */
hmac_vap_stru *hmac_find_up_wur_enabled_vap(void)
{
    uint8_t vap_idx;
    hmac_vap_stru *hmac_vap = NULL;
    mac_device_stru *mac_device = NULL;

    mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return NULL;
    }

    for (vap_idx = 0; vap_idx < mac_device->uc_vap_num; vap_idx++) {
        hmac_vap = mac_res_get_hmac_vap(mac_device->auc_vap_id[vap_idx]);
        if (hmac_vap == NULL) {
            continue;
        }
        if (hmac_wur_is_enabled(hmac_vap) == OAL_TRUE &&
            mac_vap_is_up(&hmac_vap->st_vap_base_info) == OAL_TRUE) {
            return hmac_vap;
        }
    }

    return NULL;
}

OAL_STATIC void hmac_sta_wur_mode_param_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t is_always_on, uint8_t count, uint8_t ratio)
{
    mac_vap_stru *mac_vap = &hmac_vap->st_vap_base_info;
    uint32_t duty_cycle_period = 0;
    mac_wur_stru *wur_info = &(hmac_vap->wur_info);

    if (wur_info->duty_cycle_period_units == 0) {
        return;
    }
    if (is_always_on == OAL_TRUE) {
        wur_info->cfg_info.is_always_on = OAL_TRUE;
        wur_info->cfg_info.on_duration = 0;
        wur_info->duty_cycle_period_num = 0;
        wur_info->cfg_info.duty_cycle_period = 0;
    } else {
        wur_info->cfg_info.is_always_on = OAL_FALSE;
        duty_cycle_period = (mac_mib_get_BeaconPeriod(mac_vap) * count * HMAC_MS_TO_US);
        /* 100：说明service time占比多少百分比 */
        wur_info->cfg_info.on_duration = duty_cycle_period * ratio / 100 / MAC_WUR_TIME_4US_UNITS;
        wur_info->duty_cycle_period_num =
            (duty_cycle_period / MAC_WUR_TIME_4US_UNITS / wur_info->duty_cycle_period_units);
        wur_info->cfg_info.duty_cycle_period = duty_cycle_period / MAC_WUR_TIME_4US_UNITS;
    }

    oam_warning_log4(0, OAM_SF_ANY,
        "{hmac_sta_wur_mode_param_init::period_units[%d], period_num[%d], period=[%d] service_time=[%d]}",
        wur_info->duty_cycle_period_units, wur_info->duty_cycle_period_num,
        wur_info->cfg_info.duty_cycle_period, wur_info->cfg_info.on_duration);
}

OAL_STATIC uint32_t hmac_wur_check_capable(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    uint8_t vap_id;

    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    vap_id = hmac_vap->st_vap_base_info.uc_vap_id;

    /* wur定制化关闭，不进入wur */
    if (wlan_chip_get_wur_switch() != OAL_TRUE) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_wur_check_capable::wur custom close}");
        return OAL_FAIL;
    }

    if (hmac_user == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_wur_check_capable::user is NULL}");
        return OAL_FAIL;
    }

    /* 判断该AP是否支持wur */
    if (mac_user_is_wur_user(&hmac_user->st_user_base_info) == OAL_FALSE) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_wur_check_capable::ap dont support wur}");
        return OAL_FAIL;
    }

    /* 只有wlan0 vap可以下发 */
    if (mac_is_primary_legacy_vap(&hmac_vap->st_vap_base_info) == OAL_FALSE) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_wur_check_capable::only wlan0 support wur}");
        return OAL_FAIL;
    }

    /* wifi6及以上才支持wur */
    if (mac_vap_is_he_eht_protocol(&hmac_vap->st_vap_base_info) == OAL_FALSE) {
        oam_warning_log1(vap_id, OAM_SF_ANY, "{hmac_wur_check_capable::protocol[%d] < HE dont support wur}",
            hmac_vap->st_vap_base_info.en_protocol);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_wur_check_saved_wur_param(hmac_vap_stru *hmac_vap)
{
    uint32_t ret = OAL_SUCC;
    mac_wur_stru *wur_info = &hmac_vap->wur_info;
    uint8_t vap_id = hmac_vap->st_vap_base_info.uc_vap_id;

    /* 1、检查保存的duty cycle相关参数 */
    if (wur_info->cfg_info.is_always_on == OAL_TRUE) {
        if (wur_info->cfg_info.on_duration != 0 ||
            wur_info->cfg_info.duty_cycle_period != 0 ||
            wur_info->duty_cycle_period_num != 0) {
            ret = OAL_FAIL;
        }
    } else {
        if (wur_info->cfg_info.on_duration >= wur_info->cfg_info.duty_cycle_period) {
            ret = OAL_FAIL;
        }
    }

    /* 2、检查保存的睡眠相关参数 */
    if (wur_info->cfg_info.pm_timeout != WLAN_SLEEP_TIMER_PERIOD ||
        wur_info->cfg_info.psm_max_timer_restart_cnt == 0 ||
        wur_info->host_pm_timeout == 0) {
        ret = OAL_FAIL;
    }
    if (wur_info->cfg_info.sleep_mode == WLAN_WUR_MIXSLEEP) {
        if (wur_info->cfg_info.ls_listen_interval == 0) {
            ret = OAL_FAIL;
        }
    }

    if (ret != OAL_SUCC) {
        oam_warning_log4(vap_id, OAM_SF_11AX, "{hmac_wur_check_saved_wur_param:: \
            is_always_on[%u], on_duration[%u], duty_cycle_period[%u], duty_cycle_period_num[%u]}",
            wur_info->cfg_info.is_always_on, wur_info->cfg_info.on_duration,
            wur_info->cfg_info.duty_cycle_period, wur_info->duty_cycle_period_num);
        oam_warning_log4(vap_id, OAM_SF_11AX,
            "{hmac_wur_check_saved_wur_param::sleep_mode[%u], pm_timeout[%u], restart_cnt[%u], host_pm_timeout[%u]}",
            wur_info->cfg_info.sleep_mode, wur_info->cfg_info.pm_timeout,
            wur_info->cfg_info.psm_max_timer_restart_cnt, wur_info->host_pm_timeout);
        oam_warning_log2(vap_id, OAM_SF_11AX,
            "{hmac_wur_check_saved_wur_param::ls_listen_interval[%u], is_enabled[%d]}",
            wur_info->cfg_info.ls_listen_interval, wur_info->is_enabled);
    }

    return ret;
}

/* 在wur vap漫游成功或者断连之后，检查并恢复bitmap为初始值(多vap位除外) */
OAL_STATIC void hmac_reset_and_check_wur_disable_bitmap(void)
{
    hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_ROAM_EVENT);
    hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_AP_DISABLE_EVENT);
    hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_FAILURE_DETECT_EVENT);
    hmac_wur_set_wur_disable_bitmap_bit(WUR_CFGVENDOR_EVENT);
    if (g_wur_disable_bitmap != BIT(WUR_CFGVENDOR_EVENT) &&
        g_wur_disable_bitmap != (BIT(WUR_CFGVENDOR_EVENT) | BIT(COFEATURE_MVAP_EVENT))) {
        oam_warning_log1(0, OAM_SF_11AX,
            "{hmac_reset_and_check_wur_disable_bitmap::wur_disable_bitmap[0x%x]}", g_wur_disable_bitmap);
        hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_EMLSR_EVENT);
        hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_LINK_SWITCH_EVENT);
    }
}

#define HMAC_WUR_PSM_TIMER_HIGH_PPS_TIMEOUT_CNT 10
uint32_t hmac_wur_update_parameters_to_device(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, uint8_t action_field)
{
    mac_vap_stru *mac_vap = NULL;
    frw_event_mem_stru *event_mem = NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *enent = NULL;
    dmac_ctx_update_wur_stru *update_wur_cfg_event = NULL;

    if (oal_any_null_ptr2(hmac_vap, hmac_user)) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_sta_wur_update_parameters_to_device::hmac_vap or hmac_user is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = &(hmac_vap->st_vap_base_info);

    /* 抛事件到DMAC处理 */
    event_mem = frw_event_alloc_m(sizeof(dmac_ctx_update_wur_stru));
    if (event_mem == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_sta_wur_update_parameters_to_device::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    enent = frw_get_event_stru(event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(enent->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_UPDATE_WUR,
                       sizeof(dmac_ctx_update_wur_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       mac_vap->uc_chip_id,
                       mac_vap->uc_device_id,
                       mac_vap->uc_vap_id);

    update_wur_cfg_event = (dmac_ctx_update_wur_stru *)(enent->auc_event_data);
    update_wur_cfg_event->action_category = MAC_ACTION_CATEGORY_WUR;
    update_wur_cfg_event->action_field = action_field;
    update_wur_cfg_event->user_idx = hmac_user->st_user_base_info.us_assoc_id;
    memcpy_s(&update_wur_cfg_event->cfg_info, sizeof(mac_cfg_wur_stru),
        &hmac_vap->wur_info.cfg_info, sizeof(mac_cfg_wur_stru));

    /* 高流量模式更新睡眠参数，更新到dmac_vap，hmac_vap保留上层下发的 */
    if (action_field == MAC_WUR_ACTION_WUR_SLEEP_PARAM_UPDATE &&
        hmac_vap->wur_info.is_high_pps == OAL_TRUE) {
        update_wur_cfg_event->cfg_info.sleep_mode = WLAN_WUR_DEEPSLEEP;
        update_wur_cfg_event->cfg_info.pm_timeout = WLAN_SLEEP_TIMER_PERIOD;
        update_wur_cfg_event->cfg_info.psm_max_timer_restart_cnt = HMAC_WUR_PSM_TIMER_HIGH_PPS_TIMEOUT_CNT;
    }

    /* 分发 */
    frw_event_dispatch_event(event_mem);

    /* 释放事件内存 */
    frw_event_free_m(event_mem);
    return OAL_SUCC;
}

uint32_t hmac_rx_wur_setup_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len)
{
    uint8_t *wur_oper_ie = NULL;
    uint8_t *wur_mode_ie = NULL;
    uint32_t ret;
    payload += MAC_WUR_SETUP_FIXED_LEN;

    /* 处理wur operation ie,可不携带 */
    wur_oper_ie = mac_find_ie_ext_ie_in_frame(MAC_EID_WUR, MAC_EID_EXT_WUR_OPERATION, payload, (int32_t)payload_len);
    if (wur_oper_ie != NULL) {
        mac_process_wur_operation_ie(&hmac_vap->st_vap_base_info, &hmac_user->st_user_base_info, wur_oper_ie);
    }

    /* 处理wur mode ie */
    wur_mode_ie = mac_find_ie_ext_ie_in_frame(MAC_EID_WUR, MAC_EID_EXT_WUR_MODE, payload, (int32_t)payload_len);
    ret = mac_process_wur_mode_ie(&hmac_vap->st_vap_base_info, hmac_user, wur_mode_ie);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* 下发wur参数到dmac */
    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_SETUP);

    /* 协商完成配置host睡眠定时器 */
    wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, hmac_vap->wur_info.host_pm_timeout);

    /* 通知上层建立wur会话 */
    hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_WUR, WUR_ENABLED, 0);

    return OAL_SUCC;
}

/* 退出wur协商标记 */
void hmac_wur_exit(hmac_vap_stru *hmac_vap)
{
    hmac_vap->wur_info.cfg_info.negotiated = OAL_FALSE;
    hmac_vap->wur_info.is_high_pps = OAL_FALSE;
}

/* 去关联，去初始化wur */
void hmac_wur_deinit(hmac_vap_stru *hmac_vap)
{
    memset_s(&hmac_vap->wur_info, sizeof(mac_wur_stru), 0, sizeof(mac_wur_stru));
}

uint32_t hmac_rx_wur_teardown_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len)
{
    if (oal_any_null_ptr3(hmac_vap, hmac_user, payload)) {
        oam_error_log0(0, OAM_SF_OPMODE,
            "{hmac_rx_wur_teardown_frame::hmac_vap or hmac_user or payload is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, COFEATURE_AP_DISABLE_EVENT, COFEATURE_ENTER);

    return OAL_SUCC;
}

/*
 * 功能描述  : 组装wur_setup帧
 * 日    期  : 2018年12月11日
 * 修改内容  : 新生成函数
 */
static uint16_t hmac_mgmt_encap_wur_setup(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *data)
{
    uint16_t frame_len = 0;

    oam_warning_log3(0, OAM_SF_11AX, "{hmac_mgmt_encap_wur_setup_req:: mac_addr[%02x:XX:XX:XX:%02x:%02x]!}",
                     hmac_user->st_user_base_info.auc_user_mac_addr[MAC_ADDR_0],
                     hmac_user->st_user_base_info.auc_user_mac_addr[MAC_ADDR_4],
                     hmac_user->st_user_base_info.auc_user_mac_addr[MAC_ADDR_5]);

    memset_s(data, MAC_80211_FRAME_LEN, 0, MAC_80211_FRAME_LEN);
    mac_hdr_set_frame_control(data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    oal_set_mac_addr(data + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);
    oal_set_mac_addr(data + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(mac_vap));
    oal_set_mac_addr(data + WLAN_HDR_ADDR3_OFFSET, mac_vap->auc_bssid);

    frame_len += MAC_80211_FRAME_LEN;

    data[frame_len++] = MAC_ACTION_CATEGORY_WUR;
    data[frame_len++] = MAC_WUR_ACTION_WUR_MODE_SETUP;
    data[frame_len++] = 1; /* 填写dialog token, 暂固定填1 */

    /* 填写wur mode ie */
    frame_len += mac_set_wur_mode_ie(mac_vap, hmac_user, (data + frame_len), ENTER_WUR_MODE_REQ, WUR_ACCEPT);

    return frame_len;
}

/* 设置wur beacon、wur wake up、wur short wake up  CB参数 */
static void hmac_set_wur_frame_cb(oal_netbuf_stru *buffer, hmac_user_stru *hmac_user,
    uint16_t frame_len, wlan_cb_frame_type_enum_uint8 cb_frame_type, wlan_cb_frame_subtype_enum_uint8 cb_frame_subtype,
    uint8_t is_mcast)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    memset_s(oal_netbuf_cb(buffer), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(buffer);
    if (tx_ctl == NULL) {
        return;
    }
    mac_get_cb_mpdu_len(tx_ctl) = frame_len;
    mac_get_cb_frame_type(tx_ctl) = cb_frame_type;
    mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_AC_MGMT;
    mac_get_cb_frame_subtype(tx_ctl) = cb_frame_subtype;
    mac_get_cb_is_mcast(tx_ctl) = is_mcast;
    if (is_mcast == OAL_TRUE) {
        mac_get_cb_tx_user_idx(tx_ctl) = g_wlan_spec_cfg->invalid_user_id;
    } else {
        mac_get_cb_tx_user_idx(tx_ctl) = hmac_user->st_user_base_info.us_assoc_id;
    }
}

/* 设置wur action帧  CB参数 */
static void hmac_set_wur_action_frame_cb(oal_netbuf_stru *buffer, hmac_user_stru *hmac_user,
    uint16_t frame_len, wlan_cb_frame_type_enum_uint8 cb_frame_type, wlan_cb_frame_subtype_enum_uint8 cb_frame_subtype)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    memset_s(oal_netbuf_cb(buffer), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(buffer);
    if (tx_ctl == NULL) {
        return;
    }
    mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_AC_MGMT;
    mac_get_cb_frame_type(tx_ctl) = cb_frame_type;
    mac_get_cb_frame_subtype(tx_ctl) = cb_frame_subtype;
    mac_get_cb_mpdu_len(tx_ctl) = frame_len;
    mac_get_cb_is_mcast(tx_ctl) = OAL_FALSE;
    mac_get_cb_tx_user_idx(tx_ctl) = hmac_user->st_user_base_info.us_assoc_id;
}

uint32_t hmac_mgmt_tx_wur_setup(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    mac_vap_stru *mac_vap = NULL;
    oal_netbuf_stru *buffer = NULL;
    uint16_t frame_len = 0;
    uint32_t ret;

    if (oal_any_null_ptr2(hmac_vap, hmac_user)) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_mgmt_tx_wur_setup::arg is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = &(hmac_vap->st_vap_base_info);

    /* 申请WUR SETUP帧内存 */
    buffer = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (buffer == NULL) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_mgmt_tx_wur_setup::buffer null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(buffer, OAL_TRUE);

    /* 调用封装管理帧接口 */
    frame_len = hmac_mgmt_encap_wur_setup(mac_vap, hmac_user, (uint8_t *)oal_netbuf_header(buffer));

    oal_netbuf_put(buffer, frame_len);

    /* 初始化CB */
    hmac_set_wur_action_frame_cb(buffer, hmac_user, frame_len, WLAN_CB_FRAME_TYPE_MGMT, WLAN_ACTION_WUR_SETUP);

    ret = hmac_tx_mgmt_send_event(mac_vap, buffer, frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(buffer);
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_mgmt_send_disassoc_frame::hmac_tx_mgmt_send_event failed[%d].}", ret);
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint16_t hmac_mgmt_encap_wur_teardown(mac_vap_stru *mac_vap,
    hmac_user_stru *hmac_user, uint8_t *data)
{
    uint16_t frame_len = 0;

    oam_warning_log3(0, OAM_SF_11AX, "{hmac_mgmt_encap_wur_teardown:: mac_addr[%02x:XX:XX:XX:%02x:%02x]!}",
                     hmac_user->st_user_base_info.auc_user_mac_addr[MAC_ADDR_0],
                     hmac_user->st_user_base_info.auc_user_mac_addr[MAC_ADDR_4],
                     hmac_user->st_user_base_info.auc_user_mac_addr[MAC_ADDR_5]);

    mac_hdr_set_frame_control(data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    oal_set_mac_addr(data + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);
    oal_set_mac_addr(data + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(mac_vap));
    oal_set_mac_addr(data + WLAN_HDR_ADDR3_OFFSET, mac_vap->auc_bssid);

    frame_len += MAC_80211_FRAME_LEN;

    data[frame_len++] = MAC_ACTION_CATEGORY_WUR;
    data[frame_len++] = MAC_WUR_ACTION_WUR_MODE_TEARDOWN;

    return frame_len;
}

OAL_STATIC void hmac_wur_teardown_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t reason)
{
    if (oal_any_null_ptr2(hmac_vap, hmac_user)) {
        return;
    }

    /* 清掉已协商标记 */
    hmac_wur_exit(hmac_vap);

    /* 恢复默认host睡眠超时时间 */
    wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, WLAN_SLEEP_DEFAULT_CHECK_CNT);

    /* 通知device走teardown流程 */
    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_TEARDOWN);

    /* 通知上层退出wur会话 */
    hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_WUR, WUR_DISABLED, reason);
}

uint32_t hmac_mgmt_tx_wur_teardown(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t reason)
{
    mac_vap_stru *mac_vap = NULL;
    oal_netbuf_stru *buffer = NULL;
    uint16_t frame_len;
    uint32_t ret;

    if (oal_any_null_ptr2(hmac_vap, hmac_user)) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_mgmt_tx_wur_teardown::hmac_vap or hmac_user is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = &(hmac_vap->st_vap_base_info);

    /* 未协商完参数，不能teardown */
    if (hmac_wur_is_negotiated(hmac_vap) == OAL_FALSE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_mgmt_tx_wur_teardown::not negotiated.}");
        return OAL_FAIL;
    }

    /* 申请WUR TEARDOWN管理帧内存 */
    buffer = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (buffer == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_mgmt_tx_wur_teardown::wur_teardown_req null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(buffer, OAL_TRUE);

    /* 调用封装管理帧接口 */
    frame_len = hmac_mgmt_encap_wur_teardown(mac_vap, hmac_user, oal_netbuf_header(buffer));

    oal_netbuf_put(buffer, frame_len);

    /* 初始化CB */
    hmac_set_wur_action_frame_cb(buffer, hmac_user, frame_len, WLAN_CB_FRAME_TYPE_MGMT, WLAN_ACTION_WUR_TEARDOWN);

    ret = hmac_tx_mgmt_send_event(mac_vap, buffer, frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(buffer);
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_mgmt_tx_wur_teardown::hmac_tx_mgmt_send_event failed[%d].}", ret);
        return ret;
    }

    /* host teardown处理 */
    hmac_wur_teardown_process(hmac_vap, hmac_user, reason);

    return OAL_SUCC;
}

/*
 * 功能描述  : WUR ACTION帧发送
 */
void hmac_mgmt_tx_wur_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_wur_action_mgmt_args_stru *wur_action_args)
{
    if (wur_action_args->category == MAC_ACTION_CATEGORY_WUR) {
        switch (wur_action_args->action) {
            case MAC_WUR_ACTION_WUR_MODE_SETUP:
                hmac_mgmt_tx_wur_setup(hmac_vap, hmac_user);
                break;
            case MAC_WUR_ACTION_WUR_MODE_TEARDOWN:
                hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user, WUR_CFGVENDOR_EVENT);
                break;
            default:
                break;
        }
    }
}

/*
 * 功能描述  : WUR SETUP配置命令,mpw不支持ml场景的wur
 * 1.日    期  : 2018年12月11日
  *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_config_tx_wur_setup(mac_vap_stru *mac_vap, mac_cfg_wur_setup_param_stru *wur_setup)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    uint32_t ret;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_vap_get_hmac_user_by_addr(mac_vap, wur_setup->mac_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        return OAL_FAIL;
    }

    /* 检查发送wur setup 需要满足的条件 */
    ret = hmac_wur_check_capable(hmac_vap, hmac_user);
    if (ret != OAL_SUCC) {
        return ret;
    }
    /* 初始化sta自己的param */
    hmac_sta_wur_mode_param_init(hmac_vap, hmac_user, wur_setup->is_always_on, wur_setup->count, wur_setup->ratio);

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_CONFIG_CHECK_WUR_ALLOWED);
    return OAL_SUCC;
}

uint32_t hmac_config_device_allow_tx_wur_setup(frw_event_mem_stru *event_mem)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    frw_event_stru *event = NULL; /* 事件结构体 */
    mac_cfg_wur_stru *wur_cfg = NULL;
    mac_wur_stru *wur_info = NULL;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_device_allow_tx_wur_setup::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    wur_cfg = (mac_cfg_wur_stru *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_device_allow_tx_wur_setup::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id,
            OAM_SF_ANY, "{hmac_config_device_allow_tx_wur_setup::dmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    wur_info = &(hmac_vap->wur_info);
    if (wur_cfg->duty_cycle_period != 0) {
        wur_info->cfg_info.is_always_on = OAL_FALSE;
    }
    wur_info->cfg_info.on_duration = wur_cfg->on_duration;
    wur_info->cfg_info.duty_cycle_period = wur_cfg->duty_cycle_period;

    if (wur_info->duty_cycle_period_units == 0) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id,
            OAM_SF_ANY, "{hmac_config_device_allow_tx_wur_setup::duty_cycle_period_units is 0.}");
        return OAL_ERR_CODE_DIV_ZERO;
    }
    wur_info->duty_cycle_period_num = wur_info->cfg_info.duty_cycle_period / wur_info->duty_cycle_period_units;

    if (hmac_wur_is_negotiated(hmac_vap) == OAL_TRUE) {
        hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_SLEEP_PARAM_UPDATE);
        return OAL_SUCC;
    }

    hmac_mgmt_tx_wur_setup(hmac_vap, hmac_user);
    return OAL_SUCC;
}

/*
 * 功能描述  : delete wur SETUP会话的配置命令
 * 1.日    期  : 2018年12月11日
  *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_config_tx_wur_teardown(mac_vap_stru *mac_vap, mac_cfg_wur_teardown_param_stru *wur_teardown)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    mac_wur_action_mgmt_args_stru wur_action_args = {0}; /* 用于填写ACTION帧的参数 */

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_wur_teardown::hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_vap_get_hmac_user_by_addr(mac_vap, wur_teardown->mac_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_wur_teardown::hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    wur_action_args.category = MAC_ACTION_CATEGORY_WUR;
    wur_action_args.action = MAC_WUR_ACTION_WUR_MODE_TEARDOWN;

    hmac_mgmt_tx_wur_action(hmac_vap, hmac_user, &wur_action_args);

    return OAL_SUCC;
}

uint16_t hmac_encap_wur_wakeup(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    uint8_t *frame_hdr)
{
    uint8_t *payload = NULL;
    uint16_t frame_len = 0;
    wur_wake_up_mac_header_stru *mac_header = NULL;
    wur_fcs_stru *fcs_filed;

    memset_s(frame_hdr, MAC_WUR_MAC_HEADER_LEN, 0, MAC_WUR_MAC_HEADER_LEN);

    /* TODO:暂没有frame body，暂时只单播wake up帧唤醒单个sta */
    mac_header = (wur_wake_up_mac_header_stru *)frame_hdr;
    mac_header->type = WUR_WAKE_UP_FRAME; /*  */
    mac_header->id = 0; /* TODO: 从user下取出wur id */

    frame_len += MAC_WUR_MAC_HEADER_LEN;
    payload = frame_hdr + MAC_WUR_MAC_HEADER_LEN;

    /* TODO:暂时固定长度 没有frame body */

    /* 由软件填写fcs */
    fcs_filed = (wur_fcs_stru *)payload;
    fcs_filed->fcs = 0; /* TODO：fcs暂时填0，sta关闭fcs校验 */
    frame_len += sizeof(wur_fcs_stru);
    payload += sizeof(wur_fcs_stru);

    return frame_len;
}

uint16_t hmac_encap_wur_short_wakeup(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    uint8_t *frame_hdr)
{
    uint8_t *payload = NULL;
    uint16_t frame_len = 0;
    wur_short_wake_up_frame_stru *mac_header = NULL;
    wur_fcs_stru *fcs_filed;

    memset_s(frame_hdr, MAC_SHORT_WUR_WAKEUP_FRAME_LEN, 0, MAC_SHORT_WUR_WAKEUP_FRAME_LEN);

    /* short wake up 只单播wake up帧唤醒单个sta */
    mac_header = (wur_short_wake_up_frame_stru *)frame_hdr;
    mac_header->type = WUR_WAKE_UP_FRAME; /*  */
    mac_header->wur_id = 0; /* TODO: 从user下取出wur id */

    frame_len += MAC_SHORT_WUR_WAKEUP_FRAME_LEN;
    payload = frame_hdr + MAC_SHORT_WUR_WAKEUP_FRAME_LEN;

    /* short wake up没有frame body */
    /* 由软件填写fcs */
    fcs_filed = (wur_fcs_stru *)payload;
    fcs_filed->fcs = 0; /* TODO：fcs暂时填0，sta关闭fcs校验 */
    frame_len += sizeof(wur_fcs_stru);
    payload += sizeof(wur_fcs_stru);

    return frame_len;
}

uint32_t hmac_ap_tx_wur_wake_up(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t is_short_wakeup)
{
    oal_netbuf_stru *buffer = NULL;
    uint16_t frame_len = 0;
    uint32_t ret;

    buffer = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (buffer == NULL) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_mgmt_tx_wur_setup::buffer null.}");
        return OAL_FAIL;
    }

    oal_mem_netbuf_trace(buffer, OAL_TRUE);

    if (is_short_wakeup) {
        frame_len = hmac_encap_wur_short_wakeup(mac_vap, hmac_user, (uint8_t *)oal_netbuf_header(buffer));
    } else {
        frame_len = hmac_encap_wur_wakeup(mac_vap, hmac_user, (uint8_t *)oal_netbuf_header(buffer));
    }

    oal_netbuf_put(buffer, frame_len);

    hmac_set_wur_frame_cb(buffer, hmac_user, frame_len, WLAN_CB_FRAME_TYPE_MGMT, WLAN_ACTION_WUR_WAKEUP, OAL_FALSE);

    ret = hmac_tx_mgmt_send_event(mac_vap, buffer, frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(buffer);
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_mgmt_tx_wur_teardown::hmac_tx_mgmt_send_event failed[%d].}", ret);
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_config_tx_wur_wakeup(mac_vap_stru *mac_vap, mac_cfg_wur_wakeup_param_stru *wur_wake_up)
{
    hmac_user_stru *hmac_user = NULL;

    hmac_user = mac_vap_get_hmac_user_by_addr(mac_vap, wur_wake_up->mac_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        return OAL_FAIL;
    }

    hmac_ap_tx_wur_wake_up(mac_vap, hmac_user, wur_wake_up->is_short_wake_up);

    return OAL_SUCC;
}

uint16_t hmac_encap_wur_beacon(mac_vap_stru *mac_vap, uint8_t *frame_hdr)
{
    uint8_t *payload = NULL;
    uint16_t frame_len = 0;
    wur_beacon_mac_header_stru *mac_header = NULL;
    wur_fcs_stru *fcs_filed;

    memset_s(frame_hdr, MAC_WUR_MAC_HEADER_LEN, 0, MAC_WUR_MAC_HEADER_LEN);

    /* wur beacon没有frame body只有mac header和fcs */
    mac_header = (wur_beacon_mac_header_stru *)frame_hdr;
    mac_header->type = WUR_BEACON_FRAME;
    mac_header->transmitter_id =
        (((mac_vap->auc_bssid[MAC_ADDR_4] & 0xF) << BIT_OFFSET_8) | mac_vap->auc_bssid[MAC_ADDR_5]);
    mac_header->partial_tsf = ((oal_time_get_high_precision_ms() & 0x1FFE0) >> BIT_OFFSET_5); /* TODO:获取当前tsf的5-16位 */

    frame_len += MAC_WUR_MAC_HEADER_LEN;
    payload = frame_hdr + MAC_WUR_MAC_HEADER_LEN;

    /* 由软件填写fcs */
    fcs_filed = (wur_fcs_stru *)payload;
    fcs_filed->fcs = 0; /* TODO：fcs暂时填0，sta关闭fcs校验 */
    frame_len += sizeof(wur_fcs_stru);
    payload += sizeof(wur_fcs_stru);

    return frame_len;
}

OAL_STATIC uint32_t hmac_ap_tx_wur_beacon(mac_vap_stru *mac_vap)
{
    oal_netbuf_stru *buffer = NULL;
    uint16_t frame_len = 0;
    uint32_t ret;

    buffer = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (buffer == NULL) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_mgmt_tx_wur_setup::buffer null.}");
        return OAL_FAIL;
    }

    oal_mem_netbuf_trace(buffer, OAL_TRUE);

    frame_len = hmac_encap_wur_beacon(mac_vap, (uint8_t *)oal_netbuf_header(buffer));

    oal_netbuf_put(buffer, frame_len);

    hmac_set_wur_frame_cb(buffer, NULL, frame_len, WLAN_CB_FRAME_TYPE_MGMT, WLAN_ACTION_WUR_BEACON, OAL_TRUE);

    ret = hmac_tx_mgmt_send_event(mac_vap, buffer, frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(buffer);
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_mgmt_tx_wur_teardown::hmac_tx_mgmt_send_event failed[%d].}", ret);
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_config_set_wur_fastsleep_params(hmac_vap_stru *hmac_vap,
    mac_cfg_wur_fastsleep_param_stru *fastsleep_params, uint8_t is_hipriv_cmd)
{
    hmac_vap->wur_info.host_pm_timeout = fastsleep_params->host_pm_timeout;
    hmac_vap->wur_info.cfg_info.pm_timeout = fastsleep_params->pm_timeout;
    hmac_vap->wur_info.cfg_info.psm_max_timer_restart_cnt = fastsleep_params->psm_max_timer_restart_cnt;
    hmac_vap->wur_info.cfg_info.ls_listen_interval = fastsleep_params->ls_listen_interval;
    if (is_hipriv_cmd == OAL_TRUE) {
        wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, hmac_vap->wur_info.host_pm_timeout);
    }
    oam_warning_log4(0, OAM_SF_11AX,
        "{hmac_config_set_wur_fastsleep_params::pm_timeout[%d], psm_max_timer_restart_cnt[%d], \
        ls_listen_interval[%d], host_pm_timeout_cnt[%d]",
        hmac_vap->wur_info.cfg_info.pm_timeout, hmac_vap->wur_info.cfg_info.psm_max_timer_restart_cnt,
        hmac_vap->wur_info.cfg_info.ls_listen_interval, fastsleep_params->host_pm_timeout);
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_config_set_wur_sleep_mode(hmac_vap_stru *hmac_vap,
    uint32_t sleep_mode)
{
    hmac_vap->wur_info.cfg_info.sleep_mode = (uint8_t)sleep_mode;
    oam_warning_log1(0, OAM_SF_11AX,
        "{hmac_config_set_wur_sleep_mode::sleep_mode[%d] 0:deepsleep 1:lightsleep 2:mix_mode",
        hmac_vap->wur_info.cfg_info.sleep_mode);
    return OAL_SUCC;
}

uint32_t hmac_config_wur_debug_process(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    mac_cfg_wur_debug_param_stru *wur_debug = (mac_cfg_wur_debug_param_stru *)param;
    hmac_vap_stru *hmac_vap = NULL;
    uint32_t ret;

    if (oal_any_null_ptr2(mac_vap, param)) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_config_wur_debug_process::null ptr}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* wur定制化关闭，不处理wur命令 */
    if (wlan_chip_get_wur_switch() != OAL_TRUE) {
        return OAL_FAIL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((wur_debug->cmd_bitmap & BIT(MAC_TX_WUR_WAKEUP)) != 0) {
        hmac_config_tx_wur_wakeup(mac_vap, &wur_debug->wakeup_params);
    } else if ((wur_debug->cmd_bitmap & BIT(MAC_TX_WUR_BEACON)) != 0) {
        hmac_ap_tx_wur_beacon(mac_vap);
    } else if ((wur_debug->cmd_bitmap & BIT(MAC_TX_WUR_SETUP)) != 0) {
        hmac_config_tx_wur_setup(mac_vap, &wur_debug->setup_params);
    } else if ((wur_debug->cmd_bitmap & BIT(MAC_TX_WUR_TEARDOWN)) != 0) {
        hmac_config_tx_wur_teardown(mac_vap, &wur_debug->teardown_params);
    } else if ((wur_debug->cmd_bitmap & BIT(MAC_WUR_FASTSLEEP_PARAM)) != 0) {
        hmac_config_set_wur_fastsleep_params(hmac_vap, &wur_debug->fastsleep_params, OAL_TRUE);
    } else if ((wur_debug->cmd_bitmap & BIT(MAC_WUR_SLEEP_MODE)) != 0) {
        hmac_config_set_wur_sleep_mode(hmac_vap, wur_debug->sleep_mode);
    } else {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_config_wur_debug_process::wrong cmd!}");
    }

    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_WUR_DEBUG, len, param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_config_wur_debug_process::hmac_config_send_event failed[%d].}", ret);
    }

    return ret;
}

/* 下发参数同步到另一个link */
OAL_STATIC void hmac_wur_param_sync_to_another_link(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru *another_hmac_vap = NULL;
    mac_wur_stru *another_wur_info = NULL;

#ifdef _PRE_WLAN_FEATURE_ML
    another_hmac_vap = hmac_mld_get_another_vap(hmac_vap);
#endif
    if (another_hmac_vap == NULL) {
        return;
    }
    another_wur_info = &another_hmac_vap->wur_info;
    another_wur_info->cfg_info.sleep_mode = hmac_vap->wur_info.cfg_info.sleep_mode;
    another_wur_info->host_pm_timeout = hmac_vap->wur_info.host_pm_timeout;
    another_wur_info->cfg_info.pm_timeout = hmac_vap->wur_info.cfg_info.pm_timeout;
    another_wur_info->cfg_info.psm_max_timer_restart_cnt = hmac_vap->wur_info.cfg_info.psm_max_timer_restart_cnt;
    another_wur_info->cfg_info.ls_listen_interval = hmac_vap->wur_info.cfg_info.ls_listen_interval;
    another_wur_info->cfg_info.is_always_on = hmac_vap->wur_info.cfg_info.is_always_on;
    another_wur_info->cfg_info.on_duration = hmac_vap->wur_info.cfg_info.on_duration;
    another_wur_info->duty_cycle_period_num = hmac_vap->wur_info.duty_cycle_period_num;
    another_wur_info->cfg_info.duty_cycle_period = hmac_vap->wur_info.cfg_info.duty_cycle_period;
    another_wur_info->duty_cycle_period_units = hmac_vap->wur_info.duty_cycle_period_units;
}

uint32_t hmac_config_set_wur_param(mac_vap_stru *mac_vap, uint8_t *param)
{
    mac_vendor_wur_param_stru *cfg_wur_param = (mac_vendor_wur_param_stru *)param;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    mac_cfg_wur_fastsleep_param_stru fastsleep_param = {0};
    uint8_t cnt;

    if (oal_any_null_ptr2(mac_vap, param)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_res_get_hmac_user(mac_vap->us_assoc_vap_id);
    if (hmac_user == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (cfg_wur_param->wur_enable == OAL_TRUE &&
        (hmac_vap->fastsleep_en == OAL_TRUE || hmac_vap->fastsleep3_en == OAL_TRUE)) {
        oam_error_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_11AX,
            "{hmac_config_set_wur_param::fs[%d] fs3[%d], not enable wur]}",
            hmac_vap->fastsleep_en, hmac_vap->fastsleep3_en);
        return OAL_FAIL;
    }

    /* 检查wur基本能力 */
    if (hmac_wur_check_capable(hmac_vap, hmac_user) != OAL_SUCC) {
        hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_WUR, WUR_REJECTED, MAC_WUR_CAP_REJECT_REASON);
        return OAL_FAIL;
    }

    /* 保存参数 */
    if (cfg_wur_param->wur_enable == OAL_TRUE) {
        /* 配置睡眠模式 */
        hmac_config_set_wur_sleep_mode(hmac_vap, cfg_wur_param->wur_sleep_mode);
        /* 配置快睡参数 */
        fastsleep_param.pm_timeout = WLAN_SLEEP_TIMER_PERIOD;
        cnt = cfg_wur_param->listen_interval / WLAN_SLEEP_TIMER_PERIOD;
        fastsleep_param.psm_max_timer_restart_cnt = (cnt <= 1) ? MAC_WUR_DEVICE_SLEEP_MIN_RESTART_CNT : cnt;
        fastsleep_param.host_pm_timeout = fastsleep_param.psm_max_timer_restart_cnt - 1;
        if (cfg_wur_param->wur_sleep_mode == WLAN_WUR_MIXSLEEP) {
            fastsleep_param.ls_listen_interval = cfg_wur_param->ls_listen_interval;
        }
        hmac_config_set_wur_fastsleep_params(hmac_vap, &fastsleep_param, OAL_FALSE);

        /* 初始化sta自己的param,先默认长醒模式 */
        hmac_sta_wur_mode_param_init(hmac_vap, hmac_user, OAL_TRUE,
            MAC_WUR_DEFAULT_DUTY_CYCLE_PERIOD, MAC_WUR_DEFAULT_SERVICE_TIME_RATID);

        /* 如果是mld vap，同步下发的配置参数到另一个link vap */
        if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_FALSE) {
            hmac_wur_param_sync_to_another_link(hmac_vap);
        }

        /* 通知wur模块，使能wur */
        hmac_wur_set_enabled(hmac_vap, OAL_TRUE);
        hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, WUR_CFGVENDOR_EVENT, COFEATURE_ENTER);
    } else {
        /* 通知wur模块，关闭wur */
        hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, WUR_CFGVENDOR_EVENT, COFEATURE_EXIT);
        hmac_wur_set_enabled(hmac_vap, OAL_FALSE);
    }

    return OAL_SUCC;
}

OAL_STATIC void hmac_wur_roam_connect_exit_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    if (hmac_wur_is_negotiated(hmac_vap) == OAL_FALSE) {
        hmac_vap->wur_info.is_negotiated_before_roam = OAL_FALSE;
        return;
    }

    /* 漫游前已经协商wur，用于漫游失败后回原bss */
    hmac_vap->wur_info.is_negotiated_before_roam = OAL_TRUE;

    /* 清掉已协商标记 */
    hmac_wur_exit(hmac_vap);

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_TEARDOWN);

    /* 通知上层退出wur会话 */
    hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_WUR, WUR_DISABLED, COFEATURE_ROAM_EVENT);
}

OAL_STATIC void hmac_wur_roam_to_old_bss_recover_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    if (hmac_vap->wur_info.is_negotiated_before_roam == OAL_FALSE) {
        return;
    }

    /* 恢复协商标记 */
    hmac_vap->wur_info.cfg_info.negotiated = OAL_TRUE;

    /* 漫游结束，清掉清掉已协商标记 */
    hmac_vap->wur_info.is_negotiated_before_roam = OAL_FALSE;

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_SETUP);

    /* 通知上层建立wur会话 */
    hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_WUR, WUR_ENABLED, 0);
}

/* 其他vap入网、启动时，sta 退出wur会话，发起wur teardown */
OAL_STATIC void hmac_wur_enter_mvap_exit_wur(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru *hmac_wur_vap = NULL;
    mac_vap_stru *mac_sta_vap = NULL;
    hmac_user_stru *hmac_wur_user = NULL;
    mac_device_stru *mac_device = NULL;

    mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return;
    }

    /* 起双wifi的wlan1不退wur */
    if (mac_is_secondary_sta(hmac_vap)) {
        return;
    }

    /* 非双wifi，如果多vap，置位 */
    if (mac_device_calc_up_vap_num(mac_device) >= 1) {
        hmac_wur_set_wur_disable_bitmap_bit(COFEATURE_MVAP_EVENT);
    }
    /* 特殊场景识别：如果是入参是up vap触发自愈，防止误识别 */
    if (mac_device_calc_up_vap_num(mac_device) == 1) {
        mac_device_find_up_sta(mac_device, &mac_sta_vap);
        if (mac_sta_vap != NULL && hmac_vap->st_vap_base_info.uc_vap_id == mac_sta_vap->uc_vap_id) {
            hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_MVAP_EVENT);
        }
    }

    oam_warning_log4(0, OAM_SF_11AX,
        "{hmac_wur_enter_mvap_exit_wur::vap_id[%d] mac_device->uc_vap_num[%d], up_vap_num[%d] wur_bitmap[0x%x]}",
        hmac_vap->st_vap_base_info.uc_vap_id, mac_device->uc_vap_num,
        mac_device_calc_up_vap_num(mac_device), g_wur_disable_bitmap);

    hmac_wur_vap = hmac_find_up_wur_vap();
    if (hmac_wur_vap == NULL) {
        return;
    }

    if (hmac_vap->st_vap_base_info.uc_vap_id == hmac_wur_vap->st_vap_base_info.uc_vap_id) {
        oam_warning_log0(0, OAM_SF_11AX, "{hmac_wur_mvap_exit_wur::wrong state}");
        return;
    }

    hmac_wur_user = mac_res_get_hmac_user(hmac_wur_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_wur_user == NULL) {
        return;
    }

    hmac_mgmt_tx_wur_teardown(hmac_wur_vap, hmac_wur_user, COFEATURE_MVAP_EVENT);
}

/* 检查能否重新进入wur */
OAL_STATIC uint32_t hmac_wur_enter_check(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    /* wur是否已协商 */
    if (hmac_wur_is_negotiated(hmac_vap) == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_wur_enter_check::wur is negotiated}");
        return OAL_FAIL;
    }

    /* 检查wur基本能力 */
    if (hmac_wur_check_capable(hmac_vap, hmac_user) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 检查wur bitmap */
    if (hmac_wur_check_bitmap() != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_wur_enter_check::wur_disable_bitmap[0x%x]}", g_wur_disable_bitmap);
        return OAL_FAIL;
    }

    /* 检查已保存的wur参数 */
    if (hmac_wur_check_saved_wur_param(hmac_vap) != OAL_SUCC) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* 删除入参vap，判断是否有up sta可以重新建立wur会话，可以的话发起wur setup */
OAL_STATIC void hmac_wur_exit_mvap_enter_wur(hmac_vap_stru *hmac_vap)
{
    hmac_vap_stru *hmac_sta_vap = NULL;
    mac_vap_stru *mac_sta_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    mac_device_stru *mac_device = NULL;

    mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return;
    }

    /* 删vap后，退出多vap，清bit位 */
    if (mac_device_calc_up_vap_num(mac_device) <= 1) {
        hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_MVAP_EVENT);
    }

    oam_warning_log2(0, OAM_SF_ANY,
        "{hmac_wur_exit_mvap_enter_wur::mac_device_calc_up_vap_num[%d] wur_disable_bitmap[0x%x]}",
        mac_device_calc_up_vap_num(mac_device), g_wur_disable_bitmap);

    mac_device_find_up_sta(mac_device, &mac_sta_vap);
    if (mac_sta_vap == NULL || hmac_vap->st_vap_base_info.uc_vap_id == mac_sta_vap->uc_vap_id) {
        return;
    }
    hmac_sta_vap = mac_res_get_hmac_vap(mac_sta_vap->uc_vap_id);

    hmac_user = mac_res_get_hmac_user(mac_sta_vap->us_assoc_vap_id);
    if (hmac_user == NULL) {
        return;
    }

    /* 检查能否重新进入wur */
    if (hmac_wur_enter_check(hmac_sta_vap, hmac_user) != OAL_SUCC) {
        return;
    }

    hmac_wur_update_parameters_to_device(hmac_sta_vap, hmac_user, MAC_WUR_ACTION_CONFIG_CHECK_WUR_ALLOWED);
}

/* 退出emlsr，入参是工作的link vap，判断能否进wur */
OAL_STATIC void hmac_wur_close_emlsr_enter_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    /* 检查能否重新进入wur */
    if (hmac_wur_enter_check(hmac_vap, hmac_user) != OAL_SUCC) {
        return;
    }

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_CONFIG_CHECK_WUR_ALLOWED);
}

/* link切换结束，不论成功与否，入参是原link vap，判断work_vap能否进wur */
OAL_STATIC void hmac_wur_mlsr_link_switch_complete_enter_wur(hmac_vap_stru *ori_hmac_vap,
    hmac_user_stru *ori_hmac_user, uint8_t event_subtype)
{
    hmac_vap_stru *work_hmac_vap = ori_hmac_vap;
    hmac_user_stru *work_hmac_user = ori_hmac_user;

#ifdef _PRE_WLAN_FEATURE_ML
    /* link切换成功，把enable标记从原link切到工作link，保证只有一个vap是enable的 */
    if (event_subtype == COFEATURE_ENTER_SUCC) {
        work_hmac_vap = hmac_mld_get_another_vap(ori_hmac_vap);
        if (work_hmac_vap != NULL) {
            work_hmac_user = mac_res_get_hmac_user(work_hmac_vap->st_vap_base_info.us_assoc_vap_id);
            hmac_wur_set_enabled(work_hmac_vap, ori_hmac_vap->wur_info.is_enabled);
            hmac_wur_set_enabled(ori_hmac_vap, OAL_FALSE);
        } else {
            return;
        }
    }
#endif

    /* 检查能否重新进入wur */
    if (hmac_wur_enter_check(work_hmac_vap, work_hmac_user) != OAL_SUCC) {
        return;
    }

    hmac_wur_update_parameters_to_device(work_hmac_vap, work_hmac_user, MAC_WUR_ACTION_CONFIG_CHECK_WUR_ALLOWED);
}

/* 上层vendor命令下发，判断能否进wur */
OAL_STATIC void hmac_wur_cfgvendor_enter_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    /* 检查wur bitmap */
    if (hmac_wur_check_bitmap() != OAL_SUCC) {
        hmac_cofeture_state_change_notify_sta(hmac_vap, COFEATURE_TYPE_WUR, WUR_REJECTED,
            oal_bit_find_first_bit_four_byte(g_wur_disable_bitmap));
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_wur_cfgvendor_enter_wur::not enter wur, reason[%u]}",
            oal_bit_find_first_bit_four_byte(g_wur_disable_bitmap));
        return;
    }

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_CONFIG_CHECK_WUR_ALLOWED);
}

OAL_STATIC void hmac_wur_cfgvendor_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        hmac_wur_clear_wur_disable_bitmap_bit(WUR_CFGVENDOR_EVENT); /* 上层vendor命令触发进入wur，清掉bit位 */
        hmac_wur_cfgvendor_enter_wur(hmac_vap, hmac_user);
    } else if (event_subtype == COFEATURE_EXIT) {
        hmac_wur_set_wur_disable_bitmap_bit(WUR_CFGVENDOR_EVENT); /* 命令触发退出wur，置bit位，退出wur */
        hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user, WUR_CFGVENDOR_EVENT);
    }
}

OAL_STATIC void hmac_wur_mvap_event_handle(hmac_vap_stru *hmac_vap, uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        hmac_wur_enter_mvap_exit_wur(hmac_vap); /* start vap退出wur */
    } else if (event_subtype == COFEATURE_EXIT) {
        hmac_wur_exit_mvap_enter_wur(hmac_vap); /* 删vap，判断能否进入wur */
    }
}

OAL_STATIC void hmac_wur_roam_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t event_type,
    uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        hmac_wur_set_wur_disable_bitmap_bit(COFEATURE_ROAM_EVENT);
        hmac_wur_roam_connect_exit_wur(hmac_vap, hmac_user); /* 开始漫游入网，退出wur */
    } else if (event_subtype == COFEATURE_ENTER_SUCC) {
        /* 检查并重置wur_disable_bitmap */
        hmac_reset_and_check_wur_disable_bitmap();
        hmac_wur_set_enabled(hmac_vap, OAL_FALSE);
    } else if (event_subtype == COFEATURE_ENTER_FAIL) {
        hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_ROAM_EVENT);
        hmac_wur_roam_to_old_bss_recover_wur(hmac_vap, hmac_user);
    }
}

/* 退出emlsr，入参是工作的link vap，判断能否进wur */
OAL_STATIC void hmac_wur_bt_release_channel_enter_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    /* 检查能否重新进入wur */
    if (hmac_wur_enter_check(hmac_vap, hmac_user) != OAL_SUCC) {
        return;
    }

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_CONFIG_CHECK_WUR_ALLOWED);
}

/* 蓝牙占用/释放 wur通道处理,不在bitmap里体现 */
OAL_STATIC void hmac_wur_bt_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t event_type,
    uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user, event_type);
    } else if (event_subtype == COFEATURE_EXIT) {
        hmac_wur_bt_release_channel_enter_wur(hmac_vap, hmac_user);
    }
}

OAL_STATIC void hmac_wur_emlsr_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t event_type,
    uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        hmac_wur_set_wur_disable_bitmap_bit(COFEATURE_EMLSR_EVENT); /* 进入emlsr，置bit位，退出wur */
        hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user, event_type);
    } else if (event_subtype == COFEATURE_ENTER_FAIL || event_subtype == COFEATURE_EXIT) {
        hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_EMLSR_EVENT); /* 退出emlsr，清bit位 */
        hmac_wur_close_emlsr_enter_wur(hmac_vap, hmac_user); /* 退出emlsr，进入wur */
    }
}

OAL_STATIC void hmac_wur_link_switch_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        hmac_wur_set_wur_disable_bitmap_bit(COFEATURE_LINK_SWITCH_EVENT); /* 开始link切换，置bit位，退出wur */
        hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user, event_type);
    } else if (event_subtype == COFEATURE_ENTER_FAIL || event_subtype == COFEATURE_ENTER_SUCC) {
        hmac_wur_clear_wur_disable_bitmap_bit(COFEATURE_LINK_SWITCH_EVENT); /* link切换完成，清bit位 */
        hmac_wur_mlsr_link_switch_complete_enter_wur(hmac_vap, hmac_user, event_subtype); /* link切换完成，进入wur */
    }
}

OAL_STATIC void hmac_wur_bitmap_sync_notify(hmac_vap_stru *hmac_vap, uint8_t event_type,
    uint8_t event_subtype, uint32_t ori_bitamp)
{
    /* bitmap没变化不同步到device */
    if (g_wur_disable_bitmap == ori_bitamp) {
        return;
    }

    oam_warning_log4(0, OAM_SF_ANY, "{hmac_wur_bitmap_sync_notify::event_type[%d], event_subtype[%d], \
        ori_wur_disable_bitmap[0x%x], wur_disable_bitmap[0x%x]}",
        event_type, event_subtype, ori_bitamp, g_wur_disable_bitmap);

    if (hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_WUR_BITMAP_SYNC,
        sizeof(uint32_t), (uint8_t *)&g_wur_disable_bitmap) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_wur_bitmap_sync_notify::send event failed}");
    }
}

/* wur sta删用户去初始化wur */
OAL_STATIC void hmac_wur_del_user_exit_wur(hmac_vap_stru *hmac_vap)
{
    /* 检查并重置wur_disable_bitmap */
    hmac_reset_and_check_wur_disable_bitmap();

    /* 去初始化wur */
    hmac_wur_deinit(hmac_vap);

    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
        "{hmac_wur_del_user_exit_wur::vap_mode[%d] p2p_mode[%d]}",
        hmac_vap->st_vap_base_info.en_vap_mode, hmac_vap->st_vap_base_info.en_p2p_mode);
}

OAL_STATIC void hmac_wur_pps_counter_clear(hmac_vap_stru *hmac_vap)
{
    hmac_vap->wur_info.high_pps_enter_cnt = 0;
    hmac_vap->wur_info.high_pps_exit_cnt = 0;
}

OAL_STATIC void hmac_wur_pm_set_host_timeout(hmac_vap_stru *hmac_vap)
{
    if (hmac_vap->wur_info.is_high_pps == OAL_TRUE) {
        wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, WLAN_SLEEP_DEFAULT_CHECK_CNT);
    } else {
        wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, hmac_vap->wur_info.host_pm_timeout);
    }
}

OAL_STATIC void hmac_wur_high_pps_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t event_type,
    uint8_t event_subtype)
{
    uint8_t is_high_pps = (event_subtype == COFEATURE_ENTER) ? OAL_TRUE : OAL_FALSE;

    /* wur未协商，不根据流量来调整wur开启时的睡眠定时器参数 */
    if (hmac_wur_is_negotiated(hmac_vap) == OAL_FALSE) {
        return;
    }

    /* 流量模式没有变化，不处理 */
    if (hmac_vap->wur_info.is_high_pps == is_high_pps) {
        return;
    }

    hmac_vap->wur_info.is_high_pps = is_high_pps;

    if (hmac_wur_check_saved_wur_param(hmac_vap) == OAL_SUCC) {
        hmac_wur_pm_set_host_timeout(hmac_vap);
        hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_SLEEP_PARAM_UPDATE);
    }

    hmac_wur_pps_counter_clear(hmac_vap);
}

OAL_STATIC void hmac_wur_pm_enable_event_handle(hmac_vap_stru *hmac_vap, uint8_t event_type,
    uint8_t event_subtype)
{
    /* wur未协商，不根据协议低功耗使能来调整wur开启时的睡眠定时器参数 */
    if (hmac_wur_is_negotiated(hmac_vap) == OAL_FALSE) {
        return;
    }

    if (event_subtype == COFEATURE_ENTER) {
        /* 使能协议低功耗 */
        if (hmac_wur_check_saved_wur_param(hmac_vap) == OAL_SUCC) {
            wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, hmac_vap->wur_info.host_pm_timeout);
        }
    } else if (event_subtype == COFEATURE_EXIT) {
        /* 关闭协议低功耗，恢复默认 */
        wlan_pm_set_host_timeout(hmac_vap->st_vap_base_info.uc_device_id, WLAN_SLEEP_DEFAULT_CHECK_CNT);
    }
}

OAL_STATIC void hmac_wur_ap_disable_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        /* 置上层bit位 */
        hmac_wur_set_wur_disable_bitmap_bit(WUR_CFGVENDOR_EVENT);

        /* 关闭wur enable，防止进快睡被该标记拦截 */
        hmac_wur_set_enabled(hmac_vap, OAL_FALSE);

        /* host teardown处理，本次连接不进入wur */
        hmac_wur_teardown_process(hmac_vap, hmac_user, COFEATURE_AP_DISABLE_EVENT);
    }
}

OAL_STATIC void hmac_wur_failure_detect_event_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype)
{
    if (event_subtype == COFEATURE_ENTER) {
        /* 置上层bit位 */
        hmac_wur_set_wur_disable_bitmap_bit(WUR_CFGVENDOR_EVENT);

        /* 关闭wur enable，防止进快睡被该标记拦截 */
        hmac_wur_set_enabled(hmac_vap, OAL_FALSE);

        /* 发送teardwon帧，host teardown处理，本次连接不进入wur */
        hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user, event_type);
    }
}

/* 处理device触发的wur teardown */
uint32_t hmac_wur_teardown_event_process(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    uint8_t reason;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wur_teardown_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    event = frw_get_event_stru(event_mem);
    reason = *(uint8_t *)event->auc_event_data;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_wur_teardown_event_process::hmac_vap[%d] null.}",
            event->st_event_hdr.uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (hmac_wur_is_negotiated(hmac_vap) == OAL_FALSE) {
        return OAL_SUCC;
    }
    hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, reason, COFEATURE_ENTER);

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_vap_state_change_notify_wur_check(hmac_vap_stru *hmac_vap, uint8_t event_type)
{
    /* wur定制化关闭，不处理通知wur的事件 */
    if (wlan_chip_get_wur_switch() == OAL_FALSE) {
        return OAL_FAIL;
    }

    /* 除多vap事件、emlsr事件以外，其他事件的处理必须是wur_enbaled的vap才处理 */
    if (hmac_wur_is_enabled(hmac_vap) == OAL_FALSE && event_type != COFEATURE_MVAP_EVENT &&
        event_type != COFEATURE_EMLSR_EVENT) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}
void hmac_vap_state_change_notify_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t event_type, uint8_t event_subtype)
{
    uint32_t ori_bitamp = g_wur_disable_bitmap;

    if (hmac_vap == NULL) {
        return;
    }

    if (hmac_vap_state_change_notify_wur_check(hmac_vap, event_type) != OAL_SUCC) {
        return;
    }

    switch (event_type) {
        case WUR_CFGVENDOR_EVENT:
            hmac_wur_cfgvendor_event_handle(hmac_vap, hmac_user, event_subtype);
            break;
        case COFEATURE_MVAP_EVENT:
            hmac_wur_mvap_event_handle(hmac_vap, event_subtype);
            break;
        case COFEATURE_ROAM_EVENT:
            hmac_wur_roam_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        case COFEATURE_BT_EVENT:
            hmac_wur_bt_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        case COFEATURE_EMLSR_EVENT:
            hmac_wur_emlsr_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        case COFEATURE_LINK_SWITCH_EVENT:
            hmac_wur_link_switch_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        case COFEATURE_DEL_USER_EVENT:
            hmac_wur_del_user_exit_wur(hmac_vap);
            break;
        case COFEATURE_HIGH_PPS_EVENT:
            hmac_wur_high_pps_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        case COFEATURE_PM_ENABLE_EVENT:
            hmac_wur_pm_enable_event_handle(hmac_vap, event_type, event_subtype);
            break;
        case COFEATURE_AP_DISABLE_EVENT:
            hmac_wur_ap_disable_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        case COFEATURE_FAILURE_DETECT_EVENT:
            hmac_wur_failure_detect_event_handle(hmac_vap, hmac_user, event_type, event_subtype);
            break;
        default:
            break;
    }

    /* bitmap同步到device */
    hmac_wur_bitmap_sync_notify(hmac_vap, event_type, event_subtype, ori_bitamp);
}

/* bt通知wifi，释放wur通路，允许进入wur */
uint32_t hmac_wur_bt_release_channel_event_process(frw_event_mem_stru *event_mem)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wur_bt_release_channel_event_process::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = hmac_find_up_wur_enabled_vap();
    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_wur_bt_release_channel_event_process::wur_enabled hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, COFEATURE_BT_EVENT, COFEATURE_EXIT);

    return OAL_SUCC;
}

#define HMAC_WUR_HIGH_PPS_TH    8
#define HMAC_WUR_LOW_PPS_TH     4
#define HMAC_WUR_HIGH_PPS_CNT   3
#define HMAC_WUR_LOW_PPS_CNT    10
OAL_STATIC void hmac_wur_vap_pps_proc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint32_t throughput)
{
    /* 进入高流量计数以及处理 */
    if (throughput > HMAC_WUR_HIGH_PPS_TH) {
        hmac_vap->wur_info.high_pps_enter_cnt++;
    } else {
        hmac_vap->wur_info.high_pps_enter_cnt = 0;
    }
    if (hmac_vap->wur_info.high_pps_enter_cnt > HMAC_WUR_HIGH_PPS_CNT) {
        hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, COFEATURE_HIGH_PPS_EVENT, COFEATURE_ENTER);
    }
    /* 退出高流量计数以及处理 */
    if (throughput < HMAC_WUR_LOW_PPS_TH) {
        hmac_vap->wur_info.high_pps_exit_cnt++;
    } else {
        hmac_vap->wur_info.high_pps_exit_cnt = 0;
    }
    if (hmac_vap->wur_info.high_pps_exit_cnt > HMAC_WUR_LOW_PPS_CNT) {
        hmac_vap_state_change_notify_feature(hmac_vap, hmac_user, COFEATURE_HIGH_PPS_EVENT, COFEATURE_EXIT);
    }
}

void hmac_wur_high_pps_check(uint32_t throughput)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;

    if (g_wlan_spec_cfg->high_pps_fastsleep_close == OAL_FALSE) {
        return;
    }

    hmac_vap = hmac_find_up_wur_vap();
    if (hmac_vap == NULL) {
        return;
    }

    hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        return;
    }

    hmac_wur_vap_pps_proc(hmac_vap, hmac_user, throughput);
}

/*
 * 功能描述  : process_wur_sync_event发送适配
 */
uint32_t hmac_proc_rx_process_wur_sync_event_tx_adapt(frw_event_mem_stru *event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(event_mem, sizeof(dmac_ctx_update_wur_stru));
}
#endif /* end of _PRE_WLAN_FEATURE_WUR */
