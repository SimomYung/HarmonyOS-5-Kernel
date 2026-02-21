/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : Multi-Link ttlm特性处理
 * 作    者 : wifi
 * 创建日期 : 2024年2月2日
 */
#include "hmac_ml_ttlm.h"
#include "mac_frame_inl.h"
#include "mac_mib_sta_eht_cfg.h"
#include "mac_mib_sta_config.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_mld_ext.h"
#include "hmac_emlsr.h"
#include "hmac_config.h"
#include "host_hal_ext_if.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ML_TTLM_C

#ifdef _PRE_WLAN_FEATURE_ML

uint32_t hmac_mld_user_ttlm_init(hmac_user_stru *hmac_user)
{
    uint8_t tid, link_id;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    if (oal_unlikely(hmac_user == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_ttlm_init::hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_ttlm_init::mld_user_manage is NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_set_mld_user_default_ttlm_info(hmac_user);
    for (tid = 0; tid < TTLM_TIDNO_BUTT; tid++) {
        for (link_id = 0; link_id < WLAN_MAX_ML_LINK_NUM; link_id++) {
            mld_user_manage->ttlm_info.ttlm_status[tid][link_id] = OAL_TRUE;
        }
    }
    return OAL_SUCC;
}
oal_bool_enum_uint8 hmac_vap_check_ttlm_is_support(mac_vap_stru *mac_vap)
{
    hmac_vap_stru *hmac_vap = NULL;
    if (oal_unlikely(mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_vap_check_ttlm_is_support::param null.}");
        return OAL_FALSE;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_vap_check_ttlm_is_support::hmac_vap is null.}");
        return OAL_FALSE;
    }
    if (mac_vap->en_vap_state != MAC_VAP_STATE_UP) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_vap_check_ttlm_is_support::hmac_vap is not up.}");
        return OAL_FALSE;
    }
    if (hmac_vap_is_legacy_mld_mode(hmac_vap) == OAL_TRUE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_vap_check_ttlm_is_support::hmac_vap is not mld_vap.}");
        return OAL_FALSE;
    }
    if (mac_mib_get_TIDtoLinkMappingActivated(hmac_vap->st_vap_base_info.pst_mib_info) == OAL_FALSE) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_vap_check_ttlm_is_support::TTLM mib false.}");
        return OAL_FALSE;
    }
    return OAL_TRUE;
}

oal_bool_enum_uint8 hmac_user_check_ttlm_is_support(hmac_user_stru *hmac_user)
{
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    if (oal_unlikely(hmac_user == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_check_ttlm_is_support::param null.}");
        return OAL_FALSE;
    }
    if (hmac_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_check_ttlm_is_support::user is not assoc}");
        return OAL_FALSE;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_check_ttlm_is_support::mld_user_manage is NULL}");
        return OAL_FALSE;
    }
    if (mld_user_manage->mld_cap.tid_to_link_mapping_negotiation_supported == 0) {
        oam_warning_log0(0, OAM_SF_CFG,
            "{hmac_user_check_ttlm_is_support::mld user not support tid-to-link mapping.}");
        return OAL_FALSE;
    }
    return OAL_TRUE;
}


OAL_STATIC oal_bool_enum_uint8 hmac_ttlm_issubset_links(mac_ttlm_stru *ttlm, mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    uint8_t tid;
    uint8_t link_bitmap = 0;
    uint8_t link_req_bitmap = 0;

    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        link_bitmap = ttlm->link_bitmap[tid];
        link_req_bitmap = ttlm_req_param->cfg_ttlm_ie.link_bitmap[tid];
        if ((uint8_t)(link_bitmap | (~link_req_bitmap)) != 0xff) {
            return OAL_FALSE;
        }
    }
    return OAL_TRUE;
}

OAL_STATIC oal_bool_enum_uint8 hmac_ttlm_check_issubset_links(hmac_mld_user_manage_stru *mld_user_manage,
    mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    mac_ttlm_stru *ttlm = NULL;
    uint8_t direction;
    direction = ttlm_req_param->cfg_ttlm_ie.ttlm_ctrl.direction;
    if (direction == WLAN_DOWNUPLINK) {
        ttlm = &(mld_user_manage->ttlm_info.ttlm[WLAN_UPLINK]);
        if (hmac_ttlm_issubset_links(ttlm, ttlm_req_param) != OAL_TRUE) {
            oam_warning_log0(0, OAM_SF_CFG, "{hmac_ttlm_check_issubset_links::it's not subset link in up direction}");
            return OAL_FALSE;
        }
        ttlm = &(mld_user_manage->ttlm_info.ttlm[WLAN_DOWNLINK]);
        if (hmac_ttlm_issubset_links(ttlm, ttlm_req_param) != OAL_TRUE) {
            oam_warning_log0(0, OAM_SF_CFG, "{hmac_ttlm_check_issubset_links::it's not subset link in down direction}");
            return OAL_FALSE;
        }
    } else {
        ttlm = &(mld_user_manage->ttlm_info.ttlm[direction]);
        if (hmac_ttlm_issubset_links(ttlm, ttlm_req_param) != OAL_TRUE) {
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_ttlm_check_issubset_links::it's not subset link in direction[%u]}",
                direction);
            return OAL_FALSE;
        }
    }
    return OAL_TRUE;
}

