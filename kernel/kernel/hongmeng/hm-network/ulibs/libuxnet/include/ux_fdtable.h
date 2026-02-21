/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:  Header file for fdtalbe declare
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 6 17:01:19 2023
 */

#ifndef __LIBUXNET_UX_FDTABLE__
#define __LIBUXNET_UX_FDTABLE__

#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <ux_socket.h>

#ifdef CONFIG_NETFS_FUSION
#define UX_FD_TYPE_OF		FD_SOCKFS
#else
#define UX_FD_TYPE_OF		FD_NET
#endif

int ux_fdtable_acquire_rref(uint32_t cnode, rref_t *res);
int ux_fdtable_set_sock_owner(ux_sock_owner_t *owner);

int ux_fdtable_alloc_fd(rref_t fdtable_rref, unsigned int flags);
int ux_fdtable_alloc_fs_fd(rref_t fdtable_rref, unsigned int flags);
int ux_fdtable_alloc_net_fd(rref_t fdtable_rref, unsigned int flags);
int ux_fdtable_install_fd(rref_t fdtable_rref,
			  unsigned int fd, ux_socket_t *sk);
int ux_fdtable_alloc_install_fd(rref_t fdtable_rref, ux_socket_t *sock, unsigned int flags);
void ux_fdtable_free_fd(rref_t fdtable_rref, unsigned int fd);
int ux_fdtable_close_fd(rref_t fdtable_rref, unsigned int fd,
			  ux_socket_t **sock, unsigned int *rpccnt);
int ux_fdtable_dup_fd(rref_t fdtable_rref, struct sysarg_fdtable_fd_dup_req *dup_req);
int ux_fdtable_bind_fd(rref_t fdtable_rref,
		       int type, void *common_file, unsigned int flags);

ux_socket_t *ux_fdtable_get_fd(rref_t fdtable_rref, unsigned int fd);

int ux_fdtable_fork_fdtable(ux_sock_owner_t *src, ux_sock_owner_t *dst);
void ux_fdtable_revoke_sockets(rref_t fdtable_rref, uint32_t cnode);

#endif
