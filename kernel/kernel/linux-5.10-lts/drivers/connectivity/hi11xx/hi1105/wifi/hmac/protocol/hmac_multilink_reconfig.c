/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Multi-Link Reconfig特性处理
 * 作    者 : wifi
 * 创建日期 : 2024年2月8日
 */

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
#include "hmac_mgmt_sta.h"
#include "hmac_mld_ext.h"
#include "hmac_multilink.h"
#include "hmac_multilink_reconfig.h"
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_function.h"
#include "hmac_chba_frame.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MULTILINK_RECONFIG_C

#ifdef _PRE_WLAN_FEATURE_ML
OAL_STATIC uint32_t hmac_ml_remove_link_vap_timeout_handle(void *arg)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)arg;
    if (hmac_vap == NULL) {
        return OAL_FAIL;
    }
    oam_warning_log0(0, OAM_SF_CFG, "{hmac_ml_remove_link_vap_timeout_handle}");
    return hmac_mld_delete_link(&hmac_vap->st_vap_base_info);
}

uint32_t hmac_ml_reconfig_remove_link(mac_vap_stru *mac_vap, uint16_t tbtt_num, hmac_vap_stru *delete_hmac_vap)
{
    mac_vap_stru *delete_mac_vap = NULL;
    uint32_t timeout;

    if (mac_vap == NULL || delete_hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_prepare_remove_link_ap::mac_vap or param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    delete_mac_vap = &(delete_hmac_vap->st_vap_base_info);

    delete_hmac_vap->delete_timer_info.tbtt_num = tbtt_num;
    if (tbtt_num == 0) {
        if (delete_hmac_vap->delete_timer_info.delete_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&delete_hmac_vap->delete_timer_info.delete_timer);
        }
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_prepare_remove_link_ap::tbtt=0 remove now.}");
        return hmac_mld_delete_link(delete_mac_vap);
    }

    timeout = mac_mib_get_BeaconPeriod(delete_mac_vap) * tbtt_num;
    oam_warning_log2(0, OAM_SF_CFG, "{hmac_ml_reconfig_remove_link::start timer, delete link[%d] after [%d] TBTT}",
        delete_mac_vap->uc_vap_id, tbtt_num);
    frw_timer_create_timer_m(&delete_hmac_vap->delete_timer_info.delete_timer, hmac_ml_remove_link_vap_timeout_handle,
        timeout, delete_hmac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, delete_mac_vap->core_id);

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_config_ml_reconfig_request_cap_check(hmac_vap_stru *hmac_vap)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    wlan_mib_ieee802dot11_stru *mib_info = NULL;

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_ml_reconfig_request_cap_check::not_mld_vap.}");
        return OAL_FAIL;
    }

    hmac_user = mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_config_ml_reconfig_request_cap_check::user is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    mib_info = hmac_vap->st_vap_base_info.pst_mib_info;

    if (mld_user_manage == NULL || mib_info == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_config_ml_reconfig_request_cap_check::mld_user_manage null}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 如果对端ap没有link reconfig operation能力则不进入 */
    if (mld_user_manage->mld_cap.link_reconfiguration_operation_support == 0) {
        oam_warning_log0(0, OAM_SF_CFG,
            "{hmac_config_ml_reconfig_request_cap_check::mld user not support reconfig operation.}");
        return OAL_FAIL;
    }

    /* 获取sta交互能力位 */
    if (mac_mib_get_EHTLinkReconfigurationOperationActivated(mib_info) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmnac_config_tx_action_ml_reconfig_request::get mib false.}");
        return OAL_FAIL;
    }

    if (hmac_vap->reconfig_op_info.op_valid == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmnac_config_tx_action_ml_reconfig_request::already sent.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_reconfiguration_ml_ie_sta_control(mac_ml_sta_control_union *sta_control)
{
    /* ML Reconfiguration IE STA Control */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |Link ID |  Complete Profile  | STA MAC Address Present  |  AP Removal Timer Present      */
    /*    4bit   |      1bit         |            1bit          |                1bit             */
    /* |Reconfiguration Operation Type|Operation Parameters Present |NSTR Bitmap Size |NSTR Indication Bitmap Present */
    /*               4bit             |              1bit            |         1bit   |          1bit             */
    /*****************************************************************************************************/
    if (sta_control->reconfiguration_ml_sta_control.complete_profile == OAL_TRUE ||
        sta_control->reconfiguration_ml_sta_control.mac_address_present == OAL_TRUE ||
        sta_control->reconfiguration_ml_sta_control.operation_parameters_present == OAL_TRUE ||
        sta_control->reconfiguration_ml_sta_control.nstr_indication_bitmap_present == OAL_TRUE ||
        sta_control->reconfiguration_ml_sta_control.delete_timer_present == OAL_FALSE) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_sta_control::sta control = 0x%x }",
            *((uint16_t *)sta_control));
        /* 目前仅限于remove ap场景 */
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_reconfiguration_ml_ie_sta_info(hmac_vap_stru *hmac_vap,
    mac_ml_sta_control_union *sta_control, uint8_t *sta_info, uint8_t *sta_info_len, uint16_t *tbtt_num)
{
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    uint8_t offset = MLD_RECFG_PER_STA_STA_INFO_LEN_FIELD_LEN;

    /* ML Reconfiguration IE STA Info */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |STA Info Length | STA MAC Address  | AP Removal Timer  |  Operation Parameters  | NSTR Indication Bitmap  */
    /*       1           |        0or6      |        0or2       |          0or3          |           0or1or2       */
    /*****************************************************************************************************/
    *sta_info_len = 0;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_sta_info::tmp_link_vap is null.}");
        return OAL_FAIL;
    }

    if (sta_info[0] <= MLD_RECFG_PER_STA_STA_INFO_LEN_FIELD_LEN) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_sta_info::sta info len err}");
        return OAL_FAIL;
    }

    if (sta_control->reconfiguration_ml_sta_control.mac_address_present == OAL_TRUE) {
        offset += WLAN_MAC_ADDR_LEN;
    }

    if (sta_control->reconfiguration_ml_sta_control.delete_timer_present == OAL_TRUE) {
        *tbtt_num = *((uint16_t *)(sta_info + offset));
        offset += DELETE_TIMER_FIELD_LEN;
    }
    if (sta_info[0] != offset) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_sta_info::invalid sta info length.}");
        return OAL_FAIL;
    }
    *sta_info_len = offset;

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_reconfiguration_ml_ie_link_info(hmac_vap_stru *hmac_vap, uint8_t *link_info,
    uint8_t link_info_len)
{
    uint8_t sta_info_len = 0;
    uint8_t offset = 0;
    uint32_t ret;
    mac_ml_sta_control_union *sta_ctrl = NULL;
    uint8_t *sta_info = NULL;
    uint8_t ap_link_id = 0xff;
    uint16_t tbtt_num = 0;
    hmac_vap_stru *delete_hmac_vap = NULL;

    /* ML Reconfiguration IE Per-STA Profile */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |Subelement ID | Length | STA Control | STA Info  | STA Profile  */
    /*       1         |    1   |      2      |   VAR     |      VAR     */
    /*****************************************************************************************************/
    while (link_info_len > (offset + ML_STA_INFO_OFFSET)) {
        if (link_info[offset] != ML_SUBEID_PER_STA_PROFILE) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_link_info::invalid subelement id.}");
            break;
        }

        sta_ctrl = (mac_ml_sta_control_union *)(link_info + offset + ML_STA_CONTROL_OFFSET);
        ret = hmac_parse_reconfiguration_ml_ie_sta_control(sta_ctrl);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_link_info::parse sta control fail.}");
            break;
        }
        ap_link_id = (uint8_t)sta_ctrl->reconfiguration_ml_sta_control.link_id;

        sta_info = (link_info + offset + ML_STA_INFO_OFFSET);
        ret = hmac_parse_reconfiguration_ml_ie_sta_info(hmac_vap, sta_ctrl, sta_info, &sta_info_len, &tbtt_num);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_link_info::local sta info err}");
            break;
        }

        if (link_info_len < (offset + ML_STA_INFO_OFFSET + sta_info_len) ||
            link_info[offset + MLD_RECFG_PER_STA_LENGTH_FIELD_OFFSET] != (STA_CONTROL_FIELD_LEN + sta_info_len)) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_link_info::invalid length.}");
            break;
        }

        delete_hmac_vap = hmac_mld_get_sta_vap_by_ap_link_id(hmac_vap, ap_link_id);
        if (ap_link_id == 0xff || delete_hmac_vap == NULL) {
            offset += (ML_STA_INFO_OFFSET + sta_info_len);
            continue;
        }

        /* 确认需要delete link，通知alg关闭link切换选择算法 */
        hmac_mld_sync_link_switch_cap(hmac_vap, OAL_FALSE, 0);

        if (hmac_ml_reconfig_remove_link(&hmac_vap->st_vap_base_info, tbtt_num, delete_hmac_vap) != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_link_info::remove link err}");
            break;
        }

        offset += (ML_STA_INFO_OFFSET + sta_info_len);
    }

    if (link_info_len != offset) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_link_info::invalid link info length.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_reconfiguration_ml_ie_common_info(hmac_vap_stru *hmac_vap, uint8_t *common_info,
    uint8_t *comm_info_len, mac_ml_control_union ml_control)
{
    uint16_t offset = MLD_RECFG_COMMON_INFO_LENGTH_FIELD_LEN;
    /* ML Reconfiguration IE Common Info */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |Common Info Length |  MLD MAC Address  | EML Capabilities  |  MLD Capabilities And Operations   */
    /*          1          |             6         |     2               |                 2             */
    /*****************************************************************************************************/

    if (ml_control.reconfiguration_ml_control.ml_mld_mac_address_present == OAL_TRUE) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_common_info::mld addr field not null}");
        offset += WLAN_MAC_ADDR_LEN;
    }

    if (ml_control.reconfiguration_ml_control.ml_eml_capabilities_present == OAL_TRUE) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_common_info::mld addr field not null}");
        offset += MLD_RECFG_COMMON_INFO_EML_CAP_FIELD_LEN;
    }

    if (ml_control.reconfiguration_ml_control.ml_mld_capabilities_and_operations_present == OAL_TRUE) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_common_info::mld addr field not null}");
        offset += MLD_RECFG_COMMON_INFO_ML_CAP_OP_FIELD_LEN;
    }

    if (common_info[0] != offset) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie_common_info::invalid common info length.}");
        return OAL_FAIL;
    }

    *comm_info_len = offset;
    return OAL_SUCC;
}

