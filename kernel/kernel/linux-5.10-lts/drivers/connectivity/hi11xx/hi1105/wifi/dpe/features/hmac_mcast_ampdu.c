/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : 组播聚合功能
 * 作者       : wifi
 * 创建日期   : 2021年04月15日
 */

#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
/* 1 其他头文件包含 */

#include "hmac_tx_data.h"
#include "hmac_host_tx_data.h"
#include "hmac_resource.h"
#include "hmac_tx_ring_alloc.h"
#include "hmac_rx_reorder.h"
#include "oam_stat_wifi.h"
#include "hmac_wapi.h"
#include "hmac_mcast_ampdu.h"
#ifdef _PRE_WLAN_FEATURE_PWL
#include "hmac_pwl.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MCAST_AMPDU_C

#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
/* CHBA可靠组播收帧统计信息 */
hmac_mcast_ampdu_rx_stats_stru g_mcast_ampdu_rx_info_stru[WLAN_VAP_MAX_NUM_LIMIT];
/*
 * 功能描述 : 组播聚合场景，host侧tid、ring信息初始化
 * 1.日    期 : 2021年4月15日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
void hmac_mcast_ampdu_user_tid_init(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user, uint8_t *multi_user_lut_idx)
{
    mac_chip_stru *mac_chip;
    if (mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) {
        return;
    }
    if (hmac_ring_tx_enabled() != OAL_TRUE) {
        oam_warning_log0(0, 0, "{hmac_mcast_ampdu_user_tid_init::return due to tx_switch != HOST_TX}");
        return;
    }
    mac_chip = hmac_res_get_mac_chip(mac_vap->uc_chip_id);
    if (mac_chip == NULL) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_mcast_ampdu_user_tid_init::mac_chip null.}");
        return;
    }
    /* 组播用户也申请lut index, 最大用户数是否需要新增一个？ */
    if (hmac_alloc_user_lut_index(hmac_user, mac_chip, MAC_AP_TYPE_NORMAL) != OAL_SUCC) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_TX,
            "{hmac_mcast_ampdu_user_tid_init::get multi user lut failed.}");
        hmac_user_free(hmac_user->st_user_base_info.us_assoc_id);
        return;
    }
    /* 配置lut index，同步到device */
    *multi_user_lut_idx = hmac_user->lut_index;
    oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_TX,
        "{MCAST AMPDU::hmac_mcast_ampdu_user_tid_init::multi user lut idx = [%d].}", hmac_user->lut_index);
    /* 初始化ring信息 */
    hmac_tid_sche_list_dump(HMAC_TID_SCHE_LIST_BEFORE_ADD_MCAST_USER);
    hmac_ring_tx_init(hmac_user->st_user_base_info.us_assoc_id, WLAN_TIDNO_MCAST_AMPDU);
    hmac_tid_sche_list_dump(HMAC_TID_SCHE_LIST_AFTER_ADD_MCAST_USER);
}

void hmac_mcast_ampdu_stats_stru_init(hmac_vap_stru *hmac_vap)
{
    hmac_mcast_ampdu_rx_stats_stru *rx_info = NULL;

    hmac_vap->mcast_ampdu_rx_info = (void *)(&g_mcast_ampdu_rx_info_stru[hmac_vap->st_vap_base_info.uc_vap_id]);
    memset_s((void *)hmac_vap->mcast_ampdu_rx_info, sizeof(hmac_mcast_ampdu_rx_stats_stru),
        0, sizeof(hmac_mcast_ampdu_rx_stats_stru));

    rx_info = (hmac_mcast_ampdu_rx_stats_stru *)hmac_vap->mcast_ampdu_rx_info;
    /* 初始化不开启PACK反馈功能 */
    rx_info->pack_enable = OAL_FALSE;

    /* 初始化device接收窗口 */
    memset_s((void *)rx_info->rx_window_bitmap, sizeof(uint32_t) * BYTE_OFFSET_8,
        0, sizeof(uint32_t) * BYTE_OFFSET_8);
    rx_info->reorder_start = 0;
    rx_info->reorder_end = mac_ba_seqno_add(rx_info->reorder_start, 1);
    rx_info->window_size = WLAN_AMPDU_RX_HE_BUFFER_SIZE;
    rx_info->window_end =
        mac_ba_seqno_add(rx_info->reorder_start, rx_info->window_size - 1);
}

