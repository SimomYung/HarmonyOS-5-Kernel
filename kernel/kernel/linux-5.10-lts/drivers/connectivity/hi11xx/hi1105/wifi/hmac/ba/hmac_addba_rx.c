/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : AMPDU聚合、BA处理接口定义源文件
 * 作    者 :
 * 创建日期 : 2014年11月25日
 */

/* 1 头文件包含 */
#include "wlan_spec.h"
#include "mac_vap.h"
#include "hmac_rx_reorder.h"
#include "hmac_main.h"
#include "hmac_rx_data.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_user.h"
#include "hmac_auto_adjust_freq.h"
#include "wlan_chip_i.h"
#include "mac_mib.h"
#include "mac_frame_11be.h"
#include "hmac_config.h"
#include "hmac_tx_data.h"
#include "dpe_wlan_vap.h"
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#include "hmac_btcoex.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ADDBA_RX_C

mac_rx_buffer_size_stru g_st_rx_buffer_size_stru = { 0 };
mac_rx_buffer_size_stru *mac_vap_get_rx_buffer_size(void)
{
    return &g_st_rx_buffer_size_stru;
}
#define TX_ADDBA_RSP
OAL_STATIC void hmac_btcoex_set_addba_rsp_ba_param(
    void *vap, void *user, hmac_ba_rx_para_stru *hmac_ba_rx_info, uint16_t *ba_param)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)vap;
    hmac_user_stru *hmac_user = (hmac_user_stru *)user;
    hmac_user_btcoex_stru *hmac_user_btcoex = NULL;
    if (mac_btcoex_check_valid_sta(&(hmac_vap->st_vap_base_info)) == OAL_TRUE) {
        hmac_user_btcoex = &(hmac_user->st_hmac_user_btcoex);
        /* 1.黑名单用户 */
        if (hmac_user_btcoex->btcoex_addba_req.en_ba_handle_allow == OAL_FALSE) {
            /* BIT6~BIT15赋值前需要清理内容 */
            *ba_param &= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
            if (hmac_user->st_hmac_user_btcoex.btcoex_addba_req.en_blacklist_tpye == BTCOEX_BLACKLIST_TPYE_FIX_BASIZE) {
                *ba_param |= (uint16_t)(BTCOEX_BLACKLIST_BA_SIZE_LIMIT << 6); /* BA SIZE在BA param set的BIT6~BIT15 */
            } else {
                /* 黑名单时，btcoex聚合业务处于结束状态，按照默认聚合个数恢复wifi性能 */
                *ba_param |= (uint16_t)(hmac_ba_rx_info->us_baw_size << 6); /* BA SIZE在BA param set的BIT6~BIT15 */
            }
        } else if ((hmac_user_btcoex->en_delba_btcoex_trigger == OAL_TRUE) &&
                   (hmac_user_btcoex->us_ba_size != 0)) { /* 2.共存触发的删除，按照共存配置来 */
            /* BIT6~BIT15赋值前需要清理内容 */
            *ba_param &= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
            *ba_param |= (uint16_t)(hmac_user_btcoex->us_ba_size << 6); /* 最大聚合个数是BA param set的BIT6~BIT15 */
        }
    }
}

OAL_STATIC void hmac_mgmt_encap_addba_rsp_set_ba_param(hmac_vap_stru *pst_vap,
    hmac_user_stru *pst_hmac_user, hmac_ba_rx_para_stru *pst_addba_rsp, uint8_t uc_tid, uint16_t *pus_ba_param)
{
#if defined _PRE_WLAN_FEATURE_11AX || defined _PRE_WLAN_FEATURE_11BE
    oal_bool_enum_uint8 is_he_implemented;

    // 降低圈复杂度
    is_he_implemented = ((g_wlan_spec_cfg->feature_11ax_is_open || g_wlan_spec_cfg->feature_11be_is_open) &&
                            ((mac_mib_get_HEOptionImplemented(&pst_vap->st_vap_base_info) == OAL_TRUE) &&
                            (mac_user_is_he_user(&pst_hmac_user->st_user_base_info))));

    *pus_ba_param |= (uint16_t)(hmac_addba_get_buffer_size(pst_hmac_user->aus_tid_baw_size[uc_tid]) << 6); /* 左移6位 */

    /* 标志vap工作在11ax */
    if (is_he_implemented) {
        *pus_ba_param |= (uint16_t)(pst_hmac_user->aus_tid_baw_size[uc_tid] << 6); /* BA param set的BIT6~BIT15 */
    } else {
        *pus_ba_param |= (uint16_t)(pst_addba_rsp->us_baw_size << 6); /* 最大聚合个数是BA param set的BIT6~BIT15 */
    }
#else
    *pus_ba_param |= (uint16_t)(pst_addba_rsp->us_baw_size << 6); /* 最大聚合个数是BA param set的BIT6~BIT15 */
#endif
    hmac_btcoex_set_addba_rsp_ba_param(pst_vap, pst_hmac_user, pst_addba_rsp, pus_ba_param);
}

