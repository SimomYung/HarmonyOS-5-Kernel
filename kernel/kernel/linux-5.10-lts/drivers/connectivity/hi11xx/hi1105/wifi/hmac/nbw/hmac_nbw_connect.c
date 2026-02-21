/*
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 * Description: nbw connect mgnt
 * Author: wifi
 * Create: 2025-01-14
 */

#include "oam_ext_if.h"
#include "oal_types.h"
#include "mac_resource.h"
#include "hmac_sme_sta.h"
#include "hmac_nbw_frame.h"
#include "hmac_chba_sync.h"
#include "hmac_nbw_mgmt.h"
#include "hmac_nbw_connect.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_CONNECT_C

#ifdef _PRE_WLAN_FEATURE_NBW

#define NBW_CONN_SLAVE_WAIT_SLOT_INDICATE_TIMER 4000  /* NBW建链，slave等待slaveAp发送时隙指示的超时时间(4000ms) */


void hmac_nbw_connect_fail_handler(hmac_vap_stru *hmac_vap)
{
    hmac_chba_user_stru *hmac_chba_user = NULL;
    chba_connect_status_stru *connect_info = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_chba_vap_stru *chba_vap_info = hmac_vap->hmac_chba_vap_info;
    hmac_nbw_current_connect_info_stru *curr_conn_info = hmac_nbw_get_curr_conn_info();

    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, curr_conn_info->peer_addr,
        WLAN_MAC_ADDR_LEN);
    if (chba_vap_info == NULL || hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_connect_fail_handler::chba vap or hmac user is null.");
        return;
    }

    hmac_chba_user = &(hmac_user->chba_user);
    connect_info = &(hmac_chba_user->connect_info);

    /* 上报建链失败给上层 */
    hmac_report_connect_failed_result(hmac_vap, connect_info->status_code,
        hmac_user->st_user_base_info.auc_user_mac_addr);

    /* 删除用户 */
    hmac_user_del(&hmac_vap->st_vap_base_info, hmac_user);

    /* 清空当前连接信息 */
    hmac_nbw_clear_curr_conn_info();
}


uint32_t hmac_nbw_slave_wait_slot_indicate_timeout(void *arg)
{
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;

    hmac_user = (hmac_user_stru *)arg;
    if (hmac_user == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (hmac_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* slave等待slaveap时隙指示超时，上报建链失败 */
    oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CHBA,
        "NBW: hmac_nbw_wait_slot_indicate_timeout::slave wait slaveap slot indicate timeout");
    hmac_nbw_connect_fail_handler(hmac_vap);
    return OAL_SUCC;
}


void hmac_nbw_slave_config_sync_slaveap(hmac_vap_stru *hmac_vap)
{
    uint8_t *master_addr = NULL;
    chba_params_config_stru chba_params_sync = { 0 };
    hmac_nbw_current_connect_info_stru *curr_conn_info = hmac_nbw_get_curr_conn_info();

    /* 配置同步slaveap */
    chba_params_sync.chba_state = CHBA_NON_SYNC;
    chba_params_sync.chba_role = (curr_conn_info->connect_role == CHBA_CONN_RESPONDER) ?
        MAC_NBW_SLAVEAP_ROLE : MAC_NBW_SLAVE_ROLE;
    oal_set_mac_addr(chba_params_sync.domain_bssid, hmac_nbw_get_master_macaddr());
    chba_params_sync.info_bitmap = CHBA_STATE | CHBA_ROLE | CHBA_BSSID;
    hmac_chba_params_sync(&hmac_vap->st_vap_base_info, &chba_params_sync);

    master_addr = hmac_nbw_get_master_macaddr();
    oam_warning_log3(0, OAM_SF_ANY,
        "{hmac_nbw_slave_config_sync_slaveap:master mac[%02x:XX:XX:XX:%02x:%02x]}",
        master_addr[MAC_ADDR_0], master_addr[MAC_ADDR_4], master_addr[MAC_ADDR_5]);
}


void hmac_nbw_slave_sync_set(hmac_vap_stru *hmac_vap)
{
    chba_params_config_stru chba_params_sync = { 0 };
    chba_params_sync.chba_state = CHBA_WORK;

    chba_params_sync.info_bitmap = CHBA_STATE;
    hmac_chba_params_sync(&hmac_vap->st_vap_base_info, &chba_params_sync);

    oam_warning_log0(0, OAM_SF_CHBA, "NBW: {hmac_nbw_slave_sync_config:slave sync}");
}


static void hmac_nbw_ready_to_connect(hmac_vap_stru *hmac_vap, hmac_nbw_current_connect_info_stru *nbw_curr_conn_info)
{
    if (oal_any_null_ptr2(hmac_vap, nbw_curr_conn_info)) {
        return;
    }

    oal_cfg80211_connect_ready_report(hmac_vap->pst_net_device, nbw_curr_conn_info->peer_addr,
        nbw_curr_conn_info->op_id, nbw_curr_conn_info->status_code, GFP_ATOMIC);
}


void hmac_nbw_slaveap_wait_assoc_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user)
{
    hmac_nbw_current_connect_info_stru *nbw_curr_conn_info = hmac_nbw_get_curr_conn_info();
    int32_t waiting_time = nbw_curr_conn_info->connect_timeout * HMAC_S_TO_MS;

    oam_warning_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_slaveap_wait_assoc_req::slaveap wait assoc req.");

    /* 此处参考chba, 通知ready to connect, nbw正式方案待设计对齐上下层交互, 可能不需要 */
    hmac_nbw_ready_to_connect(hmac_vap, nbw_curr_conn_info);

    /* 启动assoc waiting定时器 */
    hmac_user->chba_user.connect_info.assoc_waiting_time = waiting_time;
    frw_timer_create_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer),
        hmac_nbw_slave_wait_slot_indicate_timeout, hmac_user->chba_user.connect_info.assoc_waiting_time,
        hmac_user, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
}