static uint16_t hmac_get_bitmap_index(uint16_t seqno, uint16_t bitmap_size)
{
    return (seqno & (bitmap_size - 1));
}

static uint32_t hmac_tx_buf_bitmap_is_set(uint32_t *bitmap, uint16_t idx)
{
    return ((bitmap[(idx >> BIT_OFFSET_5)] & ((uint32_t)(1U << (idx & BIT_OFFSET_31)))) != 0);
}

static void hmac_flush_mcast_ampdu_rx_window(hmac_mcast_ampdu_rx_stats_stru *rx_reorder)
{
    uint16_t idx;

    /* device接收窗口头部右移 */
    while (hmac_ba_rx_seqno_lt(rx_reorder->reorder_start, rx_reorder->reorder_end)) {
        idx = hmac_get_bitmap_index(rx_reorder->reorder_start, rx_reorder->window_size);
        if (!hmac_tx_buf_bitmap_is_set(rx_reorder->rx_window_bitmap, idx)) {
            break;
        }
        /* reorder窗口起始位置右移一个MPDU */
        rx_reorder->reorder_start = mac_ba_seqno_add(rx_reorder->reorder_start, 1);
    }

    /* device接收窗口尾部右移 */
    while (rx_reorder->window_end != mac_ba_seqno_add(rx_reorder->reorder_start, rx_reorder->window_size - 1)) {
        rx_reorder->window_end = mac_ba_seqno_add(rx_reorder->window_end, 1);
        /* 清理device接收窗口 */
        idx = hmac_get_bitmap_index(rx_reorder->window_end, rx_reorder->window_size);
        hmac_rx_buf_bitmap_clr(rx_reorder->rx_window_bitmap, idx);
    }

    /* 更新device接收窗口中reorder窗口 */
    if (hmac_ba_rx_seqno_geq(rx_reorder->reorder_start, rx_reorder->reorder_end)) {
        rx_reorder->reorder_end = mac_ba_seqno_add(rx_reorder->reorder_start, 1);
    }
}

static void hmac_move_mcast_ampdu_rx_window_one_mpdu(hmac_mcast_ampdu_rx_stats_stru *rx_reorder)
{
    uint16_t idx;

    /* 窗口强制向右移动一个MPDU */
    rx_reorder->reorder_start = mac_ba_seqno_add(rx_reorder->reorder_start, 1);
    if (hmac_ba_rx_seqno_geq(rx_reorder->reorder_start, rx_reorder->reorder_end)) {
        rx_reorder->reorder_end = mac_ba_seqno_add(rx_reorder->reorder_start, 1);
    }
    rx_reorder->window_end = mac_ba_seqno_add(rx_reorder->window_end, 1);
    idx = hmac_get_bitmap_index(rx_reorder->window_end, rx_reorder->window_size);
    hmac_rx_buf_bitmap_clr(rx_reorder->rx_window_bitmap, idx);

    /* 强制移窗后刷新device接收窗口 */
    hmac_flush_mcast_ampdu_rx_window(rx_reorder);
}

