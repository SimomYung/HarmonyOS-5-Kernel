/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 * Description: MAS MQ ioscheduler
 */

#define pr_fmt(fmt) "[BLK-IO]" fmt

#include <libstrict/strict.h>
#include <libsched/priority.h>
#include <udk/delay.h>
#include <udk/block/mas_blk/mas_blk.h>
#include <udk/block/mas_blk/mas_blk_iosched_interface.h>
#include <udk/block/mas_blk/mas_blk_mq_tag.h>
#include <udk/block/mas_blk/mas_blk_flush_interface.h>
#include <udk/block/mas_blk/mas_blk_core_interface.h>
#include <udk/mm.h>

#define MAS_UFS_MQ_PLUG_MERGE_ENABLE 1
#define MAS_UFS_MQ_SYNC_IO_WAIT_US 100
#define MAS_BLK_CP_LVL1_MS 100
#define MAS_BLK_CP_LVL2_MS 500

#define MAS_UFS_MQ_PLUG_MERGE_MAX_SIZE (512 * 1024)

#define MAS_MQ_SYNC_DISPATCH_LIMIT 28

#define ASYNC_DISP_LMT_HI_PRESSURE 16
#define ASYNC_DISP_LMT_SB1 14
#define ASYNC_DISP_LMT_SB2 10
#define ASYNC_DISP_LMT_SB1_FWB 14
#define ASYNC_DISP_LMT_SB2_FWB 10

#ifdef CONFIG_MAS_BLK_BW_OPTIMIZE
#define ASYNC_DISP_LMT 14
#define ASYNC_DISP_LMT_SB_LONG 14
#define ASYNC_DISP_LMT_SB_LONG_HP 14
#define SB_CHECK_PERIOD_MS 5
#define SB_LEVEL1_HOLD_MS 5
#define SB_LONG_HOLD_MS 5
#else
#define ASYNC_DISP_LMT 12
#define ASYNC_DISP_LMT_SB_LONG 8
#define ASYNC_DISP_LMT_SB_LONG_HP 12
/* SB short for sync burst */
#define SB_CHECK_PERIOD_MS 10
#define SB_LEVEL1_HOLD_MS 30
#define SB_LONG_HOLD_MS 20
#endif

#define SB_LONG_DUR_MS 500
#define IO_HW_PENDING_THRESH 8

#define MAS_MQ_IO_JAM_MS 2000

#define MAS_BLK_IO_GUARD_PERIOD_MS 2000

#define MAS_BLK_TURBO_CHECK_PERIOD_MS 500
#define ASYNC_THROT_STAGE_ONE 0
#define ASYNC_STAGE_ONE_LIMIT 5
#define ASYNC_THROT_STAGE_TWO 1
#define ASYNC_STAGE_TWO_LIMIT 20
#define ASYNC_WRITE_CHECK_MS 1000

enum mas_blk_mq_sync_burst_level {
	MAS_UFS_MQ_SB_NORMAL = 0,
	MAS_UFS_MQ_SB_LEVEL1,
	MAS_UFS_MQ_SB_LEVEL2,
	MAS_UFS_MQ_SB_LONG,
};

enum mas_ufs_mq_async_dispatch_mode {
	/* async io will be limited by a fixed value */
	MAS_UFS_MQ_ASYNC_NORMAL_MODE = 0,
	/* async io will be limited by sync io */
	MAS_UFS_MQ_ASYNC_FWB_MODE,
	/* async io will be dispatched a.s.a.p */
	MAS_UFS_MQ_ASYNC_LM_MODE,
};

enum mas_ufs_mq_async_sched_type {
	/* async io will be sort by submitted time */
	MAS_UFS_MQ_SCHED_ASYNC_FIFO = 0,
};

struct mas_ufs_mq_work {
	struct timespec last_queue_tm;
	struct timespec last_enter_tm;
	struct timespec last_exit_tm;
	struct udk_work io_dispatch_work;
	struct udk_request_queue *queue;
};

/*
 * This struct defines all the variable for ufs mq io-scheduler per lld.
 */
struct mas_ufs_sched_ds_lld {
	struct blk_dev_lld *lld;

#define MAS_MQ_DEFAULT_CP_LIMIT 5
	unsigned int cp_io_limit;
	/* IOs with cp flag in low level driver */
	vatomic32_t cp_io_inflt_cnt;
	vatomic32_t cpp_io_inflt_cnt;

	/* MQ tag usage statistic */
	vatomic32_t mq_tag_used_cnt;
	vatomic32_t mq_resrvd_tag_used_cnt;
	vatomic32_t mq_prio_tag_used_cnt;
	/* count of VIP IOs that wait for tags */
	vatomic32_t vip_wait_cnt;

	/* Sync IOs in low level driver */
	vatomic32_t sync_io_inflt_cnt;
	/* Async IOs in low level driver */
	vatomic32_t async_io_inflt_cnt;
	/* VIP IOs in low level driver */
	vatomic32_t vip_io_inflt_cnt;
	/* FG IOs in low level driver */
	vatomic32_t fg_io_inflt_cnt;

	struct uspinlock_s sync_disp_lock;
	/* List for all requeued high prio IOs */
	struct dlist_node hp_sync_disp_list;
	/* IO counts in high_prio_sync_dispatch_list */
	vatomic32_t hp_io_list_cnt;

	/* List for all requeued sync IOs */
	struct dlist_node sync_disp_list;
	/* IO counts in sync_dispatch_list */
	vatomic32_t sync_io_list_cnt;

	struct uspinlock_s async_disp_lock;
	bool async_io_sched_inited;
	struct dlist_node async_fifo_list;
	/* IO counts in async_dispatch_list */
	vatomic32_t async_io_list_cnt;
	/* Max sync IOs allowed */
	int sync_io_inflt_lmt;
	/* Max async IOs allowed */
	int async_io_inflt_lmt;

	/* Submit time for latest sync io */
	struct timespec last_sync_io_submit_tm;
	/* Burst mode trigger time */
	struct timespec last_sync_burst_trig_tm;
	/* Complete time for latest sync io */
	struct timespec last_sync_io_compl_tm;
	/* Complete time for latest async io */
	struct timespec last_async_io_compl_tm;
	/* Last async inflight limit update time */
	struct timespec last_async_io_inflt_lmt_update_tm;
	struct udk_timer sb_dispatch_check_timer;
	struct raw_mutex sb_dispatch_check_timer_lock;
	/* Status for sync burst state machine */
	vatomic32_t sb_dispatch_level;
	/* Dispatch mode for async io */
	enum mas_ufs_mq_async_dispatch_mode async_dispatch_mode;
	struct udk_timer write_throttle_check_timer;
	struct raw_mutex write_throttle_check_timer_lock;
	struct uspinlock_s async_limit_update_lock;
	bool async_limit_update_enable;

	vatomic32_t ref_cnt;

	struct udk_timer turbo_timer;
	struct raw_mutex turbo_timer_lock;
	bool turbo_mode;
};

struct mas_ufs_mq_sched {
	struct udk_request_queue *q;
	/* The work will dispatch all the requeue sync IOs */
	struct mas_ufs_mq_work sync_dispatch_work;
	/* The work will dispatch all the requeue async IOs */
	struct mas_ufs_mq_work async_dispatch_work;
	struct dlist_node io_guard_list_node;
	struct mas_ufs_sched_ds_lld *sched_ds_lld;
};

/*
 * This struct defines all the variable for ufs mq async io-scheduler.
 */
struct mas_ufs_mq_async_sched {
	enum mas_ufs_mq_async_sched_type type;
	/* async io sched init interface */
	void (*async_sched_init_fn)(struct mas_ufs_sched_ds_lld *ds_lld);
	/* async io insert request interface */
	void (*async_sched_insert_fn)(
		struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld);
	/* async io seek request interface */
	struct udk_request *(*async_sched_seek_fn)(
		const struct mas_ufs_sched_ds_lld *ds_lld);
	/* async io requeue request interface */
	void (*async_sched_requeue_fn)(
		struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld);
	/* async io request merge interface */
	bool (*async_sched_attempt_merge_fn)(
		struct udk_bio *bio, struct udk_request_queue *q);
	/* interface to query async io sched empty or not */
	bool (*async_sched_is_empty_fn)(const struct mas_ufs_sched_ds_lld *ds_lld);
};

struct mas_ufs_mq_priv {
	struct mas_ufs_mq_async_sched *async_io_sched_strategy;
};

static struct udk_workqueue *mas_blk_mq_async_disp_wq = NULL;
static struct udk_workqueue *mas_blk_mq_sync_disp_wq = NULL;
static struct udk_workqueue *mas_blk_io_guard_wq = NULL;
static DLIST_HEAD(mas_io_guard_queue_list);
static struct uspinlock_s io_guard_queue_list_lock = USPINLOCK_INITIALIZER;
static struct udk_work mas_io_guard_work;

struct mas_ufs_sched_ds_lld *get_sched_ds_lld(
	const struct udk_request_queue *q)
{
	void *queuedata = q->mas_queue.cust_queuedata;

	return queuedata ? ((struct mas_ufs_mq_sched *)queuedata)->sched_ds_lld
		       : NULL;
}

int get_mq_all_tag_used(struct udk_request_queue *q)
{
	struct mas_ufs_sched_ds_lld *sched_ds_lld = NULL;
	struct blk_dev_lld *lld = NULL;

	if (!q)
		return 0;
	lld = mas_blk_get_lld(q);
	if (!lld)
		return 0;
	sched_ds_lld = (struct mas_ufs_sched_ds_lld *)lld->sched_ds_lld;
	if (!sched_ds_lld)
		return 0;
	return vatomic32_read(&sched_ds_lld->mq_prio_tag_used_cnt) +
	       vatomic32_read(&sched_ds_lld->mq_tag_used_cnt) +
	       vatomic32_read(&sched_ds_lld->mq_resrvd_tag_used_cnt);
}

int get_mq_prio_tag_used(struct udk_request_queue *q)
{
	struct mas_ufs_sched_ds_lld *sched_ds_lld = NULL;
	struct blk_dev_lld *lld = NULL;

	if (!q)
		return 0;
	lld = mas_blk_get_lld(q);
	if (!lld)
		return 0;
	sched_ds_lld = (struct mas_ufs_sched_ds_lld *)lld->sched_ds_lld;
	if (!sched_ds_lld)
		return 0;
	return vatomic32_read(&sched_ds_lld->mq_prio_tag_used_cnt);
}