void hmac_nbw_save_curr_conn_info(hmac_chba_conn_param_stru *conn_param, uint8_t connect_role)
{
    hmac_nbw_current_connect_info_stru *nbw_curr_conn_info = hmac_nbw_get_curr_conn_info();
    nbw_curr_conn_info->connect_status = MAC_NBW_CONNECT_ASSOC;
    nbw_curr_conn_info->connect_role = (connect_role == CHBA_CONN_RESPONDER) ?
        MAC_NBW_SLAVEAP_ROLE : MAC_NBW_SLAVE_ROLE;
    oal_set_mac_addr(nbw_curr_conn_info->peer_addr, conn_param->peer_addr);
    nbw_curr_conn_info->op_id = conn_param->op_id;
    nbw_curr_conn_info->conn_flag = conn_param->conn_flag;
    nbw_curr_conn_info->status_code = conn_param->status_code;
    nbw_curr_conn_info->connect_timeout = conn_param->connect_timeout;

    /* Demo阶段无建链协商，默认建链发起方是Slave，响应方设备在信道侦听和master选择后，确认为master和slaveap */
    if (connect_role == CHBA_CONN_INITIATOR) {
        hmac_nbw_set_role(MAC_NBW_SLAVE_ROLE);
    }

    oam_warning_log1(0, OAM_SF_CHBA, "NBW: {hmac_nbw_save_curr_conn_info, connect role: %d}", connect_role);
}


void hmac_nbw_clear_curr_conn_info(void)
{
    hmac_nbw_current_connect_info_stru *nbw_curr_conn_info = hmac_nbw_get_curr_conn_info();

    /* 删除定时器 */
    if (nbw_curr_conn_info->assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(nbw_curr_conn_info->assoc_waiting_timer));
    }
    memset_s(nbw_curr_conn_info, sizeof(hmac_nbw_current_connect_info_stru), 0,
        sizeof(hmac_nbw_current_connect_info_stru));
}


uint32_t hmac_nbw_beacom_master_handler(hmac_vap_stru *hmac_vap)
{
    hmac_chba_vap_stru *chba_vap_info = hmac_get_chba_vap();
    oam_warning_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_beacom_master_handler: slaveap set master role");

    /* 设置本端角色为master */
    hmac_nbw_set_role(MAC_NBW_MASTER_ROLE);
    hmac_chba_set_sync_state(CHBA_WORK);

    /* 设置本设备bssid为master bssid */
    oal_set_mac_addr(hmac_nbw_get_master_macaddr(), hmac_vap->st_vap_base_info.auc_bssid);

    oam_warning_log3(0, OAM_SF_ANY,
        "{hmac_nbw_beacom_master_handler:master bssid[%02x:XX:XX:XX:%02x:%02x]}",
        hmac_vap->st_vap_base_info.auc_bssid[MAC_ADDR_0], hmac_vap->st_vap_base_info.auc_bssid[MAC_ADDR_4],
        hmac_vap->st_vap_base_info.auc_bssid[MAC_ADDR_5]);

    /* 封装beacon vendor信息 */
    hmac_nbw_encap_master_beacon(chba_vap_info, chba_vap_info->beacon_buf, &chba_vap_info->beacon_buf_len,
        MAC_NBW_BEACON_TYPE_BASIC);

    return hmac_nbw_sync_beacon_info(hmac_vap, chba_vap_info);
}


