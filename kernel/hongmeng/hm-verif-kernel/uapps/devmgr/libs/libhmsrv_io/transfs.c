/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: ulibs interface of transfs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 1 13:58:38 2021
 */

#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libhwsecurec/securec.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/api.h>
#include <libstrict/strict.h>
#include <libmem/flags.h>
#include <vfs.h>
#include <libhmsrv_sys/hm_select_common.h>
#include "internal.h"

#include <libhmsrv_io/dev.h>
#include <libhmsrv_io/transfs.h>

struct hmsrv_io_transfs_mount_private {
	struct hmsrv_io_transfs_mnt_ctx mnt_ctx;
	/* root inode index */
	unsigned int root_v_index;
	/* root dentry index */
	unsigned int root_d_index;
	/* root node link count */
	unsigned int root_lnkcnt;
	/* device block size */
	size_t block_size;
	/* granularity of ctime/mtime/atime in nsec */
	uint32_t s_time_gran;
	/* max for ctime/mtime/atime in sec */
	int64_t s_max_time;
	/* min for ctime/mtime/atime in sec */
	int64_t s_min_time;
	uint32_t trans_dev_id;
};

#define transfs_mnt_pri(m_ctx) (((m_ctx) != NULL) ? \
	(container_of((m_ctx), struct hmsrv_io_transfs_mount_private, mnt_ctx)) : NULL)

struct hmsrv_io_transfs_mount_xactv_data {
	struct hm_actv_buf fs_name;
	struct hm_actv_buf dev_name;
	struct hm_actv_buf mount_opts;
	struct hmsrv_io_transfs_mount_recv recv;
};

unsigned int hmsrv_transfs_get_sb_index(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->mnt_ctx.sb_index;
}

unsigned int hmsrv_transfs_get_rv_index(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->root_v_index;
}

uint32_t hmsrv_transfs_get_dev_id(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->trans_dev_id;
}

unsigned int hmsrv_transfs_get_rd_index(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->root_d_index;
}

unsigned int hmsrv_transfs_get_r_lnkcnt(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->root_lnkcnt;
}

size_t hmsrv_transfs_get_block_size(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->block_size;
}

uint32_t hmsrv_transfs_get_time_gran(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->s_time_gran;
}

int64_t hmsrv_transfs_get_max_time(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->s_max_time;
}

int64_t hmsrv_transfs_get_min_time(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	return mnt_pri->s_min_time;
}

static void __prepare_transfs_mounted_xactv_data(
			struct hmsrv_io_transfs_mount_xactv_data *data,
			const struct hmsrv_io_transfs_mount_info *mnt_info,
			dev_t dev_id, int dev_mode, const char *fs_name)
{
	struct hm_actv_buf *actv_fs_name = &data->fs_name;
	struct hm_actv_buf *actv_dev_name = &data->dev_name;
	struct hm_actv_buf *actv_mount_opts = &data->mount_opts;
	struct hmsrv_io_transfs_mount_recv *recv = &data->recv;

	hm_actv_buf_init(actv_fs_name, fs_name, strlen(fs_name) + 1);
	hm_actv_buf_init(actv_dev_name, mnt_info->devname,
		(mnt_info->devname == NULL) ? 0 : strlen(mnt_info->devname) + 1U);
	hm_actv_buf_init(actv_mount_opts, mnt_info->mount_opts,
			 (mnt_info->mount_opts == NULL ? 0 :
			  strlen(mnt_info->mount_opts) + 1U));

	recv->dev_id = dev_id;
	recv->dev_mode = dev_mode;
	recv->mount_flags = (int)(unsigned int)mnt_info->mount_flags;
	recv->per_write_size = mnt_info->trans_per_write_size;
	recv->sector_node_bits = mnt_info->sector_node_bits;
}