uint32_t hmac_parse_reconfiguration_ml_ie(hmac_vap_stru *hmac_vap, mac_ies_info *ie_info)
{
    uint8_t common_info_len, link_info_len;
    uint32_t ret;
    uint8_t *ml_ie = NULL;
    mac_eht_ml_ie_header_stru *ml_ie_stru = NULL;
    uint8_t *common_info = NULL;
    uint8_t *link_info = NULL;

    /* ML Reconfiguration IE */
    /* ------------------------------------------------------------------------------------------------- */
    /*    |Element ID |Length |Element ID Extension| Multi-Link Control|Common Info  | Link Info |       */
    /* ------------------------------------------------------------------------------------------------- */
    /*         |1          |1      | 1               |     2               |VAR          |VAR            */
    /*****************************************************************************************************/
    if (hmac_vap == NULL || ie_info == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie::invalid arguments.}");
        return OAL_FAIL;
    }

    ml_ie = mac_find_ml_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, RECONFIGURATION_MULTI_LINK, ie_info);
    if (ml_ie == NULL) {
        return OAL_SUCC;
    }

    ml_ie_stru = (mac_eht_ml_ie_header_stru *)ml_ie;
    if (ml_ie_stru->ml_control.reconfiguration_ml_control.ml_control_type != RECONFIGURATION_MULTI_LINK) {
        return OAL_SUCC;
    }

    oam_warning_log1(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie::ml control = 0x%x",
        *((uint16_t*)(&ml_ie_stru->ml_control)));
    if (ml_ie_stru->ie_len <= (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie::invalid ie len.}");
        return OAL_FAIL;
    }
    /* 解析Common Info */
    common_info = ml_ie + MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN;
    ret = hmac_parse_reconfiguration_ml_ie_common_info(hmac_vap, common_info, &common_info_len,
        ml_ie_stru->ml_control);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie::parse common info fail.}");
        return OAL_FAIL;
    }

    if (ml_ie_stru->ie_len <= (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len)) {
        oam_error_log0(0, OAM_SF_CSA, "{hmac_parse_reconfiguration_ml_ie::invalid ml ie_len.}");
        return OAL_FAIL;
    }

    /* 解析Link Info */
    link_info = ml_ie + MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN + common_info_len;
    link_info_len = ml_ie_stru->ie_len - (MAC_IE_EID_LEN + ML_CONTROL_FIELD_LEN + common_info_len);
    ret = hmac_parse_reconfiguration_ml_ie_link_info(hmac_vap, link_info, link_info_len);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_parse_reconfiguration_ml_ie::parse link info fail.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC uint8_t hmac_is_ml_reconfig_operation(mac_cfg_tx_ml_reconfig_param_stru *reconfig_para)
{
    uint8_t idx;
    for (idx = 0; idx < reconfig_para->link_op_num; idx++) {
        if (reconfig_para->op_type[idx] == ML_RECONFIG_ADD_LINK ||
            reconfig_para->op_type[idx] == ML_RECONFIG_DELETE_LINK) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

OAL_STATIC uint16_t hmac_encap_ml_reconfig_ie_ml_control(hmac_vap_stru *hmac_vap, mac_eht_ml_ie_header_stru *ml_ie_hdr,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para)
{
    uint8_t is_reconfig_op = hmac_is_ml_reconfig_operation(reconfig_para);

    /* ML Reconfiguration IE ML Control */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |  Type  | Reserved  |  Presence Bitmap   */
    /*    3bit   |     1bit    |        12bit     */
    /*****************************************************************************************************/
    memset_s(&ml_ie_hdr->ml_control, ML_CONTROL_FIELD_LEN, 0, ML_CONTROL_FIELD_LEN);
    ml_ie_hdr->ml_control.reconfiguration_ml_control.ml_control_type = RECONFIGURATION_MULTI_LINK;

    if (is_reconfig_op == OAL_TRUE) {
        ml_ie_hdr->ml_control.reconfiguration_ml_control.ml_mld_mac_address_present = OAL_TRUE;
        ml_ie_hdr->ml_control.reconfiguration_ml_control.ml_mld_capabilities_and_operations_present = OAL_TRUE;
    } else {
        ml_ie_hdr->ml_control.reconfiguration_ml_control.ml_mld_mac_address_present = OAL_FALSE;
        ml_ie_hdr->ml_control.reconfiguration_ml_control.ml_mld_capabilities_and_operations_present = OAL_FALSE;
    }
    ml_ie_hdr->ml_control.reconfiguration_ml_control.ml_eml_capabilities_present =
        mac_get_ml_reconfig_eml_capabilities(hmac_vap);

    return ML_CONTROL_FIELD_LEN;
}

OAL_STATIC uint16_t hmac_set_reconfiguration_ml_ie_common_info(hmac_vap_stru *hmac_vap, uint8_t *buff,
    mac_ml_control_union ml_control)
{
    errno_t ret;
    uint8_t *buffer = buff;
    uint8_t comm_info_len;
    hmac_mld_manage_stru *mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    uint8_t max_simultaneous_link_cnt;
    /* ML Reconfiguration IE Common Info */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |Common Info Length |  MLD MAC Address  | EML Capabilities  |  MLD Capabilities And Operations   */
    /*          1          |             6         |     2               |                 2             */
    /*****************************************************************************************************/
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_common_info::memcpy_s fail.}");
        return 0;
    }

    buffer += MLD_RECFG_COMMON_INFO_LENGTH_FIELD_LEN;

    if (ml_control.reconfiguration_ml_control.ml_mld_mac_address_present == OAL_TRUE) {
        ret = memcpy_s(buffer, WLAN_MAC_ADDR_LEN, mld_vap_manage->mld_addr,
            WLAN_MAC_ADDR_LEN);
        if (ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_common_info::memcpy_s fail.}");
        }
        buffer += WLAN_MAC_ADDR_LEN;
    }

    if (ml_control.reconfiguration_ml_control.ml_eml_capabilities_present == OAL_TRUE) {
        buffer += mac_set_eml_capabilities_field(&hmac_vap->mld_vap_link_info, buffer);
    }
    max_simultaneous_link_cnt = (hmac_get_mld_assoc_mode(hmac_vap) == MLD_ASSOC_MODE_MLSR) ?
        1 : mld_vap_manage->mld_link_cnt;
    if (ml_control.reconfiguration_ml_control.ml_mld_capabilities_and_operations_present == OAL_TRUE) {
        buffer += mac_set_mld_reconfig_cap_and_op_field(buffer, max_simultaneous_link_cnt);
    }

    comm_info_len = buffer - buff;
    buff[0] = comm_info_len;

    return comm_info_len;
}

