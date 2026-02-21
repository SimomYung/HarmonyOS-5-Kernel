/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : mac_frame_ml.c
 * 创建日期   : 2023年11月15日
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "mac_frame_ml.h"
#include "oal_ext_if.h"
#include "mac_resource.h"
#include "mac_ie.h"
#include "securec.h"
#include "mac_frame_common.h"
#include "mac_mib.h"
#include "mac_user.h"
#include "hmac_sae.h"
#include "hmac_mld_ext.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_ML_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/*****************************************************************************
  3 函数实现
*****************************************************************************/
#ifdef _PRE_WLAN_FEATURE_ML

/*
 * 函 数 名   : mac_set_auth_basic_ml_ie
 * 功能描述   : 封装auth帧的basic multilink IE
 */
uint8_t mac_set_auth_basic_ml_ie(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t *addr, uint8_t addr_len)
{
    mac_eht_ml_ie_header_stru *ml_ie_hdr = NULL;
    uint8_t *buffer_tmp = buffer;
    uint8_t ie_len = 0;

    if (oal_any_null_ptr3(mac_vap, buffer, addr)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_auth_basic_ml_ie::argument is null.}");
        return 0;
    }

    ml_ie_hdr = (mac_eht_ml_ie_header_stru *)buffer_tmp;
    ml_ie_hdr->eid = MAC_EID_MULTILINK;
    ml_ie_hdr->eid_extension = MAC_EID_EXT_EHT_ML;
    ml_ie_hdr->ml_control.basic_ml_control.ml_control_type = BASIC_MULTI_LINK;
    ml_ie_hdr->ml_control.basic_ml_control.ml_control_resv = 0;
    ml_ie_hdr->ml_control.basic_ml_control.ml_link_id_info_present = 0;
    ml_ie_hdr->ml_control.basic_ml_control.ml_bss_parameters_change_count_present = 0;
    ml_ie_hdr->ml_control.basic_ml_control.ml_medium_synchronization_delay_information_present = 0;
    ml_ie_hdr->ml_control.basic_ml_control.ml_eml_capabilities_present = 0;
    ml_ie_hdr->ml_control.basic_ml_control.ml_mld_capabilities_and_operations_present = 0;
    ml_ie_hdr->ml_control.basic_ml_control.ml_mld_id_present = 0;
    ml_ie_hdr->ml_control.basic_ml_control.reserved = 0;

    buffer_tmp += (MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN);

    /* 填写Common Info Len的长度 */
    buffer_tmp[0] = 1 + WLAN_MAC_ADDR_LEN;
    buffer_tmp++;
    /* 设置MLD MAC地址 */
    if (memcpy_s(buffer_tmp, WLAN_MAC_ADDR_LEN, addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return ie_len;
    }

    ml_ie_hdr->ie_len = (WLAN_MAC_ADDR_LEN + 1 + ML_CONTROL_FIELD_LEN) + 1;
    ie_len = (WLAN_MAC_ADDR_LEN + 1 + ML_CONTROL_FIELD_LEN + MAC_IE_EXT_HDR_LEN);

    return ie_len;
}

/*
 * 函 数 名   : mac_free_sub_ie_defrag_buffer
 * 功能描述   : 释放去分片per-STA profile字段时申请的内存
 */
void mac_free_sub_ie_defrag_buffer(uint8_t *defrag_sub_ie)
{
    /* 判断是否需要释放内存 */
    if (defrag_sub_ie == NULL) {
        return;
    }

    oal_free(defrag_sub_ie);
}

/*
 * 函 数 名   : mac_check_sub_ie_need_defragment
 * 功能描述   : 检查per-STA profile字段是否分片
 */
uint8_t mac_check_sub_ie_need_defragment(uint8_t *sub_ie, uint16_t max_len)
{
    uint16_t next_frag_offset = MAC_IE_HDR_LEN + IE_FRAGMENT_THRESHOLD;

    if (sub_ie == NULL) {
        return OAL_FALSE;
    }

    /*
       判断条件:
       1 sub ie长度=255
       2 link info字段存在下一个分片
       3 下一个分片的ID为 分片ID 254
    */
    return sub_ie[1] == IE_FRAGMENT_THRESHOLD && next_frag_offset < max_len &&
           sub_ie[next_frag_offset] == MAC_EID_FRAGMENT_SUB;
}

