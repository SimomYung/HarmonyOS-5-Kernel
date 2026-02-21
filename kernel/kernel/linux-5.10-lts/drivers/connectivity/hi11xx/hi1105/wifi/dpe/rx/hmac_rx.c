/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : rx方向公共处理流程
 * 作    者 : wifi
 * 创建日期 : 2024年12月13日
 */

/* 头文件包含 */
#include "hmac_rx.h"
#include "mac_data.h"
#include "mac_frame_if.h"
#ifndef _PRE_MULTI_CORE_DPE_OFFLOAD
#include "hmac_rx_defrag.h"
#include "hmac_11i.h"
#include "hmac_wapi.h"
#include "hmac_ht_self_cure.h"
#include "hmac_tx_amsdu.h"
#include "hmac_ota_report.h"
#include "hmac_mgmt_sta_up.h"
#include "hmac_wifi_delay.h"
#include "hmac_mintp_log.h"
#include "oam_event_wifi.h"
#endif
#include "hmac_rx_reorder.h"
#include "dpe_wlan_resource.h"
#include "dpe_wlan_stat.h"
#include "host_hal_ext_if.h"
#include "hmac_pm.h"
#include "hmac_stat.h"
#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
#include "hmac_mcast_ampdu.h"
#endif
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_ps.h"
#endif
#ifdef _PRE_WLAN_FEATURE_PWL
#include "hmac_pwl.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_C

/*
 * 功能描述  : 释放netbuf链
 * 1.日    期  : 2020年03月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_netbuf_list_free(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next = NULL;

    while (netbuf != NULL) {
        next = oal_netbuf_next(netbuf);
        oal_netbuf_free(netbuf);
        netbuf = next;
    }
}

/*
 * 函 数 名  : hmac_rx_free_netbuf_list
 * 功能描述  : 释放指定个数的netbuf
 * 1.日    期  : 2012年12月6日
 *   修改内容  : 新生成函数
 */
void hmac_rx_free_netbuf(oal_netbuf_stru *netbuf, uint16_t nums)
{
    oal_netbuf_stru *netbuf_temp = NULL;
    uint16_t us_netbuf_num;

    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_free_netbuf::netbuf null.}\r\n");
        return;
    }

    for (us_netbuf_num = nums; us_netbuf_num > 0; us_netbuf_num--) {
        netbuf_temp = oal_netbuf_next(netbuf);

        /* 减少netbuf对应的user引用计数 */
        oal_netbuf_free(netbuf);

        netbuf = netbuf_temp;

        if (netbuf == NULL) {
            if (oal_unlikely(us_netbuf_num != 1)) {
                oam_error_log2(0, OAM_SF_RX, "{hmac_rx_free_netbuf::netbuf list broken, us_netbuf_num[%d]nums[%d].}",
                    us_netbuf_num, nums);
                return;
            }
            break;
        }
    }
}

