/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description:  mas block flush reduce core
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <libstrict/strict.h>

#include <udk/log.h>
#include <udk/mutex.h>
#include <udk/delay.h>
#include <udk/block/mas_blk/mas_blk_flush_interface.h>
#include <udk/block/mas_blk/mas_blk.h>

#define MAS_BLK_ASYNC_FLUSH_MAX_RETRIES		5
#define MAS_BLK_ASYNC_FLUSH_DELAY_MS		20U

static DLIST_HEAD(all_flush_q_list);
static struct udk_mutex all_flush_q_lock = UDK_MUTEX_INIT;

static DLIST_HEAD(all_object_list);
static struct uspinlock_s all_object_lock = USPINLOCK_INITIALIZER;

int mas_blk_direct_flush_fn(void *device)
{
	if (device == NULL) {
		return -EINVAL;
	}

	return udk_blk_submit_cmd_sync(device, UDK_REQ_FLUSH, 0ULL, 0ULL);
}

static int __flush_end_io(void *private, int bi_error, unsigned int io_idx)
{
	UNUSED(io_idx);
	if (private != NULL) {
		mas_blk_flush_update((struct udk_request_queue *)private, bi_error);
	}

	return 0;
}

static int __flush_work_fn(struct udk_work *work)
{
	int ret = 0;
	int retry = 0;
	struct udk_disk *queue_disk = NULL;
	struct blk_queue_cust *member = NULL;
	struct udk_request_queue *q = NULL;
	struct udk_simple_bio_set bio_set = {
		.req_type = (UDK_REQ_FLUSH | UDK_REQ_FLAGS_NO_WAIT),
		.cb = __flush_end_io,
	};

	if (work == NULL) {
		return E_HM_INVAL;
	}

	member = container_of(work, struct blk_queue_cust, flush_work_handler);
	q = container_of(member, struct udk_request_queue, mas_queue);
	bio_set.cb_private = q;
	queue_disk = member->queue_disk;
	if (queue_disk) {
		do {
			/* submit async flush io */
			ret = udk_simple_submit_cmd(&queue_disk->part0.udkdev, 0ULL, 0ULL, &bio_set);
			if (ret < 0) {
				udk_mdelay(1);
			}
			retry++;
		} while ((ret < 0) && (retry < MAS_BLK_ASYNC_FLUSH_MAX_RETRIES));
		if (ret < 0) {
			mas_blk_flush_update(q, ret);
			udk_error("%s: blkdev_issue_flush fail, ret=%s!\n", __func__, strerror(-ret));
		}
	}
	udk_work_destroy(work);

	return posix2hmerrno(-ret);
}

void mas_blk_flush_work_fn(unsigned long val)
{
	int ret;
	struct udk_timer *timer = (struct udk_timer *)val;
	struct blk_queue_cust *member = (struct blk_queue_cust *)container_of(timer,
			struct blk_queue_cust, flush_work);
	struct udk_request_queue *q =
				container_of(member, struct udk_request_queue, mas_queue);

	ret = udk_workqueue_add_work_default(&member->flush_work_handler);
	if (ret < 0) {
		udk_warn("mas blk flush add work failed\n");
		mas_blk_flush_update(q, ret);
	}
}

/*
 * This function is used to judge if the bio should be dispatch async or not
 */
bool mas_blk_flush_async_dispatch(struct udk_request_queue *q,
	const struct udk_bio *bio)
{
	struct blk_queue_cust *mas_queue = &q->mas_queue;

	if (unlikely(!mas_queue->flush_optimize))
		return false;

	if (likely(!(udk_bio_req_op(bio) == UDK_REQ_FLUSH) || bio->bi_size))
		goto out;

	if (bio->mas_bio.bi_async_flush || bio->async_flush) {
		udk_timer_mod(&mas_queue->flush_work, MAS_BLK_ASYNC_FLUSH_DELAY_MS);
		return true;
	}

	if (unlikely(!vatomic32_read(&mas_queue->write_after_flush)))
		return true;

out:
	if (udk_bio_req_op(bio) == UDK_REQ_WRITE)
		vatomic32_inc(&mas_queue->write_after_flush);

	return false;
}

void mas_blk_flush_update(struct udk_request_queue *queue, int error)
{
	if (likely(queue && !error))
		vatomic32_init(&queue->mas_queue.write_after_flush, 0);
}

/*
 * This function is used to send the emergency flush before system power down
 */
void blk_power_off_flush(int emergency)
{
	int ret;
	struct udk_disk *queue_disk = NULL;
	struct blk_queue_cust *mas_queue = NULL;
	struct udk_request_queue *q = NULL;
	struct blk_dev_lld *blk_lld = NULL;

	udk_error("%s: emergency = %d\n", __func__, emergency);
	udk_mutex_lock(&all_flush_q_lock);
	dlist_for_each_entry(mas_queue, &all_flush_q_list, struct blk_queue_cust, flush_queue_node) {
		if (!mas_queue->blk_part_tbl_exist)
			continue;
		q = (struct udk_request_queue *)container_of(
			mas_queue, struct udk_request_queue, mas_queue);
		blk_lld = mas_blk_get_lld(q);
		if (blk_lld && blk_lld->flush_fn && mas_queue->queue_disk) {
			queue_disk = mas_queue->queue_disk;
			udk_error("emergency flush on %s\n", queue_disk->name);
			ret = blk_lld->flush_fn((void *)&queue_disk->part0.udkdev);
			if (ret)
				udk_error("emergency flush on %s fail!\n", queue_disk->name);
			else
				udk_error("emergency flush on %s done!\n", queue_disk->name);
		}
	}

	udk_mutex_unlock(&all_flush_q_lock);
	udk_error("%s: done!\n", __func__);
}