static inline bool io_submit_intrvl_l_than(
	struct mas_ufs_sched_ds_lld *ds_lld, struct timespec base,
	unsigned long ms)
{
	struct timespec tmp;

	mas_time_add(&ds_lld->last_sync_io_submit_tm, (long)(ms * NSEC_PER_MSEC), &tmp);

	return mas_time_before(&base, &tmp);
}

static inline bool io_submit_intrvl_g_than(
	struct mas_ufs_sched_ds_lld *ds_lld, struct timespec base,
	unsigned long ms)
{
	struct timespec tmp;

	mas_time_add(&ds_lld->last_sync_io_submit_tm, (long)(ms * NSEC_PER_MSEC), &tmp);

	return mas_time_before(&tmp, &base);
}

static inline bool last_sync_io_compl_g_than(
	struct mas_ufs_sched_ds_lld *ds_lld, struct timespec base,
	unsigned long ms)
{
	struct timespec tmp;

	mas_time_add(&ds_lld->last_sync_io_compl_tm, (long)(ms * NSEC_PER_MSEC), &tmp);

	return mas_time_before(&tmp, &base);
}

static inline bool last_async_io_compl_g_than(
	struct mas_ufs_sched_ds_lld *ds_lld, struct timespec base,
	unsigned long ms)
{
	struct timespec tmp;

	mas_time_add(&ds_lld->last_async_io_compl_tm, (long)(ms * NSEC_PER_MSEC), &tmp);

	return mas_time_before(&tmp, &base);
}

static inline bool burst_mode_dur_g_than(
	struct mas_ufs_sched_ds_lld *ds_lld, struct timespec base,
	unsigned long ms)
{
	struct timespec tmp;

	mas_time_add(&ds_lld->last_sync_burst_trig_tm, (long)(ms * NSEC_PER_MSEC), &tmp);

	return mas_time_before(&tmp, &base);
}

static void ufs_mq_burst_mode_calc_at_sync_io_continue(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	switch (vatomic32_read(&ds_lld->sb_dispatch_level)) {
	case MAS_UFS_MQ_SB_LEVEL1:
		vatomic32_init(&ds_lld->sb_dispatch_level, MAS_UFS_MQ_SB_LEVEL2);
		break;
	case MAS_UFS_MQ_SB_LONG:
		if (!vatomic32_read(&(ds_lld->mq_resrvd_tag_used_cnt)))
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_LEVEL1);
		break;
	default:
		break;
	}
}

static bool ufs_mq_burst_mode_calc_at_sync_io_pause(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	bool burst_mode = true;
	struct timespec now;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &now);

	switch (vatomic32_read(&ds_lld->sb_dispatch_level)) {
	case MAS_UFS_MQ_SB_LEVEL1:
		if (last_sync_io_compl_g_than(ds_lld, now, SB_LEVEL1_HOLD_MS)) {
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_NORMAL);
			burst_mode = false;
		}
		break;
	case MAS_UFS_MQ_SB_LEVEL2:
		if (io_submit_intrvl_g_than(ds_lld, now, SB_CHECK_PERIOD_MS))
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_LEVEL1);
		break;
	case MAS_UFS_MQ_SB_LONG:
		if (last_sync_io_compl_g_than(ds_lld, now, SB_LONG_HOLD_MS)) {
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_NORMAL);
			burst_mode = false;
		}
		break;
	default:
		break;
	}

	return burst_mode;
}

void ufs_mq_sync_burst_check_timer_expire(struct mas_ufs_sched_ds_lld *sched_ds_lld)
{
	bool restart_timer = false;
	struct timespec now_ktime;
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds_lld;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &now_ktime);

	switch (vatomic32_read(&ds_lld->sb_dispatch_level)) {
	case MAS_UFS_MQ_SB_LEVEL1:
	case MAS_UFS_MQ_SB_LEVEL2:
		if (burst_mode_dur_g_than(ds_lld, now_ktime, SB_LONG_DUR_MS))
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_LONG);
		break;
	default:
		break;
	}

	if (io_submit_intrvl_l_than(ds_lld, now_ktime, SB_CHECK_PERIOD_MS)) {
		ufs_mq_burst_mode_calc_at_sync_io_continue(ds_lld);
		restart_timer = true;
	} else {
		restart_timer = ufs_mq_burst_mode_calc_at_sync_io_pause(ds_lld);
	}

	if (restart_timer)
		udk_timer_mod(&ds_lld->sb_dispatch_check_timer, SB_CHECK_PERIOD_MS);
}

void ufs_mq_sync_burst_dispatch_check(struct mas_ufs_sched_ds_lld *ds_lld)
{
	struct timespec now_ktime;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &now_ktime);

	switch (vatomic32_read(&ds_lld->sb_dispatch_level)) {
	case MAS_UFS_MQ_SB_NORMAL:
		if (likely(io_submit_intrvl_l_than(ds_lld, now_ktime,
			    SB_CHECK_PERIOD_MS))) {
			ds_lld->last_sync_burst_trig_tm = now_ktime;
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_LEVEL1);

			raw_mutex_lock(&ds_lld->sb_dispatch_check_timer_lock);
			udk_timer_del_sync(&ds_lld->sb_dispatch_check_timer);
			udk_timer_add(&ds_lld->sb_dispatch_check_timer);
			raw_mutex_unlock(&ds_lld->sb_dispatch_check_timer_lock);
		}
		break;
	case MAS_UFS_MQ_SB_LEVEL1:
		if (likely(io_submit_intrvl_l_than(ds_lld, now_ktime,
			    SB_CHECK_PERIOD_MS)))
			vatomic32_init(&ds_lld->sb_dispatch_level,
				MAS_UFS_MQ_SB_LEVEL2);
		break;
	default:
		break;
	}
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &ds_lld->last_sync_io_submit_tm);
}

static inline void ufs_mq_dump_dispatch_work_stat(
	const struct mas_ufs_mq_work *work)
{
	udk_error("lst_queue_t: %ld %ld, lst_in_t: %ld %ld, lst_out_t:%ld %ld\n",
		work->last_queue_tm.tv_sec, work->last_queue_tm.tv_nsec,
		work->last_enter_tm.tv_sec, work->last_enter_tm.tv_nsec,
		work->last_exit_tm.tv_sec, work->last_exit_tm.tv_nsec);
}

#ifdef CONFIG_MAS_DEBUG_FS
#ifdef CONFIG_MAS_MQ_USING_CP
static size_t mas_queue_cp_get_enabled(struct udk_request_queue *q,
	char *buffer, size_t buffer_len)
{
	size_t offset = 0;
	struct blk_dev_lld *lld = mas_blk_get_lld(q);
	if (unlikely(!lld))
		return 0;

	offset += snprintf_s(buffer, buffer_len, buffer_len - 1, "cp_enabled: %d\n",
			   mas_blk_get_cp_flag() ? 1 : 0);

	return offset;
}

