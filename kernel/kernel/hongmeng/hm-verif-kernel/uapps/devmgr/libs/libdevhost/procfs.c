/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Libdh procfs helpers
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 19 08:53:16 2020
 */
#include <libdevhost/procfs.h>

#include <stdarg.h>
#include <fs_procfs.h>
#include <sys/stat.h>
#include <libalgo/idr.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/api.h>
#include <libhmucap/ucap.h>
#include <libdevhost/map.h>
#include <libdevhost/kernfs.h>
#include <libdevhost/devhost.h>
#include <libhwsecurec/securec.h>
#include <fs/version.h>

#include "internals.h"
#include "kernfs_internal.h"

static struct libdh_kernfs_context procfs_ctx;
static struct libdh_map_context procfs_map_ctx;
static bool is_remote = false;

struct libdh_procfs_entry {
	struct kernfs_entry kernfs;
	struct libdh_procfs_args args;
};

struct __procfs_ops {
	int (*create)(const char *name, mode_t mode,
		      const struct procfs_ops_cb *ops, const char *linkdata);
	int (*unlink)(const char *path);
};

static struct __procfs_ops __ops __read_mostly = {
	.create = procfs_create_data,
	.unlink = procfs_delete_data,
};

int __attribute__((format(printf, 2, 3)))
libdh_procfs_init(bool remote, const char *basefmt, ...)
{
	va_list args;
	int ret;

	/* init procfs ctx */
	ret = libdh_kernfs_init(&procfs_ctx);
	if (ret == E_HM_OK) {
		ret = libdh_str_map_init(&procfs_map_ctx);
	}

	/* build base if require */
	if ((ret == E_HM_OK) && (basefmt != NULL)) {
		va_start(args, basefmt);
		ret = vsprintf_s(procfs_ctx.base, sizeof(procfs_ctx.base),
				 basefmt, args);
		va_end(args);
		if (ret < 0) {
			ret = E_HM_INVAL;
		} else {
			ret = E_HM_OK;
		}
	}

	if (ret == E_HM_OK) {
		is_remote = remote;
	}

	return ret;
}

static int __attribute__((format(printf, 3, 0)))
__libdh_procfs_path(char *name, size_t size,
		    const char *namefmt, va_list args)
{
	int ret = E_HM_OK;
	size_t name_len;

	/* check args */
	if ((name == NULL) || (size == 0UL) || (namefmt == NULL)) {
		ret = E_HM_INVAL;
		goto err_out;
	}

	/* copy procfs ctx base first if local */
	if (!is_remote) {
		NOFAIL(strncpy_s(name, size,
				 procfs_ctx.base, sizeof(procfs_ctx.base)));
	}
	name_len = strlen(name);

	/* append namefmt */
	ret = vsnprintf_s(name + name_len,
			  size - name_len, (size - name_len) - 1U,
			  namefmt, args);
	if (ret < 0) {
		ret = E_HM_INVAL;
	} else {
		ret = E_HM_OK;
	}
err_out:
	return ret;
}

int libdh_procfs_set_remote_ops(int (*create)(const char *path, mode_t mode,
					      const struct procfs_ops_cb *ops,
					      const char *linkpath),
				int (*unlink)(const char *path))
{
	if (!is_remote) {
		return E_HM_INVAL;
	}

	__ops.create = create;
	__ops.unlink = unlink;
	return E_HM_OK;
}

static __hmerr_t __attribute__((format(printf, 1, 0)))
__proc_create(const char *namefmt, const struct procfs_ops_cb *ops,
	      mode_t mode, const char *linkdata, va_list args)
{
	char *name = NULL;
	__hmerr_t ret;

	name = (char *)malloc(PROCFS_PATH_NAME_MAX);
	if (name == NULL) {
		ret = E_HM_NOMEM;
		goto err_malloc;
	}
	mem_zero_p(name, PROCFS_PATH_NAME_MAX, PROCFS_PATH_NAME_MAX);

	ret = __libdh_procfs_path(name, PROCFS_PATH_NAME_MAX, namefmt, args);
	if (ret < 0) {
		goto err;
	}

	ret = __ops.create(name, mode, ops, linkdata);
err:
	free(name);
err_malloc:
	return ret;
}

