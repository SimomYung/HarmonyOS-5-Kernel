/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : 1103\1105 AMPDU聚合、BA处理接口定义源文件
 * 作    者 :
 * 创建日期 : 2024年11月12日
 */

#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
/* 1 头文件包含 */
#include "wlan_spec.h"
#include "mac_vap.h"
#include "hmac_rx_reorder.h"
#include "hmac_main.h"
#include "hmac_rx.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_user.h"
#include "hmac_auto_adjust_freq.h"
#include "wlan_chip_i.h"
#include "mac_mib.h"
#include "hmac_config.h"
#include "dpe_wlan_vap.h"
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_REORDER_SW_C

/*
 * 函 数 名  : hmac_ba_check_rx_aggr
 * 功能描述  : 检查是否能做ba重排序处理
 * 1.日    期  : 2013年11月28日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE uint32_t hmac_ba_check_rx_aggr(mac_vap_stru *pst_vap,
                                                     mac_ieee80211_frame_stru *pst_frame_hdr)
{
    /* 该vap是否是ht */
    if (mac_mib_get_HighThroughputOptionImplemented(pst_vap) == OAL_FALSE) {
        oam_info_log0(pst_vap->uc_vap_id, OAM_SF_BA, "{hmac_ba_check_rx_aggr::ht not supported by this vap.}");
        return OAL_FAIL;
    }

    /* 判断该帧是不是qos帧 */
    if (((uint8_t *)pst_frame_hdr)[0] != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) {
        oam_info_log0(pst_vap->uc_vap_id, OAM_SF_BA, "{hmac_ba_check_rx_aggr::not qos data.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_ba_buffer_frame_in_reorder
 * 1.日    期  : 2013年4月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC hmac_rx_buf_stru *hmac_ba_buffer_frame_in_reorder(hmac_ba_rx_stru *pst_ba_rx_hdl, uint16_t us_seq_num,
    mac_rx_ctl_stru *pst_cb_ctrl)
{
    uint16_t us_buf_index;
    hmac_rx_buf_stru *pst_rx_buf;

    us_buf_index = (us_seq_num & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    pst_rx_buf = hmac_get_ba_rx_dhl(pst_ba_rx_hdl, us_buf_index);
    if (pst_rx_buf->in_use == 1) {
        hmac_rx_free_netbuf_list(&pst_rx_buf->st_netbuf_head, pst_rx_buf->uc_num_buf);
        pst_ba_rx_hdl->uc_mpdu_cnt--;
        pst_rx_buf->in_use = 0;
        pst_rx_buf->uc_num_buf = 0;
        oam_info_log1(0, OAM_SF_BA, "{hmac_ba_buffer_frame_in_reorder::slot already used, seq[%d].}", us_seq_num);
    }

    if (pst_cb_ctrl->bit_amsdu_enable == OAL_TRUE) {
        if (pst_cb_ctrl->bit_is_first_buffer == OAL_TRUE) {
            if (oal_netbuf_list_len(&pst_rx_buf->st_netbuf_head) != 0) {
                hmac_rx_free_netbuf_list(&pst_rx_buf->st_netbuf_head,
                                         oal_netbuf_list_len(&pst_rx_buf->st_netbuf_head));
                oam_info_log1(0, OAM_SF_BA,
                    "{hmac_ba_buffer_frame_in_reorder::seq[%d] amsdu first buffer, need clear st_netbuf_head first}",
                    us_seq_num);
            }
            pst_rx_buf->uc_num_buf = 0;
        }

        /* offload下,amsdu帧拆成单帧分别上报 */
        pst_rx_buf->uc_num_buf += pst_cb_ctrl->bit_buff_nums;

        /* 遇到最后一个amsdu buffer 才标记in use 为 1 */
        if (pst_cb_ctrl->bit_is_last_buffer == OAL_TRUE) {
            pst_ba_rx_hdl->uc_mpdu_cnt++;
            pst_rx_buf->in_use = 1;
        } else {
            pst_rx_buf->in_use = 0;
        }
    } else {
        pst_rx_buf->uc_num_buf = pst_cb_ctrl->bit_buff_nums;
        pst_ba_rx_hdl->uc_mpdu_cnt++;
        pst_rx_buf->in_use = 1;
    }
    /* Update the buffered receive packet details */
    pst_rx_buf->us_seq_num = us_seq_num;
    pst_rx_buf->rx_time = (uint32_t)oal_time_get_stamp_ms();

    return pst_rx_buf;
}

/*
 * 函 数 名  : hmac_ba_buffer_rx_frame
 * 功能描述  : 将报文缓存至重排序队列
 * 1.日    期  : 2014年11月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_ba_buffer_rx_frame(hmac_ba_rx_stru *pst_ba_rx_hdl,
                                                   mac_rx_ctl_stru *pst_cb_ctrl,
                                                   oal_netbuf_head_stru *pst_netbuf_header,
                                                   uint16_t us_seq_num)
{
    hmac_rx_buf_stru *pst_rx_netbuf = NULL;
    oal_netbuf_stru *pst_netbuf = NULL;
    uint8_t uc_netbuf_index;

    /* Get the pointer to the buffered packet */
    pst_rx_netbuf = hmac_ba_buffer_frame_in_reorder(pst_ba_rx_hdl, us_seq_num, pst_cb_ctrl);

    /* all buffers of this frame must be deleted from the buf list */
    for (uc_netbuf_index = pst_cb_ctrl->bit_buff_nums; uc_netbuf_index > 0; uc_netbuf_index--) {
        pst_netbuf = oal_netbuf_delist_nolock(pst_netbuf_header);

        oal_mem_netbuf_trace(pst_netbuf, OAL_TRUE);
        if (oal_unlikely(pst_netbuf != NULL)) {
            /* enqueue reorder queue */
            oal_netbuf_add_to_list_tail(pst_netbuf, &pst_rx_netbuf->st_netbuf_head);
        } else {
            oam_error_log0(pst_cb_ctrl->uc_mac_vap_id, OAM_SF_BA, "{hmac_ba_buffer_rx_frame:netbuff error in amsdu.}");
        }
    }

    if (oal_netbuf_list_len(&pst_rx_netbuf->st_netbuf_head) != pst_rx_netbuf->uc_num_buf) {
        oam_warning_log2(pst_cb_ctrl->uc_mac_vap_id, OAM_SF_BA, "{hmac_ba_buffer_rx_frame: list_len=%d numbuf=%d}",
                         oal_netbuf_list_len(&pst_rx_netbuf->st_netbuf_head), pst_rx_netbuf->uc_num_buf);
        pst_rx_netbuf->uc_num_buf = oal_netbuf_list_len(&pst_rx_netbuf->st_netbuf_head);
    }
}

/*
 * 函 数 名  : hmac_ba_reorder_rx_data
 * 功能描述  : 将重排序队列中可以上传的报文加到buf链表的尾部
 * 1.日    期  : 2013年4月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_ba_reorder_rx_data(hmac_ba_rx_stru *pst_ba_rx_hdl,
    oal_netbuf_head_stru *pst_netbuf_header, uint16_t us_seq_num)
{
    uint8_t uc_seqnum_pos;
    uint16_t us_temp_winstart;

    uc_seqnum_pos = hmac_ba_seqno_bound_chk(pst_ba_rx_hdl->us_baw_start, pst_ba_rx_hdl->us_baw_end, us_seq_num);
    if (uc_seqnum_pos == DMAC_BA_BETWEEN_SEQLO_SEQHI) {
        pst_ba_rx_hdl->us_baw_start = hmac_ba_send_frames_in_order(pst_ba_rx_hdl, pst_netbuf_header);
        pst_ba_rx_hdl->us_baw_end = mac_ba_seqno_add(pst_ba_rx_hdl->us_baw_start, (pst_ba_rx_hdl->us_baw_size - 1));
    } else if (uc_seqnum_pos == DMAC_BA_GREATER_THAN_SEQHI) {
        us_temp_winstart = hmac_ba_seqno_sub(us_seq_num, (pst_ba_rx_hdl->us_baw_size - 1));

        hmac_ba_send_frames_with_gap(pst_ba_rx_hdl, pst_netbuf_header, us_temp_winstart);
        pst_ba_rx_hdl->us_baw_start = us_temp_winstart;
        pst_ba_rx_hdl->us_baw_start = hmac_ba_send_frames_in_order(pst_ba_rx_hdl, pst_netbuf_header);
        pst_ba_rx_hdl->us_baw_end = hmac_ba_seqno_add(pst_ba_rx_hdl->us_baw_start, (pst_ba_rx_hdl->us_baw_size - 1));
    } else {
        oam_warning_log3(0, OAM_SF_BA,
            "{hmac_ba_reorder_rx_data::else branch seqno[%d] ws[%d] we[%d].}",
            us_seq_num, pst_ba_rx_hdl->us_baw_start, pst_ba_rx_hdl->us_baw_end);
    }
}

/*
 * 函 数 名  : hmac_reorder_ba_timer_start
 * 功能描述  : 有帧进入重排序队列时,需要启动定时器
 * 1.日    期  : 2018年8月4日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_reorder_ba_timer_start(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user, uint8_t uc_tid)
{
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_ba_rx_stru *pst_ba_rx_stru = NULL;
    mac_device_stru *pst_device = NULL;
    uint16_t us_timeout;
    hmac_tid_ba_stru *hmac_tid = NULL;
    dpe_wlan_vap_stru *dpe_vap = NULL;

    hmac_tid = hmac_get_tid_ba_info(pst_hmac_user->st_user_base_info.us_assoc_id, uc_tid);
    dpe_vap = dpe_wlan_vap_get(pst_hmac_vap->st_vap_base_info.uc_vap_id);
    if (hmac_tid == NULL || dpe_vap == NULL) {
        return;
    }
    /* 如果超时定时器已经被注册则返回 */
    if (hmac_tid->st_ba_timer.en_is_registerd == OAL_TRUE) {
        return;
    }

    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_reorder_ba_timer_start::pst_device[%d] null.}",
                       pst_mac_vap->uc_device_id);
        return;
    }

    /* 业务量较小时,使用小周期的重排序定时器,保证及时上报至协议栈;
       业务量较大时,使用大周期的重排序定时器,保证尽量不丢包 */
    if (hmac_wifi_rx_is_busy() == OAL_FALSE) {
        us_timeout = dpe_vap->us_rx_timeout_min[WLAN_WME_TID_TO_AC(uc_tid)];
    } else {
        us_timeout = dpe_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(uc_tid)];
    }

    pst_ba_rx_stru = hmac_tid->pst_ba_rx_info;

    oal_spin_lock(&(hmac_tid->st_ba_timer_lock));

    frw_timer_create_timer_m(&(hmac_tid->st_ba_timer),
                             hmac_ba_timeout_fn,
                             us_timeout,
                             &(hmac_tid->st_alarm_data),
                             OAL_FALSE,
                             OAM_MODULE_ID_HMAC,
                             pst_device->core_id);

    oal_spin_unlock(&(hmac_tid->st_ba_timer_lock));
}

