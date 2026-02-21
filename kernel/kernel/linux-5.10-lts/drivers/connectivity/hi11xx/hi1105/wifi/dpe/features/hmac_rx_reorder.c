/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : AMPDU聚合、BA处理接口定义源文件
 * 作    者 :
 * 创建日期 : 2014年11月25日
 */

/* 1 头文件包含 */
#include "hmac_rx_reorder.h"
#include "wlan_spec.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_auto_adjust_freq.h"
#endif
#include "dpe_wlan_rx_reorder.h"
#include "dpe_wlan_vap.h"
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_REORDER_C

OAL_STATIC uint8_t hmac_ba_timeout_param_is_valid(hmac_ba_alarm_stru *alarm_data, dpe_wlan_vap_stru *dpe_vap,
    dpe_wlan_user_stru *dpe_user, uint8_t rx_tid)
{
    if (rx_tid >= WLAN_TID_MAX_NUM) {
        oam_error_log1(0, OAM_SF_BA, "{hmac_ba_timeout_fn::tid %d overflow.}", rx_tid);
        return OAL_FALSE;
    }

    if (dpe_vap == NULL) {
        oam_error_log1(0, 0, "{hmac_ba_timeout_fn::vap[%d] NULL}", alarm_data->uc_vap_id);
        return OAL_FALSE;
    }

    if (dpe_user == NULL) {
        oam_error_log1(0, 0, "{hmac_ba_timeout_fn::user[%d] NULL}", alarm_data->us_mac_user_idx);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

OAL_STATIC void hmac_ba_send_frames_add_list(hmac_rx_buf_stru *pst_rx_buf, oal_netbuf_stru *pst_netbuf,
    oal_netbuf_head_stru *pst_netbuf_header)
{
    uint8_t uc_loop_index;

    for (uc_loop_index = 0; uc_loop_index < pst_rx_buf->uc_num_buf; uc_loop_index++) {
        pst_netbuf = oal_netbuf_delist(&pst_rx_buf->st_netbuf_head);
        oal_mem_netbuf_trace(pst_netbuf, OAL_FALSE);
        if (pst_netbuf != NULL) {
            oal_netbuf_list_tail_nolock(pst_netbuf_header, pst_netbuf);
        }
    }
}

/*
 * 函 数 名  : hmac_ba_send_frames_with_gap
 * 功能描述  : 冲刷重排序缓冲区至给定的sequence number位置
 * 1.日    期  : 2014年11月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_ba_send_frames_with_gap(hmac_ba_rx_stru *pst_ba_rx_hdl, oal_netbuf_head_stru *pst_netbuf_header,
    uint16_t us_last_seqnum)
{
    uint8_t uc_num_frms = 0;
    uint16_t us_seq_num;
    hmac_rx_buf_stru *pst_rx_buf = NULL;
    oal_netbuf_stru *pst_netbuf = NULL;

    us_seq_num = pst_ba_rx_hdl->us_baw_start;

    oam_info_log1(0, OAM_SF_BA, "{hmac_ba_send_frames_with_gap::to seq[%d].}", us_last_seqnum);

    while (us_seq_num != us_last_seqnum) {
        pst_rx_buf = hmac_remove_frame_from_reorder_q(pst_ba_rx_hdl, us_seq_num);
        if (pst_rx_buf != NULL) {
            pst_netbuf = oal_netbuf_peek(&pst_rx_buf->st_netbuf_head);
            if (oal_unlikely(pst_netbuf == NULL)) {
                oam_warning_log1(0, OAM_SF_BA,
                                 "{hmac_ba_send_frames_with_gap::gap[%d].\r\n}",
                                 us_seq_num);

                us_seq_num = mac_ba_seqno_add(us_seq_num, 1);
                pst_rx_buf->uc_num_buf = 0;

                continue;
            }
            hmac_ba_send_frames_add_list(pst_rx_buf, pst_netbuf, pst_netbuf_header);
            pst_rx_buf->uc_num_buf = 0;
            uc_num_frms++;
        }

        us_seq_num = mac_ba_seqno_add(us_seq_num, 1);
    }

    oam_warning_log4(0, OAM_SF_BA,
        "{hmac_ba_send_frames_with_gap::old_baw_start[%d], new_baw_start[%d], uc_num_frms[%d], uc_mpdu_cnt=%d.}",
        pst_ba_rx_hdl->us_baw_start, us_last_seqnum, uc_num_frms, pst_ba_rx_hdl->uc_mpdu_cnt);

    return uc_num_frms;
}

/*
 * 函 数 名  : hmac_ba_send_frames_in_order
 * 功能描述  : All MSDUs with sequence number starting from the
               start of the BA-Rx window are processed in order and
               are added to the list which will be passed up to hmac.
               Processing is stopped when the first missing MSDU is encountered.
 * 1.日    期  : 2013年4月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint16_t hmac_ba_send_frames_in_order(hmac_ba_rx_stru *pst_ba_rx_hdl, oal_netbuf_head_stru *pst_netbuf_header)
{
    uint16_t us_seq_num;
    hmac_rx_buf_stru *pst_rx_buf = NULL;
    uint8_t uc_loop_index;
    oal_netbuf_stru *pst_netbuf = NULL;

    us_seq_num = pst_ba_rx_hdl->us_baw_start;

    while ((pst_rx_buf = hmac_remove_frame_from_reorder_q(pst_ba_rx_hdl, us_seq_num)) != NULL) {
        us_seq_num = hmac_ba_seqno_add(us_seq_num, 1);
        pst_netbuf = oal_netbuf_peek(&pst_rx_buf->st_netbuf_head);
        if (pst_netbuf == NULL) {
            oam_warning_log1(0, OAM_SF_BA,
                             "{hmac_ba_send_frames_in_order::[%d] slot error.}",
                             us_seq_num);
            pst_rx_buf->uc_num_buf = 0;
            continue;
        }

        for (uc_loop_index = 0; uc_loop_index < pst_rx_buf->uc_num_buf; uc_loop_index++) {
            pst_netbuf = oal_netbuf_delist(&pst_rx_buf->st_netbuf_head);
            oal_mem_netbuf_trace(pst_netbuf, OAL_FALSE);
            if (pst_netbuf != NULL) {
                oal_netbuf_list_tail_nolock(pst_netbuf_header, pst_netbuf);
            }
        }

        pst_rx_buf->uc_num_buf = 0;
    }

    return us_seq_num;
}

/*
 * 函 数 名  : hmac_ba_flush_reorder_q
 * 功能描述  : 冲刷重排序队列
 * 1.日    期  : 2013年4月15日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_ba_flush_reorder_q(hmac_ba_rx_stru *pst_rx_ba)
{
    hmac_rx_buf_stru *pst_rx_buf = NULL;
    uint16_t us_index;

    for (us_index = 0; us_index < WLAN_AMPDU_RX_HE_BUFFER_SIZE; us_index++) {
        pst_rx_buf = hmac_get_ba_rx_dhl(pst_rx_ba, us_index);
        if (pst_rx_buf->in_use == OAL_TRUE) {
            hmac_rx_free_netbuf_list(&pst_rx_buf->st_netbuf_head, pst_rx_buf->uc_num_buf);
            pst_rx_buf->in_use = OAL_FALSE;
            pst_rx_buf->uc_num_buf = 0;
            pst_rx_ba->uc_mpdu_cnt--;
        }
    }

    if (pst_rx_ba->uc_mpdu_cnt != 0) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_ba_flush_reorder_q:: %d mpdu cnt left.}", pst_rx_ba->uc_mpdu_cnt);
    }
}

/*
 * 函 数 名  : hmac_reorder_ba_rx_buffer_bar
 * 功能描述  : This function reorders the Reciver buffer and sends frames to the higher
 *             layer on reception of a Block-Ack-Request frame. It also updates the
 *             receiver buffer window.
 * 1.日    期  : 2014年11月29日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_reorder_ba_rx_buffer_bar(uint16_t user_idx, uint8_t tidno, uint16_t us_start_seq_num)
{
    oal_netbuf_head_stru st_netbuf_head;
    uint8_t uc_seqnum_pos;
    hmac_ba_rx_stru *pst_rx_ba = NULL;
    hmac_tid_ba_stru *hmac_tid = hmac_get_tid_ba_info(user_idx, tidno);
    if (hmac_tid == NULL) {
        return;
    }
    pst_rx_ba = hmac_tid->pst_ba_rx_info;
    if (pst_rx_ba == NULL) {
        oam_warning_log0(0, OAM_SF_BA, "{hmac_reorder_ba_rx_buffer_bar::rcv a bar, but ba not set up.}");
        return;
    }

    /* 针对 BAR 的SSN和窗口的start_num相等时，不需要移窗 */
    if (pst_rx_ba->us_baw_start == us_start_seq_num) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_reorder_ba_rx_buffer_bar::seq is equal to start num.}");
        return;
    }

    oal_netbuf_list_head_init(&st_netbuf_head);

    uc_seqnum_pos = hmac_ba_seqno_bound_chk(pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
    /* 针对BAR的的SSN在窗口内才移窗 */
    if (uc_seqnum_pos == DMAC_BA_BETWEEN_SEQLO_SEQHI) {
        hmac_ba_send_frames_with_gap(pst_rx_ba, &st_netbuf_head, us_start_seq_num);
        pst_rx_ba->us_baw_start = us_start_seq_num;
        pst_rx_ba->us_baw_start = hmac_ba_send_frames_in_order(pst_rx_ba, &st_netbuf_head);
        pst_rx_ba->us_baw_end = hmac_ba_seqno_add(pst_rx_ba->us_baw_start, (pst_rx_ba->us_baw_size - 1));

        oam_warning_log3(0, OAM_SF_BA,
            "{hmac_reorder_ba_rx_buffer_bar::receive a bar, us_baw_start=%d us_baw_end=%d. us_seq_num=%d.}",
            pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
        hmac_rx_lan_frame(&st_netbuf_head);
    } else if (uc_seqnum_pos == DMAC_BA_GREATER_THAN_SEQHI) {
        /* 异常 */
        oam_warning_log3(0, OAM_SF_BA,
            "{hmac_reorder_ba_rx_buffer_bar::recv a bar and ssn out of winsize,baw_start=%d baw_end=%d seq_num=%d}",
            pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
    } else {
        oam_warning_log3(0, OAM_SF_BA,
            "{hmac_reorder_ba_rx_buffer_bar::recv a bar ssn less than baw_start,baw_start=%d end=%d seq_num=%d}",
            pst_rx_ba->us_baw_start, pst_rx_ba->us_baw_end, us_start_seq_num);
    }
}

/*
 * 功能描述  : ba会话超时处理
 * 1.日    期  : 2013年4月12日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_ba_timeout_fn(void *p_arg)
{
    hmac_ba_rx_stru *pst_rx_ba = NULL;
    dpe_wlan_user_stru *dpe_user = NULL;
    dpe_wlan_vap_stru *dpe_vap = NULL;
    hmac_ba_alarm_stru *alarm_data = (hmac_ba_alarm_stru *)p_arg;
    mac_delba_initiator_enum_uint8 en_direction = alarm_data->en_direction;
    uint8_t rx_tid = alarm_data->uc_tid;
    uint32_t timeout;
    hmac_tid_ba_stru *hmac_tid = NULL;

    dpe_user = dpe_wlan_user_get(alarm_data->us_mac_user_idx);
    dpe_vap = dpe_wlan_vap_get(alarm_data->uc_vap_id);
    if (hmac_ba_timeout_param_is_valid(alarm_data, dpe_vap, dpe_user, rx_tid) != OAL_TRUE) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_tid = hmac_get_tid_ba_info(dpe_user->us_assoc_id, rx_tid);
    if (oal_any_null_ptr1(hmac_tid)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (en_direction == MAC_RECIPIENT_DELBA) {
        pst_rx_ba = (hmac_ba_rx_stru *)hmac_tid->pst_ba_rx_info;
        if (pst_rx_ba == NULL) {
            oam_error_log0(0, OAM_SF_BA, "{hmac_ba_timeout_fn::pst_rx_ba is null.}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* 接收业务量较少时只能靠超时定时器冲刷重排序队列,为改善游戏帧延时,需要将超时时间设小 */
        if (hmac_wifi_rx_is_busy() == OAL_FALSE) {
            timeout = dpe_vap->us_rx_timeout_min[WLAN_WME_TID_TO_AC(rx_tid)];
        } else {
            timeout = dpe_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(rx_tid)];
        }

        if (pst_rx_ba->uc_mpdu_cnt > 0) {
            if (g_wlan_spec_cfg->rx_support_hw_reorder == OAL_TRUE) {
                hmac_ba_send_ring_reorder_timeout(pst_rx_ba, dpe_vap, alarm_data, &timeout);
            } else {
                hmac_ba_send_reorder_timeout(pst_rx_ba, dpe_vap, alarm_data, &timeout);
            }
        }

        /* 若重排序队列刷新后,依然有缓存帧则需要重启定时器;
           若重排序队列无帧则为了节省功耗不启动定时器,在有帧入队时重启 */
        if (pst_rx_ba->uc_mpdu_cnt > 0) {
            oal_spin_lock(&(hmac_tid->st_ba_timer_lock));
            /* 此处不需要判断定时器是否已经启动,如果未启动则启动定时器;
               如果此定时器已经启动 */
            frw_timer_create_timer_m(&(hmac_tid->st_ba_timer),
                hmac_ba_timeout_fn, timeout, alarm_data,
                OAL_FALSE, OAM_MODULE_ID_HMAC, dpe_vap->core_id);
            oal_spin_unlock(&(hmac_tid->st_ba_timer_lock));
        }
    } else {
        /* tx ba不删除 */
        frw_timer_create_timer_m(&(hmac_tid->st_ba_timer),
            hmac_ba_timeout_fn, dpe_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(rx_tid)],
            alarm_data, OAL_FALSE, OAM_MODULE_ID_HMAC, dpe_vap->core_id);
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_ba_reset_rx_handle
 * 功能描述  : 重置rx ba结构体
 * 1.日    期  : 2014年12月5日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_ba_reset_rx_handle(uint16_t user_idx, uint8_t uc_tid, oal_bool_enum_uint8 en_is_aging)
{
    hmac_ba_rx_stru *pst_ba_rx_info = NULL;
    hmac_tid_ba_stru *hmac_tid = NULL;

    if (uc_tid >= WLAN_TID_MAX_NUM) {
        oam_error_log1(0, OAM_SF_BA, "{hmac_ba_reset_rx_handle::tid %d overflow.}", uc_tid);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }
    hmac_tid = hmac_get_tid_ba_info(user_idx, uc_tid);
    if (hmac_tid == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_ba_rx_info = hmac_tid->pst_ba_rx_info;
    if (oal_unlikely((pst_ba_rx_info == NULL) || (pst_ba_rx_info->en_is_ba != OAL_TRUE))) {
        oam_warning_log0(0, OAM_SF_BA, "{hmac_ba_reset_rx_handle::rx ba not set yet.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* Step1: disable the flag of ba session */
    pst_ba_rx_info->en_is_ba = OAL_FALSE;

    /* Step2: flush reorder q */
    hmac_ba_flush_reorder_q(pst_ba_rx_info);
    /* step 3~4 goto hmac_rx_ba_reset_pre_handle */
    oal_spin_lock(&(hmac_tid->st_ba_timer_lock));
    /* Step5: Del Timer */
    if (hmac_tid->st_ba_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(hmac_tid->st_ba_timer));
    }
    oal_spin_unlock(&(hmac_tid->st_ba_timer_lock));

    /* Step6: Free rx handle */
    oal_mem_free_m(pst_ba_rx_info, OAL_TRUE);

    hmac_tid->pst_ba_rx_info = NULL;

    return OAL_SUCC;
}
