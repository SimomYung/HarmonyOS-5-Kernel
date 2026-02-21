/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: liblinux PAL implement for procfs operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 21 08:46:22 2020
 */
#include <liblinux/pal.h>

#include <hongmeng/errno.h>
#include <libdevhost/procfs.h>
#include <devhost/err.h>

static struct liblinux_pal_kernfs_ops ___liblinux_procfs_fops_bypath_struct = {0};
static struct liblinux_pal_kernfs_ops * const __liblinux_procfs_fops_bypath = &___liblinux_procfs_fops_bypath_struct;

static int liblinux_procfs_read(const struct kernfs_entry *entry,
				const struct libdh_kernfs_rw_args *r_args,
				unsigned long *r_size_out)
{
	int ret;
	unsigned long rsize;
	const struct libdh_procfs_args *args = NULL;
	struct liblinux_pal_kernfs_ops *ops = NULL;

	if (r_args == NULL) {
		return E_HM_INVAL;
	}

	args = libdh_procfs_acquire_args(entry);
	if ((args == NULL) || (args->ops == NULL)) {
		return E_HM_INVAL;
	}

	ops = (struct liblinux_pal_kernfs_ops *)args->ops;
	if (ops->read == NULL) {
		return E_HM_INVAL;
	}

	ret = ops->read(r_args->pos, r_args->ubuf, r_args->size,
			args->ctx, &rsize);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	if (r_size_out != NULL) {
		*r_size_out = rsize;
	}
	return E_HM_OK;
}

static int liblinux_procfs_write(const struct kernfs_entry *entry,
				 const struct libdh_kernfs_rw_args *w_args,
				 unsigned long *w_size_out)
{
	int ret;
	unsigned long rsize;
	const struct libdh_procfs_args *args = NULL;
	struct liblinux_pal_kernfs_ops *ops = NULL;

	if (w_args == NULL) {
		return E_HM_INVAL;
	}

	args = libdh_procfs_acquire_args(entry);
	if ((args == NULL) || (args->ops == NULL)) {
		return E_HM_INVAL;
	}

	ops = (struct liblinux_pal_kernfs_ops *)args->ops;
	if (ops->write == NULL) {
		return E_HM_INVAL;
	}

	ret = ops->write(w_args->pos, w_args->ubuf, w_args->size,
			 args->ctx, &rsize);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	if (w_size_out != NULL) {
		*w_size_out = rsize;
	}
	return E_HM_OK;
}

static struct libdh_kernfs_ops liblinux_procfs_fops __read_mostly = {
	.read = liblinux_procfs_read,
	.write = liblinux_procfs_write,
	.poll = NULL,
};

int liblinux_pal_procfs_create_file(const char *name,
				struct liblinux_pal_kernfs_ops *ops,
				void *ctx, unsigned int mode)
{
	int ret;
	struct libdh_procfs_args liblinux_procfs_args;
	liblinux_procfs_args.ops = (void *)ops;
	liblinux_procfs_args.ctx = ctx;
	liblinux_procfs_args.mode = (mode_t)mode;

	ret = libdh_procfs_create_file("%s", &liblinux_procfs_fops,
				       &liblinux_procfs_args, NULL, name);
	return -hmerrno2posix(ret);
}

static int liblinux_procfs_read_bypath(unsigned long long fd, unsigned long long pos, unsigned long buf,
				unsigned long size, size_t *rsize_out)
{
	int ret;
	/* fd (the filep opened in ldk) is saved in vfs without change */
	void *filep = u64_to_ptr(fd, void);

	if (unlikely(__liblinux_procfs_fops_bypath->read == NULL)) {
		return E_HM_NOSYS;
	}

	ret = __liblinux_procfs_fops_bypath->read(pos, buf, size,
			filep, (unsigned long *)rsize_out);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}
