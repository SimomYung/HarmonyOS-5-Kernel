/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : 1106 AMPDU聚合、BA处理接口定义源文件
 * 作    者 :
 * 创建日期 : 2024年11月12日
 */

/* 1 头文件包含 */
#include "wlan_spec.h"
#include "hmac_rx_reorder.h"
#include "hmac_rx.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_auto_adjust_freq.h"
#endif
#include "dpe_wlan_rx_reorder.h"
#ifdef _PRE_WLAN_FEATURE_11BE
#include "hmac_11be.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_REORDER_HW_C

const uint16_t g_ba_rx_timeout_len[WLAN_WME_AC_PSM + 2] = { // 2 enum num
    HMAC_BA_RX_BE_TIMEOUT,      /* WLAN_WME_AC_BE */
    HMAC_BA_RX_BK_TIMEOUT,      /* WLAN_WME_AC_BK */
    HMAC_BA_RX_VI_TIMEOUT,      /* WLAN_WME_AC_VI */
    HMAC_BA_RX_VO_TIMEOUT,      /* WLAN_WME_AC_VO */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_SOUNDING */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_MGMT */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_MC */
};

const uint16_t g_ba_rx_timeout_len_min[WLAN_WME_AC_PSM + 2] = { // 2 enum num
    HMAC_BA_RX_BE_TIMEOUT_MIN,      /* WLAN_WME_AC_BE */
    HMAC_BA_RX_BK_TIMEOUT_MIN,      /* WLAN_WME_AC_BK */
    HMAC_BA_RX_VI_TIMEOUT_MIN,      /* WLAN_WME_AC_VI */
    HMAC_BA_RX_VO_TIMEOUT_MIN,      /* WLAN_WME_AC_VO */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_SOUNDING */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_MGMT */
    HMAC_BA_RX_DEFAULT_TIMEOUT, /* WLAN_WME_AC_MC */
};

/*
 * 功能描述   : 更新ba_handler中的last release seqnum
 * 1.日    期   : 2019-08-22
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC void hmac_ba_rx_update_release_seqnum(hmac_ba_rx_stru *ba_rx_hdl, uint16_t release_sn)
{
    if (oal_unlikely((ba_rx_hdl == NULL) || (release_sn > MAX_BA_SN))) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_ba_rx_update_release_seqnum::release_sn[%d].}", release_sn);
        return;
    }

    if ((hmac_baw_rightside(ba_rx_hdl->us_last_relseq, release_sn)) || (ba_rx_hdl->us_last_relseq == release_sn)) {
        ba_rx_hdl->us_last_relseq = release_sn;
        return;
    } else {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_ba_rx_update_release_seqnum::sw req update wrong release sn.}");
        return;
    }
}

/*
 * 函 数 名  : hmac_remove_frame_from_reorder_q
 * 功能描述  : This function reads out the TX-Dscr indexed by the specified sequence number in
 *             the Retry-Q Ring-Buffer.
 * 1.日    期  : 2013年4月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE hmac_rx_buf_stru *hmac_remove_frame_from_ring_reorder_q(
    hmac_ba_rx_stru *pst_ba_rx_hdl, uint16_t us_seq_num)
{
    uint16_t us_idx;
    hmac_rx_buf_stru *pst_rx_buff;

    us_idx = (us_seq_num & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    pst_rx_buff = hmac_get_ba_rx_dhl(pst_ba_rx_hdl, us_idx);

    oal_spin_lock(&pst_ba_rx_hdl->st_ba_lock);
    if (pst_rx_buff->in_use == OAL_FALSE) {
        if (oal_unlikely(!oal_netbuf_list_empty(&pst_rx_buff->st_netbuf_head))) {
            oam_error_log1(0, OAM_SF_BA, "{hmac_remove_frame_from_reorder_q::[MEMORY-LEAK], seq[%d].}", us_seq_num);
        }
        oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);
        return NULL;
    }

    if (pst_rx_buff->us_seq_num != us_seq_num) {
        hmac_rx_free_netbuf_list(&pst_rx_buff->st_netbuf_head, pst_rx_buff->uc_num_buf);
        pst_rx_buff->in_use = OAL_FALSE;
        if (oal_unlikely(pst_ba_rx_hdl->uc_mpdu_cnt == 0)) {
            oam_warning_log0(0, OAM_SF_RX, "{hmac_remove_frame_from_reorder_q::uc_mpdu_cnt is 0.}");
            oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);
            return NULL;
        }
        pst_ba_rx_hdl->uc_mpdu_cnt--;
        oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);
        return NULL;
    }

    pst_rx_buff->in_use = OAL_FALSE;
    if (pst_ba_rx_hdl->uc_mpdu_cnt == 0) {
        hmac_rx_free_netbuf_list(&pst_rx_buff->st_netbuf_head, pst_rx_buff->uc_num_buf);
        oam_warning_log0(0, OAM_SF_RX, "{hmac_remove_frame_from_reorder_q::pst_ba_rx_hdl->uc_mpdu_cnt is 0.}");
        oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);
        return NULL;
    }

    pst_ba_rx_hdl->uc_mpdu_cnt--;
    oal_spin_unlock(&pst_ba_rx_hdl->st_ba_lock);

    return pst_rx_buff;
}

/*
 * 功能描述   : 重排序释放到(sn-1)的序列号
 * 1.日    期   : 2020.7.20
 *   作    者   : wifi
 *   修改内容   : 新生成函数
*  2.日    期   : 2021.4.10
 *   作    者   : wifi
 *   修改内容   : 增加reorder超时逻辑
 */
