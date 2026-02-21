/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : DMAC模块接收帧的公共操作函数以及数据帧的操作函数定义的源文件
 * 作    者 :
 * 创建日期 : 2012年11月20日
 */

#include "hmac_rx_data.h"
#include "oal_net.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif
#include "mac_frame.h"
#include "mac_data.h"
#include "hmac_rx.h"
#include "hmac_rx_defrag.h"
#include "hmac_11i.h"
#include "host_hal_device.h"
#include "hmac_rx_reorder.h"
#include "hmac_tcp_opt.h"
#include "hmac_rx_host.h"
#include "hmac_sniffer.h"
#include "hmac_wapi.h"
#include "oam_stat_wifi.h"
#include "hmac_wifi_delay.h"
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
#include <net/tcp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <linux/ip.h>
#include <linux/ieee80211.h>
#include <linux/ipv6.h>
#endif
#include <hmac_auto_adjust_freq.h>
#include "hmac_user.h"
#include "hmac_tx_amsdu.h"
#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif
#ifdef _PRE_WLAN_FEATURE_FTM
#include "hmac_ftm.h"
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
#include "hmac_csi.h"
#endif
#include "oam_event_wifi.h"
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#ifdef CONFIG_HUAWEI_DUBAI
#include <chipset_common/dubai/dubai.h>
#endif
#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
#include "hmac_rr_performance.h"
#endif
#include "hmac_ht_self_cure.h"
#include "hmac_ota_report.h"
#include "hmac_stat.h"
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_ps.h"
#endif
#ifdef _PRE_WLAN_FEATURE_VSP
#include "hmac_vsp_if.h"
#endif
#ifdef _PRE_WLAN_FEATURE_PWL
#include "hmac_pwl.h"
#endif
#ifdef _PRE_WLAN_FEATURE_ESPE
#include "hmac_espe.h"
#endif
#ifdef _PRE_WLAN_FEATURE_ISOLATION
#include "hmac_isolation.h"
#endif
#include "hmac_mgmt_sta_up.h"
#ifdef CONFIG_ATP_SERVICE_CLASSIFY
#include "atp_skb_mark.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_DATA_C

/* 2 全局变量定义 */
/* 3 函数实现 */
OAL_STATIC oal_netbuf_stru *dpe_rx_lan_frame_feature_proc_wapi(uint8_t vap_idx, uint16_t user_idx,
    oal_netbuf_stru *netbuf, mac_ieee80211_frame_stru *frame_hdr)
{
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(vap_idx);
    if (hmac_vap == NULL) {
        return NULL;
    }
    return hmac_rx_lan_frame_wapi_proc(hmac_vap, user_idx, netbuf, frame_hdr);
#endif
    return netbuf;
}
OAL_STATIC oal_netbuf_stru *hmac_rx_lan_frame_feature_proc(uint8_t vap_idx, uint16_t user_idx,
    oal_netbuf_stru *netbuf, mac_ieee80211_frame_stru *frame_hdr)
{
#ifdef _PRE_WLAN_FEATURE_PWL
    hmac_vap_stru *hmac_vap = mac_res_get_hmac_vap(vap_idx);
    hmac_user_stru *hmac_user = mac_res_get_hmac_user(user_idx);
    if (hmac_vap == NULL || hmac_user == NULL) {
        return NULL;
    }
    if (hmac_vap->st_vap_base_info.pwl_enable == OAL_TRUE) {
        return hmac_pwl_process_rx_lan_frame(hmac_vap, hmac_user, netbuf, frame_hdr);
    }
#endif
    return dpe_rx_lan_frame_feature_proc_wapi(vap_idx, user_idx, netbuf, frame_hdr);
}

OAL_STATIC uint32_t hmac_rx_lan_frame_ap_proc(uint8_t vap_idx, dmac_msdu_stru *msdu,
    oal_netbuf_head_stru *netbuf_hdr)
{
    uint16_t user_idx = g_wlan_spec_cfg->invalid_user_id;
    /*  当msdu.da在vap下,进行wlan_to_wlan转发,防止to_lan到内核不进行转发 */
    if (dpe_vap_find_user_by_macaddr(vap_idx, msdu->auc_da, &user_idx) == OAL_SUCC) {
        /* 将MSDU转化为以太网格式的帧 */
        if (hmac_rx_frame_80211_to_eth(msdu->pst_netbuf, msdu->auc_da, msdu->auc_sa) != OAL_SUCC) {
            oal_netbuf_free(msdu->pst_netbuf);
            return OAL_RETURN;
        }
        /* 将MSDU加入到netbuf链的最后 */
        oal_netbuf_add_to_list_tail(msdu->pst_netbuf, netbuf_hdr);
        return OAL_RETURN;
    }
    return OAL_CONTINUE;
}

OAL_STATIC OAL_INLINE void hmac_transmit_msdu_to_lan(
    dpe_wlan_vap_stru *dpe_vap, oal_netbuf_stru *netbuf, mac_ether_header_stru *pst_ether_hdr)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 11, 0))
    oal_net_device_stru *pst_device = NULL;
#endif
    /* 统计收包信息 */
    hmac_stat_vap_rx_netbuf(dpe_vap->uc_vap_id, netbuf);

#ifdef _PRE_WLAN_DFT_DUMP_FRAME
    hmac_rx_report_eth_frame(dpe_vap->uc_vap_id, netbuf);
#endif

    oal_mem_netbuf_trace(netbuf, OAL_TRUE);

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    /* RR性能检测接收流程host转发至lan口位置打点 */
    hmac_rr_rx_h2w_timestamp();
#endif

    /* 将skb转发给桥 */
    hmac_rx_skb_netif_rpt(dpe_vap->pst_net_device, netbuf);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
    /* 4.11以上内核版本net_device结构体中没有last_rx字段 */
#else
    /* 置位net_dev->jiffies变量 */
    pst_device = dpe_vap->pst_net_device;
    if (pst_device != NULL) {
        oal_netdevice_last_rx(pst_device) = OAL_TIME_JIFFY;
    }
#endif
}

/*
 * 功能描述  : 将MSDU转发到LAN的接口，包括地址转换等信息的设置
 */
OAL_STATIC void hmac_rx_transmit_msdu_to_lan(dpe_wlan_vap_stru *dpe_vap, uint16_t user_idx, dmac_msdu_stru *msdu)
{
    uint32_t ret;
    oal_netbuf_stru *netbuf = msdu->pst_netbuf;
    mac_ether_header_stru *pst_ether_hdr = NULL;
    uint8_t *puc_mac_addr = msdu->auc_ta;

    oal_netbuf_prev(netbuf) = NULL;
    oal_netbuf_next(netbuf) = NULL;

    if (oal_unlikely(dpe_vap == NULL)) {
        oal_netbuf_free(netbuf);
        return;
    }
    ret = hmac_rx_frame_80211_to_eth(netbuf, msdu->auc_da, msdu->auc_sa);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_netbuf_free(netbuf);
        return;
    }
    /* hmac_rx_frame_80211_to_eth函数内已经判断过eth头的长度合法性，所以可以不判断 */
    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (wlan_pm_wkup_src_debug_get() == OAL_TRUE) {
        oal_io_print(WIFI_WAKESRC_TAG "rx: hmac_parse_packet!\n");
        hmac_parse_packet(dpe_vap->uc_vap_id, netbuf);
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
    }
#endif
    if (pst_ether_hdr == NULL) {
        oal_netbuf_free(netbuf);
        return;
    }
    if (hmac_11i_ether_type_filter(dpe_vap->uc_vap_id, user_idx, pst_ether_hdr->us_ether_type) != OAL_SUCC) {
        /* 接收安全数据过滤 */
        /* ,us_ether_type为0导致企业级加密失败,添加维测信息看是否报文解析异常 */
        oam_report_eth_frame(puc_mac_addr, (uint8_t *)pst_ether_hdr,
                             (uint16_t)oal_netbuf_len(netbuf), OAM_OTA_FRAME_DIRECTION_TYPE_RX);

        oal_netbuf_free(netbuf);
        oam_stat_vap_incr(dpe_vap->uc_vap_id, rx_portvalid_check_fail_dropped, 1);
        return;
    }
    if (ether_is_multicast(msdu->auc_da) == OAL_FALSE) {
        oal_atomic_inc(&(dpe_vap->rx_unicast_pkt_to_lan));
    }
    hmac_transmit_msdu_to_lan(dpe_vap, netbuf, pst_ether_hdr);
}