uint32_t hmac_nbw_master_election_finish_proc(void *arg)
{
    /* 如果两个beacon周期内未收到master beacon，则自己当选master，申请分配时隙，并发送时隙申请指示 */
    mac_vap_stru *mac_vap = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_user_stru *hmac_user = NULL;
    hmac_nbw_current_connect_info_stru *conn_info = hmac_nbw_get_curr_conn_info();
    oam_warning_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_master_election_finish_proc start");

    mac_vap = (mac_vap_stru *)arg;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_master_election_finish_proc::hmac vap is null.");
        return OAL_FAIL;
    }

    /* 向驱动配置同步beacon并更新master信息 */
    if (hmac_nbw_beacom_master_handler(hmac_vap) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_master_election_finish_proc::send nbw param to dmac err.");
        hmac_nbw_connect_fail_handler(hmac_vap);
        return OAL_FAIL;
    }

    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, conn_info->peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_master_election_finish_proc::hmac user is null.");
        return OAL_FAIL;
    }

    /* 如果已经关联，则向master申请时隙 */
    if (hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
        hmac_nbw_apply_slot(hmac_vap);
    }

    return OAL_SUCC;
}


uint32_t hmac_nbw_apply_slot(hmac_vap_stru *hmac_vap)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    uint8_t nbw_role = hmac_nbw_get_role();
    hmac_nbw_current_connect_info_stru *conn_info = hmac_nbw_get_curr_conn_info();

    if (nbw_role != MAC_NBW_SLAVEAP_ROLE && nbw_role != MAC_NBW_MASTER_ROLE &&
        nbw_role != MAC_NBW_SLAVEAP_SLAVE_MIX_ROLE) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_apply_slot: device role wrong");
        return OAL_FAIL;
    }

    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, conn_info->peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_apply_slot::hmac user is null.");
        return OAL_FAIL;
    }

    oam_warning_log1(0, OAM_SF_NBW, "NBW: hmac_nbw_apply_slot: device role is [%d]", nbw_role);
    if (nbw_role == MAC_NBW_SLAVEAP_ROLE) {
        /* 如果未同步master，等同步后申请时隙 */
        if (hmac_chba_get_sync_state() != CHBA_WORK) {
            return OAL_FAIL;
        }
        uslot_bitmap_info.timeslot_interval = 1;
        /* 向master发送申请时隙请求action帧 */
        hmac_nbw_send_timeslot_frame(hmac_nbw_get_master_macaddr(), conn_info->peer_addr, WLAN_MAC_ADDR_LEN,
            uslot_bitmap_info, MAC_NBW_TIMESLOT_NEG_ACTION, MAC_NBW_TIMESLOT_NEG_REQUEST_ACTION);

        /* 设置时隙请求等待超时器 */
        frw_timer_create_timer_m(&(conn_info->assoc_waiting_timer),
            hmac_nbw_waiting_master_slot_time_response_proc, WAITTING_TWO_DURATION_MS,
            hmac_vap, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_vap->st_vap_base_info.core_id);
    } else if (nbw_role == MAC_NBW_MASTER_ROLE) {
        /* 本地申请时隙分配 */
        uslot_bitmap_info = hmac_nbw_calculate_uslotbitmap(1);
        /* 如果申请不到时隙，结束流程, 理论上如果自身选举为master，不会有申请不到时隙的情况 */
        if (uslot_bitmap_info.uslot_bitmap == INVALID_USLOT_BITMAP) {
            oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_apply_slot: apply slot faile by device is master");
            hmac_nbw_connect_fail_handler(hmac_vap);
            return OAL_FAIL;
        }

        /* 存储申请到时隙资源到全域资源表中 */
        hmac_update_master_island_info(uslot_bitmap_info, conn_info->peer_addr, hmac_nbw_get_master_macaddr());

        /* 申请到的时隙配置到map表中 */
        hmac_nbw_set_available_uslot_bitmap(uslot_bitmap_info.uslot_bitmap);

        /* 给slaveap发送时隙指示 */
        hmac_nbw_send_timeslot_frame(conn_info->peer_addr, conn_info->peer_addr, uslot_bitmap_info,
            MAC_NBW_TIMESLOT_NEG_ACTION, MAC_NBW_TIMESLOT_NEG_INDICATE_ACTION);

        hmac_nbw_sync_user_bitmap(hmac_vap, hmac_user, mac_nbw_uslot_bitmap_to_bitmap(uslot_bitmap_info.uslot_bitmap));
        hmac_nbw_update_uslot_bitmap_for_slot_alloced(uslot_bitmap_info.uslot_bitmap);
        hmac_nbw_sync_vap_bitmap_info(&(hmac_vap->st_vap_base_info));

        /* 时隙申请完成后，清除当前建链信息 */
        hmac_nbw_clear_curr_conn_info();
    } else {
        /* 群岛时隙申请后续补充 */
    }
    return OAL_SUCC;
}
#endif