/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: block header
 */

#ifndef MAS_BLK_INTERNAL_H
#define MAS_BLK_INTERNAL_H

#include <hmsysmgr/dumpstack.h>
#include <libhmsrv_sys/hm_dump.h>
#include <libhmsrv_sys/hm_timer.h>
#include <udk/iolib.h>
#include <udk/log.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk.h>
#include <libstrict/strict.h>

#define IOSCHED_NAME_MAX 16

#define IO_FROM_SUBMIT_BIO_MAGIC 0x4C
#define IO_FROM_BLK_EXEC 0x4D

#define BLK_LLD_INIT_MAGIC 0x7A
#define IO_BUF_LEN 10

#define TEN_MS 10
#define ONE_HUNDRED 100

/* Used to device sqr sqw bw */
#define BLK_MID_SQW_BW  300
#define BLK_MAX_SQW_BW  500
#define BLK_MAX_SQR_BW  0
#define BLK_MAX_SQW_BW_3_1 995
#define BLK_MAX_SQR_BW_3_1 1800
#define DEVICE_CAPACITY_128_G  0x10000000  /* 128G/512 */
#define DEVICE_CAPACITY_256_G  0x20000000

#define NO_EXTRA_MSG NULL

enum blk_lld_feature_bits {
	__BLK_LLD_DUMP_SUPPORT = 0,
	__BLK_LLD_LATENCY_SUPPORT,
	__BLK_LLD_FLUSH_REDUCE_SUPPORT,
	__BLK_LLD_BUSYIDLE_SUPPORT,
	__BLK_LLD_IOSCHED_UFS_MQ,
	__BLK_LLD_IOSCHED_UFS_HW_IDLE,
	__BLK_LLD_UFS_CP_EN,
	__BLK_LLD_UFS_ORDER_EN,
	__BLK_LLD_UFS_UNISTORE_EN,
	__BLK_LLD_IOSCHED_MMC_MQ,
	__BLK_LLD_UFS_CPPLUS_EN,
#ifdef CONFIG_MAS_BLK_NVME	
	__BLK_LLD_IOSCHED_NVME_MQ
#endif
};

#define BLK_LLD_DUMP_SUPPORT (1ULL << __BLK_LLD_DUMP_SUPPORT)
#define BLK_LLD_LATENCY_SUPPORT (1ULL << __BLK_LLD_LATENCY_SUPPORT)
#define BLK_LLD_FLUSH_REDUCE_SUPPORT (1ULL << __BLK_LLD_FLUSH_REDUCE_SUPPORT)
#define BLK_LLD_BUSYIDLE_SUPPORT (1ULL << __BLK_LLD_BUSYIDLE_SUPPORT)
#define BLK_LLD_IOSCHED_UFS_MQ (1ULL << __BLK_LLD_IOSCHED_UFS_MQ)
#define BLK_LLD_IOSCHED_UFS_HW_IDLE (1ULL << __BLK_LLD_IOSCHED_UFS_HW_IDLE)
#define BLK_LLD_UFS_CP_EN (1ULL << __BLK_LLD_UFS_CP_EN)
#define BLK_LLD_UFS_ORDER_EN (1ULL << __BLK_LLD_UFS_ORDER_EN)
#define BLK_LLD_UFS_UNISTORE_EN (1ULL << __BLK_LLD_UFS_UNISTORE_EN)
#define BLK_LLD_IOSCHED_MMC_MQ (1ULL << __BLK_LLD_IOSCHED_MMC_MQ)
#define BLK_LLD_UFS_CPPLUS_EN (1ULL << __BLK_LLD_UFS_CPPLUS_EN)
#ifdef CONFIG_MAS_BLK_NVME
#define BLK_LLD_IOSCHED_NVME_MQ (1ULL << __BLK_LLD_IOSCHED_NVME_MQ)
#endif

#define BLK_QUEUE_DURATION_UNIT_NS 10000

#define MAS_SSCANF_OK 2
#define MAS_DECIMAL 10

enum blk_busyidle_nb_flag {
	BLK_BUSYIDLE_NB_NOT_JOIN_POLL = 0,
};

#define BLK_BUSYIDLE_NB_FLAG_NOT_JOIN_POLL                                    \
	(1 << BLK_BUSYIDLE_NB_NOT_JOIN_POLL)

enum iosched_strategy {
	IOSCHED_NONE = 0,
	IOSCHED_MAS_UFS_MQ,
	IOSCHED_MAS_MMC_MQ,
#ifdef CONFIG_MAS_BLK_NVME
	IOSCHED_MAS_NVME_MQ,
#endif	
};

enum customer_rq_flag_bits {
	__REQ_HEAD_OF_QUEUE = 0,
	__REQ_COMMAND_PRIO,
	__REQ_COMMAND_ORDER,
	__REQ_TURBO_ZONE,
};

