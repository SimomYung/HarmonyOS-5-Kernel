/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: block io latency framework
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <libstrict/strict.h>
#include <udk/block/udk_disk.h>
#include <udk/delay.h>
#include <udk/block/mas_blk/mas_blk_latency_interface.h>
#include <udk/mm.h>
#include <udk/delay.h>

#define LATENCY_WARNING_DEF_MS 2000
#define LATENCY_LOG_BUF_SIZE 500UL
#define NORMAL_TIMEOUT_MS 1000UL
#define IN_IRQ_TIMEOUT_MS 10UL
#define BIO_UDELAY_UNIT_US 10UL

static struct raw_mutex latency_log_protect_lock = RAW_MUTEX_INITIALIZER;

static void mas_blk_latency_bio_submit_func(struct udk_bio *bio);
static void mas_blk_latency_bio_endio_func(struct udk_bio *bio);
static void mas_blk_latency_req_init_func(struct udk_request *req);
static void mas_blk_latency_req_free_func(struct udk_request *req);

const struct bio_delay_stage_config bio_stage_cfg[BIO_PROC_STAGE_MAX] = {
	{ "BIO_SUBMIT", mas_blk_latency_bio_submit_func },
	{ "BIO_END", mas_blk_latency_bio_endio_func },
	{ "BIO_ENTER", NULL },
	{ "BIO_MAKE_REQ", NULL },
	{ "BIO_TAG_START", NULL },
	{ "BIO_MERGE", NULL },
};

const struct req_delay_stage_config req_stage_cfg[REQ_PROC_STAGE_MAX] = {
	{ "REQ_INIT", mas_blk_latency_req_init_func },
	{ "FSEQ_PREFLUSH", NULL },
	{ "FSEQ_DATA", NULL },
	{ "FSEQ_POSTFLUSH", NULL },
	{ "FSEQ_DONE", NULL },
	{ "REQ_RQ_START", NULL },
	{ "REQ_ADDTO_PLUGLIST", NULL },
	{ "REQ_FLUSH_PLUGLIST", NULL },
	{ "REQ_ADDTO_SYNC_LIST", NULL },
	{ "REQ_ADDTO_ASYNC_LIST", NULL },
	{ "REQ_QUEUE_ERR", NULL },
	{ "REQ_QUEUE_RQ", NULL },
	{ "REQ_SCSI_OUT", NULL },
	{ "REQ_SCSI_TARGET", NULL },
	{ "REQ_SCSI_HOST", NULL },
	{ "REQ_SCSI_RECOVERY", NULL },
	{ "REQ_SCSI_HBB", NULL },
	{ "REQ_SCSI_CAN", NULL },
	{ "REQ_SCSI_SELF_BLOCKED", NULL },
	{ "REQ_HOLD_START", NULL },
	{ "REQ_HOLD_FAIL", NULL },
	{ "REQ_HOLD_SUCCESS", NULL },
	{ "REQ_SQ_REQUEUE", NULL },
	{ "REQ_IRQ_DONE", NULL },
	{ "REQ_IRQ_DONE_UNISTORE", NULL },
	{ "REQ_IRQ_INSERT", NULL },
	{ "REQ_IRQ_FINISH_REQUEUE", NULL },
	{ "REQ_IRQ_EH_ADD", NULL },
	{ "REQ_EH_INC", NULL },
	{ "REQ_EH_STU", NULL },
	{ "REQ_EH_DEVICE", NULL },
	{ "REQ_EH_TARGET", NULL },
	{ "REQ_EH_BUS", NULL },
	{ "REQ_EH_HOST", NULL },
	{ "REQ_EH_OFFLINE", NULL },
	{ "REQ_EH_FLUSH", NULL },
	{ "REQ_UFS_EH_START", NULL },
	{ "REQ_UFS_EH_END", NULL },
	{ "REQ_RQ_UPDATE", NULL },
	{ "REQ_RQ_FREE", mas_blk_latency_req_free_func },
};

static unsigned long bio_timeout_threshold(
	const struct udk_bio *bio, const struct udk_request_queue *q)
{
#ifdef CONFIG_MAS_MQ_USING_CP
	if (mas_blk_bio_is_cp(bio))
		return MSEC_PER_SEC;
#else
	UNUSED(bio);
#endif

	return q->mas_queue.io_lat_warning_thresh;
}

static void mas_blk_setup_latency_timer(
	struct udk_bio *bio, const struct udk_request_queue *q)
{
	struct blk_bio_cust *mas_bio = NULL;
	int ret;

