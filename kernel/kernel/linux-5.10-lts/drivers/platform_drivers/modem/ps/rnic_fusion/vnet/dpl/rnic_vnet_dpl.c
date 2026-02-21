/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <net/sock.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/jump_label.h>
#include "securec.h"
#include "mdrv_diag.h"
#include "mdrv_timer.h"
#include "rnic_data.h"
#include "rnic_log.h"
#include "rnic_private.h"
#include "rnic_dma_map.h"
#include "netfilter_ips_mntn.h"
#include "rnic_vnet.h"
#include "rnic_vnet_dpl_latency.h"
#include "rnic_vnet_dpl.h"
#include "rnic_dsm_msg_pif.h"

STATIC struct rnic_vnet_dpl_s *rnic_vnet_dpl_ptr = NULL;
STATIC DEFINE_STATIC_KEY_TRUE(rnic_dpl_dyn_rpt_en);
STATIC DEFINE_STATIC_KEY_FALSE(rnic_ips_en);
DEFINE_STATIC_KEY_FALSE(rnic_diag_conn);

STATIC void rnic_vnet_dpl_ips_key_set(bool val)
{
	RNIC_LOGI("set rnic vnet dpl ips trace key to %d", val);

	if (val)
		static_branch_enable(&rnic_ips_en);
	else
		static_branch_disable(&rnic_ips_en);
}

STATIC void rnic_vnet_dpl_diag_conn_key_set(bool val)
{
	RNIC_LOGI("set rnic vnet dpl diag conn key to %d", val);

	if (val)
		static_branch_enable(&rnic_diag_conn);
	else
		static_branch_disable(&rnic_diag_conn);
}

STATIC void rnic_vnet_dpl_diag_notify_func(unsigned int state)
{
	if (DIAG_GET_DIAG_TOOL_CONN_STATE(state) || DIAG_GET_HMI_SRV_CONN_STATE(state))
		rnic_vnet_dpl_diag_conn_key_set(true);
	else
		rnic_vnet_dpl_diag_conn_key_set(false);
}

STATIC void rnic_vnet_dpl_decode_l4(unsigned char *data, struct rnic_dpl_ip_info_s *ip_info)
{
	struct tcphdr *tcph = NULL;
	struct udphdr *udph = NULL;
	struct icmphdr *icmph = NULL;

	switch (ip_info->l4_proto) {
	case IPPROTO_TCP:
		tcph = (struct tcphdr *)data;
		ip_info->src_port = ntohs(tcph->source);
		ip_info->dst_port = ntohs(tcph->dest);
		ip_info->l4_id = ntohl(tcph->seq);
		ip_info->tcp_ack_seq = ntohl(tcph->ack_seq);
		ip_info->payload_len -= __tcp_hdrlen(tcph);
		break;
	case IPPROTO_UDP:
		udph = (struct udphdr *)data;
		ip_info->src_port = ntohs(udph->source);
		ip_info->dst_port = ntohs(udph->dest);
		ip_info->payload_len -= sizeof(struct udphdr);
		break;
	case IPPROTO_ICMP:
	case IPPROTO_ICMPV6:
		icmph = (struct icmphdr *)data;
		ip_info->icmp_type = icmph->type;
		if (ip_info->icmp_type == ICMP_ECHO ||
		    ip_info->icmp_type == ICMP_ECHOREPLY ||
		    ip_info->icmp_type == ICMPV6_ECHO_REQUEST ||
		    ip_info->icmp_type == ICMPV6_ECHO_REPLY) {
			ip_info->l4_id = ntohs(icmph->un.echo.id);
			ip_info->tcp_ack_seq = ntohs(icmph->un.echo.sequence);
		}
		break;
	default:
		ip_info->l4_id = ntohl(((u32 *)data)[0]);
		ip_info->tcp_ack_seq = ntohl(((u32 *)data)[1]);
		break;
	}
}

