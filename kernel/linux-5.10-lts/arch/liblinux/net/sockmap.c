// SPDX-License-Identifier: GPL-2.0-only
/*
 * sock map support
 *
 * Copyright(c) 2023 Huawei Technologies Co., Ltd
 */

#include <linux/kernel.h>
#include <linux/skmsg.h>
#include <linux/skmsg.h>
#include <linux/filter.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/util_macros.h>
#include <net/inet_common.h>
#include <net/udp.h>

#include <trace/hooks/liblinux.h>

/* Note: Do not conflict with enum sk_psock_state_bits */
#define SK_PSOCK_TCPMAP_DISABLE 6
#define SK_PSOCK_TCPMAP_CLOSE 7

static struct proto tcp_map_prot __read_mostly;
static DECLARE_BITMAP(tcp_map_ports, 65536);
static DECLARE_BITMAP(udp_map_ports, 65536);

static unsigned short tcp_allowed_ports[] = {
	31234,
};

static unsigned short udp_allowed_ports[] = {
	31238,
	34298,
};

static inline bool tcp_map_port_enabled(int port)
{
	return test_bit(port, tcp_map_ports);
}

static inline void tcp_map_port_set(int port)
{
	bitmap_set(tcp_map_ports, port, 1);
}

static inline bool udp_map_port_enabled(int port)
{
	return test_bit(port, udp_map_ports);
}

static inline void udp_map_port_set(int port)
{
	bitmap_set(udp_map_ports, port, 1);
}

struct sock_key {
	uint32_t sip4;
	uint32_t dip4;
	uint8_t family;
	uint16_t sport;
	uint16_t dport;
};

static inline void get_peer_sock_key(struct sock *sk, struct sock_key *key)
{
	key->dip4 = sk->sk_daddr;
	key->sip4 = sk->sk_rcv_saddr;
	key->dport = sk->sk_dport;
	key->sport = htons(sk->sk_num);
}

/* refer to tcp_bpf_push */
static int tcp_map_push(struct sock *sk, struct sk_msg *msg, u32 apply_bytes,
			int flags, bool uncharge)
{
	bool apply = apply_bytes;
	struct scatterlist *sge;
	struct page *page;
	int size, ret = 0;
	u32 off;

	while (1) {
		sge = sk_msg_elem(msg, msg->sg.start);
		size = (apply && apply_bytes < sge->length) ?
			apply_bytes : sge->length;
		off  = sge->offset;
		page = sg_page(sge);

		tcp_rate_check_app_limited(sk);
retry:
		if (rcu_dereference(inet_csk(sk)->icsk_ulp_data)) {
			flags |= MSG_SENDPAGE_NOPOLICY;
			ret = kernel_sendpage_locked(sk,
					page, off, size, flags);
		} else {
			ret = do_tcp_sendpages(sk, page, off, size, flags);
		}

		if (ret <= 0)
			return ret;
		if (apply)
			apply_bytes -= ret;
		msg->sg.size -= ret;
		sge->offset += ret;
		sge->length -= ret;
		if (uncharge)
			sk_mem_uncharge(sk, ret);
		if (ret != size) {
			size -= ret;
			off  += ret;
			goto retry;
		}
		if (!sge->length) {
			put_page(page);
			sk_msg_iter_next(msg, start);
			sg_init_table(sge, 1);
			if (msg->sg.start == msg->sg.end)
				break;
		}
		if (apply && !apply_bytes)
			break;
	}

	return 0;
}

static int tcp_map_send_verdict(struct sock *sk, struct sk_psock *psock,
		struct sk_msg *msg, int *copied, int flags)
{
	struct sock *sk_redir;
	u32 tosend, origsize, sent;
	int ret;

	if (!sk_psock_test_state(psock, SK_PSOCK_TCPMAP_DISABLE) && !psock->sk_redir) {
		struct sock_key peer_key;

		get_peer_sock_key(sk, &peer_key);
		psock->sk_redir = inet_lookup_established(sock_net(sk), &tcp_hashinfo,
			peer_key.sip4, peer_key.sport, peer_key.dip4, peer_key.dport, 0);
		if (unlikely(!psock->sk_redir)) {
			sk_psock_set_state(psock, SK_PSOCK_TCPMAP_DISABLE);
		} else if (psock->sk_redir->sk_state == TCP_NEW_SYN_RECV) {
			sk_psock_set_state(psock, SK_PSOCK_TCPMAP_DISABLE);
			reqsk_put(inet_reqsk(psock->sk_redir));
			psock->sk_redir = NULL;
		} else if (psock->sk_redir->sk_state != TCP_ESTABLISHED) {
			sk_psock_set_state(psock, SK_PSOCK_TCPMAP_DISABLE);
			sock_put(psock->sk_redir);
			psock->sk_redir = NULL;
		}
	}

