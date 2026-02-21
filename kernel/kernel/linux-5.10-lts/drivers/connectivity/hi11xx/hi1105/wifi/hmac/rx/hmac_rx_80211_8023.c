/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : rx方向去分片功能
 * 作    者 : wifi
 * 创建日期 : 2024年12月10日
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

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_80211_8023_C

OAL_STATIC uint32_t hmac_rx_get_da_sa_addr(dmac_msdu_stru *rx_msdu,
    mac_ieee80211_frame_stru *frame_hdr, uint8_t vap_idx)
{
    uint8_t *addr = NULL;
#ifdef _PRE_WLAN_FEATURE_ML
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return OAL_FAIL;
    }
#endif
    /* 获取sa addr,mld模式下,MAC中的addr3(SA)是mld MAC地址,直接获取即可 */
    mac_rx_get_sa(frame_hdr, &addr);
    oal_set_mac_addr(rx_msdu->auc_sa, addr);

    /* 获取da addr,mld模式下,MAC中的addr1是link MAC地址,不可直接获取 */
    mac_rx_get_da(frame_hdr, &addr);
#ifdef _PRE_WLAN_FEATURE_ML
    if (dpe_vap->mld_mode != WLAN_LEGACY_MLD_MODE) {
        addr = dpe_vap->mld_addr;
    }
#endif

    oal_set_mac_addr(rx_msdu->auc_da, addr);
    return OAL_SUCC;
}
static inline uint8_t mac_get_snap_type(const mac_llc_snap_stru *llc_snap)
{
    return (*(uint8_t *)(llc_snap));
}
/*
 * 函 数 名  : hmac_rx_frame_80211_to_eth
 * 功能描述  : 将MSDU转化为以太网格式的帧
               空间不够放eth head时返回fail
 * 1.日    期  : 2013年12月19日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_rx_frame_80211_to_eth(oal_netbuf_stru *netbuf, const unsigned char *puc_da, const unsigned char *puc_sa)
{
    mac_rx_ctl_stru *rx_ctrl = NULL;
    mac_ether_header_stru *pst_ether_hdr = NULL;
    mac_llc_snap_stru *llc_snap = NULL;
    uint8_t offset_len;
    if (oal_unlikely(oal_netbuf_len(netbuf) < sizeof(mac_llc_snap_stru))) {
        oam_report_eth_frame(BROADCAST_MACADDR, (uint8_t *)oal_netbuf_data(netbuf),
            (uint16_t)oal_netbuf_len(netbuf), OAM_OTA_FRAME_DIRECTION_TYPE_RX);
        rx_ctrl = (mac_rx_ctl_stru*)oal_netbuf_cb(netbuf);
        if (rx_ctrl != NULL) {
            oam_warning_log3(0, OAM_SF_RX, "hmac_rx_frame_80211_to_eth::netbuf len[%d] head %d frame %d",
                oal_netbuf_len(netbuf), rx_ctrl->uc_mac_header_len, rx_ctrl->us_frame_len);
        }
        return OAL_FAIL;
    }
    /* 注意:默认使用protocol形式的snap头，length形式snap头形式仅前两个字节格式与protocol形式相同 */
    llc_snap = (mac_llc_snap_stru *)oal_netbuf_data(netbuf);
    if (llc_snap == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 对于802.3和非802.3的以太头格式不同，使用snap头的前两个字节来区分 */
    if (oal_likely(mac_snap_is_protocol_type(mac_get_snap_type(llc_snap)))) {
        offset_len = HMAC_RX_DATA_ETHER_OFFSET_LENGTH;
    } else {
        offset_len = MAC_SUBMSDU_HEADER_LEN;
    }
    /*
     * 将payload向前扩充一定长度来构成以太网头的14字节空间。protocl形式的以太头会覆盖原本的snap头，
     * length形式的会将snap头作为数据部分
     */
    oal_netbuf_push(netbuf, offset_len);
    if (oal_unlikely(oal_netbuf_len(netbuf) < sizeof(mac_ether_header_stru))) {
        oam_warning_log2(0, OAM_SF_RX, "hmac_rx_frame_80211_to_eth::"
            "No room for eth hdr, offset[%d], netbuf len[%d]", offset_len, oal_netbuf_len(netbuf));
        return OAL_FAIL;
    }

    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (pst_ether_hdr == NULL) {
        return OAL_FAIL;
    }
    pst_ether_hdr->us_ether_type =
        (offset_len == HMAC_RX_DATA_ETHER_OFFSET_LENGTH) ? llc_snap->us_ether_type : netbuf->len;
    oal_set_mac_addr(pst_ether_hdr->auc_ether_shost, puc_sa);
    oal_set_mac_addr(pst_ether_hdr->auc_ether_dhost, puc_da);
    return OAL_SUCC;
}

/*
 * 功能描述  : 802.11转换以太网格式
 * 1.日    期  : 2020年1月6日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_rx_proc_80211_to_eth(oal_netbuf_stru *netbuf,
    mac_rx_ctl_stru *rx_ctrl, hmac_host_rx_context_stru *rx_context)
{
    dmac_msdu_stru rx_msdu;
    mac_ieee80211_frame_stru *frame_hdr = (mac_ieee80211_frame_stru *)oal_netbuf_data(netbuf);
    uint8_t *frame_addr = NULL;

    memset_s(&rx_msdu, sizeof(dmac_msdu_stru), 0, sizeof(dmac_msdu_stru));
    mac_get_transmit_addr(frame_hdr, &frame_addr);
    oal_set_mac_addr(rx_msdu.auc_ta, frame_addr);

    /* 对当前的msdu进行赋值 */
    rx_msdu.pst_netbuf = netbuf;
    /* 802.11帧头结构 */
    /* 非聚合首帧：24字节的802.11mac_header + 2字节的QoS_control字段 */
    /* 聚合首帧：  24字节的802.11mac_header + 2字节的QoS_control字段 + 14字节的sub_amsdu头 */
    /* 聚合非首帧：14字节的sub_amsdu头 */
    if (mac_get_rx_cb_is_first_sub_msdu(rx_ctrl)) {
        /* 聚合首帧处理，获取源地址和目的地址 */
        if (hmac_rx_get_da_sa_addr(&rx_msdu, frame_hdr, rx_context->dpe_vap->uc_vap_id) != OAL_SUCC) {
            return OAL_FAIL;
        }

        /* 将netbuf的data指针指向mac frame的payload处 */
        oal_netbuf_pull(netbuf, rx_ctrl->uc_mac_header_len + (mac_rxcb_is_amsdu(rx_ctrl) ?
            MAC_SUBMSDU_HEADER_LEN : 0));
    } else {
        /* 聚合非首帧处理 */
        oal_set_mac_addr(rx_msdu.auc_da, (uint8_t *)oal_netbuf_data(netbuf));
        oal_set_mac_addr(rx_msdu.auc_sa, (uint8_t *)(oal_netbuf_data(netbuf) + OAL_MAC_ADDRESS_LEN));
        /* 将netbuf的data指针指向mac frame的payload处 */
        oal_netbuf_pull(netbuf, MAC_SUBMSDU_HEADER_LEN);
    }
    return hmac_rx_frame_80211_to_eth(netbuf, rx_msdu.auc_da, rx_msdu.auc_sa);
}
