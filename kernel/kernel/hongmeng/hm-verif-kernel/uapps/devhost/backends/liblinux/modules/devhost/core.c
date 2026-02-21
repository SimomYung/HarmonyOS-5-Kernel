/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/uio.h>
#include <linux/file.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/genhd.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/uidgid.h>
#include <linux/version.h>
#include <linux/securec.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mm_types.h>
#include <linux/eventpoll.h>
#include <linux/posix-clock.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#include <linux/dma-direct.h>
#include <linux/dma-mapping.h>
#include <linux/dma-buf.h>
#endif
#include <linux/platform_device.h>
#include <linux/notifier.h>
#include <linux/kmemleak.h>

#include <asm/ioctls.h>

#include <devhost/memory.h>
#include <devhost/backend.h>
#include <devhost/netlink.h>
#include <devhost/process.h>
#include <lnxbase/lnxbase.h>
#include <liblinux_device.h>

#include "devfs.h"
#include "devhost.h"
#include "linux_device.h"

#ifndef liblinux_register_devfs_open_handler
#define liblinux_register_devfs_open_handler(fn)
#endif

#ifndef liblinux_unlink_file_vma
#define liblinux_unlink_file_vma(vma)
#endif
#ifndef liblinux_link_file_vma
#define liblinux_link_file_vma(mm, vma)
#endif

/* for anonymous file */
static struct device_info anon_devinfo;

/* devhost fops */
static int devhost_fops_no_open(struct device_info *info, void **ctx,
				unsigned int flags)
{
	UNUSED(info);
	UNUSED(ctx);
	UNUSED(flags);
	return -ENXIO;
}

static
int devhost_fops_open(struct device_info *info, void **ctx, unsigned int flags)
{
	struct linux_device_object *obj = NULL;
	struct file *filp = NULL;

	if (info == NULL) {
		return -EINVAL;
	}
	obj = container_of(info, struct linux_device_object, info);
	filp = __devfs_open_file(obj, flags, 0);
	if (IS_ERR(filp)) {
		return PTR_ERR(filp);
	}

	/* test_io_pclk may not open CONFIG_POSIX_TIMERS */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0) || defined(CONFIG_POSIX_TIMERS)
	/* for posix-clock filp, set clkops to posix_clkops */
	if (file_is_posix_clock(filp)) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
		devhost_device_capable_pclk(info, &posix_clkops);
#else
		dev_warn(obj->dev, " UNIMPL: posix-clock device unsupported\n");
#endif
	}
#endif
	/* filp will be tracked by devhost, mark it as false positive */
	kmemleak_not_leak(filp);
	*ctx = filp;

	return 0;
}

static
int devhost_fops_open_ex(struct device_info *info, void **ctx, unsigned int flags, void *kobj_ctx)
{
	struct file *filp = NULL;
	int ret;

	current_thread_info()->kobj_ctx = kobj_ctx;
	ret = devhost_fops_open(info, ctx, flags);
	if (ret == 0) {
		filp = (struct file *)*ctx;
	}
	current_thread_info()->kobj_ctx = NULL;

	return ret;
}
#ifndef FMODE_STREAM
#define FMODE_STREAM	((__force fmode_t)0x200000)
#endif

static
ssize_t __new_sync_read(struct file *filp, char __user *buf,
			 size_t len, loff_t *ppos)
{
	ssize_t ret;
	struct kiocb kiocb;
	struct iov_iter iter;
	struct iovec iov = {
		.iov_base = buf,
		.iov_len = len
	};

	iov_iter_init(&iter, READ, &iov, 1, len);
	init_sync_kiocb(&kiocb, filp);
	kiocb.ki_pos = (ppos ? *ppos : 0);

	ret = filp->f_op->read_iter(&kiocb, &iter);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
}

static
int devhost_fops_read(void *ctx, void *buf, unsigned long count,
		      unsigned long long off)
{
	struct file *filp = (struct file *)ctx;

	if (filp->f_op->read != NULL) {
		return filp->f_op->read(filp, buf, count, &off);
	} else if (filp->f_op->read_iter != NULL) {
		return __new_sync_read(filp, buf, count, &off);
	}

	return -ENOSYS;
}

