/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Liblinux PAL implement for sysfs operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 20 18:43:51 2020
 */
#include <liblinux/pal.h>
#include <fs_sysfs.h>
#include <limits.h>
#include <devhost/log.h>
#include <hongmeng/errno.h>
#include <libalgo/idr.h>
#include <libdevhost/sysfs.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libsysif/devmgr/api.h>

static DEFINE_IDR(g_liblinux_sysfs_idr);
static struct raw_mutex g_sysfs_mutex = RAW_MUTEX_INITIALIZER;
static struct liblinux_pal_kernfs_ops _liblinux_sysfs_fops_struct;
static struct liblinux_pal_kernfs_ops * const _liblinux_sysfs_fops = &_liblinux_sysfs_fops_struct;
static struct liblinux_pal_kernfs_ops _liblinux_sysfs_ns_fops_struct;
static struct liblinux_pal_kernfs_ops * const _liblinux_sysfs_fops_ns = &_liblinux_sysfs_ns_fops_struct;

static int liblinux_sysfs_read(unsigned long long fd, unsigned long long pos, unsigned long buf,
			       unsigned long size, size_t *rsize)
{
	int ret;

	if ((_liblinux_sysfs_fops == NULL) ||
	    (_liblinux_sysfs_fops->read == NULL)) {
		return E_HM_NOSYS;
	}

	if (_liblinux_sysfs_fops->open == NULL) {
		unsigned int id = (unsigned int)fd;
		ret = _liblinux_sysfs_fops->read(pos, buf, size, &id, (unsigned long *)rsize);
	} else {
		void *filep = NULL;
		raw_mutex_lock(&g_sysfs_mutex);
		filep = idr_find(&g_liblinux_sysfs_idr, (unsigned int)fd);
		raw_mutex_unlock(&g_sysfs_mutex);
		if (filep == NULL) {
			return E_HM_INVAL;
		}

		ret = _liblinux_sysfs_fops->read(pos, buf, size, filep, (unsigned long *)rsize);
	}

	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

static int liblinux_sysfs_write(unsigned long long fd, unsigned long long pos, unsigned long buf,
				unsigned long size, size_t *wsize)
{
	int ret;

	if ((_liblinux_sysfs_fops == NULL) ||
	    (_liblinux_sysfs_fops->write == NULL)) {
		return E_HM_NOSYS;
	}

	if (_liblinux_sysfs_fops->open == NULL) {
		unsigned int id = (unsigned int)fd;
		ret = _liblinux_sysfs_fops->write(pos, buf, size, &id, (unsigned long *)wsize);
	} else {
		void *filep = NULL;
		raw_mutex_lock(&g_sysfs_mutex);
		filep = idr_find(&g_liblinux_sysfs_idr, (unsigned int)fd);
		raw_mutex_unlock(&g_sysfs_mutex);
		if (filep == NULL) {
			return E_HM_INVAL;
		}

		ret = _liblinux_sysfs_fops->write(pos, buf, size, filep, (unsigned long *)wsize);
	}
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

int liblinux_pal_sysfs_notify(const void *ctx, const char *name)
{
	int ret;
	ret = libdh_sysfs_notify((uintptr_t)ctx, name);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sysfs_rename_link_ns(const char *old_name, const char *new_name,
									  unsigned int old_nsid, unsigned int new_nsid)
{
	int ret = E_HM_OK;

	if (!__devmgr_type_is_valid_name(old_name, SYSFS_PATH_NAME_MAX) ||
	    !__devmgr_type_is_valid_name(new_name, SYSFS_PATH_NAME_MAX)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = libdh_sysfs_rename_link(old_name, new_name, old_nsid, new_nsid);
	}

	return -hmerrno2posix(ret);
}

int liblinux_pal_sysfs_rename_link(const char *old_name, const char *new_name)
{
	return liblinux_pal_sysfs_rename_link_ns(old_name, new_name, KERNFS_COMMON_NSID, KERNFS_COMMON_NSID);
}

/* This function is an ldk callback.
 * The return value must comply with posix errno.
 */
static int _liblinux_poll_notify(void *ctx, int pollable)
{
	int ret = 0;
	if (ctx == NULL) {
		return -EINVAL;
	}

	if (pollable != 0) {
		ret = libdh_sysfs_notify_insert((uintptr_t)ctx);
		ret = -hmerrno2posix(ret);
	} else {
		libdh_sysfs_notify_remove((uintptr_t)ctx);
	}

	return ret;
}

static int liblinux_sysfs_poll(unsigned int id, bool pollable)
{
	int ret;
	if ((_liblinux_sysfs_fops == NULL) ||
	    (_liblinux_sysfs_fops->poll == NULL)) {
		return E_HM_NOSYS;
	}

	if (id > INT_MAX) {
		return E_HM_INVAL;
	}

	ret = _liblinux_sysfs_fops->poll(_liblinux_poll_notify, (int)id, pollable ? 1 : 0);
	return posix2hmerrno(-ret);
}

static int liblinux_sysfs_open(void *id, unsigned long long *fd, unsigned int flags, unsigned int mode)
{
	int ret = 0;
	void *filp = NULL;

	if (unlikely(fd == NULL)) {
		return E_HM_INVAL;
	}

	/* Support no open/close version */
	if (_liblinux_sysfs_fops->open == NULL || _liblinux_sysfs_fops->close == NULL) {
		*fd = ptr_to_u64(id);
		return E_HM_OK;
	}

	filp = _liblinux_sysfs_fops->open(id, flags, mode);
	if (filp == NULL) {
		return E_HM_INVAL;
	}

	raw_mutex_lock(&g_sysfs_mutex);
	ret = idr_alloc_cyclic(&g_liblinux_sysfs_idr, filp, 1, IDR_MAX_ID);
	raw_mutex_unlock(&g_sysfs_mutex);
	if (ret < 0) {
		_liblinux_sysfs_fops->close(filp);
		return ret;
	}

	*fd = (unsigned long long)ret;
	return E_HM_OK;
}

static int liblinux_sysfs_close(unsigned long long fd)
{
	void *filp = NULL;

	if (_liblinux_sysfs_fops == NULL) {
		return E_HM_NOSYS;
	}

	/* Support no open/close version */
	if (_liblinux_sysfs_fops->open == NULL) {
		return E_HM_OK;
	}

	raw_mutex_lock(&g_sysfs_mutex);
	filp = idr_remove(&g_liblinux_sysfs_idr, (unsigned int)fd);
	raw_mutex_unlock(&g_sysfs_mutex);
	if (filp != NULL) {
		_liblinux_sysfs_fops->close(filp);
	}

	return E_HM_OK;
}

static int liblinux_sysfs_flush(void)
{
	if (_liblinux_sysfs_fops == NULL || _liblinux_sysfs_fops->flush == NULL) {
		return E_HM_NOSYS;
	}

	_liblinux_sysfs_fops->flush();
	return 0;
}

static struct libdh_kernfs_ops_simple liblinux_sysfs_fops __read_mostly = {
	.open = liblinux_sysfs_open,
	.close = liblinux_sysfs_close,
	.read = liblinux_sysfs_read,
	.write = liblinux_sysfs_write,
	.poll = liblinux_sysfs_poll,
	.flush = liblinux_sysfs_flush,
};

static int liblinux_sysfs_read_ns(unsigned long long fd, unsigned long long pos, unsigned long buf,
								  unsigned long size, size_t *rsize)
{
	int ret = E_HM_OK;

	if ((_liblinux_sysfs_fops_ns == NULL) ||
	    (_liblinux_sysfs_fops_ns->read == NULL)) {
		return E_HM_NOSYS;
	}

	if (_liblinux_sysfs_fops_ns->open == NULL) {
		unsigned int id = (unsigned int)fd;
		ret = _liblinux_sysfs_fops_ns->read(pos, buf, size, &id, (unsigned long *)rsize);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
	} else {
		return E_HM_OPNOTSUPP;
	}

	return E_HM_OK;
}

static int liblinux_sysfs_write_ns(unsigned long long fd, unsigned long long pos, unsigned long buf,
								   unsigned long size, size_t *wsize)
{
	int ret = E_HM_OK;

	if ((_liblinux_sysfs_fops_ns == NULL) ||
	    (_liblinux_sysfs_fops_ns->write == NULL)) {
		return E_HM_NOSYS;
	}

	if (_liblinux_sysfs_fops_ns->open == NULL) {
		unsigned int id = (unsigned int)fd;
		ret = _liblinux_sysfs_fops_ns->write(pos, buf, size, &id, (unsigned long *)wsize);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
	} else {
		return E_HM_OPNOTSUPP;
	}

	return E_HM_OK;
}

static int liblinux_sysfs_open_ns(void *id, unsigned long long *fd, unsigned int flags, unsigned int mode)
{
	UNUSED(flags, mode);
	if (_liblinux_sysfs_fops_ns == NULL) {
		return E_HM_NOSYS;
	}

	/* Support no open/close version */
	if (_liblinux_sysfs_fops_ns->open == NULL) {
		*fd = ptr_to_u64(id);
		return E_HM_OK;
	} else {
		return E_HM_OPNOTSUPP;
	}
}

static int liblinux_sysfs_close_ns(unsigned long long fd)
{
	UNUSED(fd);
	if (_liblinux_sysfs_fops_ns == NULL) {
		return E_HM_NOSYS;
	}

	/* Support no open/close version */
	if (_liblinux_sysfs_fops_ns->close == NULL) {
		return E_HM_OK;
	} else {
		return E_HM_OPNOTSUPP;
	}
}

static int liblinux_sysfs_poll_ns(unsigned int id, bool pollable)
{
	int ret;
	if ((_liblinux_sysfs_fops_ns == NULL) ||
	    (_liblinux_sysfs_fops_ns->poll == NULL)) {
		return E_HM_NOSYS;
	}

	if (id > INT_MAX) {
		return E_HM_INVAL;
	}

	ret = _liblinux_sysfs_fops_ns->poll(_liblinux_poll_notify, (int)id, pollable ? 1 : 0);
	return posix2hmerrno(-ret);
}

static int liblinux_sysfs_flush_ns(void)
{
	if (_liblinux_sysfs_fops_ns == NULL || _liblinux_sysfs_fops_ns->flush == NULL) {
		return E_HM_NOSYS;
	}

	_liblinux_sysfs_fops_ns->flush();
	return 0;
}

static struct libdh_kernfs_ops_simple liblinux_sysfs_fops_ns __read_mostly = {
	.open = liblinux_sysfs_open_ns,
	.close = liblinux_sysfs_close_ns,
	.read = liblinux_sysfs_read_ns,
	.write = liblinux_sysfs_write_ns,
	.poll = liblinux_sysfs_poll_ns,
	.flush = liblinux_sysfs_flush_ns,
};

int liblinux_pal_sysfs_create_dir_ns(const char *name, unsigned int mode, unsigned int nsid)
{
	int ret;
	ret = libdh_sysfs_create_dir_ns(name, mode, nsid);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sysfs_create_dir(const char *name, unsigned int mode)
{
	return liblinux_pal_sysfs_create_dir_ns(name, mode, KERNFS_COMMON_NSID);
}

int liblinux_pal_sysfs_create_file(const char *name, unsigned int mode,
				   const struct liblinux_pal_kernfs_ops *fops,
				   void *ctx)
{
	int ret;
	if (fops != NULL) {
		return -EINVAL;
	}

	ret = libdh_sysfs_create_file_simple(name, mode, mk_sysfs_tag((unsigned int)(uintptr_t)ctx));
	return -hmerrno2posix(ret);
}

int liblinux_pal_sysfs_create_file_ex(const char *name, unsigned int mode, unsigned long long size,
				   const struct liblinux_pal_kernfs_ops *fops,
				   void *ctx, unsigned int nsid)
{
	int ret;
	if (fops != NULL) {
		return -EINVAL;
	}

	if (nsid == KERNFS_COMMON_NSID) {
		ret = libdh_sysfs_create_file_simple_ex(name, mode, size,
			mk_sysfs_tag((unsigned int)(uintptr_t)ctx), nsid);
	} else {
		ret = libdh_sysfs_create_file_simple_ex(name, mode, size,
			mk_sysfsns_tag((unsigned int)(uintptr_t)ctx), nsid);
	}
	return -hmerrno2posix(ret);
}


int liblinux_pal_sysfs_create_data_batch(void *ldk_entry, int cnt)
{
	int i;
	ldk_sysfs_entry_t *tmp_ldk_entry = (ldk_sysfs_entry_t *)ldk_entry;

	for (i = 0; i < cnt; i++) {
		if (tmp_ldk_entry->type == KERNFS_FILE) {
			if (tmp_ldk_entry->nsid == KERNFS_COMMON_NSID) {
				tmp_ldk_entry->id = mk_sysfs_tag(tmp_ldk_entry->id);
			} else {
				tmp_ldk_entry->id = mk_sysfsns_tag(tmp_ldk_entry->id);
			}
		}
		tmp_ldk_entry++;
	}

	return libdh_sysfs_devmgr_create_batch(ldk_entry, cnt);
}

int liblinux_pal_sysfs_create_link_ns(const char *name, unsigned int mode,
									  const char *linkdata, unsigned int nsid)
{
	int ret;
	ret = libdh_sysfs_create_symlink_ns(name, (mode_t)mode, linkdata, nsid);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sysfs_create_link(const char *name, unsigned int mode,
								   const char *linkdata)
{
	return liblinux_pal_sysfs_create_link_ns(name, mode, linkdata, KERNFS_COMMON_NSID);
}

int liblinux_pal_sysfs_delete_ns(const char *name, unsigned int nsid)
{
	int ret;
	ret = libdh_sysfs_delete_simple(name, NULL, nsid);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sysfs_delete(const char *name)
{
	return liblinux_pal_sysfs_delete_ns(name, KERNFS_COMMON_NSID);
}

int liblinux_pal_sysfs_register_ops(void *ops)
{
	int ret;
	struct liblinux_pal_kernfs_ops *kernfs_ops = (struct liblinux_pal_kernfs_ops *)ops;

	if (_liblinux_sysfs_fops->read != NULL) {
		return -EEXIST;
	}

	ret = libdh_kernfs_register((int)DEVHOST_SYSFS, &liblinux_sysfs_fops);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	_liblinux_sysfs_fops->read = kernfs_ops->read;
	_liblinux_sysfs_fops->write = kernfs_ops->write;
	_liblinux_sysfs_fops->poll = kernfs_ops->poll;

	return ret;
}

int liblinux_pal_sysfs_register_ops_ex(void *ops)
{
	int ret;
	struct liblinux_pal_kernfs_ops *kernfs_ops = (struct liblinux_pal_kernfs_ops *)ops;

	ret = liblinux_pal_sysfs_register_ops(ops);
	if (!ret) {
		_liblinux_sysfs_fops->open = kernfs_ops->open;
		_liblinux_sysfs_fops->close = kernfs_ops->close;
		_liblinux_sysfs_fops->flush = kernfs_ops->flush;
	}
	return ret;
}

int liblinux_pal_sysfs_register_ops_ex_ns(void *ops)
{
	int ret;
	struct liblinux_pal_kernfs_ops *kernfs_ops = (struct liblinux_pal_kernfs_ops *)ops;

	if (_liblinux_sysfs_fops_ns->read != NULL) {
		return -EEXIST;
	}

	ret = libdh_kernfs_register((int)DEVHOST_SYSFS_NS, &liblinux_sysfs_fops_ns);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	_liblinux_sysfs_fops_ns->read = kernfs_ops->read;
	_liblinux_sysfs_fops_ns->write = kernfs_ops->write;
	_liblinux_sysfs_fops_ns->poll = kernfs_ops->poll;
	_liblinux_sysfs_fops_ns->open = kernfs_ops->open;
	_liblinux_sysfs_fops_ns->close = kernfs_ops->close;
	_liblinux_sysfs_fops_ns->flush = kernfs_ops->flush;

	return ret;
}
