/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 02 04:43:16 2019
 */
#include "udk_block_internal.h"
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_stats.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk_stats.h>

#include <string.h>
#include <inttypes.h>

#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/timer.h>
#include <internal/init.h>
#include <udk/workqueue.h>
#include <udk/mutex.h>
#include <udk/mm.h>
#include <udk/block/udk_disk.h>
#include <hongmeng/errno.h>
#include <libsysif/devhost/api.h>
#include <libhwsecurec/securec.h>
#ifdef CONFIG_HISI_UDK_MAS_BLK
#include <basicplatform/udk/block/mas_blk/mas_blk.h>
#endif
#include <udk/trace/events/trace_list.h>

#ifdef CONFIG_UDK_FALLBACK_ENCRYPTION
#include <udk/block/udk_block_fallback.h>
#endif

#ifdef UDK_FUSION
#include "../../fusion/pm.h"
#endif

#define BLOCK_QUEUE_NAME		"udk-block-wq"
/* define 1 worker and 1 queue to process block works */
#define BLOCK_WORK_NUM_DEFAULT		1
#define BLOCK_WORK_PRIO_NUM		1
/* define workqueue priority 80 */
#define BLOCK_QUEUE_PRIORITY		80

static struct udk_workqueue *g_block_queue = NULL;

