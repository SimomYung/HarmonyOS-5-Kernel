/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block bio dfx
 * Author: Huawei OS Kernel Lab
 * Create: Thr Dec 21 14:51:23 2023
 */

#include "udk_block_internal.h"

#include <string.h>
#include <inttypes.h>

#include <udk/log.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/lib/dlist.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_stats.h>
#include <internal/init.h>
#include <hongmeng/errno.h>
#include <libsysif/devhost/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>
#include <intapi/guardmgr/guardmgr.h>
#include <intapi/guardmgr/sysrq.h>

#ifdef UDK_FUSION
struct udk_bio_info {
	int bi_idx;
	devnum_t devt;
	unsigned long bi_rw;
	struct udk_bio *bio;
	pid_t tid;
	enum udk_bio_stage_enum bi_stage;
	struct timespec time[UDK_BIO_STAGE_NUM];
};

struct udk_bio_stats {
	struct udk_bio_info bio_info[UDK_BIO_INFO_MAX_COUNT];
	DEFINE_RAW_BITMAP(used_bio, UDK_BIO_INFO_MAX_COUNT);
	struct udk_mutex bio_lock;
};

struct udk_blk_hungtask_notifier {
	udk_hungtask_notifier notify_func;
	void *args;
	struct dlist_node notifier_list;
};

static struct udk_bio_stats bio_stats;
static DLIST_HEAD(g_notifier_list);
static struct udk_mutex g_notifier_list_lock;

static inline void hungtask_notifier_list_lock(void)
{
	udk_mutex_lock(&g_notifier_list_lock);
}

static inline void hungtask_notifier_list_unlock(void)
{
	udk_mutex_unlock(&g_notifier_list_lock);
}

static const char *bio_stage_str[UDK_BIO_STAGE_NUM + 1] = {
	"uninitialized",
	"submit",
	"requeue",
	"requeue_dispatch",
	"timeout",
	"error_handler",
	"endio",
	"finish",
	"unknown"
};

static int bio_stats_init(void)
{
	mem_zero_s(bio_stats);
	dlist_init(&g_notifier_list);
	udk_mutex_init(&g_notifier_list_lock);
	udk_mutex_init(&bio_stats.bio_lock);

	return 0;
}
udk_init_call(bio_stats_init);

static void bio_info_stage_set(struct udk_bio_info *bio_info, int stage)
{
	if (bio_info == NULL) {
		return;
	}

	bio_info->bi_stage = stage;
	if (stage == UDK_BIO_STAGE_SUBMIT &&
	    udk_bio_time_updated(&bio_info->bio->start)) {
		bio_info->time[stage] = bio_info->bio->start;
	} else if (stage == UDK_BIO_STAGE_ENDIO &&
		   udk_bio_time_updated(&bio_info->bio->finish)) {
		bio_info->time[stage] = bio_info->bio->finish;
	} else {
		clock_gettime(CLOCK_MONOTONIC, &bio_info->time[stage]);
	}
}

static inline bool check_bio_need_stats(struct udk_bio *bio)
{
	struct udk_disk *disk = NULL;

	BUG_ON(bio == NULL);
	disk = udkdev_to_disk(bio->bi_dev);

	return udk_disk_check_flag(disk, UDK_DISK_FLAG_BIO_STATS);
}

static int bio_get_free_idx(void)
{
	unsigned int idx;

	(void)udk_mutex_lock(&bio_stats.bio_lock);
	idx = raw_bitmap_find_first_zero(bio_stats.used_bio, UDK_BIO_INFO_MAX_COUNT);
	if (idx >= UDK_BIO_INFO_MAX_COUNT) {
		udk_mutex_unlock(&bio_stats.bio_lock);
		return -EBUSY;
	}
	(void)raw_bitmap_set_bit(bio_stats.used_bio, UDK_BIO_INFO_MAX_COUNT, idx);
	udk_mutex_unlock(&bio_stats.bio_lock);

	return (int)idx;
}

