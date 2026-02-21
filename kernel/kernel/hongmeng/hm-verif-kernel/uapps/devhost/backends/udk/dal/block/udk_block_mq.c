/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block multi queue framework
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <hmkernel/sched/sched.h>

#include <inttypes.h>
#include <udk/log.h>
#include <udk/delay.h>
#include <udk/iolib.h>
#include <udk/block/udk_disk.h>
#include <udk/block/udk_block_stats.h>
#include <libhmsync/uspinlock.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libpreempt/preempt.h>

#include <intapi/mem/mcache.h>

#ifdef CONFIG_HISI_UDK_MAS_BLK
#include <basicplatform/udk/block/mas_blk/mas_blk.h>
#include <basicplatform/udk/block/mas_blk/mas_blk_core_interface.h>
#include <basicplatform/udk/block/mas_blk/mas_blk_iosched_interface.h>
#include <basicplatform/udk/block/mas_blk/mas_blk_iosched_mq.h>
#endif

#define UDK_IO_RESERVED_TAG_CNT		8
#define UDK_MAX_IO_QUEUE_DEPTH		1024

#ifdef CONFIG_HISI_UDK_MAS_BLK
#define UDK_MAX_TAGS_BITMAP_ITEM	((((UDK_MAX_IO_QUEUE_DEPTH * MAS_MAX_PRIO_TYPES) - 1) / \
					sizeof(unsigned long long)) + 1)
#else
#define UDK_MAX_TAGS_BITMAP_ITEM	(((UDK_MAX_IO_QUEUE_DEPTH - 1) / \
					sizeof(unsigned long long)) + 1)
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

DEFINE_MCACHE_STATIC(udk_request_queue, 60, sizeof(struct udk_request_queue));
static unsigned int cpu_num = 0;

static unsigned int
raw_bitmap_find_next_one_with_start(const unsigned long long *bitmap,
				    unsigned int sz, unsigned int _start)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz), off;
	unsigned int bit_off;
	unsigned long long tmp;
	unsigned int start = _start;

	if (start >= sz) {
		return sz;
	}
	off = start / BITS_PER_LONGLONG;
	bit_off = start & (BITS_PER_LONGLONG - 1U);
	tmp = bitmap[off] & ~((1ULL << bit_off) - 1U);
	if (tmp != 0ULL) {
		bit_off = (unsigned int)__builtin_ffsll((long long)tmp) - 1;
		goto found;
	}

	for (off = off + 1U; off < long_sz; off++) {
		if (bitmap[off] == 0ULL) {
			continue;
		}
		bit_off = (unsigned int)__builtin_ffsll((long long)bitmap[off]) - 1;
		break;
	}

found:
	bit_off += BITS_PER_LONGLONG * off;
	return bit_off >= sz ? sz : bit_off;
}

#define bitmap_foreach_set_bit(bit, off, end, addr, size)                      \
	for ((bit) = raw_bitmap_find_next_one_with_start(addr, size, off);     \
	     (bit) < (end); (bit) = raw_bitmap_find_next_one(addr, size, bit))

typedef bool (*bitmap_for_each_fn)(unsigned long long *bitmap, unsigned int,
				   void *);

static void mq_bitmap_foreach_set_bit(unsigned long long *bitmap,
				      unsigned int sz, unsigned int _start,
				      bitmap_for_each_fn fn, void *data)
{
	unsigned int idx;

	bitmap_foreach_set_bit(idx, _start, sz, bitmap, sz) {
		if (!fn(bitmap, idx, data)) {
			return;
		}
	}

	if (_start != 0) {
		bitmap_foreach_set_bit(idx, 0, _start, bitmap, sz) {
			if (!fn(bitmap, idx, data)) {
				return;
			}
		}
	}

	return;
}

static int udk_blk_mq_get_tag(struct udk_blk_mq_tags *tags,
			      unsigned int flags)
{
	unsigned int tag;
	unsigned int offset = 0;

	if ((flags & UDK_REQ_OP_MASK) == UDK_REQ_WRITE) {
		offset = UDK_IO_RESERVED_TAG_CNT;
	}

	uspinlock_lock(&tags->lock);
	tag = raw_bitmap_find_next_zero(tags->bitmap_tags, tags->nr_tags,
					offset - 1);
	if (tag >= tags->nr_tags) {
		uspinlock_unlock(&tags->lock);
		return UDK_BLK_MQ_NO_TAG;
	}
	raw_bitmap_set_bit(tags->bitmap_tags, tags->nr_tags, tag);
	uspinlock_unlock(&tags->lock);

	return tag;
}

void udk_blk_mq_put_tag(struct udk_blk_mq_tags *tags, unsigned int tag)
{
	uspinlock_lock(&tags->lock);
	raw_bitmap_clear_bit(tags->bitmap_tags, tags->nr_tags, tag);
	uspinlock_unlock(&tags->lock);

	return;
}

#ifndef CONFIG_HISI_UDK_MAS_BLK
static void udk_blk_mq_init_bio_request(struct udk_request *req,
					struct udk_bio *bio)
#else
void udk_blk_mq_init_bio_request(struct udk_request *req,
					struct udk_bio *bio)
#endif
{
	req->sector = bio->bi_sector;
	req->data_len = bio->bi_size;
	req->bio = req->biotail = bio;
	req->ioprio = bio->bi_ioprio;
	req->pvec_num = bio->bi_vec_num;
	req->bio_num = 1;
	req->disk = udkdev_to_disk(bio->bi_dev);

#ifdef CONFIG_HISI_UDK_MAS_BLK
	req->mas_cmd_flags = bio->mas_bi_opf;
	mas_blk_request_init_from_bio(req, bio);
#endif
}

static inline struct udk_blk_mq_tags *
udk_blk_mq_hctx_tags(struct udk_blk_mq_hw_ctx *hctx, uint32_t req_flags)

{
	if (!(req_flags & UDK_RQF_SCHED)) {
		return hctx->tags;
	} else {
		return hctx->sched_tags;
	}
}

