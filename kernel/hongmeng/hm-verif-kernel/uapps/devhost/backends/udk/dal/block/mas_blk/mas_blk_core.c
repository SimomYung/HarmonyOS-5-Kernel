/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2021. All rights reserved.
 * Description: mas block core framework
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <udk/block/mas_blk/mas_blk_core_interface.h>
#include <udk/block/mas_blk/mas_blk_flush_interface.h>
#include <udk/block/mas_blk/mas_blk_iosched_interface.h>
#ifdef CONFIG_MAS_BLK_NVME
#include <udk/block/mas_blk/mas_blk_iosched_nvme_interface.h>
#endif
#include <udk/block/mas_blk/mas_blk_dump_interface.h>
#include <udk/block/mas_blk/mas_blk_latency_interface.h>
#ifdef CONFIG_MAS_BKOPS
#include <udk/block/mas_blk/mas_bkops_core.h>
#endif
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk.h>
#include <libstrict/strict.h>
#include <udk/mm.h>
#include <intapi/thread/thread_qos.h>

struct blk_dev_lld *mas_blk_get_lld(struct udk_request_queue *q)
{
	if (likely(q->mq_ops))
		return (q->tag_set ? &q->tag_set->lld_func : &q->lld_func);

	return &q->lld_func;
}

struct udk_request_queue *mas_blk_get_queue_by_lld(struct blk_dev_lld *lld)
{
	struct udk_blk_mq_tag_set *tag_set = NULL;
	switch (lld->type) {
	case BLK_LLD_QUEUE_BASE:
		return (struct udk_request_queue *)(lld->data);
	case BLK_LLD_TAGSET_BASE:
		tag_set = (struct udk_blk_mq_tag_set *)(lld->data);
		return dlist_last_entry(
			&tag_set->tag_list, struct udk_request_queue, tag_set_list);
	default:
		mas_blk_rdr_panic("Unknown lld type");
		return NULL;
	}
	return NULL;
}

void mas_blk_bio_clone_fast(
	struct udk_bio *bio, struct udk_bio *bio_src)
{
	bio->mas_bi_opf = bio_src->mas_bi_opf;
}

/*
 * This interface will be called when the split bio will be run
 */
void mas_blk_bio_queue_split(
	struct udk_request_queue *q, struct udk_bio **bio, struct udk_bio *split)
{
	mas_blk_busyidle_check_bio_endio(*bio);
	mas_blk_busyidle_check_bio(q, split);
}

/*
 * This interface will be called after a bio has been merged.
 */
void mas_blk_bio_merge_done(const struct udk_request *req,
	const struct udk_request *next)
{
	mas_blk_latency_for_merge(req, next);
}

/*
 * This interface will be called in blk_account_io_completion().
 */
int mas_blk_account_io_completion(
	const struct udk_request *req, unsigned int bytes)
{
	struct blk_dev_lld *lld = NULL;

	if (!req || !req->bio)
		return -1;

	lld = mas_blk_get_lld(req->queue);
	if (lld) {
		if (udk_bio_req_op(req->bio) == UDK_REQ_DISCARD)
			lld->discard_len += bytes;

		if (udk_bio_req_op(req->bio) == UDK_REQ_WRITE)
			lld->write_len += bytes;
	}

	return 0;
}

static void mas_blk_set_prio(struct udk_bio *bio)
{
	int ret;
	int thread_qos = QOS_LEVEL_INVALID;

	if (bio->bi_rw & (UDK_REQ_FLAGS_META | UDK_REQ_FLAGS_PRIO | UDK_REQ_FLAGS_FG)) {
		bio->mas_bi_opf |= MAS_REQ_FG;
	}

	ret = thread_qos_self_get(&thread_qos);
	if (unlikely(ret != E_HM_OK)) {
		return;
	}

	if (thread_qos > QOS_LEVEL_4) {
		bio->mas_bi_opf |= MAS_REQ_FG;
		if (thread_qos >= QOS_LEVEL_9) {
			bio->mas_bi_opf |= REQ_VIP;
		}
	}
}