	sk_redir = psock->sk_redir;
	tosend = msg->sg.size;

	if (!sk_redir) {
		ret = tcp_map_push(sk, msg, tosend, flags, true);
		if (unlikely(ret))
			*copied -= sk_msg_free(sk, msg);

		return ret;
	}
more_data:
	sk_msg_return(sk, msg, tosend);
	release_sock(sk);

	origsize = msg->sg.size;
	msg->flags |= BPF_F_INGRESS;
	ret = tcp_bpf_sendmsg_redir(sk_redir, msg, tosend, flags);
	sent = origsize - msg->sg.size;
	lock_sock(sk);
	if (unlikely(ret < 0)){
		int free = sk_msg_free_nocharge(sk, msg);
		*copied -= free;
	}
	if (likely(!ret)) {
		if (msg &&
		    msg->sg.data[msg->sg.start].page_link &&
		    msg->sg.data[msg->sg.start].length) {
			sk_mem_charge(sk, tosend - sent);
			tosend = msg->sg.size;
			goto more_data;
		}
	}
	return ret;
}

static int tcp_map_sendmsg(struct sock *sk, struct msghdr *msg, size_t size)
{
	struct sk_msg tmp, *msg_tx = NULL;
	int copied = 0, err = 0;
	struct sk_psock *psock;
	long timeo;
	int flags;

	/* Don't let internal flags through */
	flags = (msg->msg_flags & ~MSG_SENDPAGE_DECRYPTED);
	flags |= MSG_NO_SHARED_FRAGS;

	psock = sk_psock_get(sk);
	if (unlikely(!psock))
		return tcp_sendmsg(sk, msg, size);

	lock_sock(sk);
	timeo = sock_sndtimeo(sk, msg->msg_flags & MSG_DONTWAIT);
	while (msg_data_left(msg)) {
		u32 copy, osize;

		if (sk->sk_err) {
			err = -sk->sk_err;
			goto out_err;
		}

		copy = msg_data_left(msg);
		if (!sk_stream_memory_free(sk))
			goto wait_for_sndbuf;

		msg_tx = &tmp;
		sk_msg_init(msg_tx);

		osize = msg_tx->sg.size;
		err = sk_msg_alloc(sk, msg_tx, msg_tx->sg.size + copy, msg_tx->sg.end - 1);
		if (err) {
			if (err != -ENOSPC)
				goto wait_for_memory;
			copy = msg_tx->sg.size - osize;
		}

		err = sk_msg_memcopy_from_iter(sk, &msg->msg_iter, msg_tx,
					       copy);
		if (err < 0) {
			sk_msg_trim(sk, msg_tx, osize);
			goto out_err;
		}

		copied += copy;

		err = tcp_map_send_verdict(sk, psock, msg_tx, &copied, flags);
		if (unlikely(err < 0))
			goto out_err;
		continue;
wait_for_sndbuf:
		set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
wait_for_memory:
		err = sk_stream_wait_memory(sk, &timeo);
		if (err) {
			if (msg_tx)
				sk_msg_free(sk, msg_tx);
			goto out_err;
		}
	}
out_err:
	if (err < 0)
		err = sk_stream_error(sk, msg->msg_flags, err);
	release_sock(sk);
	sk_psock_put(sk, psock);
	return copied ? copied : err;
}

static int tcp_msg_wait_data(struct sock *sk, struct sk_psock *psock,
			     long timeo)
{
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
	int ret = 0;

	if (sk->sk_shutdown & RCV_SHUTDOWN)
		return 1;

	if (!timeo)
		return ret;

	add_wait_queue(sk_sleep(sk), &wait);
	sk_set_bit(SOCKWQ_ASYNC_WAITDATA, sk);
	ret = sk_wait_event(sk, &timeo,
			    !list_empty(&psock->ingress_msg) ||
			    !skb_queue_empty_lockless(&sk->sk_receive_queue), &wait);
	sk_clear_bit(SOCKWQ_ASYNC_WAITDATA, sk);
	remove_wait_queue(sk_sleep(sk), &wait);
	return ret;
}