static void hmac_update_mcast_ampdu_rx_window(hmac_mcast_ampdu_rx_stats_stru *rx_reorder, uint16_t seqno)
{
    uint16_t idx;

    /* MPDU在reorder区中 */
    if (hmac_ba_rx_seqno_geq(seqno, rx_reorder->reorder_start) && hmac_ba_seqno_lt(seqno, rx_reorder->reorder_end)) {
        idx = hmac_get_bitmap_index(seqno, rx_reorder->window_size);
        hmac_rx_buf_bitmap_set(rx_reorder->rx_window_bitmap, idx);
        hmac_flush_mcast_ampdu_rx_window(rx_reorder);
        return;
    }

    /* MPDU在reorder区外的接收窗口中,需更新reorder队尾 */
    if (hmac_ba_rx_seqno_geq(seqno, rx_reorder->reorder_end) && hmac_ba_rx_seqno_lt(seqno, rx_reorder->window_end)) {
        idx = hmac_get_bitmap_index(seqno, rx_reorder->window_size);
        hmac_rx_buf_bitmap_set(rx_reorder->rx_window_bitmap, idx);
        rx_reorder->reorder_end = mac_ba_seqno_add(seqno, 1);
        if (hmac_ba_rx_seqno_geq(rx_reorder->reorder_end, rx_reorder->window_end)) {
            /* device接收窗口已满,需强制移窗 */
            oam_warning_log0(0, OAM_SF_RX, "{hmac_update_mcast_ampdu_rx_window:rx windows overflow!}");
            hmac_move_mcast_ampdu_rx_window_one_mpdu(rx_reorder);
        }
        return;
    }

    /* MPDU落在device接收窗口外,需强制移窗 */
    if (hmac_ba_rx_seqno_geq(seqno, rx_reorder->window_end)) {
        while (hmac_ba_rx_seqno_geq(seqno, rx_reorder->window_end)) {
            idx = hmac_get_bitmap_index(rx_reorder->reorder_start, rx_reorder->window_size);
            hmac_rx_buf_bitmap_clr(rx_reorder->rx_window_bitmap, idx);
            rx_reorder->window_end = mac_ba_seqno_add(rx_reorder->window_end, 1);
            rx_reorder->reorder_start = mac_ba_seqno_sub(rx_reorder->window_end, rx_reorder->window_size - 1);
        }
        /* 更新MPDU信息 */
        idx = hmac_get_bitmap_index(seqno, rx_reorder->window_size);
        hmac_rx_buf_bitmap_set(rx_reorder->rx_window_bitmap, idx);
        rx_reorder->reorder_end = mac_ba_seqno_add(seqno, 1);
        if (hmac_ba_rx_seqno_geq(rx_reorder->reorder_end, rx_reorder->window_end)) {
            /* device接收窗口已满,需强制移窗 */
            hmac_move_mcast_ampdu_rx_window_one_mpdu(rx_reorder);
        }
        hmac_flush_mcast_ampdu_rx_window(rx_reorder);
    }
}

