/*
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 * 功能说明 : dpe_tx_data.c
 * 创建日期 : 2025年1月4日
 */

#include "mac_data.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_host_ring_tx.h"
#include "hmac_tx_ring_alloc.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_tcp_ack_buf.h"
#endif
#include "hmac_tid_sche.h"
#include "hmac_tid_update.h"
#include "hmac_tid_ring_switch.h"
#include "dpe_wlan_vap.h"
#include "hd_trx_data.h"
#include "host_hal_dma.h"
#include "dpe_tx_data.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DPE_TX_DATA_C

#define oal_high_half_byte(a) (((a) & 0xF0) >> 4)
#define oal_low_half_byte(a)  ((a) & 0x0F)

wlan_trx_switch_stru g_dpe_trx_switch = { 0 };

void dpe_trx_switch_sync(wlan_trx_switch_stru *trx_switch)
{
    int32_t ret;
    ret = memcpy_s(&g_dpe_trx_switch, sizeof(wlan_trx_switch_stru), trx_switch, sizeof(wlan_trx_switch_stru));
    if (ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "{dpe_trx_switch_sync::memcpy fail}");
    }
}

static inline void hmac_host_ring_tx_unlock_ring(hmac_msdu_info_ring_stru *tx_ring)
{
    oal_mutex_unlock(&tx_ring->tx_lock);
    hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_HOST_RING_TX);
}

/*
 * host ring tx流控水线, ring中sk_buff数量:
 * 达到上水线(suspend)时, 停止mac_vap对应的netdev下发数据帧
 * 降低至下水线(resume)时, 恢复mac_vap对应的netdev下发数据帧
 */
static inline uint32_t hmac_ring_tx_suspend_th(hmac_tid_info_stru *tid_info)
{
    return (hal_host_tx_tid_ring_depth_get(tid_info->tx_ring.base_ring_info.size) * 7 / 8); // 7/8: 流控上水线(87.5%)
}

static inline uint32_t hmac_ring_tx_resume_th(hmac_tid_info_stru *tid_info)
{
    return (hal_host_tx_tid_ring_depth_get(tid_info->tx_ring.base_ring_info.size) * 1 / 2); // 1/2: 流控上水线(50%)
}

OAL_STATIC void hmac_host_ring_tx_suspend(dpe_wlan_vap_stru *dpe_vap, uint16_t user_idx, hmac_tid_info_stru *tid_info)
{
    dpe_wlan_user_stru *dpe_user = dpe_wlan_user_get(user_idx);
    if (dpe_vap == NULL || dpe_user == NULL) {
        return;
    }

    /* AP模式不能关net_dev队列，否则会导致其他用户无法发包 */
    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        return;
    }

    if (oal_atomic_read(&tid_info->tx_ring.msdu_cnt) < hmac_ring_tx_suspend_th(tid_info) ||
        oal_atomic_read(&dpe_user->netdev_tx_suspend)) {
        return;
    }

    if (oal_unlikely(dpe_vap->pst_net_device == NULL)) {
        return;
    }

    if (oal_atomic_read(&dpe_vap->tx_suspend_user_cnt) == 0) {
        oal_net_tx_stop_all_queues(dpe_vap->pst_net_device);
    }

    oal_atomic_inc(&dpe_vap->tx_suspend_user_cnt);
    oal_atomic_set(&dpe_user->netdev_tx_suspend, OAL_TRUE);
}

void hmac_host_ring_tx_resume(dpe_wlan_vap_stru *dpe_vap, dpe_wlan_user_stru *dpe_user, hmac_tid_info_stru *tid_info)
{
    if (dpe_user == NULL || dpe_vap == NULL) {
        return;
    }

    if (oal_atomic_read(&tid_info->tx_ring.msdu_cnt) > hmac_ring_tx_resume_th(tid_info) ||
        !oal_atomic_read(&dpe_user->netdev_tx_suspend)) {
        return;
    }

    if (oal_unlikely(dpe_vap->pst_net_device == NULL)) {
        return;
    }

    oal_atomic_dec(&dpe_vap->tx_suspend_user_cnt);
    if (oal_atomic_read(&dpe_vap->tx_suspend_user_cnt) == 0) {
        oal_net_tx_wake_all_queues(dpe_vap->pst_net_device);
    }

    oal_atomic_set(&dpe_user->netdev_tx_suspend, OAL_FALSE);
}

