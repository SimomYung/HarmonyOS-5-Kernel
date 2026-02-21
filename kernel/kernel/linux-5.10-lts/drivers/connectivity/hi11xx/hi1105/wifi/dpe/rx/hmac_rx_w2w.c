/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : AMPDU聚合、BA处理接口定义源文件
 * 作    者 :
 * 创建日期 : 2014年11月25日
 */

/* 1 头文件包含 */
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
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
#include <net/tcp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <linux/ip.h>
#include <linux/ieee80211.h>
#include <linux/ipv6.h>
#endif
#ifdef CONFIG_HUAWEI_DUBAI
#include <chipset_common/dubai/dubai.h>
#include <huawei_platform/log/hwlog_kernel.h>
#endif
#ifdef _PRE_WLAN_FEATURE_ISOLATION
#include "hmac_isolation.h"
#endif
#ifdef _PRE_WLAN_FEATURE_PWL
#include "hmac_pwl.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_W2W_C

/*
 * 函 数 名  : hmac_rx_transmit_to_wlan
 * 功能描述  : 将数据帧发送到WLAN侧的接口函数，将一个netbuf链抛给发送流程，每个
 * 1.日    期  : 2012年11月19日
 *   修改内容  : 新生成函数
 */
uint32_t hmac_rx_transmit_to_wlan(frw_event_hdr_stru *event_hdr, oal_netbuf_head_stru *netbuf_head)
{
    oal_netbuf_stru *netbuf = NULL; /* 从netbuf链上取下来的指向netbuf的指针 */
    uint32_t netbuf_num;
    uint32_t ret;
    oal_netbuf_stru *pst_buf_tmp = NULL; /* 暂存netbuf指针，用于while循环 */
    mac_tx_ctl_stru *pst_tx_ctl = NULL;
    mac_vap_stru *pst_mac_vap = NULL;

    if (oal_unlikely(oal_any_null_ptr2(event_hdr, netbuf_head))) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_transmit_to_wlan::input params null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取链头的net buffer */
    netbuf = oal_netbuf_peek(netbuf_head);

    /* 获取mac vap 结构 */
    ret = hmac_tx_get_mac_vap(event_hdr->uc_vap_id, &pst_mac_vap);
    if (oal_unlikely(ret != OAL_SUCC)) {
        netbuf_num = oal_netbuf_list_len(netbuf_head);
        hmac_rx_free_netbuf(netbuf, (uint16_t)netbuf_num);
        oam_warning_log3(event_hdr->uc_vap_id, OAM_SF_RX, "{hmac_rx_transmit_to_wlan::find vap [%d] failed[%d], \
            free [%d] netbuffer.}", event_hdr->uc_vap_id, ret, netbuf_num);
        return ret;
    }

    /* 循环处理每一个netbuf，按照以太网帧的方式处理 */
    while (netbuf != NULL) {
        pst_buf_tmp = oal_netbuf_next(netbuf);

        oal_netbuf_next(netbuf) = NULL;
        oal_netbuf_prev(netbuf) = NULL;
#ifdef _PRE_WLAN_FEATURE_ISOLATION
        if (hmac_isolation_filter(pst_mac_vap, netbuf) == OAL_TRUE) {
            hmac_free_netbuf_list(netbuf);
            netbuf = pst_buf_tmp;
            continue;
        }
#endif
        pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
        memset_s(pst_tx_ctl, sizeof(mac_tx_ctl_stru), 0, sizeof(mac_tx_ctl_stru));

        mac_get_cb_event_type(pst_tx_ctl) = FRW_EVENT_TYPE_WLAN_DTX;
        mac_get_cb_event_subtype(pst_tx_ctl) = DMAC_TX_WLAN_DTX;

        /* set the queue map id when wlan to wlan */
        oal_skb_set_queue_mapping(netbuf, WLAN_NORMAL_QUEUE);
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD
        /* DPE在DPA上WLAN TO WLAN转发 */
        dpe_wlan_xmit_from_wlan(netbuf);
#else
        ret = hmac_tx_lan_to_wlan(pst_mac_vap, &netbuf);
#endif
        if (ret != OAL_SUCC) {
            hmac_free_netbuf_list(netbuf);
        }
        netbuf = pst_buf_tmp;
    }
    return OAL_SUCC;
}

/*
 * 功能描述  : 转交w2w处理
 * 1.日    期  : 2020年03月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_wlan_to_wlan_proc(dpe_wlan_vap_stru *dpe_vap, oal_netbuf_head_stru *pw2w_netbuf_hdr)
{
    frw_event_hdr_stru event_hdr;
    memset_s(&event_hdr, sizeof(frw_event_hdr_stru), 0, sizeof(frw_event_hdr_stru));

    event_hdr.uc_chip_id = dpe_vap->uc_chip_id;
    event_hdr.uc_device_id = dpe_vap->uc_device_id;
    event_hdr.uc_vap_id = dpe_vap->uc_vap_id;

    if (oal_netbuf_list_empty(pw2w_netbuf_hdr) == OAL_FALSE && oal_netbuf_tail(pw2w_netbuf_hdr) != NULL &&
        oal_netbuf_peek(pw2w_netbuf_hdr) != NULL) {
        oal_netbuf_next((oal_netbuf_tail(pw2w_netbuf_hdr))) = NULL;
        oal_netbuf_prev((oal_netbuf_peek(pw2w_netbuf_hdr))) = NULL;
        if (hmac_rx_transmit_to_wlan(&event_hdr, pw2w_netbuf_hdr) != OAL_SUCC) {
            oam_warning_log0(0, 0, "{hmac_rx_wlan_to_wlan_proc:: rx transmit to wlan fail}");
        }
    }
    return;
}