OAL_STATIC oal_bool_enum_uint8 hmac_ttlm_check_is_in_process(hmac_mld_user_manage_stru *mld_user_manage,
    uint8_t direction)
{
    if (direction == WLAN_DOWNUPLINK) {
        if (mld_user_manage->ttlm_info.ttlm[WLAN_UPLINK].status == WLAN_TTLM_PROCESS ||
            mld_user_manage->ttlm_info.ttlm[WLAN_DOWNLINK].status == WLAN_TTLM_PROCESS) {
            return OAL_TRUE;
        }
    } else {
        if (mld_user_manage->ttlm_info.ttlm[direction].status == WLAN_TTLM_PROCESS) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

uint32_t hmac_ttlm_req_param_check(hmac_user_stru *hmac_user, mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    uint8_t direction;
    ttlm_control *ttlm_ctrl = NULL;
    if (oal_unlikely(oal_any_null_ptr2(hmac_user, ttlm_req_param))) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_req_param_check::NULL args.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    ttlm_ctrl = &ttlm_req_param->cfg_ttlm_ie.ttlm_ctrl;
    if (ttlm_ctrl->direction > WLAN_DOWNUPLINK || ttlm_ctrl->default_link_mapping != 0 ||
        ttlm_ctrl->mapping_switch_time_present != 0 || ttlm_ctrl->expected_duration_present != 0 ||
        ttlm_ctrl->link_mapping_size > 1 || ttlm_ctrl->link_mapping_presence_bitmap == 0) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_req_param_check::invalid params.}");
        return OAL_FAIL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_req_param_check::mld user manage is null.}");
        return OAL_FAIL;
    }
    direction = ttlm_ctrl->direction;
    if (hmac_ttlm_check_is_in_process(mld_user_manage, direction) == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_ttlm_req_param_check::previous ttlm request process is not end.}");
        return OAL_FAIL;
    }
    if (direction == WLAN_DOWNUPLINK) {
        if ((mld_user_manage->ttlm_info.ttlm[WLAN_UPLINK].default_mapping == 1) &&
            (mld_user_manage->ttlm_info.ttlm[WLAN_DOWNLINK].default_mapping == 1)) {
            return OAL_SUCC;
        }
    } else {
        if (mld_user_manage->ttlm_info.ttlm[direction].default_mapping == 1) {
            return OAL_SUCC;
        }
    }

    if (mld_user_manage->ttlm_info.is_advertised == OAL_TRUE) {
        if (hmac_ttlm_check_issubset_links(mld_user_manage, ttlm_req_param) == OAL_FALSE) {
            oam_warning_log0(0, OAM_SF_CFG, "{hmac_ttlm_req_param_check::it's not subset link of advertised ttlm}");
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}


OAL_STATIC void hmac_set_mld_user_manage_ttlm_info(hmac_mld_user_manage_stru *mld_user_manage, mac_ttlm_stru *ttlm)
{
    mac_ttlm_stru   *ttlm_tmp = NULL;
    if (mld_user_manage->ttlm_info.direction == WLAN_DOWNLINK || mld_user_manage->ttlm_info.direction == WLAN_UPLINK) {
        ttlm_tmp = &(mld_user_manage->ttlm_info.ttlm[mld_user_manage->ttlm_info.direction]);
        memcpy_s(ttlm_tmp, sizeof(mac_ttlm_stru), ttlm, sizeof(mac_ttlm_stru));
    } else if (mld_user_manage->ttlm_info.direction == WLAN_DOWNUPLINK) {
        ttlm_tmp = &(mld_user_manage->ttlm_info.ttlm[WLAN_DOWNLINK]);
        memcpy_s(ttlm_tmp, sizeof(mac_ttlm_stru), ttlm, sizeof(mac_ttlm_stru));
        ttlm_tmp = &(mld_user_manage->ttlm_info.ttlm[WLAN_UPLINK]);
        memcpy_s(ttlm_tmp, sizeof(mac_ttlm_stru), ttlm, sizeof(mac_ttlm_stru));
    }
}


void hmac_set_mld_user_default_ttlm_info(hmac_user_stru *hmac_user)
{
    uint8_t         tid;
    mac_ttlm_stru   ttlm_tmp;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    if (oal_unlikely(hmac_user == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_set_mld_user_default_ttlm_info::param null.}");
        return;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_set_mld_user_default_ttlm_info::mld user manage is null.}");
        return;
    }
    memset_s(&ttlm_tmp, sizeof(mac_ttlm_stru), 0, sizeof(mac_ttlm_stru));
    mld_user_manage->ttlm_info.direction = WLAN_DOWNUPLINK;
    mld_user_manage->ttlm_info.dialog_token = 1;
    mld_user_manage->ttlm_info.is_advertised = OAL_FALSE;
    ttlm_tmp.default_mapping = 1;
    ttlm_tmp.status = WLAN_TTLM_INIT;
    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        ttlm_tmp.link_bitmap[tid] = (1 << mld_user_manage->mld_link_cnt) - 1;
    }
    hmac_set_mld_user_manage_ttlm_info(mld_user_manage, &ttlm_tmp);
}

void hmac_save_ttlm_req_param(hmac_user_stru *hmac_user, mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    uint8_t         tid;
    mac_ttlm_stru   ttlm_tmp;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;

    if (oal_unlikely(oal_any_null_ptr2(hmac_user, ttlm_req_param))) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_save_ttlm_req_param::param null.}");
        return;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_save_ttlm_req_param::mld user manage is null.}");
        return;
    }
    memset_s(&ttlm_tmp, sizeof(mac_ttlm_stru), 0, sizeof(mac_ttlm_stru));
    mld_user_manage->ttlm_info.direction = ttlm_req_param->cfg_ttlm_ie.ttlm_ctrl.direction;
    mld_user_manage->ttlm_info.dialog_token = ttlm_req_param->dialog_token;
    ttlm_tmp.status = WLAN_TTLM_PROCESS;
    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        if ((ttlm_req_param->cfg_ttlm_ie.ttlm_ctrl.link_mapping_presence_bitmap & BIT(tid)) == OAL_FALSE) {
            continue;
        }
        ttlm_tmp.link_bitmap[tid] = ttlm_req_param->cfg_ttlm_ie.link_bitmap[tid];
    }
    hmac_set_mld_user_manage_ttlm_info(mld_user_manage, &ttlm_tmp);
}

void hmac_set_ttlm_status(hmac_mld_user_manage_stru *mld_user_manage, wlan_ttlm_status_enum status)
{
    mac_ttlm_stru   *ttlm_tmp = NULL;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_set_ttlm_status::mld_user_manage null.}");
        return;
    }
    if (mld_user_manage->ttlm_info.direction == WLAN_DOWNLINK || mld_user_manage->ttlm_info.direction == WLAN_UPLINK) {
        ttlm_tmp = &(mld_user_manage->ttlm_info.ttlm[mld_user_manage->ttlm_info.direction]);
        ttlm_tmp->status = status;
    } else if (mld_user_manage->ttlm_info.direction == WLAN_DOWNUPLINK) {
        mld_user_manage->ttlm_info.ttlm[WLAN_DOWNLINK].status = status;
        mld_user_manage->ttlm_info.ttlm[WLAN_UPLINK].status = status;
    }
    oam_warning_log1(0, OAM_SF_CFG, "{hmac_set_ttlm_status::cur ttlm status[%d]}", status);
}

OAL_STATIC void hmac_tx_mgmt_action_cb_init(oal_netbuf_stru *netbuf, uint16_t user_id, uint16_t mgmt_len,
    wlan_cb_action_subtype_enum action_subtype)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    if (user_id == MAC_CB_DEFULT) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_tx_mgmt_cb_partial_init:: user id is required.}");
    }
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (tx_ctl == NULL)  {
        return;
    }
    memset_s(tx_ctl, sizeof(mac_tx_ctl_stru), 0, sizeof(mac_tx_ctl_stru));

    mac_get_cb_tx_user_idx(tx_ctl) = (uint8_t)user_id;
    mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_AC_MGMT;
    mac_get_cb_frame_type(tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    mac_get_cb_frame_subtype(tx_ctl) = action_subtype;
    mac_get_cb_frame_header_length(tx_ctl) = MAC_80211_FRAME_LEN;
    mac_get_cb_mpdu_num(tx_ctl) = 1;
    mac_get_cb_mpdu_len(tx_ctl) = mgmt_len + sizeof(dmac_ctx_action_event_stru);
    oal_netbuf_put(netbuf, mgmt_len + sizeof(dmac_ctx_action_event_stru));
}

OAL_STATIC uint8_t hmac_set_ttlm_ie_in_request(uint8_t *payload, mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    uint8_t                  tid;
    uint8_t                  offset = MAC_IE_EXT_HDR_LEN;
    mac_frame_ie_ext_hdr    *ie_ext_hdr = (mac_frame_ie_ext_hdr *)payload;
    ttlm_control            *ttlm_ctrl = (ttlm_control *)(payload + offset);

    ie_ext_hdr->element_id = MAC_EID_EXTENSION;
    ie_ext_hdr->element_id_ext = MAC_EID_EXT_EHT_TIDTOLINK;

    memset_s(ttlm_ctrl, sizeof(ttlm_control), 0, sizeof(ttlm_control));
    ttlm_ctrl->direction = ttlm_req_param->cfg_ttlm_ie.ttlm_ctrl.direction;
    ttlm_ctrl->link_mapping_size = 1;
    ttlm_ctrl->link_mapping_presence_bitmap = ttlm_req_param->cfg_ttlm_ie.ttlm_ctrl.link_mapping_presence_bitmap;
    offset += sizeof(ttlm_control);

    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        if ((ttlm_ctrl->link_mapping_presence_bitmap & BIT(tid)) == OAL_FALSE) {
            continue;
        }
        payload[offset++] = ttlm_req_param->cfg_ttlm_ie.link_bitmap[tid];
    }
    ie_ext_hdr->length = offset - MAC_IE_HDR_LEN;

    return offset;
}