/* called in submit bio stage */
#define GET_AVAILABLE_BIO_SLEEP_TIME		5000
void udk_bio_info_init(struct udk_bio *bio)
{
	int idx;
	struct udk_bio_info *bio_info = NULL;

	if (!check_bio_need_stats(bio)) {
		return;
	}

	do {
		idx = bio_get_free_idx();
		if (idx < 0) {
			hm_usleep(GET_AVAILABLE_BIO_SLEEP_TIME);
		}
	} while (idx < 0);

	bio->bi_idx = idx;
	bio_info = &bio_stats.bio_info[idx];
	bio_info->bi_idx = idx;
	bio_info->bio = bio;
	bio_info->bi_rw = bio->bi_rw;
	bio_info->devt = bio->bi_dev->devt;
	bio_info->tid = hm_actv_local_thread_tid_of();
	bio_info_stage_set(bio_info, UDK_BIO_STAGE_SUBMIT);
}

void udk_bio_info_reset(struct udk_bio *bio)
{
	int ret;
	int idx;
	struct udk_bio_info *bio_info = NULL;

	idx = bio->bi_idx;
	if (idx == (UDK_BIO_UNALLOCATED_IDX) || !check_bio_need_stats(bio)) {
		return ;
	}
	(void)udk_mutex_lock(&bio_stats.bio_lock);
	ret = raw_bitmap_test_bit(bio_stats.used_bio, UDK_BIO_INFO_MAX_COUNT, (unsigned int)idx);
	if (ret < 0) {
		udk_mutex_unlock(&bio_stats.bio_lock);
		BUG_ON(true);
	}
	bio_info = &bio_stats.bio_info[idx];
	bio_info->bio = NULL;
	bio_info->bi_stage = UDK_BIO_STAGE_FINISH;
	(void)raw_bitmap_clear_bit(bio_stats.used_bio, UDK_BIO_INFO_MAX_COUNT, (unsigned int)idx);
	udk_mutex_unlock(&bio_stats.bio_lock);
}

void udk_bio_info_set_stage(struct udk_bio *bio, int stage)
{
	int idx;
	struct udk_bio_info *bio_info = NULL;

	idx = bio->bi_idx;
	if ((idx == UDK_BIO_UNALLOCATED_IDX) || !check_bio_need_stats(bio)) {
		return ;
	}
	bio_info = &bio_stats.bio_info[idx];
	bio_info_stage_set(bio_info, stage);
}

void udk_request_info_set_stage(struct udk_request *req, int stage)
{
	int idx;
	struct udk_bio *bio = NULL;
	struct udk_bio_info *bio_info = NULL;

	if (req == NULL) {
		return;
	}

	bio = req->bio;
	while (bio != NULL) {
		udk_bio_info_set_stage(bio, stage);
		if (bio == req->biotail) {
			break;
		}
		bio = bio->next;
	}
}

static bool hungtask_notifier_is_registered(udk_hungtask_notifier notify_func, void *args)
{
	struct udk_blk_hungtask_notifier *curr = NULL;
	struct udk_blk_hungtask_notifier *next = NULL;

	dlist_for_each_entry_safe(curr, next, &g_notifier_list,
				  struct udk_blk_hungtask_notifier, notifier_list) {
		if (((uintptr_t)curr->notify_func == (uintptr_t)notify_func) &&
		    ((uintptr_t)curr->args == (uintptr_t)args)) {
			return true;
		}
	}

	return false;
}

int udk_blk_register_hungtask_notifier(udk_hungtask_notifier notify_func, void *args)
{
	struct udk_blk_hungtask_notifier *notifier = NULL;

	if (notify_func == NULL) {
		return -EINVAL;
	}

	hungtask_notifier_list_lock();
	if (hungtask_notifier_is_registered(notify_func, args)) {
		hungtask_notifier_list_unlock();
		return 0;
	}

	notifier = udk_malloc(sizeof(struct udk_blk_hungtask_notifier));
	if (notifier == NULL) {
		hungtask_notifier_list_unlock();
		udk_error("failed to alloc mem\n");
		return -ENOMEM;
	}
	mem_zero_s(*notifier);

	notifier->notify_func = notify_func;
	notifier->args = args;

	dlist_init(&notifier->notifier_list);
	dlist_insert_tail(&g_notifier_list, &notifier->notifier_list);
	hungtask_notifier_list_unlock();

	return 0;
}

static inline
const char *covert_bio_stage_to_str(enum udk_bio_stage_enum bi_stage)
{
	return bio_stage_str[bi_stage];
}

