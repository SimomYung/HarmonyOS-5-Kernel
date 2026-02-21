// SPDX-License-Identifier: GPL-2.0-only
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fsnotify.h>
#include <linux/module.h>
#include <linux/namei.h>
#include <linux/backing-dev.h>
#include <linux/mount.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/personality.h>
#include <linux/pagemap.h>
#include <linux/syscalls.h>
#include <linux/rcupdate.h>
#include <linux/falloc.h>
#include <linux/fs_struct.h>
#include <linux/ima.h>
#include <linux/dnotify.h>
#include <linux/compat.h>
#include <linux/firmware.h>
#include <linux/export.h>
#include <linux/shrinker.h>
#include <liblinux/file_utils.h>
#include <liblinux/pal.h>
#include <liblinux/sched.h>
#include <trace/hooks/liblinux.h>

#include <linux/stat.h>

/* kernel/oh-ldk/fs/internal.h */
#include "../fs/internal.h"

#include <securec.h>
#define FILE_NAME_LEN  64

struct cache_info_s {
	struct list_head list;
	struct mutex lock;
	char *name;
	char *buf;
	long size;
	long ctime_sec;
	int order;
};

struct plat_file {
	struct inode f_inode;
	struct dentry dentry;
	struct inode d_inode;
	int fd;
	struct mutex flock;
	char name[FILE_NAME_LEN];
	struct cache_info_s *cache_info;
};

struct file_cache_s {
	struct list_head info_list;
	struct mutex list_lock;
	struct shrinker shrinker;
};
struct file_cache_s file_cache = {0};

struct {
	const char *name;
	bool has_wildcard;
} file_cache_whitelist[] = {
	{"/data/vendor/cust_conn/ini_cfg", false},
	{"/vendor/etc/cfg_*", true},
};

static ssize_t __read_from_file_cache(struct plat_file *fp, const struct iovec *iov,
				      int iovcnt, off_t offset)
{
	int cur_cp;
	int total_cp = 0;
	int remain;
	off_t cur_offset = offset;
	struct cache_info_s *cache_info = fp->cache_info;

	mutex_lock(&cache_info->lock);

	if (!cache_info->buf) {
		fp->cache_info = NULL;
		total_cp = -EAGAIN;
		goto out;
	}

	if (offset >= cache_info->size)
		goto out;

	remain = cache_info->size - offset;

	for (int i = 0; i < iovcnt; i++) {
		if (remain >= iov[i].iov_len)
			cur_cp = iov[i].iov_len;
		else
			cur_cp = remain;

		if (memcpy_s(iov[i].iov_base, iov[i].iov_len,
			     cache_info->buf + cur_offset, cur_cp) != 0) {
			pr_err("liblinux: %s: memcpy_s failed\n", __func__);
			total_cp = -EFAULT;
			break;
		}

		remain -= cur_cp;
		cur_offset += cur_cp;
		total_cp += cur_cp;

		if (remain == 0)
			break;
	}

out:
	mutex_unlock(&cache_info->lock);
	return total_cp;
}

static ssize_t __firmware_file_read_iter(struct kiocb *kiocb, struct iov_iter *iter)
{
	struct plat_file *internal_fp;
	ssize_t ret = 0;
	struct file *file = kiocb->ki_filp;
	loff_t offset = kiocb->ki_pos;
	int iovcnt = iter->nr_segs;

	internal_fp = file->private_data;
	if (internal_fp == NULL) {
		printk("inval file to read\n");
		return -EINVAL;
	}

	mutex_lock(&internal_fp->flock);
	if (!internal_fp->cache_info) {
		liblinux_ctx_leave();
		ret = liblinux_pal_ksys_preadv(internal_fp->fd, iter->iov, iovcnt, offset);
		liblinux_ctx_enter();
	} else {
		ret = __read_from_file_cache(internal_fp, iter->iov, iovcnt, offset);
		if (ret == -EAGAIN) {
			liblinux_ctx_leave();
			ret = liblinux_pal_ksys_preadv(internal_fp->fd, iter->iov, iovcnt, offset);
			liblinux_ctx_enter();
		}
	}
	mutex_unlock(&internal_fp->flock);
	if (ret > 0) {
		kiocb->ki_pos = offset + ret;
	}

	return ret;
}