#define MQ_TAG_WAIT_TIME_US 100U
#define MQ_TAG_MAX_WAIT_TIME_US 10000U
struct udk_request *
udk_blk_mq_alloc_request(struct udk_request_queue *queue, struct udk_bio *bio)
{
	struct udk_blk_mq_ctx *ctx = NULL;
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	struct udk_blk_mq_tags *tags = NULL;
	struct udk_request *req = NULL;
	uint32_t req_flags = 0;
	uint32_t cmd_flags = bio->bi_rw;
	uint32_t wait_utime = MQ_TAG_WAIT_TIME_US;
	int tag, cpu;

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_latency_bio_check(bio, BIO_PROC_STAGE_TAG_START);
#endif

retry:
	cpu = preempt_process_cpu_get();
	ctx = udk_blk_mq_get_ctx(queue, cpu);
	if ((queue->mq_ops) && (queue->mq_ops->update_hctx)) {
		hctx = queue->mq_ops->update_hctx(queue, ctx->hctx, cpu, cmd_flags);
	} else {
		hctx = ctx->hctx;
	}
	tags = udk_blk_mq_hctx_tags(hctx, req_flags);
#ifdef CONFIG_HISI_UDK_MAS_BLK
	if (queue->mas_queue_ops && queue->mas_queue_ops->mq_tag_get_fn)
		tag = queue->mas_queue_ops->mq_tag_get_fn(tags, bio, queue);
	else
		tag = udk_blk_mq_get_tag(tags, cmd_flags);
#else
	tag = udk_blk_mq_get_tag(tags, cmd_flags);
#endif
	if (tag == UDK_BLK_MQ_NO_TAG) {
		if (udk_req_is_no_wait(cmd_flags)) {
#ifdef CONFIG_HISI_UDK_MAS_BLK
			mas_blk_bio_endio(bio);
#endif
			return NULL;
		}
		udk_blk_mq_queue_exit(queue);
		if (queue->mq_ops->polling) {
			queue->mq_ops->polling(queue, 0);
		}
		if (!(cmd_flags & UDK_REQ_FLAGS_DIO)) {
			hm_usleep(wait_utime);
		}
		wait_utime = min(wait_utime * 2U, MQ_TAG_MAX_WAIT_TIME_US);
		udk_blk_mq_queue_enter(queue);
		goto retry;
	}

	req = tags->static_reqs[tag];
	mem_zero_s(*req);
	req->queue = queue;
	req->mq_ctx = ctx;
	req->mq_hctx = hctx;
	req->cmd_flags = cmd_flags;
	req->req_flags = queue->scheduler == NULL ? 0 : UDK_RQF_SCHED;
	req->tag = tag;
	dlist_init(&req->queuelist);

	if (req->req_flags & UDK_RQF_SCHED) {
		struct udk_io_scheduler_queue *sched = queue->scheduler;
		RB_CLEAR_NODE(&req->rbt_node);
		if (!udk_req_is_flush(cmd_flags) &&
		    sched->type->ops.prepare_request) {
			sched->type->ops.prepare_request(req);
			req->req_flags |= UDK_RQF_SCHEDPRIV;
		}
	}

	if (udk_req_is_flush(req->cmd_flags)) {
		req->req_flags |= UDK_RQF_FLUSH_SEQ;
	}

	udk_blk_mq_init_bio_request(req, bio);

	return req;
}

static void udk_blk_mq_hctx_mark_pending(struct udk_blk_mq_hw_ctx *hctx,
					 struct udk_blk_mq_ctx *ctx)
{
	const int bit = ctx->idx_hctx;

	uspinlock_lock(&hctx->ctxmap_lock);
	if (!raw_bitmap_test_bit(hctx->ctx_map, hctx->nr_ctx, bit))
		raw_bitmap_set_bit(hctx->ctx_map, hctx->nr_ctx, bit);
	uspinlock_unlock(&hctx->ctxmap_lock);
}

static void udk_blk_mq_insert_request(struct udk_blk_mq_hw_ctx *hctx,
				      struct udk_request *req)
{
	struct udk_blk_mq_ctx *ctx = req->mq_ctx;

	uspinlock_lock(&ctx->lock);
	dlist_insert_tail(&ctx->req_list, &req->queuelist);
	uspinlock_unlock(&ctx->lock);
	vatomic32_inc(&req->queue->queue_depth);
	udk_blk_mq_hctx_mark_pending(hctx, ctx);
}

void udk_blk_mq_free_request(struct udk_request *req)
{
	struct udk_request_queue *queue = req->queue;
	struct udk_blk_mq_hw_ctx *hctx = req->mq_hctx;
	unsigned int tag = (unsigned int)req->tag;

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_request_put(req);
#endif

	if ((req->req_flags & UDK_RQF_SCHEDPRIV) &&
	    queue->scheduler->type->ops.finish_request)
		queue->scheduler->type->ops.finish_request(req);

	if (req->tag != UDK_BLK_MQ_NO_TAG) {
		req->tag = UDK_BLK_MQ_NO_TAG;
#ifdef CONFIG_HISI_UDK_MAS_BLK
		if (req->queue && req->queue->mas_queue_ops &&
		    req->queue->mas_queue_ops->mq_tag_put_fn)
			req->queue->mas_queue_ops->mq_tag_put_fn(
				hctx->tags, tag, req->queue);
		else
			udk_blk_mq_put_tag(hctx->tags, tag);
#else
		udk_blk_mq_put_tag(hctx->tags, tag);
#endif
	}
}

static void udk_request_bio_endio(struct udk_request *req, struct udk_bio *bio,
				  unsigned int bytes, unsigned int error)
{
	struct udk_bio_context *bi_ctx = NULL;
	int ret;

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_bio_endio(bio);
#endif
	bio->bi_error = error;
	bio->bi_sector += bytes >> SECTOR_SHIFT;

	if (bytes >= bio->bi_size) {
		bio->bi_size = 0;
	} else {
		bio->bi_size -= bytes;
	}

	if (bio->bi_size == 0) {
		bi_ctx = &bio->bi_ctx;
		if ((bi_ctx->devhost_end_bio_fn != NULL) &&
		    bi_ctx->direct_map_paddr) {
			ret = bi_ctx->devhost_end_bio_fn(bi_ctx->block_ctx,
							 bio->bi_error,
							 bi_ctx->bio_idx);
			if (ret < 0) {
				udk_error("udk bio end io err=%s\n",
					  strerror(-ret));
			}
		}
		udk_bio_free(bio);
	}
}

bool udk_blk_mq_update_request(struct udk_request *req, int error,
			       unsigned int bytes)
{
	struct udk_bio *bio = NULL;
	unsigned int bio_bytes = 0;
	unsigned int total_bytes = 0;
	unsigned int nr_bytes = bytes;

	if (req->bio == NULL) {
		return false;
	}

	while (req->bio != NULL) {
		bio = req->bio;
		bio_bytes = min(bio->bi_size, nr_bytes);

		if (bio_bytes == bio->bi_size) {
			req->bio = bio->next;
		}

		udk_request_bio_endio(req, bio, bio_bytes, error);
		total_bytes += bio_bytes;
		nr_bytes -= bio_bytes;

		if (nr_bytes == 0) {
			break;
		}
	}

	if (req->bio == NULL) {
		req->data_len = 0;
		return false;
	}

	req->data_len -= total_bytes;

	return true;
}

void udk_blk_mq_end_request(struct udk_request *req, int error)
{
	if (udk_blk_mq_update_request(req, error, udk_req_bytes(req))) {
		udk_error("updated request still remain data\n");
	}

	udk_blk_mq_free_request(req);
}

static unsigned int udk_blk_mq_dispatch_req_list(struct udk_blk_mq_hw_ctx *hctx,
						 struct dlist_node *list)
{
	struct udk_request_queue *queue = hctx->queue;
	struct udk_request *req = NULL;
	int queued;
	int ret = 0;

	if (dlist_empty(list)) {
		return 0;
	}