static void __procfs_release_entry(struct kernfs_entry *_entry)
{
	struct libdh_procfs_entry *entry = NULL;

	BUG_ON(_entry == NULL);
	entry = container_of(_entry, struct libdh_procfs_entry, kernfs);
	free(entry->args.pathname);
	free(entry);
}

static int __kernfs_insert_file(struct procfs_ops_cb *libdh_ops,
				struct libdh_kernfs_ops *ops,
				struct libdh_procfs_args *create_args,
				char *name,
				int *tag_out)
{
	__u32 self_cnode_idx;
	int ret = E_HM_OK;
	int tag = 0;
	struct libdh_procfs_entry *entry = NULL;

	self_cnode_idx = hm_ucap_self_cnode_idx();
	if (is_cnode_idx_err(self_cnode_idx)) {
		ret = cnode_idx_to_err(self_cnode_idx);
		goto err_out;
	}

	entry = (struct libdh_procfs_entry *)malloc(sizeof(*entry));
	if (entry == NULL) {
		ret = E_HM_NOMEM;
		goto err_out;
	}

	entry->args.pathname = name;
	entry->args.ops = create_args->ops;
	entry->args.ctx = create_args->ctx;
	entry->args.mode = create_args->mode;
	entry->kernfs.ops = ops;
	entry->kernfs.release = __procfs_release_entry;

	ret = libdh_kernfs_insert(&procfs_ctx, &entry->kernfs, &tag);
	if (ret < 0) {
		free(entry);
		goto err_out;
	}

	libdh_ops->tagid = !is_remote ? tag : mk_remote_tag((unsigned int)tag);
	libdh_ops->ctx = (uint64_t)self_cnode_idx;

	if (tag_out != NULL) {
		*tag_out = tag;
	}
err_out:
	return ret;
}

static int procfs_idr_entry_acquire(int tagid, struct kernfs_entry **entry_out)
{
	int ret;

	ret = libdh_kernfs_entry_acquire(&procfs_ctx, tagid, entry_out);
	return ret;
}

static void __procfs_release_entry_ldk_simple(struct kernfs_entry *entry)
{
	BUG_ON(entry == NULL || entry->pathname == NULL);
	free(entry->pathname);
	free(entry);
}

static int procfs_idr_insert(const char *name_in, int *tagid_out)
{
	int ret = E_HM_INVAL;
	char *name = NULL;
	struct kernfs_entry *kernfs_entry = NULL;

	name = (char *)malloc(PROCFS_PATH_NAME_MAX);
	if (name == NULL) {
		return E_HM_NOMEM;
	}

	kernfs_entry = (struct kernfs_entry *)malloc(sizeof(*kernfs_entry));
	if (kernfs_entry == NULL) {
		free(name);
		return E_HM_NOMEM;
	}

	NOFAIL(strncpy_s(name, PROCFS_PATH_NAME_MAX, name_in, strlen(name_in)));
	kernfs_entry->pathname = name;
	kernfs_entry->release = __procfs_release_entry_ldk_simple;
	ret = libdh_kernfs_insert(&procfs_ctx, kernfs_entry, tagid_out);
	if (ret < 0) {
		free(kernfs_entry);
		free(name);
	}

	return ret;
}

static void procfs_idr_remove_by_id(int tagid)
{
	libdh_kernfs_remove(&procfs_ctx, tagid);
}

static int procfs_idr_remove_by_key(const char *name)
{
	int ret;
	int tagid = LIBDH_KERNFS_TAG_MAX + 1;

	ret = libdh_str_map_delete(&procfs_map_ctx, name, &tagid);
	if (ret) {
		hm_error("libdh: name-id map delete <%s, %x> failed, ret=%s\n", name, tagid, hmstrerror(ret));
		return ret;
	}

	procfs_idr_remove_by_id(tagid);
	return ret;
}