int mas_queue_cp_enabled_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize)
{
	struct udk_disk *disk = NULL;
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if  ((disk == NULL) || (disk->bd_queue == NULL)) {
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

	offset = mas_queue_cp_get_enabled(disk->bd_queue, buf, MAS_BLK_SYS_BUF_LEN);

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


int mas_queue_cp_enabled_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
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
	if (val)
		mas_blk_set_cp_flag(true);
	else
		mas_blk_set_cp_flag(false);

	if (wsize != NULL) {
		*wsize = len;
	}

	udk_free(write_buf);
	return E_HM_OK;
}

static int cp_debug_en;
static int mas_blk_cp_debug_en(void)
{
	return cp_debug_en;
}

static size_t mas_queue_cp_get_debug_en(struct udk_request_queue *q,
	char *buffer, size_t buff_len)
{
	UNUSED(q);
	size_t offset = 0;

	offset += snprintf_s(buffer, buff_len, buff_len - 1,
			"cp_debug_en: %d\n", cp_debug_en);

	return offset;
}

int mas_queue_cp_debug_en_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize)
{
	struct udk_disk *disk = NULL;
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if  ((disk == NULL) || (disk->bd_queue == NULL)) {
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

	offset = mas_queue_cp_get_debug_en(disk->bd_queue, buf, MAS_BLK_SYS_BUF_LEN);

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


int mas_queue_cp_debug_en_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
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
	if (val)
		cp_debug_en = (int)val;
	else
		cp_debug_en = 0;

	if (wsize != NULL) {
		*wsize = len;
	}

	udk_free(write_buf);
	return E_HM_OK;
}

static size_t mas_queue_cp_get_limit(struct udk_request_queue *q,
	char *buffer, size_t buff_len)
{
	ssize_t offset = 0;
	struct mas_ufs_sched_ds_lld *ds_lld = get_sched_ds_lld(q);

	if (unlikely(!ds_lld))
		return offset;

	offset += snprintf_s(buffer, buff_len, buff_len - 1, "cp_io_limit: %u\n",
		ds_lld ? ds_lld->cp_io_limit : 0U);

	return offset;
}

int mas_queue_cp_limit_show(void *ctx, void *dst, size_t pos,
	size_t size, size_t *rsize)
{
	struct udk_disk *disk = NULL;
	size_t offset;
	size_t buf_size;
	char *buf = NULL;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	disk = (struct udk_disk *)ctx;
	if  ((disk == NULL) || (disk->bd_queue == NULL)) {
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

	offset = mas_queue_cp_get_limit(disk->bd_queue, buf, MAS_BLK_SYS_BUF_LEN);

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

int mas_queue_cp_limit_store(void *ctx,
	void *src, size_t pos, size_t size, size_t *wsize)
{
	UNUSED(pos);
	struct udk_disk *disk = NULL;
	size_t len;
	char *write_buf = NULL;
	int ret;
	unsigned long val;
	struct mas_ufs_sched_ds_lld *ds_lld = NULL;

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
	ds_lld = get_sched_ds_lld(disk->bd_queue);
	if (ds_lld != NULL)
		ds_lld->cp_io_limit = val;

	if (wsize != NULL) {
		*wsize = len;
	}

	udk_free(write_buf);
	return E_HM_OK;
}
#endif /* CONFIG_MAS_MQ_USING_CP */
#endif /* CONFIG_MAS_DEBUG_FS */

static inline bool ufs_mq_rq_is_fg(const struct udk_request *rq)
{
	return (rq->mas_cmd_flags & REQ_FG_META) && !(rq->mas_cmd_flags & REQ_VIP);
}

static inline int ufs_mq_get_hp_inflt(struct mas_ufs_sched_ds_lld *ds_lld)
{
	return vatomic32_read(&ds_lld->fg_io_inflt_cnt) + vatomic32_read(&ds_lld->vip_io_inflt_cnt);
}

#ifdef CONFIG_MAS_MQ_USING_CP
static void ufs_mq_add_cp_flag(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
#ifdef CONFIG_MAS_DEBUG_FS
	if (req_cp(rq))
		mas_blk_rdr_panic("rq has cp flag set already!");
#endif

	rq->mas_req.mas_featrue_flag |= CUST_REQ_COMMAND_PRIO;
	rq->mas_cmd_flags |= REQ_CP;
	vatomic32_inc(&ds_lld->cp_io_inflt_cnt);

#ifdef CONFIG_MAS_DEBUG_FS
	if (unlikely(cp_debug_en)) {
		ufs_mq_status_dump(rq->queue, BLK_DUMP_WARNING);
	}
#endif
}

static void ufs_mq_remove_cp_flag(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	if (req_cp(rq)) {
		rq->mas_req.mas_featrue_flag &= ~CUST_REQ_COMMAND_PRIO;
#ifdef CONFIG_MAS_DEBUG_FS
		if (!vatomic32_read(&ds_lld->cp_io_inflt_cnt))
			mas_blk_rdr_panic("cp_io_inflt_cnt is zero!");
#endif
		vatomic32_dec(&ds_lld->cp_io_inflt_cnt);
	}
}

#define UFS_MQ_VIP_BURST_THRESHOLD 2
static inline bool ufs_mq_vip_inflt_burst(struct mas_ufs_sched_ds_lld *ds_lld)
{
	return vatomic32_read(&ds_lld->vip_io_inflt_cnt) > UFS_MQ_VIP_BURST_THRESHOLD;
}

static bool ufs_mq_could_add_cp_flag(
	struct udk_request *rq,
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	int ret = E_HM_INVAL;
	struct blk_dev_lld *lld = mas_blk_get_lld(rq->queue);
	struct udk_bio *oldest_bio = NULL;
	struct timespec now_ktime;
	struct timespec oldest_bio_ktime = {0};
	struct timespec cmp_ktime;

	if (!rq->bio)
		return false;

	if ((rq->mas_cmd_flags & REQ_VIP) && (udk_bio_req_op(rq->bio) != UDK_REQ_WRITE))
		return true;

	if (ufs_mq_rq_is_fg(rq) && ufs_mq_vip_inflt_burst(ds_lld))
		return false;

	if (!vatomic32_read(&ds_lld->sync_io_inflt_cnt) &&
		!vatomic32_read(&ds_lld->async_io_inflt_cnt))
		return true;

	if (!vatomic32_read(&ds_lld->cp_io_inflt_cnt))
		return true;

	if (dlist_empty(&lld->blk_idle.bio_list))
		return true;

	uspinlock_lock(&lld->blk_idle.counted_list_lock);
	if (!dlist_empty(&lld->blk_idle.bio_list)) {
		oldest_bio = dlist_first_entry(&lld->blk_idle.bio_list,
			struct udk_bio, cnt_list);
		ret = oldest_bio->mas_bio.bio_stage_ktime_ret[BIO_PROC_STAGE_SUBMIT];
		(void)memcpy_s(&oldest_bio_ktime, sizeof(struct timespec),
			&oldest_bio->mas_bio.bio_stage_ktime[BIO_PROC_STAGE_SUBMIT],
			sizeof(struct timespec));
	}
	uspinlock_unlock(&lld->blk_idle.counted_list_lock);

	if (ret != E_HM_OK)
		return false;

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &now_ktime);
	if (ret != E_HM_OK)
		return false;

	mas_time_add(&oldest_bio_ktime, MAS_BLK_CP_LVL1_MS * NSEC_PER_MSEC, &cmp_ktime);
	if (mas_time_before(&now_ktime, &cmp_ktime))
		return true;

	mas_time_add(&oldest_bio_ktime, MAS_BLK_CP_LVL2_MS * NSEC_PER_MSEC, &cmp_ktime);
	if (mas_time_before(&now_ktime, &cmp_ktime) &&
		((unsigned int)vatomic32_read(&ds_lld->cp_io_inflt_cnt) <
			 ds_lld->cp_io_limit))
		return true;

#ifdef CONFIG_MAS_DEBUG_FS
	if (unlikely(mas_blk_cp_debug_en())) {
		if (mas_time_before(&now_ktime, &cmp_ktime))
			udk_error("CP limited to %d!\n", MAS_MQ_DEFAULT_CP_LIMIT);
		else
			udk_error("Forbid CP IO!\n");
	}
#endif

	return false;
}

static inline int mas_cp_enabled(const struct udk_request_queue *q)
{
	UNUSED(q);
	return mas_blk_get_cp_flag();
}

static void ufs_mq_cp_io_limit(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	if (!mas_cp_enabled(rq->queue))
		return;

	if (ds_lld->turbo_mode)
		return;

	if (ufs_mq_could_add_cp_flag(rq, ds_lld))
		ufs_mq_add_cp_flag(rq, (struct mas_ufs_sched_ds_lld *)ds_lld);
}
#endif /* CONFIG_MAS_MQ_USING_CP */

static inline bool ufs_mq_rq_is_highprio(struct udk_request *rq)
{
	return unlikely(rq->mas_cmd_flags & REQ_FG_META);
}

static int ufs_mq_sync_io_limit(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	int ret = E_HM_OK;

	if (ufs_mq_rq_is_highprio(rq)) {
		/* Dispatch unconditionally for high priority sync io */
		if (ufs_mq_rq_is_fg(rq))
			vatomic32_inc(&ds_lld->fg_io_inflt_cnt);
		else
			vatomic32_inc(&ds_lld->vip_io_inflt_cnt);

#ifdef CONFIG_MAS_MQ_USING_CP
		ufs_mq_cp_io_limit(rq, ds_lld);
#endif
		goto out;
	}

#if defined(CONFIG_MAS_BLK_BW_OPTIMIZE) && defined(CONFIG_MAS_MQ_USING_CP)
	if (unlikely(((int)vatomic32_inc_get(&ds_lld->sync_io_inflt_cnt) >
		     ds_lld->sync_io_inflt_lmt)
			&& (!ds_lld->turbo_mode))) {
		ret = -EBUSY;
		rq->mas_req.requeue_reason = REQ_REQUEUE_IO_SW_LIMIT;
	} else {
		if (!ds_lld->async_limit_update_enable && rq->bio &&
		    mas_cp_enabled(rq->queue) && (udk_bio_req_op(rq->bio) == UDK_REQ_READ))
			ufs_mq_add_cp_flag(rq, ds_lld);
	}
#else
	if (unlikely(((int)vatomic32_inc_get(&ds_lld->sync_io_inflt_cnt) >
		     ds_lld->sync_io_inflt_lmt)
			&& (!ds_lld->turbo_mode))) {
		ret = -EBUSY;
		rq->mas_req.requeue_reason = REQ_REQUEUE_IO_SW_LIMIT;
	}
#endif
out:
	return ret;
}

static int ufs_mq_async_io_limit(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	if (unlikely(((int)vatomic32_inc_get(&ds_lld->async_io_inflt_cnt) >
			ds_lld->async_io_inflt_lmt) &&
			(!ds_lld->turbo_mode))) {
		rq->mas_req.requeue_reason = REQ_REQUEUE_IO_SW_LIMIT;

		return -EBUSY;
	}

	return E_HM_OK;
}

int ufs_mq_fs_io_limit(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	if (likely(rq->mas_cmd_flags & MAS_REQ_SYNC))
		return ufs_mq_sync_io_limit(rq, ds_lld);

	return ufs_mq_async_io_limit(rq, ds_lld);
}

void ufs_mq_rq_inflt_update(
	const struct udk_request *rq,
	struct mas_ufs_sched_ds_lld *ds_lld, bool update_complete_time)
{
	if (likely(rq->mas_cmd_flags & MAS_REQ_SYNC)) {
		if (ufs_mq_rq_is_highprio((struct udk_request *)rq)) {
#ifdef CONFIG_MAS_DEBUG_FS
			if (unlikely(!ufs_mq_get_hp_inflt(ds_lld)))
				mas_blk_rdr_panic("high_prio_sync_io is 0!");
#endif
			if (ufs_mq_rq_is_fg(rq))
				vatomic32_dec(&ds_lld->fg_io_inflt_cnt);
			else
				vatomic32_dec(&ds_lld->vip_io_inflt_cnt);
#ifdef CONFIG_MAS_MQ_USING_CP
			ufs_mq_remove_cp_flag((struct udk_request *)rq, ds_lld);
#endif
		} else {
#if defined(CONFIG_MAS_BLK_BW_OPTIMIZE) && defined(CONFIG_MAS_MQ_USING_CP)
			ufs_mq_remove_cp_flag((struct udk_request *)rq, ds_lld);
#endif
#ifdef CONFIG_MAS_DEBUG_FS
			if (!vatomic32_read(&ds_lld->sync_io_inflt_cnt))
				mas_blk_rdr_panic("sync_io_inflt_cnt is 0!");
#endif
			vatomic32_dec(&ds_lld->sync_io_inflt_cnt);
		}
		if (likely(update_complete_time))
			(void)hm_clock_gettime(CLOCK_MONOTONIC, &ds_lld->last_sync_io_compl_tm);
	} else {
#ifdef CONFIG_MAS_DEBUG_FS
		if (unlikely(!vatomic32_read(&ds_lld->async_io_inflt_cnt)))
			mas_blk_rdr_panic("async_io_inflt_cnt is 0!");
#endif
		vatomic32_dec(&ds_lld->async_io_inflt_cnt);
		if (likely(update_complete_time))
			(void)hm_clock_gettime(CLOCK_MONOTONIC, &ds_lld->last_async_io_compl_tm);
	}
}

static inline int __ufs_mq_queue_rq_internal(struct udk_request *rq,
	struct udk_request_queue *queue, struct udk_blk_mq_hw_ctx *hctx)
{
	int ret;

	ret = queue->mq_ops->queue_req(hctx, rq);

	mas_blk_latency_req_check(rq, REQ_PROC_STAGE_MQ_QUEUE_RQ);

	return ret;
}

/* for all IO except passthrough */
static int __ufs_mq_queue_rq(struct udk_request *rq,
	struct udk_request_queue *queue, struct udk_blk_mq_hw_ctx *hctx,
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	int ret;

	ret = ufs_mq_fs_io_limit(rq, (struct mas_ufs_sched_ds_lld *)ds_lld);
	if (unlikely(ret != E_HM_OK)) {
		ret = -EBUSY;
		goto exit;
	}

	ret = __ufs_mq_queue_rq_internal(rq, queue, hctx);
	if (likely(ret == E_HM_OK))
		return ret;

	rq->mas_req.requeue_reason = REQ_REQUEUE_IO_HW_LIMIT;
	if (ufs_mq_get_hp_inflt(ds_lld) +
			vatomic32_read(&ds_lld->sync_io_inflt_cnt) +
			vatomic32_read(&ds_lld->async_io_inflt_cnt) <=
		IO_HW_PENDING_THRESH)
		rq->mas_req.requeue_reason = REQ_REQUEUE_IO_HW_PENDING;

exit:
	ufs_mq_rq_inflt_update(
		rq, (struct mas_ufs_sched_ds_lld *)ds_lld, false);

	return ret;
}

/* for sync and async dispatch */
static int ufs_mq_queue_rq(
	struct udk_request *rq, struct udk_blk_mq_hw_ctx *hctx,
	struct mas_ufs_sched_ds_lld *ds_lld,
	struct udk_request_queue *q)
{
	return __ufs_mq_queue_rq(rq, q, hctx, ds_lld);
}

static void ufs_mq_run_delay_sync_list(const struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);

	udk_workqueue_add_work(mas_blk_mq_sync_disp_wq, &sched_ds->sync_dispatch_work.io_dispatch_work);
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->sync_dispatch_work.last_queue_tm);
}

