/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service library of device fops
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#include <hongmeng/types.h>
#include <libdevhost/fusion/dev.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libhwsecurec/securec.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/api.h>
#include <libstrict/strict.h>
#include <libhmactv/actv.h>

#include <libmem/flags.h>
#include <vfs.h>

#include <libhmsrv_sys/hm_select_common.h>
#include <libhmsrv_io/dev.h>
#include <libdevhost/fusion/dev.h>
#include "internal.h"

static bool is_ctty(int vfs_mode, unsigned int devno)
{
	return vfs_mode == MODE_CHR &&
	       devno == MKDEV(DEV_CTTY_MAJOR, DEV_CTTY_MINOR);
}

static int get_ctty(unsigned int *devno)
{
	struct ttyinfo_s info = {0};
	int ret;

	ret = hm_session_get_ttyinfo(&info, true);
	if (ret < 0) {
		hm_error("failed to get ttyinfo, ret=%s\n", hmstrerror(ret));
		return ret;
	}

	if (info.devno == 0) {
		return E_HM_NXIO;
	}

	if (devno != NULL) {
		*devno = info.devno;
	}

	return ret;
}

int hmsrv_io_dev_open(unsigned int devno, int vfs_mode, unsigned int vfs_flags,
		      unsigned int flags, struct hmsrv_io_ctx *ctx /* out */)
{
	struct __actvret_drvcall_devmgr_fops_open open_ret;
	struct sysarg_cnode_statrref stat;
	rref_t dh_ap_rref;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (is_ctty(vfs_mode, devno)) {
		ret = get_ctty(&devno);
		if (ret < 0) {
			return ret;
		}
	}

	ret = libdh_dev_open(ctx, devno, vfs_mode, vfs_flags, flags);
	/*
	 * if libdh_dev_open return E_HM_NODEV, it should use original path
	 * In libdh_dev_open, some devices (e.g. null/zero/full) will fast open
	 */
	if (ret != E_HM_NODEV) {
		return ret;
	}

	/* require rref for devhost ap */
	dh_ap_rref = hmsrv_io_require();
	if (IS_REF_ERR(dh_ap_rref)) {
		return REF_TO_ERR(dh_ap_rref);
	}

	mem_zero_s(open_ret);
	ret = actvxactcapcall_drvcall_devmgr_fops_open(
			(flags & HMSRV_IO_CTX_FLAGS_OPEN_FORWARD) != 0,
			true,	/* allow refwd */
			g_devmgr_ap_rref,
			devno, vfs_flags, vfs_mode, dh_ap_rref,
			&open_ret);
	if (ret < 0) {
		(void)hm_ucap_reject(0, dh_ap_rref);
		return ret;
	}

	ret = sysfast_cnode_stat_rref(&stat, dh_ap_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret < 0) {
		(void)hm_ucap_reject(0, dh_ap_rref);
		return ret;
	}

	ctx->fastpath = open_ret.fastpath;
	ctx->filp = open_ret.filp;
	ctx->kshm_key = open_ret.kshm_key;
	ctx->kobj_offset = open_ret.kobj_offset;
	ctx->nonvapre = open_ret.nonvapre;
	ctx->dev_fops_flags = open_ret.dev_fops_flags;
	ctx->dh_cnode_idx = stat.src_cnode_idx;
	ctx->dh_ap_rref = dh_ap_rref;
	ctx->flags = flags;

	return 0;
}

int hmsrv_io_dev_close(struct hmsrv_io_ctx *ctx)
{
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_close(ctx);
	}

	if (IS_REF_ERR(ctx->dh_ap_rref) || (ctx->filp == ~0ULL)) {
		hm_warn("hmsrv_io: try to close invalid ctx: dh_ap_rref=0x%"PRIxref", filp=0x%llx\n", ctx->dh_ap_rref, ctx->filp);
		return E_HM_OK;
	}

	ret = actvxactcapcall_drvcall_devhost_fops_close(
			(ctx->flags & HMSRV_IO_CTX_FLAGS_OPEN_FORWARD) != 0,
			true,	/* allow refwd */
			ctx->dh_ap_rref, ctx->fastpath, ctx->filp);
	if (ret < 0) {
		return ret;
	}

	/*
	 * do not reject dh_ap_rref when closing an anonymous fd because all
	 * anonymous files share one single dh_ap_rref stored in vfs
	 */
	if ((ctx->flags & HMSRV_IO_CTX_FLAGS_CLOSE_NOREJECT) != 0) {
		/* do nothing */
	} else {
		ret = hm_ucap_reject(0, ctx->dh_ap_rref);
		if (ret < 0) {
			hm_warn("hmsrv_io: reject dh_ap_rref failed: %s\n", hmstrerror(ret));
		}
	}

	ctx->filp = ~0ULL;
	ctx->dh_ap_rref = ERR_TO_REF(E_HM_INVAL);

	return E_HM_OK;
}

