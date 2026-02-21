// SPDX-License-Identifier: GPL-2.0-only
#include <linux/cdev.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/splice.h>
#include <linux/compat.h>
#include <linux/backing-dev.h>
#include <linux/kmemleak.h>
#include <liblinux/sched.h>
/* dummy pipe ops for lib/iov_iter.c */
const struct pipe_buf_operations default_pipe_buf_ops;
const struct pipe_buf_operations page_cache_pipe_buf_ops;
const struct pipe_buf_operations nosteal_pipe_buf_ops = {NULL};
EXPORT_SYMBOL(nosteal_pipe_buf_ops);
EXPORT_SYMBOL(cd_forget);

/* dummy dentry/file ops */
const struct dentry_operations ns_dentry_operations;
const struct file_operations fscontext_fops;
const struct file_operations pipefifo_fops;

int get_unused_fd_flags(unsigned flags)
{
	return liblinux_pal_get_unused_fd_flags(flags);
}
EXPORT_SYMBOL(get_unused_fd_flags);

void fd_install(unsigned int fd, struct file *file)
{
	struct dh_fd_install_context ctx = {0};

	if (file->f_path.dentry && file->f_path.dentry->d_name.name) {
		ctx.file_name = file->f_path.dentry->d_name.name;
		ctx.buffer_len = file->f_path.dentry->d_name.len + 1;
	}
	ctx.f_flags = file->f_flags;
	liblinux_pal_fd_install_ex(fd, file, &ctx);
	kmemleak_not_leak((void *)file);
}
EXPORT_SYMBOL(fd_install);

void put_unused_fd(unsigned int fd)
{
	liblinux_pal_put_unused_fd((int)fd);
}
EXPORT_SYMBOL(put_unused_fd);

static void __fget(void *file)
{
	get_file(file);
}

struct file *fget(unsigned int fd)
{
	struct file *filp = NULL;
	if (fd == UINT_MAX) {
		return NULL;
	}

	filp = liblinux_pal_fget_ex(ANON_FD, fd, __fget);
	if (filp == NULL) {
		return NULL;
	}
	return filp;
}
EXPORT_SYMBOL(fget);

int __close_fd(struct files_struct *files, unsigned fd)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_close(fd);
	if (ret != 0)
		pr_err("%s, close fd:%lu, err:%d\n", __func__, fd, ret);

	return ret;
}
EXPORT_SYMBOL(__close_fd);

#ifdef CONFIG_COMPAT
long compat_ptr_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	if (!file->f_op->unlocked_ioctl)
		return -ENOIOCTLCMD;

	return file->f_op->unlocked_ioctl(file, cmd, (unsigned long)compat_ptr(arg));
}
EXPORT_SYMBOL(compat_ptr_ioctl);
#endif

unsigned int round_pipe_size(unsigned long size)
{
	if (size > (1U << 31))
		return 0;

	/* Minimum pipe size, as required by POSIX */
	if (size < PAGE_SIZE)
		return PAGE_SIZE;

	return roundup_pow_of_two(size);
}

void
file_ra_state_init(struct file_ra_state *ra, struct address_space *mapping)
{
	ra->ra_pages = inode_to_bdi(mapping->host)->ra_pages;
	ra->prev_pos = -1;
}
EXPORT_SYMBOL_GPL(file_ra_state_init);

extern struct file *alloc_empty_file(int, const struct cred *);
extern char *simple_dname(struct dentry *, char *, int);
extern struct dentry * d_alloc_pseudo(struct super_block *, const struct qstr *);

struct file *alloc_file(const struct path *path, int flags,
		const struct file_operations *fop)
{
	struct file *file;

	file = alloc_empty_file(flags, current_cred());
	if (IS_ERR(file))
		return file;

	file->f_path = *path;
	file->f_inode = path->dentry->d_inode;
	file->f_mapping = path->dentry->d_inode->i_mapping;
	/* ignore sample wb err */
#ifndef CONFIG_LIBLINUX
	file->f_wb_err = filemap_sample_wb_err(file->f_mapping);
	file->f_sb_err = file_sample_sb_err(file);
#endif /* !CONFIG_LIBLINUX */
	if ((file->f_mode & FMODE_READ) &&
	     likely(fop->read || fop->read_iter))
		file->f_mode |= FMODE_CAN_READ;
	if ((file->f_mode & FMODE_WRITE) &&
	     likely(fop->write || fop->write_iter))
		file->f_mode |= FMODE_CAN_WRITE;
	file->f_mode |= FMODE_OPENED;
	file->f_op = fop;
	if ((file->f_mode & (FMODE_READ | FMODE_WRITE)) == FMODE_READ)
		i_readcount_inc(path->dentry->d_inode);
	return file;
}

struct file * __override alloc_file_pseudo(struct inode *inode, struct vfsmount *mnt,
				const char *name, int flags,
				const struct file_operations *fops)
{
	static const struct dentry_operations anon_ops = {
		.d_dname = simple_dname
	};
	struct qstr this = QSTR_INIT(name, strlen(name));
	struct path path;
	struct file *file;

	path.dentry = d_alloc_pseudo(mnt->mnt_sb, &this);
	if (!path.dentry)
		return ERR_PTR(-ENOMEM);
	if (!mnt->mnt_sb->s_d_op)
		d_set_d_op(path.dentry, &anon_ops);
	path.mnt = mntget(mnt);
	d_instantiate(path.dentry, inode);
	file = alloc_file(&path, flags, fops);
	if (IS_ERR(file)) {
		ihold(inode);
		path_put(&path);
	}
	return file;
}

