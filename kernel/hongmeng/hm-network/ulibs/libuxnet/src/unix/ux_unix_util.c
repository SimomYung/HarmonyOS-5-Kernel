/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Util function and Operations of lists for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 12 10:21:00 2020
 */

#include <stdio.h>
#include <libsec/cred.h>
#include <lib/utils.h>
#include <sys/epoll.h>
#include <vfs.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

#include "osal.h"
#include "ux_unix_scm.h"
#include "ux_unix_util.h"

#include "security/sec_hook.h"
#include "ux_unix_stat.h"
#include "ux_unix_fastpath.h"
#include "ux_netlog.h"
#include "ux_eventpoll.h"
#include "ux_sockopt.h"
#include "ux_socket.h"
#include "ux_hash.h"
#include "osal.h"

struct ux_scm_aggr {
	const struct ucred *ucred;
	const ux_scm_cookie_t *scm;
};

static bool unix_dac_enabled;

/*
 * UNIX path management consists of two parts:
 *     bindtable: hash + rbt (resolve conflicts): responsible for abstract path
 *     bindlist: responsible for bsd path
 *     Guaranteed:
 *         bindtable + bindlist = all
 *         bindtable & bindlist = null
 *     Use ux_unix_bindlock + isbind state to ensure that bindtable insertion and bindlist insertion
 *     do not occur at the same time.
 */
static struct raw_rwlock ux_unix_bindlock = RAW_RDLOCK_INITIALIZER;
static struct easy_htable ux_unix_bindtable;
static DLIST_HEAD(ux_unix_bindlist);

static int autobind(ux_unix_sock_t *usk);

static inline size_t size_min(size_t a, size_t b)
{
	return min(a, b);
}

/* XXX: split to osal */
int osal_get_ucred(struct ucred *ucred)
{
	int rc;
	struct sec_chk_cred cred = SEC_CHK_CRED_RESET;
	struct sec_cred_basic *basic_cred;
	struct sec_cred_dac *dac_cred;

	rc = sec_chk_src_cred_init_requester_simple(&cred);
	if (rc != E_HM_OK) {
		net_debug(HMNET_UNIX, "%s: fail to init cred\n", __func__);
		return rc;
	}

	basic_cred = basic_cred_of_sec_cred(&cred);
	ucred->pid = basic_cred->pid;
	if (unix_dac_enabled) {
		dac_cred = dac_cred_of_sec_cred(&cred);

		ucred->uid = dac_cred->euid;
		ucred->gid = dac_cred->egid;
	} else {
		ucred->uid = 0;
		ucred->gid = 0;
	}

	return rc;
}

void set_unix_dac_enabled(bool is_enabled)
{
	unix_dac_enabled = is_enabled;
}

/* copy path to addr->sun_path */
void ux_unix_make_addr(const char *path, unsigned char len, struct sockaddr_un *addr,
		       socklen_t *addr_len)
{
	socklen_t tmp_len = (socklen_t)len;
	socklen_t sunpath_len = 0;

	sunpath_len = *addr_len > (unsigned int)offset_of(struct sockaddr_un, sun_path) ?
		      *addr_len - (unsigned int)offset_of(struct sockaddr_un, sun_path) : 0;
	addr->sun_family = AF_UNIX;

	sunpath_len = sunpath_len <= USK_PATH_USER_MAX ? sunpath_len : USK_PATH_USER_MAX;
	if (tmp_len > 0 && sunpath_len > 0) {
		tmp_len = tmp_len > sunpath_len ? sunpath_len : tmp_len;
		NOFAIL(memset_s(addr->sun_path, sunpath_len, 0, sunpath_len));
		if (memcpy_s((char *)addr->sun_path, sunpath_len, (void *)path, tmp_len) != 0) {
			net_info(HMNET_UNIX, "copy path failed\n");
		}
	}

	if (path[0] != '\0') {
		/*
		 * tmp_len before conversion: path string size(not include '\0')
		 * bsd linux guarante that include a \0 if space enough in unix_mkname
		 */
		tmp_len = tmp_len == 0 ? 0 : tmp_len + 1;
	}
	tmp_len = tmp_len  > sizeof(addr->sun_path) ? sizeof(addr->sun_path) : tmp_len;
	*addr_len = offsetof(struct sockaddr_un, sun_path[0]) + tmp_len;
}

static void clean_epollin(ux_unix_sock_t *usk)
{
	/*
	 * In linux, we can always find EPOLLIN event after unix socket has been shutdown rcv.
	 * So, don't clean EPOLLIN if RCV_SHUTDOWN was set.
	 */
	if ((usk->shutdown & RCV_SHUTDOWN) != 0U) {
		return;
	}

	if (ux_buf_len(&usk->recv_queue) == 0) {
		ux_sock_poll_common(&usk->sk, EPOLLIN << CLEAN_EVENT_SHIFT);
	}
}

static long unix_cmp_key(void *cur_data, void *key)
{
	long rc;
	ux_unix_sock_t *data = (ux_unix_sock_t *)cur_data;
	struct unix_path_key *k = (struct unix_path_key *)key;

	if (data->pathlen != k->pathlen) {
		rc = data->pathlen - k->pathlen;
	} else if (ux_sock_ns(&data->sk) != k->ns) {
		rc = ux_sock_ns(&data->sk) - k->ns;
	} else {
		rc = memcmp(data->path, k->path, k->pathlen);
	}

	return rc;
}

#define HASH_CHAR_FACTOR 5
#define BIND_BUCKET_MAX 108
#define BIND_TABLE_MAX 65536

static unsigned int unix_calc_hval(void *key)
{
	unsigned int i;
	unsigned int hash = 0;
	unsigned char uchar;
	struct unix_path_key *k = (struct unix_path_key *)key;

	for (i = 0; i < k->pathlen; i++) {
		uchar = (unsigned char)k->path[i];
		hash = ((hash << HASH_CHAR_FACTOR) - hash) + (unsigned int)uchar;
	}

	return hash;
}

int bindtable_init(void)
{
	int ret;

	ux_htable_register(H_UNIX_PATH, unix_cmp_key, unix_calc_hval);
	ret = easy_htable_init(&ux_unix_bindtable, malloc, BIND_BUCKET_MAX, BIND_TABLE_MAX,
				offset_of(struct ux_unix_sock, path),
				offset_of(struct ux_unix_sock, u.bindrnode));
	if (ret != E_HM_OK) {
		hm_error("Fail to init unix bind hash table!\n");
	}

	return ret;
}