static
int devhost_fops_read_ex(void *ctx, void *buf, unsigned long count,
			 unsigned long long *off, unsigned int is_pread)
{
	struct file *filp = (struct file *)ctx;
	loff_t *ppos = NULL;
	loff_t pos;
	int ret = -ENOSYS;

	if (off == NULL) {
		return -EINVAL;
	}

	pos = is_pread ? ((loff_t)*off) : filp->f_pos;
	if ((filp->f_mode & FMODE_ATOMIC_POS) && ((unsigned long long)pos != *off)) {
		pr_warn("the file metadata is not consistent with vfs, pos 0x%lx, *off 0x%llx\n",
				(long)pos, *off);
		return -EINVAL;
	}

	ppos = filp->f_mode & FMODE_STREAM ? NULL : &pos;
	if (filp->f_op->read != NULL) {
		ret = filp->f_op->read(filp, buf, count, ppos);
	} else if (filp->f_op->read_iter != NULL) {
		ret = __new_sync_read(filp, buf, count, ppos);
	}
	if (ret >= 0 && !is_pread && ppos) {
		/* it's safe, since vfs hold the lock of vfs's file */
		filp->f_pos = pos;
		*off = (unsigned long long)pos;
	}

	return ret;
}

static
ssize_t __new_sync_write(struct file *filp, const char __user *buf,
			 size_t len, loff_t *ppos)
{
	ssize_t ret;
	struct iov_iter iter;
	struct kiocb kiocb;
	struct iovec iov = {
		.iov_base = (void __user *)buf,
		.iov_len = len
	};

	iov_iter_init(&iter, WRITE, &iov, 1, len);
	init_sync_kiocb(&kiocb, filp);
	kiocb.ki_pos = (ppos ? *ppos : 0);

	ret = filp->f_op->write_iter(&kiocb, &iter);
	if (ret > 0 && ppos)
		*ppos = kiocb.ki_pos;
	return ret;
}

static
int devhost_fops_write_ex(void *ctx, const void *buf, unsigned long count,
			  unsigned long long *off, unsigned int is_pwrite)
{
	struct file *filp = (struct file *)ctx;
	loff_t *ppos = NULL;
	loff_t pos;
	int ret = -ENOSYS;

	if (off == NULL) {
		return -EINVAL;
	}

	pos = is_pwrite ? ((loff_t)*off) : filp->f_pos;
	if ((filp->f_mode & FMODE_ATOMIC_POS) && ((unsigned long long)pos != *off)) {
		pr_warn("the file metadata is not consistent with vfs, pos 0x%lx, off 0x%llx\n", (long)pos, *off);
		return -EINVAL;
	}

	ppos = filp->f_mode & FMODE_STREAM ? NULL : &pos;
	if (filp->f_op->write != NULL) {
		ret = filp->f_op->write(filp, buf, count, ppos);
	} else if (filp->f_op->write_iter != NULL) {
		ret = __new_sync_write(filp, buf, count, ppos);
	}
	if (ret >= 0 && !is_pwrite && ppos) {
		/* it's safe, since vfs hold the lock of vfs's file */
		filp->f_pos = pos;
		*off = (unsigned long long)pos;
	}

	return ret;
}

static
int devhost_fops_write(void *ctx, const void *buf, unsigned long count,
		       unsigned long long off)
{
	struct file *filp = (struct file *)ctx;
	int ret = -ENOSYS;
	loff_t off_loff;

	if (unlikely(off > LLONG_MAX)) {
		return -EINVAL;
	}

	off_loff = (loff_t)off;
	if (filp->f_op->write != NULL) {
		return filp->f_op->write(filp, buf, count, &off_loff);
	} else if (filp->f_op->write_iter != NULL) {
		return __new_sync_write(filp, buf, count, &off_loff);
	}

	return ret;
}

static int devhost_fops_do_loop_rw(struct file *filp, struct iov_iter *iter,
				   loff_t *ppos)
{
	int ret = 0;

	while (iov_iter_count(iter)) {
		ssize_t nr = 0;
		struct iovec iovec = iov_iter_iovec(iter);

		if (iovec.iov_len == 0) {
			iov_iter_advance(iter, nr);
			continue;
		}
		if (iter->type & WRITE) {
			nr = filp->f_op->write(filp, iovec.iov_base, iovec.iov_len, ppos);
		} else {
			nr = filp->f_op->read(filp, iovec.iov_base, iovec.iov_len, ppos);
		}
		if (nr < 0) {
			if (!ret) {
				ret = nr;
			}
			break;
		}

		ret += nr;
		if (nr != iovec.iov_len) {
			break;
		}
		iov_iter_advance(iter, nr);
	}

	return ret;
}

static ssize_t devhost_fops_do_iter_rw(struct file *filp, struct iov_iter *iter,
				       loff_t *ppos)
{
	ssize_t ret;
	struct kiocb kiocb;

	init_sync_kiocb(&kiocb, filp);
	/* now vfs only support sync aio */
	kiocb.ki_complete = NULL;
	kiocb.ki_pos = (ppos ? *ppos : 0);

	if (iter->type & WRITE) {
		ret = filp->f_op->write_iter(&kiocb, iter);
	} else {
		ret = filp->f_op->read_iter(&kiocb, iter);
	}
	if (ret > 0 && ppos) {
		*ppos = kiocb.ki_pos;
	}

	return ret;
}