OAL_STATIC void hmac_encap_ttlm_request(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    mac_cfg_ttlm_frame_param *ttlm_req_param, oal_netbuf_stru *netbuf, uint16_t *len)
{
    uint8_t *mac_header = (uint8_t*)oal_netbuf_header(netbuf);
    uint8_t *payload_addr = mac_get_80211_mgmt_payload(netbuf);
    uint8_t *mac_addr = NULL;
    uint16_t index = 0;

    *len = 0;
    mac_addr = mac_mib_get_StationID(mac_vap);
    if (mac_addr == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_encap_ttlm_request:mac addr is NULL!}");
        return;
    }
    /*************************************************************************/
    /* Management Frame Format */
    /* -------------------------------------------------------------------- */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS| */
    /* -------------------------------------------------------------------- */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  | */
    /* -------------------------------------------------------------------- */
    /*************************************************************************/
    /*************************************************************************/
    /* Set the fields in the frame header */
    /*************************************************************************/
    /* 设置 Frame Control field */
    /* 帧控制字段全为0，除了type和subtype */
    mac_hdr_set_frame_control(mac_header,
        WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* 设置分片序号为0 */
    mac_hdr_set_fragment_number(mac_header, 0);

    /* 设置地址1，与STA连接的AP MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);

    /* 设置地址2为自己的MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR2_OFFSET, mac_addr);

    /* 设置地址3，为VAP的MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR3_OFFSET, mac_vap->auc_bssid);
    /*************************************************************************/
    /* Set the fields in the frame body */
    /*************************************************************************/
    /*************************************************************************/
    /* ttlm request frame - Frame Body */
    /* --------------------------------------------------------------------- */
    /* |Category  |Protected EHT Action |Dialog Token |TTLM element | */
    /* --------------------------------------------------------------------- */
    /* |1         |1                    |1            |Variable    |       */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    /* 设置Category */
    payload_addr[index++] = MAC_ACTION_CATEGORY_PROTECTED_EHT;

    /* 设置Action */
    /* 参见mac_protected_eht_action_type_enum枚举类型定义 */
    payload_addr[index++] = MAC_PROTECTED_EHT_ACTION_TID_TO_LINK_MAPPING_REQUEST;

    /* 设置Dialog Token */
    payload_addr[index++] = ttlm_req_param->dialog_token;

    /* 设置TID-To-Link Mapping IE */
    index += hmac_set_ttlm_ie_in_request(payload_addr + index, ttlm_req_param);
    *len = MAC_80211_FRAME_LEN + index;
}


uint32_t hmac_send_ttlm_request(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    oal_netbuf_stru *mgmt_buf = NULL;
    uint8_t vap_id;
    uint16_t mgmt_len;
    uint32_t ret;
    dmac_ctx_action_event_stru wlan_ctx_action;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, hmac_user, ttlm_req_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_send_ttlm_request::input param null.}");
        return OAL_FAIL;
    }
    vap_id = mac_vap->uc_vap_id;
    /* 申请管理帧内存 */
    mgmt_buf = oal_mem_netbuf_alloc(OAL_MGMT_NETBUF, WLAN_MGMT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
    if (mgmt_buf == NULL) {
        oam_error_log0(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_request::mgmt_buf alloc null.}");
        return OAL_FAIL;
    }

    hmac_encap_ttlm_request(mac_vap, hmac_user, ttlm_req_param, mgmt_buf, &mgmt_len);
    if (mgmt_len > WLAN_MGMT_NETBUF_SIZE) {
        oam_error_log1(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_request:encap ttlm request len[%u] too large}", mgmt_len);
    }
    if (mgmt_len == 0) {
        oal_netbuf_free(mgmt_buf);
        return OAL_FAIL;
    }
    memset_s((uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru), 0, sizeof(dmac_ctx_action_event_stru));
    wlan_ctx_action.us_user_idx = hmac_user->st_user_base_info.us_assoc_id;

    if (memcpy_s((uint8_t *)(oal_netbuf_data(mgmt_buf) + mgmt_len), (WLAN_MGMT_NETBUF_SIZE - mgmt_len),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_send_ttlm_request::memcpy fail!");
        oal_netbuf_free(mgmt_buf);
        return OAL_FAIL;
    }
    /* 填写netbuf的cb字段，供发送管理帧和发送完成接口使用 */
    hmac_tx_mgmt_action_cb_init(mgmt_buf, mac_vap->us_assoc_vap_id, mgmt_len, WLAN_ACTION_MLD_TTLM_REQUEST);

    /* 调用发送管理帧接口 */
    ret = hmac_tx_mgmt_send_event(mac_vap, mgmt_buf, mgmt_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(mgmt_buf);
        oam_error_log1(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_request::tx ttlm request action failed[%u].}", ret);
        return ret;
    }
    return OAL_SUCC;
}

uint32_t hmac_mld_ttlm_request_process(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
                                       mac_cfg_ttlm_frame_param *ttlm_req_param)
{
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    uint32_t ret;

    if (oal_any_null_ptr3(mac_vap, hmac_user, ttlm_req_param)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_mld_ttlm_request_process::param is null!}");
        return OAL_FAIL;
    }

    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (mld_user_manage == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_mld_ttlm_request_process::mld_user_manage is null!}");
        return OAL_FAIL;
    }

    /* 发送 tid-to-link-mapping request报文，若发送成功则启动ttlm协商定时器 */
    ret = hmac_send_ttlm_request(mac_vap, hmac_user, ttlm_req_param);
    if (ret == OAL_SUCC) {
        hmac_save_ttlm_req_param(hmac_user, ttlm_req_param);
        hmac_set_ttlm_status(mld_user_manage, WLAN_TTLM_PROCESS);
        if (mld_user_manage->ttlm_info.ttlm_request_timer.en_is_registerd != OAL_TRUE) {
            /* 启动一个TTLM 协商定时器, 等待TTLM响应 */
            frw_timer_create_timer_m(&(mld_user_manage->ttlm_info.ttlm_request_timer),
                hmac_ttlm_request_time_timeout, TTLM_REQUEST_TIME, (void *)hmac_user, OAL_FALSE,
                OAM_MODULE_ID_HMAC, mac_vap->core_id);
        } else {
            frw_timer_restart_timer_m(&(mld_user_manage->ttlm_info.ttlm_request_timer), TTLM_REQUEST_TIME, OAL_FALSE);
        }
    }

    return ret;
}


OAL_STATIC void hmac_encap_ttlm_teardown(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    mac_cfg_ttlm_frame_param *ttlm_teardown_param, oal_netbuf_stru *netbuf, uint16_t *len)
{
    uint16_t index = 0;
    uint8_t *mac_header = oal_netbuf_header(netbuf);
    uint8_t *payload_addr = mac_get_80211_mgmt_payload(netbuf);
    uint8_t *mac_addr = NULL;
    *len = 0;
    mac_addr = mac_mib_get_StationID(mac_vap);
    if (mac_addr == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_encap_ttlm_teardown:mac addr is NULL!}");
        return;
    }
    /*************************************************************************/
    /* Management Frame Format */
    /* -------------------------------------------------------------------- */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS| */
    /* -------------------------------------------------------------------- */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  | */
    /* -------------------------------------------------------------------- */
    /*************************************************************************/
    /*************************************************************************/
    /* Set the fields in the frame header */
    /*************************************************************************/
    /* 帧控制字段全为0，除了type和subtype */
    mac_hdr_set_frame_control(mac_header, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    /* 设置分片序号为0 */
    mac_hdr_set_fragment_number(mac_header, 0);
    /* 设置地址1，与STA连接的AP MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);

    /* 设置地址2为自己的MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR2_OFFSET, mac_addr);

    /* 设置地址3，为VAP的MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR3_OFFSET, mac_vap->auc_bssid);
    /*************************************************************************/
    /* Set the fields in the frame body */
    /*************************************************************************/
    /* --------------------------------------------------------------------- */
    /* |Category  |Protected EHT Action |                                    */
    /* --------------------------------------------------------------------- */
    /* |1         |1                    |                                    */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    /* 设置Category */
    payload_addr[index++] = MAC_ACTION_CATEGORY_PROTECTED_EHT;
    /* 设置Action */
    /* 参见mac_protected_eht_action_type_enum枚举类型定义 */
    payload_addr[index++] = MAC_PROTECTED_EHT_ACTION_TID_TO_LINK_MAPPING_TEARDOWN;
    *len = MAC_80211_FRAME_LEN + index;
}

