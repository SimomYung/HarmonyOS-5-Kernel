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

#include <net/ip.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/cpu.h>
#include <linux/if_ether.h>
#include <linux/workqueue.h>
#include <net/xdp_sock_drv.h>
#include <xdp/xsk_queue.h>
#if (defined(CONFIG_BASTET) || defined(CONFIG_HW_DPIMARK_MODULE))
#include <net/inet_sock.h>
#endif
#include "securec.h"
#include "mdrv_dra.h"
#include "mdrv_pfa.h"
#include "rnic_vnet.h"
#include "rnic_vnet_ioctl.h"
#include "rnic_vnet_dpl_i.h"
#include "rnic_vnet_rx_sched.h"
#include "rnic_pktcap.h"
#include "rnic_priority_map.h"
#include "rnic_fragment_i.h"
#include "rnic_private.h"
#include "rnic_log.h"
#include "rnic_vnet_hdlr.h"
#include "rnic_checksum.h"
#include "rnic_construct_skb.h"

void (*rnic_gro_rcv_burst_entry)(struct sk_buff_head *) __rcu __read_mostly = NULL;
#define VNET_PROFINET_PROT 0x8892

STATIC void rnic_vnet_netif_rx(struct sk_buff *skb)
{
	if (in_softirq())
		netif_receive_skb(skb);
	else
		netif_rx_ni(skb);
}

#ifdef CONFIG_RNIC_LAN_FORWARD
STATIC struct rnic_vnet_priv_s *rnic_vnet_lan_select(struct sk_buff *skb)
{
	struct rnic_vnet_priv_s *priv = netdev_priv(skb->dev);
	struct rnic_vnet_ctx_s *vctx = priv->back;
	struct vlan_ethhdr *vlan_hdr = NULL;
	u16 vlan_tci = (u16)(rnic_map_skb_cb(skb)->userfield1);
	u16 vlan_id;
	bool is_in = false;

	if (vlan_tci == 0)
		return priv;

	/* VLAN TCI is based on RD descriptor */
	if (skb->protocol == htons(ETH_P_8021Q)) {
		dma_unmap_single(priv->dev, virt_to_phys(skb->data), VLAN_ETH_HLEN,
				 DMA_FROM_DEVICE);
		vlan_hdr = (struct vlan_ethhdr *)skb->data;
		if (vlan_hdr->h_vlan_TCI != vlan_tci) {
			vlan_hdr->h_vlan_TCI = vlan_tci;
			is_in = true;
		}
		dma_map_single(priv->dev, skb->data, VLAN_ETH_HLEN,
			       is_in ? DMA_TO_DEVICE : DMA_FROM_DEVICE);
	}

	vlan_id = ntohs(vlan_tci) & (u32)VLAN_VID_MASK;
	if (vctx->vlan_netdevs[vlan_id] != NULL)
		skb->dev = vctx->vlan_netdevs[vlan_id];

	return netdev_priv(skb->dev);
}

int rnic_vnet_lan_xmit(struct sk_buff *skb)
{
	struct rx_cb_map_s *map = rnic_map_skb_cb(skb);
	struct rnic_vnet_priv_s *priv = NULL;

	skb->protocol = map->packet_info.bits.l2_hdr_offeset == VLAN_ETH_HLEN ?
			htons(ETH_P_8021Q) : 0;
	RNIC_LOGI("skb->protocol %x", skb->protocol);

	priv = rnic_vnet_lan_select(skb);

	rnic_set_mem_flag(skb, RNIC_MEMF_LAN);
	return rnic_vnet_tx_data(priv, skb);
}