/*
 * 1.日    期  : 2013年4月9日
  *   修改内容  : 新生成函数
 */
uint16_t hmac_mgmt_encap_addba_rsp(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *data,
    hmac_ba_rx_para_stru *addba_rsp, uint8_t tid, uint8_t status)
{
    uint16_t index;
    uint16_t ba_param;
    uint8_t para_set;
    uint8_t ie_len;
    hmac_addba_exten_ie_union addba_ie;
    if (oal_any_null_ptr4(hmac_vap, hmac_user, data, addba_rsp)) {
        oam_error_log0(0, OAM_SF_BA,
                       "{hmac_mgmt_encap_addba_rsp::hmac_vap or hmac_user or data or addba_rsp is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* All the fields of the Frame Control Field are set to zero. Only the   */
    /* Type/Subtype field is set.                                            */
    mac_hdr_set_frame_control(data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    /* DA is address of STA requesting association */
    oal_set_mac_addr(data + WLAN_HDR_ADDR1_OFFSET, hmac_user->st_user_base_info.auc_user_mac_addr);
    /* SA is the dot11MACAddress */
    oal_set_mac_addr(data + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(&hmac_vap->st_vap_base_info));
    oal_set_mac_addr(data + WLAN_HDR_ADDR3_OFFSET, hmac_vap->st_vap_base_info.auc_bssid);
    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*             ADDBA Response Frame - Frame Body                         */
    /*    ---------------------------------------------------------------    */
    /*    | Category | Action | Dialog | Status  | Parameters | Timeout |    */
    /*    ---------------------------------------------------------------    */
    /*    | 1        | 1      | 1      | 2       | 2          | 2       |    */
    /*    ---------------------------------------------------------------    */
    /*                                                                       */
    /*************************************************************************/
    /* Initialize index and the frame data pointer */
    index = MAC_80211_FRAME_LEN;
    /* Action Category设置 */
    data[index++] = MAC_ACTION_CATEGORY_BA;
    /* 特定Action种类下的action的帧类型 */
    data[index++] = MAC_BA_ACTION_ADDBA_RSP;
    /* Dialog Token域设置，需要从req中copy过来 */
    data[index++] = addba_rsp->uc_dialog_token;
    /* 状态域设置 */
    data[index++] = status;
    data[index++] = 0;
    /* Block Ack Parameter设置 */
    /* B0 - AMSDU Support, B1- Immediate or Delayed block ack */
    /* B2-B5 : TID, B6-B15: Buffer size */
    ba_param = addba_rsp->en_amsdu_supp;        /* BIT0 */
    ba_param |= (addba_rsp->uc_ba_policy << 1); /* BIT1 */
    ba_param |= (tid << 2);                     /* BIT2 */
    /* 手动设置聚合个数，屏蔽删建BA时不采用64聚合 */
    if (hmac_user != NULL) {
        hmac_mgmt_encap_addba_rsp_set_ba_param(hmac_vap, hmac_user, addba_rsp, tid, &ba_param);
    } else {
        ba_param |= (uint16_t)(hmac_addba_get_buffer_size(addba_rsp->us_baw_size) << 6); /* BIT6 */
    }
    data[index++] = (uint8_t)(ba_param & 0xFF);
    data[index++] = (uint8_t)((ba_param >> 8) & 0xFF); /* BIT8 */
    /* BlockAck timeout值设置 解决timeout兼容性问题 */
    data[index++] = 0x00;
    data[index++] = 0x00;

    if (addba_rsp->extension_ie_received == OAL_TRUE) {
        addba_ie.exten_ie.extended_buffer_size = hmac_addba_get_extended_buffer_size(addba_rsp->us_baw_size);
        para_set = ((addba_rsp->addba_para_set & ADDBA_EXTENDED_BUFFER_SIZE_MASK) == 0x0) ? 0x0 : addba_ie.value;
        /* 若对端的extend buffer size段为0,则addba rsp中也填0，否则填写1支持1 * 1024个buffer,其他分片相关能力默认不支持 */
        /* extend buffer size即使host填写了1,device也可能因为分配不到rx_ba_lut而再次清零 */
        if (mac_set_addba_extension_ie(&data[index], &ie_len, para_set) == OAL_SUCC) {
            index += ie_len;
        }
    }
    /* 返回的帧长度中不包括FCS */
    return index;
}

static uint32_t hmac_mgmt_tx_addba_rsp_check(hmac_vap_stru *hmac_vap)
{
    mac_vap_stru *mac_vap = NULL;
    mac_device_stru *mac_dev = NULL;

    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_rsp::hmac_vap or hmac_user or ba_rx_info is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = &(hmac_vap->st_vap_base_info);
    /*  确定vap处于工作状态 */
    if (mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_tx_addba_rsp:: vap has been down/del, vap_state[%d].}", mac_vap->en_vap_state);
        return OAL_FAIL;
    }
    /*  END */
    /* 获取device结构 */
    mac_dev = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_dev == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_rsp::send addba rsp failed, mac_dev null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}
static void hmac_mgmt_tx_addba_rsp_fill_cb(mac_tx_ctl_stru *tx_ctl, hmac_user_stru *hmac_user,
    uint8_t tid, uint16_t frame_len)
{
    mac_get_cb_tx_user_idx(tx_ctl) = hmac_user->st_user_base_info.us_assoc_id;
    mac_get_cb_wme_tid_type(tx_ctl) = tid;
    mac_get_cb_mpdu_len(tx_ctl) = frame_len + sizeof(dmac_ctx_action_event_stru);
    mac_get_cb_frame_type(tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    mac_get_cb_frame_subtype(tx_ctl) = WLAN_ACTION_BA_ADDBA_RSP;
}
/*
 * 1.日    期  : 2014年11月29日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_mgmt_tx_addba_rsp(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, hmac_ba_rx_para_stru *ba_rx_info,
    uint8_t tid, uint8_t status)
{
    mac_vap_stru *mac_vap = NULL;
    dmac_ctx_action_event_stru wlan_ctx_action;
    oal_netbuf_stru *addba_rsp = NULL;
    uint16_t frame_len;
    uint32_t ret;
    mac_tx_ctl_stru *tx_ctl = NULL;

    ret = hmac_mgmt_tx_addba_rsp_check(hmac_vap);
    if (ret != OAL_SUCC) {
        return ret;
    }
    mac_vap = &(hmac_vap->st_vap_base_info);
    /* 申请ADDBA_RSP管理帧内存 */
    addba_rsp = hmac_mgmt_ba_session_alloc(mac_vap->uc_vap_id);
    if (addba_rsp == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    frame_len = hmac_mgmt_encap_addba_rsp(hmac_vap, hmac_user, oal_netbuf_data(addba_rsp), ba_rx_info, tid, status);

    memset_s((uint8_t *)&wlan_ctx_action, sizeof(wlan_ctx_action), 0, sizeof(wlan_ctx_action));
    wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_BA;
    wlan_ctx_action.uc_action = MAC_BA_ACTION_ADDBA_RSP;
    wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    wlan_ctx_action.us_baw_size = hmac_user->aus_tid_baw_size[tid];
    wlan_ctx_action.us_frame_len = frame_len;
    wlan_ctx_action.us_user_idx = hmac_user->st_user_base_info.us_assoc_id;
    wlan_ctx_action.uc_tidno = tid;
    wlan_ctx_action.uc_status = status;
    wlan_ctx_action.us_ba_timeout = ba_rx_info->us_ba_timeout;
    wlan_ctx_action.en_back_var = ba_rx_info->en_back_var;
    wlan_ctx_action.uc_lut_index = ba_rx_info->uc_lut_index;
    wlan_ctx_action.us_baw_start = ba_rx_info->us_baw_start;
    wlan_ctx_action.uc_ba_policy = ba_rx_info->uc_ba_policy;

    if (memcpy_s((uint8_t *)(oal_netbuf_data(addba_rsp) + frame_len), (WLAN_MEM_NETBUF_SIZE2 - frame_len),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oam_warning_log0(0, 0, "{hmac_mgmt_tx_addba_rsp::memcpy fail}");
    }

    oal_netbuf_put(addba_rsp, (frame_len + sizeof(dmac_ctx_action_event_stru)));

    /* 填写netbuf的cb字段，供发送管理帧和发送完成接口使用 */
    memset_s(oal_netbuf_cb(addba_rsp), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(addba_rsp);
    hmac_mgmt_tx_addba_rsp_fill_cb(tx_ctl, hmac_user, tid, frame_len);

    ret = hmac_tx_action_event(mac_vap, addba_rsp, frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(addba_rsp);
    }
    hmac_rx_ba_reset_pre_handle(hmac_vap, hmac_user, tid);
    dpe_wlan_rx_ba_init(ba_rx_info, mac_vap->uc_vap_id, hmac_user->st_user_base_info.us_assoc_id, tid);

    return ret;
}

#define RX_ADDBA_REQ
OAL_STATIC uint32_t dbac_need_change_ba_param(mac_device_stru *mac_dev, mac_vap_stru *mac_vap)
{
    if (mac_dev == NULL || mac_vap == NULL) {
        return OAL_FALSE;
    }

    return (mac_dev->en_dbac_running == OAL_TRUE && mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA);
}

OAL_STATIC void hmac_ba_rx_baw_init_he_eht(hmac_user_stru *hmac_user,
    hmac_ba_rx_para_stru *ba_rx_stru, uint8_t tid, uint16_t baw_size)
{
    mac_rx_buffer_size_stru *rx_buffer_size = mac_vap_get_rx_buffer_size();
    /* resp反馈值与addba req保持一致,但是BA BAW需要更新,避免兼容性问题,因此不更新user */
    ba_rx_stru->us_baw_size = baw_size;

    /* 定制化设定BA BAW, 则取小 */
    if (rx_buffer_size->en_rx_ampdu_bitmap_ini == OAL_TRUE) {
        ba_rx_stru->us_baw_size =
            oal_min(rx_buffer_size->us_rx_buffer_size, baw_size);
    }

    /* 兼容性问题:如果决策的BAW size 小于对端BAW size,那么更新resp帧BAW size,否则按照对端的BAW size反馈 */
    hmac_user->aus_tid_baw_size[tid] =
        oal_min(hmac_user->aus_tid_baw_size[tid], ba_rx_stru->us_baw_size);
    ba_rx_stru->us_baw_size = hmac_user->aus_tid_baw_size[tid];
}

OAL_STATIC uint8_t hmac_mgmt_check_rx_ba_policy(hmac_vap_stru *pst_hmac_vap, hmac_ba_rx_para_stru *ba_rx_info)
{
    /* 立即块确认判断 */
    if (ba_rx_info->uc_ba_policy == MAC_BA_POLICY_IMMEDIATE) {
        if (mac_mib_get_dot11ImmediateBlockAckOptionImplemented(&pst_hmac_vap->st_vap_base_info) == OAL_FALSE) {
            /* 不支持立即块确认 */
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                             "{hmac_mgmt_check_set_rx_ba_ok::not support immediate Block Ack.}");
            return MAC_INVALID_REQ_PARAMS;
        } else {
            if (ba_rx_info->en_back_var != MAC_BACK_COMPRESSED) {
                /* 不支持非压缩块确认 */
                oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                                 "{hmac_mgmt_check_set_rx_ba_ok::not support non-Compressed Block Ack.}");
                return MAC_REQ_DECLINED;
            }
        }
    } else if (ba_rx_info->uc_ba_policy == MAC_BA_POLICY_DELAYED) {
        /* 延迟块确认不支持 */
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_check_set_rx_ba_ok::not support delayed Block Ack.}");
        return MAC_INVALID_REQ_PARAMS;
    }
    return MAC_SUCCESSFUL_STATUSCODE;
}

/*
 * 函 数 名  : hmac_mgmt_check_set_rx_ba_ok
 * 功能描述  : 从空口接收ADDBA_REQ帧的处理函数
 * 1.日    期  : 2014年11月28日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint8_t hmac_mgmt_check_set_rx_ba_ok(hmac_vap_stru *pst_hmac_vap,
                                     hmac_user_stru *pst_hmac_user,
                                     hmac_ba_rx_para_stru *ba_rx_info,
                                     mac_device_stru *pst_device,
                                     uint8_t tid)
{
    hmac_ba_stru *ba_info = NULL;
    mac_chip_stru *pst_mac_chip = NULL;
    uint8_t ba_status;
    uint8_t ret;

    ba_rx_info->uc_lut_index = MAC_INVALID_RX_BA_LUT_INDEX;
    ret = hmac_mgmt_check_rx_ba_policy(pst_hmac_vap, ba_rx_info);
    if (ret != MAC_SUCCESSFUL_STATUSCODE) {
        return ret;
    }

    pst_mac_chip = hmac_res_get_mac_chip(pst_device->uc_chip_id);
    if (pst_mac_chip == NULL) {
        return MAC_REQ_DECLINED;
    }
    if (mac_mib_get_RxBASessionNumber(&pst_hmac_vap->st_vap_base_info) > g_wlan_spec_cfg->max_rx_ba) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_check_set_rx_ba_ok::uc_rx_ba_session_num[%d] is up to max.}\r\n",
                         mac_mib_get_RxBASessionNumber(&pst_hmac_vap->st_vap_base_info));
        return MAC_REQ_DECLINED;
    }

    /* 获取BA LUT INDEX */
    ba_rx_info->uc_lut_index = hmac_ba_get_lut_index(pst_mac_chip->st_lut_table.auc_rx_ba_lut_idx_table,
        0, HAL_MAX_RX_BA_LUT_SIZE);

    if ((ba_rx_info->uc_lut_index == MAC_INVALID_RX_BA_LUT_INDEX) &&
        (wlan_chip_ba_need_check_lut_idx() == OAL_TRUE)) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, 0, "{hmac_mgmt_check_set_rx_ba_ok:ba lut idx tb full");
        return MAC_REQ_DECLINED;
    }
    ba_status = hmac_btcoex_check_addba_req(pst_hmac_vap, pst_hmac_user);
    if (ba_status != MAC_SUCCESSFUL_STATUSCODE) {
        return ba_status;
    }
    /* 该tid下不允许建BA，配置命令需求 */
    ba_info = hmac_get_ba_info(pst_hmac_user, tid);
    if (ba_info->en_ba_handle_rx_enable == OAL_FALSE) {
        oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_check_set_rx_ba_ok::uc_tid_no[%d] user_idx[%d] is not available.}",
                         tid, pst_hmac_user->st_user_base_info.us_assoc_id);
        return MAC_REQ_DECLINED;
    }
    return MAC_SUCCESSFUL_STATUSCODE;
}
/*
 * 函 数 名   : hmac_process_addba_req_exten_ie
 * 功能描述   : 处理addba req中的extension ie,记录ADDBA Additional Parameter Set
 */
