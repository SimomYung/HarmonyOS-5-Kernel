/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block multi queue definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */
#ifndef __UDK_BLOCK_MQ_H__
#define __UDK_BLOCK_MQ_H__

#include <stdbool.h>

#include <udk/block/udk_block.h>
#include <udk/block/udk_io_scheduler.h>
#include <udk/compiler.h>
#include <udk/log.h>
#include <libhmsync/uspinlock.h>
#include <libpreempt/preempt.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/raw_rwlock.h>
#include <vsync/atomic.h>
#ifdef CONFIG_MAS_BLK
#include <udk/workqueue.h>
#endif

enum bio_merge_status {
	UDK_BIO_MERGE_OK,
	UDK_BIO_MERGE_NONE,
	UDK_BIO_MERGE_FAILED,
};

#define UDK_RQF_STARTED		((1U << 1))
#define UDK_RQF_FLUSH_SEQ	((1U << 4))
#define UDK_RQF_DONTPREP	((1U << 7))
#define UDK_RQF_SCHEDPRIV	((1U << 12))
#define UDK_RQF_HASHED		((1U << 16))
#define UDK_RQF_TIMED_OUT	((1U << 21))
#define UDK_RQF_SCHED		((1U << 22))
#define UDK_RQF_REQUEUED	((1U << 23))

#define UDK_RQF_NOMERGE_FLAGS (UDK_RQF_STARTED | UDK_RQF_FLUSH_SEQ)
#define UDK_REQ_NOMERGE_FLAGS (UDK_REQ_FLUSH | UDK_REQ_FUA)

#define UDK_BLK_MQ_TAG_HCTX_SHARED (1U)
#define UDK_BLK_MQ_NO_SCHED ((1U << 1))
#define UDK_BLK_MQ_SHOULD_MERGE ((1U << 2))

#define SECTOR_SHIFT 9
#define UDK_BLK_MQ_NO_TAG (-1)
#define UDK_BLK_QUEUE_FLAG_NO_BG_DISPATCH	(1U)
#define UDK_BLK_QUEUE_FLAG_QOS			(30U) /* device supports qos on off */

#define UDK_BLK_STATIC_REQUEST_SIZE  8192U

#define UDK_BLK_TIMEOUT_DEFAULT 30000

#define list_entry_req(ptr) dlist_entry((ptr), struct udk_request, queuelist)
#define udk_req_data_dir(req) ((req)->cmd_flags & UDK_REQ_WRITE)

struct udk_request_queue;
struct udk_request;
struct udk_blk_mq_hw_ctx;

#ifdef CONFIG_MAS_BLK
enum mas_prio_type {
	MAS_NORMAL_PRIO,
	MAS_RESERVED_PRIO,
	MAS_HIGH_PRIO,
	MAS_MAX_PRIO_TYPES
};

struct mas_prio_queue_info {
	unsigned int tag_offset;
	unsigned int tag_num;
};

enum blk_lld_base {
	BLK_LLD_QUEUE_BASE = 0,
	BLK_LLD_QUEUE_TAG_BASE,
	BLK_LLD_TAGSET_BASE,
};

enum blk_dump_scene {
	BLK_DUMP_WARNING = 0,
	BLK_DUMP_PANIC,
};

enum blk_busyidle_callback_ret {
	/* Event Proc won't trigger IO */
	BUSYIDLE_NO_IO = 0,
	/* Event Proc will trigger new IO */
	BUSYIDLE_IO_TRIGGERED,
	/* Event Proc meets errors */
	BUSYIDLE_ERR,
};

enum blk_idle_notify_state {
	BLK_BUSY_NOTIFY = 0, /* IO Busy Event */
	BLK_IDLE_NOTIFY,     /* IO Idle Event */
};

enum blk_io_state {
	BLK_IO_BUSY = 0,
	BLK_IO_IDLE,
};

enum blk_idle_dur_enum {
	BLK_IDLE_DUR_IDX_100MS,
	BLK_IDLE_DUR_IDX_500MS,
	BLK_IDLE_DUR_IDX_1000MS,
	BLK_IDLE_DUR_IDX_2000MS,
	BLK_IDLE_DUR_IDX_4000MS,
	BLK_IDLE_DUR_IDX_6000MS,
	BLK_IDLE_DUR_IDX_8000MS,
	BLK_IDLE_DUR_IDX_10000MS,
	BLK_IDLE_DUR_IDX_FOR_AGES,
	BLK_IDLE_DUR_IDX_DUR_NUM,
};