/*
 * 函 数 名  : hmac_rx_free_netbuf_list
 * 功能描述  : for list
 * 1.日    期  : 2015年1月3日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_free_netbuf_list(oal_netbuf_head_stru *netbuf_hdr, uint16_t num_buf)
{
    oal_netbuf_stru *netbuf = NULL;
    uint16_t us_idx;

    if (oal_unlikely(netbuf_hdr == NULL)) {
        oam_info_log0(0, OAM_SF_RX, "{hmac_rx_free_netbuf_list::netbuf null.}");
        return;
    }

    oam_info_log1(0, OAM_SF_RX, "{hmac_rx_free_netbuf_list::free [%d].}", num_buf);

    for (us_idx = num_buf; us_idx > 0; us_idx--) {
        netbuf = oal_netbuf_delist(netbuf_hdr);
        if (netbuf != NULL) {
            oam_info_log0(0, OAM_SF_RX, "{hmac_rx_free_netbuf_list::netbuf null.}");
            oal_netbuf_free(netbuf);
        }
    }
}

uint32_t g_wifi_trx_log = 0;
void hmac_rx_route_print(hal_host_device_stru *hal_device, uint8_t step, mac_rx_ctl_stru *mac_rx_ctl)
{
    if (g_wifi_trx_log & HAL_LOG_RX_ROUTE_LOG) {
        oam_warning_log4(hal_device->device_id, OAM_SF_RX,
            "{rx_route_print::rx step[%d],tid[%d],process_flag[%d],seqnum[%d]}",
            step, mac_rx_ctl->bit_rx_tid, mac_rx_ctl->bit_process_flag, mac_rx_ctl->us_seq_num);
    }
}

OAL_STATIC void hmac_skb_info_dump(oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (cb == NULL) {
        return;
    }

    oam_warning_log3(0, 0, "{hmac_skb_info_dump::mac_vap_id[%d] hal_vap_id[%d] lut_user_id[%d]}",
        mac_get_rx_cb_mac_vap_id(cb), mac_get_rx_cb_hal_vap_id(cb), cb->bit_rx_user_id);
    oam_warning_log2(0, 0, "{hmac_skb_info_dump::amsdu[%d] first_msdu[%d]}",
        mac_rxcb_is_amsdu(cb), mac_get_rx_cb_is_first_sub_msdu(cb));

    oal_print_hex_dump((uint8_t *)cb, sizeof(mac_rx_ctl_stru), HEX_DUMP_GROUP_SIZE, "rx cb:");
}

OAL_STATIC void __hmac_hal_vap_info_dump(hal_host_device_stru *hal_device)
{
    uint8_t hal_vap_id;
    hal_vap_status_info *info = NULL;

    for (hal_vap_id = 0; hal_vap_id < dpe_mac_chip_get_hal_max_vap_num(); hal_vap_id++) {
        info = &hal_device->hal_vap_sts_info[hal_vap_id];
        oam_warning_log3(0, 0, "{__hmac_hal_vap_info_dump::hal_vap_id[%d], mac_vap_id[%d] hal_vap_valid[%d]}",
            hal_vap_id, info->mac_vap_id, info->hal_vap_valid);
    }
}

OAL_STATIC void hmac_hal_vap_info_dump(void)
{
    uint8_t hal_device_id;
    uint8_t hal_dev_num = mac_chip_get_hal_dev_num();

    for (hal_device_id = 0; hal_device_id < hal_dev_num; hal_device_id++) {
        __hmac_hal_vap_info_dump(hal_get_host_device(hal_device_id));
    }
}

OAL_STATIC void hmac_mac_vap_info_dump(dpe_wlan_vap_stru *dpe_vap)
{
    oam_warning_log2(0, 0, "{hmac_mac_vap_info_dump::mac_vap_id[%d] device_id[%d]}",
        dpe_vap->uc_vap_id, dpe_vap->uc_device_id);
}

OAL_STATIC void hmac_rx_enqueue_vap_abnormal_handle(oal_netbuf_stru *netbuf, dpe_wlan_vap_stru *dpe_vap)
{
    if (dpe_vap != NULL) {
        hmac_mac_vap_info_dump(dpe_vap);
    }
    hmac_skb_info_dump(netbuf);
    hmac_hal_vap_info_dump();
}

void hmac_rx_skb_netif_rpt(oal_net_device_stru *net_dev, oal_netbuf_stru *netbuf)
{
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
    /* DPE在DPA上转发 */
#else
    if (net_dev == NULL) {
        oal_netbuf_free(netbuf);
        return;
    }
    /* 对protocol模式赋值, pull ETH_HLEN至payload */
    oal_netbuf_protocol(netbuf) = oal_eth_type_trans(netbuf, net_dev);

    /* 接收线程使能则提交接收线程处理,线程决定直接传递orNAPI处理 */
    if (hmac_get_rxthread_enable() == OAL_TRUE) {
        hmac_rxdata_netbuf_enqueue(netbuf);
        return;
    }

#ifdef _PRE_DELAY_DEBUG
    hmac_wifi_delay_rx_notify(net_dev, netbuf);
#endif
    /* 清空cb */