uint32_t hmac_send_ttlm_teardown(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *param)
{
    oal_netbuf_stru *mgmt_buf = NULL;
    uint16_t mgmt_len;
    mac_cfg_ttlm_frame_param *ttlm_teardown_param = NULL;
    uint8_t vap_id;
    uint32_t ret;
    dmac_ctx_action_event_stru wlan_ctx_action;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;

    if (oal_unlikely(oal_any_null_ptr3(mac_vap, hmac_user, param))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_send_ttlm_teardown:: input param null.}");
        return OAL_FAIL;
    }

    vap_id = mac_vap->uc_vap_id;
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_send_ttlm_teardown::mld_user_manage null.}");
        return OAL_FAIL;
    }
    if (mld_user_manage->ttlm_info.is_advertised == OAL_TRUE) {
        oam_warning_log0(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_teardown::can not teardown advertised ttlm.}");
        return OAL_FAIL;
    }

    /* 申请管理帧内存 */
    mgmt_buf = oal_mem_netbuf_alloc(OAL_MGMT_NETBUF, WLAN_MGMT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
    if (mgmt_buf == NULL) {
        oam_error_log0(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_teardown::mgmt_buf alloc null.}");
        return OAL_FAIL;
    }

    ttlm_teardown_param = (mac_cfg_ttlm_frame_param *)param;
    hmac_encap_ttlm_teardown(mac_vap, hmac_user, ttlm_teardown_param, mgmt_buf, &mgmt_len);
    if (mgmt_len == 0) {
        oam_error_log0(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_teardown::mgmt_len is zero.}");
        oal_netbuf_free(mgmt_buf);
        return OAL_FAIL;
    }

    memset_s((uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru), 0, sizeof(dmac_ctx_action_event_stru));
    wlan_ctx_action.us_user_idx = hmac_user->st_user_base_info.us_assoc_id;

    if (memcpy_s((uint8_t *)(oal_netbuf_data(mgmt_buf) + mgmt_len), (WLAN_MGMT_NETBUF_SIZE - mgmt_len),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oal_netbuf_free(mgmt_buf);
        return OAL_FAIL;
    }

    /* 填写netbuf的cb字段，供发送管理帧和发送完成接口使用 */
    hmac_tx_mgmt_action_cb_init(mgmt_buf, mac_vap->us_assoc_vap_id, mgmt_len, WLAN_ACTION_MLD_TTLM_TEARDOWN);
    /* 调用发送管理帧接口 */
    ret = hmac_tx_mgmt_send_event(mac_vap, mgmt_buf, mgmt_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(mgmt_buf);
        oam_error_log1(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_teardown::tx ttlm teardown action failed[%u].}", ret);
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint16_t hmac_encap_ttlm_response(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    uint16_t status_code, uint8_t dialog_token, oal_netbuf_stru *netbuf)
{
    uint16_t index = 0;
    uint8_t *mac_header = oal_netbuf_header(netbuf);
    uint8_t *payload_addr = mac_get_80211_mgmt_payload(netbuf);
    uint8_t *mac_addr = NULL;
    mac_addr = mac_mib_get_StationID(mac_vap);
    if (mac_addr == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_encap_ttlm_response:mac addr is NULL!}");
        return 0;
    }
    /*************************************************************************/
    /* Management Frame Format */
    /* -------------------------------------------------------------------- */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS| */
    /* -------------------------------------------------------------------- */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  | */
    /* -------------------------------------------------------------------- */
    /*************************************************************************/
    /*************************************************************************/
    /* Set the fields in the frame header */
    /*************************************************************************/
    /* 帧控制字段全为0，除了type和subtype */
    mac_hdr_set_frame_control(mac_header, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    /* 设置分片序号为0 */
    mac_hdr_set_fragment_number(mac_header, 0);
    /* 设置地址1，与STA连接的AP MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);

    /* 设置地址2为自己的MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR2_OFFSET, mac_addr);

    /* 设置地址3，为VAP的MAC地址 */
    oal_set_mac_addr(mac_header + WLAN_HDR_ADDR3_OFFSET, mac_vap->auc_bssid);
    /*************************************************************************/
    /* Set the fields in the frame body */
    /*************************************************************************/
    /* --------------------------------------------------------------------- */
    /* |Category  |Protected EHT Action |                                    */
    /* --------------------------------------------------------------------- */
    /* |1         |1                    |                                    */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    /* 设置Category */
    payload_addr[index++] = MAC_ACTION_CATEGORY_PROTECTED_EHT;
    /* 设置Action */
    /* 参见mac_protected_eht_action_type_enum枚举类型定义 */
    payload_addr[index++] = MAC_PROTECTED_EHT_ACTION_TID_TO_LINK_MAPPING_RESPONSE;
    payload_addr[index++] = dialog_token;
    *((uint16_t *)&payload_addr[index]) = status_code;
    index += sizeof(uint16_t);
    return MAC_80211_FRAME_LEN + index;
}

OAL_STATIC uint32_t hmac_send_ttlm_response(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    uint16_t status_code, uint8_t dialog_token)
{
    oal_netbuf_stru *mgmt_buf = NULL;
    uint16_t mgmt_len;
    uint8_t vap_id;
    uint32_t ret;
    dmac_ctx_action_event_stru wlan_ctx_action;
    vap_id = mac_vap->uc_vap_id;

    /* 申请管理帧内存 */
    mgmt_buf = oal_mem_netbuf_alloc(OAL_MGMT_NETBUF, WLAN_MGMT_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
    if (mgmt_buf == NULL) {
        oam_error_log0(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_response::mgmt_buf alloc null.}");
        return OAL_FAIL;
    }
    mgmt_len = hmac_encap_ttlm_response(mac_vap, hmac_user, status_code, dialog_token, mgmt_buf);
    if (mgmt_len == 0) {
        oam_error_log0(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_response::mgmt_len is zero.}");
        oal_netbuf_free(mgmt_buf);
        return OAL_FAIL;
    }
    memset_s((uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru), 0, sizeof(dmac_ctx_action_event_stru));
    wlan_ctx_action.us_user_idx = hmac_user->st_user_base_info.us_assoc_id;
    if (memcpy_s((uint8_t *)(oal_netbuf_data(mgmt_buf) + mgmt_len), (WLAN_MGMT_NETBUF_SIZE - mgmt_len),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_send_ttlm_response::memcpy fail!");
        oal_netbuf_free(mgmt_buf);
        return OAL_FAIL;
    }
    /* 填写netbuf的cb字段，供发送管理帧和发送完成接口使用 */
    hmac_tx_mgmt_action_cb_init(mgmt_buf, mac_vap->us_assoc_vap_id, mgmt_len, WLAN_ACTION_MLD_TTLM_RESPONSE);

    /* 调用发送管理帧接口 */
    ret = hmac_tx_mgmt_send_event(mac_vap, mgmt_buf, mgmt_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(mgmt_buf);
        oam_error_log1(vap_id, OAM_SF_ANY, "{hmac_send_ttlm_response::tx ttlm response action failed[%u].}", ret);
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_ttlm_request(hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf, mac_ttlm_stru *ttlm)
{
    uint8_t          tid, ttlm_field_len;
    uint8_t          offset = 0;
    uint8_t         *payload = mac_get_80211_mgmt_payload(netbuf);
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    ttlm_control *ttlm_ctrl = NULL;
    mac_frame_ie_ext_hdr *ie_ext_hdr = NULL;
    if (payload[MAC_ACTION_OFFSET_TOKEN] == 0) {
        return OAL_FAIL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_parse_ttlm_request::mld_user_manage null.}");
        return OAL_FAIL;
    }
    mld_user_manage->ttlm_info.dialog_token = payload[MAC_ACTION_OFFSET_TOKEN];
    offset += MAC_ACTION_OFFSET_TOKEN + 1;
    /* 解析TID-To-Link Mapping IE */
    ie_ext_hdr = (mac_frame_ie_ext_hdr *)(payload + offset);
    if (ie_ext_hdr->element_id != MAC_EID_EXTENSION || ie_ext_hdr->element_id_ext != MAC_EID_EXT_EHT_TIDTOLINK) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_parse_ttlm_request::it's not TTLM ie.}");
        return OAL_FAIL;
    }
    offset += MAC_IE_EXT_HDR_LEN;
    ttlm_ctrl = (ttlm_control *)(payload + offset);
    if (ttlm_ctrl->direction > WLAN_DOWNUPLINK || ttlm_ctrl->default_link_mapping != 0 ||
        ttlm_ctrl->mapping_switch_time_present != 0 || ttlm_ctrl->expected_duration_present != 0 ||
        ttlm_ctrl->link_mapping_size > 1 || ttlm_ctrl->link_mapping_presence_bitmap == 0) {
        oam_warning_log4(0, OAM_SF_ANY, "{hmac_parse_ttlm_request::invalid ttlm param:dir[%u],dflkmp[%u],mpswtp[%u],\
            epdp[%u]}", ttlm_ctrl->direction, ttlm_ctrl->default_link_mapping, ttlm_ctrl->mapping_switch_time_present,
            ttlm_ctrl->expected_duration_present);
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_parse_ttlm_request::invalid ttlm param:lkmps[%u],lkmppb[%u]}",
            ttlm_ctrl->link_mapping_size, ttlm_ctrl->link_mapping_presence_bitmap);
        return OAL_FAIL;
    }
    offset += sizeof(ttlm_control);
    mld_user_manage->ttlm_info.direction = ttlm_ctrl->direction;

    ttlm_field_len = (ttlm_ctrl->link_mapping_size == 0 ? 2 : 1); /* 2 表示Link Mapping Of TID字段的长度 */

    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        if ((ttlm_ctrl->link_mapping_presence_bitmap & BIT(tid)) == OAL_FALSE) {
            continue;
        }
        ttlm->link_bitmap[tid] = (payload[offset] & 0xff);
        offset += ttlm_field_len;
    }
    return OAL_SUCC;
}

uint32_t hmac_rx_ttlm_request_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf)
{
    uint32_t         ret;
    uint16_t         status_code;
    mac_vap_stru    *mac_vap = NULL;
    mac_ttlm_stru    ttlm;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    if (oal_unlikely(oal_any_null_ptr3(hmac_vap, hmac_user, netbuf))) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_rx_ttlm_request_process::invalid args.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mac_vap = &hmac_vap->st_vap_base_info;
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_rx_ttlm_request_process::mld_user_manage null.}");
        return OAL_FAIL;
    }
    memset_s(&ttlm, sizeof(mac_ttlm_stru), 0, sizeof(mac_ttlm_stru));
    ret = hmac_parse_ttlm_request(hmac_user, netbuf, &ttlm);
    status_code = (ret == OAL_SUCC ? MAC_SUCCESSFUL_STATUSCODE : MAC_DENIED_TID_TO_LINK_MAPPING);
    ret = hmac_send_ttlm_response(mac_vap, hmac_user, status_code, mld_user_manage->ttlm_info.dialog_token);
    if (ret == OAL_SUCC && status_code == MAC_SUCCESSFUL_STATUSCODE) {
        mld_user_manage->ttlm_info.is_advertised = OAL_FALSE;
        hmac_set_mld_user_manage_ttlm_info(mld_user_manage, &ttlm);
        hmac_set_ttlm_status(mld_user_manage, WLAN_TTLM_COMPLETE);
        if (hmac_ttlm_update_status(mac_vap, hmac_user) != OAL_SUCC) {
            oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_rx_ttlm_request_process::update ttlm status failed}");
            return OAL_FAIL;
        }
    }
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_ttlm_request_process::rx ttlm request.ret=%u}", ret);
    return ret;
}