OAL_STATIC void hmac_rx_reorder_release_to_sn(hmac_ba_rx_stru *ba_rx_hdl, mac_rx_ctl_stru *rx_ctl,
    uint16_t sn, oal_netbuf_head_stru *rpt_list)
{
    uint16_t          start_sn;
    hmac_rx_buf_stru *rx_reo_entry = NULL;

    /* 从us_last_relseq + 1 一直上报到check_sequence_number -1，因为check_sequence_number走自己的逻辑上报 */
    start_sn = mac_ba_seqno_add(ba_rx_hdl->us_last_relseq, 1);
    if (sn == (mac_ba_seqno_add(ba_rx_hdl->us_last_relseq, 1))) {
        /* check的是last_relseq的下一个包,不需要主动上报任何包 */
        return;
    }
    oam_warning_log3(0, OAM_SF_RX,
        "{hmac_rx_reorder_release_to_sn::release frame.last_relseq[%d] process_flag[%d] release_is_valid[%d]}",
        ba_rx_hdl->us_last_relseq, rx_ctl->bit_process_flag, rx_ctl->bit_release_valid);
    oam_warning_log4(0, OAM_SF_RX,
        "{hmac_rx_reorder_release_to_sn::release frame.sn[%d] release_start_sn[%d] release_end_sn[%d], seq_num[%d]}",
        sn, rx_ctl->bit_release_start_sn, rx_ctl->bit_release_end_sn, rx_ctl->us_seq_num);
    do {
        rx_reo_entry = hmac_remove_frame_from_ring_reorder_q(ba_rx_hdl, start_sn);
        if (rx_reo_entry != NULL) {
            oal_netbuf_queue_splice_tail_init(&rx_reo_entry->st_netbuf_head, rpt_list);
            rx_reo_entry->uc_num_buf = 0;
        }
        start_sn = mac_ba_seqno_add(start_sn, 1);
    } while (start_sn != sn);

    hmac_ba_rx_update_release_seqnum(ba_rx_hdl, (uint16_t)mac_ba_seqno_sub(start_sn, 1));
}