struct blk_dev_lld;

#define SUBSCRIBER_NAME_LEN 32
struct blk_busyidle_event_node {
	/* Identify a subscriber uniquely */
	char subscriber[SUBSCRIBER_NAME_LEN];
	/* should be provided by subscriber module */
	enum blk_busyidle_callback_ret (*busyidle_callback)(
		struct blk_busyidle_event_node *, enum blk_idle_notify_state);
	/* optional */
	void *param_data;

	/* busy idle private data, don't touch it */
	struct blk_dev_lld *lld;
};

struct blk_idle_state {
	/* Hardware idle support or not */
	bool idle_intr_support;
	/* Idle event process worker */
	struct udk_timer idle_notify_worker;
	/* delay avoid jitter */
	unsigned int idle_notify_delay_ms;

	/* list of subcribed events */
	struct dlist_node subscribed_list;
	struct udk_blocking_notifier_list *nh;
	vatomic32_t io_count; /* io count variable */
	struct raw_mutex io_count_mutex;
	vatomic32_t is_del_timer;
	/* busy idle state */
	enum blk_io_state idle_state;

#ifdef CONFIG_MAS_DEBUG_FS
	/*
	 * for busy idle statistic purpose
	 */
	struct timespec last_idle_ktime;
	int last_idle_ktime_ret;
	struct timespec last_busy_ktime;
	int last_busy_ktime_ret;
	unsigned long long total_busy_ktime;
	unsigned long long total_idle_ktime;
	unsigned long long total_idle_count;
	/* statistic for idle time */
	long long blk_idle_dur[BLK_IDLE_DUR_IDX_DUR_NUM];
	/* max idle time */
	long long max_idle_dur;
	struct blk_busyidle_event_node busy_idle_test_node;
	struct blk_busyidle_event_node busy_idle_test_nodes[5];
#endif

	/*
	 * Below info is just for busy idle debug purpose!
	 */
	/* The number of bios have been counted */
	unsigned long bio_count;
	/* The number of reqs have been counted */
	unsigned long req_count;
	/* The list for all the counted bios */
	struct dlist_node bio_list;
	/* The list for all the counted reqs */
	struct dlist_node req_list;
	struct uspinlock_s counted_list_lock;
};

typedef void (*lld_dump_status_fn)(
	struct udk_request_queue *, enum blk_dump_scene);

typedef int (*blk_direct_flush_fn)(void *scsi_dev);

struct blk_dev_lld {
	/* Magic Number for the struct */
	unsigned int init_magic;
	/* The object on queue, tag or tagset */
	enum blk_lld_base type;
	/* Private data */
	void *data;

	/* LLD Feature flag bit */
	unsigned long features;
	lld_dump_status_fn dump_fn;
	/* IO Latency warning threshold */
	unsigned int latency_warning_threshold_ms;

	/* Emergency Flush Operation */
	blk_direct_flush_fn flush_fn;

	/* For busy idle feature */
	struct blk_idle_state blk_idle;
	/* accumulated write len of the whole device */
	unsigned long write_len;
	/* accumulated discard len of the whole device */
	unsigned long discard_len;
	struct dlist_node lld_list;

	/*
	 * MAS IO Scheduler private data
	 */
	bool sched_ds_lld_inited;
	void *sched_ds_lld;
};

struct blk_queue_cust {
	/* The disk struct of the request queue */
	struct udk_disk *queue_disk;
	/* The request queue has the partition table or not */
	bool blk_part_tbl_exist;

	/*
	 * Flush Optimise
	 */
	struct udk_timer flush_work;
	vatomic32_t flush_work_trigger;
	vatomic32_t write_after_flush;
	struct dlist_node flush_queue_node;
	int flush_optimize;
	struct uspinlock_s flush_lock;
	struct udk_work flush_work_handler;

	/*
	 * IO latency statistic function
	 */
	int io_latency_enable;
	unsigned int io_lat_warning_thresh;
	struct dlist_node dump_list;

	/*
	 * MAS IO Scheduler private data
	 */
	void *cust_queuedata;
};

/*
 * This struct defines all the variable in vendor block layer.
 */