#ifdef _PRE_WLAN_PKT_TIME_STAT
    memset_s(oal_netbuf_cb(netbuf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
    memset_s(oal_netbuf_cb(netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif

#ifdef _PRE_WLAN_FEATURE_ESPE
    hmac_espe_recv(netbuf);
#else
    oal_netif_rx_ni(netbuf);
#endif
#endif
}

/*
 * 功能描述  : 通用统计处理流程
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_host_rx_static_proc(hmac_host_rx_context_stru *rx_context)
{
    dpe_wlan_vap_stru *dpe_vap = rx_context->dpe_vap;
    oal_netbuf_stru *netbuf = rx_context->netbuf;
    mac_rx_ctl_stru *rx_ctl = NULL;

    for (;netbuf != NULL; netbuf = oal_netbuf_next(netbuf)) {
        hmac_stat_vap_rx_netbuf(dpe_vap->uc_vap_id, netbuf);
        hmac_stat_device_rx_msdu(oal_netbuf_len(netbuf));
        rx_ctl = (mac_rx_ctl_stru*)oal_netbuf_cb(netbuf);
        hmac_stat_user_rx_netbuf(mac_res_get_hmac_user(rx_ctl->bit_rx_user_id), netbuf);
    }

    if (rx_context->cb->bit_mcast_bcast == OAL_FALSE) {
        oal_atomic_inc(&(dpe_vap->rx_unicast_pkt_to_lan));
    }
}

/*
 * 函 数 名  : hmac_rx_copy_netbuff
 * 1.日    期  : 2015年1月3日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_rx_copy_netbuff(oal_netbuf_stru **ppst_dest_netbuf,
    oal_netbuf_stru *pst_src_netbuf, uint8_t uc_vap_id, mac_ieee80211_frame_stru **ppul_mac_hdr_start_addr)
{
    mac_rx_ctl_stru *pst_rx_ctrl = NULL;
    int32_t l_ret;

    if (pst_src_netbuf == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    *ppst_dest_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (oal_unlikely(*ppst_dest_netbuf == NULL)) {
        oam_warning_log0(uc_vap_id, OAM_SF_RX, "{hmac_rx_copy_netbuff::netbuf_copy null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 信息复制 */
    l_ret = memcpy_s(oal_netbuf_cb(*ppst_dest_netbuf), sizeof(mac_rx_ctl_stru),
                     oal_netbuf_cb(pst_src_netbuf), sizeof(mac_rx_ctl_stru));  // modify src bug
    l_ret += memcpy_s(oal_netbuf_data(*ppst_dest_netbuf), oal_netbuf_len(pst_src_netbuf),
                      oal_netbuf_data(pst_src_netbuf), oal_netbuf_len(pst_src_netbuf));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_RX, "hmac_rx_copy_netbuff::memcpy fail!");
        oal_netbuf_free(*ppst_dest_netbuf);
        return OAL_FAIL;
    }

    /* 设置netbuf长度、TAIL指针 */
    oal_netbuf_put(*ppst_dest_netbuf, oal_netbuf_get_len(pst_src_netbuf));

    /* 调整MAC帧头的指针copy后，对应的mac header的头已经发生变化) */
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(*ppst_dest_netbuf);
    mac_get_rx_cb_mac_header_addr(pst_rx_ctrl) = (uint64_t)(uintptr_t)oal_netbuf_data(*ppst_dest_netbuf);
    *ppul_mac_hdr_start_addr = (mac_ieee80211_frame_stru *)oal_netbuf_data(*ppst_dest_netbuf);

    return OAL_SUCC;
}

OAL_STATIC uint8_t *hmac_rx_get_eth_da(oal_netbuf_stru *netbuf)
{
    mac_ether_header_stru *p_eth_hdr = NULL;

    p_eth_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (p_eth_hdr == NULL) {
        return NULL;
    }
    return (uint8_t *)p_eth_hdr->auc_ether_dhost;
}