oal_bool_enum hmac_filter_duplicate_mcast_ampdu_pkt(hmac_mcast_ampdu_rx_stats_stru *rx_reorder,
    uint16_t seqno)
{
    uint16_t idx;
    /* reorder区前的帧,认为是重复帧 */
    if (hmac_ba_seqno_lt(seqno, rx_reorder->reorder_start)) {
        return OAL_TRUE;
    }

    if (hmac_ba_rx_seqno_geq(seqno, rx_reorder->reorder_start) && hmac_ba_seqno_lt(seqno, rx_reorder->reorder_end)) {
        /* reorder区间的帧,通过bitmap确认是否已经收到 */
        idx = hmac_get_bitmap_index(seqno, rx_reorder->window_size);
        if (hmac_tx_buf_bitmap_is_set(rx_reorder->rx_window_bitmap, idx)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

hmac_rx_frame_ctrl_enum hmac_mcast_ampdu_rx_proc(uint8_t vap_idx, oal_netbuf_stru *netbuf)
{
    hmac_mcast_ampdu_rx_stats_stru *client_info = NULL;
    mac_ether_header_stru *ether_hdr = NULL;
    mac_rx_ctl_stru *cb = NULL;
    uint16_t seq_num;
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(vap_idx);

    if (netbuf == NULL || hmac_vap == NULL) {
        oam_error_log0(vap_idx, 0, "{hmac_mcast_ampdu_rx_proc::netbuf or hmac_vap is null}");
        return HMAC_RX_FRAME_CTRL_GOON;
    }
    client_info = (hmac_mcast_ampdu_rx_stats_stru *)(hmac_vap->mcast_ampdu_rx_info);
    if (client_info == NULL) {
        oam_error_log0(vap_idx, 0, "{hmac_mcast_ampdu_rx_proc::mcast rx info is null}");
        return HMAC_RX_FRAME_CTRL_GOON;
    }
    if ((mac_get_mcast_ampdu_switch() != OAL_TRUE) || (client_info->pack_enable != OAL_TRUE)) {
        return HMAC_RX_FRAME_CTRL_GOON;
    }

    ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if ((hmac_vap->st_vap_base_info.en_vap_mode != WLAN_VAP_MODE_BSS_STA) ||
        (ether_hdr != NULL && ether_is_multicast(ether_hdr->auc_ether_dhost) == OAL_FALSE)) {
        return HMAC_RX_FRAME_CTRL_GOON;
    }

    /* 统计全部收到的组播聚合帧 */
    client_info->rx_all_pkts++;
    cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (cb == NULL) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }
    seq_num = cb->us_seq_num;
    /* 过滤重复帧 */
    if (hmac_filter_duplicate_mcast_ampdu_pkt(client_info, seq_num) == OAL_TRUE) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    /* 统计全部收到的非重复组播聚合帧 */
    client_info->rx_non_retry_pkts++;

    /* 更新device接收窗口 */
    hmac_update_mcast_ampdu_rx_window(client_info, seq_num);
    return HMAC_RX_FRAME_CTRL_GOON;
}

void hmac_mcast_ampdu_set_pack_switch(mac_vap_stru *mac_vap, uint8_t en)
{
    hmac_vap_stru *hmac_vap;
    hmac_mcast_ampdu_rx_stats_stru *client_info = NULL;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(mac_vap->uc_vap_id, 0, "{hmac_mcast_ampdu_set_pack_switch::hmac_vap null.}");
        return;
    }

    client_info = (hmac_mcast_ampdu_rx_stats_stru *)(hmac_vap->mcast_ampdu_rx_info);
    client_info->pack_enable = en;
    oam_warning_log1(0, 0, "{hmac_mcast_ampdu_set_pack_switch::pack switch %d}", en);
}
#endif

#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU_HW
/*
 * 功能描述 : 组播聚合sta 添加组播用户,通知dmac
 * 1.日    期 : 2021年5月12日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC uint32_t hmac_mcast_ampdu_user_add_post_event(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    uint8_t *mac_addr, uint16_t user_idx)
{
    frw_event_mem_stru *event_mem = NULL;
    frw_event_stru *frw_event = NULL;
    dmac_ctx_add_user_stru *add_user_payload = NULL;
    uint32_t ret;

    event_mem = frw_event_alloc_m(sizeof(dmac_ctx_add_user_stru));
    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::event_mem null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    frw_event = frw_get_event_stru(event_mem);
    add_user_payload = (dmac_ctx_add_user_stru *)frw_event->auc_event_data;
    add_user_payload->us_user_idx = user_idx;
    add_user_payload->en_ap_type = hmac_user->en_user_ap_type;
    add_user_payload->lut_index = hmac_user->lut_index;
    oal_set_mac_addr(add_user_payload->auc_user_mac_addr, mac_addr);

    /* 填充事件头 */
    frw_event_hdr_init(&(frw_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_ADD_MULTI_USER,
        sizeof(dmac_ctx_add_user_stru), FRW_EVENT_PIPELINE_STAGE_1, mac_vap->uc_chip_id,
        mac_vap->uc_device_id, mac_vap->uc_vap_id);

    ret = frw_event_dispatch_event(event_mem);
    dpe_wlan_mcast_ampdu_user_add(event_mem);

    frw_event_free_m(event_mem);
    return ret;
}

/*
 * 功能描述 : 组播聚合sta 添加组播用户
 * 1.日    期 : 2021年5月12日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
static void hmac_mcast_ampdu_add_user(mac_vap_stru *mac_vap, uint8_t *mac_addr, uint8_t mac_addr_len)
{
    uint32_t ret;
    uint16_t user_index;
    mac_user_stru *mac_user = NULL;
    hmac_user_stru *hmac_user = NULL;
    uint8_t multi_user_lut_idx;

    ret = hmac_user_alloc(&user_index);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_UM,
            "{hmac_mcast_ampdu_add_user::hmac_user_alloc failed[%d].}", ret);
        return;
    }

    /* 初始化组播用户基本信息 */
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(user_index);
    if (hmac_user == NULL) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_mcast_ampdu_add_user::mac_user[%d] null.}", user_index);
        return;
    }
    mac_user = &hmac_user->st_user_base_info;
    mac_user_init(mac_user, user_index, mac_addr,
        mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);
    mac_user->en_is_multi_user = OAL_TRUE;
    mac_user_init_rom(mac_user, user_index);

    mac_user->en_user_asoc_state = MAC_USER_STATE_ASSOC;
    /* 设置mac地址 */
    oal_set_mac_addr(mac_user->auc_user_mac_addr, mac_addr);
    mac_user_set_qos(mac_user, OAL_TRUE);
