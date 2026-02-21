/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_frame_wur.c
 * 创建日期   : 2023年9月18日
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "oal_ext_if.h"
#include "mac_resource.h"
#include "mac_ie.h"
#include "securec.h"
#include "mac_frame_common.h"
#include "mac_mib.h"
#include "hmac_user.h"
#include "mac_frame_wur_ie.h"
#include "hmac_vap.h"
#ifdef _PRE_WLAN_FEATURE_WUR
#include "hmac_wur.h"
#endif
#include "wlan_chip_i.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_WUR_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/*****************************************************************************
  3 函数实现
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_WUR
/*
 * 函 数 名   : mac_set_wur_capabilities_ie
 * 功能描述   : 填充wur capabilities信息
 * 修改历史
 * 1.日    期 : 2023年9月18日
 *   修改内容 : 新生成函数
 */
void mac_set_wur_capabilities_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len)
{
    mac_frame_ie_ext_hdr *hdr;
    mac_frame_wur_cap_ie_stru *cap_ie;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, buffer, ie_len))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_he_capabilities_ie::input NULL.}");
        return;
    }

    if (mac_mib_get_WUROptionImplemented(mac_vap) != OAL_TRUE || wlan_chip_get_wur_switch() != OAL_TRUE) {
        return;
    }

    /****************************************************************
    -----------------------------------------------------------------
    |EID |Length |EID Extension|SUPPORTED BANDS |WUR Capabilities Info|
    -----------------------------------------------------------------
    |1   |1      |1            |1               |2                    |
    -----------------------------------------------------------------
    ****************************************************************/

    *ie_len = 0;

    hdr = (mac_frame_ie_ext_hdr *)buffer;
    hdr->element_id = MAC_EID_WUR;
    hdr->element_id_ext = MAC_EID_EXT_WUR_CAP;

    buffer += MAC_IE_EXT_HDR_LEN;
    *ie_len += MAC_IE_EXT_HDR_LEN;

    cap_ie = (mac_frame_wur_cap_ie_stru *)buffer;
    cap_ie->supported_bands.support_2g = OAL_TRUE;
    cap_ie->supported_bands.support_5g = MAC_WUR_5G_SUPPORTED;
    cap_ie->wur_cap_info.transition_delay = MAC_WUR_TRANSITION_DELAY; /* 从休眠到唤醒需要的最大时间，0：256us */
    cap_ie->wur_cap_info.vl_wur_frame_support = MAC_WUR_VL_FRAME_SUPPORTED;
    cap_ie->wur_cap_info.wur_group_ids_support = MAC_WUR_GROUP_IDS_SUPPORTED;
    cap_ie->wur_cap_info.hdr_support = MAC_WUR_HDR_SUPPORTED;
    cap_ie->wur_cap_info.fdma_chan_switch_support = mac_mib_get_WURFDMAChannelSwitchImplemented(mac_vap);
    cap_ie->wur_cap_info.short_wake_up_support = MAC_WUR_SHORT_WAKE_UP_SUPPORTED;

    *ie_len += sizeof(mac_frame_wur_cap_ie_stru);

    hdr->length = *ie_len - MAC_IE_HDR_LEN;
}

void mac_set_wur_operation_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *ie_len)
{
    mac_frame_ie_ext_hdr *hdr = NULL;
    mac_frame_wur_oper_ie_stru *oper_ie = NULL;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, buffer, ie_len))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_he_capabilities_ie::input NULL.}");
        return;
    }

    if (mac_mib_get_WUROptionImplemented(mac_vap) != OAL_TRUE || wlan_chip_get_wur_switch() != OAL_TRUE) {
        return;
    }
    /**************************************************
    --------------------------------------------------
    |EID |Length |EID Extension|WUR OPERATION PARAMS |
    --------------------------------------------------
    |1   |1      |1            |10                   |
    --------------------------------------------------
    ***************************************************/

    *ie_len = 0;

    hdr = (mac_frame_ie_ext_hdr *)buffer;
    hdr->element_id = MAC_EID_WUR;
    hdr->element_id_ext = MAC_EID_EXT_WUR_OPERATION;

    buffer += MAC_IE_EXT_HDR_LEN;
    *ie_len += MAC_IE_EXT_HDR_LEN;

    oper_ie = (mac_frame_wur_oper_ie_stru *)buffer;
    oper_ie->min_wake_up_duration = MAC_WUR_MIN_WAKEUP_DURATION;
    oper_ie->duty_cycle_period_units = MAC_WUR_DUTY_CYCLE_PERIOD_UNITS;
    oper_ie->wur_operating_class = MAC_GLOBAL_OPERATING_CLASS_NUMBER81;
    oper_ie->wur_channel = (mac_vap->st_channel.en_band == WLAN_BAND_2G) ?
        mac_vap->st_channel.uc_chan_number : MAC_WUR_DEFAULT_CHANNEL; /* vap在2G时wur用同信道，否则wur用默认信道 */
    oper_ie->wur_beacon_period = mac_mib_get_WURBeaconPeriod(mac_vap);
    oper_ie->offset_of_twbtt = MAC_WUR_DEFAULT_OFFSET_OF_TWBTT;
    oper_ie->counter = 0;
    oper_ie->common_ipn = OAL_FALSE;

    *ie_len += sizeof(mac_frame_wur_oper_ie_stru);

    hdr->length = *ie_len - MAC_IE_HDR_LEN;
}