STATIC void rnic_vnet_dpl_decode_ipv4(unsigned char *data, struct rnic_dpl_ip_info_s *ip_info)
{
	struct iphdr *iph = (struct iphdr *)data;
	u16 hlen = iph->ihl * 4; /* ip header length unit is 4 bytes (32bit) */

	/* ip version 4 */
	ip_info->ip_ver = 4;
	ip_info->l4_proto = iph->protocol;
	ip_info->ip4_id = ntohs(iph->id);
	ip_info->data_len = ntohs(iph->tot_len);
	ip_info->payload_len = ip_info->data_len - hlen;

	rnic_vnet_dpl_decode_l4(data + hlen, ip_info);
}

STATIC void rnic_vnet_dpl_decode_ipv6(unsigned char *data, struct rnic_dpl_ip_info_s *ip_info)
{
	struct ipv6hdr *ip6h = (struct ipv6hdr *)data;

	/* ip version 6 */
	ip_info->ip_ver = 6;
	ip_info->l4_proto = ip6h->nexthdr;
	ip_info->ip4_id = 0;
	ip_info->data_len = ntohs(ip6h->payload_len) + sizeof(struct ipv6hdr);
	ip_info->payload_len = ntohs(ip6h->payload_len);

	/* decode wrongly if options exist */
	rnic_vnet_dpl_decode_l4(data + sizeof(struct ipv6hdr), ip_info);
}

STATIC void rnic_vnet_dpl_decode_ip(unsigned char *data, struct rnic_dpl_ip_info_s *ip_info)
{
	switch (data[0] & RNIC_IPVER_MASK) {
	case RNIC_IPVER_4:
		rnic_vnet_dpl_decode_ipv4(data, ip_info);
		break;
	case RNIC_IPVER_6:
		rnic_vnet_dpl_decode_ipv6(data, ip_info);
		break;
	default:
		ip_info->ip_ver = data[0];
		break;
	}
}

STATIC void rnic_vnet_dpl_decode_vlan(unsigned char *data,
				      struct rnic_dpl_pkt_info_s *pinfo)
{
	const struct vlan_ethhdr *vlan_hdr = (struct vlan_ethhdr *)data;

#ifdef CONFIG_PS_AP_ENG
	ether_addr_copy(pinfo->mac_info.dest, data);
	ether_addr_copy(pinfo->mac_info.source, data + ETH_ALEN);
#endif
	pinfo->mac_info.vlan_tci = ntohs(vlan_hdr->h_vlan_TCI);
	pinfo->mac_info.vlan_encap_proto = ntohs(vlan_hdr->h_vlan_encapsulated_proto);

	if (ntohs(vlan_hdr->h_vlan_encapsulated_proto) == ETH_P_IP)
		rnic_vnet_dpl_decode_ipv4(data + VLAN_ETH_HLEN, &pinfo->ip_info);
	else if (ntohs(vlan_hdr->h_vlan_encapsulated_proto) == ETH_P_IPV6)
		rnic_vnet_dpl_decode_ipv6(data + VLAN_ETH_HLEN, &pinfo->ip_info);
}

STATIC void rnic_vnet_dpl_decode_eth(unsigned char *data,
				     struct rnic_dpl_pkt_info_s *pinfo)
{
	const struct ethhdr *eth = (struct ethhdr *)data;

#ifdef CONFIG_PS_AP_ENG
	ether_addr_copy(pinfo->mac_info.dest, data);
	ether_addr_copy(pinfo->mac_info.source, data + ETH_ALEN);
#endif
	pinfo->mac_info.vlan_tci = 0;
	pinfo->mac_info.vlan_encap_proto = ntohs(eth->h_proto);

	if (ntohs(eth->h_proto) == ETH_P_IP)
		rnic_vnet_dpl_decode_ipv4(data + ETH_HLEN, &pinfo->ip_info);
	else if (ntohs(eth->h_proto) == ETH_P_IPV6)
		rnic_vnet_dpl_decode_ipv6(data + ETH_HLEN, &pinfo->ip_info);
}

STATIC void rnic_vnet_dpl_decode_mac(unsigned char *data,
				     struct rnic_dpl_pkt_info_s *pinfo)
{
	const struct vlan_ethhdr *vlan_hdr = (struct vlan_ethhdr *)data;

	if (vlan_hdr->h_vlan_proto == htons(ETH_P_8021Q))
		rnic_vnet_dpl_decode_vlan(data, pinfo);
	else
		rnic_vnet_dpl_decode_eth(data, pinfo);
}

