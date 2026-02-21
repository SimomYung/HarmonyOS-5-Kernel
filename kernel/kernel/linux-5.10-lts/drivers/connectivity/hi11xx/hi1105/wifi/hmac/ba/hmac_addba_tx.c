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
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ADDBA_TX_C

#define TX_ADDBA_REQ
/*
 * 1.日    期  : 2013年4月9日
  *   修改内容  : 新生成函数
 */
uint16_t hmac_mgmt_encap_addba_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *data, dmac_ba_tx_stru *tx_ba, uint8_t tid)
{
    uint16_t index;
    uint16_t ba_param;
#ifdef _PRE_WLAN_FEATURE_11BE
    uint8_t ie_len;
    hmac_addba_exten_ie_union addba_ie;
#endif
    if (oal_any_null_ptr3(hmac_vap, data, tx_ba)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_encap_addba_req::hmac_vap or data or tx_ba is null.}");
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
    /* Frame Control Field 中只需要设置Type/Subtype值，其他设置为0 */
    mac_hdr_set_frame_control(data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* DA is address of STA requesting association */
    oal_set_mac_addr(data + WLAN_HDR_ADDR1_OFFSET, tx_ba->puc_dst_addr);

    /* SA的值为dot11MACAddress的值 */
    oal_set_mac_addr(data + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(&hmac_vap->st_vap_base_info));

    oal_set_mac_addr(data + WLAN_HDR_ADDR3_OFFSET, hmac_vap->st_vap_base_info.auc_bssid);

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /* 将索引指向frame body起始位置 */
    index = MAC_80211_FRAME_LEN;

    /* 设置Category */
    data[index++] = MAC_ACTION_CATEGORY_BA;

    /* 设置Action */
    data[index++] = MAC_BA_ACTION_ADDBA_REQ;

    /* 设置Dialog Token */
    data[index++] = tx_ba->uc_dialog_token;

    /*
        设置Block Ack Parameter set field
        bit0 - AMSDU Allowed
        bit1 - Immediate or Delayed block ack
        bit2-bit5 - TID
        bit6-bit15 -  Buffer size
    */
    ba_param = tx_ba->en_amsdu_supp;                   /* bit0 */
    ba_param |= (tx_ba->uc_ba_policy << 1);            /* bit1 */
    ba_param |= (tid << 2);                            /* bit2 */
    ba_param |= (uint16_t)(hmac_addba_get_buffer_size(tx_ba->us_baw_size) << 6);   /* bit6 */

    data[index++] = (uint8_t)(ba_param & 0xFF);
    data[index++] = (uint8_t)((ba_param >> 8) & 0xFF); /* bit8 */

    /* 设置BlockAck timeout */
    data[index++] = (uint8_t)(tx_ba->us_ba_timeout & 0xFF);
    data[index++] = (uint8_t)((tx_ba->us_ba_timeout >> 8) & 0xFF); /* bit8 */

    /*
        Block ack starting sequence number字段由硬件设置
        bit0-bit3 fragmentnumber
        bit4-bit15: sequence number
    */
    /* us_buf_seq此处暂不填写，在dmac侧会补充填写 */
    *(uint16_t *)&data[index++] = 0;
    index++;

#ifdef _PRE_WLAN_FEATURE_11BE
    if ((mac_vap_is_work_eht_protocol(&hmac_vap->st_vap_base_info) == OAL_TRUE) &&
        (mac_user_get_eht_capable(&hmac_user->st_user_base_info) == OAL_TRUE)) {
        /* Extended Buffer Size 在11be只支持到0x1 * 1024 */
        addba_ie.exten_ie.extended_buffer_size = hmac_addba_get_extended_buffer_size(tx_ba->us_baw_size);
        if (mac_set_addba_extension_ie(&data[index], &ie_len, addba_ie.value) == OAL_SUCC) {
            index += ie_len;
        }
    }
#endif
    /* 返回的帧长度中不包括FCS */
    return index;
}

static uint32_t hmac_mgmt_tx_ba_check(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_action_mgmt_args_stru *action_args)
{
    mac_vap_stru *mac_vap = NULL;
    mac_device_stru *mac_dev = NULL;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, action_args)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_tx_ba_check::hmac_vap or hmac_user or action_args is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_vap = &(hmac_vap->st_vap_base_info);
    /*  确定vap处于工作状态 */
    if (mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_tx_ba_check:: vap has been down/del, vap_state[%d].}", mac_vap->en_vap_state);
        return OAL_FAIL;
    }
    /*  END */
    /* 获取device结构 */
    mac_dev = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_dev == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_ba_check::mac_dev null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}

