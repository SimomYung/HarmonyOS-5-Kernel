/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : DMAC模块接收帧的公共操作函数以及数据帧的操作函数定义的源文件
 * 作    者 :
 * 创建日期 : 2024年11月12日
 */

#include "oal_net.h"
#include "mac_data.h"
#include "hmac_rx.h"
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#include "hmac_sniffer.h"
#endif
#include "dpe_wlan_user.h"
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
#include <net/tcp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <linux/ip.h>
#include <linux/ieee80211.h>
#include <linux/ipv6.h>
#endif
#include "oam_event_wifi.h"
#ifdef CONFIG_HUAWEI_DUBAI
#include <chipset_common/dubai/dubai.h>
#include <huawei_platform/log/hwlog_kernel.h>
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
#include "hmac_vsp_if.h"
#endif
#include "hmac_pm.h"
#include "hmac_rx_dev.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_DEV_C

/*
 * 功能描述  : dev rx构建host rx上下文
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_dev_build_rx_context(oal_netbuf_stru *netbuf, hmac_host_rx_context_stru *rx_context)
{
    dpe_wlan_vap_stru *dpe_vap = NULL;
    mac_rx_ctl_stru *cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (cb == NULL) {
        return;
    }
    dpe_vap = dpe_wlan_vap_get(cb->uc_mac_vap_id);
    if (dpe_vap == NULL) {
        return;
    }

    rx_context->hal_dev = hal_get_host_device(dpe_vap->hal_dev_id);
    rx_context->dpe_vap = dpe_vap;
    rx_context->dpe_user = dpe_wlan_user_get(cb->bit_rx_user_id);
    rx_context->netbuf = netbuf;
    rx_context->cb = cb;
}
/*
 * 功能描述  : device rx专有流程
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_dev_rx_unique_proc(uint8_t vap_idx, hmac_host_rx_context_stru *rx_context)
{
    oal_netbuf_stru *netbuf = rx_context->netbuf;

    for (; netbuf != NULL; netbuf = oal_netbuf_next(netbuf)) {
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
        /* host低功耗唤醒维测 */
        if (hmac_pm_wkup_src_parse_get() == OAL_TRUE) {
            oal_io_print(WIFI_WAKESRC_TAG "rx: hmac_parse_packet!\n");
            hmac_parse_packet(vap_idx, netbuf);
            hmac_pm_wkup_src_parse_set(OAL_FALSE);
        }
#endif
    }
}

