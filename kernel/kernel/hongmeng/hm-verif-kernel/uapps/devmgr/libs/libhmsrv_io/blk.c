/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service library of block device operation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmucap/ucap.h>
#include <libsysif/devhost/api.h>
#include <libhwsecurec/securec.h>

#include <libhmsrv_io/blk.h>
#include <libdevhost/fusion/blk.h>

int hmsrv_io_blk_check_feature(const struct hmsrv_io_ctx *ctx,
			       int idx, uint64_t feature, uint64_t *feature_out)
{
	struct __actvret_drvcall_devhost_block_check_feature pret;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(pret);
	ret = actvcapcall_drvcall_devhost_block_check_feature(ctx->dh_ap_rref,
			idx, ctx->filp, feature, &pret);
	if (ret < 0) {
		return ret;
	}

	if (feature_out != NULL) {
		*feature_out = pret.supported_features;
	}

	return E_HM_OK;
}

int hmsrv_io_blk_register_bio_notify(const struct hmsrv_io_ctx *ctx,
				     cref_t ap_cref,
				     unsigned int method,
				     size_t bd_shm_size,
				     uint64_t *submit_fd, uint64_t *end_fd,
				     unsigned long *shm_vaddr,
				     size_t *obj_cnt)
{
	struct __actvret_drvcall_devhost_block_register_bio_notify pret;
	struct __actvpool_grant grant;
	uref_t uref;
	int ret;

	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;

	if ((ctx == NULL) || (submit_fd == NULL) || (end_fd == NULL) || (shm_vaddr == NULL)
	    || (obj_cnt == NULL)) {
		return E_HM_INVAL;
	}

	uref = hm_ucap_export(0, ctx->dh_cnode_idx,
			      /* flags= */
			      CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
			      CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX |
			      CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF,
			      ap_cref,
			      /* rights= */
			      1, /* CRIGHT_ACTVPOOL_CALL */
			      /* args= */
			      ptr_to_ulong(&grant));
	if (IS_REF_ERR(uref)) {
		return REF_TO_ERR(uref);
	}

	mem_zero_s(pret);
	ret = actvxactcapcall_drvcall_devhost_block_register_bio_notify(false,
			true, ctx->dh_ap_rref, (unsigned long)ctx->filp, uref,
			method, bd_shm_size, &pret);
	if (ret < 0) {
		(void)hm_ucap_revoke(0, uref);
		return ret;
	}

	*submit_fd = pret.submit_fd;
	*end_fd = pret.end_fd;
	*shm_vaddr = pret.shm_vaddr;
	*obj_cnt = (size_t)pret.obj_cnt;

	return E_HM_OK;
}

int hmsrv_io_blk_run_queue(const struct hmsrv_io_ctx *ctx, unsigned int flags,
			   unsigned int idx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvcapcall_drvcall_devhost_block_run_queue(ctx->dh_ap_rref,
			(unsigned long)ctx->filp, flags, idx);
}

void hmsrv_io_blk_polling(const struct hmsrv_io_ctx *ctx, int rw)
{
	if (ctx == NULL) {
		return;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		libdh_blk_bio_polling(ctx, rw);
	}
}

int hmsrv_io_blk_submit_bio(const struct hmsrv_io_ctx *ctx,
			    unsigned long long rw, unsigned long long sector,
			    struct hmsrv_io_blk_vec *vec, int cnt,
			    unsigned int priority, void *bi_crypt_context,
				void (*dump_fs)(void))
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_blk_submit_bio(ctx, rw, sector, vec, cnt, priority,
					    bi_crypt_context, dump_fs);
	}

	return actvcapcall_drvcall_devhost_block_submit_bio(ctx->dh_ap_rref,
			(unsigned long)ctx->filp, rw, sector, cnt, priority,
			(unsigned long long)(uintptr_t)bi_crypt_context, vec,
			sizeof(*vec) * (size_t)(unsigned int)cnt);
}

int hmsrv_io_blk_submit_cmd(const struct hmsrv_io_ctx *ctx,
			    unsigned long long cmd, unsigned long long sector,
			    unsigned int size, unsigned int priority)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->dh_cnode_idx == hm_ucap_self_cnode_idx()) {
		return libdh_blk_submit_cmd(ctx, cmd, sector, size, priority);
	}

	return actvcapcall_drvcall_devhost_block_submit_cmd(ctx->dh_ap_rref,
			(unsigned long)ctx->filp, cmd, sector, size, priority);
}