struct blk_req_cust {
	/* mas feature flag */
	unsigned long long mas_featrue_flag;
	/* io comes from fs or not */
	unsigned char fs_io_flag;
	/* The reason for IO requeue */
	enum requeue_reason_enum requeue_reason;
	/* The CTX which make the request */
	struct udk_blk_mq_ctx *mq_ctx_generate;

	struct timespec req_stage_ktime[REQ_PROC_STAGE_MAX];
	bool req_stage_ktime_flag[REQ_PROC_STAGE_MAX];
	int req_stage_ktime_ret[REQ_PROC_STAGE_MAX];
	void *real_req;

	/*
	 * Below info for debug info
	 */

#ifdef CONFIG_MAS_DEBUG_FS
	vatomic32_t req_used;
#endif
};
#endif


struct udk_blk_mq_queue_map {
	unsigned int *mq_map;
	unsigned int nr_queues;
};

struct udk_blk_mq_tags {
	unsigned int nr_tags;
	unsigned long long *bitmap_tags;
	struct udk_request **static_reqs;
	struct uspinlock_s lock;

#ifdef CONFIG_MAS_BLK
	struct mas_prio_queue_info prio_info[MAS_MAX_PRIO_TYPES];
	unsigned int nr_high_prio_tags;
	unsigned int tags_id_offset;
	unsigned int reserved_tags_id_offset;
	unsigned int high_prio_tags_id_offset;
	struct udk_blk_mq_tag_set *set;
#endif
};

struct udk_blk_mq_tag_set {
	struct udk_blk_mq_queue_map map;
	unsigned int nr_maps;
	const struct udk_blk_mq_ops *ops;
	unsigned int nr_hw_queues;
	unsigned int queue_depth;
	unsigned int flags;
	void *driver_data;
	struct udk_blk_mq_tags **tags;
	struct udk_blk_mq_tags *shared_tags;
#ifdef CONFIG_MAS_BLK
	struct mas_prio_queue_info prio_info[MAS_MAX_PRIO_TYPES];
	struct blk_dev_lld lld_func;
	struct blk_tagset_ops *mas_tagset_ops;
	unsigned int high_prio_tags;
	unsigned int reserved_tags;

	struct dlist_node tag_list;
	bool ufs_flag;
#ifdef CONFIG_MAS_BLK_NVME
	bool nvme_flag;
#endif
#endif
};

struct udk_blk_mq_ctx {
	struct {
		struct uspinlock_s lock;
		struct dlist_node req_list;
	};

	unsigned int cpu;
	unsigned short idx_hctx;
	struct udk_blk_mq_hw_ctx *hctx;
	struct udk_request_queue *queue;
};

struct udk_blk_mq_hw_ctx {
	unsigned long flags;
	void *sched_data;
	struct udk_request_queue *queue;
	unsigned long long *ctx_map;
	struct udk_blk_mq_ctx *dispatch_from;
	unsigned short nr_ctx;
	struct udk_blk_mq_ctx **ctxs;

	struct udk_blk_mq_tags *tags;
	struct udk_blk_mq_tags *sched_tags;
	struct uspinlock_s ctxmap_lock;

	void *driver_data;
};

enum udk_blk_eh_timer_return {
	UDK_BLK_EH_DONE,
	UDK_BLK_EH_RESET_TIMER,
};

struct udk_blk_mq_ops {
	int (*queue_req)(struct udk_blk_mq_hw_ctx *, struct udk_request *);
	unsigned int (*get_budget)(struct udk_request_queue *);
	enum udk_blk_eh_timer_return (*timeout)(struct udk_request *);
	int (*init_hctx)(struct udk_blk_mq_hw_ctx *hctx, void *data, unsigned int hctx_idx);
	void (*polling)(struct udk_request_queue *queue, int rw);
};

struct udk_request_queue {
	struct udk_request *last_merge;
	struct udk_io_scheduler_queue *scheduler;
	const struct udk_blk_mq_ops *mq_ops;
	struct udk_blk_mq_ctx *queue_ctx;
	struct udk_blk_mq_hw_ctx **hctx_table;
	unsigned int nr_hw_queues;
	void *queuedata;

