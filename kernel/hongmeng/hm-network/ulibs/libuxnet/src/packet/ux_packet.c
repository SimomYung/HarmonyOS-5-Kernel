/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interface for network packet
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 30 17:12:17 2019
 */

#include <stdlib.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <bits/ioctl.h>
#include <libhmsync/raw_rwlock.h>
#include <libstrict/strict.h>
#include <netinet/if_ether.h>
#include <libhmiov/iov_iter.h>

#include "hm_if_packet.h"
#include "ux_compat.h"
#include "ux_netlog.h"
#include "ux_domain.h"
#include "ux_sockopt.h"
#include "ux_packet.h"
#include "ux_vlan.h"

#include "security/sec_hook.h"
#include "ux_eventpoll.h"
#include "bpf/ux_bpf_filter.h"

static struct raw_rwlock ux_packet_lock = RAW_RDLOCK_INITIALIZER;
static DLIST_HEAD(ux_packet_list);

static void ux_sem_post_to_daemon(ux_epoll_context_t *epoll_ctx, uint32_t event)
{
	ux_epoll_wakeup_clean(epoll_ctx, event);
}

static void ux_packet_sock_free(ux_sock_t *sk)
{
	ux_packet_sock_t *pktsk = ux_packet_sk(sk);
	ux_buf_t *tmpbuf = NULL;
	int i;

	(void)raw_rwlock_wrlock(&ux_packet_lock);
	dlist_delete(&pktsk->head);
	(void)raw_rwlock_unlock(&ux_packet_lock);

	for (i = (int)ux_buf_queue_len(&pktsk->psk_receive_queue); i > 0; i--) {
		tmpbuf = (ux_buf_t *)ux_buf_queue_dqueue_first(&pktsk->psk_receive_queue);
		if (tmpbuf != NULL) {
			ux_packet_buf_t *tmp_packet_buf = container_of(tmpbuf, ux_packet_buf_t, ux_buf);
			free(tmp_packet_buf);
			tmpbuf = NULL;
			tmp_packet_buf = NULL;
		}
	}
	ux_packet_mclist_free(&pktsk->ml);
	free(pktsk);
}

static inline ux_packet_sock_t *pkt_sk(const ux_sock_t *sk)
{
	return (ux_packet_sock_t *)(void *)sk;
}

/* Attach a packet hook. */
static int ux_packet_do_bind(ux_sock_t *sk, ux_netdev_t *dev,
			     uint16_t protocol)
{
	ux_packet_sock_t *pktsk = pkt_sk(sk);

	if (pktsk->running) {
		pktsk->running = false;
		pktsk->protocol_num = 0;
		ux_dev_remove_pt(&pktsk->prot_hook);
		ux_sock_put(sk);
	}

	pktsk->protocol_num = protocol;
	pktsk->ifindex = dev->index;
	pktsk->prot_hook.type = protocol;
	pktsk->prot_hook.dev = dev;

	if (protocol == 0) {
		return 0;
	}
	ux_sock_get(sk);
	ux_dev_add_pt(&pktsk->prot_hook);
	pktsk->running = true;
	return 0;
}

static int ux_packet_bind_handler(ux_socket_t *sock, const struct sockaddr *addr,
			  socklen_t addrlen)
{
	struct sockaddr_ll *sll = (struct sockaddr_ll *)(void *)addr;
	ux_sock_t *sk = NULL;
	ux_netdev_t *dev = NULL;
	int ret = 0;

	if (addrlen < sizeof(struct sockaddr_ll)) {
		return E_HM_INVAL;
	}
	if (sll->sll_family != AF_PACKET) {
		return E_HM_INVAL;
	}
	if (sll->sll_ifindex == 0) {
		return E_HM_SOCKTNOSUPPORT;
	}

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);

	dev = ux_netdev_get_by_index(sll->sll_ifindex);
	if (dev == NULL) {
		ux_sock_put(sk);
		return E_HM_NODEV;
	}
	ret = ux_packet_do_bind(sk, dev, (sll->sll_protocol != 0) ? sll->sll_protocol : pkt_sk(sk)->protocol_num);

	ux_netdev_put(dev);
	ux_sock_put(sk);
	return ret;
}

static int ux_packet_bind(ux_socket_t *sock, const struct sockaddr *addr,
		socklen_t addrlen)
{
	return socket_bind_wrapper(sock, addr, addrlen, ux_packet_bind_handler);
}
static int ux_packet_close(ux_socket_t *sock)
{
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *pktsk = NULL;

	ux_clear_eventpoll_list(sock);
	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	pktsk = pkt_sk(sk);
	if (pktsk->running) {
		ux_dev_remove_pt(&pktsk->prot_hook);
		pktsk->running = false;
		pktsk->protocol_num = 0;
		/* put sk after dev_remove */
		ux_sock_put(sk);
	}
	ux_sock_put(sk);
	return 0;
}

static int create_eth_header(struct ethhdr *ehdr, uint16_t proto, const void *daddr,
			     const void *saddr, size_t len)
{
	int ret = 0;

	if (saddr == NULL || daddr == NULL) {
		return E_HM_INVAL;
	}

	ehdr->h_proto = htons(proto);
	ret = memcpy_s(ehdr->h_source, sizeof(ehdr->h_source), saddr, len);
	if (ret != 0) {
		return E_HM_POSIX_FAULT;
	}

	ret = memcpy_s(ehdr->h_dest, sizeof(ehdr->h_dest), daddr, len);
	if (ret != 0) {
		return E_HM_POSIX_FAULT;
	}
	return 0;
}

