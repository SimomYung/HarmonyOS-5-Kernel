/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header of fusion dev
 * Author: Huawei OS Kernel Lab
 * Create: Thur Nov 30 10:12:35 2023
 */
#ifndef ULIBS_LIBDEVHOST_FUSION_DEV_H
#define ULIBS_LIBDEVHOST_FUSION_DEV_H

#include <libhmsrv_io/type.h>
#include <libhmsrv_io/dev.h>

struct vfs_iommap_info;
/*
 * Following definitions are the same as libhmsrv_io/dev.h
 *
 * These interfaces are used to transform IPC to selfcall when libdh
 * and libhmsrv_io are linked together.
 *
 * libdh_blk is a stub that returns E_HM_NOSYS as default.
 */
int libdh_dev_open(struct hmsrv_io_ctx *ctx, unsigned int devno, int vfs_mode,
		   unsigned int vfs_flags, unsigned int flags);
int libdh_dev_close(struct hmsrv_io_ctx *ctx);
int libdh_dev_ioctl(const struct hmsrv_io_ctx *ctx,
		    int request, unsigned long arg);
struct __actvret_drvcall_devhost_fops_read;
struct __actvret_drvcall_devhost_fops_write;
int libdh_dev_read(const struct hmsrv_io_ctx *ctx, unsigned long ubuf, unsigned long offset,
		   unsigned long size, unsigned int flag,
		   struct __actvret_drvcall_devhost_fops_read *read_ret);

int libdh_dev_write(const struct hmsrv_io_ctx *ctx, unsigned long ubuf, unsigned long offset,
		   unsigned long size, unsigned int flag,
		   struct __actvret_drvcall_devhost_fops_write *write_ret);

struct __actvret_drvcall_devhost_fops_read_iter;
struct __actvret_drvcall_devhost_fops_write_iter;
int libdh_dev_read_iter(const struct hmsrv_io_ctx *ctx, unsigned long iov, unsigned long long offset,
			int nr_segs, size_t count, unsigned int flags,
			struct __actvret_drvcall_devhost_fops_read_iter *read_ret);

int libdh_dev_write_iter(const struct hmsrv_io_ctx *ctx, unsigned long iov, unsigned long long offset,
			 int nr_segs, size_t count, unsigned int flags,
			 struct __actvret_drvcall_devhost_fops_write_iter *write_ret);

int libdh_dev_llseek(const struct hmsrv_io_ctx *ctx,
			long long offset, int whence,
			long long *pos);
int libdh_dev_poll(const struct hmsrv_io_ctx *ctx,
		      const struct ppoll_info_args *poll_args,
		      int need_add, unsigned int *revents);
int libdh_dev_fcntl(const struct hmsrv_io_ctx *ctx,
		    int request, unsigned long arg);
int libdh_dev_mmap(const struct hmsrv_io_ctx *ctx,
		   const struct vfs_iommap_info *info, struct vfs_iommap_ret *iomap_ret);
int libdh_dev_munmap(unsigned int key, unsigned long len, rref_t sys2dev_rref);
#endif