static int do_io_dev_read(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
			  const void *buf, unsigned long size, unsigned int flag,
			  unsigned long *nread_out)
{
	struct __actvret_drvcall_devhost_fops_read read_ret;
	int ret;
	bool is_xact_fwd;

	if (ctx == NULL || buf == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(read_ret);
	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		ret = libdh_dev_read(ctx, ptr_to_ulong(buf), *offset, size, flag, &read_ret);
	} else {
		is_xact_fwd = (ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0;
		if (!is_xact_fwd) {
			hm_actv_set_accessible_mem_range(ptr_to_ulong(buf), size);
		}
		ret = actvxactcapcall_drvcall_devhost_fops_read(
				is_xact_fwd, /* is xact fwd */
				true, /* allow refwd */
				ctx->dh_ap_rref, ctx->fastpath, ptr_to_ulong(buf), ctx->filp,
				(unsigned long long)*offset, size, flag, &read_ret);
	}
	if (ret < 0) {
		return ret;
	}

	if (nread_out != NULL) {
		*nread_out = (unsigned long)read_ret.nread;
	}

	if (flag == 0) {
		*offset = (unsigned long)read_ret.pos;
	}

	return E_HM_OK;
}

int hmsrv_io_dev_read(const struct hmsrv_io_ctx *ctx,
		      unsigned long *offset_io, const void *buf, unsigned long size,
		      unsigned long *nread_out)
{
	if (offset_io == NULL) {
		return E_HM_INVAL;
	}

	return do_io_dev_read(ctx, offset_io, buf, size, 0, nread_out);
}

int hmsrv_io_dev_pread(const struct hmsrv_io_ctx *ctx,
		       unsigned long offset, const void *buf, unsigned long size,
		       unsigned long *nread_out)
{
	return do_io_dev_read(ctx, &offset, buf, size, DEV_PIO, nread_out);
}

static int do_io_dev_write(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
			   const void *buf, unsigned long size, unsigned int flag,
			   unsigned long *nwrite_out)
{
	struct __actvret_drvcall_devhost_fops_write write_ret;
	int ret;

	if (ctx == NULL || buf == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(write_ret);
	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		ret = libdh_dev_write(ctx, ptr_to_ulong(buf), *offset, size, flag, &write_ret);
	} else {
		ret = actvxactcapcall_drvcall_devhost_fops_write(
				(ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0,
				true, /* allow refwd */
				ctx->dh_ap_rref, ctx->fastpath, ptr_to_ulong(buf), ctx->filp,
				(unsigned long long)*offset, size, flag, &write_ret);
	}
	if (ret < 0) {
		return ret;
	}

	if (nwrite_out != NULL) {
		*nwrite_out = (unsigned long)write_ret.nwrite;
	}

	if (flag == 0) {
		*offset = (unsigned long)write_ret.pos;
	}

	return E_HM_OK;
}

int hmsrv_io_dev_write(const struct hmsrv_io_ctx *ctx,
		       unsigned long *offset_io,
		       const void *buf, unsigned long size,
		       unsigned long *nwrite_out)
{
	if (offset_io == NULL) {
		return E_HM_INVAL;
	}

	return do_io_dev_write(ctx, offset_io, buf, size, 0, nwrite_out);
}

int hmsrv_io_dev_pwrite(const struct hmsrv_io_ctx *ctx,
			unsigned long offset,
			const void *buf, unsigned long size,
			unsigned long *nwrite_out)
{
	return do_io_dev_write(ctx, &offset, buf, size, DEV_PIO, nwrite_out);
}

static long do_io_dev_read_iter(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
				const void *iov, int nr_segs, size_t count,
				unsigned int flags)
{
	int ret;
	unsigned long recv_size = ((unsigned int)nr_segs < MAX_RESV_IOV_COUNT) ?
		(sizeof(struct iovec) * (unsigned int)nr_segs) : 0;
	struct __actvret_drvcall_devhost_fops_read_iter read_ret;

	if (ctx == NULL || iov == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(read_ret);
	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		ret = libdh_dev_read_iter(ctx, ptr_to_ulong(iov), *offset, nr_segs,
					  count, flags, &read_ret);
	} else {
		ret = actvxactcapcall_drvcall_devhost_fops_read_iter(
				(ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0,
				true, /* allow refwd */
				ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
				(unsigned long long)*offset,
				iov, nr_segs, count, flags,
				(void *)iov, recv_size,	&read_ret);
	}
	if (ret < 0) {
		return ret;
	}

	if (flags == 0) {
		*offset = (unsigned long)read_ret.pos;
	}

	return (long)read_ret.nread;
}

long hmsrv_io_dev_read_iter(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
			    const void *iov, int nr_segs, size_t count)
{
	if (offset == NULL) {
		return E_HM_INVAL;
	}

	return do_io_dev_read_iter(ctx, offset, iov, nr_segs, count, 0);
}

long hmsrv_io_dev_pread_iter(const struct hmsrv_io_ctx *ctx, unsigned long offset,
			     const void *iov, int nr_segs, size_t count)
{
	return do_io_dev_read_iter(ctx, &offset, iov, nr_segs, count, DEV_PIO);
}

static long do_io_dev_write_iter(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
				 const void *iov, int nr_segs, size_t count,
				 unsigned int flags)
{
	int ret;
	unsigned long recv_size = ((unsigned int)nr_segs < MAX_RESV_IOV_COUNT) ?
		(sizeof(struct iovec) * (unsigned int)nr_segs) : 0;
	struct __actvret_drvcall_devhost_fops_write_iter write_ret;

	if (ctx == NULL || iov == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(write_ret);
	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		ret = libdh_dev_write_iter(ctx, ptr_to_ulong(iov), *offset, nr_segs,
					   count, flags, &write_ret);
	} else {
		ret = actvxactcapcall_drvcall_devhost_fops_write_iter(
				(ctx->flags & HMSRV_IO_CTX_FLAGS_RW_FORWARD) != 0,
				true, /* allow refwd */
				ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
				(unsigned long long)*offset,
				iov, nr_segs, count, flags,
				(void *)iov, recv_size,	&write_ret);
	}
	if (ret < 0) {
		return ret;
	}

	if (flags == 0) {
		*offset = (unsigned long)write_ret.pos;
	}

	return (long)write_ret.nwrite;
}

long hmsrv_io_dev_write_iter(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
			     const void *iov, int nr_segs, size_t count)
{
	if (offset == NULL) {
		return E_HM_INVAL;
	}

	return do_io_dev_write_iter(ctx, offset, iov, nr_segs, count, 0);
}

long hmsrv_io_dev_pwrite_iter(const struct hmsrv_io_ctx *ctx, unsigned long offset,
			      const void *iov, int nr_segs, size_t count)
{
	return do_io_dev_write_iter(ctx, &offset, iov, nr_segs, count, DEV_PIO);
}

int hmsrv_io_dev_llseek(const struct hmsrv_io_ctx *ctx,
			long long offset, int whence,
			long long *pos)
{
	int ret;
	struct __actvret_drvcall_devhost_fops_llseek llseek_ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_llseek(ctx, offset, whence, pos);
	}

	mem_zero_s(llseek_ret);
	ret = actvxactcapcall_drvcall_devhost_fops_llseek(
					true,	/* is fwd */
					true,	/* allow refwd */
					ctx->dh_ap_rref, ctx->fastpath, whence,
					ctx->filp, offset, &llseek_ret);
	if (ret < 0) {
		return ret;
	}

	if (pos != NULL) {
		*pos = llseek_ret.pos;
	}

	return E_HM_OK;
}

int hmsrv_io_dev_ioctl(const struct hmsrv_io_ctx *ctx,
		       int request, unsigned long arg,
		       size_t size /* deprecated */)
{
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	UNUSED(size);

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_ioctl(ctx, request, arg);
	}

	ret = actvxactcapcall_drvcall_devhost_fops_ioctl(
			(ctx->flags & HMSRV_IO_CTX_FLAGS_IOCTL_FORWARD) != 0,
			true,	/* allow refwd */
			ctx->dh_ap_rref, ctx->fastpath, request, ctx->filp, arg);

	return ret;
}

int hmsrv_io_dev_fcntl(const struct hmsrv_io_ctx *ctx,
		       int request, unsigned long arg)
{
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_fcntl(ctx, request, arg);
	}

	ret = actvxactcapcall_drvcall_devhost_fops_fcntl(
			true, true, ctx->dh_ap_rref, ctx->fastpath, request,
			ctx->filp, arg);

	return ret;
}