static inline void ufs_mq_run_sync_list(const struct udk_request_queue *q)
{
	ufs_mq_run_delay_sync_list(q);
}

static inline bool ufs_mq_nr_before(unsigned int nr1, unsigned int nr2)
{
	return (signed int)(nr1 - nr2) < 0;
}

static void __add_vip_rq_to_list(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	struct udk_request *pos = NULL;

	if (dlist_empty(&ds_lld->hp_sync_disp_list)) {
		dlist_insert_tail(&ds_lld->hp_sync_disp_list, &rq->queuelist);
		return;
	}

	dlist_for_each_entry(pos, &ds_lld->hp_sync_disp_list, struct udk_request, queuelist) {
		if (!(pos->mas_cmd_flags & REQ_VIP)) {
			dlist_insert_tail(&pos->queuelist, &rq->queuelist);
			return;
		}
		if (dlist_get_last(&ds_lld->hp_sync_disp_list) == &pos->queuelist) {
			dlist_insert_head(&pos->queuelist, &rq->queuelist);
			return;
		}
	}
}

static void ufs_mq_insert_sync_list(struct udk_request *rq, struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	mas_blk_latency_req_check(
		(struct udk_request *)rq, REQ_PROC_STAGE_MQ_ADDTO_SYNC_LIST);
	uspinlock_lock(&ds_lld->sync_disp_lock);
	if (rq->mas_cmd_flags & REQ_VIP) {
		__add_vip_rq_to_list(rq, ds_lld);
		vatomic32_inc(&ds_lld->hp_io_list_cnt);
	} else if (rq->mas_cmd_flags & REQ_FG_META) {
		dlist_insert_tail(&rq->queuelist, &ds_lld->hp_sync_disp_list);
		vatomic32_inc(&ds_lld->hp_io_list_cnt);
	} else {
		dlist_insert_tail(&ds_lld->sync_disp_list, &rq->queuelist);
		vatomic32_inc(&ds_lld->sync_io_list_cnt);
	}
	uspinlock_unlock(&ds_lld->sync_disp_lock);
}

void ufs_mq_requeue_sync_list(
	struct udk_request *rq, struct udk_request_queue *q)
{
	ufs_mq_insert_sync_list(rq, q);
	ufs_mq_run_sync_list(q);
}

void ufs_mq_bio_is_sync(struct udk_request_queue *q, struct udk_bio *bio)
{
	unsigned long bop;
	UNUSED(q);

	bop = udk_bio_req_op(bio);
	if ((bop == UDK_REQ_READ) || (bop == UDK_REQ_FLUSH) ||
		!bio->bi_ctx.async || !bio->bi_ctx.direct_map_paddr ||
		(bio->bi_rw & UDK_REQ_FUA) || (bio->mas_bi_opf & REQ_FG_META))
		bio->mas_bi_opf |= MAS_REQ_SYNC;
	else
		bio->mas_bi_opf |= MAS_REQ_ASYNC;
}

static inline void ufs_mq_bio_to_request(struct udk_request *rq, struct udk_bio *bio,
	unsigned int nr_segs)
{
	UNUSED(nr_segs);
	udk_blk_mq_init_bio_request(rq, bio);
}

static bool ufs_mq_attempt_merge(
	const struct udk_request_queue *q, const struct udk_bio *bio)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	if (unlikely(bio->mas_bi_opf & MAS_REQ_SYNC))
		return false;

	if (likely(q->mas_queue_ops &&
		    q->mas_queue_ops->scheduler_priv)) {
		struct mas_ufs_mq_priv *priv = (struct mas_ufs_mq_priv
				*)(q->mas_queue_ops->scheduler_priv);

		if (priv->async_io_sched_strategy->async_sched_attempt_merge_fn) {
			bool merged = false;

			uspinlock_lock(&ds_lld->async_disp_lock);
			merged = priv->async_io_sched_strategy->async_sched_attempt_merge_fn(
						(struct udk_bio *)bio,
						(struct udk_request_queue *)q);
			uspinlock_unlock(&ds_lld->async_disp_lock);
			return merged;
		}
	}

	return false;
}

void ufs_mq_merge_queue_io(struct udk_blk_mq_hw_ctx *hctx,
	struct udk_blk_mq_ctx *ctx, struct udk_request *rq,
	struct udk_bio *bio)
{
	struct udk_request_queue *q = hctx->queue;

	if (!udk_bio_mergeable(bio)) {
		uspinlock_lock(&ctx->lock);
insert_rq:
		ufs_mq_req_insert(rq, hctx->queue);

		uspinlock_unlock(&ctx->lock);

		return;
	}

	uspinlock_lock(&ctx->lock);
	if (!ufs_mq_attempt_merge(q, bio))
		goto insert_rq;
	uspinlock_unlock(&ctx->lock);

	udk_blk_mq_free_request(rq);
}

static int ufs_mq_sync_queue_rq(struct udk_request *rq,
	struct udk_request_queue *queue, struct udk_blk_mq_hw_ctx *hctx,
	struct mas_ufs_mq_sched *sched_ds)
{
	return rq ? __ufs_mq_queue_rq(rq, queue, hctx, sched_ds->sched_ds_lld) : E_HM_OK;
}

static int ufs_mq_make_sync_request(struct udk_request *rq,
	struct udk_request_queue *queue, struct udk_blk_mq_hw_ctx *hctx)
{
	int ret;
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(rq->queue->mas_queue.cust_queuedata);

retry:
	ret = ufs_mq_sync_queue_rq(rq, queue, hctx, sched_ds);
	if (likely(ret == E_HM_OK)) {
		return ret;
	} else if ((ret == -EBUSY) && !udk_req_is_no_wait(rq->cmd_flags)) {
		udk_blk_mq_queue_exit(queue);
		if (queue->mq_ops->polling) {
			queue->mq_ops->polling(queue, 0);
		}
		if (!(rq->cmd_flags & UDK_REQ_FLAGS_DIO)) {
			udk_udelay(MAS_UFS_MQ_SYNC_IO_WAIT_US);
		}
		udk_blk_mq_queue_enter(queue);
		goto retry;
	}

	udk_error("blk mq queue request error=%s!\n",
				  strerror(-ret));
	udk_blk_mq_end_request(rq, ret);
	return 0;
}

int ufs_mq_make_request(struct udk_bio *bio)
{
	UNUSED(bio);
	return 0;
}

struct udk_request *ufs_mq_pick_sync_rq(
	const struct mas_ufs_sched_ds_lld *ds_lld)
{
	if (unlikely(!dlist_empty(&ds_lld->hp_sync_disp_list))) {
		vatomic32_dec((vatomic32_t *)&ds_lld->hp_io_list_cnt);
		return dlist_first_entry(
			&ds_lld->hp_sync_disp_list, struct udk_request, queuelist);
	}
	if (!dlist_empty(&ds_lld->sync_disp_list)) {
		vatomic32_dec((vatomic32_t *)&ds_lld->sync_io_list_cnt);
		return dlist_first_entry(
			&ds_lld->sync_disp_list, struct udk_request, queuelist);
	}

	return NULL;
}

static int ufs_mq_sync_dispatch(struct udk_request_queue *q)
{
	int ret = E_HM_OK;
	int err_cnt = 0;
	struct udk_request *rq = NULL;
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->sync_dispatch_work.last_enter_tm);

	do {
		uspinlock_lock(&ds_lld->sync_disp_lock);
		rq = ufs_mq_pick_sync_rq(ds_lld);
		if (unlikely(!rq)) {
			uspinlock_unlock(&ds_lld->sync_disp_lock);
			break;
		}
		dlist_delete(&rq->queuelist);
		uspinlock_unlock(&ds_lld->sync_disp_lock);

		hctx = rq->mq_hctx;
		ret = ufs_mq_queue_rq(rq, hctx, ds_lld, q);
		if (likely(ret == E_HM_OK)) {
			continue;
		} else if (ret == -EBUSY) {
			ufs_mq_insert_sync_list(rq, (struct udk_request_queue *)q);
			udk_mdelay(1);
			continue;
		}
		udk_blk_mq_free_request(rq);
		err_cnt++;
	} while (1);

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->sync_dispatch_work.last_exit_tm);

	if (err_cnt)
		ret = -EIO;

	return ret;
}

void ufs_mq_sync_io_dispatch_work_fn(const struct udk_work *work)
{
	struct mas_ufs_mq_work *mq_work = container_of(work, struct mas_ufs_mq_work, io_dispatch_work);

	(void)ufs_mq_sync_dispatch(mq_work->queue);
}

static void ufs_mq_async_sched_fifo_init(struct mas_ufs_sched_ds_lld *ds_lld)
{
	if (!ds_lld->async_io_sched_inited) {
		dlist_init(&ds_lld->async_fifo_list);
		ds_lld->async_io_sched_inited = 1;
	}
}

static void ufs_mq_async_sched_fifo_insert(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	dlist_insert_tail(&ds_lld->async_fifo_list, &rq->async_list);
}