OAL_STATIC uint32_t hmac_ba_filter_and_reorder_rx_data(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_rx_ctl_stru *rx_ctrl, oal_netbuf_head_stru *netbuf_header, hmac_ba_rx_stru *ba_rx_hdl, uint16_t seq_num,
    oal_bool_enum_uint8 *pen_is_ba_buf, uint8_t tid)
{
    uint16_t baw_start_temp;

    /* 暂时保存BA窗口的序列号，用于鉴别是否有帧上报 */
    baw_start_temp = ba_rx_hdl->us_baw_start;

    /* duplicate frame判断 */
    if (hmac_ba_rx_seqno_lt(seq_num, ba_rx_hdl->us_baw_start) == OAL_TRUE) {
        /* 上次非定时器上报，直接删除duplicate frame帧，否则，直接上报 */
        if (ba_rx_hdl->en_timer_triggered == OAL_FALSE) {
            /* 确实已经收到该帧 */
            /*  新增了bitmap记录收包 防止dup误丢包 */
            if (hmac_ba_isset(ba_rx_hdl, seq_num) == OAL_TRUE) {
                return OAL_FAIL;
            }
        }

        return OAL_SUCC;
    }

    /* 更新tail */
    if (hmac_ba_seqno_lt(ba_rx_hdl->us_baw_tail, seq_num) == OAL_TRUE) {
        ba_rx_hdl->us_baw_tail = seq_num;
    }

    /* 接收到的帧的序列号等于BAW_START，并且缓存队列帧个数为0，则直接上报给HMAC */
    if ((seq_num == ba_rx_hdl->us_baw_start) && (ba_rx_hdl->uc_mpdu_cnt == 0) &&
        /* offload 下amsdu帧由于可能多个buffer组成，一律走重排序 */
        (rx_ctrl->bit_amsdu_enable == OAL_FALSE)) {
        ba_rx_hdl->us_baw_start = mac_ba_seqno_add(ba_rx_hdl->us_baw_start, 1);
        ba_rx_hdl->us_baw_end = mac_ba_seqno_add(ba_rx_hdl->us_baw_end, 1);
    } else {
        /* Buffer the new MSDU */
        *pen_is_ba_buf = OAL_TRUE;

        /* buffer frame to reorder */
        hmac_ba_buffer_rx_frame(ba_rx_hdl, rx_ctrl, netbuf_header, seq_num);

        /* put the reordered netbufs to the end of the list */
        hmac_ba_reorder_rx_data(ba_rx_hdl, netbuf_header, seq_num);

        /* Check for Sync loss and flush the reorder queue when one is detected */
        if ((ba_rx_hdl->us_baw_tail == mac_ba_seqno_sub(ba_rx_hdl->us_baw_start, 1)) &&
            (ba_rx_hdl->uc_mpdu_cnt > 0)) {
            oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                             "{hmac_ba_filter_serv::Sync loss and flush the reorder queue.}");
            hmac_ba_flush_reorder_q(ba_rx_hdl);
        }

        /* 重排序队列刷新后,如果队列中有帧那么启动定时器 */
        if (ba_rx_hdl->uc_mpdu_cnt > 0) {
            hmac_reorder_ba_timer_start(hmac_vap, hmac_user, tid);
        }
    }

    if (baw_start_temp != ba_rx_hdl->us_baw_start) {
        ba_rx_hdl->en_timer_triggered = OAL_FALSE;
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_ba_filter_serv
 * 功能描述  : 过滤ampdu的每一个mpdu 有未确认报文需要入重传队列
 * 1.日    期  : 2014年11月25日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_ba_filter_serv(hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf, oal_netbuf_head_stru *netbuf_header,
    oal_bool_enum_uint8 *pen_is_ba_buf)
{
    hmac_ba_rx_stru *ba_rx_hdl = NULL;
    uint16_t seq_num;
    uint8_t tid;
    mac_ieee80211_frame_stru *frame_hdr = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_tid_ba_stru *hmac_tid = NULL;
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (oal_any_null_ptr3(netbuf_header, rx_ctrl, pen_is_ba_buf)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_filter_serv::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    frame_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(netbuf);
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctrl->uc_mac_vap_id);
    if (oal_unlikely(hmac_vap == NULL) || oal_unlikely(frame_hdr == NULL)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_filter_serv::hmac_vap or frame_hdr null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (hmac_ba_check_rx_aggr(&hmac_vap->st_vap_base_info, frame_hdr) != OAL_SUCC) {
        return OAL_SUCC;
    }

    /* 考虑四地址情况获取报文的tid */
    tid = hmac_mac_get_tid_from_frame_hdr((uint8_t *)frame_hdr);

    hmac_tid = hmac_get_tid_ba_info(hmac_user->st_user_base_info.us_assoc_id, tid);
    if (hmac_tid == NULL) {
        return OAL_FAIL;
    }
    ba_rx_hdl = hmac_tid->pst_ba_rx_info;
    if (ba_rx_hdl == NULL) {
        return OAL_SUCC;
    }
    if (ba_rx_hdl->en_ba_status != DMAC_BA_COMPLETE) {
        oam_warning_log1(rx_ctrl->uc_mac_vap_id, OAM_SF_BA, "{hmac_ba_filter_serv::ba_status = %d.",
            ba_rx_hdl->en_ba_status);
        return OAL_SUCC;
    }

    seq_num = mac_get_seq_num((uint8_t *)frame_hdr);

    /*  兼容接收方向聚合和分片共存的情况 */
    if ((oal_bool_enum_uint8)frame_hdr->st_frame_control.bit_more_frag == OAL_TRUE) {
        oam_warning_log1(rx_ctrl->uc_mac_vap_id, OAM_SF_BA,
            "{hmac_ba_filter_serv::We get a frag_frame[seq_num=%d] When BA_session is set UP!", seq_num);
        return OAL_SUCC;
    }

    return hmac_ba_filter_and_reorder_rx_data(hmac_vap, hmac_user, rx_ctrl, netbuf_header, ba_rx_hdl,
        seq_num, pen_is_ba_buf, tid);
}

/*
 * 函 数 名  : hmac_ba_rx_prepare_bufflist
 * 功能描述  : 从重排序队列中获取skb链
 * 1.日    期  : 2014年5月21日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_ba_rx_prepare_bufflist(dpe_wlan_vap_stru *dpe_vap, hmac_rx_buf_stru *pst_rx_buf,
    oal_netbuf_head_stru *pst_netbuf_head)
{
    oal_netbuf_stru *pst_netbuf;
    uint8_t uc_loop_index;

    pst_netbuf = oal_netbuf_peek(&pst_rx_buf->st_netbuf_head);
    if (pst_netbuf == NULL) {
        oam_warning_log0(dpe_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_ba_rx_prepare_bufflist::pst_netbuf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_loop_index = 0; uc_loop_index < pst_rx_buf->uc_num_buf; uc_loop_index++) {
        pst_netbuf = oal_netbuf_delist(&pst_rx_buf->st_netbuf_head);
        if (pst_netbuf != NULL) {
            oal_netbuf_add_to_list_tail(pst_netbuf, pst_netbuf_head);
        } else {
            oam_warning_log0(dpe_vap->uc_vap_id, OAM_SF_BA,
                             "{hmac_ba_rx_prepare_bufflist::uc_num_buf in reorder list is error.}");
        }
    }

    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_ba_send_reorder_timeout
 * 功能描述  : 上报重排序队列中超时的报文
 * 1.日    期  : 2014年5月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_ba_send_reorder_timeout(hmac_ba_rx_stru *rx_ba, dpe_wlan_vap_stru *dpe_vap,
    hmac_ba_alarm_stru *alarm_data, uint32_t *timeout)
{
    uint32_t time_diff, rx_timeout, ret;
    oal_netbuf_head_stru st_netbuf_head;
    uint16_t us_baw_head, us_baw_end, us_baw_start; /* 保存最初的窗口起始序列号 */
    hmac_rx_buf_stru *pst_rx_buf = NULL;
    uint8_t uc_buff_count = 0;
    uint16_t uc_send_count = 0;

    oal_netbuf_list_head_init(&st_netbuf_head);
    us_baw_head = rx_ba->us_baw_start;
    us_baw_start = rx_ba->us_baw_start;
    us_baw_end = hmac_ba_seqno_add(rx_ba->us_baw_tail, 1);
    rx_timeout = (*timeout);

    oal_spin_lock(&rx_ba->st_ba_lock);

    while (us_baw_head != us_baw_end) {
        pst_rx_buf = hmac_get_frame_from_reorder_q(rx_ba, us_baw_head);
        if (pst_rx_buf == NULL) {
            uc_buff_count++;
            us_baw_head = hmac_ba_seqno_add(us_baw_head, 1);
            continue;
        }

        /* 如果冲排序队列中的帧滞留时间小于定时器超时时间,那么暂时不强制flush */
        time_diff = (uint32_t)oal_time_get_stamp_ms() - pst_rx_buf->rx_time;
        if (time_diff < rx_timeout) {
            *timeout = (rx_timeout - time_diff);
            break;
        }

        rx_ba->uc_mpdu_cnt--;
        pst_rx_buf->in_use = 0;

        ret = hmac_ba_rx_prepare_bufflist(dpe_vap, pst_rx_buf, &st_netbuf_head);
        if (ret != OAL_SUCC) {
            uc_buff_count++;
            us_baw_head = hmac_ba_seqno_add(us_baw_head, 1);
            continue;
        }

        uc_send_count++;
        uc_buff_count++;
        us_baw_head = hmac_ba_seqno_add(us_baw_head, 1);
        rx_ba->us_baw_start = hmac_ba_seqno_add(rx_ba->us_baw_start, uc_buff_count);
        rx_ba->us_baw_end = hmac_ba_seqno_add(rx_ba->us_baw_start, (rx_ba->us_baw_size - 1));

        uc_buff_count = 0;
    }

    oal_spin_unlock(&rx_ba->st_ba_lock);

    /* 判断本次定时器超时是否有帧上报 */
    if (us_baw_start != rx_ba->us_baw_start) {
        rx_ba->en_timer_triggered = OAL_TRUE;
        oam_warning_log4(dpe_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_ba_send_reorder_timeout::new baw_start:%d, old baw_start:%d, rx_timeout:%d, send mpdu cnt:%d.}",
            rx_ba->us_baw_start, us_baw_start, rx_timeout, uc_send_count);
    }

    hmac_rx_lan_frame(&st_netbuf_head);

    return OAL_SUCC;
}

#endif