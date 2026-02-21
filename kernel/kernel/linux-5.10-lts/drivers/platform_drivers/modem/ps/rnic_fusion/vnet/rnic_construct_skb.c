/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
#include <net/ip6_checksum.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/if_ether.h>

#include "rnic_log.h"
#include "rnic_private.h"
#include "rnic_data.h"
#include "rnic_construct_skb.h"

#define RNIC_SKB_SHARED_INFO_SIZE SKB_DATA_ALIGN(sizeof(struct skb_shared_info))
#define RNIC_PAGE_SKB_TRUE_SIZE (PAGE_SIZE >> 1)
#define RNIC_PAGE_MASK (PAGE_SIZE -  1)
// addr must 2k align
#define RNIC_PAGE_OFFSET(addr) (((addr) & RNIC_PAGE_MASK) ? RNIC_PAGE_SKB_TRUE_SIZE : 0)
struct rnic_construct_skb_info {
	u64 skb_one_num;
	u64 skb_gro_num;
	u64 skb_gro_flist;
	u64 skb_failed;
};

struct rnic_construct_skb_info rnic_con_skb_info = { 0 };

STATIC void rnic_construct_skb_udp_flist(struct sk_buff *skb)
{
	if (skb->ip_summed == CHECKSUM_UNNECESSARY) {
		if (skb->csum_level < SKB_MAX_CSUM_LEVEL)
			skb->csum_level++;
	} else {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
		skb->csum_level = 0;
	}

	skb_shinfo(skb)->gso_type = SKB_GSO_FRAGLIST | SKB_GSO_UDP_L4;
}

STATIC void skb_construct_correct_skb_iphdr(struct sk_buff *skb, struct rnic_construct_skb_para *para)
{
	struct iphdr *ip4h = (struct iphdr *)skb->data;
	struct tcphdr *th;
	struct udphdr *uh;
	unsigned int iphdr_len = sizeof(struct iphdr);
	__be16 newlen;

	newlen = htons(skb->len);
	csum_replace2(&ip4h->check, ip4h->tot_len, newlen);
	ip4h->tot_len = newlen;

	switch (ip4h->protocol) {
	case IPPROTO_TCP:
		th = (struct tcphdr *)(ip4h + 1);
		th->check = ~csum_tcpudp_magic(ip4h->saddr, ip4h->daddr,
			skb->len - iphdr_len, IPPROTO_TCP, 0);

		tcp_flag_word(th) |= para->flags;
		skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;
		skb->csum_offset = offsetof(struct tcphdr, check);
		break;

	case IPPROTO_UDP:
		uh = (struct udphdr *)(ip4h + 1);
		uh->len = htons(skb->len - iphdr_len);
		if (para->flist) {
			rnic_construct_skb_udp_flist(skb);
			return;
		}

		if (uh->check)
			uh->check = ~csum_tcpudp_magic(ip4h->saddr, ip4h->daddr,
				skb->len - iphdr_len, IPPROTO_UDP, 0);

		skb->csum_offset = offsetof(struct udphdr, check);
		skb_shinfo(skb)->gso_type = SKB_GSO_UDP_L4;
		break;

	default:
		return;
	}

	skb->ip_summed = CHECKSUM_PARTIAL;
	skb->csum_start = (__u16)(skb->data - skb->head) + iphdr_len;
}

STATIC void skb_construct_correct_skb_ipv6hdr(struct sk_buff *skb,
					      struct rnic_construct_skb_para *para)
{
	struct ipv6hdr *ip6h = (struct ipv6hdr *)skb->data;
	struct tcphdr *th;
	struct udphdr *uh;
	unsigned int iphdr_len = sizeof(struct ipv6hdr);

	ip6h->payload_len = htons(skb->len - iphdr_len);

