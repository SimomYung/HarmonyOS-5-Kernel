/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block definitions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 02 04:43:16 2019
 */
#ifndef __UDK_BLOCK_H__
#define __UDK_BLOCK_H__

#include <limits.h>
#include <stdbool.h>

#include <udk/sync.h>
#include <udk/device.h>
#include <lib/dlist.h>
#include <lib/utils.h>
#include <udk/workqueue.h>

struct udk_bio;
typedef int (*devhost_end_bio_fn_t)(void *private, int bi_error, unsigned int io_idx);

#define UDK_BLKSIZE_DEFAULT 512
#define UDK_BLKSHIFT_DEFAULT 9
#define UDK_PFN_PER_SECTOR_SHIFT 3
#define UDK_PFN_PER_SECTOR_MASK ((1 << UDK_PFN_PER_SECTOR_SHIFT) - 1)
#define UDK_PFN_SHIFT (UDK_BLKSHIFT_DEFAULT + UDK_PFN_PER_SECTOR_SHIFT)

#define FMODE_READ 0x1
#define FMODE_WRITE 0x2
#define FMODE_RW (FMODE_READ | FMODE_WRITE)

/*
 * UDK_REQ_xx and UDK_REQ_FLAGS_xx is used in the cmd_flags field of struct udk_request,
 * and in bi_rw of struct udk_bio.
 * And we use 8 bits for encoding the req operation, and the remaining 24 for req flags.
 */
enum udk_rq_op_bits {
	/* common req op */
	__UDK_REQ_WRITE,		/* not set, read. set, write */
	__UDK_REQ_DISCARD,		/* request to discard sectors */
	__UDK_REQ_FUA,			/* forced unit access */
	__UDK_REQ_FLUSH,		/* request for cache flush */
	__UDK_REQ_GC,			/* request for garbage collection */
};

/* UDK_REQ_xx is the type of UDK request */
#define UDK_REQ_READ			0U
#define UDK_REQ_WRITE			(1ULL << ((int)(__UDK_REQ_WRITE)))
#define UDK_REQ_DISCARD			(1ULL << ((int)__UDK_REQ_DISCARD))
#define UDK_REQ_GC			(1ULL << ((int)__UDK_REQ_GC))
#define UDK_REQ_FUA			(1ULL << ((int)__UDK_REQ_FUA))
#define UDK_REQ_FLUSH			(1ULL << ((int)__UDK_REQ_FLUSH))
#define UDK_REQ_OP_BITS			8U
#define UDK_FS_REQ_OP_BITS		8U
#define UDK_REQ_OP_MASK			((1ULL << ((int)UDK_REQ_OP_BITS)) - 1ULL)
#define UDK_REQ_FLAGS_MASK		(~((int)UDK_REQ_OP_MASK))

enum udk_fs_req_flags_bits {
	__UDK_FS_REQ_POLL_OBJ = UDK_FS_REQ_OP_BITS,
	__UDK_FS_REQ_ASYNC_RQ,
	__UDK_FS_REQ_SYNC,
	__UDK_FS_REQ_BG,
	__UDK_FS_REQ_FG,
	__UDK_FS_REQ_JOURNAL,
	__UDK_FS_REQ_GC,
	__UDK_FS_REQ_META,
	__UDK_FS_REQ_PRIO,
	__UDK_FS_REQ_FUA,
	__UDK_FS_REQ_PREFLUSH,
	__UDK_FS_REQ_DIO = 19,
};

#define UDK_FS_REQ_SYNC			(1ULL << (int)(__UDK_FS_REQ_SYNC))
#define UDK_FS_REQ_FG			(1ULL << (int)(__UDK_FS_REQ_FG))
#define UDK_FS_REQ_META			(1ULL << (int)(__UDK_FS_REQ_META))
#define UDK_FS_REQ_PRIO			(1ULL << (int)(__UDK_FS_REQ_PRIO))
#define UDK_FS_REQ_DIO			(1ULL << (int)(__UDK_FS_REQ_DIO))