static int do_dgram_sendto(ux_netdev_t *dev, const void *buf, int cplen, const uint8_t *addr,
			   uint16_t proto)
{
	uint8_t saddr[ETH_ALEN];
	char *uxbuf = NULL;
	int ret;

	uxbuf = calloc(1, (unsigned int)cplen);
	if (uxbuf == NULL) {
		return E_HM_NOMEM;
	}

	NOFAIL(memset_s(saddr, ETH_ALEN, 0, ETH_ALEN));

	ret = ux_netdev_get_mac(dev->name, saddr, ETH_ALEN);
	if (ret != 0) {
		goto out;
	}

	ret = create_eth_header((struct ethhdr *)(uintptr_t)uxbuf, ntohs(proto), addr, saddr, ETH_ALEN);
	if (ret != 0) {
		goto out;
	}

	ret = memcpy_s((void *)(uxbuf + ETH_HLEN), ((unsigned int)cplen - ETH_HLEN), buf,
		       ((unsigned int)cplen - ETH_HLEN));
	if (ret != 0) {
		ret = E_HM_POSIX_FAULT;
		goto out;
	}
	ret = ux_netdev_xmit(dev, (void *)uxbuf, cplen, 1);

out:
	free(uxbuf);
	return ret;
}

static int do_sendto(const struct ux_packet_sock *psk, const void *buf, size_t len,
		     const struct sockaddr *daddr, socklen_t addrlen)
{
	struct sockaddr_ll *sll = (struct sockaddr_ll *)(void *)daddr;
	ux_netdev_t *dev = NULL;
	uint8_t *addr = NULL;
	uint16_t proto;
	int ret;
	size_t cplen;
	int mtu;

	if (sll == NULL) {
		dev = ux_netdev_get_by_index(psk->ifindex);
		proto = psk->protocol_num;
	} else {
		if (addrlen < sizeof(struct sockaddr_ll)) {
			return E_HM_INVAL;
		}
		if (sll->sll_family != AF_PACKET) {
			return E_HM_INVAL;
		}

		dev = ux_netdev_get_by_index(sll->sll_ifindex);
		addr = sll->sll_addr;
		proto = sll->sll_protocol;
	}

	if (dev == NULL) {
		return E_HM_NODEV;
	}

	ret = __ux_netdev_get_mtu(dev, &mtu);
	if (ret != E_HM_OK) {
		ux_netdev_put(dev);
		return E_HM_INVAL;
	}

	if (psk->type == SOCK_DGRAM) {
		if (len > (size_t)(unsigned int)mtu) {
			ux_netdev_put(dev);
			return E_HM_MSGSIZE;
		}
		cplen = len + ETH_HLEN;
		ret = do_dgram_sendto(dev, buf, (int)cplen, addr, proto);
	} else {
		cplen = len;
		if (cplen > (size_t)(unsigned int)mtu + ETH_HLEN) {
			ux_netdev_put(dev);
			return E_HM_MSGSIZE;
		}
		ret = ux_netdev_xmit(dev, (void *)buf, (int)cplen, 1);
	}
	ux_netdev_put(dev);
	return (ret != E_HM_OK) ? ret : (int)cplen;
}

static ssize_t packet_sendto_handler(ux_socket_t *sock, const void *buf, size_t len,
				int flags, const struct sockaddr *daddr,
				socklen_t addrlen)
{
	ux_sock_t *sk = NULL;
	struct ux_packet_sock *psk = NULL;
	ssize_t ret = 0;

	if (len == 0) {
		return 0;
	}
	if (((unsigned int)flags & ~MSG_DONTWAIT) != 0) {
		return E_HM_INVAL;
	}

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	psk = pkt_sk(sk);

	ret = do_sendto(psk, buf, len, daddr, addrlen);

	return ret;
}

static ssize_t ux_packet_sendto(ux_socket_t *sock, const void *buf, size_t len,
				int flags, const struct sockaddr *daddr,
				socklen_t addrlen)
{
	return socket_sendto_wrapper(sock, buf, len, flags, daddr, addrlen,
				     packet_sendto_handler);
}

static ssize_t packet_send_handler(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	return packet_sendto_handler(sock, buf, len, flags, NULL, 0);
}

static ssize_t ux_packet_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	return socket_send_wrapper(sock, buf, len, flags, packet_send_handler);
}

static ssize_t packet_sendmsg_handler(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	int i;
	ssize_t ret = 0;
	ssize_t buflen = 0;

	for (i = 0; i < msg->msg_iovlen; i++) {
		ret = ux_packet_sendto(sock, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len,
				       flags, msg->msg_name, msg->msg_namelen);
		if (ret < 0) {
			return ret;
		}
		buflen = (ssize_t)(buflen + ret);
	}
	return buflen;
}

static ssize_t ux_packet_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	return socket_sendmsg_wrapper(sock, msg, flags, packet_sendmsg_handler);
}

static int ux_exec_filter(const struct ux_sock *sk, const void *buf, unsigned int data_len)
{
	unsigned int ret_data_len = data_len;
	if (sk != NULL && sk->filter && sk->filter->prog) {
		ret_data_len = ux_bpf_code_exec(buf, sk->filter->prog, (int)data_len);
	}
	return (int)ret_data_len;
}

static void __drops_inc(ux_packet_sock_t *psk)
{
	psk->stats.drops++;
}

static void drops_inc(ux_packet_sock_t *psk)
{
	ux_buf_queue_lock(&psk->psk_receive_queue);
	__drops_inc(psk);
	ux_buf_queue_unlock(&psk->psk_receive_queue);
}

static int match_packet_type(const struct ethhdr *ehdr, const struct ux_packet_type *target)
{
	if ((ehdr->h_proto == target->type) || (target->type == htons(ETH_P_ALL)))
		return 1;
	return 0;
}

static inline bool eth_type_vlan(uint16_t ethertype)
{
	switch (ethertype) {
	case ETH_P_8021Q:
	case ETH_P_8021AD:
		return true;
	default:
		return false;
	}
}

static void assign_pkt_info(ux_packet_buf_t *pkt_buf, ux_netdev_t *dev,
			    size_t len, int isxmit)
{
	if (isxmit == 1) {
		pkt_buf->pkttype = PACKET_OUTGOING;
	}

	pkt_buf->devindex = dev->index;
	pkt_buf->devtype = dev->type;
	pkt_buf->ux_buf.data_len = len;
	if (dev->priv_ops->get_mac) {
		dev->priv_ops->get_mac(dev, pkt_buf->dev_addr, pkt_buf->dev_halen);
	}
}