static struct udk_request *ufs_mq_async_sched_fifo_seek(
	const struct mas_ufs_sched_ds_lld *ds_lld)
{
	struct udk_request *cur_req = NULL;

	if (!dlist_empty(&ds_lld->async_fifo_list)) {
		cur_req = dlist_first_entry(&ds_lld->async_fifo_list,
			struct udk_request, async_list);
		dlist_delete(&cur_req->async_list);
		dlist_init(&cur_req->async_list);
		return cur_req;
	}
	return NULL;
}

static void ufs_mq_async_sched_fifo_requeue(
	struct udk_request *rq, struct mas_ufs_sched_ds_lld *ds_lld)
{
	dlist_insert_head(&rq->async_list, &ds_lld->async_fifo_list);
}

static bool ufs_mq_async_sched_fifo_attempt_merge_bio(
	struct udk_bio *bio, struct udk_request_queue *q)
{
	struct udk_request *rq = NULL;
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	dlist_for_each_entry_reverse(rq, &ds_lld->async_fifo_list,
		struct udk_request, async_list) {
		int el_ret;

		if (!udk_sched_bio_merge_ok(rq, bio))
			continue;
		el_ret = udk_blk_try_merge(rq, bio);
		if (el_ret == UDK_IO_SCHEDULER_BACK_MERGE) {
			if (udk_bio_attempt_back_merge(rq, bio) == UDK_BIO_MERGE_OK)
				return true;
		} else if (el_ret == UDK_IO_SCHEDULER_FRONT_MERGE) {
			if (udk_bio_attempt_front_merge(rq, bio) == UDK_BIO_MERGE_OK)
				return true;
		}
	}

	return false;
}

static inline bool ufs_mq_async_sched_fifo_empty(
	const struct mas_ufs_sched_ds_lld *ds_lld)
{
	return dlist_empty(&ds_lld->async_fifo_list);
}

static struct mas_ufs_mq_async_sched mas_ufs_mq_async_io_fifo_sched = {
	.type = MAS_UFS_MQ_SCHED_ASYNC_FIFO,
	.async_sched_init_fn = ufs_mq_async_sched_fifo_init,
	.async_sched_insert_fn = ufs_mq_async_sched_fifo_insert,
	.async_sched_seek_fn = ufs_mq_async_sched_fifo_seek,
	.async_sched_requeue_fn = ufs_mq_async_sched_fifo_requeue,
	.async_sched_attempt_merge_fn =
		ufs_mq_async_sched_fifo_attempt_merge_bio,
	.async_sched_is_empty_fn = ufs_mq_async_sched_fifo_empty,
};

static void ufs_mq_async_dispatch(struct udk_request_queue *q);

struct async_work_data {
	struct udk_request_queue *q;
	struct udk_work work;
};

static void ufs_mq_run_delay_async_list(
	struct udk_request_queue *q)
{
	int ret;
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct async_work_data *async_work = udk_malloc(sizeof(struct async_work_data));

	if (!async_work)
		goto async_dispatch;

	ret = udk_work_init(&async_work->work, __cfi_ufs_mq_async_io_dispatch_work_fn, 1);
	if (ret < 0) {
		udk_error("udk_work_init fail\n");
		goto async_dispatch;
	}
	async_work->q = q;

	ret = udk_workqueue_add_work(mas_blk_mq_async_disp_wq, &async_work->work);
	if (ret < 0) {
		udk_error("udk_workqueue_add_work fail\n");
		goto async_dispatch;
	}
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->async_dispatch_work.last_queue_tm);
	return;

async_dispatch:
	udk_free(async_work);
	ufs_mq_async_dispatch(q);
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->async_dispatch_work.last_queue_tm);
}

static inline void ufs_mq_run_async_list(struct udk_request_queue *q)
{
	ufs_mq_run_delay_async_list(q);
}

static void ufs_mq_insert_async_list(
	const struct udk_request *rq, const struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;
	struct mas_ufs_mq_priv *priv = NULL;

	if (!q->mas_queue_ops || !q->mas_queue_ops->scheduler_priv)
		return;

	priv = (struct mas_ufs_mq_priv *)(q->mas_queue_ops->scheduler_priv);
	if (!priv->async_io_sched_strategy->async_sched_insert_fn)
		return;

	mas_blk_latency_req_check(
		(struct udk_request *)rq, REQ_PROC_STAGE_MQ_ADDTO_ASYNC_LIST);
	uspinlock_lock(&ds_lld->async_disp_lock);
	priv->async_io_sched_strategy->async_sched_insert_fn(
		(struct udk_request *)rq, ds_lld);
	vatomic32_inc(&ds_lld->async_io_list_cnt);
	uspinlock_unlock(&ds_lld->async_disp_lock);
}

void ufs_mq_async_requeue(
	const struct udk_request *rq, const struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;
	struct mas_ufs_mq_priv *priv = NULL;

	if (!q->mas_queue_ops || !q->mas_queue_ops->scheduler_priv)
		return;

	priv = (struct mas_ufs_mq_priv *)(q->mas_queue_ops->scheduler_priv);
	if (!priv->async_io_sched_strategy->async_sched_requeue_fn)
		return;

	mas_blk_latency_req_check(
		(struct udk_request *)rq, REQ_PROC_STAGE_MQ_ADDTO_ASYNC_LIST);
	uspinlock_lock(&ds_lld->async_disp_lock);
	priv->async_io_sched_strategy->async_sched_requeue_fn(
		(struct udk_request *)rq, ds_lld);
	vatomic32_inc(&ds_lld->async_io_list_cnt);
	uspinlock_unlock(&ds_lld->async_disp_lock);
}

static bool ufs_mq_async_dispatch_work_trigger_judgement(
	const struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;
	struct mas_ufs_mq_priv *priv = NULL;
	bool result = false;

	if (!q->mas_queue_ops || !q->mas_queue_ops->scheduler_priv)
		goto out;

	priv = (struct mas_ufs_mq_priv *)(q->mas_queue_ops->scheduler_priv);
	if (!priv->async_io_sched_strategy->async_sched_is_empty_fn)
		goto out;

	uspinlock_lock(&ds_lld->async_disp_lock);
	result = priv->async_io_sched_strategy->async_sched_is_empty_fn(ds_lld);
	uspinlock_unlock(&ds_lld->async_disp_lock);
	return !result;
out:
	mas_blk_rdr_panic("async_dispatch_work_trigger_judgement error!");

	return true;
}

struct udk_request *ufs_mq_seek_request(const struct udk_request_queue *q)
{
	struct udk_request *cur_req = NULL;
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;
	struct mas_ufs_mq_priv *priv = NULL;

	if (!q->mas_queue_ops || !q->mas_queue_ops->scheduler_priv)
		return NULL;

	priv = (struct mas_ufs_mq_priv *)(q->mas_queue_ops->scheduler_priv);
	if (!priv->async_io_sched_strategy->async_sched_seek_fn)
		return NULL;

	uspinlock_lock(&ds_lld->async_disp_lock);
	cur_req = priv->async_io_sched_strategy->async_sched_seek_fn(ds_lld);
	if (cur_req)
		vatomic32_dec(&ds_lld->async_io_list_cnt);
	uspinlock_unlock(&ds_lld->async_disp_lock);
	return cur_req;
}

static void ufs_mq_async_inflt_lmt_update(
	struct mas_ufs_sched_ds_lld *ds_lld, int limit)
{
	if (ds_lld->async_io_inflt_lmt == limit)
		return;

#ifdef CONFIG_MAS_BLK_BW_OPTIMIZE
	uspinlock_lock(&ds_lld->async_limit_update_lock);
	if (ds_lld->async_limit_update_enable)
		ds_lld->async_io_inflt_lmt = limit;
	uspinlock_unlock(&ds_lld->async_limit_update_lock);
#else

	ds_lld->async_io_inflt_lmt = limit;
#endif

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &ds_lld->last_async_io_inflt_lmt_update_tm);
}

static inline struct udk_blk_mq_tag_set *get_tag_set_from_ds_lld(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	return container_of(ds_lld->lld, struct udk_blk_mq_tag_set, lld_func);
}

#define UFS_MQ_ASYNC_LMT_CALC_THRESH 4
static void ufs_mq_async_normal_mode_inflt_lmt_calc(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	unsigned int limit;
	unsigned int resrvd_tag_remain =
		get_tag_set_from_ds_lld(ds_lld)->prio_info[MAS_RESERVED_PRIO].tag_num -
		(unsigned int)vatomic32_read(&ds_lld->mq_resrvd_tag_used_cnt);

	switch (vatomic32_read(&ds_lld->sb_dispatch_level)) {
	case MAS_UFS_MQ_SB_NORMAL:
		limit = resrvd_tag_remain < UFS_MQ_ASYNC_LMT_CALC_THRESH
				? ASYNC_DISP_LMT_HI_PRESSURE
				: ASYNC_DISP_LMT;
		ufs_mq_async_inflt_lmt_update(ds_lld, limit);
		break;
	case MAS_UFS_MQ_SB_LEVEL1:
		ufs_mq_async_inflt_lmt_update(ds_lld, ASYNC_DISP_LMT_SB1);
		break;
	case MAS_UFS_MQ_SB_LEVEL2:
		ufs_mq_async_inflt_lmt_update(ds_lld, ASYNC_DISP_LMT_SB2);
		break;
	case MAS_UFS_MQ_SB_LONG:
		limit = resrvd_tag_remain < UFS_MQ_ASYNC_LMT_CALC_THRESH
				? ASYNC_DISP_LMT_SB_LONG_HP
				: ASYNC_DISP_LMT_SB_LONG;
		ufs_mq_async_inflt_lmt_update(ds_lld, limit);
		break;
	default:
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic("unknown sb_dispatch_level!");
#else
		limit = resrvd_tag_remain < UFS_MQ_ASYNC_LMT_CALC_THRESH
				? ASYNC_DISP_LMT_HI_PRESSURE
				: ASYNC_DISP_LMT;
		ufs_mq_async_inflt_lmt_update(ds_lld, limit);
#endif
	}
}