/*
 * 函 数 名   : mac_per_sta_profile_subelement_defragment
 * 功能描述   : 去分片per-STA profile字段信息
 */
uint32_t mac_per_sta_profile_subelement_defragment(uint8_t **sub_ie, uint16_t *defrag_len, uint16_t *frag_len,
    uint8_t *defrag_sub_ie, uint16_t defrag_sub_ie_len)
{
    uint8_t  *tmp_sub_ie = *sub_ie;
    uint16_t offset = MAC_IE_HDR_LEN + IE_FRAGMENT_THRESHOLD;  // 偏移掉第一个SUB IE分片
    uint16_t defrag_offset = 0;
    uint8_t  last_len = 0;

    if (defrag_sub_ie == NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{mac_per_sta_profile_subelement_defragment::invalid sta info length.}");
        return OAL_FAIL;
    }

    if (memcpy_s(defrag_sub_ie, defrag_sub_ie_len, tmp_sub_ie, offset) != EOK) {
        return OAL_FAIL;
    }
    tmp_sub_ie += offset;
    defrag_offset += offset;
    defrag_sub_ie_len -= offset;

    while ((tmp_sub_ie[0] == MAC_EID_FRAGMENT_SUB) && (tmp_sub_ie[1] == IE_FRAGMENT_THRESHOLD) &&
           (defrag_sub_ie_len >= IE_FRAGMENT_THRESHOLD)) {
        if (memcpy_s(defrag_sub_ie + defrag_offset, defrag_sub_ie_len,
                     tmp_sub_ie + MAC_IE_HDR_LEN, IE_FRAGMENT_THRESHOLD) != EOK) {
            return OAL_FAIL;
        }
        offset += (MAC_IE_HDR_LEN + IE_FRAGMENT_THRESHOLD);
        tmp_sub_ie += (MAC_IE_HDR_LEN + IE_FRAGMENT_THRESHOLD);
        defrag_offset += IE_FRAGMENT_THRESHOLD;
        defrag_sub_ie_len -= IE_FRAGMENT_THRESHOLD;
    }

    // 最后一个ML IE分片
    if ((tmp_sub_ie[0] == MAC_EID_FRAGMENT_SUB) && (tmp_sub_ie[1] > 0) && (defrag_sub_ie_len >= tmp_sub_ie[1])) {
        last_len = tmp_sub_ie[1];
        if (memcpy_s(defrag_sub_ie + defrag_offset, defrag_sub_ie_len, tmp_sub_ie + MAC_IE_HDR_LEN, last_len) != EOK) {
            return OAL_FAIL;
        }
        offset += (MAC_IE_HDR_LEN + last_len);
        tmp_sub_ie += (MAC_IE_HDR_LEN + last_len);
        defrag_offset += last_len;
        defrag_sub_ie_len -= last_len;
    }

    *defrag_len = defrag_offset - MAC_IE_HDR_LEN;
    *frag_len = offset - MAC_IE_HDR_LEN;

    return OAL_SUCC;
}

/*
 * 函 数 名   : mac_count_basic_ml_sta_info_length
 * 功能描述   : 计算 Basic Multi-Link IE 中 STA Info field 的长度
 */