static void set_packet_buf_hdr_info(ux_packet_buf_t *pkt_buf, unsigned int total)
{
	struct ethhdr *eth_header = (struct ethhdr *)(uintptr_t)pkt_buf->ux_buf.data;
	struct vlanhdr *vlan_header = NULL;
	pkt_buf->nethdr_offset = sizeof(struct ethhdr);
	pkt_buf->vlan_present = false;
	if (eth_type_vlan(ntohs(eth_header->h_proto)) && total >= (sizeof(struct vlanhdr) + sizeof(struct ethhdr))) {
		vlan_header = (struct vlanhdr *)(uintptr_t)(eth_header + 1);
		pkt_buf->vlan_present = true;
		pkt_buf->vlan_tci = ntohs(vlan_header->tp_vid);
		pkt_buf->vlan_tpid = ntohs(vlan_header->tp_proto);
		pkt_buf->nethdr_offset += sizeof(struct vlanhdr);
	}
}

static void do_msg_inqueue(ux_packet_sock_t *psk, ux_packet_buf_t *buf)
{
	ux_packet_buf_t *pkt_buf = buf;
	ux_buf_queue_lock(&psk->psk_receive_queue);
	if (ux_buf_len(&psk->psk_receive_queue) + pkt_buf->ux_buf.data_len >
	    (size_t)(uint32_t)psk->sk.rcvbuf) {
		free(pkt_buf);
		pkt_buf = NULL;
		__drops_inc(psk);
		ux_buf_queue_unlock(&psk->psk_receive_queue);
		return;
	}
	ux_buf_queue_insert_tail(&psk->psk_receive_queue, &pkt_buf->ux_buf);
	psk->stats.packets++;
	ux_sem_signal(&psk->recvsem);
	ux_sem_post_to_daemon(&psk->sk.epoll_ctx, EPOLLIN);
	ux_buf_queue_unlock(&psk->psk_receive_queue);
	return;
}
/* flags:  1 means raw packet, 0 means pbuf or sbuf packet */
static int dev_callbak_func(const void *data, int data_len, ux_netdev_t *dev,
			    struct ux_packet_type *pt, int flags, int isxmit)
{
	ux_sock_t *sk = pt->af_packet_priv;
	ux_packet_sock_t *psk = pkt_sk(sk);
	ux_packet_buf_t *pkt_buf = NULL;
	unsigned int total = (unsigned int)data_len;
	size_t hdr_len = 0;
	int ret;
	ux_copy_data_args_t args;

	if (dev->priv_ops->get_packet_hdr_len != NULL) {
		hdr_len = dev->priv_ops->get_packet_hdr_len(dev, data, (size_t)(uint32_t)data_len, flags);
	}

	total = ((unsigned int)data_len) + ((unsigned int)hdr_len);

	pkt_buf = (ux_packet_buf_t *)calloc(1, (sizeof(ux_packet_buf_t) + total));
	if (pkt_buf == NULL) {
		drops_inc(psk);
		return E_HM_NOMEM;
	}

	pkt_buf->len = (unsigned int)data_len;
	if (hdr_len != 0 && dev->priv_ops->set_packet_hdr != NULL) {
		(void)dev->priv_ops->set_packet_hdr(dev, pkt_buf->ux_buf.data, data, (size_t)(uint32_t)data_len, flags);
	}

	args.dst = (void *)((char *)pkt_buf->ux_buf.data + hdr_len);
	args.dstlen = (size_t)(unsigned int)data_len;
	args.src = data;
	args.count = (size_t)(unsigned int)data_len;
	args.offset = 0;
	args.flags = flags;
	if ((ret = ux_netdev_copy_data(dev, &args)) <= 0) {
		drops_inc(psk);
		goto err_free;
	}

	if ((ret = match_packet_type((struct ethhdr *)(uintptr_t)pkt_buf->ux_buf.data, pt)) == 0) {
		goto err_free;
	}

	set_packet_buf_hdr_info(pkt_buf, total);
	/* NOTE: hack for wpa_supplicant to recv ETH_P_PAE packet
	 * Remove it when filter supports `ldb #type = skb->pkt_type`
	 * */
	ret = ux_exec_filter(sk, pkt_buf->ux_buf.data, (unsigned int)total);
	if ((ret == 0) && (pt->type != htons(ETH_P_PAE))) {
		goto err_free;
	}

	pkt_buf->snaplen = ((unsigned int)ret < total) ? (unsigned int)ret : total;
	if (psk->type == SOCK_DGRAM) {
		pkt_buf->ux_buf.start += ETH_HLEN;
		total -= ETH_HLEN;
	}
	assign_pkt_info(pkt_buf, dev, total, isxmit);
	do_msg_inqueue(psk, pkt_buf);
	return 0;

err_free:
	free(pkt_buf);
	pkt_buf = NULL;
	return ret;
}

static int assign_sll(struct sockaddr *saddr, socklen_t *addrlen,
		      ux_packet_buf_t *pkt_buf)
{
	struct ethhdr *eth_hdr = NULL;
	struct sockaddr_ll *sll = (struct sockaddr_ll *)(void *)saddr;

	if ((sll != NULL) && (addrlen != NULL) && (*addrlen != 0)) {
		if (*addrlen < sizeof(struct sockaddr_ll)) {
			return E_HM_INVAL;
		}
		eth_hdr = (struct ethhdr *)(void *)pkt_buf->ux_buf.data;
		sll->sll_pkttype = pkt_buf->pkttype;
		sll->sll_family = AF_PACKET;
		sll->sll_hatype = pkt_buf->devtype ;
		sll->sll_protocol = eth_hdr->h_proto;
		sll->sll_ifindex = pkt_buf->devindex;
		sll->sll_halen = pkt_buf->dev_halen;
		if (memcpy_s(sll->sll_addr, SSL_ADDR_LEN, eth_hdr->h_source,
			     sizeof(eth_hdr->h_source)) != 0) {
			return E_HM_INVAL;
		}
		*addrlen = sizeof(struct sockaddr_ll);
	}
	return 0;
}