enum udk_rq_flags_bits {
	/* common flags */
	__UDK_REQ_FLAGS_FUA =
		UDK_REQ_OP_BITS, /* request with forced unit access flags */
	__UDK_REQ_FLAGS_PREFLUSH, /* request with cache flush */
	__UDK_REQ_FLAGS_SYNC,
	__UDK_REQ_FLAGS_FG,
	__UDK_REQ_FLAGS_META,
	__UDK_REQ_FLAGS_PRIO,
	__UDK_REQ_FLAGS_DIO,
	__UDK_REQ_FLAGS_NO_WAIT, /* request is not allowed to be blocked */
};

#define UDK_REQ_FLAGS_FUA		(1ULL << (int)(__UDK_REQ_FLAGS_FUA))
#define UDK_REQ_FLAGS_PREFLUSH		(1ULL << (int)(__UDK_REQ_FLAGS_PREFLUSH))
#define UDK_REQ_FLAGS_SYNC		(1ULL << (int)(__UDK_REQ_FLAGS_SYNC))
#define UDK_REQ_FLAGS_FG		(1ULL << (int)(__UDK_REQ_FLAGS_FG))
#define UDK_REQ_FLAGS_META		(1ULL << (int)(__UDK_REQ_FLAGS_META))
#define UDK_REQ_FLAGS_PRIO		(1ULL << (int)(__UDK_REQ_FLAGS_PRIO))
#define UDK_REQ_FLAGS_DIO		(1ULL << (int)(__UDK_REQ_FLAGS_DIO))
#define UDK_REQ_FLAGS_NO_WAIT		(1ULL << (int)(__UDK_REQ_FLAGS_NO_WAIT))

/* UDK_REQ OP */
#define udk_bio_req_op(bio)		((bio)->bi_rw & UDK_REQ_OP_MASK)
/* Return data direction, READ or WRITE */
#define udk_bio_data_dir(bio)		((bio)->bi_rw & UDK_REQ_WRITE)

/* UDK_REQ_FLAGS */
#define udk_bio_req_flags(bio)		((bio)->bi_rw & UDK_REQ_FLAGS_MASK)
#define udk_bio_with_fua_flags(bio)	((bio)->bi_rw & UDK_REQ_FLAGS_FUA)

#ifdef CONFIG_MAS_BLK
enum requeue_reason_enum {
	REQ_REQUEUE_IO_NO_REQUEUE = 0,
	REQ_REQUEUE_IO_HW_LIMIT,
	REQ_REQUEUE_IO_SW_LIMIT,
	REQ_REQUEUE_IO_CP_LIMIT,
	REQ_REQUEUE_IO_HW_PENDING,
	REQ_REQUEUE_MAX,
};

enum bio_process_stage_enum {
	BIO_PROC_STAGE_SUBMIT = 0,
	BIO_PROC_STAGE_ENDBIO,
	BIO_PROC_STAGE_ENTER,
	BIO_PROC_STAGE_GENERIC_MAKE_REQ,
	BIO_PROC_STAGE_TAG_START,
	BIO_PROC_STAGE_MERGE,
	BIO_PROC_STAGE_MAX
};