static
int devhost_fops_do_rw_iter(struct file *filp, unsigned long long *off,
			    struct iov_iter *iter, int support_iter,
			    unsigned int flags)
{
	int ret;
	loff_t pos;
	loff_t *ppos = NULL;

	pos = (flags == 0) ? ((loff_t)*off) : filp->f_pos;
	if ((filp->f_mode & FMODE_ATOMIC_POS) && ((unsigned long long)pos != *off)) {
		pr_warn("the file metadata is not consistent with vfs, pos 0x%lx, off 0x%llx\n", (long)pos, *off);
		return -EINVAL;
	}

	ppos = filp->f_mode & FMODE_STREAM ? NULL : &pos;
	if (support_iter) {
		ret = devhost_fops_do_iter_rw(filp, iter, ppos);
	} else {
		ret = devhost_fops_do_loop_rw(filp, iter, ppos);
	}
	if ((ret >= 0) && (flags == 0)) {
		filp->f_pos = pos;
		*off = (unsigned long long)pos;
	}

	return ret;
}

static
int devhost_fops_read_iter(void *ctx, unsigned long long *off,
			   void *iov, int nr_segs, size_t count,
			   unsigned int flags)
{
	int support_iter = 0;
	struct iov_iter iter = {0};
	struct file *filp = (struct file *)ctx;

	if (filp->f_op->read_iter != NULL) {
		support_iter = 1;
	} else if (filp->f_op->read == NULL) {
		return -EINVAL;
	}
	iov_iter_init(&iter, READ, iov, nr_segs, count);
	return devhost_fops_do_rw_iter(filp, off, &iter, support_iter, flags);
}

static
int devhost_fops_write_iter(void *ctx, unsigned long long *off,
			    void *iov, int nr_segs, size_t count,
			    unsigned int flags)
{
	int support_iter = 0;
	struct iov_iter iter = {0};
	struct file *filp = (struct file *)ctx;

	if (filp->f_op->write_iter != NULL) {
		support_iter = 1;
	} else if (filp->f_op->write == NULL) {
		return -EINVAL;
	}
	iov_iter_init(&iter, WRITE, iov, nr_segs, count);
	return devhost_fops_do_rw_iter(filp, off, &iter, support_iter, flags);
}

static int __ioctl_fionbio(struct file *filp, int __user *argp)
{
	int on;
	int ret;

	ret = get_user(on, argp);
	if (ret) {
		return ret;
	}

	if (on) {
		filp->f_flags |= O_NONBLOCK;
	} else {
		filp->f_flags &= ~O_NONBLOCK;
	}

	return ret;
}

static int __do_vfs_ioctl(struct file *filp, unsigned int request, unsigned long arg)
{
	int ret;

	if (request == FIONBIO) {
		ret = __ioctl_fionbio(filp, (int __user *)arg);
	} else {
		ret = -ENOIOCTLCMD;
	}

	return ret;
}

static
int devhost_fops_ioctl(void *ctx, unsigned int request, unsigned long arg)
{
	struct file *filp = (struct file *)ctx;
	int ret;

	ret = __do_vfs_ioctl(filp, request, arg);
	if (ret != -ENOIOCTLCMD) {
		return ret;
	}

	if (filp->f_op->unlocked_ioctl != NULL) {
		return filp->f_op->unlocked_ioctl(filp, request, arg);
	} else if (filp->f_op->compat_ioctl != NULL) {
		return filp->f_op->compat_ioctl(filp, request, arg);
	} else {
		return -ENOSYS;
	}
}

static
int devhost_fops_compat_ioctl(void *ctx, unsigned int request,
			      unsigned long arg)
{
	int __ret = 0;
	struct file *filp = (struct file *)ctx;
	int ret;

	ret = __do_vfs_ioctl(filp, request, arg);
	if (ret != -ENOIOCTLCMD) {
		return ret;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0)
	set_thread_flag(TIF_32BIT);
#endif
	if (filp->f_op->compat_ioctl != NULL) {
		__ret =  filp->f_op->compat_ioctl(filp, request, arg);
	} else {
		__ret =  -ENOSYS;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0)
	clear_thread_flag(TIF_32BIT);
#endif
	return __ret;
}

