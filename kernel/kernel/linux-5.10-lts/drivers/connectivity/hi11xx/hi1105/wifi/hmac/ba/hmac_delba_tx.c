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
#include "hmac_dfx.h"
#include "hmac_config.h"
#include "hmac_tx_data.h"
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DELBA_TX_C

static void hmac_del_ba_event_set_action_args(mac_action_mgmt_args_stru *action_args,
    uint8_t tid, hmac_user_stru *hmac_user)
{
    action_args->uc_category = MAC_ACTION_CATEGORY_BA;
    action_args->uc_action = MAC_BA_ACTION_DELBA;
    action_args->arg1 = tid; /* 该数据帧对应的TID号 */
    /* ADDBA_REQ中，buffer_size的默认大小 */
    action_args->arg2 = MAC_ORIGINATOR_DELBA;
    action_args->arg3 = MAC_UNSPEC_REASON;                                   /* BA会话的确认策略 */
    action_args->puc_arg5 = hmac_user->st_user_base_info.auc_user_mac_addr; /* ba会话对应的user */
}

/*
 * 函 数 名  : hmac_del_ba_event
 * 1.日    期  : 2013年11月21日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_del_ba_event(frw_event_mem_stru *pst_event_mem)
{
    uint8_t uc_tid;
    frw_event_stru *pst_event = NULL;
    hmac_user_stru *pst_hmac_user = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    mac_action_mgmt_args_stru st_action_args; /* 用于填写ACTION帧的参数 */
    uint32_t ret;
    dmac_to_hmac_ctx_event_stru *pst_del_ba_event;
    hmac_ba_stru *ba_info = NULL;
    pst_event = frw_get_event_stru(pst_event_mem);

    pst_del_ba_event = (dmac_to_hmac_ctx_event_stru *)pst_event->auc_event_data;

    pst_hmac_user = mac_res_get_hmac_user(pst_del_ba_event->us_user_index);
    if (pst_hmac_user == NULL) {
        /* dmac抛事件到hmac侧删除ba，此时host侧可能已经删除用户了，此时属于正常，直接返回即可 */
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_del_ba_event::pst_hmac_user[%d] null.}",
                         pst_del_ba_event->us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_del_ba_event->uc_vap_id);
    if (pst_hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_del_ba_event::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_set_cur_protocol_mode(&pst_hmac_user->st_user_base_info, pst_del_ba_event->uc_cur_protocol);
    ret = hmac_config_user_info_syn(&(pst_hmac_vap->st_vap_base_info), &(pst_hmac_user->st_user_base_info));
    if (ret != OAL_SUCC) {
        return ret;
    }

    for (uc_tid = 0; uc_tid < WLAN_TID_MAX_NUM; uc_tid++) {
        ba_info = hmac_get_ba_info(pst_hmac_user, uc_tid);
        if (ba_info->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                          "{hmac_del_ba_event::the tx hdl is not exist.}");
            continue;
        }

        hmac_del_ba_event_set_action_args(&st_action_args, uc_tid, pst_hmac_user);

        /* 删除BA会话 */
        ret = hmac_mgmt_tx_action(pst_hmac_vap, pst_hmac_user, &st_action_args);
        if (ret != OAL_SUCC) {
            oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                             "{hmac_del_ba_event::hmac_mgmt_tx_action failed.}");
            continue;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC void hmac_set_delba_netbuf_cb(oal_netbuf_stru *delba_netbuf, uint16_t frame_len)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(delba_netbuf);
    if (tx_ctl == NULL) {
        return;
    }

    memset_s(tx_ctl, oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    mac_get_cb_mpdu_len(tx_ctl) = frame_len + sizeof(dmac_ctx_action_event_stru);
    mac_get_cb_frame_type(tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    mac_get_cb_frame_subtype(tx_ctl) = WLAN_ACTION_BA_DELBA;
    mac_get_cb_frame_header_length(tx_ctl) = MAC_80211_FRAME_LEN;
}

OAL_STATIC uint32_t hmac_set_delba_ctx_action(
    hmac_user_stru *hmac_user, mac_action_mgmt_args_stru *action_args,
    oal_netbuf_stru *delba_netbuf, uint16_t frame_len)
{
    dmac_ctx_action_event_stru wlan_ctx_action = {0};

    wlan_ctx_action.us_frame_len = frame_len;
    wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_BA;
    wlan_ctx_action.uc_action = MAC_BA_ACTION_DELBA;
    wlan_ctx_action.us_user_idx = hmac_user->st_user_base_info.us_assoc_id;
    wlan_ctx_action.uc_tidno = (uint8_t)(action_args->arg1);
    wlan_ctx_action.uc_initiator = (mac_delba_initiator_enum_uint8)(action_args->arg2);
    if (memcpy_s((uint8_t *)(oal_netbuf_data(delba_netbuf) + frame_len), (WLAN_MEM_NETBUF_SIZE2 - frame_len),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_BA, "hmac_set_delba_ctx_action::memcpy fail!");
        /* 释放管理帧内存到netbuf内存池 */
        /* 对tid对应的tx BA会话状态解锁 */
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
/*
 * 1.日    期  : 2013年4月9日
  *   修改内容  : 新生成函数
 */
uint16_t hmac_mgmt_encap_delba(hmac_vap_stru *pst_vap, uint8_t *puc_data, uint8_t *puc_addr, uint8_t uc_tid,
    mac_delba_initiator_enum_uint8 en_initiator, uint8_t reason)
{
    uint16_t us_index;

    if (oal_any_null_ptr3(pst_vap, puc_data, puc_addr)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_encap_delba::vap or data or addr is null.}");
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
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* duration */
    puc_data[BYTE_OFFSET_2] = 0;
    puc_data[BYTE_OFFSET_3] = 0;

    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + WLAN_HDR_ADDR1_OFFSET, puc_addr);

    /* SA is the dot11MACAddress */
    oal_set_mac_addr(puc_data + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(&pst_vap->st_vap_base_info));

    oal_set_mac_addr(puc_data + WLAN_HDR_ADDR3_OFFSET, pst_vap->st_vap_base_info.auc_bssid);

    /* seq control */
    puc_data[BYTE_OFFSET_22] = 0;
    puc_data[BYTE_OFFSET_23] = 0;

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*             DELBA Response Frame - Frame Body                         */
    /*        -------------------------------------------------              */
    /*        | Category | Action |  Parameters | Reason code |              */
    /*        -------------------------------------------------              */
    /*        | 1        | 1      |       2     | 2           |              */
    /*        -------------------------------------------------              */
    /*                          Parameters                                   */
    /*                  -------------------------------                      */
    /*                  | Reserved | Initiator |  TID  |                     */
    /*                  -------------------------------                      */
    /*             bit  |    11    |    1      |  4    |                     */
    /*                  --------------------------------                     */
    /*************************************************************************/
    /* Initialize index and the frame data pointer */
    us_index = MAC_80211_FRAME_LEN;

    /* Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_BA;

    /* Action */
    puc_data[us_index++] = MAC_BA_ACTION_DELBA;

    /* DELBA parameter set */
    /* B0 - B10 -reserved */
    /* B11 - initiator */
    /* B12-B15 - TID */
    puc_data[us_index++] = 0;
    puc_data[us_index] = (uint8_t)(en_initiator << 3); /* bit3 */
    puc_data[us_index++] |= (uint8_t)(uc_tid << 4);    /* bit4 */

    /* Reason field */
    /* Reason can be either of END_BA, QSTA_LEAVING, UNKNOWN_BA */
    puc_data[us_index++] = reason;
    puc_data[us_index++] = 0;

    /* 返回的帧长度中不包括FCS */
    return us_index;
}

OAL_STATIC uint32_t hmac_mgmt_send_delba_netbuf(
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, mac_action_mgmt_args_stru *action_args)
{
    oal_netbuf_stru *delba_netbuf = NULL;
    uint16_t frame_len;
    uint8_t tidno = (uint8_t)(action_args->arg1);

    /* 申请DEL_BA管理帧内存 */
    delba_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (delba_netbuf == NULL) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_send_delba_netbuf::delba_netbuf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    oal_mem_netbuf_trace(delba_netbuf, OAL_TRUE);
    oal_netbuf_prev(delba_netbuf) = NULL;
    oal_netbuf_next(delba_netbuf) = NULL;
    /* 调用封装管理帧接口 */
    frame_len = hmac_mgmt_encap_delba(hmac_vap, (uint8_t *)oal_netbuf_header(delba_netbuf), action_args->puc_arg5,
        tidno, (mac_delba_initiator_enum_uint8)(action_args->arg2), (uint8_t)action_args->arg3);
    if (hmac_set_delba_ctx_action(hmac_user, action_args, delba_netbuf, frame_len) != OAL_SUCC) {
        oal_netbuf_free(delba_netbuf);
        return OAL_FAIL;
    }
    oal_netbuf_put(delba_netbuf, (frame_len + sizeof(dmac_ctx_action_event_stru)));
    hmac_chr_set_del_ba_info(tidno, (uint16_t)action_args->arg3);
    hmac_set_delba_netbuf_cb(delba_netbuf, frame_len);
    if (hmac_tx_action_event(&hmac_vap->st_vap_base_info, delba_netbuf, frame_len) != OAL_SUCC) {
        oal_netbuf_free(delba_netbuf);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 1.日    期  : 2013年4月14日
  *   修改内容  : 新生成函数
 */
uint32_t hmac_mgmt_tx_delba(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, mac_action_mgmt_args_stru *action_args)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_ba_stru *hmac_tid = NULL;
    mac_delba_initiator_enum_uint8 initiator;
    uint8_t tidno;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, action_args)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_tx_delba::hmac_vap or hmac_user or action_args is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    mac_vap = &(hmac_vap->st_vap_base_info);
    /*  需确定vap处于工作状态 */
    if (mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_delba:vap_state[%d].}", mac_vap->en_vap_state);
        return OAL_FAIL;
    }

    initiator = (mac_delba_initiator_enum_uint8)(action_args->arg2);
    tidno = (uint8_t)(action_args->arg1);

    hmac_tid = hmac_get_ba_info(hmac_user, tidno);

    /* 对tid对应的tx BA会话状态加锁 */
    oal_spin_lock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
    /*  接收方向判断接收方向status，发送方向判断发送方向status */
    /*
     * ,接收方向delba发送失败，或者对端没有收到，要再次发送delba时需要放宽判断,
     * 这里主要保证rx delba始终可以发送即可，无其他影响
     */
    if (initiator == MAC_ORIGINATOR_DELBA) {
        if (hmac_tid->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
            return OAL_SUCC;
        }
    }
    if (hmac_mgmt_send_delba_netbuf(hmac_vap, hmac_user, action_args) != OAL_SUCC) {
        oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
        return OAL_FAIL;
    }
    if (initiator == MAC_RECIPIENT_DELBA) {
        /* 更新对应的TID信息 */
        hmac_rx_ba_reset_pre_handle(hmac_vap, hmac_user, tidno);
        hmac_ba_reset_rx_handle(hmac_user->st_user_base_info.us_assoc_id, tidno, OAL_FALSE);
        hmac_tid->en_ba_status = DMAC_BA_INIT;
    } else {
        /* 更新对应的TID信息 */
        hmac_tid->st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
        hmac_tx_ba_session_decr(hmac_vap, tidno);
        /* 还原设置AMPDU下AMSDU的支持情况 */
        hmac_user_set_amsdu_support(hmac_user, tidno);
    }
    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
    return OAL_SUCC;
}
