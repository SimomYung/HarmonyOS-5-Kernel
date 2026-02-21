/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network sock manager
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>
#include <libmem/utils.h>
#include <hm_socket.h>

#include "osal.h"
#include "ux_utils.h"
#include "ux_socket.h"
#include "ux_netlog.h"
#include "ux_fdtable.h"
#include "ux_sockopt.h"
#include "ux_eventpoll.h"
#include "ux_namespace.h"
#include "ux_revoke_handler.h"

static struct raw_mutex ux_owner_mutex = RAW_MUTEX_INITIALIZER;
static struct rbt_tree ux_owner_tree = RBT_ROOT;

static int sock_insert(ux_sock_owner_t *owner, ux_socket_t *sock, int fd);
static void sock_remove(ux_sock_owner_t *owner, ux_socket_t *sock, int fd);

static ux_sock_owner_t *__socket_lookup_owner(struct rbt_tree *tree,
						uint32_t key)
{
	struct rbt_node *p = tree->root;
	ux_sock_owner_t *owner = NULL;

	while (p != NULL) {
		owner = rbt_entry(p, ux_sock_owner_t, node);
		if (owner->key == key) {
			return owner;
		} else if (owner->key > key) {
			p = p->rbt_left;
		} else {
			p = p->rbt_right;
		}
	}

	return NULL;
}

static int __socket_insert_owner(struct rbt_tree *tree,
				 ux_sock_owner_t *owner)
{
	struct rbt_node *p = tree->root, *parent = NULL;
	ux_sock_owner_t *cur = NULL;
	bool is_rchild = false;

	while (p != NULL) {
		parent = p;
		cur = rbt_entry(p, ux_sock_owner_t, node);
		if (owner->key > cur->key) {
			p = p->rbt_right;
			if (p == NULL) {
				is_rchild = true;
			}
		} else if (owner->key < cur->key) {
			p = p->rbt_left;
			if (p == NULL) {
				is_rchild = false;
			}
		} else {
			return E_HM_INVAL;
		}
	}

	rbt_insert(tree, &owner->node, parent, is_rchild);
	return 0;
}

static void __ux_sock_owner_free(ux_sock_owner_t *owner)
{
	int ret;

	if (owner == NULL) {
		return;
	}

	ret = syscap_FDTableCleanupPolllist(owner->fdtable_rref);
	if (ret < 0) {
		net_error(HMNET_CORE, "cleaup polllist failed. ret = %d(%s)\n", ret, hmstrerror(ret));
	}

	if (IS_REF_VALID(owner->fdtable_rref)) {
		ret = hm_ucap_reject(0, owner->fdtable_rref);
		if (ret < 0) {
			net_error(HMNET_CORE, "reject fdtable_rref failed. ret = %d(%s)\n",
				  ret, hmstrerror(ret));
		}
	}

	if (owner->sockets != NULL) {
		for (unsigned int i = 0; i < owner->max_sockets; ++i) {
			if (owner->sockets[i] != NULL) {
				sock_remove(owner, owner->sockets[i], (int)i);
			}
		}
		free(owner->sockets);
	}

	ux_namespace_revoke(owner);

	free(owner->ssocks);
	free(owner);
}

static int __ux_sock_owner_init_fdtable(ux_sock_owner_t *owner, uint32_t cnode)
{
	int ret;

	ret = ux_fdtable_acquire_rref(cnode, &owner->fdtable_rref);
	if (ret == E_HM_OK) {
		ret = ux_fdtable_set_sock_owner(owner);
	}

	return ret;
}

static int __ux_sock_owner_do_fork(ux_sock_owner_t *src_owner, ux_sock_owner_t *dst_owner)
{
	ux_socket_t *sock = NULL;
	int ret = E_HM_OK;
	int rc = E_HM_OK;

	for (unsigned int i = 0; i < src_owner->max_sockets; i++) {
		if (src_owner->sockets[i] != NULL) {
			sock = src_owner->sockets[i];
			ret = sock_insert(dst_owner, sock, (int)i);
			if (ret != E_HM_OK) {
				net_warn(HMNET_CORE, "sock_insert failed, fd=%u, ret=%s\n", i, hmstrerror(ret));
			}
		}
	}

	rc = ux_fdtable_fork_fdtable(src_owner, dst_owner);
	if (rc == E_HM_OK) {
		ux_namespace_fork(src_owner, dst_owner);
	}

	return rc;
}