STATIC void rnic_vnet_dpl_tx_trans(struct rnic_vnet_dpl_tx_s *tx)
{
	unsigned int size;

	size = sizeof(*tx) - (RNIC_VNET_DPL_TX_MAX - tx->pkt_num) *
	       sizeof(struct rnic_dpl_pkt_info_s);
	tx->head.snd_pid = RNIC_FIX_PID;

	rnic_trans_report(ID_DIAG_RNIC_VNET_DPL_TX, tx, size);
}

STATIC void rnic_vnet_dpl_rx_trans(struct rnic_vnet_dpl_rx_s *rx)
{
	unsigned int size;

	size = sizeof(*rx) - (RNIC_VNET_DPL_RX_MAX - rx->pkt_num) *
	       sizeof(struct rnic_dpl_pkt_info_s);
	rx->head.snd_pid = RNIC_FIX_PID;

	rnic_trans_report(ID_DIAG_RNIC_VNET_DPL_RX, rx, size);
}

STATIC void rnic_vnet_dpl_tx_private(const struct sk_buff *skb,
				     const struct wan_info_s *winfo,
				     struct rnic_dpl_priv_info_s *priv_info)
{
	struct sock *sk = skb->sk;

	priv_info->tft_value = *(u32 *)&winfo->info;
	priv_info->param0 = winfo->userfield0;
	priv_info->param1 = (sk != NULL && sk_fullsock(sk)) ? (u32)sk_wmem_alloc_get(sk) : 0;
	priv_info->param2 = winfo->userfield2;
}

STATIC void rnic_vnet_dpl_tx_rpt(struct rnic_vnet_dpl_tx_s *tx)
{
	/* version 102 */
	tx->ver = 102;
	tx->pkt_num = 1;
	rnic_vnet_dpl_tx_trans(tx);
}

STATIC void rnic_vnet_dpl_tx_mac(const struct sk_buff *skb)
{
	struct rnic_vnet_dpl_tx_s tx = {0};
	struct wan_info_s *winfo = rnic_wan_skb_cb(skb);
	struct rnic_dpl_pkt_info_s *pinfo = &tx.pkt_infos[0];

	rnic_vnet_dpl_decode_mac(skb->data, pinfo);
	rnic_vnet_dpl_tx_private(skb, winfo, &pinfo->priv_info);
	rnic_vnet_dpl_tx_rpt(&tx);
}

STATIC void rnic_vnet_dpl_tx_ip(const struct sk_buff *skb)
{
	struct rnic_vnet_dpl_tx_s tx = {0};
	struct wan_info_s *winfo = rnic_wan_skb_cb(skb);
	struct rnic_dpl_pkt_info_s *pinfo = &tx.pkt_infos[0];

	rnic_vnet_dpl_decode_ip(skb->data + ETH_HLEN, &pinfo->ip_info);
	rnic_vnet_dpl_tx_private(skb, winfo, &pinfo->priv_info);
	rnic_vnet_dpl_tx_rpt(&tx);
}

STATIC void rnic_vnet_dpl_rx_private(struct rx_cb_map_s *map,
				     struct rnic_dpl_priv_info_s *priv_info,
				     pfa_version_type pfa_ver)
{
	priv_info->tft_value = map->pfa_tft_result.u32;
	priv_info->param0 = map->userfield0;
	priv_info->param1 = map->userfield1;
	priv_info->rd_ts = rnic_vnet_dpl_ptr->rd_ts;
	priv_info->l2dle_csum = map->pfa_result.bits.checksum;
	priv_info->param2 = (pfa_ver != PFA_VERSION_V200) ? map->userfield2 :
		(map->userfield2 << RNIC_MAP_PDCP_CNT_OFFSET |
		(u32)((map->packet_info.bits.net_id) & RNIC_MAP_NETID_MASK));
}

STATIC void rnic_vnet_dpl_rx_rpt(struct rnic_vnet_dpl_rx_s *rx)
{
	/* version 103 */
	rx->ver = 103;
	if (++rx->pkt_num >= RNIC_VNET_DPL_RX_MAX) {
		rnic_vnet_dpl_rx_trans(rx);
		rx->pkt_num = 0;
	}
}