uint8_t mac_count_basic_ml_sta_info_length(uint8_t *sta_control)
{
    mac_ml_sta_control_union *sta_ctrl = (mac_ml_sta_control_union *)sta_control;
    uint8_t sta_info_len = 1; /* STA Info Length能力位 */

    if (sta_control == NULL) {
        return 0;
    }

    if (sta_ctrl->basic_ml_sta_control.sta_mac_address_present == 1) {
        sta_info_len += WLAN_MAC_ADDR_LEN;
    }
    if (sta_ctrl->basic_ml_sta_control.beacon_interval_present == 1) {
        sta_info_len += MAC_BEACON_INTERVAL_LEN;
    }
    if (sta_ctrl->basic_ml_sta_control.tsf_offset_present == 1) {
        sta_info_len += ML_TSF_OFFSET;
    }
    if (sta_ctrl->basic_ml_sta_control.dtim_info_present == 1) {
        sta_info_len += ML_DTIM_INFO_LEN;
    }

    /*
       若nstr_link_pair_present为1，则STA Info字段包含nstr_indication_bitmap子字段
       若nstr_bitmap_size为0，则nstr_indication_bitmap为1字节
       若nstr_bitmap_size为1，则nstr_indication_bitmap为2字节
    */
    if (sta_ctrl->basic_ml_sta_control.nstr_link_pair_present == 1) {
        sta_info_len += (uint8_t)sta_ctrl->basic_ml_sta_control.nstr_bitmap_size_present + 1;
    }
    if (sta_ctrl->basic_ml_sta_control.bss_parameters_change_count_present == 1) {
        sta_info_len += BSS_PARAMETERS_CHANGE_COUNT_LEN;
    }

    return sta_info_len;
}

/*
 * 函 数 名   : mac_sta_parse_basic_ml_sta_info
 * 功能描述   : 解析Basic Multi-Link IE 中 STA Info字段
 */
uint32_t mac_sta_parse_basic_ml_sta_info(uint8_t *sta_info, uint8_t *sta_control,
    ml_link_info_stru *ml_link_info)
{
    mac_ml_sta_control_union *sta_ctrl = (mac_ml_sta_control_union *)sta_control;
    uint8_t offset = 0;

    if (sta_ctrl == NULL || sta_control == NULL) {
        return OAL_FAIL;
    }

    offset += 1; /* STA Info Length能力位 */
    if (sta_ctrl->basic_ml_sta_control.sta_mac_address_present == 1) {
        if (memcpy_s(ml_link_info->sta_mac_addr, WLAN_MAC_ADDR_LEN, &sta_info[offset], WLAN_MAC_ADDR_LEN) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_sta_parse_basic_ml_sta_info::memcpy_s fail.}");
            return OAL_FAIL;
        }
        offset += WLAN_MAC_ADDR_LEN;
    }
    if (sta_ctrl->basic_ml_sta_control.beacon_interval_present == 1) {
        offset += MAC_BEACON_INTERVAL_LEN;
    }
    if (sta_ctrl->basic_ml_sta_control.tsf_offset_present == 1) {
        offset += ML_TSF_OFFSET;
    }
    if (sta_ctrl->basic_ml_sta_control.dtim_info_present == 1) {
        offset += ML_DTIM_INFO_LEN;
    }
    if (sta_ctrl->basic_ml_sta_control.nstr_link_pair_present == 1) {
        offset += 1;
    }
    if (sta_ctrl->basic_ml_sta_control.nstr_bitmap_size_present == 1) {
        offset += 1;
    }
    if (sta_ctrl->basic_ml_sta_control.bss_parameters_change_count_present == 1) {
        ml_link_info->bpcc = sta_info[offset];
        offset += 1;
    }

    if (sta_info[0] != offset) {
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_sta_parse_basic_ml_sta_info::invalid sta info length %d, offset %d.}",
            sta_info[0], offset);
        return OAL_FAIL;
    }

    ml_link_info->link_id = sta_ctrl->basic_ml_sta_control.link_id;
    ml_link_info->complete_profile = sta_ctrl->basic_ml_sta_control.complete_profile;
    ml_link_info->is_add_link_list = OAL_TRUE;
    return OAL_SUCC;
}

