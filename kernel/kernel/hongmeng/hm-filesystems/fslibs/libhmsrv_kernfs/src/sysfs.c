/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Sysfs functions implementations
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 15:22:20 2020
 */

#include <fs_sysfs.h>

#include <unistd.h>
#include <inttypes.h>
#include <fs_mutex.h>

#include <libalgo/idr.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/atomic.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_path.h>
#include <fslib_common.h>

#include "kernfslib_common.h"

#define SYSFS_PATH_ACTV "fs-sysfs-actv"

static rref_t sysfs_rref;

/* protect for idr slot */
static struct raw_rwlock sysfs_idr_rwlock = RAW_RDLOCK_INITIALIZER;

static int get_sysfs_rref(rref_t *rref)
{
	int ret;

	if (sysfs_rref != 0) {
		*rref = sysfs_rref;
		return E_HM_OK;
	}

	ret = kernfslib_get_rref(SYSFS_PATH_ACTV, &sysfs_rref);
	if (ret != E_HM_OK) {
		return ret;
	}

	*rref = sysfs_rref;

	return E_HM_OK;
}

int sysfs_data_create_batch(ldk_sysfs_entry_t *ldk_enrty, struct sysfs_ops_cb *ops_arr, int cnt)
{
	int err;
	rref_t rref = 0;

	err = get_sysfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_sysfs_create_batch(rref, (void *)ldk_enrty, ops_arr, cnt);
	if (err != E_HM_OK) {
		hm_error("fscall sysfs_create_batch failed, err=%s\n", hmstrerror(err));
	}

	return err;
}

/* size of sysfs's regular file is PAGE_SIZE as default both in sysfs_create_file()
 * and sysfs_create_file_helper(), if you want to make it different please use
 * sysfs_create_file_attr() and sysfs_create_file_attr_helper()
 */