STATIC void rnic_vnet_dpl_rx_mac(const struct sk_buff *skb, pfa_version_type pfa_ver)
{
	struct rx_cb_map_s *map = rnic_map_skb_cb(skb);
	struct rnic_vnet_dpl_rx_s *rx = &rnic_vnet_dpl_ptr->rx;
	struct rnic_dpl_pkt_info_s *pinfo = &rx->pkt_infos[rx->pkt_num];

	rnic_vnet_dpl_decode_mac(skb->data, pinfo);
	rnic_vnet_dpl_rx_private(map, &pinfo->priv_info, pfa_ver);
	rnic_vnet_dpl_rx_rpt(rx);
}

STATIC void rnic_vnet_dpl_rx_ip(const struct sk_buff *skb, pfa_version_type pfa_ver)
{
	struct rx_cb_map_s *map = rnic_map_skb_cb(skb);
	struct rnic_vnet_dpl_rx_s *rx = &rnic_vnet_dpl_ptr->rx;
	struct rnic_dpl_pkt_info_s *pinfo = &rx->pkt_infos[rx->pkt_num];

	rnic_vnet_dpl_decode_ip(skb->data, &pinfo->ip_info);
	rnic_vnet_dpl_rx_private(map, &pinfo->priv_info, pfa_ver);
	rnic_vnet_dpl_rx_rpt(rx);
}

void __rnic_vnet_dpl_tx(const struct sk_buff *skb)
{
	rnic_vnet_dpl_pts_tx_rpt(skb);
	rnic_vnet_dpl_ptr->dpl_stats.dpl_pkts_num++;

	if (static_branch_unlikely(&rnic_ips_en) && static_branch_unlikely(&rnic_dpl_dyn_rpt_en)) {
		switch (rnic_wan_pkttype(skb)) {
		case RNIC_WAN_PKTTYPE_IP:
			rnic_vnet_dpl_tx_ip(skb);
			break;
		case RNIC_WAN_PKTTYPE_ETH:
		case RNIC_WAN_PKTTYPE_VLAN:
			rnic_dma_unmap(rnic_vnet_dpl_ptr->dev, skb,
				       RNIC_VNET_DPL_CACHE_LEN, DMA_FROM_DEVICE);
			rnic_vnet_dpl_tx_mac(skb);
			rnic_dma_map(rnic_vnet_dpl_ptr->dev, skb,
				     RNIC_VNET_DPL_CACHE_LEN, DMA_FROM_DEVICE);
			break;
		default:
			break;
		}
	}
}

void __rnic_vnet_dpl_rx(const struct sk_buff *skb, pfa_version_type pfa_ver)
{
	rnic_vnet_dpl_pts_rx_rpt(skb);
	rnic_vnet_dpl_ptr->dpl_stats.dpl_pkts_num++;

	if (static_branch_unlikely(&rnic_ips_en) && static_branch_unlikely(&rnic_dpl_dyn_rpt_en)) {
		if (unlikely(rnic_vnet_dpl_ptr->rd_ts == 0))
			rnic_vnet_dpl_ptr->rd_ts = mdrv_timer_get_normal_timestamp();

		switch (rnic_map_pkttype(skb)) {
		case RNIC_MAP_PKTTYPE_IP:
			rnic_vnet_dpl_rx_ip(skb, pfa_ver);
			break;
		case RNIC_MAP_PKTTYPE_ETH:
		case RNIC_MAP_PKTTYPE_VLAN:
#ifdef CONFIG_RNIC_LAN_FORWARD
			rnic_dma_unmap(rnic_vnet_dpl_ptr->dev, skb,
				       RNIC_VNET_DPL_CACHE_LEN, DMA_FROM_DEVICE);
#endif
			rnic_vnet_dpl_rx_mac(skb, pfa_ver);
#ifdef CONFIG_RNIC_LAN_FORWARD
			rnic_dma_map(rnic_vnet_dpl_ptr->dev, skb,
				     RNIC_VNET_DPL_CACHE_LEN, DMA_FROM_DEVICE);
#endif
			break;
		default:
			break;
		}
	}
}