/*
 * 功能描述   : 软件检查ba handler记录的last release sn与输入的sn的关系，上报手中应该上报的帧，更新last release sn
 * 1.日    期   : 2019-08-22
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC uint32_t hmac_rx_ba_release_check_sn(hmac_ba_rx_stru *ba_rx_hdl,
    uint16_t sn, mac_rx_ctl_stru *rx_ctl, oal_netbuf_head_stru *rpt_list)
{
    if ((hmac_baw_rightside(ba_rx_hdl->us_last_relseq, sn))) {
        hmac_rx_reorder_release_to_sn(ba_rx_hdl, rx_ctl, sn, rpt_list);
    } else if (ba_rx_hdl->us_last_relseq == sn) {
        return OAL_SUCC;
    } else {
        oam_info_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_check::input err check sn lrelease[%d] check[%d]}",
            ba_rx_hdl->us_last_relseq, sn);
        oam_info_log2(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_check::RELEASE_VALID[%d] PROCESS_FLAG[%d]}",
            rx_ctl->bit_release_valid, rx_ctl->bit_process_flag);
        oam_info_log4(0, OAM_SF_RX, "{hmac_ba_rx_buffered_data_check::SSN[%d] SN[%d] REL_START[%d] REL_END[%d].}",
            rx_ctl->bit_start_seqnum, mac_get_rx_cb_seq_num(rx_ctl),
            rx_ctl->bit_release_start_sn, rx_ctl->bit_release_end_sn);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_rx_ba_release_check_sn_failed(hmac_ba_rx_stru *ba_rx_hdl, mac_rx_ctl_stru *rx_ctl,
    uint16_t *release_start_sn, uint16_t release_end_sn)
{
    if (hmac_baw_rightside(ba_rx_hdl->us_last_relseq, release_end_sn)) {
        /* us_release_start_sn小于last_release而us_release_end_sn大于last_release则从last_release+1开始上报 */
        *release_start_sn = mac_ba_seqno_add(ba_rx_hdl->us_last_relseq, 1);
    } else if (ba_rx_hdl->us_last_relseq == release_end_sn) {
        return OAL_FAIL;
    } else {
        oam_warning_log3(0, OAM_SF_RX, "{hmac_rx_release_valid_check:: release_end_sn is less than \
                         last_relseq + 1, release_end_sn [%d] last_relseq [%d], start from [%d].}",
                         release_end_sn, ba_rx_hdl->us_last_relseq, rx_ctl->us_seq_num);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 功能描述   : release参数有效性检查
 * 1.日    期   : 2020.7.20
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
uint32_t hmac_rx_release_valid_check(hmac_ba_rx_stru *ba_rx_hdl, mac_rx_ctl_stru  *rx_ctl,
    oal_netbuf_head_stru *rpt_list, uint16_t *release_start_sn, uint16_t release_end_sn)
{
    uint32_t ret;

    if (oal_unlikely(*release_start_sn > MAX_BA_SN || release_end_sn > MAX_BA_SN)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_rx_release_valid_check::invalid param release_start_sn[%d] end_sn[%d].}",
            *release_start_sn, release_end_sn);
        return OAL_FAIL;
    }

    if ((hmac_baw_rightside(*release_start_sn, release_end_sn)) || (*release_start_sn == release_end_sn)) {
        /* 如果release_start_sn向后跳了，软件主动上报release_start_sn之前的包 */
        ret = hmac_rx_ba_release_check_sn(ba_rx_hdl, *release_start_sn, rx_ctl, rpt_list);
        if (ret != OAL_SUCC) {
            return hmac_rx_ba_release_check_sn_failed(ba_rx_hdl, rx_ctl, release_start_sn, release_end_sn);
        }
    } else {
        oam_warning_log2(0, OAM_SF_RX,
            "{hmac_rx_release_valid_check:: report wrong realese sequence number, start [%d] end [%d].}",
            *release_start_sn, release_end_sn);
        /* buffered frame走到此处则不做操作 */
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_get_ba_ring_reorder_timeout_len(uint8_t uc_tid)
{
    return (hmac_wifi_rx_is_busy() == OAL_FALSE) ? g_ba_rx_timeout_len_min[WLAN_WME_TID_TO_AC(uc_tid)] :
        g_ba_rx_timeout_len[WLAN_WME_TID_TO_AC(uc_tid)];
}

/*
 * 函 数 名   : hmac_rx_reo_timer_restart
 * 功能描述   : 重启重排序定时器
 * 1.日    期   : 2018-04-18
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC uint32_t hmac_ba_rx_reo_timer_restart(hmac_tid_ba_stru *hmac_tid,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_stru *netbuf_mpdu)
{
    uint8_t          rx_tid;
    dpe_wlan_vap_stru *dpe_vap = NULL;
    mac_rx_ctl_stru *rx_ctl = NULL;
    uint32_t timeout;

    if (oal_any_null_ptr3(hmac_tid, ba_rx_hdl, netbuf_mpdu)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_reo_timer_restart::invalid param.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_mpdu);
    rx_tid = mac_get_rx_cb_tid(rx_ctl);

    dpe_vap = dpe_wlan_vap_get(rx_ctl->uc_mac_vap_id);
    if (oal_unlikely(oal_any_null_ptr1(dpe_vap))) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_ba_rx_reo_timer_restart::dpe_vap NULL.}", rx_ctl->uc_mac_vap_id);
        return OAL_FAIL;
    }

    timeout = hmac_get_ba_ring_reorder_timeout_len(rx_tid);
    oal_spin_lock(&(hmac_tid->st_ba_timer_lock));
    frw_timer_create_timer_m(&(hmac_tid->st_ba_timer), hmac_ba_timeout_fn,
        timeout, &(hmac_tid->st_alarm_data), OAL_FALSE,
        OAM_MODULE_ID_HMAC, dpe_vap->core_id);
    oal_spin_unlock(&(hmac_tid->st_ba_timer_lock));

    return OAL_SUCC;
}

/*
 * 功能描述   : release接收缓存队列中的报文
 * 1.日    期   : 2020.7.20
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC void hmac_rx_ba_handle_release_flag(hmac_host_rx_context_stru *rx_context,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_head_stru *rpt_list)
{
    uint8_t           tid;
    uint16_t          release_end_sn;
    uint16_t          release_start_sn;
    uint32_t          rel_valid_check;
    mac_rx_ctl_stru  *rx_ctl = rx_context->cb;
    hmac_rx_buf_stru *rx_reo_entry = NULL;
    dpe_wlan_user_stru *dpe_user = rx_context->dpe_user;
    hmac_tid_ba_stru    *hmac_tid = NULL;

    if (rx_ctl->bit_release_valid == 0) {
        return;
    }

    /* 如果缓存帧需要处理，则根据描述符中的信息把缓存帧放入pst_netbuf_head链表中 */
    tid = mac_get_rx_cb_tid(rx_ctl);
    release_start_sn = rx_ctl->bit_release_start_sn;
    release_end_sn = rx_ctl->bit_release_end_sn;
    rel_valid_check = hmac_rx_release_valid_check(ba_rx_hdl, rx_ctl, rpt_list, &release_start_sn, release_end_sn);
    if (rel_valid_check != OAL_SUCC) {
        /* 直接返回，不做后续的释放操作 */
        return;
    }

    /* 更新release sequence number */
    hmac_ba_rx_update_release_seqnum(ba_rx_hdl, (uint16_t)release_end_sn);
    release_end_sn = mac_ba_seqno_add(release_end_sn, 1);
    do {
        rx_reo_entry = hmac_remove_frame_from_ring_reorder_q(ba_rx_hdl, release_start_sn);
        if (rx_reo_entry != NULL) {
            oal_netbuf_queue_splice_tail_init(&(rx_reo_entry->st_netbuf_head), rpt_list);
            rx_reo_entry->uc_num_buf = 0;
        }

        release_start_sn = mac_ba_seqno_add(release_start_sn, 1);
    } while ((release_start_sn != release_end_sn) && (ba_rx_hdl->uc_mpdu_cnt != 0));

    hmac_tid = hmac_get_tid_ba_info(dpe_user->us_assoc_id, mac_get_rx_cb_tid(rx_ctl));
    if (hmac_tid == NULL) {
        return;
    }
    /* 新增us_mpdu_cnt判断条件防止芯片上报异常的release sequence造成空循环 */
    /* 如果接收缓存队列里面还有报文，restart timer */
    if (ba_rx_hdl->uc_mpdu_cnt > 0) {
        if (hmac_tid->st_ba_timer.en_is_registerd == OAL_FALSE) {
            hmac_ba_rx_reo_timer_restart(hmac_tid, ba_rx_hdl, rx_context->netbuf);
        }
    }
}