/*
 * 函 数 名  : oal_free_netbuf_list
 * 功能描述  : 释放一个netbuf链，此链中的skb或者都来自lan，或者都来自wlan
 * 1.日    期  : 2012年11月14日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_free_netbuf_list(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next = NULL;

    if (oal_unlikely(!netbuf)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_free_netbuf_list::netbuf is null}");
        return;
    }

    while (netbuf) {
        next = oal_netbuf_list_next(netbuf);
        oal_netbuf_free(netbuf);
        netbuf = next;
    }
}

#define MAC_DATA_DOUBLE_VLAN_MIN_LEN           22   /* 双vlan最小报文长度 */
#define MAC_DATA_DOUBLE_VLAN_ETHER_TYPE_OFFSET (ETHER_ADDR_LEN * 2 + sizeof(mac_vlan_tag_stru) * 2)
/*
 * 功能描述 : 设置VLAN以太帧的data type、snap cb字段
 * 1.日    期 : 2020年4月23日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_ether_with_double_vlan_tag(oal_netbuf_stru *netbuf, uint16_t ether_type)
{
    return (oal_netbuf_len(netbuf) >= MAC_DATA_DOUBLE_VLAN_MIN_LEN &&
           (ether_type == oal_host2net_short(ETHER_TYPE_VLAN_88A8) ||
           ether_type == oal_host2net_short(ETHER_TYPE_VLAN_9100) ||
           ether_type == oal_host2net_short(ETHER_TYPE_VLAN))) ? OAL_TRUE : OAL_FALSE;
}

/*
 * 功能描述 : 双vlan以太网报文的情况下, 返回第二个vlan的ether type
 * 1.日    期 : 2020年4月23日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE uint16_t hmac_ether_get_second_vlan_type(uint8_t *vlan_ether_hdr)
{
    return *(uint16_t *)(vlan_ether_hdr + MAC_DATA_DOUBLE_VLAN_ETHER_TYPE_OFFSET);
}

/*
 * 功能描述 : 设置VLAN以太帧的data type、snap cb字段
 * 1.日    期 : 2020年4月23日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_ether_set_vlan_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf)
{
    uint16_t ether_type = 0;
    oal_vlan_ethhdr_stru *vlan_ether_hdr = (oal_vlan_ethhdr_stru *)oal_netbuf_data(netbuf);
    if (vlan_ether_hdr == NULL) {
        oam_error_log0(0, 0, "hmac_ether_set_vlan_data_type:vlan_ether_hdr null");
        return;
    }
    ether_type = vlan_ether_hdr->h_vlan_encapsulated_proto;

    /* 单vlan以太网类型报文 */
    mac_get_cb_data_type(tx_ctl) = DATA_TYPE_1_VLAN_ETH;

    if (hmac_ether_with_double_vlan_tag(netbuf, ether_type) == OAL_TRUE) {
        /* 双vlan以太网报文 */
        mac_get_cb_data_type(tx_ctl) = DATA_TYPE_2_VLAN_ETH;

        /* 取第2个vlan后面的2字节用于802.3报文判断, DA|SA|VLAN|VLAN|TYPE(LEN)|DATA */
        ether_type = hmac_ether_get_second_vlan_type((uint8_t *)vlan_ether_hdr);
    }

    /* 判断是否是802.3报文 */
    mac_get_cb_is_802_3_snap(tx_ctl) = oal_net2host_short(ether_type) <= MAC_DATA_TYPE_INVALID_MIN_VALUE;
}

/*
 * 功能描述 : 设置其他以太帧的data type、snap cb字段
 * 1.日    期 : 2020年4月23日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_ether_set_other_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf)
{
    uint16_t ether_type = mac_get_cb_ether_type(tx_ctl);

    mac_get_cb_is_802_3_snap(tx_ctl) = oal_net2host_short(ether_type) <= MAC_DATA_TYPE_INVALID_MIN_VALUE;
}

/*
 * 功能描述 : vlan、其他报文类型检查与设置
 * 1.日    期 : 2020年4月23日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
void hmac_ether_set_data_type(mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf)
{
    /* 默认设置 */
    mac_get_cb_data_type(tx_ctl) = DATA_TYPE_ETH;

    switch (mac_get_cb_frame_subtype(tx_ctl)) {
        case MAC_DATA_VLAN:
            hmac_ether_set_vlan_data_type(tx_ctl, netbuf);
            break;
        case MAC_DATA_BUTT:
            hmac_ether_set_other_data_type(tx_ctl, netbuf);
            break;
        default:
            break;
    }

    /* 802.3格式 */
    if (mac_get_cb_is_802_3_snap(tx_ctl)) {
        mac_get_cb_data_type(tx_ctl) |= DATA_TYPE_802_3_SNAP;
    }
}
/*
 * 函 数 名  : oal_netbuf_is_tcp_ack
 * 功能描述  : 判断ipv4 tcp报文是否为tcp 关键帧，包括TCP ACK\TCP SYN帧等
 */
