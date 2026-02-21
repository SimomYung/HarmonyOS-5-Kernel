/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation for socket
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#include <libsysif_net/api.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_select_common.h>

#include <sys/un.h>
#include <sys/uio.h>
#include <fd.h>

#include "hm_socket.h"
#include "ss_shmem.h"
#include "ss_socket.h"
#include "ss_channel.h"
#include "ux_event.h"
#include "ux_sock_filter.h"

struct ux_socket;
int hm_net_put_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx)
{
	rref_t rref;
	int ret;
	ux_binder_args_t info = {0};

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	info.send_fd = send_fd;
	info.send_cidx = send_cidx;
	info.recv_cidx = recv_cidx;
	info.is_scm = false;

	ret = actvcapcall_netcall_socket_put_send_fd(rref,
						     (uintptr_t)&info);
	if (ret < 0) {
		hm_debug("put send_fd %d send_cidx %u recv_cidx %u failed:%s\n",
			 send_fd, send_cidx, recv_cidx, hmstrerror(ret));
	}

	hm_net_put_rref(&rref);
	return ret;
}

int hm_net_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx, int *recv_fd)
{
	ux_binder_args_t info;
	rref_t rref;
	int ret;
	struct __actvret_netcall_socket_send_fd netcall_ret = {0};

	NOFAIL(memset_s(&info, sizeof(info), 0, sizeof(info)));

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	info.send_fd = send_fd;
	info.send_cidx = send_cidx;
	info.recv_cidx = recv_cidx;
	info.fstype = SCM_FS_NET;
	info.is_scm = false;

	ret = actvcapcall_netcall_socket_send_fd(rref,
						 (unsigned long)&info, &netcall_ret);
	if (ret == E_HM_OK && recv_fd != NULL) {
		*recv_fd = netcall_ret.recv_fd;
	}

	hm_net_put_rref(&rref);
	return ret;
}

int hm_net_close(int sockfd)
{
	return lsyscall_socket_close(sockfd, sockfd);
}

int hm_net_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return lsyscall_socket_bind(sockfd, addr, addrlen);
}

int hm_net_listen(int sockfd, int backlog)
{
	return lsyscall_socket_listen(sockfd, backlog);
}

int hm_net_read_fds(uint32_t cnode_idx, int *fdcnt, int *fds, unsigned int max_files)
{
	unsigned long long *fd_map = NULL;
	unsigned int j = 0;
	rref_t rref;
	int rc;

	if (fdcnt == NULL || fds == NULL) {
		return E_HM_INVAL;
	}

	if (max_files == 0) {
		return E_HM_INVAL;
	}

	fd_map = calloc(BITS_TO_LONGLONGS(max_files), sizeof(unsigned long long));
	if (fd_map == NULL) {
		return E_HM_NOMEM;
	}

	rc = hm_net_ap_rref(&rref);
	if (rc < 0) {
		goto out_fd_map;
	}
	hm_actv_set_accessible_mem_range(ptr_to_ulong(fd_map),
					 BITS_TO_LONGLONGS(max_files) * sizeof(*fd_map));
	rc = actvcapcall_netcall_procfs_fds_read(rref, cnode_idx, (uintptr_t)fd_map, max_files);
	if (rc != E_HM_OK) {
		hm_debug("get process fds failed, rc=%s\n", hmstrerror(rc));
		goto out;
	}

	for (j = 0; j < max_files && (unsigned int)*fdcnt < max_files; j++) {
		if (raw_bitmap_test_bit(fd_map, max_files, j) != 0) {
			fds[(*fdcnt)++] = (int)j;
		}
	}

	rc = E_HM_OK;

out:
	hm_net_put_rref(&rref);
out_fd_map:
	free(fd_map);
	return rc;
}

