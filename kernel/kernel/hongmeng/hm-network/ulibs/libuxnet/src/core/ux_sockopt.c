/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network socketopt
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/time.h>

#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_timer.h>

#include "ux_sockopt.h"
#include "bpf/ux_bpf_filter.h"
#include "ux_eventpoll.h"
#include "ux_compat.h"
#include "security/sec_hook.h"

#define tv_round_up(a, b) (((a) + (b) - 1) / (b))

static unsigned long long g_hmnetd_cookie = HMNETD_SOCKET_MIN_COOKIE_NUMBER;
static struct raw_mutex hmnetd_cookie_lock = RAW_MUTEX_INITIALIZER;

unsigned long long generate_hmnetd_cookie(unsigned long long cookie)
{
	unsigned long long sk_cookie = 0;

	if (cookie != 0) {
		return cookie;
	}

	raw_mutex_lock(&hmnetd_cookie_lock);
	g_hmnetd_cookie = (g_hmnetd_cookie == HMNETD_SOCKET_MAX_COOKIE_NUMBER) ?
			  HMNETD_SOCKET_MIN_COOKIE_NUMBER : (g_hmnetd_cookie + HMNETD_SOCKET_COOKIE_STEP);
	sk_cookie = g_hmnetd_cookie;
	raw_mutex_unlock(&hmnetd_cookie_lock);

	return sk_cookie;
}

static void sndrcvtimeo_set(long loc, struct timeval *tv)
{
	if (loc == LONG_MAX) {
		tv->tv_sec = 0;
		tv->tv_usec = 0;
		return;
	}
	tv->tv_sec = loc / TV_HZ;
	tv->tv_usec = (loc % TV_HZ) * TV_HZ;
}

static long sndrcvtimeo_get_ms(const struct timeval *tv)
{
	if (tv->tv_sec < 0) {
		return 0;
	}

	if (tv->tv_sec == 0 && tv->tv_usec == 0) {
		return LONG_MAX;
	}

	if (tv->tv_sec < (LONG_MAX / TV_HZ - 1)) {
		return (tv->tv_sec * TV_HZ + tv_round_up(tv->tv_usec, TV_HZ));
	}

	return LONG_MAX;
}

static unsigned long set_lingertime(const struct linger *ling)
{
#if __LONG_MAX <= 0x7fffffff
	if ((u32_t)ling->l_linger >= (u32_t)(LONG_MAX / TV_HZ)) {
		return LONG_MAX;
	}
#endif
	return ((unsigned long)(long)(ling->l_linger)) * TV_HZ;
}