/*
 * 函 数 名   : hmac_ba_buffer_frame_in_reorder
 * 1.日    期   : 2013年4月11日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC hmac_rx_buf_stru *hmac_ba_buffer_frame_in_reodr(hmac_ba_rx_stru *ba_rx_hdl,
    uint16_t sn, oal_netbuf_head_stru *pnetbuf_head)
{
    uint16_t        buf_index;
    hmac_rx_buf_stru *rx_buf = NULL;

    buf_index = (sn & (WLAN_AMPDU_RX_HE_BUFFER_SIZE - 1));
    rx_buf = hmac_get_ba_rx_dhl(ba_rx_hdl, buf_index);
    if (rx_buf == NULL) {
        return NULL;
    }

    if (rx_buf->in_use) {
        /* 如果缓存位置已经有包，则上报已存包并将新包存入 */
        oal_netbuf_queue_splice_tail_init(&(rx_buf->st_netbuf_head), pnetbuf_head);
        oam_info_log1(0, OAM_SF_BA, "{hmac_ba_buffer_frame_in_reorder::slot already used, seq[%d].}", sn);
    } else {
        if (oal_unlikely(!oal_netbuf_list_empty(&rx_buf->st_netbuf_head))) {
            oam_error_log1(0, OAM_SF_BA, "{hmac_ba_buffer_frame_in_reorder::[MEMORY-LEAK], seq[%d].}", sn);
        }
        ba_rx_hdl->uc_mpdu_cnt++;
    }

    rx_buf->in_use = OAL_TRUE;

    return rx_buf;
}

/*
 * 功能描述   : 把mpdu(amsdu)串到netbuf_head上
 * 1.日    期   : 2018-04-11
 *   作    者   : wifi
 *   修改内容   : 新增函数
 */
