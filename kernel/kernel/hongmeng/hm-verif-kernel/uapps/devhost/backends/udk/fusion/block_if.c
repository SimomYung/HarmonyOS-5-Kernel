/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK block sysif
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 10 10:03:27 2023
 */

#include <hmasm/page.h>
#include <hongmeng/compiler.h>
#include <libdevhost/fusion/blk.h>
#include <libstrict/strict.h>
#include <libsysif/devhost/server.h>
#include <libsysif/fs/api.h>
#include <libhmsrv_io/blk.h>

#include <devhost/file.h>
#include <udk/block/udk_block.h>
#include <udk/log.h>
#include <udk/mm.h>

#include "device.h"
#include "file.h"
#include "pm.h"
#include "fops_core.h"

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_check_feature,
		    unsigned long long, sender, unsigned long, credential,
		    int, idx,
		    unsigned long long, file_id,
		    unsigned long long, features)
{
	struct __actvret_drvcall_devhost_block_check_feature *__ret =
		actvhdlr_drvhandler_devhost_block_check_feature_prepare_ret(sender, credential);
	struct filp_node *node = u64_to_ptr(file_id, struct filp_node);
	struct devhost_block_ops *blkops = NULL;
	int ret = E_HM_OK;

	BUG_ON(__ret == NULL);
	if(!udk_fusion_is_selfcall()) {
		return E_HM_PERM;
	}

	blkops = node->devinfo->p->blkops;
	if ((blkops == NULL) || (blkops->dev_check_feature == NULL)) {
		return E_HM_NOSYS;
	}

	ret = blkops->dev_check_feature(node->filp, idx, features,
					ptr_to_type(&__ret->supported_features, uint64_t *));
	if (ret < 0) {
		udk_error("check device supported features failed\n");
		return posix2hmerrno(-ret);
	}
	return E_HM_OK;
}

/**
 * in fusion mode, pvec reuse vec from fs, so we need to
 * make sure struct udk_bio_vec is consistent with struct hmsrv_io_blk_vec.
 */
static void __assert_bio_pvec_consistent(void)
{
	raw_static_assert(sizeof(struct udk_bio_pvec) == sizeof(struct hmsrv_io_blk_vec),
			  udk_bio_pvec_size_is_wrong);
	struct udk_bio_pvec pvec;
	struct hmsrv_io_blk_vec vec = {
		.pfn = ptr_to_type(&pvec, unsigned long long),
		.len = PAGE_SIZE,
		.offset = 1U,
	};
	NOFAIL(memcpy_s(&pvec, sizeof(struct udk_bio_pvec), &vec, sizeof(struct hmsrv_io_blk_vec)));
	BUG_ON(pvec.pfn != vec.pfn);
	BUG_ON(pvec.len != vec.len);
	BUG_ON(pvec.offset != vec.offset);
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_register_bio_notify,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned long, file_id, uref_t, uref,
		    unsigned int, method, size_t, bd_shm_size)
{
	UNUSED(sender, credential, bd_shm_size);
	struct filp_node *node = u64_to_ptr(file_id, struct filp_node);
	struct block_ctx *ctx = NULL;
	rref_t rref;

	if (node->blk_ctx != NULL) {
		return E_HM_BUSY;
	}

	ctx = (struct block_ctx *)mcache_block_ctx_malloc();
	if (ctx == NULL) {
		return E_HM_NOMEM;
	}

	rref = hm_ucap_accept_grant(0, uref, 0);
	if (IS_REF_ERR(rref)) {
		free(ctx);
		return REF_TO_ERR(rref);
	}

	mem_zero_s(*ctx);
	ctx->rref_notify = rref;
	ctx->service_id = node->devinfo->p->devt;
	ctx->method = method;
	node->blk_ctx = ctx;

	__assert_bio_pvec_consistent();

	return E_HM_OK;
}

static int __block_end_io(void *private, int bi_error, unsigned int rq_idx)
{
	int ret = E_HM_OK;
	struct block_ctx *ctx = (struct block_ctx *)private;

	if (ctx == NULL) {
		return -EINVAL;
	}
	ret = actvcapcall_fscall_endio_io_done(ctx->rref_notify,
					       ctx->service_id,
					       rq_idx, bi_error);
	return -hmerrno2posix(ret);
}

static int init_bi_crypt_ctx(void *bi_crypt_context, struct bd_io *bd_io_ptr)
{
	int ret = 0;
#if defined(CONFIG_HISI_INLINE_ENCRYPTION) || defined(CONFIG_UDK_FALLBACK_ENCRYPTION)
	if (bi_crypt_context != NULL) {
		struct bd_io_fs_hisi_crypt_ctx *fs_crypt_ctx = (struct bd_io_fs_hisi_crypt_ctx *)bi_crypt_context;
		struct bd_io_hisi_crypt_ctx *bd_io_crypt_ctx = &bd_io_ptr->bi_hisi_crypt_context;

		if (fs_crypt_ctx->ci_key != NULL) {
			// save key address
			ret = memcpy_s(bd_io_crypt_ctx->ci_key, sizeof(fs_crypt_ctx->ci_key),
					(uint8_t *)&fs_crypt_ctx->ci_key, sizeof(fs_crypt_ctx->ci_key));
			if (ret != E_HM_OK) {
				udk_warn("udk copy ci_key address failed\n");
				return ret;
			}
		}

		if (fs_crypt_ctx->ci_metadata != NULL) {
			// save metadata address
			ret = memcpy_s(bd_io_crypt_ctx->ci_metadata, sizeof(fs_crypt_ctx->ci_metadata),
					(uint8_t *)&fs_crypt_ctx->ci_metadata, sizeof(fs_crypt_ctx->ci_metadata));
			if (ret != E_HM_OK) {
				udk_warn("udk copy ci_metadata address failed\n");
				return ret;
			}
		}

		bd_io_crypt_ctx->ci_key_len = fs_crypt_ctx->ci_key_len;
		bd_io_crypt_ctx->ci_key_index = fs_crypt_ctx->ci_key_index;
		bd_io_crypt_ctx->index = fs_crypt_ctx->index;
	}
#else
	UNUSED(bi_crypt_context, bd_io_ptr);
#endif
	return ret;
}