	switch (ip6h->nexthdr) {
	case IPPROTO_TCP:
		th = (struct tcphdr *)(ip6h + 1);
		th->check = ~tcp_v6_check((int)(skb->len - iphdr_len),
			&ip6h->saddr, &ip6h->daddr, 0);

		tcp_flag_word(th) |= para->flags;
		skb_shinfo(skb)->gso_type = SKB_GSO_TCPV6;
		skb->csum_offset = offsetof(struct tcphdr, check);
		break;

	case IPPROTO_UDP:
		uh = (struct udphdr *)(ip6h + 1);
		uh->len = htons(skb->len - iphdr_len);
		if (para->flist) {
			rnic_construct_skb_udp_flist(skb);
			return;
		}

		if (uh->check)
			uh->check = ~csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
				skb->len - iphdr_len, IPPROTO_UDP, 0);

		skb->csum_offset = offsetof(struct udphdr, check);
		skb_shinfo(skb)->gso_type = SKB_GSO_UDP_L4;
		break;

	default:
		return;
	}

	skb->ip_summed = CHECKSUM_PARTIAL;
	skb->csum_start = (__u16)(skb->data - skb->head) + iphdr_len;
}

STATIC void skb_construct_correct_skb_hdr(struct sk_buff *skb, struct rnic_construct_skb_para *para)
{
	if (skb->protocol == htons(ETH_P_IP))
		skb_construct_correct_skb_iphdr(skb, para);
	else
		skb_construct_correct_skb_ipv6hdr(skb, para);

	skb_shinfo(skb)->gso_size = para->size;
	skb_shinfo(skb)->gso_segs = para->num;
}

STATIC void rnic_construct_free_pskb_list(struct sk_buff *pskb)
{
	struct sk_buff *skb_next, *skb_head;

	skb_head = pskb;
	while(skb_head->next != NULL) {
		skb_next = skb_head->next;
		rnic_kfree_skb_rx(skb_head);
		skb_head = skb_next;
	}

	rnic_kfree_skb_rx(skb_head);
}

struct sk_buff *rnic_construct_skb_one(struct sk_buff *pskb)
{
	struct sk_buff *skb_new;
	struct sk_buff pskb_bk = *pskb;
	unsigned int truesize;

	truesize = pskb->end + RNIC_SKB_SHARED_INFO_SIZE;

	skb_new = build_skb((void *)pskb->head, truesize);
	if (unlikely(skb_new == NULL)) {
		rnic_construct_free_pskb_list(pskb);
		rnic_con_skb_info.skb_failed++;
		return NULL;
	}

	skb_reserve(skb_new, (pskb_bk.data - pskb_bk.head));

	skb_put(skb_new, pskb_bk.len);

	/* refresh csum offload result */
	skb_new->dev = pskb_bk.dev;
	skb_new->protocol = pskb_bk.protocol;
	skb_new->ip_summed = pskb_bk.ip_summed;
	skb_new->csum_level = pskb_bk.csum_level;

	*((struct rx_cb_map_s *)skb_new->cb) = *((struct rx_cb_map_s *)pskb_bk.cb);

	rnic_set_mem_flag(skb_new, 0);
	rnic_con_skb_info.skb_one_num++;
	return skb_new;
}

STATIC void rnic_construct_skb_add_data(struct sk_buff *first, struct sk_buff *skb_frag)
{
	first->len += skb_frag->len;
	first->data_len += skb_frag->len;
	first->truesize += skb_frag->truesize;
}

STATIC struct sk_buff *rnic_construct_skb_gro(struct sk_buff *pskb, struct sk_buff *skb_next,
					      struct rnic_construct_skb_para *para)
{
	struct sk_buff *skb_first, *skb_frag, *skb_ftemp, *skb_ntemp;
	unsigned int truesize = RNIC_PAGE_SKB_TRUE_SIZE;
	bool first_flag = true;

	skb_first = rnic_construct_skb_one(pskb);
	if (unlikely(skb_first == NULL)) {
		return NULL;
	}
	rnic_con_skb_info.skb_gro_num++;

