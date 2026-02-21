/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Block device support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 26 15:01:02 2019
 */

#include "devhost_bio.h"

#include <hmasm/page.h>
#include <librb/shm_ring_buffer.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <libsysif/devhost/api.h>
#include <libdevhost/mm.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhwsecurec/securec.h>

#include "types.h"
#include <devhost/file.h>
#include "devhost.h"
#include "devhost_api.h"
#include <devhost/log.h>
#include "utils/utils.h"

#define DEVHOST_BIO_WORKER_NR		1
#define DEVHOST_BIO_WQ_NR		1
#define MAX_BIO_PAGE_NUM		256

static int init_bi_crypt_ctx(void *bi_crypt_context, struct bd_io *bd_io_ptr)
{
	int ret = 0;
#if !defined(CONFIG_BLK_INLINE_ENCRYPTION) && !defined(CONFIG_HISI_INLINE_ENCRYPTION)
	UNUSED(bi_crypt_context, bd_io_ptr);
#elif defined(CONFIG_BLK_INLINE_ENCRYPTION)
	if (bi_crypt_context != NULL) {
		ret = devhost_pal_copy_from_user(&bd_io_ptr->bi_crypt_context, bi_crypt_context,
						 sizeof(struct bd_io_crypt_ctx));
		if (ret != E_HM_OK) {
			return ret;
		}
	}
#elif defined(CONFIG_HISI_INLINE_ENCRYPTION)
	/* assume that the hisi_inline_encryption and the blk_inline_encryption
	   will not be defined at the same time, so reuse the variable bi_crypt_context */
	if (bi_crypt_context != NULL) {
		ret = devhost_pal_copy_from_user(&bd_io_ptr->bi_hisi_crypt_context, bi_crypt_context,
						 sizeof(struct bd_io_hisi_crypt_ctx));
		if (ret != E_HM_OK) {
			return ret;
		}
	}
#endif
	return ret;
}

int devhost_submit_bio(struct filp_node *node, unsigned long long io_rw,
		       unsigned long long sector, struct hmsrv_io_blk_vec *vec,
		       int vec_cnt, unsigned int priority,
		       void *bi_crypt_context)
{
	struct device_info *devinfo = NULL;
	struct devhost_block_ops *blkops = NULL;
	unsigned int rq_idx = (unsigned int)hmsrv_io_get_rq_idx(io_rw);
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
	bd_io.rw = (int)hmsrv_io_get_op(io_rw);
	bd_io.io_flags = (unsigned int)hmsrv_io_get_io_flags(io_rw);
	bd_io.block_offset = sector;
	ret = init_bi_crypt_ctx(bi_crypt_context, &bd_io);
	if (ret != E_HM_OK) {
		return ret;
	}
	bd_ctx.vec_cnt = (unsigned int)vec_cnt;
	bd_ctx.direct_map_paddr = 1;
	bd_ctx.private = node->blk_ctx;
	bd_ctx.io_priority = priority;
	bio = blkops->bio_alloc(node->filp, &bd_io, &bd_ctx, rq_idx);
	if (bio == NULL) {
		dh_debug("bio alloc bio error\n");
		return E_HM_NOMEM;
	}
	if (blkops->bio_set_ctx != NULL) {
		blkops->bio_set_ctx(bio, devhost_block_end_io);
	}

	for (int i = 0; i < vec_cnt; i++) {
		unsigned long long pfn = vec[i].pfn;
		unsigned int len = vec[i].len;
		unsigned int offset = vec[i].offset;
		BUG_ON(offset + len > PAGE_SIZE, "offset:%u, len:%u", offset, len);
		ret = blkops->bio_add_page_by_pfn(bio, pfn, len, offset);
		if (ret < 0) {
			dh_debug("bio add data error\n");
			ret = posix2hmerrno(-ret);
			goto free_bio;
		}
	}

	ret = blkops->submit_bio(bio, 0);
	if (ret < 0) {
		dh_debug("bio submit bio error\n");
		ret = posix2hmerrno(-ret);
	}

free_bio:
	if (ret != E_HM_OK) {
		blkops->bio_free(bio);
	}
	return ret;
}

