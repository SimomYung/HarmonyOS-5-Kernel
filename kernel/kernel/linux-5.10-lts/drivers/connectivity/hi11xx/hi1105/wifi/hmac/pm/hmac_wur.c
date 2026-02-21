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

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_WUR_C

#ifdef _PRE_WLAN_FEATURE_WUR
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
    return (wlan_chip_get_wur_switch() == OAL_TRUE && hmac_vap->wur_info.cfg_info.negotiated == OAL_TRUE);
}

uint8_t hmac_wur_is_wur_enable(hmac_vap_stru *hmac_vap)
{
    return hmac_vap->wur_info.wur_enable;
}

void hmac_wur_set_wur_enable(hmac_vap_stru *hmac_vap, uint8_t val)
{
    hmac_vap->wur_info.wur_enable = val;
    oam_warning_log1(0, OAM_SF_PWR, "hmac_wur_set_wur_enable::wur_enable[%d]", hmac_vap->wur_info.wur_enable);
}

/* 找到协商过wur的sta所在hmac_vap */
hmac_vap_stru *hmac_find_up_wur_vap(void)
{
    uint8_t vap_idx;
    hmac_vap_stru *hmac_vap = NULL;
    mac_vap_stru *mac_vap = NULL;
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
        mac_vap = &hmac_vap->st_vap_base_info;
        if (hmac_wur_is_negotiated(hmac_vap) == OAL_TRUE) {
            return hmac_vap;
        }
    }

    return NULL;
}

OAL_STATIC void hmac_sta_wur_mode_param_init(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_cfg_wur_setup_param_stru *wur_setup)
{
    mac_vap_stru *mac_vap = &hmac_vap->st_vap_base_info;
    uint32_t duty_cycle_period = 0;
    mac_wur_stru *wur_info = &(hmac_vap->wur_info);

    if (wur_info->duty_cycle_period_units == 0) {
        return;
    }
    if (wur_setup->is_always_on == OAL_TRUE) {
        wur_info->cfg_info.on_duration = 0;
        wur_info->duty_cycle_period_num = 0;
    } else {
        duty_cycle_period = (mac_mib_get_BeaconPeriod(mac_vap) * wur_setup->count * HMAC_MS_TO_US);
        /* 100：说明service time占比多少百分比 */
        wur_info->cfg_info.on_duration = duty_cycle_period * wur_setup->ratio / 100 / MAC_WUR_TIME_4US_UNITS;
        wur_info->duty_cycle_period_num =
            (duty_cycle_period / MAC_WUR_TIME_4US_UNITS / wur_info->duty_cycle_period_units);
    }

    oam_warning_log4(0, OAM_SF_ANY,
        "{hmac_sta_wur_mode_param_init::period_units[%d], period_num[%d], period=[%d] service_time=[%d]}",
        wur_info->duty_cycle_period_units, wur_info->duty_cycle_period_num,
        duty_cycle_period, wur_info->cfg_info.on_duration);
}

OAL_STATIC uint32_t hmac_check_wur_setup_conditions(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    mac_device_stru *mac_device = mac_res_get_dev(0);
    uint8_t vap_id = 0;
    if (hmac_vap == NULL) {
        oam_error_log0(vap_id, OAM_SF_ANY, "{hmac_check_wur_setup_conditions::hmac_vap null}");
        return OAL_FAIL;
    }
    vap_id = hmac_vap->st_vap_base_info.uc_vap_id;

    if (hmac_wur_is_wur_enable(hmac_vap) == OAL_FALSE) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_check_wur_setup_conditions::wur not enable}");
        return OAL_FAIL;
    }

    /* 判断该AP是否支持wur */
    if (mac_user_is_wur_user(&hmac_user->st_user_base_info) == OAL_FALSE) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_check_wur_setup_conditions::ap dont support wur}");
        return OAL_FAIL;
    }

    /* 多vap时不进入wur */
    if (mac_device_calc_up_vap_num(mac_device) > 1) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_check_wur_setup_conditions::multivap dont enter wur}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

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

    return OAL_SUCC;
}

/* 退出wur协商标记 */
void hmac_wur_exit(hmac_vap_stru *hmac_vap)
{
    hmac_vap->wur_info.cfg_info.negotiated = OAL_FALSE;
}

/* 去关联，去初始化wur */
void hmac_wur_deinit(hmac_vap_stru *hmac_vap)
{
    memset_s(&hmac_vap->wur_info, sizeof(mac_wur_stru), 0, sizeof(mac_wur_stru));
}

uint32_t hmac_rx_wur_teardown_frame(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, uint16_t payload_len)
{
    /* 清掉已协商标记 */
    hmac_wur_exit(hmac_vap);

    return hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_TEARDOWN);
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
    mac_get_cb_wlan_frame_type(tx_ctl) = WLAN_MANAGEMENT;
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