	mas_bio = &bio->mas_bio;
	mas_bio->latency_timer_running = 1;
	udk_timer_init(&mas_bio->latency_expire_timer,
		__mas_blk_latency_check_timer_expire,
		(unsigned long)(uintptr_t)&mas_bio->latency_expire_timer,
		bio_timeout_threshold(bio, q));

	/* latency_expire_timer will access bio, get it first */
	mas_blk_bio_get(bio);
	ret = udk_timer_add(&mas_bio->latency_expire_timer);
	if (unlikely(ret != E_HM_OK)) {
		mas_bio->latency_timer_running = 0;
		mas_blk_bio_put(bio);
	}
}

static void mas_blk_latency_bio_submit_func(struct udk_bio *bio)
{
	int i;
	struct udk_disk *disk = udkdev_to_disk(bio->bi_dev);
	struct udk_request_queue *q = disk->bd_queue;

	if (unlikely(!q || !q->mas_queue.io_latency_enable))
		return;

	if (likely(bio->mas_bio.fs_io_flag != IO_FROM_SUBMIT_BIO_MAGIC))
		mas_blk_setup_latency_timer(bio, q);

	bio->mas_bio.fs_io_flag = IO_FROM_SUBMIT_BIO_MAGIC;
	bio->mas_bio.io_req = NULL;
	for (i = 0; i < BIO_PROC_STAGE_MAX; i++) {
		bio->mas_bio.bio_stage_ktime_flag[i] = false;
	}
}

static void mas_blk_dump_timeouted_bio(
	const struct udk_bio *bio, const struct udk_request_queue *q)
{
	struct blk_dev_lld *lld = mas_blk_get_lld((struct udk_request_queue *)q);
	struct blk_queue_ops *ops = q->mas_queue_ops;

	mas_blk_dump_bio(bio, BLK_DUMP_WARNING);
	mas_blk_dump_queue_status(q, BLK_DUMP_WARNING);
	mas_blk_dump_fs(bio);

	if (lld->dump_fn)
		lld->dump_fn((struct udk_request_queue *)q, BLK_DUMP_WARNING);
	if (ops && ops->blk_status_dump_fn)
		ops->blk_status_dump_fn((struct udk_request_queue *)q, BLK_DUMP_WARNING);
}

/*
 * This enforces a rate limit: not more than 10 kernel messages every 5s.
 */
static struct uspinlock_s latency_ratelimit_lock = USPINLOCK_INITIALIZER;

static bool mas_blk_latency_ratelimit(vatomic32_t *cnt, int max_cnt,
	struct timespec *last_ktime, long nsec_num, bool *time_flag)
{
	struct timespec cmp_ktime;

	if (!(*time_flag)) {
		uspinlock_lock(&latency_ratelimit_lock);
		if (!(*time_flag)) {
			if (hm_clock_gettime(CLOCK_MONOTONIC, last_ktime) != E_HM_OK)
				goto out;
			cmp_ktime.tv_sec = last_ktime->tv_sec;
			cmp_ktime.tv_nsec = last_ktime->tv_nsec;
			mas_time_add(&cmp_ktime, nsec_num, last_ktime);
			*time_flag = true;
		}
		uspinlock_unlock(&latency_ratelimit_lock);
	}

	if ((int)vatomic32_inc_get(cnt) <= max_cnt)
		return false;

	if (hm_clock_gettime(CLOCK_MONOTONIC, &cmp_ktime) != E_HM_OK)
		return true;

	uspinlock_lock(&latency_ratelimit_lock);
	if (mas_time_before(&cmp_ktime, last_ktime))
		goto out;

	mas_time_add(&cmp_ktime, nsec_num, last_ktime);
	vatomic32_init(cnt, 0);
out:
	uspinlock_unlock(&latency_ratelimit_lock);
	return true;
}

#define MAX_WAIT_RETRY_CNT 100
static void mas_blk_latency_bio_endio_func(struct udk_bio *bio)
{
	unsigned long retry_cnt = MAX_WAIT_RETRY_CNT;

	bio->mas_bio.fs_io_flag = 0;
	bio->mas_bio.io_req = NULL;
	if (likely(udk_timer_del_sync(&bio->mas_bio.latency_expire_timer) == E_HM_OK)) {
		if (likely(bio->mas_bio.latency_timer_running)) {
			mas_blk_bio_put(bio);
		}
	} else {
		while (bio->mas_bio.latency_timer_running && retry_cnt--) {
			udk_udelay(BIO_UDELAY_UNIT_US);
		}
	}
	bio->mas_bio.latency_timer_running = 0;
	bio->mas_bio.io_req = NULL;
}