static ux_buf_t *get_buf_queue_first(ux_buf_head_t *buf_queue, bool peek)
{
	if (peek) {
		return ux_buf_queue_first(buf_queue);
	}
	return ux_buf_queue_dqueue_first(buf_queue);
}

static ssize_t do_packet_recv(ux_socket_t *sock, void *buf, size_t len, int flags,
			      struct sockaddr *saddr, socklen_t *addrlen)
{
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *psk = NULL;
	ux_packet_buf_t *pkt_buf = NULL;
	ux_buf_t *uxbuf = NULL;
	bool peek = ((((unsigned int)flags) & MSG_PEEK) != 0);
	size_t copylen = 0;
	ssize_t ret;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	psk = pkt_sk(sk);

	uxbuf = get_buf_queue_first(&psk->psk_receive_queue, peek);
	if (uxbuf == NULL) {
		ux_buf_queue_unlock(&psk->psk_receive_queue);
		ux_sock_put(sk);
		return E_HM_POSIX_NOENT;
	}

	if (!peek && ux_buf_queue_first(&psk->psk_receive_queue) == NULL) {
		ux_sem_post_to_daemon(&psk->sk.epoll_ctx, EPOLLIN << CLEAN_EVENT_SHIFT);
	}
	ux_buf_queue_unlock(&psk->psk_receive_queue);

	pkt_buf = container_of(uxbuf, ux_packet_buf_t, ux_buf);
	if (len != 0) {
		copylen = (len < uxbuf->data_len) ? len : uxbuf->data_len;
		ret = memcpy_s(buf, len, uxbuf->data + uxbuf->start, copylen);
		if (ret != 0) {
			ret = E_HM_INVAL;
			goto out;
		}
	}

	len = (((unsigned int)flags & MSG_TRUNC) != 0U) ? uxbuf->data_len : copylen;
	ret = assign_sll(saddr, addrlen, pkt_buf);
	if (ret != 0) {
		goto out;
	}

	if ((sock->flags & UX_SOCK_TIMESTAMP) != 0U) {
		(void)gettimeofday(&sock->ts, NULL);
	}
	ret = (ssize_t)len;

out:
	if (!peek) {
		free(pkt_buf);
	}
	pkt_buf = NULL;

	ux_sock_put(sk);
	return ret;
}

static ssize_t packet_recvfrom_handler(ux_socket_t *sock, void *buf, size_t len, int flags,
				  struct sockaddr *saddr, socklen_t *addrlen)
{
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *psk = NULL;
	ssize_t ret;

	if (((unsigned int)flags & ~(MSG_TRUNC | MSG_DONTWAIT | MSG_PEEK)) != 0U) {
		return E_HM_INVAL;
	}

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	psk = pkt_sk(sk);

	ux_buf_queue_lock(&psk->psk_receive_queue);
	if (ux_buf_queue_len(&psk->psk_receive_queue) == 0) {
		if (sk_is_noblock(sk) || (((unsigned int)flags & MSG_DONTWAIT) != 0U)) {
			ux_buf_queue_unlock(&psk->psk_receive_queue);
			ux_sock_put(sk);
			return E_HM_AGAIN;
		}
		/* Clear accumulated expired signals */
		if (raw_sem_init(&psk->recvsem, 0) != 0) {
			ux_buf_queue_unlock(&psk->psk_receive_queue);
			ux_sock_put(sk);
			return E_HM_INVAL;
		}
		ux_buf_queue_unlock(&psk->psk_receive_queue);
		ret = ux_sem_cond_waiti(&(psk->recvsem), sk->rcvtimeo);
		if (ret < 0) {
			ux_sock_put(sk);
			return ret == E_HM_TIMEDOUT ? E_HM_AGAIN : ret;
		}
		ux_buf_queue_lock(&psk->psk_receive_queue);
	}

	ret = do_packet_recv(sock, buf, len, flags, saddr, addrlen);
	ux_sock_put(sk);
	return ret;
}

static ssize_t ux_packet_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
				  struct sockaddr *saddr, socklen_t *addrlen)
{
	return socket_recvfrom_wrapper(sock, buf, len, flags, saddr, addrlen,
				       packet_recvfrom_handler);
}

static ssize_t ux_packet_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags)
{
	UNUSED(flags);
	return socket_writev_wrapper(sock, iov, iovcnt, packet_sendmsg_handler);
}

static ssize_t ux_packet_recv(ux_socket_t *sock, void *buf, size_t len, int flags)
{
	return ux_packet_recvfrom(sock, buf, len, flags, NULL, NULL);
}