OAL_STATIC uint8_t hmac_rx_da_is_in_same_bss(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl, uint8_t vap_idx)
{
    dpe_wlan_user_stru *dpe_user = NULL;
    uint16_t user_idx = 0;
    uint8_t *puc_da = hmac_rx_get_eth_da(netbuf);

    if (dpe_vap_find_user_by_macaddr(vap_idx, puc_da, &user_idx) != OAL_SUCC) {
        oam_info_log0(0, 0, "{hmac_rx_da_is_in_same_bss::find da fail}");
        return OAL_FALSE;
    }

    dpe_user = dpe_wlan_user_get(user_idx);
    if (dpe_user == NULL) {
        oam_error_log1(0, 0, "{hmac_rx_da_is_in_same_bss::find da fail usr idx:%d}", user_idx);
        return OAL_FALSE;
    }

    if (dpe_user->en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        hmac_mgmt_send_deauth_frame(vap_idx, dpe_user->auc_user_mac_addr, MAC_NOT_AUTHED, OAL_FALSE);
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

/*
 * 功能描述  : AP转发逻辑，仅处理单个netbuf
 * 1.日    期  : 2020年03月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_ap_w2w_proc(hmac_host_rx_context_stru *rx_context,
    oal_netbuf_head_stru *release_list, oal_netbuf_head_stru *w2w_list)
{
    oal_netbuf_stru          *netbuf = NULL;
    mac_rx_ctl_stru          *rx_ctl = NULL;
    mac_ieee80211_frame_stru *copy_frame_hdr = NULL;
    oal_netbuf_stru          *netbuf_copy = NULL;
    dpe_wlan_vap_stru        *dpe_vap = rx_context->dpe_vap;
    oal_netbuf_stru          *next_netbuf = NULL;
    uint8_t                  *frame_ctl = NULL;

    oal_netbuf_search_for_each_safe(netbuf, next_netbuf, release_list) {
        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (rx_ctl == NULL) {
            continue;
        }
        frame_ctl = (uint8_t *)&rx_ctl->us_frame_control;
        /* 四地址amsdu不进行转发，上报协议栈解析后再转发 */
        if ((mac_is_4addr((mac_ieee80211_frame_stru *)frame_ctl) == OAL_TRUE) && rx_ctl->bit_amsdu_enable) {
            continue;
        }
        if ((hmac_rx_get_eth_da(netbuf) != NULL) &&
            (ether_is_multicast(hmac_rx_get_eth_da(netbuf)) == OAL_TRUE)) {
            /* copy_frame_hdr复用mp13接口，mp16不用 */
            if (hmac_rx_copy_netbuff(&netbuf_copy, netbuf, rx_ctl->uc_mac_vap_id, &copy_frame_hdr) != OAL_SUCC) {
                continue;
            }
            oal_netbuf_add_to_list_tail(netbuf_copy, w2w_list);
        } else if (hmac_rx_da_is_in_same_bss(netbuf, rx_ctl, dpe_vap->uc_vap_id) == OAL_TRUE) {
            /* 目的用户已在AP的用户表中，进行WLAN_TO_WLAN转发 */
            oal_netbuf_delete(netbuf, release_list);
            oal_netbuf_add_to_list_tail(netbuf, w2w_list);
        } else {
            /* 该帧呆在链表里面 等待to lan */
            oam_info_log1(0, OAM_SF_RX, "{hmac_rx_host_ring_ap_proc::dst_is_valid[%d]}", rx_ctl->bit_dst_is_valid);
            oam_info_log4(0, OAM_SF_RX, "{hmac_rx_host_ring_ap_proc::band[%d] dst[%d] vap[%d]dst[%d]}",
                rx_ctl->bit_band_id, rx_ctl->bit_dst_band_id, rx_ctl->bit_vap_id, rx_ctl->dst_hal_vap_id);
        }
    }
}

/*
 * 功能描述  : host rx接收方向去分片流程封装
 * 1.日    期  : 2020.5.21
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC oal_netbuf_stru *hmac_rx_defrag(hmac_host_rx_context_stru *rx_context)
{
    dpe_wlan_user_stru *dpe_user = rx_context->dpe_user;
    oal_netbuf_stru *netbuf = rx_context->netbuf;
    mac_rx_ctl_stru *rx_ctl = rx_context->cb;

    if ((rx_ctl->bit_amsdu_enable == OAL_TRUE) ||
        (rx_ctl->bit_frame_format != MAC_FRAME_TYPE_80211) ||
        dpe_user == NULL) {
        return netbuf;
    }
    return hmac_defrag_process(dpe_user->us_assoc_id, netbuf, rx_ctl->uc_mac_header_len);
}

/*
 * 功能描述  : rx_context上下文中msdu转化成8023
 * 1.日    期  : 2021年1月4日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_rx_context_prepare_msdu_list_to_eth(hmac_host_rx_context_stru *rx_context)
{
    mac_rx_ctl_stru *rx_ctrl = NULL;
    oal_netbuf_stru *netbuf = rx_context->netbuf;
    oal_netbuf_stru *next_netbuf = NULL;
    oal_netbuf_head_stru netbuf_head;
    uint8_t data_type;
    if (netbuf == NULL) {
        return OAL_FAIL;
    }

    oal_netbuf_head_init(&netbuf_head);
    while (netbuf != NULL) {
        next_netbuf = netbuf->next;
        rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (rx_ctrl->bit_frame_format == MAC_FRAME_TYPE_80211) {
            /* 80211转换成8023 */
            if (hmac_rx_proc_80211_to_eth(netbuf, rx_ctrl, rx_context) != OAL_SUCC) {
                oal_netbuf_free(netbuf);
                netbuf = next_netbuf;
                continue;
            }
        }

        data_type = rx_ctrl->bit_data_type;
        if (data_type <= MAC_DATA_ARP_REQ) {
            hmac_rx_vip_info_proc(rx_context->dpe_vap, data_type, netbuf, rx_ctrl);
        } else if (data_type == MAC_DATA_RTSP) {
            oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_vip_info_proc::rtsp");
        }
        oal_netbuf_add_to_list_tail(netbuf, &netbuf_head);

        netbuf = next_netbuf;
    }

    if (oal_netbuf_list_empty(&netbuf_head)) {
        rx_context->netbuf = NULL;
        return OAL_FAIL;
    }

    /* 将最后一个NETBUF置空 */
    oal_set_netbuf_next((oal_netbuf_tail(&netbuf_head)), NULL);
    rx_context->netbuf = oal_netbuf_peek(&netbuf_head);

    return OAL_SUCC;
}