STATIC void rnic_vnet_lan_deliver_skb(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	struct rx_cb_map_s *map = rnic_map_skb_cb(skb);
	struct vlan_ethhdr *vlan_hdr = NULL;

	map->userfield1 = 0;
	map->userfield2 = rnic_map_pktid(skb);

	vlan_hdr = (struct vlan_ethhdr *)skb->data;
	if (vlan_hdr->h_vlan_proto == htons(ETH_P_8021Q)) {
		map->userfield1 = vlan_hdr->h_vlan_TCI;
		skb->mac_len = VLAN_ETH_HLEN;
	} else {
		skb->mac_len = ETH_HLEN;
	}

	skb->protocol = 0;
	skb->dev = priv->netdev;
	mdrv_lan_tx(skb, rnic_mem_flag_is_set(skb));
}

#else
u32 rnic_vnet_set_xsk_open_stat(u32 vnet_id, u32 value)
{
	struct rnic_vnet_priv_s *priv = rnic_vnet_get_priv(vnet_id);
	if (priv == NULL)
		return -EINVAL;

	priv->xdp_stats.xdp_xsk_open = value ? true : false;
	return 0;
}

STATIC bool rnic_vnet_is_pn_pkt(struct sk_buff *skb)
{
	struct ethhdr* eth_header;
	struct vlan_ethhdr* vlan_header;
	unsigned short proto_type;

	eth_header = (struct ethhdr*)skb->data;
	if (eth_header->h_proto == htons(ETH_P_8021Q)) {
		vlan_header = (struct vlan_ethhdr*)skb->data;
		proto_type = vlan_header->h_vlan_encapsulated_proto;
	} else {
		proto_type = eth_header->h_proto;
	}

	return (proto_type == htons(VNET_PROFINET_PROT)) ? true : false;
}

#ifdef CONFIG_XDP_SOCKETS
STATIC void rnic_vnet_xsk_generic_rcv_skb(struct sk_buff *skb, struct rnic_vnet_priv_s *priv)
{
	struct net_device *dev = priv->netdev;
	struct xdp_sock *xs = (struct xdp_sock *)dev->xsk_priv;
	unsigned int pkt_len;
	char *pkt_data;
	u64 addr;
	u32 refresh_pkt_num;
	int err = 0;

	if (unlikely(xs == NULL || xs->pool == NULL)) {
		priv->xdp_stats.xdp_sock_null++;
		goto out;
	}

	/* refresh the producer in refresh_pkt_num pkts */
	refresh_pkt_num = (xs->pool->fq->nentries >> 2);
	(void)xskq_cons_has_entries(xs->pool->fq, refresh_pkt_num);
	if (xskq_cons_read_addr_unchecked(xs->pool->fq, &addr) == false) {
		priv->xdp_stats.ring_empty++;
		goto out;
	}

	pkt_data = xp_raw_get_data(xs->pool, addr);
	(void)memcpy_s(pkt_data, skb->len, skb->data, skb->len);
	pkt_len = skb->len;

	spin_lock_bh(&xs->rx_lock);
	/* move the desc fill queue idx */
	xskq_cons_release(xs->pool->fq);

	/* move the rx queue idx */
	err = xskq_prod_reserve_desc(xs->rx, addr, pkt_len);
	if (err) {
		xs->rx_queue_full++;
		spin_unlock_bh(&xs->rx_lock);
		priv->xdp_stats.rx_ring_full++;
		goto out;
	}

	/* notify the app */
	xsk_flush(xs);
	spin_unlock_bh(&xs->rx_lock);

out:
	rnic_kfree_skb_rx(skb);
}
#endif

STATIC void rnic_vnet_lan_deliver_skb(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
#ifdef CONFIG_DRA_PAGE
	if (likely(rnic_mem_flag_is_set(skb))) {
		skb = rnic_construct_skb_one(skb);
		if (unlikely(skb == NULL))
			return;
	}
#else
	if (unlikely(rnic_mem_flag_is_set(skb))) {
		rnic_kfree_skb_rx(skb);
		priv->stats.rx.rx_maa_errs++;
		return;
	}
#endif

	rnic_pktcap(skb);

	if (priv->xdp_stats.xdp_xsk_open && rnic_vnet_is_pn_pkt(skb)) {
#ifdef CONFIG_XDP_SOCKETS
		rnic_vnet_xsk_generic_rcv_skb(skb, priv);
#else
		skb->protocol = eth_type_trans(skb, priv->netdev);
		rnic_vnet_netif_rx(skb);
#endif
	} else {
		skb->protocol = eth_type_trans(skb, priv->netdev);
		rnic_vnet_netif_rx(skb);
	}
}
#endif