	queued = 0;
	do {
		req = dlist_first_entry(list, struct udk_request, queuelist);
		dlist_delete(&req->queuelist);
#ifdef CONFIG_HISI_UDK_MAS_BLK
		ret = udk_mas_mq_make_async_request(hctx, req);
#else
		ret = queue->mq_ops->queue_req(hctx, req);
#endif
		if (ret == 0) {
			queued++;
		} else if ((ret == -EBUSY) && !udk_req_is_no_wait(req->cmd_flags)) {
			bool dio = req->cmd_flags & UDK_REQ_FLAGS_DIO;
			dlist_insert(list, &req->queuelist);
			udk_blk_mq_queue_exit(queue);
			if (queue->mq_ops->polling) {
				queue->mq_ops->polling(queue, 0);
			}
			if (!dio) {
				udk_mdelay(1);
			}
			udk_blk_mq_queue_enter(queue);
		} else {
			udk_error("blk mq queue request error=%s!\n",
				  strerror(-ret));
			udk_blk_mq_end_request(req, ret);
		}
	} while (!dlist_empty(list));

	return queued;
}

static int udk_blk_mq_sched_dispatch(struct udk_blk_mq_hw_ctx *hctx)
{
	struct udk_request_queue *queue = hctx->queue;
	struct udk_io_scheduler_queue *sched = queue->scheduler;
	struct udk_request *req = NULL;
	unsigned int dispatched = 0;
	unsigned int max_dispatch;
	unsigned int count = 0;
	DLIST_HEAD(req_list);

	max_dispatch = hctx->queue->nr_requests;
	do {
		if (sched->type->ops.has_work &&
		    !sched->type->ops.has_work(hctx))
			break;
		req = sched->type->ops.dispatch_request(hctx);
		if (req == NULL) {
			break;
		}
		dlist_insert_tail(&req->queuelist, &req_list);
		count++;
	} while (count < max_dispatch);

	if (count > 0) {
		dispatched = udk_blk_mq_dispatch_req_list(hctx, &req_list);
		if (count != dispatched) {
			return -EIO;
		}
	}

	return 0;
}

struct dispatch_req_data {
	struct udk_blk_mq_hw_ctx *hctx;
	struct udk_request *req;
};

static bool dispatch_req_from_ctx(unsigned long long *sb, unsigned int bitnr,
				  void *data)
{
	struct dispatch_req_data *dispatch_data = data;
	struct udk_blk_mq_hw_ctx *hctx = dispatch_data->hctx;
	struct udk_blk_mq_ctx *ctx = hctx->ctxs[bitnr];

	uspinlock_lock(&ctx->lock);
	if (!dlist_empty(&ctx->req_list)) {
		dispatch_data->req = list_entry_req(ctx->req_list.next);
		dlist_delete(&dispatch_data->req->queuelist);
		dlist_init(&dispatch_data->req->queuelist);
		if (dlist_empty(&ctx->req_list)) {
			raw_bitmap_clear_bit(sb, hctx->nr_ctx, bitnr);
		}
	}
	uspinlock_unlock(&ctx->lock);

	return !dispatch_data->req;
}

static struct udk_request *
udk_blk_mq_dequeue_from_ctx(struct udk_blk_mq_hw_ctx *hctx,
			    struct udk_blk_mq_ctx *start)
{
	unsigned off = start ? start->idx_hctx : 0;
	struct dispatch_req_data data = {
		.hctx = hctx,
		.req = NULL,
	};

	uspinlock_lock(&hctx->ctxmap_lock);
	mq_bitmap_foreach_set_bit(hctx->ctx_map, hctx->nr_ctx, off,
				  dispatch_req_from_ctx, &data);
	uspinlock_unlock(&hctx->ctxmap_lock);

	return data.req;
}

static struct udk_blk_mq_ctx *
udk_blk_mq_next_ctx(struct udk_blk_mq_hw_ctx *hctx, struct udk_blk_mq_ctx *ctx)
{
	unsigned short idx = ctx->idx_hctx;

	if (++idx == hctx->nr_ctx)
		idx = 0;

	return hctx->ctxs[idx];
}

#define MQ_REQ_WATERLINE 8
static int udk_blk_mq_ctx_dispatch(struct udk_blk_mq_hw_ctx *hctx, bool remain)
{
	struct udk_request_queue *queue = hctx->queue;
	struct udk_blk_mq_ctx *ctx = hctx->dispatch_from;
	unsigned int budget = 0;
	unsigned int count = 0;
	unsigned int dispatched = 0;
	unsigned int budget_lowline = 0;
	unsigned int flush_reqs = 0;
	struct udk_request *req = NULL;
	DLIST_HEAD(req_list);
	int ret = 0;

	if (remain && (queue->tag_set->flags & UDK_BLK_MQ_SHOULD_MERGE)) {
		unsigned int queue_depth = vatomic32_read(&queue->queue_depth);
		budget_lowline = queue->mq_ops->get_budget(queue);
		flush_reqs = queue_depth > MQ_REQ_WATERLINE ?
				     queue_depth - MQ_REQ_WATERLINE : 0;
		if (budget_lowline > 0) {
			budget = max(budget_lowline, flush_reqs);
		} else {
			budget = flush_reqs;
		}
	} else {
		budget = queue->nr_requests;
	}

	while (budget) {
		req = udk_blk_mq_dequeue_from_ctx(hctx, ctx);
		if (req == NULL) {
			goto out;
		}
		vatomic32_dec(&queue->queue_depth);
		dlist_insert_tail(&req_list, &req->queuelist);
		ctx = udk_blk_mq_next_ctx(hctx, req->mq_ctx);
		budget--;
		count++;
	}

out:
	if (count > 0) {
		dispatched = udk_blk_mq_dispatch_req_list(hctx, &req_list);
		if (count != dispatched) {
			ret = -EIO;
		}
		hctx->dispatch_from = ctx;
	}

	return ret;
}

static int udk_blk_mq_dispatch_requests(struct udk_blk_mq_hw_ctx *hctx)
{
	struct udk_request_queue *queue = hctx->queue;
	const bool has_sched = queue->scheduler;
	int ret = 0;

	if (has_sched) {
		ret = udk_blk_mq_sched_dispatch(hctx);
	} else {
		ret = udk_blk_mq_ctx_dispatch(hctx, true);
	}

	return ret;
}

int udk_blk_mq_sched_request(struct udk_request *req)
{
	struct udk_request_queue *queue = req->queue;
	struct udk_io_scheduler_queue *sched = queue->scheduler;
	struct udk_blk_mq_hw_ctx *hctx = req->mq_hctx;
	DLIST_HEAD(list);
	int ret;

	if (sched) {
		dlist_insert(&list, &req->queuelist);
		sched->type->ops.insert_requests(hctx, &list, false);
	} else {
		if (!(queue->tag_set->flags & UDK_BLK_MQ_SHOULD_MERGE)) {
			dlist_insert(&list, &req->queuelist);
			(void)udk_blk_mq_dispatch_req_list(hctx, &list);
			return 0;
		} else {
			udk_blk_mq_insert_request(hctx, req);
		}
	}

	ret = udk_blk_mq_dispatch_requests(hctx);
	if (ret < 0) {
		udk_error("attempt to dispatch request failed, error=%s\n",
			  strerror(-ret));
	}

	return 0;
}

