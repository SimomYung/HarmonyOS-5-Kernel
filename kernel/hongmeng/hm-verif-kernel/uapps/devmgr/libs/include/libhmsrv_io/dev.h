/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service APIs of device fops
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#ifndef ULIBS_LIBHMSRV_IO_DEV_H
#define ULIBS_LIBHMSRV_IO_DEV_H

#include <stdlib.h>
#include <libhmsrv_io/type.h>
#include <libhmsrv_sys/hm_select_common.h>

struct vfs_iommap_info;
struct vfs_iommap_ret;
/*
 * recvbuf size / iovec size = 2048 / 16  = 128
 */
#define MAX_RESV_IOV_COUNT 128U

int hmsrv_io_dev_open(unsigned int devno, int vfs_mode, unsigned int vfs_flags,
		      unsigned int flags, struct hmsrv_io_ctx *ctx /* out */);

int hmsrv_io_dev_close(struct hmsrv_io_ctx *ctx);

int hmsrv_io_dev_read(const struct hmsrv_io_ctx *ctx,
		      unsigned long *offset_io, const void *buf, unsigned long size,
		      unsigned long *nread_out);

int hmsrv_io_dev_pread(const struct hmsrv_io_ctx *ctx,
		       unsigned long offset, const void *buf, unsigned long size,
		       unsigned long *nread_out);

int hmsrv_io_dev_write(const struct hmsrv_io_ctx *ctx,
		       unsigned long *offset_io,
		       const void *buf, unsigned long size,
		       unsigned long *nwrite_out);

int hmsrv_io_dev_pwrite(const struct hmsrv_io_ctx *ctx,
			unsigned long offset,
			const void *buf, unsigned long size,
			unsigned long *nwrite_out);

long hmsrv_io_dev_read_iter(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
			    const void *iov, int nr_segs, size_t count);

long hmsrv_io_dev_pread_iter(const struct hmsrv_io_ctx *ctx, unsigned long offset,
			     const void *iov, int nr_segs, size_t count);

long hmsrv_io_dev_write_iter(const struct hmsrv_io_ctx *ctx, unsigned long *offset,
			     const void *iov, int nr_segs, size_t count);

long hmsrv_io_dev_pwrite_iter(const struct hmsrv_io_ctx *ctx, unsigned long offset,
			      const void *iov, int nr_segs, size_t count);

int hmsrv_io_dev_llseek(const struct hmsrv_io_ctx *ctx,
			long long offset, int whence,
			long long *pos);

int hmsrv_io_dev_ioctl(const struct hmsrv_io_ctx *ctx,
		       int request, unsigned long arg, size_t size);

int hmsrv_io_dev_fcntl(const struct hmsrv_io_ctx *ctx,
		       int request, unsigned long arg);

int hmsrv_io_dev_flush(const struct hmsrv_io_ctx *ctx);

int hmsrv_io_dev_epoll_close(const struct hmsrv_io_ctx *ctx,
			     __u32 app_cnode_idx, int fd);

int hmsrv_io_dev_epoll_et(const struct hmsrv_io_ctx *ctx, const void *pre_info,
			  uint32_t *revents);

int hmsrv_io_dev_epoll_reinit_add(const struct hmsrv_io_ctx *ctx,
				  __u32 app_cnode_idx, int fd,
				  const void *pre_reg_info,
				  const void *reg_info);

int hmsrv_io_dev_poll(const struct hmsrv_io_ctx *ctx,
		      const struct ppoll_info_args *poll_args,
		      int need_add, unsigned int *revents);

int libdh_dev_epoll_ctl(struct hmsrv_io_ctx *ctx, struct epoll_info_arg *arg);

int hmsrv_io_dev_poll_remove(const struct hmsrv_io_ctx *ctx, int fd, cref_t tcb_cref);

int hmsrv_io_dev_epoll_ctl(const struct hmsrv_io_ctx *ctx, const void *ep_arg);
int hmsrv_io_dev_epoll_remove(const struct hmsrv_io_ctx *ctx, cref_t ep_cref);
int hmsrv_io_dev_epoll_recheck(const struct hmsrv_io_ctx *ctx,
			       unsigned int events, unsigned int *revents);

int hmsrv_io_dev_net_poll(const struct hmsrv_io_ctx *ctx,
		      const struct ppoll_info_args *poll_args,
		      int need_add, unsigned int *revents);

int hmsrv_io_dev_net_poll_remove(const struct hmsrv_io_ctx *ctx, int fd, cref_t tcb_cref);

int hmsrv_io_dev_net_epoll_ctl(const struct hmsrv_io_ctx *ctx, const void *ep_arg);

int hmsrv_io_dev_net_epoll_recheck(const struct hmsrv_io_ctx *ctx,
			       unsigned int events, unsigned int *revents);

int hmsrv_io_dev_net_sync_ctrlmem_polllist(const struct hmsrv_io_ctx *ctx);

int hmsrv_io_dev_sync_sec(struct hmsrv_io_ctx *ctx, unsigned int devno, int vfs_mode,
			  char *scon, size_t scon_len, unsigned int task_sid, unsigned int state);

int hmsrv_io_dev_file_get(const struct hmsrv_io_ctx *ctx);
int hmsrv_io_dev_mmap(struct hmsrv_io_ctx *ctx, const struct vfs_iommap_info *info,
		      struct vfs_iommap_ret *iomap_ret);
int hmsrv_io_dev_munmap(unsigned int iomem_id, unsigned long start, unsigned long len,
			rref_t sys2dev_rref, bool vs_is_releasing);
#endif	/* ULIBS_LIBHMSRV_IO_DEV_H */