STATIC void rnic_vnet_set_ethhdr(struct sk_buff *skb, struct net_device *netdev)
{
	struct ethhdr *ethh = (struct ethhdr *)(skb->data - ETH_HLEN);

	ether_addr_copy(ethh->h_dest, netdev->dev_addr);
	ether_addr_copy(ethh->h_source, netdev->dev_addr);
	ethh->h_proto = skb->protocol;
}

void rnic_vnet_deliver_skb(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	rnic_vnet_set_ethhdr(skb, priv->netdev);

	skb_push(skb, ETH_HLEN);
	rnic_pktcap(skb);

	__skb_pull(skb, ETH_HLEN);
	skb_set_mac_header(skb, -ETH_HLEN);
	skb_set_network_header(skb, 0);
	skb->pkt_type = PACKET_HOST;

	rnic_vnet_netif_rx(skb);
}

STATIC void rnic_vnet_deliver_rxq(struct rnic_vnet_ctx_s *vctx)
{
	if (likely(vctx->srs_ops != NULL))
		vctx->srs_ops->rx_skb_list(&vctx->rx_list);

	if (!skb_queue_empty(&vctx->rx_list)) {
		struct sk_buff *skb = NULL;
		while ((skb = __skb_dequeue(&vctx->rx_list)) != NULL) {
#ifdef CONFIG_DRA_PAGE
			if (rnic_mem_flag_is_set(skb)) {
				skb = rnic_construct_skb_one(skb);
				if (unlikely(skb == NULL))
					continue;
			}
#endif
			rnic_vnet_deliver_skb(netdev_priv(skb->dev), skb);
		}
	}
}

STATIC bool rnic_vnet_ip4_can_parse(struct sk_buff *skb)
{
	struct iphdr *iph = NULL;
	struct tcphdr *th = NULL;

	/*
	 * If the skb is MAA mem in relay mode, it must be from SNE lan port and can parse.
	 * If not in relay mode, need attention TCP SYNC pkt.
	 */
	if (rnic_mem_flag_is_set(skb))
		goto succ;

	if (unlikely(!pskb_may_pull(skb, ETH_HLEN + sizeof(*iph))))
		goto fail;

	iph = ip_hdr(skb);
	if (unlikely(*(u8 *)iph != 0x45))
		goto fail;

	if (ip_is_fragment(iph) || iph->protocol != IPPROTO_TCP)
		goto fail;

	if (unlikely(!pskb_may_pull(skb, ETH_HLEN + sizeof(*iph) + sizeof(*th))))
		goto fail;

	/*
	 * Due to IPF can't distinguish ACK and SYNC pkt,
	 * if the parse_en (return value of this function) is ture,
	 * means pkt should be parse by ipf to identify ack pkt,
	 * and ack pkt will be put into high-priority queue of PDCP.
	 * So here return false for SYNC pkt.
	 */
	th = tcp_hdr(skb);
	if (tcp_flag_word(th) & TCP_FLAG_SYN)
		goto fail;

succ:
	return true;

fail:
	return false;
}

STATIC bool rnic_vnet_ip6_can_parse(struct sk_buff *skb)
{
	struct ipv6hdr *ip6h = NULL;
	struct tcphdr *th = NULL;

	/* attention：refer to rnic_vnet_ip4_can_parse */
	if (rnic_mem_flag_is_set(skb))
		goto succ;

	if (unlikely(!pskb_may_pull(skb, ETH_HLEN + sizeof(*ip6h))))
		goto fail;

	ip6h = ipv6_hdr(skb);
	if (ip6h->nexthdr != IPPROTO_TCP)
		goto fail;

	if (unlikely(!pskb_may_pull(skb, ETH_HLEN + sizeof(*ip6h) + sizeof(*th))))
		goto fail;

	th = tcp_hdr(skb);
	if (tcp_flag_word(th) & TCP_FLAG_SYN)
		goto fail;

succ:
	return true;

fail:
	return false;
}

