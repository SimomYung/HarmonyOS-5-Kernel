/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Interface for tuntap socket
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 12 09:37:59 2021
 */

#include <stdlib.h>
#include <net/if_arp.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_rwlock.h>
#include <netinet/if_ether.h>

#include "ux_netlog.h"
#include "ux_domain.h"
#include "ux_sockopt.h"
#include "ux_eventpoll.h"
#include "security/net_admin.h"
#include "ux_tun.h"

#include "security/sec_hook.h"
#include "hm_if_tun.h"

#define TUN_READQ_SIZE		100
#define UX_NETDEV_MAX_NUM	128

static DLIST_HEAD(g_tun_list);
static unsigned int g_tdev_amount = 0;

static unsigned int g_tsk_amount = 0;
static struct raw_mutex g_tsk_amount_lock = RAW_MUTEX_INITIALIZER;

static struct raw_mutex g_tun_set_iff_lock = RAW_MUTEX_INITIALIZER;
static struct raw_mutex g_tun_xmit_lock = RAW_MUTEX_INITIALIZER;

static void ux_tun_epoll_wake(ux_epoll_context_t *epoll_ctx, uint32_t event)
{
	ux_epoll_wakeup_clean(epoll_ctx, event);
}

static inline ux_sock_t *sk_from_socket(ux_socket_t *socket)
{
	return ux_socket_get_sock(socket);
}

static void ux_tun_sock_free(ux_sock_t *sk)
{
	ux_tun_sock_t *tsk = tsk_from_sk(sk);
	ux_buf_t *buf = NULL;

	/* avoid net xmit when close */
	raw_mutex_lock(&g_tun_xmit_lock);
	/* avoid create dev whose name is the same when close */
	raw_mutex_lock(&g_tun_set_iff_lock);

	ux_buf_queue_lock(&tsk->read_queue);
	for (int i = (int)ux_buf_queue_len(&tsk->read_queue); i > 0; i--) {
		buf = (ux_buf_t *)ux_buf_queue_dqueue_first(&tsk->read_queue);
		if (buf != NULL) {
			free(buf);
			buf = NULL;
		}
	}

	if (tsk->attached == true) {
		tsk->attached = false;
		tsk->tdev->tsk = NULL;
		tsk->tdev->attached = false;
		ux_netdev_put(tsk->tdev->dev);
		if ((tsk->flags & TUN_PERSIST) == 0) {
			dlist_delete(&tsk->tdev->head);
			g_tdev_amount--;
			ux_netdev_unregister(tsk->tdev->dev);
		}
		tsk->tdev = NULL;
	}
	ux_buf_queue_unlock(&tsk->read_queue);
	free(tsk);
	tsk = NULL;
	raw_mutex_lock(&g_tsk_amount_lock);
	g_tsk_amount--;
	raw_mutex_unlock(&g_tsk_amount_lock);

	raw_mutex_unlock(&g_tun_set_iff_lock);
	raw_mutex_unlock(&g_tun_xmit_lock);
}

static int ux_tun_getsockevent(ux_socket_t *sock,
			       unsigned int *rcvev, unsigned int *sendev,
			       unsigned int *errev, int *error)
{
	int ret = E_HM_OK;
	ux_sock_t *sk = NULL;
	ux_tun_sock_t *tsk = NULL;

	sk = sk_from_socket(sock);
	if (sk == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		raw_mutex_lock(&sk->lock);
		tsk = tsk_from_sk(sk);
		if (tsk->attached == false) {
			raw_mutex_unlock(&sk->lock);
			ret = E_HM_BADFD;
		}
	}
	if (ret == E_HM_OK) {
		if (rcvev != NULL) {
			*rcvev = ux_buf_queue_len(&tsk->read_queue) == 0 ? 0 : EPOLLIN;
		}

		if (errev != NULL) {
			*errev = (sk->sk_err != 0) ? EPOLLERR : 0;
		}

		if (sendev != NULL) {
			*sendev = EPOLLOUT;
		}

		if (error != NULL) {
			*error = 0;
		}
		raw_mutex_unlock(&sk->lock);
	}

	return ret;
}