oal_bool_enum_uint8 oal_netbuf_is_tcp_ack(oal_ip_header_stru *ip_hdr)
{
    oal_tcp_header_stru *tcp_hdr = NULL;
    uint32_t ip_pkt_len;
    uint32_t ip_hdr_len;
    uint32_t tcp_hdr_len;

    if (oal_unlikely(ip_hdr == NULL)) {
        oal_warn_on(1);
        return OAL_FALSE;
    }

    tcp_hdr     = (oal_tcp_header_stru *)(ip_hdr + 1);
    ip_pkt_len  = oal_net2host_short(ip_hdr->us_tot_len);
    ip_hdr_len  = (oal_low_half_byte(ip_hdr->us_ihl)) << 2; /* 2 协议规范 */
    tcp_hdr_len = (oal_high_half_byte(tcp_hdr->uc_offset)) << 2; /* 2 协议规范 */
    if (tcp_hdr_len + ip_hdr_len == ip_pkt_len) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}
static void hmac_set_cb_is_ipv4_tcp(mac_ether_header_stru *ether_hdr, mac_tx_ctl_stru *tx_ctl)
{
    oal_ip_header_stru *ip_hdr = (oal_ip_header_stru *)(ether_hdr + 1);
    oal_tcp_header_stru *tcp_hdr = (oal_tcp_header_stru *)(ip_hdr + 1);

    if (ip_hdr->uc_protocol != MAC_TCP_PROTOCAL) {
        return;
    }

    if (oal_netbuf_is_tcp_ack(ip_hdr)) {
        mac_get_cb_is_tcp_ack(tx_ctl) = OAL_TRUE;

#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
        if (hmac_device_tcp_buf_duplicate_ack_proc(ip_hdr)) {
            mac_get_cb_urgent_tcp_ack(tx_ctl) = OAL_TRUE;
        }
#endif
    } else {
        mac_get_cb_is_tcp_data(tx_ctl) = OAL_TRUE;
    }

    if (tcp_hdr->uc_flags & FILTER_FLAG_MASK) {
        mac_get_cb_urgent_tcp_ack(tx_ctl) = OAL_TRUE;
    }
}
/*
 * 功能描述  : 判断ipv6 tcp报文是否为tcp ack
 */