uint16_t mac_set_wur_mode_ie_sta(mac_vap_stru *mac_vap, uint8_t *buffer,
    uint8_t action_type, uint8_t resp_status)
{
    mac_frame_wur_mode_fixed_ie_stru *fixed_ie = NULL;
    wur_sta_params_stru *sta_ie = NULL;
    recommended_wur_params_stru *rec_param = NULL;
    uint8_t *origin_buffer = buffer;
    uint16_t len = 0;
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);

    if ((is_sta(mac_vap) != OAL_TRUE) || (hmac_vap == NULL)) {
        return 0;
    }

    /********************************************************************************************************
      -------------------------------------------------------------------------------------------------------
      |EID |Length |EID Extension|Action Type |WUR Mode Response Status|WUR Parameters Control|WUR Paramters|
      -------------------------------------------------------------------------------------------------------
      |  1 |   1   |      1      |      1     |           1            |          1           |   varible   |
      -------------------------------------------------------------------------------------------------------
    ********************************************************************************************************/
    fixed_ie = (mac_frame_wur_mode_fixed_ie_stru *)buffer;
    fixed_ie->action_type = action_type;
    fixed_ie->wur_mode_resp_status = resp_status; /* wur_mode_resp_status_enum */
    fixed_ie->wur_params_control.wur_duty_cycle_start_timer_present = OAL_FALSE;
    fixed_ie->wur_params_control.wur_group_id_list_present = OAL_FALSE;
    fixed_ie->wur_params_control.recommended_wur_params_present = OAL_FALSE; /* sta默认不会携带 推荐的wur参数 */
    buffer += sizeof(mac_frame_wur_mode_fixed_ie_stru);

    /****************************************************************
     WUR Paramters:
             --------------------------------------------------------
             |On Duration |Duty Cycle Period |Recommended WUR Params|
             --------------------------------------------------------
     Octets: |  4         |         2        |       0 or 1         |
             --------------------------------------------------------
    *****************************************************************/
    sta_ie = (wur_sta_params_stru *)buffer;
    sta_ie->on_duration = hmac_vap->wur_info.cfg_info.on_duration;
    sta_ie->duty_cycle_period = hmac_vap->wur_info.duty_cycle_period_num;
    buffer += sizeof(wur_sta_params_stru);

    /***********************************************************************************
     Recommended WUR Params:
           -----------------------------------------------------------------------------
           |Recommended WUR Wake-up Frame Rate |Recommended WUR Channel Offset|Reserved|
           ----------------------------------------------------------------------------
     Bits: |                2                  |              3               |   3    |
           -----------------------------------------------------------------------------
    ***********************************************************************************/
    if (fixed_ie->wur_params_control.recommended_wur_params_present == OAL_TRUE) {
        rec_param = (recommended_wur_params_stru *)buffer;
        rec_param->recommended_wur_wake_up_frame_rate = WUR_WAKE_UP_FRAME_RATE_HDR;
        rec_param->recommended_wur_channel_offset = WUR_WAKE_UP_FRAME_IN_PRIMARY_CHANNEL;
        buffer += sizeof(recommended_wur_params_stru);
    }

    len = (uint16_t)(buffer - origin_buffer);
    return len;
}