/*
 * This interface will be called when a bio is submitted.
 */
bool mas_blk_generic_make_request_check(struct udk_bio *bio)
{
	struct udk_disk *disk = udkdev_to_disk(bio->bi_dev);
	struct udk_request_queue *q = disk->bd_queue;

	if (unlikely(mas_blk_flush_async_dispatch(q, bio))) {
		bio->bi_error = E_HM_OK;
		(void)udk_bio_end(bio);
		return true;
	}
	mas_blk_latency_bio_check(bio, BIO_PROC_STAGE_SUBMIT);
	mas_blk_busyidle_check_bio(q, bio);
	if (udk_blk_queue_flag_is_set(q, UDK_BLK_QUEUE_FLAG_QOS)) {
		mas_blk_set_prio(bio);
	}

	return false;
}

/*
 * This interface will be called when a bio will be passed into io scheduler.
 */
void mas_blk_generic_make_request(const struct udk_bio *bio)
{
	mas_blk_latency_bio_check(
		(struct udk_bio *)bio, BIO_PROC_STAGE_GENERIC_MAKE_REQ);
}

/*
 * This interface will be called when the bio is ended.
 */
void mas_blk_bio_endio(const struct udk_bio *bio)
{
	mas_blk_busyidle_check_bio_endio((struct udk_bio *)bio);
	mas_blk_latency_bio_check((struct udk_bio *)bio, BIO_PROC_STAGE_ENDBIO);
}

/*
 * This interface will be called before the bio will be freed.
 */
void mas_blk_bio_free(const struct udk_bio *bio)
{
	/* io maybe free when submit_bio return fail */
	if (unlikely(bio->mas_bio.io_in_count & MAS_IO_IN_COUNT_SET))
		mas_blk_busyidle_check_bio_endio((struct udk_bio *)bio);
}

/*
 * This interface will be called when a request will be configed according to a
 */
void mas_blk_request_init_from_bio(struct udk_request *req, struct udk_bio *bio)
{
	mas_blk_latency_req_check(req, REQ_PROC_STAGE_INIT_FROM_BIO);
	req->mas_req.requeue_reason = REQ_REQUEUE_IO_NO_REQUEUE;
	bio->mas_bio.io_req = req;

	dlist_init(&req->async_list);
	dlist_init(&req->cnt_list);
}

/*
 * This interface will be called when dm device pass a cloned request to block
 * layer.
 */
void mas_blk_insert_cloned_request(
	const struct udk_request_queue *q, const struct udk_request *rq)
{
	mas_blk_busyidle_check_request_bio(q, rq);
}

/*
 * This interface will be called when request will be inited in MQ.
 */
void mas_blk_mq_rq_ctx_init(
	struct udk_request_queue *q, struct udk_blk_mq_ctx *ctx, struct udk_request *rq)
{
	if (q->mas_queue_ops && q->mas_queue_ops->mq_req_init_fn)
		q->mas_queue_ops->mq_req_init_fn(q, ctx, rq);
}

/*
 * This interface will be called when request will be passed to driver in MQ.
 */
void mas_blk_mq_request_start(const struct udk_request *req)
{
	mas_blk_latency_req_check((struct udk_request *)req, REQ_PROC_STAGE_START);
}

/*
 * This interface will be called when request will be passed to driver.
 */
void mas_blk_request_start(const struct udk_request *req)
{
	mas_blk_latency_req_check((struct udk_request *)req, REQ_PROC_STAGE_START);
}

/*
 * This interface will be called when request will be requeue.
 */
void mas_blk_requeue_request(const struct udk_request *rq)
{
	mas_blk_latency_req_check(
		(struct udk_request *)rq, REQ_PROC_STAGE_SQ_REQUEUE);
}

