/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for device file operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 13 16:05:44 2019
 */
#include <libsysif/devhost/server.h>

#include <limits.h>
#include <libmem/flags.h>
#include <devhost/memory.h>
#include <devhost/plugin.h>
#include <devhost/backend.h>
#include <hmkernel/errno.h>
#include <hongmeng/types.h>
#include <libhmsrv_io/dev.h>
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>

#include "types.h"
#include "devhost.h"
#include <devhost/log.h>
#include <devhost/file.h>
#ifdef CONFIG_SEHARMONY
#include <devhost/security_seharmony.h>
#endif

#define READ 0
#define WRITE 1

#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)

static int devhost_fops_read(struct filp_node *node, unsigned long long *offset,
			     unsigned long ubuf, size_t count, unsigned int flags)
{
	int ret;
	bool is_pread = (flags & DEV_PIO);
	struct device_info *devinfo = node->devinfo;

	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((devinfo->p->fops == NULL) ||
	    (devinfo->p->fops->read == NULL && devinfo->p->fops->read_ex == NULL)) {
		return E_HM_NOSYS;
	}

	if (count == 0) {
		return E_HM_OK;
	} else if (count > MAX_RW_COUNT) {
		count =  MAX_RW_COUNT;
	}

	if (devinfo->p->fops->read_ex != NULL) {
		ret = devinfo->p->fops->read_ex(node->filp, ulong_to_ptr(ubuf, void),
						count, offset, is_pread);
	} else {
		ret = devinfo->p->fops->read(node->filp, ulong_to_ptr(ubuf, void), count, *offset);
		/* don't support pread */
		*offset += (ret > 0 ? (unsigned long long)ret : 0);
	}

	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_read, dh_fops_read_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath,
			 unsigned long, ubuf,
			 unsigned long long, file_id,
			 unsigned long long, offset,
			 unsigned long, count,
			 unsigned int, flags)
{
	int ret = E_HM_OK;
	struct __actvret_drvcall_devhost_fops_read *__ret =
		actvhdlr_drvhandler_devhost_fops_read_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	struct filp_node *node = NULL;

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	ret = devhost_fops_read(node, &offset, ubuf, count, flags);
	if (ret < 0) {
		return ret;
	}
	__ret->nread = (unsigned long long)(unsigned int)ret;
	__ret->pos = offset;

	return E_HM_OK;
}

static int devhost_fops_write(struct filp_node *node, unsigned long long *offset,
			      unsigned long ubuf, size_t count, unsigned int flags)
{
	int ret;
	bool is_pwrite = (flags & DEV_PIO);
	struct device_info *devinfo = node->devinfo;

	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((devinfo->p->fops == NULL) ||
	    (devinfo->p->fops->write == NULL && devinfo->p->fops->write_ex == NULL)) {
		return E_HM_NOSYS;
	}

	if (count == 0) {
		return E_HM_OK;
	} else if (count > MAX_RW_COUNT) {
		count =  MAX_RW_COUNT;
	}

	if (devinfo->p->fops->write_ex != NULL) {
		ret = devinfo->p->fops->write_ex(node->filp, ulong_to_ptr(ubuf, void),
						count, offset, is_pwrite);
	} else {
		ret = devinfo->p->fops->write(node->filp, ulong_to_ptr(ubuf, void), count, *offset);
		/* don't support pread */
		*offset += (ret > 0 ? (unsigned long long)ret : 0);
	}

	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_write, dh_fops_write_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath,
			 unsigned long, ubuf,
			 unsigned long long, file_id,
			 unsigned long long, offset,
			 unsigned long, count,
			 unsigned int, flags)
{
	struct __actvret_drvcall_devhost_fops_write *__ret =
		actvhdlr_drvhandler_devhost_fops_write_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	struct filp_node *node = NULL;
	int ret = E_HM_OK;

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	ret = devhost_fops_write(node, &offset, ubuf, count, flags);
	if (ret < 0) {
		return ret;
	}
	__ret->nwrite = (unsigned long long)(unsigned int)ret;
	__ret->pos = offset;

	return E_HM_OK;
}