oal_bool_enum_uint8 oal_netbuf_is_tcp_ack6(oal_ipv6hdr_stru *ipv6hdr)
{
    oal_tcp_header_stru *tcp_hdr = NULL;
    uint32_t ip_pkt_len;
    uint32_t tcp_hdr_len;

    if (oal_unlikely(ipv6hdr == NULL)) {
        oal_warn_on(1);
        return OAL_FALSE;
    }

    tcp_hdr = (oal_tcp_header_stru *)(ipv6hdr + 1);
    /* ipv6 净载荷, ipv6报文头部固定为40字节 */
    ip_pkt_len = oal_net2host_short(ipv6hdr->payload_len);
    tcp_hdr_len = (oal_high_half_byte(tcp_hdr->uc_offset)) << 2; /* 2 协议规范 */
    if (tcp_hdr_len == ip_pkt_len) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

static void hmac_set_cb_is_ipv6_tcp(mac_ether_header_stru *ether_hdr, mac_tx_ctl_stru *tx_ctl)
{
    oal_ipv6hdr_stru *ipv6_hdr = (oal_ipv6hdr_stru *)(ether_hdr + 1);

    if (ipv6_hdr->nexthdr != TCP_PROTOCAL) {
        return;
    }

    if (oal_netbuf_is_tcp_ack6(ipv6_hdr)) {
        mac_get_cb_ipv6_tcp_ack(tx_ctl) = OAL_TRUE;
    } else {
        mac_get_cb_is_tcp_data(tx_ctl) = OAL_TRUE;
    }
}

/*
 * 功能描述  :  初始化netbuf中CB字段中的is_tcp_ack和urgent_tcp_ack
 */
void hmac_set_cb_is_tcp(mac_tx_ctl_stru *tx_ctl, mac_ether_header_stru *ether_hdr, uint16_t ether_type)
{
    switch (ether_type) {
        case oal_host2net_short(ETHER_TYPE_IP):
            hmac_set_cb_is_ipv4_tcp(ether_hdr, tx_ctl);
            break;
        case oal_host2net_short(ETHER_TYPE_IPV6):
            hmac_set_cb_is_ipv6_tcp(ether_hdr, tx_ctl);
            break;
        default:
            break;
    }
}
/*
 * 功能描述  : 判断ipv4 是否为icmp报文
 * 输入参数  : pst_ip_hdr: IP报文头部
 */
oal_bool_enum_uint8 oal_netbuf_is_icmp(oal_ip_header_stru *ip_hdr)
{
    if (oal_unlikely(ip_hdr == NULL)) {
        oal_warn_on(1);
        return OAL_FALSE;
    }

    /* ICMP报文检查 */
    if (ip_hdr->uc_protocol == ICMP_PROTOCAL) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

void hmac_tx_cb_field_init(mac_tx_ctl_stru *tx_ctl, uint8_t uc_vap_id, uint8_t en_vap_mode,
    uint16_t user_idx, mac_ether_header_stru *ether_hdr)
{
    uint16_t ether_type;

    if (tx_ctl == NULL || ether_hdr == NULL) {
        return;
    }
    ether_type = ether_hdr->us_ether_type;
    if (en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        mac_get_cb_is_mcast(tx_ctl) = ether_is_multicast(ether_hdr->auc_ether_dhost);
    }

    if (ether_type == oal_host2net_short(ETHER_TYPE_IP)) {
        mac_get_cb_is_icmp(tx_ctl) = oal_netbuf_is_icmp((oal_ip_header_stru *)(ether_hdr + 1));
    }

    hmac_set_cb_is_tcp(tx_ctl, ether_hdr, ether_type);
    mac_get_cb_ether_type(tx_ctl) = ether_type;
    mac_get_cb_mpdu_num(tx_ctl) = 1;
    mac_get_cb_netbuf_num(tx_ctl) = 1;
    mac_get_cb_ack_policy(tx_ctl) = WLAN_TX_NORMAL_ACK;
    mac_get_cb_tx_vap_index(tx_ctl) = uc_vap_id;
    mac_get_cb_tx_user_idx(tx_ctl) = user_idx;
    /*  初始队列，非qos帧入VO队列 */
    mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_AC_VO;
    mac_get_cb_wme_tid_type(tx_ctl) = WLAN_TIDNO_VOICE;
    mac_get_cb_frame_type(tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
    mac_get_cb_frame_subtype(tx_ctl) = MAC_DATA_NUM; /* 后续识别模块刷新 */
    mac_get_cb_data_type(tx_ctl) = DATA_TYPE_80211;
}
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
/*
 * 功能描述 : 采取host tx时，组播帧处理
 * 1.日    期 : 2021年4月15日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
void hmac_mcast_ampdu_netbuf_process(dpe_wlan_vap_stru *mac_vap, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl,
    mac_ether_header_stru *eth_hdr, oal_bool_enum_uint8 *multicast_need_host_tx)
{
    hmac_user_stru *hmac_user;
    if (mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_AP) {
        return;
    }
    /* 单播数据不处理，走正常发送流程 */
    if (oal_likely(!mac_get_cb_is_mcast(tx_ctl))) {
        return;
    }

    /* 更新ACK策略 */
    mac_get_cb_ack_policy(tx_ctl) = WLAN_TX_NO_ACK;

    /* 获取组播用户 */
    hmac_user = mac_res_get_hmac_user(mac_vap->us_multi_user_idx);
    if (oal_unlikely(hmac_user == NULL)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_TX,
            "{hmac_mcast_ampdu_netbuf_process::get multi user failed[%d].}", mac_vap->us_multi_user_idx);
        return;
    }

    mac_get_cb_tx_user_idx(tx_ctl) = mac_vap->us_multi_user_idx;
    /* 组播数据用tid3发送 */
    mac_get_cb_wme_tid_type(tx_ctl) = WLAN_TIDNO_MCAST_AMPDU;
    mac_get_cb_wme_ac_type(tx_ctl) = WLAN_WME_TID_TO_AC(WLAN_TIDNO_MCAST_AMPDU);
    /* 设置支持host tx */
    *multicast_need_host_tx = OAL_TRUE;
}
#endif

static uint8_t hmac_tid_ring_tx_allowed(mac_tx_ctl_stru *tx_ctl)
{
    uint8_t tid = 0;
    hmac_tid_info_stru *tid_info = NULL;
    dpe_wlan_user_stru *dpe_user = NULL;
    if (tx_ctl == NULL) {
        oam_warning_log0(0, OAM_SF_TX, "{hmac_tid_ring_tx_allowed::tx_ctl NULL}");
        return OAL_FALSE;
    }
    tid = mac_get_cb_wme_tid_type(tx_ctl);
    dpe_user = dpe_wlan_user_get(mac_get_cb_tx_user_idx(tx_ctl));
    if (oal_unlikely(dpe_user == NULL)) {
        oam_warning_log0(0, OAM_SF_TX, "{hmac_tid_ring_tx_allowed::dpe_user NULL}");
        return OAL_FALSE;
    }

    /* 不支持组播聚合的组播用户预期没有ring，直接返回 */
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    if ((dpe_user->en_is_multi_user == OAL_TRUE) && (dpe_user->is_mcast_ampdu == OAL_FALSE)) {
#else
    if (dpe_user->en_is_multi_user == OAL_TRUE) {
#endif
        return OAL_FALSE;
    }

    tid_info = hmac_get_tx_tid_info(dpe_user->us_assoc_id, tid);
    if (tid_info == NULL) {
        return OAL_FALSE;
    }

    return hmac_tid_info_inited(tid_info);
}

/*
 * 功能描述 : netbuf是否采用ring tx模式
 * 1.日    期 : 2020年4月13日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
oal_bool_enum_uint8 dpe_is_ring_tx(uint8_t vapid, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl)
{
    mac_ether_header_stru *eth_hdr = NULL;
    oal_bool_enum_uint8 need_ring_tx = OAL_TRUE;
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vapid);
    if (dpe_vap == NULL) {
        return OAL_FALSE;
    }
    if (dpe_ring_tx_enabled() != OAL_TRUE) {
        return OAL_FALSE;
    }

    if (!hmac_tid_ring_tx_allowed(tx_ctl)) {
        return OAL_FALSE;
    }

    eth_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (dpe_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        /* AP模式的单播帧支持ring tx, 组播帧不支持ring tx; STA模式没有组播帧 */
        need_ring_tx = !mac_get_cb_is_mcast(tx_ctl);
    }
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    /* 如果组播聚合打开，则组播数据需要走ring tx流程，并走tid6 */
    if (mac_get_mcast_ampdu_switch() == OAL_TRUE) {
        hmac_mcast_ampdu_netbuf_process(dpe_vap, netbuf, tx_ctl, eth_hdr, &need_ring_tx);
    }
#endif
    return (mac_get_cb_wme_tid_type(tx_ctl) != WLAN_TIDNO_BCAST) && need_ring_tx;
}