OAL_STATIC uint8_t hmac_set_reconfiguration_ml_ie_sta_control(uint8_t op_idx,
    mac_ml_per_sta_profiler_stru *per_sta, mac_cfg_tx_ml_reconfig_param_stru *reconfig_para)
{
    /* ML Reconfiguration IE STA Control */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |Link ID |  Complete Profile  | STA MAC Address Present  |  AP Removal Timer Present      */
    /*    4bit   |      1bit         |            1bit          |                1bit             */
    /* |Reconfiguration Operation Type|Operation Parameters Present |NSTR Bitmap Size |NSTR Indication Bitmap Present */
    /*               4bit             |              1bit            |         1bit   |          1bit             */
    /*****************************************************************************************************/
    memset_s(&per_sta->ml_control, STA_CONTROL_FIELD_LEN, 0, STA_CONTROL_FIELD_LEN);

    per_sta->ml_control.reconfiguration_ml_sta_control.link_id = reconfig_para->ap_link_id[op_idx];
    per_sta->ml_control.reconfiguration_ml_sta_control.complete_profile =
        (reconfig_para->op_type[op_idx] == ML_RECONFIG_ADD_LINK) ? OAL_TRUE : OAL_FALSE;
    if (reconfig_para->op_type[op_idx] == ML_RECONFIG_ADD_LINK ||
        reconfig_para->op_type[op_idx] == ML_RECONFIG_DELETE_LINK) {
        per_sta->ml_control.reconfiguration_ml_sta_control.mac_address_present = OAL_TRUE;
    } else {
        per_sta->ml_control.reconfiguration_ml_sta_control.mac_address_present = OAL_FALSE;
    }
    per_sta->ml_control.reconfiguration_ml_sta_control.delete_timer_present =
        (reconfig_para->op_type[op_idx] == ML_RECONFIG_AP_REMOVAL) ? OAL_TRUE : OAL_FALSE;
    per_sta->ml_control.reconfiguration_ml_sta_control.reconfiguration_operation_type = reconfig_para->op_type[op_idx];
    per_sta->ml_control.reconfiguration_ml_sta_control.operation_parameters_present = OAL_FALSE;

    return STA_CONTROL_FIELD_LEN;
}