int udk_blk_mq_submit_bio(struct udk_bio *bio)
{
	struct udk_disk *disk = udkdev_to_disk(bio->bi_dev);
	struct udk_request_queue *queue = disk->bd_queue;
	struct udk_request *req = NULL;

#ifdef CONFIG_HISI_UDK_MAS_BLK
	if (unlikely(mas_blk_generic_make_request_check(bio)))
		return 0;
	udk_mas_mq_bio_is_sync(queue, bio);
#endif

	if (udk_blk_mq_try_merge_bio(queue, bio)) {
#ifdef CONFIG_HISI_UDK_MAS_BLK
		mas_blk_latency_bio_check(bio, BIO_PROC_STAGE_MERGE);
#endif
		return 0;
	}

	req = udk_blk_mq_alloc_request(queue, bio);
	if (req == NULL) {
		return -ENOMEM;
	}

#ifdef CONFIG_HISI_UDK_MAS_BLK
	if (req->mas_cmd_flags & MAS_REQ_SYNC)
		return udk_mas_mq_sched_request(req, queue);
#endif

	return udk_blk_mq_sched_request(req);
}

static void udk_blk_mq_map_queues(struct udk_blk_mq_queue_map *qmap)
{
	unsigned int queue;
	unsigned cpu = 0;
	unsigned queue_num = qmap->nr_queues;

	if (cpu_num >= queue_num) {
		for (queue = 0; queue < queue_num; queue++) {
			for (; cpu < (queue + 1) * (cpu_num / queue_num); cpu++)
				qmap->mq_map[cpu] = queue;
		}
	} else {
		unsigned queue_percpu = queue_num / cpu_num;
		for (cpu = 0; cpu < cpu_num; cpu++) {
			qmap->mq_map[cpu] = cpu * queue_percpu;
		}
	}
}

static void udk_blk_mq_update_queue_map(struct udk_blk_mq_tag_set *set)
{
	if ((set->ops) && (set->ops->map_queues)) {
		set->ops->map_queues(set, cpu_num);
	} else {
		udk_blk_mq_map_queues(&set->map);
	}
}

#ifndef CONFIG_HISI_UDK_MAS_BLK
static void udk_blk_mq_free_tags(struct udk_blk_mq_tags *tags)
#else
void udk_blk_mq_free_tags(struct udk_blk_mq_tags *tags)
#endif
{
	unsigned int i;

	for (i = 0; i < tags->nr_tags; i++) {
#ifdef UDK_FUSION
		sysmem_pfree((uintptr_t)tags->static_reqs[i]);
#else
		free(tags->static_reqs[i]);
#endif
	}

	if (tags->static_reqs != NULL) {
		free(tags->static_reqs);
	}

	if (tags->bitmap_tags != NULL) {
		free(tags->bitmap_tags);
	}

	free(tags);
}

#ifndef CONFIG_HISI_UDK_MAS_BLK
static struct udk_blk_mq_tags *udk_blk_mq_alloc_tags(unsigned int nr_tags)
#else
struct udk_blk_mq_tags *udk_blk_mq_alloc_tags(unsigned int nr_tags)
#endif
{
	struct udk_blk_mq_tags *tags = NULL;
	int i;

	tags = malloc(sizeof(struct udk_blk_mq_tags));
	if (tags == NULL) {
		return NULL;
	}

	mem_zero_s(*tags);
	tags->bitmap_tags =
		calloc(BITS_TO_LONGLONGS(nr_tags), sizeof(unsigned long long));
	if (tags->bitmap_tags == NULL) {
		free(tags);
		return NULL;
	}

	tags->nr_tags = nr_tags;
	uspinlock_init(&tags->lock);
	tags->static_reqs = malloc(nr_tags * sizeof(struct udk_request *));
	if (tags->static_reqs == NULL) {
		free(tags->bitmap_tags);
		free(tags);
		return NULL;
	}
	for (i = 0; i < (int)nr_tags; i++) {
#ifdef UDK_FUSION
		tags->static_reqs[i] = (struct udk_request *)sysmem_palloc(UDK_BLK_STATIC_REQUEST_SIZE);
#else
		tags->static_reqs[i] = malloc(UDK_BLK_STATIC_REQUEST_SIZE);
#endif
		if (tags->static_reqs[i] == NULL) {
			goto out;
		}
	}

	return tags;
out:
	for (i--; i >= 0; i--) {
#ifdef UDK_FUSION
		sysmem_pfree((uintptr_t)tags->static_reqs[i]);
#else
		free(tags->static_reqs[i]);
#endif
	}
	free(tags->static_reqs);
	free(tags->bitmap_tags);
	free(tags);
	return NULL;
}

static int __blk_mq_alloc_tags(struct udk_blk_mq_tag_set *set, int hctx_idx)
{
	if (udk_blk_mq_is_shared_tags(set->flags)) {
		set->tags[hctx_idx] = set->shared_tags;
		return 0;
	}

#ifdef CONFIG_HISI_UDK_MAS_BLK
	if (set->mas_tagset_ops && set->mas_tagset_ops->tagset_init_tags_fn)
		set->tags[hctx_idx] = set->mas_tagset_ops->tagset_init_tags_fn(set);
	else
		set->tags[hctx_idx] = udk_blk_mq_alloc_tags(set->queue_depth);
#else
	set->tags[hctx_idx] = udk_blk_mq_alloc_tags(set->queue_depth);
#endif
	if (set->tags[hctx_idx] == NULL) {
		return -ENOMEM;
	}

	return 0;
}

static int udk_blk_mq_alloc_set_tags(struct udk_blk_mq_tag_set *set)
{
	int i;

	if (udk_blk_mq_is_shared_tags(set->flags)) {
		set->shared_tags = udk_blk_mq_alloc_tags(set->queue_depth);
		if (set->shared_tags == NULL) {
			return -ENOMEM;
		}
	}

	for (i = 0; i < (int)set->nr_hw_queues; i++) {
		if (__blk_mq_alloc_tags(set, i) < 0) {
			goto out;
		}
	}

	return 0;

out:
	while (--i >= 0) {
		udk_blk_mq_free_tags(set->tags[i]);
		set->tags[i] = NULL;
	}

	if (udk_blk_mq_is_shared_tags(set->flags)) {
		udk_blk_mq_free_tags(set->shared_tags);
	}

	return -ENOMEM;
}

static unsigned long udk_mq_init_cpu_num(void)
{
	int ret;
	unsigned long config_cpu_num;

	ret = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, &config_cpu_num,
				   sizeof(config_cpu_num));
	if (ret < 0) {
		udk_warn("get config cpu nr failed\n");
		return -hmerrno2posix(ret);
	}

	return config_cpu_num;
}

int udk_blk_mq_alloc_tag_set(struct udk_blk_mq_tag_set *set)
{
	int ret;

	if (!set->nr_hw_queues || !set->queue_depth) {
		return -EINVAL;
	}

	set->queue_depth = min(set->queue_depth, UDK_MAX_IO_QUEUE_DEPTH);
#ifdef CONFIG_HISI_UDK_MAS_BLK
	udk_mas_tag_tags_init(set);
#endif

	if (cpu_num == 0) {
		cpu_num = (unsigned int)udk_mq_init_cpu_num();
	}

	set->tags = calloc(set->nr_hw_queues, sizeof(struct udk_blk_mq_tags *));
	if (set->tags == NULL) {
		return -ENOMEM;
	}

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_mq_allocated_tagset_init(set);
#endif

	set->map.mq_map = calloc(cpu_num, sizeof(unsigned int));
	if (set->map.mq_map == NULL) {
		free(set->tags);
		set->tags = NULL;
		return -ENOMEM;
	}
	set->map.nr_queues = set->nr_hw_queues;

	udk_blk_mq_update_queue_map(set);
	ret = udk_blk_mq_alloc_set_tags(set);
	if (ret < 0) {
		free(set->map.mq_map);
		free(set->tags);
		set->map.mq_map = NULL;
		set->tags = NULL;
		return ret;
	}

	return 0;
}