	unsigned long queue_flags;
	unsigned int nr_requests;
	struct udk_blk_mq_tags *sched_shared_tags;
	struct udk_blk_mq_tag_set *tag_set;
	raw_thread_t dispatch_thread;
	vatomic32_t queue_depth;
	unsigned int timeout;
	struct udk_timer timer;
	struct dlist_node prio_requeue_list;
	struct dlist_node requeue_list;
	int requeue_nr;
	struct uspinlock_s requeue_lock;
	struct udk_work requeue_work;

	/* we use rwlock to freeze/unfreeze queue */
	struct raw_rwlock freeze_lock;

#ifdef CONFIG_MAS_BLK
	struct blk_queue_cust mas_queue;
	struct blk_queue_ops *mas_queue_ops;
	struct blk_dev_lld lld_func;

	struct dlist_node tag_set_list;
#endif
};

/*
 * Request state for udk-blk-mq.
 */
enum udk_mq_rq_state {
	UDK_MQ_RQ_IDLE,
	UDK_MQ_RQ_IN_FLIGHT,
	UDK_MQ_RQ_COMPLETE,
};

struct udk_request {
	struct udk_request_queue *queue;
	struct udk_blk_mq_ctx *mq_ctx;
	struct udk_blk_mq_hw_ctx *mq_hctx;
	struct udk_disk *disk;
	unsigned int cmd_flags;
	unsigned int req_flags;
	int tag;
	unsigned int data_len;
	uint64_t sector;
	unsigned int pvec_num;
	unsigned int bio_num;
	struct udk_bio *bio;
	struct udk_bio *biotail;

	struct dlist_node queuelist;
	unsigned short ioprio;
	uint64_t hash_key;
	struct rbt_hnode hash;
	struct rbt_node rbt_node;

	struct {
		void *priv[2];
	} sched;

	uint64_t fifo_time;
	unsigned int timeout;
	struct timespec deadline;
	enum udk_mq_rq_state state;

#ifdef CONFIG_MAS_BLK
	unsigned long mas_cmd_flags;

	struct blk_req_cust mas_req;
	/*
	 * Below is for MAS IO Scheduler
	 */
	struct dlist_node async_list;
	/*
	 * Below is for MAS Block Debug purpose
	 */
	struct dlist_node cnt_list;
#endif /* CONFIG_MAS_BLK */
};

raw_static_assert(sizeof(struct udk_request) <= 4096U, struct_udk_request_is_oversize);

static inline bool udk_req_is_flush(unsigned int flags)
{
	return flags & (UDK_REQ_FUA | UDK_REQ_FLUSH);
}

static inline bool udk_req_is_no_wait(unsigned int flags)
{
	return ((flags & UDK_REQ_FLAGS_NO_WAIT) != 0);
}

static inline bool udk_bio_mergeable(struct udk_bio *bio)
{
	return !(bio->bi_rw & UDK_REQ_NOMERGE_FLAGS);
}

static inline uint64_t udk_req_pos(const struct udk_request *req)
{
	return req->sector;
}

static inline unsigned int udk_req_bytes(const struct udk_request *req)
{
	return req->data_len;
}

static inline unsigned int udk_req_sectors(const struct udk_request *req)
{
	return udk_req_bytes(req) >> SECTOR_SHIFT;
}

static inline bool req_mergeable(struct udk_request *req)
{
	if (req->cmd_flags & UDK_REQ_NOMERGE_FLAGS)
		return false;

	if (req->req_flags & UDK_RQF_NOMERGE_FLAGS)
		return false;

	return true;
}

static inline bool udk_blk_mq_is_shared_tags(unsigned int flags)
{
	return flags & UDK_BLK_MQ_TAG_HCTX_SHARED;
}

static inline struct udk_blk_mq_ctx *
udk_blk_mq_get_ctx(struct udk_request_queue *queue, int cpu)
{
	return queue->queue_ctx + cpu;
}

static inline void udk_blk_mq_mark_rq_complete(struct udk_request *req)
{
	if (req->state != UDK_MQ_RQ_IN_FLIGHT) {
		udk_error("blk-mq: request not inflight, already completed?");
	}
	req->state = UDK_MQ_RQ_COMPLETE;
}

static inline void __dlist_splice(const struct dlist_node *list,
				 struct dlist_node *prev,
				 struct dlist_node *next)
{
	struct dlist_node *first = list->next;
	struct dlist_node *last = list->prev;