uint8_t mac_get_ml_reconfig_eml_capabilities(hmac_vap_stru *hmac_vap)
{
    wlan_mib_ieee802dot11_stru *mib_info = hmac_vap->st_vap_base_info.pst_mib_info;

    if (mib_info == NULL) {
        return OAL_FALSE;
    }

    if (mib_info->wlan_mib_eht_sta_config.dot11EHTEMLMROptionActivated == OAL_TRUE ||
        mib_info->wlan_mib_eht_sta_config.dot11EHTEMLSROptionActivated == OAL_TRUE) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

/*
 * 函 数 名   : mac_set_basic_ml_ie_control
 * 功能描述   : 设置ml control字段
 */
void mac_set_basic_ml_ie_control(hmac_vap_stru *hmac_vap, mac_ml_control_union *ml_control)
{
    if (hmac_vap == NULL || ml_control == NULL) {
        return;
    }

    memset_s(ml_control, ML_CONTROL_FIELD_LEN, 0, ML_CONTROL_FIELD_LEN);
    ml_control->basic_ml_control.ml_control_type = BASIC_MULTI_LINK;

    ml_control->basic_ml_control.ml_mld_capabilities_and_operations_present = 1;
    ml_control->basic_ml_control.ml_eml_capabilities_present =
        mac_mib_get_EHTEMLSROptionActivated(&hmac_vap->st_vap_base_info);

    if (is_ap(&hmac_vap->st_vap_base_info)) {
        ml_control->basic_ml_control.ml_link_id_info_present = 1;
        ml_control->basic_ml_control.ml_bss_parameters_change_count_present = 1;
    }
}

uint8_t mac_set_ml_link_id_info_field(uint8_t link_id, uint8_t *buffer)
{
    mac_ml_link_id_info_stru *link_id_info = (mac_ml_link_id_info_stru *)buffer;
    if (buffer == NULL) {
        return 0;
    }
    memset_s(link_id_info, sizeof(mac_ml_link_id_info_stru), 0, sizeof(mac_ml_link_id_info_stru));
    link_id_info->link_id = link_id;
    link_id_info->reserved = 0;
    return ML_LINK_ID_INFO_FIELD_LEN;
}

/*
 * 函 数 名   : mac_set_eml_capabilities_field
 * 功能描述   : 填写mld_capabilities域
 */
uint8_t mac_set_eml_capabilities_field(hmac_mld_manage_stru *mld_vap_manage, uint8_t *buffer)
{
    mac_eml_capabilities_stru *eml_cap = (mac_eml_capabilities_stru *)buffer;

    if (buffer == NULL) {
        return 0;
    }

    eml_cap->emlsr_support = mld_vap_manage->emlsr_cap.emlsr_support;
    eml_cap->emlsr_padding_delay = mld_vap_manage->emlsr_cap.emlsr_padding_delay;
    eml_cap->emlsr_transition_delay = mld_vap_manage->emlsr_cap.emlsr_transition_delay;
    eml_cap->emlmr_support = OAL_FALSE;
    eml_cap->emlmr_delay = 0;
    eml_cap->transition_timeout = 0;
    eml_cap->reserved = 0;

    return EML_CAPBILITIES_FIELD_LEN;
}

/*
 * 函 数 名   : mac_set_mld_capabilities_field
 * 功能描述   : 填写mld_capabilities域
 */
uint8_t mac_set_mld_capabilities_field(mac_vap_stru *mac_vap, uint8_t *buffer, uint8_t link_cnt)
{
    mac_mld_capabilities_stru *mld_cap = (mac_mld_capabilities_stru *)buffer;

    if (oal_any_null_ptr2(mac_vap, buffer)) {
        return 0;
    }

    memset_s(mld_cap, sizeof(mac_mld_capabilities_stru), 0, sizeof(mac_mld_capabilities_stru));

    /************************** MLD Capabilities 子字段 *************************
     |--------------------------------------------------------------------------|
     |   Maximum    |   SRS   | TID-To-Link | Frequency  |   AAR   |  Reserved  |
     |  Number Of   | Support | Mapping     | Separation | Support |            |
     | Simultaneous |         | Negotiation |  For STR   |         |            |
     |    Links     |         | Supported   |            |         |            |
     |--------------------------------------------------------------------------|
     | B0        B3 |    B4   | B5       B6 | B7     B11 | B12     | B13   B16  |
     |--------------------------------------------------------------------------|
    *****************************************************************************/
    mld_cap->maximum_number_of_simultaneous_links = (link_cnt > 0 ? link_cnt - 1 : 0);
    mld_cap->srs_support = 0;
    if (mac_vap->pst_mib_info != NULL) {
        mld_cap->tid_to_link_mapping_negotiation_supported =
            mac_mib_get_TIDtoLinkMappingActivated(mac_vap->pst_mib_info);
        mld_cap->link_reconfiguration_operation_support =
            mac_mib_get_EHTLinkReconfigurationOperationActivated(mac_vap->pst_mib_info);
    } else {
        mld_cap->tid_to_link_mapping_negotiation_supported = 0;
        mld_cap->link_reconfiguration_operation_support = 0;
    }
    mld_cap->frequency_separation_for_str = 0;
    mld_cap->aar_supplort = 0;

    return MLD_CAPBILITIES_FIELD_LEN;
}
/*
 * 函 数 名   : mac_set_mld_reconfig_cap_and_op_field
 * 功能描述   : 填写mld reconfig capabilities域
 */
uint8_t mac_set_mld_reconfig_cap_and_op_field(uint8_t *buffer, uint8_t link_cnt)
{
    mac_mld_capabilities_stru *mld_cap = (mac_mld_capabilities_stru *)buffer;

    if (oal_any_null_ptr1(buffer)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_set_mld_reconfig_cap_and_op_field::argument is null}");
        return 0;
    }

    memset_s(mld_cap, sizeof(mac_mld_capabilities_stru), 0, sizeof(mac_mld_capabilities_stru));
    mld_cap->link_reconfiguration_operation_support = 1;
    mld_cap->maximum_number_of_simultaneous_links = (link_cnt > 0 ? link_cnt - 1 : 0);

    return MLD_CAPBILITIES_FIELD_LEN;
}