static void ufs_mq_async_fwb_mode_inflt_lmt_calc(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	switch (vatomic32_read(&ds_lld->sb_dispatch_level)) {
	case MAS_UFS_MQ_SB_NORMAL:
		ufs_mq_async_inflt_lmt_update(
			ds_lld, ASYNC_DISP_LMT_HI_PRESSURE);
		break;
	case MAS_UFS_MQ_SB_LEVEL1:
		ufs_mq_async_inflt_lmt_update(ds_lld, ASYNC_DISP_LMT_SB1_FWB);
		break;
	case MAS_UFS_MQ_SB_LEVEL2:
		ufs_mq_async_inflt_lmt_update(ds_lld, ASYNC_DISP_LMT_SB2_FWB);
		break;
	case MAS_UFS_MQ_SB_LONG:
		ufs_mq_async_inflt_lmt_update(
			ds_lld, ASYNC_DISP_LMT_SB_LONG_HP);
		break;
	default:
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic("unknown sb_dispatch_level!");
#else
		ufs_mq_async_inflt_lmt_update(
			ds_lld, ASYNC_DISP_LMT_HI_PRESSURE);
#endif
	}
}

static inline void ufs_mq_async_lm_mode_inflt_lmt_calc(
	const struct mas_ufs_sched_ds_lld *ds_lld)
{
	ufs_mq_async_inflt_lmt_update((struct mas_ufs_sched_ds_lld *)ds_lld,
		ASYNC_DISP_LMT_HI_PRESSURE);
}
#define VM_DIRTY_RATIO_THRESH 50
#define VM_DIRTY_RATIO_ADJUST 10
static enum mas_ufs_mq_async_dispatch_mode ufs_mq_async_dispatch_mode_choose(
	void)
{
#ifdef CONFIG_MM_PAGECACHE_HELPER
	unsigned long fg_thresh;
	unsigned long dirty;

	if (pch_lowmem_check2() > 0)
		return MAS_UFS_MQ_ASYNC_LM_MODE;

	if (likely(vm_dirty_ratio <= VM_DIRTY_RATIO_THRESH))
		fg_thresh = (((unsigned long)vm_dirty_ratio + /*lint !e571 */
				     VM_DIRTY_RATIO_ADJUST) *
				    global_dirtyable_memory()) / ONE_HUNDRED;
	else
		fg_thresh = ((unsigned long)vm_dirty_ratio * /*lint !e571 */
				    global_dirtyable_memory()) / ONE_HUNDRED;

	dirty = global_node_page_state(NR_FILE_DIRTY) +
	      global_node_page_state(NR_WRITEBACK);
	return (enum mas_ufs_mq_async_dispatch_mode)(
		dirty <= fg_thresh ? MAS_UFS_MQ_ASYNC_NORMAL_MODE
				   : MAS_UFS_MQ_ASYNC_FWB_MODE);
#else
	return MAS_UFS_MQ_ASYNC_NORMAL_MODE;
#endif
}

static void ufs_mq_async_disp_inflt_lmt_decision(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	ds_lld->async_dispatch_mode = ufs_mq_async_dispatch_mode_choose();
	switch (ds_lld->async_dispatch_mode) {
	case MAS_UFS_MQ_ASYNC_NORMAL_MODE:
		ufs_mq_async_normal_mode_inflt_lmt_calc(ds_lld);
		break;
	case MAS_UFS_MQ_ASYNC_FWB_MODE:
		ufs_mq_async_fwb_mode_inflt_lmt_calc(ds_lld);
		break;
	case MAS_UFS_MQ_ASYNC_LM_MODE:
		ufs_mq_async_lm_mode_inflt_lmt_calc(ds_lld);
		break;
	default:
#ifdef CONFIG_MAS_DEBUG_FS
		mas_blk_rdr_panic("unknown async_dispatch_mode!");
#else
		ufs_mq_async_fwb_mode_inflt_lmt_calc(ds_lld);
#endif
	}
}

void ufs_mq_write_throttle_check_timer_expire(struct mas_ufs_sched_ds_lld *sched_ds_lld)
{
	uspinlock_lock(&sched_ds_lld->async_limit_update_lock);
	sched_ds_lld->async_limit_update_enable = 1;
	uspinlock_unlock(&sched_ds_lld->async_limit_update_lock);
	ufs_mq_async_disp_inflt_lmt_decision(sched_ds_lld);
}

void ufs_mq_write_throttle_handler(struct udk_request_queue *q, bool level)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *sched_ds_lld = sched_ds->sched_ds_lld;

	uspinlock_lock(&sched_ds_lld->async_limit_update_lock);
	sched_ds_lld->async_limit_update_enable = 0;
	switch ((int)level) {
	case ASYNC_THROT_STAGE_ONE:
		sched_ds_lld->async_io_inflt_lmt = ASYNC_STAGE_ONE_LIMIT;
		break;
	case ASYNC_THROT_STAGE_TWO:
		sched_ds_lld->async_io_inflt_lmt = ASYNC_STAGE_TWO_LIMIT;
		break;
	default:
		udk_error("%s: level = %d\n", __func__, level);
		break;
	}
	uspinlock_unlock(&sched_ds_lld->async_limit_update_lock);

	raw_mutex_lock(&sched_ds_lld->write_throttle_check_timer_lock);
	udk_timer_del_sync(&sched_ds_lld->write_throttle_check_timer);
	udk_timer_add(&sched_ds_lld->write_throttle_check_timer);
	raw_mutex_unlock(&sched_ds_lld->write_throttle_check_timer_lock);
}

static void ufs_mq_async_end_bio(struct udk_request *cur_req, int error)
{
	struct udk_bio *bio = NULL;
	struct udk_bio *next = NULL;

	for (bio = cur_req->bio; bio != NULL; bio = next) {
		next = bio->next;
		bio->bi_error = error;
		udk_bio_end(bio);
	}
}

static void ufs_mq_async_dispatch(struct udk_request_queue *q)
{
	int ret;
	struct udk_blk_mq_hw_ctx *hctx = NULL;
	struct udk_request *cur_req = NULL;
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->async_dispatch_work.last_enter_tm);

	do {
		ufs_mq_async_disp_inflt_lmt_decision(ds_lld);
		cur_req = ufs_mq_seek_request(q);
		if (!cur_req)
			break;

		hctx = cur_req->mq_hctx;
		ret = ufs_mq_queue_rq(cur_req, hctx, ds_lld, q);
		if (likely(ret == E_HM_OK)) {
			continue;
		} else if (ret == -EBUSY) {
			ufs_mq_async_requeue(cur_req, q);
			udk_mdelay(1);
			continue;
		}

		udk_error("blk mq queue request error=%s!\n",
				  strerror(-ret));

		ufs_mq_async_end_bio(cur_req, -EIO);
		udk_blk_mq_free_request(cur_req);
	} while (1);

	(void)hm_clock_gettime(CLOCK_MONOTONIC, &sched_ds->async_dispatch_work.last_exit_tm);
}

void ufs_mq_async_io_dispatch_work_fn(struct udk_work *work_p)
{
	struct async_work_data *async_work = container_of(work_p, struct async_work_data, work);

	ufs_mq_async_dispatch(async_work->q);
	udk_work_destroy(work_p);
	udk_free(async_work);
}

static void ufs_mq_io_guard_queue(struct mas_ufs_mq_sched *sched_ds)
{
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	if (vatomic32_read(&ds_lld->mq_resrvd_tag_used_cnt) &&
		!vatomic32_read(&ds_lld->async_io_inflt_cnt))
		(void)ufs_mq_run_async_list(sched_ds->q);
}

void ufs_mq_io_guard_work_fn(struct udk_work *work)
{
	struct mas_ufs_mq_sched *sched_ds = NULL;

	uspinlock_lock(&io_guard_queue_list_lock);
	dlist_for_each_entry(
		sched_ds, &mas_io_guard_queue_list, struct mas_ufs_mq_sched, io_guard_list_node) {
		uspinlock_unlock(&io_guard_queue_list_lock);
		ufs_mq_io_guard_queue(sched_ds);
		uspinlock_lock(&io_guard_queue_list_lock);
	}
	uspinlock_unlock(&io_guard_queue_list_lock);

	udk_work_destroy(work);
	udk_mdelay(MAS_BLK_IO_GUARD_PERIOD_MS);

	uspinlock_lock(&io_guard_queue_list_lock);
	if (!dlist_empty(&mas_io_guard_queue_list))
		udk_workqueue_add_work(mas_blk_io_guard_wq, &mas_io_guard_work);
	uspinlock_unlock(&io_guard_queue_list_lock);
}

void ufs_mq_req_init(const struct udk_blk_mq_ctx *ctx, struct udk_request *rq)
{
#ifdef CONFIG_MAS_DEBUG_FS
	if (unlikely(vatomic32_read(&rq->mas_req.req_used)))
		mas_blk_rdr_panic("Reinit unreleased request!");

	vatomic32_init(&rq->mas_req.req_used, 1);
#endif
	rq->mas_req.mq_ctx_generate = (struct udk_blk_mq_ctx *)ctx;
	rq->mas_req.mas_featrue_flag = 0;
}

static inline bool req_complete_on_diff_cpu(unsigned int req_cpu,
	unsigned int raw_cpu)
{
	return req_cpu != raw_cpu;
}

void ufs_mq_req_complete(
	struct udk_request *rq, const struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = sched_ds->sched_ds_lld;

	ufs_mq_rq_inflt_update(rq, ds_lld, true);
}

void ufs_mq_req_deinit(struct udk_request *rq)
{
#ifdef CONFIG_MAS_DEBUG_FS
	vatomic32_init(&rq->mas_req.req_used, 0);
#endif
}

void ufs_mq_req_insert(struct udk_request *req, struct udk_request_queue *q)
{
	if (likely(req->mas_cmd_flags & MAS_REQ_SYNC))
		ufs_mq_insert_sync_list(req, q);
	else
		ufs_mq_insert_async_list(req, q);
}

void ufs_mq_req_requeue(
	struct udk_request *req, const struct udk_request_queue *q)
{
	ufs_mq_req_insert(req, (struct udk_request_queue *)q);
}

void ufs_mq_exec_queue(struct udk_request_queue *q)
{
	int ret;

	ret = (int)ufs_mq_async_dispatch_work_trigger_judgement(q);
	if (ret)
		ufs_mq_run_async_list(q);
}

void ufs_mq_run_hw_queue(struct udk_request_queue *q)
{
	ufs_mq_run_sync_list(q);
	if (ufs_mq_async_dispatch_work_trigger_judgement(q))
		ufs_mq_run_async_list(q);
}

void ufs_mq_run_requeue(struct udk_request_queue *q)
{
	ufs_mq_run_sync_list(q);
	if (ufs_mq_async_dispatch_work_trigger_judgement(q))
		ufs_mq_run_async_list(q);
}

