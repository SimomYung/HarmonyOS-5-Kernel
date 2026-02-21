/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: PAL implement for sysfs operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 9 09:45:52 2020
 */
#include <libdevhost/sysfs.h>

#include <fs_sysfs.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libdevhost/devhost.h>

#include "internals.h"
#include "kernfs_internal.h"

static struct libdh_kernfs_context sysfs_ctx;

struct libdh_sysfs_entry {
	struct kernfs_entry kernfs;
	struct libdh_sysfs_args args;
};

int libdh_sysfs_devmgr_create_batch(void *ldk_entry, int cnt)
{
	int ret = actvcapcall_drvcall_devmgr_sysfs_create_batch(libdh_get_devmgr_ap(),
								ldk_entry, cnt);
	if (ret < 0) {
		hm_error("sysfs create batch failed ret %d\n", ret);
	}

	return ret;
}

int libdh_sysfs_init(void)
{
	int ret;
	if (!libdh_is_devmgr_ready()) {
		ret = E_HM_INVAL;
	} else {
		ret = libdh_kernfs_init(&sysfs_ctx);
	}
	return ret;
}

int libdh_sysfs_create_dir_ns(const char *name, unsigned int mode, unsigned int nsid)
{
	int ret = E_HM_OK;
	struct hm_actv_buf path;

	if (name == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hm_actv_buf_init(&path, name, strlen(name) + 1U);

		ret = actvcapcall_drvcall_devmgr_sysfs_create_dir(
				libdh_get_devmgr_ap(), path, mode, nsid);
		if ((ret < 0) && (ret != E_HM_POSIX_EXIST)) {
			hm_debug("create sysfs dir failed, path=%s, ret=%s\n",
				 name, hmstrerror(ret));
		} else {
			ret = E_HM_OK;
		}
	}

	return ret;
}

int libdh_sysfs_create_dir(const char *name, unsigned int mode)
{
	return libdh_sysfs_create_dir_ns(name, mode, KERNFS_COMMON_NSID);
}

static void __sysfs_release_entry(struct kernfs_entry *_entry)
{
	struct libdh_sysfs_entry *entry = NULL;

	BUG_ON(_entry == NULL);
	entry = container_of(_entry, struct libdh_sysfs_entry, kernfs);
	libdh_sysfs_entry_free(entry);
}

static struct libdh_sysfs_entry *
__alloc_sysfs_entry(struct libdh_kernfs_ops *fops,
		    const struct libdh_sysfs_args *create_args)
{
	struct libdh_sysfs_entry *entry = NULL;

	entry = (struct libdh_sysfs_entry *)libdh_sysfs_entry_malloc();
	if (entry != NULL) {
		entry->args.ops = create_args->ops;
		entry->args.ctx = create_args->ctx;
		entry->args.mode = create_args->mode;
		entry->kernfs.ops = fops;
		entry->kernfs.release = __sysfs_release_entry;
	}

	return entry;
}

size_t libdh_sysfs_entry_size(void)
{
	return sizeof(struct libdh_sysfs_entry);
}

int libdh_sysfs_create_file_simple(const char *name, unsigned int mode, int id)
{
	int ret;
	struct hm_actv_buf path;

	if (name == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&path, name, strlen(name) + 1U);

	ret = actvcapcall_drvcall_devmgr_sysfs_create_file(
			libdh_get_devmgr_ap(), path, mode, id);
	if (ret < 0) {
		hm_debug("create sysfs file failed, path=%s, ret=%s\n",
			 name, hmstrerror(ret));
	}

	return ret;
}

int libdh_sysfs_create_file_simple_ex(const char *name, unsigned int mode, unsigned long long size,
				    				  int id, unsigned int nsid)
{
	int ret;
	struct hm_actv_buf path;

	if (name == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&path, name, strlen(name) + 1U);

	ret = actvcapcall_drvcall_devmgr_sysfs_create_file_ex(
			libdh_get_devmgr_ap(), path, mode, size, id, nsid);
	if (ret < 0) {
		hm_debug("create sysfs file failed, path=%s, ret=%s\n",
			 name, hmstrerror(ret));
	}

	return ret;
}

int libdh_sysfs_create_file(const char *name, struct libdh_kernfs_ops *fops,
			    const struct libdh_sysfs_args *create_args,
			    int *tag_out)
{
	int ret, tag;
	struct libdh_sysfs_entry *entry = NULL;

	if (name == NULL) {
		ret = E_HM_INVAL;
		goto err_out;
	}

	entry = __alloc_sysfs_entry(fops, create_args);
	if (entry == NULL) {
		ret = E_HM_NOMEM;
		goto err_out;
	}

	ret = libdh_kernfs_insert(&sysfs_ctx, &entry->kernfs, &tag);
	if (ret < 0) {
		libdh_sysfs_entry_free(entry);
		goto err_out;
	}

	ret = libdh_sysfs_create_file_simple(name, create_args->mode, tag);
	if (ret < 0) {
		hm_debug("create sysfs file failed, path=%s, ret=%s\n",
			 name, hmstrerror(ret));
		libdh_kernfs_remove(&sysfs_ctx, tag);
		goto err_out;
	}

	if (tag_out != NULL) {
		*tag_out = tag;
	}
err_out:
	return ret;
}