static int devhost_fops_loop_rw(struct filp_node *node, unsigned long long *offset,
				struct iovec *iov, int nr_segs, unsigned int flags,
				int type)
{
	int i;
	int ret = 0;

	if (iov == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	for (i = 0; i < nr_segs; i++) {
		int nr = 0;
		if (type == WRITE) {
			nr = devhost_fops_write(node, offset, ptr_to_ulong(iov->iov_base), iov->iov_len, flags);
		} else {
			nr = devhost_fops_read(node, offset, ptr_to_ulong(iov->iov_base), iov->iov_len, flags);
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

static struct iovec *devhost_fops_vec_alloc(const void *vec, unsigned int nr_segs,
					    void *recv_data, unsigned long recv_size)
{
	int ret;
	struct iovec *iov = NULL;

	if (recv_size != 0) {
		return (struct iovec *)recv_data;
	}

	if (nr_segs > IOV_MAX) {
		return NULL;
	}

	iov = (struct iovec *)malloc(((size_t)nr_segs) * sizeof(struct iovec));
	if (iov == NULL) {
		return NULL;
	}

	ret = hm_actv_read_caller_vm(iov, vec, ((size_t)nr_segs) * sizeof(struct iovec));
	if (ret < 0) {
		free(iov);
		return NULL;
	}

	return iov;
}

static void devhost_fops_vec_free(struct iovec *iov, unsigned long recv_size)
{
	if (recv_size == 0) {
		free(iov);
	}
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_read_iter, dh_fops_read_iter_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 unsigned long long, offset, const void *, vec,
			 int, nr_segs, size_t, count, unsigned int, flags)
{
	ssize_t ret;
	struct iovec *iov = NULL;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	struct __actvret_drvcall_devhost_fops_read_iter *__ret =
		actvhdlr_drvhandler_devhost_fops_read_iter_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if (count == 0) {
		return E_HM_OK;
	}

	iov = devhost_fops_vec_alloc(vec, (unsigned int)nr_segs, recv.data, recv.size);
	if (iov == NULL) {
		return E_HM_NOMEM;
	}

	if (devinfo->p->ex_fops && devinfo->p->ex_fops->read_iter) {
		ret = devinfo->p->ex_fops->read_iter(node->filp, &offset, iov, nr_segs, count, flags);
	} else {
		ret = devhost_fops_loop_rw(node, &offset, iov, nr_segs, flags, READ);
	}
	if (ret < 0) {
		devhost_fops_vec_free(iov, recv.size);
		return posix2hmerrno(-(int)ret);
	}
	__ret->pos = offset;
	__ret->nread = (unsigned long long)(unsigned int)ret;
	devhost_fops_vec_free(iov, recv.size);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_write_iter, dh_fops_write_iter_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 unsigned long long, offset, const void *, vec,
			 int, nr_segs, size_t, count, unsigned int, flags)
{
	ssize_t ret;
	struct iovec *iov = NULL;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	struct __actvret_drvcall_devhost_fops_write_iter *__ret =
		actvhdlr_drvhandler_devhost_fops_write_iter_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if (count == 0) {
		return E_HM_OK;
	}

	iov = devhost_fops_vec_alloc(vec, (unsigned int)nr_segs, recv.data, recv.size);
	if (iov == NULL) {
		return E_HM_NOMEM;
	}

	if (devinfo->p->ex_fops && devinfo->p->ex_fops->write_iter) {
		ret = devinfo->p->ex_fops->write_iter(node->filp, &offset, iov, nr_segs, count, flags);
	} else {
		ret = devhost_fops_loop_rw(node, &offset, iov, nr_segs, flags, WRITE);
	}
	if (ret < 0) {
		devhost_fops_vec_free(iov, recv.size);
		return posix2hmerrno(-(int)ret);
	}
	__ret->pos = offset;
	__ret->nwrite = (unsigned long long)(unsigned int)ret;
	devhost_fops_vec_free(iov, recv.size);

	return E_HM_OK;
}

static inline long long devhost_fops_llseek(struct filp_node *node, long long offset,
					    int whence)
{
	struct device_info *devinfo = node->devinfo;
	long long pos;

	if ((devinfo->p->fops == NULL) || (devinfo->p->fops->llseek == NULL)) {
		return E_HM_NOSYS;
	}

	pos = devinfo->p->fops->llseek(node->filp, offset, whence);
	if (pos < 0) {
		pos = posix2hmerrno(-(int)pos);
	}

	return pos;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_llseek, dh_fops_llseek_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, int, whence,
			 unsigned long long, file_id, long long, offset)
{
	struct __actvret_drvcall_devhost_fops_llseek *__ret =
		actvhdlr_drvhandler_devhost_fops_llseek_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	struct filp_node *node = NULL;
	long long pos;

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	if ((node->devinfo == NULL) || (node->devinfo->p == NULL)) {
		dh_error("devinfo is invalid");
		return E_HM_INVAL;
	}

	pos = devhost_fops_llseek(node, offset, whence);
	if (pos < 0) {
		return (int)pos;
	}

	__ret->pos = pos;

	return E_HM_OK;
}

static inline bool is_compat_client(void)
{
	return hm_actv_local_is_compat();
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_ioctl, dh_fops_ioctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, int, request,
			 unsigned long long, file_id,
			 unsigned long, arg)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if (devinfo->p->fops == NULL) {
		return E_HM_NOSYS;
	}

	/* Even if the udk driver does not supportd compat ioctl,
	 * we do not want it to be affected now, and we will
	 * make it support compat ioctl in the future.
	 */
	if (is_compat_client() && (devinfo->p->ex_fops != NULL) &&
	    (devinfo->p->ex_fops->compat_ioctl != NULL)) {
		ret = devinfo->p->ex_fops->compat_ioctl(node->filp, (unsigned int)request, arg);
	} else {
		if (devinfo->p->fops->ioctl == NULL) {
			return E_HM_NOSYS;
		}
		ret = devinfo->p->fops->ioctl(node->filp, (unsigned int)request, arg);
	}

	if (ret < 0) {
		/* if it is an driver custom error, the error is returned directly */
		if (posix2hmerrno(-ret) == E_HM_USER_MAX) {
			return ret;
		}
		return posix2hmerrno(-ret);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_flush, dh_fops_flush_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id)
{
	int ret = E_HM_OK;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((devinfo->p->ex_fops == NULL) || (devinfo->p->ex_fops->flush == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devinfo->p->ex_fops->flush(node->filp);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_fcntl, dh_fops_fcntl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, int, request,
			 unsigned long long, file_id,
			 unsigned long, arg)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	int ret = E_HM_OK;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((devinfo->p->fops == NULL) || (devinfo->p->fops->fcntl == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devinfo->p->fops->fcntl(node->filp, request, arg);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return ret;
}

static int __call_drv_mmap_ops(struct filp_node *node, struct device_info *devinfo, struct devhost_vma_info *vma_info,
			       unsigned long offset, int is_ex_fops)
{
	int ret = E_HM_OK;

	vma_info->offset = offset;
	if (is_ex_fops != 0) {
		ret = devinfo->p->ex_fops->mmap_ex(node->filp, vma_info);
	} else {
		ret = devinfo->p->fops->mmap(node->filp, vma_info->start, vma_info->len,
					     vma_info->prot, vma_info->offset);
	}

	return ret;
}

static void __rollback_vm_get_unmap_area(unsigned long addr, bool va_by_dh)
{
	int ret = E_HM_OK;
	if (va_by_dh) {
		ret = actvxactcall_hmcall_mem_iomap_finish(true, true, (const void *)addr, MFLAG_IOMAP_RECLAIM, 0);
	}
	if (ret != E_HM_OK) {
		dh_warn("release unmap area failed, ret=%s\n", hmstrerror(ret));
	}
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_mmap, dh_fops_mmap_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath,
			 unsigned long, addr,
			 unsigned long long, file_id,
			 unsigned long, length,
			 unsigned long, prot,
			 unsigned int, flags,
			 unsigned long, offset,
			 bool, iomap_prepare_sysmgr)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	struct devhost_vma_info *vma_info = NULL;
	int is_ex_fops = 0;
	int ret = E_HM_OK;
	long alloced_va = 0;
	bool prepare_va_by_dh = false;
	struct __actvret_drvcall_devhost_fops_mmap *__ret =
		actvhdlr_drvhandler_devhost_fops_mmap_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((devinfo->p->ex_fops != NULL) && (devinfo->p->ex_fops->mmap_ex != NULL)) {
		/* Support mmap_ex */
		is_ex_fops = 1;
	} else if ((devinfo->p->fops != NULL) && (devinfo->p->fops->mmap != NULL)) {
		is_ex_fops = 0;
	} else {
		return E_HM_NOSYS;
	}

	/* alloc va at devhost for non prepared va */
	if (!iomap_prepare_sysmgr && is_ex_fops && devinfo->p->ex_fops->get_unmapped_area != NULL) {
		/*
		 * alloc va by get_unmapped_area first
		 * if 0 mean not support, use original flow
		 */
		alloced_va = devinfo->p->ex_fops->get_unmapped_area(node->filp, addr, length, offset, flags);
		if (alloced_va > 0) {
			/* alloc suceed */
			prepare_va_by_dh = true;
			addr = (unsigned long)alloced_va;
		} else if (alloced_va < 0) {
			return posix2hmerrno(alloced_va);
		}
	}
	/* if `iomap_prepare_sysmgr`, alloc vma_info only */
	ret = devhost_pal_usermap_prepare(addr, length, prot, flags, iomap_prepare_sysmgr || prepare_va_by_dh, &vma_info);
	if (ret < 0) {
		__rollback_vm_get_unmap_area(addr, prepare_va_by_dh);
		return ret;
	}

	ret = __call_drv_mmap_ops(node, devinfo, vma_info, offset, is_ex_fops);
	if (ret < 0) {
		devhost_pal_usermap_finish(vma_info, iomap_prepare_sysmgr, 0 /* reclaim */, 0);
		return posix2hmerrno(-ret);
	}

	/*
	 * update data `vma_id` to sysmgr if `prepare_va_by_dh`.
	 * it will be true when alloc va by `get_unmapped_area`. because it can't recognized the `vma_info`
	 */
	devhost_pal_usermap_finish(vma_info, iomap_prepare_sysmgr, 1 /* success */, (int)prepare_va_by_dh);

	__ret->rvaddr = (unsigned long long)vma_info->start;
	__ret->iomem_id = vma_info->vma_id;
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_munmap, dh_fops_munmap_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, key, unsigned long, start,
			 unsigned long, len, int, vs_is_releasing)
{
	UNUSED(sender);
	UNUSED(credential);

	return __do_devhost_usermap_munmap(key, start, len, vs_is_releasing);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_file_get, dh_fops_file_get_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id)
{
	struct filp_node *node = NULL;

	UNUSED(credential);

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  fastpath, file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	/* The filp_node will put refcnt in `lnxbase_anon_fget` after query from vfs */
	devhost_get_file(node);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER_LONG(drvhandler_devhost_devfs_read, dh_devfs_read_ext,
			      unsigned long long, sender, unsigned long, credential,
			      long long, file, void *, buf, size_t, count)
{
	long ret = 0;
	struct filp_node *node = NULL;
	unsigned long long offset = 0;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	if ((node->f_modes & MODE_READ) == 0) {
		ret = E_HM_BADF;
		goto out;
	}
#ifdef CONFIG_SEHARMONY
	ret = devhost_seharmony_check(devfs_read, ptr_to_u64(node));
	if (ret != E_HM_OK) {
		goto out;
	}
#endif
	offset = (unsigned long long)node->pos;

	ret = (long)devhost_fops_read(node, &offset, ptr_to_ulong(buf), count, 0);
	if (ret >= 0) {
		node->pos = (int64_t)offset;
	} else if (node->devinfo != NULL) {
		dh_debug("devhost: read failed, device=%s, err=%s\n",
			 node->devinfo->devname, hmstrerror((int)ret));
	}

out:
	devhost_put_file_rpccnt(node);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER_LONG(drvhandler_devhost_devfs_write, dh_devfs_write_ext,
			      unsigned long long, sender, unsigned long, credential,
			      long long, file, const void *, buf, size_t, count)
{
	long ret = 0;
	struct filp_node *node = NULL;
	unsigned long long offset = 0;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	if ((node->f_modes & MODE_WRITE) == 0) {
		ret = E_HM_BADF;
		goto out;
	}
#ifdef CONFIG_SEHARMONY
	ret = devhost_seharmony_check(devfs_write, ptr_to_u64(node));
	if (ret != E_HM_OK) {
		goto out;
	}
#endif
	offset = (unsigned long long)node->pos;
	ret = (long)devhost_fops_write(node, &offset, ptr_to_ulong(buf), count, 0);
	if (ret >= 0) {
		node->pos = (int64_t)offset;
	} else if (node->devinfo != NULL) {
		dh_debug("devhost: write failed, device=%s, err=%s\n",
			 node->devinfo->devname, hmstrerror((int)ret));
	}

out:
	devhost_put_file_rpccnt(node);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_devfs_ioctl, dh_devfs_ioctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long long, file, unsigned int, cmd, unsigned long, arg)
{
	int ret = 0;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}
#ifdef CONFIG_SEHARMONY
	ret = devhost_seharmony_check(devfs_ioctl, ptr_to_u64(node), cmd, arg);
	if (ret != E_HM_OK) {
		goto out;
	}
#endif
	if (is_compat_client() && (devinfo->p->ex_fops != NULL) &&
	    (devinfo->p->ex_fops->compat_ioctl != NULL)) {
		ret = devinfo->p->ex_fops->compat_ioctl(node->filp, cmd, arg);
	} else {
		if (devinfo->p->fops == NULL || devinfo->p->fops->ioctl == NULL) {
			ret = E_HM_NOTTY;
			goto out;
		}
		ret = devinfo->p->fops->ioctl(node->filp, cmd, arg);
	}

	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		dh_debug("devhost: ioctl failed, device=%s, err=%s\n",
			 devinfo->devname, hmstrerror(ret));
	}

out:
	devhost_put_file_rpccnt(node);
	return ret;
}

struct compat_iovec {
	unsigned int iov_base;
	unsigned int iov_len;
};

static int prepare_compat_iov(const struct compat_iovec *_vec, int vlen, struct iovec *iov)
{
	struct compat_iovec *vec = NULL;
	int ret;
	int i;

	vec = calloc((unsigned)vlen, sizeof(struct compat_iovec));
	if (vec == NULL) {
		return E_HM_NOMEM;
	}

	ret = hm_actv_read_xact_vm(vec, _vec, (unsigned)vlen * sizeof(struct compat_iovec));
	if (ret == E_HM_OK) {
		for (i = 0; i < vlen; i++) {
			iov[i].iov_base = (void *)(uintptr_t)vec[i].iov_base;
			iov[i].iov_len = (size_t)vec[i].iov_len;
		}
	}

	free(vec);
	return ret;
}

static int prepare_iov(const struct iovec *_vec, int vlen, struct iovec **dst_iov, size_t *count)
{
	struct iovec *iov = NULL;
	int ret = E_HM_OK;

	iov = (struct iovec *)calloc((unsigned)vlen, sizeof(struct iovec));
	if (iov == NULL) {
		return E_HM_NOMEM;
	}

	if (is_compat_client()) {
		ret = prepare_compat_iov(ptr_cast(_vec, const struct compat_iovec), vlen, iov);
	} else {
		ret = hm_actv_read_xact_vm(iov, _vec, (unsigned)vlen * sizeof(struct iovec));
	}
	if (ret != E_HM_OK) {
		free(iov);
		return ret;
	}

	for (int i = 0; i < vlen; i++) {
		*count += iov[i].iov_len;
	}

	*dst_iov = iov;
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER_LONG(drvhandler_devhost_devfs_readv, dh_devfs_readv_ext,
			      unsigned long long, sender, unsigned long, credential,
			      unsigned long long, file, const void *, _vec, int, vlen)
{
	int ret;
	struct iovec *iov = NULL;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	size_t count = 0;
	unsigned long long offset = 0;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((node->f_modes & MODE_READ) == 0) {
		ret = E_HM_BADF;
		goto out;
	}

#ifdef CONFIG_SEHARMONY
	ret = devhost_seharmony_check(devfs_readv, ptr_to_u64(node));
	if (ret != E_HM_OK) {
		goto out;
	}
#endif

	if (vlen == 0) {
		ret = E_HM_OK;
		goto out;
	}

	if (vlen < 0 || (unsigned)vlen > IOV_MAX) {
		ret = E_HM_INVAL;
		goto out;
	}

	ret = prepare_iov((const struct iovec *)_vec, vlen, &iov, &count);
	if (ret != E_HM_OK) {
		goto out;
	}

	offset = (unsigned long long)node->pos;

	if (devinfo->p->ex_fops && devinfo->p->ex_fops->read_iter) {
		ret = devinfo->p->ex_fops->read_iter(node->filp, &offset, iov, vlen, count, 0);
	} else {
		ret = devhost_fops_loop_rw(node, &offset, iov, vlen, 0, READ);
	}
	if (ret >= 0) {
		node->pos = (int64_t)offset;
	} else {
		ret = posix2hmerrno(-ret);
		dh_debug("devhost: readv failed, device=%s, err=%s\n",
			  devinfo->devname, hmstrerror(ret));
	}

	free(iov);
out:
	devhost_put_file_rpccnt(node);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER_LONG(drvhandler_devhost_devfs_writev, dh_devfs_writev_ext,
			      unsigned long long, sender, unsigned long, credential,
			      long long, file, const void *, _vec, int, vlen)
{
	long ret = 0;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	struct iovec *iov = NULL;
	unsigned long long offset = 0;
	size_t count = 0;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	if ((node->f_modes & MODE_WRITE) == 0) {
		ret = E_HM_BADF;
		goto out;
	}
#ifdef CONFIG_SEHARMONY
	ret = devhost_seharmony_check(devfs_writev, ptr_to_u64(node));
	if (ret != E_HM_OK) {
		goto out;
	}
#endif
	if (vlen == 0) {
		ret = E_HM_OK;
		goto out;
	}

	if (vlen < 0 || (unsigned)vlen > IOV_MAX) {
		ret = E_HM_INVAL;
		goto out;
	}

	ret = prepare_iov((const struct iovec *)_vec, vlen, &iov, &count);
	if (ret != E_HM_OK) {
		goto out;
	}

	offset = (unsigned long long)node->pos;

	if (devinfo->p->ex_fops && devinfo->p->ex_fops->write_iter) {
		ret = devinfo->p->ex_fops->write_iter(node->filp, &offset, iov, vlen, count, 0);
	} else {
		ret = devhost_fops_loop_rw(node, &offset, iov, vlen, 0, WRITE);
	}
	if (ret >= 0) {
		node->pos = (int64_t)offset;
	} else {
		ret = posix2hmerrno(-(int)ret);
		dh_debug("devhost: writev failed, device=%s, err=%s\n",
			 devinfo->devname, hmstrerror((int)ret));
	}

	free(iov);
out:
	devhost_put_file_rpccnt(node);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER_LONG(drvhandler_devhost_devfs_lseek, dh_devfs_lseek_ext,
			      unsigned long long, sender, unsigned long, credential,
			      long long, file, long, offset, int, whence)
{
	struct filp_node *node = NULL;
	long long pos;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if ((node == NULL) || (node->devinfo == NULL) || (node->devinfo->p == NULL)) {
		return E_HM_INVAL;
	}

	pos = devhost_fops_llseek(node, offset, whence);
	if (pos >= 0) {
		node->pos = pos;
	} else {
		dh_debug("devhost: lseek failed, device=%s, err=%s\n",
			 node->devinfo->devname, hmstrerror((int)pos));
		if (pos == E_HM_NOSYS) {
			pos = E_HM_SPIPE;
		}
	}

	devhost_put_file_rpccnt(node);
	return pos;
}

#if defined(__arm__) || !defined(__sysif_server_devhost_no_compat32_handlers__)
#define __LONG_BITS	32
DEFINE_SYSIF_EXT_HANDLER_NO_CHECK(drvhandler_devhost_devfs__llseek, dh_devfs__llseek_ext,
				  unsigned long long, sender, unsigned long, credential,
				  long long, file, unsigned long, offset_high,
				  unsigned long, offset_low, long long *, res,
				  int, whence)
{
	struct filp_node *node = NULL;
	long long pos;
	int ret;
	unsigned long long offset;

	UNUSED(sender, credential);

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if ((node == NULL) || (node->devinfo == NULL) || (node->devinfo->p == NULL)) {
		return E_HM_INVAL;
	}
	offset = ((((unsigned long long)offset_high) << __LONG_BITS) | offset_low);

	pos = devhost_fops_llseek(node, offset, whence);
	if (pos >= 0) {
		node->pos = pos;
		ret = hm_actv_write_xact_vm(res, &pos, sizeof(long long));
		if (ret < 0) {
			ret = E_HM_POSIX_FAULT;
		} else {
			ret = E_HM_OK;
		}
	} else {
		ret = (int)pos;
		dh_debug("devhost: llseek failed, device=%s, err=%s\n",
			 node->devinfo->devname, hmstrerror(ret));
		if (ret == E_HM_NOSYS) {
			ret = E_HM_SPIPE;
		}
	}

	devhost_put_file_rpccnt(node);
	return ret;
}
#endif

static int do_devfs_fcntl(long long file, int cmd, unsigned long arg)
{
	int ret = 0;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;

	node = u64_to_ptr((unsigned long long)file, struct filp_node);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}
#ifdef CONFIG_SEHARMONY
	ret = devhost_seharmony_check(devfs_fcntl, ptr_to_u64(node), cmd, arg);
	if (ret != E_HM_OK) {
		goto out;
	}
#endif
	if ((devinfo->p->fops == NULL) || (devinfo->p->fops->fcntl == NULL)) {
		ret = E_HM_NOSYS;
		goto out;
	}

	ret = devinfo->p->fops->fcntl(node->filp, cmd, arg);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		dh_debug("devhost: fcntl failed, device=%s, err=%s\n",
			 devinfo->devname, hmstrerror(ret));
	}

out:
	devhost_put_file_rpccnt(node);
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_devfs_fcntl, dh_devfs_fcntl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long long, file, int, cmd, unsigned long, arg,
			 int, fd)
{
	UNUSED(sender, credential, fd);

	return do_devfs_fcntl(file, cmd, arg);
}

#if defined(__arm__) || !defined(__sysif_server_devhost_no_compat32_handlers__)
DEFINE_SYSIF_EXT_HANDLER_NO_CHECK(drvhandler_devhost_devfs_fcntl64, dh_devfs_fcntl64_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long long, file, int, cmd, unsigned long, arg,
			 int, fd)
{
	UNUSED(sender, credential, fd);

	return do_devfs_fcntl(file, cmd, arg);
}
#endif

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_do_page_fault, dh_fops_page_fault_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, vma_key, unsigned long long , vr_key,
			 unsigned long long, addr, unsigned int, flags,
			 unsigned long long, offset)
{
	UNUSED(sender, credential);
	return __devhost_do_umap_page_fault(vma_key, vr_key, addr, flags, offset);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_mm_release, dh_fops_mm_release_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long long, vspace_key)
{
	int ret = E_HM_OK;
	UNUSED(sender, credential);
	ret = devhost_backend_mm_release(vspace_key);
	if (ret == -ENOSYS) {
		return E_HM_OK;
	}
	return posix2hmerrno(-ret);
}

/*
 * fops module, support device file operation
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_fops_read`
 *  - target sysif-ep: `drvhandler_devhost_fops_write`
 *  - target sysif-ep: `drvhandler_devhost_fops_llseek`
 *  - target sysif-ep: `drvhandler_devhost_fops_ioctl`
 *  - target sysif-ep: `drvhandler_devhost_fops_mmap`
 *  - target sysif-ep: `drvhandler_devhost_fops_munmap`
 *  - target sysif-ep: `drvhandler_devhost_fops_do_page_fault`
 *
 * export symbol: NULL
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(posix_fops, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_read, dh_fops_read_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_write, dh_fops_write_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_read_iter, dh_fops_read_iter_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_write_iter, dh_fops_write_iter_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_llseek, dh_fops_llseek_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_ioctl, dh_fops_ioctl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_fcntl, dh_fops_fcntl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_flush, dh_fops_flush_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_mmap, dh_fops_mmap_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_munmap, dh_fops_munmap_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_file_get, dh_fops_file_get_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_read, dh_devfs_read_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_write, dh_devfs_write_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_ioctl, dh_devfs_ioctl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_readv, dh_devfs_readv_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_writev, dh_devfs_writev_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_lseek, dh_devfs_lseek_ext),
	IMPORT_SYSIF_EXTENSION_EX(drvhandler_devhost_devfs__llseek, dh_devfs__llseek_ext)
	IMPORT_SYSIF_EXTENSION_EX(drvhandler_devhost_devfs_fcntl64, dh_devfs_fcntl64_ext)
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_devfs_fcntl, dh_devfs_fcntl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_mm_release, dh_fops_mm_release_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_do_page_fault, dh_fops_page_fault_ext)
);