STATIC bool rnic_vnet_eth_can_parse(struct sk_buff *skb)
{
	switch (skb->protocol) {
	case htons(ETH_P_8021Q):
		if (skb->len >= RNIC_VLAN_PARSE_CHK_LEN)
			return true;
		break;
	default:
		if (skb->len >= RNIC_ETH_PARSE_CHK_LEN)
			return true;
		break;
	}

	return false;
}

STATIC bool rnic_vnet_skb_is_tcp(struct sk_buff *skb)
{
	switch (skb->protocol) {
	case htons(ETH_P_IP):
		if (ip_hdr(skb)->protocol == IPPROTO_TCP)
			return true;
		break;
	case htons(ETH_P_IPV6):
		if (ipv6_hdr(skb)->nexthdr == IPPROTO_TCP)
			return true;
		break;
	default:
		break;
	}

	return false;
}

STATIC bool rnic_vnet_skb_can_clone(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;

	if (sk == NULL)
		return false;

	if(rnic_vnet_skb_is_tcp(skb)) {
		if (unlikely(!sk_fullsock(sk)))
			return false;

		if ((sk->sk_userlocks & SOCK_SNDBUF_LOCK) != 0 &&
		    sk->sk_sndbuf <= (RNIC_MAX_FIX_SNDBUF << 1))
			return true;

		return false;
	}

	return true;
}

STATIC u8 rnic_vnet_sk_drop_code(struct sk_buff *skb)
{
#if defined(CONFIG_HW_HIGH_PRIORITY_LABEL) || defined(CONFIG_BASTET)
	struct sock *sk = skb_to_full_sk(skb);

	return sk != NULL ? sk->discard_duration : 0;
#else
	return 0;
#endif
}

STATIC u8 rnic_vnet_sk_acc(struct sk_buff *skb)
{
#if (defined(CONFIG_BASTET) || defined(CONFIG_HW_HIGH_PRIORITY_LABEL) || \
	defined(CONFIG_HW_DPIMARK_MODULE))
	struct sock *sk = NULL;

	sk = skb_to_full_sk(skb);
	if (sk != NULL) {
		u8 value = 0;
#ifdef CONFIG_BASTET
		value |= sk->acc_state & 0x01;
#endif
#if defined(CONFIG_HW_DPIMARK_MODULE) || defined(CONFIG_HW_HIGH_PRIORITY_LABEL)
		value |= ((u8 *)&sk->sk_hwdpi_mark)[1];
#endif
		return value;
	}
#endif

	return 0;
}

STATIC u32 rnic_vnet_build_field0(struct rnic_vnet_priv_s *priv,
				  struct sk_buff *skb)
{
	u32 value;
	u32 acc;
	u32 priority;

	value = priv->pif_id | ((u32)(skb->cb[RNIC_DROP_CODE_CB_POS]) << RNIC_WAN_DROP_SHIFT);
	if (!(value & RNIC_WAN_DROP_MASK))
		value |= priv->drop_code << RNIC_WAN_DROP_SHIFT;

	acc = skb->cb[RNIC_SK_PRIO_CB_POS];
	if (!!acc)
		value |= (acc << RNIC_WAN_PRI_SHIFT) & RNIC_WAN_PRI_MASK;

	priority = rnic_priority_map(priv, skb);
	if (!(value & RNIC_WAN_PRI_MASK) && priority)
		value |= (priority << RNIC_WAN_PRI_SHIFT) & RNIC_WAN_PRI_MASK;

	return value;
}

STATIC u32 rnic_vnet_build_field2(struct sk_buff *skb)
{
	static u32 tx_pkt_no = 0;

	if (!rnic_mem_flag_is_set(skb))
		return RNIC_VNET_PTS_MOD_ID | (tx_pkt_no++ << RNIC_VNET_PTS_PKT_NO_OFFSET);

	return rnic_map_skb_cb(skb)->userfield2;
}

