/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block merge functions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */

#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk.h>
#include <udk/block/udk_disk_stats.h>
#include <udk/log.h>
#include <hongmeng/errno.h>
#include <libpreempt/preempt.h>
#include <libstrict/strict.h>

#define UDK_BLK_MAX_MEGE_PAGES 1024U
bool udk_req_can_merge(struct udk_request *req, struct udk_bio *bio)
{
#ifdef CONFIG_HISI_INLINE_ENCRYPTION
	struct hisi_inline_crypto_key *bc1 = req->bio->crypto_key;
	struct hisi_inline_crypto_key *bc2 = bio->crypto_key;

	if ((bc1 != NULL) || (bc2 != NULL)) {
		if ((bc1 != NULL) && (bc2 != NULL)) {
			if ((bc1->ci_key != bc2->ci_key) ||
			    (bc1->ci_key_len != bc2->ci_key_len) ||
			    (bc1->ci_key_index != bc2->ci_key_index) ||
			    (bc1->ci_metadata != bc2->ci_metadata)) {
				return false;
			}
		} else {
			return false;
		}
	}
#endif
	if (req->cmd_flags != bio->bi_rw) {
		return false;
	}

	if (udk_bio_data_dir(bio) != udk_req_data_dir(req)) {
		return false;
	}

	if (req->ioprio != bio->bi_ioprio) {
		return false;
	}

	if (req->pvec_num + bio->bi_vec_num > UDK_BLK_MAX_MEGE_PAGES) {
		return false;
	}

	if (udk_req_sectors(req) + udk_bio_nr_sectors(bio) > udk_disk_obtain_max_sectors(req->disk))
		return false;

	return true;
}

bool udk_blk_discard_mergable(struct udk_request *req)
{
	UNUSED(req);
	return false;
}

enum udk_sched_merge udk_blk_try_merge(struct udk_request *req,
				       struct udk_bio *bio)
{
	if (udk_blk_discard_mergable(req)) {
		return UDK_IO_SCHEDULER_DISCARD_MERGE;
	} else if (udk_req_pos(req) + udk_req_sectors(req) ==
		   bio->bi_sector) {
		return UDK_IO_SCHEDULER_BACK_MERGE;
	} else if (udk_req_pos(req) - udk_bio_sectors(bio) ==
		   bio->bi_sector) {
		return UDK_IO_SCHEDULER_FRONT_MERGE;
	}

	return UDK_IO_SCHEDULER_NO_MERGE;
}

#ifndef CONFIG_MAS_BLK
static enum bio_merge_status udk_bio_attempt_back_merge(struct udk_request *req,
							struct udk_bio *bio)
#else
enum bio_merge_status udk_bio_attempt_back_merge(struct udk_request *req,
							struct udk_bio *bio)
#endif
{
#ifdef CONFIG_HISI_INLINE_ENCRYPTION
	struct udk_bio *last_bio = req->biotail;
	struct hisi_inline_crypto_key *bc1 = last_bio->crypto_key;
	struct hisi_inline_crypto_key *bc2 = bio->crypto_key;

	if ((bc1 != NULL) || (bc2 != NULL)) {
		if ((bc1 != NULL) && (bc2 != NULL)) {
			if (bc1->index + last_bio->bi_vec_num != bc2->index) {
				return UDK_BIO_MERGE_NONE;
			}
		} else {
			return UDK_BIO_MERGE_NONE;
		}
	}
#endif
	req->biotail->next = bio;
	req->biotail = bio;
	req->data_len += bio->bi_size;
	req->pvec_num += bio->bi_vec_num;
	req->bio_num++;

	udk_diskstats_update_merges(bio);
	return UDK_BIO_MERGE_OK;
}

