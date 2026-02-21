/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fops core implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 15:50:54 2023
 */

#include <libhmsrv_sys/hm_timer.h>
#include <libsysif/sysmgr/api.h>
#include <libdevhost/devhost.h>
#include <libstrict/strict.h>
#include <libsysif/utils.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmsync/raw_rwlock.h>
#include <libalgo/idr.h>
#include <libmem/flags.h>
#include <vfs.h>

#include <devhost/memory.h>
#include <devhost/backend.h>

#include <udk/log.h>

#include "device.h"
#include "file.h"
#include "fops_core.h"
#include <udk/vfs.h>

/* for support user apps mmap and munmap in udk fusion mode */
STATIC_DEFINE_RAW_RWLOCK(g_udk_vma_idr_lock);
static DEFINE_IDR(g_udk_vma_idr);

bool udk_fusion_is_selfcall(void)
{
	return (sysif_actv_src_cnode_idx() == hm_ucap_self_cnode_idx());
}

static int __do_device_open(struct filp_node *node, rref_t tgt_rref,
			    unsigned int vfs_flags)
{
	int ret;
	uref_t ap_uref;
	struct device_info *info = node->devinfo;

	if ((ret = info->p->fops->open(info, &node->filp, vfs_flags)) < 0) {
		udk_error("failed to open device, devid=%d, device_name=%s\n",
			  info->devid, info->devname);
		return posix2hmerrno(-ret);
	}

	if (!udk_fusion_is_selfcall()) {
		/* ap_uref auto release */
		if (IS_REF_ERR(ap_uref = libdh_ap_grant(tgt_rref))) {
			ret = REF_TO_ERR(ap_uref);
			goto err_out;
		}
	}
	return ret;

err_out:
	if (info->p->fops->close != NULL) {
		(void)info->p->fops->close(node->filp);
	}
	return ret;
}

static int fast_open_prepare(int devid, int *real_devid)
{
	int ret = E_HM_OK;

	/* only fast open: uapp -> fs/sysmgr/libudk should trans devno to real_devid */
	if (udk_fusion_is_selfcall()) {
		ret = udk_fusion_devid_acquire(devid, real_devid);
	}
	return ret;
}

int udk_fusion_fops_open(unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags,
			 struct filp_node **nodeptr)
{
	int ret;
	struct device_info *info = NULL;
	__u32 cnode_idx = u_cidx;
	int real_devid = devid;

	BUG_ON(nodeptr == NULL);
	/* In fast open, devid is devno, we should trans to real_devid */
	if ((ret = fast_open_prepare(devid, &real_devid)) != 0) {
		return ret;
	}
	if ((ret = udk_fusion_device_acquire(real_devid, &info)) != 0) {
		return ret;
	}
	if ((info->p->fops == NULL) || (info->p->fops->open == NULL)) {
		udk_fusion_devinfo_put(info);
		return E_HM_NOSYS;
	}

	if ((*nodeptr = udk_fusion_filp_node_alloc(cnode_idx, info)) == NULL) {
		udk_fusion_devinfo_put(info);
		return E_HM_NOMEM;
	}
	if ((ret = __do_device_open(*nodeptr, tgt_rref, vfs_flags)) < 0) {
		goto err_out;
	}
	udk_fusion_devinfo_put(info);

	return ret;

err_out:
	/*
	 * twice devinfo get operation in:
	 * 1.udk_fusion_device_acquire
	 * 2.udk_fusion_filp_node_alloc
	 */
	udk_fusion_devinfo_put(info);
	udk_fusion_devinfo_put(info);
	free(*nodeptr);
	return ret;
}

int udk_fusion_fops_close(struct filp_node *node)
{
	if (node == NULL) {
		return -EINVAL;
	}
	devhost_put_file(node);

	return 0;
}

