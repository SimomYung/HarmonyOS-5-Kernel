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
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_VIP_C

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
OAL_STATIC void hmac_parse_ipv4_packet_ipprot_udp(const struct iphdr *iph, uint32_t iphdr_len, uint32_t netbuf_len)
{
    struct udphdr *uh = NULL;
    if (netbuf_len < iphdr_len + sizeof(struct udphdr)) {
        oam_error_log2(0, OAM_SF_M2U, "{ipv4::netbuf_len[%d], protocol[%d]}", netbuf_len, iph->protocol);
        return;
    }
    uh = (struct udphdr *)((uint8_t *)iph + iphdr_len);
    oal_io_print(WIFI_WAKESRC_TAG "UDP packet, src port:%d, dst port:%d.\n",
        oal_ntoh_16(uh->source), oal_ntoh_16(uh->dest));
#ifdef CONFIG_HUAWEI_DUBAI
    dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP_UDP_V4", "port", oal_ntoh_16(uh->dest));
#endif
}

OAL_STATIC void hmac_parse_ipv4_packet_ipprot_tcp(const struct iphdr *iph, uint32_t iphdr_len, uint32_t netbuf_len)
{
    struct tcphdr *th = NULL;
    if (netbuf_len < iphdr_len + sizeof(struct tcphdr)) {
        oam_error_log2(0, OAM_SF_M2U, "{ipv4::netbuf_len[%d], protocol[%d]}", netbuf_len, iph->protocol);
        return;
    }
    th = (struct tcphdr *)((uint8_t *)iph + iphdr_len);
    oal_io_print(WIFI_WAKESRC_TAG "TCP packet, src port:%d, dst port:%d.\n",
        oal_ntoh_16(th->source), oal_ntoh_16(th->dest));
#ifdef CONFIG_HUAWEI_DUBAI
    HWDUBAI_LOGE("DUBAI_TAG_PACKET_WAKEUP_TCP_V4", "port=%d", oal_ntoh_16(th->dest));
#endif
}

OAL_STATIC void hmac_parse_ipv4_packet_ipprot_icmp(const struct iphdr *iph, uint32_t iphdr_len, uint32_t netbuf_len)
{
    struct icmphdr *icmph = NULL;
    if (netbuf_len < iphdr_len + sizeof(struct icmphdr)) {
        oam_error_log2(0, OAM_SF_M2U, "{ipv4::netbuf_len[%d], protocol[%d]}", netbuf_len, iph->protocol);
        return;
    }
    icmph = (struct icmphdr *)((uint8_t *)iph + iphdr_len);
    oal_io_print(WIFI_WAKESRC_TAG "ICMP packet, type(%d):%s, code:%d.\n", icmph->type,
        ((icmph->type == 0) ? "ping reply" : ((icmph->type == 8) ? "ping request" : "other icmp pkt")), icmph->code);
#ifdef CONFIG_HUAWEI_DUBAI
    dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", (int32_t)iph->protocol);
#endif
}

/*
 * 函 数 名  : hmac_parse_ipv4_packet
 * 功能描述  : 当报文是IPV4类型时，进一步解析type，ipaddr，port
 * 1.日    期  : 2017年2月20日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_parse_ipv4_packet(mac_ether_header_stru *pst_eth, uint32_t netbuf_len)
{
    const struct iphdr *iph = NULL;
    uint32_t iphdr_len;
    /* iphdr: 最小长度为 20 */
    if (netbuf_len < BYTE_OFFSET_20) {
        oam_error_log1(0, OAM_SF_M2U, "{hmac_parse_ipv4_packet::netbuf_len[%d]}", netbuf_len);
        return;
    }
    oal_io_print(WIFI_WAKESRC_TAG "ipv4 packet.\n");
    iph = (struct iphdr *)(pst_eth + 1);
    iphdr_len = iph->ihl * BYTE_OFFSET_4;

    if (iph->protocol == IPPROTO_UDP) {
        hmac_parse_ipv4_packet_ipprot_udp(iph, iphdr_len, netbuf_len);
    } else if (iph->protocol == IPPROTO_TCP) {
        hmac_parse_ipv4_packet_ipprot_tcp(iph, iphdr_len, netbuf_len);
    } else if (iph->protocol == IPPROTO_ICMP) {
        hmac_parse_ipv4_packet_ipprot_icmp(iph, iphdr_len, netbuf_len);
    } else if (iph->protocol == IPPROTO_IGMP) {
        oal_io_print(WIFI_WAKESRC_TAG "IGMP packet.\n");
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", (int32_t)iph->protocol);
#endif
    } else {
        oal_io_print(WIFI_WAKESRC_TAG "other IPv4 packet.\n");
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", (int32_t)iph->protocol);
#endif
    }
}