static void mas_blk_latency_req_init_func(struct udk_request *req)
{
	int i;
	struct blk_req_cust *mas_req = &req->mas_req;

	if (likely(req->queue && req->queue->mas_queue.io_latency_enable)) {
		mas_req->fs_io_flag = IO_FROM_SUBMIT_BIO_MAGIC;
		for (i = 0; i < REQ_PROC_STAGE_MAX; i++) {
			mas_req->req_stage_ktime_flag[i] = false;
		}
	} else {
		mas_req->fs_io_flag = 0;
	}
}

static void mas_blk_latency_req_free_func(struct udk_request *req)
{
	req->mas_req.fs_io_flag = 0;
}

/*
 * This enforces a rate limit: not more than 5 kernel messages
 * every 3s.
 */
#define MAX_TIMEOUT_CNT_PER_THREE_SEC 5
#define MAX_TIMEOUT_RATELIMIT_THREE_TIME (3UL * NSEC_PER_SEC)
void mas_blk_latency_check_timer_expire(struct udk_bio *bio)
{
	static vatomic32_t timeout_cnt = VATOMIC_INIT(0);
	static struct timespec timeout_ktime;
	static bool ktime_flag = false;
	struct udk_disk *disk = udkdev_to_disk(bio->bi_dev);
	struct udk_request_queue *q = disk->bd_queue;

	raw_mutex_lock(&latency_log_protect_lock);
	if (unlikely(bio->mas_bio.fs_io_flag != IO_FROM_SUBMIT_BIO_MAGIC)) {
		udk_error("%s Invalid fs_io_flag: 0x%x\n", __func__, bio->mas_bio.fs_io_flag);
		goto put_bio;
	}

	if (mas_blk_latency_ratelimit(&timeout_cnt, MAX_TIMEOUT_CNT_PER_THREE_SEC,
			&timeout_ktime, MAX_TIMEOUT_RATELIMIT_THREE_TIME, &ktime_flag))
		goto put_bio;

	mas_blk_dump_timeouted_bio(bio, q);

put_bio:
	raw_mutex_unlock(&latency_log_protect_lock);
	bio->mas_bio.latency_timer_running = 0;
	mas_blk_bio_put(bio);
}

/*
 * This function is used to record the bio latency checkpoint
 */
void mas_blk_latency_bio_check(
	struct udk_bio *bio, enum bio_process_stage_enum bio_stage)
{
	if (unlikely((bio_stage != BIO_PROC_STAGE_SUBMIT) &&
		     (bio->mas_bio.fs_io_flag != IO_FROM_SUBMIT_BIO_MAGIC)))
		return;

	if (bio_stage_cfg[bio_stage].function)
		bio_stage_cfg[bio_stage].function(bio);

	if (bio_stage == BIO_PROC_STAGE_SUBMIT && udk_bio_time_updated(&bio->start)) {
		bio->mas_bio.bio_stage_ktime[bio_stage] = bio->start;
		bio->mas_bio.bio_stage_ktime_ret[bio_stage] = E_HM_OK;
	} else {
#ifdef CONFIG_MAS_DEBUG_FS
		bio->mas_bio.bio_stage_ktime_ret[bio_stage] =
			hm_clock_gettime(CLOCK_MONOTONIC, &bio->mas_bio.bio_stage_ktime[bio_stage]);
#else
		bio->mas_bio.bio_stage_ktime_ret[bio_stage] = E_HM_INVAL;
#endif
	}

	bio->mas_bio.bio_stage_ktime_flag[bio_stage] = true;
}

/*
 * This function is used to record the request latency checkpoint
 */
void mas_blk_latency_req_check(
	struct udk_request *req, enum req_process_stage_enum req_stage)
{
	if (unlikely((req_stage != REQ_PROC_STAGE_INIT_FROM_BIO) &&
		     (req->mas_req.fs_io_flag != IO_FROM_SUBMIT_BIO_MAGIC)))
		return;

	if (likely(req_stage_cfg[req_stage].function))
		req_stage_cfg[req_stage].function(req);

#ifdef CONFIG_MAS_DEBUG_FS
	req->mas_req.req_stage_ktime_ret[req_stage] =
		hm_clock_gettime(CLOCK_MONOTONIC, &req->mas_req.req_stage_ktime[req_stage]);
#else
	req->mas_req.req_stage_ktime_ret[req_stage] = E_HM_INVAL;
#endif
	req->mas_req.req_stage_ktime_flag[req_stage] = true;
}