static int ux_put_cmsg(struct msghdr *msg, int level, int type, size_t len, uintptr_t data)
{
	size_t cmlen = (size_t)CMSG_LEN(len);
	struct cmsghdr *cm = msg->msg_control;
	int ret;

	if (!msg->msg_control || msg->msg_controllen < sizeof(struct cmsghdr)) {
		msg->msg_flags |= MSG_CTRUNC;
		return E_HM_OK;
	}
	if (msg->msg_controllen < cmlen) {
		msg->msg_flags |= MSG_CTRUNC;
		cmlen = msg->msg_controllen;
	}

	cm->cmsg_level = level;
	cm->cmsg_type = type;
	cm->cmsg_len = (socklen_t)cmlen;
	ret = memcpy_s(CMSG_DATA(cm), cmlen - sizeof(*cm), (void *)data, len);
	if (ret != 0) {
		net_warn(HMNET_PACKET, "memcpy fail, ret: %d", ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static bool ux_packet_check_flag(int flags)
{
	if (!is_recv_flag_valid((unsigned int)flags)) {
		return false;
	}

	return ((unsigned int)flags & ~(MSG_TRUNC | MSG_DONTWAIT | MSG_PEEK)) == 0U;
}

static void ux_packet_set_auxdata(ux_packet_buf_t *pkt_buf, struct msghdr *msg)
{
	struct tpacket_auxdata aux;
	int ret;

	mem_zero_s(aux);
	aux.tp_status = TP_STATUS_USER;
	aux.tp_len = pkt_buf->len;
	aux.tp_snaplen = pkt_buf->snaplen;
	aux.tp_net = pkt_buf->nethdr_offset;
	if (pkt_buf->vlan_present) {
		aux.tp_vlan_tci = pkt_buf->vlan_tci;
		aux.tp_vlan_tpid = pkt_buf->vlan_tpid;
		aux.tp_status |= TP_STATUS_VLAN_VALID | TP_STATUS_VLAN_TPID_VALID;
	}
	ret = ux_put_cmsg(msg, SOL_PACKET, PACKET_AUXDATA, sizeof(aux), (uintptr_t)&aux);
	if (ret != E_HM_OK) {
		net_warn(HMNET_PACKET, "ux_put_cmsg fail, ret: %d", ret);
	}
}

static int ux_packet_get_uxbuf(ux_sock_t *sk, bool peek, int flags, ux_buf_t **uxbuf)
{
	ux_packet_sock_t *psk = pkt_sk(sk);
	int ret = E_HM_OK;

	ux_buf_queue_lock(&psk->psk_receive_queue);
	if (ux_buf_queue_len(&psk->psk_receive_queue) == 0) {
		if (sk_is_noblock(sk) || (((unsigned int)flags & MSG_DONTWAIT) != 0U)) {
			ux_buf_queue_unlock(&psk->psk_receive_queue);
			return E_HM_AGAIN;
		}
		if (raw_sem_init(&psk->recvsem, 0) != 0) {
			ux_buf_queue_unlock(&psk->psk_receive_queue);
			return E_HM_INVAL;
		}
		ux_buf_queue_unlock(&psk->psk_receive_queue);
		ret = ux_sem_cond_waiti(&(psk->recvsem), sk->rcvtimeo);
		if (ret < 0) {
			return ret == E_HM_TIMEDOUT ? E_HM_AGAIN : ret;
		}
		ux_buf_queue_lock(&psk->psk_receive_queue);
	}

	*uxbuf = get_buf_queue_first(&psk->psk_receive_queue, peek);
	if (*uxbuf == NULL) {
		ux_buf_queue_unlock(&psk->psk_receive_queue);
		return E_HM_POSIX_NOENT;
	}

	if (!peek && ux_buf_queue_first(&psk->psk_receive_queue) == NULL) {
		ux_sem_post_to_daemon(&psk->sk.epoll_ctx, EPOLLIN << CLEAN_EVENT_SHIFT);
	}
	ux_buf_queue_unlock(&psk->psk_receive_queue);
	return ret;
}

static inline void packet_free_no_peek(ux_packet_buf_t *pkt_buf, bool peek)
{
	if (!peek) {
		free(pkt_buf);
	}
}

static ssize_t packet_recvmsg_handler(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	ssize_t buflen = 0;
	int ret;
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *psk = NULL;
	ux_packet_buf_t *pkt_buf = NULL;
	ux_buf_t *uxbuf = NULL;
	bool peek = ((((unsigned int)flags) & MSG_PEEK) != 0U);
	struct iov_iter i;

	if (!ux_packet_check_flag(flags)) {
		return E_HM_INVAL;
	}
	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ret = ux_packet_get_uxbuf(sk, peek, flags, &uxbuf);
	if (ret != E_HM_OK) {
		return ret;
	}

	pkt_buf = container_of(uxbuf, ux_packet_buf_t, ux_buf);
	ret = iov_iter_init(&i, msg->msg_iov, (unsigned long)(unsigned int)msg->msg_iovlen);
	if (ret != E_HM_OK) {
		packet_free_no_peek(pkt_buf, peek);
		return (ssize_t)ret;
	}
	buflen = copy_data_to_iter(&i, uxbuf->data + uxbuf->start, uxbuf->data_len);
	if (buflen < 0) {
		packet_free_no_peek(pkt_buf, peek);
		return buflen;
	}

	if (buflen < (ssize_t)uxbuf->data_len) {
		msg->msg_flags = (int)((unsigned int)msg->msg_flags | MSG_TRUNC);
	}
	if ((((unsigned int)flags) & MSG_TRUNC) != 0U) {
		buflen = (ssize_t)uxbuf->data_len;
	}

	ret = assign_sll((struct sockaddr *)msg->msg_name, &msg->msg_namelen, pkt_buf);
	if (ret != 0) {
		packet_free_no_peek(pkt_buf, peek);
		return ret;
	}

	if ((sock->flags & UX_SOCK_TIMESTAMP) != 0U) {
		(void)gettimeofday(&sock->ts, NULL);
	}

	psk = pkt_sk(sk);
	if (psk->auxdata) {
		ux_packet_set_auxdata(pkt_buf, msg);
	}

	packet_free_no_peek(pkt_buf, peek);
	return buflen;
}

static ssize_t ux_packet_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	return socket_recvmsg_wrapper(sock, msg, flags, packet_recvmsg_handler);
}

static ssize_t ux_packet_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	return socket_readv_wrapper(sock, iov, iovcnt, packet_recvmsg_handler);
}

static ux_packet_sock_t *ux_packet_sock_alloc_and_init(ux_socket_t *sock, int domain, int type, int protocol)
{
	ux_packet_sock_t *pktsk = NULL;

	pktsk = pkt_sk(ux_sock_alloc(sock, sizeof(ux_packet_sock_t), 0));
	if (pktsk == NULL) {
		return NULL;
	}

	if (ux_sem_init(&pktsk->recvsem, 0) != 0) {
		free(pktsk);
		return NULL;
	}

	ux_buf_queue_init(&pktsk->psk_receive_queue);
	ux_packet_mclist_init(&pktsk->ml);
	ux_sock_init(&pktsk->sk, NULL, domain, type, protocol);
	pktsk->sk.release = ux_packet_sock_free;
	dlist_init(&pktsk->head);

	return pktsk;
}

static int ux_packet_ioctl(ux_socket_t *sock, int cmd, uintptr_t arg)
{
	struct timeval tv = {0};
	int ret = 0;

	if ((void *)arg == NULL) {
		return E_HM_INVAL;
	}

	switch (cmd) {
	case SIOCGSTAMP_OLD:
		ret = ux_socket_get_timestamp(sock, &tv);
		if (ret == 0 &&
		    (hm_actv_write_xact_vm((void *)arg, &tv, sizeof(struct timeval)) != 0)) {
			ret = E_HM_POSIX_FAULT;
		}
		break;
	case SIOCGSTAMP_NEW:
		ret = ux_socket_get_timestamp(sock, &tv);
		if (ret == 0 &&
		    (ux_set_compat_timeval(&tv, (void *)arg) != 0)) {
			ret = E_HM_POSIX_FAULT;
		}
		break;
	default:
		return E_HM_NOIOCTLCMD;
	}
	return ret;
}

static int getsockopt_statistics(ux_packet_sock_t *psk, void *optval,
				 socklen_t *optlen)
{
	struct ux_packet_stats stats = {0};
	socklen_t len = 0;

	if (hm_actv_read_xact_vm(&len, optlen, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	ux_buf_queue_lock(&psk->psk_receive_queue);
	stats = psk->stats;
	stats.packets += stats.drops;
	NOFAIL(memset_s(&psk->stats, sizeof(psk->stats), 0, sizeof(stats)));
	ux_buf_queue_unlock(&psk->psk_receive_queue);

	if (len > sizeof(struct ux_packet_stats)) {
		len = sizeof(struct ux_packet_stats);
	}

	if (hm_actv_write_xact_vm(optlen, &len, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (hm_actv_write_xact_vm(optval, &stats, len) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_auxdata(ux_packet_sock_t *psk, uintptr_t optval,
				 socklen_t *optlen)
{
	socklen_t len = 0;

	if (hm_actv_read_xact_vm(&len, optlen, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (len > sizeof(psk->auxdata)) {
		len = sizeof(psk->auxdata);
	}

	if (hm_actv_write_xact_vm(optlen, &len, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (hm_actv_write_xact_vm((void *)optval, &psk->auxdata, len) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return E_HM_OK;
}

static inline int dev_ndo_ioctl(ux_netdev_t *dev, int request, struct ifreq *ifr)
{
	int ret = E_HM_OPNOTSUPP;
	if (dev->ops != NULL && dev->ops->ndo_ioctl != NULL) {
		ret = dev->ops->ndo_ioctl(dev, request, (unsigned long)&ifr, false);
		if (ret != 0) {
			net_error(HMNET_PACKET, "ndo_ioctl fail, request: %d, ret: %d\n", request, ret);
		}
	}
	return ret;
}

void ux_packet_mclist_free(ux_packet_mclist_t *psk_mclist)
{
	ux_packet_mc_t *psk_mc = NULL;
	struct ifreq ifr;
	int ret;
	while (!dlist_empty(&psk_mclist->node)) {
		psk_mc = dlist_first_entry(&psk_mclist->node, ux_packet_mc_t, node);
		dlist_delete(&psk_mc->node);
		ux_netdev_t *dev = ux_netdev_get_by_index(psk_mc->ifindex);
		if (dev == NULL) {
			free(psk_mc);
			psk_mc = NULL;
			continue;
		}
		mem_zero_s(ifr);
		ret = memcpy_s(ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data),
			psk_mc->addr, psk_mc->alen);
		if (ret != 0) {
			net_warn(HMNET_PACKET, "memcpy_s fail, ret:%d", ret);
		}
		(void)dev_ndo_ioctl(dev, SIOCDELMULTI, &ifr);
		free(psk_mc);
		psk_mc = NULL;
		ux_netdev_put(dev);
	}
}

static int packet_add_mc_list(ux_packet_sock_t *psk, ux_netdev_t *dev, struct packet_mreq *mreq)
{
	int ret;
	struct ifreq ifr;
	ux_packet_mc_t *iterator_psk_mc = NULL;
	ux_packet_mc_t *mreq_psk_mc = NULL;

	ret = memcpy_s(ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data),
		mreq->mr_address, (size_t)mreq->mr_alen);
	if (ret != 0) {
		net_warn(HMNET_PACKET, "memcpy_s fail, ret:%d", ret);
	}

	ux_packet_mclist_lock(&psk->ml);
	dlist_for_each_entry(iterator_psk_mc, &psk->ml.node, ux_packet_mc_t, node) {
		if (iterator_psk_mc->ifindex == mreq->mr_ifindex &&
			iterator_psk_mc->type == mreq->mr_type &&
			iterator_psk_mc->alen == mreq->mr_alen &&
			memcmp(iterator_psk_mc->addr, mreq->mr_address, mreq->mr_alen) == 0) {
				iterator_psk_mc->count++;
				ux_packet_mclist_unlock(&psk->ml);
				return E_HM_OK;
			}
	}

	mreq_psk_mc = (ux_packet_mc_t *)calloc(1, sizeof(ux_packet_mc_t));
	if (mreq_psk_mc == NULL) {
		ux_packet_mclist_unlock(&psk->ml);
		return E_HM_NOBUFS;
	}

	mem_zero_s(*mreq_psk_mc);
	mreq_psk_mc->type = mreq->mr_type;
	mreq_psk_mc->ifindex = mreq->mr_ifindex;
	mreq_psk_mc->alen = mreq->mr_alen;
	mreq_psk_mc->count = 1;
	ret = memcpy_s(mreq_psk_mc->addr, sizeof(mreq_psk_mc->addr),
		mreq->mr_address, mreq->mr_alen);
	if (ret != 0) {
		net_warn(HMNET_PACKET, "memcpy_s fail, ret:%d", ret);
	}

	ret = dev_ndo_ioctl(dev, SIOCADDMULTI, &ifr);
	if (ret == E_HM_OK) {
		dlist_insert_tail(&psk->ml.node, &mreq_psk_mc->node);
	} else {
		free(mreq_psk_mc);
	}
	ux_packet_mclist_unlock(&psk->ml);
	return ret;
}

static int packet_del_mc_list(ux_packet_sock_t *psk, ux_netdev_t *dev, struct packet_mreq *mreq)
{
	int ret;
	struct ifreq ifr;
	ux_packet_mc_t *iterator_psk_mc = NULL;
	ux_packet_mc_t *tmp = NULL;

	ret = memcpy_s(ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data),
		mreq->mr_address, (size_t)mreq->mr_alen);
	if (ret != 0) {
		net_warn(HMNET_PACKET, "memcpy_s fail, ret:%d", ret);
	}
	ret = E_HM_OK;
	ux_packet_mclist_lock(&psk->ml);
	dlist_for_each_entry_safe(iterator_psk_mc, tmp, &psk->ml.node, ux_packet_mc_t, node) {
		if (iterator_psk_mc->ifindex == mreq->mr_ifindex &&
			iterator_psk_mc->type == mreq->mr_type &&
			iterator_psk_mc->alen == mreq->mr_alen &&
			memcmp(iterator_psk_mc->addr, mreq->mr_address, iterator_psk_mc->alen) == 0) {
				if (--iterator_psk_mc->count == 0) {
					ret = dev_ndo_ioctl(dev, SIOCDELMULTI, &ifr);
					if (ret != E_HM_OK) {
						++iterator_psk_mc->count;
						break;
					}
					dlist_delete(&iterator_psk_mc->node);
					free(iterator_psk_mc);
				}
				break;
			}
	}

	ux_packet_mclist_unlock(&psk->ml);
	return ret;
}

static int set_netdev_flags(ux_netdev_t *ndev, short int setflags, short int oldflags)
{
	if (setflags == oldflags) {
		return E_HM_OK;
	}
	return ux_netdev_set_flags(ndev->name, setflags);
}

static int setpacketsock_add_membership(ux_packet_sock_t *psk, const uintptr_t optval)
{
	struct packet_mreq mreq;
	short int oldflags;
	int ret = E_HM_INVAL;

	if (hm_actv_read_xact_vm(&mreq, (const void *)optval, sizeof(mreq)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if ((mreq.mr_alen + offsetof(struct packet_mreq, mr_address)) > sizeof(struct packet_mreq)) {
		return E_HM_INVAL;
	}

	ux_netdev_t *dev = ux_netdev_get_by_index(mreq.mr_ifindex);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (mreq.mr_type == PACKET_MR_PROMISC) {
		(void)ux_netdev_get_flags(dev->name, &oldflags);
		ret = set_netdev_flags(dev, oldflags | IFF_PROMISC, oldflags);
	} else if (mreq.mr_type == PACKET_MR_ALLMULTI) {
		(void)ux_netdev_get_flags(dev->name, &oldflags);
		ret = set_netdev_flags(dev, oldflags | IFF_ALLMULTI, oldflags);
	} else if (mreq.mr_type == PACKET_MR_MULTICAST) {
		ret = packet_add_mc_list(psk, dev, &mreq);
	}

	ux_netdev_put(dev);
	return ret;
}

static int setpacketsock_drop_membership(ux_packet_sock_t *psk, const uintptr_t optval)
{
	struct packet_mreq mreq;
	int ret = E_HM_INVAL;
	short int oldflags;

	ret = hm_actv_read_xact_vm(&mreq, (const void *)optval, sizeof(mreq));
	if (ret != E_HM_OK) {
		net_error(HMNET_PACKET, "read_xact_vm fail, ret:%d\n", ret);
		return E_HM_POSIX_FAULT;
	}

	ux_netdev_t *dev = ux_netdev_get_by_index(mreq.mr_ifindex);
	if (dev == NULL) {
		net_error(HMNET_PACKET, "no dev, ifindex:%d\n", (int)mreq.mr_ifindex);
		return E_HM_NODEV;
	}

	if (mreq.mr_type == PACKET_MR_PROMISC) {
		(void)ux_netdev_get_flags(dev->name, &oldflags);
		ret = set_netdev_flags(dev, oldflags & ~IFF_PROMISC, oldflags);
	} else if (mreq.mr_type == PACKET_MR_ALLMULTI) {
		(void)ux_netdev_get_flags(dev->name, &oldflags);
		ret = set_netdev_flags(dev, oldflags & ~IFF_ALLMULTI, oldflags);
	} else if (mreq.mr_type == PACKET_MR_MULTICAST) {
		ret = packet_del_mc_list(psk, dev, &mreq);
	}

	ux_netdev_put(dev);
	return ret;
}

static int setpacketsock_auxdata(ux_packet_sock_t *psk, const uintptr_t optval)
{
	int auxdata;

	if (hm_actv_read_xact_vm(&auxdata, (const void *)optval, sizeof(auxdata)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	psk->auxdata = (int)!!auxdata;
	return E_HM_OK;
}

struct setpacketopt_entry {
	int optname;
	socklen_t optlen;
	int (*func)(ux_packet_sock_t *psk, const uintptr_t optval);
};

static struct setpacketopt_entry setpskopt_array[] = {
	{
		.optname = PACKET_ADD_MEMBERSHIP,
		.optlen = sizeof(struct packet_mreq),
		.func = setpacketsock_add_membership,
	},
	{
		.optname = PACKET_DROP_MEMBERSHIP,
		.optlen = sizeof(struct packet_mreq),
		.func = setpacketsock_drop_membership,
	},
	{
		.optname = PACKET_AUXDATA,
		.optlen = sizeof(int),
		.func = setpacketsock_auxdata,
	},
};

static int ux_sol_packet_setsockopt(ux_socket_t *sock, int optname,
				const uintptr_t optval, socklen_t optlen)
{
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *psk = NULL;
	struct setpacketopt_entry *entry = NULL;
	int i;
	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		net_error(HMNET_PACKET, "sk is null\n");
		return E_HM_INVAL;
	}

	psk = pkt_sk(sk);
	for (i = 0; i < (int)ARRAY_SIZE(setpskopt_array); i++) {
		if (setpskopt_array[i].optname == optname)
			entry = &setpskopt_array[i];
	}

	if (entry == NULL) {
		return E_HM_NOPROTOOPT;
	}

	if (optlen < entry->optlen) {
		return E_HM_INVAL;
	}

	return entry->func(psk, optval);
}

static int ux_packet_getsockopt(ux_socket_t *sock, int level, int optname,
				void *optval, socklen_t *optlen)
{
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *psk = NULL;
	int ret = 0;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	psk = pkt_sk(sk);

	if (level == SOL_PACKET && optname == PACKET_STATISTICS) {
		ret = getsockopt_statistics(psk, optval, optlen);
	} else if (level == SOL_PACKET && optname == PACKET_AUXDATA) {
		ret = getsockopt_auxdata(psk, (uintptr_t)optval, optlen);
	} else if (level == SOL_SOCKET) {
		ret = ux_common_getsockopt(sock, level, optname, optval, optlen);
	} else {
		ret = E_HM_NOPROTOOPT;
	}

	ux_sock_put(sk);

	return ret;
}

static int ux_packet_setsockopt(ux_socket_t *sock, int level, int optname,
				const void *optval, socklen_t optlen)
{
	if (level == SOL_PACKET) {
		return ux_sol_packet_setsockopt(sock, optname, (const uintptr_t)optval, optlen);
	} else {
		return ux_common_setsockopt(sock, level, optname, optval, optlen);
	}
}

static int ux_packet_getsockevent(ux_socket_t *sock, unsigned int *rcvev, unsigned int *sendev,
				  unsigned int  *errev, int *error)
{
	ux_sock_t *sk = NULL;
	ux_packet_sock_t *psk = NULL;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	psk = pkt_sk(sk);

	if (rcvev != NULL) {
		*rcvev = (ux_buf_queue_len(&psk->psk_receive_queue) != 0) ? EPOLLIN : 0;
	}
	ux_sock_put(sk);

	if (sendev != NULL) {
		*sendev = EPOLLOUT;
	}
	if (errev != NULL) {
		*errev = 0;
	}
	if (error != NULL) {
		*error = 0;
	}
	return 0;
}

static int ux_packet_fcntl(ux_socket_t *sock, int cmd, int arg)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	unsigned int uarg = (unsigned int)arg;
	int ret = E_HM_INVAL;

	ux_sock_get(sk);
	switch (cmd) {
	case F_GETFL:
		ret = (int)sk->noblock;
		break;
	case F_SETFL:
		if ((uarg & ~(O_NONBLOCK)) == 0) {
			sk->noblock = uarg;
			ret = 0;
		} else {
			ret = E_HM_NOPROTOOPT;
		}
		break;
	default:
		break;
	}
	ux_sock_put(sk);

	return ret;
}

static ux_proto_ops_t packet_ops = {
	.close = ux_packet_close,
	.shutdown = NULL,
	.socketpair = NULL,
	.connect =NULL,
	.bind = ux_packet_bind,
	.listen = NULL,
	.accept = NULL,
	.fcntl = ux_packet_fcntl,
	.ioctl = ux_packet_ioctl,
	.send = ux_packet_send,
	.recv = ux_packet_recv,
	.sendmmsg = NULL,
	.recvmmsg = NULL,
	.sendmsg = ux_packet_sendmsg,
	.recvmsg = ux_packet_recvmsg,
	.sendto = ux_packet_sendto,
	.recvfrom = ux_packet_recvfrom,
	.writev = ux_packet_writev,
	.readv = ux_packet_readv,
	.getsockopt = ux_packet_getsockopt,
	.setsockopt = ux_packet_setsockopt,
	.getsockname = NULL,
	.getpeername = NULL,
	.getsockevent = ux_packet_getsockevent,
	.check_perm = NULL,
#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = NULL,
	.mmap = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static int ux_packet_create(ux_socket_t *sock, int domain, int type, int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);
	int ret;
	unsigned int flags = 0;
	ux_packet_sock_t *pktsk = NULL;

	net_debug(HMNET_PACKET, "%s %d\n", __func__, type);
	if (domain != AF_PACKET) {
		return E_HM_INVAL;
	}

	ret = sec_chk_div_raw_socket();
	if (ret != E_HM_OK) {
		return ret;
	}

	flags = (unsigned int)type & ~SOCK_TYPE_MASK;
	type = (int)((unsigned int)type & SOCK_TYPE_MASK);
	if (type != SOCK_RAW && type != SOCK_DGRAM) {
		return E_HM_SOCKTNOSUPPORT;
	}

	pktsk = ux_packet_sock_alloc_and_init(sock, domain, type, protocol);
	if (pktsk == NULL) {
		return E_HM_NOMEM;
	}
	sock->state = SS_UNCONNECTED;
	sock->ops = &packet_ops;
	sock->priv_ops = NULL;
	ux_socket_set_sock(sock, &pktsk->sk);
	pktsk->sk.socket_domain = domain;
	pktsk->sk.socket_protocol = protocol;
	pktsk->protocol_num = (uint16_t)protocol;
	pktsk->type = type;
	pktsk->prot_hook.func = dev_callbak_func;
	pktsk->prot_hook.af_packet_priv = ux_socket_get_sock(sock);

	(void)raw_rwlock_wrlock(&ux_packet_lock);
	dlist_insert(&ux_packet_list, &pktsk->head);
	(void)raw_rwlock_unlock(&ux_packet_lock);

	if (protocol != 0) {
		pktsk->prot_hook.type = (unsigned short)protocol;
		ux_sock_get(ux_socket_get_sock(sock));
		ux_dev_add_pt(&pktsk->prot_hook);
		pktsk->running = true;
	}

	if ((flags & SOCK_NONBLOCK) != 0) {
		(void)socket_fcntl(sock, F_SETFL, O_NONBLOCK);
	}

	return 0;
}

static ux_domain_t packet_domain = {
	.family = AF_PACKET,
	.create = ux_packet_create,
};

int af_packet_init(void)
{
	int ret;

	ret = ux_domain_register(&packet_domain);
	if (ret != E_HM_OK) {
		net_error(HMNET_PACKET, "register af_packet domain failed\n");
		return ret;
	}
	return 0;
}