enum mas_req_flag_bits {
	__REQ_CP,	 /* IO with command priority set */
	__REQ_VIP,	 /* vip activity */
	__REQ_TZ,	 /* turbo zone IO */
	__REQ_FAULT_OUT, /* HYPERHOLD FAULT OUT IO */
	__REQ_BATCH_OUT, /* HYPERHOLD BATCH OUT IO */
	__REQ_TT_RB,	 /* turbo table read buffer IO */
	__MAS_REQ_GC,	 /* gc IO - unistore */
	__MAS_REQ_LBA,	 /* expected LBA - unistore */
	__MAS_REQ_RECOVERY, /* recovery after device reset */
	__MAS_REQ_ASYNC, /* async IO */
	__MAS_REQ_RESET, /* reset powron IO */
	__MAS_REQ_CPPLUS, /* CP+ */
	__MAS_REQ_FG,	 /* foreground IO */
	__MAS_REQ_SYNC, /* sync IO */
};

#define	BLK_STS_OK 0
#define BLK_STS_NOTSUPP		(1)
#define BLK_STS_TIMEOUT		(2)
#define BLK_STS_NOSPC		(3)
#define BLK_STS_TRANSPORT	(4)
#define BLK_STS_TARGET		(5)
#define BLK_STS_NEXUS		(6)
#define BLK_STS_MEDIUM		(7)
#define BLK_STS_PROTECTION	(8)
#define BLK_STS_RESOURCE	(9)
#define BLK_STS_IOERR		(10)

#define REQ_VIP			(1ULL << __REQ_VIP)
#define REQ_FAULT_OUT		(1ULL << __REQ_FAULT_OUT)
#define REQ_BATCH_OUT		(1ULL << __REQ_BATCH_OUT)
#define REQ_TT_RB		(1ULL << __REQ_TT_RB)
#define REQ_TZ			(1ULL << __REQ_TZ)
#define REQ_CP			(1ULL << __REQ_CP)
#define MAS_REQ_GC		(1ULL << __MAS_REQ_GC)
#define MAS_REQ_LBA		(1ULL << __MAS_REQ_LBA)
#define MAS_REQ_RECOVERY	(1ULL << __MAS_REQ_RECOVERY)
#define MAS_REQ_ASYNC		(1ULL << __MAS_REQ_ASYNC)
#define MAS_REQ_RESET		(1ULL << __MAS_REQ_RESET)
#define MAS_REQ_FG		(1ULL << __MAS_REQ_FG)
#define MAS_REQ_SYNC		(1ULL << __MAS_REQ_SYNC)

#define REQ_FG_META (MAS_REQ_FG)

#define CUST_REQ_HEAD_OF_QUEUE (1ULL << __REQ_HEAD_OF_QUEUE)
#define CUST_REQ_COMMAND_PRIO (1ULL << __REQ_COMMAND_PRIO)

#define MAS_IO_IN_COUNT_SET	(1 << 0)
#define MAS_IO_IN_COUNT_SKIP_ENDIO	(1 << 1) /* busy count for the BIO has not been added, so skip it */
#define MAS_IO_IN_COUNT_ALREADY_ENDED	(1 << 2) /* this BIO has been ended already */
#define MAS_IO_IN_COUNT_WILL_BE_SEND_AGAIN	(1 << 3)
#define MAS_IO_IN_COUNT_DONE	(1 << 4)

#define req_hoq(req)                                                           \
	((req)->mas_req.mas_featrue_flag & CUST_REQ_HEAD_OF_QUEUE)
#define req_cp(req) ((req)->mas_req.mas_featrue_flag & CUST_REQ_COMMAND_PRIO)

/*
 * IO Scheduler Operation Function Pointer
 */