static struct udk_request_queue *udk_blk_mq_alloc_request_queue(void)
{
	struct udk_request_queue *queue = NULL;

	queue = (struct udk_request_queue *)mcache_udk_request_queue_malloc();
	if (queue == NULL) {
		return NULL;
	}
	mem_zero_s(*queue);
	udk_blk_queue_flag_set(queue, UDK_BLK_QUEUE_FLAG_QOS);

	return queue;
}

static int udk_blk_mq_alloc_ctxs(struct udk_request_queue *queue)
{
	struct udk_blk_mq_ctx *queue_ctx = NULL;
	unsigned int i;

	queue_ctx = calloc(cpu_num, sizeof(struct udk_blk_mq_ctx));
	if (queue_ctx == NULL) {
		return -ENOMEM;
	}

	queue->queue_ctx = queue_ctx;
	for (i = 0; i < cpu_num; i++) {
		struct udk_blk_mq_ctx *ctx = queue->queue_ctx + i;
		ctx->cpu = i;
		uspinlock_init(&ctx->lock);
		dlist_init(&ctx->req_list);
		ctx->queue = queue;
	}

	return 0;
}

static struct udk_blk_mq_hw_ctx *
udk_blk_mq_alloc_hctx(struct udk_request_queue *queue,
		      struct udk_blk_mq_tag_set *set)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;

	hctx = malloc(sizeof(struct udk_blk_mq_hw_ctx));
	if (hctx == NULL) {
		return NULL;
	}
	mem_zero_s(*hctx);
	hctx->queue = queue;
	hctx->flags = set->flags;
	uspinlock_init(&hctx->ctxmap_lock);

	hctx->ctxs = calloc(cpu_num, sizeof(void *));
	if (hctx->ctxs == NULL) {
		free(hctx);
		return NULL;
	}

	hctx->ctx_map =
		calloc(BITS_TO_LONGLONGS(cpu_num), sizeof(unsigned long long));
	if (hctx->ctxs == NULL) {
		free(hctx->ctxs);
		free(hctx);
		return NULL;
	}

	return hctx;
}

static int udk_blk_mq_alloc_and_init_hctx(struct udk_blk_mq_tag_set *set,
					  struct udk_request_queue *queue,
					  int hctx_idx)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;

	hctx = udk_blk_mq_alloc_hctx(queue, set);
	if (hctx == NULL) {
		return -ENOMEM;
	}

	hctx->tags = set->tags[hctx_idx];
	queue->hctx_table[hctx_idx] = hctx;

	if ((set->ops != NULL) && (set->ops->init_hctx != NULL))
		(void)set->ops->init_hctx(hctx, set->driver_data, hctx_idx);

	return 0;
}

static int udk_blk_mq_alloc_hctxs(struct udk_blk_mq_tag_set *set,
				  struct udk_request_queue *queue)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	unsigned long i;

	queue->hctx_table =
		calloc(set->nr_hw_queues, sizeof(struct udk_blk_mq_hw_ctx *));
	if (queue->hctx_table == NULL) {
		return -ENOMEM;
	}
	for (i = 0; i < set->nr_hw_queues; i++) {
		if (udk_blk_mq_alloc_and_init_hctx(set, queue, i) < 0) {
			goto out;
		}
	}
	queue->nr_hw_queues = set->nr_hw_queues;

	return 0;

out:
	for (i = 0; i < set->nr_hw_queues; i++) {
		hctx = queue->hctx_table[i];
		if (hctx != NULL) {
			free(hctx->ctx_map);
			free(hctx->ctxs);
			free(hctx);
			queue->hctx_table[i] = NULL;
		}
	}
	free(queue->hctx_table);
	queue->hctx_table = NULL;

	return -ENOMEM;
}

static inline struct udk_blk_mq_hw_ctx *
udk_blk_mq_map_queue_type(struct udk_request_queue *queue, unsigned int cpu)
{
	return queue->hctx_table[queue->tag_set->map.mq_map[cpu]];
}

static void udk_blk_mq_map_ctxs(struct udk_request_queue *queue)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	struct udk_blk_mq_ctx *ctx = NULL;
	unsigned long i;

	for (i = 0; i < cpu_num; i++) {
		ctx = queue->queue_ctx + i;
		hctx = udk_blk_mq_map_queue_type(queue, i);
		ctx->hctx = hctx;
		ctx->idx_hctx = hctx->nr_ctx;
		hctx->ctxs[hctx->nr_ctx++] = ctx;
	}
}


struct bt_tags_iter_data {
	struct udk_blk_mq_tags *tags;
	busy_tag_iter_fn *fn;
	void *priv;
};

static bool udk_blk_mq_tag_iter_fn(unsigned long long *sb, unsigned int tag,
				     void *data)
{
	struct bt_tags_iter_data *iter_data = data;
	struct udk_blk_mq_tags *tags = iter_data->tags;
	struct udk_request *req;

	req = tags->static_reqs[tag];

	return iter_data->fn(req, iter_data->priv);
}

static void __blk_mq_all_tag_iter(struct udk_blk_mq_tags *tags,
		busy_tag_iter_fn *fn, void *priv)
{
	struct bt_tags_iter_data iter_data = {
		.tags = tags,
		.fn = fn,
		.priv = priv,
	};
	uspinlock_lock(&tags->lock);
	mq_bitmap_foreach_set_bit(tags->bitmap_tags, tags->nr_tags, 0, udk_blk_mq_tag_iter_fn, &iter_data);
	uspinlock_unlock(&tags->lock);
}

void blk_mq_tagset_busy_iter(struct udk_blk_mq_tag_set *tagset,
		busy_tag_iter_fn *fn, void *priv)
{
	unsigned int flags = tagset->flags;
	int i;

	for (i = 0; i < tagset->nr_hw_queues; i++) {
		if (tagset->tags && tagset->tags[i]) {
			__blk_mq_all_tag_iter(tagset->tags[i], fn, priv);
		}
	}
}

struct blk_expired_data {
	struct udk_blk_mq_hw_ctx *hctx;
	bool has_timedout_rq;
	bool next;
	struct timespec timeout_start;
	struct timespec timeout_next;
};

static long timespec_sub(struct timespec *a, struct timespec *b)
{
	long sec = a->tv_sec - b->tv_sec;
	long nsec = a->tv_nsec;

	if (a->tv_nsec < b->tv_nsec) {
		nsec += NSEC_PER_SEC;
		sec--;
	}
	nsec -= b->tv_nsec;

	return (sec * MSEC_PER_SEC + nsec / NSEC_PER_MSEC);
}