int libdh_sysfs_create_symlink_ns(const char *name, mode_t mode,
			       const char *linkdata, unsigned int nsid)
{
	int ret = E_HM_OK;
	struct hm_actv_buf path;
	struct hm_actv_buf linkpath;

	if ((name == NULL) || (linkdata == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hm_actv_buf_init(&path, name, strlen(name) + 1U);
		hm_actv_buf_init(&linkpath, linkdata, strlen(linkdata) + 1U);

		ret = actvcapcall_drvcall_devmgr_sysfs_create_symlink(
				libdh_get_devmgr_ap(), path, mode, linkpath, nsid);
		if (ret < 0) {
			hm_debug("create sysfs symlink failed, path=%s, link_path=%s, ret=%s\n",
				 name, linkdata, hmstrerror(ret));
		}
	}

	return ret;
}

int libdh_sysfs_create_symlink(const char *name, mode_t mode,
			       const char *linkdata)
{
	return libdh_sysfs_create_symlink_ns(name, mode, linkdata, KERNFS_COMMON_NSID);
}

int libdh_sysfs_delete_simple(const char *name, int *tagid, unsigned int nsid)
{
	int ret = E_HM_OK;
	struct hm_actv_buf path;
	struct __actvret_drvcall_devmgr_sysfs_delete pret;

	hm_actv_buf_init(&path, name, strlen(name) + 1U);

	ret = actvcapcall_drvcall_devmgr_sysfs_delete(libdh_get_devmgr_ap(),
							      path, nsid, &pret);
	if (tagid != NULL) {
		*tagid = pret.tagid;
	}

	return ret;
}

int libdh_sysfs_delete(const char *name)
{
	int ret, tagid;

	if (name == NULL) {
		return E_HM_INVAL;
	}

	ret = libdh_sysfs_delete_simple(name, &tagid, KERNFS_COMMON_NSID);
	if (ret != E_HM_OK) {
		libdh_kernfs_tagid_invalid(&sysfs_ctx, tagid);
	}

	if (ret == E_HM_OK) {
		libdh_kernfs_remove(&sysfs_ctx, tagid);
	}

	return ret;
}

int libdh_sysfs_notify(uintptr_t kernfs_node_ctx, const char *name)
{
	int ret = E_HM_OK;
	struct hm_actv_buf path;

	if (name == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = libdh_kernfs_notify_search(kernfs_node_ctx);
	}

	if (ret == E_HM_OK) {
		hm_actv_buf_init(&path, name, strlen(name) + 1U);

		ret = actvcapcall_drvcall_devmgr_sysfs_notify(libdh_get_devmgr_ap(),
							      path);
	}

	return ret;
}

int libdh_sysfs_rename_link(const char *old_name, const char *new_name,
							unsigned int old_nsid, unsigned int new_nsid)
{
	int ret = E_HM_OK;
	struct hm_actv_buf old_path;
	struct hm_actv_buf new_path;

	if (!__devmgr_type_is_valid_name(old_name, SYSFS_PATH_NAME_MAX) ||
	    !__devmgr_type_is_valid_name(new_name, SYSFS_PATH_NAME_MAX)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hm_actv_buf_init(&old_path, old_name, strlen(old_name) + 1U);
		hm_actv_buf_init(&new_path, new_name, strlen(new_name) + 1U);

		ret = actvcapcall_drvcall_devmgr_sysfs_rename_link(libdh_get_devmgr_ap(),
							      old_path, new_path, old_nsid, new_nsid);
	}

	return ret;
}
int libdh_sysfs_read(unsigned int tagid, unsigned long long pos,
		     const struct hm_actv_buf *ubuf, uintptr_t ctx,
		     size_t *rsize)
{
	struct libdh_kernfs_rw_args r_args;
	r_args.pos = pos;
	r_args.ubuf = (unsigned long)ubuf->buf;
	r_args.size = (unsigned long)ubuf->size;

	return libdh_kernfs_read(&sysfs_ctx, tagid, &r_args, ctx, rsize);
}

int libdh_sysfs_write(unsigned int tagid, unsigned long long pos,
		      const struct hm_actv_buf *ubuf, uintptr_t ctx,
		      size_t *wsize)
{
	struct libdh_kernfs_rw_args w_args;
	w_args.pos = pos;
	w_args.ubuf = (unsigned long)ubuf->buf;
	w_args.size = (unsigned long)ubuf->size;

	return libdh_kernfs_write(&sysfs_ctx, tagid, &w_args, ctx, wsize);
}

int libdh_sysfs_poll(unsigned int tagid, uintptr_t ctx, bool pollable)
{
	return libdh_kernfs_poll(&sysfs_ctx, tagid, ctx, pollable);
}

int libdh_sysfs_notify_insert(uintptr_t kernfs_node_ctx)
{
	return libdh_kernfs_notify_insert(kernfs_node_ctx);
}

void libdh_sysfs_notify_remove(uintptr_t kernfs_node_ctx)
{
	libdh_kernfs_notify_remove(kernfs_node_ctx);
}

const struct libdh_sysfs_args *
libdh_sysfs_acquire_args(const struct kernfs_entry *_entry)
{
	const struct libdh_sysfs_entry *entry = NULL;
	const struct libdh_sysfs_args *args = NULL;
	if (_entry != NULL) {
		entry = container_of(_entry, struct libdh_sysfs_entry, kernfs);
		args = &entry->args;
	}
	return args;
}