struct blk_queue_ops {
	enum iosched_strategy io_scheduler_strategy;
	/* MQ Scheduler Init */
	int (*mq_iosched_init_fn)(struct udk_request_queue *);
	/* MQ Scheduler Deinit */
	void (*mq_iosched_exit_fn)(struct udk_request_queue *);
	/* Request init in MQ */
	void (*mq_req_init_fn)(
		struct udk_request_queue *, struct udk_blk_mq_ctx *, struct udk_request *);
	/* Request complete in MQ */
	void (*mq_req_complete_fn)(
		struct udk_request *, struct udk_request_queue *);
	/* Request deinit in MQ */
	void (*mq_req_deinit_fn)(struct udk_request *);
	/* Request insert to MQ */
	void (*mq_req_insert_fn)(struct udk_request *req, struct udk_request_queue *);
	/* Request requeue in MQ */
	void (*mq_req_requeue_fn)(struct udk_request *, struct udk_request_queue *);
	/* Request timeout process in MQ */
	void (*mq_req_timeout_fn)(struct udk_request *);
	/* Get the CTX in MQ */
	struct udk_blk_mq_ctx *(*mq_ctx_get_fn)(struct udk_request_queue *);
	/* Release the CTX in MQ */
	void (*mq_ctx_put_fn)(struct udk_request_queue *);
	/* Get hctx object by request */
	void (*mq_hctx_get_by_req_fn)(
		struct udk_request *, struct udk_blk_mq_hw_ctx **);
	/* Get tag in MQ */
	int (*mq_tag_get_fn)(struct udk_blk_mq_tags *tags,
		struct udk_bio *bio, struct udk_request_queue *queue);
	/* Release tag in MQ */
	void (*mq_tag_put_fn)(struct udk_blk_mq_tags *tags,
		unsigned int tag, struct udk_request_queue *queue);
	/* wakeup all threads waiting tag */
	int (*mq_tag_wakeup_all_fn)(struct udk_blk_mq_tags *);
	/* Execute queue function directly in MQ */
	void (*mq_exec_queue_fn)(struct udk_request_queue *);
	/* Run hw queue in MQ */
	void (*mq_run_hw_queue_fn)(struct udk_request_queue *);
	/* Run requeue in MQ */
	void (*mq_run_requeue_fn)(struct udk_request_queue *);
	/* Enable poll */
	void (*blk_poll_enable_fn)(bool *);
	void (*blk_write_throttle_fn)(struct udk_request_queue *, bool);
	/* Dump ioscheduler status */
	void (*blk_status_dump_fn)(struct udk_request_queue *,
		enum blk_dump_scene);
	void *scheduler_priv;
	char iosched_name[IOSCHED_NAME_MAX];
};

/*
 * MQ tagset Operation Function Pointer
 */
struct blk_tagset_ops {
	/* MQ tagset init */
	struct udk_blk_mq_tags *(*tagset_init_tags_fn)(struct udk_blk_mq_tag_set *set);
	/* MQ tagset free */
	void (*tagset_free_tags_fn)(struct udk_blk_mq_tags *);
	void (*tagset_power_off_proc_fn)(struct blk_dev_lld *);
	struct blk_queue_ops *queue_ops;
};

struct bio_delay_stage_config {
	char *stage_name;
	void (*function)(struct udk_bio *bio);
};

struct req_delay_stage_config {
	char *stage_name;
	void (*function)(struct udk_request *req);
};

static inline char *mas_blk_prefix_str(enum blk_dump_scene s)
{
	return (s == BLK_DUMP_PANIC) ? "dump" : "io_latency";
}

static inline void *mas_get_debug_addr(void *q)
{
#ifdef CONFIG_MAS_DEBUG_FS
	return q;
#else
	return NULL;
#endif
}

#ifdef CONFIG_MAS_DEBUG_FS
static inline void mas_blk_rdr_panic(const char *msg)
{
	if (msg)
		udk_error("%s\n", msg);
	BUG_ON(1);
}
#else
static inline void mas_blk_rdr_panic(const char *msg)
{
}
#endif /* CONFIG_MAS_DEBUG_FS */

#ifdef CONFIG_MAS_MQ_USING_CP
static inline bool mas_blk_bio_is_cp(const struct udk_bio *bio)
{
	return (bio->mas_bi_opf & REQ_CP) ? true : false;
}
#endif

static inline void mas_blk_show_stack(void)
{
	unsigned int flags = 0;

	dumpstack_set_flag(flags, DUMPSTACK_FLAGS_ELF_LOAD_INFO);
	dumpstack_set_flag(flags, DUMPSTACK_FLAGS_STACK_BACKTRACE);
	hm_dump_thread_to_klog(0, flags);
}

static inline void mas_time_add(struct timespec *in, long nsec, struct timespec *out)
{
	long add_nsec;

	add_nsec = in->tv_nsec + nsec;

	out->tv_sec = in->tv_sec + add_nsec / NSEC_PER_SEC;
	out->tv_nsec = add_nsec % NSEC_PER_SEC;
}

static inline bool mas_time_before(struct timespec *ts1, struct timespec *ts2)
{
	if (ts1->tv_sec != ts2->tv_sec) {
		if (ts1->tv_sec < ts2->tv_sec) {
			return true;
		}

		return false;
	}

	if (ts1->tv_nsec < ts2->tv_nsec) {
		return true;
	}

	return false;
}

static inline long long mas_time_sub_ns(struct timespec *ts1, struct timespec *ts2)
{
	long long add_nsec1 = (long)ts1->tv_sec * NSEC_PER_SEC + ts1->tv_nsec;
	long long add_nsec2 = (long)ts2->tv_sec * NSEC_PER_SEC + ts2->tv_nsec;

	return (add_nsec1 - add_nsec2);
}