OAL_STATIC uint32_t hmac_parse_ttlm_response(hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf)
{
    uint16_t         status_code;
    uint8_t          offset = MAC_ACTION_OFFSET_TOKEN;
    uint8_t         *payload = mac_get_80211_mgmt_payload(netbuf);
    /* 检查Dialog Token */
    if (payload[offset] != hmac_user->mld_user_link_info.mld_user_manage->ttlm_info.dialog_token) {
        return OAL_FAIL;
    }
    offset += 1;
    /* 检查Status Code */
    status_code = *((uint16_t *)(payload + offset));
    if (status_code == MAC_SUCCESSFUL_STATUSCODE) {
        return OAL_SUCC;
    }
    return OAL_FAIL;
}

uint32_t hmac_rx_ttlm_response_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf)
{
    uint32_t         ret;
    mac_vap_stru    *mac_vap = NULL;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    if (oal_unlikely(oal_any_null_ptr3(hmac_vap, hmac_user, netbuf))) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_rx_ttlm_response_process::invalid args.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mac_vap = &hmac_vap->st_vap_base_info;
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_rx_ttlm_response_process::mld_user_manage null.}");
        return OAL_FAIL;
    }
    ret = hmac_parse_ttlm_response(hmac_user, netbuf);
    if (ret == OAL_SUCC) {
        mld_user_manage->ttlm_info.is_advertised = OAL_FALSE;
        hmac_set_ttlm_status(mld_user_manage, WLAN_TTLM_COMPLETE);
        if (hmac_ttlm_update_status(mac_vap, hmac_user) != OAL_SUCC) {
            oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_rx_ttlm_response_process::update ttlm status fail.}");
            return OAL_FAIL;
        }
    }
    if (mld_user_manage->ttlm_info.ttlm_request_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&mld_user_manage->ttlm_info.ttlm_request_timer);
    }
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_ttlm_response_process::rx ttlm response. ret=%u}", ret);
    return ret;
}