void __rnic_vnet_dpl_rx_cmplt(void)
{
	if (static_branch_unlikely(&rnic_ips_en)) {
		struct rnic_vnet_dpl_rx_s *rx = NULL;

		rnic_vnet_dpl_ptr->rd_ts = 0;
		rx = &rnic_vnet_dpl_ptr->rx;

		if (rx->pkt_num != 0) {
			rnic_vnet_dpl_rx_trans(rx);
			rx->pkt_num = 0;
		}
	}
}

STATIC bool rnic_dpl_check_diag_threshold_reached(struct rnic_vnet_dpl_s *dptr, u32 ts_diff)
{
	return ((u64)dptr->dpl_stats.dpl_pkts_num * (u64)dptr->ts_freq >
		((u64)dptr->diag_open_threshold * (u64)ts_diff)) ? true : false;
}

STATIC void rnic_vnet_dpl_set_diag_threshold(const void *cfg)
{
	const struct rnic_vnet_fea_cfg_s *vcfg = (const struct rnic_vnet_fea_cfg_s *)cfg;
	struct rnic_vnet_dpl_s *dptr = rnic_vnet_dpl_ptr;

	dptr->diag_open_threshold = vcfg->diag_open_threshold;
}

STATIC void rnic_vnet_dpl_dyn_rpt_stats_switch(void)
{
	struct rnic_vnet_dpl_s *dptr = rnic_vnet_dpl_ptr;
	u32 ts_cur;
	u32 ts_diff;
	bool reached;

	if (!dptr->diag_open_threshold)
		return;

	ts_cur = mdrv_timer_get_normal_timestamp();
	ts_diff = rnic_caculate_gap_time(ts_cur, dptr->dpl_stats.dpl_ts);
	reached = rnic_dpl_check_diag_threshold_reached(dptr, ts_diff);

	dptr->dpl_stats.dpl_ts = ts_cur;
	dptr->dpl_stats.dpl_pkts_num = 0;

	if (reached)
		static_branch_disable(&rnic_dpl_dyn_rpt_en);
	else
		static_branch_enable(&rnic_dpl_dyn_rpt_en);
}

STATIC const struct rnic_dpl_ops_s rnic_dpl_ops = {
	.set_diag_open_threshold	= rnic_vnet_dpl_set_diag_threshold,
	.rpt_stats			= rnic_vnet_dpl_dyn_rpt_stats_switch,
};

void rnic_vnet_dpl_init(struct rnic_handle_s *handle)
{
	struct rnic_vnet_ctx_s *vctx = (struct rnic_vnet_ctx_s *)handle->priv;

	rnic_vnet_dpl_ptr = kzalloc(sizeof(*rnic_vnet_dpl_ptr), GFP_KERNEL);
	if (rnic_vnet_dpl_ptr == NULL) {
		RNIC_LOGE("alloc dpl failed");
		return;
	}

	(void)mdrv_diag_conn_state_notify_fun_reg(rnic_vnet_dpl_diag_notify_func);
	IPSMNTN_RegTraceCfgNotify(rnic_vnet_dpl_ips_key_set);

	rnic_vnet_dpl_ptr->dev = handle->dev;
	/* version 101 */
	rnic_vnet_dpl_ptr->tx.ver = 101;
	/* version 101 */
	rnic_vnet_dpl_ptr->rx.ver = 101;
	rnic_vnet_dpl_ptr->rx.pfa_ver = handle->pfa_ver;

	vctx->dpl_ops = &rnic_dpl_ops;

	rnic_vnet_dpl_ptr->dpl_stats.dpl_ts = mdrv_timer_get_normal_timestamp();
	rnic_vnet_dpl_ptr->ts_freq = mdrv_get_normal_timestamp_freq();
}

void rnic_vnet_dpl_exit(struct rnic_handle_s *handle)
{
	struct rnic_vnet_ctx_s *vctx = (struct rnic_vnet_ctx_s *)handle->priv;

	vctx->dpl_ops = NULL;

	if (rnic_vnet_dpl_ptr == NULL)
		return;

	kfree(rnic_vnet_dpl_ptr);
	rnic_vnet_dpl_ptr = NULL;
}
