// SPDX-License-Identifier: GPL-2.0-only
#include <linux/cdev.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/splice.h>
#include <linux/compat.h>
#include <linux/backing-dev.h>
#include <linux/kmemleak.h>
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
	liblinux_pal_ksys_close(fd);
	return 0;
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

int __override current_umask(void)
{
	/* `umask` depend on init of cred. must call `current_cred()` here */
	if (current_cred() == &init_cred) {
		pr_warn("get kthread umask!\n");
	}
	return current_thread_info()->umask;
}