static int liblinux_procfs_write_bypath(unsigned long long fd, unsigned long long pos, unsigned long buf,
				unsigned long size, size_t *wsize_out)
{
	int ret;
	/* fd (the filep opened in ldk) is saved in vfs without change */
	void *filep = u64_to_ptr(fd, void);

	if (unlikely(__liblinux_procfs_fops_bypath->write == NULL)) {
		return E_HM_NOSYS;
	}

	ret = __liblinux_procfs_fops_bypath->write(pos, buf, size,
			filep, (unsigned long *)wsize_out);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

static int liblinux_procfs_open_bypath(void *pathname, unsigned long long *fd_out,
		unsigned int flags, unsigned int mode)
{
	void *filp = NULL;

	if (unlikely(__liblinux_procfs_fops_bypath->open == NULL)) {
		return E_HM_NOSYS;
	}

	if (unlikely(fd_out == NULL)) {
		return E_HM_INVAL;
	}

	filp = __liblinux_procfs_fops_bypath->open(pathname, flags, mode);
	if (filp == NULL) {
		return E_HM_INVAL;
	} else if (IS_ERR(filp)) {
		return PTR_ERR(filp);
	}

	*fd_out = ptr_to_u64(filp);
	return E_HM_OK;
}

static int liblinux_procfs_close_bypath(unsigned long long fd)
{
	void *filp = u64_to_ptr(fd, void);

	if (unlikely(__liblinux_procfs_fops_bypath->close == NULL)) {
		return E_HM_NOSYS;
	}

	if (filp != NULL) {
		__liblinux_procfs_fops_bypath->close(filp);
	}

	return E_HM_OK;
}

static struct libdh_kernfs_ops_simple liblinux_procfs_fops_bypath __read_mostly = {
	.read = liblinux_procfs_read_bypath,
	.write = liblinux_procfs_write_bypath,
	.open = liblinux_procfs_open_bypath,
	.close = liblinux_procfs_close_bypath,
	.poll = NULL,
};

int liblinux_pal_procfs_register_ops(struct liblinux_pal_kernfs_ops *kernfs_ops)
{
	int ret;

	if (__liblinux_procfs_fops_bypath->read != NULL) {
		return -EEXIST;
	}

	ret = libdh_kernfs_register((int)DEVHOST_PROCFS, &liblinux_procfs_fops_bypath);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	__liblinux_procfs_fops_bypath->read = kernfs_ops->read;
	__liblinux_procfs_fops_bypath->write = kernfs_ops->write;
	__liblinux_procfs_fops_bypath->open = kernfs_ops->open;
	__liblinux_procfs_fops_bypath->close = kernfs_ops->close;

	return 0;
}

int liblinux_pal_procfs_create_file_bypath(const char *name, unsigned int mode)
{
	int ret;
	struct libdh_procfs_args liblinux_procfs_args = {0};

	liblinux_procfs_args.mode = (mode_t)mode;
	/* Now need_passthrough 1 means do direct read\write for procfs node created by it */
	liblinux_procfs_args.need_passthrough = 1;
	ret = libdh_procfs_create_file_ldk_simple(name, &liblinux_procfs_args);
	return -hmerrno2posix(ret);
}

int liblinux_pal_procfs_create_dir(const char *name, unsigned int mode)
{
	int ret;
	ret = libdh_procfs_mkdir("%s", mode, name);
	return -hmerrno2posix(ret);
}

int liblinux_pal_procfs_create_link(const char *name, unsigned int mode,
				    char *linkdata)
{
	int ret;
	ret = libdh_procfs_symlink("%s", (mode_t)mode, linkdata, name);
	return -hmerrno2posix(ret);
}

int liblinux_pal_procfs_delete(const char *name)
{
	int ret;
	ret = libdh_procfs_delete("%s", name);
	return -hmerrno2posix(ret);
}

int liblinux_pal_procfs_delete_bypath(const char *name)
{
	int ret;

	ret = libdh_procfs_delete_ldk_simple(name);
	return -hmerrno2posix(ret);
}