void hmac_rx_mpdu_to_netbuf_list(oal_netbuf_head_stru *netbuf_head, oal_netbuf_stru *netbuf_mpdu)
{
    oal_netbuf_stru *netbuf_cur = NULL;
    oal_netbuf_stru *netbuf_next = NULL;

    if (netbuf_head == NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_mpdu_to_netbuf_list::netbuf_head NULL.}");
        return;
    }

    netbuf_cur = netbuf_mpdu;
    while (netbuf_cur != NULL) {
        netbuf_next = oal_netbuf_next(netbuf_cur);
        oal_netbuf_add_to_list_tail(netbuf_cur, netbuf_head);
        netbuf_cur = netbuf_next;
    }
}

/*
 * 函 数 名   : hmac_rx_add_netbuf_2roe_list
 * 功能描述   : 把mpdu添加到接收缓存队列
 * 1.日    期   : 2018-04-18
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC void hmac_ba_add_netbuf_to_reo_list(hmac_ba_rx_stru *ba_rx_hdl,
    oal_netbuf_stru *netbuf_mpdu, oal_netbuf_head_stru *netbuf_head)
{
    mac_rx_ctl_stru  *rx_ctl = NULL;
    hmac_rx_buf_stru *rx_reo_entry = NULL;

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_mpdu);
    if (rx_ctl == NULL) {
        return;
    }
    rx_reo_entry = hmac_ba_buffer_frame_in_reodr(ba_rx_hdl, mac_get_rx_cb_seq_num(rx_ctl), netbuf_head);
    if (rx_reo_entry == NULL) {
        return;
    }

    /*
    * 在报文缓存到接收缓存队列的时候，队列中每个元素包含的是skb结构；
    * 如果是amsdu，则会把所有msdu以skb的形式串链
    **/
    rx_reo_entry->us_seq_num = mac_get_rx_cb_seq_num(rx_ctl);
    rx_reo_entry->rx_time = (uint32_t)oal_time_get_stamp_ms();
    oal_netbuf_list_head_init(&(rx_reo_entry->st_netbuf_head));
    hmac_rx_mpdu_to_netbuf_list(&(rx_reo_entry->st_netbuf_head), netbuf_mpdu);
    rx_reo_entry->uc_num_buf = oal_netbuf_list_len(&(rx_reo_entry->st_netbuf_head));
    if (hmac_ba_seqno_lt(ba_rx_hdl->us_baw_tail, mac_get_rx_cb_seq_num(rx_ctl)) == OAL_TRUE) {
        ba_rx_hdl->us_baw_tail = mac_get_rx_cb_seq_num(rx_ctl);
    }
    return;
}

uint32_t hmac_ba_check_rx_aggrate(dpe_wlan_vap_stru *dpe_vap, mac_rx_ctl_stru *rx_ctl, dpe_wlan_user_stru *dpe_user)
{
    uint8_t frame_type;

    if (oal_unlikely(oal_any_null_ptr2(dpe_vap, rx_ctl))) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_ba_check_rx_aggr::input params null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    frame_type = mac_get_frame_type_and_subtype((uint8_t *)&rx_ctl->us_frame_control);
    /* 判断该帧是不是qos帧 */
    if (frame_type != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_ba_check_rx_aggr::not qos data.}");
        return OAL_FAIL;
    }
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    /* 设置组播标记位 */
    if (dpe_user->is_mcast_ampdu == OAL_TRUE && rx_ctl->bit_mcast_bcast == OAL_TRUE) {
        rx_ctl->bit_mcast_bcast = OAL_FALSE;
        oam_warning_log2(0, OAM_SF_BA, "{hmac_ba_check_rx_aggr::This is a multicast data frame,seqnum[%d], proc[%d].}",
            rx_ctl->us_seq_num, rx_ctl->bit_process_flag);
    }