static void hmac_mgmt_set_tx_ba_para(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, mac_vap_stru *mac_vap,
    mac_action_mgmt_args_stru *action_args, dmac_ba_tx_stru *tx_ba)
{
    uint8_t tidno = (uint8_t)(action_args->arg1);

    hmac_vap->uc_ba_dialog_token++;
    tx_ba->uc_dialog_token = hmac_vap->uc_ba_dialog_token;
    /* 非802.11ax/11be协议下ADDBA窗口大小最大不能超过64 */
    if (hmac_user->st_user_base_info.en_avail_protocol_mode == WLAN_EHT_MODE) {
        tx_ba->us_baw_size = oal_min((uint16_t)(action_args->arg2), WLAN_AMPDU_TX_EHT_MAX_NUM);
    } else if (hmac_user->st_user_base_info.en_avail_protocol_mode == WLAN_HE_MODE) {
        tx_ba->us_baw_size = oal_min((uint16_t)(action_args->arg2), WLAN_AMPDU_TX_HE_MAX_NUM);
    } else {
        tx_ba->us_baw_size = oal_min((uint16_t)(action_args->arg2), WLAN_AMPDU_TX_MAX_NUM);
    }
    tx_ba->uc_ba_policy = (uint8_t)(action_args->arg3);
    tx_ba->us_ba_timeout = (uint16_t)(action_args->arg4);
    tx_ba->puc_dst_addr = hmac_user->st_user_base_info.auc_user_mac_addr;

    /* 发端对AMPDU+AMSDU的支持 */
    tx_ba->en_amsdu_supp = (oal_bool_enum_uint8)mac_mib_get_AmsduPlusAmpduActive(mac_vap);
#ifdef _PRE_WLAN_FEATURE_PWL
    /* pwl模式不支持amsdu */
    if (mac_vap->pwl_enable == OAL_TRUE) {
        tx_ba->en_amsdu_supp = OAL_FALSE;
    }
#endif

    if (tx_ba->en_amsdu_supp == OAL_FALSE) {
        hmac_user_set_amsdu_not_support(hmac_user, tidno);
    } else {
        hmac_user_set_amsdu_support(hmac_user, tidno);
    }
}

static void hmac_mgmt_tx_set_ctx(uint16_t frame_len, uint8_t tidno, uint16_t assoc_id, dmac_ba_tx_stru st_tx_ba,
    dmac_ctx_action_event_stru *wlan_ctx_action)
{
    wlan_ctx_action->us_frame_len = frame_len;
    wlan_ctx_action->uc_hdr_len = MAC_80211_FRAME_LEN;
    wlan_ctx_action->en_action_category = MAC_ACTION_CATEGORY_BA;
    wlan_ctx_action->uc_action = MAC_BA_ACTION_ADDBA_REQ;
    wlan_ctx_action->us_user_idx = assoc_id;
    wlan_ctx_action->uc_tidno = tidno;
    wlan_ctx_action->uc_dialog_token = st_tx_ba.uc_dialog_token;
    wlan_ctx_action->uc_ba_policy = st_tx_ba.uc_ba_policy;
    wlan_ctx_action->us_baw_size = st_tx_ba.us_baw_size;
    wlan_ctx_action->us_ba_timeout = st_tx_ba.us_ba_timeout;
    wlan_ctx_action->en_amsdu_supp = st_tx_ba.en_amsdu_supp;
    return;
}