static int ux_tun_buf_alloc(ux_buf_t **ppbuf, const ux_tun_sock_t *tsk, int len)
{
	int ret = E_HM_OK;

	if (ux_buf_queue_len(&tsk->read_queue) >= TUN_READQ_SIZE) {
		ret = E_HM_NOSPC;
	}

	if (ret == E_HM_OK) {
		*ppbuf = (ux_buf_t *)calloc(1, (sizeof(ux_buf_t) + (size_t)len));
		if (*ppbuf == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	return ret;
}

static int tun_net_ndo_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	ux_tun_dev_t *tdev = NULL;
	ux_tun_sock_t *tsk = NULL;
	ux_buf_t *ux_buf = NULL;
	int ret = E_HM_OK;
	ux_copy_data_args_t args;

	tdev = (ux_tun_dev_t *)tdev_from_dev(dev);
	/*
	 * Fix potential deadlock during ux_tun_sock_free
	 * which may send out a TCP RST in lwip netif_remove.
	 * When sock_free, tdev->tsk will be set null,
	 * so we use variable "tdev->tsk" to identify whether sock_free run.
	 */
	if (tdev->tsk == NULL) {
		return ret;
	}

	raw_mutex_lock(&g_tun_xmit_lock);
	if (tdev->attached == false) {
		raw_mutex_unlock(&g_tun_xmit_lock);
		return ret;
	}
	tsk = tdev->tsk;
	ux_sock_get(&tsk->sk);

	ux_buf_queue_lock(&tsk->read_queue);
	ret = ux_tun_buf_alloc(&ux_buf, tsk, len);
	if (ret != E_HM_OK) {
		goto err_out;
	}
	args.dst = (void *)ux_buf->data;
	args.dstlen = (size_t)(unsigned int)len;
	args.src = data;
	args.count = (size_t)(unsigned int)len;
	args.offset = 0;
	args.flags = flags;
	ret = ux_netdev_copy_data(dev, &args);
	if (ret <= 0) {
		free(ux_buf);
		ux_buf = NULL;
		goto err_out;
	}
	ux_buf->data_len = (size_t)ret;
	ret = 0;
	ux_buf_queue_insert_tail(&tsk->read_queue, ux_buf);
	if (raw_sem_getvalue(&tsk->recvsem) < 0) {
		ux_sem_signal(&tsk->recvsem);
	}
	ux_tun_epoll_wake(&tsk->sk.epoll_ctx, EPOLLIN);

err_out:
	ux_buf_queue_unlock(&tsk->read_queue);
	ux_sock_put(&tsk->sk);
	raw_mutex_unlock(&g_tun_xmit_lock);
	return ret;
}

static ux_netdev_ops_t tun_net_ops = {
	.ndo_init = NULL,
	.ndo_xmit = tun_net_ndo_xmit,
	.ndo_set_mtu = NULL,
	.ndo_set_mac = NULL,
	.ndo_ioctl = NULL,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

static ux_tun_dev_t *tun_get_by_name(const char *name)
{
	ux_tun_dev_t *tdev = NULL;
	bool find = false;

	if (name != NULL) {
		dlist_for_each_entry(tdev, &g_tun_list, ux_tun_dev_t, head) {
			if (strcmp(name, tdev->dev->name) == 0) {
				ux_netdev_get(tdev->dev);
				find = true;
				break;
			}
		}
	}

	if (find == false) {
		tdev = NULL;
	}

	return tdev;
}

static void tun_dev_alloc_name(char *buf, int len, const char *name)
{
	ux_tun_dev_t *tdev = NULL;

	for (int i = 0; i < UX_NETDEV_MAX_NUM; i++) {
		if (snprintf_s(buf, (size_t)len, (size_t)(len - 1), name, i) < 0) {
			buf[0] = '\0';
			break;
		}
		tdev = tun_get_by_name(buf);
		if (tdev == NULL) {
			break;
		} else {
			ux_netdev_put(tdev->dev);
			tdev = NULL;
		}
	}
}

static ux_tun_dev_t *__tun_netdev_create(const char *name)
{
	ux_netdev_t* dev = NULL;
	ux_tun_dev_t *tdev = NULL;

	if (name != NULL && name[0] != '\0') {
		dev = ux_netdev_alloc(name, UX_NETDEV_PRIV_DEFAULT + sizeof(ux_tun_dev_t));
		if (dev != NULL) {
			tdev = (ux_tun_dev_t *)tdev_from_dev(dev);
			tdev->dev = dev;
		}
	}

	return tdev;
}

static int __tun_netdev_alloc(ux_tun_sock_t *tsk, const struct ifreq *ifr)
{
	int ret;
	char *name = NULL;
	char buf[IFNAMSIZ];
	ux_tun_dev_t *tdev = NULL;

	if (*ifr->ifr_name) {
		NOFAIL(strncpy_s(buf, IFNAMSIZ, ifr->ifr_name, IFNAMSIZ - 1));
	} else if ((((unsigned short)ifr->ifr_flags) & IFF_TUN) != 0) {
		name = "tun%d";
		tun_dev_alloc_name(buf, IFNAMSIZ, name);
	} else if ((((unsigned short)ifr->ifr_flags) & IFF_TAP) != 0) {
		name = "tap%d";
		tun_dev_alloc_name(buf, IFNAMSIZ, name);
	} else {
		return E_HM_INVAL;
	}

	tdev = __tun_netdev_create(buf);
	if (tdev == NULL) {
		return E_HM_NOMEM;
	}
	ux_netdev_get(tdev->dev);
	tdev->dev->ops = &tun_net_ops;
	if ((((unsigned short)ifr->ifr_flags) & IFF_TUN) != 0) {
		tdev->dev->type = ARPHRD_PPP;
	} else {
		tdev->dev->type = ARPHRD_ETHER;
	}
	tdev->dev->_mtu = ETHERNET_MTU;

	ret = ux_netdev_register(tdev->dev);
	if (ret != 0) {
		free(tdev->dev);
		tdev = NULL;
		return ret;
	}

	tdev->tsk = tsk;
	tdev->attached = true;
	dlist_init(&tdev->head);
	dlist_insert(&g_tun_list, &tdev->head);
	g_tdev_amount++;
	tsk->tdev = tdev;

	return ret;
}

static int __tun_set_iff(ux_tun_sock_t *tsk, struct ifreq *ifr)
{
	ux_tun_dev_t *tdev = NULL;
	int ret;

	tdev = tun_get_by_name(ifr->ifr_name);
	if (tdev != NULL) {
		unsigned short dev_type = (((unsigned short)ifr->ifr_flags) & IFF_TUN) != 0 ?
					  ARPHRD_PPP : ARPHRD_ETHER;
		if (tdev->attached == false &&
		    tdev->dev->type == dev_type) {
			tsk->tdev = tdev;
			tdev->tsk = tsk;
			tdev->attached = true;
			ret = 0;
		} else {
			ret = E_HM_BUSY;
		}
	} else if (g_tdev_amount >= UX_NETDEV_MAX_NUM) {
		ret = E_HM_MFILE;
	} else {
		ret = __tun_netdev_alloc(tsk, ifr);
	}

	if (ret == 0) {
		tsk->flags |= (((unsigned short)ifr->ifr_flags) & IFF_TUN) != 0 ? TUN_TUN_DEV : TUN_TAP_DEV;
		tsk->name = tsk->tdev->dev->name;
		tsk->attached = true;
		NOFAIL(strncpy_s(ifr->ifr_name, IFNAMSIZ, tsk->name, IFNAMSIZ - 1));
	}

	return ret;
}

static int tun_set_iff(ux_tun_sock_t *tsk, uintptr_t arg)
{
	struct ifreq ifr;
	int ret;

	ret = hm_actv_read_xact_vm(&ifr, (void *)arg, sizeof(struct ifreq));
	if (ret != 0) {
		return ret;
	}

	ifr.ifr_name[IFNAMSIZ - 1] = '\0';

	raw_mutex_lock(&g_tun_set_iff_lock);
	if (tsk->attached == true) {
		raw_mutex_unlock(&g_tun_set_iff_lock);
		return E_HM_INVAL;
	}
	ret = __tun_set_iff(tsk, &ifr);
	raw_mutex_unlock(&g_tun_set_iff_lock);

	if (ret == 0) {
		(void)hm_actv_write_xact_vm((void *)arg, &ifr, sizeof(struct ifreq));
	}

	return ret;
}

static int ux_tun_ioctl(ux_socket_t *socket, int cmd, uintptr_t arg)
{
	ux_sock_t *sk = NULL;
	ux_tun_sock_t *tsk = NULL;
	int ret = 0;

	sk = ux_socket_get_sock(socket);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	tsk = tsk_from_sk(sk);
	switch (cmd) {
	case TUNSETIFF:
		ret = tun_set_iff(tsk, arg);
		break;
	case TUNSETOWNER:
		tsk->owner = (int)arg;
		break;
	case TUNSETPERSIST:
		if ((bool)arg) {
			tsk->flags |= TUN_PERSIST;
		} else {
			tsk->flags &= ~TUN_PERSIST;
		}
		break;
	default:
		ret = E_HM_NOIOCTLCMD;
		break;
	}
	ux_sock_put(sk);

	return ret;
}

static ssize_t tun_send_handler(ux_socket_t *socket, const void *buf, size_t len, int flags)
{
	ux_sock_t *sk = NULL;
	struct ux_tun_sock *tsk = NULL;
	ux_netdev_t *dev = NULL;
	ssize_t ret = E_HM_OK;
	int mtu = ETHERNET_MTU;
	UNUSED(flags);

	if (len == 0) {
		return 0;
	}

	sk = ux_socket_get_sock(socket);
	if (sk == NULL || buf == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	tsk = tsk_from_sk(sk);
	if (tsk->attached != true) {
		ret = E_HM_BADFD;
	}

	if (ret == E_HM_OK) {
		dev = tsk->tdev->dev;
		ret = __ux_netdev_get_mtu(dev, &mtu);
	}

	if (ret == E_HM_OK) {
		if (len > (size_t)(unsigned int)mtu + ETH_HLEN) {
			ret = E_HM_MSGSIZE;
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_netdev_receive(dev, buf, (int)len, UXDEV_PKT_RAW);
	}

	ux_sock_put(sk);

	return ret != 0 ? ret : (ssize_t)len;
}

static ssize_t ux_tun_send(ux_socket_t *socket, const void *buf, size_t len, int flags)
{
	return socket_send_wrapper(socket, buf, len, flags, tun_send_handler);
}

static ssize_t tun_sendto_handler(ux_socket_t *socket, const void *buf, size_t len, int flags,
				  const struct sockaddr *daddr, socklen_t addrlen)
{
	UNUSED(daddr);
	UNUSED(addrlen);

	return tun_send_handler(socket, buf, len, flags);
}

static ssize_t ux_tun_sendto(ux_socket_t *socket, const void *buf, size_t len, int flags,
			     const struct sockaddr *daddr, socklen_t addrlen)
{
	return socket_sendto_wrapper(socket, buf, len, flags,
				     daddr, addrlen, tun_sendto_handler);
}

static ssize_t do_tun_recv(ux_tun_sock_t *tsk, void *buf, size_t len)
{
	ux_buf_t *uxbuf = NULL;
	size_t copylen;
	ssize_t ret;

	uxbuf = ux_buf_queue_dqueue_first(&tsk->read_queue);
	if (uxbuf == NULL) {
		ux_buf_queue_unlock(&tsk->read_queue);
		return E_HM_POSIX_NOENT;
	}

	if (ux_buf_queue_first(&tsk->read_queue) == NULL) {
		ux_tun_epoll_wake(&tsk->sk.epoll_ctx, EPOLLIN << CLEAN_EVENT_SHIFT);
	}
	ux_buf_queue_unlock(&tsk->read_queue);

	copylen = (len < uxbuf->data_len) ? len : uxbuf->data_len;

	ret = memcpy_s(buf, copylen, uxbuf->data, copylen);
	if (ret != 0) {
		ret = E_HM_INVAL;
	} else {
		ret = (ssize_t)copylen;
	}

	free(uxbuf);
	uxbuf = NULL;

	return ret;
}

static ssize_t ux_tun_recv(ux_socket_t *socket, void *buf, size_t len, int flags)
{
	ux_sock_t *sk = NULL;
	ux_tun_sock_t *tsk = NULL;
	ssize_t ret;

	if (len == 0) {
		return 0;
	}

	sk = ux_socket_get_sock(socket);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ux_sock_get(sk);
	tsk = tsk_from_sk(sk);
	if (tsk->attached == false) {
		ux_sock_put(sk);
		return E_HM_BADFD;
	}
	ux_buf_queue_lock(&tsk->read_queue);
	if (ux_buf_queue_len(&tsk->read_queue) == 0) {
		ux_buf_queue_unlock(&tsk->read_queue);
		if (((unsigned int)flags & MSG_DONTWAIT) != 0 ||
		    is_tun_noblock(sk)) {
			ux_sock_put(sk);
			return E_HM_AGAIN;
		}
		ret = ux_sem_cond_waiti(&(tsk->recvsem), LONG_MAX);
		if (ret != 0) {
			ux_sock_put(sk);
			return ret;
		}
		ux_buf_queue_lock(&tsk->read_queue);
	}
	ret = do_tun_recv(tsk, buf, len);
	ux_sock_put(sk);

	return ret;
}

static ssize_t tun_recvfrom_handler(ux_socket_t *socket, void *buf, size_t len, int flags,
			       struct sockaddr *saddr, socklen_t *addrlen)
{
	UNUSED(saddr);
	UNUSED(addrlen);

	return ux_tun_recv(socket, buf, len, flags);
}

static ssize_t ux_tun_recvfrom(ux_socket_t *socket, void *buf, size_t len, int flags,
			       struct sockaddr *saddr, socklen_t *addrlen)
{
	return socket_recvfrom_wrapper(socket, buf, len, flags, saddr, addrlen,
				       tun_recvfrom_handler);
}

static int ux_tun_fcntl(ux_socket_t *sock, int cmd, int arg)
{
	int ret = E_HM_OK;
	ux_sock_t *sk = NULL;
	unsigned int uarg = (unsigned int)arg;

	sk = sk_from_socket(sock);
	if (sk == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		raw_mutex_lock(&sk->lock);
		switch (cmd) {
		case F_GETFL:
			ret = (int)sk->noblock;
			break;
		case F_SETFL:
			/* mask O_LARGEFILE */
			uarg &= ~O_LARGEFILE;
			if ((uarg & ~(O_NONBLOCK | O_RDWR)) == 0) {
				/* only O_NONBLOCK|O_RDWR, all other bits are zero */
				sk->noblock = uarg;
			} else {
				ret = E_HM_INVAL;
			}
			break;
		default:
			ret = E_HM_INVAL;
			break;
		}
		raw_mutex_unlock(&sk->lock);
	}

	return ret;
}

static int ux_tun_close(ux_socket_t *sock)
{
	ux_sock_t *sk = sk_from_socket(sock);

	ux_clear_eventpoll_list(sock);
	ux_sock_put(sk);
	return 0;
}

static ux_proto_ops_t tsk_ops = {
	.close = ux_tun_close,
	.shutdown = NULL,
	.socketpair = NULL,
	.connect = NULL,
	.bind = NULL,
	.listen = NULL,
	.accept = NULL,
	.fcntl = ux_tun_fcntl,
	.ioctl = ux_tun_ioctl,
	.send = ux_tun_send,
	.recv = ux_tun_recv,
	.sendmmsg = NULL,
	.recvmmsg = NULL,
	.sendmsg = NULL,
	.recvmsg = NULL,
	.sendto = ux_tun_sendto,
	.recvfrom = ux_tun_recvfrom,
	.writev = NULL,
	.readv = NULL,
	.getsockopt = NULL,
	.setsockopt = NULL,
	.getsockname = NULL,
	.getpeername = NULL,
	.getsockevent = ux_tun_getsockevent,
	.check_perm = NULL,
#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = NULL,
	.mmap = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static ux_tun_sock_t *ux_tsk_alloc_and_init(ux_socket_t *socket, int domain, int type, int protocol)
{
	ux_tun_sock_t *tsk = NULL;

	tsk = tsk_from_sk(ux_sock_alloc(socket, sizeof(ux_tun_sock_t), 0));
	if (tsk != NULL) {
		ux_sock_init(&tsk->sk, NULL, domain, type, protocol);
		tsk->sk.release = ux_tun_sock_free;

		tsk->owner = -1;
		tsk->attached = false;
		tsk->epoll_info = NULL;
		tsk->flags = __UL(0);
		if (ux_sem_init(&tsk->recvsem, 0) != 0) {
			free(tsk);
			return NULL;
		}
		ux_buf_queue_init(&tsk->read_queue);
		tsk->tdev = NULL;
	}

	return tsk;
}

static int tun_socket_create(ux_socket_t *socket, int domain, int type, int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);

	ux_tun_sock_t *tsk = NULL;
	unsigned flags = (unsigned int)type & ~SOCK_TYPE_MASK;

	UNUSED(protocol);

	if (sec_chk_div_net_admin() != E_HM_OK) {
		return E_HM_PERM;
	}

	raw_mutex_lock(&g_tsk_amount_lock);
	if (g_tsk_amount == UX_NETDEV_MAX_NUM) {
		raw_mutex_unlock(&g_tsk_amount_lock);
		return E_HM_NOMEM;
	}

	tsk = ux_tsk_alloc_and_init(socket, domain, type, protocol);
	if (tsk == NULL) {
		raw_mutex_unlock(&g_tsk_amount_lock);
		return E_HM_NOMEM;
	}

	ux_socket_set_sock(socket, &tsk->sk);
	socket->ops = &tsk_ops;
	socket->priv_ops = NULL;

	g_tsk_amount++;
	raw_mutex_unlock(&g_tsk_amount_lock);

	if ((flags & SOCK_NONBLOCK) != 0) {
		(void)socket_fcntl(socket, F_SETFL, O_NONBLOCK);
	}

	return 0;
}

static ux_domain_t tun_domain = {
	.family = AF_TUN,
	.create = tun_socket_create,
};

int af_tun_init(void)
{
	int ret;

	ret = ux_domain_register(&tun_domain);
	if (ret != 0) {
		net_error(HMNET_TUN, "fail to register tun domain\n");
	}

	return ret;
}

void af_tun_uninit(void)
{
	ux_domain_unregister(tun_domain.family);
}