uint16_t mac_set_wur_mode_ie_ap(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *buffer,
    uint8_t action_type, uint8_t resp_status)
{
    mac_frame_wur_mode_fixed_ie_stru *fixed_ie = NULL;
    wur_ap_params_stru *ap_ie = NULL;
    wur_group_id_list_stru *list = NULL;
    uint8_t *origin_buffer = buffer;
    uint16_t len = 0;

    if (is_ap(mac_vap) != OAL_TRUE || resp_status != WUR_ACCEPT) {
        return 0;
    }

    /********************************************************************************************************
      -------------------------------------------------------------------------------------------------------
      |EID |Length |EID Extension|Action Type |WUR Mode Response Status|WUR Parameters Control|WUR Paramters|
      -------------------------------------------------------------------------------------------------------
      |  1 |   1   |      1      |      1     |           1            |          1           |   varible   |
      -------------------------------------------------------------------------------------------------------
    *******************************************************************************************************/
    fixed_ie = (mac_frame_wur_mode_fixed_ie_stru *)buffer;
    fixed_ie->action_type = action_type;
    fixed_ie->wur_mode_resp_status = resp_status; /* wur_mode_resp_status_enum */
    fixed_ie->wur_params_control.wur_duty_cycle_start_timer_present = OAL_TRUE;
    fixed_ie->wur_params_control.wur_group_id_list_present = OAL_TRUE;
    fixed_ie->wur_params_control.recommended_wur_params_present = OAL_FALSE;
    buffer += sizeof(mac_frame_wur_mode_fixed_ie_stru);

    /*************************************************************************************
     WUR Paramters:
           -------------------------------------------------------------------------------
           |WUR ID |WUR CHANNEL OFFSET |RESERVED|DUTY CYCLE START TIME |WUR GROUP ID LIST|
           -------------------------------------------------------------------------------
     bits: |  12   |         3         |    1   |         64           |     variable    |
           -------------------------------------------------------------------------------
    *************************************************************************************/
    ap_ie = (wur_ap_params_stru *)buffer;
    ap_ie->wur_id = (uint8_t)(hmac_user->st_user_base_info.us_assoc_id * MAC_WUR_ID_EXPAND_NUM); /* 默认等于user_id*2 */
    ap_ie->wur_channel_offset = WUR_WAKE_UP_FRAME_IN_PRIMARY_CHANNEL;
    ap_ie->wur_duty_cycle_start_time = 0; /* TODO:如何确认填多少 */
    buffer += sizeof(wur_ap_params_stru);

    /**************************************************************************************
      WUR GROUP ID LIST:
            -------------------------------------------------------------------------------
            |WUR GROUP ID BITMAP SIZE |STARTING WUR GROUP ID | WUR GROUP ID BITMAP        |
            -------------------------------------------------------------------------------
      bits: |            4            |          12          |variable（和BITMAP SIZE相关）|
            -------------------------------------------------------------------------------
    **************************************************************************************/
    if (fixed_ie->wur_params_control.wur_group_id_list_present == OAL_TRUE) {
        list = (wur_group_id_list_stru *)buffer;
        list->wur_group_id_bitmap_size = WUR_GROUP_ID_BITMAP_SIZE_0; /* 不带group id bitmap */
        list->starting_wur_group_id =
          (uint8_t)(hmac_user->st_user_base_info.us_assoc_id + WUR_GROUP_ID_OFFSET); /* 默认每个sta都在不同组 */
        buffer += sizeof(wur_group_id_list_stru);
    }

    len = (uint16_t)(buffer - origin_buffer);
    return len;
}

uint16_t mac_set_wur_mode_ie(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *buffer,
    uint8_t action_type, uint8_t resp_status)
{
    mac_frame_ie_ext_hdr *hdr = NULL;
    uint16_t ie_len = 0;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, buffer))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_he_capabilities_ie::input NULL.}");
        return 0;
    }

    if (mac_mib_get_WUROptionImplemented(mac_vap) != OAL_TRUE || wlan_chip_get_wur_switch() != OAL_TRUE) {
        return 0;
    }

    /********************************************************************************************************
      -------------------------------------------------------------------------------------------------------
      |EID |Length |EID Extension|Action Type |WUR Mode Response Status|WUR Parameters Control|WUR Paramters|
      -------------------------------------------------------------------------------------------------------
      |  1 |   1   |      1      |      1     |           1            |          1           |   varible   |
      -------------------------------------------------------------------------------------------------------
    *******************************************************************************************************/

    hdr = (mac_frame_ie_ext_hdr *)buffer;
    hdr->element_id = MAC_EID_WUR;
    hdr->element_id_ext = MAC_EID_EXT_WUR_MODE;

    buffer += MAC_IE_EXT_HDR_LEN;
    ie_len += MAC_IE_EXT_HDR_LEN;

    if (is_sta(mac_vap)) {
        ie_len += mac_set_wur_mode_ie_sta(mac_vap, buffer, action_type, resp_status);
    }

    if (is_ap(mac_vap)) {
        ie_len += mac_set_wur_mode_ie_ap(mac_vap, hmac_user, buffer, action_type, resp_status);
    }

    hdr->length = ie_len - MAC_IE_HDR_LEN;
    return ie_len;
}