static void hmac_mgmt_tx_addba_req_cb_init(oal_netbuf_stru *addba_req, uint16_t frame_len,
    uint8_t frame_type, uint8_t frame_subtype)
{
    mac_tx_ctl_stru *tx_ctl = NULL;

    memset_s(oal_netbuf_cb(addba_req), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(addba_req);
    if (tx_ctl == NULL) {
        return;
    }
    mac_get_cb_mpdu_len(tx_ctl) = frame_len + sizeof(dmac_ctx_action_event_stru);
    mac_get_cb_frame_type(tx_ctl) = frame_type;
    mac_get_cb_frame_subtype(tx_ctl) = frame_subtype;
}

/*
 * 1.日    期  : 2013年4月14日
  *   修改内容  : 新生成函数
 */
uint32_t hmac_mgmt_tx_addba_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, mac_action_mgmt_args_stru *args)
{
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info);
    oal_netbuf_stru *addba_req = NULL;
    dmac_ba_tx_stru st_tx_ba = { 0 };
    uint8_t tidno = (uint8_t)(args->arg1);
    uint16_t frame_len;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    uint32_t ret;
    hmac_ba_stru *hmac_tid = hmac_get_ba_info(hmac_user, tidno);
    ret = hmac_mgmt_tx_ba_check(hmac_vap, hmac_user, args);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* 申请ADDBA_REQ管理帧内存 */
    addba_req = hmac_mgmt_ba_session_alloc(mac_vap->uc_vap_id);
    if (addba_req == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 对tid对应的txBA会话状态加锁 */
    oal_spin_lock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));

    hmac_mgmt_set_tx_ba_para(hmac_vap, hmac_user, mac_vap, args, &st_tx_ba);

    /* 调用封装管理帧接口 */
    frame_len = hmac_mgmt_encap_addba_req(hmac_vap, hmac_user, oal_netbuf_data(addba_req), &st_tx_ba, tidno);
    memset_s((uint8_t *)&st_wlan_ctx_action, sizeof(st_wlan_ctx_action), 0, sizeof(st_wlan_ctx_action));
    /* 赋值要传入Dmac的信息 */
    hmac_mgmt_tx_set_ctx(frame_len, tidno, hmac_user->st_user_base_info.us_assoc_id, st_tx_ba, &st_wlan_ctx_action);

    if (memcpy_s((uint8_t *)(oal_netbuf_data(addba_req) + frame_len), (WLAN_MEM_NETBUF_SIZE2 - frame_len),
        (uint8_t *)&st_wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oal_netbuf_free(addba_req);
        oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
        return OAL_FAIL;
    }
    oal_netbuf_put(addba_req, (frame_len + sizeof(dmac_ctx_action_event_stru)));

    hmac_mgmt_tx_addba_req_cb_init(addba_req, frame_len, WLAN_CB_FRAME_TYPE_ACTION, WLAN_ACTION_BA_ADDBA_REQ);
    ret = hmac_tx_action_event(mac_vap, addba_req, frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(addba_req);
        oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
        return ret;
    }
    /* 更新对应的TID信息 */
    hmac_tid->st_ba_tx_info.en_ba_status = DMAC_BA_INPROGRESS;
    hmac_tid->st_ba_tx_info.uc_dialog_token = st_tx_ba.uc_dialog_token;
    hmac_tid->st_ba_tx_info.uc_ba_policy = st_tx_ba.uc_ba_policy;
    mac_mib_incr_TxBASessionNumber(mac_vap);

    /* 同步更新定时器中的user id和vap id */
    hmac_tid->st_ba_tx_info.st_alarm_data.us_mac_user_idx = hmac_user->st_user_base_info.us_assoc_id;
    hmac_tid->st_ba_tx_info.st_alarm_data.uc_vap_id = hmac_vap->st_vap_base_info.uc_vap_id;
    /* 启动ADDBA超时计时器 */
    frw_timer_create_timer_m(&hmac_tid->st_ba_tx_info.st_addba_timer,
        hmac_mgmt_tx_addba_timeout, WLAN_ADDBA_TIMEOUT, &hmac_tid->st_ba_tx_info.st_alarm_data,
        OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_MLOG, "{hmac_mgmt_tx_addba_req::baw_size[%d], en_amsdu[%d]}",
        st_wlan_ctx_action.us_baw_size, st_wlan_ctx_action.en_amsdu_supp);
    return OAL_SUCC;
}