int hm_net_get_fd_path(int fd, char *path_buf, unsigned int path_buf_len, uint32_t cnode_idx)
{
	rref_t rref;
	int rc;

	if (path_buf == NULL || path_buf_len == 0) {
		return E_HM_INVAL;
	}

	rc = hm_net_ap_rref(&rref);
	if (rc < 0) {
		return rc;
	}

	hm_actv_set_accessible_mem_range(ptr_to_ulong(path_buf), (unsigned long)path_buf_len);
	rc = actvcapcall_netcall_procfs_get_fd_path(rref, cnode_idx, fd,
						    (uintptr_t)path_buf, path_buf_len);
	if (rc != E_HM_OK) {
		hm_debug("get fd path failed, rc=%s\n", hmstrerror(rc));
		goto out;
	}

	rc = E_HM_OK;

out:
	hm_net_put_rref(&rref);
	return rc;
}

int hm_net_fork_notify(uint32_t src_cidx, uint32_t dst_cidx)
{
	rref_t rref;
	int rc;

	rc = hm_net_ap_rref_noput(&rref);
	if (rc < 0) {
		return E_HM_OK;
	}

	rc = net_notify_fork(rref, src_cidx, dst_cidx);
	if (rc != E_HM_OK) {
		hm_info("net_notify_fork ret = %s\n", hmstrerror(rc));
	}

	return rc;
}

uint32_t hm_net_recheck_epoll_revents(struct __epoll_fetch_info_s *info)
{
	uint32_t revents = 0U;
	rref_t rref;

	if (info != NULL && !hm_net_ap_rref(&rref)) {
		revents = (uint32_t)actvcapcall_netcall_epoll_recheck_revents(rref, info);
		hm_net_put_rref(&rref);
	}
	return revents;
}

int hm_net_proc_unauthoried_files(int sel_null_fd)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_proc_unauthoried_files(true,
								true, rref, sel_null_fd);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_net_query_file_id(int fd, uint32_t app_cnode_idx, unsigned long long *file_idx)
{
	rref_t rref;
	int ret;

	if (file_idx == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}
	ret = actvcapcall_netcall_socket_query_info(rref, fd, app_cnode_idx, file_idx);
	if (ret != E_HM_OK) {
		hm_error("query net info failed, cnode_idx:%u, fd:%d, %s\n",
			 app_cnode_idx, fd, hmstrerror(ret));
	}

	hm_net_put_rref(&rref);
	return ret;
}

ssize_t hm_net_sys_write(uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_socket_sys_write(rref,
						    cnode, fd, buf, len, flags);

	hm_net_put_rref(&rref);
	return ret;
}

ssize_t hm_net_sys_read(uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_socket_sys_read(rref,
						   cnode, fd, buf, len, flags);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_net_sys_ioctl(uint32_t cnode, int fd, int request, unsigned long arg)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_sys_ioctl(true, true, rref,
				   cnode, fd, request, arg);

	return ret;
}

int hm_net_sys_dev_ioctl(int request, void *args)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_dev_ioctl(true, true, rref, request, args);

	return ret;
}

int hm_net_dev_ioctl(int request, void *args)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_dev_ioctl(false, true, rref, request, args);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_net_mmap(int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t len,
		unsigned int prot, unsigned int flags, long long offset, uintptr_t *rvaddr)
{
	rref_t rref;
	int ret;
	struct __actvret_netcall_socket_mmap netcall_ret = {0};

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_mmap(true, true, rref, fd, vs_cnode_idx, vaddr,
						len, prot, flags, offset, &netcall_ret);
	if (ret == E_HM_OK) {
		*rvaddr = netcall_ret.rvaddr;
	}
	hm_net_put_rref(&rref);
	return ret;
}

int hm_net_sys_socket_attach(struct ux_socket *sock, unsigned int cnode, int fd)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_attach(false, true, rref, sock, cnode, fd);

	return ret;
}

void *hm_net_sys_socket_get(unsigned int cnode, int fd)
{
	rref_t rref;
	int ret;
	struct __actvret_netcall_socket_get sock_ret = {0};

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return NULL;
	}

	ret = actvxactcapcall_netcall_socket_get(false, true, rref,
						 cnode, fd, &sock_ret);
	if (ret < 0) {
		return NULL;
	}

	return sock_ret.sock;
}

int hm_net_sys_socket_put(struct ux_socket *sock)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvxactcapcall_netcall_socket_put(false, true, rref, sock);

	return ret;
}