OAL_STATIC uint8_t hmac_set_reconfiguration_ml_ie_sta_info(hmac_vap_stru *op_vap, uint8_t *buffer,
    mac_ml_per_sta_profiler_stru *per_sta)
{
    errno_t ret;
    uint8_t *buffer_tmp = buffer;
    uint8_t sta_info_len;

    /* ML Reconfiguration IE STA Info */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |STA Info Length | STA MAC Address  | AP Removal Timer  |  Operation Parameters  | NSTR Indication Bitmap  */
    /*       1           |        0or6      |        0or2       |          0or3          |           0or1or2       */
    /*****************************************************************************************************/

    buffer_tmp += MLD_RECFG_PER_STA_STA_INFO_LEN_FIELD_LEN;

    if (per_sta->ml_control.reconfiguration_ml_sta_control.mac_address_present == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_sta_info::mac_address_present = 1}");
        ret = memcpy_s(buffer_tmp, WLAN_MAC_ADDR_LEN,
            mac_mib_get_StationID(&op_vap->st_vap_base_info), WLAN_MAC_ADDR_LEN);
        if (ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_sta_info::memcpy_s fail.}");
        }
        buffer_tmp += WLAN_MAC_ADDR_LEN;
    }

    if (per_sta->ml_control.reconfiguration_ml_sta_control.delete_timer_present == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_sta_info::delete_timer_present = 1}");
        *((uint16_t *)buffer_tmp) = op_vap->delete_timer_info.tbtt_num;
        buffer_tmp += DELETE_TIMER_FIELD_LEN;
        if (op_vap->delete_timer_info.tbtt_num > 0) {
            op_vap->delete_timer_info.tbtt_num--;
        }
    }

    if (per_sta->ml_control.reconfiguration_ml_sta_control.operation_parameters_present == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_sta_info::operation_parameters_present = 1}");
        buffer_tmp += MLD_RECFG_PER_STA_STA_INFO_OP_PARA_LEN;
    }
    if (per_sta->ml_control.reconfiguration_ml_sta_control.nstr_indication_bitmap_present == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_sta_info::nstr_indication_bitmap_present=1}");
        buffer_tmp +=
            (per_sta->ml_control.reconfiguration_ml_sta_control.nstr_bitmap_size == 0 ? BIT_OFFSET_1 : BIT_OFFSET_2);
    }

    sta_info_len = buffer_tmp - buffer;
    buffer[0] = sta_info_len;

    return sta_info_len;
}