int hmsrv_io_dev_flush(const struct hmsrv_io_ctx *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return E_HM_NOSYS;
	}

	return actvxactcapcall_drvcall_devhost_fops_flush(
		true, true,
		ctx->dh_ap_rref, ctx->fastpath, ctx->filp);
}

int hmsrv_io_dev_epoll_close(const struct hmsrv_io_ctx *ctx,
			     __u32 app_cnode_idx, int fd)
{
	UNUSED(ctx, app_cnode_idx, fd);
	return E_HM_NOSYS;
}

int hmsrv_io_dev_epoll_et(const struct hmsrv_io_ctx *ctx, const void *pre_info,
			  uint32_t *revents)
{
	UNUSED(ctx, pre_info, revents);
	return E_HM_NOSYS;
}

int hmsrv_io_dev_epoll_reinit_add(const struct hmsrv_io_ctx *ctx,
				  __u32 app_cnode_idx, int fd,
				  const void *pre_reg_info,
				  const void *reg_info)
{
	UNUSED(ctx, app_cnode_idx, fd, pre_reg_info, reg_info);
	return E_HM_NOSYS;
}

int hmsrv_io_dev_poll(const struct hmsrv_io_ctx *ctx,
		      const struct ppoll_info_args *poll_args,
		      int need_add, unsigned int *revents)
{
	struct __actvret_drvcall_devhost_fops_poll poll_ret;
	int ret;

	if (ctx == NULL || poll_args == NULL || revents == NULL) {
		return E_HM_INVAL;
	}
	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_poll(ctx, poll_args, need_add, revents);
	}

	ret = actvxactcapcall_drvcall_devhost_fops_poll(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
			poll_args->tcb_key, poll_args->fdtable_rref, poll_args->events,
			poll_args->idx, poll_args->fd, need_add, &poll_ret);
	if (ret == E_HM_OK) {
		*revents = poll_ret.revents;
	}

	return ret;
}