/*
 * 函 数 名  : hmac_parse_ipv4_packet
 * 功能描述  : 当报文是IPV6类型时，进一步解析type，ipaddr
 * 1.日    期  : 2017年2月20日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_parse_ipv6_packet(mac_ether_header_stru *pst_eth, uint32_t buf_len)
{
    struct ipv6hdr *ipv6h = NULL;
    oal_icmp6hdr_stru *icmph = NULL;
#ifdef CONFIG_HUAWEI_DUBAI
    struct tcphdr *tcph = NULL;
#endif
    size_t len = buf_len;
    if (len < sizeof(struct ipv6hdr)) {
        oam_error_log2(0, OAM_SF_ANY, "{buf_len[%d], ipv6hdr[%d]}", len, sizeof(struct ipv6hdr));
        return;
    }
    len -= sizeof(struct ipv6hdr);
    oal_io_print(WIFI_WAKESRC_TAG "ipv6 packet.\n");
    ipv6h = (struct ipv6hdr *)(pst_eth + 1);
    oal_io_print(WIFI_WAKESRC_TAG "version: %d, payload length: %d, nh->nexthdr: %d. \n",
                 ipv6h->version, oal_ntoh_16(ipv6h->payload_len), ipv6h->nexthdr);
    oal_io_print(WIFI_WAKESRC_TAG "ipv6 src addr:%04x:x:x:x:x:x:x:%04x \n", ipaddr6(ipv6h->saddr));
    oal_io_print(WIFI_WAKESRC_TAG "ipv6 dst addr:%04x:x:x:x:x:x:x:%04x \n", ipaddr6(ipv6h->daddr));
    if (ipv6h->nexthdr == OAL_IPPROTO_ICMPV6) {
        if (len < sizeof(oal_icmp6hdr_stru)) {
            oam_error_log2(0, OAM_SF_ANY, "{buf_len[%d], icmp6hdr[%d]}", len, sizeof(oal_icmp6hdr_stru));
            return;
        }
        icmph = (oal_icmp6hdr_stru *)(ipv6h + 1);
        oal_io_print(WIFI_WAKESRC_TAG "ipv6 nd type: %d. \n", icmph->icmp6_type);
    }
#ifdef CONFIG_HUAWEI_DUBAI
    if (ipv6h->nexthdr == IPPROTO_TCP) {
        tcph = (struct tcphdr *)(ipv6h + 1);
        HWDUBAI_LOGE("DUBAI_TAG_PACKET_WAKEUP_TCP_V6", "port=%d", ntohs(tcph->dest));
    } else {
        HWDUBAI_LOGE("DUBAI_TAG_PACKET_WAKEUP", "protocol=%d", IPPROTO_IPV6);
    }
#endif

    return;
}

/*
 * 函 数 名  : hmac_parse_arp_packet
 * 功能描述  : 当报文是arp类型时，进一步解析type，subtype
 * 1.日    期  : 2017年2月20日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_parse_arp_packet(mac_ether_header_stru *pst_eth, uint32_t buf_len)
{
    struct arphdr *arp = NULL;

    if (buf_len < sizeof(struct arphdr)) {
        oam_error_log1(0, 0, "{hmac_parse_arp_packet::buf_len[%d].}", buf_len);
        return;
    }

    arp = (struct arphdr *)(pst_eth + 1);
    oal_io_print(WIFI_WAKESRC_TAG "ARP packet, hardware type:%d, protocol type:%d, opcode:%d.\n",
                 oal_ntoh_16(arp->ar_hrd), oal_ntoh_16(arp->ar_pro), oal_ntoh_16(arp->ar_op));

    return;
}

/*
 * 函 数 名  : parse_8021x_packet
 * 功能描述  : 当报文是arp类型时，进一步解析type，subtype
 * 1.日    期  : 2017年2月20日
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_parse_8021x_packet(mac_ether_header_stru *pst_eth, uint32_t buf_len)
{
    struct ieee8021x_hdr *hdr = (struct ieee8021x_hdr *)(pst_eth + 1);
    if (buf_len < sizeof(struct ieee8021x_hdr)) {
        oam_error_log1(0, 0, "{hmac_parse_packet::buf_len}", buf_len);
        return;
    }
    oal_io_print(WIFI_WAKESRC_TAG "802.1x frame: version:%d, type:%d, length:%d\n",
                 hdr->version, hdr->type, oal_ntoh_16(hdr->length));

    return;
}

/*
 * 函 数 名  : hmac_parse_packet
 * 功能描述  : 当系统被wifi唤醒时，解析数据报文的格式。
 * 1.日    期  : 2017年2月20日
 *   修改内容  : 新生成函数
 */