int hmsrv_transfs_mounted(dev_t dev_id, int dev_mode, const char *fs_name,
			  const struct hmsrv_io_transfs_mount_info *mnt_info,
			  struct hmsrv_io_transfs_mnt_ctx **mnt_ctx)
{
	int ret;
	struct hmsrv_io_transfs_mount_xactv_data xactv_data = {0};
	struct hmsrv_io_transfs_mount_private *mnt_pri = NULL;
	struct __actvret_drvcall_devhost_transfs_mounted pret = {0};

	if (fs_name == NULL || mnt_info == NULL || mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	mnt_pri = malloc(sizeof(struct hmsrv_io_transfs_mount_private));
	if (mnt_pri == NULL) {
		return E_HM_NOMEM;
	}

	ret = hmsrv_io_dev_open((unsigned int)dev_id, dev_mode,
				O_NO_OPEN,
				HMSRV_IO_CTX_FLAGS_ALL_FORWARD &
				(~HMSRV_IO_CTX_FLAGS_OPEN_FORWARD),
				&mnt_pri->mnt_ctx.ctx);
	if (ret != E_HM_OK) {
		hm_warn("failed to open device: %s\n", hmstrerror(ret));
		free(mnt_pri);
		return ret;
	}

	__prepare_transfs_mounted_xactv_data(&xactv_data, mnt_info,
					     dev_id, dev_mode, fs_name);

	ret = actvxactcapcall_drvcall_devhost_transfs_mounted(
			false, true,
			mnt_pri->mnt_ctx.ctx.dh_ap_rref,
			xactv_data.fs_name, xactv_data.dev_name,
			xactv_data.mount_opts,
			&xactv_data.recv, sizeof(xactv_data.recv),
			&pret);
	if (ret == E_HM_OK) {
		mnt_pri->root_lnkcnt = pret.m_info.root_lnkcnt;
		mnt_pri->root_v_index = pret.m_info.root_v_index;
		mnt_pri->root_d_index = pret.m_info.root_d_index;
		mnt_pri->mnt_ctx.sb_index = pret.m_info.sb_index;
		mnt_pri->mnt_ctx.ctx.filp = pret.m_info.filp;
		mnt_pri->block_size = pret.m_info.block_size;
		mnt_pri->s_time_gran = pret.m_info.s_time_gran;
		mnt_pri->s_max_time = pret.m_info.s_max_time;
		mnt_pri->s_min_time = pret.m_info.s_min_time;
		mnt_pri->trans_dev_id = pret.m_info.trans_dev_id;
		*mnt_ctx = &mnt_pri->mnt_ctx;
	} else {
		free(mnt_pri);
	}

	return ret;
}

int hmsrv_transfs_unmounted(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx, int flags)
{
	int ret;
	struct hmsrv_io_transfs_mount_private *mnt_pri;

	if (mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	mnt_pri = transfs_mnt_pri(mnt_ctx);
	BUG_ON(mnt_pri == NULL);

	ret = actvxactcapcall_drvcall_devhost_transfs_unmounted(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, flags, mnt_ctx->ctx.filp);
	if (ret != E_HM_OK) {
		hm_warn("unmount fs failed:%s\n", hmstrerror(ret));
	}

	/* no matter drvcall_devhost_transfs_unmounted is success or not just release rref,
	 * because the errno will be ignored by vfs
	 */
	(void)hm_ucap_reject(0, mnt_ctx->ctx.dh_ap_rref);
	free(mnt_pri);

	return ret;
}

int hmsrv_transfs_drop_caches(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			      unsigned int mode, int max_nr)
{
	int ret;

	if (mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_drop_caches(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, mode, max_nr);

	return ret;
}

int hmsrv_transfs_mkdir(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			mode_t mode, const char *dname, size_t len,
			struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct __actvret_drvcall_devhost_transfs_mkdir pret = {0};

	if (mnt_ctx == NULL || pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_mkdir(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index, mode,
			actv_dname, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
		dn_info_out->v_index = pret.dn_info_out.v_index;
		dn_info_out->ino = pret.dn_info_out.ino;
	}

	return ret;
}

int hmsrv_transfs_rmdir(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *d_ctx)
{
	int ret;

	if (mnt_ctx == NULL || pd_ctx == NULL ||
	    d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_rmdir(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index,
			d_ctx->d_index);

	return ret;
}

int hmsrv_transfs_create(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			 mode_t mode, const char *dname, size_t len,
			 struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct __actvret_drvcall_devhost_transfs_create pret = {0};

	if (mnt_ctx == NULL || pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_create(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index, mode,
			actv_dname, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
		dn_info_out->v_index = pret.dn_info_out.v_index;
		dn_info_out->ino = pret.dn_info_out.ino;
	}

	return ret;
}

int hmsrv_transfs_unlink(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *d_ctx)
{
	int ret;

	if (mnt_ctx == NULL || pd_ctx == NULL ||
	    d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_unlink(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index,
			d_ctx->d_index);

	return ret;
}

int hmsrv_transfs_mknod(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			unsigned int mode, dev_t devno, const char *dname, size_t len,
			struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct __actvret_drvcall_devhost_transfs_mknod pret = {0};

	if (mnt_ctx == NULL || pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_mknod(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index, mode,
			devno, actv_dname, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
		dn_info_out->v_index = pret.dn_info_out.v_index;
		dn_info_out->ino = pret.dn_info_out.ino;
	}

	return ret;
}

int hmsrv_transfs_link(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
		       const struct hmsrv_io_transfs_dentry_ctx *old_d_ctx,
		       const struct hmsrv_io_transfs_dentry_ctx *new_pd_ctx,
		       const char *dname, size_t len,
		       struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct __actvret_drvcall_devhost_transfs_link pret = {0};

	if (mnt_ctx == NULL || old_d_ctx == NULL || new_pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_link(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index,
			old_d_ctx->d_index, new_pd_ctx->d_index, actv_dname, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
	}

	return ret;
}

int hmsrv_transfs_symlink(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			  const char *dname, size_t dlen,
			  const char *path, unsigned int len,
			  struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct hm_actv_buf actv_path;
	struct __actvret_drvcall_devhost_transfs_symlink pret = {0};

	if (mnt_ctx == NULL || pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_dname, dname, dlen);
	hm_actv_buf_init(&actv_path, path, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_symlink(
			true, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index,
			actv_dname, actv_path, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
		dn_info_out->v_index = pret.dn_info_out.v_index;
		dn_info_out->ino = pret.dn_info_out.ino;
	}

	return ret;
}

int hmsrv_transfs_statfs(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			 struct statfs *buf)
{
	int ret;

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_statfs(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, d_ctx->d_index, buf);

	return ret;
}

int hmsrv_transfs_rename(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *old_pd_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *old_d_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *new_pd_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *new_d_ctx,
			 const char *dname, size_t len)
{
	int ret;
	struct hm_actv_buf actv_dname;
	unsigned int new_d_idx;

	if (mnt_ctx == NULL || old_pd_ctx == NULL || old_d_ctx == NULL || new_pd_ctx == NULL) {
		return E_HM_INVAL;
	}

	new_d_idx = (new_d_ctx != NULL) ? new_d_ctx->d_index : UINT_MAX;

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_rename(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, old_pd_ctx->d_index,
			old_d_ctx->d_index, new_pd_ctx->d_index, new_d_idx, actv_dname);

	return ret;
}

int hmsrv_transfs_lookup(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			 const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			 const char *dname, size_t len, void *meta,
			 struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct __actvret_drvcall_devhost_transfs_lookup pret = {0};

	if (mnt_ctx == NULL || pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	BUG_ON(dname == NULL || len == 0, "len is %zu, expected non-zero\n", len);

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_lookup(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index,
			actv_dname, meta, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
		dn_info_out->v_index = pret.dn_info_out.v_index;
	}

	return ret;
}

int hmsrv_transfs_setattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			  const void *attr, uint64_t size)
{
	int ret;

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_setattr(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, d_ctx->d_index, attr, size);

	return ret;
}

ssize_t hmsrv_transfs_readlink(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       char *buf, size_t bufsize)
{
	int ret;

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_readlink(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, d_ctx->d_index, buf, bufsize);

	return (ssize_t)ret;
}

int hmsrv_transfs_getattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			  void *meta)
{
	int ret;

	if (mnt_ctx == NULL || d_ctx == NULL || meta == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_getattr(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, d_ctx->d_index,
			meta);

	return ret;
}

ssize_t hmsrv_transfs_listxattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
				const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
				char *buffer, size_t size)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_listxattr pret = {0};

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_listxattr(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, d_ctx->d_index, buffer,
			size, &pret);
	if (ret < 0) {
		return ret;
	}

	return (ssize_t)pret.written;
}

ssize_t hmsrv_transfs_getxattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       int prefix, const char *name, char * value,
			       size_t size)
{
	int ret;
	struct hm_actv_buf actv_name;
	struct __actvret_drvcall_devhost_transfs_getxattr pret = {0};

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_name, name, strlen(name));
	ret = actvxactcapcall_drvcall_devhost_transfs_getxattr(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index,
			d_ctx->d_index, prefix, actv_name,
			value, size, &pret);
	if (ret < 0) {
		return ret;
	}

	return (ssize_t)pret.nbytes;
}

ssize_t hmsrv_transfs_setxattr(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
			       int prefix, const char *name, const char * value,
			       size_t size, int flags)
{
	int ret;
	struct hm_actv_buf actv_name;

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_name, name, strlen(name));
	ret = actvxactcapcall_drvcall_devhost_transfs_setxattr(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index,
			d_ctx->d_index, prefix, actv_name,
			value, size, flags);

	return ret;
}

int hmsrv_transfs_tmpfile(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			  const struct hmsrv_io_transfs_dentry_ctx *pd_ctx,
			  mode_t mode, const char *dname, size_t len,
			  struct transfs_dn_info_t *dn_info_out)
{
	int ret;
	struct hm_actv_buf actv_dname;
	struct __actvret_drvcall_devhost_transfs_tmpfile pret = {0};

	if (mnt_ctx == NULL || pd_ctx == NULL || dn_info_out == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&actv_dname, dname, len);
	ret = actvxactcapcall_drvcall_devhost_transfs_tmpfile(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, pd_ctx->d_index, mode,
			actv_dname, &pret);
	if (ret == E_HM_OK) {
		dn_info_out->d_index = pret.dn_info_out.d_index;
		dn_info_out->v_index = pret.dn_info_out.v_index;
		dn_info_out->ino = pret.dn_info_out.ino;
	}

	return ret;
}

int hmsrv_transfs_kill_dentry(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			      const struct hmsrv_io_transfs_dentry_ctx *d_ctx)
{
	int ret;

	if (mnt_ctx == NULL || d_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_kill_dentry(
			false, true, mnt_ctx->ctx.dh_ap_rref,
			mnt_ctx->sb_index, d_ctx->d_index);

	return ret;
}

int hmsrv_transfs_node_put(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			   const struct hmsrv_io_transfs_node_ctx *v_ctx)
{
	int ret;

	if (mnt_ctx == NULL || v_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_node_put(
			false, true, mnt_ctx->ctx.dh_ap_rref,
			mnt_ctx->sb_index, v_ctx->v_index);

	return ret;
}

int hmsrv_transfs_sync(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx)
{
	int ret;

	if (mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_sync(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index);

	return ret;
}

ssize_t hmsrv_transfs_write(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			    uint64_t pos, const void *src, size_t len)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_write pret = {0};

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_write(
			true, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, pos, src, len, &pret);
	if (ret < 0) {
		return ret;
	}

	return (ssize_t)pret.nwritten;
}

ssize_t hmsrv_transfs_read(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			   uint64_t pos, const void *src, size_t len)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_read pret = {0};

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_read(
			true, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, pos, src, len, &pret);
	if (ret < 0) {
		return ret;
	}

	return (ssize_t)pret.nread;
}

ssize_t hmsrv_transfs_write_iter(const struct hmsrv_io_transfs_file_ctx *f_ctx,
				 uint64_t pos, const void *iov, int nr_segs, size_t count)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_write_iter pret = {0};

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_write_iter(
			true, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, pos, iov, nr_segs, count, &pret);
	if (ret < 0) {
		return ret;
	}

	return (ssize_t)pret.nwritten;
}

int hmsrv_transfs_fsync(const struct hmsrv_io_transfs_file_ctx *f_ctx)
{
	int ret;

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_fsync(
			false, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index, f_ctx->f_index);

	return ret;
}

int hmsrv_transfs_readdir(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			  uint64_t *pos, void *buf, size_t size)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_readdir pret = {0};

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_readdir(
			false, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, *pos, buf, size, &pret);
	if (ret >= 0) {
		*pos = (uint64_t)pret.fpos;
	}

	return ret;
}

int hmsrv_transfs_open(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
		       const struct hmsrv_io_transfs_dentry_ctx *d_ctx,
		       unsigned int f_modes, unsigned int f_flags,
		       struct hmsrv_io_transfs_file_ctx *f_ctx)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_open pret = {0};

	if (mnt_ctx == NULL || d_ctx == NULL || f_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_open(
			false, true,
			mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, d_ctx->d_index, f_modes,
			f_flags, &pret);
	if (ret == E_HM_OK) {
		f_ctx->f_index = pret.f_index;
		f_ctx->mnt_ctx = mnt_ctx;
	}

	return ret;
}

int hmsrv_transfs_fallocate(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			    unsigned int mode, uint64_t pos, uint64_t len)
{
	int ret;

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_fallocate(
			false, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, mode, pos, len);

	return ret;
}

int hmsrv_transfs_close(const struct hmsrv_io_transfs_file_ctx *f_ctx)
{
	int ret;

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_close(
			false, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index);

	return ret;
}

int hmsrv_transfs_ioctl(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			unsigned int cmd, unsigned long arg)
{
	int ret;

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_ioctl(
			(f_ctx->mnt_ctx->ctx.flags & HMSRV_IO_CTX_FLAGS_IOCTL_FORWARD) != 0,
			true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, cmd, arg);

	return ret;
}

ssize_t hmsrv_transfs_direct_IO(const struct hmsrv_io_transfs_file_ctx *f_ctx,
				uint64_t offset, void *buf, size_t len,
				unsigned int type)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_direct_IO pret = {0};

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	ret = actvxactcapcall_drvcall_devhost_transfs_direct_IO(
			false, true,
			f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
			f_ctx->f_index, offset, buf, len, type, &pret);
	if (ret < 0) {
		return ret;
	}

	return (ssize_t)pret.nbytes;
}

int hmsrv_transfs_update_opts(const struct hmsrv_io_transfs_mnt_ctx *mnt_ctx,
			      char *buf, size_t buflen)
{
	if (mnt_ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_transfs_update_opts(
		false, true,
		mnt_ctx->ctx.dh_ap_rref, mnt_ctx->sb_index, buf, buflen);
}

int hmsrv_transfs_iommap(const struct hmsrv_io_transfs_file_ctx *f_ctx,
			 const struct vfs_iommap_info *info, struct vfs_iommap_ret *iommap_ret)
{
	int ret;
	struct __actvret_drvcall_devhost_transfs_iommap pret = {0};

	if (f_ctx == NULL || f_ctx->mnt_ctx == NULL || info == NULL || iommap_ret == NULL) {
		return E_HM_INVAL;
	}
	ret = actvxactcapcall_drvcall_devhost_transfs_iommap(
		true, true,
		f_ctx->mnt_ctx->ctx.dh_ap_rref, f_ctx->mnt_ctx->sb_index,
		f_ctx->f_index, info->vaddr, info->vs_rref, info->aligned_len,
		info->prot, info->flags, (unsigned long)info->offset, &pret);
	iommap_ret->rvaddr = (uintptr_t)pret.addr;
	iommap_ret->iomem_id = pret.iomem_id;
	iommap_ret->cnode_idx = f_ctx->mnt_ctx->ctx.dh_cnode_idx;
	return ret;
}
