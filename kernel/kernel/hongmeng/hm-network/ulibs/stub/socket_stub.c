/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Stub interface for network socket
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 11 10:15:31 2022
 */

#include "hmnet/hm_socket.h"

#include <errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_select_common.h>

#ifndef __ACTV_HIGHTABLE_START
#define __ACTV_HIGHTABLE_START 128
#endif

#define NORIFY_FORK_CALL_NUM 2

void net_revoke_process(cref_t rref, uint32_t cnode_idx)
{
	int ret;
	unsigned long long rpc_info;

	rpc_info = __RPC_INFO_ENCODE(__ACTV_HIGHTABLE_START + 1, sizeof(uint32_t), 0);

	do {
		ret = syscap_ActivationPoolSendBufCall(rref, rpc_info, &cnode_idx);
	} while (ret == E_HM_ACTIVATION_RPC_RETRY);
}

int net_notify_fork(cref_t ch, uint32_t src_cidx, uint32_t dst_cidx)
{
	int ret;
	/* 32 is uint32_t bit */
	uint64_t cnode_idx = (((uint64_t)src_cidx << 32) | dst_cidx);
	unsigned long long rpc_info =
		__RPC_INFO_ENCODE(__ACTV_HIGHTABLE_START + NORIFY_FORK_CALL_NUM, sizeof(uint64_t), 0);

	do {
		ret = syscap_ActivationPoolSendBufCall(ch, rpc_info, &cnode_idx);
	} while (ret == E_HM_ACTIVATION_RPC_RETRY);

	return ret;
}

int hm_net_close(int sockfd)
{
	UNUSED(sockfd);

	return E_HM_NOSYS;
}

int hm_net_bind(int sockfd, const struct sockaddr *addr, socklen_t len)
{
	UNUSED(sockfd, addr, len);

	return E_HM_NOSYS;
}

int hm_net_listen(int sockfd, int backlog)
{
	UNUSED(sockfd, backlog);

	return E_HM_NOSYS;
}

int hm_net_dev_ioctl(int request, void *args)
{
	UNUSED(request, args);

	return E_HM_NOSYS;
}

int hm_net_fcntl(int sockfd, int cmd, int val)
{
	UNUSED(sockfd, cmd, val);

	return E_HM_NOSYS;
}

int hm_net_dup(int oldfd)
{
	UNUSED(oldfd);

	return E_HM_NOSYS;
}

int hm_net_getifaddrs(struct ifaddrs **ifap)
{
	UNUSED(ifap);

	return E_HM_NOSYS;
}

int hm_net_put_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx)
{
	UNUSED(send_fd, send_cidx, recv_cidx);
	return E_HM_NOSYS;
}

int hm_net_mmap(int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t len,
		unsigned int prot, unsigned int flags, long long offset, uintptr_t *rvaddr)
{
	UNUSED(fd, vs_cnode_idx, vaddr, len, prot, flags, offset, rvaddr);
	return E_HM_BADF;
}

#include "hmnet/hmnet_config.h"
int hm_net_cfg_log(const char *module, const char *arg)
{
	UNUSED(module);
	UNUSED(arg);
	return E_HM_NOSYS;
}

int hm_net_show_stats(const char *protocol)
{
	UNUSED(protocol);
	return E_HM_NOSYS;
}

int hm_net_read_fds(uint32_t cnode_idx, int *fdcnt, int *fds, unsigned int max_files)
{
	UNUSED(cnode_idx, fdcnt, fds, max_files);
	return E_HM_OK;
}

int hm_net_get_fd_path(int fd, char *path_buf, unsigned int path_buf_len, uint32_t cnode_idx)
{
	UNUSED(fd, path_buf, path_buf_len, cnode_idx);
	return E_HM_BADF;
}

int hm_net_fork_notify(uint32_t src_cidx, uint32_t dst_cidx)
{
	UNUSED(src_cidx, dst_cidx);
	return E_HM_OK;
}

uint32_t hm_net_recheck_epoll_revents(struct __epoll_fetch_info_s *info)
{
	UNUSED(info);
	return 0U;
}

ssize_t hm_net_sys_write(uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	UNUSED(cnode, fd, buf, len, flags);
	return E_HM_NOSYS;
}

ssize_t hm_net_sys_read(uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	UNUSED(cnode, fd, buf, len, flags);
	return E_HM_NOSYS;
}

int hm_net_sys_ioctl(uint32_t cnode, int fd, int request, unsigned long arg)
{
	UNUSED(cnode, fd, request, arg);
	return E_HM_NOSYS;
}

#include "hmnet/hmnet_ldk.h"
int hm_net_ldk_domain_register(int family, unsigned long long dh_ap_cref)
{
	UNUSED(family, dh_ap_cref);
	return E_HM_NOSYS;
}

#include "hmnet/hmnet_procfs.h"
int hmnet_procfs_create_ldk_entry(unsigned int nsid, const char* path,
				struct hmnet_procfs_ldk_entry_ops *ops,
				void *ctx, unsigned int mode)
{
	UNUSED(nsid, path, ops, ctx, mode);
	return -hmerrno2posix(E_HM_NOSYS);
}

int hmnet_procfs_create_ldk_entries(void* parent, unsigned int size,
			struct hmnet_procfs_ldk_entry_ops *ops)
{
	UNUSED(parent, size, ops);
	return -hmerrno2posix(E_HM_NOSYS);
}

int hmnet_procfs_delete_ldk_entry_specific(const char *pathname, unsigned int nsid)
{
	UNUSED(pathname, nsid);
	return -hmerrno2posix(E_HM_NOSYS);
}