void mas_blk_flush_list_register(struct dlist_node *lld_list)
{
	uspinlock_lock(&all_object_lock);
	dlist_insert_tail(&all_object_list, lld_list);
	uspinlock_unlock(&all_object_lock);
}

void mas_blk_flush_list_unregister(struct dlist_node *lld_list)
{
	uspinlock_lock(&all_object_lock);
	dlist_delete(lld_list);
	uspinlock_unlock(&all_object_lock);
}

void mas_blk_panic_flush(void)
{
	struct blk_dev_lld *blk_lld = NULL;

	blk_power_off_flush(0);

	uspinlock_lock(&all_object_lock);
	dlist_for_each_entry(blk_lld, &all_object_list, struct blk_dev_lld, lld_list) {
		if (blk_lld->type != BLK_LLD_TAGSET_BASE) {
			udk_error("<%s> type = %d\n",
				__func__, blk_lld->type);
			continue;
		}
	}
	uspinlock_unlock(&all_object_lock);
}

int mas_blk_poweroff_flush_notifier_call(unsigned int cmd, unsigned long arg)
{
	UNUSED(arg);
	UNUSED(cmd);

	blk_power_off_flush(1);

	return 0;
}

int mas_blk_flush_init(void)
{
	return 0;
}

static void mas_blk_flush_reduced_queue_register(struct udk_request_queue *q)
{
	udk_mutex_lock(&all_flush_q_lock);
	dlist_insert_tail(&all_flush_q_list, &q->mas_queue.flush_queue_node);
	udk_mutex_unlock(&all_flush_q_lock);
}

void mas_blk_flush_reduced_queue_unregister(struct udk_request_queue *q)
{
	struct blk_queue_cust *mas_queue = &q->mas_queue;
	struct dlist_node *flush_queue_node = &mas_queue->flush_queue_node;

	if (mas_queue->flush_optimize) {
		udk_timer_del_sync(&mas_queue->flush_work);
		if (!flush_queue_node->next || !flush_queue_node->prev) {
			mas_blk_rdr_panic("Invalid flush_queue_node!");
			return;
		}
		udk_mutex_lock(&all_flush_q_lock);
		dlist_delete(flush_queue_node);
		udk_mutex_unlock(&all_flush_q_lock);
	}
}

/*
 * This function is used to enable the register the emergency flush interface to
 * the MQ tagset
 */
void blk_mq_tagset_direct_flush_register(
	struct udk_blk_mq_tag_set *tag_set, blk_direct_flush_fn func)
{
	if (!tag_set) {
		udk_error("%s: tag_set is NULL!\n", __func__);
		return;
	}

	tag_set->lld_func.flush_fn = func;
}

void mas_blk_queue_async_flush_init(struct udk_request_queue *q)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);
	struct blk_queue_cust *mas_queue = &q->mas_queue;
	struct dlist_node *flush_queue_node = &mas_queue->flush_queue_node;

	if (!(blk_lld->features & BLK_LLD_FLUSH_REDUCE_SUPPORT))
		return;

	udk_timer_init(&mas_queue->flush_work,
			__cfi_mas_blk_flush_work_fn,
			(unsigned long)(uintptr_t)&mas_queue->flush_work,
			MAS_BLK_ASYNC_FLUSH_DELAY_MS);
	udk_work_init(&mas_queue->flush_work_handler, __flush_work_fn, 1);
	vatomic32_init(&mas_queue->write_after_flush, 0);
	if (!flush_queue_node->next && !flush_queue_node->prev)
		dlist_init(flush_queue_node);

	if (blk_lld->flush_fn && dlist_empty(flush_queue_node))
		mas_blk_flush_reduced_queue_register(q);

	uspinlock_init(&mas_queue->flush_lock);
	mas_queue->flush_optimize = 1;
}

/*
 * This function is used to allow the flush to dispatch async
 */
void blk_flush_set_async(struct udk_bio *bio)
{
	if (bio)
		bio->mas_bio.bi_async_flush = 1;
}

/*
 * This function is used to enable the flush reduce on the MQ tagset
 */
void blk_mq_tagset_flush_reduce_config(
	struct udk_blk_mq_tag_set *tag_set, bool flush_reduce_enable)
{
	if (!tag_set) {
		udk_error("%s: tag_set is NULL!\n", __func__);
		return;
	}

	if (flush_reduce_enable)
		tag_set->lld_func.features |= BLK_LLD_FLUSH_REDUCE_SUPPORT;
	else
		tag_set->lld_func.features &= ~BLK_LLD_FLUSH_REDUCE_SUPPORT;
}

/*
 * This function is for FS module to aware flush reduce function
 */
int blk_flush_async_support(const struct udk_disk *disk)
{
	struct udk_request_queue *q = NULL;

	if (!disk) {
		udk_error("disk is NULL!\n");
		return 0;
	}

	q = disk->bd_queue;
	if (q)
		return q->mas_queue.flush_optimize;
	else
		return 0;
}