static void timespec_add_ms(struct timespec *tm, unsigned int add)
{
	long long nsec;

	nsec = (long long)tm->tv_nsec + ((long long)(unsigned long long)add * NSEC_PER_MSEC);
	tm->tv_sec = tm->tv_sec + (long)(nsec / NSEC_PER_SEC);
	tm->tv_nsec = (long)(nsec % NSEC_PER_SEC);
}

static bool udk_blk_req_expired(struct udk_request *req, struct blk_expired_data *d)
{
	struct timespec deadline;

	if (req->state != UDK_MQ_RQ_IN_FLIGHT) {
		return false;
	}
	smp_rmb();

	if (req->req_flags & UDK_RQF_TIMED_OUT) {
		return false;
	}

	deadline = req->deadline;
	if (timespec_compare(&d->timeout_start, &deadline) >= 0) {
		udk_info("now %lu-%lu, deadline %lu-%lu\n",
			d->timeout_start.tv_sec, d->timeout_start.tv_nsec,
			deadline.tv_sec, deadline.tv_nsec);
		return true;
	}

	if (!d->next) {
		d->timeout_next = deadline;
		d->next = true;
	} else if (timespec_compare(&d->timeout_next, &deadline) > 0) {
		d->timeout_next = deadline;
	}

	return false;
}

static void udk_blk_mq_rq_timed_out(struct udk_request *req)
{
	udk_info("blk-mq: req timed out, tag#%d sector#%lu data_len#%u cmd_flags#%u deadline#%lu\n",
		req->tag, req->sector, req->data_len, req->cmd_flags, req->deadline.tv_sec);
	req->req_flags |= UDK_RQF_TIMED_OUT;
	if (req->queue->mq_ops->timeout) {
		enum udk_blk_eh_timer_return ret;

		ret = req->queue->mq_ops->timeout(req);
		if (ret == UDK_BLK_EH_DONE)
			return;
		BUG_ON(ret != UDK_BLK_EH_RESET_TIMER);
	}

	/* reset timer */
	udk_blk_mq_start_timer(req);
}

static bool udk_blk_mq_check_expired(unsigned long long *sb, unsigned int tag,
				     void *data)
{
	struct blk_expired_data *d = data;
	struct udk_blk_mq_hw_ctx *hctx = d->hctx;
	struct udk_blk_mq_tags *tags = udk_blk_mq_hctx_tags(hctx, 0);
	struct udk_request *req;

	req = tags->static_reqs[tag];

	if (udk_blk_req_expired(req, d)) {
		d->has_timedout_rq = true;
	}

	return true;
}

static bool udk_blk_mq_timed_out(unsigned long long *sb, unsigned int tag,
				     void *data)
{
	struct blk_expired_data *d = data;
	struct udk_blk_mq_hw_ctx *hctx = d->hctx;
	struct udk_blk_mq_tags *tags = udk_blk_mq_hctx_tags(hctx, 0);
	struct udk_request *req;

	req = tags->static_reqs[tag];

	if (udk_blk_req_expired(req, d)) {
		udk_blk_mq_rq_timed_out(req);
	}

	return true;
}

static void blk_mq_queue_hctx_iter(struct udk_blk_mq_hw_ctx *hctx, struct blk_expired_data *data)
{
	struct udk_blk_mq_tags *tags = udk_blk_mq_hctx_tags(hctx, 0);
	unsigned long long bitmap_tags[UDK_MAX_TAGS_BITMAP_ITEM] = {0};
	unsigned int bits = 0U;

	data->hctx = hctx;

	uspinlock_lock(&tags->lock);
	bits = BITS_TO_LONGLONGS(tags->nr_tags);
	NOFAIL(memcpy_s(bitmap_tags, bits * sizeof(unsigned long long),
			tags->bitmap_tags, bits * sizeof(unsigned long long)));
	uspinlock_unlock(&tags->lock);

	mq_bitmap_foreach_set_bit(bitmap_tags, tags->nr_tags, 0, udk_blk_mq_timed_out, data);
}

static void blk_mq_queue_timed_out(struct udk_request_queue *queue, struct blk_expired_data *data)
{
	int idx;
	struct udk_blk_mq_hw_ctx *hctx = NULL;

	udk_blk_mq_freeze_queue(queue);
	for (idx = 0; idx < queue->nr_hw_queues; idx++) {
		hctx = queue->hctx_table[idx];
		blk_mq_queue_hctx_iter(hctx, data);
	}
	udk_blk_mq_unfreeze_queue(queue);
}

static bool blk_mq_queue_has_timeout_req(struct udk_request_queue *queue, struct blk_expired_data *data)
{
	int idx;
	struct udk_blk_mq_tags *tags = NULL;

	for (idx = 0; idx < queue->nr_hw_queues; idx++) {
		data->hctx = queue->hctx_table[idx];
		tags = udk_blk_mq_hctx_tags(queue->hctx_table[idx], 0);
		uspinlock_lock(&tags->lock);
		mq_bitmap_foreach_set_bit(tags->bitmap_tags, tags->nr_tags, 0, udk_blk_mq_check_expired, data);
		if (data->has_timedout_rq) {
			uspinlock_unlock(&tags->lock);
			return true;
		}
		uspinlock_unlock(&tags->lock);
	}

	return false;
}

static void udk_blk_mq_time_out(unsigned long p)
{
	struct udk_request_queue *queue = ulong_to_ptr(p, struct udk_request_queue);
	struct blk_expired_data data = {0};
	struct udk_blk_mq_hw_ctx *hctx;
	struct timespec now;
	long sub;
	int idx;

	hm_clock_gettime(CLOCK_MONOTONIC, &now);
	data.timeout_start = now;

	udk_info("blk-mq: timer triggered now %lu\n", now.tv_sec);

	if (blk_mq_queue_has_timeout_req(queue, &data)) {
		blk_mq_queue_timed_out(queue, &data);
	}

	if (data.next) {
		hm_clock_gettime(CLOCK_MONOTONIC, &now);
		sub = timespec_sub(&data.timeout_next, &now);
		if (sub < 0) {
			udk_info("blk-mq: sub < 0, sub = %ld\n", sub);
			sub = 1000;
		}
		udk_timer_mod(&queue->timer, sub);
		udk_debug("blk-mq: timer shifting to next %lu\n", data.timeout_next.tv_sec);
	} else {
		udk_debug("blk-mq: timer shutdown\n");
	}
}

void udk_blk_mq_start_timer(struct udk_request *req)
{
	struct udk_request_queue *queue = req->queue;
	struct timespec expire, before;

	if (req->timeout == 0) {
		req->timeout = req->queue->timeout;
	}
	hm_clock_gettime(CLOCK_MONOTONIC, &expire);
	before = expire;
	timespec_add_ms(&expire, req->timeout);
	req->req_flags &= ~UDK_RQF_TIMED_OUT;
	req->deadline = expire;
	smp_wmb();
	req->state = UDK_MQ_RQ_IN_FLIGHT;

	if (!udk_timer_is_pending_lockless(&queue->timer)) {
		udk_timer_mod(&queue->timer, req->timeout);
		return;
	}

	if (timespec_compare(&expire, &queue->timer.expire) < 0) {
		long sub = timespec_sub(&queue->timer.expire, &expire);
		if (sub > MSEC_PER_SEC) {
			udk_timer_mod(&queue->timer, req->timeout);
		}
	}
}