static int tcp_map_recvmsg(struct sock *sk, struct msghdr *msg, size_t len,
			    int nonblock, int flags, int *addr_len)
{
	struct sk_psock *psock;
	int copied, ret;

	if (unlikely(flags & MSG_ERRQUEUE))
		return inet_recv_error(sk, msg, len, addr_len);

	if (!len)
		return 0;

	psock = sk_psock_get(sk);
	if (unlikely(!psock))
		return tcp_recvmsg(sk, msg, len, nonblock, flags, addr_len);

	if (!skb_queue_empty(&sk->sk_receive_queue) &&
		sk_psock_queue_empty(psock)) {
		sk_psock_put(sk, psock);
		return tcp_recvmsg(sk, msg, len, nonblock, flags, addr_len);
	}

	lock_sock(sk);
msg_bytes_ready:
	copied = __tcp_bpf_recvmsg(sk, psock, msg, len, flags);
	if(!copied) {
		long timeo;
		int data;

		timeo = sock_rcvtimeo(sk, flags & MSG_DONTWAIT);
		data = tcp_msg_wait_data(sk, psock, timeo);
		if (data < 0) {
			ret = data;
			goto unlock;
		}

		if (data) {
			if (!sk_psock_queue_empty(psock))
				goto msg_bytes_ready;
			release_sock(sk);
			sk_psock_put(sk, psock);
			return tcp_recvmsg(sk, msg, len, nonblock, flags, addr_len);
		}
		copied = -EAGAIN;
	}
	ret = copied;

unlock:
	release_sock(sk);
	sk_psock_put(sk, psock);
	return ret;
}

static void tcp_map_destroy(struct sock *sk)
{
	void (*saved_destroy)(struct sock *sk);
	struct sk_psock *psock;

	rcu_read_lock();
	psock = sk_psock_get(sk);
	if (unlikely(!psock)) {
		rcu_read_unlock();
		saved_destroy = READ_ONCE(sk->sk_prot)->destroy;
	} else {
		if (sk_psock_test_state(psock, SK_PSOCK_TCPMAP_CLOSE))
			saved_destroy = NULL;
		else
			saved_destroy = READ_ONCE(psock->sk_proto)->destroy;
		sk_psock_set_state(psock, SK_PSOCK_TCPMAP_CLOSE);
		rcu_read_unlock();
		sk_psock_put(sk, psock);
		/* free the psock */
		sk_psock_put(sk, psock);
	}

	if (saved_destroy)
		saved_destroy(sk);
}

static void tcp_map_close(struct sock *sk, long timeout)
{
	void (*saved_close)(struct sock *sk, long timeout);
	struct sk_psock *psock;

	lock_sock(sk);
	rcu_read_lock();
	psock = sk_psock_get(sk);
	if (unlikely(!psock)) {
		rcu_read_unlock();
		release_sock(sk);
		saved_close = READ_ONCE(sk->sk_prot)->close;
	} else {
		saved_close = psock->saved_close;
		rcu_read_unlock();
		release_sock(sk);
		cancel_work_sync(&psock->work);
		sk_psock_put(sk, psock);
		/* free the psock */
		sk_psock_put(sk, psock);
	}

	if (WARN_ON_ONCE(saved_close == tcp_map_close))
		return;
	saved_close(sk, timeout);
}

static bool tcp_map_stream_read(const struct sock *sk)
{
	struct sk_psock *psock;
	bool empty = true;

	rcu_read_lock();
	psock = sk_psock(sk);
	if (likely(psock))
		empty = list_empty(&psock->ingress_msg);
	rcu_read_unlock();
	return !empty;
}

static int sockmap_transfer_init_proto(struct sock *sk, struct sk_psock *psock)
{
	struct proto *prot;

	switch (sk->sk_type) {
	case SOCK_STREAM:
		prot = &tcp_map_prot;
		break;
	default:
		return -EINVAL;
	}

	sk_psock_update_proto(sk, psock, prot);
	return 0;
}