#define RX_ADDBA_RSP
#ifdef _PRE_WLAN_FEATURE_11BE
/*
 * 函 数 名   : hmac_process_addba_rsp_exten_ie
 * 功能描述   : 处理addba rsp中的extension ie,记录ADDBA Additional Parameter Set
 */
void hmac_process_addba_rsp_exten_ie(hmac_ba_rx_rsp_stru *ba_rx_rsp, uint8_t *payload, uint16_t payload_len)
{
    uint8_t *ie_ptr = NULL;
    mac_ies_info ies_info = { 0 };

    if (oal_unlikely(oal_any_null_ptr2(ba_rx_rsp, payload))) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_process_addba_rsp_exten_ie:null ptr.}");
        return;
    }
    ies_info.frame_ies = payload + MAC_ADDBA_RSP_FRAME_BODY_LEN;
    if (payload_len < MAC_ADDBA_RSP_FRAME_BODY_LEN) { // 长度合法性判断，避免回绕
        oam_error_log1(0, OAM_SF_BA, "{hmac_process_addba_rsp_exten_ie:payload_len invalid [%d].}", payload_len);
        return;
    }
    ies_info.frame_len = payload_len - MAC_ADDBA_RSP_FRAME_BODY_LEN;

    if (payload_len >= MAC_ADDBA_RSP_FRAME_BODY_LEN + MAC_IE_HDR_LEN + MAC_ADDBA_EXTENSION_IE_LEN) {
        ie_ptr = mac_find_ie(MAC_EID_ADDBA_EXTENSION, &ies_info);
        if ((ie_ptr != NULL) && (ie_ptr[1] == MAC_ADDBA_EXTENSION_IE_LEN)) {
            ba_rx_rsp->extension_ie_received = OAL_TRUE;
            ba_rx_rsp->addba_para_set = ie_ptr[MAC_IE_HDR_LEN];
            oam_warning_log2(0, OAM_SF_BA, "{hmac_process_addba_rsp_exten_ie:tid:%d received extension ie,\
                parameter set:%x.}", ba_rx_rsp->tidno, ie_ptr[MAC_IE_HDR_LEN]);
            if ((ba_rx_rsp->addba_para_set & ADDBA_EXTENDED_BUFFER_SIZE_MASK) != 0) {
                ba_rx_rsp->baw_size = ba_rx_rsp->baw_size +
                    (ba_rx_rsp->addba_para_set >> ADDBA_EXTENDED_BUFFER_SIZE_OFFSET) * ADDBA_EXTENDED_BUFFER_SIZE_UNIT;
            }
        }
    }
}
#endif

OAL_STATIC void hmac_mgmt_get_addba_rsp_info(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    hmac_ba_rx_rsp_stru *ba_rx_rsp, uint8_t *payload, uint32_t payload_len)
{
    /******************************************************************/
    /*       ADDBA Response Frame - Frame Body                        */
    /* --------------------------------------------------------------- */
    /* | Category | Action | Dialog | Status  | Parameters | Timeout | */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1      | 2       | 2          | 2       | */
    /* --------------------------------------------------------------- */
    /*                                                                */
    /******************************************************************/
    ba_rx_rsp->tidno = (payload[BYTE_OFFSET_5] & 0x3C) >> 2; /* bit2 */
    ba_rx_rsp->dialog_token = payload[BYTE_OFFSET_2];
    ba_rx_rsp->status_code = payload[BYTE_OFFSET_3];
    ba_rx_rsp->ba_policy = ((payload[BYTE_OFFSET_5] & 0x02) >> 1); /* bit1 */
    ba_rx_rsp->amsdu_supp = payload[BYTE_OFFSET_5] & BIT0;
    ba_rx_rsp->ba_timeout = payload[BYTE_OFFSET_7] | (payload[BYTE_OFFSET_8] << NUM_8_BITS);
    ba_rx_rsp->baw_size =
        (uint16_t)(((payload[BYTE_OFFSET_5] & 0xC0) >> NUM_6_BITS) | (payload[BYTE_OFFSET_6] << NUM_2_BITS));

#ifdef _PRE_WLAN_FEATURE_11BE
    if ((mac_vap_is_work_eht_protocol(&hmac_vap->st_vap_base_info) == OAL_TRUE) &&
        (mac_user_get_eht_capable(&hmac_user->st_user_base_info) == OAL_TRUE)) {
        hmac_process_addba_rsp_exten_ie(ba_rx_rsp, (uint8_t*)payload, payload_len);
    }
#endif
}