struct file * __override alloc_file_clone(struct file *base, int flags,
				const struct file_operations *fops)
{
	struct file *f = alloc_file(&base->f_path, flags, fops);
	if (!IS_ERR(f)) {
		path_get(&f->f_path);
		f->f_mapping = base->f_mapping;
	}
	return f;
}

long __override ksys_rmdir(const char *pathname)
{
	liblinux_ctx_guard(long, ret);
	ret = liblinux_pal_ksys_rmdir(pathname);
	return ret;
}

long __override ksys_unlink(const char *pathname)
{
	liblinux_ctx_guard(long, ret);
	ret = liblinux_pal_ksys_unlink(pathname);
	return ret;
}

long __override do_sys_open(int dfd, const char *filename, int flags, unsigned short mode)
{
	liblinux_ctx_guard(long, ret);
	ret = (long)liblinux_pal_ksys_openat(dfd, filename, flags, mode);
	return ret;
}

int ksys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_ioctl(fd, cmd, arg, sizeof(arg));
	return ret;
}

ssize_t __override ksys_write(unsigned int fd, const char *buf, size_t count)
{
	liblinux_ctx_guard(ssize_t, ret);
	ret = liblinux_pal_ksys_write(fd, buf, count);
	return ret;
}

ssize_t __override ksys_read(unsigned int fd, char *buf, size_t count)
{
	liblinux_ctx_guard(ssize_t, ret);
	ret = liblinux_pal_ksys_read(fd, buf, count);
	return ret;
}

int __override ksys_getdents64(unsigned int fd, struct linux_dirent64 __user *dirent,
			       unsigned int count)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_readdir(fd, (char *)dirent, count);
	return ret;
}

int __override ksys_fchown(unsigned int fd, uid_t user, gid_t group)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_fchown(fd, user, group);
	return ret;
}

/* Need replace by ksys_chown */
int __override do_fchownat(int dfd, const char *filename, uid_t user,
			   gid_t group, int flag)
{
	int fd;
	int ret;
	fd = do_sys_open(dfd, filename, O_RDONLY, 0);
	if (fd < 0) {
		return fd;
	}

	ret = ksys_fchown(fd, user, group);
	ksys_close(fd);
	return ret;
}

long __override ksys_symlink(const char *oldname, const char *newname)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_symlink(oldname, newname);
	return ret;
}

off_t __override ksys_lseek(unsigned int fd, off_t offset, unsigned int whence)
{
	int64_t tmp = 0;
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_lseek(fd, offset, whence, &tmp);
	return ret >= 0 ? tmp : ret;
}

#ifdef CONFIG_BOOT_DETECTOR
long __override ksys_statfs(const char *path, struct statfs *buf)
{
	liblinux_ctx_guard(long, ret);
	ret = (long)liblinux_pal_ksys_statfs(path, buf);
	return ret;
}
#endif

int __override ksys_fsync(unsigned int fd)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_fsync(fd);
	return ret;
}

void __override ksys_sync(void)
{
	liblinux_ctx_leave();
	(void)liblinux_pal_ksys_sync();
	liblinux_ctx_enter();
}

long __override ksys_access(const char *filename, int mode)
{
	liblinux_ctx_guard(long, ret);
	ret = (long)liblinux_pal_ksys_access(filename, mode);
	return ret;
}
EXPORT_SYMBOL(ksys_access);

long __override ksys_rename(const char *oldname, const char *newname)
{
	liblinux_ctx_guard(long, ret);
	ret = (long)liblinux_pal_ksys_rename(oldname, newname);
	return ret;
}
EXPORT_SYMBOL(ksys_rename);

long __override ksys_mkdir(const char *pathname, unsigned short mode)
{
	liblinux_ctx_guard(long, ret);
	ret =  (long)liblinux_pal_ksys_mkdir(pathname, mode);
	return ret;
}
EXPORT_SYMBOL(ksys_mkdir);

int liblinux_vfs_open(const char *filename, unsigned int flag, unsigned int mode)
{
	liblinux_ctx_guard(int, fd);
	fd = liblinux_pal_vfs_open(filename, flag, mode);
	return fd;
}

int liblinux_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_vfs_ioctl(fd, cmd, arg, arg_sz);
	return ret;
}

int liblinux_vfs_fstat(int fd, struct stat *stat)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_vfs_fstat(fd, stat);
	return ret;
}

long liblinux_vfs_read(int fd, void *buf, unsigned int nbyte)
{
	liblinux_ctx_guard(long, ret);
	ret = liblinux_pal_vfs_read(fd, buf, nbyte);
	return ret;
}

int liblinux_vfs_close(int fd)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_vfs_close(fd);
	return ret;
}

int liblinux_ksys_fstat_nocheck(int fd, struct stat *stat)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_fstat_nocheck(fd, stat);
	return ret;
}

int liblinux_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags)
{
	liblinux_ctx_guard(int, ret);
	ret = liblinux_pal_ksys_fstatat(dfd, filename, stat, flags);
	return ret;
}

int __override current_umask(void)
{
	/* `umask` depend on init of cred. must call `current_cred()` here */
	if (current_cred() == &init_cred) {
		pr_warn("get kthread umask!\n");
	}
	return current_thread_info()->umask;
}