OAL_STATIC void hmac_rx_transmit_msdu(uint8_t vap_idx, uint16_t user_idx,
    dmac_msdu_stru *msdu, oal_netbuf_head_stru *w2w_netbuf_hdr)
{
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);
    if (dpe_vap == NULL) {
        return;
    }
    /* ap wlan to wlan转发 */
    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        if (hmac_rx_lan_frame_ap_proc(vap_idx, msdu, w2w_netbuf_hdr) != OAL_CONTINUE) {
            return;
        }
    }

    /* 将MSDU转发到LAN */
    hmac_rx_transmit_msdu_to_lan(dpe_vap, user_idx, msdu);
}

OAL_STATIC void hmac_rx_vip_info_other_data_eapol(mac_llc_snap_stru *llc, mac_rx_ctl_stru *rx_ctrl,
    oal_netbuf_stru *buf, uint8_t vap_idx, uint32_t len)
{
    mac_eapol_type_enum_uint8 en_eapol_type;
    /* netbuf 长度判断 */
    if (len < rx_ctrl->uc_mac_header_len + sizeof(mac_llc_snap_stru) + sizeof(mac_eapol_header_stru)) {
        oam_error_log1(0, OAM_SF_ANY, "hmac_rx_vip_info::len[%d]", len);
        return;
    }
    en_eapol_type = mac_get_eapol_key_type((uint8_t *)(llc + 1));
    oam_warning_log3(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info::EAPOL type=%u, len==%u, \
        user[%d]}[1:1/4 2:2/4 3:3/4 4:4/4]", en_eapol_type, oal_netbuf_len(buf), rx_ctrl->bit_rx_user_id);
}

OAL_STATIC void hmac_rx_set_ht_self_cure_event(uint8_t vap_idx,
    mac_ieee80211_qos_htc_frame_addr4_stru *mac_header, uint8_t event_id)
{
    if (mac_header == NULL) {
        return;
    }

    hmac_ht_self_cure_event_set(vap_idx, mac_header->auc_address2, event_id);
}

OAL_STATIC void hmac_rx_vip_info_other_data_type(mac_llc_snap_stru *llc, mac_rx_ctl_stru *rx_ctrl,
    uint8_t vap_idx, uint32_t len, uint8_t data_type)
{
    oal_eth_arphdr_stru *arp_head = NULL;
    if (len < rx_ctrl->uc_mac_header_len + sizeof(mac_llc_snap_stru) + sizeof(oal_eth_arphdr_stru)) {
        oam_error_log2(0, OAM_SF_ANY, "hmac_rx_vip_info::len[%d] data_type[%d]", len, data_type);
        return;
    }
    arp_head = (oal_eth_arphdr_stru *)(llc + 1);
    oam_warning_log1(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info::ARP type=%d[2:arp resp 3:arp req]", data_type);
    oam_warning_log4(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info:: ARP sip: %d.%d, dip: %d.%d",
        arp_head->auc_ar_sip[BYTE_OFFSET_2], arp_head->auc_ar_sip[BYTE_OFFSET_3],
        arp_head->auc_ar_tip[BYTE_OFFSET_2], arp_head->auc_ar_tip[BYTE_OFFSET_3]);
}

/*
 * 函 数 名  : hmac_rx_vip_info
 * 功能描述  : hmac 接收一个mpdu的关键帧识别维测
 * 1.日    期  : 2019年10月19日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_vip_info(uint8_t vap_idx, uint8_t data_type,
    oal_netbuf_stru *pst_buf, mac_rx_ctl_stru *pst_rx_ctrl)
{
    mac_ieee80211_qos_htc_frame_addr4_stru *pst_mac_header = NULL;
    mac_llc_snap_stru *pst_llc = NULL;
    oal_ip_header_stru *ip_hdr = NULL;
    int32_t l_ret = EOK;
    uint8_t sip[ETH_SENDER_IP_ADDR_LEN], dip[ETH_SENDER_IP_ADDR_LEN]; /* sender IP address */
    uint8_t uc_dhcp_type;
    uint32_t netbuf_len;
    /* 收到skb为80211头 */
    /* 获取LLC SNAP */
    pst_llc = (mac_llc_snap_stru *)(oal_netbuf_data(pst_buf) + pst_rx_ctrl->uc_mac_header_len);
    pst_mac_header = (mac_ieee80211_qos_htc_frame_addr4_stru *)(uintptr_t)mac_get_rx_cb_mac_header_addr(pst_rx_ctrl);
    if (pst_llc == NULL || pst_mac_header == NULL) {
        return;
    }
    netbuf_len = oal_netbuf_len(pst_buf);
    if (data_type == MAC_DATA_EAPOL) {
        hmac_rx_vip_info_other_data_eapol(pst_llc, pst_rx_ctrl, pst_buf, vap_idx, netbuf_len);
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
        hmac_rx_group_rekey_stat(vap_idx);
#endif
        hmac_rx_set_ht_self_cure_event(vap_idx, pst_mac_header, HAMC_HT_SELF_CURE_EVENT_RX_EAPOL);
    } else if (data_type == MAC_DATA_DHCP) {
        if (netbuf_len < pst_rx_ctrl->uc_mac_header_len + sizeof(mac_llc_snap_stru) + sizeof(oal_ip_header_stru)) {
            oam_error_log2(0, OAM_SF_ANY, "hmac_rx_vip_info::DHCP netbuf_len[%d]data_type[%d]", netbuf_len, data_type);
            return;
        }
        ip_hdr = (oal_ip_header_stru *)(pst_llc + 1);

        l_ret += memcpy_s((uint8_t *)sip, ETH_SENDER_IP_ADDR_LEN, (uint8_t *)&ip_hdr->saddr, sizeof(uint32_t));
        l_ret += memcpy_s((uint8_t *)dip, ETH_SENDER_IP_ADDR_LEN, (uint8_t *)&ip_hdr->daddr, sizeof(uint32_t));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_rx_vip_info::DHCP memcpy fail!");
            return;
        }

        netbuf_len -= (pst_rx_ctrl->uc_mac_header_len + sizeof(mac_llc_snap_stru));
        uc_dhcp_type = mac_get_dhcp_frame_type(ip_hdr, netbuf_len);
        hmac_rx_set_ht_self_cure_event(vap_idx, pst_mac_header, HMAC_HT_SELF_CURE_EVENT_RX_DHCP_FRAME);
        oam_warning_log1(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info::DHCP type=%d.[1:discovery 2:offer \
            3:request 4:decline 5:ack 6:nack 7:release 8:inform.]", uc_dhcp_type);
        oam_warning_log4(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info:: DHCP sip: %d.%d, dip: %d.%d.",
                         sip[BYTE_OFFSET_2], sip[BYTE_OFFSET_3], dip[BYTE_OFFSET_2], dip[BYTE_OFFSET_3]);
    } else {
        hmac_rx_vip_info_other_data_type(pst_llc, pst_rx_ctrl, vap_idx, netbuf_len, data_type);
    }

    oam_warning_log3(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info::smac:%02X:XX:XX:XX:%02X:%02X",
        pst_mac_header->auc_address2[MAC_ADDR_0],
        pst_mac_header->auc_address2[MAC_ADDR_4], pst_mac_header->auc_address2[MAC_ADDR_5]);
    oam_warning_log3(vap_idx, OAM_SF_ANY, "{hmac_rx_vip_info:: dmac:%02X:XX:XX:XX:%02X:%02X",
        pst_mac_header->auc_address1[MAC_ADDR_0],
        pst_mac_header->auc_address1[MAC_ADDR_4], pst_mac_header->auc_address1[MAC_ADDR_5]);
}