enum req_process_stage_enum {
	REQ_PROC_STAGE_INIT_FROM_BIO = 0,
	REQ_PROC_STAGE_FSEQ_PREFLUSH,
	REQ_PROC_STAGE_FSEQ_DATA,
	REQ_PROC_STAGE_FSEQ_POSTFLUSH,
	REQ_PROC_STAGE_FSEQ_DONE,
	REQ_PROC_STAGE_START,
	REQ_PROC_STAGE_MQ_ADDTO_PLUGLIST,
	REQ_PROC_STAGE_MQ_FLUSH_PLUGLIST,
	REQ_PROC_STAGE_MQ_ADDTO_SYNC_LIST,
	REQ_PROC_STAGE_MQ_ADDTO_ASYNC_LIST,
	REQ_PROC_STAGE_MQ_QUEUE_ERR,
	REQ_PROC_STAGE_MQ_QUEUE_RQ,
	REQ_PROC_STAGE_SCSI_QUEUE_OUT,
	REQ_PROC_STAGE_SCSI_QUEUE_TARGET,
	REQ_PROC_STAGE_SCSI_QUEUE_HOST,
	REQ_PROC_STAGE_SCSI_QUEUE_RECOVERY,
	REQ_PROC_STAGE_SCSI_QUEUE_HBB,
	REQ_PROC_STAGE_SCSI_QUEUE_CAN,
	REQ_PROC_STAGE_SCSI_QUEUE_SELF_BLOCKED,
	REQ_PROC_STAGE_MQ_HOLD_START,
	REQ_PROC_STAGE_MQ_HOLD_FAIL,
	REQ_PROC_STAGE_MQ_HOLD_SUCCESS,
	REQ_PROC_STAGE_SQ_REQUEUE,
	REQ_PROC_STAGE_DONE_SFTIRQ,
	REQ_PROC_STAGE_DONE_SFTIRQ_UNISTORE,
	REQ_PROC_STAGE_SCSI_INSERT,
	REQ_PROC_STAGE_SCSI_FINISH_REQUEUE,
	REQ_PROC_STAGE_SCSI_EH_ADD,
	REQ_PROC_STAGE_SCSI_EH_INC,
	REQ_PROC_STAGE_SCSI_EH_STU,
	REQ_PROC_STAGE_SCSI_EH_DEVICE,
	REQ_PROC_STAGE_SCSI_EH_TARGET,
	REQ_PROC_STAGE_SCSI_EH_BUS,
	REQ_PROC_STAGE_SCSI_EH_HOST,
	REQ_PROC_STAGE_SCSI_EH_OFFLINE,
	REQ_PROC_STAGE_SCSI_EH_FLUSH,
	REQ_PROC_STAGE_UFS_EH_START,
	REQ_PROC_STAGE_UFS_EH_END,
	REQ_PROC_STAGE_UPDATE,
	REQ_PROC_STAGE_FREE,
	REQ_PROC_STAGE_MAX
};

/*
* This struct defines all the variable in vendor block layer.
*/
struct blk_bio_cust {
	struct udk_request_queue *q; /* The request queue where the bio is sent to */
	struct udk_request *io_req; /* The request which carrys the bio */

	unsigned char io_in_count; /* the bio has been count in busy idle module or not */
	unsigned char fs_io_flag; /* io comes from fs or not */
	unsigned char bi_async_flush; /* async flush flag */

	/*
	* Below info is IO latency
	*/
	struct timespec bio_stage_ktime[BIO_PROC_STAGE_MAX];
	bool bio_stage_ktime_flag[BIO_PROC_STAGE_MAX];
	int bio_stage_ktime_ret[BIO_PROC_STAGE_MAX];
	struct udk_timer latency_expire_timer;
	volatile int latency_timer_running;
};
#endif

struct udk_bio_vec {
	void		*vaddr;
	unsigned int	bv_len;
};

struct udk_bio_pvec {
	unsigned long long	pfn;
	unsigned int	len;
	unsigned int	offset;
};

struct udk_workqueue;
struct udk_bio_context {
	void *block_ctx;
	devhost_end_bio_fn_t devhost_end_bio_fn;
	unsigned int bio_idx;
	bool direct_map_paddr;
	bool async;
	/* async mode, we may chain bio to list */
	struct dlist_node bio_node;
	/* async mode maybe use end_bio_wq to process end bio */
	struct udk_workqueue *end_bio_wq;
};

struct udk_bio {
	uint64_t		bi_sector;	/* device address in 512 byte */
	unsigned int		bi_size;	/* residual I/O count */
	unsigned long		bi_rw;		/* the lowest 8 bits represent UDK_REQ op,
						 * other bits are UDK_REQ flags,
						 * which used in udk_blk_submit_bio()
						 */
	unsigned short		bi_ioprio;
	int			bi_error;
	int			bi_idx; /* each bio is assigned a unique idx */
	struct udk_bio_vec	*bio_io_vec;
	struct udk_device	*bi_dev;	/* device info */
	struct udk_device	*bi_part_dev; /* device info for diskstats */
	struct udk_bio		*next;		/* next udk_bio in the
						 * same udk_request
						 */
	unsigned int		bd_io_idx;
	unsigned int		bi_vec_num; /* current number of bio_vec/bio_pvec */
	unsigned int		bi_vec_size; /* malloc size of bio_vec/bio_pvec */
	void *crypto_key; /* crypto key of bio data */
	bool async_flush;
	vatomic32_t remain_cnt;

#ifdef CONFIG_MAS_BLK
	unsigned long mas_bi_opf;
	struct blk_bio_cust mas_bio;
	struct dlist_node cnt_list;
	vatomic32_t __bi_cnt;
	void (*dump_fs)(void);
#endif