/*
 * 功能描述 : 通过ring发送数据帧
 * 1.日    期 : 2020年2月17日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
uint32_t hmac_host_ring_tx(dpe_wlan_vap_stru *dpe_vap, uint16_t user_idx, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next_buf = NULL;
    uint8_t tid_no;
    hmac_msdu_info_ring_stru *tx_ring = NULL;
    hmac_tid_info_stru *tid_info = NULL;
    if (netbuf == NULL) {
        return OAL_FAIL;
    }
    tid_no = mac_get_cb_wme_tid_type((mac_tx_ctl_stru *)oal_netbuf_cb(netbuf));
    tid_info = hmac_get_tx_tid_info(user_idx, tid_no);
    if (tid_info == NULL) {
        return OAL_FAIL;
    }
    tx_ring = &tid_info->tx_ring;
    hal_pm_try_wakeup_forbid_sleep(HAL_PM_FORBIDE_SLEEP_HOST_RING_TX);
    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        hal_pm_try_wakeup_allow_sleep(HAL_PM_FORBIDE_SLEEP_HOST_RING_TX);
        return OAL_FAIL;
    }

    oal_mutex_lock(&tx_ring->tx_lock);

    if (oal_atomic_read(&tid_info->ring_tx_mode) != HOST_RING_TX_MODE) {
        hmac_host_ring_tx_unlock_ring(tx_ring);
        oam_warning_log3(0, 0, "{hmac_host_ring_tx::usr[%d] tid[%d] mode[%d]}",
            tid_info->user_index, tid_info->tid_no, oal_atomic_read(&tid_info->ring_tx_mode));
        return OAL_FAIL;
    }

    if (!hmac_tx_ring_alloced(tx_ring) && hmac_alloc_tx_ring_after_inited(tid_info, tx_ring) != OAL_SUCC) {
        hmac_host_ring_tx_unlock_ring(tx_ring);
        hmac_tid_info_deinit(tid_info);
        return OAL_FAIL;
    }

    if (dpe_tx_ring_switch_enabled() == OAL_TRUE) {
        hmac_tid_ring_switch_list_enqueue(tid_info);
    }

    while (netbuf != NULL) {
        next_buf = oal_netbuf_next(netbuf);
        if (hmac_tx_ring_push_msdu(dpe_vap, tx_ring, netbuf) != OAL_SUCC) {
            /*
             * 1. 流量高时一次性处理大量netbuf, 可能会使tx ring table写指针更新不及时, 导致MAC无帧可发但软件ring满的情况
             * 此时需要强制更新一次写指针, 加速MAC消耗ring中数据帧, 提升发送效率, 降低入ring失败带来的无效MIPS开销
             * 2. 此处返回fail一定是netbuf未进行dma map或已unmap的情况, 可直接释放
             *
             */
            hmac_tx_reg_write_ring_info(tid_info, TID_CMDID_ENQUE, OAL_FALSE);
            oam_warning_log0(0, OAM_SF_TX, "{hmac_host_ring_tx::tx ring push fail}");
            hal_host_tx_unmap_netbuf_list(netbuf);
            hmac_free_netbuf_list(netbuf);
            break;
        }

        netbuf = next_buf;
    }

    hmac_host_ring_tx_suspend(dpe_vap, user_idx, tid_info);

    hmac_host_ring_tx_unlock_ring(tx_ring);
    return OAL_SUCC;
}