static int __submit_bio(struct filp_node *node, unsigned long long io_rw,
			unsigned long long sector, struct hmsrv_io_blk_vec *vec,
			int vec_cnt, unsigned int priority,
			void *bi_crypt_context, void (*dump_fs)(void))
{
	struct device_info *devinfo = NULL;
	struct devhost_block_ops *blkops = NULL;
	unsigned int rq_idx = (unsigned int)hmsrv_io_get_rq_idx(io_rw);
	struct udk_bio *bio = NULL;
	struct bd_io bd_io;
	struct bd_context bd_ctx = { 0 };
	int ret;

	if ((node == NULL) || (node->devinfo == NULL) ||
	    (node->devinfo->p->blkops == NULL)) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	blkops = devinfo->p->blkops;

	mem_zero_s(bd_io);
	bd_io.rw = (int)hmsrv_io_get_op(io_rw);
	bd_io.io_flags = (int)hmsrv_io_get_io_flags(io_rw);
	bd_io.block_offset = sector;
	bd_io.dump_fs = dump_fs;
	ret = init_bi_crypt_ctx(bi_crypt_context, &bd_io);
	if (ret != E_HM_OK) {
		return ret;
	}

	bd_ctx.vec_cnt = vec_cnt;
	bd_ctx.direct_map_paddr = 1;
	bd_ctx.private = node->blk_ctx;
	bd_ctx.io_priority = priority;
	bio = (struct udk_bio *)blkops->bio_alloc(node->filp, &bd_io, &bd_ctx, rq_idx);
	if (bio == NULL) {
		return E_HM_NOMEM;
	}
	if (blkops->bio_set_ctx != NULL) {
		blkops->bio_set_ctx(bio, __block_end_io);
	}

	if (vec_cnt != 0) {
		bio->bio_io_pvec = (struct udk_bio_pvec *)vec;
		bio->bi_vec_num = vec_cnt;
		for (int i = 0; i < vec_cnt; ++i) {
			bio->bi_size += vec[i].len;
		}
	}

	udk_fusion_pm_rdlock();
	ret = blkops->submit_bio(bio, 0);
	if (ret < 0) {
		blkops->bio_free(bio);
		ret = posix2hmerrno(-ret);
	}
	udk_fusion_pm_unlock();

	return ret;
}

static int __submit_cmd(struct filp_node *node, unsigned long long io_cmd,
			unsigned long long sector, unsigned int size,
			unsigned int priority)
{
	struct device_info *devinfo = NULL;
	struct devhost_block_ops *blkops = NULL;
	unsigned int rq_idx = (unsigned int)hmsrv_io_get_rq_idx(io_cmd);
	void *bio = NULL;
	struct bd_io bd_io;
	struct bd_context bd_ctx = { 0 };
	int ret;

	if ((node == NULL) || (node->devinfo == NULL) ||
	    (node->devinfo->p->blkops == NULL)) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	blkops = devinfo->p->blkops;

	mem_zero_s(bd_io);
	bd_io.rw = (int)hmsrv_io_get_op(io_cmd);
	bd_io.io_flags = (int)hmsrv_io_get_io_flags(io_cmd);
	bd_io.block_offset = sector;
	bd_io.bv_len = size;
	bd_ctx.direct_map_paddr = 1;
	bd_ctx.private = node->blk_ctx;
	bd_ctx.io_priority = priority;
	bio = blkops->bio_alloc(node->filp, &bd_io, &bd_ctx, rq_idx);
	if (bio == NULL) {
		return E_HM_NOMEM;
	}
	if (blkops->bio_set_ctx != NULL) {
		blkops->bio_set_ctx(bio, __block_end_io);
	}

	udk_fusion_pm_rdlock();
	ret = blkops->submit_bio(bio, 0);
	if (ret < 0) {
		blkops->bio_free(bio);
		ret = posix2hmerrno(-ret);
	}
	udk_fusion_pm_unlock();

	return ret;
}

void libdh_blk_bio_polling(const struct hmsrv_io_ctx *ctx, int rw)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	struct devhost_block_ops *blkops = NULL;

	if (ctx == NULL) {
		udk_error("invalid ctx\n");
		return;
	}

	node = u64_to_ptr(ctx->filp, struct filp_node);
	devinfo = node->devinfo;
	blkops = devinfo->p->blkops;

	if (blkops->bio_polling) {
		blkops->bio_polling(node->filp, rw);
	}
}

int libdh_blk_submit_bio(const struct hmsrv_io_ctx *ctx, unsigned long long rw,
			 unsigned long long sector,
			 struct hmsrv_io_blk_vec *vec, int cnt,
			 unsigned int priority, void *bi_crypt_context,
			 void (*dump_fs)(void))
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	struct filp_node *node = u64_to_ptr(ctx->filp, struct filp_node);

	return __submit_bio(node, rw, sector, vec, cnt, priority,
			    bi_crypt_context, dump_fs);
}

int libdh_blk_submit_cmd(const struct hmsrv_io_ctx *ctx, unsigned long long cmd,
			 unsigned long long sector, unsigned int size,
			 unsigned int priority)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	struct filp_node *node = u64_to_ptr(ctx->filp, struct filp_node);

	return __submit_cmd(node, cmd, sector, size, priority);
}