static int udk_blk_mq_requeue_work(struct udk_work *p)
{
	struct udk_request_queue *q = container_of(p, struct udk_request_queue, requeue_work);
	struct udk_request *req;
	struct dlist_node *node = NULL;
	DLIST_HEAD(rq_list);
	int count = 0;
	int ret;

	udk_debug("blk-mq: requeuing %d request\n", q->requeue_nr);
	udk_work_destroy(p);

	uspinlock_lock(&q->requeue_lock);
	dlist_splice_tail_init(&q->prio_requeue_list, &rq_list);
	dlist_splice_tail_init(&q->requeue_list, &rq_list);
	uspinlock_unlock(&q->requeue_lock);

	while (!dlist_empty(&rq_list)) {
		req = dlist_entry(rq_list.next, struct udk_request, queuelist);
		udk_debug("blk-mq: dispatching req=%lx, #%d\n", req, req->tag);
		udk_request_info_set_stage(req, UDK_BIO_STAGE_REQUEUE_DISPATCH);
		dlist_delete(&req->queuelist);
		ret = q->mq_ops->queue_req(req->mq_hctx, req);
		count++;
		if (ret != 0) {
			udk_error("blk-mq: queue req failed, ret=%d\n", ret);
			udk_blk_mq_end_request(req, ret);
		}
	}

	uspinlock_lock(&q->requeue_lock);
	q->requeue_nr -= count;
	uspinlock_unlock(&q->requeue_lock);

	udk_debug("blk-mq: requeued %d requests, left %d\n", count, q->requeue_nr);
	return 0;
}

void udk_blk_mq_kick_requeue_list(struct udk_request_queue *q)
{
	udk_workqueue_add_work_default(&q->requeue_work);
}

void udk_blk_mq_requeue_req(struct udk_request *req, bool kick, bool prio)
{
	struct udk_request_queue *q = req->queue;

	if (req->state != UDK_MQ_RQ_IDLE) {
		req->state = UDK_MQ_RQ_IDLE;
		req->req_flags &= ~UDK_RQF_TIMED_OUT;
	}

	uspinlock_lock(&q->requeue_lock);
	if (prio) {
		dlist_insert_tail(&q->prio_requeue_list, &req->queuelist);
	} else {
		dlist_insert_tail(&q->requeue_list, &req->queuelist);
	}
	q->requeue_nr++;
	udk_request_info_set_stage(req, UDK_BIO_STAGE_REQUEUE);
	uspinlock_unlock(&q->requeue_lock);
	req->req_flags |= UDK_RQF_REQUEUED;
	udk_debug("blk-mq: insert #%d(0x%x,%lu,%u) to requeue list, requests in queue %d\n",
		req->tag, req->cmd_flags, req->sector, req->data_len, q->requeue_nr);

	if (kick) {
		udk_blk_mq_kick_requeue_list(q);
	}
}

static int udk_blk_mq_init_request_queue(struct udk_blk_mq_tag_set *set,
					 struct udk_request_queue *queue)
{
	int ret = 0;

	queue->mq_ops = set->ops;
	queue->tag_set = set;
	queue->nr_requests = set->queue_depth;
	queue->timeout = UDK_BLK_TIMEOUT_DEFAULT;
	vatomic32_init(&queue->queue_depth, 0);
	dlist_init(&queue->prio_requeue_list);
	dlist_init(&queue->requeue_list);

	udk_mutex_init(&queue->freeze_lock);
	vatomic32_init(&queue->freeze_depth, 0);
	vatomic32_init(&queue->active_depth, 0);
	udk_wait_cond_init(&queue->wait_cond);

	udk_timer_init(&queue->timer, udk_blk_mq_time_out, queue, 0);
	udk_work_init(&queue->requeue_work, udk_blk_mq_requeue_work, 1);

	ret = udk_blk_mq_alloc_ctxs(queue);
	if (ret < 0) {
		return -ENOMEM;
	}

	ret = udk_blk_mq_alloc_hctxs(set, queue);
	if (ret < 0) {
		free(queue->queue_ctx);
		queue->queue_ctx = NULL;
		return -ENOMEM;
	}

	udk_blk_mq_map_ctxs(queue);

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_mq_init_allocated_queue(queue);
#endif

	return 0;
}

#define MQ_BG_DISPATCH_INTERVAL_US 3000
static void *udk_blk_mq_background_dispatch(void *args)
{
	struct udk_request_queue *queue = (struct udk_request_queue *)args;

	if (queue == NULL) {
		return NULL;
	}

	while ((queue->queue_flags & UDK_BLK_QUEUE_FLAG_NO_BG_DISPATCH) == 0) {
		hm_usleep(MQ_BG_DISPATCH_INTERVAL_US);
		for (unsigned int i = 0; i < queue->nr_hw_queues; i++) {
			udk_blk_mq_ctx_dispatch(queue->hctx_table[i], false);
		}
	}

	queue->dispatch_thread = NULL;
	udk_info("dispatch thread exit\n");

	return NULL;
}

static int udk_blk_mq_set_background_dispatch(struct udk_request_queue *queue)
{
	raw_thread_attr_t attr = { 0 };
	struct raw_sched_param param = { 0 };
	int ret;

	ret = raw_thread_attr_init(&attr);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}
	ret = raw_thread_attr_setinheritsched(&attr, RAW_THREAD_EXPLICIT_SCHED);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}
	ret = raw_thread_attr_setschedpolicy(&attr, SCHED_OTHER);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}
	param.priority = 0;
	ret = raw_thread_attr_setschedparam(&attr, &param);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}

	ret = raw_thread_create(&queue->dispatch_thread, &attr,
				udk_blk_mq_background_dispatch,
				(void *)queue);
	if (ret != 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static void
udk_blk_mq_terminate_dispatch_thread(struct udk_request_queue *queue)
{
	if (queue->dispatch_thread != NULL) {
		queue->queue_flags |= UDK_BLK_QUEUE_FLAG_NO_BG_DISPATCH;
		(void)raw_thread_join(queue->dispatch_thread, NULL);
	}

	return;
}

void udk_blk_mq_free_queue(struct udk_request_queue *queue)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	unsigned int i;

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_mq_free_queue(queue);
	mas_blk_cleanup_queue(queue);
#endif

	udk_blk_mq_terminate_dispatch_thread(queue);

	if (queue->queue_ctx != NULL) {
		free(queue->queue_ctx);
	}

	if (queue->hctx_table != NULL) {
		for (i = 0; i < queue->nr_hw_queues; i++) {
			hctx = queue->hctx_table[i];
			if (hctx != NULL) {
				free(hctx->ctx_map);
				free(hctx->ctxs);
				free(hctx);
			}
		}
		free(queue->hctx_table);
	}

	mcache_udk_request_queue_free(queue);
}