static int __init sockmap_transfer_init(void)
{
	int i;

	tcp_map_prot = tcp_prot;
	tcp_map_prot.sendmsg = tcp_map_sendmsg;
	tcp_map_prot.recvmsg = tcp_map_recvmsg;
	tcp_map_prot.destroy = tcp_map_destroy;
	tcp_map_prot.stream_memory_read = tcp_map_stream_read;
	tcp_map_prot.close = tcp_map_close;

	for (i = 0; i < ARRAY_SIZE(tcp_allowed_ports); i++)
		tcp_map_port_set(tcp_allowed_ports[i]);

	for (i = 0; i < ARRAY_SIZE(udp_allowed_ports); i++)
		udp_map_port_set(udp_allowed_ports[i]);

	return 0;
}
late_initcall(sockmap_transfer_init);

static inline bool tcp_sockmap_filter(struct sock *sk, int op)
{
	if ((op == BPF_SOCK_OPS_PASSIVE_ESTABLISHED_CB &&
		tcp_map_port_enabled(sk->sk_num)) ||
		(op == BPF_SOCK_OPS_ACTIVE_ESTABLISHED_CB &&
		tcp_map_port_enabled(ntohs(sk->sk_dport))))
		return true;

	return false;
}

static void hook_tcp_init_transfer(void *args, struct sock *sk, int op, struct sk_buff *skb)
{
	struct sk_psock *psock;
	int ret;

	if (!ipv4_is_loopback(sk->sk_daddr))
		return;

	if (!tcp_sockmap_filter(sk, op))
		return;

	psock = sk_psock_init(sk, 0);
	if (IS_ERR(psock))
		return;

	if (unlikely(tcp_map_prot.close != tcp_map_close))
		return;

	ret = sockmap_transfer_init_proto(sk, psock);
	if (ret < 0)
		sk_psock_put(sk, psock);
}
INIT_VENDOR_HOOK(ldk_rvh_tcp_init_transfer, hook_tcp_init_transfer);

static inline bool udp_sockmap_filter(struct flowi4 *fl4)
{
	if (udp_map_port_enabled(ntohs(fl4->fl4_sport)) ||
		udp_map_port_enabled(ntohs(fl4->fl4_dport)))
		return true;

	return false;
}

static void hook_udp_send_skb(void *args, struct sk_buff *skb, struct flowi4 *fl4, int *ret)
{
	struct sock *sk = skb->sk;
	struct inet_sock *inet = inet_sk(sk);
	struct sock *peer;
	u8 saved_ip_summed;

	*ret = -EINVAL;
	if (!ipv4_is_loopback(fl4->daddr))
		return;

	if (!udp_sockmap_filter(fl4))
		return;

	peer = __udp4_lib_lookup(sock_net(sk), fl4->saddr, inet->inet_sport,
			fl4->daddr, fl4->fl4_dport, 0, 0, &udp_table, NULL);
	if (!peer) {
		pr_debug("udp_send_skb_fastpath: can't find sk: 0x%x:0x%x -> 0x%x:0x%x\n",
			fl4->saddr, inet->inet_sport, fl4->daddr, fl4->fl4_dport);
		return;
	}

	if (!pskb_pull(skb, skb_network_header_len(skb) + sizeof(struct udphdr))) {
		pr_debug("udp_send_skb_fastpath: failed to psock_pull\n");
		return;
	}

	refcount_add(-skb->truesize, &sk->sk_wmem_alloc);
	saved_ip_summed = skb->ip_summed;
	skb->ip_summed = CHECKSUM_UNNECESSARY;

	if (__udp_enqueue_schedule_skb(peer, skb)) {
		pr_debug("udp_send_skb_fastpath: failed to send to sk: 0x%x:0x%x -> 0x%x:0x%x\n",
			fl4->saddr, inet->inet_sport, fl4->daddr, fl4->fl4_dport);
		refcount_add(skb->truesize, &sk->sk_wmem_alloc);
		skb->ip_summed = saved_ip_summed;
		skb_push(skb, skb_network_header_len(skb) + sizeof(struct udphdr));
		return;
	}

	*ret = 0;
}
INIT_VENDOR_HOOK(ldk_rvh_udp_send_skb, hook_udp_send_skb);