uint32_t hmac_rx_ttlm_teardown_process(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    mac_vap_stru    *mac_vap = NULL;
    if (oal_unlikely(oal_any_null_ptr2(hmac_vap, hmac_user))) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_rx_ttlm_teardown_process::invalid args.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mac_vap = &hmac_vap->st_vap_base_info;

    hmac_set_mld_user_default_ttlm_info(hmac_user);
    if (hmac_ttlm_update_status(mac_vap, hmac_user) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_rx_ttlm_teardown_process::update ttlm status failed.}");
        return OAL_FAIL;
    }
    oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_ttlm_teardown_process::rx ttlm teardown success}");
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_beacon_ttlm_ie_check(uint8_t *ttlm_ie, uint8_t ie_len)
{
    fixed_ttlm_control *fixed_ttlm_ctrl = NULL;

    if (ie_len < MAC_IE_EID_LEN + sizeof(fixed_ttlm_control) || ie_len > TTLM_IE_MAX_LEN) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_parse_beacon_ttlm_ie_check::invalid ie len[%u]}", ie_len);
        return OAL_FAIL;
    }

    fixed_ttlm_ctrl = (fixed_ttlm_control *)(ttlm_ie + MAC_IE_EXT_HDR_LEN);

    if (fixed_ttlm_ctrl->direction != WLAN_DOWNUPLINK || fixed_ttlm_ctrl->default_link_mapping != 0 ||
        fixed_ttlm_ctrl->expected_duration_present == 0) {
        oam_warning_log3(0, OAM_SF_ANY,
            "{hmac_parse_beacon_ttlm_ie_check::invalid ttlm param:dir[%u],dflkmp[%u] edp[%d]}",
            fixed_ttlm_ctrl->direction, fixed_ttlm_ctrl->default_link_mapping,
            fixed_ttlm_ctrl->expected_duration_present);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* 解析TID-To-Link Mapping IE */
OAL_STATIC uint32_t hmac_parse_beacon_ttlm_ie(hmac_vap_stru *hmac_vap, uint8_t *ttlm_ie, mac_cfg_ttlm_ie *ttlm_cfg_ie)
{
    uint8_t tid, ttlm_field_len;
    uint8_t offset = MAC_IE_EXT_HDR_LEN;
    uint8_t ie_len = ttlm_ie[1];
    ttlm_control *ttlm_ctrl = NULL;

    if (hmac_parse_beacon_ttlm_ie_check(ttlm_ie, ie_len) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (ie_len + MAC_IE_HDR_LEN < offset + sizeof(ttlm_control)) {
        return OAL_FAIL;
    }
    ttlm_ctrl = (ttlm_control *)(ttlm_ie + offset);

    if (memcpy_s(&(ttlm_cfg_ie->ttlm_ctrl), sizeof(mac_cfg_ttlm_ie), ttlm_ctrl, sizeof(ttlm_control)) != EOK) {
        return OAL_FAIL;
    }
    offset += sizeof(ttlm_control);

    if (ttlm_ctrl->mapping_switch_time_present != 0) {
        if (ie_len + MAC_IE_HDR_LEN < offset + TTLM_MAPPING_SWITCH_TIME_LEN) {
            return OAL_FAIL;
        }
        if (memcpy_s(&(ttlm_cfg_ie->mapping_switch_time), TTLM_MAPPING_SWITCH_TIME_LEN,
            ttlm_ie + offset, TTLM_MAPPING_SWITCH_TIME_LEN) != EOK) {
            return OAL_FAIL;
        }
        offset += TTLM_MAPPING_SWITCH_TIME_LEN;
    }

    if (ttlm_ctrl->expected_duration_present != 0) {
        if (ie_len + MAC_IE_HDR_LEN < offset + TTLM_EXPECTED_DURATION_LEN) {
            return OAL_FAIL;
        }
        if (memcpy_s(ttlm_cfg_ie->expected_duration, TTLM_EXPECTED_DURATION_LEN,
            ttlm_ie + offset, TTLM_EXPECTED_DURATION_LEN) != EOK) {
            return OAL_FAIL;
        }
        offset += TTLM_EXPECTED_DURATION_LEN;
    }

    ttlm_field_len = (ttlm_ctrl->link_mapping_size == 0 ? 2 : 1); /* 2 表示Link Mapping Of TID字段的长度 */
    if (ie_len + MAC_IE_HDR_LEN < offset + ttlm_field_len * WLAN_TIDNO_BUTT) {
        return OAL_FAIL;
    }
    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        if ((ttlm_ctrl->link_mapping_presence_bitmap & BIT(tid)) == OAL_FALSE) {
            oam_warning_log0(0, OAM_SF_ANY, "{hmac_parse_beacon_ttlm_ie::each tid must mapping to at least one link.}");
            return OAL_FAIL;
        }
        ttlm_cfg_ie->link_bitmap[tid] = (ttlm_ie[offset] & 0xff);
        offset += ttlm_field_len;
    }
    return OAL_SUCC;
}

uint32_t hmac_ttlm_request_time_timeout(void *arg)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = (hmac_user_stru *)arg;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    mac_ttlm_info_stru *ttlm_info = NULL;
    mac_ttlm_stru  *ttlm = NULL;
    uint8_t direction;
    if (oal_unlikely(hmac_user == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_request_time_timeout:hmac_user is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_request_time_timeout:hmac_vap is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_request_time_timeout::mld_user_manage null.}");
        return OAL_FAIL;
    }
    ttlm_info = &(mld_user_manage->ttlm_info);
    direction = ttlm_info->direction;
    if (direction == WLAN_DOWNUPLINK) {
        ttlm = &(ttlm_info->ttlm[WLAN_UPLINK]);
        ttlm->status = WLAN_TTLM_INIT;
        ttlm = &(ttlm_info->ttlm[WLAN_DOWNLINK]);
        ttlm->status = WLAN_TTLM_INIT;
    } else {
        ttlm = &(ttlm_info->ttlm[direction]);
        ttlm->status = WLAN_TTLM_INIT;
    }
    oam_warning_log0(0, OAM_SF_CFG, "{hmac_ttlm_request_time_timeout::ttlm request time out.}");
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ttlm_update_process_in_diff_mlo_state(mac_vap_stru *mac_vap, mac_ttlm_msg_stru *ttlm_msg)
{
    hmac_mld_manage_stru *mld_vap_manage = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *disable_user = NULL;
    hmac_vap_stru *disable_vap = NULL;
    hmac_vap_stru *enable_vap = NULL;
    uint16_t idx;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL || ttlm_msg->count != WLAN_TIDNO_BUTT) {
        return OAL_FAIL;
    }

    /* 非mlsr关联，不执行后续流程 */
    if (hmac_get_mld_assoc_mode(hmac_vap) != MLD_ASSOC_MODE_MLSR) {
        return OAL_SUCC;
    }

    mld_vap_manage = hmac_mld_get_mld_vap_manage(hmac_vap);
    if (mld_vap_manage == NULL) {
        return OAL_FAIL;
    }

    for (idx = 1; idx < ttlm_msg->count; idx++) {
        if (ttlm_msg->ttlm_status[idx].enable == OAL_TRUE ||
            ttlm_msg->ttlm_status[idx].link_user_id != ttlm_msg->ttlm_status[idx - 1].link_user_id ||
            ttlm_msg->ttlm_status[idx].enable != ttlm_msg->ttlm_status[idx - 1].enable) {
            return OAL_SUCC;
        }
    }

    disable_user = (hmac_user_stru *)mac_res_get_hmac_user(ttlm_msg->ttlm_status[0].link_user_id);
    if (disable_user == NULL) {
        return OAL_FAIL;
    }

    disable_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(disable_user->st_user_base_info.uc_vap_id);
    if (disable_vap == NULL) {
        return OAL_FAIL;
    }

    enable_vap = hmac_mld_get_another_vap(disable_vap);
    if (enable_vap == NULL) {
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_EMLSR
    /* emlsr模式下ttlm禁用一路link，emlsr执行退出 并 退回另一路link */
    if (mld_vap_manage->emlsr_record_info.emlsr_state == EMLSR_OPENED) {
        hmac_vap_state_change_notify_emlsr(enable_vap, NULL, COFEATURE_TTLM_EVENT, OAL_FALSE);
        return OAL_SUCC;
    }
#endif

    if (disable_vap->mld_vap_link_info.mld_mode != WLAN_ML_LINK_MODE ||
        enable_vap->mld_vap_link_info.mld_mode != WLAN_LINK_MODE) {
        return OAL_SUCC;
    }

    return hmac_mlsr_link_switch(disable_vap, enable_vap, LINK_SWITCH_FOR_TTLM, 0);
}

OAL_STATIC uint32_t hmac_ttlm_cal_ttlm_msg(hmac_user_stru *hmac_user,
    hmac_mld_user_manage_stru *mld_user_manage, mac_ttlm_stru *mac_ttlm, mac_ttlm_msg_stru *ttlm_msg)
{
    hmac_user_stru *link_hmac_user = NULL;
    uint8_t ap_link_id;
    uint8_t link_id, tid, status;
    uint32_t count = 0;

    for (link_id = 0; link_id < mld_user_manage->mld_link_cnt; link_id++) {
        link_hmac_user = mac_res_get_hmac_user(mld_user_manage->record_link_info[link_id].link_user_id);
        if (link_hmac_user == NULL) {
            continue;
        }
        ap_link_id = link_hmac_user->mld_user_link_info.ap_link_id;

        for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
            status = (uint8_t)((mac_ttlm->link_bitmap[tid] & BIT(ap_link_id)) >> ap_link_id);
            if (status != mld_user_manage->ttlm_info.ttlm_status[tid][link_id]) {
                mld_user_manage->ttlm_info.ttlm_status[tid][link_id] = status;
                ttlm_msg->ttlm_status[count].tid = tid;
                ttlm_msg->ttlm_status[count].enable = mld_user_manage->ttlm_info.ttlm_status[tid][link_id];
                ttlm_msg->ttlm_status[count].link_user_id = mld_user_manage->record_link_info[link_id].link_user_id;
                count++;
            }
        }
    }
    ttlm_msg->count = count;

    return OAL_SUCC;
}