/*
 * 函 数 名  : hmac_set_reconfiguration_ml_ie_sta_complete_profile
 * 功能描述  : 组帧ml reconfig complete profile部分
 * 1.日    期  : 2024年2月18日
 *   作    者  : w30008019
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint16_t hmac_set_reconfiguration_ml_ie_sta_complete_profile(hmac_vap_stru *hmac_sta, uint8_t *frame,
    uint8_t *dest_addr)
{
    uint8_t *frame_origin = frame;
    uint8_t *req_frame = frame;
    mac_vap_stru *mac_vap = NULL;
    mac_device_stru *mac_device = NULL;
    hmac_scanned_bss_info *scaned_bss = NULL;
#ifdef _PRE_WLAN_CHBA_MGMT
    hmac_scanned_bss_info st_scaned_bss;
#endif

    mac_vap = &(hmac_sta->st_vap_base_info);

    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == NULL) {
        return 0;
    }

    mac_vap_set_bssid(mac_vap, dest_addr, WLAN_MAC_ADDR_LEN);
    scaned_bss = hmac_vap_get_scan_bss_info(mac_vap);
#ifdef _PRE_WLAN_CHBA_MGMT
    if (hmac_chba_vap_start_check(hmac_sta) == OAL_TRUE) {
        memset_s(&st_scaned_bss, sizeof(hmac_scanned_bss_info), 0, sizeof(hmac_scanned_bss_info));
        scaned_bss = &st_scaned_bss;
        hmac_chba_set_assoc_scan_bss(hmac_sta, scaned_bss);
    }
#endif
    req_frame += hmac_set_reconfiguration_ml_ie_sta_complete_ie(hmac_sta, mac_vap, req_frame, scaned_bss);

    return (uint16_t)(req_frame - frame_origin);
}

OAL_STATIC uint16_t hmac_set_reconfiguration_ml_ie_sta_profile(hmac_vap_stru *op_vap, uint8_t *buffer,
    mac_ml_per_sta_profiler_stru *per_sta)
{
    // 根据per_sta->ml_control.reconfiguration_ml_sta_control.complete_profile封装所有ie
    uint16_t len = 0;
    uint8_t *dest_addr = op_vap->mld_vap_link_info.link_ap_info.sta_mac_addr;

    if (per_sta->ml_control.reconfiguration_ml_sta_control.complete_profile != OAL_TRUE) {
        return 0;
    }
    len = hmac_set_reconfiguration_ml_ie_sta_complete_profile(op_vap, buffer, dest_addr);
    return len;
}

OAL_STATIC uint16_t hmac_set_reconfiguration_ml_ie_per_sta_profile(hmac_vap_stru *op_vap, uint8_t *buff,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para, uint8_t op_idx)
{
    uint8_t *buffer = buff;
    uint16_t per_sta_profile_len;
    mac_ml_per_sta_profiler_stru *per_sta = (mac_ml_per_sta_profiler_stru *)buffer;

    /* ML Reconfiguration IE Per-STA Profile */
    /* ------------------------------------------------------------------------------------------------- */
    /*  |Subelement ID | Length | STA Control | STA Info  | STA Profile  */
    /*       1         |    1   |      2      |   VAR     |      VAR     */
    /*****************************************************************************************************/
    per_sta->sub_eid = 0;   /* 固定SUBELEMENT ID = 0 */
    buffer += MAC_IE_HDR_LEN;
    /* 封装sta control域 */
    buffer += hmac_set_reconfiguration_ml_ie_sta_control(op_idx, per_sta, reconfig_para);
    /* 封装sta info域 */
    buffer += hmac_set_reconfiguration_ml_ie_sta_info(op_vap, buffer, per_sta);
    /* 封装sta profile域 */
    buffer += hmac_set_reconfiguration_ml_ie_sta_profile(op_vap, buffer, per_sta);

    per_sta_profile_len = buffer - buff;
    per_sta->ie_len = per_sta_profile_len - MAC_IE_HDR_LEN;

    return per_sta_profile_len;
}

OAL_STATIC uint16_t hmac_set_reconfiguration_ml_ie_link_info(hmac_vap_stru *hmac_vap, uint8_t *buff,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para)
{
    uint16_t link_info_len;
    uint8_t op_idx, ap_link_id;
    uint8_t *buffer = buff;
    hmac_vap_stru *op_vap = NULL;
    hmac_mld_manage_stru *mld_vap_manage = NULL;

    mld_vap_manage = hmac_vap->mld_vap_link_info.mld_vap_manage;
    if (mld_vap_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_link_info::mld_vap_manage null err.}");
        return 0;
    }
    if (reconfig_para->link_op_num > WLAN_MAX_ML_LINK_NUM) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_link_info::link_op_num err.}");
        return 0;
    }
    for (op_idx = 0; op_idx < reconfig_para->link_op_num; op_idx++) {
        ap_link_id = reconfig_para->ap_link_id[op_idx];
        if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
            op_vap = hmac_mld_get_sta_vap_by_ap_link_id(hmac_vap, ap_link_id);
        } else {
            op_vap = mld_vap_manage->link_vap[ap_link_id];
        }
        if (op_vap == NULL) {
            oam_warning_log0(0, OAM_SF_ANY, "{hmac_set_reconfiguration_ml_ie_link_info::op link vap not found}");
            continue;
        }
        /* 设置Per-STA Info */
        buffer += hmac_set_reconfiguration_ml_ie_per_sta_profile(op_vap, buffer, reconfig_para, op_idx);
    }
    link_info_len = buffer - buff;

    return link_info_len;
}

OAL_STATIC uint16_t hmac_encap_ml_reconfig_ie(hmac_vap_stru *hmac_vap, uint8_t *buff,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para)
{
    uint16_t ml_reconfig_len;
    uint8_t *buffer = buff;
    mac_eht_ml_ie_header_stru *ml_ie_hdr = (mac_eht_ml_ie_header_stru *)buffer;

    /*****************************************************************************************************/
    /* ML Reconfiguration IE */
    /* ------------------------------------------------------------------------------------------------- */
    /*    |Element ID |Length |Element ID Extension| Multi-Link Control|Common Info  | Link Info |       */
    /* ------------------------------------------------------------------------------------------------- */
    /*         |1          |1      | 1               |     2               |VAR          |VAR            */
    /*****************************************************************************************************/

    ml_ie_hdr->eid = MAC_EID_MULTILINK;
    ml_ie_hdr->eid_extension = MAC_EID_EXT_EHT_ML;
    buffer += MAC_IE_EXT_HDR_LEN;

    buffer += hmac_encap_ml_reconfig_ie_ml_control(hmac_vap, ml_ie_hdr, reconfig_para);
    /* 设置Common Info字段 */
    buffer += hmac_set_reconfiguration_ml_ie_common_info(hmac_vap, buffer, ml_ie_hdr->ml_control);
    /* 设置Link Info字段 */
    buffer += hmac_set_reconfiguration_ml_ie_link_info(hmac_vap, buffer, reconfig_para);

    ml_reconfig_len = buffer - buff;
    ml_ie_hdr->ie_len = ml_reconfig_len - MAC_IE_HDR_LEN;

    return ml_reconfig_len;
}