/*
 * This function is used to immigrate the req info in the bio from origin req
 */
void mas_blk_latency_for_merge(
	const struct udk_request *req, const struct udk_request *next)
{
	struct udk_bio *bio = NULL;

	bio = next->bio;
	while (bio) {
		bio->mas_bio.io_req = (struct udk_request *)req;
		bio = bio->next;
	}
}

void mas_blk_queue_latency_init(struct udk_request_queue *q)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);

	if (!blk_lld->latency_warning_threshold_ms)
		blk_lld->latency_warning_threshold_ms = LATENCY_WARNING_DEF_MS;
	q->mas_queue.io_lat_warning_thresh =
		blk_lld->latency_warning_threshold_ms;
	q->mas_queue.io_latency_enable =
		blk_lld->features & BLK_LLD_LATENCY_SUPPORT ? 1 : 0;
}

/*
 * This interface will be used to enable latency and set the warning threshold
 * on MQ tagset
 */
void blk_mq_tagset_latency_warning_set(
	struct udk_blk_mq_tag_set *tag_set, unsigned int warning_threshold_ms)
{
	tag_set->lld_func.features |= BLK_LLD_LATENCY_SUPPORT;
	tag_set->lld_func.latency_warning_threshold_ms = warning_threshold_ms ?
							 warning_threshold_ms :
							LATENCY_WARNING_DEF_MS;
}

/*
 * This interface will be used to enable latency and set the warning threshold
 * on request queue
 */
void blk_queue_latency_warning_set(
	struct udk_request_queue *q, unsigned int warning_threshold_ms)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);

	blk_lld->features |= BLK_LLD_LATENCY_SUPPORT;
	blk_lld->latency_warning_threshold_ms =
		warning_threshold_ms ? warning_threshold_ms
				     : LATENCY_WARNING_DEF_MS;
	q->mas_queue.io_lat_warning_thresh =
		blk_lld->latency_warning_threshold_ms;
	q->mas_queue.io_latency_enable = 1;
}

static void mas_blk_queue_io_latency_check_enable(
	struct udk_request_queue *q, int enable)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);

	if (q->mas_queue.io_latency_enable == enable)
		return;

	q->mas_queue.io_latency_enable = enable ? 1 : 0;
	if (enable)
		blk_lld->features |= BLK_LLD_LATENCY_SUPPORT;
	else
		blk_lld->features &= ~BLK_LLD_LATENCY_SUPPORT;
}

static void mas_blk_queue_io_latency_warning_threshold(
	struct udk_request_queue *q, unsigned int warning_threshold_ms)
{
	struct blk_dev_lld *blk_lld = mas_blk_get_lld(q);

	blk_lld->latency_warning_threshold_ms =
		warning_threshold_ms ? warning_threshold_ms
				     : LATENCY_WARNING_DEF_MS;
	q->mas_queue.io_lat_warning_thresh =
		warning_threshold_ms ? warning_threshold_ms
				     : LATENCY_WARNING_DEF_MS;
}

int mas_queue_io_latency_warning_threshold_store(void *ctx, void *src,
	size_t pos, size_t size, size_t *wsize)
{
	UNUSED(pos);
	struct udk_disk *disk = NULL;
	size_t len;
	char *write_buf = NULL;
	int ret;
	unsigned long val;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if ((disk == NULL) || (disk->bd_queue == NULL)) {
		return E_HM_INVAL;
	}

	write_buf = mas_malloc_sysfs_buf();
	if (write_buf == NULL) {
		return E_HM_NOMEM;
	}

	len = (size >= MAS_BLK_SYS_BUF_LEN) ? MAS_BLK_SYS_BUF_LEN : size;
	ret = udk_copy_from_user(write_buf, src, len);
	if (ret != E_HM_OK) {
		udk_free(write_buf);
		udk_warn("copy_from_user failed, ret=%d\n", ret);
		return ret;
	}

	val = strtoul(write_buf, NULL, MAS_DECIMAL);
	mas_blk_queue_io_latency_check_enable(disk->bd_queue, val ? 1 : 0);
	mas_blk_queue_io_latency_warning_threshold(disk->bd_queue, val);

	if (wsize != NULL) {
		*wsize = len;
	}

	udk_free(write_buf);
	return E_HM_OK;
}