static ssize_t __firmware_file_write_iter(struct kiocb *kiocb, struct iov_iter *iter)
{
	struct plat_file *internal_fp;
	ssize_t ret = 0;
	struct file *file = kiocb->ki_filp;
	loff_t offset = kiocb->ki_pos;
	int iovcnt = iter->nr_segs;

	internal_fp = file->private_data;
	if (internal_fp == NULL) {
		printk("inval file to read\n");
		return -EINVAL;
	}

	mutex_lock(&internal_fp->flock);
	liblinux_ctx_leave();
	ret = liblinux_pal_ksys_pwritev(internal_fp->fd, iter->iov, iovcnt, offset);
	liblinux_ctx_enter();
	mutex_unlock(&internal_fp->flock);
	if (ret > 0) {
		kiocb->ki_pos = offset + ret;
	}

	return ret;
}

static int __firmware_file_fsync(struct file *file, loff_t start, loff_t end, int datasync)
{
	struct plat_file *internal_fp;
	int ret = 0;

	internal_fp = file->private_data;
	if (internal_fp == NULL) {
		printk("inval file to fsync\n");
		return -EINVAL;
	}

	mutex_lock(&internal_fp->flock);
	ret = ksys_fsync(internal_fp->fd);
	mutex_unlock(&internal_fp->flock);
	return ret;
}

static loff_t __firmware_file_llseek(struct file *file, loff_t offset, int whence)
{
	struct plat_file *internal_fp = NULL;
	off_t ret = 0;

	internal_fp = file->private_data;
	if (internal_fp == NULL) {
		return -EINVAL;
	}

	mutex_lock(&internal_fp->flock);
	ret = ksys_lseek(internal_fp->fd, offset, whence);
	if (ret >= 0) {
		file->f_pos = ret;
	}
	mutex_unlock(&internal_fp->flock);
	return ret;
}

const struct file_operations firmware_file_ops = {
	.read_iter	= __firmware_file_read_iter,
	.write_iter	= __firmware_file_write_iter,
	.fsync		= __firmware_file_fsync,
	.llseek		= __firmware_file_llseek,
};

/*
 * __is_filp_open_file - Check if struct file* is associated with filp_open
 */
static int __is_filp_open_file(struct file *file)
{
	return file->f_op == &firmware_file_ops;
}

static struct file *open_with_noaccount(int flags, struct inode *f_inode,
					const struct file_operations *f_ops,
					const struct cred *cred)
{
	struct file *filp = alloc_empty_file_noaccount(flags, cred);
	if (!IS_ERR_OR_NULL(filp)) {
		filp->f_inode = f_inode;
		filp->f_flags = O_DSYNC;
		filp->f_mode  = FMODE_LSEEK | FMODE_READ | FMODE_CAN_READ |
				FMODE_WRITE | FMODE_CAN_WRITE | FMODE_NONOTIFY;
		filp->f_op   = f_ops;
	}
	return filp;
}

static int __file_cache_build(struct cache_info_s *cache_info, int fd,
			      const char *name, long size, long ctime_sec)
{
	int ret;
	int order = get_order(size);

	cache_info->buf = __get_free_pages(GFP_KERNEL | __GFP_ZERO, order);
	if (!cache_info->buf) {
		pr_err("liblinux: %s: get free pages failed\n", __func__);
		return -ENOMEM;
	}

	ret = ksys_read(fd, cache_info->buf, size);
	if (ret < 0) {
		pr_err("liblinux: %s: ksys_read failed, ret=%d\n", __func__, ret);
		free_pages(cache_info->buf, order);
		cache_info->buf = NULL;
		return ret;
	}

	cache_info->size = size;
	cache_info->ctime_sec = ctime_sec;
	cache_info->order = order;

	return ret;
}

static struct cache_info_s *__file_cache_find(const char *name)
{
	struct cache_info_s *cache_info = NULL;
	bool rst = false;