void hmac_parse_packet(uint8_t vap_idx, oal_netbuf_stru *netbuf_eth)
{
    uint16_t us_type;
    uint32_t buf_len;
    mac_ether_header_stru *pst_ether_hdr = NULL;
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_eth);
    buf_len = oal_netbuf_len(netbuf_eth);
    if (buf_len < sizeof(mac_ether_header_stru)) {
        oam_error_log1(0, 0, "{hmac_parse_packet::buf_len}", buf_len);
        return;
    }
    buf_len -= sizeof(mac_ether_header_stru);
    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf_eth);
    if (oal_unlikely(pst_ether_hdr == NULL || rx_ctrl == NULL)) {
        oal_io_print(WIFI_WAKESRC_TAG "ether header or rx_ctrl is null .\n");
        return;
    }

    us_type = pst_ether_hdr->us_ether_type;
    oal_io_print(WIFI_WAKESRC_TAG "protocol type:0x%04x, is_amsdu: %d, is_fragmented: %d, msdu_in_buffer: %d. \n",
        oal_ntoh_16(us_type), rx_ctrl->bit_amsdu_enable, rx_ctrl->bit_is_fragmented, rx_ctrl->uc_msdu_in_buffer);

    if (us_type == oal_host2net_short(ETHER_TYPE_IP)) {
        hmac_parse_ipv4_packet(pst_ether_hdr, buf_len);
    } else if (us_type == oal_host2net_short(ETHER_TYPE_IPV6)) {
        hmac_parse_ipv6_packet(pst_ether_hdr, buf_len);
    } else if (us_type == oal_host2net_short(ETHER_TYPE_ARP)) {
        hmac_parse_arp_packet(pst_ether_hdr, buf_len);
    } else if (us_type == oal_host2net_short(ETHER_TYPE_PAE)) {
        hmac_parse_8021x_packet(pst_ether_hdr, buf_len);
    } else {
        oal_io_print(WIFI_WAKESRC_TAG "receive other packet.\n");
        oam_warning_log2(vap_idx, OAM_SF_ANY,
            "{hmac_parse_packet:rx other packet! type[0x%x], len[%d]}", us_type, buf_len);
        hmac_rx_netbuf_force_ota_report((mac_rx_ctl_stru *)oal_netbuf_cb(netbuf_eth), netbuf_eth);
    }
}
#endif