/*
 * 函 数 名   : mac_basic_ml_element_defragment
 * 功能描述   : 去分片basic Multi-Link IE信息
 */
uint32_t mac_basic_ml_element_defragment(uint8_t **ml_ie, uint16_t *ret_len, uint8_t *defrag_ie, uint16_t defrag_ie_len)
{
    uint8_t *tmp_ml_ie = *ml_ie;
    uint16_t first_frag_len = MAC_IE_EXT_HDR_LEN + EXT_IE_FRAGMENT_THRESHOLD;
    uint8_t  last_frag_len = 0;
    uint16_t defrag_offset = 0;
    uint32_t ret = OAL_FAIL;

    if (defrag_ie == NULL) {
        return OAL_FAIL;
    }

    /* copy 第一个ML分片IE */
    if (memcpy_s(defrag_ie, defrag_ie_len, tmp_ml_ie, first_frag_len) != EOK) {
        return OAL_FAIL;
    }
    tmp_ml_ie += first_frag_len;
    defrag_offset += first_frag_len;
    defrag_ie_len -= first_frag_len;

    /* 中间ML IE分片，判断条件确保defrag_ie_len不会减翻 */
    while ((defrag_ie_len >= IE_FRAGMENT_THRESHOLD) &&
           (tmp_ml_ie[0] == MAC_EID_FRAGMENT) && (tmp_ml_ie[1] == IE_FRAGMENT_THRESHOLD)) {
        if (memcpy_s(defrag_ie + defrag_offset, defrag_ie_len,
                     tmp_ml_ie + MAC_IE_HDR_LEN, IE_FRAGMENT_THRESHOLD) != EOK) {
            return OAL_FAIL;
        }
        tmp_ml_ie += (MAC_IE_HDR_LEN + IE_FRAGMENT_THRESHOLD);
        defrag_offset += IE_FRAGMENT_THRESHOLD;
        defrag_ie_len -= IE_FRAGMENT_THRESHOLD;
        ret = OAL_SUCC;
    }

    /* 最后一个ML IE分片，判断条件确保defrag_ie_len不会减翻 */
    if ((defrag_ie_len >= tmp_ml_ie[1]) && (tmp_ml_ie[0] == MAC_EID_FRAGMENT) && (tmp_ml_ie[1] > 0)) {
        last_frag_len = tmp_ml_ie[1];
        if (memcpy_s(defrag_ie + defrag_offset, defrag_ie_len, tmp_ml_ie + MAC_IE_HDR_LEN, last_frag_len) != EOK) {
            return OAL_FAIL;
        }
        tmp_ml_ie += (MAC_IE_HDR_LEN + last_frag_len);
        defrag_offset += last_frag_len;
        defrag_ie_len -= last_frag_len;
        ret = OAL_SUCC;
    }

    *ret_len = defrag_offset - MAC_IE_HDR_LEN;
    return ret;
}