int libdh_procfs_create_file_ldk_simple(const char *name,
			 struct libdh_procfs_args *create_args)
{
	int ret = 0;
	int tagid;
	__u32 self_cnode_idx;
	struct procfs_ops_cb libdh_ops = {
		.svpname = DEVMGR_PATH,
		.read_method_id = (unsigned int)__devhost_method_procfs_read,
		.write_method_id = (unsigned int)__devhost_method_procfs_write,
		.open_method_id = (unsigned int)__devhost_method_procfs_open,
		.close_method_id = (unsigned int)__devhost_method_procfs_close,
		.need_passthrough = create_args->need_passthrough,
		.tagid = 0, .ctx = 0ULL };

	self_cnode_idx = hm_ucap_self_cnode_idx();
	if (is_cnode_idx_err(self_cnode_idx)) {
		return cnode_idx_to_err(self_cnode_idx);
	}

	ret = procfs_idr_insert(name, &tagid);
	if (ret < 0) {
		return ret;
	}

	ret = libdh_str_map_insert(&procfs_map_ctx, name, tagid);
	if (ret < 0) {
		hm_error("libdh: str map insert %s failed, ret=%s\n", name, hmstrerror(ret));
		goto map_insert_err;
	}

	libdh_ops.tagid = mk_procfs_tag((unsigned int)tagid);
	libdh_ops.ctx = (uint64_t)self_cnode_idx;
	ret =  __ops.create(name, create_args->mode, &libdh_ops, NULL);
	if (ret == 0 ) {
		return 0;
	}

	(void)libdh_str_map_delete(&procfs_map_ctx, name, &tagid);
map_insert_err:
	procfs_idr_remove_by_id(tagid);
	return ret;
}

int libdh_procfs_delete_ldk_simple(const char *name)
{
	int ret = 0;

	ret = procfs_idr_remove_by_key(name);
	if (ret == 0) {
		ret = __ops.unlink(name);
	}

	return ret;
}

int libdh_procfs_open_simple(unsigned int tagid, int flags, int mode, uintptr_t ctx, unsigned long long *fd_out)
{
	int ret = E_HM_OK;
	struct kernfs_entry *entry = NULL;

	if (is_devhost_remote_tag(tagid)) {
		/* only ldk procfs node support open now */
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);

		ret = E_HM_INVAL;
		if (ops != NULL) {
			ret = procfs_idr_entry_acquire(local_tag_of(tagid), &entry);
			if (ret == E_HM_OK) {
				BUG_ON(entry == NULL);
				ret = ops->open(entry->pathname, fd_out, flags, mode);
				libdh_kernfs_put(entry);
			}
		}
	}

	return ret;
}

int libdh_procfs_close_simple(unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	int ret = E_HM_OK;

	if (fd != FAKE_FD && is_devhost_remote_tag(tagid)) {
		/* only ldk procfs node support open now */
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);

		ret = E_HM_INVAL;
		if (ops != NULL) {
			ret = ops->close(fd);
		}
	}

	return ret;
}

int __attribute__((format(printf, 1, 5)))
libdh_procfs_create_file(const char *namefmt, struct libdh_kernfs_ops *fops,
			 struct libdh_procfs_args *create_args,
			 int *tagid_out, ...)
{
	struct procfs_ops_cb libdh_ops = {
		.svpname = DEVMGR_PATH,
		.read_method_id = (unsigned int)__devmgr_method_procfs_read,
		.write_method_id = (unsigned int)__devmgr_method_procfs_write,
		.open_method_id = 0,
		.close_method_id = 0,
		.need_passthrough = 0,
		.tagid = 0, .ctx = 0ULL };
	char *name = NULL;
	va_list args;
	int ret, tagid;

	name = (char *)malloc(PROCFS_PATH_NAME_MAX);
	if (name == NULL) {
		ret = E_HM_NOMEM;
		goto err_malloc;
	}
	mem_zero_p(name, PROCFS_PATH_NAME_MAX, PROCFS_PATH_NAME_MAX);

	va_start(args, tagid_out);
	ret = __libdh_procfs_path(name, PROCFS_PATH_NAME_MAX, namefmt, args);
	if (ret < 0) {
		free(name);
		goto make_path_err;
	}

	ret = __kernfs_insert_file(&libdh_ops, fops, create_args, name, &tagid);
	if (ret < 0) {
		/* After file is inserted in kernfs successfully, `name` will be freed in libdh_kernfs_release */
		free(name);
		goto kernfs_insert_err_or_succ;
	}

	ret = libdh_str_map_insert(&procfs_map_ctx, name, tagid);
	if (ret < 0) {
		hm_error("libdh: str map insert failed, ret=%s\n", hmstrerror(ret));
		goto map_insert_err;
	}

	ret = __proc_create(namefmt, &libdh_ops, create_args->mode, NULL, args);
	if (ret < 0) {
		goto proc_create_err;
	}

	if (tagid_out != NULL) {
		*tagid_out = tagid;
	}

	goto kernfs_insert_err_or_succ;
proc_create_err:
	(void)libdh_str_map_delete(&procfs_map_ctx, name, NULL);
map_insert_err:
	libdh_kernfs_release(&procfs_ctx, tagid);
kernfs_insert_err_or_succ:
make_path_err:
	va_end(args);
err_malloc:
	return ret;
}

