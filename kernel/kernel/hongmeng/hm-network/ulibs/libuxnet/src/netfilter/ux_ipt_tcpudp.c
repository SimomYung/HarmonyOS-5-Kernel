/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ipatables TCP/UDP filter rule impl code
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 14:22:56 2021
 */
#include <stdbool.h>
#include <lib/dlist.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include "ux_netdev.h"
#include "ux_pktbuf.h"
#include "ux_iptables.h"
#include "ux_netfilter.h"
#include "ux_ipt_tcpudp.h"

#define UX_IPT_TCP_INV_SRCPT	0x01
#define UX_IPT_TCP_INV_DSTPT	0x02
#define UX_IPT_TCP_INV_FLAGS	0x04
#define UX_IPT_TCP_INV_MASK	0x0F

#define UX_IPT_UDP_INV_SRCPT	0x01
#define UX_IPT_UDP_INV_DSTPT	0x02
#define UX_IPT_UDP_INV_MASK	0x03
#define TCP_FLAG_OFFSET		13
#define IHL_BYTES		4

static inline bool port_match(u_int16_t min, u_int16_t max,
			      u_int16_t port, bool invert)
{
	return ((unsigned int)(port >= min && port <= max)) ^ ((unsigned int)invert);
}

static bool tcp_mt(const ux_pktbuf_t *skb,
		   const struct ux_netdev *in,
		   const struct ux_netdev *out,
		   const struct ux_ipt_match *match,
		   const void *matchinfo)
{
	struct ux_ipt_tcp *tcpinfo = (struct ux_ipt_tcp *)matchinfo;
	struct iphdr *ip;
	struct tcphdr *tcp;
	unsigned int iphdr_len;

	if (skb->len < sizeof(struct iphdr)) {
		return false;
	}

	ip = (struct iphdr *)skb->payload;
	iphdr_len = (unsigned int)(ip->ihl * IHL_BYTES);
	if (iphdr_len < sizeof(struct iphdr)) {
		return false;
	}

	if (ip->protocol != IPPROTO_TCP) {
		return false;
	}

	if (skb->len < iphdr_len + sizeof(struct tcphdr)) {
		return false;
	}

	tcp = (struct tcphdr *)((char *)ip + ip->ihl * IHL_BYTES);

	if (!port_match(tcpinfo->spts[0], tcpinfo->spts[1],
			ntohs(tcp->th_sport),
			!!((tcpinfo->invflags & UX_IPT_TCP_INV_SRCPT) != 0))) {
		return false;
	}

	if (!port_match(tcpinfo->dpts[0], tcpinfo->dpts[1],
			ntohs(tcp->th_dport),
			!!((tcpinfo->invflags & UX_IPT_TCP_INV_DSTPT) != 0))) {
		return false;
	}
	if (!fwinv((((unsigned char *)tcp)[TCP_FLAG_OFFSET] & tcpinfo->flg_mask) ==
		     tcpinfo->flg_cmp, tcpinfo->invflags, UX_IPT_TCP_INV_FLAGS)) {
		return false;
	}

	return true;
}

static bool udp_mt(const struct ux_pktbuf *skb, const struct ux_netdev *in,
		   const struct ux_netdev *out, const struct ux_ipt_match *match,
		   const void *matchinfo)
{
	struct ux_ipt_udp *udpinfo = (struct ux_ipt_udp *)matchinfo;
	struct iphdr *ip;
	struct udphdr *udp;
	unsigned int iphdr_len;

	if (skb->len < sizeof(struct iphdr)) {
		return false;
	}

	ip = (struct iphdr *)skb->payload;
	iphdr_len = (unsigned int)(ip->ihl * IHL_BYTES);
	if (iphdr_len < sizeof(struct iphdr)) {
		return false;
	}

	if (ip->protocol != IPPROTO_UDP) {
		return false;
	}

	if (skb->len < iphdr_len + sizeof(struct udphdr)) {
		return false;
	}

	udp = (struct udphdr *)((char *)ip + ip->ihl * IHL_BYTES);

	if (!port_match(udpinfo->spts[0], udpinfo->spts[1],
			ntohs(udp->uh_sport),
			!!((udpinfo->invflags & UX_IPT_UDP_INV_SRCPT) != 0))) {
		return false;
	}

	if (!port_match(udpinfo->dpts[0], udpinfo->dpts[1], ntohs(udp->uh_dport),
			!!((udpinfo->invflags & UX_IPT_UDP_INV_DSTPT) != 0))) {
		return false;
	}

	return true;
}

static int udp_checkentry(const char *tablename,
			  const struct ux_ipt_ip *ip,
			  const void *matchinfo,
			  unsigned int matchinfosize)
{
	UNUSED(tablename);
	const struct ux_ipt_udp *info = matchinfo;

	if ((ip->proto != IPPROTO_UDP) || ((ip->invflags & UX_IPT_INV_PROTO) != 0)) {
		return E_HM_INVAL;
	}

	if (matchinfosize != ux_ipt_align(sizeof(struct ux_ipt_udp))) {
		return E_HM_INVAL;
	}

	if ((info->invflags & ~UX_IPT_UDP_INV_MASK) != 0) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static int tcp_checkentry(const char *tablename,
			  const struct ux_ipt_ip *ip,
			  const void *matchinfo,
			  unsigned int matchinfosize)
{
	UNUSED(tablename);
	const struct ux_ipt_tcp *info = matchinfo;

	if ((ip->proto != IPPROTO_TCP) || ((ip->invflags & UX_IPT_INV_PROTO) != 0)) {
		return E_HM_INVAL;
	}

	if (matchinfosize != ux_ipt_align(sizeof(struct ux_ipt_tcp))) {
		return E_HM_INVAL;
	}

	if ((info->invflags & ~UX_IPT_TCP_INV_MASK) != 0) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static struct ux_ipt_match tcpudp_mt_reg[] = {
	{
		.name       = "tcp",
		.family     = AF_INET,
		.match      = tcp_mt,
		.matchsize  = sizeof(struct ux_ipt_tcp),
		.checkentry = tcp_checkentry,
	},
	{
		.name       = "udp",
		.family     = AF_INET,
		.match      = udp_mt,
		.matchsize  = sizeof(struct ux_ipt_udp),
		.checkentry = udp_checkentry,
	},
};

int init_tcpudp_mt(void)
{
	ux_add_ipt_match(&tcpudp_mt_reg[0]);
	ux_add_ipt_match(&tcpudp_mt_reg[1]);

	return 0;
}