static ux_sock_owner_t *__ux_sock_owner_alloc(uint32_t cnode, unsigned int max_sockets,
					      ux_sock_owner_t *src_owner)
{
	ux_sock_owner_t *owner = calloc(1, sizeof(ux_sock_owner_t));
	int ret = 0;

	if (owner == NULL) {
		ret = E_HM_NOMEM;
	} else {
		raw_mutex_init(&owner->lock);
		raw_mutex_init(&owner->sslock);
		owner->key = cnode;
		owner->count = 0;

		/**
		 * initial refcnt count set to 1, pair with ux_sock_owner_detach
		 * when do revoke
		 */
		raw_refcnt_nr_init(&owner->refcnt, 1);

		/* use an array to store sockets */
		owner->max_sockets = max_sockets;
		owner->sockets = (ux_socket_t **)calloc(max_sockets, sizeof(ux_socket_t *));
		if (owner->sockets == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		ret = __ux_sock_owner_init_fdtable(owner, cnode);
	}

	if (ret == E_HM_OK) {
		if (src_owner != NULL) {
			ret = __ux_sock_owner_do_fork(src_owner, owner);
		}
	}

	if (ret == E_HM_OK) {
		/*
		 * __ux_sock_owner_free do not support remove from rbt tree
		 * here must be the last step
		 */
		ret = __socket_insert_owner(&ux_owner_tree, owner);
	}

	if (ret != E_HM_OK) {
		__ux_sock_owner_free(owner);
		owner = NULL;
	}

	return owner;
}

int ux_sock_owner_foreach(int (*iter)(ux_sock_owner_t *owner, void *arg), void *arg)
{
	int ret = E_HM_OK;
	ux_sock_owner_t *owner;
	struct rbt_node *next;

	raw_mutex_lock(&ux_owner_mutex);
	rbt_for_each_entry_safe(owner, next, &ux_owner_tree, ux_sock_owner_t, node) {
		ret = (*iter)(owner, arg);
		if (ret < 0) {
			break;
		}
	}

	raw_mutex_unlock(&ux_owner_mutex);
	return ret;
}

ux_sock_owner_t *ux_sock_owner_current(void)
{
	return ulong_to_ptr(osal_hm_actv_cred(), ux_sock_owner_t);
}

ux_sock_owner_t *ux_sock_owner_get(uint32_t cnode, int create)
{
	ux_sock_owner_t *owner = NULL;

	raw_mutex_lock(&ux_owner_mutex);
	owner = __socket_lookup_owner(&ux_owner_tree, cnode);
	if (owner == NULL && (create != 0)) {
		owner = __ux_sock_owner_alloc(cnode, MAX_SOCKET_NUMS, NULL);
	}
	if (owner != NULL) {
		int ret = raw_refcnt_nr_get(&owner->refcnt);
		BUG_ON(ret <= 0, "owner refcnt fail, refcnt %d, ret %d(%s)\n",
		       raw_refcnt_nr_counter(&owner->refcnt), ret, hmstrerror(ret));
	}
	raw_mutex_unlock(&ux_owner_mutex);

	return owner;
}

void ux_sock_owner_put(ux_sock_owner_t *owner)
{
	bool destroy = false;

	if (owner == NULL) {
		return;
	}

	raw_mutex_lock(&ux_owner_mutex);
	if (raw_refcnt_nr_put(&owner->refcnt) == 0) {
		rbt_remove(&ux_owner_tree, &owner->node);
		destroy = true;
	}
	raw_mutex_unlock(&ux_owner_mutex);

	if (destroy == true) {
		__ux_sock_owner_free(owner);
	}
}

ux_sock_owner_t *ux_sock_owner_get_fast(uint32_t cnode, int create)
{
#if defined(LLT) || defined(CONFIG_NETFS_FUSION)
	return ux_sock_owner_get(cnode, create);
#else
	UNUSED(cnode, create);
	return ux_sock_owner_current();
#endif
}

void ux_sock_owner_put_fast(ux_sock_owner_t *owner)
{
#if defined(LLT) || defined(CONFIG_NETFS_FUSION)
	return ux_sock_owner_put(owner);
#else
	UNUSED(owner);
#endif
}

void ux_sock_owner_detach(ux_sock_owner_t *owner)
{
	if (owner == NULL) {
		return;
	}

	raw_mutex_lock(&ux_owner_mutex);
	rbt_remove(&ux_owner_tree, &owner->node);
	raw_mutex_unlock(&ux_owner_mutex);

	ux_sock_owner_put(owner);
}

int ux_sock_owner_fork(ux_sock_owner_t *src_owner, uint32_t dst_cnode_idx)
{
	ux_sock_owner_t *owner = NULL;
	int ret = E_HM_OK;

	raw_mutex_lock(&ux_owner_mutex);
	raw_mutex_lock(&src_owner->lock);

	owner = __socket_lookup_owner(&ux_owner_tree, dst_cnode_idx);
	if (owner != NULL) {
		ret = E_HM_EXIST;
	} else {
		owner = __ux_sock_owner_alloc(dst_cnode_idx, src_owner->max_sockets, src_owner);
		if (owner == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	raw_mutex_unlock(&src_owner->lock);
	raw_mutex_unlock(&ux_owner_mutex);

	return ret;
}

static int sock_insert(ux_sock_owner_t *owner, ux_socket_t *sock, int fd)
{
	int ret = E_HM_OK;

	if (owner == NULL || sock == NULL) {
		return E_HM_INVAL;
	}

	owner->count++;
	if ((unsigned int)owner->count > owner->max_sockets) {
		owner->count = (int)owner->max_sockets;
		ret = E_HM_MFILE;
	} else {
		if (owner->sockets[fd] == NULL) {
			owner->sockets[fd] = sock;
			NOFAIL(raw_compounded_refcnt_get(&sock->refcnt, NULL));
			ret = E_HM_OK;
		} else {
			ret = E_HM_INVAL;
			owner->count--;
			net_warn(HMNET_CORE, "shouldn't reach here fd %d\n", fd);
		}
	}

	return ret;
}

static void sock_remove(ux_sock_owner_t *owner, ux_socket_t *sock, int fd)
{
	if (owner == NULL || sock == NULL) {
		return;
	}

	if (owner->sockets != NULL) {
		if (owner->sockets[fd] != sock) {
			net_error(HMNET_CORE, "fd %d is not consistent\n", fd);
			owner->sockets[fd] = NULL;
		} else {
			owner->sockets[fd] = NULL;
			ux_socket_put(sock);
		}
	}

	owner->count--;
	if (owner->count < 0) {
		owner->count = 0;
	}
}

ux_socket_t *ux_socket_alloc(void)
{
	ux_socket_t *socket = NULL;

	socket = calloc(1, sizeof(ux_socket_t));
	if (socket == NULL) {
		net_error(HMNET_CORE, "failed to alloc socket\n");
		return NULL;
	}

	raw_compounded_refcnt_init(&socket->refcnt, 0ULL);
	raw_mutex_init(&socket->lock);
	socket->mark = 0;
	socket->cookie = generate_hmnetd_cookie(socket->cookie);

	return socket;
}

void ux_socket_free(ux_socket_t *sock)
{
	if (sock->sk != NULL) {
		ux_sock_set_socket(sock->sk, NULL);
		if (sock->ops != NULL && sock->ops->close != NULL) {
			sock->ops->close(sock);
		}

		ux_socket_set_sock(sock, NULL);
	}

	free(sock);
}

int ux_socket_insert(ux_sock_owner_t *owner, ux_socket_t *sock, int fd)
{
	int ret = E_HM_OK;

	while ((unsigned int)fd >= owner->max_sockets) {
		/*
		 * enlarge by MAX_SOCKET_NUMS each time
		 */
		ret = ux_sock_owner_resize_sock_table(owner, fd + MAX_SOCKET_NUMS);
		if (ret != E_HM_OK) {
			break;
		}
	}

	if (ret == E_HM_OK) {
		ret = sock_insert(owner, sock, fd);
	}

	return ret;
}

int ux_socket_attach(ux_sock_owner_t *owner, ux_socket_t *sock, int fd)
{
	int ret;

	raw_mutex_lock(&owner->lock);

	ret = ux_socket_insert(owner, sock, fd);
	if (ret == E_HM_OK) {
		ret = ux_fdtable_install_fd(owner->fdtable_rref, (unsigned int)fd, sock);
		if (ret != E_HM_OK) {
			sock_remove(owner, sock, fd);
			net_error(HMNET_CORE, "install fd %d failed, ret = %d(%s)\n",
				  fd, ret, hmstrerror(ret));
		}
	}

	raw_mutex_unlock(&owner->lock);

	return ret;
}

int ux_socket_detach(ux_sock_owner_t *owner, ux_socket_t *sock, int fd)
{
	int ret = E_HM_OK;

	raw_mutex_lock(&owner->lock);

	if (owner->sockets[fd] == NULL) {
		net_warn(HMNET_CORE, "fd %d already detached\n", fd);
		ret = E_HM_NOENT;
	} else {
		sock_remove(owner, sock, fd);
	}

	raw_mutex_unlock(&owner->lock);

	return ret;
}

int ux_socket_detach_fd(ux_sock_owner_t *owner, ux_socket_t *sock, int fd, bool balance)
{
	int ret = E_HM_OK;
	unsigned int rpccnt = 0;

	raw_mutex_lock(&owner->lock);

	if ((unsigned int)fd >= owner->max_sockets) {
		net_warn(HMNET_CORE, "fd %d invalid, max_sockets: %u\n", fd, owner->max_sockets);
		raw_mutex_unlock(&owner->lock);
		return E_HM_INVAL;
	}

	ret = ux_fdtable_close_fd(owner->fdtable_rref, (unsigned)fd, &sock,
				  &rpccnt);
	if (ret != E_HM_OK) {
		raw_mutex_unlock(&owner->lock);
		net_error(HMNET_CORE, "free fd %d failed, ret is %s\n", fd, hmstrerror(ret));
		return ret;
	}

	if (balance && rpccnt > 0) {
		uint64_t res = 0ULL;
		NOFAIL(raw_compounded_rpccnt_balance(&sock->refcnt,
						     (uint64_t)rpccnt,
						     &res));
		BUG_ON(res == 0ULL);
	}

	if (owner->sockets[fd] == NULL) {
		net_warn(HMNET_CORE, "fd %d already detached\n", fd);
		ret = E_HM_NOENT;
	} else {
		sock_remove(owner, sock, fd);
	}

	raw_mutex_unlock(&owner->lock);

	return ret;
}

static ux_socket_t *__ux_socket_get(ux_sock_owner_t *owner, int fd)
{
	ux_socket_t *sock = NULL;

	raw_mutex_lock(&owner->lock);
	if (fd >= 0 && (unsigned int)fd < owner->max_sockets) {
		sock = owner->sockets[fd];
		if (sock != NULL) {
			NOFAIL(raw_compounded_refcnt_get(&sock->refcnt, NULL));
		}
	}
	raw_mutex_unlock(&owner->lock);

	return sock;
}

ux_socket_t *ux_socket_get_slow(uint32_t cnode, int fd)
{
	ux_sock_owner_t *owner = NULL;
	ux_socket_t *sock = NULL;

	owner = ux_sock_owner_get(cnode, 0);
	if (owner == NULL) {
		return NULL;
	}

	sock = __ux_socket_get(owner, fd);

	ux_sock_owner_put(owner);

	return sock;
}

static inline ux_socket_t *ux_socket_get_fast(uint32_t cnode, int fd)
{
	ux_sock_owner_t *owner = NULL;
	ux_socket_t *sock = NULL;

	UNUSED(cnode);
	owner = ux_sock_owner_current();
	if (owner == NULL) {
		return NULL;
	}

	sock = __ux_socket_get(owner, fd);

	return sock;
}

ux_socket_t *ux_socket_get(uint32_t cnode, int fd)
{
#if defined(LLT) || defined(CONFIG_NETFS_FUSION)
	return ux_socket_get_slow(cnode, fd);
#else
	return ux_socket_get_fast(cnode, fd);
#endif
}

void ux_socket_put(ux_socket_t *sock)
{
	if (sock != NULL) {
		uint64_t res = 0ULL;
		int ret = raw_compounded_refcnt_put(&sock->refcnt, &res);
		if (unlikely(ret != E_HM_OK)) {
			hm_panic("refcnt=%llx, err=%s\n",(long long)vatomic64_read(&sock->refcnt.v), hmstrerror(ret));
		}
		if (res == 0ULL) {
			ux_socket_free(sock);
		}
	}
}

void ux_socket_rpccnt_put(ux_socket_t *sock)
{
	uint64_t res = 0ULL;
	int ret = raw_compounded_rpccnt_put(&sock->refcnt, &res);
	if (unlikely(ret != E_HM_OK)) {
		hm_panic("fail to put, sock refcnt=0x%llx, ret=%d(%s), res=0x%llx\n",
			 (unsigned long long)vatomic64_read(&sock->refcnt.v),
			 ret, hmstrerror(ret), (unsigned long long)res);
	}
	if (res == 0ULL) {
		ux_socket_free(sock);
	}
}

int ux_sock_owner_resize_sock_table(ux_sock_owner_t *owner, unsigned int new_count)
{
	const size_t new_size = sizeof(struct ux_socket *) * new_count;
	struct ux_socket **sockets = NULL;
	int ret = E_HM_OK;

	sockets = malloc(new_size);
	if (sockets == NULL) {
		ret = E_HM_NOMEM;
	} else {
		NOFAIL(memset_s(sockets, new_size, 0, new_size));
		/**
		 * owner->max_sockets may be greater than new_count.
		 */
		NOFAIL(memcpy_s(sockets, new_size, owner->sockets,
				owner->max_sockets * sizeof(struct ux_socket *)));
		free(owner->sockets);
		owner->sockets = sockets;
		owner->max_sockets = new_count;
	}

	return ret;
}

void ux_socket_lock(ux_socket_t *sock)
{
	raw_mutex_lock(&sock->lock);
}

void ux_socket_unlock(ux_socket_t *sock)
{
	raw_mutex_unlock(&sock->lock);
}

int ux_socket_get_timestamp(ux_socket_t *sock, struct timeval *arg)
{
	struct timeval tv;
	int ret = 0;

	bzero(&tv, sizeof(tv));
	if ((sock->flags & UX_SOCK_TIMESTAMP) == 0UL) {
		sock->flags |= UX_SOCK_TIMESTAMP;
	}

	tv = sock->ts;
	if (tv.tv_sec == 0) {
		if ((ret = gettimeofday(&sock->ts, NULL)) != 0) {
			return posix2hmerrno(errno);
		}
		tv = sock->ts;
	}
	ret = memcpy_s(arg, sizeof(struct timeval), &tv, sizeof(struct timeval));
	if (ret != 0) {
		return E_HM_POSIX_FAULT;
	}
	return E_HM_OK;
}

void socket_clear_ssocks(ux_sock_owner_t *owner)
{
	ux_sock_trans_info_t tmp, *ssock;

	if (owner->ssocks == NULL) {
		return;
	}

	for (int i = 0; i < owner->ssocks_count; ++i) {
		ssock = &owner->ssocks[i];
		raw_mutex_lock(&owner->sslock);
		if (ssock->fstype == SCM_FS_INVALID) {
			raw_mutex_unlock(&owner->sslock);
			continue;
		}
		tmp = *ssock;
		NOFAIL(memset_s(ssock, sizeof(ux_sock_trans_info_t), 0,
				sizeof(ux_sock_trans_info_t)));
		ssock->vfsfd_idx = -1;
		raw_mutex_unlock(&owner->sslock);
		do_put_send_fd(tmp.fstype, tmp.sock, tmp.vfsfd_idx, tmp.send_cidx,
			       tmp.fssrv_name);
	}
}

DEFINE_UPCALL_ACTVHDLR_ALS(handle_common_file_close)
{
	int err = E_HM_OK;
	ux_sock_owner_t *owner = NULL;
	ux_socket_t *sock = NULL;
	int fd = (int)args.common_file_close.fd;
	__u32 cnode_idx = args.common_file_close.cnode_idx;
	unsigned int rpccnt = args.common_file_close.rpccnt;

	owner = ux_sock_owner_current();
	if (owner == NULL) {
		/* update FILE_CLOSE_RET when this upcall support deal ret */
		return __ACTV_WFERET_FILE_CLOSE_OK;
	}
	sock = ux_socket_get(cnode_idx, fd);

	err = ux_socket_detach(owner, sock, fd);
	if (err != E_HM_OK) {
		net_warn(HMNET_CORE, "Failed to detach cnode=0x%x, fd=%d\n", cnode_idx, fd);
	}
	if (rpccnt > 0) {
		uint64_t res = 0ULL;
		NOFAIL(raw_compounded_rpccnt_balance(&sock->refcnt,
						     (uint64_t)rpccnt,
						     &res));
		BUG_ON(res == 0);
	}
	ux_socket_put(sock);

	return __ACTV_WFERET_FILE_CLOSE_OK;
}

static void ux_sockmgr_revoke_handler(uint32_t cnode)
{
	ux_sock_owner_t *owner = NULL;
	ux_socket_t *sock = NULL;

#ifdef CONFIG_NETFS_FUSION
	if (is_raw_mutex_disabled()) {
		return;
	}
#endif

	raw_mutex_lock(&ux_owner_mutex);
	owner = __socket_lookup_owner(&ux_owner_tree, cnode);
	/* the terminated client process didn't create a socket */
	if (owner == NULL) {
		raw_mutex_unlock(&ux_owner_mutex);
		return;
	}
	rbt_remove(&ux_owner_tree, &owner->node);
	raw_mutex_unlock(&ux_owner_mutex);

	raw_mutex_lock(&owner->lock);

	for (unsigned int i = 0; i < owner->max_sockets; i++) {
		if (owner->sockets != NULL && owner->sockets[i] != NULL) {
			sock = owner->sockets[i];
			owner->sockets[i] = NULL;
			ux_socket_put(sock);
		}
	}

	owner->count = 0;

	raw_mutex_unlock(&owner->lock);

	socket_clear_ssocks(owner);

	if (raw_refcnt_nr_put(&owner->refcnt) == 0) {
		__ux_sock_owner_free(owner);
	}
}

int ux_sockmgr_init(void)
{
	return ux_register_revoke_handler("sockmgr", ux_sockmgr_revoke_handler);
}

DEFINE_UPCALL_ACTVHDLR_ALS(handle_close_on_exec)
{
	int err;
	int rc = E_HM_OK;
	unsigned int fd_start = 0U;
	__u32 cnode_idx = args.close_on_exec.cnode_idx;
	size_t num = PAGE_SIZE / sizeof(struct sysarg_fdtable_close_on_exec);
	struct sysarg_fdtable_close_on_exec *data = NULL;

	ux_sock_owner_t *owner = ux_sock_owner_current();
	if (owner == NULL) {
		hm_actvpool_set_kcall_retval(sender, credential, 0);
		return __ACTV_WFERET_CLOSE_ON_EXEC_RET;
	}
	BUG_ON(IS_REF_ERR(owner->fdtable_rref));

	data = (struct sysarg_fdtable_close_on_exec *)osal_malloc(PAGE_SIZE);
	if (data == NULL) {
		hm_actvpool_set_kcall_retval(sender, credential, (unsigned long)(long)E_HM_NOMEM);
		return __ACTV_WFERET_CLOSE_ON_EXEC_RET;
	}

	do {
		err = syscap_FDTableGatherCommonFiles(owner->fdtable_rref,
						      data, num, &fd_start,
						      GATHER_CLOEXEC_FILES, FD_NET);
		if (err <= 0) {
			break;
		}
		BUG_ON(err > (int)num);
		for (int i = 0; i < err; i++) {
			int fd = data[i].remote_fd;
			unsigned int rpccnt = data[i].rpccnt;
			ux_socket_t *sock = ux_socket_get(cnode_idx, fd);
			BUG_ON(sock == NULL);

			rc = ux_socket_detach_fd(owner, sock, fd, false);
			if (rpccnt > 0) {
				uint64_t res = 0ULL;
				NOFAIL(raw_compounded_rpccnt_balance(&sock->refcnt,
								     (uint64_t)rpccnt,
								     &res));
				BUG_ON(res == 0);
			}
			ux_socket_put(sock);
			if (rc != E_HM_OK) {
				break;
			}
		}
	} while (err == (int)num);

	if (err < 0) {
		rc = err;
	}

	osal_free(data);
	hm_actvpool_set_kcall_retval(sender, credential, (unsigned long)(long)rc);
	return __ACTV_WFERET_CLOSE_ON_EXEC_RET;
}

int net_do_handle_rpccnt_put(union __arch_actv_upcall_args_u args)
{
	int ret;
	uint64_t res = 0ULL;
	ux_socket_t *sock = NULL;
	uintptr_t common_file = args.rpccnt.common_file;

	if (is_common_file_overflow(common_file)) {
		common_file = clear_common_file_overflow(common_file);
		sock = (ux_socket_t *)common_file;
		NOFAIL(raw_compounded_rpccnt_balance(&sock->refcnt,
						     MAX_FD_RPCCNT, &res));
		ret = __ACTV_WFERET_RPCCNT_PUT_REWIND;
	} else {
		sock = (ux_socket_t *)common_file;
		NOFAIL(raw_compounded_rpccnt_put(&sock->refcnt, &res));
		ret = __ACTV_WFERET_RPCCNT_PUT_RET;
	}

	if (res == 0) {
		ux_socket_free(sock);
	}

	return ret;
}

DEFINE_UPCALL_ACTVHDLR_ALS(handle_rpccnt_put)
{
	return net_do_handle_rpccnt_put(args);
}