static uint32_t hmac_host_tx_tid_enqueue_nolock(hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next_buf = NULL;

    if (!hmac_tid_info_inited(tid_info)) {
        oam_warning_log1(0, 0, "{hmac_host_tx_tid_enqueue_nolock::tid[%d] not inited}", tid_info->tid_no);
        return OAL_FAIL;
    }
#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
    if (hmac_tid_check_queue_overflow() == OAL_TRUE) {
        return OAL_FAIL;
    }
#endif
    while (netbuf != NULL) {
        next_buf = oal_netbuf_next(netbuf);
        hmac_tid_netbuf_enqueue(&tid_info->tid_queue, netbuf);
#ifdef _PRE_WLAN_FEATURE_TID_DEPTH_CTRL
        hmac_tid_queue_depth_increase(1);
#endif
#ifdef _PRE_WLAN_FEATURE_TX_STAT
        tid_info->tx_stat.enqueue_msdu_cnt++;
#endif
        netbuf = next_buf;
    }

    if ((hmac_tid_sche_allowed(tid_info) == OAL_TRUE) && oal_likely(!hmac_tx_ring_switching(tid_info))) {
        hmac_tid_schedule();
    }

    return OAL_SUCC;
}

/*
 * 功能描述 : netbuf入tid缓存队列, 并将对应的user tid结构体挂给tid调度管理结构体等待调度
 * 1.日    期 : 2020年7月29日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
uint32_t hmac_host_tx_tid_enqueue(hmac_tid_info_stru *tid_info, oal_netbuf_stru *netbuf)
{
    uint32_t ret;

    if (tid_info == NULL) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_host_tx_tid_enqueue::tid_info null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    oal_spin_lock_bh(&tid_info->lock);
    ret = hmac_host_tx_tid_enqueue_nolock(tid_info, netbuf);
    oal_spin_unlock_bh(&tid_info->lock);

    return ret;
}

OAL_STATIC uint32_t hmac_host_tx_data_prepare(oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (oal_unlikely(tx_ctl == NULL)) {
        return OAL_FAIL;
    }
    if (oal_unlikely(hal_host_tx_clear_msdu_padding(netbuf) != OAL_SUCC)) {
        oam_error_log0(0, OAM_SF_TX, "hal_host_tx_clear_msdu_padding fail");
        return OAL_FAIL;
    }
    if (oal_unlikely(hmac_tx_netbuf_expand_headroom(netbuf, tx_ctl) != OAL_SUCC)) {
        oam_error_log0(0, OAM_SF_TX, "hmac_tx_netbuf_expand_headroom fail");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

uint32_t hmac_host_tx_data(uint8_t vap_idx, uint16_t user_idx, oal_netbuf_stru *netbuf)
{
    uint32_t ret = OAL_FAIL;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    uint8_t tid;
    mac_tx_ctl_stru *tx_ctl = NULL;
#else
    dpe_wlan_vap_stru *dpe_vap = NULL;
#endif

    if (oal_unlikely(netbuf == NULL)) {
        return OAL_FAIL;
    }
    /* 对netbuf payload调整的动作在这里完成，后续入ring的netbuf不允许再操作payload */
    if (oal_unlikely(hmac_host_tx_data_prepare(netbuf) != OAL_SUCC)) {
        return OAL_FAIL;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (oal_likely(hmac_get_tid_schedule_list()->tid_schedule_thread)) {
        tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (tx_ctl == NULL) {
            oam_error_log0(0, OAM_SF_TX, "{hmac_host_tx_data::tx_ctl NULL}");
            return ret;
        }
        tid = mac_get_cb_wme_tid_type(tx_ctl);
        ret = hmac_host_tx_tid_enqueue(hmac_get_tx_tid_info(user_idx, tid), netbuf);

        host_end_record_performance(1, TX_XMIT_PROC);
    }

    return ret;
#else
    dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_host_tx_data::dpe_vap NULL}");
        return OAL_FAIL;
    }
    /* 入ring前进行map操作，后续不再进行 */
    ret = hal_host_ring_tx_single_map(netbuf);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hal_host_ring_tx_single_map::fail}");
        return OAL_FAIL;
    }

    return hmac_host_ring_tx(dpe_vap, user_idx, netbuf);