static void delete_bindnode(ux_unix_sock_t *usk)
{
	struct unix_path_key key = {usk->path, 0, usk->pathlen}; /* only find */

	(void)raw_rwlock_wrlock(&ux_unix_bindlock);
	if (if_unix_sock_bind_abstract(usk)) {
		(void)ux_htable_remove(&ux_unix_bindtable, H_UNIX_PATH, &key, &usk->u.bindrnode);
	} else {
		dlist_delete(&usk->u.bindlnode);
	}
	(void)raw_rwlock_unlock(&ux_unix_bindlock);

	ux_sock_put(&usk->sk);
}

void ux_unix_get_peercred(ux_unix_sock_t *usk, struct ucred *optval)
{
	ux_unix_sock_t *peer = NULL;

	ux_unix_sock_lock(usk);
	peer = usk_peer(usk);
	if (peer == NULL) {
		optval->pid = 0;
		optval->uid = (uid_t)(-1);
		optval->gid = (gid_t)(-1);
	} else {
		*optval = peer->cred;

		optval->pid = osal_hm_try_convert_pid_to_current_pidns(peer->root_ns_pid);
	}
	ux_unix_sock_unlock(usk);
}

void ux_unix_getpeerinfo(ux_unix_sock_t *usk, int *peer_pid, unsigned long long *peer_cookie)
{
	ux_unix_sock_t *peer = NULL;

	ux_unix_sock_lock(usk);
	peer = usk_peer(usk);
	if (peer == NULL) {
		ux_unix_sock_unlock(usk);
		return;
	}
	ux_sock_get(&peer->sk);
	ux_unix_sock_unlock(usk);

	ux_unix_sock_lock(peer);
	if (peer != NULL && peer->sk.socket != NULL) {
		*peer_cookie = ux_socket_ino(peer->sk.socket);
		*peer_pid = peer->cred.pid;
	}

	ux_unix_sock_unlock(peer);
	ux_sock_put(&peer->sk);
}