void hmac_process_addba_req_exten_ie(hmac_ba_rx_para_stru *ba_rx_stru, hmac_user_stru *hmac_user, uint8_t *payload,
    uint16_t payload_len, uint8_t tid)
{
    uint8_t *ie_ptr = NULL;
    mac_ies_info ies_info = { 0 };

    if (oal_unlikely(oal_any_null_ptr2(ba_rx_stru, payload))) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_process_addba_req_exten_ie::null ptr.}");
        return;
    }
    ies_info.frame_ies = payload + MAC_ADDBA_RSP_FRAME_BODY_LEN;
    if (payload_len < MAC_ADDBA_RSP_FRAME_BODY_LEN) { // 长度合法性判断，避免回绕
        oam_error_log1(0, OAM_SF_BA, "{hmac_process_addba_req_exten_ie:payload_len invalid [%d].}", payload_len);
        return;
    }
    ies_info.frame_len = payload_len - MAC_ADDBA_RSP_FRAME_BODY_LEN;

    if (payload_len >= MAC_ADDBA_REQ_FRAME_BODY_LEN + MAC_IE_HDR_LEN + MAC_ADDBA_EXTENSION_IE_LEN) {
        ie_ptr = mac_find_ie(MAC_EID_ADDBA_EXTENSION, &ies_info);
        if ((ie_ptr != NULL) && (ie_ptr[1] == MAC_ADDBA_EXTENSION_IE_LEN)) {
            ba_rx_stru->extension_ie_received = OAL_TRUE;
            ba_rx_stru->addba_para_set = ie_ptr[MAC_IE_HDR_LEN];
            oam_warning_log3(0, OAM_SF_BA, "{hmac_process_addba_req_exten_ie:user:%d tid:%d received extension ie,\
                parameter set:%x.}", hmac_user->st_user_base_info.us_assoc_id, tid, ie_ptr[MAC_IE_HDR_LEN]);
#ifdef _PRE_WLAN_FEATURE_11BE
            if (((ba_rx_stru->addba_para_set & ADDBA_EXTENDED_BUFFER_SIZE_MASK) != 0) &&
                (mac_user_get_eht_capable(&hmac_user->st_user_base_info) == OAL_TRUE)) {
                ba_rx_stru->us_baw_size = ba_rx_stru->us_baw_size +
                    (ba_rx_stru->addba_para_set >>  ADDBA_EXTENDED_BUFFER_SIZE_OFFSET) *
                    ADDBA_EXTENDED_BUFFER_SIZE_UNIT;
            }
#endif
        }
    }
}