int rnic_vnet_fill_ip_desc(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	struct wan_info_s *winfo = rnic_wan_skb_cb(skb);
	struct pfa_tft_info_s *tft_info = NULL;
	struct rnic_handle_s *handle = priv->handle;
	bool parse_en = false;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		tft_info = &priv->tft_infos[RNIC_ADDR_IPV4];
		parse_en = rnic_vnet_ip4_can_parse(skb);
		break;
	case htons(ETH_P_IPV6):
		tft_info = &priv->tft_infos[RNIC_ADDR_IPV6];
		parse_en = rnic_vnet_ip6_can_parse(skb);
		break;
	default:
		return -EPROTONOSUPPORT;
	}

	winfo->info = *tft_info;
	winfo->info.parse_en = !!parse_en;

	switch (handle->pfa_ver) {
	case PFA_VERSION_V200:
		winfo->info.pkt_ind = 1;
		winfo->info.copy_mode = !!priv->copy_en;
		winfo->info.tft_filter_ind = 1;
		winfo->info.bypass_dst_pkt_ind = 1;
		break;
	default:
		winfo->info.td_mode_en = 1;
		winfo->info.td_mode = priv->copy_en ?
				PFA_TD_MOD_COPY_AND_FILTER : PFA_TD_MOD_FILTER_ONLY;
		winfo->info.pkt_ind = 0;
		break;
	}

	winfo->userfield2 = rnic_vnet_build_field2(skb);
	winfo->userfield1 = 0;
	winfo->userfield0 = rnic_vnet_build_field0(priv, skb);

	skb->mac_len = ETH_HLEN;
	return 0;
}

int rnic_vnet_fill_eth_desc(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	struct wan_info_s *winfo = rnic_wan_skb_cb(skb);
	struct rnic_handle_s *handle = priv->handle;
	struct ethhdr* eth_hdr;

	winfo->info = priv->tft_infos[RNIC_ADDR_ETH];
	winfo->info.parse_en = !!rnic_vnet_eth_can_parse(skb);

	switch (handle->pfa_ver) {
	case PFA_VERSION_V200:
		winfo->info.pkt_ind = 0;
		winfo->info.copy_mode = 0;
		winfo->info.tft_filter_ind = 1;
		winfo->info.bypass_dst_pkt_ind = 0;
		break;
	default:
		winfo->info.td_mode_en = 1;
		winfo->info.td_mode = PFA_TD_MOD_FILTER_ONLY;
		winfo->info.pkt_ind = 1;
		break;
	}

	winfo->userfield2 = rnic_vnet_build_field2(skb);
	winfo->userfield1 = 0;

	eth_hdr = (struct ethhdr *)(skb->data);
	winfo->userfield0 = eth_hdr->h_proto == htons(ETH_P_8021Q) ?
			    priv->pif_id | RNIC_WAN_PKTTYPE_VLAN :
			    priv->pif_id | RNIC_WAN_PKTTYPE_ETH;

	skb->mac_len = 0;
	return 0;
}

int rnic_vnet_fill_frag_desc(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	struct wan_info_s *winfo = rnic_wan_skb_cb(skb);
	struct pfa_tft_info_s *tft_info = NULL;
	struct rnic_handle_s *handle = priv->handle;
	u8 frag_value = rnic_wan_frag_cb(skb)->value;

	switch (skb->protocol) {
	case htons(ETH_P_IP):
		tft_info = &priv->tft_infos[RNIC_ADDR_IPV4];
		break;
	case htons(ETH_P_IPV6):
		tft_info = &priv->tft_infos[RNIC_ADDR_IPV6];
		break;
	default:
		return -EPROTONOSUPPORT;
	}

	winfo->info = *tft_info;
	switch (handle->pfa_ver) {
	case PFA_VERSION_V200:
		winfo->info.pkt_ind = 1;
		winfo->info.copy_mode = 1;
		winfo->info.tft_filter_ind = 0;
		winfo->info.bypass_dst_pkt_ind = 1;
		break;
	default:
		winfo->info.td_mode_en = 1;
		winfo->info.td_mode = PFA_TD_MOD_COPY_ONLY;
		winfo->info.pkt_ind = 0;
		break;
	}

	winfo->userfield2 = 0;
	winfo->userfield1 = frag_value;
	winfo->userfield0 = priv->pif_id | RNIC_WAN_FIELD1_MASK;

	skb->mac_len = ETH_HLEN;
	return 0;
}