uint16_t mac_encap_wur_ie(mac_vap_stru *mac_vap, uint8_t *buffer, wlan_frame_mgmt_subtype_enum mgmt_subtype)
{
    uint8_t  ie_len = 0;
    uint16_t total_ie_len;
    uint8_t  *buffer_tmp = NULL;

    buffer_tmp = buffer;
    switch (mgmt_subtype) {
        case WLAN_BEACON:
        case WLAN_PROBE_RSP:
        case WLAN_ASSOC_RSP:
        case WLAN_REASSOC_RSP:
            /* 填充wur capabilities信息 */
            mac_set_wur_capabilities_ie(mac_vap, buffer_tmp, &ie_len);
            buffer_tmp += ie_len;

            /* 填充wur operation信息 */
            mac_set_wur_operation_ie(mac_vap, buffer_tmp, &ie_len);
            buffer_tmp += ie_len;
            break;

        case WLAN_PROBE_REQ:
        case WLAN_ASSOC_REQ:
        case WLAN_REASSOC_REQ:
             /* 填充wur capabilities信息 */
            mac_set_wur_capabilities_ie(mac_vap, buffer_tmp, &ie_len);
            buffer_tmp += ie_len;
            break;

        default:
            break;
        }

    total_ie_len = (uint16_t)(buffer_tmp - buffer);
    return total_ie_len;
}

uint16_t hmac_mgmt_encap_asoc_rsp_ap_set_wur_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user,
    uint8_t *buffer)
{
    if (mac_user_is_wur_user(mac_user) != OAL_TRUE) {
        return 0;
    }

    return mac_encap_wur_ie(mac_vap, buffer, WLAN_ASSOC_RSP);
}