bool ufs_order_panic_wait_datasync_handle(struct blk_dev_lld *blk_lld)
{
	bool need_flush = false;

	struct mas_ufs_sched_ds_lld *sched_ds_lld =
			(struct mas_ufs_sched_ds_lld *)blk_lld->sched_ds_lld;

	if (vatomic32_read(&(sched_ds_lld->mq_prio_tag_used_cnt)) +
			vatomic32_read(&(sched_ds_lld->mq_tag_used_cnt)) > 0) {
		sched_ds_lld->turbo_mode = true;
	} else {
		sched_ds_lld->turbo_mode = false;
		need_flush = true;
	}

	return need_flush;
}

static void ufs_turbo_check_timer_expire(struct mas_ufs_sched_ds_lld *sched_ds_lld)
{
	static int check_count = 9000 / MAS_BLK_TURBO_CHECK_PERIOD_MS;

	udk_error("%s: check_count = %d turbo_mode = %d prio io = %d, sync io = %d "
		"fg_inflt: %d, vip_inflt: %d, s_inflt: %d a_inflt: %d\n",
		__func__, check_count, sched_ds_lld->turbo_mode,
		vatomic32_read(&(sched_ds_lld->mq_prio_tag_used_cnt)),
		vatomic32_read(&(sched_ds_lld->mq_tag_used_cnt)),
		vatomic32_read(&sched_ds_lld->fg_io_inflt_cnt),
		vatomic32_read(&sched_ds_lld->vip_io_inflt_cnt),
		vatomic32_read(&(sched_ds_lld->sync_io_inflt_cnt)),
		vatomic32_read(&(sched_ds_lld->async_io_inflt_cnt)));
	if (sched_ds_lld->lld->dump_fn)
		sched_ds_lld->lld->dump_fn(
			mas_blk_get_queue_by_lld(sched_ds_lld->lld), BLK_DUMP_WARNING);

	if (check_count) {
		if (vatomic32_read(&(sched_ds_lld->mq_prio_tag_used_cnt)) +
			vatomic32_read(&(sched_ds_lld->mq_tag_used_cnt)) > 0) {
			sched_ds_lld->turbo_mode = true;
		} else {
			blk_power_off_flush(1);
			sched_ds_lld->turbo_mode = false;
		}
		check_count--;

		raw_mutex_lock(&sched_ds_lld->turbo_timer_lock);
		udk_timer_del_sync(&sched_ds_lld->turbo_timer);
		udk_timer_add(&sched_ds_lld->turbo_timer);
		raw_mutex_unlock(&sched_ds_lld->turbo_timer_lock);
	} else {
		sched_ds_lld->turbo_mode = false;
		check_count = 9000 / MAS_BLK_TURBO_CHECK_PERIOD_MS;
	}
}

static void __ufs_turbo_check_timer_expire(unsigned long val)
{
	struct mas_ufs_sched_ds_lld *ds_lld = (struct mas_ufs_sched_ds_lld *)val;
	ufs_turbo_check_timer_expire(ds_lld);
}

void ufs_tagset_power_off_proc(struct blk_dev_lld *lld)
{
	struct mas_ufs_sched_ds_lld *sched_ds_lld = lld->sched_ds_lld;

	sched_ds_lld->turbo_mode = true;

	raw_mutex_lock(&sched_ds_lld->turbo_timer_lock);
	udk_timer_del_sync(&sched_ds_lld->turbo_timer);
	udk_timer_add(&sched_ds_lld->turbo_timer);
	raw_mutex_unlock(&sched_ds_lld->turbo_timer_lock);
}

void ufs_mq_status_dump(const struct udk_request_queue *q, enum blk_dump_scene s)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);
	struct mas_ufs_sched_ds_lld *ds_lld = NULL;
	char *prefix = mas_blk_prefix_str(s);

	if (!sched_ds || !(sched_ds->sched_ds_lld))
		return;

	ds_lld = sched_ds->sched_ds_lld;
	udk_error("%s: vip_wait_cnt: %d\n",
		prefix, vatomic32_read(&ds_lld->vip_wait_cnt));
	udk_error("%s: h_tag_used_cnt: %d tag_used_cnt: %d r_tag_used_cnt: %d\n",
		prefix, vatomic32_read(&ds_lld->mq_prio_tag_used_cnt),
		vatomic32_read(&ds_lld->mq_tag_used_cnt),
		vatomic32_read(&ds_lld->mq_resrvd_tag_used_cnt));
	udk_error("%s: fg_inflt: %d, vip_inflt: %d, s_inflt: %d a_inflt: %d, cp_inflt: %d, cpp_inflt: %d\n",
		prefix, vatomic32_read(&ds_lld->fg_io_inflt_cnt),
		vatomic32_read(&ds_lld->vip_io_inflt_cnt),
		vatomic32_read(&ds_lld->sync_io_inflt_cnt),
		vatomic32_read(&ds_lld->async_io_inflt_cnt),
		vatomic32_read(&ds_lld->cp_io_inflt_cnt),
		vatomic32_read(&ds_lld->cpp_io_inflt_cnt));
}

static void ufs_mq_ktime_init(struct mas_ufs_sched_ds_lld *ds_lld)
{
	(void)memset_s(&ds_lld->last_sync_io_submit_tm, sizeof(struct timespec),
		0, sizeof(struct timespec));
	(void)memset_s(&ds_lld->last_sync_burst_trig_tm, sizeof(struct timespec),
		0, sizeof(struct timespec));
	(void)memset_s(&ds_lld->last_sync_io_compl_tm, sizeof(struct timespec),
		0, sizeof(struct timespec));
	(void)memset_s(&ds_lld->last_async_io_compl_tm, sizeof(struct timespec),
		0, sizeof(struct timespec));
	(void)memset_s(&ds_lld->last_async_io_inflt_lmt_update_tm, sizeof(struct timespec),
		0, sizeof(struct timespec));
}

static void ufs_mq_tag_used_cnt_init(struct mas_ufs_sched_ds_lld *ds_lld)
{
	vatomic32_init(&ds_lld->mq_tag_used_cnt, 0);
	vatomic32_init(&ds_lld->mq_resrvd_tag_used_cnt, 0);
	vatomic32_init(&ds_lld->mq_prio_tag_used_cnt, 0);
	vatomic32_init(&ds_lld->vip_wait_cnt, 0);
}

static void ufs_mq_inflt_cnt_init(struct mas_ufs_sched_ds_lld *ds_lld)
{
	vatomic32_init(&ds_lld->cpp_io_inflt_cnt, 0);
	vatomic32_init(&ds_lld->cp_io_inflt_cnt, 0);
	vatomic32_init(&ds_lld->async_io_inflt_cnt, 0);
	vatomic32_init(&ds_lld->fg_io_inflt_cnt, 0);
	vatomic32_init(&ds_lld->vip_io_inflt_cnt, 0);
	vatomic32_init(&ds_lld->sync_io_inflt_cnt, 0);
}

static void ufs_mq_inflt_lmt_init(struct mas_ufs_sched_ds_lld *ds_lld)
{
	ds_lld->cp_io_limit = MAS_MQ_DEFAULT_CP_LIMIT;
	ds_lld->sync_io_inflt_lmt = MAS_MQ_SYNC_DISPATCH_LIMIT;
	ds_lld->async_io_inflt_lmt = ASYNC_DISP_LMT;
}

/* define 1 worker and 1 queue to process block works */
#define IOSCHED_WORK_NUM_DEFAULT 1
#define IOSCHED_WORK_PRIO_NUM 1
/* define workqueue priority 80 */
#define IOSCHED_ASYNC_QUEUE_PRIORITY 80

static int ufs_mq_workqueue_init(void)
{
	struct udk_workqueue_attr attr = {
		.nr_workers = IOSCHED_WORK_NUM_DEFAULT,
		.nr_queues = IOSCHED_WORK_PRIO_NUM,
		.thread_priority = HM_PRIORITY_USER_MAX,
		.name = "sync_dispatch"
	};

	if (mas_blk_mq_sync_disp_wq)
		return 0;

	mas_blk_mq_sync_disp_wq = udk_workqueue_alloc(&attr);
	if (!mas_blk_mq_sync_disp_wq) {
		udk_error("%s Failed to alloc sync_dispatch_workqueue\n", __func__);
		return -ENOMEM;
	}

	(void)memcpy_s(attr.name, UDK_THREADPOOL_NAME_LEN,
		"async_dispatch", strlen("async_dispatch") + 1);
	attr.thread_priority = IOSCHED_ASYNC_QUEUE_PRIORITY;
	mas_blk_mq_async_disp_wq = udk_workqueue_alloc(&attr);
	if (!mas_blk_mq_async_disp_wq)
		goto destroy_sync_wkq;

	(void)memcpy_s(attr.name, UDK_THREADPOOL_NAME_LEN,
		"io_guard", strlen("io_guard") + 1);
	mas_blk_io_guard_wq =
		udk_workqueue_alloc(&attr);
	if (!mas_blk_io_guard_wq)
		goto destroy_async_wkq;

	udk_work_init(&mas_io_guard_work, __cfi_ufs_mq_io_guard_work_fn, 0);
	return 0;

destroy_async_wkq:
	udk_workqueue_destroy(mas_blk_mq_async_disp_wq);
destroy_sync_wkq:
	udk_workqueue_destroy(mas_blk_mq_sync_disp_wq);

	mas_blk_mq_sync_disp_wq = NULL;
	mas_blk_mq_async_disp_wq = NULL;
	mas_blk_io_guard_wq = NULL;

	udk_error("%s init Failed!\n", __func__);
	return -ENOMEM;
}

static void ufs_mq_dispatch_list_init(
	struct mas_ufs_sched_ds_lld *ds_lld)
{
	dlist_init(&ds_lld->hp_sync_disp_list);
	dlist_init(&ds_lld->sync_disp_list);

	uspinlock_init(&ds_lld->sync_disp_lock);
	uspinlock_init(&ds_lld->async_disp_lock);

	vatomic32_init(&ds_lld->hp_io_list_cnt, 0);
	vatomic32_init(&ds_lld->sync_io_list_cnt, 0);
	vatomic32_init(&ds_lld->async_io_list_cnt, 0);
}