int hmsrv_io_dev_poll_remove(const struct hmsrv_io_ctx *ctx, int fd, cref_t tcb_cref)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_fops_poll_remove(
			false, true, ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
			fd, tcb_cref);
}

int hmsrv_io_dev_epoll_ctl(const struct hmsrv_io_ctx *ctx, const void *ep_arg)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_epoll_ctl(ctx, ep_arg);
	}
	return actvxactcapcall_drvcall_devhost_fops_epoll_ctl(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
			ep_arg);
}

int hmsrv_io_dev_epoll_remove(const struct hmsrv_io_ctx *ctx, cref_t ep_cref)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_fops_epoll_remove(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
			ep_cref);
}

int hmsrv_io_dev_epoll_recheck(const struct hmsrv_io_ctx *ctx,
				    unsigned int events, unsigned int *revents)
{
	int ret;
	struct __actvret_drvcall_devhost_fops_epoll_recheck epoll_ret;

	if ((ctx == NULL) || (revents == NULL)) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_fops_epoll_recheck(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->filp,
			events, &epoll_ret);
	if (ret == E_HM_OK) {
		*revents = epoll_ret.revents;
	}

	return ret;
}

int hmsrv_io_dev_net_poll(const struct hmsrv_io_ctx *ctx,
		      const struct ppoll_info_args *poll_args,
		      int need_add, unsigned int *revents)
{
	struct __actvret_drvcall_devhost_fops_poll poll_ret;
	int rc;

	if (ctx == NULL || poll_args == NULL || revents == NULL) {
		return E_HM_INVAL;
	}

	rc = actvxactcapcall_drvcall_devhost_fops_poll(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->file_id,
			poll_args->tcb_key, poll_args->fdtable_rref, poll_args->events,
			poll_args->idx, poll_args->fd, need_add, &poll_ret);
	if (rc == E_HM_OK) {
		*revents = poll_ret.revents;
	}

	return rc;
}