OAL_STATIC uint32_t mac_parse_wur_capabilities_info_handle(mac_vap_stru *mac_vap,
    mac_frame_wur_cap_ie_stru *wur_cap_ie, mac_frame_wur_cap_ie_stru *wur_cap_info)
{
    uint8_t cap_len = sizeof(mac_frame_wur_cap_ie_stru);

    if (memcpy_s((uint8_t *)wur_cap_info, cap_len, (uint8_t *)wur_cap_ie, cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_wur_capabilities_info_handle::memory copy failed.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_parse_wur_capabilities_ie(mac_vap_stru *mac_vap, uint8_t *wur_cap_ie,
    mac_frame_wur_cap_ie_stru *wur_cap_info)
{
    mac_frame_wur_cap_ie_stru cap_ie;
    uint8_t *buffer = NULL;
    uint8_t len = 0;
    uint8_t cap_len = sizeof(mac_frame_wur_cap_ie_stru);

    len = wur_cap_ie[1];
    if (len != MAC_WUR_CAP_IE_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_parse_wur_capabilities_ie::invalid len[%d].}", len);
        return OAL_FAIL;
    }

    buffer = wur_cap_ie + MAC_IE_EXT_HDR_LEN;

    memset_s(&cap_ie, sizeof(mac_frame_wur_cap_ie_stru), 0, sizeof(mac_frame_wur_cap_ie_stru));
    /* 获取 WUR MAC Capabilities Info */
    if (memcpy_s((uint8_t *)(&cap_ie), cap_len, buffer, cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_wur_capabilities_ie::memory copy failed.}");
        return OAL_FAIL;
    }

    /* 取cap交集，存到wur_cap_info */
    if (mac_parse_wur_capabilities_info_handle(mac_vap, &cap_ie, wur_cap_info) != OAL_SUCC) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_process_wur_capabilities_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user, uint8_t *wur_cap_ie)
{
    mac_wur_hdl_stru mac_wur_hdl;
    mac_frame_wur_cap_ie_stru wur_cap_info;
    uint8_t cap_len = sizeof(mac_frame_wur_cap_ie_stru);

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, mac_user, wur_cap_ie))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_wur_capabilities_ie::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(&mac_wur_hdl, sizeof(mac_wur_hdl_stru), 0, sizeof(mac_wur_hdl_stru));
    memset_s(&wur_cap_info, cap_len, 0, cap_len);

    /* 解析 WUR Capabilities Info */
    if (mac_parse_wur_capabilities_ie(mac_vap, wur_cap_ie, &wur_cap_info) != OAL_SUCC) {
        return OAL_FAIL;
    }

    mac_user_get_wur_hdl(mac_user, &mac_wur_hdl);

    if (memcpy_s(&mac_wur_hdl.wur_cap_ie, cap_len, &wur_cap_info, cap_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_wur_capabilities_ie::memory copy failed.}");
        return OAL_FAIL;
    }

    mac_user_set_wur_hdl(mac_user, &mac_wur_hdl);

    /* 对端支持2g wur，标记对端支持 */
    if (wur_cap_info.supported_bands.support_2g == OAL_TRUE) {
        mac_user_set_wur_capable(mac_user, OAL_TRUE);
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t mac_parse_wur_operation_ie(mac_vap_stru *mac_vap, uint8_t *wur_oper_ie,
    mac_frame_wur_oper_ie_stru *wur_oper_info)
{
    uint8_t *buffer = NULL;
    uint8_t len = 0;
    uint8_t oper_len = sizeof(mac_frame_wur_oper_ie_stru);

    len = wur_oper_ie[1];
    if (len != MAC_WUR_OPER_IE_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_parse_wur_operation_ie::invalid len[%d].}", len);
        return OAL_FAIL;
    }

    buffer = wur_oper_ie + MAC_IE_EXT_HDR_LEN;

    /* 获取 wur oper Info */
    if (memcpy_s(wur_oper_info, oper_len, buffer, oper_len) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_wur_operation_ie::memory copy failed.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/* sta保存ap的wur operation参数到vap下 */
OAL_STATIC void hmac_sta_save_wur_operation(mac_vap_stru *mac_vap, mac_frame_wur_oper_ie_stru *wur_oper)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return;
    }

    hmac_vap->wur_info.min_wake_up_duration = wur_oper->min_wake_up_duration;
    hmac_vap->wur_info.duty_cycle_period_units = wur_oper->duty_cycle_period_units;
    hmac_vap->wur_info.cfg_info.wur_channel = wur_oper->wur_channel;
    hmac_vap->wur_info.wur_beacon_period = wur_oper->wur_beacon_period;
    hmac_vap->wur_info.offset_of_twbtt = wur_oper->offset_of_twbtt;
}

uint32_t mac_process_wur_operation_ie(mac_vap_stru *mac_vap, mac_user_stru *mac_user, uint8_t *wur_oper_ie)
{
    mac_frame_wur_oper_ie_stru wur_oper;
    mac_wur_hdl_stru wur_hdl;
    uint8_t oper_len = sizeof(mac_frame_wur_oper_ie_stru);
    uint32_t ret = MAC_NO_CHANGE;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, mac_user, wur_oper_ie))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_wur_operation_ie::param null}");
        return ret;
    }

    memset_s(&wur_oper, oper_len, 0, oper_len);

    /* 解析 WUR Operation Info */
    if (mac_parse_wur_operation_ie(mac_vap, wur_oper_ie, &wur_oper) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_wur_operation_ie::parse wur opern ie FAIL!}");
        return ret;
    }

    mac_user_get_wur_hdl(mac_user, &wur_hdl);

    memcpy_s(&wur_hdl.wur_oper_ie, oper_len, &wur_oper, oper_len);

    mac_user_set_wur_hdl(mac_user, &wur_hdl);

    mac_user_set_wur_operation(mac_user, OAL_TRUE);

    /* sta保存ap的wur operation参数到vap下 */
    if (is_sta(mac_vap)) {
        hmac_sta_save_wur_operation(mac_vap, &wur_oper);
    }

    return ret;
}