static inline void mas_blk_bio_put(struct udk_bio *bio)
{
	if (!vatomic32_read(&bio->__bi_cnt)) {
		udk_error("bio is zero\n");
		mas_blk_rdr_panic(NO_EXTRA_MSG);
		return;
	}

	if (vatomic32_dec_get(&bio->__bi_cnt))
		return;

	free(bio);
}

static inline void mas_blk_bio_init(struct udk_bio *bio)
{
	vatomic32_init(&bio->__bi_cnt, 1);
	dlist_init(&bio->cnt_list);
}

static inline void mas_blk_bio_get(struct udk_bio *bio)
{
	vatomic32_inc(&bio->__bi_cnt);
}

static inline void *mas_zmalloc(size_t size)
{
	void *ptr = udk_malloc(size);

	if (likely(ptr))
		(void)memset_s(ptr, size, 0, size);

	return ptr;
}

#define MAS_BLK_SYS_BUF_LEN 4096
static inline char *mas_malloc_sysfs_buf(void)
{
	return mas_zmalloc(MAS_BLK_SYS_BUF_LEN);
}

static inline void mas_blk_dump_fs(const struct udk_bio *bio)
{
	if (bio->dump_fs) {
		bio->dump_fs();
	}
}

struct blk_dev_lld *mas_blk_get_lld(struct udk_request_queue *q);
struct udk_request_queue *mas_blk_get_queue_by_lld(struct blk_dev_lld *lld);
void mas_blk_bio_clone_fast(
	struct udk_bio *bio, struct udk_bio *bio_src);
void mas_blk_bio_queue_split(
	struct udk_request_queue *q, struct udk_bio **bio, struct udk_bio *split);
void mas_blk_bio_merge_done(const struct udk_request *req,
	const struct udk_request *next);
int mas_blk_account_io_completion(
	const struct udk_request *req, unsigned int bytes);

void mas_blk_busyidle_check_bio(const struct udk_request_queue *q, struct udk_bio *bio);
bool mas_blk_busyidle_check_request_bio(
	const struct udk_request_queue *q, const struct udk_request *rq);
int blk_queue_busyidle_event_subscribe(
	const struct udk_request_queue *q,
	const struct blk_busyidle_event_node *event_node);
int blk_lld_busyidle_event_subscribe(
	const struct blk_dev_lld *lld,
	struct blk_busyidle_event_node *event_node);
int mas_blk_busyidle_state_init(struct blk_idle_state *blk_idle);
void mas_blk_busyidle_check_bio_endio(struct udk_bio *bio);

void mas_blk_generic_make_request(const struct udk_bio *bio);
void mas_blk_bio_endio(const struct udk_bio *bio);
void mas_blk_bio_free(const struct udk_bio *bio);
void mas_blk_request_init_from_bio(struct udk_request *req, struct udk_bio *bio);
void mas_blk_insert_cloned_request(
	const struct udk_request_queue *q, const struct udk_request *rq);
void mas_blk_mq_rq_ctx_init(
	struct udk_request_queue *q, struct udk_blk_mq_ctx *ctx, struct udk_request *rq);
void mas_blk_mq_request_start(const struct udk_request *req);
void mas_blk_request_start(const struct udk_request *req);
void mas_blk_requeue_request(const struct udk_request *rq);

void mas_blk_mq_request_free(struct udk_request *rq);
void mas_blk_request_put(struct udk_request *req);
void mas_blk_latency_for_merge(
	const struct udk_request *req, const struct udk_request *next);

void mas_blk_latency_bio_check(
	struct udk_bio *bio, enum bio_process_stage_enum bio_stage);
void mas_blk_latency_req_check(
	struct udk_request *req, enum req_process_stage_enum req_stage);

void mas_blk_cleanup_queue(struct udk_request_queue *q);
void mas_blk_mq_allocated_tagset_init(struct udk_blk_mq_tag_set *set);

void mas_blk_dump_bio(const struct udk_bio *bio, enum blk_dump_scene s);
void mas_blk_dump_queue_status(
	const struct udk_request_queue *q, enum blk_dump_scene s);
int mas_blk_dump_lld_status(struct blk_dev_lld *lld, char *buf, int len);
int mas_blk_dump_queue_status2(
	struct udk_request_queue *q, char *buf, int len);

bool mas_blk_support_feature(struct udk_request_queue *q, unsigned int feature);

void mas_tag_tags_init(bool ufs_flag, struct udk_blk_mq_tag_set *tag_set);
#ifdef CONFIG_MAS_BLK_NVME
void mas_tag_nvme_tags_init(bool nvme_flag, struct udk_blk_mq_tag_set *tag_set);
#endif
int mas_blk_direct_flush_fn(void *device);
void blk_mq_tagset_direct_flush_register(
	struct udk_blk_mq_tag_set *tag_set, blk_direct_flush_fn func);
void mas_blk_sysfs_block_init(const char *prefix_name, void *ctx);
#endif