STATIC int rnic_vnet_output(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	struct rnic_handle_s *handle = priv->handle;

	int (*fill_desc)(struct rnic_vnet_priv_s *, struct sk_buff *) = priv->fill_desc;
	if (fill_desc == NULL || fill_desc(priv, skb)) {
		priv->stats.tx.tx_carr_errs++;
		goto err_drop;
	}

	rnic_vnet_dpl_tx(skb);
	rnic_pktcap(skb);

	priv->stats.tx.tx_pkts++;
	priv->stats.tx.tx_bytes += skb->len;

	handle->ops->tx_data(handle, skb);
	return 0;

err_drop:
	rnic_kfree_skb(skb);
	return -EFAULT;
}

STATIC int rnic_vnet_fragment_finish(struct sk_buff *skb, struct net_device *netdev)
{
	struct rnic_vnet_priv_s *priv = netdev_priv(netdev);

	skb_push(skb, ETH_HLEN);
	skb_reset_mac_header(skb);
	skb_set_network_header(skb, ETH_HLEN);
	eth_hdr(skb)->h_proto = skb->protocol;
	return rnic_vnet_output(priv, skb);
}

STATIC int rnic_vnet_fragment(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	if (unlikely(skb_mac_header_len(skb) != ETH_HLEN)) {
		rnic_kfree_skb(skb);
		priv->stats.tx.tx_carr_errs++;
		return -EPROTONOSUPPORT;
	}

	skb_pull(skb, ETH_HLEN);
	return rnic_fragment(skb, priv->netdev, rnic_vnet_fragment_finish);
}

STATIC struct sk_buff *rnic_vnet_tx_skb_clone(struct sk_buff *skb)
{
	skb->cb[RNIC_DROP_CODE_CB_POS] = rnic_vnet_sk_drop_code(skb);
	skb->cb[RNIC_SK_PRIO_CB_POS] = rnic_vnet_sk_acc(skb);

	if (!rnic_mem_flag_is_set(skb) && rnic_vnet_skb_can_clone(skb)) {
		struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
		consume_skb(skb);
		skb = skb2;
	}

	return skb;
}

STATIC bool rnic_vnet_tx_can_send(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	if (skb_linearize(skb)) {
		priv->stats.tx.tx_linear_errs++;
		return false;
	}

	/* Complete checksum if needed */
	if (skb->ip_summed == CHECKSUM_PARTIAL && unlikely(skb_checksum_help(skb))) {
		priv->stats.tx.tx_csum_errs++;
		return false;
	}

	if (unlikely(skb->len < ETH_HLEN)) {
		priv->stats.tx.tx_len_errs++;
		return false;
	}

	if (unlikely(test_bit(RNIC_VNET_STATE_DOWN, &priv->vnet_state))) {
		priv->stats.tx.tx_link_errs++;
		return false;
	}

	return true;
}

int rnic_vnet_tx_data(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	if (!rnic_vnet_tx_can_send(priv, skb))
		goto err_drop;

	skb = rnic_vnet_tx_skb_clone(skb);
	if (unlikely(skb == NULL)) {
		priv->stats.tx.tx_clone_errors++;
		goto err_out;
	}

	if (unlikely(priv->frag_en && !rnic_mem_flag_is_set(skb)))
		return rnic_vnet_fragment(priv, skb);

	return rnic_vnet_output(priv, skb);

err_drop:
	rnic_kfree_skb(skb);
err_out:
	return -EFAULT;
}