	first->prev = prev;
	prev->next = first;
	last->next = next;
	next->prev = last;
}

static inline void dlist_splice_tail_init(struct dlist_node *list,
					 struct dlist_node *head)
{
	if (!dlist_empty(list)) {
		__dlist_splice(list, head->prev, head);
		dlist_init(list);
	}
}


typedef bool (busy_tag_iter_fn)(struct udk_request *, void *);
void blk_mq_tagset_busy_iter(struct udk_blk_mq_tag_set *tagset,
		busy_tag_iter_fn *fn, void *priv);
void udk_blk_mq_start_timer(struct udk_request *req);
void udk_blk_mq_requeue_req(struct udk_request *req, bool kick, bool prio);
void udk_blk_mq_kick_requeue_list(struct udk_request_queue *q);
int udk_blk_mq_submit_bio(struct udk_bio *bio);
struct udk_request_queue *
udk_blk_mq_init_queue_data(struct udk_blk_mq_tag_set *set, void *queuedata);
int udk_blk_mq_alloc_tag_set(struct udk_blk_mq_tag_set *set);
void udk_blk_mq_put_tag(struct udk_blk_mq_tags *tags, unsigned int tag);
void udk_blk_mq_free_request(struct udk_request *req);
void udk_blk_mq_end_request(struct udk_request *req, int error);
bool udk_blk_mq_update_request(struct udk_request *req, int error,
			       unsigned int nr_bytes);

int udk_blk_mq_init_sched(struct udk_request_queue *queue,
			  struct udk_io_scheduler_type *sched_type);
void udk_blk_mq_exit_sched(struct udk_request_queue *queue,
			   struct udk_io_scheduler_queue *sched);
void udk_blk_mq_free_queue(struct udk_request_queue *queue);

enum udk_sched_merge udk_blk_try_merge(struct udk_request *req,
				       struct udk_bio *bio);
bool udk_req_can_merge(struct udk_request *req, struct udk_bio *bio);
bool udk_blk_mq_try_merge_bio(struct udk_request_queue *queue,
			      struct udk_bio *bio);
bool udk_blk_mq_sched_try_merge(struct udk_request_queue *queue,
				struct udk_bio *bio,
				struct udk_request **merged_request);
bool udk_blk_attempt_req_merge(struct udk_request_queue *queue,
			       struct udk_request *req,
			       struct udk_request *next);
bool udk_blk_discard_mergable(struct udk_request *req);
bool udk_blk_mq_sched_try_insert_merge(struct udk_request_queue *queue,
				       struct udk_request *req,
				       struct dlist_node *free);

extern uintptr_t sysmem_palloc(unsigned long size);
extern void sysmem_pfree(uintptr_t vaddr);

static inline void udk_blk_queue_flag_set(struct udk_request_queue *queue, unsigned int flag)
{
	if (queue == NULL) {
		return;
	}
	queue->queue_flags |= (1UL << flag);
}

static inline void udk_blk_queue_flag_clear(struct udk_request_queue *queue, unsigned int flag)
{
	if (queue == NULL) {
		return;
	}
	queue->queue_flags &= (~(1UL << flag));
}

static inline bool udk_blk_queue_flag_is_set(struct udk_request_queue *queue, unsigned int flag)
{
	if (queue == NULL) {
		return false;
	}
	return ((queue->queue_flags & (1UL << flag)) != 0);
}

void udk_blk_mq_freeze_queue(struct udk_request_queue *queue);
void udk_blk_mq_unfreeze_queue(struct udk_request_queue *queue);
int udk_blk_mq_queue_enter(struct udk_request_queue *queue);
void udk_blk_mq_queue_exit(struct udk_request_queue *queue);

#ifdef CONFIG_MAS_BLK
void udk_blk_mq_init_bio_request(struct udk_request *req,
					struct udk_bio *bio);
enum bio_merge_status udk_bio_attempt_back_merge(struct udk_request *req,
	struct udk_bio *bio);
enum bio_merge_status udk_bio_attempt_front_merge(struct udk_request *req,
	struct udk_bio *bio);
void udk_blk_mq_free_tags(struct udk_blk_mq_tags *tags);
struct udk_blk_mq_tags *udk_blk_mq_alloc_tags(unsigned int nr_tags);
#endif
uint32_t udk_blk_mq_req_pending(struct udk_request_queue *queue);
#endif
