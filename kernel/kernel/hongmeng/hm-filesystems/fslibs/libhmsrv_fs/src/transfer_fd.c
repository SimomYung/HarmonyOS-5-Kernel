/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of transfer file descriptor ulib interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 24 15:30:04 2021
 */

#include <fs_transfer_fd.h>
#include <unistd.h>
#include <fcntl.h>
#include <libhmlog/hmlog.h>
#include <fslib_common.h>

static bool sfiles_index_is_valid(const vfs_transfer_fd_args_t *info)
{
	if (info == NULL) {
		return false;
	}
	if (info->sfiles_idx < SFILE_MIN_VALUE || info->sfiles_idx >= SFILE_MAX_VALUE) {
		return false;
	}
	return true;
}

/*
 * vfs_put_send_fd - Releases the sfiles resources whose file descriptor
 * is "in-flight"
 *
 * @si         file descriptor server_info
 * @info->send_cidx  cnode_index of the send fd process
 * @info->recv_cidx  cnode_index of the recvice fd process
 * @info->sfiles_idx the index of the sfiles for the "in-flight" file
 *
 * @return     Error code
 */
int vfs_put_send_fd(struct server_info *si, const vfs_transfer_fd_args_t *info)
{
	int ret = E_HM_INVAL;

	if (sfiles_index_is_valid(info) && si != NULL) {
		ret = actvx_fscall(vfs_sfile_put, si, false, true,
				   info->send_cidx, info->recv_cidx, info->sfiles_idx);
		if (ret < 0) {
			hm_debug("put vfs send fd failed, sfiles_idx:%d, errmsg:%s\n",
				 info->sfiles_idx, hmstrerror(ret));
		}
	}

	return ret;
}

/*
 * vfs_send_fd - Send file descriptor
 *
 * @si              file descriptor server_info
 * @info->send_fd   file descriptor to be sent
 * @info->send_cidx cnode_index of the send fd process
 * @info->recv_cidx cnode_index of the recvice fd process
 *
 * @return    If return >= 0, then indicate the index of the sfiles of the sent file;
 *            Else, Error code
 */
int vfs_send_fd(struct server_info *si, const vfs_transfer_fd_args_t *info)
{
	int ret = E_HM_INVAL;

	if ((si != NULL) && (info != NULL)) {
		ret = actvx_fscall(vfs_send_fd, si, false, true,
				   info->send_fd, info->send_cidx, info->recv_cidx);
		if (ret < 0) {
			hm_debug("vfs send fd failed, send_fd:%d, errmsg:%s\n",
				 info->send_fd, hmstrerror(ret));
		}
	}
	return ret;
}

/*
 * vfs_recv_fd - Receive file descriptor
 *
 * @si               file descriptor server_info
 * @info->sfiles_idx the index of the sfiles to be received
 * @info->send_cidx  cnode_index of the send fd process
 * @info->recv_cidx  cnode_index of the recvice fd process
 * @info->recv_fd    file descriptor of client to be received
 * @file_type        return recv fd file_type
 * @remote_fd        return recv fd remote_fd
 *
 * @return     Error code
 */
int vfs_recv_fd(struct server_info *si, const vfs_transfer_fd_args_t *info,
		int *file_type, int *remote_fd)
{
	int err = E_HM_INVAL;
	struct __actvret_fscall_vfs_recv_fd ret = {0};

	if (si == NULL || file_type == NULL || remote_fd == NULL) {
		return err;
	}
	if (sfiles_index_is_valid(info)) {
		err = actvx_fscall(vfs_recv_fd, si, false, true,
				   info->sfiles_idx, info->send_cidx,
				   info->recv_fd, info->recv_cidx, &ret);
		if (err < 0) {
			hm_debug("vfs receive fd failed, sfiles_idx:%d, recv_fd:%d, errmsg:%s\n",
				 info->send_fd, info->recv_fd, hmstrerror(err));
		} else {
			*file_type = ret.type;
			*remote_fd = err;
			err = E_HM_OK;
		}
	}

	return err;
}