OAL_STATIC uint32_t hmac_rx_vip_info_proc_data_dhcp(mac_ether_header_stru *ether_hdr, uint8_t data_type,
    uint8_t vap_idx, uint32_t payload_len)
{
    oal_ip_header_stru *ip_hdr = NULL;
    uint8_t sip[ETH_SENDER_IP_ADDR_LEN], dip[ETH_SENDER_IP_ADDR_LEN]; /* sender IP address */
    int32_t l_ret = EOK;
    uint8_t dhcp_type;

    if (payload_len < sizeof(mac_ether_header_stru) + sizeof(oal_ip_header_stru)) {
        oam_error_log2(0, OAM_SF_RX, "hmac_rx_vip_info_proc::DHCP buf_len[%d]data_type[%d]", payload_len, data_type);
        return -OAL_EFAIL;
    }

    ip_hdr = (oal_ip_header_stru *)(ether_hdr + 1);
    l_ret += memcpy_s((uint8_t *)sip, ETH_SENDER_IP_ADDR_LEN, (uint8_t *)&ip_hdr->saddr, sizeof(uint32_t));
    l_ret += memcpy_s((uint8_t *)dip, ETH_SENDER_IP_ADDR_LEN, (uint8_t *)&ip_hdr->daddr, sizeof(uint32_t));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_RX, "hmac_rx_vip_info_proc::DHCP memcpy fail!");
        return l_ret;
    }

    /* ip tot len校验 */
    if (payload_len != sizeof(mac_ether_header_stru) + oal_net2host_short(ip_hdr->us_tot_len)) {
        oam_warning_log2(0, OAM_SF_RX, "hmac_rx_vip_info_proc::DHCP netbuf_len[%d] tot_len[%d]",
            payload_len, oal_net2host_short(ip_hdr->us_tot_len));
        return -OAL_EFAIL;
    }

    dhcp_type = mac_get_dhcp_frame_type(ip_hdr, payload_len - sizeof(mac_ether_header_stru));
    oam_warning_log3(vap_idx, OAM_SF_MLOG, "{hmac_rx_vip_info_proc::vap_id=%d DHCP type=%d len=%d [1:discovery \
        2:offer 3:request 4:decline 5:ack 6:nack 7:release 8:inform.]", vap_idx, dhcp_type, payload_len);
    oam_warning_log4(vap_idx, OAM_SF_MLOG, "{hmac_rx_vip_info_proc:: DHCP sip: %d.%d, dip: %d.%d.",
        sip[BYTE_OFFSET_2], sip[BYTE_OFFSET_3], dip[BYTE_OFFSET_2], dip[BYTE_OFFSET_3]);

    return l_ret;
}

OAL_STATIC uint8_t hmac_rx_vip_info_proc_data_eapol(mac_ether_header_stru *ether_hdr, mac_rx_ctl_stru *rx_ctrl,
    uint8_t vap_idx, uint32_t len)
{
    mac_eapol_type_enum_uint8 eapol_type;
    /* netbuf 长度判断 */
    if (len < sizeof(mac_ether_header_stru) + sizeof(mac_eapol_header_stru)) {
        oam_error_log1(0, OAM_SF_RX, "hmac_rx_vip_info_proc::len[%d]", len);
        return MAC_EAPOL_PTK_BUTT;
    }
    eapol_type = mac_get_eapol_key_type((uint8_t *)(ether_hdr + 1));
    oam_warning_log3(vap_idx, OAM_SF_MLOG, "{hmac_rx_vip_info_proc::EAPOL type=%u, len==%u, \
        user[%d].}[1:1/4 2:2/4 3:3/4 4:4/4]", eapol_type, len, rx_ctrl->bit_rx_user_id);
    return eapol_type;
}

OAL_STATIC void hmac_rx_vip_info_proc_data_arp(mac_ether_header_stru *ether_hdr,
    uint8_t vap_idx, uint32_t len, uint8_t data_type)
{
    oal_eth_arphdr_stru *arp_head = NULL;
    if (len < sizeof(mac_ether_header_stru) + sizeof(oal_eth_arphdr_stru)) {
        oam_error_log2(0, OAM_SF_RX, "hmac_rx_vip_info_proc::len[%d] data_type[%d]", len, data_type);
        return;
    }

    arp_head = (oal_eth_arphdr_stru *)(ether_hdr + 1);
    oam_warning_log1(vap_idx, OAM_SF_RX, "{hmac_rx_vip_info_proc::ARP type=%d[2:arp rsp 3:arp req]", data_type);
    oam_warning_log4(vap_idx, OAM_SF_RX, "{hmac_rx_vip_info_proc:: ARP sip: %d.%d, dip: %d.%d",
        arp_head->auc_ar_sip[BYTE_OFFSET_2], arp_head->auc_ar_sip[BYTE_OFFSET_3],
        arp_head->auc_ar_tip[BYTE_OFFSET_2], arp_head->auc_ar_tip[BYTE_OFFSET_3]);
}