/*
 * 功能描述  : device rx msdu聚合处理
 * 1.日    期  : 2021.4.25
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_device_rx_amsdu_proc(mac_rx_ctl_stru *cb, dpe_wlan_user_stru *dpe_user,
    oal_netbuf_stru  **netbuf)
{
    oal_netbuf_stru *temp_netbuf = NULL;
    oal_netbuf_stru *first_netbuf = NULL;
    oal_netbuf_stru *next_netbuf = NULL;
    oal_netbuf_head_stru *list_head = NULL;
    mac_rx_ctl_stru *temp_cb = NULL;

    list_head = &dpe_user->device_rx_list[mac_get_rx_cb_tid(cb)];
    if (mac_rxcb_is_first_msdu(cb) && (!mac_rxcb_is_last_msdu(cb))) {
        if (!oal_netbuf_list_empty(list_head)) {
            oam_error_log2(0, OAM_SF_RX, "{hmac_device_rx_amsdu_proc::list is not empty len = [%d], sn = [%d]}",
                oal_netbuf_list_len(list_head), mac_get_rx_cb_seq_num(cb));

            while ((temp_netbuf = oal_netbuf_delist(list_head)) != NULL) {
                temp_cb = (mac_rx_ctl_stru *)oal_netbuf_cb(temp_netbuf);
                oam_error_log3(0, OAM_SF_RX, "{hmac_device_rx_amsdu_proc::sn = [%d], first = [%d], last = [%d]}",
                    mac_get_rx_cb_seq_num(temp_cb), mac_rxcb_is_first_msdu(temp_cb), mac_rxcb_is_last_msdu(temp_cb));
                oal_netbuf_free(temp_netbuf);
            }
            oal_netbuf_free_list(list_head, oal_netbuf_list_len(list_head));
        }
        oal_netbuf_list_head_init(list_head);
        oal_netbuf_add_to_list_tail(*netbuf, list_head);
        return OAL_RETURN;
    } else if (mac_rxcb_is_amsdu_sub_msdu(cb)) {
        oal_netbuf_add_to_list_tail(*netbuf, list_head);
        if (!mac_rxcb_is_last_msdu(cb)) {
            return OAL_RETURN;
        }
        first_netbuf = oal_netbuf_delist(list_head);
        temp_netbuf = first_netbuf;
        while (temp_netbuf != NULL) {
            next_netbuf = oal_netbuf_delist(list_head);
            oal_netbuf_next(temp_netbuf) = next_netbuf;
            temp_netbuf = next_netbuf;
        }
        *netbuf = first_netbuf;
    }

    return OAL_CONTINUE;
}

OAL_STATIC OAL_INLINE void hmac_dev_rx_process_data_proc(hmac_host_rx_context_stru *rx_context)
{
    oal_netbuf_head_stru rpt_list;
    oal_netbuf_list_head_init(&rpt_list);
    hmac_rx_common_proc(rx_context, &rpt_list);
    hmac_rx_route_print(rx_context->hal_dev, DEVICE_RX_HOST_REPORT, rx_context->cb);
    if (oal_netbuf_list_len(&rpt_list)) {
        hmac_rx_rpt_netbuf(&rpt_list);
    }
}

uint32_t hmac_dev_rx_process_data(oal_netbuf_stru *netbuf)
{
    hmac_host_rx_context_stru rx_context = { 0 };
    mac_rx_ctl_stru *cb = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    dpe_wlan_user_stru *dpe_user = NULL;
    dpe_wlan_vap_stru *dpe_vap = NULL;
    uint8_t vapid;

    if (cb == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    vapid = mac_get_rx_cb_mac_vap_id(cb);
    dpe_vap = dpe_wlan_vap_get(vapid);
    if (dpe_vap == NULL) {
        oam_error_log0(vapid, OAM_SF_RX, "{hmac_dev_rx_process_data::dpe_vap null.}");
        oal_netbuf_free(netbuf);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_SNIFFER
    if (hmac_sniffer_device_rx_proc(dpe_vap->hal_dev_id, netbuf, cb) != OAL_CONTINUE) {
        hmac_rx_netbuf_list_free(netbuf);
        return OAL_SUCC;
    }
#endif
    dpe_user = dpe_wlan_user_get(cb->bit_rx_user_id);
    if (dpe_user == NULL) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_dev_rx_process_data::dpe_user null}");
        hmac_rx_netbuf_list_free(netbuf);
        return OAL_SUCC;
    }

    if (hmac_device_rx_amsdu_proc(cb, dpe_user, &netbuf) != OAL_CONTINUE) {
        return OAL_SUCC;
    }

    hmac_dev_build_rx_context(netbuf, &rx_context);
    hmac_dev_rx_unique_proc(dpe_vap->uc_vap_id, &rx_context);

    if (rx_context.dpe_user == NULL) {
        return OAL_SUCC;
    }
#ifdef _PRE_WLAN_FEATURE_VSP
    /* vsp帧不采用上报协议栈方式，单独处理 */
    if (hmac_vsp_rx_proc(rx_context.dpe_user->us_assoc_id, netbuf) == VSP_RX_ACCEPT) {
        return OAL_SUCC;
    }
#endif

    hmac_dev_rx_process_data_proc(&rx_context);
    return OAL_SUCC;
}
/*
 * 功能描述  : dev rx数据帧上报总入口
 * 1.日    期  : 2020年1月5日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_dev_rx_process_data_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru           *event = NULL;
    dmac_wlan_drx_event_stru *wlan_rx_event = NULL;
    oal_netbuf_stru          *netbuf = NULL;

    if (oal_unlikely(event_mem == NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    event = frw_get_event_stru(event_mem);
    wlan_rx_event = (dmac_wlan_drx_event_stru *)(event->auc_event_data);
    netbuf = wlan_rx_event->pst_netbuf;

    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_ring_data_event::netbuf null}");
        return OAL_SUCC;
    }

    return hmac_dev_rx_process_data(netbuf);
}