int udk_fusion_fops_read(struct filp_node *node, unsigned long *offset,
			 unsigned long ubuf, unsigned long count, unsigned int flags)
{
	UNUSED(flags);

	int ret;
	struct device_info *info = NULL;

	if (node == NULL || node->devinfo == NULL || node->devinfo->p == NULL) {
		return -EINVAL;
	}

	info = node->devinfo;
	if ((info->p->fops == NULL) || (info->p->fops->read == NULL)) {
		return E_HM_NOSYS;
	}

	ret = info->p->fops->read(node->filp, ulong_to_ptr(ubuf, void), count, *offset);
	*offset += (ret > 0 ? (unsigned long)ret : 0);

	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

int udk_fusion_fops_write(struct filp_node *node, unsigned long *offset,
			  unsigned long ubuf, unsigned long count, unsigned int flags)
{
	UNUSED(flags);

	int ret;
	struct device_info *info = NULL;

	if (node == NULL || node->devinfo == NULL || node->devinfo->p == NULL) {
		return -EINVAL;
	}

	info = node->devinfo;
	if ((info->p->fops == NULL) || (info->p->fops->write == NULL)) {
		return E_HM_NOSYS;
	}

	ret = info->p->fops->write(node->filp, ulong_to_ptr(ubuf, void), count, *offset);
	*offset += (ret > 0 ? (unsigned long)ret : 0);

	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

int udk_fusion_fops_ioctl(struct filp_node *node, unsigned int cmd, unsigned long arg)
{
	int ret;
	struct device_info *info = NULL;

	if (node == NULL || node->devinfo == NULL || node->devinfo->p == NULL) {
		return -EINVAL;
	}

	info = node->devinfo;
	if ((info->p->fops == NULL) || (info->p->fops->ioctl == NULL)) {
		return E_HM_NOSYS;
	}

	ret = info->p->fops->ioctl(node->filp, cmd, arg);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

int udk_fusion_fops_fcntl(struct filp_node *node, int request, unsigned long arg)
{
	int ret;
	struct device_info *info = NULL;

	if (node == NULL || node->devinfo == NULL || node->devinfo->p == NULL) {
		return -EINVAL;
	}

	info = node->devinfo;
	if ((info->p->fops == NULL) || (info->p->fops->fcntl == NULL)) {
		return E_HM_NOSYS;
	}

	ret = info->p->fops->fcntl(node->filp, request, arg);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

long long udk_fusion_fops_llseek(struct filp_node *node, long long offset, int whence)
{
	long long pos;
	struct device_info *info = NULL;

	if (node == NULL || node->devinfo == NULL || node->devinfo->p == NULL) {
		return -EINVAL;
	}

	info = node->devinfo;
	if ((info->p->fops == NULL) || (info->p->fops->llseek == NULL)) {
		return E_HM_NOSYS;
	}

	pos = info->p->fops->llseek(node->filp, offset, whence);
	if (pos < 0) {
		pos = posix2hmerrno(-(int)pos);
	}

	return pos;
}

static struct devhost_vma_info *vma_info_create(const struct vfs_iommap_info *info)
{
	struct devhost_vma_info *vma_info = NULL;
	int vma_id = E_HM_INVAL;

	vma_info = malloc(sizeof(struct devhost_vma_info));
	if (vma_info == NULL) {
		udk_error("devhost vma_info alloc failed\n");
		return NULL;
	}
	mem_zero_s(*vma_info);

	RAW_RWLOCK_WR_GUARD(_, &g_udk_vma_idr_lock);
	vma_id = idr_alloc(&g_udk_vma_idr, vma_info);
	if (vma_id < 0) {
		udk_error("devhost vma_info idr_alloc failed\n");
		free(vma_info);
		return NULL;
	}
	vma_info->vma_id = (unsigned int)vma_id;
	vma_info->start = (unsigned long)info->vaddr;
	vma_info->len = (unsigned long)info->aligned_len;
	vma_info->prot = (unsigned long)info->prot;
	vma_info->flags = info->flags;
	vma_info->offset = (unsigned long)info->offset;

	return vma_info;
}

static void vma_info_release(unsigned int vma_id)
{
	struct devhost_vma_info *vma_info = NULL;

	RAW_RWLOCK_WR_GUARD(_, &g_udk_vma_idr_lock);
	vma_info = idr_remove(&g_udk_vma_idr, vma_id);
	if (vma_info == NULL) {
		udk_warn("remove id-%u failed\n", vma_id);
		return;
	}

	free(vma_info);
}

int udk_fusion_fops_mmap(struct filp_node *node, const struct vfs_iommap_info *iomap_info,
			 struct vfs_iommap_ret *iomap_ret)
{
	int ret;
	struct device_info *info = NULL;
	struct devhost_vma_info *vma_info = NULL;

	BUG_ON(node == NULL);
	info = node->devinfo;

	vma_info = vma_info_create(iomap_info);
	if (vma_info == NULL) {
		return -ENOMEM;
	}

	if ((info->p->ex_fops != NULL && (info->p->ex_fops->mmap_ex != NULL))) {
		ret = info->p->ex_fops->mmap_ex(node->filp, vma_info);
	} else {
		ret = -ENOSYS;
	}
	if (ret < 0 || (vma_info->start != (unsigned long)iomap_info->vaddr)) {
		/* Note:
		 * for some virtual devices (/dev/zero), its mmap process will be
		 * implemented as an anon mmap (will reset the new vma_info->start value),
		 * and the iomem unmapping process is not be triggered.
		 * so it needs to release "struct devhost_vma_info" after the current mmap process.
		 */
		iomap_ret->rvaddr = (uintptr_t)vma_info->start;
		vma_info_release(vma_info->vma_id);
		return ret;
	}
	iomap_ret->rvaddr = (uintptr_t)vma_info->start;
	iomap_ret->iomem_id = vma_info->vma_id;
	return ret;
}

int udk_fusion_fops_munmap(unsigned int vma_id, unsigned long len)
{
	int ret;
	struct devhost_vma_info *vma_info = NULL;

	RAW_RWLOCK_WR_GUARD(_, &g_udk_vma_idr_lock);
	vma_info = (struct devhost_vma_info *)idr_find(&g_udk_vma_idr, vma_id);
	if (vma_info == NULL) {
		udk_error("find id-%u failed\n", vma_id);
		return -EINVAL;
	}

	/*
	 * All must be released at once,
	 * releasing sub-region of the mapping area is unacceptable.
	 */
	if (vma_info->len != len) {
		udk_error("invalid vma_info's len=%#lx, len=%#lx\n", vma_info->len, len);
		return -EINVAL;
	}

	vma_info = idr_remove(&g_udk_vma_idr, vma_id);
	if (vma_info == NULL) {
		udk_warn("remove id-%u failed\n", vma_id);
		return -ENOENT;
	}

	ret = devhost_backend_umap_munmap(vma_info->priv, 0);
	if (ret < 0) {
		udk_error("devhost backend umap munmap failed, ret=%d\n", ret);
	}
	free(vma_info);

	return ret;
}

static int do_loop_rw(struct filp_node *node, unsigned long long *offset,
		      struct iovec *iov, int nr_segs, unsigned int flags, int type)
{
	int i;
	int ret = 0;
	int nr = 0;

	if (iov == NULL) {
		return E_HM_INVAL;
	}

	for (i = 0; i < nr_segs; i++) {
		nr = 0;
		if (type == WRITE) {
			nr = udk_fusion_fops_write(node, (unsigned long *)offset,
				ptr_to_ulong(iov->iov_base), iov->iov_len, flags);
		} else {
			nr = udk_fusion_fops_read(node, (unsigned long *)offset,
				ptr_to_ulong(iov->iov_base), iov->iov_len, flags);
		}
		if (nr < 0) {
			if (!ret) {
				ret = nr;
			}
			break;
		}

		ret += nr;
		if ((unsigned int)nr != iov->iov_len) {
			break;
		}
		iov++;
	}

	return ret;
}

int udk_fusion_fops_read_iter(struct filp_node *node, unsigned long long *offset,
			      struct iovec *iov, int nr_segs, size_t count, unsigned int flags)
{
	int ret;
	struct device_info *info;

	if (node == NULL) {
		return E_HM_INVAL;
	}

	info = node->devinfo;
	if ((info == NULL) || (info->p == NULL) || (info->p->fops == NULL) ||
	    (info->p->fops->read == NULL)) {
		return E_HM_NOSYS;
	}

	if ((info->p->ex_fops != NULL) && (info->p->ex_fops->read_iter != NULL)) {
		ret = info->p->ex_fops->read_iter(node->filp, offset, (void *)iov,
						  nr_segs, count, flags);
		if (ret < 0) {
			ret = posix2hmerrno(-ret);
		}
	} else {
		ret = do_loop_rw(node, offset, iov, nr_segs, flags, READ);
	}

	return ret;
}

int udk_fusion_fops_write_iter(struct filp_node *node, unsigned long long *offset,
			       struct iovec *iov, int nr_segs, size_t count, unsigned int flags)
{
	int ret;
	struct device_info *info;

	if (node == NULL) {
		return E_HM_INVAL;
	}

	info = node->devinfo;
	if ((info == NULL) || (info->p == NULL) || (info->p->fops == NULL) ||
	    (info->p->fops->write == NULL)) {
		return E_HM_NOSYS;
	}

	if ((info->p->ex_fops != NULL) && (info->p->ex_fops->write_iter != NULL)) {
		ret = info->p->ex_fops->write_iter(node->filp, offset, (void *)iov,
						   nr_segs, count, flags);
		if (ret < 0) {
			ret = posix2hmerrno(-ret);
		}
	} else {
		ret = do_loop_rw(node, offset, iov, nr_segs, flags, WRITE);
	}

	return ret;
}