	mutex_lock(&file_cache.list_lock);
	list_for_each_entry(cache_info, &file_cache.info_list, list) {
		if (strcmp(cache_info->name, name) == 0) {
			rst = true;
			break;
		}
	}
	mutex_unlock(&file_cache.list_lock);

	if (rst)
		goto out;

	cache_info = kzalloc(sizeof(struct cache_info_s), GFP_KERNEL);
	if (!cache_info) {
		pr_err("liblinux: %s: kzalloc failed\n", __func__);
		goto out;
	}

	cache_info->name = kstrdup(name, GFP_KERNEL);
	if (!cache_info->name) {
		pr_err("liblinux: %s: kstrdup name failed\n", __func__);
		kfree(cache_info);
		cache_info = NULL;
		goto out;
	}

	cache_info->order = -1;
	mutex_init(&cache_info->lock);
	INIT_LIST_HEAD(&cache_info->list);

	mutex_lock(&file_cache.list_lock);
	list_add_tail(&cache_info->list, &file_cache.info_list);
	mutex_unlock(&file_cache.list_lock);
out:
	return cache_info;
}

static bool __file_cache_is_allowed(const char *name)
{
	bool allow = false;
	size_t len;

	for (int i = 0; i < sizeof(file_cache_whitelist) / sizeof(file_cache_whitelist[0]); i++) {
		len = (file_cache_whitelist[i].has_wildcard) ?
		      strlen(file_cache_whitelist[i].name) - 1 :
		      strlen(file_cache_whitelist[i].name);
		if (strncmp(file_cache_whitelist[i].name, name, len) == 0) {
			allow = true;
			break;
		}
	}

	return allow;
}

static void __file_cache_check(struct plat_file *fp, const char *name, struct stat *stat)
{
	int ret = 0;
	struct cache_info_s *cache_info;

	if (!__file_cache_is_allowed(name))
		return;

	cache_info = __file_cache_find(name);
	if (!cache_info)
		return;

	mutex_lock(&cache_info->lock);
	if (!cache_info->buf) {
		ret = __file_cache_build(cache_info, fp->fd, name, stat->st_size,
					 stat->st_ctime);
	} else {
		if ((cache_info->ctime_sec != stat->st_ctime) ||
		    (cache_info->size != stat->st_size)) {
			free_pages(cache_info->buf, cache_info->order);
			cache_info->buf = NULL;
			cache_info->order = -1;
			ret = __file_cache_build(cache_info, fp->fd, name,
						 stat->st_size, stat->st_ctime);
		}
	}

	if (ret >= 0)
		fp->cache_info = cache_info;

	mutex_unlock(&cache_info->lock);
	return;
}

struct file __override *filp_open(const char *filename, int flags, umode_t mode)
{
	struct plat_file *fp = NULL;
	struct file *filp = NULL;
	int fd = 0;
	struct stat stat;
	int ret = 0;
	char *tmp = NULL;

	if (filename == NULL) {
		return ERR_PTR(-ENOENT);
	}

	fd = ksys_open(filename, flags, mode);
	if (fd < 0) {
		pr_debug("[error] %s, open %s failed, flags:%d, mode:0x%x, fd=%d\n", __func__,
			 filename, flags, mode, fd);
		return ERR_PTR(-ENOENT);
	}

	ret = liblinux_ksys_fstat_nocheck(fd, &stat);
	if (ret != 0) {
		printk("[error] %s, stat %s failed, flags:%d, mode:0x%x\n", __func__,
		       filename, flags, mode);
		ksys_close(fd);
		return ERR_PTR(-ENOENT);
	}

	fp = (struct plat_file *)kzalloc(sizeof(struct plat_file), GFP_KERNEL);
	if (fp == NULL) {
		ksys_close(fd);
		return ERR_PTR(-ENOMEM);
	}

	mutex_init(&fp->flock);
	fp->fd = fd;
	filp = open_with_noaccount(flags, &fp->f_inode, &firmware_file_ops, current_cred());
	if (IS_ERR_OR_NULL(filp)) {
		kfree(fp);
		ksys_close(fd);
		return ERR_PTR(-ENOENT);
	}

