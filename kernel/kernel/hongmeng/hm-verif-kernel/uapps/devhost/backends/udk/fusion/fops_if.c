/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fops sysif
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 10:03:27 2023
 */
#include <vfs.h>
#include <poll.h>
#include <libstrict/strict.h>
#include <libsysif/devhost/server.h>
#include <libdevhost/devhost.h>
#include <libdevhost/fusion/dev.h>
#include <devhost/memory.h>
#include "fops_core.h"
#include "file.h"
#include "fusion.h"
#include "device.h"
#include <udk/vfs.h>

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_fops_open,
		    unsigned long long, sender, unsigned long, credential, unsigned int, u_cidx,
		    int, devid, rref_t, tgt_rref, unsigned int, vfs_flags)
{
	int ret;
	struct filp_node *node = NULL;
	struct __actvret_drvcall_devhost_fops_open *pret =
		actvhdlr_drvhandler_devhost_fops_open_prepare_ret(sender, credential);

	BUG_ON(pret == NULL);
	ret = udk_fusion_fops_open(u_cidx, devid, tgt_rref, vfs_flags, &node);
	if (ret < 0) {
		return ret;
	}
	pret->filp = ptr_to_u64(node);
	pret->dev_fops_flags = devhost_device_get_fops_flags(node->devinfo, node->filp);

	return E_HM_OK;
}

int libdh_dev_close(struct hmsrv_io_ctx *ctx)
{
	int ret;
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = udk_fusion_fops_close(u64_to_ptr(ctx->filp, struct filp_node));
	if (ret < 0) {
		return ret;
	}

	if (IS_REF_VALID(ctx->dh_ap_rref) && (ctx->flags & HMSRV_IO_CTX_FLAGS_CLOSE_NOREJECT) == 0) {
		(void)hm_ucap_reject(0, ctx->dh_ap_rref);
	}

	ctx->filp = ~0ULL;
	ctx->dh_ap_rref = ERR_TO_REF(E_HM_INVAL);
	return ret;
}

int libdh_dev_read(const struct hmsrv_io_ctx *ctx, unsigned long ubuf, unsigned long offset,
		   unsigned long size, unsigned int flag,
		   struct __actvret_drvcall_devhost_fops_read *pret)
{
	int ret = 0;

	if ((ctx == NULL) || (pret == NULL)) {
		return E_HM_INVAL;
	}
	if ((ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0) {
		ret = udk_fusion_fops_read(u64_to_ptr(ctx->filp, struct filp_node), &offset, ubuf, size, flag);
	} else {
		SYSIF_DIRECTCALL_GUARD();
		/* don't move it outside of GUARD */
		ret = udk_fusion_fops_read(u64_to_ptr(ctx->filp, struct filp_node), &offset, ubuf, size, flag);
	}
	if (ret < 0) {
		return ret;
	}
	pret->nread = (unsigned long long)(unsigned int)ret;
	pret->pos = (unsigned long long)offset;

	return E_HM_OK;
}

int libdh_dev_write(const struct hmsrv_io_ctx *ctx, unsigned long ubuf, unsigned long offset,
		    unsigned long size, unsigned int flag,
		    struct __actvret_drvcall_devhost_fops_write *pret)
{
	int ret;

	if ((ctx == NULL) || (pret == NULL)) {
		return E_HM_INVAL;
	}

	if ((ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0) {
		ret = udk_fusion_fops_write(u64_to_ptr(ctx->filp, struct filp_node),
					    &offset, ubuf, size, flag);
	} else {
		SYSIF_DIRECTCALL_GUARD();
		/* don't move it outside of GUARD */
		ret = udk_fusion_fops_write(u64_to_ptr(ctx->filp, struct filp_node),
					    &offset, ubuf, size, flag);
	}
	if (ret < 0) {
		return ret;
	}
	pret->nwrite = (unsigned long long)(unsigned int)ret;
	pret->pos = (unsigned long long)offset;

	return E_HM_OK;
}

int libdh_dev_ioctl(const struct hmsrv_io_ctx *ctx,
		    int request, unsigned long arg)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	if ((ctx->flags & HMSRV_IO_CTX_FLAGS_IOCTL_FORWARD) != 0) {
		return udk_fusion_fops_ioctl(u64_to_ptr(ctx->filp, struct filp_node),
					     (unsigned int)request, arg);
	} else {
		SYSIF_DIRECTCALL_GUARD();
		/* don't move it outside of GUARD */
		return udk_fusion_fops_ioctl(u64_to_ptr(ctx->filp, struct filp_node),
					     (unsigned int)request, arg);
	}
}

int libdh_dev_llseek(const struct hmsrv_io_ctx *ctx,
		     long long offset, int whence, long long *pos)
{
	long long ret;

	if (ctx == NULL || pos == NULL) {
		return E_HM_INVAL;
	}

	ret = udk_fusion_fops_llseek(u64_to_ptr(ctx->filp, struct filp_node), offset, whence);
	if (ret < 0) {
		return (int)ret;
	}
	*pos = ret;

	return 0;
}

int libdh_dev_poll(const struct hmsrv_io_ctx *ctx,
		   const struct ppoll_info_args *poll_args,
		   int need_add, unsigned int *revents)
{
	int ret = 0;
	UNUSED(ctx, poll_args, need_add);

	if (ctx == NULL || poll_args == NULL || revents == NULL) {
		return E_HM_INVAL;
	}
	*revents = POLLIN | POLLRDNORM;

	return ret;
}

int libdh_dev_epoll_ctl(struct hmsrv_io_ctx *ctx, struct epoll_info_arg *arg)
{
	int ret = 0;
	unsigned int revents;
	const struct __epoll_ctl_info_s *ctl_info = &arg->ctl_info;
	const struct epoll_cap_info_s *cap_info = &arg->cap_info;
	struct filp_node *node = NULL;
	node = u64_to_ptr(ctx->filp, struct filp_node);
	struct device_info *info = node->devinfo;
	/* for the sake of filtering out random/urandom dev to execute following epoll operation.
	 * In libdh, we can't get what's the type of dev, so just execute the poll interface.
	 * If return >= 0, it means random/urandom dev because only them implement poll.
	 */
	if (info->p != NULL && info->p->ex_fops != NULL && info->p->ex_fops->poll != NULL) {
		ret = info->p->ex_fops->poll(node->filp, NULL, &revents);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
	} else {
		return E_HM_NOSYS;
	}

	filp_epoll_list_lock(node);
	ret = eventpoll_ctl_common(&(node->ep_list.node), arg, revents, NULL);
	filp_epoll_list_unlock(node);
	if (ret != E_HM_OK) {
		return ret;
	}

	hm_debug("args: ref = %llu, fd = %d, events = %u, key = %llu, tock = %llu\n",
		 arg->cap_info.cap.rref, arg->ctl_info.fd, revents,
		 arg->ctl_info.key, arg->cap_info.cap.tock);
	/* Wakeup random/urandom dev immediately because we don't support self define wakeup right now. */
	if (ctl_info->op == EPOLL_CTL_ADD) {
		ret = syscap_EpollWakeup(cap_info->cap.rref, ctl_info->fd, revents,
					 ctl_info->key, cap_info->cap.tock);
		if (ret != E_HM_OK) {
			hm_warn("libdh_dev_epoll_ctl failed, err = %s\n", hmstrerror(ret));
		}
	}

	return ret;
}

int libdh_dev_fcntl(const struct hmsrv_io_ctx *ctx,
		    int request, unsigned long arg)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	return udk_fusion_fops_fcntl(u64_to_ptr(ctx->filp, struct filp_node), request, arg);
}

