/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of transfer fd
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 24 15:30:04 2021
 */

#ifndef LIBHMSRV_FS_TRANSFER_FD_H
#define LIBHMSRV_FS_TRANSFER_FD_H

#include <vfs.h>

#define SFILE_MAX_VALUE VFS_FDTABLESIZE
#define SFILE_MIN_VALUE 0

typedef struct vfs_transfer_fd_args {
	uint32_t send_cidx;
	uint32_t recv_cidx;
	int send_fd;
	int recv_fd;
	int sfiles_idx;
} vfs_transfer_fd_args_t;

#ifdef CONFIG_FSLIB_TRANSFER_FD
int vfs_put_send_fd(struct server_info *si, const vfs_transfer_fd_args_t *info);
int vfs_send_fd(struct server_info *si, const vfs_transfer_fd_args_t *info);
int vfs_recv_fd(struct server_info *si, const vfs_transfer_fd_args_t *info,
		int *file_type, int *remote_fd);
#else
static inline int vfs_put_send_fd(struct server_info *si, const vfs_transfer_fd_args_t *info)
{
	UNUSED(si);
	UNUSED(info);
	return E_HM_NOSYS;
}
static inline int vfs_send_fd(struct server_info *si, const vfs_transfer_fd_args_t *info)
{
	UNUSED(si);
	UNUSED(info);
	return E_HM_NOSYS;
}
static inline int vfs_recv_fd(struct server_info *si, const vfs_transfer_fd_args_t *info,
			      int *file_type, int *remote_fd)
{
	UNUSED(si);
	UNUSED(info);
	UNUSED(file_type);
	UNUSED(remote_fd);
	return E_HM_NOSYS;
}
#endif

#endif