OAL_STATIC void hmac_mgmt_rx_addba_rsp_result_handle(
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, hmac_ba_rx_rsp_stru *ba_rx_rsp)
{
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info); // 有效性在调用处保证
    hmac_ba_stru *tid_info = hmac_get_ba_info(hmac_user, ba_rx_rsp->tidno);
    uint8_t mib_amsdu_active;

    /* 先抛事件，再处理host BA句柄，防止异步发送ADDBA REQ */
    if (ba_rx_rsp->status_code == MAC_SUCCESSFUL_STATUSCODE) {
        /* 设置hmac模块对应的BA句柄的信息 */
        tid_info->st_ba_tx_info.en_ba_status = DMAC_BA_COMPLETE;
        tid_info->st_ba_tx_info.uc_addba_attemps = 0;
        mib_amsdu_active = mac_mib_get_AmsduPlusAmpduActive(mac_vap);
#ifdef _PRE_WLAN_FEATURE_PWL
        /* pwl模式不支持amsdu */
        if (mac_vap->pwl_enable == OAL_TRUE) {
            mib_amsdu_active = OAL_FALSE;
        }
#endif

        if (ba_rx_rsp->amsdu_supp && mib_amsdu_active) {
            hmac_user_set_amsdu_support(hmac_user, ba_rx_rsp->tidno);
        } else {
            hmac_user_set_amsdu_not_support(hmac_user, ba_rx_rsp->tidno);
        }
    } else { /* BA被拒绝 */
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_rsp_result_handle::addba rsp  status err[%d].tid[%d],DEL BA.}",
            ba_rx_rsp->status_code, ba_rx_rsp->tidno);

        hmac_tx_ba_session_decr(hmac_vap, ba_rx_rsp->tidno);

        /* 先抛事件删除dmac旧BA句柄后，再重置HMAC模块信息，确保删除dmac ba事件在下一次ADDBA REQ事件之前到达dmac */
        tid_info->st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
    }
}

OAL_STATIC uint32_t hmac_mgmt_rx_addba_rsp_status_check(hmac_vap_stru *hmac_vap,
    hmac_user_stru *hmac_user, hmac_ba_rx_rsp_stru *ba_rx_rsp, oal_bool_enum_uint8 *is_need_delba)
{
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info); // 有效性在调用处保证
    hmac_ba_stru *tid_info = hmac_get_ba_info(hmac_user, ba_rx_rsp->tidno);
    /*
     * BA状态成功，但token、policy不匹配，需要删除聚合
     *  使用超时机制去删除聚合。降低主动删除聚合后，立马收到匹配的ADDBA RSP，
     * 造成上述两个操作向DMAC抛事件，在DMAC中又不能保序执行带来的异常报错
     */
    if ((tid_info->st_ba_tx_info.en_ba_status == DMAC_BA_INPROGRESS) &&
        (ba_rx_rsp->status_code == MAC_SUCCESSFUL_STATUSCODE)) {
        if ((ba_rx_rsp->dialog_token != tid_info->st_ba_tx_info.uc_dialog_token) ||
            (ba_rx_rsp->ba_policy != tid_info->st_ba_tx_info.uc_ba_policy)) {
            oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_BA,
                "{hmac_mgmt_rx_addba_rsp::addba rsp tid[%d],status SUCC,but token/policy wr}", ba_rx_rsp->tidno);
            oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_BA,
                "{hmac_mgmt_rx_addba_rsp::rsp policy[%d],req policy[%d], rsp dialog[%d], req dialog[%d]}",
                ba_rx_rsp->ba_policy, tid_info->st_ba_tx_info.uc_ba_policy,
                ba_rx_rsp->dialog_token, tid_info->st_ba_tx_info.uc_dialog_token);
            return OAL_SUCC_GO_ON;
        }
    }

    /* 停止计时器 */
    if (tid_info->st_ba_tx_info.st_addba_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&tid_info->st_ba_tx_info.st_addba_timer);
    }

    if (tid_info->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
        *is_need_delba = TRUE;
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_rsp::addba rsp is received when ba status is DMAC_BA_INIT.tid[%d]}", ba_rx_rsp->tidno);
        return OAL_SUCC_GO_ON;
    }

    if (tid_info->st_ba_tx_info.en_ba_status == DMAC_BA_COMPLETE) {
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_rsp::addba rsp is received when ba status is DMAC_BA_COMPLETE. tid[%d], status[%d]}",
            ba_rx_rsp->tidno, tid_info->st_ba_tx_info.en_ba_status);
        return OAL_SUCC_GO_ON;
    }
    return OAL_SUCC;
}