/* ap处理wur setup req，需要回复wur setup resp */
uint32_t mac_process_wur_mode_req(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *buffer)
{
    mac_frame_wur_mode_fixed_ie_stru *fixed_ie = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    wur_sta_params_stru *wur_params = NULL;
    mac_wur_action_mgmt_args_stru wur_action_args;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_FAIL;
    }

    fixed_ie = (mac_frame_wur_mode_fixed_ie_stru *)buffer;

    buffer += sizeof(mac_frame_wur_mode_fixed_ie_stru);

    wur_params = (wur_sta_params_stru *)buffer;
    hmac_vap->wur_info.cfg_info.on_duration = wur_params->on_duration;
    hmac_vap->wur_info.duty_cycle_period_num = wur_params->duty_cycle_period;

    if (fixed_ie->wur_params_control.recommended_wur_params_present == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{mac_process_wur_mode_req::has recommend params but not parse!}");
    }

    wur_action_args.category = MAC_ACTION_CATEGORY_WUR;
    wur_action_args.action = MAC_WUR_ACTION_WUR_MODE_SETUP;
    wur_action_args.action_type = ENTER_WUR_MODE_RESP;
    wur_action_args.resp_status = WUR_ACCEPT;

    /* setup已协商 */
    hmac_vap->wur_info.cfg_info.negotiated = OAL_TRUE;

    hmac_wur_update_parameters_to_device(hmac_vap, hmac_user, MAC_WUR_ACTION_WUR_MODE_SETUP);

    /* 回复wur setup resp */
    hmac_mgmt_tx_wur_action(hmac_vap, hmac_user, &wur_action_args);
    return OAL_SUCC;
}

static uint32_t hmac_wur_calc_compressed_id(uint8_t *bssid, uint16_t bssid_len)
{
    return calc_crc32(bssid, bssid_len);
}

/* sta处理ap发的setup resp中的wur mode ie */
uint32_t mac_process_wur_mode_resp(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *buffer)
{
    mac_frame_wur_mode_fixed_ie_stru *fixed_ie = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    wur_ap_params_stru *wur_params = NULL;
    wur_group_id_list_stru *groud_id_list = NULL;
    mac_cfg_wur_stru *cfg_info = NULL;
    uint32_t compressed_id = 0;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_FAIL;
    }

    cfg_info = &(hmac_vap->wur_info.cfg_info);

    fixed_ie = (mac_frame_wur_mode_fixed_ie_stru *)buffer;

    /* ap拒绝，不建立wur会话，不配置wur参数到dmac */
    if (fixed_ie->wur_mode_resp_status != WUR_ACCEPT) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_process_wur_mode_resp::reject![%d]}", fixed_ie->wur_mode_resp_status);
        return OAL_FAIL;
    }

    buffer += sizeof(mac_frame_wur_mode_fixed_ie_stru);

    wur_params = (wur_ap_params_stru *)buffer;

    cfg_info->wur_id = wur_params->wur_id;
    if (fixed_ie->wur_params_control.wur_duty_cycle_start_timer_present == OAL_TRUE) {
        cfg_info->duty_cycle_start_time = wur_params->wur_duty_cycle_start_time;
    }

    buffer += sizeof(wur_ap_params_stru);

    groud_id_list = (wur_group_id_list_stru *)buffer;
    /* 配置start group id和group id bitmap */
    if (fixed_ie->wur_params_control.wur_group_id_list_present == OAL_TRUE) {
        buffer += sizeof(wur_group_id_list_stru);
        cfg_info->start_group_id = groud_id_list->starting_wur_group_id;
        if (groud_id_list->wur_group_id_bitmap_size == MAC_WUR_GROUP_ID_BITMAP_SIZE_0BIT) {
            cfg_info->group_id_bitmap = 0x1; /* 不支持sta多个group id，默认BIT0置位 */
        } else if (groud_id_list->wur_group_id_bitmap_size == MAC_WUR_GROUP_ID_BITMAP_SIZE_16BIT) {
            cfg_info->group_id_bitmap = *((uint16_t *)buffer);
        } else if (groud_id_list->wur_group_id_bitmap_size == MAC_WUR_GROUP_ID_BITMAP_SIZE_32BIT) {
            cfg_info->group_id_bitmap = *((uint32_t *)buffer);
        } else if (groud_id_list->wur_group_id_bitmap_size == MAC_WUR_GROUP_ID_BITMAP_SIZE_64BIT) {
            cfg_info->group_id_bitmap = *((uint64_t *)buffer);
        }
    } else {
        cfg_info->start_group_id = 0;
        cfg_info->group_id_bitmap = 0;
    }

    /* transmit_id、embedded_bssid不在帧内携带，通过ap的bssid计算而来 */
    compressed_id = hmac_wur_calc_compressed_id(mac_vap->auc_bssid, WLAN_MAC_ADDR_LEN);
    cfg_info->transmit_id = compressed_id & 0xFFF;
    cfg_info->embedded_bssid = (compressed_id & 0xFFFF0000) >> 16; // compressed_id的高16bit是embedded bssid

    /* 配置duty_cycle_period */
    cfg_info->duty_cycle_period = hmac_vap->wur_info.duty_cycle_period_units * hmac_vap->wur_info.duty_cycle_period_num;

    /* 配置wur工作的时候是否always on */
    cfg_info->is_always_on = (cfg_info->on_duration == cfg_info->duty_cycle_period) ? OAL_TRUE : OAL_FALSE;

    /* ap回复的setup resp帧中resp_status为accept，标记已协商 */
    cfg_info->negotiated = OAL_TRUE;

    return OAL_SUCC;
}