static int do_create_file(const char *name, mode_t mode, const struct sysfs_ops_cb *ops,
			  size_t size, unsigned int nsid)
{
	rref_t rref = 0;
	int err;

	if (ops == NULL) {
		hm_error("sysfs_ops_cb is null\n");
		return E_HM_INVAL;
	}

	if (name == NULL || (strlen(name) + 1) > SYSFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	err = get_sysfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_sysfs_create_file(rref, name, strlen(name) + 1,
						   mode, *ops, (unsigned long)size, nsid);
	if (err != E_HM_OK) {
		hm_debug("fscall sysfs_create_file failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int sysfs_create_file(const char *name, mode_t mode, const struct sysfs_ops_cb *ops)
{
	/* set sysfs's file default size as PAGE_SIZE */
	return do_create_file(name, mode, ops, PAGE_SIZE, KERNFS_COMMON_NSID);
}

int sysfs_create_dir_ns(const char *name, mode_t mode, unsigned int nsid)
{
	rref_t rref = 0;
	int ret;

	if (name == NULL || (strlen(name) + 1) > SYSFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	ret = get_sysfs_rref(&rref);
	if (ret != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvcapcall_fscall_sysfs_create_dir(rref, name, strlen(name) + 1, mode, nsid);
	if (ret != E_HM_OK) {
		hm_debug("fscall sysfs_create_dir failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}

	return E_HM_OK;
}

int sysfs_create_dir(const char *name, mode_t mode)
{
	return sysfs_create_dir_ns(name, mode, KERNFS_COMMON_NSID);
}

int sysfs_create_symlink_ns(const char *name, mode_t mode, const char *linkdata, unsigned int nsid)
{
	rref_t rref = 0;
	int err;

	if (name == NULL || (strlen(name) + 1) > SYSFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	if (linkdata == NULL || strlen(linkdata) + 1 > PATH_MAX) {
		hm_debug("invalid linkdata\n");
		return E_HM_INVAL;
	}

	err = get_sysfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_sysfs_create_symlink(rref, (char *)name, strlen(name) + 1,
						      mode, nsid, (char *)linkdata, strlen(linkdata) + 1);
	if (err != E_HM_OK) {
		hm_debug("fscall sysfs_create_symlink failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int sysfs_create_symlink(const char *name, mode_t mode, const char *linkdata)
{
	return sysfs_create_symlink_ns(name, mode, linkdata, KERNFS_COMMON_NSID);
}

int sysfs_delete_ns(const char *name, int *tagid, unsigned int nsid)
{
	rref_t rref = 0;
	int rc;
	struct __actvret_fscall_sysfs_delete ret = {-1};

	if (name == NULL || (strlen(name) + 1) > SYSFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	rc = get_sysfs_rref(&rref);
	if (rc != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, rc=%s\n", hmstrerror(rc));
		return rc;
	}

	rc = actvcapcall_fscall_sysfs_delete(rref, name, strlen(name) + 1, nsid, &ret);
	if (rc != E_HM_OK) {
		hm_debug("fscall sysfs_delete_data failed, rc=%s\n", hmstrerror(rc));
		return rc;
	}

	if (tagid != NULL) {
		*tagid = ret.tagid;
	}

	return E_HM_OK;
}

int sysfs_delete(const char *name, int *tagid)
{
	return sysfs_delete_ns(name, tagid, KERNFS_COMMON_NSID);
}

int sysfs_rename_link_ns(const char *old_path, const char *new_path,
					  unsigned int old_nsid, unsigned int new_nsid)
{
	rref_t rref = 0;
	int rc = E_HM_OK;

	if (old_path == NULL || (strlen(old_path) + 1) > SYSFS_PATH_NAME_MAX ||
	    new_path == NULL || (strlen(new_path) + 1) > SYSFS_PATH_NAME_MAX ) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	rc = get_sysfs_rref(&rref);
	if (rc != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, rc=%s\n", hmstrerror(rc));
		return rc;
	}

	rc = actvcapcall_fscall_sysfs_rename_link(rref, old_path, strlen(old_path) + 1,
						  new_path, strlen(new_path) + 1, old_nsid, new_nsid);
	if (rc != E_HM_OK) {
		hm_debug("fscall sysfs_rename_link failed, rc=%s\n", hmstrerror(rc));
	}

	return rc;
}

int sysfs_rename_link(const char *old_path, const char *new_path)
{
	return sysfs_rename_link_ns(old_path, new_path, KERNFS_COMMON_NSID, KERNFS_COMMON_NSID);
}

int sysfs_notify(const char *name, size_t name_size)
{
	rref_t rref = 0;
	int err;

	if (name == NULL || name_size > SYSFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	err = get_sysfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_sysfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_sysfs_notify(rref, (char *)name, name_size);
	if (err != E_HM_OK) {
		hm_debug("fscall sysfs_notify failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

/*
 * Following APIs are wrapped helper functions for easier usage when
 * create/read/write/delete sysfs files.
 *
 * They will help to manage ops,ctx -> tagid convesion and
 * read/write auto dispatch.
 */
struct sysfs_file_info {
	const char *name;
	sysfs_file_ops *ops;
	void *ctx;
};

static DEFINE_IDR(sysfs_idr);

static int gen_tagid(const char *name, sysfs_file_ops *ops, void *ctx, int *tagid_out)
{
	int tagid;
	struct sysfs_file_info *info = NULL;

	info = (struct sysfs_file_info *)malloc(sizeof(struct sysfs_file_info));
	if (info == NULL) {
		return E_HM_NOMEM;
	}

	info->name = name;
	info->ops = ops;
	info->ctx = ctx;

	(void)raw_rwlock_wrlock(&sysfs_idr_rwlock);
	tagid = idr_alloc(&sysfs_idr, info);
	(void)raw_rwlock_unlock(&sysfs_idr_rwlock);
	if (tagid < 0) {
		free(info);
		return tagid;
	}

	if (tagid_out != NULL) {
		*tagid_out = tagid;
	}

	return E_HM_OK;
}

struct name_tagid {
	const char *name;
	int *tagid;
};

static int find_name(unsigned int id, void *ptr, void *data)
{
	struct sysfs_file_info *info = (struct sysfs_file_info *)ptr;
	struct name_tagid *param = (struct name_tagid *)data;

	if (info->name && param->name &&
	    (strcmp(info->name, param->name) == 0)) {
		*(param->tagid) = (int)id;
		return 1;
	}

	return 0;
}

static int name_to_tagid(const char *name)
{
	int tagid = -1;
	struct name_tagid data = {0};

	if (name == NULL) {
		return -1;
	}

	data.name = name;
	data.tagid = &tagid;

	(void)idr_for_each(&sysfs_idr, find_name, &data);
	return tagid;
}

static int do_sysfs_create_file_helper(struct sysfs_create_file_info *f_info,
				       sysfs_file_ops *fops, void *ctx,
				       const struct sysfs_svp_info *svp_info)
{
	struct sysfs_ops_cb ops = {0};
	int err;
	size_t svpname_len;
	struct sysfs_file_info *info = NULL;
	char *pathname = NULL;

	if (f_info == NULL) {
		return E_HM_INVAL;
	}

	if (f_info->name == NULL || (strlen(f_info->name) + 1) > SYSFS_PATH_NAME_MAX) {
		hm_debug("invalid name %s\n", f_info->name);
		return E_HM_INVAL;
	}

	if (fops == NULL) {
		return E_HM_INVAL;
	}

	if (svp_info == NULL || svp_info->svpname == NULL) {
		return E_HM_INVAL;
	}

	svpname_len = strnlen(svp_info->svpname, SVP_NAME_MAX);
	if (svpname_len == SVP_NAME_MAX) {
		return E_HM_INVAL;
	}

	/* pathname should be freed when delete it from idr */
	pathname = malloc(SYSFS_PATH_NAME_MAX);
	if (pathname == NULL) {
		return E_HM_NOMEM;
	}

	NOFAIL(memcpy_s(ops.svpname, SVP_NAME_MAX, svp_info->svpname, svpname_len + 1));
	NOFAIL(memcpy_s(pathname, SYSFS_PATH_NAME_MAX, f_info->name, strlen(f_info->name) + 1));

	ops.read_method_id = svp_info->read_method_id;
	ops.write_method_id = svp_info->write_method_id;
	ops.need_rcache = svp_info->need_rcache;
	err = gen_tagid(pathname, fops, ctx, &ops.tagid);
	if (err < 0) {
		hm_debug("gen tagid failed\n");
		free(pathname);
		return E_HM_POSIX_FAULT;
	}
	ops.ctx = 0ull;

	err = do_create_file(pathname, f_info->mode, &ops, f_info->size, KERNFS_COMMON_NSID);
	if (err != E_HM_OK) {
		(void)raw_rwlock_wrlock(&sysfs_idr_rwlock);
		info = idr_remove(&sysfs_idr, (unsigned int)ops.tagid);
		(void)raw_rwlock_unlock(&sysfs_idr_rwlock);
		if (info != NULL) {
			free(info);
		}
		free(pathname);
	}

	return err;
}

int sysfs_create_file_helper(const char *name, mode_t mode, sysfs_file_ops *fops,
			     void *ctx, const struct sysfs_svp_info *svp_info)
{
	/* set sysfs's file default size as PAGE_SIZE */
	struct sysfs_create_file_info f_info = {
		.name = name,
		.mode = mode,
		.size = PAGE_SIZE,
	};

	return do_sysfs_create_file_helper(&f_info, fops, ctx, svp_info);
}

int __sysfs_delete_helper(const char *name, bool free_ctx)
{
	int err;
	int tagid = -1;
	struct sysfs_file_info *info = NULL;

	err = sysfs_delete(name, NULL);
	if (err != E_HM_OK) {
		return err;
	}

	(void)raw_rwlock_wrlock(&sysfs_idr_rwlock);
	tagid = name_to_tagid(name);
	if (tagid >= 0) {
		info = idr_remove(&sysfs_idr, (unsigned int)tagid);
		if (info != NULL) {
			free((char *)(info->name));
			if (free_ctx) {
				free(info->ctx);
			}
			free(info);
		}
	}
	(void)raw_rwlock_unlock(&sysfs_idr_rwlock);

	return E_HM_OK;
}

int sysfs_delete_helper(const char *name)
{
	return __sysfs_delete_helper(name, false);
}

static int acquire_shm(uint64_t shm_key, void **rvaddr, size_t *shm_len)
{
	int err;
	int shm_id;
	uint32_t src_cnode_idx;

	src_cnode_idx = sysif_actv_src_cnode_idx();
	err = hm_ashm_check_key(shm_key, src_cnode_idx);
	if (err < 0) {
		hm_debug("hm_ashm_check_key failed, err=%s\n", hmstrerror(err));
		return err;
	}

	shm_id = hm_ashm_open(shm_key, 0, SHM_O_WRONLY, shm_len);
	if (shm_id < 0) {
		hm_debug("open shmem failed, err=%s\n", hmstrerror(shm_id));
		return shm_id;
	}

	*rvaddr = hm_mem_mmap_posix(NULL, *shm_len, MPROT_WRITE,
				    MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
				    shm_id, 0, &err);
	if (err < 0) {
		hm_debug("mmap shmem failed, err=%s\n", hmstrerror(err));
		(void)hm_ashm_close(shm_id);
		return err;
	}

	return shm_id;
}

static int __read_cache_dispath(const actvcall_sysfs_read_args *read_args,
				struct sysfs_file_info *info)
{
	int ret;
	int shm_id;
	size_t shm_len;
	void *vaddr = NULL;
	size_t rsize = 0;

	shm_id = acquire_shm(read_args->vaddr_key, &vaddr, &shm_len);
	if (shm_id < 0) {
		hm_debug("acquire_shm failed, %s\n", hmstrerror(shm_id));
		return shm_id;
	}

	ret = info->ops->read(info->ctx, vaddr, 0, shm_len, &rsize);
	if (ret < 0) {
		hm_error("fops.read failed, %s\n", hmstrerror(ret));
	}

	(void)hm_mem_munmap(vaddr, shm_len);
	(void)hm_ashm_close(shm_id);

	return (ret < 0) ? ret : (int)rsize;
}

static int __read_direct_dispath(const actvcall_sysfs_read_args *read_args,
				 struct sysfs_file_info *info)
{
	void *vaddr = ulong_to_ptr(read_args->vaddr_key, void);
	size_t rsize = 0;
	int ret;

	ret = info->ops->read(info->ctx, vaddr, read_args->pos, read_args->buf_size, &rsize);
	if (ret < 0) {
		hm_error("fops.read failed, %s\n", hmstrerror(ret));
	}

	return (ret < 0) ? ret : (int)rsize;
}

/* This is a helper func for read handler.
 * If tagid is addr of ops, read handler can call this dispatch func to do all the work.
 */
int sysfs_read_dispatch(const actvcall_sysfs_read_args *read_args)
{
	struct sysfs_file_info *info = NULL;

	if (read_args == NULL) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_rdlock(&sysfs_idr_rwlock);
	info = (struct sysfs_file_info *)idr_find(&sysfs_idr, (unsigned int)read_args->tagid);
	(void)raw_rwlock_unlock(&sysfs_idr_rwlock);
	if (info == NULL) {
		return E_HM_NOSYS;
	}

	if (info->ops == NULL || info->ops->read == NULL) {
		hm_debug("read not supported\n");
		return E_HM_NOSYS;
	}

	if (read_args->need_rcache == 0) {
		return __read_direct_dispath(read_args, info);
	} else {
		return __read_cache_dispath(read_args, info);
	}
}

/* This is a helper func for write handler.
 * If tagid is addr of ops, write handler can call this dispatch func to
 * do all the work.
 */
int sysfs_write_dispatch(const actvcall_sysfs_write_args *write_args,  size_t len)
{
	int err;
	char *param = NULL;
	size_t wsize;
	size_t buf_len;
	struct sysfs_file_info *info = NULL;

	if (write_args == NULL) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_rdlock(&sysfs_idr_rwlock);
	info = (struct sysfs_file_info *)idr_find(
		&sysfs_idr, (unsigned int)write_args->tagid);
	(void)raw_rwlock_unlock(&sysfs_idr_rwlock);
	if (info == NULL) {
		return E_HM_NOSYS;
	}

	param = (char *)write_args + sizeof(actvcall_sysfs_write_args);
	buf_len = len - sizeof(actvcall_sysfs_write_args);
	if (buf_len != (size_t)write_args->size) {
		hm_debug("invalid buffer len\n");
		return E_HM_INVAL;
	}

	if (info->ops != NULL && info->ops->write != NULL) {
		err = info->ops->write(info->ctx, param, (size_t)write_args->pos,
				       (size_t)write_args->size, &wsize);
		if (err != E_HM_OK) {
			hm_debug("fops.write failed, err=%s\n", hmstrerror(err));
		}
	} else {
		hm_debug("write not supported\n");
		err = E_HM_NOSYS;
	}

	return (err < 0) ? err : (int)wsize;
}

/* create sysfs file with target mode and size by the interface */
int sysfs_create_file_attr(struct sysfs_create_file_info *f_info,
			   const struct sysfs_ops_cb *ops, unsigned int nsid)
{
	if (f_info == NULL) {
		return E_HM_INVAL;
	}

	return do_create_file(f_info->name, f_info->mode, ops, f_info->size, nsid);
}

/* create sysfs file helper with target mode and size by the interface */
int sysfs_create_file_attr_helper(struct sysfs_create_file_info *f_info,
				  sysfs_file_ops *fops, void *ctx,
				  const struct sysfs_svp_info *svp_info)
{
	return do_sysfs_create_file_helper(f_info, fops, ctx, svp_info);
}
