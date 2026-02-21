/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_NET_SOCK_H__
#define __LIBLINUX_NET_SOCK_H__

#define sk_page_frag __sk_page_frag
#define sock_rps_record_flow __sock_rps_record_flow
#include_next <net/sock.h>
#undef sk_page_frag
#undef sock_rps_record_flow

static inline struct page_frag *sk_page_frag(struct sock *sk)
{
	/*
	* The original implementation may return task_frag in current.
	* In hm, current is now simulated per activation.
	* So here we always use sk_frag.
	*/
	return &sk->sk_frag;
}

static inline void sock_rps_record_flow(const struct sock *sk)
{
#ifdef CONFIG_RPS
	if (!sk->sk_rxhash) {
		/*
		 * See rps_record_sock_flow :
		 * rps records CPU information, depending on
		 * whether sk_rxhash is valid.
		 */
		return;
	}

	return __sock_rps_record_flow(sk);
#endif
}

#define EPOLL_RECHECK	0x20000U
#define POLL_RECHECK	0x0800U
#define DEFAULT_RECHECK	(EPOLL_RECHECK | POLL_RECHECK)
static inline void sk_event_sync_to_hmkobj(struct sock *sk, unsigned int revents)
{
	struct file *filp = NULL;

	read_lock_bh(&sk->sk_callback_lock);
	if (!sock_flag(sk, SOCK_DEAD)) {
		filp = sk->sk_socket ? sk->sk_socket->file : NULL;
		if (filp) {
			/* liblinux_private: filp_node */
			liblinux_pal_sync_wakeup_trad(READ_ONCE(filp->liblinux_private), revents);
		}
	}
	read_unlock_bh(&sk->sk_callback_lock);
}

#endif	/* __LIBLINUX_NET_SOCK_H__ */