#ifndef CONFIG_MAS_BLK
static enum bio_merge_status
udk_bio_attempt_front_merge(struct udk_request *req, struct udk_bio *bio)
#else
enum bio_merge_status
udk_bio_attempt_front_merge(struct udk_request *req, struct udk_bio *bio)
#endif
{
#ifdef CONFIG_HISI_INLINE_ENCRYPTION
	struct udk_bio *first_bio = req->bio;
	struct hisi_inline_crypto_key *bc1 = first_bio->crypto_key;
	struct hisi_inline_crypto_key *bc2 = bio->crypto_key;

	if ((bc1 != NULL) || (bc2 != NULL)) {
		if ((bc1 != NULL) && (bc2 != NULL)) {
			if (bc2->index + bio->bi_vec_num != bc1->index) {
				return UDK_BIO_MERGE_NONE;
			}
		} else {
			return UDK_BIO_MERGE_NONE;
		}
	}
#endif
	bio->next = req->bio;
	req->bio = bio;

	req->sector = bio->bi_sector;
	req->data_len += bio->bi_size;
	req->pvec_num += bio->bi_vec_num;
	req->bio_num++;

	udk_diskstats_update_merges(bio);
	return UDK_BIO_MERGE_OK;
}

static enum bio_merge_status
udk_bio_attempt_discard_merge(struct udk_request_queue *queue,
			      struct udk_request *req, struct udk_bio *bio)
{
	UNUSED(queue, req);

	udk_diskstats_update_merges(bio);
	return UDK_BIO_MERGE_FAILED;
}

static enum bio_merge_status
udk_blk_attempt_bio_merge(struct udk_request_queue *queue,
			  struct udk_request *req, struct udk_bio *bio)
{
	if (!udk_req_can_merge(req, bio)) {
		return UDK_BIO_MERGE_NONE;
	}

	switch (udk_blk_try_merge(req, bio)) {
	case UDK_IO_SCHEDULER_BACK_MERGE:
		return udk_bio_attempt_back_merge(req, bio);
	case UDK_IO_SCHEDULER_FRONT_MERGE:
		return udk_bio_attempt_front_merge(req, bio);
	case UDK_IO_SCHEDULER_DISCARD_MERGE:
		return udk_bio_attempt_discard_merge(queue, req, bio);
	default:
		return UDK_BIO_MERGE_NONE;
	}

	return UDK_BIO_MERGE_FAILED;
}

static bool udk_blk_bio_list_merge(struct udk_request_queue *queue,
				   struct dlist_node *list, struct udk_bio *bio,
				   unsigned int merge_cnt)
{
	struct udk_request *req = NULL;
	int checked = merge_cnt;

	dlist_for_each_entry_reverse(req, list, struct udk_request, queuelist)
	{
		if (!checked--) {
			break;
		}

		switch (udk_blk_attempt_bio_merge(queue, req, bio)) {
		case UDK_BIO_MERGE_NONE:
			continue;
		case UDK_BIO_MERGE_OK:
			return true;
		case UDK_BIO_MERGE_FAILED:
			return false;
		default:
			break;
		}
	}

	return false;
}

#define UDK_BLK_BIO_MERGE_CNT 8U
static bool udk_blk_mq_sched_bio_merge(struct udk_request_queue *queue,
				       struct udk_bio *bio)
{
	struct udk_io_scheduler_queue *sched = queue->scheduler;
	struct udk_blk_mq_ctx *ctx = NULL;
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	bool ret = false;
	int cpu;

	if (sched && sched->type->ops.bio_merge) {
		ret = sched->type->ops.bio_merge(queue, bio, 1);
		goto out_put;
	}

	cpu = preempt_process_cpu_get();
	ctx = udk_blk_mq_get_ctx(queue, cpu);
	hctx = ctx->hctx;
	if (!(hctx->flags & UDK_BLK_MQ_SHOULD_MERGE) ||
	    dlist_empty(&ctx->req_list)) {
		goto out_put;
	}

	uspinlock_lock(&ctx->lock);
	if (udk_blk_bio_list_merge(queue, &ctx->req_list, bio,
				   UDK_BLK_BIO_MERGE_CNT)) {
		ret = true;
	}

	uspinlock_unlock(&ctx->lock);

out_put:
	return ret;
}

bool udk_blk_mq_try_merge_bio(struct udk_request_queue *queue,
			      struct udk_bio *bio)
{
	return udk_bio_mergeable(bio) && udk_blk_mq_sched_bio_merge(queue, bio);
}

bool udk_blk_mq_sched_try_insert_merge(struct udk_request_queue *queue,
				       struct udk_request *req,
				       struct dlist_node *free)
{
	return req_mergeable(req) &&
	       udk_sched_attempt_insert_merge(queue, req, free);
}

