/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ops for transfs operation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 29 10:42:14 2021
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <liblinux/pal.h>
#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/file.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/kthread.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/math64.h>
#include <linux/namei.h>
#include <linux/parser.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/statfs.h>
#include <linux/securec.h>
#include <linux/uio.h>
#include <linux/uidgid.h>
#include <linux/uaccess.h>
#include <linux/writeback.h>
#include <linux/gfp.h>
#include <linux/vmalloc.h>
#include <devhost/transfs.h>
#include <devhost/devhost.h>
#include <uapi/linux/xattr.h>
#include <lnxbase/lnxbase.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <linux/workqueue.h>
#include <linux/pagemap.h>
#include <linux/buffer_head.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include "transfs.h"
#include <linux/kmemleak.h>
#include <liblinux/pal.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
#include <linux/xattr.h>
#include <linux/sched/signal.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
#include <uapi/linux/mount.h>
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("transfs ops");
MODULE_VERSION("0.01");

/*
 * printk rate limiting, lifted from transfs subsystem.
 * This enforces a rate limit: not more than 1 kernel messages
 * every 3600s to make a denial-of-service attack impossible.
 */
DEFINE_RATELIMIT_STATE(trans_printk_ratelimit_state, 3600 * HZ, 1);
#define trans_printk_ratelimit() ___ratelimit(&trans_printk_ratelimit_state, __func__)
#define transfs_limit_warn(limit, fmt, ...) \
{ \
	if (!limit) { \
		pr_warn(fmt, ##__VA_ARGS__); \
	} else if (trans_printk_ratelimit() != 0) { \
		pr_warn(fmt, ##__VA_ARGS__); \
	} \
}

static struct workqueue_struct *g_transfs_wq = NULL;
static struct devhost_transfs_ops transfs_ops __read_mostly;
struct hm_file_info {
	unsigned int me_index;
	unsigned long long hm_filp;
};

struct transfs_dropcache_work {
	struct work_struct work;
	struct super_block *sb;
};

static unsigned long transfs_page_sync_watermark = DEFAULT_PAGE_SYNC_MAX_NR;
static unsigned long transfs_page_clean_watermark = DEFAULT_PAGE_CLEAN_MAX_NR;
module_param(transfs_page_sync_watermark, ulong, 0);
module_param(transfs_page_clean_watermark, ulong, 0);

static void drop_pagecache_sb_handler(struct work_struct *work);
static inline void transfs_init_dropcache_work(struct transfs_dropcache_work *twork,
					       struct super_block *sb)
{
	INIT_WORK(&twork->work, drop_pagecache_sb_handler);
	twork->sb = sb;
}

static int transfs_alloc_dentry(struct dentry *pdentry, const char *dname,
				struct dentry **ret_dentry)
{
	struct dentry *dentry = NULL;

	dentry = lookup_one_len(dname, pdentry, strlen(dname));
	if (IS_ERR(dentry)) {
		pr_warn("lookup %s failed\n", dname);
		return PTR_ERR(dentry);
	}

	if (dentry->d_inode != NULL) {
		pr_warn("dentry:%s been found\n", dname);
		dput(dentry);
		return -EEXIST;
	}

	*ret_dentry = dentry;
	return 0;
}

static void transfs_dget(struct dentry *dentry)
{
	dget(dentry);
}

static int transfs_alloc_file(struct vfsmount *vmnt, struct dentry *dentry,
			      int flag, struct file **file)
{
	struct file *f = NULL;
	struct inode *inode = dentry->d_inode;
	struct path path = {
		.mnt = vmnt,
		.dentry = dentry,
	};

	vmnt->mnt_root = inode->i_sb->s_root;
	vmnt->mnt_sb = inode->i_sb;
	vmnt->mnt_flags = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
	f = alloc_file(&path, flag, inode->i_fop);
#else
	f = alloc_file(&path, OPEN_FMODE(flag), inode->i_fop);
#endif
	if (IS_ERR(f)) {
		pr_warn("alloc file failed\n");
		return PTR_ERR(f);
	}

	*file = f;

	return 0;
}

static int devhost_transfs_mounted(const char *fs_name, const char *dev_path,
				   const char *mount_opts, int mount_flags,
				   struct transfs_mounted_ret *mount_ret)
{
	struct dentry *root_dentry = NULL;
	struct file_system_type *type = NULL;
	struct vfsmount *vmnt = NULL;
	struct transfs_dropcache_work *twork = NULL;
	int err;

	twork = liblinux_pal_calloc(1, sizeof(struct transfs_dropcache_work));
	if (twork == NULL) {
		pr_warn("transfs: alloc work failed\n");
		return -ENOMEM;
	}

	type = get_fs_type(fs_name);
	if (type == NULL) {
		pr_warn("get fs_type[%s] failed\n", fs_name);
		err = -EFAULT;
		goto out;
	}

	vmnt = vfs_kern_mount(type, mount_flags, dev_path, (void *)mount_opts);
	if (IS_ERR(vmnt)) {
		pr_debug("kern mount %s failed, err:%ld\n", fs_name, PTR_ERR(vmnt));
		err = PTR_ERR(vmnt);
		goto out;
	}

	root_dentry = vmnt->mnt_root;
	if (IS_ERR(root_dentry)) {
		err = PTR_ERR(root_dentry);
		goto out;
	}

	igrab(root_dentry->d_inode);
	transfs_init_dropcache_work(twork, root_dentry->d_inode->i_sb);
	mount_ret->root_lnkcnt = root_dentry->d_inode->__i_nlink;
	mount_ret->dentry = root_dentry;
	mount_ret->inode = root_dentry->d_inode;
	mount_ret->sb = root_dentry->d_inode->i_sb;
	mount_ret->vmnt = vmnt;
	mount_ret->ops = &transfs_ops;
	mount_ret->block_size = root_dentry->d_sb->s_blocksize;
	mount_ret->s_time_gran = root_dentry->d_sb->s_time_gran;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	mount_ret->s_max_time = root_dentry->d_sb->s_time_max;
	mount_ret->s_min_time = root_dentry->d_sb->s_time_min;
#else
	mount_ret->s_max_time = 0;
	mount_ret->s_min_time = 0;
#endif
	mount_ret->data = twork;
	kmemleak_not_leak((void *)twork);
	return 0;

out:
	liblinux_pal_free(twork);
	return err;
}

static void devhost_transfs_unmounted(struct vfsmount *vmnt, int flags, void *data)
{
	struct transfs_dropcache_work *twork = NULL;

	BUG_ON(data == NULL);

	twork = (struct transfs_dropcache_work *)data;
	if (work_busy(&twork->work) != 0) {
		(void)flush_work(&twork->work);
	}

	liblinux_pal_free(twork);

	kern_unmount(vmnt);
}

/*
 * Caller should make sure lock/unlock parent_node and inc/dec mnt_writers.
 * (hmfs's vfs will do lock/unlock and inc/dec)
 */
static int devhost_transfs_mkdir(struct dentry *pdentry, mode_t mode, const char *dname,
				 struct transfs_dn_meta_info_t *dn_meta_out)
{
	int ret;
	struct dentry *dentry = NULL;

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	ret = transfs_alloc_dentry(pdentry, dname, &dentry);
	if (ret) {
		return ret;
	}

	inode_lock_nested(pdentry->d_inode, I_MUTEX_PARENT);
	ret = vfs_mkdir(pdentry->d_inode, dentry, (umode_t)mode);
	inode_unlock(pdentry->d_inode);
	if (ret) {
		pr_debug("mkdir %s failed, err:%d\n", dentry->d_name.name, ret);
		dput(dentry);
		return ret;
	}

	igrab(dentry->d_inode);
	dn_meta_out->dentry = dentry;
	dn_meta_out->inode = dentry->d_inode;
	dn_meta_out->ino = (uint32_t)dentry->d_inode->i_ino;

	return 0;
}

/*
 * hmfs's vfs will do lock/unlock parent and inc/dec
 */
static int devhost_transfs_rmdir(struct dentry *pdentry, struct dentry *dentry)
{
	int ret;

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	inode_lock(pdentry->d_inode);
	/*
	 * d_delete will be called in vfs_rmdir/vfs_unlink,
	 * d_delete will do dentry_unlink_inode when dentry's refcnt was 1.
	 * So should dget here, to make sure:
	 * a. dentry's refcnt == 1, when hmfs has released dentry
	 * b. dentry's refcnt > 1, when hmfs is using this dentry
	 * */
	dget(dentry);
	/* hmfs's vfs will also check whether dir is a mountpoint and set inode to S_DEAD */
	ret = vfs_rmdir(pdentry->d_inode, dentry);
	dput(dentry);
	inode_unlock(pdentry->d_inode);

	return ret;
}

/*
 * Caller should make sure lock/unlock parent_node and inc/dec mnt_writers.
 * (hmfs's vfs will do lock/unlock parent and inc/dec)
 */
static int devhost_transfs_create(struct dentry *pdentry, mode_t mode, const char *dname,
				  struct transfs_dn_meta_info_t *dn_meta_out)
{
	struct dentry *dentry = NULL;
	int ret;

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	ret = transfs_alloc_dentry(pdentry, dname, &dentry);
	if (ret) {
		return ret;
	}

	/*
	 * "want_excl" unused now
	 * when NFS was used, want_excl should be passed from hmfs's vfs
	 */
	ret = vfs_create(pdentry->d_inode, dentry, (umode_t)mode, true);
	if (ret) {
		pr_debug("create %s failed, err:%d\n", dentry->d_name.name, ret);
		dput(dentry);
		return ret;
	}

	igrab(dentry->d_inode);
	dn_meta_out->dentry = dentry;
	dn_meta_out->inode = dentry->d_inode;
	dn_meta_out->ino = (uint32_t)dentry->d_inode->i_ino;

	return 0;
}

/*
 * hmfs's vfs will do lock/unlock parent and inc/dec mount_entry'r refcnt
 */
static int devhost_transfs_unlink(struct dentry *pdentry, struct dentry *dentry)
{
	int ret;
	struct inode *delegated_inode = NULL;

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	inode_lock(pdentry->d_inode);
	/* do dget same with rmdir */
	dget(dentry);
	ret = vfs_unlink(pdentry->d_inode, dentry, &delegated_inode);
	dput(dentry);
	inode_unlock(pdentry->d_inode);

	return ret;
}

static ssize_t transfs_do_iter_rw(struct file *filp, struct iov_iter *iter,
				  loff_t *ppos, int type)
{
	struct kiocb kiocb;
	ssize_t ret;

	init_sync_kiocb(&kiocb, filp);
	kiocb.ki_pos = (ppos ? *ppos : 0);
	/* now vfs only support sync aio */
	kiocb.ki_complete = NULL;

	if (type == READ) {
		ret = filp->f_op->read_iter(&kiocb, iter);
	} else {
		ret = filp->f_op->write_iter(&kiocb, iter);
	}

	BUG_ON(ret == -EIOCBQUEUED);

	if (ret > 0 && ppos) {
		*ppos = kiocb.ki_pos;
	}

	return ret;
}

static ssize_t transfs_do_loop_rw(struct file *filp, struct iov_iter *iter,
				  loff_t *ppos, int type)
{
	ssize_t ret = 0;

	while (iov_iter_count(iter)) {
		struct iovec iovec = iov_iter_iovec(iter);
		ssize_t nr;

		if (type == READ) {
			nr = filp->f_op->read(filp, iovec.iov_base, iovec.iov_len, ppos);
		} else {
			nr = filp->f_op->write(filp, iovec.iov_base, iovec.iov_len, ppos);
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

static ssize_t transfs_vfs_write(struct file *file, const char __user *p,
				 size_t count, loff_t *pos, bool is_usr)
{
	if (file->f_op->write) {
		return file->f_op->write(file, p, count, pos);
	} else if (file->f_op->write_iter) {
		struct iovec iov = {
			.iov_base = (void __user *)p,
			.iov_len = count
		};
		struct iov_iter iter = {0};

		iov_iter_init(&iter, WRITE, &iov, 1, count);
		if (is_usr) {
			iter.type |= ITER_IOVEC;
		} else {
			iter.type |= ITER_KVEC;
		}

		return transfs_do_iter_rw(file, &iter, pos, WRITE);
	}

	return -EINVAL;
}

static ssize_t transfs_vfs_read(struct file *file, char __user *p,
				size_t count, loff_t *pos, bool is_usr)
{
	if (file->f_op->read) {
		return file->f_op->read(file, p, count, pos);
	} else if (file->f_op->read_iter) {
		struct iovec iov = {
			.iov_base = (void __user *)p,
			.iov_len = count
		};
		struct iov_iter iter = {0};

		iov_iter_init(&iter, READ, &iov, 1, count);
		if (is_usr) {
			iter.type |= ITER_IOVEC;
		} else {
			iter.type |= ITER_KVEC;
		}

		return transfs_do_iter_rw(file, &iter, pos, READ);
	}

	return -EINVAL;
}

static inline void transfs_sync_check(struct file *file)
{
	/*
	 * Write back data and metadata to disk.
	 * zero means metadata and data is written.
	 */
	vfs_fsync(file, 0);
	/*
	 * Invalidate all the unlocked pages of one inode.
	 * will not invalidate pages which are dirty, locked,
	 * under writeback or mapped into pagetables.
	 * invalidate pages from offset 0 to -1(means all pages)
	 */
	invalidate_mapping_pages(file->f_inode->i_mapping, 0, -1);
}

static long long devhost_transfs_write(struct file *file, uint64_t pos,
				       const void *src, size_t len)
{
	ssize_t ret;
	loff_t ppos = (loff_t)pos;

	ret = transfs_vfs_write(file, (char __user *)src, len, &ppos, true);
	if (ret < 0) {
		pr_debug("write failed, err:%zd\n", ret);
		return ret;
	}

	transfs_sync_check(file);

	return ret;
}

static long long devhost_transfs_read(struct file *file, uint64_t pos,
				      const void *src, size_t len)
{
	ssize_t ret;
	loff_t ppos = (loff_t)pos;

	ret = transfs_vfs_read(file, (char __user *)src, len, &ppos, true);
	if (ret < 0) {
		pr_err("read failed, err:%zd\n", ret);
		return ret;
	}

	transfs_sync_check(file);

	return (long long)ret;
}

static ssize_t devhost_transfs_write_iter(struct file *file, uint64_t pos,
					  struct iovec *iov, int nr_segs, size_t count)
{
	ssize_t ret;
	loff_t ppos = (loff_t)pos;
	struct iov_iter iter = {
		.type = WRITE | ITER_IOVEC,
		.iov_offset = 0,
		.count = count,
		.iov = iov,
		.nr_segs = nr_segs,
	};

	if (file->f_op->write_iter != NULL) {
		ret = transfs_do_iter_rw(file, &iter, &ppos, WRITE);
	} else if (file->f_op->write != NULL) {
		ret = transfs_do_loop_rw(file, &iter, &ppos, WRITE);
	} else {
		ret = -EINVAL;
	}

	if (ret < 0) {
		pr_debug("write iter failed, err:%zd\n", ret);
		return ret;
	}

	transfs_sync_check(file);

	return ret;
}

static int devhost_transfs_fsync(struct file *file)
{
	return vfs_fsync(file, 0);
}

/*
 * Caller should make sure lock/unlock parent_node and inc/dec mnt_writers.
 * (hmfs's vfs will do lock/unlock parent and inc/dec)
 */
static int devhost_transfs_mknod(struct dentry *pdentry, unsigned int mode,
				 unsigned long long devno, const char *dname,
				 struct transfs_dn_meta_info_t *dn_meta_out)
{
	int ret;
	struct dentry *dentry = NULL;

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	ret = transfs_alloc_dentry(pdentry, dname, &dentry);
	if (ret) {
		return ret;
	}

	ret = vfs_mknod(pdentry->d_inode, dentry, mode, devno);
	if (ret) {
		pr_debug("mknod %s failed, err:%d\n", dentry->d_name.name, ret);
		dput(dentry);
		return ret;
	}

	igrab(dentry->d_inode);
	dn_meta_out->dentry = dentry;
	dn_meta_out->inode = dentry->d_inode;
	dn_meta_out->ino = (uint32_t)dentry->d_inode->i_ino;

	return 0;
}

/*
 * hmfs lock/unlock parent_node and inc/dec mnt_writers.
 */
static int devhost_transfs_link(struct dentry *old_dentry, struct dentry *new_pdentry,
				const char *dname, struct dentry **res_new_dentry)
{
	int ret;
	struct dentry *new_dentry = NULL;
	struct inode *delegated_inode = NULL;

	if (new_pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	ret = transfs_alloc_dentry(new_pdentry, dname, &new_dentry);
	if (ret) {
		return ret;
	}

	/* hmfs's vfs will also lock old_dentry->d_inode */
	inode_lock(new_pdentry->d_inode);
	ret = vfs_link(old_dentry, new_pdentry->d_inode, new_dentry, &delegated_inode);
	inode_unlock(new_pdentry->d_inode);
	if (ret) {
		pr_debug("link %s to %s failed, err:%d\n", new_dentry->d_name.name,
			 old_dentry->d_name.name, ret);
		dput(new_dentry);
		return ret;
	}

	*res_new_dentry = new_dentry;

	return 0;
}

/*
 * Caller should make sure lock/unlock parent_node and inc/dec mnt_writers.
 * (hmfs's vfs will do lock/unlock parent and inc/dec)
 */
static int devhost_transfs_symlink(struct dentry *pdentry, const char *dname,
				   const char *path,
				   struct transfs_dn_meta_info_t *dn_meta_out)
{
	int ret;
	struct dentry *dentry = NULL;

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	inode_lock(pdentry->d_inode);
	ret = transfs_alloc_dentry(pdentry, dname, &dentry);
	if (ret) {
		inode_unlock(pdentry->d_inode);
		return ret;
	}

	ret = vfs_symlink(pdentry->d_inode, dentry, path);
	if (ret) {
		pr_debug("symlink %s to %s failed, err:%d\n",
			 path, dentry->d_name.name, ret);
		dput(dentry);
		inode_unlock(pdentry->d_inode);
		return ret;
	}

	igrab(dentry->d_inode);
	dn_meta_out->dentry = dentry;
	dn_meta_out->inode = dentry->d_inode;
	dn_meta_out->ino = (uint32_t)dentry->d_inode->i_ino;

	inode_unlock(pdentry->d_inode);
	return 0;
}

static int devhost_transfs_statfs(struct vfsmount *vmnt, struct dentry *dentry,
				  struct transfs_statfs *transfs_st)
{
	int ret;
	struct kstatfs st = {0};
	struct path path = {
		.mnt = vmnt,
		.dentry = dentry
	};

	if (dentry->d_sb == NULL) {
		return -ENOENT;
	}

	if (dentry->d_sb->s_op->statfs == NULL) {
		return -ENOSYS;
	}

	ret = vfs_statfs(&path, &st);
	if (ret) {
		pr_debug("statfs %s failed, err:%d\n", dentry->d_name.name, ret);
		return ret;
	}

	transfs_st->f_type = st.f_type;
	transfs_st->f_bsize = st.f_bsize;
	transfs_st->f_blocks = st.f_blocks;
	transfs_st->f_bfree = st.f_bfree;
	transfs_st->f_bavail = st.f_bavail;
	transfs_st->f_files = st.f_files;
	transfs_st->f_ffree = st.f_ffree;
	transfs_st->f_fsid.val[0] = st.f_fsid.val[0];
	transfs_st->f_fsid.val[1] = st.f_fsid.val[1];
	transfs_st->f_namelen = st.f_namelen;
	transfs_st->f_frsize = st.f_frsize;
	transfs_st->f_flags = st.f_flags;
	ret = memset_s(transfs_st->f_spare, sizeof(transfs_st->f_spare),
		       0, sizeof(transfs_st->f_spare));
	if (ret != 0) {
		return -EFAULT;
	}

	return 0;
}

static int do_transfs_rename(struct inode *old_dir, struct dentry *old_dentry,
			     struct inode *new_dir, struct dentry *new_dentry,
			     unsigned int flags)
{
	int ret;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
	ret = old_dir->i_op->rename(old_dir, old_dentry, new_dir, new_dentry, flags);
#else
	if (old_dir->i_op->rename2 == NULL) {
		ret = old_dir->i_op->rename(old_dir, old_dentry, new_dir, new_dentry);
	} else {
		ret = old_dir->i_op->rename2(old_dir, old_dentry, new_dir, new_dentry, flags);
	}
#endif

	return ret;
}

static void transfs_lock_nodes(struct inode *source, struct inode *target, bool is_dir)
{
	if (!is_dir) {
		lock_two_nondirectories(source, target);
	} else if (target != NULL) {
		inode_lock(target);
	}
}

static void transfs_unlock_nodes(struct inode *source, struct inode *target, bool is_dir)
{
	if (!is_dir) {
		unlock_two_nondirectories(source, target);
	} else if (target != NULL) {
		inode_unlock(target);
	}
}

static int devhost_transfs_rename(struct dentry *old_pdentry, struct dentry *old_dentry,
				  struct dentry *new_pdentry, struct dentry *new_dentry,
				  const char *dname)
{
	int ret;
	struct dentry *dentp = NULL;
	struct dentry *trap = NULL;
	bool need_new = (new_dentry == NULL) ? true : false;
	bool is_dir = d_is_dir(old_dentry);

	if (old_pdentry->d_inode == NULL) {
		return -ENOENT;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
	if (old_pdentry->d_inode->i_op->rename == NULL) {
		return -EPERM;
	}
#else
	if (old_pdentry->d_inode->i_op->rename == NULL &&
	    old_pdentry->d_inode->i_op->rename2 == NULL) {
		return -EPERM;
	}
#endif

	/* get target dentry */
	if (need_new) {
		ret = transfs_alloc_dentry(new_pdentry, dname, &dentp);
		if (ret) {
			return ret;
		}
	} else {
		dentp = new_dentry;
		dget(dentp);
	}

	trap = lock_rename(new_pdentry, old_pdentry);
	if (trap == old_dentry || trap == dentp) {
		pr_debug("source or target should not be ancestor of the other\n");
		unlock_rename(new_pdentry, old_pdentry);
		dput(dentp);
		return -EINVAL;
	}

	transfs_lock_nodes(old_dentry->d_inode, dentp->d_inode, is_dir);

	ret = do_transfs_rename(old_pdentry->d_inode, old_dentry,
				new_pdentry->d_inode, dentp, 0);
	if (ret) {
		pr_debug("rename %s to %s failed, err:%d\n", old_dentry->d_name.name,
			 dentp->d_name.name, ret);
		transfs_unlock_nodes(old_dentry->d_inode, dentp->d_inode, is_dir);
		unlock_rename(new_pdentry, old_pdentry);
		dput(dentp);
		return ret;
	}

	d_move(old_dentry, dentp);

	transfs_unlock_nodes(old_dentry->d_inode, dentp->d_inode, is_dir);

	dput(dentp);

	unlock_rename(new_pdentry, old_pdentry);

	return 0;
}

static int transfs_set_prevdirent(struct getdents_callback *buf, loff_t offset)
{
	struct linux_dirent __user *prev = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	struct linux_dirent __user *dirent = NULL;
	int prev_reclen = 0;
	int reclen = 0;

	prev_reclen = buf->prev_reclen;
	if (prev_reclen && signal_pending(current)) {
		return -EINTR;
	}
	dirent = buf->current_dir;
	prev = (void __user *)dirent - prev_reclen;
	reclen = sizeof(struct transfs_dirent);
	if (!user_write_access_begin(prev, reclen + prev_reclen)) {
		buf->error = -EFAULT;
		return -EFAULT;
	} else {
		prev->d_off = offset;
	}
#else
	prev = buf->previous;
	if (prev != NULL) {
		if (signal_pending(current)) {
			return -EINTR;
		}
		prev->d_off = offset;
	}
#endif

	return 0;
}

static int transfs_filldir(struct dir_context *ctx, const char *name, int namlen,
			   loff_t offset, u64 ino, unsigned int d_type)
{
	struct linux_dirent __user *dirent = NULL;
	struct getdents_callback *buf = NULL;
	unsigned long d_ino;
	int reclen;
	int err;

	buf = container_of(ctx, struct getdents_callback, ctx);
	buf->error = -EINVAL;

	reclen = sizeof(struct transfs_dirent);
	if (reclen > buf->count) {
		return -EINVAL;
	}

	if (ino > ULONG_MAX) {
		buf->error = -EOVERFLOW;
		return -EOVERFLOW;
	}

	d_ino = ino;

	err = transfs_set_prevdirent(buf, offset);
	if (err) {
		return err;
	}

	dirent = buf->current_dir;
	dirent->d_off = 0;
	dirent->d_ino = d_ino;
	dirent->d_reclen = reclen;
	err = memcpy_s(dirent->d_name, DIRENT_DNAME_MAX_LEN, name, namlen + 1);
	if (err) {
		return -EFAULT;
	}
	*((unsigned char *)dirent + reclen - 1) = (unsigned char)d_type;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	buf->prev_reclen = reclen;
#else
	buf->previous = dirent;
#endif
	buf->current_dir = (void __user *)dirent + reclen;
	buf->count -= reclen;

	return 0;
}

static int transfs_fill_dirents(struct linux_dirent *current_dir,
				struct linux_dirent *dir_linux,
				struct transfs_dirent *dirbuf,
				size_t size, int *dir_num)
{
	int ret = 0;
	size_t buf_len = 0;

	while (buf_len + sizeof(struct transfs_dirent) <= size) {
		struct transfs_dirent t_dir = {0};
		if (current_dir == dir_linux) {
			break;
		}
		t_dir.d_ino = dir_linux->d_ino;
		t_dir.d_off = dir_linux->d_off;
		t_dir.d_type = *((unsigned char *)dir_linux + dir_linux->d_reclen - 1);
		t_dir.d_name_len = strlen(dir_linux->d_name);
		ret = memcpy_s(t_dir.d_name, DIRENT_DNAME_MAX_LEN,
			       dir_linux->d_name, strlen(dir_linux->d_name) + 1);
		if (ret) {
			return -EFAULT;
		}
		ret = copy_to_user(dirbuf, &t_dir, sizeof(struct transfs_dirent));
		if (ret) {
			return -EFAULT;
		}
		dirbuf++;
		buf_len += sizeof(struct transfs_dirent);
		(*dir_num)++;
		dir_linux = (struct linux_dirent *)((void __user *)dir_linux +
							dir_linux->d_reclen);
	}

	return ret;
}

static void transfs_update_file_status(struct file *file, loff_t pos)
{
	if (file->f_pos == pos) {
		return;
	}

	file->f_pos = pos;
	file->f_version = 0;
}

static int devhost_transfs_readdir(struct file *file, uint64_t *pos, void *buf,
				   size_t size)
{
	int ret;
	int dir_num = 0;
	void *dir = NULL;
	uint64_t fpos = *pos;
	struct transfs_dirent *dirbuf = buf;
	struct linux_dirent *dir_linux = NULL;
	struct linux_dirent *lastdirent = NULL;
	struct getdents_callback dents_buf = {
		.ctx.actor = transfs_filldir,
	};

	dir = liblinux_pal_calloc(1, size);
	if (dir == NULL) {
		return -ENOMEM;
	}

	/* get target number dirents */
	transfs_update_file_status(file, fpos);
	dents_buf.count = size;
	dents_buf.current_dir = (struct linux_dirent __user *)dir;
	ret = iterate_dir(file, &(dents_buf.ctx));
	if (ret) {
		liblinux_pal_free(dir);
		return ret;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	if (dents_buf.prev_reclen) {
		lastdirent = (void __user *)dents_buf.current_dir - dents_buf.prev_reclen;
		lastdirent->d_off = dents_buf.ctx.pos;
	}
#else
	if (dents_buf.previous) {
		lastdirent = dents_buf.previous;
		lastdirent->d_off = dents_buf.ctx.pos;
	}
#endif

	dir_linux = dir;
	ret = transfs_fill_dirents(dents_buf.current_dir, dir_linux, dirbuf, size,
				   &dir_num);
	if (ret) {
		liblinux_pal_free(dir);
		return ret;
	}

	liblinux_pal_free(dir);

	*pos = file->f_pos;

	return dir_num;
}

static void transfs_fill_inode(struct trans_node_meta *buf, struct dentry *dentry)
{
	buf->dev_id = (dev_t)dentry->d_inode->i_sb->s_dev;
	buf->index = (uint32_t)dentry->d_inode->i_ino;
	buf->lnkcnt = (unsigned)dentry->d_inode->__i_nlink;
	buf->uid = (uid_t)dentry->d_inode->i_uid.val;
	buf->gid = (gid_t)dentry->d_inode->i_gid.val;
	buf->mode = (unsigned)dentry->d_inode->i_mode;
	buf->flags = (unsigned)dentry->d_inode->i_flags;
	buf->devno = (dev_t)dentry->d_inode->i_rdev;
	buf->size = (uint64_t)dentry->d_inode->i_size;
	buf->blksize = (unsigned long)i_blocksize(dentry->d_inode);
	buf->blocks = (long)dentry->d_inode->i_blocks;
	buf->a_tv_sec = (int64_t)dentry->d_inode->i_atime.tv_sec;
	buf->a_tv_nsec = (long)dentry->d_inode->i_atime.tv_nsec;
	buf->c_tv_sec = (int64_t)dentry->d_inode->i_ctime.tv_sec;
	buf->c_tv_nsec = (long)dentry->d_inode->i_ctime.tv_nsec;
	buf->m_tv_sec = (int64_t)dentry->d_inode->i_mtime.tv_sec;
	buf->m_tv_nsec = (long)dentry->d_inode->i_mtime.tv_nsec;
}

static int devhost_transfs_lookup(struct dentry *pdentry, const char *dname,
				  void *meta, struct transfs_dn_meta_info_t *dn_meta_out)
{
	struct dentry *dentry = NULL;
	struct trans_node_meta buf = {0};
	int ret;

	if (dname == NULL) {
		pr_err("invalid dname (NULL)\n");
		return -EFAULT;
	}

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	inode_lock(d_inode(pdentry));
	dentry = lookup_one_len(dname, pdentry, strlen(dname));
	if (IS_ERR(dentry)) {
		pr_debug("lookup %s in %s failed, err:%d\n",
			 dname, pdentry->d_name.name, ret);
		inode_unlock(d_inode(pdentry));
		return PTR_ERR(dentry);
	}
	inode_unlock(d_inode(pdentry));

	/* no such file/dir, just return */
	if (dentry->d_inode == NULL) {
		dput(dentry);
		return -ENOENT;
	}

	/* fill the metadata in inode */
	transfs_fill_inode(&buf, dentry);

	ret = copy_to_user(meta, &buf, sizeof(struct trans_node_meta));
	if (ret) {
		pr_warn("copy_to_user failed, ret:%d\n", ret);
		dput(dentry);
		return -EFAULT;
	}

	igrab(dentry->d_inode);
	dn_meta_out->dentry = dentry;
	dn_meta_out->inode = dentry->d_inode;

	return 0;
}

static int devhost_transfs_setattr(struct dentry *dentry, struct transfs_vattr *vattr,
				   uint64_t size)
{
	struct iattr iattr = {0};
	int ret;
	umode_t ia_mode;

	if (dentry->d_inode == NULL) {
		return -ENOENT;
	}

	/* init iattr */
	ia_mode = d_inode(dentry)->i_mode;
	ia_mode &= S_IFMT;
	ia_mode |= vattr->mode & ~S_IFMT;
	iattr.ia_valid = vattr->valid;
	iattr.ia_mode = ia_mode;
	iattr.ia_uid.val = vattr->uid;
	iattr.ia_gid.val = vattr->gid;
	iattr.ia_size = size;
	iattr.ia_atime.tv_sec = vattr->atime.tv_sec;
	iattr.ia_atime.tv_nsec = vattr->atime.tv_nsec;
	iattr.ia_mtime.tv_sec = vattr->mtime.tv_sec;
	iattr.ia_mtime.tv_nsec = vattr->mtime.tv_nsec;
	iattr.ia_ctime.tv_sec = vattr->ctime.tv_sec;
	iattr.ia_ctime.tv_nsec = vattr->ctime.tv_nsec;
	iattr.ia_file = NULL;

	inode_lock(dentry->d_inode);
	if (dentry->d_inode->i_op->setattr != NULL) {
		ret = dentry->d_inode->i_op->setattr(dentry, &iattr);
	} else {
		ret = simple_setattr(dentry, &iattr);
	}
	inode_unlock(dentry->d_inode);
	if (ret) {
		pr_debug("setattr %s failed, err:%d\n", dentry->d_name.name, ret);
		return ret;
	}

	return 0;
}

static int devhost_transfs_readlink(struct dentry *dentry, char * __user buf, size_t bufsize)
{
	int ret;

	if (dentry->d_inode == NULL) {
		return -ENOENT;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
	ret = vfs_readlink(dentry, buf, (int)bufsize);
#else
	if (dentry->d_inode->i_op->readlink == NULL) {
		return -EINVAL;
	}

	ret = dentry->d_inode->i_op->readlink(dentry, buf, (int)bufsize);
#endif
	invalidate_mapping_pages(dentry->d_inode->i_mapping, 0, -1);
	if (ret < 0) {
		pr_debug("readlink %s failed, err:%d\n", dentry->d_name.name, ret);
	}

	return ret;
}

static int devhost_transfs_open(struct vfsmount *vmnt, struct dentry *dentry, unsigned int f_modes,
				unsigned int f_flags, struct file **res_file)
{
	struct file *file = NULL;
	int ret;
	struct vfsmount *mnt = NULL;

	if (f_flags & O_DIRECT) {
		if (dentry->d_inode->i_mapping == NULL ||
		    dentry->d_inode->i_mapping->a_ops == NULL ||
		    dentry->d_inode->i_mapping->a_ops->direct_IO == NULL) {
			return -EINVAL;
		}
	}

	mnt = mntget(vmnt);
	ret = transfs_alloc_file(mnt, dentry, (int)f_flags, &file);
	if (ret) {
		mntput(mnt);
		return -ENOMEM;
	}

	dget(dentry);
	if (file->f_op->open) {
		ret = file->f_op->open(dentry->d_inode, file);
		if (ret) {
			pr_debug("open %s failed, err:%d\n", dentry->d_name.name, ret);
			put_filp(file);
			mntput(mnt);
			return ret;
		}
	}

	*res_file = file;
	kmemleak_not_leak((void *)file);
	return 0;
}

static int devhost_transfs_fallocate(struct file *file, unsigned int mode, uint64_t pos,
				     uint64_t len)
{
	int ret;

	if (file->f_op->fallocate == NULL) {
		return -EOPNOTSUPP;
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	/*
	 * Repeated block allocation may occur when the ldk of a version earlier
	 * than 5.10 executes fallocate. In this example, the vfs_fsync_range is
	 * used to save the data in the corresponding range to the disk.
	 */
	(void)vfs_fsync_range(file, (loff_t)pos, (loff_t)len, 0);
#endif
	ret = file->f_op->fallocate(file, (int)mode, (loff_t)pos, (loff_t)len);
	if (ret < 0) {
		pr_debug("fallocate failed, err:%d\n", ret);
		return ret;
	}

	return 0;
}

static void transfs_fill_inode_with_stat(struct trans_node_meta *buf, struct kstat *stat)
{
	buf->dev_id = (unsigned long)stat->dev;
	buf->index = (uint32_t)stat->ino;
	buf->lnkcnt = (unsigned)stat->nlink;
	buf->uid = (uid_t)stat->uid.val;
	buf->gid = (gid_t)stat->gid.val;
	buf->mode = (unsigned)stat->mode;
	buf->devno = (dev_t)stat->rdev;
	buf->size = (uint64_t)stat->size;
	buf->blksize = (unsigned long)stat->blksize;
	buf->blocks = (long)stat->blocks;
	buf->a_tv_sec = (int64_t)stat->atime.tv_sec;
	buf->a_tv_nsec = (long)stat->atime.tv_nsec;
	buf->c_tv_sec = (int64_t)stat->ctime.tv_sec;
	buf->c_tv_nsec = (long)stat->ctime.tv_nsec;
	buf->m_tv_sec = (int64_t)stat->mtime.tv_sec;
	buf->m_tv_nsec = (long)stat->mtime.tv_nsec;
}

static int devhost_transfs_getattr(struct vfsmount *vmnt, struct dentry *dentry,
				   void *meta)
{
	int ret = 0;
	struct kstat stat = {0};
	struct path path = {
		.mnt = vmnt,
		.dentry = dentry
	};
	struct trans_node_meta buf = {0};

	if (dentry->d_inode == NULL) {
		return -ENOENT;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
	/* statx not supported now, so directly pass STATX_BASIC_STATS && 0 */
	ret = vfs_getattr(&path, &stat, STATX_BASIC_STATS, 0);
#else
	ret = vfs_getattr(&path, &stat);
#endif
	if (ret < 0) {
		pr_debug("getattr %s failed, err:%d\n", dentry->d_name.name, ret);
		return ret;
	}

	transfs_fill_inode_with_stat(&buf, &stat);
	ret = copy_to_user(meta, &buf, sizeof(struct trans_node_meta));
	if (ret) {
		pr_warn("copy_to_user failed, ret:%d\n", ret);
		return -EFAULT;
	}

	return 0;
}

static long long devhost_transfs_listxattr(struct dentry *dentry, char *buffer, size_t size)
{
	ssize_t ret;
	int err;
	char *tmp_buf = NULL;

	if (buffer != NULL) {
		tmp_buf = liblinux_pal_calloc(1, size);
		if (tmp_buf == NULL) {
			return -ENOMEM;
		}
	}

	if (dentry->d_inode == NULL) {
		liblinux_pal_free(tmp_buf);
		return -ENOENT;
	}

	if (dentry->d_inode->i_op->listxattr == NULL) {
		liblinux_pal_free(tmp_buf);
		return -EOPNOTSUPP;
	}

	ret = dentry->d_inode->i_op->listxattr(dentry, tmp_buf, size);
	if (ret < 0) {
		liblinux_pal_free(tmp_buf);
		pr_debug("listxattr %s failed, err:%zd\n", dentry->d_name.name, ret);
		return ret;
	}

	if (buffer != NULL) {
		err = copy_to_user(buffer, tmp_buf, ret);
		if (err) {
			liblinux_pal_free(tmp_buf);
			pr_warn("liblinux copy to user failed\n");
			return -EFAULT;
		}
	}
	liblinux_pal_free(tmp_buf);

	return ret;
}

static long long devhost_transfs_getxattr(struct dentry *dentry, int prefix, const char *name,
					  char *value, size_t size)
{
	ssize_t ret;
	int err;
	char *value_buf = NULL;

	if (value != NULL) {
		if (size > XATTR_SIZE_MAX) {
			size = XATTR_SIZE_MAX;
		}
		value_buf = liblinux_pal_calloc(1, size);
		if (value_buf == NULL) {
			return -ENOMEM;
		}
	}

	if (dentry->d_inode == NULL) {
		ret = -ENOENT;
		goto out;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
#if LINUX_VERSION_CODE == KERNEL_VERSION(5,10,43)
	ret = __vfs_getxattr(dentry, dentry->d_inode, name, value_buf, size, XATTR_NOSECURITY);
#else
	ret = __vfs_getxattr(dentry, dentry->d_inode, name, value_buf, size);
#endif
#else
	if (dentry->d_inode->i_op->getxattr == NULL) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = dentry->d_inode->i_op->getxattr(dentry, name, value_buf, size);
#endif
	if (ret < 0) {
		pr_debug("getxattr %s failed, err:%zd\n", dentry->d_name.name, ret);
		goto out;
	}

	if (value != NULL) {
		err = copy_to_user(value, value_buf, ret);
		if (err) {
			pr_warn("liblinux copy to user failed\n");
			ret = -EFAULT;
		}
	}

out:
	liblinux_pal_free(value_buf);

	return ret;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,9,0)
static ssize_t transfs_setxattr(struct dentry *dentry, const char *name,
				const char *value, size_t size, int flags)
{
	if (dentry->d_inode->i_op->setxattr == NULL) {
		return (ssize_t)(-EOPNOTSUPP);
	}

	return dentry->d_inode->i_op->setxattr(dentry, name, value, size, flags);
}

static ssize_t transfs_removexattr(struct dentry *dentry, const char *name)
{
	if (dentry->d_inode->i_op->removexattr == NULL) {
		return (ssize_t)(-EOPNOTSUPP);
	}

	return dentry->d_inode->i_op->removexattr(dentry, name);
}
#endif

static long long devhost_transfs_setxattr(struct dentry *dentry, int prefix, const char *name,
					  const char *value, size_t size, int flags)
{
	ssize_t ret;

	if (dentry->d_inode == NULL) {
		return -ENOENT;
	}

	inode_lock(dentry->d_inode);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,9,0)
	if (value != NULL) {
		ret = __vfs_setxattr(dentry, dentry->d_inode, name, value, size, flags);
	} else {
		ret = __vfs_removexattr(dentry, name);
	}
#else
	if (value != NULL) {
		ret = transfs_setxattr(dentry, name, value, size, flags);
	} else {
		ret = transfs_removexattr(dentry, name);
	}
#endif
	inode_unlock(dentry->d_inode);

	return ret;
}

static int devhost_transfs_tmpfile(struct dentry *pdentry, mode_t mode, const char *dname,
				   struct transfs_dn_meta_info_t *dn_meta_out)
{
	int ret;
	struct dentry *dentry = NULL;
	struct qstr filename = QSTR_INIT(dname, strlen(dname));

	if (pdentry->d_inode == NULL) {
		return -ENOENT;
	}

	if (pdentry->d_inode->i_op->tmpfile == NULL) {
		return -EOPNOTSUPP;
	}

	dentry = d_alloc(pdentry, &filename);
	if (dentry == NULL) {
		return -ENOMEM;
	}

	ret = pdentry->d_inode->i_op->tmpfile(pdentry->d_inode, dentry, (umode_t)mode);
	if (ret) {
		pr_debug("tmpfile %s failed, err:%d\n", dentry->d_name.name, ret);
		dput(dentry);
		return ret;
	}

	igrab(dentry->d_inode);
	dn_meta_out->dentry = dentry;
	dn_meta_out->inode = dentry->d_inode;
	dn_meta_out->ino = (uint32_t)dentry->d_inode->i_ino;

	return 0;
}

static int devhost_transfs_sync(struct super_block *sb)
{
	int ret;

	ret = sync_filesystem(sb);
	if (ret < 0) {
		pr_debug("sync_filesystem failed, err:%d\n", ret);
		return ret;
	}

	return 0;
}

static int devhost_transfs_drop_caches(struct super_block *sb, unsigned int mode, int max_nr)
{
#if (LINUX_VERSION_CODE != KERNEL_VERSION(4,14,61)) && (defined(CONFIG_LDK_FS_MM_CACHE))
	unsigned long cnt;
	int max_size;

	/* sb will be used later */
	if (sb == NULL || max_nr < -1) {
		return -EINVAL;
	}

	if (max_nr > (INT_MAX / (int)(unsigned int)PAGE_SIZE)) {
		pr_info("except to drop too much memory in devhost trans, %d pages\n", max_nr);
		max_size = -1;
	} else {
		max_size = (max_nr == -1 ? max_nr : max_nr * (int)(unsigned int)PAGE_SIZE);
	}

	transfs_drop_caches(mode);
	synchronize_rcu();
	cnt = lnxbase_shrink_mem(max_size);
	cnt = cnt >> PAGE_SHIFT;

	/*
	 * Overflow may happen if we return cnt or INT_MAX.
	 *
	 * When max_nr is -1, we return 0, while numbers returned
	 * has no effect to this dropcache action.
	 * Or We return except numbers, to finish this dropcache.
	 */
	if (cnt > INT_MAX) {
		pr_info("devhost trans drop too much memory, %lu pages\n", cnt);
		cnt = (max_nr == -1 ? 0 : max_nr);
	}

	return (int)cnt;
#else
	return -EOPNOTSUPP;
#endif
}

static void devhost_transfs_close(struct file *file)
{
	/*
	 * For a file that has been opened, the fput needs to be used to
	 * end the life of the file. The fput invokes the release interface
	 * of FS to release the data constructed during the open process
	 * in FS.
	 */
	fput(file);
}

static int devhost_transfs_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;

	if (file->f_op->unlocked_ioctl == NULL) {
		return -ENOTTY;
	}

	ret = file->f_op->unlocked_ioctl(file, cmd, arg);
	if (ret) {
		pr_debug("ioctl fs failed, err:%d\n", ret);
		return ret;
	}

	return 0;
}

static long long devhost_transfs_direct_IO(struct file *file, uint64_t offset, void *buf,
					   size_t len, unsigned int type)
{
	ssize_t ret;
	struct kiocb kiocb;
	struct iov_iter iter;
	struct iovec iov = {
		.iov_base = (void __user *)buf,
		.iov_len = len
	};
	int io_ops;

	/* init iter, should be replaced ? */
	if (type == 0) {
		io_ops = READ;
	} else if (type == 1) {
		io_ops = WRITE;
	} else {
		return -EINVAL;
	}
	init_sync_kiocb(&kiocb, file);
	kiocb.ki_pos = offset;
	iov_iter_init(&iter, io_ops, &iov, 1, len);

	if (file->f_mapping == NULL) {
		return -ENOENT;
	}

	if (file->f_mapping->a_ops->direct_IO == NULL) {
		return -EINVAL;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
	ret = file->f_mapping->a_ops->direct_IO(&kiocb, &iter);
#else
	ret = file->f_mapping->a_ops->direct_IO(&kiocb, &iter, offset);
#endif
	if (ret < 0) {
		pr_debug("direct_IO failed, err:%zd\n", ret);
		return ret;
	}

	return 0;
}

static void devhost_transfs_kill_dentry(struct dentry *dentry)
{
	/* root_dentry cannot be killed here */
	if (dentry == dentry->d_sb->s_root) {
		return;
	}

	dput(dentry);
}

static void devhost_transfs_node_put(struct inode *inode)
{
	iput(inode);
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
static inline void set_page_count(struct page *page, int v)
{
	atomic_set(&page->_count, v);
}
#endif

#ifdef CONFIG_LDK_FS_MM_CACHE
static int transfs_copy_page_range(struct page *to, struct page *from,
				   size_t len, unsigned long long padding)
{
	char *vfrom, *vto;
	int err;

	BUG_ON(padding + len > PAGE_SIZE);
	vfrom = kmap_atomic(from);
	vto = kmap_atomic(to);
	err = memcpy_s(vto + padding, len, vfrom + padding, len);
	kunmap_atomic(vto);
	kunmap_atomic(vfrom);
	if (err != 0) {
		pr_warn("copy page data failed\n");
		return -EFAULT;
	}

	return 0;
}

static inline int transfs_copy_page(struct page *to, struct page *from)
{
	return transfs_copy_page_range(to, from, PAGE_SIZE, 0);
}

static inline size_t transfs_pg_count(unsigned long long offset, size_t size)
{
	unsigned long long padding = offset & (~PAGE_MASK);
	return ((size + padding + PAGE_SIZE - 1) >> PAGE_SHIFT);
}

static int transfs_wait_init_page(struct page *pg)
{
#define WAIT_PG_USECS 10
#define WAIT_PG_COUNT 100000
	int wait_count = 0;

	while (PageReserved(pg)) {
		udelay(WAIT_PG_USECS);
		yield();
		wait_count++;
		if (wait_count >= WAIT_PG_COUNT) {
			/*
			 * Wait for the reserved flag of the page for
			 * more than 1s and dump the page's information.
			 */
			dump_page(pg, "transfs wait page init timeout");
			return -ETIMEDOUT;
		}
	}

	(void)memset_s(pg, sizeof(*pg), 0, sizeof(struct page));
	set_page_zone(pg, ZONE_EXTERNAL);
	SetPageReserved(pg);
	set_page_count(pg, 1);
	page_mapcount_reset(pg);
#ifdef WANT_PAGE_VIRTUAL
	set_page_address(pg, page_to_virt(pg));
#endif
	return 0;
}

static int transfs_read_page_locked(struct file *file, struct page *old)
{
	int ret;

	ClearPageError(old);
	ret = file->f_mapping->a_ops->readpage(file, old);
	if (ret < 0) {
		pr_err("read: read old page failed\n");
	} else {
		lock_page(old);
		if (!PageUptodate(old)) {
			pr_err("read: find not uptodate page flags=%pGp\n",
			       &old->flags);
			ret = -EIO;
			SetPageError(old);
		}
	}

	return ret;
}

static int transfs_read_old_page(struct file *file, struct page *old, bool *need_retry)
{
	int ret;

	if (PageUptodate(old)) {
		return 0;
	}

	ret = transfs_read_page_locked(file, old);
	if (!PageUptodate(old) && (page_mapping(old) == NULL)) {
		*need_retry = true;
	}

	return ret;
}

static inline void __transfs_read_prepare_unlock_pages(unsigned long long *pfn, size_t cnt)
{
	int i;
	/*
	 * In the read process, when a page is inserted into the mapping,
	 * the page is in the locked state. When an error occurs, the
	 * pages inserted into the mapping need to be unlocked.
	 */
	for (i = 0; i < cnt; i++) {
		struct page *pg = pfn_to_page(pfn[i]);
		if (page_mapping(pg) != NULL) {
			unlock_page(pg);
		}
	}
}

static int __transfs_read_prepare_page(struct file *file, unsigned long long *pfn,
				       size_t size, unsigned long long offset)
{
	struct address_space *mapping = file->f_mapping;
	size_t cnt = transfs_pg_count(offset, size);
	int i;
	int ret = 0;

	BUG_ON((offset & (~PAGE_MASK)) != 0);

	/*
	 * refcount of pg:
	 * 0:
	 *   i.  page failed to add to file mapping;
	 *   ii. file mapping was already exist, and data was copied;
	 * 2: page was added to file mapping and LOCKED
	 */
	for (i = 0; i < cnt; i++) {
		struct page *old = NULL;
		struct page *pg = pfn_to_page(pfn[i]);

		ret = transfs_wait_init_page(pg);
		if (ret < 0) {
			break;
		}

retry:
		old = find_lock_page(mapping, (offset>>PAGE_SHIFT) + i);
		if (old == NULL) {
			ret = add_to_page_cache(pg, file->f_mapping,
						(offset>>PAGE_SHIFT) + i,
						GFP_KERNEL);
			if (ret == -EEXIST) {
				/*
				 * In the scenario of concurrent read processes, the -EEXIST error
				 * code may be returned when a page is added. In this case, try again.
				 */
				goto retry;
			}
			if (ret < 0) {
				pr_warn("read: add pages to pagecache failed, index=%lld+%d, ret=%d\n",
					offset >> PAGE_SHIFT, i, ret);
				put_page(pg);
				break;
			}
		} else {
			bool need_retry = false;
			ret = transfs_read_old_page(file, old, &need_retry);
			if (need_retry) {
				unlock_page(old);
				put_page(old);
				goto retry;
			}
			if (ret == 0) {
				ret = transfs_copy_page(pg, old);
			}
			unlock_page(old);
			put_page(old);
			put_page(pg);
			if (ret < 0) {
				break;
			}
		}
	}

	if (ret < 0) {
		__transfs_read_prepare_unlock_pages(pfn, cnt);
	}

	return ret;
}

static int __transfs_direct_map_pages_read(struct file *file,unsigned long long *pfn,
					   size_t cnt)
{
	int i;
	int ret = 0;
	struct address_space *mapping = file->f_mapping;

	for (i = 0; i < cnt; i++) {
		struct page *pg = pfn_to_page(pfn[i]);
		if (page_mapping(pg) == NULL) {
			continue;
		}
		if (ret < 0) {
			/*
			 * In the current process, all pages in the page
			 * array are locked. When an error occurs, the
			 * remaining pages in the array need to be unlocked.
			 * For pages in the I/O state, endio process will
			 * unlock the pages.
			 */
			unlock_page(pg);
			continue;
		}
		ret = mapping->a_ops->readpage(file, pg);
		if (ret < 0) {
			pr_warn("failed to readpage, ret=%d\n", ret);
			unlock_page(pg);
			/* Unlock the remaining locked pages in the page array */
			continue;
		}
	}

	return ret;
}

static ssize_t transfs_direct_map_page_read(struct file *file, unsigned long long *pfn,
					    size_t size, unsigned long long offset)
{
	struct blk_plug plug;
	size_t cnt = transfs_pg_count(offset, size);
	int i;
	int ret;

	blk_start_plug(&plug);
	ret = __transfs_read_prepare_page(file, pfn, size, offset);
	if (ret < 0) {
		goto out;
	}

	ret = __transfs_direct_map_pages_read(file, pfn, cnt);

out:
	blk_finish_plug(&plug);

	for (i = 0; i < cnt; i++) {
		struct page *pg = pfn_to_page(pfn[i]);
		if (page_mapping(pg) == NULL) {
			continue;
		}
		wait_on_page_locked(pg);
		if (!PageUptodate(pg)) {
			pr_warn("page not up-to-date, page flags=%pGp, zone:%d\n",
				&pg->flags, page_zonenum(pg));
			ret = -EIO;
		}

		lock_page(pg);
		if (page_has_private(pg) && try_to_release_page(pg, 0) == 0) {
			pr_warn("try_to free buffer failed, page flags=%pGp, zone:%d\n",
				&pg->flags, page_zonenum(pg));
			ret = -EIO;
		}
		delete_from_page_cache(pg);
		unlock_page(pg);
		put_page(pg);
	}

	return (ret < 0) ? (ssize_t)ret : (ssize_t)size;
}

static ssize_t transfs_direct_map_cache_read(struct file *file, unsigned long long *pfn,
					     size_t size, unsigned long long offset)
{
	ssize_t ret;
	ssize_t read_bytes = 0;
	size_t cnt = transfs_pg_count(offset, size);
	loff_t pos = (loff_t)offset;
	size_t remained_size = size;
	int i;

	BUG_ON((offset & (~PAGE_MASK)) != 0);

	for (i = 0; i < cnt; i++) {
		void *addr =  __va(PFN_PHYS(pfn[i]));
		size_t sz = min(remained_size, (size_t)PAGE_SIZE);

		ret = transfs_vfs_read(file, addr, sz, &pos, false);
		if (ret < 0) {
			pr_err("do vfs read error\n");
			return ret;
		}
		read_bytes += ret;
		remained_size -= sz;
	}

	return read_bytes;
}

#else
static ssize_t transfs_direct_map_page_read(struct file *file, unsigned long long *pfn,
					    size_t size, unsigned long long offset)
{
	return -EOPNOTSUPP;
}

static ssize_t transfs_direct_map_cache_read(struct file *file, unsigned long long *pfn,
					     size_t size, unsigned long long offset)
{
	return -EOPNOTSUPP;
}
#endif

static int devhost_transfs_io_read(struct vfsmount *vmnt, struct inode *inode,
				   char *buf, unsigned int size,
				   unsigned long long offset, bool direct_map_page)
{
	ssize_t ret;
	struct file *file = NULL;
	struct dentry dentry;
	loff_t pos = (loff_t)offset;

	/*
	 * The page in the HM cache cannot obtain file-related information.
	 * Therefore, when the I/O is transmitted to the devhost transfs,
	 * the file needs to be constructed to carry the current I/O.
	 */
	ret = memset_s(&dentry, sizeof(struct dentry), 0, sizeof(struct dentry));
	if (ret != 0) {
		return -EFAULT;
	}
	dentry.d_inode = inode;
	ret = transfs_alloc_file(vmnt, &dentry, O_RDONLY, &file);
	if (ret) {
		return -EINVAL;
	}

	if (!direct_map_page) {
		ret = transfs_vfs_read(file, buf, size, &pos, false);
		if (ret < 0) {
			pr_warn("read failed, err:%zd\n", ret);
		}
		transfs_sync_check(file);
	} else {
		ret = transfs_direct_map_page_read(file, (unsigned long long *)buf,
						   size, offset);
		if (ret < 0 && ret != -ETIMEDOUT) {
			pr_warn("direct map page read failed, err:%zd\n", ret);
		}
		if (ret == -ETIMEDOUT) {
			ret = transfs_direct_map_cache_read(file, (unsigned long long *)buf,
							    size, offset);
			if (ret < 0) {
				pr_warn("do cache read failed, err:%zd\n", ret);
			}
			transfs_sync_check(file);
		}
	}

	file->f_path.dentry = NULL;
	/* tmp file should be removed */
	put_filp(file);

	return (int)ret;
}

#ifdef CONFIG_LDK_FS_MM_CACHE
struct map_pg_write_mgr {
	/* pfn of pages to be written to disk */
	unsigned long long *pfn;
	/* records pages that already exist in mapping */
	struct page **old_pages;
	size_t pages_nr;
};

static int __transfs_write_prepare_page(struct file *file, struct map_pg_write_mgr *wr_mgr,
					size_t size, unsigned long long offset)
{
	struct address_space *mapping = file->f_mapping;
	size_t cnt = wr_mgr->pages_nr;
	int i;
	int ret = 0;

	/*
	 * refcount of pg:
	 * 0:
	 *   i.  page failed to add to file mapping;
	 *   ii. file mapping was already exist, and data will be copied between
	 *       `pagecache_write_{begin,end}`;
	 * 2: page was added to file mapping WITHOUT lock
	 */
	for (i = 0; i < cnt; i++) {
		struct page *old = NULL;
		struct page *pg = pfn_to_page(wr_mgr->pfn[i]);

		ret = transfs_wait_init_page(pg);
		if (ret < 0) {
			break;
		}

retry:
		old = find_lock_page(mapping, (offset>>PAGE_SHIFT) + i);
		if (old == NULL) {
			SetPageUptodate(pg);
			ret = add_to_page_cache(pg, file->f_mapping,
						(offset>>PAGE_SHIFT) + i,
						GFP_KERNEL);
			if (ret < 0) {
				pr_warn("write: add pages to pagecache failed, index=%lld+%d, ret=%d\n",
					offset >> PAGE_SHIFT, i, ret);
				put_page(pg);
				break;
			}
			unlock_page(pg);
		} else {
			bool need_retry = false;
			ret = transfs_read_old_page(file, old, &need_retry);
			if (need_retry) {
				unlock_page(old);
				put_page(old);
				goto retry;
			}
			/*
			 * keep refcount of old page until copy data between
			 * `pagecache_write_{begin,end}`.
			 */
			unlock_page(old);
			if (ret < 0) {
				put_page(old);
				break;
			}
			wr_mgr->old_pages[i] = old;
		}
	}

	return ret;
}

static void transfs_clean_buffers(struct page *pg)
{
	struct buffer_head *bh = NULL;
	struct buffer_head *head = NULL;

	if (!page_has_buffers(pg)) {
		return;
	}
	head = page_buffers(pg);
	bh = head;
	do {
		clear_buffer_dirty(bh);
		bh = bh->b_this_page;
	} while (bh != head);
}

static void transfs_try_to_release_page(struct page *pg, int *ret)
{
	if (page_has_private(pg) && try_to_release_page(pg, 0) == 0) {
		*ret = -EIO;
		/*
		 * When the buffers of the page are dirty, the
		 * drop_buffers function will fails and returns 0.
		 */
		transfs_clean_buffers(pg);
		if (try_to_release_page(pg, 0) == 0) {
			pr_warn("try_to free buffer failed, page flags=%pGp, zone:%d\n",
				&pg->flags, page_zonenum(pg));
		}
	}
}

static int transfs_delete_direct_map_pages(struct map_pg_write_mgr *wr_mgr)
{
	int i;
	int ret = 0;
	size_t cnt = wr_mgr->pages_nr;

	for (i = 0; i < cnt; i++) {
		struct page *pg = pfn_to_page(wr_mgr->pfn[i]);
		if (page_mapping(pg) == NULL) {
			/*
			 * If an error occurs during the write process and the
			 * put_page func is not executed for the old page and
			 * corresponding pg, need to do put_page command for
			 * these pages to avoid page counting errors.
			 */
			if (wr_mgr->old_pages[i] != NULL) {
				put_page(wr_mgr->old_pages[i]);
				put_page(pg);
			}
			pr_debug("find page i=%d mapping is NULL, page flags=%pGp, zone:%d\n",
				 i, &pg->flags, page_zonenum(pg));
			continue;
		}
		lock_page(pg);
		wait_on_page_writeback(pg);
		transfs_try_to_release_page(pg, &ret);
		delete_from_page_cache(pg);
		unlock_page(pg);
		put_page(pg);
	}

	return ret;
}

static int __transfs_direct_map_page_write(struct file *file, struct map_pg_write_mgr *wr_mgr,
					    size_t size, unsigned long long offset,
					    loff_t *__last_pos)
{
	int i;
	int ret = 0;
	size_t remained_size = size;
	size_t cnt = wr_mgr->pages_nr;
	loff_t last_pos = offset;
	unsigned long long padding = offset & (~PAGE_MASK);
	struct address_space *mapping = file->f_mapping;

	for (i = 0; i < cnt; i++) {
		struct page *pg = pfn_to_page(wr_mgr->pfn[i]);
		struct page *old;
		void *fsdata;
		size_t sz = min(remained_size, (size_t)(PAGE_SIZE - padding));
		size_t copied = sz;

		ret = pagecache_write_begin(file, mapping, last_pos, sz, 0, &old, &fsdata);
		if (ret < 0) {
			transfs_limit_warn((ret == (-EROFS) || ret == (-ENOSPC)),
					   "write_begin failed, ret=%d\n", ret);
			break;
		}
		if (old != pg) {
			ret = transfs_copy_page_range(old, pg, sz, padding);
			put_page(old);
			put_page(pg);
			if (old != wr_mgr->old_pages[i]) {
				pr_err("found page is not the same as the recorded page\n");
				BUG_ON(1);
			}
			wr_mgr->old_pages[i] = NULL;
			if (ret < 0) {
				pr_warn("copy pg to old data failed\n");
				copied = 0;
			}
		}
		ret = pagecache_write_end(file, mapping, last_pos, sz, copied, old, fsdata);
		if (ret < 0) {
			pr_warn("write_end failed, ret=%d\n", ret);
			break;
		}

		remained_size -= sz;
		last_pos += sz;
		padding = 0;
	}

	*__last_pos = last_pos;

	return ret;
}

static int transfs_direct_map_page_write(struct file *file, unsigned long long *pfn,
					 size_t size, unsigned long long offset)
{
	loff_t last_pos = 0;
	size_t cnt = transfs_pg_count(offset, size);
	int ret, err;
	struct map_pg_write_mgr wr_mgr = {0};

	wr_mgr.old_pages = liblinux_pal_calloc(1, (cnt * sizeof(struct page *)));
	if (wr_mgr.old_pages == NULL) {
		pr_warn("transfs: alloc write mgr pages failed\n");
		return -ENOMEM;
	}
	wr_mgr.pfn = pfn;
	wr_mgr.pages_nr = cnt;

	inode_lock(file->f_inode);

	ret = __transfs_write_prepare_page(file, &wr_mgr, size, offset);
	if (ret < 0) {
		goto out;
	}

	ret = __transfs_direct_map_page_write(file, &wr_mgr, size, offset, &last_pos);
	err = filemap_fdatawrite_range(file->f_mapping, offset, last_pos);
	if (err < 0) {
		pr_warn("filemap write failed, ret=%d\n", err);
	}
	ret = ret < 0 ? ret : err;

out:
	err = transfs_delete_direct_map_pages(&wr_mgr);
	inode_unlock(file->f_inode);
	ret = ret < 0 ? ret : err;

	liblinux_pal_free(wr_mgr.old_pages);

	return (ret < 0) ? ret : size;
}

static ssize_t transfs_direct_map_cache_write(struct file *file, unsigned long long *pfn,
					      size_t size, unsigned long long offset)
{
	ssize_t ret;
	ssize_t write_bytes = 0;
	size_t cnt = transfs_pg_count(offset, size);
	unsigned long long padding = offset & (~PAGE_MASK);
	loff_t pos = (loff_t)offset;
	size_t remained_size = size;
	int i;

	for (i = 0; i < cnt; i++) {
		void *addr =  __va(PFN_PHYS(pfn[i]));
		size_t sz = min(remained_size, (size_t)(PAGE_SIZE - padding));

		ret = transfs_vfs_write(file, (char *)addr + padding, sz, &pos, false);
		if (ret < 0) {
			pr_err("do vfs write error\n");
			return ret;
		}
		write_bytes += ret;
		remained_size -= sz;
		padding = 0;
	}

	return write_bytes;
}
#else
static int transfs_direct_map_page_write(struct file *file, unsigned long long *pfn,
					 size_t size, unsigned long long offset)
{
	return -EOPNOTSUPP;
}

static ssize_t transfs_direct_map_cache_write(struct file *file, unsigned long long *pfn,
					      size_t size, unsigned long long offset)
{
	return -EOPNOTSUPP;
}
#endif


static int devhost_transfs_io_write(struct vfsmount *vmnt, struct inode *inode,
				    char *buf, unsigned int size,
				    unsigned long long offset, bool direct_map_page)
{
	ssize_t ret;
	struct file *file = NULL;
	struct dentry dentry;
	loff_t pos = (loff_t)offset;

	/*
	 * The page in the HM cache cannot obtain file-related information.
	 * Therefore, when the I/O is transmitted to the devhost transfs,
	 * the file needs to be constructed to carry the current I/O.
	 */
	ret = memset_s(&dentry, sizeof(struct dentry), 0, sizeof(struct dentry));
	if (ret != 0) {
		return -EFAULT;
	}
	dentry.d_inode = inode;
	ret = transfs_alloc_file(vmnt, &dentry, O_RDWR, &file);
	if (ret) {
		return -EINVAL;
	}

	if (!direct_map_page) {
		ret = transfs_vfs_write(file, buf, size, &pos, false);
		if (ret < 0) {
			pr_warn("write failed, err:%zd\n", ret);
		}

		transfs_sync_check(file);
	} else {
		ret = transfs_direct_map_page_write(file, (unsigned long long*)buf,
						    size, offset);
		if (ret < 0 && ret != -ETIMEDOUT) {
			transfs_limit_warn((ret == (-EROFS) || ret == (-ENOSPC)),
					   "direct map page write failed, err:%zd\n", ret);
		}
		if (ret == -ETIMEDOUT) {
			ret = transfs_direct_map_cache_write(file, (unsigned long long*)buf,
							     size, offset);
			if (ret < 0) {
				pr_warn("do cache write failed, err:%zd\n", ret);
			}
			transfs_sync_check(file);
		}
	}

	file->f_path.dentry = NULL;
	/* tmp file should be removed */
	put_filp(file);

	if ((size_t)ret != (size_t)size) {
		transfs_limit_warn(true, "IO error: expect write size:%u, return:%zd\n",
				   size, ret);
	}

	return (int)ret;
}

static int devhost_transfs_get_file_info(void *file, unsigned int *me_idx,
					 unsigned long long *hm_filp)
{
	struct file *filp = (struct file *)file;
	struct hm_file_info *hfi = NULL;

	if (filp->liblinux_private == NULL) {
		return -EINVAL;
	}

	hfi = (struct hm_file_info *)filp->liblinux_private;

	*me_idx = hfi->me_index;
	*hm_filp = hfi->hm_filp;

	if (file_count(filp) == 1) {
		liblinux_pal_free(hfi);
	}

	return 0;
}

static int devhost_transfs_set_file_info(void *file, unsigned int me_idx,
					 unsigned long long hm_filp)
{
	struct file *filp = (struct file *)file;
	struct hm_file_info *hfi = NULL;

	if (filp->liblinux_private != NULL) {
		return 0;
	}

	hfi = liblinux_pal_calloc(1, sizeof(struct hm_file_info));
	if (hfi == NULL) {
		pr_warn("calloc hm file info failed\n");
		return -ENOMEM;
	}

	hfi->me_index = me_idx;
	hfi->hm_filp = hm_filp;

	filp->liblinux_private = hfi;

	return 0;
}

static long devhost_transfs_update_opts(struct super_block *sb,
					char *buf, size_t buflen)
{
	struct seq_file seq = {0};
	int ret;

	seq.buf = buf;
	seq.size = buflen;
	seq.private = sb;

	if (sb->s_op == NULL) {
		pr_warn("sb's s_op is null\n");
		return 0;
	}
	if (sb->s_op->show_options == NULL) {
		return 0;
	}
	ret = sb->s_op->show_options(&seq, sb->s_root);
	if (ret == 0) {
		ret = (ssize_t)seq.count;
	}
	return ret;
}

static int devhost_get_ubi_major2num(unsigned int major)
{
	ubi_major2num __ubi_major2num =
		(ubi_major2num)kallsyms_lookup_name("ubi_major2num");
	if (__ubi_major2num == NULL) {
		return -EOPNOTSUPP;
	}
	return __ubi_major2num(major);
}

static int devhost_transfs_iommap(struct file *filp, struct devhost_vma_info *info)
{
	return devhost_transfs_fops_mmap(filp, info);
}

static struct devhost_transfs_ops transfs_ops __read_mostly = {
	.unmounted = devhost_transfs_unmounted,
	.mkdir = devhost_transfs_mkdir,
	.rmdir = devhost_transfs_rmdir,
	.create = devhost_transfs_create,
	.unlink = devhost_transfs_unlink,
	.write = devhost_transfs_write,
	.read = devhost_transfs_read,
	.write_iter = devhost_transfs_write_iter,
	.fsync = devhost_transfs_fsync,
	.mknod = devhost_transfs_mknod,
	.link = devhost_transfs_link,
	.symlink = devhost_transfs_symlink,
	.statfs = devhost_transfs_statfs,
	.rename = devhost_transfs_rename,
	.readdir = devhost_transfs_readdir,
	.lookup = devhost_transfs_lookup,
	.setattr = devhost_transfs_setattr,
	.readlink = devhost_transfs_readlink,
	.open = devhost_transfs_open,
	.fallocate = devhost_transfs_fallocate,
	.getattr = devhost_transfs_getattr,
	.listxattr = devhost_transfs_listxattr,
	.getxattr = devhost_transfs_getxattr,
	.setxattr = devhost_transfs_setxattr,
	.tmpfile = devhost_transfs_tmpfile,
	.sync = devhost_transfs_sync,
	.drop_caches = devhost_transfs_drop_caches,
	.close = devhost_transfs_close,
	.ioctl = devhost_transfs_ioctl,
	.direct_IO = devhost_transfs_direct_IO,
	.kill_dentry = devhost_transfs_kill_dentry,
	.node_put = devhost_transfs_node_put,
	.update_opts = devhost_transfs_update_opts,
	.io_read = devhost_transfs_io_read,
	.io_write = devhost_transfs_io_write,
	.iommap = devhost_transfs_iommap,
};

static struct devhost_transfs_file_ops transfile_ops __read_mostly = {
	.get_file_info = devhost_transfs_get_file_info,
	.set_file_info = devhost_transfs_set_file_info,
};

static void drop_pagecache_sb_handler(struct work_struct *work)
{
	struct super_block *sb = NULL;
	struct transfs_dropcache_work *twork = NULL;

	if (work == NULL) {
		pr_err("transfs: invalid work\n");
		return;
	}
	twork = container_of(work, struct transfs_dropcache_work, work);
	sb = twork->sb;
	if (sb == NULL) {
		return;
	}

#define DROPCACHE_MODE 3
	(void)devhost_transfs_drop_caches(sb, DROPCACHE_MODE, -1);
}

#ifdef CONFIG_LDK_FS_MM_CACHE
static void transfs_drop_pagecache_sb(struct super_block *sb, void *data)
{
	struct transfs_dropcache_work *twork = NULL;

	if (sb == NULL || data == NULL) {
		pr_err("transfs: wrong sb\n");
		return;
	}

	twork = (struct transfs_dropcache_work *)data;
	BUG_ON(twork->sb != sb);

	if (work_busy(&twork->work) == 0) {
		queue_work(g_transfs_wq, &twork->work);
	}
}

static struct page *transfs_pagecache_alloc(gfp_t gfp_mask, unsigned order)
{
	/*
	 * To accelerate the process of reclaiming the ldk memory and ensure
	 * that existing cache data can be stored to disks quickly when the
	 * I/O speed is high. When the total number of pages in the file on
	 * the LDK side exceeds the threshold, which is set to half of the
	 * transfs_page_clean_watermark, the background write-back thread is
	 * automatically woken up.
	 */
	if (liblinux_get_pagecache_nr() > (transfs_page_clean_watermark >> 1)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
		wakeup_flusher_threads(WB_REASON_BACKGROUND);
#else
		wakeup_flusher_threads(0, WB_REASON_BACKGROUND);
#endif
	}
	if (liblinux_get_pagecache_nr() > transfs_page_clean_watermark) {
		transfs_iterate_supers(transfs_drop_pagecache_sb);
	}

	return alloc_pages_cache(gfp_mask, order);
}
#endif

static int __init transfs_init(void)
{
	int ret = 0;

	if (g_transfs_wq != NULL) {
		pr_err("g_transfs_wq is not NULL\n");
		return -EFAULT;
	}
	g_transfs_wq = alloc_workqueue("dev-transfs-wq", WQ_UNBOUND, 0);
	if (g_transfs_wq == NULL) {
		pr_err("alloc g_transfs_wq failed\n");
		return -ENOMEM;
	}
	ret = devhost_register_transfs(devhost_transfs_mounted, &transfile_ops,
				       devhost_get_ubi_major2num,
				       transfs_dget);
	if (ret == 0) {
#ifdef CONFIG_LDK_FS_MM_CACHE
		liblinux_enable_page_cache(transfs_pagecache_alloc);
#endif
	} else {
		destroy_workqueue(g_transfs_wq);
		g_transfs_wq = NULL;
	}

	return ret;
}
module_init(transfs_init);