#endif
    /* 对于STA还需要判断是否是广播帧，考虑兼容某些设备广播帧带qos域 */
    if ((dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (rx_ctl->bit_mcast_bcast == OAL_TRUE)) {
        oam_info_log0(0, OAM_SF_BA, "{hmac_ba_check_rx_aggr::This is a multicast data frame.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述   : release到当前报文为止的所以缓存帧
 * 1.日    期   : 2020.7.20
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
uint32_t hmac_ba_rx_data_release(hmac_host_rx_context_stru *rx_context,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_head_stru *rpt_list, uint8_t *buff_is_valid)
{
    uint32_t         ret;
    oal_netbuf_stru *netbuf = rx_context->netbuf;
    mac_rx_ctl_stru *rx_ctl = rx_context->cb;

    *buff_is_valid = OAL_FALSE;

    /* 1. 从上次释放报文的seq到当前报文的seq - 1之间的报文全部上报 */
    ret = hmac_rx_ba_release_check_sn(ba_rx_hdl, mac_get_rx_cb_seq_num(rx_ctl), rx_ctl, rpt_list);
    if (ret != OAL_SUCC) {
        rx_ctl->is_before_last_release = OAL_TRUE;
        oam_info_log2(0, OAM_SF_RX,
            "{hmac_ba_rx_data_release:: release a frame sn = [%d] before last release = [%d]}",
            mac_get_rx_cb_seq_num(rx_ctl), ba_rx_hdl->us_last_relseq);
        return ret;
    }

    /* 2.当前buf上报 */
    hmac_rx_mpdu_to_netbuf_list(rpt_list, netbuf);
    /* 单个报文上报，更新release_seqnum */
    hmac_ba_rx_update_release_seqnum(ba_rx_hdl, (uint16_t)mac_get_rx_cb_seq_num(rx_ctl));

    ba_rx_hdl->us_baw_start = rx_ctl->bit_start_seqnum;
    /* 3.继续判断是否需要进行缓存上报 */
    hmac_rx_ba_handle_release_flag(rx_context, ba_rx_hdl, rpt_list);

    /* 4.如果SSN还在last_release_seqnum后面，则可以继续release */
    if (hmac_baw_rightside(ba_rx_hdl->us_last_relseq, rx_ctl->bit_start_seqnum)) {
        hmac_rx_ba_release_check_sn(ba_rx_hdl, rx_ctl->bit_start_seqnum, rx_ctl, rpt_list);
    }

    return OAL_SUCC;
}

/*
 * 功能描述   : HAL_RX_PROC_FLAGS_BUFFER 处理函数
 *              1)如果需要release接收缓存队列中的报文，则先release;
 *              2)缓存当前报文到接收缓存队列。
 * 1.日    期   : 2018-04-18
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
uint32_t hmac_ba_rx_data_buffer(hmac_host_rx_context_stru *rx_context,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_head_stru *rpt_list, uint8_t *buff_is_valid)
{
    oal_netbuf_stru *netbuf = rx_context->netbuf;
    mac_rx_ctl_stru *rx_ctl = rx_context->cb;
    dpe_wlan_user_stru *dpe_user = rx_context->dpe_user;
    hmac_tid_ba_stru   *hmac_tid = NULL;

    if (oal_any_null_ptr3(netbuf, rx_ctl, dpe_user)) {
        return OAL_FAIL;
    }
    hmac_tid = hmac_get_tid_ba_info(dpe_user->us_assoc_id, mac_get_rx_cb_tid(rx_ctl));
    if (hmac_tid == NULL) {
        return OAL_FAIL;
    }

    *buff_is_valid = OAL_FALSE;
    ba_rx_hdl->us_baw_start = rx_ctl->bit_start_seqnum;
    /* 1.BA window start移到us_baw_start后，硬件后续不会再收us_baw_start之前的包，缓存的us_baw_start之前的包都可以上报 */
    hmac_rx_ba_release_check_sn(ba_rx_hdl, rx_ctl->bit_start_seqnum, rx_ctl, rpt_list);

    /* 2.判断缓存数据是否需要进行上报 */
    hmac_rx_ba_handle_release_flag(rx_context, ba_rx_hdl, rpt_list);

    /* 3.当前数据进行缓存 */
    hmac_ba_add_netbuf_to_reo_list(ba_rx_hdl, netbuf, rpt_list);

    /* 4.启动超时定时器 */
    if (hmac_tid->st_ba_timer.en_is_registerd == OAL_FALSE) {
        oam_info_log3(0, OAM_SF_RX, "{hmac_ba_rx_data_buffer::restart timer user id[%d], tid[%d], mpdu cnt[%d].}",
                      rx_ctl->bit_rx_user_id, rx_ctl->bit_rx_tid, ba_rx_hdl->uc_mpdu_cnt);
        hmac_ba_rx_reo_timer_restart(hmac_tid, ba_rx_hdl, netbuf);
    }

    return OAL_SUCC;
}

OAL_STATIC uint16_t hmac_rx_ba_get_baw_head(hmac_ba_rx_stru *rx_ba, uint16_t baw_head)
{
    return (hmac_baw_rightside(baw_head, hmac_ba_seqno_add(rx_ba->us_last_relseq, 1))) ? baw_head :
        hmac_ba_seqno_add(rx_ba->us_last_relseq, 1);
}

OAL_STATIC uint32_t hmac_rx_frm_diff_time(hmac_rx_buf_stru *rx_buf, uint32_t curr_time)
{
    uint32_t time_diff;

    if (curr_time >= rx_buf->rx_time) {
        time_diff = curr_time - rx_buf->rx_time;
    } else {
        time_diff = (HMAC_U32_MAX - rx_buf->rx_time) + curr_time;
    }

    return time_diff;
}

/*
 * 功能描述   : 从重排序队列中获取skb链
 * 1.日    期   : 2014年5月21日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC uint32_t hmac_ba_rx_prepare_buffer(hmac_rx_buf_stru *rx_entry, oal_netbuf_head_stru *pnetbuf_head)
{
    oal_netbuf_stru    *netbuf = NULL;
    mac_rx_ctl_stru    *rx_ctl = NULL;

    if (oal_any_null_ptr1(pnetbuf_head)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_rx_prepare_buffer::input params null.}");
        return OAL_FAIL;
    }

    if (rx_entry == NULL) {
        oam_warning_log0(0, OAM_SF_BA, "{hmac_ba_rx_prepare_buffer::rx_reo_entry null.}");
        return OAL_FAIL;
    }

    if (oal_netbuf_list_empty(&rx_entry->st_netbuf_head)) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_ba_rx_prepare_buffer::netbuf_head is empty.}");
        return OAL_FAIL;
    }

    /* 按顺序串链缓存帧 */
    while (!oal_netbuf_list_empty(&(rx_entry->st_netbuf_head))) {
        netbuf = oal_netbuf_delist(&(rx_entry->st_netbuf_head));
        if (netbuf == NULL) {
            oam_error_log0(0, OAM_SF_RX, "{hmac_ba_rx_prepare_buffer::netbuf null.}");
            return OAL_FAIL;
        }

        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        rx_ctl->bit_is_reo_timeout = OAL_TRUE;
        oal_netbuf_add_to_list_tail(netbuf, pnetbuf_head);
    }

    return OAL_SUCC;
}

/*
 * 功能描述   : 上报重排序队列中超时的报文
 * 1.日    期   : 2014年5月19日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
uint32_t hmac_ba_send_ring_reorder_timeout(hmac_ba_rx_stru *rx_ba, dpe_wlan_vap_stru *dpe_vap,
    hmac_ba_alarm_stru *alarm_data, uint32_t *p_timeout)
{
    uint32_t             curr_time, time_diff, rx_timeout;
    uint16_t             baw_head, baw_start, baw_end;
    uint32_t             ret;
    oal_netbuf_head_stru netbuf_head;
    hmac_rx_buf_stru    *rx_buf = NULL;

    oal_netbuf_list_head_init(&netbuf_head);
    baw_head = rx_ba->us_baw_start;
    baw_start = rx_ba->us_baw_start;
    rx_timeout = hmac_get_ba_ring_reorder_timeout_len(alarm_data->uc_tid);
    baw_end = hmac_ba_seqno_add(rx_ba->us_baw_tail, 1);

    if (baw_head != hmac_ba_seqno_add(rx_ba->us_last_relseq, 1)) {
        /* baw_start eq. us_last_relseq + 1 从较靠左侧的开始判断超时 */
        oam_info_log3(0, 0, "{hmac_ba_send_ring_reorder_timeout:baw_start[%d]!=last_relseq[%d]+1, baw_end = [%d]}",
            baw_head, rx_ba->us_last_relseq, rx_ba->us_baw_tail);
        baw_head = hmac_rx_ba_get_baw_head(rx_ba, baw_head);
    }

    curr_time = (uint32_t)oal_time_get_stamp_ms();
    while (baw_head != baw_end) {
        rx_buf = hmac_get_frame_from_reorder_q(rx_ba, baw_head);
        if (rx_buf == NULL) {
            baw_head = hmac_ba_seqno_add(baw_head, 1);
            continue;
        }

        /* 获取当前时间和收到报文时的差值，差值如果大于定时器timer，则release报文 */
        time_diff = hmac_rx_frm_diff_time(rx_buf, curr_time);
        if (time_diff < rx_timeout) {
            *p_timeout = rx_timeout - time_diff;
            break;
        }
        rx_buf = hmac_remove_frame_from_ring_reorder_q(rx_ba, baw_head);
        ret = hmac_ba_rx_prepare_buffer(rx_buf, &netbuf_head);

        /* timeout上报中无需check软件是否需要主动上报缓存帧，因为对于rx_ba->us_baw_start之前的缓存帧一定会在此 */
        /* baw_start handler的地方检查了，此处只需更新last release sequence number */
        hmac_ba_rx_update_release_seqnum(rx_ba, baw_head);
        if (ret != OAL_SUCC) {
            baw_head = hmac_ba_seqno_add(baw_head, 1);
            continue;
        }
        baw_head = hmac_ba_seqno_add(baw_head, 1);
    }
    rx_ba->us_baw_start = baw_head;
    if (baw_start != rx_ba->us_baw_start) {
        rx_ba->en_timer_triggered = OAL_TRUE;
    }
    /* To kernel or to wlan */
    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_rx_data_sta_proc(dpe_vap, &netbuf_head);
        g_hisi_softwdt_check.rxshed_sta++;
    }
    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_data_ap_proc(dpe_vap, &netbuf_head);
        g_hisi_softwdt_check.rxshed_ap++;
    }
    /* sch frames to kernel */
    if (hmac_get_rxthread_enable() == OAL_TRUE) {
        hmac_rxdata_sched();
    }
    return OAL_SUCC;
}