	skb_frag = skb_first;
	while (skb_next != NULL) {
		if (unlikely(skb_shinfo(skb_frag)->nr_frags >= MAX_SKB_FRAGS)) {
			skb_ntemp = skb_next->next;
			skb_ftemp = rnic_construct_skb_one(skb_next);
			if (unlikely(skb_ftemp == NULL))
				goto free_skb;

			skb_set_mac_header(skb_ftemp, -ETH_HLEN);
			skb_set_network_header(skb_ftemp, 0);
			// release next skb_header
			__skb_pull(skb_ftemp, para->offset);

			if (first_flag) {
				skb_shinfo(skb_first)->frag_list = skb_ftemp;
				first_flag = false;
			} else {
				rnic_construct_skb_add_data(skb_first, skb_frag);
				skb_frag->next = skb_ftemp;
			}

			skb_frag = skb_ftemp;
			skb_next = skb_ntemp;
			continue;
		}
		// release next skb_header
		__skb_pull(skb_next, para->offset);
		skb_add_rx_frag(skb_frag, skb_shinfo(skb_frag)->nr_frags,
			virt_to_page(skb_next->head),
			RNIC_PAGE_OFFSET((uintptr_t)(skb_next->head)) + skb_headroom(skb_next),
			skb_next->len, truesize);

		skb_next = skb_next->next;
	}

	if (skb_shinfo(skb_first)->frag_list != NULL)
		rnic_construct_skb_add_data(skb_first, skb_frag);

	if (likely(skb_shinfo(skb_first)->nr_frags))
		skb_construct_correct_skb_hdr(skb_first, para);

	return skb_first;

free_skb:
	rnic_construct_free_pskb_list(skb_first);
	return NULL;
}

STATIC struct sk_buff *rnic_construct_skb_gro_list(struct sk_buff *pskb, struct sk_buff *skb_next,
						   struct rnic_construct_skb_para *para)
{
	struct sk_buff *skb_first, *skb_prev, *skb_ftemp, *skb_ntemp;
	bool first_flag = true;

	skb_first = rnic_construct_skb_one(pskb);
	if (unlikely(skb_first == NULL)) {
		return NULL;
	}
	rnic_con_skb_info.skb_gro_flist++;
	while (skb_next != NULL) {
		skb_ntemp = skb_next->next;
		skb_ftemp = rnic_construct_skb_one(skb_next);
		if (unlikely(skb_ftemp == NULL))
			goto free_skb;

		skb_set_mac_header(skb_ftemp, -ETH_HLEN);
		skb_set_network_header(skb_ftemp, 0);
		// remove the network header and transport header
		__skb_pull(skb_ftemp, para->offset);

		if (first_flag) {
			skb_shinfo(skb_first)->frag_list = skb_ftemp;
			first_flag = false;
		} else {
			skb_prev->next = skb_ftemp;
		}

		rnic_construct_skb_add_data(skb_first, skb_ftemp);

		skb_next = skb_ntemp;
		skb_prev = skb_ftemp;
	}

	if (likely(!first_flag))
		skb_construct_correct_skb_hdr(skb_first, para);

	return skb_first;

free_skb:
	rnic_construct_free_pskb_list(skb_first);
	return NULL;
}

struct sk_buff *rnic_construct_skb(struct sk_buff *pskb, struct rnic_construct_skb_para *para)
{
	if (pskb->next == NULL)
		return rnic_construct_skb_one(pskb);

	return para->flist ? rnic_construct_skb_gro_list(pskb, pskb->next, para) :
			     rnic_construct_skb_gro(pskb, pskb->next, para);
}

#ifndef LLT_OS_VER
u32 rnic_construct_skb_print(void)
{
	pr_err("rnic construct_skb one %llu, gro %llu, gro_flist %llu, failed %llu\n",
		rnic_con_skb_info.skb_one_num, rnic_con_skb_info.skb_gro_num,
		rnic_con_skb_info.skb_gro_flist, rnic_con_skb_info.skb_failed);
	return 0;
}
#endif