static char *covert_bio_op_to_str(unsigned long bi_rw)
{
	unsigned int type = bi_rw & UDK_REQ_OP_MASK;

	switch (type) {
	case UDK_REQ_READ:
		return "READ";
	case UDK_REQ_WRITE:
		return "WRITE";
	case UDK_REQ_FLUSH:
		return "FLUSH";
	case UDK_REQ_DISCARD:
		return "DISCARD";
	default:
		return "UNKNOWN";
	}
}

static inline bool bio_is_inflight(struct udk_bio_info *bio_info)
{
	return ((bio_info->bi_stage != UDK_BIO_STAGE_UNINITIALIZED) &&
		(bio_info->bi_stage != UDK_BIO_STAGE_FINISH));
}

static void trigger_blk_hungtask_notifier(void)
{
	struct udk_blk_hungtask_notifier *curr = NULL;
	struct udk_blk_hungtask_notifier *next = NULL;

	dlist_for_each_entry_safe(curr, next, &g_notifier_list,
				  struct udk_blk_hungtask_notifier, notifier_list) {
		if (curr->notify_func != NULL) {
			(void)curr->notify_func(curr->args);
		}
	}
}

static int bio_hungtask_notifier(void)
{
	int inflight = 0;
	struct udk_bio_info *bio_info = NULL;

	trigger_blk_hungtask_notifier();
	udk_info("Dump inflight udk bio info begin\n");
	for (int i = 0; i < (int)UDK_BIO_INFO_MAX_COUNT; i++) {
		bio_info = &bio_stats.bio_info[i];
		if (bio_is_inflight(bio_info)) {
			inflight++;
			udk_info("bio_idx=%d, bi_rw=[%s, 0x%lx], stage=%s, devt=0x%x, tid=%u\n",
				bio_info->bi_idx,  covert_bio_op_to_str(bio_info->bi_rw),
				bio_info->bi_rw, covert_bio_stage_to_str(bio_info->bi_stage),
				(unsigned int)bio_info->devt, (unsigned int)bio_info->tid);
#ifdef __MUSL_V120_TIME64
			udk_info("submit_bio_time=%" PRId64 ".%09" PRId64 "\n",
				 bio_info->time[UDK_BIO_STAGE_SUBMIT].tv_sec,
				 bio_info->time[UDK_BIO_STAGE_SUBMIT].tv_nsec);
			udk_info("curr_stage_time=%" PRId64 ".%09" PRId64 "\n",
				 bio_info->time[bio_info->bi_stage].tv_sec,
				 bio_info->time[bio_info->bi_stage].tv_nsec);
#else
			udk_info("submit_bio_time=%ld.%ld\n",
				 bio_info->time[UDK_BIO_STAGE_SUBMIT].tv_sec,
				 bio_info->time[UDK_BIO_STAGE_SUBMIT].tv_nsec);
			udk_info("curr_stage_time=%ld.%ld\n",
				 bio_info->time[bio_info->bi_stage].tv_sec,
				 bio_info->time[bio_info->bi_stage].tv_nsec);
#endif
		}
	}
	udk_info("inflight bio number=%d\n", inflight);
	udk_info("Dump inflight udk bio info end\n");

	return 0;
}

int udk_bio_register_hungtask_notifier(void)
{
	int err;

	sysrq_register_dump_freeze(bio_hungtask_notifier);

	err = guardmgr_register_hungtask_notifier(bio_hungtask_notifier);
	if (err != E_HM_OK) {
		udk_error("register hungtask notifier failed\n");
	}

	return err;
}

#else

void udk_bio_info_init(struct udk_bio *bio)
{
	UNUSED(bio);
}

void udk_bio_info_reset(struct udk_bio *bio)
{
	UNUSED(bio);
}

void udk_bio_info_set_stage(struct udk_bio *bio, int stage)
{
	UNUSED(bio, stage);
}

void udk_request_info_set_stage(struct udk_request *req, int stage)
{
	UNUSED(req, stage);
}

int udk_bio_register_hungtask_notifier(void)
{
	return 0;
}

int udk_blk_register_hungtask_notifier(udk_hungtask_notifier notify_func, void *args)
{
	UNUSED(notify_func, args);
	return 0;
}
#endif