int __attribute__((format(printf, 1, 3)))
libdh_procfs_mkdir(const char *namefmt, mode_t mode, ...)
{
	va_list args;
	int ret;

	va_start(args, mode);
	ret = __proc_create(namefmt, NULL, mode, NULL, args);
	va_end(args);
	return ret;
}

int __attribute__((format(printf, 1, 4)))
libdh_procfs_symlink(const char *namefmt, mode_t mode, char *linkdata, ...)
{
	va_list args;
	int ret;

	va_start(args, linkdata);
	ret = __proc_create(namefmt, NULL, mode, linkdata, args);
	va_end(args);
	return ret;
}

int __attribute__((format(printf, 1, 2)))
libdh_procfs_delete(const char *namefmt, ...)
{
	char *name = NULL;
	va_list args;
	int ret, tagid;

	name = (char *)malloc(PROCFS_PATH_NAME_MAX);
	if (name == NULL) {
		ret = E_HM_NOMEM;
		goto err_malloc;
	}
	mem_zero_p(name, PROCFS_PATH_NAME_MAX, PROCFS_PATH_NAME_MAX);

	/* build procfs path */
	va_start(args, namefmt);
	ret = __libdh_procfs_path(name, PROCFS_PATH_NAME_MAX, namefmt, args);
	va_end(args);
	if (ret < 0) {
		goto err;
	}

	ret = __ops.unlink(name);
	if (libdh_str_map_delete(&procfs_map_ctx, name, &tagid) >= 0) {
	/* The map and kernfs will not record dir's and symlink's name,
	 * so it need handle tagid only if find the name.
	 */
		if (ret < 0) {
			libdh_kernfs_tagid_invalid(&procfs_ctx, tagid);
		} else {
			libdh_kernfs_remove(&procfs_ctx, tagid);
		}
	}

err:
	free(name);
err_malloc:
	return ret;
}

int libdh_procfs_read(unsigned int tagid, unsigned long long pos,
		      const struct hm_actv_buf *ubuf, uintptr_t ctx,
		      size_t *rsize /* out */)
{
	struct libdh_kernfs_rw_args r_args;
	r_args.pos = pos;
	r_args.ubuf = (unsigned long)ubuf->buf;
	r_args.size = (unsigned long)ubuf->size;

	return libdh_kernfs_read(&procfs_ctx, tagid, &r_args, ctx, rsize);
}

int libdh_procfs_write(unsigned int tagid, unsigned long long pos,
		       const struct hm_actv_buf *ubuf, uintptr_t ctx,
		       size_t *wsize /* out */)
{
	struct libdh_kernfs_rw_args w_args;
	w_args.pos = pos;
	w_args.ubuf = (unsigned long)ubuf->buf;
	w_args.size = (unsigned long)ubuf->size;

	return libdh_kernfs_write(&procfs_ctx, tagid, &w_args, ctx, wsize);
}

const struct libdh_procfs_args *
libdh_procfs_acquire_args(const struct kernfs_entry *_entry)
{
	const struct libdh_procfs_entry *entry = NULL;
	const struct libdh_procfs_args *args = NULL;
	if (_entry != NULL) {
		entry = container_of(_entry, struct libdh_procfs_entry, kernfs);
		args = &entry->args;
	}
	return args;
}