/*
 * 功能描述  : 将netbuf链加入netbuf_head结构
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_rx_netbuf_join_to_list(oal_netbuf_stru *netbuf, oal_netbuf_head_stru *list)
{
    oal_netbuf_stru *next = NULL;

    while (netbuf != NULL) {
        next = oal_netbuf_next(netbuf);
        oal_netbuf_add_to_list_tail(netbuf, list);
        netbuf = next;
    }
}

/*
 * 功能描述  : 将接收上下文中的netbuf链加入链表尾部
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_rx_context_netbuf_add_to_list(hmac_host_rx_context_stru *rx_context,
    oal_netbuf_head_stru *list, uint8_t buff_is_valid)
{
    oal_netbuf_stru *netbuf = rx_context->netbuf;

    if (buff_is_valid) {
        hmac_rx_netbuf_join_to_list(netbuf, list);
    }
    rx_context->netbuf = NULL;
}

/*
 * 函 数 名  : hmac_rx_is_qos_data_frame
 * 功能描述  : 判断是否是qos帧
 * 1.日    期  : 2020年5月18日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC oal_bool_enum_uint8 hmac_rx_is_qos_data_frame(mac_rx_ctl_stru *rx_ctl)
{
    uint8_t frame_type = mac_get_frame_type_and_subtype((uint8_t *)&rx_ctl->us_frame_control);

    return (frame_type != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) ? OAL_FALSE : OAL_TRUE;
}

/*
 * 函 数 名   : hmac_rx_qos_data_need_drop
 * 功能描述   : 在未建ba场景下，软件检查qos data是否重复帧，此时硬件无法识别，sn 重复过滤，不重复更新sn
 * 1.日    期   : 2020-05-11
 *   作    者   : wifi
 */
OAL_STATIC oal_bool_enum_uint8 hmac_rx_qos_data_need_drop(hmac_host_rx_context_stru *rx_context)
{
    mac_rx_ctl_stru     *rx_ctl = rx_context->cb;
    dpe_wlan_user_stru *dpe_user = rx_context->dpe_user;
    hmac_tid_ba_stru       *hmac_tid = NULL;
    hmac_ba_rx_stru     *ba_rx_hdl = NULL;
    uint16_t             seq_frag_num;
    uint8_t              rx_tid;

    if (!hmac_rx_is_qos_data_frame(rx_ctl)) {
        return OAL_FALSE;
    }
    rx_tid = mac_get_rx_cb_tid(rx_ctl);
    if (!mac_tid_is_valid(rx_tid)) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_qos_data_need_drop:: tid invalid[%d]}", rx_tid);
        return OAL_TRUE;
    }

    /* qos 已建ba返回 */
    hmac_tid = hmac_get_tid_ba_info(dpe_user->us_assoc_id, rx_tid);
    if (hmac_tid == NULL) {
        return OAL_TRUE;
    }
    ba_rx_hdl = hmac_tid->pst_ba_rx_info;
    if (ba_rx_hdl != NULL && (ba_rx_hdl->en_ba_status == DMAC_BA_COMPLETE)) {
        return OAL_FALSE;
    }

    if (rx_ctl->bit_is_fragmented) {
        seq_frag_num = ((uint16_t)(rx_ctl->us_seq_num) << 4) | rx_ctl->bit_fragment_num; /* 合并sn放高4位 */
    } else {
        seq_frag_num = rx_ctl->us_seq_num;
    }

    /* no ba qos data，retry 且seq num相同则过滤，否则更新 */
    if (((mac_header_frame_control_stru *)&rx_ctl->us_frame_control)->bit_retry == OAL_TRUE &&
        (seq_frag_num == hmac_tid->qos_last_seq_frag_num)) {
        oam_warning_log2(dpe_user->uc_vap_id, OAM_SF_RX,
            "{hmac_rx_qos_data_need_drop::tid is[%d],qos duplicate frame[%d].}", rx_tid, seq_frag_num);
        return OAL_TRUE;
    }

    hmac_tid->qos_last_seq_frag_num = seq_frag_num;
    return OAL_FALSE;
}