static
int devhost_fops_fcntl(void *ctx, int cmd, unsigned long arg)
{
	struct file *filp = (struct file *)ctx;
	int ret = 0;

	switch (cmd) {
	case F_GETFL:
		ret = (int)(filp->f_flags);
		break;
	case F_SETFL:
		if (arg & O_NONBLOCK) {
			filp->f_flags |= O_NONBLOCK;
		} else {
			filp->f_flags &= ~O_NONBLOCK;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static
int devhost_fops_mmap(void *ctx,
		      unsigned long addr, unsigned long length, unsigned long prot,
		      unsigned long offset)
{
	int ret;
	struct vm_area_struct vma;
	struct file *filp = (struct file *)ctx;

	if (filp->f_op->mmap == NULL) {
		return -ENOSYS;
	}

	ret = memset_s(&vma, sizeof(struct vm_area_struct), 0, sizeof(struct vm_area_struct));
	BUG_ON(ret != 0);
	vma.vm_start = addr;
	vma.vm_end = addr + length;
	vma.vm_pgoff = offset >> PAGE_SHIFT;
	pgprot_val(vma.vm_page_prot) = prot;

	return filp->f_op->mmap(filp, &vma);
}

/* When the memory is not reclaimed, kzalloc increases the memory of devhost.
 * Therefore, isolated by config.
 */
#ifdef CONFIG_LIBLINUX_MEM_SHRINK
#define alloc_vma()	((struct vm_area_struct *)kzalloc(sizeof(struct vm_area_struct), GFP_KERNEL))
#define free_vma(vma)	kfree(vma)
#else
#define alloc_vma()	((struct vm_area_struct *)liblinux_pal_calloc(1, sizeof(struct vm_area_struct)))
#define free_vma(vma)	liblinux_pal_free(vma)
#endif

static long devhost_fops_get_unmap_area(void *ctx, unsigned long addr, unsigned long len,
					unsigned long offset, unsigned long flags)
{
	struct file *filp = (struct file *)ctx;
	unsigned long area = 0;

	if (filp->f_op->get_unmapped_area == NULL) {
		return 0; /* not support */
	}

	area = filp->f_op->get_unmapped_area(filp, addr, len, offset >> PAGE_SHIFT, flags);
	if (IS_ERR_VALUE(area)) {
		return (long)area;
	}
	return area;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
#define mmap_write_lock(mm)
#define mmap_write_unlock(mm)
#define mmap_read_lock(mm)
#define mmap_read_unlock(mm)
#endif
static
int devhost_fops_mmap_ex(void *ctx, struct devhost_vma_info *vma_info)
{
	struct vm_area_struct *vma = NULL;
	struct file *filp = (struct file *)ctx;
	int ret;
	bool can_use_read_lock = false;

	if (filp->f_op->mmap == NULL) {
		return -ENOSYS;
	}

	vma = alloc_vma();
	if (vma == NULL) {
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&vma->anon_vma_chain);
	vma->vm_start = vma_info->start;
	vma->vm_end = vma->vm_start + vma_info->len;
	vma->vm_pgoff = vma_info->offset >> PAGE_SHIFT;
	if (vma_info->flags & MAP_SHARED) {
		vma->vm_flags = VM_SHARED;
	}
	vma->vm_flags |= vma_info->prot & (PROT_READ | PROT_WRITE | PROT_EXEC);
	vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
	vma->vm_file = get_file(filp);
	vma->vm_mm = current->mm;

#ifdef FMODE_MAP_DOWNGRADE
	if (is_dma_buf_file(filp) || (filp->f_mode & FMODE_MAP_DOWNGRADE)) {
		can_use_read_lock = true;
	}
#endif
	if (can_use_read_lock) {
		mmap_read_lock(vma->vm_mm);
	} else {
		mmap_write_lock(vma->vm_mm);
	}
	ret =  filp->f_op->mmap(filp, vma);
	if (can_use_read_lock) {
		mmap_read_unlock(vma->vm_mm);
	} else {
		mmap_write_unlock(vma->vm_mm);
	}
	if (ret < 0) {
		fput(filp);
		free_vma(vma);
		return ret;
	}

	liblinux_link_file_vma(vma->vm_mm, vma);
	vma_info->priv = vma;
	kmemleak_not_leak((void *)vma);
	return ret;
}

/* for transfs mmap */
int devhost_transfs_fops_mmap(void *ctx, struct devhost_vma_info *vma_info)
{
	return devhost_fops_mmap_ex(ctx, vma_info);
}
EXPORT_SYMBOL(devhost_transfs_fops_mmap);

static
int devhost_fops_close(void *ctx)
{
	struct file *filp = (struct file *)ctx;

	__fput_sync(filp);

	return 0;
}

static
int devhost_fops_flush(void *ctx)
{
	int ret = 0;
	struct file *filp = (struct file *)ctx;

	if (filp->f_op && filp->f_op->flush)
		ret = filp->f_op->flush(filp, 0);

	return ret;
}

static unsigned int devhost_fops_get_flags(void *ctx)
{
	unsigned int ret = 0;
	struct file *filp = (struct file *)ctx;

	if (!(filp->f_op->flush))
		ret |= DH_IO_CTX_DEVFOPS_NO_FLUSH_FLAG;

	return ret;
}

#define EPOLL_RECHECK 0x00020000U

static
int devhost_fops_poll_ex(void *ctx, void *pinfo, unsigned int *revents)
{
	int ret;
	struct file *filp = (struct file *)ctx;
	poll_table *pt = NULL;
	poll_table temp_pt;

	if (unlikely(revents == NULL)) {
		pr_err("%s: invalid params\n", __func__);
		return -EINVAL;
	}

	/* Note:
	 * 1) For syscall poll/select, the default mask(DEFAULT_POLLMASK)
	 *    is returned for files that do not support poll.
	 * 2) For syscall epoll, the target file descriptor must support poll.
	 */
	if (filp->f_op->poll == NULL) {
		*revents = DEFAULT_POLLMASK;
		pr_debug("%s: not support poll\n", __func__);
		return -EPERM;
	}

	/* no wait to get revents */
	if (pinfo == NULL) {
		/* Note: cam_cfgdev_vo_poll() detects that the pt is NULL,
		 * err-level logs will be generated, which may cause massive logs.
		 * Maybe this driver is not written properly
		 */
		init_poll_funcptr(&temp_pt, NULL);
		pt = &temp_pt;
	} else {
		ret = devhost_poll_initwait(pinfo, (void **)&pt);
		if (ret < 0) {
			return ret;
		}
	}

	*revents = filp->f_op->poll(filp, pt);
	/* All revents need recheck flags */
	*revents |= EPOLL_RECHECK;

	return 0;
}

/* for ldk_ext/net_sock */
int devhost_net_fops_poll(void *ctx, void *pinfo, unsigned int *revents)
{
	struct file *filp = (struct file *)ctx;
	int ret;

	if (revents == NULL) {
		if (filp->f_op->poll == NULL) {
			pr_debug("%s: not support poll\n", __func__);
			ret = -EPERM;
		} else {
			(void)filp->f_op->poll(filp, NULL);
			ret = 0;
		}
	} else {
		ret = devhost_fops_poll_ex(ctx, pinfo, revents);
	}

	return ret;
}
EXPORT_SYMBOL(devhost_net_fops_poll);

int devhost_net_fops_mmap(void *ctx, struct devhost_vma_info *vma_info)
{
	return devhost_fops_mmap_ex(ctx, vma_info);
}
EXPORT_SYMBOL(devhost_net_fops_mmap);

static
long long devhost_fops_llseek(void *ctx, long long offset, int whence)
{
	struct file *filp = (struct file *)ctx;

	if (filp->f_op->llseek == NULL) {
		return -ENOSYS;
	}

	return filp->f_op->llseek(filp, offset, whence);
}

static struct devhost_fops linux_device_fops = {
	.open = devhost_fops_open,
	.ioctl = devhost_fops_ioctl,
	.fcntl = devhost_fops_fcntl,
	.llseek = devhost_fops_llseek,
	.read_ex = devhost_fops_read_ex,
	.write_ex = devhost_fops_write_ex,
	.read = devhost_fops_read,
	.write = devhost_fops_write,
	.mmap = devhost_fops_mmap,
	.close = devhost_fops_close,
	.poll = NULL,
};

/* for anonymous device */
static struct devhost_fops linux_anon_fops = {
	.open = devhost_fops_no_open,
	.ioctl = devhost_fops_ioctl,
	.fcntl = devhost_fops_fcntl,
	.llseek = devhost_fops_llseek,
	.read = devhost_fops_read,
	.write = devhost_fops_write,
	.mmap = devhost_fops_mmap,
	.close = devhost_fops_close,
	.poll = NULL,
};

static struct devhost_fops_ex linux_device_ex_fops = {
	.size = sizeof(struct devhost_fops_ex),
	.open = devhost_fops_open_ex,
	.poll = devhost_fops_poll_ex,
	.compat_ioctl = devhost_fops_compat_ioctl,
	.get_unmapped_area = devhost_fops_get_unmap_area,
	.mmap_ex = devhost_fops_mmap_ex,
	.write_iter = devhost_fops_write_iter,
	.read_iter = devhost_fops_read_iter,
	.flush = devhost_fops_flush,
	.get_fops_flags = devhost_fops_get_flags,

};

static int devhost_umap_ops_munmap(void *data, int vs_is_releasing)
{
	struct vm_area_struct *vma = (struct vm_area_struct *)data;

	if (vma == NULL) {
		return 0;
	}

	liblinux_unlink_file_vma(vma);
	if (vs_is_releasing != 0) {
		current->flags |= PF_EXITING; /* add PF_EXITING */
	} else {
		current->flags &= (~PF_EXITING); /* clear PF_EXITING */
		mmap_write_lock(vma->vm_mm);
		/*
		 * Since that memory mappings are already canceled in __do_devhost_usermap_munmap
		 * holding global vma idr write lock, and that we does not allow UPGROW/DOWNGROW
		 * of memory, we can downgrade the vma write lock to read lock. But munmap must first
		 * hold write lock to avoid racing with find_vma:
		 *
		 *          find_vma:                             munmap:
		 *
		 *                                      __do_devhost_usermap_munmap
		 *                                          WLOCK: g_vma_idr_lock
		 *                                              find + remove
		 *                                          WunLOCK
		 *                                      devhost_backend_umap_munmap
		 * mmap_read_lock(current->mm)
		 * find_vma
		 *     devhost_pal_find_vma
		 *         RLOCK: g_vma_idr_lock
		 *             find
		 *         RunLOCK
		 * (do something to vma)
		 * mmap_read_unlock(current->mm)
		 *                                      devhost_backend_umap_munmap
		 *                                          mmap_write_lock(vma->mm)
		 *                                          mmap_write_unlock(vma->mm)
		 *                                          mmap_read_lock(vma->mm)
		 *                                              close(vma)
		 *                                          mmap_read_unlock(vma->mm)
		 *
		 * If we directly change mmap_write_lock to mmap_read_lock here, close(vma) may
		 * race with find_vma and unmap the memory that is found by find_vma(), eventually
		 * causing a UAF. So we must first hold write lock to wait for all vma that have
		 * been successfully found are released. After that, we can safely unlock and change
		 * to read lock because the vma has already been removed by devhost_usermap_munmap
		 * and cannot be found by find_vma().
		 * mmap_write_downgrade will do nothing if the head of wait list is a WRITE, so here
		 * we use unlock instead of downgrade to accelerate the downgrade of write locks.
		 */
		mmap_write_unlock(vma->vm_mm);
		mmap_read_lock(vma->vm_mm);
	}
	if ((vma->vm_ops != NULL) && (vma->vm_ops->close != NULL)) {
		vma->vm_ops->close(vma);
	}
	current->flags &= (~PF_EXITING);
	if (vma->vm_file != NULL) {
		fput(vma->vm_file);
	}

	if (vs_is_releasing == 0) {
		mmap_read_unlock(vma->vm_mm);
	}

	free_vma(vma);
	return 0;
}

static struct devhost_umap_ops linux_umap_ops __read_mostly = {
	.munmap = devhost_umap_ops_munmap,
};

static void __linux_device_release(struct device_info *info)
{
	struct linux_device_object *obj = NULL;

	BUG_ON(info == NULL);
	obj = container_of(info, struct linux_device_object, info);

	BUG_ON(obj->dev == NULL);

#ifndef CONFIG_DEVTMPFS
	if (obj->inode != NULL) {
		rm_dev_inode(obj->inode);
	}
#endif

	obj->dev->archdata.liblinux_private = NULL;
	put_device(obj->dev);
	kfree(obj);
}

int linux_device_init(struct device *dev)
{
	struct linux_device_object *obj = NULL;
	int ret;

	if (dev == NULL || linux_device_object_of(dev) != NULL) {
		return -EINVAL;
	}

	/* alloc linux_device_object */
	obj = (struct linux_device_object *)kzalloc(sizeof(*obj), GFP_KERNEL);
	if (obj == NULL) {
		return -ENOMEM;
	}

	/* init device_info */
	ret = devhost_device_init(&obj->info);
	if (ret < 0) {
		kfree(obj);
		return ret;
	}
	obj->dev = get_device(dev);
	obj->info.devid = -1; /* fill after register to devmgr */
	obj->info.release = __linux_device_release;

	/* store device_object to device's private archdata */
	dev->archdata.liblinux_private = &obj->info;

	return 0;
}
EXPORT_SYMBOL(linux_device_init);

static int __linux_device_init_prop(struct device *dev,
				    struct linux_device_object *obj);
static void __linux_device_set_dma_ops(struct device *dev);
static int linux_device_prepare(struct device *dev,
				struct linux_device_object *obj)
{
	struct linux_device_object *parent_obj = NULL;
	int ret;

	/* create inode for device with devt set */
	if (dev->devt != 0) {
		ret = __devfs_node_init(dev);
		if (ret < 0) {
			pr_err("devfs_node_init for device %s failed\n", dev_name(dev));
			return ret;
		}
		/* All devices can be accessed using fops */
		devhost_device_capable_devfs(&obj->info, dev->devt,
					     is_blockdev(dev) ? MODE_BLK : MODE_CHR,
					     linux_get_devnode,
					     &linux_device_fops);
		devhost_device_capable_devfs_ex(&obj->info, &linux_device_ex_fops);
#ifdef CONFIG_BLOCK
		if (is_blockdev(dev)) {
			devhost_device_capable_blk(&obj->info, &blkops);
			devhost_device_capable_directmap_shm(&obj->info);
		}
#endif
	}

	/* fill device_info */
	obj->info.devname = dev_name(dev);

	/* set parent */
	parent_obj = (dev->parent == NULL) ? NULL
					   : linux_device_object_of(dev->parent);
	if (parent_obj != NULL) {
		obj->info.parent = parent_obj->info.devid;
	} else {
		/* try to get parent_devid from of_node's property */
		ret = of_property_read_s32(dev_of_node(dev), "parent-devid",
					   &obj->info.parent);
		if (ret < 0) {
			obj->info.parent = 0;
		}
	}

	ret = __linux_device_init_prop(dev, obj);
	if (ret < 0) {
		pr_err("init prop failed, device: %s\n", dev_name(dev));
		return ret;
	}

	/* set pm ops */
	devhost_device_capable_pm(&obj->info, &dev_pm_ops);

	__linux_device_set_dma_ops(dev);

	return 0;
}

static int __isolate_device_driver_override(struct device *dev)
{
	struct platform_device *pdev = NULL;
	if (dev->bus == &platform_bus_type) {
		pdev = to_platform_device(dev);
		pdev->driver_override = kstrdup("of:proxy", GFP_KERNEL);
		if (pdev->driver_override == NULL) {
			return -ENOMEM;
		}
	} else {
		pr_warn("support platfrom isolate device only\n");
		return -ENOSYS;
	}
	return 0;
}

static int __linux_device_init_prop(struct device *dev,
				    struct linux_device_object *obj)
{
	int ret;
	bool isolate = false;
	bool autoprobe = false;
	struct property *prop = NULL;

	if (dev->of_node) {
		if (of_find_property(dev->of_node, "autoprobe", NULL) != NULL) {
			autoprobe = true;
		}
		prop = of_find_property(dev->of_node, "isolate", NULL);
		if (prop != NULL) {
			/* set a dummy driver to avoid further driver probe */
			ret = __isolate_device_driver_override(dev);
			if (ret < 0) {
				return ret;
			}
			devhost_device_set_isolate(&obj->info, prop->value);
			isolate = true;
		}
		prop = of_find_property(dev->of_node, "no-autoload", NULL);
		if (prop == NULL) {
			devhost_device_set_autoload(&obj->info);
		}
		prop = of_find_property(dev->of_node, "compatible", NULL);
	}

	/*
	 * 1. device with `bus` set is probeable when ldk disable devices_autoprobe
	 * 2. when ldk enable devices_autoprobe and disable devmgr autoprobe:
	 *    proxy devhost use devmgr-autoprobe;
	 *    isolate device at root devhost use devmgr-autoprobe;
	 */
	if (dev->bus != NULL) {
		if ((!lnxbase_support_device_autoprobe()) ||
		     (lnxbase_support_device_autoprobe() && (autoprobe || isolate))) {
			ret = devhost_device_set_probeable(&obj->info,
							   (prop != NULL) ? prop->value : NULL,
							   (prop != NULL) ? prop->length : 0);
			if (ret < 0) {
				return ret;
			}
		}
	}

	return 0;
}

static void __linux_device_set_dma_ops(struct device *dev)
{
	/* set dma ops if required */
	#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,206)
	# define dma_archdata(dev)	(&((dev)->archdata))
	#else
	# define dma_archdata(dev)	(dev)
	#endif
	if (dma_archdata(dev)->dma_ops == NULL) {
		dma_archdata(dev)->dma_ops = &dma_devhost_ops;
	}
}

static int devhost_platform_notify(struct device *dev)
{
	struct linux_device_object *obj = NULL;
	int ret;

	/* device object may already exist, e.g. root device or proxy device */
	obj = linux_device_object_of(dev);
	if (obj == NULL) {
		/* if not exist, create it */
		ret = linux_device_init(dev);
		if (ret < 0) {
			return ret;
		}
		obj = linux_device_object_of(dev);
	}

	/* init linux_device_object */
	ret = linux_device_prepare(dev, obj);
	if (ret < 0) {
		devhost_device_destroy(&obj->info);
		return ret;
	}

	/* register to devhost */
	ret = devhost_register_device(&obj->info);
	if (ret < 0) {
		devhost_device_destroy(&obj->info);
		return ret;
	}

	return 0;
}

static int devhost_platform_notify_remove(struct device *dev)
{
	struct linux_device_object *obj = NULL;
	int ret;

	obj = linux_device_object_of(dev);
	if (obj == NULL) {
		return -EINVAL;
	}

	linux_device_set_dead(obj);
	/* unregister from devhost */
	ret = devhost_unregister_device(&obj->info);
	if (ret != 0) {
		return ret;
	}

	/* destroy device */
	devhost_device_destroy(&obj->info);

	return 0;
}

static int __devhost_device_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
	struct device *dev = (struct device *)data;
	int ret;

	if (action == LIBLINUX_DEVICE_ADD) {
		ret = devhost_platform_notify(dev);
	} else if (action == LIBLINUX_DEVICE_DEL) {
		ret = devhost_platform_notify_remove(dev);
	} else {
		ret = -EINVAL;
	}

	if (ret < 0) {
		return notifier_from_errno(ret);
	}
	return NOTIFY_DONE;
}

static int __devhost_anon_devinfo_init(void)
{
	int ret;

	ret = devhost_device_init(&anon_devinfo);
	if (ret < 0) {
		/* devhost_device_init() returns posix errno */
		pr_warn("failed to init anon_devinfo, ret=%d\n", ret);
		return ret;
	}

	devhost_device_capable_devfs(&anon_devinfo, 0, 0, NULL, &linux_anon_fops);
	devhost_device_capable_devfs_ex(&anon_devinfo, &linux_device_ex_fops);
	devhost_device_set_anonymous(&anon_devinfo);
	ret = lnxbase_anon_devinfo_init(&anon_devinfo);
	if (ret < 0) {
		devhost_device_destroy(&anon_devinfo);
	}
	return ret;
}

static struct notifier_block device_notify __read_mostly = {
	.notifier_call = __devhost_device_notifier,
};

static int __net_later_init_handler(void *arg)
{
	return devhost_net_init();
}

static int __init __devhost_driver_init(void)
{
	int ret;

	/* init module support first */
	ret = devhost_module_init();
	if (ret < 0) {
		pr_err("module support init failed, ret=%d\n", ret);
		return ret;
	}

	(void)devhost_backend_register_umap_ops(&linux_umap_ops);

#ifdef CONFIG_PROFILING
	devhost_task_exit_notify_register(liblinux_profile_task_exit);
#endif
	/* register anonymous fops */
	ret = __devhost_anon_devinfo_init();
	if (ret < 0) {
		pr_warn("anonymous file init failed, err=%d\n", ret);
	}

#ifdef CONFIG_DEVHOST_NETLINK
	/* register netlink */
	ret = devhost_netlink_register_entr(devhost_linux_netlink_recv);
	if (ret) {
		pr_warn("register netlink entrance failed %d\n", ret);
	}
#endif

	/* register earlyfdt notifier for virqchip */
	ret = devhost_virqchip_init();
	if (ret < 0) {
		pr_err("virqchip init failed, err=%d\n", ret);
		return ret;
	}

	/* register kvic virt irqchip */
	ret = devhost_kvic_init();
	if (ret < 0) {
		pr_err("kvic init failed, err=%d\n", ret);
		return ret;
	}

	/* register vtimer */
	ret = vtimer_init();
	if (ret < 0) {
		pr_err("vtimer init failed, err=%d\n", ret);
		return ret;
	}

	/* calibrate loops_per_jiffy in liblinux */
	calibrate_delay();
	ret = devhost_postinit(__net_later_init_handler, NULL);
	if (ret < 0) {
		pr_err("register net post init failed\n");
		return -ENOMEM;
	}
	ret = devhost_devfs_init();
	if (ret < 0) {
		pr_err("devhost devfs init failed, err=%d\n", ret);
		return ret;
	}

	/* after devfs init done */
	ret = liblinux_register_device_notifier(&device_notify);
	if (ret < 0) {
		pr_err("register device notifier failed, ret=%d\n", ret);
		return ret;
	}

	ret = devhost_power_init();
	if (ret < 0) {
		pr_err("devhost power init failed, err=%d\n", ret);
		return ret;
	}

	ret = devhost_key_init();
	if (ret < 0) {
		pr_err("devhost key init failed, err=%d\n", ret);
		return ret;
	}

	ret = devhost_poll_init();
	if (ret < 0) {
		pr_err("devhost poll init failed, err=%d\n", ret);
		return ret;
	}

	liblinux_register_devfs_open_handler((devfs_open_handler_t)devhost_devfs_open);

	return devhost_register_driver("__builtin", &devhost_builtin_ops, NULL);
}
module_init(__devhost_driver_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jiuyue Ma");
MODULE_DESCRIPTION("devhost driver");
MODULE_VERSION("0.1");