/*
 * This interface will be called when request has been returned by driver.
 */
void mas_blk_request_update(
	const struct udk_request *req, int error, unsigned int nr_bytes)
{
	struct blk_dev_lld *lld = mas_blk_get_lld(req->queue);

	if (!req->bio)
		return;

	if (udk_bio_req_op(req->bio) == UDK_REQ_DISCARD)
		lld->discard_len += nr_bytes;

	if (udk_bio_req_op(req->bio) == UDK_REQ_WRITE)
		lld->write_len += nr_bytes;

	if (udk_bio_req_op((req->bio)) == UDK_REQ_FLUSH)
		mas_blk_flush_update(req->queue, error);

	mas_blk_latency_req_check((struct udk_request *)req, REQ_PROC_STAGE_UPDATE);
}

/*
 * This interface will be called when request is releaseing in MQ.
 */
void mas_blk_mq_request_free(struct udk_request *rq)
{
	mas_blk_latency_req_check(rq, REQ_PROC_STAGE_FREE);
}

/*
 * This interface will be called when request is releaseing in SQ.
 */
void mas_blk_request_put(struct udk_request *req)
{
	mas_blk_latency_req_check(req, REQ_PROC_STAGE_FREE);
}

static void mas_blk_dev_lld_init(
	struct blk_dev_lld *blk_lld, enum blk_lld_base type, const void *data)
{
#ifdef CONFIG_MAS_DEBUG_FS
	if (blk_lld->init_magic == BLK_LLD_INIT_MAGIC)
		mas_blk_rdr_panic("blk_lld has been inited already!");
#endif
	blk_lld->init_magic = BLK_LLD_INIT_MAGIC;

	dlist_init(&blk_lld->lld_list);
	blk_lld->type = type;
	blk_lld->data = (void *)data;
	if (mas_blk_busyidle_state_init(&blk_lld->blk_idle))
		blk_lld->features &= ~BLK_LLD_BUSYIDLE_SUPPORT;
}

/*
 * This interface will be called after block device partition table check done.
 */
void mas_blk_check_partition_done(struct udk_disk *disk, bool has_part_tbl)
{
	disk->bd_queue->mas_queue.blk_part_tbl_exist = has_part_tbl;
}

/*
 * This interface will be called when init request queue.
 */
void mas_blk_allocated_queue_init(struct udk_request_queue *q)
{
	dlist_init(&q->mas_queue.dump_list);
	q->mas_queue.blk_part_tbl_exist = false;
	mas_blk_dev_lld_init(&q->lld_func, BLK_LLD_QUEUE_BASE, (void *)q);
}

static void mas_blk_queue_init_common(struct udk_request_queue *q)
{
	mas_blk_dump_register_queue(q);
	mas_blk_queue_latency_init(q);
	mas_blk_queue_async_flush_init(q);
#ifdef CONFIG_MAS_BKOPS
	mas_bkops_init(q);
#endif
}

/*
 * This interface will be called when SQ init request queue.
 */
void mas_blk_sq_init_allocated_queue(struct udk_request_queue *q)
{
	mas_blk_queue_init_common(q);
}

/*
 * This interface will be called when MQ init request queue.
 */
void mas_blk_mq_init_allocated_queue(struct udk_request_queue *q)
{
	unsigned int i;
	struct udk_blk_mq_tag_set *set = q->tag_set;

	for (i = 0; i < set->nr_hw_queues; i++) {
		if (!set->tags[i])
			continue;
		set->tags[i]->set = set;
	}

	mas_blk_queue_init_common(q);

	q->mas_queue_ops =
		set->mas_tagset_ops ? set->mas_tagset_ops->queue_ops : NULL;
	if (q->mas_queue_ops && q->mas_queue_ops->mq_iosched_init_fn) {
		int ret = q->mas_queue_ops->mq_iosched_init_fn(q);
		if (unlikely(ret)) {
			udk_error("%s: mq_iosched_init_fn failed. err = %d\n",
				__func__, ret);
			mas_blk_rdr_panic(NO_EXTRA_MSG);
		}
	}
}