/* Initial sched_ds_lld per lld */
static struct mas_ufs_sched_ds_lld *ufs_mq_sched_ds_lld_init(
	const struct udk_request_queue *q)
{
	struct blk_dev_lld *lld = mas_blk_get_lld((struct udk_request_queue *)q);
	struct mas_ufs_sched_ds_lld *ds_lld = NULL;

	if (unlikely(!lld))
		return NULL;

	if (lld->sched_ds_lld_inited) {
		ds_lld = (struct mas_ufs_sched_ds_lld *)lld->sched_ds_lld;
		vatomic32_inc(&ds_lld->ref_cnt);
		return (struct mas_ufs_sched_ds_lld *)lld->sched_ds_lld;
	}

	ds_lld = mas_zmalloc(sizeof(struct mas_ufs_sched_ds_lld));
	if (unlikely(!ds_lld))
		return NULL;

	vatomic32_init(&ds_lld->ref_cnt, 1);
	udk_timer_init(&ds_lld->turbo_timer, __ufs_turbo_check_timer_expire,
		(unsigned long)(uintptr_t)ds_lld, MAS_BLK_TURBO_CHECK_PERIOD_MS);
	raw_mutex_init(&ds_lld->turbo_timer_lock);
	ds_lld->turbo_mode = false;
	ufs_mq_tag_used_cnt_init(ds_lld);
	ufs_mq_dispatch_list_init(ds_lld);
	ufs_mq_inflt_lmt_init(ds_lld);
	ufs_mq_inflt_cnt_init(ds_lld);
	ufs_mq_ktime_init(ds_lld);

	udk_timer_init(&ds_lld->sb_dispatch_check_timer,
		__cfi_ufs_mq_sync_burst_check_timer_expire,
		(unsigned long)(uintptr_t)ds_lld, SB_CHECK_PERIOD_MS);
	raw_mutex_init(&ds_lld->sb_dispatch_check_timer_lock);

#ifdef CONFIG_MAS_BLK_BW_OPTIMIZE
	udk_timer_init(&ds_lld->write_throttle_check_timer,
		__cfi_ufs_mq_write_throttle_check_timer_expire,
		(unsigned long)(uintptr_t)ds_lld, ASYNC_WRITE_CHECK_MS);
	raw_mutex_init(&ds_lld->write_throttle_check_timer_lock);
	uspinlock_init(&ds_lld->async_limit_update_lock);
	ds_lld->async_limit_update_enable = 1;
#endif

	vatomic32_init(&ds_lld->sb_dispatch_level, MAS_UFS_MQ_SB_NORMAL);

	ds_lld->async_dispatch_mode = MAS_UFS_MQ_ASYNC_NORMAL_MODE;

	if (ufs_mq_workqueue_init())
		goto free_sched_ds_lld;

	ds_lld->lld = lld;
	lld->sched_ds_lld = ds_lld;
	lld->sched_ds_lld_inited = true;

	if (lld->type == BLK_LLD_TAGSET_BASE)
		mas_blk_flush_list_register(&lld->lld_list);

	return ds_lld;

free_sched_ds_lld:
	udk_free(ds_lld);
	return NULL;
}

static void ufs_mq_sched_ds_lld_exit(const struct udk_request_queue *q)
{
	struct blk_dev_lld *lld = mas_blk_get_lld((struct udk_request_queue *)q);
	struct mas_ufs_sched_ds_lld *ds_lld = NULL;

	if (unlikely(!lld || !lld->sched_ds_lld))
		return;

	ds_lld = lld->sched_ds_lld;

	if (lld->type == BLK_LLD_TAGSET_BASE)
		mas_blk_flush_list_unregister(&lld->lld_list);

	if (lld->sched_ds_lld_inited &&
		vatomic32_dec_get(&ds_lld->ref_cnt)) {
		lld->sched_ds_lld_inited = false;
		udk_free(lld->sched_ds_lld);
		lld->sched_ds_lld = NULL;
	}
}

int ufs_mq_iosched_init(struct udk_request_queue *q)
{
	struct mas_ufs_sched_ds_lld *ds_lld = NULL;
	struct mas_ufs_mq_sched *sched_ds = mas_zmalloc(sizeof(struct mas_ufs_mq_sched));
	struct mas_ufs_mq_priv *priv = NULL;

	if (!sched_ds) {
		udk_error("%s Failed to alloc sched_ds!\n", __func__);
		return -ENOMEM;
	}

	ds_lld = ufs_mq_sched_ds_lld_init(q);
	if (!ds_lld)
		goto free_sched_ds;

	if (!q->mas_queue_ops || !q->mas_queue_ops->scheduler_priv)
		goto sched_ds_lld_exit;

	priv = (struct mas_ufs_mq_priv *)(q->mas_queue_ops->scheduler_priv);
	if (priv->async_io_sched_strategy->async_sched_init_fn) {
		uspinlock_lock(&ds_lld->async_disp_lock);
		priv->async_io_sched_strategy->async_sched_init_fn(ds_lld);
		uspinlock_unlock(&ds_lld->async_disp_lock);
	}

	udk_work_init(&sched_ds->sync_dispatch_work.io_dispatch_work,
		__cfi_ufs_mq_sync_io_dispatch_work_fn, 1);
	sched_ds->sync_dispatch_work.queue = q;

	sched_ds->q = q;
	sched_ds->sched_ds_lld = ds_lld;
	q->mas_queue.cust_queuedata = (void *)sched_ds;

	return 0;

sched_ds_lld_exit:
	ufs_mq_sched_ds_lld_exit(q);
free_sched_ds:
	udk_free(sched_ds);
	return -ENOMEM;
}

void ufs_mq_iosched_exit(struct udk_request_queue *q)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(q->mas_queue.cust_queuedata);

	if (!sched_ds)
		return;

	udk_workqueue_cancel_work(mas_blk_mq_sync_disp_wq,
		&sched_ds->sync_dispatch_work.io_dispatch_work);
	udk_workqueue_cancel_work(mas_blk_mq_async_disp_wq,
		&sched_ds->async_dispatch_work.io_dispatch_work);

	uspinlock_lock(&io_guard_queue_list_lock);
	dlist_delete(&sched_ds->io_guard_list_node);
	uspinlock_unlock(&io_guard_queue_list_lock);

	/* ufs_mq_async_sched_exit_fn */
	ufs_mq_sched_ds_lld_exit(q);

	udk_free(q->mas_queue.cust_queuedata);
	q->mas_queue.cust_queuedata = NULL;
}

void blk_mq_tagset_ufs_mq_iosched_enable(
	struct udk_blk_mq_tag_set *tag_set, int enable)
{
	if (enable)
		tag_set->lld_func.features |= BLK_LLD_IOSCHED_UFS_MQ;
	else
		tag_set->lld_func.features &= ~BLK_LLD_IOSCHED_UFS_MQ;
}

struct mas_ufs_mq_priv mas_ufs_mq = {
	.async_io_sched_strategy = &mas_ufs_mq_async_io_fifo_sched,
};

static void ufs_tagset_get_tag_stats(struct mas_ufs_sched_ds_lld *ds_lld, enum mas_prio_type type)
{
	if (type == MAS_NORMAL_PRIO)
		vatomic32_inc(&(ds_lld->mq_tag_used_cnt));
	else if (type == MAS_HIGH_PRIO)
		vatomic32_inc(&(ds_lld->mq_prio_tag_used_cnt));
	else
		vatomic32_inc(&(ds_lld->mq_resrvd_tag_used_cnt));
}

static int ufs_tagset_get_tag(struct udk_blk_mq_tags *tags,
	unsigned int tag_start, unsigned int tag_end)
{
	unsigned int tag;

	uspinlock_lock(&tags->lock);
	if (tag_start)
		/* from LSB, search scope is bits [start+1, last], so start = tag_start - 1 */
		tag = raw_bitmap_find_next_zero(tags->bitmap_tags, tags->nr_tags, tag_start - 1);
	else
		tag = raw_bitmap_find_first_zero(tags->bitmap_tags, tags->nr_tags);
	if ((tag >= tag_end) || (tag < tag_start) || (tag >= tags->nr_tags)) {
		uspinlock_unlock(&tags->lock);
		return UDK_BLK_MQ_NO_TAG;
	}
	raw_bitmap_set_bit(tags->bitmap_tags, tags->nr_tags, tag);
	uspinlock_unlock(&tags->lock);

	return (int)tag;
}

static enum mas_prio_type ufs_mq_tag_get_type(struct udk_bio *bio)
{
	unsigned long flag = bio->mas_bi_opf & (MAS_REQ_SYNC | REQ_FG_META);

	if (likely(flag == MAS_REQ_SYNC))
		return MAS_NORMAL_PRIO;
	else if (flag & REQ_FG_META)
		return MAS_HIGH_PRIO;
	else
		return MAS_RESERVED_PRIO;
}

int ufs_mq_tag_get(struct udk_blk_mq_tags *tags, struct udk_bio *bio,
	struct udk_request_queue *queue)
{
	int tag;
	struct mas_ufs_sched_ds_lld *ds_lld = get_sched_ds_lld(queue);
	enum mas_prio_type type = ufs_mq_tag_get_type(bio);

	if (unlikely(!ds_lld))
		return UDK_BLK_MQ_NO_TAG;

	tag = ufs_tagset_get_tag(tags, tags->prio_info[type].tag_offset,
		tags->prio_info[type].tag_offset + tags->prio_info[type].tag_num);
	if (tag != UDK_BLK_MQ_NO_TAG)
		ufs_tagset_get_tag_stats(ds_lld, type);

	return tag;
}

void ufs_mq_tag_put(struct udk_blk_mq_tags *tags, unsigned int tag,
	struct udk_request_queue *queue)
{
	struct mas_ufs_sched_ds_lld *ds_lld = get_sched_ds_lld(queue);

	if (likely(ds_lld)) {
		if (unlikely(tag >= tags->prio_info[MAS_HIGH_PRIO].tag_offset))
			vatomic32_dec(&(ds_lld->mq_prio_tag_used_cnt));
		else if (unlikely(tag >= tags->prio_info[MAS_RESERVED_PRIO].tag_offset))
			vatomic32_dec(&(ds_lld->mq_resrvd_tag_used_cnt));
		else
			vatomic32_dec(&(ds_lld->mq_tag_used_cnt));
	}

	udk_blk_mq_put_tag(tags, tag);
}

int ufs_mq_sched_request(struct udk_request *rq, struct udk_request_queue *queue)
{
	return ufs_mq_make_sync_request(rq, queue, rq->mq_hctx);
}

int ufs_mq_make_async_request(struct udk_blk_mq_hw_ctx *hctx, struct udk_request *rq)
{
	struct mas_ufs_mq_sched *sched_ds =
		(struct mas_ufs_mq_sched *)(rq->queue->mas_queue.cust_queuedata);

	return __ufs_mq_queue_rq(rq, rq->queue, hctx, sched_ds->sched_ds_lld);
}