/*
 * 函 数 名   : mac_free_ie_defrag_buffer
 * 功能描述   : 释放去分片Multi-Link element时申请的内存
 */
void mac_free_ie_defrag_buffer(uint8_t *defrag_ie)
{
    if (defrag_ie == NULL) {
        return;
    }

    oal_free(defrag_ie);
}

uint32_t mac_parse_auth_req_ml_info(uint8_t *payload, uint16_t payload_len,
    hmac_auth_req_info_stru *auth_req_info)
{
    uint16_t  req_min_len = MAC_AUTH_ALG_LEN + MAC_AUTH_TRANS_SEQ_NUM_LEN + MAC_STATUS_CODE_LEN;
    uint8_t  *ml_ie = NULL;
    uint8_t  *mld_mac_addr = NULL;
    uint8_t  *common_info = NULL;
    mac_eht_ml_ie_header_stru *ml_ie_hdr = NULL;
    mac_ies_info ies_info = {0};
    int32_t remain_len;

    if (oal_any_null_ptr2(payload, auth_req_info)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_parse_auth_req_ml_info::argument is null}");
        return OAL_FAIL;
    }

    auth_req_info->is_ml_setup = OAL_FALSE;

    if (auth_req_info->auth_type == WLAN_WITP_AUTH_SAE) {
        /* TODO: wpa3入网，只有supplicant知道fixed字段长度，才能找到ml字段的起始位置。开源代码ml ie在auth帧最后。 */
        oam_warning_log1(0, OAM_SF_ANY, "{mac_parse_auth_req_ml_info::sae, payload_len:%d}", payload_len);
        return OAL_SUCC;
    }

    if (payload_len < req_min_len) {
        oam_warning_log1(0, OAM_SF_ANY, "{mac_parse_auth_req_ml_info::payload_len:%d}", payload_len);
        return OAL_FAIL;
    }

    remain_len = payload_len - req_min_len;
    ies_info.frame_ies = payload + req_min_len;
    ies_info.frame_len = remain_len;
    ml_ie = mac_find_ml_ie(MAC_EID_MULTILINK, MAC_EID_EXT_EHT_ML, BASIC_MULTI_LINK, &ies_info);
    if (ml_ie == NULL) {
        return OAL_SUCC;
    }

    if (ml_ie[1] < sizeof(mac_eht_ml_ie_header_stru) - MAC_IE_HDR_LEN) {
        return OAL_FAIL;
    }

    ml_ie_hdr = (mac_eht_ml_ie_header_stru *)ml_ie;
    if (ml_ie_hdr->ml_control.basic_ml_control.ml_control_type != BASIC_MULTI_LINK) {
        return OAL_FAIL;
    }

    /* ml ie长度填写合法，剩余长度足够auth帧的ml ie */
    if (ml_ie_hdr->ie_len != (AUTH_MLD_IE_LEN - MAC_IE_HDR_LEN) || remain_len < AUTH_MLD_IE_LEN) {
        return OAL_FAIL;
    }

    common_info = ml_ie + (MAC_IE_EXT_HDR_LEN + ML_CONTROL_FIELD_LEN);
    if (common_info[0] != (ML_COMMON_INFO_LEN + WLAN_MAC_ADDR_LEN)) {
        return OAL_FAIL;
    }

    mld_mac_addr = common_info + ML_COMMON_INFO_LEN;
    if (memcpy_s(auth_req_info->mld_mac_addr, WLAN_MAC_ADDR_LEN, mld_mac_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    auth_req_info->is_ml_setup = OAL_TRUE;
    return OAL_SUCC;
}
#endif
