/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Supply epoll ulib interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 19 15:06:55 2020
 */

#include <vfs_epoll.h>
#include <libhmlog/hmlog.h>

int vfs_epoll_rref_grant(struct server_info *si, int epfd, __u32 cnode_idx, rref_t in_rref, rref_t *out_rref)
{
	int err = E_HM_INVAL;
	if (si != NULL && out_rref != NULL) {
		err = actvx_fscall(vfs_epoll_rref_grant, si, false, true, epfd, cnode_idx, in_rref, out_rref);
	}
	return err;
}

int vfs_epoll_rref_acquire(struct server_info *si, int epfd, __u32 cnode_idx, struct epoll_cap_grant_s *ep)
{
	int err = E_HM_INVAL;
	if (si != NULL && ep != NULL) {
		err = actvx_fscall(vfs_epoll_rref_acquire, si, false, true, epfd, cnode_idx, ep);
	}
	return err;
}