OAL_STATIC void hmac_mgmt_fill_addba_rsp_event_data(
    hmac_user_stru *hmac_user, frw_event_stru *event, hmac_ba_rx_rsp_stru *ba_rx_rsp)
{
    dmac_ctx_action_event_stru *event_data = (dmac_ctx_action_event_stru *)(event->auc_event_data);
    event_data->en_action_category = MAC_ACTION_CATEGORY_BA;
    event_data->uc_action = MAC_BA_ACTION_ADDBA_RSP;
    event_data->us_user_idx = hmac_user->st_user_base_info.us_assoc_id;
    event_data->uc_status = ba_rx_rsp->status_code;
    event_data->uc_tidno = ba_rx_rsp->tidno;
    event_data->uc_dialog_token = ba_rx_rsp->dialog_token;
    event_data->uc_ba_policy = ba_rx_rsp->ba_policy;
    event_data->us_ba_timeout = ba_rx_rsp->ba_timeout;
    event_data->en_amsdu_supp = ba_rx_rsp->amsdu_supp;
    event_data->us_baw_size = ((ba_rx_rsp->baw_size == 0) ||
        (ba_rx_rsp->baw_size > g_wlan_spec_cfg->max_tx_ampdu_num)) ?
        g_wlan_spec_cfg->max_tx_ampdu_num : ba_rx_rsp->baw_size;
}