/*
 * This interface will be called when MQ start to release request queue.
 */
void mas_blk_mq_free_queue(const struct udk_request_queue *q)
{
	if (q->mas_queue_ops && q->mas_queue_ops->mq_iosched_exit_fn)
		q->mas_queue_ops->mq_iosched_exit_fn(
			(struct udk_request_queue *)q);
}

/*
 * This interface will be called when request queue release.
 */
void mas_blk_cleanup_queue(struct udk_request_queue *q)
{
	mas_blk_dump_unregister_queue(q);
	mas_blk_flush_reduced_queue_unregister(q);
}

/*
 * This interface will be called when MQ tagset init.
 */
void mas_blk_mq_allocated_tagset_init(struct udk_blk_mq_tag_set *set)
{
	mas_blk_dev_lld_init(&set->lld_func, BLK_LLD_TAGSET_BASE, (void *)set);

	/*
	 * MAS IO Scheduler Configuration
	 */
	if (set->lld_func.features & BLK_LLD_IOSCHED_UFS_MQ)
		set->mas_tagset_ops = &mas_ufs_blk_tagset_ops;
#ifdef CONFIG_MAS_BLK_NVME
	else if (set->lld_func.features & BLK_LLD_IOSCHED_NVME_MQ)
		set->mas_tagset_ops = &mas_nvme_blk_tagset_ops;
#endif
	else
		set->mas_tagset_ops = NULL;
}

#ifdef CONFIG_MAS_MQ_USING_CP
static bool mas_blk_cp_flag = false;
bool mas_blk_get_cp_flag(void)
{
	return mas_blk_cp_flag;
}

void mas_blk_set_cp_flag(bool val)
{
	mas_blk_cp_flag = val;
}

int mas_blk_cp_enable(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	mas_blk_set_cp_flag(true);

	return 0;
}
#endif

int mas_queue_status_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize)
{
	struct udk_disk *disk = NULL;
	struct blk_dev_lld *lld = NULL;
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if ((disk == NULL) || (disk->bd_queue == NULL)) {
		return E_HM_INVAL;
	}

	if (pos >= MAS_BLK_SYS_BUF_LEN) {
		*rsize = 0;
		return E_HM_OK;
	}

	buf = mas_malloc_sysfs_buf();
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	lld = mas_blk_get_lld(disk->bd_queue);
	offset = mas_blk_dump_lld_status(lld, buf, (int)MAS_BLK_SYS_BUF_LEN);
	offset += mas_blk_dump_queue_status2(disk->bd_queue, buf + offset,
		(int)(MAS_BLK_SYS_BUF_LEN - offset));

	/* copy output buf to dst addr */
	buf_size = (size <= offset) ? size : offset;
	if (pos >= buf_size) {
		*rsize = 0;
		udk_free(buf);
		return E_HM_OK;
	}

	ret = udk_copy_to_user(dst, buf + pos, buf_size - pos);
	if (ret != E_HM_OK) {
		udk_free(buf);
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}

	udk_free(buf);
	*rsize = buf_size - pos;
	return E_HM_OK;
}

bool mas_blk_support_feature(struct udk_request_queue *q, unsigned int feature)
{
	struct blk_dev_lld *blk_lld = NULL;

	if (q == NULL) {
		return false;
	}

	blk_lld = mas_blk_get_lld(q);
	if (blk_lld != NULL &&
	    (blk_lld->features & feature) != 0) {
		return true;
	}
	return false;
}

/*
 * This interface will be called when request queue bind with block disk
 */
void mas_blk_queue_register(struct udk_request_queue *q, const struct udk_disk *disk)
{
	if (q != NULL) {
		q->mas_queue.queue_disk = (struct udk_disk *)disk;
	}
}