uint32_t hmac_mgmt_tx_wur_teardown(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
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

    /* 清掉已协商标记 */
    hmac_wur_exit(hmac_vap);

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
                hmac_mgmt_tx_wur_teardown(hmac_vap, hmac_user);
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

    /* wur定制化关闭，不处理wur ie */
    if (wlan_chip_get_wur_switch() != OAL_TRUE) {
        return OAL_FAIL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_vap_get_hmac_user_by_addr(mac_vap, wur_setup->mac_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        return OAL_FAIL;
    }

    /* 命令触发wur协商，默认使能wur */
    hmac_wur_set_wur_enable(hmac_vap, OAL_TRUE);

    /* 检查发送wur setup 需要满足的条件 */
    ret = hmac_check_wur_setup_conditions(hmac_vap, hmac_user);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* 初始化sta自己的param */
    hmac_sta_wur_mode_param_init(hmac_vap, hmac_user, wur_setup);

    wlan_pm_set_timeout(1);

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

    /* 未协商完参数，不能teardown */
    if (hmac_vap->wur_info.cfg_info.negotiated == OAL_TRUE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_wur_teardown::not negotiated.}");
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
    mac_cfg_wur_fastsleep_param_stru *fastsleep_params)
{
    wlan_pm_set_timeout(fastsleep_params->host_pm_timeout);
    hmac_vap->wur_info.cfg_info.pm_timeout = fastsleep_params->pm_timeout;
    hmac_vap->wur_info.cfg_info.psm_max_timer_restart_cnt = fastsleep_params->psm_max_timer_restart_cnt;
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_config_set_wur_sleep_mode(hmac_vap_stru *hmac_vap,
    uint32_t sleep_mode)
{
    hmac_vap->wur_info.cfg_info.sleep_mode = (uint32_t)sleep_mode;
    oam_warning_log1(0, OAM_SF_11AX, "{hmac_config_set_wur_sleep_mode::sleep_mode[%d] 0:deepsleep 1:lightsleep",
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
        hmac_config_set_wur_fastsleep_params(hmac_vap, &wur_debug->fastsleep_params);
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

void hmac_wur_roam_connect_exit_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    if (hmac_wur_is_negotiated(hmac_vap) != OAL_TRUE) {
        return;
    }

    /* 清掉已协商标记 */
    hmac_wur_exit(hmac_vap);

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_TEARDOWN);
}

void hmac_wur_roam_to_old_bss_recover_wur(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    if (hmac_wur_is_negotiated(hmac_vap) != OAL_TRUE) {
        return;
    }

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_SETUP);
}

/* 其他vap入网、启动时，sta 退出wur会话，发起wur teardown */
void hmac_wur_mvap_exit_wur(mac_vap_stru *mac_vap)
{
    hmac_vap_stru *hmac_wur_vap = NULL;
    hmac_user_stru *hmac_wur_user = NULL;

    hmac_wur_vap = hmac_find_up_wur_vap();
    if (hmac_wur_vap == NULL) {
        return;
    }

    if (mac_vap->uc_vap_id == hmac_wur_vap->st_vap_base_info.uc_vap_id) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_wur_mvap_exit_wur::wrong state}");
        return;
    }

    hmac_wur_user = mac_res_get_hmac_user(hmac_wur_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_wur_user == NULL) {
        return;
    }

    hmac_mgmt_tx_wur_teardown(hmac_wur_vap, hmac_wur_user);
}

/* 删除入参vap，判断是否有up sta可以重新建立wur会话，可以的话发起wur setup */
void hmac_wur_del_vap_enter_wur(mac_vap_stru *mac_vap)
{
    hmac_vap_stru *hmac_sta_vap = NULL;
    mac_vap_stru *mac_sta_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    mac_device_stru *mac_device = NULL;
    mac_cfg_wur_setup_param_stru wur_setup;

    mac_device = mac_res_get_dev(0);
    if (mac_device == NULL) {
        return;
    }

    mac_device_find_up_sta(mac_device, &mac_sta_vap);
    if (mac_sta_vap == NULL || mac_vap->uc_vap_id == mac_sta_vap->uc_vap_id) {
        return;
    }
    hmac_sta_vap = mac_res_get_hmac_vap(mac_sta_vap->uc_vap_id);

    hmac_user = mac_res_get_hmac_user(mac_sta_vap->us_assoc_vap_id);
    if (hmac_user == NULL) {
        return;
    }

    /* 检查发送wur setup 需要满足的条件 */
    if (hmac_check_wur_setup_conditions(hmac_sta_vap, hmac_user) != OAL_SUCC) {
        return;
    }

    /* 如果已经有wur参数了，使用上一次的wur参数，否则使用默认参数 */
    if (hmac_sta_vap->wur_info.cfg_info.is_always_on == OAL_FALSE &&
        hmac_sta_vap->wur_info.cfg_info.on_duration != 0 &&
        hmac_sta_vap->wur_info.duty_cycle_period_num != 0) {
        hmac_mgmt_tx_wur_setup(hmac_sta_vap, hmac_user);
    } else {
        memset_s(&wur_setup, sizeof(mac_cfg_wur_setup_param_stru), 0, sizeof(mac_cfg_wur_setup_param_stru));
        wur_setup.is_always_on = OAL_FALSE;
        wur_setup.count = MAC_WUR_DEFAULT_DUTY_CYCLE_PERIOD;
        wur_setup.ratio = MAC_WUR_DEFAULT_SERVICE_TIME_RATID;

        hmac_sta_wur_mode_param_init(hmac_sta_vap, hmac_user, &wur_setup);

        hmac_mgmt_tx_wur_setup(hmac_sta_vap, hmac_user);
    }
}
/*
 * 功能描述  : process_wur_sync_event发送适配
 */
uint32_t hmac_proc_rx_process_wur_sync_event_tx_adapt(frw_event_mem_stru *event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(event_mem, sizeof(dmac_ctx_update_wur_stru));
}
#endif /* end of _PRE_WLAN_FEATURE_WUR */
