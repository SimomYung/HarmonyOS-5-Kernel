/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for block device operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#include <libsysif/devhost/server.h>

#include <hmasm/page.h>
#include <sys/mman.h>
#include <librb/shm_ring_buffer.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/api.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>

#include "types.h"
#include "devhost_api.h"
#include "devhost_bio.h"
#include <devhost/log.h>
#include <devhost/file.h>

/* set max bd_shm_size to 1MB */
#define MAX_BD_SHM_SIZE		(256 * 4096)

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_check_feature,
		    unsigned long long, sender, unsigned long, credential,
		    int, idx,
		    unsigned long long, file_id,
		    unsigned long long, features)
{
	struct __actvret_drvcall_devhost_block_check_feature *__ret =
		actvhdlr_drvhandler_devhost_block_check_feature_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	struct filp_node *node = NULL;
	struct devhost_block_ops *blkops = NULL;
	int ret = E_HM_OK;

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  0, (unsigned int)file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	blkops = node->devinfo->p->blkops;
	if ((blkops == NULL) || (blkops->dev_check_feature == NULL)) {
		return E_HM_NOSYS;
	}

	ret = blkops->dev_check_feature(node->filp, idx, features,
					ptr_to_type(&__ret->supported_features, uint64_t *));
	if (ret < 0) {
		dh_error("check device supported features failed\n");
		return posix2hmerrno(-ret);
	}
	return E_HM_OK;
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_run_queue,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned long, file_id,
		    unsigned int, flags, unsigned int, idx)
{
	struct filp_node *node = NULL;
	struct devhost_block_ops *blkops = NULL;
	__u32 cnode_idx;
	int ret = E_HM_OK;

	UNUSED(credential);

	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	acquire_devhost_filp_node(node, (uint32_t)cnode_idx, 0, (unsigned int)file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	if ((node->filp == NULL) || (node->blk_ctx == NULL)) {
		return E_HM_NOSYS;
	}
	blkops = node->devinfo->p->blkops;
	if ((blkops == NULL) ||
	    (blkops->bio_alloc == NULL) || (blkops->bio_add_data == NULL) ||
	    (blkops->submit_bio == NULL) || (blkops->bio_free == NULL)) {
		return E_HM_NOSYS;
	}

	if ((flags & REQ_SIO) != 0) {
		ret = devhost_do_sync_submit_bio(node, idx);
	} else {
		ret = devhost_do_async_submit_bio(node);
	}

	return ret;
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_direct_access,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned long, file_id,
		    unsigned long, offset, unsigned long, size)
{
	struct __actvret_drvcall_devhost_block_direct_access *__ret =
		actvhdlr_drvhandler_devhost_block_direct_access_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	struct filp_node *node = NULL;
	struct devhost_block_ops *blkops = NULL;
	int ret = E_HM_OK;
	unsigned long long ret_paddr;
	unsigned long ret_size;

	acquire_devhost_filp_node(node, __RPC_SENDER_DECODE_SRC_CIDX(sender),
				  0, (unsigned int)file_id);
	if (node == NULL) {
		dh_error("file_id is invalid\n");
		return E_HM_INVAL;
	}
	if ((node->filp == NULL) || (node->blk_ctx == NULL)) {
		return E_HM_NOSYS;
	}
	blkops = node->devinfo->p->blkops;
	if ((blkops == NULL) || (blkops->direct_access == NULL)) {
		return E_HM_NOSYS;
	}

	ret = blkops->direct_access(node->filp, offset, size,
				    &ret_paddr, &ret_size);
	if (ret < 0) {
		dh_error("direct access failed\n");
		return posix2hmerrno(-ret);
	}

	__ret->paddr = (unsigned long long)ret_paddr;
	__ret->ret_size = (unsigned long long)ret_size;
	return E_HM_OK;
}

static int devhost_block_prepare_bio(struct filp_node *node,
	struct bd_vfs_queue **bd_vfs_queue,
	size_t bd_shm_size,
	struct __actvret_drvcall_devhost_block_register_bio_notify *__ret)
{
	struct bd_grant_ret_s ret;
	int err = E_HM_OK;

	mem_zero_s(ret);
	if (bd_shm_size > MAX_BD_SHM_SIZE) {
		dh_warn("invalid bd_shm_size provided, bd_shm_size=%zu\n",
			bd_shm_size);
		return E_HM_INVAL;
	}

	err = devhost_init_bio(node, bd_vfs_queue, bd_shm_size);
	if (err < 0) {
		dh_warn("init bio subsystem failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = devhost_grant_bio(*bd_vfs_queue, &ret);
	if (err < 0) {
		dh_warn("grant bio subsystem failed, err=%s\n", hmstrerror(err));
		devhost_destroy_bio(node, false);
		return err;
	}

	__ret->submit_fd = ret.submit_fd;
	__ret->end_fd = ret.end_fd;
	__ret->shm_vaddr = (*bd_vfs_queue)->shm_info.vma_info->start;
	__ret->obj_cnt = (unsigned long long)(*bd_vfs_queue)->obj_cnt;

	return err;
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_submit_bio,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned long long, file_id, unsigned long long, rw,
		    unsigned long long, sector, int, vec_cnt,
		    unsigned int, priority, unsigned long long, bi_crypt_context)
{
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	__u32 cnode_idx;
	struct hmsrv_io_blk_vec *vec = NULL;
	struct filp_node *node = NULL;

	if (recv.size == 0 || recv.data == NULL ||
	    (size_t)recv.size != (unsigned int)vec_cnt * sizeof(struct hmsrv_io_blk_vec) ||
	    (unsigned int)vec_cnt * sizeof(struct hmsrv_io_blk_vec) < (unsigned int)vec_cnt) {
		return E_HM_INVAL;
	}
	vec = (struct hmsrv_io_blk_vec *)recv.data;

	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	acquire_devhost_filp_node(node, (uint32_t)cnode_idx,
				  0, (unsigned int)file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	return devhost_submit_bio(node, rw, sector, vec, vec_cnt, priority,
				  ulong_to_ptr(bi_crypt_context, void));
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_submit_cmd,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned long long, file_id, unsigned long long, cmd,
		    unsigned long long, sector, unsigned int, size,
		    unsigned int, priority)
{
	__u32 cnode_idx;
	struct filp_node *node = NULL;

	UNUSED(credential);
	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	acquire_devhost_filp_node(node, (uint32_t)cnode_idx,
				  0, (unsigned int)file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	return devhost_submit_cmd(node, cmd, sector, size, priority);
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_block_register_bio_notify,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned long, file_id, uref_t, uref,
		    unsigned int, method, size_t, bd_shm_size)
{
	struct filp_node *node = NULL;
	struct block_ctx *ctx = NULL;
	int err = E_HM_OK;
	rref_t rref;
	__u32 cnode_idx;
	struct __actvret_drvcall_devhost_block_register_bio_notify *__ret =
		actvhdlr_drvhandler_devhost_block_register_bio_notify_prepare_ret(sender, credential);
	BUG_ON_D(__ret == NULL);
	struct device_info *devinfo = NULL;
	struct bd_vfs_queue *bd_vfs_queue = NULL;

	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	acquire_devhost_filp_node(node, (uint32_t)cnode_idx,
				  0, (unsigned int)file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}
	if (cnode_idx != __REF_CNODE_IDX_OF(uref)) {
		dh_error("uref is invalid\n");
		return E_HM_PERM;
	}

	devinfo = node->devinfo;
	if (devinfo->p->blkops == NULL) {
		return E_HM_NOSYS;
	}

	RAW_MUTEX_GUARD(_, &node->ctx_lock);

	if (node->blk_ctx != NULL) {
		return E_HM_BUSY;
	}

	ctx = malloc(sizeof(struct block_ctx));
	if (ctx == NULL) {
		return E_HM_NOMEM;
	}

	/* bio subsystem init */
	err = devhost_block_prepare_bio(node, &bd_vfs_queue, bd_shm_size, __ret);
	if (err < 0) {
		dh_warn("init bio subsystem failed, err=%s\n", hmstrerror(err));
		free(ctx);
		return err;
	}

	rref = hm_ucap_accept_grant(0, uref, 0);
	if (IS_REF_ERR(rref)) {
		devhost_destroy_bio(node, false);
		dh_error("hm_ucap_accept_grant failed, err=%s\n",
			 hmstrerror(err));
		free(ctx);
		return REF_TO_ERR(rref);
	}

	ctx->rref_notify = rref;
	ctx->service_id = node->devinfo->p->devt;
	ctx->end_srbuf = (struct sring_buf *)(bd_vfs_queue->bd_queue.end_queue);
	ctx->method = method;
	node->blk_ctx = ctx;

	return E_HM_OK;
}