/*
 * 功能描述   : 对收到的mpdu做重排序处理
 * 1.日    期   : 2018-04-11
 *   作    者   : wifi
 *   修改内容   : 新增函数
 */
OAL_STATIC uint32_t hmac_rx_data_wnd_proc(hmac_host_rx_context_stru *rx_context,
    hmac_ba_rx_stru *ba_rx_hdl, oal_netbuf_head_stru *rpt_list, uint8_t *buff_is_valid)
{
    uint32_t ret = OAL_SUCC;
    mac_rx_ctl_stru *rx_ctl = rx_context->cb;

    switch (rx_ctl->bit_process_flag) {
        case HAL_RX_PROC_FLAGS_RELEASE:
            ret = hmac_ba_rx_data_release(rx_context, ba_rx_hdl, rpt_list, buff_is_valid);
            break;
        case HAL_RX_PROC_FLAGS_BUFFER:
            ret = hmac_ba_rx_data_buffer(rx_context, ba_rx_hdl, rpt_list, buff_is_valid);
            break;
        case HAL_RX_PROC_FLAGS_OUT_OF_WIND_DROP:
        case HAL_RX_PROC_FLAGS_DUPLICATE_DROP:
        default:
            return OAL_FAIL;
    }

    if (oal_unlikely(ret != OAL_SUCC)) {
        /* 缓存或者直接上报处理失败时会进入此处，这里需要check失败的包上报 */
        hmac_rx_mpdu_to_netbuf_list(rpt_list, rx_context->netbuf);
        oam_info_log1(0, OAM_SF_RX, "{hmac_rx_data_wnd_proc:: process flag[%d].}", rx_ctl->bit_process_flag);
    }

    return OAL_SUCC;
}