#endif
}
/*
 * 函 数 名  : dpe_tx_get_user_id
 * 功能描述  : tx方向获取dpe user id
 * 日    期  : 2025年02月14日
 */
uint16_t dpe_tx_get_user_id(uint8_t vap_id, mac_ether_header_stru *ether_hdr)
{
    dpe_wlan_vap_stru   *dpe_vap = NULL;
    uint16_t user_idx = g_wlan_spec_cfg->invalid_user_id;
    uint8_t *addr = NULL;                     /* 目的地址 */
    uint32_t ret;

    dpe_vap = dpe_wlan_vap_get(vap_id);
    if (dpe_vap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        return dpe_vap->us_assoc_vap_id;
    }

    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        if (ether_hdr == NULL) {
            oam_error_log0(dpe_vap->uc_vap_id, OAM_SF_TX, "{dpe_tx_get_user_id::ether_hdr null}");
            return g_wlan_spec_cfg->invalid_user_id;
        }
        /* 判断是组播或单播,对于lan to wlan的单播帧，返回以太网地址 */
        addr = ether_hdr->auc_ether_dhost;
        /* 单播数据帧 */
        if (oal_likely(!ether_is_multicast(addr))) {
            ret = dpe_vap_find_user_by_macaddr(vap_id, addr, &user_idx);
            if (oal_unlikely(ret != OAL_SUCC)) {
                oam_warning_log3(dpe_vap->uc_vap_id, OAM_SF_TX, "{dpe_tx_get_user_id::dpe_tx_find_user failed \
                    %02X:XX:XX:XX:%02X:%02X}", addr[MAC_ADDR_0], addr[MAC_ADDR_4], addr[MAC_ADDR_5]);
                return g_wlan_spec_cfg->invalid_user_id;
            }
        } else { /* 组播 or 广播 */
            user_idx = dpe_vap->us_multi_user_idx;
        }
    }

    return user_idx;
}