int libdh_io_dev_file_get(const struct hmsrv_io_ctx *ctx)
{
	struct filp_node *node = NULL;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	node = u64_to_ptr(ctx->filp, struct filp_node);
	(void)devhost_get_file(node);
	return 0;
}

int libdh_dev_mmap(const struct hmsrv_io_ctx *ctx,
		   const struct vfs_iommap_info *info, struct vfs_iommap_ret *iomap_ret)
{
	int ret = E_HM_OK;
	struct filp_node *node = NULL;

	if (ctx == NULL || info == NULL || iomap_ret  == NULL) {
		return E_HM_INVAL;
	}
	node = u64_to_ptr(ctx->filp, struct filp_node);

	ret = udk_fusion_fops_mmap(node, info, iomap_ret);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}
	iomap_ret->cnode_idx = ctx->dh_cnode_idx;

	return ret;
}

int libdh_dev_munmap(unsigned int key, unsigned long len, rref_t sys2dev_rref)
{
	int ret = E_HM_OK;

	if (sys2dev_rref != UDK_FUSION_MAGIC_RREF) {
		return E_HM_NODEV;
	}

	SYSIF_DIRECTCALL_GUARD();
	ret = udk_fusion_fops_munmap(key, len);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return ret;
}

int libdh_dev_read_iter(const struct hmsrv_io_ctx *ctx, unsigned long iov, unsigned long long offset,
			int nr_segs, size_t count, unsigned int flags,
			struct __actvret_drvcall_devhost_fops_read_iter *pret)
{
	int ret = 0;
	struct filp_node *node = NULL;

	if ((ctx == NULL) || (pret == NULL)) {
		return E_HM_INVAL;
	}

	if (count == 0) {
		return E_HM_OK;
	}

	node = u64_to_ptr(ctx->filp, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	if ((ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0) {
		ret = udk_fusion_fops_read_iter(node, &offset,
			(struct iovec *)ulong_to_ptr(iov, void), nr_segs, count, flags);
	} else {
		SYSIF_DIRECTCALL_GUARD();
		/* don't move it outside of GUARD */
		ret = udk_fusion_fops_read_iter(node, &offset,
			(struct iovec *)ulong_to_ptr(iov, void), nr_segs, count, flags);
	}
	if (ret < 0) {
		return ret;
	}

	pret->pos = offset;
	pret->nread = (unsigned long long)(unsigned int)ret;

	return E_HM_OK;
}

int libdh_dev_write_iter(const struct hmsrv_io_ctx *ctx, unsigned long iov, unsigned long long offset,
			 int nr_segs, size_t count, unsigned int flags,
			 struct __actvret_drvcall_devhost_fops_write_iter *pret)
{
	int ret = 0;
	struct filp_node *node = NULL;

	if ((ctx == NULL) || (pret == NULL)) {
		return E_HM_INVAL;
	}

	if (count == 0) {
		return E_HM_OK;
	}

	node = u64_to_ptr(ctx->filp, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	if ((ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0) {
		ret = udk_fusion_fops_write_iter(node, &offset,
			(struct iovec *)ulong_to_ptr(iov, void), nr_segs, count, flags);
	} else {
		SYSIF_DIRECTCALL_GUARD();
		/* don't move it outside of GUARD */
		ret = udk_fusion_fops_write_iter(node, &offset,
			(struct iovec *)ulong_to_ptr(iov, void), nr_segs, count, flags);
	}
	if (ret < 0) {
		return ret;
	}

	pret->pos = offset;
	pret->nwrite = (unsigned long long)(unsigned int)ret;

	return E_HM_OK;
}