/*
 * 功能描述  : 初始化ba相关参数
 * 1.日    期  : 2018年8月4日
 *   修改内容  : 新生成函数
 */
void hmac_ba_rx_hdl_baw_init(hmac_vap_stru *hmac_vap, hmac_user_stru *pst_hmac_user,
    hmac_ba_rx_para_stru *pst_ba_rx_stru, uint8_t uc_tid)
{
    mac_rx_buffer_size_stru *rx_buffer_size = mac_vap_get_rx_buffer_size();
    int32_t chip_type = get_mpxx_subchip_type();
    mac_device_stru *mac_dev = mac_res_get_dev(pst_hmac_user->st_user_base_info.uc_device_id);

    /* 保存ba size实际大小,用于反馈addba rsp */
    pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;

    /*  解决goldenap新固件版本兼容性问题 */
    if (pst_ba_rx_stru->us_baw_size <= 1) {
        pst_ba_rx_stru->us_baw_size = WLAN_AMPDU_RX_BUFFER_SIZE;
        /* 异常更新反馈值 */
        pst_hmac_user->aus_tid_baw_size[uc_tid] = WLAN_AMPDU_RX_BUFFER_SIZE;
    }

    if (g_wlan_spec_cfg->feature_11be_is_open &&
        ((pst_ba_rx_stru->us_baw_size > WLAN_AMPDU_RX_HE_BUFFER_SIZE) &&
        mac_user_is_eht_user(&pst_hmac_user->st_user_base_info))) {
        hmac_ba_rx_baw_init_he_eht(pst_hmac_user, pst_ba_rx_stru, uc_tid, WLAN_AMPDU_RX_EHT_BUFFER_SIZE);
    } else if (g_wlan_spec_cfg->feature_11ax_is_open &&
        ((pst_ba_rx_stru->us_baw_size > WLAN_AMPDU_RX_BUFFER_SIZE) &&
        mac_user_is_he_user(&pst_hmac_user->st_user_base_info))) {
        /* HE设备并且BA窗大于64时 */
        hmac_ba_rx_baw_init_he_eht(pst_hmac_user, pst_ba_rx_stru, uc_tid, WLAN_AMPDU_RX_HE_BUFFER_SIZE);
    } else {
        /* BA窗小于64时协商BA窗SIZE */
        pst_ba_rx_stru->us_baw_size = oal_min(pst_ba_rx_stru->us_baw_size, WLAN_AMPDU_RX_BUFFER_SIZE);
        /* 非HE设备更新,反馈值与baw同步更新 */
        pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;
    }
    /* DBAC 减小baw */
    if (dbac_need_change_ba_param(mac_dev, &hmac_vap->st_vap_base_info) == OAL_TRUE) {
        pst_ba_rx_stru->us_baw_size = oal_min(pst_ba_rx_stru->us_baw_size, WLAN_AMPDU_RX_BUFFER_SIZE);
        pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;
    }
    /* 不支持DDR收发产品,关闭256聚合 */
    if ((chip_type >= BOARD_VERSION_MP16) && (dpe_rx_ring_switch_enabled() == OAL_FALSE)) {
        pst_ba_rx_stru->us_baw_size = oal_min(pst_ba_rx_stru->us_baw_size, WLAN_AMPDU_RX_BUFFER_SIZE);
        pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;
    }

    /* 配置优先级最高，手动配置接收聚合个数 */
    if (rx_buffer_size->en_rx_ampdu_bitmap_cmd == OAL_TRUE) {
        pst_ba_rx_stru->us_baw_size = rx_buffer_size->us_rx_buffer_size;
        pst_hmac_user->aus_tid_baw_size[uc_tid] = pst_ba_rx_stru->us_baw_size;
    }
}
void hmac_rx_ba_reset_pre_handle(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t tid)
{
    mac_chip_stru *pst_mac_chip = NULL;
    hmac_ba_stru *ba_info = hmac_get_ba_info(hmac_user, tid);
    if (ba_info->en_ba_status != DMAC_BA_COMPLETE) {
        return;
    }
    /* dec the ba session cnt maitence in device struc */
    hmac_rx_ba_session_decr(hmac_vap, tid);

    /* if lut index is valid, del lut index alloc before */
    if (ba_info->uc_lut_index == MAC_INVALID_RX_BA_LUT_INDEX) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_rx_ba_reset_pre_handle::no need to del lut index, lut index[%d]}\n",
                         ba_info->uc_lut_index);
        return;
    }
    pst_mac_chip = hmac_res_get_mac_chip(hmac_vap->st_vap_base_info.uc_chip_id);
    if (pst_mac_chip == NULL) {
        return;
    }
    hmac_ba_del_lut_index(pst_mac_chip->st_lut_table.auc_rx_ba_lut_idx_table, ba_info->uc_lut_index);
    ba_info->uc_lut_index = MAC_INVALID_RX_BA_LUT_INDEX;
}
uint32_t hmac_rx_ba_info_init(mac_device_stru *dev, hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t tid,
    uint8_t *payload, uint32_t framelen)
{
    hmac_ba_rx_para_stru ba_rx_info = { 0 };
    hmac_ba_stru *ba_info = hmac_get_ba_info(hmac_user, tid);
    uint32_t ret;
    uint8_t status;

    /* Ba会话参数初始化 */
    ba_rx_info.en_back_var = MAC_BACK_COMPRESSED;
    ba_rx_info.uc_dialog_token = payload[BYTE_OFFSET_2];
    ba_rx_info.uc_ba_policy = (payload[BYTE_OFFSET_3] & 0x02) >> 1;
    ba_rx_info.us_ba_timeout = payload[BYTE_OFFSET_5] | (payload[BYTE_OFFSET_6] << 8); /* bit8 */

    /* 初始化接收窗口 */
    ba_rx_info.us_baw_start = (payload[BYTE_OFFSET_7] >> 4) | (payload[BYTE_OFFSET_8] << 4); /* bit4 */
    ba_rx_info.us_baw_size = (payload[BYTE_OFFSET_3] & 0xC0) >> 6; /* bit6 */
    ba_rx_info.us_baw_size |= (payload[BYTE_OFFSET_4] << 2); /* bit2 */
    hmac_process_addba_req_exten_ie(&ba_rx_info, hmac_user, (uint8_t *)payload, framelen, tid);

    mac_mib_incr_RxBASessionNumber(&(hmac_vap->st_vap_base_info));

    /* DBAC不开启rx amsdu */
    if (dbac_need_change_ba_param(dev, &hmac_vap->st_vap_base_info) == OAL_TRUE) {
        hmac_vap->bit_rx_ampduplusamsdu_active = 0;
    }
#ifdef _PRE_WLAN_FEATURE_PWL
    /* pwl模式不支持amsdu */
    if (hmac_vap->st_vap_base_info.pwl_enable == OAL_TRUE) {
        hmac_vap->bit_rx_ampduplusamsdu_active = 0;
    }
#endif
    ba_rx_info.en_amsdu_supp =
        ((hmac_vap->bit_rx_ampduplusamsdu_active & hmac_vap->en_ps_rx_amsdu) ? OAL_TRUE : OAL_FALSE);

    /* 初始化BAW */
    hmac_ba_rx_hdl_baw_init(hmac_vap, hmac_user, &ba_rx_info, tid);

    /* 判断建立能否成功 */
    status = hmac_mgmt_check_set_rx_ba_ok(hmac_vap, hmac_user, &ba_rx_info, dev, tid);
    ret = hmac_mgmt_tx_addba_rsp(hmac_vap, hmac_user, &ba_rx_info, tid, status);
    ba_info->uc_lut_index = ba_rx_info.uc_lut_index;
    ba_info->en_ba_status = DMAC_BA_COMPLETE;
    oam_warning_log4(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_MLOG,
        "{hmac_mgmt_rx_addba_req:: tid[%d], status[%d], baw_size[%d], RXAMSDU[%d].}\r\n",
        tid, status, ba_rx_info.us_baw_size, ba_rx_info.en_amsdu_supp);
    if ((status != MAC_SUCCESSFUL_STATUSCODE) || (ret != 0)) {
        hmac_rx_ba_reset_pre_handle(hmac_vap, hmac_user, tid);
        hmac_ba_reset_rx_handle(hmac_user->st_user_base_info.us_assoc_id, tid, OAL_FALSE);
        ba_info->en_ba_status = DMAC_BA_INIT;
    }

    return OAL_SUCC;
}
/*
 * 1.日    期  : 2013年4月14日
  *   修改内容  : 新生成函数
 */