	fp->d_inode.i_data.host = &fp->d_inode;
	fp->d_inode.i_mapping = &fp->d_inode.i_data;
	filp->f_mapping = fp->d_inode.i_mapping;
	filp->f_inode->i_size = stat.st_size;
	filp->f_inode->i_mode = (umode_t)stat.st_mode;
	filp->private_data = fp;
	filp->f_pos = 0;
	filp->f_path.dentry = &fp->dentry;
	filp->f_path.dentry->d_inode = &fp->d_inode;
	filp->f_path.dentry->d_inode->i_ctime.tv_sec = stat.st_ctime;
#ifdef STAT_HAVE_NSEC
	filp->f_path.dentry->d_inode->i_ctime.tv_nsec = stat.st_ctime_nsec;
#endif

	tmp = strrchr(filename, '/');
	if (tmp && *(tmp + 1) != '\0') {
		if (strncpy_s(fp->name, FILE_NAME_LEN, tmp + 1, FILE_NAME_LEN - 1))
			return filp;
		filp->f_path.dentry->d_name.name = fp->name;
		filp->f_path.dentry->d_name.len = strlen(fp->name);
	}

	__file_cache_check(fp, filename, &stat);

	return filp;
}

static void __filp_priv_close(struct file *filp)
{
	struct plat_file *internal_fp = NULL;

	if (IS_ERR_OR_NULL(filp)) {
		return;
	}

	internal_fp = filp->private_data;
	if (internal_fp == NULL) {
		return;
	}

	mutex_lock(&internal_fp->flock);
	ksys_close(internal_fp->fd);
	mutex_unlock(&internal_fp->flock);
	kfree(internal_fp);

	return;
}