OAL_STATIC uint32_t hmac_mgmt_sync_addba_rsp(
    mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, hmac_ba_rx_rsp_stru *ba_rx_rsp)
{
    frw_event_mem_stru *event_mem = NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *event = NULL;
    /* 抛事件到DMAC处理 */
    event_mem = frw_event_alloc_m(sizeof(dmac_ctx_action_event_stru));
    if (event_mem == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_addba_rsp::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 获得事件指针 */
    event = frw_get_event_stru(event_mem);
    /* 填写事件头 */
    frw_event_hdr_init(&(event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC,
        sizeof(dmac_ctx_action_event_stru), FRW_EVENT_PIPELINE_STAGE_1,
        mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);
    /* 获取帧体信息，由于DMAC的同步，填写事件payload */
    hmac_mgmt_fill_addba_rsp_event_data(hmac_user, event, ba_rx_rsp);
    /* 分发 */
    frw_event_dispatch_event(event_mem);
    /* 释放事件内存 */
    frw_event_free_m(event_mem);
    return OAL_SUCC;
}

uint32_t hmac_mgmt_rx_addba_rsp(
    hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *payload, uint32_t frame_body_len)
{
    mac_vap_stru *mac_vap = NULL;
    hmac_ba_rx_rsp_stru ba_rx_rsp;
    oal_bool_enum_uint8 is_need_delba = FALSE;
    hmac_ba_stru *hmac_tid = NULL;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, payload)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_rx_addba_rsp::hmac_vap or hmac_user or payload is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    } else if (frame_body_len < MAC_ADDBA_RSP_FRAME_BODY_LEN) {
        oam_warning_log1(0, OAM_SF_BA, "{frame_body_len[%d] < MAC_ADDBA_REQ_FRAME_BODY_LEN.}", frame_body_len);
        return OAL_FAIL;
    }
    mac_vap = &(hmac_vap->st_vap_base_info);
    memset_s((uint8_t *)&ba_rx_rsp, sizeof(hmac_ba_rx_rsp_stru), 0, sizeof(hmac_ba_rx_rsp_stru));
    hmac_mgmt_get_addba_rsp_info(hmac_vap, hmac_user, &ba_rx_rsp, payload, frame_body_len);
    oam_warning_log4(mac_vap->uc_vap_id, OAM_SF_MLOG, "{hmac_mgmt_get_addba_rsp_info::user_id[%d],status[%d], \
        baw_size[%d], en_amsdu[%d]}", hmac_user->st_user_base_info.us_assoc_id, ba_rx_rsp.status_code,
        ba_rx_rsp.baw_size, ba_rx_rsp.amsdu_supp);
    /* 协议支持tid为0~15, 02只支持tid0~7 */
    if (ba_rx_rsp.tidno >= WLAN_TID_MAX_NUM) {
        /* 对于tid > max 的resp直接忽略 */
        oam_warning_log3(mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_addba_rsp::rsp tid[%d]} token[%d] state[%d]",
                         ba_rx_rsp.tidno, ba_rx_rsp.dialog_token, ba_rx_rsp.status_code);
        return OAL_SUCC;
    }

    hmac_tid = hmac_get_ba_info(hmac_user, ba_rx_rsp.tidno);
    /* 对tid对应的tx BA会话状态加锁 */
    oal_spin_lock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
    if (hmac_mgmt_rx_addba_rsp_status_check(hmac_vap, hmac_user, &ba_rx_rsp, &is_need_delba) == OAL_SUCC_GO_ON) {
         /* 对tid对应的tx BA会话状态解锁 */
        oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
        if (is_need_delba == TRUE) {
            hmac_tx_ba_del(hmac_vap, hmac_user, ba_rx_rsp.tidno);
        }
        return OAL_SUCC;
    }
    if (hmac_mgmt_sync_addba_rsp(mac_vap, hmac_user, &ba_rx_rsp) != OAL_SUCC) {
        oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
        return OAL_FAIL;
    }
    hmac_mgmt_rx_addba_rsp_result_handle(hmac_vap, hmac_user, &ba_rx_rsp);
    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(hmac_tid->st_ba_tx_info.st_ba_status_lock));
    return OAL_SUCC;
}

#define TX_ADDBA_TIMEOUT
/*
 * 1.日    期  : 2013年4月22日
  *   修改内容  : 新生成函数
 */
uint32_t hmac_mgmt_tx_addba_timeout(void *arg)
{
    hmac_vap_stru *vap = NULL; /* vap指针 */
    uint8_t *usr_dst_mac = NULL; /* 保存用户目的地址的指针 */
    hmac_user_stru *hmac_user = NULL;
    mac_action_mgmt_args_stru action_args;
    hmac_ba_alarm_stru *alarm_data = NULL;
    uint32_t ret;

    if (arg == NULL) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_timeout::p_arg null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    alarm_data = (hmac_ba_alarm_stru *)arg;
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(alarm_data->us_mac_user_idx);
    if (hmac_user == NULL) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_timeout::pst_hmac_user[%d] null.}",
                         alarm_data->us_mac_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    usr_dst_mac = hmac_user->st_user_base_info.auc_user_mac_addr;
    vap = (hmac_vap_stru *)mac_res_get_hmac_vap(alarm_data->uc_vap_id);
    if (oal_unlikely(vap == NULL)) {
        oam_error_log0(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_timeout::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 生成DELBA帧 */
    action_args.uc_category = MAC_ACTION_CATEGORY_BA;
    action_args.uc_action = MAC_BA_ACTION_DELBA;
    action_args.arg1 = alarm_data->uc_tid; /* 该数据帧对应的TID号 */
    action_args.arg2 = MAC_ORIGINATOR_DELBA; /* DELBA中，触发删除BA会话的发起端 */
    action_args.arg3 = MAC_QSTA_TIMEOUT; /* DELBA中代表删除reason */
    action_args.puc_arg5 = usr_dst_mac; /* DELBA中代表目的地址 */

    oam_warning_log1(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_BA,
        "{hmac_mgmt_tx_addba_timeout::TID[%d] add timeout.}", alarm_data->uc_tid);
    ret = hmac_mgmt_tx_delba(vap, hmac_user, &action_args);

    return ret;
}