uint32_t hmac_mgmt_rx_addba_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *payload, uint32_t framelen)
{
    mac_device_stru *dev = NULL;
    mac_vap_stru *mac_vap = NULL;
    uint8_t tid;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, payload)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_rx_addba_req::hmac_vap or hmac_user or payload is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (framelen < MAC_ADDBA_REQ_FRAME_BODY_LEN) {
        oam_warning_log1(0, OAM_SF_BA, "{framelen[%d] < MAC_ADDBA_REQ_FRAME_BODY_LEN.}", framelen);
        return OAL_FAIL;
    }
    mac_vap = &(hmac_vap->st_vap_base_info);

    /* 11n以上能力才可接收ampdu */
    if ((!(mac_vap->en_protocol >= WLAN_HT_MODE)) ||
        (!(hmac_user->st_user_base_info.en_protocol_mode >= WLAN_HT_MODE))) {
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_req::ampdu not supprot or not open,vap protocol mode:%d,user protocol mode:%d.}",
            mac_vap->en_protocol, hmac_user->st_user_base_info.en_protocol_mode);
        return OAL_SUCC;
    }
    /* 获取device结构 */
    dev = mac_res_get_dev(mac_vap->uc_device_id);
    if (dev == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_addba_req::receive failed, device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /******************************************************************/
    /*       ADDBA Request Frame - Frame Body                         */
    /* --------------------------------------------------------------- */
    /* | Category | Action | Dialog | Parameters | Timeout | SSN     | */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1      | 2          | 2       | 2       | */
    /* --------------------------------------------------------------- */
    /*                                                                 */
    /******************************************************************/
    tid = (payload[BYTE_OFFSET_3] & 0x3C) >> 2; /* bit2 */
    if (tid >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_rx_addba_req::addba req receive failed, tid %d overflow.}", tid);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }
    return hmac_rx_ba_info_init(dev, hmac_vap, hmac_user, tid, payload, framelen);
}