OAL_STATIC uint32_t hmac_rx_lan_frame_normal(uint8_t vap_idx, uint16_t user_idx,
    oal_netbuf_stru *netbuf, oal_netbuf_head_stru *w2w_netbuf_hdr)
{
    mac_ieee80211_frame_stru *frame_hdr = NULL;
    mac_rx_ctl_stru *rx_ctrl = NULL;
    dmac_msdu_stru msdu; /* 保存解析出来的每一个MSDU */
    uint8_t datatype;
    uint8_t *addr = NULL;

    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (rx_ctrl == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    netbuf = hmac_defrag_process(user_idx, netbuf, rx_ctrl->uc_mac_header_len);
    if (netbuf == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 重新获取该MPDU的控制信息 */
    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    /* 去分片有可能释放netbuf，需要重新获取frame hdr  */
    frame_hdr = (mac_ieee80211_frame_stru *)(uintptr_t)mac_get_rx_cb_mac_header_addr(rx_ctrl);

    /* 打印出关键帧信息 */
    datatype = mac_get_data_type_from_80211(netbuf, mac_get_rx_cb_mac_header_len(rx_ctrl),
        mac_get_rx_cb_frame_len(rx_ctrl) - mac_get_rx_cb_mac_header_len(rx_ctrl));
    if (datatype <= MAC_DATA_ARP_RSP) {
        hmac_rx_vip_info(vap_idx, datatype, netbuf, rx_ctrl);
    }

#ifdef _PRE_WINDOWS_SUPPORT
    oal_skb_to_wdi(netbuf);
#else
    memset_s(&msdu, sizeof(dmac_msdu_stru), 0, sizeof(dmac_msdu_stru));
    mac_get_transmit_addr(frame_hdr, &addr);
    oal_set_mac_addr(msdu.auc_ta, addr);

    /* 对当前的msdu进行赋值 */
    msdu.pst_netbuf = netbuf;

    /* 获取源地址和目的地址 */
    mac_rx_get_sa(frame_hdr, &addr);
    oal_set_mac_addr(msdu.auc_sa, addr);

    mac_rx_get_da(frame_hdr, &addr);
    oal_set_mac_addr(msdu.auc_da, addr);

    /* 将netbuf的data指针指向mac frame的payload处 */
    oal_netbuf_pull(netbuf, rx_ctrl->uc_mac_header_len);

    hmac_rx_transmit_msdu(vap_idx, user_idx, &msdu, w2w_netbuf_hdr);
#endif
    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_rx_init_amsdu_state
 * 功能描述  : 初始化amsdu_state 变量(圈复杂度)
 * 1.日    期  : 2019年8月31日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_init_amsdu_state(oal_netbuf_stru *netbuf, dmac_msdu_proc_state_stru *pst_msdu_state)
{
    mac_rx_ctl_stru *pst_rx_ctrl = NULL; /* MPDU的控制信息 */
    uint32_t       need_pull_len;

    /* 首次进入该函数解析AMSDU */
    if (oal_all_zero_value2(pst_msdu_state->uc_procd_netbuf_nums, pst_msdu_state->uc_procd_msdu_in_netbuf)) {
        pst_msdu_state->pst_curr_netbuf      = netbuf;

        /* AMSDU时，首个netbuf的中包含802.11头，对应的payload需要偏移 */
        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_msdu_state->pst_curr_netbuf);
        if (pst_rx_ctrl == NULL) {
            return;
        }

        pst_msdu_state->puc_curr_netbuf_data   = (uint8_t*)((uintptr_t)mac_get_rx_cb_mac_header_addr(pst_rx_ctrl) +
                                                 pst_rx_ctrl->uc_mac_header_len);
        pst_msdu_state->uc_netbuf_nums_in_mpdu = pst_rx_ctrl->bit_buff_nums;
        pst_msdu_state->uc_msdu_nums_in_netbuf = pst_rx_ctrl->uc_msdu_in_buffer;
        pst_msdu_state->us_submsdu_offset      = 0;
        pst_msdu_state->uc_flag = pst_rx_ctrl->bit_is_first_buffer;
        /* 使netbuf 指向amsdu 帧头 */
        need_pull_len = (uint32_t)(pst_msdu_state->puc_curr_netbuf_data - oal_netbuf_data(netbuf));
        oal_netbuf_pull(pst_msdu_state->pst_curr_netbuf, need_pull_len);
    }
}

OAL_STATIC void hmac_rx_free_amsdu_netbuf(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *netbuf_next = NULL;
    while (netbuf != NULL) {
        netbuf_next = oal_get_netbuf_next(netbuf);
        oal_netbuf_free(netbuf);
        netbuf = netbuf_next;
    }
}

OAL_STATIC uint32_t hmac_rx_amsdu_check_frame_da(dmac_msdu_proc_state_stru *msdu_state, dmac_msdu_stru *msdu)
{
    if (msdu_state->uc_flag && !oal_memcmp(msdu->auc_da, &g_st_mac_11c_snap_header, WLAN_MAC_ADDR_LEN)) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_amsdu_check_frame_da::first msdu in a-msdu da is snap llc header!.}");
        msdu_state->uc_flag = OAL_FALSE;
        hmac_rx_free_amsdu_netbuf(msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_large_amsdu(dmac_msdu_stru *msdu, dmac_msdu_proc_state_stru *msdu_state,
    oal_bool_enum_uint8 *need_free, mac_msdu_proc_status_enum_uint8 *proc_state)
{
    uint16_t submsdu_len = 0; /* submsdu的长度 */
    uint8_t *submsdu_hdr = NULL; /* 指向submsdu头部的指针 */
    /* 获取待处理submsdu的头指针 */
    submsdu_hdr = msdu_state->puc_curr_netbuf_data + msdu_state->us_submsdu_offset;

    mac_get_submsdu_len(submsdu_hdr, &submsdu_len);
    oal_set_mac_addr(msdu->auc_sa, (submsdu_hdr + MAC_SUBMSDU_SA_OFFSET));
    oal_set_mac_addr(msdu->auc_da, (submsdu_hdr + MAC_SUBMSDU_DA_OFFSET));
    if (hmac_rx_amsdu_check_frame_da(msdu_state, msdu) != OAL_SUCC) {
        *proc_state = MAC_PROC_ERROR;
        return OAL_FAIL;
    }
    msdu_state->uc_flag = OAL_FALSE;
    /* 指向amsdu帧体 */
    oal_netbuf_pull(msdu_state->pst_curr_netbuf, MAC_SUBMSDU_HEADER_LEN);

    if (submsdu_len > oal_netbuf_len(msdu_state->pst_curr_netbuf)) {
        *proc_state = MAC_PROC_ERROR;
        oam_warning_log2(0, OAM_SF_RX, "{hmac_rx_parse_amsdu::us_submsdu_len %d is not valid netbuf len=%d.}",
            submsdu_len, oal_netbuf_len(msdu_state->pst_curr_netbuf));
        hmac_rx_free_amsdu_netbuf(msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }

    oal_netbuf_trim(msdu_state->pst_curr_netbuf, oal_netbuf_len(msdu_state->pst_curr_netbuf));
    oal_netbuf_put(msdu_state->pst_curr_netbuf, submsdu_len);

    /* 直接使用该netbuf上报给内核 免去一次netbuf申请和拷贝 */
    *need_free = OAL_FALSE;
    msdu->pst_netbuf = msdu_state->pst_curr_netbuf;

    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_rx_amsdu_check_frame_len
 * 功能描述  : 检查amsdu帧是否合理。
 *             OAL_SUCC:AMSDU 长度合理
 *             OAL_FAIL:AMSDU 长度不合理
 * 1.日    期  : 2019年8月31日
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_rx_amsdu_check_frame_len(oal_netbuf_stru *netbuf,
    dmac_msdu_proc_state_stru *pst_msdu_state, uint16_t us_submsdu_len, uint8_t uc_submsdu_pad_len)
{
    if (us_submsdu_len > WLAN_MAX_NETBUF_SIZE ||
        (pst_msdu_state->us_submsdu_offset + MAC_SUBMSDU_HEADER_LEN + us_submsdu_len >
        oal_netbuf_len(netbuf))) {
        oam_warning_log4(0, OAM_SF_RX, "hmac_rx_amsdu_check_frame_len::submsdu not valid. submsdu_len %d, \
            offset %d. msdu_nums %d, procd_msdu %d.", us_submsdu_len, pst_msdu_state->us_submsdu_offset,
            pst_msdu_state->uc_msdu_nums_in_netbuf, pst_msdu_state->uc_procd_msdu_in_netbuf);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_parse_small_amsdu(oal_netbuf_stru *netbuf, dmac_msdu_stru *msdu,
    dmac_msdu_proc_state_stru *msdu_state, oal_bool_enum_uint8 *need_free)
{
    uint16_t submsdu_len = 0; /* submsdu的长度 */
    uint8_t submsdu_pad_len = 0; /* submsdu的填充长度 */
    uint8_t *submsdu_hdr = NULL; /* 指向submsdu头部的指针 */

    /* 获取待处理submsdu的头指针 */
    submsdu_hdr = msdu_state->puc_curr_netbuf_data + msdu_state->us_submsdu_offset;

    /* 获取submsdu的相关信息 */
    mac_get_submsdu_len(submsdu_hdr, &submsdu_len);
    mac_get_submsdu_pad_len(MAC_SUBMSDU_HEADER_LEN + submsdu_len, &submsdu_pad_len);
    oal_set_mac_addr(msdu->auc_sa, (submsdu_hdr + MAC_SUBMSDU_SA_OFFSET));
    oal_set_mac_addr(msdu->auc_da, (submsdu_hdr + MAC_SUBMSDU_DA_OFFSET));
    if (hmac_rx_amsdu_check_frame_da(msdu_state, msdu) != OAL_SUCC) {
        oam_stat_vap_incr(0, rx_no_buff_dropped, 1);
        return OAL_FAIL;
    }
    msdu_state->uc_flag = OAL_FALSE;

    if (hmac_rx_amsdu_check_frame_len(msdu_state->pst_curr_netbuf, msdu_state,
        submsdu_len, submsdu_pad_len) != OAL_SUCC) {
        oam_stat_vap_incr(0, rx_no_buff_dropped, 1);
        hmac_rx_free_amsdu_netbuf(msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }
#ifdef _PRE_WINDOWS_SUPPORT
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);    /* MPDU的控制信息 */
    /* 针对当前的netbuf，申请新的subnetbuf，并设置对应的netbuf的信息，赋值给对应的msdu */
    msdu->pst_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
        (rx_ctrl->uc_mac_header_len + MAC_SUBMSDU_HEADER_LEN + submsdu_len + submsdu_pad_len),
        OAL_NETBUF_PRIORITY_MID);
    msdu->pst_netbuf->dev = netbuf->dev;
#else
    /* 针对当前的netbuf，申请新的subnetbuf，并设置对应的netbuf的信息，赋值给对应的msdu */
    msdu->pst_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, (MAC_SUBMSDU_HEADER_LEN + submsdu_len + submsdu_pad_len),
        OAL_NETBUF_PRIORITY_MID);
#endif
    if (msdu->pst_netbuf == NULL) {
        oam_error_log3(0, 0, "{hmac_rx_parse_amsdu::alloc netbuf fail.submsdu len %d, pad_len %d, total_len %d}",
            submsdu_len, submsdu_pad_len, (MAC_SUBMSDU_HEADER_LEN + submsdu_len + submsdu_pad_len));
        oam_stat_vap_incr(0, rx_no_buff_dropped, 1);
        hmac_rx_free_amsdu_netbuf(msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }
    oal_mem_netbuf_trace(msdu->pst_netbuf, OAL_TRUE);

    /* 针对每一个子msdu，修改netbuf的end、data、tail、len指针 */
    /* length形式以太头需要向前扩充一个MAC头长度，因此netbuf的headroom全部预留14字节 */
    oal_netbuf_put(msdu->pst_netbuf, submsdu_len + MAC_SUBMSDU_HEADER_LEN);
    oal_netbuf_pull(msdu->pst_netbuf, MAC_SUBMSDU_HEADER_LEN);
    if (memcpy_s(msdu->pst_netbuf->data, submsdu_len + submsdu_pad_len,
        (submsdu_hdr + MAC_SUBMSDU_HEADER_LEN), submsdu_len) != EOK) {
        oam_warning_log4(0, OAM_SF_RX,
            "{hmac_rx_parse_amsdu::memcpy fail. submsdu len %d, pad_len %d. msdu_nums %d, procd_msdu %d.}",
            submsdu_len, submsdu_pad_len, msdu_state->uc_msdu_nums_in_netbuf, msdu_state->uc_procd_msdu_in_netbuf);
    }

    msdu_state->us_submsdu_offset += submsdu_len + submsdu_pad_len + MAC_SUBMSDU_HEADER_LEN;
    *need_free = OAL_TRUE;
    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_rx_parse_amsdu_post_proc(dmac_msdu_proc_state_stru *msdu_state,
    oal_bool_enum_uint8 need_free, mac_msdu_proc_status_enum_uint8 *proc_state)
{
    oal_netbuf_stru *netbuf_prev = NULL;
    mac_rx_ctl_stru *rx_ctrl = NULL; /* MPDU的控制信息 */
    /* 增加当前已处理的msdu的个数 */
    msdu_state->uc_procd_msdu_in_netbuf++;
    /* 当前的netbuf中的还有msdu待处理 */
    if (msdu_state->uc_procd_msdu_in_netbuf < msdu_state->uc_msdu_nums_in_netbuf) {
        *proc_state = MAC_PROC_MORE_MSDU;
        return OAL_SUCC;
    }

    if (oal_unlikely(msdu_state->uc_procd_msdu_in_netbuf > msdu_state->uc_msdu_nums_in_netbuf)) {
        *proc_state = MAC_PROC_ERROR;
        oam_warning_log0(0, OAM_SF_RX,
            "{hmac_rx_parse_amsdu::pen_proc_state is err for procd_msdu_in_netbuf > msdu_nums_in_netbuf.}");
        hmac_rx_free_amsdu_netbuf(msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }

    /* 当前的netbuf中msdu处理完毕 */
    msdu_state->uc_procd_netbuf_nums++;

    netbuf_prev = msdu_state->pst_curr_netbuf;

    /* 获取该MPDU对应的下一个netbuf的内容 */
    if (msdu_state->uc_procd_netbuf_nums < msdu_state->uc_netbuf_nums_in_mpdu) {
        *proc_state = MAC_PROC_MORE_MSDU;
        msdu_state->pst_curr_netbuf = oal_netbuf_next(msdu_state->pst_curr_netbuf);
        msdu_state->puc_curr_netbuf_data = oal_netbuf_data(msdu_state->pst_curr_netbuf);

        rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(msdu_state->pst_curr_netbuf);
        msdu_state->uc_msdu_nums_in_netbuf = rx_ctrl->uc_msdu_in_buffer;
        msdu_state->us_submsdu_offset = 0;
        msdu_state->uc_procd_msdu_in_netbuf = 0;
        /* amsdu 第二个netbuf len是0, 先put到最大size */
        oal_netbuf_put(msdu_state->pst_curr_netbuf, WLAN_MEM_NETBUF_SIZE2);
    } else if (msdu_state->uc_procd_netbuf_nums == msdu_state->uc_netbuf_nums_in_mpdu) {
        /* MPDU全部处理完毕 */
        *proc_state = MAC_PROC_LAST_MSDU;
    } else {
        *proc_state = MAC_PROC_ERROR;
        oam_warning_log0(0, OAM_SF_RX,
            "{hmac_rx_parse_amsdu::pen_proc_state is err for procd_netbuf_nums > netbuf_nums_in_mpdu.}");
        hmac_rx_free_amsdu_netbuf(msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }
    if (need_free) {
        oal_netbuf_free(netbuf_prev);
    }
    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_parse_amsdu
 * 功能描述  : 解析出每一个AMSDU中的MSDU
 * 1.日    期  : 2012年11月20日
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_rx_parse_amsdu(oal_netbuf_stru *netbuf, dmac_msdu_stru *pst_msdu,
    dmac_msdu_proc_state_stru *pst_msdu_state, mac_msdu_proc_status_enum_uint8 *pen_proc_state)
{
    oal_bool_enum_uint8 b_need_free_netbuf = FALSE;
    uint32_t ret;

    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_parse_amsdu::netbuf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 首次进入该函数解析AMSDU */
    hmac_rx_init_amsdu_state(netbuf, pst_msdu_state);

    /* 1个netbuf 只包含一个msdu */
    if (pst_msdu_state->uc_msdu_nums_in_netbuf == 1) {
        ret = hmac_parse_large_amsdu(pst_msdu, pst_msdu_state, &b_need_free_netbuf, pen_proc_state);
    } else {
        ret = hmac_parse_small_amsdu(netbuf, pst_msdu, pst_msdu_state, &b_need_free_netbuf);
    }

    if (ret != OAL_SUCC) {
        return ret;
    }

    return hmac_rx_parse_amsdu_post_proc(pst_msdu_state, b_need_free_netbuf, pen_proc_state);
}

/*
 * 函 数 名  : hmac_rx_clear_amsdu_last_netbuf_pointer
 * 功能描述  : 设置amsdu 最后一个 netbuf next指针为null
 * 1.日    期  : 2017年07月25日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_clear_amsdu_last_netbuf_pointer(oal_netbuf_stru *netbuf, uint16_t uc_num_buf)
{
    if (uc_num_buf == 0 && netbuf != NULL) {
        netbuf->next = NULL;
        return;
    }
    while (netbuf != NULL) {
        uc_num_buf--;
        if (uc_num_buf == 0) {
            netbuf->next = NULL;
            break;
        }
        netbuf = oal_get_netbuf_next(netbuf);
    }
}

OAL_STATIC uint32_t hmac_rx_lan_frame_amsdu(uint8_t vap_idx, uint16_t user_idx,
    oal_netbuf_stru *netbuf, oal_netbuf_head_stru *w2w_netbuf_hdr)
{
    dmac_msdu_proc_state_stru msdu_state = { 0 }; /* 记录MPDU的处理信息 */
    uint32_t ret;
    mac_msdu_proc_status_enum_uint8 process_state = MAC_PROC_BUTT; /* 解析AMSDU的状态 */
    dmac_msdu_stru msdu; /* 保存解析出来的每一个MSDU */
    uint8_t *addr = NULL;
    mac_ieee80211_frame_stru *frame_hdr;
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    frame_hdr = (mac_ieee80211_frame_stru *)(uintptr_t)mac_get_rx_cb_mac_header_addr(rx_ctrl);
#ifdef _PRE_WINDOWS_SUPPORT
    uint8_t *mac_header = netbuf->data;
#endif

    memset_s(&msdu, sizeof(dmac_msdu_stru), 0, sizeof(dmac_msdu_stru));
    mac_get_transmit_addr(frame_hdr, &addr);
    oal_set_mac_addr(msdu.auc_ta, addr);
    /* amsdu 最后一个netbuf next指针设为 NULL 出错时方便释放amsdu netbuf */
    hmac_rx_clear_amsdu_last_netbuf_pointer(netbuf, rx_ctrl->bit_buff_nums);

    do {
        /* 获取下一个要转发的msdu */
        ret = hmac_rx_parse_amsdu(netbuf, &msdu, &msdu_state, &process_state);
        if (ret != OAL_SUCC) {
            oam_warning_log1(vap_idx, OAM_SF_RX,
                "{hmac_rx_lan_frame_amsdu::hmac_rx_parse_amsdu failed[%d].}", ret);
            return ret;
        }
#ifdef _PRE_WINDOWS_SUPPORT
        ret = memmove_s(msdu.pst_netbuf->data + rx_ctrl->uc_mac_header_len, msdu.pst_netbuf->len,
            msdu.pst_netbuf->data, msdu.pst_netbuf->len);
        if (ret != EOK) {
            oam_error_log0(0, OAM_SF_RX, "hmac_rx_lan_frame_classify::memmove fail [%d].", ret);
            return;
        }
        ret = memcpy_s(msdu.pst_netbuf->data, msdu.pst_netbuf->len + rx_ctrl->uc_mac_header_len, mac_header,
            rx_ctrl->uc_mac_header_len);
        if (ret != EOK) {
            oam_error_log0(0, OAM_SF_RX, "hmac_rx_lan_frame_classify::memcpy fail [%d].", ret);
            return;
        }
        msdu.pst_netbuf->len += rx_ctrl->uc_mac_header_len;
        oal_skb_to_wdi(msdu.pst_netbuf);
#else
        hmac_rx_transmit_msdu(vap_idx, user_idx, &msdu, w2w_netbuf_hdr);
#endif
    } while (process_state != MAC_PROC_LAST_MSDU);
    return OAL_SUCC;
}

/*
 * 函 数 名  : hmac_rx_lan_frame_classify
 * 功能描述  : HMAC接收模块，WLAN到LAN的转发接口
 * 1.日    期  : 2012年12月6日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_lan_frame_classify(dpe_wlan_vap_stru *dpe_vap, oal_netbuf_stru *netbuf,
    mac_ieee80211_frame_stru *pst_frame_hdr)
{
    mac_rx_ctl_stru *pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    dpe_wlan_user_stru *dpe_user = NULL;
    oal_netbuf_head_stru st_w2w_netbuf_hdr;
    frw_event_hdr_stru st_event_hdr;

    if (pst_rx_ctrl == NULL) {
        return;
    }
    dpe_user = dpe_wlan_user_get(pst_rx_ctrl->bit_rx_user_id);
#ifdef _PRE_WINDOWS_SUPPORT
    netbuf->dev = dpe_vap->pst_net_device;
#endif

    if (oal_unlikely(dpe_user == NULL)) {
        hmac_rx_lan_frame_no_user_log(dpe_vap->uc_vap_id, netbuf, pst_rx_ctrl);
        oal_netbuf_free(netbuf);
        return;
    }

    oal_netbuf_list_head_init(&st_w2w_netbuf_hdr);

    /* offload 下amsdu帧可能分成多个seq相同的netbuf上报 只有在last amsdu buffer才能置位
        否则容易导致重排序缓冲区在弱信号下强制移窗后收到ba start之前seq的amsdu帧丢帧
     */
    if ((pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE) || (pst_rx_ctrl->bit_is_last_buffer == OAL_TRUE)) {
        hmac_ba_update_rx_bitmap(dpe_user->us_assoc_id, pst_frame_hdr);
    }
#if (defined(_PRE_WLAN_FEATURE_SNIFFER) && defined(CONFIG_HW_SNIFFER))
    proc_sniffer_write_file(NULL, 0, (uint8_t *)oal_netbuf_payload(netbuf), pst_rx_ctrl->us_frame_len, 0);
#endif
    if (pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE) {
        /* 情况一:不是AMSDU聚合，则该MPDU对应一个MSDU，同时对应一个NETBUF */
        netbuf = hmac_rx_lan_frame_feature_proc(dpe_vap->uc_vap_id, dpe_user->us_assoc_id, netbuf, pst_frame_hdr);
        if (netbuf == NULL) {
            return;
        }
        if (hmac_rx_lan_frame_normal(dpe_vap->uc_vap_id,
            dpe_user->us_assoc_id, netbuf, &st_w2w_netbuf_hdr) != OAL_SUCC) {
            return;
        }
    } else {
        /* 情况二:AMSDU聚合 */
        if (hmac_rx_lan_frame_amsdu(dpe_vap->uc_vap_id,
            dpe_user->us_assoc_id, netbuf, &st_w2w_netbuf_hdr) != OAL_SUCC) {
            return;
        }
    }

    if (dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        /*  当msdu.da在vap下,进行wlan_to_wlan转发,防止to_lan到内核不进行转发 */
        /*  将MSDU链表交给发送流程处理 */
        if (oal_netbuf_list_empty(&st_w2w_netbuf_hdr) == OAL_FALSE && oal_netbuf_tail(&st_w2w_netbuf_hdr) != NULL &&
            oal_netbuf_peek(&st_w2w_netbuf_hdr) != NULL) {
            oal_netbuf_next((oal_netbuf_tail(&st_w2w_netbuf_hdr))) = NULL;
            oal_netbuf_prev((oal_netbuf_peek(&st_w2w_netbuf_hdr))) = NULL;
            st_event_hdr.uc_chip_id = dpe_vap->uc_chip_id;
            st_event_hdr.uc_device_id = dpe_vap->uc_device_id;
            st_event_hdr.uc_vap_id = dpe_vap->uc_vap_id;
            hmac_rx_transmit_to_wlan(&st_event_hdr, &st_w2w_netbuf_hdr);
        }
    }
}

OAL_STATIC void hmac_rx_set_hdr_info(dpe_wlan_vap_stru *dpe_vap, oal_netbuf_head_stru *netbuf_header,
    oal_netbuf_stru **temp_netbuf, uint16_t *netbuf_num, frw_event_hdr_stru *event_hdr)
{
    *temp_netbuf = oal_netbuf_peek(netbuf_header);
    *netbuf_num = (uint16_t)oal_netbuf_get_buf_num(netbuf_header);
    event_hdr->uc_chip_id = dpe_vap->uc_chip_id;
    event_hdr->uc_device_id = dpe_vap->uc_device_id;
    event_hdr->uc_vap_id = dpe_vap->uc_vap_id;
}

OAL_STATIC void hmac_rx_non_amsdu_prepare_msdu_list(uint8_t vap_idx, oal_netbuf_head_stru *netbuf_header,
    oal_netbuf_stru *ori_netbuf, mac_rx_ctl_stru *ori_rx_ctrl, mac_ieee80211_frame_stru *frame_hdr)
{
    uint32_t ret;
    uint8_t *puc_addr = NULL;
    mac_rx_ctl_stru *new_rx_ctrl = NULL;
    oal_netbuf_stru *new_netbuf = NULL;
    uint8_t auc_sa[WLAN_MAC_ADDR_LEN], auc_da[WLAN_MAC_ADDR_LEN];
    dpe_wlan_user_stru *dpe_user = dpe_wlan_user_get(ori_rx_ctrl->bit_rx_user_id);
    if (oal_unlikely(dpe_user == NULL)) {
        /* 打印此net buf相关信息 */
        mac_rx_report_80211_frame(vap_idx, (uint8_t *)ori_rx_ctrl, ori_netbuf,
            OAM_OTA_TYPE_RX_HMAC_CB);
        return;
    }

    new_netbuf = hmac_defrag_process(ori_rx_ctrl->bit_rx_user_id, ori_netbuf, ori_rx_ctrl->uc_mac_header_len);
    if (new_netbuf == NULL) {
        return;
    }

    /* 重新获取该MPDU的控制信息 */
    new_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(new_netbuf);

    /* 从MAC头中获取源地址和目的地址 */
    mac_rx_get_sa(frame_hdr, &puc_addr);
    oal_set_mac_addr(&auc_sa[0], puc_addr);

    mac_rx_get_da(frame_hdr, &puc_addr);
    oal_set_mac_addr(&auc_da[0], puc_addr);

    /* 将netbuf的data指针指向mac frame的payload处，也就是指向了8字节的snap头 */
    oal_netbuf_pull(new_netbuf, new_rx_ctrl->uc_mac_header_len);

    /* 将MSDU转化为以太网格式的帧 */
    ret = hmac_rx_frame_80211_to_eth(new_netbuf, auc_da, auc_sa);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(new_netbuf);
        return;
    }

#ifdef _PRE_WLAN_PKT_TIME_STAT
    memset_s(oal_netbuf_cb(new_netbuf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
    memset_s(oal_netbuf_cb(new_netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif
    /* 将MSDU加入到netbuf链的最后 */
    oal_netbuf_add_to_list_tail(new_netbuf, netbuf_header);
}

/*
 * 函 数 名  : hmac_rx_prepare_msdu_list_to_wlan
 * 功能描述  : 解析MPDU，如果是非AMSDU，则将MSDU还原为以太网格式的帧，并加入到
 * 1.日    期  : 2013年12月17日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_prepare_msdu_list_to_wlan(uint8_t vap_idx,
    oal_netbuf_head_stru *netbuf_header, oal_netbuf_stru *netbuf, mac_ieee80211_frame_stru *pst_frame_hdr)
{
    mac_rx_ctl_stru *pst_rx_ctrl = NULL;                      /* 指向MPDU控制块信息的指针 */
    dmac_msdu_stru st_msdu;                                           /* 保存解析出来的每一个MSDU */
    mac_msdu_proc_status_enum_uint8 en_process_state = MAC_PROC_BUTT; /* 解析AMSDU的状态 */
    dmac_msdu_proc_state_stru st_msdu_state = { 0 };                  /* 记录MPDU的处理信息 */
    uint32_t ret;

    /* 解析MPDU-->MSDU */ /* 将MSDU组成netbuf链 */
    oal_mem_netbuf_trace(netbuf, OAL_TRUE);

    /* 获取该MPDU的控制信息 */
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (pst_rx_ctrl == NULL) {
        oam_warning_log0(vap_idx, OAM_SF_RX, "{hmac_rx_prepare_msdu_list_to_wlan::pst_rx_ctrl is NULL.}");
        return;
    }
    /* 情况一:不是AMSDU聚合，则该MPDU对应一个MSDU，同时对应一个NETBUF,将MSDU还原
       成以太网格式帧以后直接加入到netbuf链表最后
    */
    if (pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE) {
        hmac_rx_non_amsdu_prepare_msdu_list(vap_idx, netbuf_header, netbuf, pst_rx_ctrl, pst_frame_hdr);
    } else { /* 情况二:AMSDU聚合 */
        memset_s(&st_msdu, sizeof(dmac_msdu_stru), 0, sizeof(dmac_msdu_stru));
        st_msdu_state.uc_procd_netbuf_nums = 0;
        st_msdu_state.uc_procd_msdu_in_netbuf = 0;
        /* amsdu 最后一个netbuf next指针设为 NULL 出错时方便释放amsdu netbuf */
        hmac_rx_clear_amsdu_last_netbuf_pointer(netbuf, pst_rx_ctrl->bit_buff_nums);
        do {
            /* 获取下一个要转发的msdu */
            ret = hmac_rx_parse_amsdu(netbuf, &st_msdu, &st_msdu_state, &en_process_state);
            if (ret != OAL_SUCC) {
                oam_warning_log1(vap_idx, OAM_SF_RX,
                    "{hmac_rx_prepare_msdu_list_to_wlan::hmac_rx_parse_amsdu failed[%d].}", ret);
                return;
            }

            /* 将MSDU转化为以太网格式的帧 */
            ret = hmac_rx_frame_80211_to_eth(st_msdu.pst_netbuf, st_msdu.auc_da, st_msdu.auc_sa);
            if (ret != OAL_SUCC) {
                oal_netbuf_free(st_msdu.pst_netbuf);
                continue;
            }
            /* 将MSDU加入到netbuf链的最后 */
            oal_netbuf_add_to_list_tail(st_msdu.pst_netbuf, netbuf_header);
        } while (en_process_state != MAC_PROC_LAST_MSDU);
    }
}

OAL_STATIC uint32_t hmac_rx_data_ap_designated_user_process(dpe_wlan_vap_stru *dpe_vap,
    mac_ieee80211_frame_stru *pst_frame_hdr, oal_netbuf_stru *netbuf, uint8_t uc_buf_nums)
{
    uint32_t rslt = OAL_FAIL;
    uint16_t user_idx;
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    uint8_t *puc_da = NULL;
    dpe_wlan_user_stru *dpe_user = NULL;

    /* 获取接收端地址  */
    mac_rx_get_da(pst_frame_hdr, &puc_da);

    /* 四地址AMSDU报文不进行WLAN_TO_WLAN转发 */
    user_idx = g_wlan_spec_cfg->invalid_user_id;
    if ((mac_is_4addr(pst_frame_hdr) == OAL_FALSE) ||
        ((mac_is_4addr(pst_frame_hdr) == OAL_TRUE) &&
        (rx_ctrl != NULL && rx_ctrl->bit_amsdu_enable == OAL_FALSE))) {
        /* 获取目的地址对应的用户指针 */
        rslt = dpe_vap_find_user_by_macaddr(dpe_vap->uc_vap_id, puc_da, &user_idx);
        if (rslt == OAL_ERR_CODE_PTR_NULL) { /* 查找用户失败 */
            /* 释放当前处理的MPDU占用的netbuf */
            hmac_rx_free_netbuf(netbuf, (uint16_t)uc_buf_nums);
            oam_stat_vap_incr(dpe_vap->uc_vap_id, rx_da_check_dropped, 1);
            return OAL_RETURN;
        }
    }

    /* 没有找到对应的用户 */
    if (rslt != OAL_SUCC) {
        oam_info_log0(dpe_vap->uc_vap_id, OAM_SF_RX, "{ap_tcp_ack_opt::the frame is a unique frame.}");
        /* 目的用户不在AP的用户表中，调用wlan_to_lan转发接口 */
        hmac_rx_lan_frame_classify(dpe_vap, netbuf, pst_frame_hdr);
        return OAL_RETURN;
    }

    /* 目的用户已在AP的用户表中，进行WLAN_TO_WLAN转发 */
    dpe_user = dpe_wlan_user_get(user_idx);
    if (dpe_user == NULL) {
        oam_warning_log1(dpe_vap->uc_vap_id, OAM_SF_RX, "{ap_tcp_ack_opt::dpe_user[%d] null.}", user_idx);
        oam_stat_vap_incr(dpe_vap->uc_vap_id, rx_da_check_dropped, 1);

        hmac_rx_free_netbuf(netbuf, (uint16_t)uc_buf_nums);
        return OAL_RETURN;
    }

    if (dpe_user->en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        oam_warning_log0(dpe_vap->uc_vap_id, OAM_SF_RX, "{ap_tcp_ack_opt::the station is not associat with ap.}");
        oam_stat_vap_incr(dpe_vap->uc_vap_id, rx_da_check_dropped, 1);

        hmac_rx_free_netbuf(netbuf, (uint16_t)uc_buf_nums);
        hmac_mgmt_send_deauth_frame(dpe_vap->uc_vap_id, puc_da, MAC_NOT_AUTHED, OAL_FALSE);
        return OAL_RETURN;
    }
    return OAL_CONTINUE;
}

OAL_STATIC void hmac_rx_msdu_to_sending_process(oal_netbuf_head_stru *w2w_netbuf_hdr, frw_event_hdr_stru *event_hdr)
{
    /*  将MSDU链表交给发送流程处理 */
    if ((oal_netbuf_list_empty(w2w_netbuf_hdr) == OAL_FALSE) && (oal_netbuf_tail(w2w_netbuf_hdr) != NULL) &&
        (oal_netbuf_peek(w2w_netbuf_hdr) != NULL)) {
        oal_netbuf_next((oal_netbuf_tail(w2w_netbuf_hdr))) = NULL;
        oal_netbuf_prev((oal_netbuf_peek(w2w_netbuf_hdr))) = NULL;

        hmac_rx_transmit_to_wlan(event_hdr, w2w_netbuf_hdr);
    }

    if (hmac_get_rxthread_enable() == OAL_TRUE) {
        g_hisi_softwdt_check.rxshed_msdu++;
        hmac_rxdata_sched();
    }
}

/*
 * 功能描述  : AP模式下，HMAC模块接收WLAN_DRX事件(数据帧)的处理函数
 */
void hmac_rx_process_data_ap_tcp_ack_opt(uint8_t vap_idx, oal_netbuf_head_stru *netbuf_header)
{
    frw_event_hdr_stru event_hdr;
    mac_ieee80211_frame_stru *pst_frame_hdr = NULL;      /* 保存mac帧的指针 */
    mac_ieee80211_frame_stru *pst_copy_frame_hdr = NULL; /* 保存mac帧的指针 */
    uint8_t *puc_da = NULL;                              /* 保存用户目的地址的指针 */
    mac_rx_ctl_stru *rx_ctrl = NULL;                     /* 每一个MPDU的控制信息 */
    uint16_t us_netbuf_num = 0;                          /* netbuf链表的个数 */
    uint8_t uc_buf_nums;                                 /* 每个mpdu占有buf的个数 */
    oal_netbuf_stru *netbuf = NULL;                      /* 用于保存当前处理的MPDU的第一个netbuf指针 */
    oal_netbuf_stru *pst_temp_netbuf = NULL;             /* 用于临时保存下一个需要处理的netbuf指针 */
    oal_netbuf_stru *netbuf_copy = NULL;                 /* 用于保存组播帧copy */
    oal_netbuf_head_stru st_w2w_netbuf_hdr;              /* 保存wlan to wlan的netbuf链表的头 */
    dpe_wlan_vap_stru *dpe_vap = dpe_wlan_vap_get(vap_idx);

    if (dpe_vap == NULL) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_rx_process_data_ap::vap[%d] null.}", vap_idx);
        return;
    }
    /* 循环收到的每一个MPDU，处情况如下:
        1、组播帧时，调用WLAN TO WLAN和WLAN TO LAN接口
        2、其他，根据实际情况，调用WLAN TO LAN接口或者WLAN TO WLAN接口 */
    oal_netbuf_list_head_init(&st_w2w_netbuf_hdr);
    hmac_rx_set_hdr_info(dpe_vap, netbuf_header, &pst_temp_netbuf, &us_netbuf_num, &event_hdr);

    while (us_netbuf_num != 0) {
        netbuf = pst_temp_netbuf;
        if (netbuf == NULL) {
            break;
        }

        rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

        /* 获取帧头信息 */
        pst_frame_hdr = (mac_ieee80211_frame_stru *)(uintptr_t)mac_get_rx_cb_mac_header_addr(rx_ctrl);

        /* 获取当前MPDU占用的netbuf数目 */
        uc_buf_nums = rx_ctrl->bit_buff_nums;

        /* 获取下一个要处理的MPDU */
        oal_netbuf_get_appointed_netbuf(netbuf, uc_buf_nums, &pst_temp_netbuf);
        us_netbuf_num = oal_sub(us_netbuf_num, uc_buf_nums);

        dpe_vap = dpe_wlan_vap_get(rx_ctrl->uc_mac_vap_id);
        if (oal_unlikely(dpe_vap == NULL)) {
            oam_warning_log0(rx_ctrl->bit_vap_id, OAM_SF_RX, "{hmac_rx_process_data_ap::vap null.}");
            hmac_rx_free_netbuf(netbuf, (uint16_t)uc_buf_nums);
            continue;
        }

        /* 获取接收端地址  */
        mac_rx_get_da(pst_frame_hdr, &puc_da);

        /* 目的地址为组播地址时，进行WLAN_TO_WLAN和WLAN_TO_LAN的转发 */
        if (ether_is_multicast(puc_da)) {
            oam_info_log0(event_hdr.uc_vap_id, OAM_SF_RX, "{hmac_rx_lan_frame_classify::frame is a group frame.}");
            oam_stat_vap_incr(dpe_vap->uc_vap_id, rx_mcast_cnt, 1);

            if (hmac_rx_copy_netbuff(&netbuf_copy, netbuf, rx_ctrl->uc_mac_vap_id, &pst_copy_frame_hdr) != OAL_SUCC) {
                oam_warning_log0(event_hdr.uc_vap_id, OAM_SF_RX, "{ap_tcp_ack_opt::send mcast pkt to air fail.}");

                oam_stat_vap_incr(dpe_vap->uc_vap_id, rx_no_buff_dropped, 1);
                hmac_rx_free_netbuf(netbuf, (uint16_t)uc_buf_nums);
                continue;
            }

            hmac_rx_lan_frame_classify(dpe_vap, netbuf, pst_frame_hdr);  // 上报网络层
            /* 将MPDU解析成单个MSDU，把所有的MSDU组成一个netbuf链 */
            hmac_rx_prepare_msdu_list_to_wlan(dpe_vap->uc_vap_id, &st_w2w_netbuf_hdr, netbuf_copy, pst_copy_frame_hdr);
            continue;
        }
        /* 用户级别处理 */
        if (hmac_rx_data_ap_designated_user_process(dpe_vap, pst_frame_hdr, netbuf, uc_buf_nums) == OAL_RETURN) {
            continue;
        }
        /* 将MPDU解析成单个MSDU，把所有的MSDU组成一个netbuf链 */
        hmac_rx_prepare_msdu_list_to_wlan(dpe_vap->uc_vap_id, &st_w2w_netbuf_hdr, netbuf, pst_frame_hdr);
    }
    /*  将MSDU链表交给发送流程处理 */
    hmac_rx_msdu_to_sending_process(&st_w2w_netbuf_hdr, &event_hdr);
}

/*
 * 函 数 名  : hmac_rx_lan_frame
 * 1.日    期  : 2015年1月3日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_lan_frame(oal_netbuf_head_stru *netbuf_header)
{
    uint32_t netbuf_num;
    oal_netbuf_stru *pst_temp_netbuf = NULL;
    oal_netbuf_stru *netbuf = NULL;
    uint8_t uc_buf_nums;
    mac_rx_ctl_stru *pst_rx_ctrl = NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = NULL;
    dpe_wlan_vap_stru *dpe_vap = NULL;

    netbuf_num = oal_netbuf_get_buf_num(netbuf_header);
    pst_temp_netbuf = oal_netbuf_peek(netbuf_header);

    while (netbuf_num != 0) {
        netbuf = pst_temp_netbuf;
        if (netbuf == NULL) {
            break;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        pst_frame_hdr = (mac_ieee80211_frame_stru *)(uintptr_t)mac_get_rx_cb_mac_header_addr(pst_rx_ctrl);
        uc_buf_nums = pst_rx_ctrl->bit_buff_nums;

        oal_netbuf_get_appointed_netbuf(netbuf, uc_buf_nums, &pst_temp_netbuf);
        netbuf_num = oal_sub(netbuf_num, uc_buf_nums);

        dpe_vap = dpe_wlan_vap_get(pst_rx_ctrl->uc_mac_vap_id);
        if (dpe_vap == NULL) {
            oam_error_log1(0, OAM_SF_RX, "{hmac_rx_lan_frame::vap null. vap_id:%u}", pst_rx_ctrl->uc_mac_vap_id);
            continue;
        }

        hmac_rx_lan_frame_classify(dpe_vap, netbuf, pst_frame_hdr);
    }

    if (hmac_get_rxthread_enable() == OAL_TRUE) {
        g_hisi_softwdt_check.rxshed_lan++;
        hmac_rxdata_sched();
    }
}

/*
 * 函 数 名  : hmac_rx_process_data_filter
 * 1.日    期  : 2015年1月3日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_process_data_filter(oal_netbuf_head_stru *netbuf_header,
    oal_netbuf_stru *temp_netbuf, uint16_t netbuf_num)
{
    oal_netbuf_stru *netbuf = NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = NULL;
    hmac_user_stru *pst_hmac_user = NULL;
    uint32_t ret;
    uint8_t uc_buf_nums, uc_vap_id, uc_netbuf_num;
    oal_bool_enum_uint8 en_is_ba_buf;
    uint8_t uc_vap_st_id = oal_board_get_service_vap_start_id();
    uint8_t uc_vap_max_id = mac_chip_get_service_vap_limit_max_id();

    while (netbuf_num != 0) {
        en_is_ba_buf = OAL_FALSE;
        netbuf = temp_netbuf;
        if (netbuf == NULL) {
            oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_process_data_filter::netbuf_num = %d}", netbuf_num);
            break;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        uc_buf_nums = pst_rx_ctrl->bit_buff_nums;

        /* 获取下一个要处理的MPDU */
        oal_netbuf_get_appointed_netbuf(netbuf, uc_buf_nums, &temp_netbuf);
        netbuf_num = oal_sub(netbuf_num, uc_buf_nums);

        uc_vap_id = pst_rx_ctrl->uc_mac_vap_id;
        /*  start, modified: 2015/04/08 */
        /* 双芯片下，0和1都是配置vap id，因此这里需要采用业务vap 其实id和整板最大vap mac num值来做判断 */
        if (uc_vap_st_id > uc_vap_id || uc_vap_id >= uc_vap_max_id) {
            oam_error_log1(0, OAM_SF_RX, "{hmac_rx_process_data_filter::Invalid vap_id.vap_id[%u]}", uc_vap_id);
            hmac_rx_free_netbuf_list(netbuf_header, uc_buf_nums);
            continue;
        } /*  end */

        // make sure ta user idx is exist
        pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_rx_ctrl->bit_rx_user_id);
        if (pst_hmac_user == NULL) {
            oam_warning_log1(0, OAM_SF_ROAM, "{hmac_rx_process_data_filter::pst_hmac_user[%d] null.}",
                pst_rx_ctrl->bit_rx_user_id);
            hmac_rx_free_netbuf_list(netbuf_header, uc_buf_nums);
            continue;
        }

        ret = hmac_ba_filter_serv(pst_hmac_user, netbuf, netbuf_header, &en_is_ba_buf);
        if (ret != OAL_SUCC) {
            hmac_rx_free_netbuf_list(netbuf_header, uc_buf_nums);
            continue;
        }

        if (en_is_ba_buf == OAL_TRUE) {
            continue;
        }

        /* 如果不buff进reorder队列，则重新挂到链表尾，保序 */
        for (uc_netbuf_num = 0; uc_netbuf_num < uc_buf_nums; uc_netbuf_num++) {
            netbuf = oal_netbuf_delist_nolock(netbuf_header);
            if (oal_likely(netbuf != NULL)) {
                oal_netbuf_list_tail_nolock(netbuf_header, netbuf);
            } else {
                oam_warning_log0(pst_rx_ctrl->bit_vap_id, OAM_SF_RX, "{hmac_rx_process_data_filter::no buff error.}");
            }
        }
    }
}

/*
 * 功能描述 : STA和AP模式下，WLAN_DRX事件(数据帧)的处理函数
 * 1.日    期  : 2020年2月11日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rx_process_data_tcp_ack_opt(hmac_vap_stru *hmac_vap, oal_netbuf_head_stru *netbuf_header)
{
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        /* 将需要上报的帧逐一出队处理 */
        hmac_rx_lan_frame(netbuf_header);
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_process_data_ap_tcp_ack_opt(hmac_vap->st_vap_base_info.uc_vap_id, netbuf_header);
    } else {
        oam_warning_log1(0, OAM_SF_RX, "{RX FAIL! VAP MODE[%d]!.}", hmac_vap->st_vap_base_info.en_vap_mode);
    }
}

OAL_STATIC void hmac_rx_process_data(hmac_device_stru *pst_hmac_device, hmac_vap_stru *pst_hmac_vap,
    dmac_wlan_drx_event_stru *wlan_rx_event)
{
    oal_netbuf_head_stru st_netbuf_header;      /* 存储上报给网络层的数据 */
    oal_netbuf_stru *pst_temp_netbuf = NULL;
    oal_netbuf_stru *netbuf = wlan_rx_event->pst_netbuf;
    uint16_t netbuf_num = wlan_rx_event->us_netbuf_num;

    /* 将所有netbuff全部入链表 */
    oal_netbuf_list_head_init(&st_netbuf_header);
    while (netbuf_num != 0) {
        pst_temp_netbuf = netbuf;
        if (pst_temp_netbuf == NULL) {
            break;
        }

        netbuf = oal_netbuf_next(pst_temp_netbuf);

        hmac_stat_device_rx_msdu(oal_netbuf_len(pst_temp_netbuf));
#ifdef _PRE_WLAN_CHBA_MGMT
        hmac_chba_stat_user_rx_netbuf(pst_temp_netbuf);
#endif
        oal_netbuf_list_tail_nolock(&st_netbuf_header, pst_temp_netbuf);
        netbuf_num--;
    }

    if (netbuf_num != 0) {
        oam_error_log2(0, OAM_SF_RX, "{netbuf_num[%d], event_buf_num[%d].}", netbuf_num, wlan_rx_event->us_netbuf_num);
    }

    hmac_rx_process_data_filter(&st_netbuf_header, wlan_rx_event->pst_netbuf, wlan_rx_event->us_netbuf_num);

#ifdef _PRE_WLAN_TCP_OPT
    if (pst_hmac_device->sys_tcp_rx_ack_opt_enable == OAL_TRUE) {
        hmac_transfer_rx_handle(pst_hmac_device, pst_hmac_vap, &st_netbuf_header);
    }
#endif

    hmac_rx_process_data_tcp_ack_opt(pst_hmac_vap, &st_netbuf_header);
}

/*
 * 功能描述  : HMAC模块接收WLAN_DRX事件(数据帧)的处理函数
 */
uint32_t hmac_rx_process_data_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru *event = NULL;
    frw_event_hdr_stru *event_hdr = NULL;
    dmac_wlan_drx_event_stru *wlan_rx_event = NULL;
    hmac_vap_stru *pst_hmac_vap = NULL;
    hmac_device_stru *pst_hmac_device = NULL;
    oal_netbuf_stru *netbuf = NULL; /* 用于临时保存下一个需要处理的netbuf指针 */
    uint16_t netbuf_num;                   /* netbuf链表的个数 */

    if (oal_unlikely(event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_data_event::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    /* RR性能检测接收流程dmac       to hmac位置打点 */
    hmac_rr_rx_d2h_timestamp();
#endif

    /* 获取事件头和事件结构体指针 */
    event = frw_get_event_stru(event_mem);
    event_hdr = &(event->st_event_hdr);
    wlan_rx_event = (dmac_wlan_drx_event_stru *)(event->auc_event_data);
    netbuf = wlan_rx_event->pst_netbuf;
    netbuf_num = wlan_rx_event->us_netbuf_num;

    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_rx_process_data_event::netbuf_num = %d.}", netbuf_num);
        return OAL_SUCC; /* 这个是事件处理函数，为了防止51的UT挂掉 返回 true */
    }

#if defined(_PRE_WLAN_TCP_OPT)
    pst_hmac_device = hmac_res_get_mac_dev(event_hdr->uc_device_id);
    if (pst_hmac_device == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_rx_process_data_event::pst_hmac_device null.}");
        hmac_rx_free_netbuf(netbuf, netbuf_num);
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif

    if (pst_hmac_device->en_monitor_mode == OAL_TRUE) {
        return hmac_rx_process_data_event_monitor_handle(pst_hmac_device, netbuf, netbuf_num);
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event_hdr->uc_vap_id);
    if (pst_hmac_vap == NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_data_event::pst_hmac_vap null.}");
        hmac_rx_free_netbuf(netbuf, netbuf_num);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* If mib info is null ptr,release the netbuf */
    if (pst_hmac_vap->st_vap_base_info.pst_mib_info == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_rx_process_data_event::pst_mib_info null.}");
        hmac_rx_free_netbuf(netbuf, netbuf_num);
        return OAL_SUCC;
    }

    hmac_rx_process_data(pst_hmac_device, pst_hmac_vap, wlan_rx_event);
    return OAL_SUCC;
}
