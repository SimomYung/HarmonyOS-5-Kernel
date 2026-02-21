/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface for fdtable ops
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 6 17:01:19 2023
 */

#include "ux_fdtable.h"
#include "ux_eventpoll.h"
#include <libfdtable/fdtable.h>
#ifdef CONFIG_NETFS_FUSION
#include <intapi/mem/pool_page.h>
#endif

#ifdef CONFIG_NETFS_FUSION
#define RETRY_COUNTS	5
#define try_fdtable(name, retval, ...) do { \
	bool retry; int count = 0; \
	do { \
		retry = false; (retval) = syscap_FDTable##name(__VA_ARGS__); \
		if (unlikely((retval) == E_HM_NOMEM)) { \
			(retval) = hm_pool_page_alloc_and_insert(); \
			retry = ((retval) == E_HM_OK); count++; \
			if (count > RETRY_COUNTS) { \
				net_warn(HMNET_CORE, "retry alloc fdtable count %d\n", count); \
			} \
		} \
	} while (retry); \
} while (0)
#endif

static inline void reclaim_fdtable_page(uint64_t reclaim_pfn)
{
#ifdef CONFIG_NETFS_FUSION
	if (reclaim_pfn != 0ULL) {
		mem_handle_pool_page_free(reclaim_pfn);
	}
#else
	BUG_ON(reclaim_pfn != 0ULL);
#endif
}

int ux_fdtable_acquire_rref(uint32_t cnode, rref_t *res)
{
	int err;

	err = hm_fdtable_require_grant(cnode, res);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "acqure fdtable rref failed, err = %d(%s)\n",
			  err, hmstrerror(err));
	}

	return err;
}

int ux_fdtable_set_sock_owner(ux_sock_owner_t *owner)
{
	int err;

	if (owner == NULL) {
		return E_HM_INVAL;
	}
	/*
	 * fd_type:
	 * FD_NET		normal netfd
	 * FD_SOCKFS	usually refers to fusion into vfs, i.e. unix socket
	 */
	err = syscap_FDTableSetClientData(owner->fdtable_rref, (void *)owner, UX_FD_TYPE_OF);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "fdtable set client data failed, err=%s\n", hmstrerror(err));
	}

	return err;
}

static int __fdtable_alloc_fd(rref_t fdtable_rref, int type, unsigned int flags)
{
	int ret;
	struct sysarg_fdtable_fd_config config = {0};

	config.start = 0;
	config.fd_type = (unsigned int)type;
	config.flags = ((flags & SOCK_CLOEXEC) == SOCK_CLOEXEC) ? FDTABLE_FD_FLAG_CLOEXEC : 0;

	BUG_ON(IS_REF_ERR(fdtable_rref));

#ifdef CONFIG_NETFS_FUSION
	try_fdtable(AllocFd, ret, fdtable_rref, &config);
#else
	ret = syscap_FDTableAllocFd(fdtable_rref, &config);
#endif
	if (ret < 0) {
		net_error(HMNET_CORE, "alloc fd failed, ret = %d(%s)\n",
			  ret, hmstrerror(ret));
	}
	BUG_ON(ret == E_HM_EXIST);

	return ret;
}

int ux_fdtable_alloc_fd(rref_t fdtable_rref, unsigned int flags)
{
	int type = UX_FD_TYPE_OF;
	return __fdtable_alloc_fd(fdtable_rref, type, flags);
}

int ux_fdtable_alloc_net_fd(rref_t fdtable_rref, unsigned int flags)
{
	return __fdtable_alloc_fd(fdtable_rref, FD_NET, flags);
}

int ux_fdtable_alloc_fs_fd(rref_t fdtable_rref, unsigned int flags)
{
	return __fdtable_alloc_fd(fdtable_rref, FD_FS, flags);
}
/*
 * Install fd in kernel fdtable, the server also passed,
 * so can route to the specified server when dispatch
 */
int ux_fdtable_install_fd(rref_t fdtable_rref, unsigned int fd, ux_socket_t *sock)
{
	int err;
	struct sysarg_fdtable_fd_install config = {0};

	config.fd = fd;
	config.remote_fd = -1;
	config.fd_type = UX_FD_TYPE_OF;
	config.server = __LSYSCALL_SVR_NETWORK;
	config.vfs_id = 0;
	config.file_in_kobj = (void *)sock;

#ifdef CONFIG_NETFS_FUSION
	config.server = __LSYSCALL_SVR_ROOTFS;
#endif

	ux_set_fd_config(sock, &config);

	BUG_ON(IS_REF_ERR(fdtable_rref));
	err = syscap_FDTableInstallFd(fdtable_rref, &config);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "install fd failed, err = %d(%s)\n",
			  err, hmstrerror(err));
	}

	return err;
}

int ux_fdtable_alloc_install_fd(rref_t fdtable_rref, ux_socket_t *sock, unsigned int flags)
{
	int ret;
	struct sysarg_fdtable_fd_install config = {0};

	config.start = 0;
	config.flags = ((flags & SOCK_CLOEXEC) == SOCK_CLOEXEC) ? FDTABLE_FD_FLAG_CLOEXEC : 0;
	config.remote_fd = -1;
	config.fd_type = UX_FD_TYPE_OF;
	config.vfs_id = 0;
	config.file_in_kobj = (void *)sock;

#ifdef CONFIG_NETFS_FUSION
	config.server = __LSYSCALL_SVR_ROOTFS;
#else
	config.server = __LSYSCALL_SVR_NETWORK;
#endif

	ux_set_fd_config(sock, &config);

	BUG_ON(IS_REF_ERR(fdtable_rref));

#ifdef CONFIG_NETFS_FUSION
	try_fdtable(AllocAndInstallFd, ret, fdtable_rref, &config);
#else
	ret = syscap_FDTableAllocAndInstallFd(fdtable_rref, &config);
#endif
	if (ret < 0) {
		net_error(HMNET_CORE, "alloc and install fd failed, ret = %d(%s)\n", ret, hmstrerror(ret));
	}

	return ret;
}