OAL_STATIC void hmac_ml_reconfig_req_set_mac_hdr(uint8_t *mgmt_hdr, uint8_t *addr1, uint8_t *addr2, uint8_t *addr3)
{
    /* 设置 Frame Control field */
    mac_hdr_set_frame_control(mgmt_hdr, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* 设置分片序号为0 */
    mac_hdr_set_fragment_number(mgmt_hdr, 0);

    /* 设置 address1(接收端): AP  (BSSID) */
    oal_set_mac_addr(mgmt_hdr + WLAN_HDR_ADDR1_OFFSET, addr1);

     /* 设置 address2(发送端): dot11StationID */
    oal_set_mac_addr(mgmt_hdr + WLAN_HDR_ADDR2_OFFSET, addr2);

    /* 设置 address3: AP  (BSSID) */
    oal_set_mac_addr(mgmt_hdr + WLAN_HDR_ADDR3_OFFSET, addr3);
}

OAL_STATIC uint16_t hmac_encap_ml_reconfig_req_frame(hmac_ml_reconfig_gen_netbuf_ctx_stru *ctx)
{
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para = NULL;
    uint16_t idx = 0;
    uint8_t *data = NULL;
    uint8_t *mac_header = NULL;

    if (ctx->netbuf == NULL || ctx->param == NULL || ctx->user_mac_addr == NULL || ctx->hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_encap_ml_reconfig_req_frame, input null");
        return 0;
    }

    reconfig_para = ctx->param;
    mac_header = (uint8_t *)oal_netbuf_header(ctx->netbuf);
    if (mac_header == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_encap_ml_reconfig_req_frame, mac_header null");
        return 0;
    }
    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /*************************************************************************/

    hmac_ml_reconfig_req_set_mac_hdr(mac_header, ctx->user_mac_addr, mac_mib_get_StationID(ctx->mac_vap),
        ctx->mac_vap->auc_bssid);

    /*************************************************************************/
    /*             Reconfiguration request Frame - Frame Body                */
    /*        -------------------------------------------------              */
    /* | Category | Action |  Dialog Token | Reconfiguration Multi-Link |    */
    /*        -------------------------------------------------              */
    /*  | 1        | 1      |       1       | Var        |                   */
    /*        -------------------------------------------------              */
    /*************************************************************************/

    data = mac_get_80211_mgmt_payload(ctx->netbuf);
    /* Category */
    data[idx++] = MAC_ACTION_CATEGORY_PROTECTED_EHT;
    /* Action */
    data[idx++] = MAC_PROTECTED_EHT_ACTION_LINK_RECONFIGURATION_REQUEST;
    /* Dialog Token */
    data[idx++] = reconfig_para->diag_token;

    idx += hmac_encap_ml_reconfig_ie(ctx->hmac_vap, data + idx, reconfig_para);

    return (uint16_t)(idx + MAC_80211_FRAME_LEN);
}

OAL_STATIC uint32_t hmac_ml_reconfig_tx_cb_init(hmac_ml_reconfig_gen_netbuf_ctx_stru *ctx)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    if (ctx->netbuf == NULL || ctx->user_mac_addr == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_tx_cb_init:: input null.");
        return OAL_FAIL;
    }

    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(ctx->netbuf);
    if (tx_ctl == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_tx_cb_init:: input null.");
        return OAL_FAIL;
    }

    memset_s(tx_ctl, oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    mac_get_cb_frame_type(tx_ctl) = ctx->cb_type;
    mac_get_cb_frame_subtype(tx_ctl) = ctx->cb_subtype;

    if (mac_vap_set_cb_tx_user_idx(&ctx->hmac_vap->st_vap_base_info, tx_ctl, ctx->user_mac_addr) != OAL_SUCC) {
        oam_warning_log3(0, 0, "(hmac_ml_reconfig_tx_cb_init::fail to find user[%02X:XX:XX:XX:%02X:%02X]}",
            ctx->user_mac_addr[MAC_ADDR_0], ctx->user_mac_addr[MAC_ADDR_4],
            ctx->user_mac_addr[MAC_ADDR_5]);
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ml_reconfig_action_event_init(hmac_ml_reconfig_gen_netbuf_ctx_stru *ctx)
{
    dmac_ctx_action_event_stru wlan_ctx_action = {0};

    if (ctx->netbuf == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_ml_reconfig_action_event_init::input null!");
        return OAL_FAIL;
    }

    wlan_ctx_action.us_user_idx = ctx->user_assoc_id;
    if (memcpy_s((uint8_t *)(oal_netbuf_data(ctx->netbuf) + ctx->payload_len), sizeof(dmac_ctx_action_event_stru),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_ml_reconfig_action_event_init::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ml_reconfig_generate_netbuf(hmac_ml_reconfig_gen_netbuf_ctx_stru *ctx)
{
    uint32_t ret;
    ctx->netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (ctx->netbuf == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_generate_netbuf:: malloc netbuf fail.");
        return OAL_FAIL;
    }

    oal_mem_netbuf_trace(ctx->netbuf, OAL_TRUE);
    oal_netbuf_prev(ctx->netbuf) = NULL;
    oal_netbuf_next(ctx->netbuf) = NULL;

    ctx->payload_len = hmac_encap_ml_reconfig_req_frame(ctx);

    ret = hmac_ml_reconfig_tx_cb_init(ctx);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_generate_netbuf:: cb init fail.");
        return ret;
    }
    ret = hmac_ml_reconfig_action_event_init(ctx);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_generate_netbuf:: action event init fail.");
        return ret;
    }
    oal_netbuf_put(ctx->netbuf, ctx->payload_len + sizeof(dmac_ctx_action_event_stru));

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ml_reconfig_send_frame(hmac_ml_reconfig_gen_netbuf_ctx_stru *ctx)
{
    uint32_t ret;

    ret = hmac_ml_reconfig_generate_netbuf(ctx);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(ctx->netbuf);
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_send_frame:: generate_netbuf fail.");
        return ret;
    }
    oam_warning_log1(0, OAM_SF_ANY, "{hmac_ml_reconfig_send_frame:: ctx->payload_len = %u", ctx->payload_len);

    ret = hmac_tx_mgmt_send_event(&ctx->hmac_vap->st_vap_base_info, ctx->netbuf, ctx->payload_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(ctx->netbuf);
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ml_reconfig_send_frame:: tx_mgmt_send fail.");
        return ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ml_reconfig_req_timeout_handle(void *arg)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)arg;

    oam_warning_log0(0, OAM_SF_CFG, "{hmac_ml_reconfig_req_timeout_handle}");
    hmac_vap->reconfig_op_info.op_valid = OAL_FALSE;

    return OAL_SUCC;
}

OAL_STATIC void hmac_ml_reconfig_op_info_update(hmac_vap_stru *hmac_vap,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_param)
{
    uint8_t idx;

    hmac_vap->reconfig_op_info.diag_token = reconfig_param->diag_token;
    hmac_vap->reconfig_op_info.op_num = reconfig_param->link_op_num;
    hmac_vap->reconfig_op_info.op_valid = OAL_TRUE;
    hmac_vap->reconfig_op_info.need_add = OAL_FALSE;
    hmac_vap->reconfig_op_info.need_del = OAL_FALSE;
    for (idx = 0; idx < reconfig_param->link_op_num; idx++) {
        hmac_vap->reconfig_op_info.op_type[idx] = reconfig_param->op_type[idx];
        hmac_vap->reconfig_op_info.op_link_id[idx] = reconfig_param->ap_link_id[idx];
        hmac_vap->reconfig_op_info.rsp_status[idx] = OAL_FAIL;
    }

    frw_timer_create_timer_m(&hmac_vap->reconfig_op_info.req_timer, hmac_ml_reconfig_req_timeout_handle,
        MLD_RECFG_REQ_TIMEOUT, hmac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
}

uint32_t hmac_tx_action_ml_reconfig_request_proc(mac_vap_stru *mac_vap,
    mac_cfg_tx_ml_reconfig_param_stru *reconfig_para)
{
    uint32_t ret;
    mac_user_stru *mac_user = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_ml_reconfig_gen_netbuf_ctx_stru ctx = { 0 };

    if (mac_vap == NULL || reconfig_para == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_tx_action_ml_reconfig_request_proc:: input NULL.");
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = mac_res_get_hmac_user(mac_vap->us_assoc_vap_id);
    mac_user = mac_res_get_mac_user(mac_vap->us_assoc_vap_id);
    hmac_vap = mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_user == NULL || mac_user == NULL || hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_tx_action_ml_reconfig_request_proc:: user/vap is NULL.");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ret = hmac_config_ml_reconfig_request_cap_check(hmac_vap);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_tx_action_ml_reconfig_request_proc::cap check fail.}");
        return ret;
    }

    ctx.hmac_vap = hmac_vap;
    ctx.mac_vap = &hmac_vap->st_vap_base_info;
    ctx.user_mac_addr = mac_user->auc_user_mac_addr;
    ctx.user_assoc_id = hmac_user->st_user_base_info.us_assoc_id;
    ctx.param = reconfig_para;
    ctx.cb_type = WLAN_CB_FRAME_TYPE_ACTION;
    ctx.cb_subtype = WLAN_ACTION_ML_RECONFIG_REQUEST;

    ret = hmac_ml_reconfig_send_frame(&ctx);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_tx_action_ml_reconfig_request_proc::send frame fail.}");
        return ret;
    }

    hmac_ml_reconfig_op_info_update(hmac_vap, reconfig_para);
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_rx_ml_reconfig_rsp_kde_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *buff, uint16_t buff_len)
{
    uint16_t len = BYTE_OFFSET_1 + buff[0];     // kde子单元的长度

    if (buff_len <= len) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action::buff_len <= BYTE_OFFSET_1 + buff[0]");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC void hmac_rx_ml_reconfig_rsp_action_delete_single_link(hmac_vap_stru *hmac_vap, uint8_t ap_link_id)
{
    // 待适配接口，注意入参是ap linkid
}

OAL_STATIC void hmac_rx_ml_reconfig_rsp_action_delete_link_proc(hmac_vap_stru *hmac_vap)
{
    uint8_t idx;

    for (idx = 0; idx < hmac_vap->reconfig_op_info.op_num; idx++) {
        if (hmac_vap->reconfig_op_info.op_type[idx] != ML_RECONFIG_DELETE_LINK ||
            hmac_vap->reconfig_op_info.rsp_status[idx] != OAL_SUCC) {
            continue;
        }
        hmac_rx_ml_reconfig_rsp_action_delete_single_link(hmac_vap, hmac_vap->reconfig_op_info.op_link_id[idx]);
    }
}

OAL_STATIC uint8_t hmac_rx_ml_reconfig_rsp_action_status_find_link_idx(hmac_vap_stru *hmac_vap, uint8_t ap_link_id)
{
    uint8_t cnt_idx;
    uint8_t link_id;

    for (cnt_idx = 0; cnt_idx < hmac_vap->reconfig_op_info.op_num; cnt_idx++) {
        link_id = hmac_vap->reconfig_op_info.op_link_id[cnt_idx];
        if (link_id != ap_link_id) {
            continue;
        } else {
            return cnt_idx;
        }
    }
    return WLAN_MAX_ML_LINK_NUM;
}

OAL_STATIC uint32_t hmac_rx_ml_reconfig_rsp_action_status_info_update(hmac_vap_stru *hmac_vap, uint8_t *buff,
    uint16_t buff_len, uint8_t cnt)
{
    uint8_t cnt_idx;
    uint16_t status;
    uint8_t link_id;
    uint8_t link_idx;

    if (buff_len < cnt * BYTE_OFFSET_3) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action_status_info_update::buff_len err");
        return OAL_FAIL;
    }

    for (cnt_idx = 0; cnt_idx < cnt; cnt_idx++) {
        link_id = buff[cnt_idx * BYTE_OFFSET_3];
        status = *((uint16_t *)(&buff[cnt_idx * BYTE_OFFSET_3 + BYTE_OFFSET_1]));

        link_idx = hmac_rx_ml_reconfig_rsp_action_status_find_link_idx(hmac_vap, link_id);
        if (link_idx != WLAN_MAX_ML_LINK_NUM) {
            if (status != OAL_SUCC) {
                oam_warning_log2(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action_status_info_update::,status[%u] = %u}",
                    link_id, status);
            } else {
                hmac_vap->reconfig_op_info.need_add |=
                    (hmac_vap->reconfig_op_info.op_type[link_idx] == ML_RECONFIG_ADD_LINK);
                hmac_vap->reconfig_op_info.need_del |=
                    (hmac_vap->reconfig_op_info.op_type[link_idx] == ML_RECONFIG_DELETE_LINK);
            }
            hmac_vap->reconfig_op_info.rsp_status[cnt_idx] = status;
        } else {
            oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action_status_info_update::link_idx err}");
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}

void hmac_rx_ml_reconfig_rsp_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *frame_hdr, uint8_t *payload, uint32_t payload_len)
{
    /* | Category | Protected EHT Action | Dialog Token | Count | Status List | KDE | OCI | Basic ML | */
    /* |     1    |            1         |       1      |   1   |      3n     | VAR | VAR |    VAR   | */
    uint8_t *buff = payload;
    uint16_t buff_len = (uint16_t)payload_len;
    uint8_t cnt;
    uint8_t diag_token;
    uint32_t ret;

    if (buff_len <= BYTE_OFFSET_4 || hmac_vap->reconfig_op_info.op_valid == OAL_FALSE) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action::buff_len <= BYTE_OFFSET_4 or op_valid = 0");
        return;
    }
    diag_token = buff[BYTE_OFFSET_2];
    cnt = buff[BYTE_OFFSET_3];
    if (diag_token != hmac_vap->reconfig_op_info.diag_token || cnt == 0) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action::action param err");
        return;
    }

    buff += BYTE_OFFSET_4;
    buff_len -= BYTE_OFFSET_4;
    ret = hmac_rx_ml_reconfig_rsp_action_status_info_update(hmac_vap, buff, buff_len, cnt);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action::status_info_update fail");
        return;
    }

    buff += cnt * BYTE_OFFSET_3;
    buff_len -= cnt * BYTE_OFFSET_3;
    /* 优先处理delete link */
    if (hmac_vap->reconfig_op_info.need_del == OAL_TRUE) {
        hmac_rx_ml_reconfig_rsp_action_delete_link_proc(hmac_vap);
    }

    if (hmac_vap->reconfig_op_info.need_add == OAL_TRUE) {
        if (mac_mib_get_rsnaactivated(&hmac_vap->st_vap_base_info) == OAL_TRUE) {
            ret = hmac_rx_ml_reconfig_rsp_kde_process(hmac_vap, hmac_user, buff, buff_len);
            if (ret != OAL_SUCC) {
                oam_error_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action::kde proc fail");
                return;
            }
        }
        // TBD 这里参考调用assoc rsp接口，解析并建立关联
        ret = hmac_mld_process_asoc_rsp(hmac_vap, hmac_user, frame_hdr, payload, (uint16_t)payload_len);
        if (ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_RX, "{hmac_rx_ml_reconfig_rsp_action::hmac_mld_process_asoc_rsp fail");
            return;
        }
    }
    hmac_vap->reconfig_op_info.op_valid = OAL_FALSE;
    frw_timer_immediate_destroy_timer_m(&hmac_vap->reconfig_op_info.req_timer);
}