static int getsockopt_reuseaddr(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &sk->reuse, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_error(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	int err;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	err = hmerrno2posix(sk->sk_err);
	if (hm_actv_write_xact_vm(optval, &err, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	sk->sk_err = 0;
	/* clean up the EPOLLERR event */
	ux_epoll_wakeup_clean(&sk->epoll_ctx, EPOLLERR << CLEAN_EVENT_SHIFT);

	return 0;
}

static int getsockopt_passcred(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &sk->passcred, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_peersec(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = NULL;
	int ret = 0;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_lock(sk);
	ret = sec_chk_div_socket_getpeersec_stream(sock, optval, optlen);
	ux_sock_unlock(sk);

	return ret;
}

static int getsockopt_domain(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &sk->socket_domain, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_sndbuf(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &sk->sndbuf, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_rcvbuf(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &sk->rcvbuf, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_sndtimeo(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	struct timeval tv = {0};
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	sndrcvtimeo_set(sk->sndtimeo, &tv);

	if (ux_is_compat_client()) {
		return ux_set_compat_timeval(&tv, optval);
	}

	if (hm_actv_write_xact_vm(optval, &tv, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_rcvtimeo(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	struct timeval tv = {0};
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	sndrcvtimeo_set(sk->rcvtimeo, &tv);

	if (ux_is_compat_client()) {
		return ux_set_compat_timeval(&tv, optval);
	}

	if (hm_actv_write_xact_vm(optval, &tv, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_protocol(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &sk->socket_protocol, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_cookie(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	if (optlen < sizeof(unsigned long long)) {
		return E_HM_INVAL;
	}

	/* Add lock here to avoid cookie get's the same when getsockopt run simultaneously */
	ux_socket_lock(sock);
	sock->cookie = generate_hmnetd_cookie(sock->cookie);
	ux_socket_unlock(sock);

	if (hm_actv_write_xact_vm(optval, &sock->cookie, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_mark(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	if (hm_actv_write_xact_vm(optval, &sock->mark, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_linger(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	struct linger ling = {0};
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ling.l_onoff = 0;
	if (ux_check_sock_flags(sk, SOCK_LINGER) != 0) {
		ling.l_onoff = 1;
	}

	ling.l_linger = (int)(((u32_t)(sk->lingertime)) / TV_HZ);

	if (hm_actv_write_xact_vm(optval, &ling, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}
	return 0;
}

static int getsockopt_priority(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	int val = 0;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, &val, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	/* no use, just return 0 */

	return 0;
}

static int getsockopt_keepalive(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	int keepalive = 0;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (optval == NULL || optlen == 0) {
		return E_HM_POSIX_FAULT;
	}

	if (ux_check_sock_flags(sk, SOCK_KEEPALIVE)) {
		keepalive = 1;
	} else {
		keepalive = 0;
	}

	if (hm_actv_write_xact_vm(optval, &keepalive, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int getsockopt_type(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	int type = 0;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (optlen != 0 && optval == NULL) {
		return E_HM_POSIX_FAULT;
	}

	type = sk->type;

	if (hm_actv_write_xact_vm(optval, &type, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

struct getsockopt_entry {
	int optname;
	socklen_t optlen;
	socklen_t optlen_compat;
	int (*func)(ux_socket_t *sock, void *optval, socklen_t optlen);
};

static struct getsockopt_entry getsockopt_array[] = {
	{
		.optname = SO_REUSEADDR,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_reuseaddr
	},
	{
		.optname = SO_ERROR,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_error
	},
	{
		.optname = SO_PASSCRED,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_passcred
	},
	{
		.optname = SO_PEERSEC,
		.optlen = UX_MAX_SOCKOPT_LEN,
		.optlen_compat = UX_MAX_SOCKOPT_LEN,
		.func = getsockopt_peersec
	},
	{
		.optname = SO_DOMAIN,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_domain
	},
	{
		.optname = SO_SNDBUF,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_sndbuf
	},
	{
		.optname = SO_RCVBUF,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_rcvbuf
	},
	{
		.optname = SO_SNDTIMEO_OLD,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = getsockopt_sndtimeo
	},
	{
		.optname = SO_RCVTIMEO_OLD,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = getsockopt_rcvtimeo
	},
	{
		.optname = SO_SNDTIMEO_NEW,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = getsockopt_sndtimeo
	},
	{
		.optname = SO_RCVTIMEO_NEW,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = getsockopt_rcvtimeo
	},
	{
		.optname = SO_PROTOCOL,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_protocol
	},
	{
		.optname = SO_MARK,
		.optlen = sizeof(unsigned int),
		.optlen_compat = sizeof(unsigned int),
		.func = getsockopt_mark
	},
	{
		.optname = SO_COOKIE,
		.optlen = sizeof(unsigned long long),
		.optlen_compat = sizeof(unsigned long long),
		.func = getsockopt_cookie
	},
	{
		.optname = SO_LINGER,
		.optlen = sizeof(struct linger),
		.optlen_compat = sizeof(struct linger),
		.func = getsockopt_linger
	},
	{
		.optname = SO_PRIORITY,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_priority
	},
	{
		.optname = SO_KEEPALIVE,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_keepalive
	},
	{
		.optname = SO_TYPE,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = getsockopt_type,
	}
};

int ux_common_getsockopt(ux_socket_t *sock, int level, int optname,
			 void *optval, socklen_t *optlen)
{
	struct getsockopt_entry *entry = NULL;
	socklen_t len = 0;
	int ret = 0;
	int i = 0;

	if (level != SOL_SOCKET) {
		return E_HM_OPNOTSUPP;
	}

	for (i = 0; i < (int)ARRAY_SIZE(getsockopt_array); i++) {
		if (getsockopt_array[i].optname == optname) {
			entry = &getsockopt_array[i];
		}
	}

	if (entry == NULL) {
		return E_HM_NOPROTOOPT;
	}

	if (hm_actv_read_xact_vm(&len, optlen, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (ux_is_compat_client()) {
		len = MIN(len, entry->optlen_compat);
	} else {
		len = MIN(len, entry->optlen);
	}

	ret = entry->func(sock, optval, len);
	if (ret != 0) {
		return ret;
	}

	if (hm_actv_write_xact_vm(optlen, &len, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int setsockopt_reuseaddr(ux_socket_t *sock, const void *optval)
{
	unsigned int val = 0;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&val, optval, sizeof(val)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	sk->reuse = val;

	return 0;
}

static int setsockopt_passcred(ux_socket_t *sock, const void *optval)
{
	int val = 0;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&val, optval, sizeof(val)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (val != 0) {
		val = 1;
	}
	sk->passcred = val;

	return 0;
}

static inline int sndbuf_max_limit(int val)
{
	return MIN(val, UX_SOCK_MAX_SNDBUF);
}

static inline int rcvbuf_max_limit(int val)
{
	return MIN(val, UX_SOCK_MAX_RCVBUF);
}

static inline int bufforce_max_limit(int val)
{
	return MAX(0, val);
}

static int __setsockopt_sndbuf(ux_socket_t *sock, const void *optval, int (*max_limit)(int))
{
	int val = 0;
	ux_sock_t *sk = NULL;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&val, optval, sizeof(val)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	val = max_limit(val);
	val = MIN(val, INT_MAX / SOCK_BUF_FIXUP) * SOCK_BUF_FIXUP;
	sk->sndbuf = MAX(val, UX_SOCK_MIN_SNDBUF);

	if (sk->set_write_space != NULL) {
		sk->set_write_space(sk, val);
	}

	return 0;
}

static int setsockopt_sndbuf(ux_socket_t *sock, const void *optval)
{
	return __setsockopt_sndbuf(sock, optval, sndbuf_max_limit);
}

static int setsockopt_sndbufforce(ux_socket_t *sock, const void *optval)
{
	return __setsockopt_sndbuf(sock, optval, bufforce_max_limit);
}

static int __setsockopt_rcvbuf(ux_socket_t *sock, const void *optval, int (*max_limit)(int))
{
	int val = 0;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&val, optval, sizeof(val)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	val = max_limit(val);
	val = MIN(val, INT_MAX / SOCK_BUF_FIXUP) * SOCK_BUF_FIXUP;
	sk->rcvbuf = MAX(val, UX_SOCK_MIN_RCVBUF);

	if (sk->set_read_space != NULL) {
		sk->set_read_space(sk, val);
	}

	return 0;
}

static int setsockopt_rcvbuf(ux_socket_t *sock, const void *optval)
{
	return __setsockopt_rcvbuf(sock, optval, rcvbuf_max_limit);
}

static int setsockopt_rcvbufforce(ux_socket_t *sock, const void *optval)
{
	return __setsockopt_rcvbuf(sock, optval, bufforce_max_limit);
}

#ifdef CONFIG_UXNET_BPF
static int setsockopt_attach_filter(ux_socket_t *sock, const void *optval)
{
	struct ux_sock_fprog fprog = {0};

	if (ux_is_compat_client()) {
		if (ux_get_compat_fprog(optval, &fprog) != 0) {
			return E_HM_POSIX_FAULT;
		}
	} else {
		if (hm_actv_read_xact_vm(&fprog, optval, sizeof(fprog)) != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}

	return ux_attach_filter(&fprog, ux_socket_get_sock(sock));
}

static int setsockopt_detach_filter(ux_socket_t *sock, const void *optval)
{
	(void)optval;
	ux_detach_filter(ux_socket_get_sock(sock));

	return 0;
}
#endif

static int setsockopt_sndtimeo(ux_socket_t *sock, const void *optval)
{
	struct timeval tv = {0};
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (ux_is_compat_client()) {
		if (ux_get_compat_timeval(optval, &tv) != 0) {
			return E_HM_POSIX_FAULT;
		}
	} else {
		if (hm_actv_read_xact_vm(&tv, optval, sizeof(tv)) != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}

	if (tv.tv_usec < 0 || tv.tv_usec >= USEC_PER_SEC) {
		return E_HM_DOM;
	}

	sk->sndtimeo = sndrcvtimeo_get_ms(&tv);

	return 0;
}

static int setsockopt_mark(ux_socket_t *sock, const void *optval)
{
	unsigned int mark = 0;

	if (hm_actv_read_xact_vm(&mark, optval, sizeof(unsigned int)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}
	sock->mark = mark;

	return 0;
}

static int setsockopt_rcvtimeo(ux_socket_t *sock, const void *optval)
{
	struct timeval tv = {0};
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (ux_is_compat_client()) {
		if (ux_get_compat_timeval(optval, &tv) != 0) {
			return E_HM_POSIX_FAULT;
		}
	} else {
		if (hm_actv_read_xact_vm(&tv, optval, sizeof(tv)) != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}

	if (tv.tv_usec < 0 || tv.tv_usec >= USEC_PER_SEC) {
		return E_HM_DOM;
	}

	sk->rcvtimeo = sndrcvtimeo_get_ms(&tv);

	return 0;
}

static int setsockopt_linger(ux_socket_t *sock, const void *optval)
{
	struct linger ling = {0};
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&ling, optval, sizeof(ling)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (ling.l_onoff == 0) {
		ux_clear_sock_flags(sk, SOCK_LINGER);
	} else {
		ux_set_sock_flags(sk, SOCK_LINGER);
		sk->lingertime = set_lingertime(&ling);
	}

	return 0;
}

static int setsockopt_priority(ux_socket_t *sock, const void *optval)
{
	UNUSED(optval);
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	/* no use, just return ok */

	return 0;
}

static int setsockopt_keepalive(ux_socket_t *sock, const void *optval)
{
	int keepalive = 0;

	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	if (optval == NULL) {
		return E_HM_POSIX_FAULT;
	}

	if (hm_actv_read_xact_vm(&keepalive, optval, sizeof(keepalive)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (keepalive == 0) {
		ux_clear_sock_flags(sk, SOCK_KEEPALIVE);
	} else {
		ux_set_sock_flags(sk, SOCK_KEEPALIVE);
	}

	return 0;
}

struct setsockopt_entry {
	int optname;
	socklen_t optlen;
	socklen_t optlen_compat;
	int (*func)(ux_socket_t *sock, const void *optval);
};

static struct setsockopt_entry setsockopt_array[] = {
	{
		.optname = SO_REUSEADDR,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_reuseaddr
	},
	{
		.optname = SO_PASSCRED,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_passcred
	},
	{
		.optname = SO_SNDBUF,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_sndbuf
	},
	{
		.optname = SO_SNDBUFFORCE,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_sndbufforce
	},
	{
		.optname = SO_RCVBUF,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_rcvbuf
	},
	{
		.optname = SO_RCVBUFFORCE,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_rcvbufforce
	},
	{
		.optname = SO_SNDTIMEO_OLD,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = setsockopt_sndtimeo
	},
	{
		.optname = SO_RCVTIMEO_OLD,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = setsockopt_rcvtimeo
	},
	{
		.optname = SO_SNDTIMEO_NEW,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = setsockopt_sndtimeo
	},
	{
		.optname = SO_RCVTIMEO_NEW,
		.optlen = sizeof(struct timeval),
		.optlen_compat = sizeof(struct timeval32),
		.func = setsockopt_rcvtimeo
	},
#ifdef CONFIG_UXNET_BPF
	{
		.optname = SO_ATTACH_FILTER,
		.optlen = sizeof(struct ux_sock_fprog),
		.optlen_compat = sizeof(struct ux_compat_sock_fprog),
		.func = setsockopt_attach_filter
	},
	{
		.optname = SO_DETACH_FILTER,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_detach_filter
	},
#endif
	{
		.optname = SO_MARK,
		.optlen = sizeof(unsigned int),
		.optlen_compat = sizeof(unsigned int),
		.func = setsockopt_mark
	},
	{
		.optname = SO_LINGER,
		.optlen = sizeof(struct linger),
		.optlen_compat = sizeof(struct linger),
		.func = setsockopt_linger
	},
	{
		.optname = SO_PRIORITY,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_priority
	},
	{
		.optname = SO_KEEPALIVE,
		.optlen = sizeof(int),
		.optlen_compat = sizeof(int),
		.func = setsockopt_keepalive
	}
};

int ux_common_setsockopt(ux_socket_t *sock, int level, int optname,
			 const void *optval, socklen_t optlen)
{
	struct setsockopt_entry *entry = NULL;
	int i = 0;

	if (level != SOL_SOCKET) {
		return E_HM_NOPROTOOPT;
	}

	for (i = 0; i < (int)ARRAY_SIZE(setsockopt_array); i++) {
		if (setsockopt_array[i].optname == optname) {
			entry = &setsockopt_array[i];
		}
	}

	if (entry == NULL) {
		return E_HM_NOPROTOOPT;
	}

	if (ux_is_compat_client()) {
		if (optlen < entry->optlen_compat) {
			return E_HM_INVAL;
		}
	} else if (optlen < entry->optlen) {
		return E_HM_INVAL;
	}

	return entry->func(sock, optval);
}