/*
 * 功能描述  : 获取netbuf对应以太帧类型,兼容80211及8023
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
#define ETHER_HEADER_ETH_TYPE_OFFSET 12
#define SNAP_HEADER_ETH_TYPE_OFFSET 6
#define ETH8023_HEADER_LLC_TYPE_OFFSET 20
uint16_t hmac_rx_netbuf_get_ether_type(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl)
{
    uint16_t frame_formate;
    uint8_t *header = NULL;
    uint16_t ether_type = 0;
    uint16_t netbuf_len;

    frame_formate = rx_ctl->bit_frame_format;
    header = oal_netbuf_data(netbuf);
    netbuf_len = oal_netbuf_len(netbuf);

    if (header == NULL) {
        return ether_type;
    }
    if (frame_formate == MAC_FRAME_TYPE_80211) {
        ether_type = *(uint16_t *)(header + rx_ctl->uc_mac_header_len + SNAP_HEADER_ETH_TYPE_OFFSET);
    } else if (frame_formate == MAC_FRAME_TYPE_RTH) {
        ether_type = *(uint16_t *)(header + ETHER_HEADER_ETH_TYPE_OFFSET);
    } else if (frame_formate == MAC_FRAME_TYPE_8023) {
        /*
        若芯片所收帧LLC Header不是AA-AA-03-00-00-00，上报的802.3帧格式为：
        |    DA   |    SA   | Length  | LLC Hdr |LLC Type |   Data   |
        | 6 Bytes | 6 Bytes | 2 Bytes | 6 Bytes | 2 Bytes | Variable |
        若芯片所收帧LLC Header是AA-AA-03-00-00-00，上报的802.3帧格式为：
        |    DA   |    SA   |LLC Type |   Data   |
        | 6 Bytes | 6 Bytes | 2 Bytes | Variable |
        */
        if (netbuf_len < ETHER_HEADER_ETH_TYPE_OFFSET + sizeof(uint16_t)) {
            return ether_type;
        }
        if (netbuf_len == ETHER_HEADER_ETH_TYPE_OFFSET + sizeof(uint16_t)) {
            ether_type = netbuf_len >= ETH8023_HEADER_LLC_TYPE_OFFSET + sizeof(uint16_t) ?
                *(uint16_t *)(header + ETH8023_HEADER_LLC_TYPE_OFFSET) : 0;
        } else {
            ether_type = netbuf_len >= ETHER_HEADER_ETH_TYPE_OFFSET + sizeof(uint16_t) ?
                *(uint16_t *)(header + ETHER_HEADER_ETH_TYPE_OFFSET) : 0;
        }
    }
    return ether_type;
}

/*
 * 功能描述  : 通用业务过滤流程
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC hmac_rx_frame_ctrl_enum hmac_host_rx_feature_filter(hmac_host_rx_context_stru *rx_context)
{
    dpe_wlan_vap_stru *dpe_vap = rx_context->dpe_vap;
    dpe_wlan_user_stru *dpe_user = rx_context->dpe_user;
    oal_netbuf_stru *netbuf = rx_context->netbuf;
    mac_rx_ctl_stru *mac_rx_ctl = NULL;
    uint16_t ether_type;

    mac_rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (mac_rx_ctl == NULL) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }
    if (dpe_vap == NULL || dpe_user == NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_host_rx_feature_filter::user[%d] null!}",
            mac_rx_ctl->bit_rx_user_id);
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    if (hmac_rx_qos_data_need_drop(rx_context) == OAL_TRUE) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }

    ether_type = hmac_rx_netbuf_get_ether_type(netbuf, mac_rx_ctl);
    if (hmac_11i_ether_type_filter(dpe_vap->uc_vap_id, dpe_user->us_assoc_id, ether_type) != OAL_SUCC) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }
#ifdef _PRE_WLAN_FEATURE_MCAST_AMPDU
    if (hmac_mcast_ampdu_rx_proc(dpe_vap->uc_vap_id, rx_context->netbuf) == HMAC_RX_FRAME_CTRL_DROP) {
        return HMAC_RX_FRAME_CTRL_DROP;
    }
#endif
    return HMAC_RX_FRAME_CTRL_GOON;
}

/*
 * 功能描述  : 通用业务处理流程
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_rx_proc_feature(hmac_host_rx_context_stru *rx_context,
    oal_netbuf_head_stru *rpt_list, oal_netbuf_head_stru *w2w_list)
{
    oal_netbuf_stru *netbuf;
    uint8_t buff_is_valid = OAL_TRUE;
    oal_netbuf_head_stru release_list;

    if (hmac_host_rx_feature_filter(rx_context) == HMAC_RX_FRAME_CTRL_DROP) {
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_WAPI
    netbuf = hmac_wapi_netbuf_rx_decrypt_sw(rx_context->dpe_vap->uc_vap_id,
        rx_context->dpe_user->us_assoc_id, rx_context->netbuf);
    if (netbuf == NULL) {
        /* 解密失败，已经释放netbuf，此处返回成功，不需要释放netbuf */
        return OAL_SUCC;
    }
    /* WAPI解密完成后，返回新的netbuff，需要重新更新rx_context的netbuf/cb指针 */
    rx_context->netbuf = netbuf;
    rx_context->cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