int hmsrv_io_dev_net_poll_remove(const struct hmsrv_io_ctx *ctx, int fd, cref_t tcb_cref)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_fops_poll_remove(
			false, true, ctx->dh_ap_rref, ctx->fastpath, ctx->file_id, fd, tcb_cref);
}

int hmsrv_io_dev_net_epoll_ctl(const struct hmsrv_io_ctx *ctx, const void *ep_arg)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_fops_epoll_ctl(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->file_id, ep_arg);
}

int hmsrv_io_dev_net_epoll_recheck(const struct hmsrv_io_ctx *ctx,
				    unsigned int events, unsigned int *revents)
{
	int rc;
	struct __actvret_drvcall_devhost_fops_epoll_recheck epoll_ret;

	if ((ctx == NULL) || (revents == NULL)) {
		return E_HM_INVAL;
	}

	rc = actvxactcapcall_drvcall_devhost_fops_epoll_recheck(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->file_id, events, &epoll_ret);
	if (rc == E_HM_OK) {
		*revents = epoll_ret.revents;
	}

	return rc;
}

int hmsrv_io_dev_net_sync_ctrlmem_polllist(const struct hmsrv_io_ctx *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->file_id);
}

/**
 * This function is only for vfs to sync security fields
 * ctx: if ctx is valid, the func is called when opened, use filp to sync security fields.
 * 	if ctx is NULL, this func is called in setfattr, use devno to find device_info.
*/
int hmsrv_io_dev_sync_sec(struct hmsrv_io_ctx *ctx, unsigned int devno, int vfs_mode,
			  char *scon, size_t scon_len, unsigned int task_sid, unsigned int state)
{
	int ret = E_HM_OK;
#ifdef CONFIG_SEHARMONY
	if (scon == NULL || scon_len == 0) {
		return E_HM_INVAL;
	}

	if (ctx) {
		ret = actvxactcapcall_drvcall_devhost_sec_sync_fastpath_open(true, true, ctx->dh_ap_rref,
									     ctx->filp, task_sid, state,
									     scon, scon_len);
	} else {
		ret = actvxactcapcall_drvcall_devmgr_sec_sync_fastpath_dev(true, true, g_devmgr_ap_rref,
									   devno, vfs_mode, task_sid,
									   state, scon, scon_len);
	}
#else
	UNUSED(ctx, devno, vfs_mode, scon, scon_len, task_sid, state);
#endif
	return ret;
}

int hmsrv_io_dev_file_get(const struct hmsrv_io_ctx *ctx)
{
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_fops_file_get(
			true, true, ctx->dh_ap_rref, ctx->fastpath, ctx->filp);

	return ret;
}

int hmsrv_io_dev_mmap(struct hmsrv_io_ctx *ctx, const struct vfs_iommap_info *info,
		      struct vfs_iommap_ret *iomap_ret)
{
	int ret;
	struct __actvret_drvcall_devhost_fops_mmap dev_mmap_ret = {0};

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_dev_mmap(ctx, info, iomap_ret);
	}

	ret = actvxactcapcall_drvcall_devhost_fops_mmap(true, /* is_fwd */
							true, /* allow refwd */
							ctx->dh_ap_rref,
							ctx->fastpath,
							info->vaddr,
							ctx->filp,
							info->aligned_len,
							info->prot,
							info->flags,
							(unsigned long)info->offset,
							info->iomap_prepare_ahead,
							&dev_mmap_ret);
	if (ret == E_HM_OK) {
		iomap_ret->rvaddr = (uintptr_t)dev_mmap_ret.rvaddr;
		iomap_ret->iomem_id = dev_mmap_ret.iomem_id;
		iomap_ret->cnode_idx = ctx->dh_cnode_idx;
	}

	return ret;
}

int hmsrv_io_dev_munmap(unsigned int iomem_id, unsigned long start, unsigned long len,
			rref_t sys2dev_rref, bool vs_is_releasing)
{
	int ret;

	ret = libdh_dev_munmap(iomem_id, len, sys2dev_rref);
	/*
	 * if libdh_dev_munmap return E_HM_NODEV, it should use original path
	 */
	if (ret != E_HM_NODEV) {
		return ret;
	}

	ret = actvxactcapcall_drvcall_devhost_fops_munmap(false, true,
							 sys2dev_rref,
							 iomem_id, start, len, (int)vs_is_releasing);

	return ret;
}