#ifdef _PRE_WLAN_FEATURE_WAPI
    /* 初始化wapi对象 */
    hmac_user_wapi_init(hmac_user, OAL_FALSE);
#endif
#ifdef _PRE_WLAN_FEATURE_PWL
    if (mac_vap->pwl_enable == OAL_TRUE) {
        hmac_user_add_init_pwl(hmac_user);
    }
#endif
    /* 组播聚合场景初始化tid、ring信息 */
    hmac_mcast_ampdu_user_tid_init(mac_vap, hmac_user, &multi_user_lut_idx);

    /***************************************************************************
        抛事件到DMAC层, 创建dmac用户
    ***************************************************************************/
    ret = hmac_mcast_ampdu_user_add_post_event(mac_vap, hmac_user, mac_addr, user_index);
    if (ret != OAL_SUCC) {
        /* 异常处理，释放内存，device下关联用户数还没有++，这里不需要判断返回值做--操作 */
        hmac_user_free(user_index);
        /* 不应该出现用户添加失败，失败需要定位具体原因 */
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_mcast_ampdu_add_user::dispatch_event failed[%d].}", ret);
        return;
    }
    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_ANY,
        "{hmac_mcast_ampdu_add_user, user index[%d] lut idx[%d].}", user_index, multi_user_lut_idx);
}

/*
 * 功能描述 : 组播聚合场景，mp16c需要为sta额外增加一个组播用户，mac地址为ap的mac地址，bit40设置为1
 * 1.日    期 : 2021年5月12日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
static void hmac_mcast_ampdu_get_addr(uint8_t *multi_addr, uint8_t multi_addr_len, uint8_t *mac_addr)
{
    oal_set_mac_addr(multi_addr, mac_addr);
    multi_addr[0] |= BIT0; /* addr bit40设置为1 */
}

/*
 * 功能描述 : 组播聚合场景，mp16c需要为sta额外增加一个组播用户，用于reorder
 * 1.日    期 : 2021年5月12日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
void hmac_mcast_ampdu_sta_add_multi_user(mac_vap_stru *mac_vap, uint8_t *mac_addr)
{
    uint8_t multi_addr[OAL_MAC_ADDRESS_LEN] = {0};
    if (mac_get_mcast_ampdu_switch() == OAL_FALSE) {
        return;
    }
    if (mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return;
    }
    hmac_mcast_ampdu_get_addr(multi_addr, OAL_MAC_ADDRESS_LEN, mac_addr);
    oam_warning_log4(0, OAM_SF_UM,
        "{hmac_mcast_ampdu_sta_add_multi_user::ucast user:mac addr:%02X:xx:xx:xx:%02X:%02X}",
        mac_addr[MAC_ADDR_0], mac_addr[MAC_ADDR_4], mac_addr[MAC_ADDR_5]);
    oam_warning_log4(0, OAM_SF_UM,
        "{hmac_mcast_ampdu_sta_add_multi_user::multi user:mac addr:%02X:xx:xx:xx:%02X:%02X}",
        multi_addr[MAC_ADDR_0], multi_addr[MAC_ADDR_4], multi_addr[MAC_ADDR_5]);
    hmac_mcast_ampdu_add_user(mac_vap, multi_addr, OAL_MAC_ADDRESS_LEN);
}
#endif
#endif  /* _PRE_MULTI_CORE_DPE_OFFLOAD */