OAL_STATIC OAL_INLINE void hmac_rx_vip_info_proc_rx_ctl_print(uint8_t vap_idx, mac_rx_ctl_stru *rx_ctrl)
{
    oam_warning_log4(vap_idx, OAM_SF_RX, "{hmac_rx_vip_info_proc::VIP data process_flag[%d], tid[%d], \
        seq[%d], start_seq[%d]",
        rx_ctrl->bit_process_flag, rx_ctrl->bit_rx_tid, rx_ctrl->us_seq_num, rx_ctrl->bit_start_seqnum);
    oam_warning_log2(vap_idx, OAM_SF_RX, "{hmac_rx_vip_info_proc:: VIP data release_start[%d], release_end[%d]",
        rx_ctrl->bit_release_start_sn, rx_ctrl->bit_release_end_sn);
}

/*
 * 函 数 名  : hmac_rx_vip_info_proc
 * 功能描述  : hmac 接收一个mpdu的关键帧识别维测
 * 1.日    期  : 2021年2月1日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_rx_vip_info_proc(dpe_wlan_vap_stru *dpe_vap,
    uint8_t data_type, oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctrl)
{
    mac_ether_header_stru *ether_hdr = NULL;
    uint32_t netbuf_len;
    uint8_t is_rx_eapol_key_open;
    uint8_t eapol_type;
    /* 收到skb为eth头 */
    ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (oal_unlikely(ether_hdr == NULL)) {
        return;
    }
    netbuf_len = oal_netbuf_len(netbuf);

    oam_warning_log3(dpe_vap->uc_vap_id, OAM_SF_MLOG,
        "{hmac_rx_vip_info_proc::send to wlan smac: %02X:XX:XX:XX:%02X:%02X]",
        ether_hdr->auc_ether_shost[MAC_ADDR_0],
        ether_hdr->auc_ether_shost[MAC_ADDR_4], ether_hdr->auc_ether_shost[MAC_ADDR_5]);
    oam_warning_log4(dpe_vap->uc_vap_id, OAM_SF_MLOG,
        "{hmac_rx_vip_info_proc::user_id[%d] dmac:%02X:XX:XX:XX:%02X:%02X]",
        rx_ctrl->bit_rx_user_id, ether_hdr->auc_ether_dhost[MAC_ADDR_0],
        ether_hdr->auc_ether_dhost[MAC_ADDR_4], ether_hdr->auc_ether_dhost[MAC_ADDR_5]);
    hmac_rx_vip_info_proc_rx_ctl_print(dpe_vap->uc_vap_id, rx_ctrl);

    if (data_type == MAC_DATA_EAPOL) {
        eapol_type = hmac_rx_vip_info_proc_data_eapol(ether_hdr, rx_ctrl, dpe_vap->uc_vap_id, netbuf_len);
        if ((dpe_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) && (eapol_type != MAC_EAPOL_PTK_BUTT)) {
            is_rx_eapol_key_open = (rx_ctrl->bit_cipher_type == WLAN_80211_CIPHER_SUITE_NO_ENCRYP);
            oam_warning_log1(dpe_vap->uc_vap_id, OAM_SF_RX, "hmac_rx_vip_info_proc::is_rx_eapol_key_open=%d",
                is_rx_eapol_key_open);
            hmac_rx_eapol_notify_process(dpe_vap->uc_vap_id, is_rx_eapol_key_open);
        }
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
        hmac_rx_group_rekey_stat(dpe_vap->uc_vap_id);
#endif
        /* H3C自愈 */
        if (!ether_is_broadcast(ether_hdr->auc_ether_dhost)) {
            hmac_ht_self_cure_event_set(dpe_vap->uc_vap_id, NULL, HAMC_HT_SELF_CURE_EVENT_RX_EAPOL);
        }
    } else if (data_type == MAC_DATA_DHCP) {
        if (hmac_rx_vip_info_proc_data_dhcp(ether_hdr, data_type, dpe_vap->uc_vap_id, netbuf_len) != EOK) {
            return;
        }

        /* H3C自愈 */
        hmac_ht_self_cure_event_set(dpe_vap->uc_vap_id, NULL, HMAC_HT_SELF_CURE_EVENT_RX_DHCP_FRAME);
    } else if (data_type == MAC_DATA_ARP_RSP || data_type == MAC_DATA_ARP_REQ) {
        hmac_rx_vip_info_proc_data_arp(ether_hdr, dpe_vap->uc_vap_id, netbuf_len, data_type);
    }
    hmac_rx_vip_data_notify_process(dpe_vap->uc_vap_id, data_type);
}