uint32_t hmac_ttlm_update_status(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user)
{
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    mac_ttlm_stru *mac_ttlm = NULL;
    mac_ttlm_msg_stru ttlm_msg;
    uint32_t ret;

    if (mac_vap == NULL || hmac_user == NULL || hmac_user->mld_user_link_info.mld_user_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_update_status::input praram is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    mac_ttlm = &(mld_user_manage->ttlm_info.ttlm[WLAN_UPLINK]);
    if (mac_ttlm->status == WLAN_TTLM_PROCESS) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_ttlm_update_status::mac_ttlm status is in progress!}");
        return OAL_FAIL;
    }

    memset_s(&ttlm_msg, sizeof(mac_ttlm_msg_stru), 0, sizeof(mac_ttlm_msg_stru));
    hmac_ttlm_cal_ttlm_msg(hmac_user, mld_user_manage, mac_ttlm, &ttlm_msg);
    if (ttlm_msg.count == 0) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_ttlm_update_status::no status changed}");
        return OAL_SUCC;
    }

    ret = hmac_ttlm_update_process_in_diff_mlo_state(mac_vap, &ttlm_msg);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_ML_TTLM_STATUS, sizeof(mac_ttlm_msg_stru), (uint8_t *)&ttlm_msg);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_config_mld_ttlm_enable::send_event failed:%u}", ret);
        return ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ttlm_expec_dur_time_timeout(void *arg)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_user_stru *hmac_user = (hmac_user_stru *)arg;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    if (oal_unlikely(hmac_user == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_expec_dur_time_timeout:hmac_user is NULL!}");
        return OAL_SUCC;
    }
    mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (oal_unlikely(mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_expec_dur_time_timeout:hmac_vap is not exist!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_expec_dur_time_timeout::mld_user_manage null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_set_mld_user_default_ttlm_info(hmac_user);
    hmac_set_ttlm_status(mld_user_manage, WLAN_TTLM_COMPLETE);
    if (hmac_ttlm_update_status(mac_vap, hmac_user) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_expec_dur_time_timeout::update ttlm status failed.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ttlm_exp_duration_time_proc(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    hmac_mld_user_manage_stru *mld_user_manage, uint32_t exp_dur_time, uint32_t diff_time)
{
    uint32_t exp_dur_period;

    // 2,1 数组下标， 16，8 字节偏移
    if (exp_dur_time < diff_time) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_ttlm_start_timers::expected duration time[%u] not right!}", exp_dur_time);
        return OAL_FAIL;
    }
    if (exp_dur_time <= diff_time + mac_mib_get_BeaconPeriod(mac_vap)) {
        if (mld_user_manage->ttlm_info.ttlm_duration_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&mld_user_manage->ttlm_info.ttlm_duration_timer);
        }
        return hmac_ttlm_expec_dur_time_timeout((void*)hmac_user);
    }

    exp_dur_period = exp_dur_time - diff_time - mac_mib_get_BeaconPeriod(mac_vap);
    oam_warning_log1(0, OAM_SF_ANY, "{hmac_ttlm_start_timers::exp_dur_period[%u]!}", exp_dur_period);
    frw_timer_create_timer_m(&(mld_user_manage->ttlm_info.ttlm_duration_timer), hmac_ttlm_expec_dur_time_timeout,
        exp_dur_period, (void *)hmac_user, OAL_FALSE, OAM_MODULE_ID_HMAC, mac_vap->core_id);

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_ttlm_map_switch_time_timeout(void *arg)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    mac_ttlm_frw_timer_stru *ttlm_frw_info = (mac_ttlm_frw_timer_stru *)arg;

    if (ttlm_frw_info == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(ttlm_frw_info->user_index);
    if (hmac_user == NULL || hmac_user->mld_user_link_info.mld_user_manage == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_map_switch_time_timeout:hmac_user or mld_user_mgmt is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;

    hmac_set_ttlm_status(mld_user_manage, WLAN_TTLM_COMPLETE);

    mac_vap = mac_res_get_mac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (oal_unlikely(mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_map_switch_time_timeout:mac_vap not exist!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (hmac_ttlm_update_status(mac_vap, hmac_user) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_map_switch_time_timeout::update ttlm status failed.}");
        return OAL_FAIL;
    }

    return hmac_ttlm_exp_duration_time_proc(mac_vap, hmac_user, mld_user_manage,
                                            ttlm_frw_info->expected_duration, ttlm_frw_info->diff_time);
}

OAL_STATIC uint32_t hmac_ttlm_cal_expected_duaration_time(mac_cfg_ttlm_ie *ttlm_cfg_ie)
{
    return ((ttlm_cfg_ie->expected_duration[BYTE_OFFSET_2] << BIT_OFFSET_16) |
            (ttlm_cfg_ie->expected_duration[BYTE_OFFSET_1] << BIT_OFFSET_8) |
            (ttlm_cfg_ie->expected_duration[BYTE_OFFSET_0]));
}

OAL_STATIC uint32_t hmac_ttlm_map_switch_time_proc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_cfg_ttlm_ie *ttlm_cfg_ie, uint32_t diff_time, uint16_t time_base)
{
    hmac_mld_user_manage_stru *mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    mac_ttlm_frw_timer_stru ttlm_frw_info = {0};
    uint32_t map_sw_period;

    ttlm_frw_info.user_index = hmac_user->st_user_base_info.us_assoc_id;
    ttlm_frw_info.diff_time = diff_time;
    ttlm_frw_info.expected_duration = hmac_ttlm_cal_expected_duaration_time(ttlm_cfg_ie);
    /* 如果当前beacon帧携带的timestamp与mapping switch time相距在一个tbtt以内，则ttlm可以生效 */
    if ((time_base <= ttlm_cfg_ie->mapping_switch_time) &&
        (ttlm_cfg_ie->mapping_switch_time - time_base <= mac_mib_get_BeaconPeriod(&hmac_vap->st_vap_base_info))) {
        if (mld_user_manage->ttlm_info.ttlm_switch_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(&mld_user_manage->ttlm_info.ttlm_switch_timer);
        }
        return hmac_ttlm_map_switch_time_timeout((void *)&ttlm_frw_info);
    }

    /* 启动一个TTLM 生效定时器, 等待TTLM生效；如果已有定时器，则更新定时器超时时间 */
    /* mapping switch time与当前系统时间取差值，确定ttlm建立时间 */
    map_sw_period = ttlm_cfg_ie->mapping_switch_time - time_base - diff_time;
    oam_warning_log1(0, OAM_SF_ANY, "{hmac_ttlm_start_timers::map_sw_period[%u]!}", map_sw_period);
    frw_timer_create_timer_m(&(mld_user_manage->ttlm_info.ttlm_switch_timer),
        hmac_ttlm_map_switch_time_timeout, map_sw_period, (void *)&ttlm_frw_info,
        OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_tllm_start_timers(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_cfg_ttlm_ie *ttlm_cfg_ie, uint32_t diff_time, uint16_t time_base)
{
    hmac_mld_user_manage_stru *mld_user_manage = NULL;
    uint32_t exp_dur_time;

    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_ttlm_start_timers::mld_user_manage null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (ttlm_cfg_ie->ttlm_ctrl.mapping_switch_time_present == 1 &&
        mld_user_manage->ttlm_info.ttlm_duration_timer.en_is_registerd == OAL_TRUE) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_ttlm_start_timers::ttlm_duration_timer already started!}");
        return OAL_FAIL;
    }

    if (ttlm_cfg_ie->ttlm_ctrl.mapping_switch_time_present == 1) {
        /* 如果beacon帧TTLM IE携带mapping switch time，更新mapping switch time定时器 */
        return hmac_ttlm_map_switch_time_proc(hmac_vap, hmac_user, ttlm_cfg_ie, diff_time, time_base);
    }

    /* 不携带mapping switch time，只携带expected duration time，更新expected duration time定时器 */
    exp_dur_time = hmac_ttlm_cal_expected_duaration_time(ttlm_cfg_ie);
    return hmac_ttlm_exp_duration_time_proc(&(hmac_vap->st_vap_base_info), hmac_user,
                                            mld_user_manage, exp_dur_time, diff_time);
}

OAL_STATIC uint32_t hmac_ttlm_cal_diff_time_between_tsf_systime(hmac_vap_stru *hmac_vap, uint32_t timestamp_ms)
{
    uint32_t cur_time = timestamp_ms;

    hal_host_get_tsf_lo(hal_get_host_device(hmac_vap->hal_dev_id), hmac_vap->hal_vap_id, &cur_time);
    cur_time = cur_time >> 10;    // 右移10位转化为ms

    return cur_time - timestamp_ms;
}

OAL_STATIC uint32_t hmac_ttlm_process_timers(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *payload, mac_cfg_ttlm_ie *ttlm_cfg_ie)
{
    uint64_t timestamp;
    uint32_t timestamp_ms;
    uint32_t diff_time;
    uint16_t time_base;

    timestamp = *(uint64_t *)(payload);         // 获取beacon timestamp
    timestamp_ms = (uint32_t)(((uint32_t)timestamp) >> 10); // 10 bit us 转为 1ms
    time_base = (uint16_t)(((uint32_t)timestamp) >> 10); /* 取timestamp的bit10-bit25 */
    diff_time = hmac_ttlm_cal_diff_time_between_tsf_systime(hmac_vap, timestamp_ms);
    if (hmac_tllm_start_timers(hmac_vap, hmac_user, ttlm_cfg_ie, diff_time, time_base) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ttlm_process_timers::start timers failed}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_sta_rx_beacon_handle_ttlm_ie(hmac_vap_stru *hmac_vap, uint8_t *payload,
    mac_cfg_ttlm_ie *ttlm_cfg_ie)
{
    hmac_user_stru             *hmac_user = NULL;
    hmac_mld_user_manage_stru  *mld_user_manage = NULL;
    mac_ttlm_stru               ttlm_tmp;
    uint8_t                     tid;
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (oal_unlikely(hmac_user == NULL)) {
        oam_warning_log0(0, OAM_SF_WPA, "{hmac_sta_rx_beacon_handle_ttlm_ie::get user null!}");
        return OAL_FAIL;
    }
    mld_user_manage = hmac_user->mld_user_link_info.mld_user_manage;
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_sta_rx_beacon_handle_ttlm_ie::mld_user_manage null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (hmac_ttlm_process_timers(hmac_vap, hmac_user, payload, ttlm_cfg_ie) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_sta_rx_beacon_handle_ttlm_ie::start ttlm timers failed.}");
        return OAL_FAIL;
    }
    memset_s(&ttlm_tmp, sizeof(mac_ttlm_stru), 0, sizeof(mac_ttlm_stru));
    ttlm_tmp.default_mapping = 0;
    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        ttlm_tmp.link_bitmap[tid] = ttlm_cfg_ie->link_bitmap[tid];
    }
    mld_user_manage->ttlm_info.direction = WLAN_DOWNUPLINK;
    hmac_set_mld_user_manage_ttlm_info(mld_user_manage, &ttlm_tmp);
    mld_user_manage->ttlm_info.is_advertised = OAL_TRUE;
    oam_warning_log0(0, OAM_SF_WPA, "{hmac_sta_rx_beacon_handle_ttlm_ie success!}");
    return OAL_SUCC;
}

uint32_t hmac_sta_rx_beacon_ttlm_process(hmac_vap_stru *hmac_vap, uint8_t *payload, mac_ies_info *ies_info)
{
    mac_cfg_ttlm_ie         ttlm_cfg_ie;
    uint8_t                 *ttlm_ie = NULL;
    uint32_t                ret;
    if (oal_unlikely((oal_any_null_ptr3(hmac_vap, payload, ies_info)))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sta_rx_beacon_ttlm_process::invalid arguments.}");
        return OAL_FAIL;
    }

    ttlm_ie = mac_find_ie_ext_ie(MAC_EID_EXTENSION, MAC_EID_EXT_EHT_TIDTOLINK, ies_info);
    if (ttlm_ie == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_sta_rx_beacon_ttlm_process::not find ttlm element.}");
        return OAL_SUCC;
    }
    memset_s(&ttlm_cfg_ie, sizeof(mac_cfg_ttlm_ie), 0, sizeof(mac_cfg_ttlm_ie));
    ret = hmac_parse_beacon_ttlm_ie(hmac_vap, ttlm_ie, &ttlm_cfg_ie);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_sta_rx_beacon_ttlm_process::parse ttlm element failed.}");
        return OAL_FAIL;
    }
    ret = hmac_sta_rx_beacon_handle_ttlm_ie(hmac_vap, payload, &ttlm_cfg_ie);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_sta_rx_beacon_ttlm_process::handle ttlm element failed.}");
        return OAL_FAIL;
    }
    oam_warning_log0(0, OAM_SF_WPA, "{hmac_sta_rx_beacon_ttlm_process success!}");
    return OAL_SUCC;
}

void hmac_ttlm_destory_timers(hmac_mld_user_manage_stru  *mld_user_manage)
{
    if (oal_unlikely(mld_user_manage == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_sta_rx_beacon_ttlm_process::mld_user_manage null.}");
        return;
    }
    if (mld_user_manage->ttlm_info.ttlm_switch_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(mld_user_manage->ttlm_info.ttlm_switch_timer));
    }
    if (mld_user_manage->ttlm_info.ttlm_duration_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(mld_user_manage->ttlm_info.ttlm_duration_timer));
    }
    if (mld_user_manage->ttlm_info.ttlm_request_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(mld_user_manage->ttlm_info.ttlm_request_timer));
    }
    return;
}

#endif