#endif /* _PRE_WLAN_FEATURE_WAPI */
    hal_host_rx_tcp_udp_checksum(rx_context->netbuf);
    netbuf = hmac_rx_defrag(rx_context);
    if (netbuf == NULL) {
        return OAL_SUCC;
    }
    /* defrag后更新参数 */
    rx_context->netbuf = netbuf;
    rx_context->cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    /* 80211转换成8023 */
    if (hmac_rx_context_prepare_msdu_list_to_eth(rx_context) != OAL_SUCC) {
        return OAL_FAIL;
    }

    hmac_mintp_log((uint8_t *)rx_context->netbuf, MINTP_LOG_LVL_2, MINTP_LOG_DATATYPE_SKB_NEXT_LIST, MINTP_LOG_TYPE_RX);

    /* release list包括需要转发和上报的netbuf */
    oal_netbuf_head_init(&release_list);
    if (hmac_rx_proc_reorder(rx_context, &release_list, &buff_is_valid) != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 此函数执行后rx context中不包含netbuf,全部挂在release list中 */
    hmac_rx_context_netbuf_add_to_list(rx_context, &release_list, buff_is_valid);

    /* APUT转发链更新 */
    if (rx_context->dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_ap_w2w_proc(rx_context, &release_list, w2w_list);
    }
    /* 剩下的报文全部上报协议栈 */
    oal_netbuf_queue_splice_tail_init(&release_list, rpt_list);
    return OAL_SUCC;
}

/*
 * 功能描述  : device/host rx通用处理流程
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_common_proc(hmac_host_rx_context_stru *rx_context, oal_netbuf_head_stru *rpt_list)
{
    oal_netbuf_head_stru w2w_list;
    dpe_wlan_vap_stru *dpe_vap = rx_context->dpe_vap;

    oal_netbuf_list_head_init(&w2w_list);
    hmac_host_rx_static_proc(rx_context);
    if (hmac_rx_proc_feature(rx_context, rpt_list, &w2w_list) != OAL_SUCC) {
        hmac_rx_netbuf_list_free(rx_context->netbuf);
        return;
    }

    g_pm_wlan_pkt_statis.host_rx_pkt += oal_netbuf_list_len(rpt_list) + oal_netbuf_list_len(&w2w_list);
    hmac_rx_wlan_to_wlan_proc(dpe_vap, &w2w_list);
}

/*
 * 功能描述  : rx data 提交内核链表
 * 1.日    期  : 2020年03月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_enqueue(oal_netbuf_stru *netbuf, dpe_wlan_vap_stru *dpe_vap)
{
    oal_net_device_stru *net_device = NULL;

    oal_netbuf_prev(netbuf) = NULL;
    oal_netbuf_next(netbuf) = NULL;

    net_device = hmac_vap_get_net_device(dpe_vap->uc_vap_id);
    if (oal_unlikely(net_device == NULL)) {
        oam_error_log0(0, 0, "{hmac_rx_enqueue_vap_abnormal_handle::vap invalid!}");
        hmac_rx_enqueue_vap_abnormal_handle(netbuf, dpe_vap);
        oal_netbuf_free(netbuf);
        return;
    }

    if (hmac_rx_pn_check(netbuf) != OAL_SUCC) {
        oal_netbuf_free(netbuf);
        return;
    }

    hmac_rx_skb_netif_rpt(net_device, netbuf);
}

/*
 * 功能描述  : rx data aput数据流处理
 * 1.日    期  : 2020年03月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_data_ap_proc(dpe_wlan_vap_stru *dpe_vap, oal_netbuf_head_stru *p_netbuf_head)
{
    oal_netbuf_stru          *netbuf = NULL;
    oal_netbuf_stru          *temp_netbuf = NULL;
    mac_rx_ctl_stru          *rx_ctl = NULL;
    mac_ieee80211_frame_stru *copy_frame_hdr = NULL;
    uint16_t                  netbuf_num;
    oal_netbuf_stru          *netbuf_copy = NULL;
    oal_netbuf_head_stru      w2w_netbuf_hdr;
    dpe_wlan_vap_stru        *dpe_vap_temp = NULL;
    uint8_t                  *puc_da = NULL;

    oal_netbuf_list_head_init(&w2w_netbuf_hdr);
    temp_netbuf = oal_netbuf_peek(p_netbuf_head);
    netbuf_num = (uint16_t)oal_netbuf_get_buf_num(p_netbuf_head);
    while (netbuf_num != 0) {
        netbuf = temp_netbuf;
        if (netbuf == NULL) {
            break;
        }

        oal_netbuf_get_appointed_netbuf(netbuf, 1, &temp_netbuf);
        netbuf_num = oal_sub(netbuf_num, 1);
        rx_ctl     = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        dpe_vap_temp = dpe_wlan_vap_get(rx_ctl->uc_mac_vap_id);
        if (dpe_vap_temp == NULL) {
            hmac_rx_free_netbuf(netbuf, (uint16_t)1);
            continue;
        }

        puc_da = hmac_rx_get_eth_da(netbuf);
        if ((puc_da != NULL) && (ether_is_multicast(puc_da) == OAL_TRUE)) {
            if (hmac_rx_copy_netbuff(&netbuf_copy, netbuf, rx_ctl->uc_mac_vap_id, &copy_frame_hdr) != OAL_SUCC) {
                hmac_rx_free_netbuf(netbuf, (uint16_t)1);
                continue;
            }

            hmac_rx_enqueue(netbuf, dpe_vap_temp);
            /* 将MSDU加入到netbuf链的最后 */
            oal_netbuf_add_to_list_tail(netbuf_copy, &w2w_netbuf_hdr);
            continue;
        }

        /* check if da belong to the same bss */
        if (hmac_rx_da_is_in_same_bss(netbuf, rx_ctl, dpe_vap_temp->uc_vap_id) == OAL_TRUE) {
            /* 目的用户已在AP的用户表中，进行WLAN_TO_WLAN转发 */
            oal_netbuf_add_to_list_tail(netbuf, &w2w_netbuf_hdr);
        } else {
            hmac_rx_enqueue(netbuf, dpe_vap_temp);
        }
    }

    /*  将MSDU链表交给发送流程处理 */
    hmac_rx_wlan_to_wlan_proc(dpe_vap, &w2w_netbuf_hdr);
}

