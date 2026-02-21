/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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
#include <net/ipv6.h>
#include <net/checksum.h>
#include <net/ip6_checksum.h>
#include <linux/udp.h>
#include <linux/tcp.h>

#include "rnic_data.h"
#include "rnic_private.h"
#include "rnic_vnet.h"

STATIC bool rnic_checksum_iphr_valid(struct sk_buff *skb, struct iphdr *iph)
{
	u32 len;

	if (unlikely(skb->len < sizeof(struct iphdr)))
		return false;

	if (unlikely(*(u8 *)iph != 0x45))
		return false;

	if (unlikely(ip_is_fragment(iph)))
		return false;

	/* ip header length value is 5 */
	if (unlikely(ip_fast_csum((u8 *)iph, 5)))
		return false;

	len = ntohs(iph->tot_len);
	if (unlikely(skb->len < len))
		return false;
        else if (unlikely(len < sizeof(struct iphdr)))
		return false;

	/* remove padding if exist */
	if (unlikely(skb->len > len))
		__skb_trim(skb, len);

	return true;
}

STATIC bool rnic_checksum_ip6hdr_valid(struct sk_buff *skb,
				       u32 payload_len, u8 protocal)
{
	u32 ip6hdr_len = sizeof(struct ipv6hdr);
	u32 l4hdr_len = (protocal == IPPROTO_TCP) ?
		sizeof(struct tcphdr) : sizeof(struct udphdr);

	/*
	 * IPv6 with non-upper-layer nxthdr at the begining would
	 * not be recognized as tcp6 by pfa.
	 */
	if (unlikely(skb->len < ip6hdr_len + l4hdr_len))
		return false;

	if (unlikely(payload_len < l4hdr_len))
		return false;

	/* padding */
	if (unlikely(skb->len > payload_len + ip6hdr_len))
		skb->len = payload_len + ip6hdr_len;

	/*
	 * If padding, next conidtion would always be false.
	 * Since there would be checksum later, these skb are allowed to move on.
	 */
	if (unlikely(payload_len != (skb->len - ip6hdr_len)))
		return false;

	return true;
}

STATIC void rnic_checksum_ip(struct sk_buff *skb, struct rnic_vnet_rx_stats_s *stats)
{
	struct iphdr *iph = (struct iphdr *)skb->data;
	struct udphdr *uh;
	u32 payload_len, ip_hdrlen;
	__sum16 subip_sum, psum, sum16;
	u8 protocal;

	if(unlikely(!rnic_checksum_iphr_valid(skb, iph))) {
		stats->rx_csum_hdr_errs++;
		return;
	}

	protocal = rnic_map_skb_cb(skb)->packet_info.bits.l4_proto;
	uh = (struct udphdr *)(iph + 1);
	if (protocal == IPPROTO_UDP && unlikely(!uh->check))
		return;

	ip_hdrlen = sizeof(struct iphdr);
	subip_sum = csum16_sub((__force __sum16)~(rnic_map_skb_cb(skb)->pfa_result.bits.checksum),
		    csum_fold(skb_checksum(skb, 0, ip_hdrlen, 0)));

	payload_len = skb->len - ip_hdrlen;

	// Pseudo header check
	psum = csum_fold(csum_tcpudp_nofold(iph->saddr, iph->daddr, payload_len, protocal, 0));

	// final value
	sum16 = ~csum16_add(subip_sum, psum);
	if (sum16) {
		stats->rx_csum_errs++;
		return;
	}

	skb->ip_summed = CHECKSUM_UNNECESSARY;
	skb->csum_level = 0;
}

STATIC void rnic_checksum_ipv6(struct sk_buff *skb, struct rnic_vnet_rx_stats_s *stats)
{
	struct ipv6hdr *iph6 = (struct ipv6hdr *)skb->data;
	struct udphdr *uh;
	u32 payload_len, ip_hdrlen;
	__sum16 subip_sum, psum, sum16;
	u8 protocal;

	payload_len = ntohs(iph6->payload_len);
	protocal = rnic_map_skb_cb(skb)->packet_info.bits.l4_proto;
	if(unlikely(!rnic_checksum_ip6hdr_valid(skb, payload_len, protocal))) {
		stats->rx_csum_hdr_errs++;
		return;
	}

	ip_hdrlen = sizeof(struct ipv6hdr);
	uh = (struct udphdr *)(iph6 + 1);
	if (protocal == IPPROTO_UDP && unlikely(!uh->check))
		return;

	subip_sum = csum16_sub((__force __sum16)~(rnic_map_skb_cb(skb)->pfa_result.bits.checksum),
		    csum_fold(skb_checksum(skb, 0, ip_hdrlen, 0)));

	// Pseudo header check
	psum = csum_ipv6_magic(&iph6->saddr, &iph6->daddr, payload_len, protocal, 0);

	// final value
	sum16 = ~csum16_add(subip_sum, psum);
	if (sum16) {
		stats->rx_csum_errs++;
		return;
	}

	skb->ip_summed = CHECKSUM_UNNECESSARY;
	skb->csum_level = 0;
}

STATIC bool rnic_checksum_support(struct sk_buff *skb)
{
	u8 protocal;

	if((rnic_map_pkttype(skb) != RNIC_MAP_PKTTYPE_IP) || !rnic_map_checksum_flag(skb))
		return false;

	protocal = rnic_map_skb_cb(skb)->packet_info.bits.l4_proto;
	if (protocal == IPPROTO_TCP || protocal == IPPROTO_UDP)
		return true;

	return false;
}

void rnic_checksum(struct sk_buff *skb, struct rnic_vnet_rx_stats_s *stats)
{
	if (!rnic_checksum_support(skb)) {
		stats->rx_csum_notsup++;
		return;
	}

	if (skb->protocol == htons(ETH_P_IP))
		rnic_checksum_ip(skb, stats);
	else
		rnic_checksum_ipv6(skb, stats);
}