uint32_t hmac_parse_reconfig_rsp_basic_ml_ie(hmac_vap_stru *hmac_vap, uint8_t *payload, uint16_t msg_len,
    ml_info_stru *ap_mld_info)
{
    uint8_t *ie = NULL;
    uint16_t offset;
    uint16_t ie_len;

    if (hmac_vap == NULL || payload == NULL || ap_mld_info == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_parse_reconfig_rsp_basic_ml_ie::invalid argument.}");
        return OAL_FAIL;
    }

    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ASSOC, "{hmac_parse_reconfig_rsp_basic_ml_ie::hmac vap is not mld link vap.}");
        return OAL_SUCC;
    }

    offset = BYTE_OFFSET_4 + payload[BYTE_OFFSET_3] * BYTE_OFFSET_3;    // catagory+sub action+token+cnt+3*cnt
    if (mac_mib_get_rsnaactivated(&hmac_vap->st_vap_base_info) == OAL_TRUE) {
        /* kde偏移 */
        offset += BYTE_OFFSET_1 + payload[offset];
    }
    if (msg_len <= offset) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_parse_reconfig_rsp_basic_ml_ie::invalid len.}");
        return OAL_FAIL;
    }

    ie = payload + offset;
    ie_len = msg_len - offset;

    return hmac_sta_parse_basic_ml_ie(hmac_vap, ie, ie_len, ap_mld_info, WLAN_ASSOC_RSP);
}

#endif