void rnic_vnet_relay_xmit(struct sk_buff *skb, unsigned int len, void *para, unsigned int flag)
{
	struct rx_cb_map_s *map = rnic_map_skb_cb(skb);

	skb->protocol = map->packet_info.bits.ip_proto == AF_INET ?
			htons(ETH_P_IP) : htons(ETH_P_IPV6);

	rnic_set_mem_flag(skb, 1);
	rnic_vnet_tx_data((struct rnic_vnet_priv_s *)para, skb);
}

STATIC bool rnic_vnet_rx_can_deliver(struct rnic_vnet_priv_s *priv, struct sk_buff *skb)
{
	if (unlikely(skb->len > RNIC_RX_MAX_LEN)) {
		priv->stats.rx.rx_len_errs++;
		return false;
	}

	if (unlikely(test_bit(RNIC_VNET_STATE_DOWN, &priv->vnet_state))) {
		priv->stats.rx.rx_link_errs++;
		return false;
	}

	if ((priv->link_state & RNIC_ADDRF_ALL) == 0) {
		priv->stats.rx.rx_carr_errs++;
		return false;
	}

	return true;
}

int rnic_vnet_rx_data(struct rnic_handle_s *handle, struct sk_buff *pskb)
{
	struct rnic_vnet_ctx_s *vctx = handle->priv;
	struct rnic_vnet_priv_s *priv = vctx->privs[rnic_map_pifid(pskb)];

	if (unlikely(!rnic_vnet_rx_can_deliver(priv, pskb)))
		goto err_drop;

	priv->stats.rx.rx_pkts++;
	priv->stats.rx.rx_bytes += pskb->len;

	if (rnic_map_pkttype(pskb) != RNIC_MAP_PKTTYPE_IP) {
		rnic_vnet_dpl_rx(pskb, handle->pfa_ver);
		rnic_vnet_lan_deliver_skb(priv, pskb);
		return 0;
	}
#ifndef CONFIG_DRA_PAGE
	if (unlikely(rnic_mem_flag_is_set(pskb))) {
		priv->stats.rx.rx_maa_errs++;
		goto err_drop;
	}
#endif
	if (handle->pfa_ver == PFA_VERSION_V200)
		skb_pull(pskb, ETH_HLEN);

	rnic_vnet_dpl_rx(pskb, handle->pfa_ver);

	pskb->dev = priv->netdev;
	pskb->protocol = (pskb->data[0] & RNIC_IPVER_MASK) == RNIC_IPVER_4 ?
			htons(ETH_P_IP) : htons(ETH_P_IPV6);

	rnic_checksum(pskb, &priv->stats.rx);

#ifdef CONFIG_RNIC_RX_SCHED
	rnic_vnet_rx_data_add_list(&vctx->rx_list, pskb);
#else
	__skb_queue_tail(&vctx->rx_list, pskb);
#endif
	return 0;

err_drop:
	rnic_kfree_skb_rx(pskb);
	return -EFAULT;
}

void rnic_vnet_rx_cmplt(struct rnic_handle_s *handle)
{
	struct rnic_vnet_ctx_s *vctx = handle->priv;
	void (*gro_rcv_burst)(struct sk_buff_head *) = NULL;

	rnic_vnet_dpl_rx_cmplt();

#ifdef CONFIG_RNIC_RX_SCHED
	if (!rnic_vnet_rx_wq_empty()) {
		rnic_vnet_rx_start_kwork();
		return;
	}
#endif

	if (unlikely(skb_queue_empty(&vctx->rx_list)))
		return;

	rcu_read_lock();
	gro_rcv_burst = rcu_dereference(rnic_gro_rcv_burst_entry);
	if (gro_rcv_burst != NULL)
		gro_rcv_burst(&vctx->rx_list);
	rcu_read_unlock();

	rnic_vnet_deliver_rxq(vctx);
}
