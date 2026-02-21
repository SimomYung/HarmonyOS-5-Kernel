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
#include "hmac_config.h"
#include "hmac_tx_data.h"
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DELBA_RX_C

OAL_STATIC uint32_t hmac_mgmt_sync_delba_event(
    mac_vap_stru *mac_vap, mac_user_stru *mac_user, uint8_t tid, uint8_t initiator)
{
    frw_event_mem_stru *event_mem = NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *hmac_to_dmac_crx_sync = NULL;
    dmac_ctx_action_event_stru *wlan_crx_action = NULL;
    /* 抛事件到DMAC处理 */
    event_mem = frw_event_alloc_m(sizeof(dmac_ctx_action_event_stru));
    if (event_mem == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_delba::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 获得事件指针 */
    hmac_to_dmac_crx_sync = frw_get_event_stru(event_mem);
    /* 填写事件头 */
    frw_event_hdr_init(&(hmac_to_dmac_crx_sync->st_event_hdr),
        FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC, sizeof(dmac_ctx_action_event_stru),
        FRW_EVENT_PIPELINE_STAGE_1, mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);

    /* 填写事件payload */
    wlan_crx_action = (dmac_ctx_action_event_stru *)(hmac_to_dmac_crx_sync->auc_event_data);
    wlan_crx_action->en_action_category = MAC_ACTION_CATEGORY_BA;
    wlan_crx_action->uc_action = MAC_BA_ACTION_DELBA;
    wlan_crx_action->us_user_idx = mac_user->us_assoc_id;
    wlan_crx_action->uc_tidno = tid;
    wlan_crx_action->uc_initiator = initiator;
    /* 分发 */
    frw_event_dispatch_event(event_mem);
    /* 释放事件内存 */
    frw_event_free_m(event_mem);
    return OAL_SUCC;
}

/*
 * 1.日    期  : 2013年4月14日
 *   修改内容  : 新生成函数
 */
uint32_t hmac_mgmt_rx_delba(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *payload, uint32_t payload_len)
{
    hmac_ba_stru *tid = NULL;
    uint8_t tid_id, initiator;
    uint16_t reason;

    if (oal_unlikely(oal_any_null_ptr3(hmac_vap, hmac_user, payload)) || (payload_len < MAC_DELBA_FRAME_BODY_LEN)) {
        oam_error_log1(0, OAM_SF_BA, "{hmac_mgmt_rx_delba:params is null or payload_len[%d] too short.}", payload_len);
        return OAL_FAIL;
    }

    /************************************************/
    /*       DELBA Response Frame - Frame Body      */
    /* -------------------------------------------- */
    /* | Category | Action | Parameters | Reason  | */
    /* -------------------------------------------- */
    /* | 1        | 1      | 2          | 2       | */
    /* -------------------------------------------- */
    /*                                              */
    /************************************************/
    tid_id = (payload[BYTE_OFFSET_3] & 0xF0) >> 4; /* bit4 */
    initiator = (payload[BYTE_OFFSET_3] & 0x08) >> 3; /* bit3 */
    reason = (payload[BYTE_OFFSET_4] & 0xFF) | ((payload[BYTE_OFFSET_5] << 8) & 0xFF00); /* bit8 */

    /* tid_id保护，避免数组越界 */
    if (tid_id >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_mgmt_rx_delba::delba receive failed, tid_id %d overflow.}", tid_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_delba::receive delba from \
        peer sta, tid_id[%d], initiator[%d], reason[%d].}", tid_id, initiator, reason);

    tid = hmac_get_ba_info(hmac_user, tid_id);
    /* 对tid对应的tx BA会话状态加锁 */
    oal_spin_lock_bh(&(tid->st_ba_tx_info.st_ba_status_lock));
    /* 重置BA发送会话 */
    if (initiator == MAC_RECIPIENT_DELBA) {
        if (tid->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oal_spin_unlock_bh(&(tid->st_ba_tx_info.st_ba_status_lock));
            return OAL_SUCC;
        }
        /* 还原设置AMPDU下AMSDU的支持情况 */
        hmac_user_set_amsdu_support(hmac_user, tid_id);
        hmac_tx_ba_session_decr(hmac_vap, tid_id);
    } else { /* 重置BA接收会话 */
        hmac_rx_ba_reset_pre_handle(hmac_vap, hmac_user, tid_id);
        hmac_ba_reset_rx_handle(hmac_user->st_user_base_info.us_assoc_id, tid_id, OAL_FALSE);
        tid->en_ba_status = DMAC_BA_INIT;
    }
    if (hmac_mgmt_sync_delba_event(&hmac_vap->st_vap_base_info, &hmac_user->st_user_base_info,
        tid_id, initiator) != OAL_SUCC) {
        oal_spin_unlock_bh(&(tid->st_ba_tx_info.st_ba_status_lock));
        return OAL_FAIL;
    }
    /* DELBA事件先处理再改状态,防止addba req先处理 */
    if (initiator == MAC_RECIPIENT_DELBA) {
        tid->st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
    }
    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(tid->st_ba_tx_info.st_ba_status_lock));
    return OAL_SUCC;
}
