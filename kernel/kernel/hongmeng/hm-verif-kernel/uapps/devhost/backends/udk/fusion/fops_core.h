/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fops core headers
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 15:50:54 2023
 */

#ifndef __UDK_FUSION_FOPS_CORE_H__
#define __UDK_FUSION_FOPS_CORE_H__

#include <libhmiov/iov_iter.h>

#define READ 0
#define WRITE 1

struct filp_node;
struct vfs_iommap_info;
struct vfs_iommap_ret;
int udk_fusion_fops_open(unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags,
			 struct filp_node **nodeptr);
int udk_fusion_fops_close(struct filp_node *node);
int udk_fusion_fops_read(struct filp_node *node, unsigned long *offset,
			 unsigned long ubuf, unsigned long count, unsigned int flags);
int udk_fusion_fops_write(struct filp_node *node, unsigned long *offset,
			  unsigned long ubuf, unsigned long count, unsigned int flags);
int udk_fusion_fops_ioctl(struct filp_node *node, unsigned int cmd, unsigned long arg);
int udk_fusion_fops_fcntl(struct filp_node *node, int request, unsigned long arg);
long long udk_fusion_fops_llseek(struct filp_node *node, long long offset, int whence);
int udk_fusion_fops_mmap(struct filp_node *node, const struct vfs_iommap_info *iomap_info,
			 struct vfs_iommap_ret *iomap_ret);
int udk_fusion_fops_munmap(unsigned int vma_id, unsigned long len);
bool udk_fusion_is_selfcall(void);

int udk_fusion_fops_read_iter(struct filp_node *node, unsigned long long *offset,
			      struct iovec *iov, int nr_segs, size_t count, unsigned int flags);
int udk_fusion_fops_write_iter(struct filp_node *node, unsigned long long *offset,
			       struct iovec *iov, int nr_segs, size_t count, unsigned int flags);

#endif