void ux_fdtable_free_fd(rref_t fdtable_rref, unsigned int fd)
{
	int err;
	__u64 reclaim_pfn = 0ULL;

	BUG_ON(IS_REF_ERR(fdtable_rref));

	err = syscap_FDTableFreeFd(fdtable_rref, fd, UX_FD_TYPE_OF, &reclaim_pfn);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "free fd %u failed, err = %d(%s)\n",
			  fd, err, hmstrerror(err));
	} else {
		reclaim_fdtable_page((uint64_t)reclaim_pfn);
	}
}

/*
 * Release fd resource and get socket pointer as output args from kernel
 */
int ux_fdtable_close_fd(rref_t fdtable_rref, unsigned int fd,
			ux_socket_t **sock, unsigned int *rpccnt)
{
	int err;
	struct sysarg_fdtable_fd_stat stat = {0};

	BUG_ON(IS_REF_ERR(fdtable_rref));

	err = syscap_FDTableCloseFd(fdtable_rref, fd, UX_FD_TYPE_OF, &stat);
	if (err < 0) {
		net_error(HMNET_CORE, "close fd %u failed, err = %d(%s)\n",
			  fd, err, hmstrerror(err));
	} else {
		reclaim_fdtable_page((uint64_t)stat.pfn);

		if (sock != NULL) {
			*sock = (ux_socket_t *)stat.common_file;
		}

		if (rpccnt != NULL) {
			*rpccnt = (unsigned int)err;
		}

		err = E_HM_OK;
	}

	return err;
}

int ux_fdtable_dup_fd(rref_t fdtable_rref, struct sysarg_fdtable_fd_dup_req *dup_req)
{
	int ret;
	struct sysarg_fdtable_fd_dup_res dup_res = {-1};

	if (dup_req == NULL) {
		return E_HM_INVAL;
	}

	BUG_ON(IS_REF_ERR(fdtable_rref));
	dup_req->fd_type = (unsigned)UX_FD_TYPE_OF;
#ifdef CONFIG_NETFS_FUSION
	try_fdtable(DupFd, ret, fdtable_rref, dup_req, &dup_res);
#else
	ret = syscap_FDTableDupFd(fdtable_rref, dup_req, &dup_res);
#endif
	if (ret != E_HM_OK) {
		net_error(HMNET_CORE, "dup old %u, new %u failed, res.new %u, err = %s\n",
			  dup_req->old_fd, dup_req->new_fd, dup_res.new_fd, hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		ret = (int)dup_res.new_fd;
	}

	return ret;
}

/*
 * Mainly used for passing fd across processes,
 * `send_fd-->common_File<--recv_fd`.
 * It creates a copy of the file descriptor send_fd,
 * the send_fd and recv_fd refer to the same common_file.
 * The steps of allocing and binding the recv_fd to common_file
 * are performed atomically.
 */
int ux_fdtable_bind_fd(rref_t fdtable_rref, int type,
		       void *common_file, unsigned int flags)
{
	UNUSED(fdtable_rref, type, common_file, flags);

	return E_HM_OK;
}

/*
 * Get socket pointer from kernel fdtable
 */
ux_socket_t *ux_fdtable_get_fd(rref_t fdtable_rref, unsigned int fd)
{
	int err;
	ux_socket_t *sock = NULL;
	struct sysarg_fdtable_fd_stat stat = {0};

	BUG_ON(IS_REF_ERR(fdtable_rref));

	err = syscap_FDTableGetFd(fdtable_rref, fd, UX_FD_TYPE_OF, &stat);
	if (err == E_HM_OK) {
		if (stat.fd_type != (unsigned)UX_FD_TYPE_OF) {
			/* NOTE: need to release no-net fds here */
		} else {
			sock = (ux_socket_t *)stat.common_file;
		}
	}

	return sock;
}

int ux_fdtable_fork_fdtable(ux_sock_owner_t *src, ux_sock_owner_t *dst)
{
	int err;

	BUG_ON(IS_REF_ERR(src->fdtable_rref));
	BUG_ON(IS_REF_ERR(dst->fdtable_rref));
#ifdef CONFIG_NETFS_FUSION
	try_fdtable(CloneFDTable, err, dst->fdtable_rref,
		    src->fdtable_rref, (int)FD_SOCKFS);
#else
	err = syscap_FDTableCloneFDTable(dst->fdtable_rref, src->fdtable_rref, UX_FD_TYPE_OF);
#endif
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "fork fdable for net failed, err = %d(%s)\n",
			  err, hmstrerror(err));
	}

	return err;
}

/*
 * fd needs to be destroyed in batches when the process exits.
 */
void ux_fdtable_revoke_sockets(rref_t fdtable_rref, uint32_t cnode)
{
	UNUSED(fdtable_rref, cnode);
}