/*
 * 功能描述  : 重排序逻辑
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_rx_proc_reorder(hmac_host_rx_context_stru *rx_context,
    oal_netbuf_head_stru *rpt_list, uint8_t *buff_is_valid)
{
    dpe_wlan_vap_stru *dpe_vap = rx_context->dpe_vap;
    dpe_wlan_user_stru *dpe_user = rx_context->dpe_user;
    mac_rx_ctl_stru *rx_ctl = rx_context->cb;
    uint32_t         ret;
    hmac_ba_rx_stru *ba_rx_hdl = NULL;
    hmac_tid_ba_stru   *hmac_tid = NULL;

    if ((dpe_user == NULL) || (buff_is_valid == OAL_FALSE)) {
        return OAL_SUCC;
    }
    if (hmac_ba_check_rx_aggrate(dpe_vap, rx_ctl, dpe_user) != OAL_SUCC) {
        return OAL_SUCC;
    }

    hmac_tid = hmac_get_tid_ba_info(dpe_user->us_assoc_id, mac_get_rx_cb_tid(rx_ctl));
    if (hmac_tid == NULL) {
        return OAL_SUCC;
    }
    ba_rx_hdl = hmac_tid->pst_ba_rx_info;
    if (ba_rx_hdl == NULL) {
        return OAL_SUCC;
    }

    if (ba_rx_hdl->en_ba_status != DMAC_BA_COMPLETE) {
        return OAL_SUCC;
    }
    ret = hmac_rx_data_wnd_proc(rx_context, ba_rx_hdl, rpt_list, buff_is_valid);
    if (ret != OAL_SUCC) {
        return ret;
    }

    return OAL_SUCC;
}