int getsockopt_peercred(ux_unix_sock_t *usk, void *optval, socklen_t *optlen)
{
	struct ucred ucred = {0};
	socklen_t len = 0;

	if (hm_actv_read_xact_vm(&len, optlen, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	ux_unix_get_peercred(usk, &ucred);

	if (len > sizeof(struct ucred)) {
		len = sizeof(struct ucred);
	}

	if (hm_actv_write_xact_vm(optlen, &len, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (hm_actv_write_xact_vm(optval, &ucred, len) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static void af_unix_set_write_space(ux_sock_t *sk, int sndbuf)
{
	ux_unix_sock_t *usk = unix_sk(sk);
	ux_unix_sock_t *pusk = NULL;

	UNUSED(sndbuf);

	ux_unix_sock_lock(usk);
	pusk = usk_peer(usk);
	if (pusk != NULL) {
		ux_wq_wakeup(&pusk->sendwq);
	}
	ux_unix_sock_unlock(usk);
}

ux_unix_sock_t *ux_unix_sock_alloc_and_init(ux_socket_t *sock, int type, int protocol,
				   struct ucred *cred, ux_namespace_t *ns)
{
	ux_unix_sock_t *usk = NULL;

	usk = unix_sk(ux_sock_alloc(sock, sizeof(ux_unix_sock_t), SHARE_POLLLIST));
	if (usk == NULL) {
		return NULL;
	}

	NOFAIL(memcpy_s(&usk->cred, sizeof(usk->cred), cred, sizeof(*cred)));
	usk->state = SS_UNCONNECTED;
	usk->type = type;
	usk->root_ns_pid = -1;
	usk->rbuffer = NULL;

	ux_wq_head_init(&usk->sendwq);
	ux_wq_head_init(&usk->recvwq);
	ux_wq_head_init(&usk->acceptwq);
	ux_wq_head_init(&usk->connwq);

	ux_sock_init(&usk->sk, ns, AF_UNIX, type, protocol);
	usk->sk.release = ux_unix_sock_free;
	usk->sk.set_write_space = af_unix_set_write_space;
	dlist_init(&usk->head);
	dlist_init(&usk->u.bindlnode);
	(void)raw_atomic_int_set(&usk->send_buffer_used, 0);

	usk->queued = 0;
	usk->shutdown = 0;
	usk->pathlen = 0;
	usk->fp_type = FP_INVAL;
	dlist_init(&usk->queue);
	dlist_init(&usk->queuenode);
	raw_mutex_init(&usk->queuelock);
	ux_buf_queue_init(&usk->recv_queue);

	return usk;
}

void ux_unix_clear_queue(ux_unix_sock_t *usk, const ux_unix_sock_t *except)
{
	ux_unix_sock_t *link = NULL;
	ux_unix_sock_t *tmp = NULL;

	raw_mutex_lock(&usk->queuelock);
	if (dlist_empty(&usk->queue) || usk->queued == 0) {
		raw_mutex_unlock(&usk->queuelock);
		return;
	}

	dlist_for_each_entry_safe(link, tmp, &usk->queue, ux_unix_sock_t, queuenode) {
		if (link == except) {
			continue;
		}

		dlist_delete(&link->queuenode);
		ux_sock_put(&link->sk);
		usk->queued--;

		ux_sock_put(&link->peer->sk);
		link->peer = NULL;

		if (link->peer != NULL) {
			ux_sock_put(&link->peer->sk);
			link->peer = NULL;
		}
		/* free conn, del the ref of create */
		ux_sock_put(&link->sk);

		if (usk->type == SOCK_STREAM && usk->queued > NR_USK) {
			net_error(HMNET_UNIX, "usk->queued overflow\n");
			break;
		}
	}
	raw_mutex_unlock(&usk->queuelock);
}

unsigned int epoll_get_shutdown_events(unsigned int uhow)
{
	if (((uhow & SEND_SHUTDOWN) != 0U) && ((uhow & RCV_SHUTDOWN) != 0U)) {
		return EPOLLIN | EPOLLHUP;
	}
	if ((uhow & RCV_SHUTDOWN) != 0) {
		return EPOLLIN;
	}
	return 0;
}

void notify_peer(ux_unix_sock_t *usk, ux_unix_sock_t *conn, unsigned int uhow, bool is_close)
{
	uint32_t event = epoll_get_shutdown_events(uhow);

	if (is_close) {
		event |= EPOLLIN | EPOLLHUP;
	}
	if (conn != NULL) {
		if (ux_buf_len(&usk->recv_queue) > 0 && is_close) {
			conn->sk.sk_err = E_HM_CONNRESET;
			event |= EPOLLERR;
		}
		ux_wq_wakeup(&conn->recvwq);

		ux_sock_poll_common(&conn->sk, event);
	}
}

void usk_free(ux_unix_sock_t *usk)
{
	int i = 0;
	ux_buf_t *tmpbuf = NULL;

	if (usk == NULL) {
		net_error(HMNET_UNIX, "usk is NULL when free\n");
		return;
	}

	if (usk->isbind) {
		net_error(HMNET_UNIX, "usk is bind when free\n");
	}

	for (i = (int)ux_buf_queue_len(&usk->recv_queue); i > 0; i--) {
		tmpbuf = (ux_buf_t *)ux_buf_queue_dqueue_first(&usk->recv_queue);
		if (tmpbuf != NULL) {
			ux_unix_buf_t *tmp_unix_buf = container_of(tmpbuf, ux_unix_buf_t, ux_buf);
			ux_unix_buff_put(tmp_unix_buf);
		}
	}

	ux_unix_release_root_ns_pid(usk);

	free(usk);
}

void ux_unix_sock_free(ux_sock_t *sk)
{
	ux_unix_sock_t *usk = unix_sk(sk);

	usk_free(usk);
}

/* The minimum sndbuf value that can be set must be greater than the size of ux_unix_buf_t. */
raw_static_assert(UX_SOCK_MIN_SNDBUF > sizeof(ux_unix_buf_t), sndbuf_size_is_too_small);

int unix_get_pusk(ux_unix_sock_t *usk, const struct netd_msghdr *msg, ux_unix_sock_t **peerp)
{
	ux_unix_sock_t *pusk = NULL;
	int ret = 0;

	if (msg != NULL && msg->msg_namelen > 0) {
		ret = find_peer_by_addr(msg->msg_name, msg->msg_namelen, ux_sock_ns(&usk->sk), &pusk);
		if (ret < 0) {
			return ret;
		}

		if (pusk->type != usk->type) {
			ux_sock_put(&pusk->sk);
			return E_HM_NOTCONN;
		}

		if ((pusk->peer != NULL) && pusk->peer != usk) {
			ux_sock_put(&pusk->sk);
			return E_HM_PERM;
		}
	} else {
		ux_unix_sock_lock(usk);
		pusk = usk_peer(usk);
		if (pusk != NULL) {
			ux_sock_get(&pusk->sk);
		}
		ux_unix_sock_unlock(usk);

		if (pusk == NULL) {
			return E_HM_NOTCONN;
		}
	}

	*peerp = pusk;

	return E_HM_OK;
}

static ux_unix_buf_t *__try_alloc_unix_buff(ux_unix_sock_t *usk, size_t size, bool nonblock, int *err)
{
	ux_unix_buf_t *buf = NULL;
	int rc = E_HM_OK;

	ux_unix_sock_lock(usk);
	while (ux_unix_send_buffer_used(usk) >= usk->sk.sndbuf) {
		struct ux_wq_entry waiter;

		if (usk->sk.sk_err != 0) {
			rc = usk->sk.sk_err;
			break;
		}

		if (ux_is_wr_shutdown(usk)) {
			rc = E_HM_PIPE;
			break;
		}

		if (sk_is_noblock(&usk->sk) || nonblock) {
			rc = E_HM_AGAIN;
			break;
		}

		ux_wq_prepare_wait(&usk->sendwq, &waiter);
		ux_unix_sock_unlock(usk);

		if (ux_unix_send_buffer_used(usk) >= usk->sk.sndbuf) {
			rc = ux_wq_do_wait(&waiter, usk->sk.sndtimeo);
		}

		ux_wq_finish_wait(&usk->sendwq, &waiter);
		ux_unix_sock_lock(usk);

		if (rc != E_HM_OK) {
			break;
		}
	}

	if (rc == E_HM_OK) {
		buf = ux_unix_alloc_buffer(usk, size);
		if (buf == NULL) {
			rc = E_HM_NOMEM;
		}
	}

	ux_unix_sock_unlock(usk);

	if (err != NULL) {
		*err = rc;
	}

	return buf;
}

static int __try_send_unix_buff_stream(ux_unix_sock_t *peer, ux_unix_buf_t *buf,
				       struct ux_scm_aggr *aggr)
{
	int ret = E_HM_OK;

	ux_unix_sock_lock(peer);

	if (ux_is_rd_shutdown(peer) || ux_check_sock_flags(&peer->sk, SOCK_DEAD)) {
		ret = E_HM_PIPE;
	} else {
#ifdef CONFIG_SCM_RIGHTS
		scm_send(aggr->scm, aggr->ucred, buf);
#endif
		ux_buf_queue_insert_tail(&peer->recv_queue, &buf->ux_buf);
		ux_sock_poll_common(&peer->sk, EPOLLIN);
	}
	ux_unix_sock_unlock(peer);

	ux_wq_wakeup(&peer->recvwq);
	return ret;
}

static ssize_t __unix_sendmsg_stream(ux_unix_sock_t *usk, ux_unix_sock_t *pusk,
				     struct netd_msghdr *msg, int flags, ux_scm_cookie_t *scm)
{
	struct ucred ucred = {0};
	struct ux_scm_aggr scm_aggr = {0};
	ssize_t sent = 0;
	ssize_t copied = 0;
	size_t len, max_data_len, size;
	int ret;

	ret = osal_get_ucred(&ucred);
	if (ret != E_HM_OK) {
		net_error(HMNET_UNIX, "%s: failed to get current process's ucred\n", __func__);
		return ret;
	}

	scm_aggr.ucred = &ucred;
	scm_aggr.scm = scm;

	len = iov_iter_left(&msg->msg_iter);
	max_data_len = (size_t)(unsigned int)(usk->sk.sndbuf / SOCK_BUF_FIXUP) - 64;    /* 64 is some extra */

	while (len > (size_t)sent) {
		ux_unix_buf_t *buff = NULL;

		size = size_min(len - (size_t)sent, max_data_len);
		buff = __try_alloc_unix_buff(usk, size, (unsigned int)flags & MSG_DONTWAIT, &ret);
		if (ret == E_HM_OK && buff != NULL) {
			copied = ux_copy_data_from_netd_msghdr(msg, buff->ux_buf.data, size);
			if (copied < 0) {
				ret = (int)copied;
				net_warn(HMNET_UNIX, "copy netd msghdr failed ret = %s\n", hmstrerror(ret));
			} else {
				buff->ux_buf.data_len = (unsigned int)copied;
			}
		}

		if (ret == E_HM_OK) {
			ret = __try_send_unix_buff_stream(pusk, buff, &scm_aggr);
		}

		if (ret != E_HM_OK) {
			if (buff != NULL) {
				ux_unix_buff_put(buff);
			}
			break;
		}

		scm_aggr.scm = NULL;    /* scm can only be send to one unix buffer */
		sent += copied;
	}

	return (sent > 0) ? sent : ret;
}

static ssize_t unix_sendmsg_stream(ux_unix_sock_t *usk, struct netd_msghdr *msg, int flags)
{
	ux_unix_sock_t *peer = NULL;
	ux_scm_cookie_t scm = {0};
	ssize_t sent = 0;
	int ret;

	ret = unix_scm_send((struct msghdr *)msg, &scm);
	if (ret != E_HM_OK) {
		net_warn(HMNET_UNIX, "unix scm send failed ret = %s\n", hmstrerror(ret));
		return ret;
	}

	if ((flags & MSG_OOB) != 0) {
		ret = E_HM_OPNOTSUPP;
	} else {
		if (msg->msg_namelen != 0) {
			ret = is_connect_established(usk) ? E_HM_ISCONN : E_HM_OPNOTSUPP;
		} else {
			peer = usk_peer(usk);
			if (peer == NULL) {
				ret = E_HM_NOTCONN;
			}
		}
	}

	if (ret == E_HM_OK) {
		if (ux_is_wr_shutdown(usk)) {
			ret = E_HM_PIPE;
		} else {
			sent = __unix_sendmsg_stream(usk, peer, msg, flags, &scm);
			if (sent < 0) {
				ret = (int)sent;
			}
		}
	}

	if (ret != E_HM_OK) {
		unix_scm_destroy(&scm);
	}

	return sent != 0 ? sent : ret;
}

static int __try_send_unix_buff_dgram(ux_unix_sock_t *usk, ux_unix_sock_t *peer,
				      ux_unix_buf_t *buf, struct ux_scm_aggr *aggr,
				      bool nonblock)
{
	bool need_check_backlog;
	int ret = E_HM_OK;

	ux_unix_sock_lock(peer);
	if (ux_check_sock_flags(&peer->sk, SOCK_DEAD) && (usk_peer(usk) == peer)) {
		ret = E_HM_CONNREFUSED;
	} else if (ux_is_rd_shutdown(peer)) {
		ret = E_HM_PIPE;
	}

	need_check_backlog = ((usk != peer) && (usk_peer(peer) != usk));
	while ((ret == E_HM_OK) && need_check_backlog && (ux_buf_queue_len(&peer->recv_queue) > peer->backlog)) {
		struct ux_wq_entry waiter;

		if (peer->sk.sk_err != 0) {
			ret = peer->sk.sk_err;
			break;
		}

		if (ux_is_rd_shutdown(peer) || ux_check_sock_flags(&peer->sk, SOCK_DEAD)) {
			ret = E_HM_PIPE;
			break;
		}

		if (sk_is_noblock(&usk->sk) || nonblock) {
			ret = E_HM_AGAIN;
			break;
		}

		ux_wq_prepare_wait(&peer->recvwq, &waiter);
		ux_unix_sock_unlock(peer);

		if (ux_buf_queue_len(&peer->recv_queue) > peer->backlog) {
			ret = ux_wq_do_wait(&waiter, usk->sk.sndtimeo);
		}

		ux_wq_finish_wait(&peer->recvwq, &waiter);
		ux_unix_sock_lock(peer);
	}

	if (ret == E_HM_OK) {
#ifdef CONFIG_SCM_RIGHTS
		scm_send(aggr->scm, aggr->ucred, buf);
#endif
		ux_buf_queue_insert_tail(&peer->recv_queue, &buf->ux_buf);
		ux_sock_poll_common(&peer->sk, EPOLLIN);
	}
	ux_unix_sock_unlock(peer);

	ux_wq_wakeup(&peer->recvwq);
	return ret;
}

static ssize_t __unix_sendmsg_dgram(ux_unix_sock_t *usk, ux_unix_sock_t *pusk,
				    struct netd_msghdr *msg, int flags, ux_scm_cookie_t *scm)
{
	bool nonblock = (((unsigned int)flags & MSG_DONTWAIT) != 0);
	ux_unix_buf_t *buff = NULL;
	struct ucred ucred = {0};
	struct ux_scm_aggr scm_aggr = {0};
	size_t size;
	ssize_t copied = 0;
	int ret;

	ret = osal_get_ucred(&ucred);
	if (ret != E_HM_OK) {
		net_error(HMNET_UNIX, "failed to get current process's ucred\n");
		return ret;
	}

	if (usk->type != SOCK_SEQPACKET) {
		ret = sec_hook_div_unix_may_send(&usk->sk, &pusk->sk);
		if (ret != E_HM_OK) {
			net_error(HMNET_UNIX, "seharmony dgram send check failed ret = %s\n", hmstrerror(ret));
			return ret;
		}
	}

	scm_aggr.ucred = &ucred;
	scm_aggr.scm = scm;

	size = iov_iter_left(&msg->msg_iter);
	if (size > (size_t)(unsigned int)usk->sk.sndbuf - 32) {    /* 32 is some extra */
		return E_HM_MSGSIZE;
	}

	buff = __try_alloc_unix_buff(usk, size, nonblock, &ret);
	if (ret == E_HM_OK && buff != NULL) {
		copied = ux_copy_data_from_netd_msghdr(msg, buff->ux_buf.data, size);
		if (copied >= 0) {
			buff->ux_buf.data_len = (unsigned int)copied;
		} else {
			ret = (int)copied;
			net_error(HMNET_UNIX, "copy unix buffer failed ret = %s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		ret = __try_send_unix_buff_dgram(usk, pusk, buff, &scm_aggr, nonblock);
	}

	if (ret != E_HM_OK) {
		if (buff != NULL) {
			ux_unix_buff_put(buff);
		}
	}

	return (ret == E_HM_OK) ? copied : ret;
}

static ssize_t unix_sendmsg_dgram(ux_unix_sock_t *usk, struct netd_msghdr *msg, int flags)
{
	ux_unix_sock_t *peer = NULL;
	ux_scm_cookie_t scm = {0};
	ssize_t sent = 0;
	int ret = 0;

	ret = unix_scm_send((struct msghdr *)msg, &scm);
	if (ret != E_HM_OK) {
		return ret;
	}

	if ((flags & MSG_OOB) != 0) {
		ret = E_HM_OPNOTSUPP;
	} else {
		ret = unix_get_pusk(usk, msg, &peer);
	}

	if (ret == E_HM_OK) {
		if (!usk->isbind) {
			ux_unix_sock_lock(usk);
			ret = autobind(usk);
			ux_unix_sock_unlock(usk);
		}
	}

	if (ret == E_HM_OK) {
		sent = __unix_sendmsg_dgram(usk, peer, msg, flags, &scm);
		if (sent < 0) {
			ret = (int)sent;
		}
	}

	if (peer != NULL) {
		ux_sock_put(&peer->sk);
	}

	if (ret != E_HM_OK) {
		unix_scm_destroy(&scm);
	}

	return sent != 0 ? sent : ret;
}

static ssize_t unix_sendmsg_seqpacket(ux_unix_sock_t *usk, struct netd_msghdr *msg, int flags)
{
	if (!is_connect_established(usk)) {
		return E_HM_NOTCONN;
	}

	if (msg->msg_namelen) {
		msg->msg_namelen = 0;
	}

	return unix_sendmsg_dgram(usk, msg, flags);
}

ssize_t common_unix_sendmsg(ux_socket_t *sock, struct netd_msghdr *msg, int flags)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if (usk->type == SOCK_STREAM) {
		return unix_sendmsg_stream(usk, msg, flags);
	} else if (usk->type == SOCK_DGRAM) {
		return unix_sendmsg_dgram(usk, msg, flags);
	} else {
		return unix_sendmsg_seqpacket(usk, msg, flags);
	}
}

static int __try_wait_unix_buff(ux_unix_sock_t *usk, int flags)
{
	struct ux_wq_entry waiter = {0};
	int ret;

	if (usk->sk.sk_err != 0) {
		ret = usk->sk.sk_err;
		return ret;
	}

	if (sk_is_noblock(&usk->sk) || (((unsigned int)flags & MSG_DONTWAIT) != 0)) {
		return E_HM_AGAIN;
	}

	ux_wq_prepare_wait(&usk->recvwq, &waiter);

	ux_unix_sock_unlock(usk);
	ret = ux_wq_do_wait(&waiter, usk->sk.rcvtimeo);
	ux_wq_finish_wait(&usk->recvwq, &waiter);
	ux_unix_sock_lock(usk);

	return ret;
}

static ssize_t unix_recvmsg_stream_normal(ux_unix_sock_t *usk, struct netd_msghdr *msg,
					  ux_scm_cookie_t *scm, int flags)
{
	bool finish = false, addr_maked = false;
	ssize_t copied = 0, total = 0;
	int ret = E_HM_OK;

	while (iov_iter_left(&msg->msg_iter) > 0) {
		ux_buf_t *buff = ux_buf_queue_first(&usk->recv_queue);
		if (buff == NULL) {
			/* we do not support rcv lowat now */
			if (total > 0 && (((unsigned int)flags & MSG_WAITALL) == 0)) {
				break;
			}

			if (ux_is_rd_shutdown(usk)) {
				ret = 0;
				break;
			}

			ret = __try_wait_unix_buff(usk, flags);
			if (ret != E_HM_OK) {
				break;
			}

			continue;
		}

		ux_unix_buf_t *unix_buf = container_of(buff, ux_unix_buf_t, ux_buf);
		size_t size_to_copy = size_min(iov_iter_left(&msg->msg_iter), buff->data_len - buff->start);
		copied = ux_copy_data_to_netd_msghdr(msg, buff->data + buff->start, size_to_copy);
		if (copied < 0) {
			ret = (int)copied;
			break;
		}

		if (!addr_maked && (msg->msg_name != NULL)) {
			addr_maked = true;
			ux_unix_make_addr(unix_buf->allocator->path, unix_buf->allocator->pathlen,
					  (struct sockaddr_un *)msg->msg_name, &msg->msg_namelen);
		}

#ifdef CONFIG_SCM_RIGHTS
		if (scm_recv(usk, scm, unix_buf)) {
			/* Stop receiving when scm_fd/scm_creds is not empty */
			finish = true;
		}
#endif
		total += copied;
		buff->start += (size_t)copied;
		usk->recv_queue.len -= copied;
		if (buff->start >= buff->data_len) {
			dlist_delete(&buff->node);
			usk->recv_queue.qlen -= 1;
			ux_unix_buff_put(unix_buf);
			clean_epollin(usk);
		}

		if (finish) {
			break;
		}
	}

	return total > 0 ? total : ret;
}

static ssize_t unix_recvmsg_stream_peek(ux_unix_sock_t *usk, struct netd_msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	ux_unix_buf_t *unix_buf = NULL;
	ux_buf_t *buff = NULL;
	bool finish = false, addr_maked = false;
	ssize_t copied = 0;
	ssize_t total = 0;
	int ret = E_HM_OK;

	while (iov_iter_left(&msg->msg_iter) > 0) {
		if (buff != NULL) {
			buff = ux_buf_queue_next(&usk->recv_queue, buff);
		} else {
			buff = ux_buf_queue_first(&usk->recv_queue);
		}

		if (buff == NULL) {
			/* we do not support rcv lowat now */
			if (copied > 0 && (((unsigned int)flags & MSG_WAITALL) == 0)) {
				break;
			}

			if (ux_is_rd_shutdown(usk)) {
				ret = 0;
				break;
			}

			ret = __try_wait_unix_buff(usk, flags);
			if (ret != E_HM_OK) {
				break;
			}

			continue;
		}

		unix_buf = container_of(buff, ux_unix_buf_t, ux_buf);
		size_t size_to_copy = size_min(iov_iter_left(&msg->msg_iter), buff->data_len - buff->start);
		copied = ux_copy_data_to_netd_msghdr(msg, buff->data + buff->start, size_to_copy);
		if (copied < 0) {
			ret = (int)copied;
			break;
		}

		if (!addr_maked && (msg->msg_name != NULL)) {
			addr_maked = true;
			ux_unix_make_addr(unix_buf->allocator->path, unix_buf->allocator->pathlen,
					  (struct sockaddr_un *)msg->msg_name, &msg->msg_namelen);
		}

#ifdef CONFIG_SCM_RIGHTS
		if (scm_recv(usk, scm, unix_buf)) {
			/* Stop receiving when scm_fd/scm_creds is not empty */
			finish = true;
		}
#endif
		total += copied;

		if (finish) {
			break;
		}
	}

	return total > 0 ? total : ret;
}

static ssize_t unix_recvmsg_stream(ux_unix_sock_t *usk, struct netd_msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	ssize_t total = 0;
	int ret = E_HM_OK;

	ux_unix_sock_lock(usk);

	if (usk->state != SS_CONNECTED) {
		ret = E_HM_INVAL;
	} else if (((unsigned int)flags & MSG_OOB) != 0) {
		ret = E_HM_OPNOTSUPP;
	} else {
		if (((unsigned int)flags & MSG_PEEK) == 0) {
			total = unix_recvmsg_stream_normal(usk, msg, scm, flags);
		} else {
			total = unix_recvmsg_stream_peek(usk, msg, scm, flags);
		}

		if (total < 0) {
			ret = (int)total;
		}
	}

	ux_unix_sock_unlock(usk);

	return total > 0 ? total : ret;
}

static ux_unix_buf_t *__try_get_unix_buff(ux_unix_sock_t *usk, int flags, int *err)
{
	ux_unix_buf_t *unix_buf = NULL;
	int ret = E_HM_OK;

	ux_unix_sock_lock(usk);

	while (ux_unix_buf_truesize(&usk->recv_queue) == 0) {
		struct ux_wq_entry waiter = {0};

		if (ux_is_rd_shutdown(usk)) {
			ret = 0;
			break;
		}

		if (usk->sk.sk_err != 0) {
			ret = usk->sk.sk_err;
			break;
		}

		if (sk_is_noblock(&usk->sk) || (((unsigned int)flags & MSG_DONTWAIT) != 0)) {
			ret = E_HM_AGAIN;
			break;
		}

		ux_wq_prepare_wait(&usk->recvwq, &waiter);
		ux_unix_sock_unlock(usk);

		if (ux_unix_buf_truesize(&usk->recv_queue) == 0) {
			ret = ux_wq_do_wait(&waiter, usk->sk.rcvtimeo);
		}

		ux_wq_finish_wait(&usk->recvwq, &waiter);
		ux_unix_sock_lock(usk);

		if (ret != E_HM_OK) {
			break;
		}
	}

	if (ret == E_HM_OK && ux_unix_buf_truesize(&usk->recv_queue) > 0) {
		ux_buf_t *buff;

		if (((unsigned int)flags & MSG_PEEK) == 0) {
			buff = (ux_buf_t *)ux_buf_queue_dqueue_first(&usk->recv_queue);
			clean_epollin(usk);
		} else {
			buff = (ux_buf_t *)ux_buf_queue_first(&usk->recv_queue);
		}

		if (buff != NULL) {
			unix_buf = container_of(buff, ux_unix_buf_t, ux_buf);
			(void)ux_unix_buff_get(unix_buf);
		}
	}

	ux_unix_sock_unlock(usk);

	if (err != NULL) {
		*err = ret;
	}

	return unix_buf;
}

static ssize_t unix_recvmsg_dgram(ux_unix_sock_t *usk, struct netd_msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	ux_unix_buf_t *unix_buf = NULL;
	ssize_t copied = 0;
	int ret = E_HM_OK;

	if (((unsigned int)flags & MSG_OOB) != 0) {
		return E_HM_OPNOTSUPP;
	}

	unix_buf = __try_get_unix_buff(usk, flags, &ret);
	if (unix_buf != NULL) {
		size_t size_to_copy = size_min(iov_iter_left(&msg->msg_iter), unix_buf->ux_buf.data_len);
		copied = ux_copy_data_to_netd_msghdr(msg, unix_buf->ux_buf.data, size_to_copy);
		if (copied < 0) {
			ret = (int)copied;
		} else {
			if (msg->msg_name != NULL) {
				ux_unix_make_addr(unix_buf->allocator->path, unix_buf->allocator->pathlen,
						  (struct sockaddr_un *)msg->msg_name, &msg->msg_namelen);
			}

#ifdef CONFIG_SCM_RIGHTS
			(void)scm_recv(usk, scm, unix_buf);
#endif
			if (copied < (ssize_t)(unix_buf->ux_buf.data_len)) {
				msg->msg_flags |= MSG_TRUNC;
			}
		}
	}

	if (((unsigned int)flags & MSG_PEEK) == 0) {
		ux_wq_wakeup_all(&usk->recvwq);
		if (unix_buf != NULL) {
			ux_unix_buff_put(unix_buf);
		}
	}

	if (unix_buf != NULL) {
		ux_unix_buff_put(unix_buf);
	}

	return copied > 0 ? copied : (ssize_t)ret;
}

static ssize_t unix_recvmsg_seqpacket(ux_unix_sock_t *usk, struct netd_msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	if (!is_connect_established(usk)) {
		return E_HM_NOTCONN;
	}

	return unix_recvmsg_dgram(usk, msg, scm, flags);
}

ssize_t common_unix_recvmsg(ux_socket_t *sock, struct netd_msghdr *msg, ux_scm_cookie_t *scm, int flags)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if (usk->type == SOCK_STREAM) {
		return unix_recvmsg_stream(usk, msg, scm, flags);
	} else if (usk->type == SOCK_DGRAM) {
		return unix_recvmsg_dgram(usk, msg, scm, flags);
	} else {
		return unix_recvmsg_seqpacket(usk, msg, scm, flags);
	}
}

static unsigned int get_bindtype_by_addr(const struct sockaddr_un *addr, socklen_t addrlen)
{
	unsigned int type;
	if (addrlen == offsetof(struct sockaddr_un, sun_path)) {
		type = BINDTYPE_AUTO_BIND;
	} else if (addr->sun_path[0] != '\0') {
		type = BINDTYPE_BSD;
	} else {
		type = BINDTYPE_ABSTRACT;
	}
	return type;
}

int check_un_addr(const struct sockaddr_un *addr, socklen_t addrlen)
{
	size_t check_addr_len;
	int ret = E_HM_OK;

	if (addr == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (addrlen < offset_of(struct sockaddr_un, sun_path)) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		check_addr_len = (size_t)addrlen - offset_of(struct sockaddr_un, sun_path);
		if (check_addr_len > USK_PATH_USER_MAX) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		if (addr->sun_family != AF_UNIX) {
			ret = E_HM_INVAL;
		}
	}

	return ret;
}

static void insert_bindlist(ux_unix_sock_t *usk)
{
	ux_sock_get(&usk->sk);
	dlist_insert(&ux_unix_bindlist, &usk->u.bindlnode);
	usk->isbind = true;
	return;
}

static int insert_bindtable(ux_unix_sock_t *usk, ux_namespace_t *ns)
{
	struct unix_path_key key = {usk->path, ns, usk->pathlen};
	return ux_htable_insert(&ux_unix_bindtable, H_UNIX_PATH, &key, &usk->u.bindrnode);
}

static int insert_abstract_path(ux_unix_sock_t *usk)
{
	int ret;

	(void)raw_rwlock_wrlock(&ux_unix_bindlock);

	ret = insert_bindtable(usk, ux_sock_ns(&usk->sk));
	if (ret == E_HM_OK) {
		ux_sock_get(&usk->sk);
		usk->isbind = true;
	} else {
		ret = E_HM_ADDRINUSE;
	}

	(void)raw_rwlock_unlock(&ux_unix_bindlock);

	return ret;
}

static int autobind(ux_unix_sock_t *usk)
{
	static unsigned int autobind_idx = 1;

	unsigned int retries = 0;
	int ret = E_HM_NOSPC;

	if (usk->isbind) {
		return E_HM_OK;
	}

	while (retries++ != 0xFFFFF) {
		int len = sprintf_s(usk->path + 1, USK_PATH_LOCAL_MAX - 1, "%05x", autobind_idx++ % 0xFFFFF);
		if (len < 0) {
			ret = E_HM_INVAL;
			break;
		}

		usk->path[0] = '\0';
		usk->pathlen = (unsigned char)len + 1;
		ret = insert_abstract_path(usk);
		if (ret == E_HM_OK) {
			break;
		}
	}

	return ret;
}

static int bind_abstract(ux_unix_sock_t *usk, const struct sockaddr_un *addr_un, socklen_t addrlen)
{
	size_t pathlen;

	if (usk->isbind) {
		return E_HM_INVAL;
	}

	pathlen = addrlen - offset_of(struct sockaddr_un, sun_path);
	NOFAIL(memcpy_s(usk->path, USK_PATH_LOCAL_MAX, (void *)addr_un->sun_path, pathlen));
	usk->pathlen = (unsigned char)pathlen;

	return insert_abstract_path(usk);
}

static size_t get_unix_bsd_path_len(const struct sockaddr_un *addr_un, socklen_t addrlen)
{
	size_t pathlen = (size_t)addrlen - offset_of(struct sockaddr_un, sun_path);
	char *tmp = memchr(addr_un->sun_path, '\0', pathlen);
	if (tmp != NULL) {
		pathlen = (size_t)(tmp - addr_un->sun_path);
	}

	return pathlen;
}

static int bind_bsd(ux_unix_sock_t *usk, const struct sockaddr_un *addr_un, socklen_t addrlen)
{
	char safe_path[USK_PATH_LOCAL_MAX] = {0};
	size_t pathlen;
	int ret = E_HM_OK;
	int rc;
	vfs_unix_node_t unode = {0};

	pathlen = get_unix_bsd_path_len(addr_un, addrlen);
	NOFAIL(memcpy_s(safe_path, sizeof(safe_path), addr_un->sun_path, pathlen));

	/*
	 * the path length may equal USK_PATH_USER_MAX, and vfs_unix_mknod need
	 * a zero end string.
	 */
	ret = vfs_unix_mknod(safe_path, S_IFSOCK | BIND_FILE_PERM, 0, &unode);
	if (ret < 0) {
		net_debug(HMNET_UNIX, "vfs_unix_mknod failed, error:%s\n", hmstrerror(ret));
		if (ret == E_HM_POSIX_EXIST) {
			ret = E_HM_ADDRINUSE;
		}
	}

	if (ret == E_HM_OK && usk->isbind) {
		ret = E_HM_INVAL;
		rc = vfs_unix_put_node(unode);
		if (rc != E_HM_OK) {
			net_error(HMNET_UNIX, "vfs_unix_put_node failed, error:%s\n", hmstrerror(rc));
		}
	}

	if (ret == E_HM_OK) {
		ux_unix_try_server_fastpath(usk, (char *)addr_un->sun_path);
	}

	if (ret == E_HM_OK) {
		NOFAIL(memcpy_s(&usk->unode, sizeof(usk->unode), &unode, sizeof(unode)));
		usk->pathlen = (unsigned char)pathlen;
		NOFAIL(memcpy_s(usk->path, USK_PATH_LOCAL_MAX, (void *)addr_un->sun_path, pathlen));
		(void)raw_rwlock_wrlock(&ux_unix_bindlock);
		insert_bindlist(usk);
		(void)raw_rwlock_unlock(&ux_unix_bindlock);
	}

	return ret;
}

int __unix_bind(ux_unix_sock_t *usk, const struct sockaddr_un *addr_un, socklen_t addrlen)
{
	int ret;

	ret = check_un_addr(addr_un, addrlen);
	if (ret == E_HM_OK) {
		unsigned int type = get_bindtype_by_addr(addr_un, addrlen);

		ux_unix_sock_lock(usk);

		switch (type) {
		case BINDTYPE_AUTO_BIND:
			ret = autobind(usk);
			break;
		case BINDTYPE_ABSTRACT:
			ret = bind_abstract(usk, addr_un, addrlen);
			break;
		case BINDTYPE_BSD:
		/* now set BSD as default */
		default:
			ret = bind_bsd(usk, addr_un, addrlen);
			break;
		}

		ux_unix_sock_unlock(usk);
	}

	return ret;
}

static ux_unix_sock_t *get_sock_from_bindlist_abstract(const char *path, size_t len, ux_namespace_t *ns)
{
	int ret = E_HM_NOENT;
	ux_unix_sock_t *usk = NULL;
	struct unix_path_key key = {path, ns, (unsigned char)len};

	(void)raw_rwlock_rdlock(&ux_unix_bindlock);
	usk = ux_htable_lookup(&ux_unix_bindtable, H_UNIX_PATH, &key);
	if (usk != NULL) {
		ret = E_HM_OK;
		ux_sock_get(&usk->sk);
	}

	(void)raw_rwlock_unlock(&ux_unix_bindlock);
	return ret == E_HM_OK ? usk : NULL;
}

static ux_unix_sock_t *get_sock_from_bindlist_bsd(vfs_unix_node_t *unode)
{
	ux_unix_sock_t *usk = NULL;
	int ret = E_HM_OK;

	(void)raw_rwlock_rdlock(&ux_unix_bindlock);

	if (dlist_empty(&ux_unix_bindlist)) {
		ret = E_HM_NOENT;
	}

	if (ret == E_HM_OK) {
		ret = E_HM_NOENT;
		dlist_for_each_entry(usk, &ux_unix_bindlist, ux_unix_sock_t, u.bindlnode) {
			if (usk->unode.dev_id == unode->dev_id &&
			    usk->unode.index == unode->index) {
				ux_sock_get(&usk->sk);
				ret = E_HM_OK;
				break;
			}
		}
	}

	(void)raw_rwlock_unlock(&ux_unix_bindlock);
	return ret == E_HM_OK ? usk : NULL;
}

static ux_unix_sock_t *find_abstract_sock_from_bindlist(const struct sockaddr_un *addr_un, socklen_t addrlen,
							ux_namespace_t *ns)
{
	size_t pathlen = (size_t)addrlen - offset_of(struct sockaddr_un, sun_path);

	return get_sock_from_bindlist_abstract(addr_un->sun_path, pathlen, ns);
}

static int find_bsd_sock_from_bindlist(const struct sockaddr_un *addr_un, socklen_t addrlen,
				       ux_unix_sock_t **peer)
{
	size_t pathlen;
	int ret;
	char safe_path[USK_PATH_LOCAL_MAX] = {0};
	vfs_unix_node_t unode = {0};
	bool hold_inode = false;
	int rc;

	*peer = NULL;

	pathlen = get_unix_bsd_path_len(addr_un, addrlen);

	/*
	 * addr_un->sun_path is not guaranteed to be null terminated, so we
	 * copy content pollinted to by path to safe_path.
	 */
	NOFAIL(memcpy_s(safe_path, sizeof(safe_path), addr_un->sun_path, pathlen));
	ret = vfs_unix_access(safe_path, W_OK, &unode);
	if (ret == E_HM_OK) {
		hold_inode = true;
		*peer = get_sock_from_bindlist_bsd(&unode);
		if (*peer == NULL) {
			ret = E_HM_CONNREFUSED;
		}
	} else {
		net_debug(HMNET_UNIX, "vfs_unix_access failed, error:%s\n", hmstrerror(ret));
	}

	if (hold_inode) {
		rc = vfs_unix_put_node(unode);
		if (rc != E_HM_OK) {
			net_error(HMNET_UNIX, "vfs_unix_put_node failed, error:%s\n", hmstrerror(rc));
		}
	}

	return ret;
}

int find_peer_by_addr(const struct sockaddr *addr, socklen_t addrlen, ux_namespace_t *ns, ux_unix_sock_t **peer)
{
	int ret;
	const struct sockaddr_un *addr_un = (const struct sockaddr_un *)(uintptr_t)addr;
	*peer = NULL;

	ret = check_un_addr(addr_un, addrlen);
	if (ret == E_HM_OK) {
		unsigned int type = get_bindtype_by_addr(addr_un, addrlen);
		switch (type) {
		case BINDTYPE_AUTO_BIND:
			ret = E_HM_INVAL;
			break;
		case BINDTYPE_ABSTRACT:
			*peer = find_abstract_sock_from_bindlist(addr_un, addrlen, ns);
			if (*peer == NULL) {
				ret = E_HM_CONNREFUSED;
			}
			break;
		case BINDTYPE_BSD:
		/* now set BSD as default */
		default:
			ret = find_bsd_sock_from_bindlist(addr_un, addrlen, peer);
			break;
		}
	}

	return ret;
}

void bind_path_release(ux_unix_sock_t *usk)
{
	int ret = E_HM_OK;

	delete_bindnode(usk);

	if (if_unix_sock_bind_bsd(usk)) {
		ret = vfs_unix_put_node(usk->unode);
		if (ret < 0) {
			net_debug(HMNET_UNIX, "vfs_unix_put_node failed, error:%s\n", hmstrerror(ret));
		}
	}
 
	/*
	 * don't clean usk->path here,
	 * for someone(focus on peer addr) getpeername, accepting or recvfroming
	 */

	return;
}

int ux_unix_init_root_ns_pid(ux_unix_sock_t *usk)
{
	pid_t root_ns_pid = -1;
	int rc = E_HM_OK;

	ux_unix_release_root_ns_pid(usk);

	root_ns_pid = osal_hm_root_ns_pid();
	if (root_ns_pid < 0) {
		return (int)root_ns_pid;
	}

	rc = osal_hm_lock_pid(root_ns_pid);
	if (rc < 0) {
		return rc;
	}

	usk->root_ns_pid = root_ns_pid;

	return E_HM_OK;
}

int ux_unix_copy_root_ns_pid(ux_unix_sock_t *from, ux_unix_sock_t *to)
{
	int rc = E_HM_OK;

	ux_unix_release_root_ns_pid(to);

	if (from->root_ns_pid > 0) {
		rc = osal_hm_lock_pid(from->root_ns_pid);
		if (rc == E_HM_OK) {
			to->root_ns_pid = from->root_ns_pid;
		}
	}

	return rc;
}

void ux_unix_release_root_ns_pid(ux_unix_sock_t *usk)
{
	if (usk->root_ns_pid > 0) {
		osal_hm_unlock_pid(usk->root_ns_pid);
		usk->root_ns_pid = -1;
	}
}