static enum udk_sched_merge udk_blk_try_req_merge(struct udk_request *req,
						  struct udk_request *next)
{
	if (udk_blk_discard_mergable(req)) {
		return UDK_IO_SCHEDULER_DISCARD_MERGE;
	} else if (udk_req_pos(req) + udk_req_sectors(req) ==
		   udk_req_pos(next)) {
		return UDK_IO_SCHEDULER_BACK_MERGE;
	}

	return UDK_IO_SCHEDULER_NO_MERGE;
}

static bool udk_req_attempt_discard_merge(struct udk_request_queue *queue,
					  struct udk_request *req,
					  struct udk_request *next)
{
	UNUSED(queue, req, next);
	return false;
}

static struct udk_request *
udk_request_attempt_merge(struct udk_request_queue *queue,
			  struct udk_request *req, struct udk_request *next)
{
	if (!req_mergeable(req) || !req_mergeable(next)) {
		return NULL;
	}

	if (req->cmd_flags != next->cmd_flags) {
		return NULL;
	}

	if (udk_req_data_dir(req) != udk_req_data_dir(next)) {
		return NULL;
	}

	if (req->ioprio != next->ioprio) {
		return NULL;
	}

	switch (udk_blk_try_req_merge(req, next)) {
	case UDK_IO_SCHEDULER_DISCARD_MERGE:
		if (!udk_req_attempt_discard_merge(queue, req, next)) {
			return NULL;
		}
		break;
	case UDK_IO_SCHEDULER_BACK_MERGE:
		break;
	default:
		return NULL;
	}

	req->biotail->next = next->bio;
	req->biotail = next->biotail;
	req->pvec_num += next->pvec_num;
	req->data_len += udk_req_bytes(next);
	if (!udk_blk_discard_mergable(req)) {
		udk_sched_merge_requests(queue, req, next);
	}

	next->bio = NULL;
	return next;
}

static struct udk_request *
udk_request_attempt_back_merge(struct udk_request_queue *queue,
			       struct udk_request *req)
{
	struct udk_request *next = udk_sched_next_request(queue, req);

	if (next != NULL) {
		return udk_request_attempt_merge(queue, req, next);
	}

	return NULL;
}

static struct udk_request *
udk_request_attempt_front_merge(struct udk_request_queue *queue,
				struct udk_request *req)
{
	struct udk_request *prev = udk_sched_prev_request(queue, req);

	if (prev != NULL) {
		return udk_request_attempt_merge(queue, prev, req);
	}

	return NULL;
}

bool udk_blk_mq_sched_try_merge(struct udk_request_queue *queue,
				struct udk_bio *bio,
				struct udk_request **merged_request)
{
	struct udk_request *req = NULL;

	switch (udk_sched_merge(queue, &req, bio)) {
	case UDK_IO_SCHEDULER_BACK_MERGE:
		if (udk_bio_attempt_back_merge(req, bio) != UDK_BIO_MERGE_OK) {
			return false;
		}
		*merged_request = udk_request_attempt_back_merge(queue, req);
		if ((*merged_request) == NULL) {
			udk_sched_merged_request(queue, req,
						 UDK_IO_SCHEDULER_BACK_MERGE);
		}
		return true;
	case UDK_IO_SCHEDULER_FRONT_MERGE:
		if (udk_bio_attempt_front_merge(req, bio) != UDK_BIO_MERGE_OK) {
			return false;
		}
		*merged_request = udk_request_attempt_front_merge(queue, req);
		if ((*merged_request) != NULL) {
			udk_sched_merged_request(queue, req,
						 UDK_IO_SCHEDULER_FRONT_MERGE);
		}
		return true;
	case UDK_IO_SCHEDULER_DISCARD_MERGE:
		return udk_bio_attempt_discard_merge(queue, req, bio) ==
		       UDK_BIO_MERGE_OK;
	default:
		return false;
	}
}

bool udk_blk_attempt_req_merge(struct udk_request_queue *queue,
			       struct udk_request *req,
			       struct udk_request *next)
{
	return udk_request_attempt_merge(queue, req, next);
}