static void hook_filp_open_put(void *__data, struct file *file)
{
	if (IS_ERR_OR_NULL(file)) {
		return;
	}
	if (__is_filp_open_file(file)) {
		__filp_priv_close(file);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_filp_open_put, hook_filp_open_put);

int __override filp_close(struct file *filp, fl_owner_t id)
{
	struct plat_file *internal_fp = NULL;

	if (IS_ERR_OR_NULL(filp)) {
		return -EINVAL;
	}

	fput(filp);
	return 0;
}

int __override vfs_fchown(struct file *filp, uid_t user, gid_t group)
{
	struct plat_file *fp = NULL;
	int ret;

	if (IS_ERR_OR_NULL(filp) || filp->private_data == NULL) {
		return -EINVAL;
	}

	fp = filp->private_data;
	mutex_lock(&fp->flock);
	ret = ksys_fchown(fp->fd, user, group);
	mutex_unlock(&fp->flock);
	return ret;
}
EXPORT_SYMBOL(vfs_fchown);

#if BITS_PER_LONG == 32
#  define choose_32_64(a,b) a
#else
#  define choose_32_64(a,b) b
#endif

#define decode_dev(x) choose_32_64(old_decode_dev,new_decode_dev)(x)

/**
 * cp stat to kstat
*/
static int cp_kstat(struct stat *stat, struct kstat *kstat)
{
	if (stat == NULL || kstat == NULL)
		return -EINVAL;
	kstat->dev = decode_dev(stat->st_dev);
	kstat->ino = stat->st_ino;
	kstat->mode = stat->st_mode;
	kstat->nlink = stat->st_nlink;
	kstat->uid.val = stat->st_uid;
	kstat->gid.val = stat->st_gid;
	kstat->rdev = decode_dev(stat->st_rdev);
	kstat->size = stat->st_size;
	kstat->atime.tv_sec = stat->st_atime;
	kstat->mtime.tv_sec = stat->st_mtime;
	kstat->ctime.tv_sec = stat->st_ctime;
#ifdef STAT_HAVE_NSEC
	kstat->atime.tv_nsec = stat->st_atime_nsec;
	kstat->mtime.tv_nsec = stat->st_mtime_nsec;
	kstat->ctime.tv_nsec = stat->st_ctime_nsec;
#endif
	kstat->blocks = stat->st_blocks;
	kstat->blksize = stat->st_blksize;
	return 0;
}

int __override vfs_fstatat(int dfd, const char *filename, struct kstat *kstat,
			   int flags)
{
	struct stat stat;
	int ret;

	if (filename == NULL || kstat == NULL || flags < 0)
		return -EINVAL;

	ret = liblinux_ksys_fstatat(dfd, filename, &stat, (unsigned int)flags);
	if (ret < 0)
		return -ENOENT;

	ret = cp_kstat(&stat, kstat);
	if (ret < 0)
		return ret;

	return 0;
}
EXPORT_SYMBOL(vfs_fstatat);

unsigned long __fdget(unsigned int fd)
{
	return FDPUT_FPUT | (unsigned long)fget(fd);
}

long __override do_unlinkat(int dfd, struct filename *name)
{
	int ret = -EINVAL;

	if (IS_ERR_OR_NULL(name))
		return -EINVAL;

	if (name->uptr == NULL)
		goto exit;

	ret = ksys_unlink(name->uptr);
exit:
	putname(name);
	return ret;
}

int vfs_fsync(struct file *file, int datasync)
{
	if (IS_ERR_OR_NULL(file) || file->f_op == NULL || file->f_op->fsync == NULL)
		return -EINVAL;

	return file->f_op->fsync(file, 0, LLONG_MAX, datasync);
}
EXPORT_SYMBOL(vfs_fsync);

static unsigned long file_cache_shrinker_scan(struct shrinker *shrinker,
					      struct shrink_control *sc)
{
	int pages_freed = 0;
	struct cache_info_s *cache_info = NULL;

	mutex_lock(&file_cache.list_lock);
	list_for_each_entry(cache_info, &file_cache.info_list, list) {
		if (!mutex_trylock(&cache_info->lock))
			continue;

		if (cache_info->buf) {
			free_pages(cache_info->buf, cache_info->order);
			pages_freed += 1 << cache_info->order;
			cache_info->buf = NULL;
			cache_info->order = -1;
			cache_info->size = 0;
			cache_info->ctime_sec = 0;
		}
		mutex_unlock(&cache_info->lock);
	}
	mutex_unlock(&file_cache.list_lock);

	return pages_freed ? pages_freed : SHRINK_STOP;
}

static unsigned long file_cache_shrinker_count(struct shrinker *shrinker,
					       struct shrink_control *sc)
{
	int pages_to_free = 0;
	struct cache_info_s *cache_info = NULL;

	mutex_lock(&file_cache.list_lock);
	list_for_each_entry(cache_info, &file_cache.info_list, list) {
		if (cache_info->order >= 0)
			pages_to_free += 1 << cache_info->order;
	}
	mutex_unlock(&file_cache.list_lock);

	return pages_to_free;
}

static int __init file_cache_init(void)
{
	INIT_LIST_HEAD(&file_cache.info_list);
	mutex_init(&file_cache.list_lock);

	file_cache.shrinker.scan_objects = file_cache_shrinker_scan;
	file_cache.shrinker.count_objects = file_cache_shrinker_count;
	file_cache.shrinker.batch = 0;
	file_cache.shrinker.seeks = DEFAULT_SEEKS;
	register_shrinker(&file_cache.shrinker);

	return 0;
}

static void file_cache_exit(void)
{
	struct cache_info_s *cache_info = NULL;
	struct cache_info_s *tmp = NULL;

	unregister_shrinker(&file_cache.shrinker);

	mutex_lock(&file_cache.list_lock);
	list_for_each_entry_safe(cache_info, tmp, &file_cache.info_list, list) {
		mutex_lock(&cache_info->lock);
		list_del(&cache_info->list);
		if (cache_info->buf)
			free_pages(cache_info->buf, cache_info->order);
		if (cache_info->name)
			kfree(cache_info->name);
		mutex_unlock(&cache_info->lock);
		kfree(cache_info);
	}
	mutex_unlock(&file_cache.list_lock);
}

postcore_initcall(file_cache_init);
module_exit(file_cache_exit);