	struct timespec start;
	struct timespec finish;

	struct udk_bio_context  bi_ctx;
	struct udk_bio_pvec *bio_io_pvec;
};

struct hisi_inline_crypto_key {
	void *ci_key; /* pass pointers only in fusion condition to reduce copy spends */
	unsigned int ci_key_len;
	unsigned int ci_key_index;
	uint8_t *ci_metadata; /* pass pointers only in fusion condition to reduce copy spends */
	unsigned long index;
};

struct udk_simple_bio_set {
	/*
	 * If async is false, cb is use for wait io complete.
	 * So cb will be ignore, you can call it after sync io finish.
	 */
	bool async;
	unsigned long req_type;

	devhost_end_bio_fn_t cb;
	void *cb_private;
};

#define udk_bio_private(bio)		((bio)->bi_private)
#define udk_bio_pos(bio)		((bio)->bi_sector << UDK_BLKSHIFT_DEFAULT)
#define udk_bio_sectors(bio)		((bio)->bi_size >> UDK_BLKSHIFT_DEFAULT)
#define udk_bio_end_sector(bio)		((bio)->bi_sector + udk_bio_sectors(bio))
#define set_udk_bio_context(bio, v) \
	((bio)->bi_ctx.block_ctx = ((void *)((uintptr_t)(v))))

#define udk_bio_sector_to_size(sector)  ((sector) << UDK_BLKSHIFT_DEFAULT)
#define udk_bio_size_to_sector(size)    ((size) >> UDK_BLKSHIFT_DEFAULT)

static inline unsigned int udk_bio_cur_bytes(struct udk_bio *bio)
{
	return bio->bi_size;
}

static inline unsigned int udk_bio_nr_sectors(const struct udk_bio *bio)
{
	return bio->bi_size >> UDK_BLKSHIFT_DEFAULT;
}

static inline bool udk_bio_has_data(const struct udk_bio *bio)
{
	if (bio != NULL && bio->bi_size != 0) {
		return true;
	}
	return false;
}

static inline void *udk_bio_data(struct udk_bio *bio)
{
	if (udk_bio_has_data(bio)) {
		return bio->bio_io_vec->vaddr;
	}
	return NULL;
}

static inline bool udk_bio_op_is_discard(struct udk_bio *bio)
{
	return ((bio->bi_rw & UDK_REQ_DISCARD) != 0 ? true : false);
}

static inline bool udk_bio_time_updated(struct timespec *ts)
{
	return (ts->tv_sec != 0 || ts->tv_nsec != 0);
}

#define UDK_BLK_DEFAULT_MAX_DISCARD UINT_MAX
#define UDK_BLK_SAFE_MAX_SECTORS    255
#define UDK_BLK_DEFAULT_MAX_SECTORS UINT_MAX

struct udk_bio *udk_bio_alloc(unsigned int vec_cnt, bool direct_map_paddr);
void udk_bio_free(struct udk_bio *bio);
int udk_bio_add_data(struct udk_bio *bio, void *vaddr, unsigned int bv_len);

int udk_bio_end(struct udk_bio *bio);
int udk_blk_submit_bio(struct udk_bio *bio, unsigned int flags);
int udk_simple_submit_bio(struct udk_device *dev, uint64_t start_sector,
			  struct udk_bio_pvec *pvec, struct udk_simple_bio_set *bio_set);
int udk_simple_submit_cmd(struct udk_device *dev, uint64_t start_sector,
			  uint64_t nr_sects, struct udk_simple_bio_set *bio_set);
void udk_bio_chain(struct udk_bio *bio, struct udk_bio *parent);
void udk_bio_reduce(struct udk_bio *bio, unsigned int sectors);
struct udk_bio* udk_bio_split(struct udk_bio *bio, unsigned int sectors);
int udk_blk_submit_cmd_sync(struct udk_device *udkdev, unsigned long bi_rw,
			    uint64_t start, uint64_t nr_sects);

#ifdef CONFIG_MAS_BLK
void udk_blk_fill_rwbs(char *rwbs, unsigned int op, unsigned int mas_op);
#else
void udk_blk_fill_rwbs(char *rwbs, unsigned int op);
#endif
#endif