uint32_t mac_process_wur_mode_ie(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *wur_mode_ie)
{
    uint8_t len = 0;
    uint32_t ret = OAL_SUCC;
    uint8_t *buffer = NULL;
    mac_frame_wur_mode_fixed_ie_stru *fixed_ie = NULL;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, hmac_user, wur_mode_ie))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_process_wur_capabilities_ie::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    len = wur_mode_ie[1];
    if (is_sta(mac_vap) && len < MAC_WUR_MODE_AP_MIN_IE_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_process_wur_mode_ie::invalid len[%d].}", len);
        return OAL_FAIL;
    }

    if (is_ap(mac_vap) && len < MAC_WUR_MODE_STA_MIN_IE_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_process_wur_mode_ie::invalid len[%d].}", len);
        return OAL_FAIL;
    }

    buffer = wur_mode_ie + MAC_IE_EXT_HDR_LEN;
    fixed_ie = (mac_frame_wur_mode_fixed_ie_stru *)buffer;
    switch (fixed_ie->action_type) {
        case ENTER_WUR_MODE_REQ:
            ret = mac_process_wur_mode_req(mac_vap, hmac_user, buffer);
            break;
        case ENTER_WUR_MODE_RESP:
            ret = mac_process_wur_mode_resp(mac_vap, hmac_user, buffer);
            break;
        case ENTER_WUR_MODE_SUSPEND_REQ:
            break;
        case ENTER_WUR_MODE_SUSPEND_RESP:
            break;
        case ENTER_WUR_MODE_SUSPEND:
            break;
        case ENTER_WUR_MODE:
            break;

        default:
            break;
    }

    return ret;
}

/* 解析(re)assoc req/resp帧中的wur ie
   wur mode ie暂不在assoc帧中携带，wur mode ie仅在wur setup帧中携带 */
uint32_t hmac_process_wur_ie(mac_vap_stru *mac_vap,
    hmac_user_stru *hmac_user, uint8_t *payload, uint32_t msg_len)
{
    uint8_t *wur_cap_ie = NULL;
    uint8_t *wur_oper_ie = NULL;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, hmac_user, payload))) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_he_capabilities_ie::input NULL.}");
        return OAL_FAIL;
    }

    if (mac_mib_get_WUROptionImplemented(mac_vap) != OAL_TRUE || wlan_chip_get_wur_switch() != OAL_TRUE) {
        return OAL_SUCC;
    }

    /* 更新WUR Capabilities ie */
    wur_cap_ie = mac_find_ie_ext_ie_in_frame(MAC_EID_WUR, MAC_EID_EXT_WUR_CAP, payload, (int32_t)msg_len);
    if (wur_cap_ie != NULL) {
        mac_process_wur_capabilities_ie(mac_vap, &hmac_user->st_user_base_info, wur_cap_ie);
    }

    /* 更新WUR Operation ie */
    wur_oper_ie = mac_find_ie_ext_ie_in_frame(MAC_EID_WUR, MAC_EID_EXT_WUR_OPERATION, payload, (int32_t)msg_len);
    if (wur_oper_ie != NULL) {
        mac_process_wur_operation_ie(mac_vap, &hmac_user->st_user_base_info, wur_oper_ie);
    }

    return OAL_SUCC;
}


#endif /* end of _PRE_WLAN_FEATURE_WUR */