int devhost_submit_cmd(struct filp_node *node, unsigned long long io_cmd,
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
	bd_io.io_flags = (unsigned int)hmsrv_io_get_io_flags(io_cmd);
	bd_io.block_offset = sector;
	bd_io.bv_len = size;
	bd_ctx.direct_map_paddr = 1;
	bd_ctx.private = node->blk_ctx;
	bd_ctx.io_priority = priority;
	bio = blkops->bio_alloc(node->filp, &bd_io, &bd_ctx, rq_idx);
	if (bio == NULL) {
		dh_debug("bio alloc bio error\n");
		return E_HM_NOMEM;
	}
	if (blkops->bio_set_ctx != NULL) {
		blkops->bio_set_ctx(bio, devhost_block_end_io);
	}

	ret = blkops->submit_bio(bio, 0);
	if (ret < 0) {
		dh_debug("bio submit bio error\n");
		blkops->bio_free(bio);
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

static inline size_t devhost_get_queue_obj_cnt(void)
{
	return CONFIG_DEVHOST_BLK_SRING_LEN;
}

static inline size_t devhost_get_queue_item_cnt(void)
{
	return CONFIG_DEVHOST_BLK_SRING_LEN;
}

struct submit_bio_work {
	struct raw_work work;
	struct bd_vfs_queue *vfs_queue;
	struct devhost_block_ops *blkops;
	struct filp_node *node;
};

static struct bd_io *try_pop_bd_io(struct sring_buf *srbuf)
{
	struct bd_io *bd_io = NULL;

	sring_set_state(srbuf, RING_ONWORKING);
	bd_io = (struct bd_io *)sring_buf_pop(srbuf);
	if (bd_io != NULL) {
		return bd_io;
	}

	sring_set_state(srbuf, RING_IDLE);
	/*
	 * pop one last time after set ring status to IDLE
	 * so that any bd_io would not be missed.
	 */
	bd_io = (struct bd_io *)sring_buf_pop(srbuf);
	if (bd_io != NULL) {
		sring_set_state(srbuf, RING_ONWORKING);
		return bd_io;
	}

	return NULL;
}

static void async_end_io(struct filp_node *node, int err,
			 const struct bd_io *bd_io, unsigned int bd_io_idx)
{
	if ((bd_io->flags & REQ_SIO) == 0) {
		int ret = E_HM_OK;

		ret = devhost_block_queue_end_io(node->blk_ctx, err, bd_io_idx);
		if (ret != E_HM_OK) {
			dh_error("failed to send endio to vfs\n");
		}
	}
}

static inline bool
is_valid_bio_shm_vaddr(const struct bd_shm_info *shm_info,
		       uint64_t bio_page_len)
{
	return (shm_info->vaddr != NULL) &&
	       (bio_page_len <= (uint64_t)shm_info->len);
}

#define SECTOR_SHIFT 9
#define min(x, y) ((x) > (typeof(x))(y) ? (typeof(x))(y) : (x))
static int add_bio_data_by_vaddr(const struct devhost_block_ops *blkops,
				 const struct bd_io *bd_io,
				 const struct bd_shm_info *shm_info,
				 unsigned int bd_io_idx,
				 void *bio)
{
	uintptr_t bio_shm_vaddr = 0;
	int ret;
	uint64_t padding = (bd_io->block_offset << SECTOR_SHIFT) &
			   (~PAGE_MASK);

	if (!is_valid_bio_shm_vaddr(shm_info, (uint64_t)PAGE_SIZE *
				    (uint64_t)bd_io->page_offset)) {
		dh_debug("bio get addr error\n");
		return E_HM_INVAL;
	}
	bio_shm_vaddr = (uintptr_t)shm_info->vaddr +
			PAGE_SIZE * bd_io->page_offset +
			(uintptr_t)(long)bd_io_idx * shm_info->bd_shm_size +
			(uintptr_t)padding;

	ret = blkops->bio_add_data(bio, bio_shm_vaddr, bd_io->bv_len);
	if (ret < 0) {
		dh_debug("bio add data error\n");
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

static int add_bio_data_by_paddr(const struct devhost_block_ops *blkops,
				 const struct bd_io *bd_io,
				 const struct bd_shm_info *shm_info,
				 unsigned int bd_io_idx,
				 void *bio)
{
	BUG_ON(bd_io->bv_len == 0);
	size_t paddrs_cnt = shm_info->len >> PAGE_SHIFT;
	unsigned long long paddr;
	unsigned int bytes_left = bd_io->bv_len;
	size_t len;
	int ret;
	uint64_t padding = (bd_io->block_offset << SECTOR_SHIFT) &
			   (~PAGE_MASK);
	unsigned int page_cnt = (unsigned int)((bd_io->bv_len + padding - 1) >> PAGE_SHIFT) + 1;
	unsigned int page_per_bd_shm = (unsigned int)(shm_info->bd_shm_size >> PAGE_SHIFT);

	BUG_ON((page_cnt + bd_io_idx * page_per_bd_shm > paddrs_cnt),
	       "page_cnt:%u, bd_io_idx:%u, paddrs_cnt:%zu\n",
	       page_cnt, bd_io_idx, paddrs_cnt);

	if (!is_valid_bio_shm_vaddr(shm_info, (uint64_t)PAGE_SIZE *
				    (uint64_t)bd_io->page_offset)) {
		dh_debug("bio get addr error\n");
		return E_HM_INVAL;
	}

	for (unsigned int i = 0; i < page_cnt; i++) {
		len = min(bytes_left, PAGE_SIZE - padding);
		BUG_ON(bytes_left < len, "bytes_left:%u, len:%zu", bytes_left, len);
		paddr = shm_info->paddrs[i + bd_io_idx * page_per_bd_shm];
		ret = blkops->bio_add_page_by_pfn(bio, paddr >> PAGE_SHIFT, len, padding);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
		bytes_left -= (unsigned int)len;
		padding = 0;
	}

	return E_HM_OK;
}

static int submit_one_bio(struct filp_node *node,
			  const struct devhost_block_ops *blkops,
			  struct bd_io *bd_io,
			  const struct bd_shm_info *shm_info,
			  unsigned int bd_io_idx)
{
	void *bio = NULL;
	int ret = E_HM_OK;
	struct bd_context bd_ctx = { 0 };
	unsigned int io_rw = (unsigned int)bd_io->rw;

	if ((bd_io->bv_len > MAX_BIO_PAGE_NUM * PAGE_SIZE) &&
	    (((uint64_t)io_rw & REQ_DISCARD_SYSIF) == 0)) {
		dh_debug("bio bv_len error\n");
		return E_HM_INVAL;
	}

	bd_ctx.vec_cnt = 1;
	bd_ctx.direct_map_paddr = 0;
	bd_ctx.private = node->blk_ctx;
	bd_ctx.io_priority = bd_io->io_priority;
	bio = blkops->bio_alloc(node->filp, bd_io, &bd_ctx, bd_io_idx);
	if (bio == NULL) {
		dh_debug("bio alloc bio error\n");
		return E_HM_NOMEM;
	}
	if (blkops->bio_set_ctx != NULL) {
		blkops->bio_set_ctx(bio, devhost_block_queue_end_io);
	}

	if ((bd_io->bv_len != 0) &&
	    ((io_rw == 0) || (((uint64_t)io_rw & REQ_WRITE_MASK) != 0))) {
		if (device_flag_test(node->devinfo->p->flags, DEVICE_CAN_DIRECTMAP_SHM)) {
			ret = add_bio_data_by_paddr(blkops, bd_io, shm_info,
						    bd_io_idx, bio);
		} else {
			ret = add_bio_data_by_vaddr(blkops, bd_io, shm_info,
						    bd_io_idx, bio);
		}
		if (ret != E_HM_OK) {
			goto free_bio;
		}
	}

	devhost_update_bio_time(&bd_io->start);
	ret = blkops->submit_bio(bio, bd_io->flags);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		dh_debug("bio submit bio error\n");
	}

free_bio:
	if (ret != E_HM_OK) {
		blkops->bio_free(bio);
	}
	return ret;
}

static int __handle_submit_bio_work(struct raw_work *work)
{
	struct submit_bio_work *swork =
		container_of(work, struct submit_bio_work, work);
	struct sring_buf *srbuf = NULL;
	struct bd_vfs_queue *vfs_queue = swork->vfs_queue;
	struct devhost_block_ops *blkops = swork->blkops;
	struct filp_node *node = swork->node;
	struct bd_io *bd_io = NULL;
	unsigned int bd_io_idx;
	int ret = E_HM_OK;

	if (vfs_queue == NULL) {
		ret = E_HM_INVAL;
		goto out;
	}
	srbuf = vfs_queue->bd_queue.submit_queue;
	sring_set_state(srbuf, RING_ONWORKING);
	bd_io = try_pop_bd_io(srbuf);
	while (bd_io != NULL) {
		bd_io_idx = (unsigned int)(bd_io -
			    (struct bd_io *)(srbuf->data_vaddr));
		bd_io->in_submit = false;
		ret = submit_one_bio(node, blkops, bd_io,
				     &(vfs_queue->shm_info),
				     bd_io_idx);
		if (ret != E_HM_OK) {
			async_end_io(node, -hmerrno2posix(ret), bd_io, bd_io_idx);
			dh_debug("failed to submit bio for device, devname=%s\n",
				 node->devinfo->devname);
		}
		bd_io = try_pop_bd_io(srbuf);
	}
	ret = E_HM_OK;
out:
	raw_work_destroy(&swork->work);
	devhost_put_file(node);
	free(swork);
	return ret;
}

static struct bd_vfs_queue *bd_get_vfs_queue(struct device_info *devinfo,
					     struct filp_node *node)
{
	struct bd_vfs_queue *vfs_queue = NULL;
	struct bd_vfs_queue *tmp = NULL;

	(void)raw_rwlock_rdlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
	dlist_for_each_entry(tmp,
			     &(devinfo->p->bd_queue_info->bd_vfs_queue_list),
			     struct bd_vfs_queue, list) {
		if (tmp->node == node) {
			vfs_queue = tmp;
			break;
		}
	}
	(void)raw_rwlock_unlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
	return vfs_queue;
}

int devhost_do_async_submit_bio(struct filp_node *node)
{
	int err = E_HM_OK;
	struct submit_bio_work *swork = NULL;
	struct device_info *devinfo = NULL;
	struct bd_vfs_queue *vfs_queue = NULL;

	if ((node == NULL) || (node->devinfo == NULL)) {
		dh_debug("devinfo or node is null\n");
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;

	if (devinfo->p->bd_queue_info == NULL) {
		dh_debug("bd_queue_info is null\n");
		return E_HM_INVAL;
	}
	vfs_queue = bd_get_vfs_queue(devinfo, node);
	if (vfs_queue == NULL) {
		dh_debug("failed to find bd_queue for device, devname=%s\n",
			 devinfo->devname);
		return E_HM_INVAL;
	}
	if (sring_is_onwork(vfs_queue->bd_queue.submit_queue)) {
		return E_HM_OK;
	}
	swork = (struct submit_bio_work *)malloc(sizeof(*swork));
	if (swork == NULL) {
		dh_debug("failed to alloc memory for submit_bio_work\n");
		return E_HM_NOMEM;
	}

	err = raw_work_init(&swork->work, __handle_submit_bio_work, 0);
	if (err < 0) {
		dh_debug("failed to init submit_bio_work\n");
		free(swork);
		return err;
	}
	swork->vfs_queue = vfs_queue;
	swork->blkops = devinfo->p->blkops;
	swork->node = node;
	err = devhost_get_file(node);
	BUG_ON(err < 0);

	err = raw_workqueue_add_work(vfs_queue->bd_queue.bio_wq, &swork->work);
	if (err < 0) {
		raw_work_destroy(&swork->work);
		devhost_put_file(node);
		free(swork);
		dh_debug("failed to add submit_bio_work\n");
		return err;
	}

	return E_HM_OK;
}

int devhost_do_sync_submit_bio(struct filp_node *node, unsigned int bd_io_idx)
{
	struct device_info *devinfo = NULL;
	struct bd_vfs_queue *vfs_queue = NULL;
	struct bd_io *bd_io = NULL;
	struct devhost_block_ops *blkops = NULL;
	struct sring_buf *submit_queue = NULL;

	if ((node == NULL) || (node->devinfo == NULL) ||
	    (node->devinfo->p->blkops == NULL)) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;
	blkops = devinfo->p->blkops;

	vfs_queue = bd_get_vfs_queue(devinfo, node);
	if (vfs_queue == NULL) {
		dh_debug("failed to find bd_queue for device, devname=%s\n",
			 devinfo->devname);
		return E_HM_INVAL;
	}

	if ((size_t)bd_io_idx >= vfs_queue->obj_cnt) {
		dh_debug("bd_io index is invalid\n");
		return E_HM_INVAL;
	}

	submit_queue = vfs_queue->bd_queue.submit_queue;
	bd_io = (struct bd_io *)(submit_queue->data_vaddr) + bd_io_idx;
	return submit_one_bio(node, blkops, bd_io,
			      &(vfs_queue->shm_info),
			      bd_io_idx);
}
/*
 * Each device_info has 1 workqueue with 1 worker thread in devhost.
 * It pops bd_io from shm_ring_buffer and dispatch bd_io to liblinux or udk.
 */
static int devhost_init_bio_wq(struct bd_vfs_queue *vfs_queue)
{
	int err = E_HM_OK;
	struct raw_workqueue_attr attr;
	struct raw_workqueue *bio_wq = NULL;

	/*
	 * We only run *one* worker in the workqueue at the same time.
	 * Make sure we can handle ring working status correctly when
	 * we need to run worker concurrently.
	 */
	raw_workqueue_attr_init(&attr, DEVHOST_BIO_WORKER_NR,
				DEVHOST_BIO_WQ_NR);

	/* init submit bio workqueue */
	bio_wq = (struct raw_workqueue *)malloc(sizeof(*bio_wq));
	if (bio_wq == NULL) {
		dh_debug("failed to alloc memory for submit bio workqueue\n");
		return E_HM_NOMEM;
	}

	err = devhost_workq_attr_set_name(&attr, DH_BIO_WORKQ_TNAME);
	if (err < 0) {
		dh_warn("failed to set workq attr name, err=%s\n",
			hmstrerror(err));
	}
	err = raw_workqueue_attr_set_inherit(&attr, RAW_THREAD_EXPLICIT_SCHED);
	if (err < 0) {
		dh_warn("failed to set workq attr inherit, err=%s\n",
			hmstrerror(err));
	}
	err = raw_workqueue_attr_set_policy(&attr, hm_thread_sched_getdefaultscheduler());
	if (err < 0) {
		dh_warn("failed to set workq attr pilicy, err=%s\n",
			hmstrerror(err));
	}
	err = raw_workqueue_attr_set_prio(&attr, hm_thread_sched_getdefaultpriority());
	if (err < 0) {
		dh_warn("failed to set workq attr prio, err=%s\n",
			hmstrerror(err));
	}

	err = raw_workqueue_init(bio_wq, &attr);
	if (err < 0) {
		dh_debug("failed to initialize submit bio workqueue\n");
		free(bio_wq);
		return err;
	}

	vfs_queue->bd_queue.bio_wq = bio_wq;
	return E_HM_OK;
}

static void devhost_destroy_bio_wq(struct bd_vfs_queue *vfs_queue)
{
	if (vfs_queue->bd_queue.bio_wq) {
		(void)raw_workqueue_flush(vfs_queue->bd_queue.bio_wq);
		(void)raw_workqueue_destroy(vfs_queue->bd_queue.bio_wq);
		free(vfs_queue->bd_queue.bio_wq);
		vfs_queue->bd_queue.bio_wq = NULL;
	}
}

static int devhost_create_submit_queue(struct bd_vfs_queue *vfs_queue)
{
	int err = E_HM_OK;
	struct sring_buf *srbuf = NULL;

	srbuf = sring_buf_create(vfs_queue->item_cnt);
	if (srbuf == NULL) {
		dh_debug("create submit queue failed\n");
		return E_HM_INVAL;
	}

	err = sring_buf_install_data(srbuf, vfs_queue->obj_cnt,
				     sizeof(struct bd_io));
	if (err != E_HM_OK) {
		dh_debug("install data to submit queue failed\n");
		sring_buf_destroy(srbuf, false, true);
		free(srbuf);
		return E_HM_INVAL;
	}

	err = sring_buf_connect(srbuf, true);
	if (err != E_HM_OK) {
		dh_debug("allocate memory for submit queue failed\n");
		sring_buf_destroy(srbuf, true, true);
		free(srbuf);
		return err;
	}

	sring_buf_init_info(srbuf, vfs_queue->item_cnt, vfs_queue->obj_cnt,
			    sizeof(struct bd_io));

	vfs_queue->bd_queue.submit_queue = srbuf;
	return E_HM_OK;
}

static int devhost_create_end_queue(struct bd_vfs_queue *vfs_queue)
{
	int err = E_HM_OK;
	struct sring_buf *srbuf = NULL;

	srbuf = sring_buf_create(vfs_queue->item_cnt);
	if (srbuf == NULL) {
		dh_debug("create end sring buf failed\n");
		return E_HM_INVAL;
	}

	err = sring_buf_connect(srbuf, false);
	if (err != E_HM_OK) {
		dh_debug("allocate memory for end sring buf failed\n");
		sring_buf_destroy(srbuf, false, true);
		free(srbuf);
		return err;
	}

	sring_buf_share_data(srbuf, vfs_queue->bd_queue.submit_queue);
	sring_buf_init_info(srbuf, vfs_queue->item_cnt, vfs_queue->obj_cnt,
			    sizeof(struct bd_io));

	vfs_queue->bd_queue.end_queue = srbuf;

	return E_HM_OK;
}

static void devhost_destroy_bd_queue(struct bd_vfs_queue *vfs_queue)
{
	/* submit ring includes fd data */
	if (vfs_queue->bd_queue.submit_queue) {
		sring_buf_disconnect(vfs_queue->bd_queue.submit_queue, true);
		sring_buf_destroy(vfs_queue->bd_queue.submit_queue, true, true);
		free(vfs_queue->bd_queue.submit_queue);
		vfs_queue->bd_queue.submit_queue = NULL;
	}
	/* end ring does not include fd data */
	if (vfs_queue->bd_queue.end_queue) {
		(void)raw_rwlock_wrlock(&vfs_queue->bd_queue.end_queue->rwlock);
		sring_buf_disconnect(vfs_queue->bd_queue.end_queue, false);
		sring_buf_destroy(vfs_queue->bd_queue.end_queue, false, true);
		(void)raw_rwlock_unlock(&vfs_queue->bd_queue.end_queue->rwlock);
		free(vfs_queue->bd_queue.end_queue);
		vfs_queue->bd_queue.end_queue = NULL;
	}
}

static int devhost_init_bd_queue(struct bd_vfs_queue *vfs_queue)
{
	int err = E_HM_OK;

	/* init submit bio queue */
	err = devhost_create_submit_queue(vfs_queue);
	if (err != E_HM_OK) {
		return err;
	}

	/* init end bio srbuf */
	err = devhost_create_end_queue(vfs_queue);
	if (err != E_HM_OK) {
		devhost_destroy_bd_queue(vfs_queue);
		return err;
	}

	return E_HM_OK;
}

/*
 * If the parameter success is true, it means that it is a normal path,
 * and if it is false, it means an error path. When the path is wrong,
 * the shared memory needs to be released on the devhost side to avoid
 * memory leaks. When it is a normal path, the shared memory is released
 * by vfs, and devhost does not perform the release operation.
 */
static void free_bio_shm(struct bd_shm_info *shm_info, bool success)
{
	if (shm_info == NULL) {
		return;
	}

	if (shm_info->paddrs != NULL) {
		free(shm_info->paddrs);
		shm_info->paddrs = NULL;
	}

	if ((shm_info->vma_info != NULL) && !success) {
		(void)devhost_pal_usermap_munmap(shm_info->vma_info->start, shm_info->len);
		shm_info->vma_info = NULL;
	}

	if (shm_info->handle != NULL) {
		BUG_ON(shm_info->vaddr == NULL);
		devhost_backend_free(shm_info->handle);
		shm_info->handle = NULL;
		shm_info->vaddr = NULL;
	}
}

static int __bd_shm_paddrs_init(struct bd_shm_info *shm_info,
				const struct mem_raw_ranges *ranges,
				unsigned int ranges_num, unsigned int page_num)
{
	unsigned int pfn_num, num, i, j;
	unsigned long long paddr;
	unsigned long long *paddrs = NULL;

	if (ranges == NULL) {
		return E_HM_INVAL;
	}

	paddrs = malloc(sizeof(unsigned long long) * page_num);
	if (paddrs == NULL) {
		return E_HM_NOMEM;
	}

	num = 0;
	for (i = 0; i < ranges_num; i++) {
		pfn_num = (unsigned int)(ranges[i].end - ranges[i].start);
		paddr = ranges[i].start << PAGE_SHIFT;
		for (j = 0; j < pfn_num; j++) {
			paddrs[num] = paddr + ((unsigned long long)j << PAGE_SHIFT);
			num++;
		}
	}
	shm_info->paddrs = paddrs;

	return E_HM_OK;
}

static int __bd_map_mem_to_vfs(struct bd_shm_info *shm_info, struct mem_raw_ranges *ranges,
			       unsigned int ranges_num)
{
	int err;
	struct devhost_vma_info *vma_info = NULL;
	unsigned int flags = LIBDH_REMAP_CACHE;
	unsigned long prot = MPROT_READ | MPROT_WRITE;

	if (ranges == NULL) {
		return E_HM_INVAL;
	}

	err = devhost_pal_usermap_prepare(0, shm_info->len, prot, flags, false, &vma_info);
	if (err < 0) {
		dh_error("bio usermap prepare failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = libdh_iomem_map_batch(ulong_to_ptr(vma_info->start, void), ranges, ranges_num,
				    (unsigned int)prot, flags);
	if (err < 0) {
		dh_error("bio iomem map failed, err=%s\n", hmstrerror(err));
		devhost_pal_usermap_finish(vma_info, false, 0 /* fail, rollback */, 0);
		return err;
	}

	devhost_pal_usermap_finish(vma_info, false, 1 /* success */, 0);
	shm_info->vma_info = vma_info;

	return E_HM_OK;
}

static int __prepare_bd_shm_info(struct bd_shm_info *shm_info, size_t bd_shm_size,
				 size_t obj_cnt, bool map_page_pa)
{
	int err;
	unsigned int page_num, ranges_num;
	struct mem_raw_ranges *ranges = NULL;
	size_t size = bd_shm_size * obj_cnt;

	/* 1. alloc memory from backend */
	shm_info->len = PAGE_ALIGN_UP(size);
	shm_info->bd_shm_size = bd_shm_size;
	shm_info->vaddr = devhost_backend_alloc(shm_info->len, &shm_info->handle);
	if (shm_info->vaddr == NULL) {
		dh_error("bio backend alloc mem failed\n");
		return E_HM_NOMEM;
	}

	/* 2. query the requested memory */
	page_num = (unsigned int)(shm_info->len >> PAGE_SHIFT);
	ranges = malloc(sizeof(*ranges) * page_num);
	if (ranges == NULL) {
		err = E_HM_NOMEM;
		goto err_out;
	}

	err = devhost_backend_query(shm_info->handle, ranges, page_num);
	if (err < 0) {
		dh_error("bio backend query mem info failed, err=%s\n", hmstrerror(err));
		goto err_query;
	}
	ranges_num = (unsigned int)err;

	/* 3. prepare paddrs */
	if (map_page_pa) {
		err = __bd_shm_paddrs_init(shm_info, ranges, ranges_num, page_num);
		if (err < 0) {
			dh_error("bio prepare shm paddrs failed\n");
			goto err_query;
		}
	}

	/* 4. map mem to vfs */
	err = __bd_map_mem_to_vfs(shm_info, ranges, ranges_num);
	if (err < 0) {
		goto err_query;
	}
	free(ranges);

	return E_HM_OK;

err_query:
	free(ranges);
err_out:
	free_bio_shm(shm_info, false);
	return err;
}

/* caller must hold devinfo->p->bd_queue_info->bd_vfs_queue_rwlock */
static int __devhost_init_bio(struct device_info *devinfo, struct filp_node *node,
			      struct bd_vfs_queue **bd_vfs_queue, size_t bd_shm_size)
{
	struct bd_vfs_queue *vfs_queue = NULL;
	int err = E_HM_OK;
	bool map_page_pa;

	*bd_vfs_queue = NULL;
	vfs_queue = (struct bd_vfs_queue *)malloc(sizeof(struct bd_vfs_queue));
	if (vfs_queue == NULL) {
		return E_HM_NOMEM;
	}
	mem_zero_s(*vfs_queue);

	vfs_queue->node = node;
	vfs_queue->obj_cnt = devhost_get_queue_obj_cnt();
	vfs_queue->item_cnt = devhost_get_queue_item_cnt();

	map_page_pa = device_flag_test(devinfo->p->flags, DEVICE_CAN_DIRECTMAP_SHM) ? true : false;
	err = __prepare_bd_shm_info(&vfs_queue->shm_info, bd_shm_size, vfs_queue->obj_cnt, map_page_pa);
	if (err < 0) {
		free(vfs_queue);
		return err;
	}

	err = devhost_init_bio_wq(vfs_queue);
	if (err < 0) {
		free_bio_shm(&(vfs_queue->shm_info), false);
		free(vfs_queue);
		dh_warn("failed to init bio workqueue\n");
		return err;
	}

	err = devhost_init_bd_queue(vfs_queue);
	if (err < 0) {
		free_bio_shm(&(vfs_queue->shm_info), false);
		devhost_destroy_bio_wq(vfs_queue);
		free(vfs_queue);
		dh_warn("failed to init bio sring buf\n");
		return err;
	}

	dlist_insert_tail(&(devinfo->p->bd_queue_info->bd_vfs_queue_list), &vfs_queue->list);
	*bd_vfs_queue = vfs_queue;

	return E_HM_OK;
}

int devhost_init_bio(struct filp_node *node,
		     struct bd_vfs_queue **bd_vfs_queue,
		     size_t bd_shm_size)
{
	int err = E_HM_OK;
	struct bd_vfs_queue *vfs_queue = NULL;
	struct device_info *devinfo = NULL;

	if (node == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;

	if ((devinfo == NULL) || (devinfo->p->bd_queue_info == NULL) ||
	    (bd_vfs_queue == NULL)) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
	dlist_for_each_entry(vfs_queue,
			     &(devinfo->p->bd_queue_info->bd_vfs_queue_list),
			     struct bd_vfs_queue, list) {
		if (vfs_queue->node == node) {
			dh_warn("bio queue already initialized for device, "
				"devname=%s",
				devinfo->devname);
			(void)raw_rwlock_unlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
			return E_HM_ALREADY;
		}
	}

	err = __devhost_init_bio(devinfo, node, bd_vfs_queue, bd_shm_size);
	if (err != E_HM_OK) {
		dh_warn("bd_queue initialized failed, device=%s, err=%s\n",
			devinfo->devname, hmstrerror(err));
	} else {
		dh_debug("bd_queue initialized successfully, device=%s\n",
			 devinfo->devname);
	}

	(void)raw_rwlock_unlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
	return err;
}

int devhost_grant_bio(struct bd_vfs_queue *vfs_queue, struct bd_grant_ret_s *ret)
{
	uint64_t key;
	int err = E_HM_OK;
	struct sring_buf *submit_queue = vfs_queue->bd_queue.submit_queue;
	struct sring_buf *end_queue = vfs_queue->bd_queue.end_queue;

	err = hm_ashm_grant_to_caller(submit_queue->ring_fd, SHM_GRANT_RDWR, &key);
	if (err < 0) {
		dh_debug("failed to grant submit fd to vfs, err=%s\n",
			hmstrerror(err));
		return err;
	}
	ret->submit_fd = key;

	err = hm_ashm_grant_to_caller(submit_queue->data_fd, SHM_GRANT_RDWR, &key);
	if (err < 0) {
		dh_debug("failed to grant data fd to vfs, err=%s\n",
			hmstrerror(err));
		return err;
	}
	sring_buf_set_info_data_fd(submit_queue, key);
	sring_buf_set_info_data_fd(end_queue, key);

	err = hm_ashm_grant_to_caller(end_queue->ring_fd, SHM_GRANT_RDWR, &key);
	if (err < 0) {
		dh_debug("failed to grant end fd to vfs, err=%s\n",
			hmstrerror(err));
		return err;
	}
	ret->end_fd = key;

	return E_HM_OK;
}

void devhost_destroy_bio(struct filp_node *node, bool success)
{
	struct bd_vfs_queue *vfs_queue = NULL;
	bool found = false;
	struct device_info *devinfo = NULL;

	if (node == NULL) {
		return;
	}
	devinfo = node->devinfo;
	if ((devinfo == NULL) || (devinfo->p->bd_queue_info == NULL)) {
		return;
	}

	(void)raw_rwlock_wrlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
	dlist_for_each_entry(vfs_queue,
			     &(devinfo->p->bd_queue_info->bd_vfs_queue_list),
			     struct bd_vfs_queue, list) {
		if (vfs_queue->node == node) {
			found = true;
			break;
		}
	}

	if (found == false) {
		(void)raw_rwlock_unlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
		dh_warn("ring not exist\n");
		return;
	}

	dlist_del_entry(vfs_queue, struct bd_vfs_queue, list);
	(void)raw_rwlock_unlock(&(devinfo->p->bd_queue_info->bd_vfs_queue_rwlock));
	/* ensure all works had been handled over */
	devhost_destroy_bio_wq(vfs_queue);
	devhost_destroy_bd_queue(vfs_queue);
	free_bio_shm(&(vfs_queue->shm_info), success);
	free(vfs_queue);
	dh_debug("ring buf released successfully, device=%s\n",
		 devinfo->devname);

	return;
}