oal_bool_enum_uint8 hmac_is_device_ba_setup(void)
{
    uint8_t uc_vap_id;
    hmac_vap_stru *pst_hmac_vap = NULL;

    for (uc_vap_id = 0; uc_vap_id < mac_chip_get_vap_support_max_num_limit(); uc_vap_id++) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_id);
        if (pst_hmac_vap == NULL) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_is_device_ba_setup pst_mac_vap is null.}");
            continue;
        }
        if ((pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP) &&
            (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_PAUSE)) {
            continue;
        }
        if ((mac_mib_get_TxBASessionNumber(&pst_hmac_vap->st_vap_base_info) != 0) ||
            (mac_mib_get_RxBASessionNumber(&pst_hmac_vap->st_vap_base_info) != 0)) {
            return OAL_TRUE;
        }
    }
    return OAL_FALSE;
}

/*
 * 函 数 名  : hmac_up_rx_bar
 * 1.日    期  : 2015年1月3日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_up_rx_bar(hmac_vap_stru *hmac_vap, mac_rx_ctl_stru *rx_info, oal_netbuf_stru *netbuf)
{
    uint8_t *puc_payload = NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = NULL;
    uint8_t *puc_sa_addr = NULL;
    uint8_t uc_tidno;
    uint16_t user_idx;
    uint16_t us_start_seqnum;
    uint16_t us_frame_len;

    if (wlan_chip_check_need_process_bar() == OAL_FALSE) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_up_rx_bar: not need process bar, return.}");
        return;
    }

    pst_frame_hdr = (mac_ieee80211_frame_stru *)(uintptr_t)mac_get_rx_cb_mac_header_addr(rx_info);
    puc_sa_addr = pst_frame_hdr->auc_address2;
    us_frame_len = rx_info->us_frame_len - rx_info->uc_mac_header_len;

    /*  获取用户指针 */
    if (mac_vap_find_user_by_macaddr(&(hmac_vap->st_vap_base_info), puc_sa_addr, &user_idx) != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_up_rx_bar::invalid user_idx[%d] is null.}", user_idx);
        return;
    }

    /* BAR Control(2) +  BlockAck Starting seq num)(2) */
    if (us_frame_len < 4) {  /* mac header帧头长度小于4，帧格式错误 */
        oam_warning_log2(0, OAM_SF_ANY, "{hmac_up_rx_bar:frame len err. frame len[%d], machdr len[%d].}",
            rx_info->us_frame_len, rx_info->uc_mac_header_len);
        return;
    }

    /* 获取帧头和payload指针 */
    puc_payload = mac_get_rx_payload_addr(rx_info, netbuf);

    /*************************************************************************/
    /*                     BlockAck Request Frame Format                     */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BAR Control|BlockAck Starting    |FCS|  */
    /* |             |        |  |  |           |Sequence number      |   |  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |2          |2                    |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    uc_tidno = (puc_payload[1] & 0xF0) >> 4; /* 取4~7bit位为TID编号类别 */
    if (uc_tidno >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_up_rx_bar::uc_tidno[%d] invalid.}", uc_tidno);
        return;
    }
    us_start_seqnum = mac_get_bar_start_seq_num(puc_payload);

    hmac_reorder_ba_rx_buffer_bar(user_idx, uc_tidno, us_start_seqnum);
}