struct udk_request_queue *
udk_blk_mq_init_queue_data(struct udk_blk_mq_tag_set *set, void *queuedata)
{
	struct udk_request_queue *queue = NULL;
	int ret;

	queue = udk_blk_mq_alloc_request_queue();
	if (queue == NULL) {
		return NULL;
	}
	queue->queuedata = queuedata;

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_allocated_queue_init(queue);
#endif

	ret = udk_blk_mq_init_request_queue(set, queue);
	if (ret < 0) {
		mcache_udk_request_queue_free(queue);
		return NULL;
	}

	if ((queue->tag_set->flags & UDK_BLK_MQ_NO_SCHED) &&
	    (queue->tag_set->flags & UDK_BLK_MQ_SHOULD_MERGE)) {
		ret = udk_blk_mq_set_background_dispatch(queue);
		if (ret < 0) {
			udk_blk_mq_free_queue(queue);
			return NULL;
		}
	}

	return queue;
}

static void udk_blk_mq_sched_free_tags(struct udk_request_queue *queue,
				       unsigned int flags)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;

	for (unsigned int idx = 0; idx < queue->nr_hw_queues; idx++) {
		hctx = queue->hctx_table[idx];
		if (hctx->sched_tags) {
			if (!udk_blk_mq_is_shared_tags(flags))
				udk_blk_mq_free_tags(hctx->sched_tags);
			hctx->sched_tags = NULL;
		}
	}

	if (udk_blk_mq_is_shared_tags(flags)) {
		udk_blk_mq_free_tags(queue->sched_shared_tags);
		queue->sched_shared_tags = NULL;
	}
}

void udk_blk_mq_exit_sched(struct udk_request_queue *queue,
			   struct udk_io_scheduler_queue *sched)
{
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	unsigned int flags = 0;

	for (unsigned int idx = 0; idx < queue->nr_hw_queues; idx++) {
		hctx = queue->hctx_table[idx];
		if (sched->type->ops.exit_hctx && hctx->sched_data) {
			sched->type->ops.exit_hctx(hctx, idx);
			hctx->sched_data = NULL;
		}
		flags = hctx->flags;
	}

	if (sched->type->ops.exit_sched)
		sched->type->ops.exit_sched(sched);
	udk_blk_mq_sched_free_tags(queue, flags);
	queue->scheduler = NULL;
}

static int udk_blk_mq_sched_alloc_tags(struct udk_request_queue *queue,
				       struct udk_blk_mq_hw_ctx *hctx)
{
	if (udk_blk_mq_is_shared_tags(queue->tag_set->flags)) {
		hctx->sched_tags = queue->sched_shared_tags;
		return 0;
	}

	hctx->sched_tags = udk_blk_mq_alloc_tags(queue->nr_requests);
	if (hctx->sched_tags == NULL) {
		return -ENOMEM;
	}

	return 0;
}

static int udk_blk_mq_alloc_sched_shared_tags(struct udk_request_queue *queue)
{
	queue->sched_shared_tags = udk_blk_mq_alloc_tags(queue->nr_requests);
	if (queue->sched_shared_tags == NULL) {
		return -ENOMEM;
	}

	return 0;
}

int udk_blk_mq_init_sched(struct udk_request_queue *queue,
			  struct udk_io_scheduler_type *sched_type)
{
	unsigned int flags = queue->tag_set->flags;
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	struct udk_io_scheduler_queue *sched = NULL;
	unsigned int idx;
	int ret;

	queue->nr_requests = queue->tag_set->queue_depth;

	if (udk_blk_mq_is_shared_tags(flags)) {
		ret = udk_blk_mq_alloc_sched_shared_tags(queue);
		if (ret < 0) {
			return ret;
		}
	}

	for (idx = 0; idx < queue->nr_hw_queues; idx++) {
		hctx = queue->hctx_table[idx];
		ret = udk_blk_mq_sched_alloc_tags(queue, hctx);
		if (ret < 0) {
			goto out;
		}
	}

	ret = sched_type->ops.init_sched(queue, sched_type);
	if (ret < 0) {
		goto out;
	}

	for (idx = 0; idx < queue->nr_hw_queues; idx++) {
		hctx = queue->hctx_table[idx];
		if (sched_type->ops.init_hctx) {
			ret = sched_type->ops.init_hctx(hctx, idx);
			if (ret) {
				sched = queue->scheduler;
				udk_blk_mq_exit_sched(queue, sched);
				return ret;
			}
		}
	}

	return 0;

out:
	udk_blk_mq_sched_free_tags(queue, flags);
	queue->scheduler = NULL;
	return ret;
}

static uint32_t udk_blk_mq_tag_pending(struct udk_blk_mq_tags *tags)
{
	uint32_t pending = 0;

	uspinlock_lock(&tags->lock);
	for (int i = 0; i < tags->nr_tags; i++) {
		if (raw_bitmap_test_bit(tags->bitmap_tags, tags->nr_tags, i) != 0)
			pending++;
	}
	uspinlock_unlock(&tags->lock);

	return pending;
}

uint32_t udk_blk_mq_req_pending(struct udk_request_queue *queue)
{
	uint32_t pending = 0;
	struct udk_blk_mq_tag_set *set = queue->tag_set;
	struct udk_blk_mq_tags **tags = set->tags;

	for (int i = 0; i < set->nr_hw_queues; i++)
		pending += udk_blk_mq_tag_pending(tags[i]);

	return pending;
}

static inline bool is_queue_freezed(struct udk_request_queue *queue)
{
	return (vatomic32_read(&queue->freeze_depth) > 0);
}

static inline bool is_queue_active(struct udk_request_queue *queue)
{
	return (vatomic32_read(&queue->active_depth) > 0);
}

void udk_blk_mq_freeze_queue(struct udk_request_queue *queue)
{
	if (queue == NULL) {
		return;
	}
	udk_mutex_lock(&queue->freeze_lock);
	vatomic32_inc(&queue->freeze_depth);
	udk_mutex_unlock(&queue->freeze_lock);
	udk_wait_cond(&queue->wait_cond, !is_queue_active(queue));
}

void udk_blk_mq_unfreeze_queue(struct udk_request_queue *queue)
{
	if (queue == NULL) {
		return;
	}
	udk_mutex_lock(&queue->freeze_lock);
	if (vatomic32_read(&queue->freeze_depth) > 0) {
		if (vatomic32_dec_get(&queue->freeze_depth) == 0) {
			udk_wakeup_all(&queue->wait_cond);
		}
	}
	udk_mutex_unlock(&queue->freeze_lock);
}

int udk_blk_mq_queue_enter(struct udk_request_queue *queue)
{
	if (queue == NULL) {
		return -EINVAL;
	}
	/* loop or wait until queue unfreeze */
	for (;;) {
		udk_mutex_lock(&queue->freeze_lock);
		if (!is_queue_freezed(queue)) {
			vatomic32_inc(&queue->active_depth);
			udk_mutex_unlock(&queue->freeze_lock);
			return 0;
		}
		udk_mutex_unlock(&queue->freeze_lock);
		udk_wait_cond(&queue->wait_cond, !is_queue_freezed(queue));
	}
}

void udk_blk_mq_queue_exit(struct udk_request_queue *queue)
{
	if (queue == NULL) {
		return;
	}
	udk_mutex_lock(&queue->freeze_lock);
	if (vatomic32_read(&queue->active_depth) > 0) {
		if (vatomic32_dec_get(&queue->active_depth) == 0) {
			udk_wakeup_all(&queue->wait_cond);
		}
	}
	udk_mutex_unlock(&queue->freeze_lock);
}