void hmac_rx_data_sta_proc(dpe_wlan_vap_stru *dpe_vap, oal_netbuf_head_stru *netbuf_header)
{
    oal_netbuf_stru *netbuf = NULL;
    mac_rx_ctl_stru *rx_ctl = NULL;

    while (!oal_netbuf_list_empty(netbuf_header)) {
        netbuf = oal_netbuf_delist(netbuf_header);
        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (rx_ctl != NULL && rx_ctl->bit_mcast_bcast == OAL_FALSE) {
            oal_atomic_inc(&(dpe_vap->rx_unicast_pkt_to_lan));
        }
        hmac_rx_enqueue(netbuf, dpe_vap);
    }
}

OAL_STATIC void hmac_rx_enqueue_for_each(oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(cb->uc_mac_vap_id);

    if (oal_unlikely(!dpe_vap)) {
        oam_error_log1(0, 0, "{hmac_rx_enqueue_for_each::dpe_vap NULL! vap id[%d]}", cb->uc_mac_vap_id);
        hmac_rx_enqueue_vap_abnormal_handle(netbuf, NULL);
        oal_netbuf_prev(netbuf) = NULL;
        oal_netbuf_next(netbuf) = NULL;
        oal_netbuf_free(netbuf);
        return;
    }

    hmac_rx_enqueue(netbuf, dpe_vap);
}

/*
 * 功能描述  : 报文上报协议栈
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_rpt_netbuf(oal_netbuf_head_stru *list)
{
    oal_netbuf_stru *netbuf = NULL;
    uint8_t list_empty = oal_netbuf_list_empty(list);

    while (!oal_netbuf_list_empty(list)) {
        netbuf = oal_netbuf_delist_nolock(list);
        if (oal_likely(netbuf)) {
            hmac_rx_enqueue_for_each(netbuf);
        } else {
            oam_error_log0(0, 0, "{hmac_rx_rpt_netbuf::netbuf NULL?}");
        }
    }

    if (!list_empty && (hmac_get_rxthread_enable() == OAL_TRUE)) {
        g_hisi_softwdt_check.rxshed_rpt++;
        hmac_rxdata_sched();
    }
}