static int block_workqueue_attr_init(struct udk_workqueue_attr *attr)
{
	int ret;

	mem_zero_s(*attr);
	attr->nr_workers = BLOCK_WORK_NUM_DEFAULT;
	attr->nr_queues = BLOCK_WORK_PRIO_NUM;
	attr->thread_priority = BLOCK_QUEUE_PRIORITY;

	ret = snprintf_s(attr->name, UDK_THREADPOOL_NAME_LEN,
			 UDK_THREADPOOL_NAME_LEN - 1, "%s", BLOCK_QUEUE_NAME);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

static int block_workqueue_init(void)
{
	int ret;
	struct udk_workqueue_attr attr;

	ret = block_workqueue_attr_init(&attr);
	if (ret < 0) {
		return ret;
	}

	g_block_queue = udk_workqueue_alloc(&attr);
	if (g_block_queue == NULL) {
		return -ENOMEM;
	}

	return 0;
}

static int block_workqueue_add_work(struct udk_work *work)
{
	return udk_workqueue_add_work(g_block_queue, work);
}

udk_init_call(block_workqueue_init);
/* block queue does not need to be destroyed. It should always exist while the system is alive */

struct udk_bio *udk_bio_alloc(unsigned int vec_cnt, bool direct_map_paddr)
{
	struct udk_bio *bio = NULL;

	if (direct_map_paddr) {
		/**
		 * There are two senarios for zero-copy
		 * fusion:
		 *   udk_bio_pvec can reuse vec passed from fs, so we don't
		 *   need to malloc extra memory for bio
		 * non-fusion:
		 *   udk_bio_pvec located at the end of struct bio,
		 *   so make bio->bio_io_pvect point bio + 1U default.
		 */
#ifdef UDK_FUSION
		bio = (struct udk_bio *)udk_malloc_ex(sizeof(struct udk_bio), UDK_PAF_NO_IO | UDK_PAF_NO_FAIL);
#else
		bio = (struct udk_bio *)malloc(sizeof(struct udk_bio) +
					       sizeof(struct udk_bio_pvec) * vec_cnt);
#endif
		if (bio == NULL) {
			udk_error("Failed to allocate udk_bio\n");
			return NULL;
		}
		mem_zero_b(bio, sizeof(struct udk_bio));
		bio->bio_io_pvec = (struct udk_bio_pvec *)(bio + 1U);
	} else {
		bio = (struct udk_bio *)malloc(sizeof(struct udk_bio));
		if (bio == NULL) {
			udk_error("Failed to allocate udk_bio\n");
			return NULL;
		}
		mem_zero_s(*bio);
		bio->bio_io_vec = (struct udk_bio_vec *)malloc(
			sizeof(struct udk_bio_vec) * vec_cnt);
		if (bio->bio_io_vec == NULL) {
			free(bio);
			return NULL;
		}
	}
	bio->bi_vec_size = vec_cnt;
	bio->bi_idx = UDK_BIO_UNALLOCATED_IDX;
	vatomic32_init(&bio->remain_cnt, 1);

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_bio_init(bio);
#endif

	return bio;
}

static int udk_blk_end_io(struct udk_bio *bio)
{
	struct udk_bio_context *bio_ctx = &bio->bi_ctx;

#ifdef CONFIG_UDK_FALLBACK_ENCRYPTION
	bool ret;
	if (udk_bio_has_crypt_ctx(bio)) {
		if (udk_bio_data_dir(bio) == UDK_REQ_WRITE) {
			udk_blk_crypto_fallback_encrypt_endio(bio);
		} else if (udk_bio_req_op(bio) == UDK_REQ_READ) {
			ret = udk_blk_crypto_fallback_decrypt_endio(bio);
			if (!ret)
				bio->bi_error = -EIO;
		}
	}
#endif

	udk_diskstats_update_nsecs(bio);
	udk_bio_info_set_stage(bio, (int)UDK_BIO_STAGE_ENDIO);
	return bio_ctx->devhost_end_bio_fn(bio_ctx->block_ctx,
						  bio->bi_error,
						  bio_ctx->bio_idx);
}

void udk_bio_free(struct udk_bio *bio)
{
	struct udk_bio_context *bi_ctx = NULL;
	int ret;

	if (bio == NULL) {
		return;
	}
	bi_ctx = &bio->bi_ctx;
	if ((bi_ctx->devhost_end_bio_fn != NULL) && !bi_ctx->direct_map_paddr) {
		ret = udk_blk_end_io(bio);
		if (ret < 0) {
			udk_error("udk bio end io err=%d\n", ret);
		}
	}

	/* reset the bio_stat->bio_info[bio->idx] */
	udk_bio_info_reset(bio);
	if (bio->bio_io_vec != NULL) {
		free(bio->bio_io_vec);
	}

#if defined(CONFIG_HISI_INLINE_ENCRYPTION) || defined(CONFIG_UDK_FALLBACK_ENCRYPTION)
	if (bio->crypto_key != NULL) {
		free(bio->crypto_key);
	}
#endif

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_bio_free(bio);
	mas_blk_bio_put(bio);
#else
	bio->bi_dev = NULL;
	bio->bi_part_dev = NULL;
	free(bio);
#endif
}

int udk_bio_add_data(struct udk_bio *bio, void *vaddr, unsigned int bv_len)
{
	struct udk_bio_vec *bv = NULL;

	if (bio == NULL) {
		return -EINVAL;
	}

	if (bio->bi_vec_num >= bio->bi_vec_size) {
		return -EIO;
	}

	bv = &bio->bio_io_vec[bio->bi_vec_num];
	bv->vaddr = vaddr;
	bv->bv_len = bv_len;
	bio->bi_vec_num++;
	bio->bi_size += bv_len;

	return 0;
}

struct bio_work_data {
	struct udk_bio *bio;
	struct udk_work work;
};

static int bio_work_handler(struct udk_work *work_p)
{
	struct bio_work_data *bio_work = container_of(work_p, struct bio_work_data, work);
	struct udk_bio *bio = bio_work->bio;
	struct udk_bio_context *bio_ctx = &bio->bi_ctx;
	int ret = 0;

	if (bio_ctx->devhost_end_bio_fn != NULL) {
		ret = udk_blk_end_io(bio);
		if (ret < 0) {
			udk_error("devhost end io err=%d\n", ret);
		}
	}
	udk_work_destroy(work_p);
	udk_bio_free(bio_work->bio);
	udk_free(bio_work);

	return ret;
}

static int queue_end_io_task(struct udk_bio *bio)
{
	int ret;

	struct bio_work_data *bio_work = udk_malloc(sizeof(struct bio_work_data));
	if (bio_work == NULL) {
		return -ENOMEM;
	}
	bio_work->bio = bio;

	ret = udk_work_init(&bio_work->work, bio_work_handler, 1);
	if (ret < 0) {
		udk_free(bio_work);
		return ret;
	}

	/* if no end_bio_wq assigned, use default udk block workqueue */
	if (bio->bi_ctx.end_bio_wq != NULL) {
		ret = udk_workqueue_add_work(bio->bi_ctx.end_bio_wq, &bio_work->work);
	} else {
		ret = block_workqueue_add_work(&bio_work->work);
	}
	if (ret < 0) {
		udk_free(bio_work);
	}

	return ret;
}

int hm_irq_exit(void);
int hm_irq_exec_time(void);
#define BIO_IRQ_LIMIT_TIME_US		2000
static bool should_queue_end_io(void)
{
	int err = 0;
	err = hm_irq_exit();
	if (err < 0) {
		return false;
	}
	return hm_irq_exec_time() > BIO_IRQ_LIMIT_TIME_US;
}

static int process_end_io_task(struct udk_bio *bio)
{
	int ret = 0;
	struct udk_bio_context *bio_ctx = &bio->bi_ctx;

	if (bio_ctx->devhost_end_bio_fn != NULL) {
		ret = udk_blk_end_io(bio);
		if (ret < 0) {
			udk_warn("devhost end io ret=%d\n", ret);
		}
	}

	udk_bio_free(bio);
	return ret;
}

static struct udk_bio *udk_bio_clone(struct udk_bio *bio,
				     unsigned int vec_offset, unsigned int vec_cnt)
{
	struct udk_bio *clone = NULL;

	if (udk_bio_op_is_discard(bio)) {
		clone = udk_bio_alloc(0, bio->bi_ctx.direct_map_paddr);
	} else {
		clone = udk_bio_alloc(vec_cnt, bio->bi_ctx.direct_map_paddr);
	}
	if (clone == NULL) {
		return NULL;
	}

	clone->bi_idx = UDK_BIO_UNALLOCATED_IDX;
	clone->bi_rw = bio->bi_rw;
	clone->bi_dev = bio->bi_dev;
	clone->bi_part_dev = bio->bi_part_dev;
	clone->bi_ctx.async = bio->bi_ctx.async;
	clone->bi_ctx.direct_map_paddr = bio->bi_ctx.direct_map_paddr;
	clone->bi_sector = bio->bi_sector + (vec_offset << UDK_PFN_PER_SECTOR_SHIFT);
	clone->bi_size = vec_cnt << (UDK_PFN_PER_SECTOR_SHIFT + UDK_BLKSHIFT_DEFAULT);

	if (!udk_bio_op_is_discard(bio)) {
		clone->bio_io_pvec = bio->bio_io_pvec + vec_offset;
		clone->bi_vec_num = vec_cnt;
	}

	return clone;
}

static bool udk_bio_param_vaild(struct udk_bio *bio)
{
	bool valid = true;

	/* Only support direct_map_paddr at present */
	if (bio == NULL || bio->bi_ctx.direct_map_paddr == 0) {
		return false;
	}

	/* Only support pvec with pfn length and offset 0 */
	for (unsigned int i = 0U; i < bio->bi_vec_num; i++) {
		if (bio->bio_io_pvec[i].len != (1 << UDK_PFN_SHIFT) ||
		    bio->bio_io_pvec[i].offset != 0) {
			valid = false;
			break;
		}
	}

	return valid;
}

/**
 * Split from the start of the bio. Bio will not change after call this fuction,
 * you can call udk_bio_increase to increase bio after split.
 * Incoming bio      |-----------|
 * Return split bio  |----|
 *
 * The split bio reused the pvec memory of bio, so no need to malloc pvec.
 */
struct udk_bio *udk_bio_split(struct udk_bio *bio, unsigned int sectors)
{
	unsigned int vec_cnt;

	if (!udk_bio_param_vaild(bio) ||
	    sectors == 0 || sectors >= udk_bio_sectors(bio) ||
	    (sectors & UDK_PFN_PER_SECTOR_MASK) != 0) {
		return NULL;
	}

	vec_cnt = sectors >> UDK_PFN_PER_SECTOR_SHIFT;

	return udk_bio_clone(bio, 0, vec_cnt);
}

static int udk_bio_chain_endio(void *private, int bi_error, unsigned int io_idx)
{
	UNUSED(io_idx);

	struct udk_bio *parent = (struct udk_bio *)private;

	if (bi_error != 0 && parent->bi_error == 0) {
		parent->bi_error = bi_error;
	}

	return udk_bio_end(parent);
}

/**
 * This function can automatically process end_io in async mode.
 * It ensure that the udk_bio_end of the parent bio is executed
 * after all the split bios are completed.
 */
void udk_bio_chain(struct udk_bio *bio, struct udk_bio *parent)
{
	if (bio == NULL || parent == NULL) {
		udk_warn("chain bio params invalid\n");
		return;
	}

	bio->bi_ctx.devhost_end_bio_fn = udk_bio_chain_endio;
	set_udk_bio_context(bio, parent);
	vatomic32_inc(&parent->remain_cnt);
}

void udk_bio_increase(struct udk_bio *bio, unsigned int sectors)
{
	unsigned int ven_cnt;

	if (bio == NULL || sectors >= udk_bio_sectors(bio) ||
	    (sectors & UDK_PFN_PER_SECTOR_MASK) != 0) {
		udk_warn("increase bio %u invalid\n", sectors);
		return;
	}

	if (!udk_bio_op_is_discard(bio)) {
		ven_cnt = sectors >> UDK_PFN_PER_SECTOR_SHIFT;
		bio->bio_io_pvec += ven_cnt;
		bio->bi_vec_num -= ven_cnt;
		bio->bi_vec_size -= ven_cnt;
	}

	bio->bi_sector += sectors;
	bio->bi_size -= sectors << UDK_BLKSHIFT_DEFAULT;
}

static bool udk_bio_remaining_done(struct udk_bio *bio)
{
	BUG_ON(vatomic32_read(&bio->remain_cnt) == 0);

	return (vatomic32_dec_get(&bio->remain_cnt) == 0);
}

int udk_bio_end(struct udk_bio *bio)
{
	int ret = 0;
	bool is_should_queue = false;

	if (bio == NULL) {
		return -EINVAL;
	}

	if (!udk_bio_remaining_done(bio)) {
		return 0;
	}

	/* fs only support async && zero copy, this case we should end bio */
	if ((bio->bi_ctx.async) && (bio->bi_ctx.direct_map_paddr)) {
		is_should_queue = should_queue_end_io();
		if(is_should_queue) {
			ret = queue_end_io_task(bio);
		}
		/* queue_end_io_task is an optimization path, if failed, just use default end io task */
		if (!is_should_queue || (ret < 0)) {
			ret = process_end_io_task(bio);
		}
	} else {
		udk_bio_free(bio);
	}

	return ret;
}

/*
 * If bio->bi_dev is a partition, remap the location to part0
 */
static int udk_blk_partition_remap(struct udk_bio *bio)
{
	unsigned int bio_sectors = udk_bio_nr_sectors(bio);
	struct udk_device *udkdev = bio->bi_dev;
	struct udk_partition *part = NULL;

	if (bio_sectors == 0) {
		return E_HM_OK;
	}
	/* Check whether this bio extends beyond the end of the partition. */
	part = udkdev_to_part(udkdev);
	/*
	 * If we support extend the partitions, One may extend the part while
	 * we are submiting bio, so following nr_sects should be converted to
	 * be protected in the future.
	 */
	if (part->nr_sects != 0) {
		if (part->nr_sects < bio_sectors ||
		    part->nr_sects - bio_sectors < bio->bi_sector) {
			udk_warn("this bio extends beyond the end of the partition\n");
			udk_warn("%s: rw=%lu, cur=%u, limit=%"PRIu64"\n", udkdev->name,
				 bio->bi_rw, bio_sectors, part->nr_sects);
			return E_HM_INVAL;
		}
	}

	if (udkdev->parent != NULL && udkdev != udkdev->parent) {
		bio->bi_sector += part->start_sect;
		bio->bi_dev = udkdev->parent;
		trace_block_bio_remap(bio, bio->bi_part_dev->devt, bio->bi_sector - part->start_sect);
	}

	return E_HM_OK;
}

static int __udk_blk_submit_bio(struct udk_bio *bio, unsigned int flags)
{
	struct udk_disk *disk = NULL;
	int ret;

	disk = udkdev_to_disk(bio->bi_dev);
	if (((flags & REQ_SIO) != 0) &&
	    !udk_disk_check_flag(disk, UDK_DISK_FLAG_SYNC_IO)) {
		udk_error("No support for SIO on %s\n", disk->name);
		return E_HM_INVAL;
	}

	ret = udk_blk_partition_remap(bio);
	if (ret != E_HM_OK) {
		return ret;
	}

	/* preflush+r/w IO split into flush IO and r/w IO */
	if (((udk_bio_req_flags(bio) & UDK_REQ_FLAGS_PREFLUSH) != 0) &&
	     ((udk_bio_req_op(bio) & UDK_REQ_FLUSH) == 0)) {
		ret = udk_blk_submit_cmd_sync(bio->bi_dev, UDK_REQ_FLUSH, 0ULL, 0ULL);
		if (ret < 0) {
			udk_warn("submit flush io faild, err=%s\n", strerror(-ret));
			return posix2hmerrno(-ret);
		}
		bio->bi_rw &= (~UDK_REQ_FLAGS_PREFLUSH);
	}

	if (disk->bd_queue != NULL) {
		(void)udk_blk_mq_queue_enter(disk->bd_queue);
		ret = udk_blk_mq_submit_bio(bio);
		udk_blk_mq_queue_exit(disk->bd_queue);
		return posix2hmerrno(-ret);
	}

	if (disk->disk_fops == NULL || disk->disk_fops->submit_bio == NULL) {
		udk_error("No submit_io for %s\n", disk->name);
		return E_HM_INVAL;
	}

	ret = disk->disk_fops->submit_bio(disk, bio);
	return posix2hmerrno(-ret);
}

static inline void blk_bio_stats_set(struct udk_bio *bio)
{
	udk_diskstats_inc_bio_and_sectors(bio);
	udk_bio_info_init(bio);
}

#ifdef CONFIG_UDK_FALLBACK_ENCRYPTION
bool udk_blk_crypto_bio_prep(struct udk_bio *bio)
{
	if (udk_bio_has_crypt_ctx(bio))
		return udk_blk_crypto_fallback_bio_prep(bio);
	return true;
}
#else
bool udk_blk_crypto_bio_prep(struct udk_bio *bio)
{
	return true;
}
#endif

static void __udk_blk_bio_init(struct udk_bio *bio,
			       struct udk_disk *disk,
			       unsigned int op)
{
	bio->bi_rw = op;
	bio->bi_dev = &(disk->part0.udkdev);
}

struct udk_request *udk_blk_fetch_request(struct udk_request_queue *queue,
					  struct udk_disk *disk,
					  unsigned int op)
{
	struct udk_request *req = NULL;
	struct udk_bio *bio = NULL;

	if (queue == NULL || disk == NULL) {
		return NULL;
	}

	if (disk->bd_queue != NULL) {
		bio = udk_bio_alloc(1, false);
		if (bio == NULL) {
			return NULL;
		}

		__udk_blk_bio_init(bio, disk, op);
		udk_blk_mq_queue_enter(disk->bd_queue);

		req = udk_blk_mq_alloc_request(queue, bio);
		if (req == NULL) {
			udk_blk_mq_queue_exit(disk->bd_queue);
			udk_bio_free(bio);
			return NULL;
		}
		req->bio = bio;
	}

	return req;
}

int udk_blk_execute_rq(struct udk_disk *disk,
		       struct udk_request *req)
{
	int ret = -EINVAL;

	if (disk == NULL || req == NULL) {
		return -EINVAL;
	}

	if (disk->bd_queue != NULL) {
		ret = udk_blk_mq_sched_request(req);
		udk_blk_mq_queue_exit(disk->bd_queue);
	}

	return ret;
}

int udk_blk_submit_bio(struct udk_bio *bio, unsigned int flags)
{
	int ret = 0;

	if (bio == NULL) {
		udk_error("Failed to get udk bio\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		blk_bio_stats_set(bio);
		if (udk_blk_crypto_bio_prep(bio)) {
			ret = __udk_blk_submit_bio(bio, flags);
		} else {
			ret = E_HM_IO;
		}
		if (ret < 0) {
			udk_diskstats_dec_bio_and_sectors(bio);
		}
	}

	return ret;
}

#ifdef CONFIG_HISI_UDK_MAS_BLK
struct op_description {
	unsigned int type;
	char description;
};

static int mas_blk_fill_rwbs(char *rwbs, unsigned int mas_op, int offset)
{
	int i = 0;
	unsigned long loop;
	struct op_description description[] = {
		{ REQ_TZ, 'Z' }, { REQ_CP, 'C' },
		{ REQ_VIP, 'V' }, { REQ_FAULT_OUT, 'P' },
		{ REQ_BATCH_OUT, 'B' }, { REQ_TT_RB, 'T'},
		{ MAS_REQ_FG, 'H' }, { MAS_REQ_SYNC, 'S' }
	};

	for (loop = 0; loop < ARRAY_SIZE(description); ++loop) {
		if (mas_op & description[loop].type) {
			rwbs[offset + i++] = description[loop].description;
		}
	}

	return i;
}
#endif

#ifdef CONFIG_HISI_UDK_MAS_BLK
void udk_blk_fill_rwbs(char *rwbs, unsigned int op, unsigned int mas_op)
#else
void udk_blk_fill_rwbs(char *rwbs, unsigned int op)
#endif
{
	int i = 0;

	if (op & UDK_REQ_FLAGS_PREFLUSH) {
		rwbs[i++] = 'F';
	}

	switch (op & UDK_REQ_OP_MASK) {
	case UDK_REQ_WRITE:
		rwbs[i++] = 'W';
		break;
	case UDK_REQ_DISCARD:
		rwbs[i++] = 'D';
		break;
	case UDK_REQ_FLUSH:
		rwbs[i++] = 'F';
		break;
	case UDK_REQ_READ:
		rwbs[i++] = 'R';
		break;
	default:
		rwbs[i++] = 'N';
	}

	if (op & UDK_REQ_FLAGS_FUA) {
		rwbs[i++] = 'F';
	}

#ifdef CONFIG_HISI_UDK_MAS_BLK
	i += mas_blk_fill_rwbs(rwbs, mas_op, i);
#endif

	rwbs[i] = '\0';
}

static int udk_bio_wait_end(void *private, int bi_error, unsigned int io_idx)
{
	UNUSED(io_idx);

	struct udk_mutex *mutex = (struct udk_mutex *)private;

	if (bi_error) {
		udk_warn("sync io fail %d\n", bi_error);
	}

	udk_mutex_unlock(mutex);
	return 0;
}

int udk_blk_submit_cmd_sync(struct udk_device *dev, unsigned long bi_rw,
			    uint64_t start_sector, uint64_t nr_sects)
{
	int ret;
	struct udk_mutex bio_lock;
	struct udk_simple_bio_set bio_set;

	udk_mutex_init(&bio_lock);
	udk_mutex_lock(&bio_lock);
	bio_set.req_type = bi_rw;
	bio_set.cb = udk_bio_wait_end;
	bio_set.cb_private = &bio_lock;
	ret = udk_simple_submit_cmd(dev, start_sector, nr_sects, &bio_set);
	if (ret == 0) {
		/*
		 * lock again for waiting the end io has been called
		 * to make sure the I/O has completed.
		 */
		udk_mutex_lock(&bio_lock);
	}
	udk_mutex_unlock(&bio_lock);

	return ret;
}

/* support simple submit FLUSH or DISCARD cmd */
int udk_simple_submit_cmd(struct udk_device *dev, uint64_t start_sector,
			  uint64_t nr_sects, struct udk_simple_bio_set *bio_set)
{
	int ret;
	struct udk_bio *bio = NULL;

	if (dev == NULL || bio_set == NULL) {
		udk_info("invalid parameter\n");
		return -EINVAL;
	}

	if (!udk_disk_check_flag(udkdev_to_disk(dev), UDK_DISK_FLAG_DIRECTMAP)) {
		udk_info("device-%s not support directmap, can not submit async cmd\n", dev->name);
		return -EOPNOTSUPP;
	}

	bio = udk_bio_alloc(0, true);
	if (bio == NULL) {
		return -ENOMEM;
	}

	bio->bi_rw = bio_set->req_type;
	bio->bi_sector = start_sector;
	bio->bi_dev = dev;
	bio->bi_ctx.direct_map_paddr = true;
	bio->bi_ctx.async = true;
	bio->bi_size = (unsigned int)(nr_sects << UDK_BLKSHIFT_DEFAULT);
	set_udk_bio_context(bio, bio_set->cb_private);
	bio->bi_ctx.devhost_end_bio_fn = bio_set->cb;
	ret = udk_blk_submit_bio(bio, 0U);
	if (ret < 0) {
		udk_bio_free(bio);
	}

	return -hmerrno2posix(ret);
}

int udk_blk_submit_bio_sync(struct udk_device *dev, unsigned long bi_rw, uint64_t start_sector,
			    struct udk_bio_pvec *pvec, unsigned int vec_num)
{
	int ret;
	struct udk_mutex bio_lock;
	struct udk_simple_bio_set bio_set;

	if (dev == NULL || pvec == NULL) {
		return -EINVAL;
	}

	udk_mutex_init(&bio_lock);
	udk_mutex_lock(&bio_lock);
	bio_set.req_type = bi_rw;
	bio_set.cb = udk_bio_wait_end;
	bio_set.cb_private = &bio_lock;
	ret = udk_simple_submit_bio(dev, start_sector, pvec, vec_num, &bio_set);
	if (ret == 0) {
		/*
		 * lock again for waiting the end io has been called
		 * to make sure the I/O has completed.
		 */
		udk_mutex_lock(&bio_lock);
	}
	udk_mutex_unlock(&bio_lock);

	return ret;
}

int udk_simple_submit_bio(struct udk_device *dev, uint64_t start_sector,
			  struct udk_bio_pvec *pvec, unsigned int vec_num,
			  struct udk_simple_bio_set *bio_set)
{
	int ret;
	struct udk_bio *bio = NULL;

	if (dev == NULL || pvec == NULL || bio_set == NULL) {
		udk_info("invalid parameter\n");
		return -EINVAL;
	}

	if (!udk_disk_check_flag(udkdev_to_disk(dev), UDK_DISK_FLAG_DIRECTMAP)) {
		udk_info("device-%s not support directmap, can not submit async bio\n", dev->name);
		return -EOPNOTSUPP;
	}

	bio = udk_bio_alloc(vec_num, true);
	if (bio == NULL) {
		return -EINVAL;
	}

	bio->bi_rw = bio_set->req_type;
	bio->bi_sector = start_sector;
	bio->bi_dev = dev;
	bio->bi_ctx.async = true;
	bio->bi_ctx.direct_map_paddr = true;
	bio->bio_io_pvec = pvec;
	bio->bi_vec_num = vec_num;
	bio->bi_size = vec_num * (1 << UDK_PFN_SHIFT);
	set_udk_bio_context(bio, bio_set->cb_private);
	bio->bi_ctx.devhost_end_bio_fn = bio_set->cb;
#ifdef UDK_FUSION
	udk_fusion_pm_rdlock();
#endif
	ret = udk_blk_submit_bio(bio, 0U);
	if (ret < 0) {
		udk_bio_free(bio);
	}
#ifdef UDK_FUSION
	udk_fusion_pm_unlock();
#endif
	return -hmerrno2posix(ret);
}

void udk_disk_set_logical_block_size(struct udk_disk *disk, unsigned int size)
{
	if (disk == NULL) {
		return;
	}
	disk->logical_block_size = size;
}

void udk_disk_set_max_discard_sectors(struct udk_disk *disk,
				      unsigned int sectors)
{
	if (disk == NULL) {
		return;
	}
	disk->max_discard_sectors = sectors;
}

unsigned int udk_disk_obtain_max_discard_sectors(const struct udk_disk *disk)
{
	if (disk == NULL) {
		return 0;
	}
	return disk->max_discard_sectors;
}

void udk_disk_set_max_sectors(struct udk_disk *disk, unsigned int sectors)
{
	if (disk == NULL) {
		return;
	}
	disk->max_sectors = sectors;
}

unsigned int udk_disk_obtain_max_sectors(const struct udk_disk *disk)
{
	if (disk == NULL) {
		return 0;
	}
	return disk->max_sectors;
}
