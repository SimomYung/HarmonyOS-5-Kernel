/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Type definations and helper prototypes for block sysif
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 22:50:36 2019
 */

#ifndef ULIBS_INCLUDE_LIBSYSIF_BD_TYPES_H
#define ULIBS_INCLUDE_LIBSYSIF_BD_TYPES_H

/*
 * This rq_flag_bits must be consistent with
 * hm-liblinux/include/linux/blk_types.h rq_flag_bits
 */
#define __REQ_WRITE_		(0ULL) /* not set, read. set, write */
#define __REQ_DISCARD_		(7ULL) /* request to discard sectors */
#define __REQ_WRITE_SAME_	(9ULL) /* write same block many times */
#define __REQ_GC_		(14ULL) /* request to do sectors garbage collection */
#define __REQ_FUA		(17ULL) /* forced unit access */
#define __REQ_PREFLUSH		(18ULL) /* request for cache flush */

#define REQ_READ_SYSIF			0
#define REQ_WRITE_SYSIF			(1ULL)
#define REQ_DISCARD_SYSIF		(1ULL << __REQ_DISCARD_)
#define REQ_WRITE_SAME_SYSIF		(1ULL << __REQ_WRITE_SAME_)
#define REQ_GC_SYSIF			(1ULL << __REQ_GC_)
#define REQ_FUA_SYSIF			(1ULL << __REQ_FUA)
#define REQ_PREFLUSH_SYSIF		(1ULL << __REQ_PREFLUSH)
#define REQ_WRITE_MASK (REQ_WRITE_SYSIF | REQ_WRITE_SAME_SYSIF)

#define REQ_SIO				1ULL

#define BD_IO_STATUS_FREE	0
#define BD_IO_STATUS_INUSE	1

#define BD_CRYPTO_CI_KEY_LEN 64
#define BD_CRYPTO_METADATA_SIZE 16
#ifdef CONFIG_BLK_INLINE_ENCRYPTION
/* same value as block_crypt.h */
#define BD_CRYPTO_MAX_KEY_SIZE     64
#define BD_CRYPTO_MAX_IV_SIZE      32
#define BD_CRYPTO_DUN_ARRAY_SIZE   (BD_CRYPTO_MAX_IV_SIZE / sizeof(uint64_t))

struct bd_crypto_config {
	int crypto_mode;
	unsigned int data_unit_size;
	unsigned int dun_bytes;
};
struct bd_crypto_key {
	struct bd_crypto_config crypto_cfg;
	unsigned int data_unit_size_bits;
	unsigned int size;
	uint8_t raw[BD_CRYPTO_MAX_KEY_SIZE];
};

struct bd_io_crypt_ctx {
	struct bd_crypto_key bc_key;
	uint64_t bc_dun[BD_CRYPTO_DUN_ARRAY_SIZE];
};
#endif

struct bd_io_hisi_crypt_ctx {
	uint8_t ci_key[BD_CRYPTO_CI_KEY_LEN]; // ldk: save key value; udk: save key addr
	unsigned int ci_key_len;
	unsigned int ci_key_index;
	uint8_t ci_metadata[BD_CRYPTO_METADATA_SIZE]; // ldk: save metadata value; udk: save metadata addr
	unsigned long index;
};

// same as hw_fs_io_crypt_ctx
struct bd_io_fs_hisi_crypt_ctx {
	void *ci_key;
	int ci_key_len;
	int ci_key_index;
	uint8_t *ci_metadata;
	unsigned long index;
};

struct bd_time {
	long long tv_sec;
	long tv_nsec;
};

struct bd_io {
	int rw;
	unsigned int io_flags;
	unsigned int io_priority;
	unsigned int page_offset;
	unsigned int bv_len;
	int error;
	uint64_t block_offset;
	unsigned int bd_io_entry_idx;
	unsigned int flags;
	struct bd_io_hisi_crypt_ctx bi_hisi_crypt_context;
	void (*dump_fs)(void);
	struct bd_time start;
	struct bd_time end;
	bool in_submit; /* bd_io is in submit io process */
	bool in_endio;  /* bd_io is in end io process */
	bool async_flush;
};

/* queue to process struct bd_io */
struct sring_buf;
struct raw_workqueue;
struct bd_queue {
	struct sring_buf *submit_queue;	/* vfs will push bio into the srbuf
					 * and devhost pop the bio then process it
					 */
	struct sring_buf *end_queue;	/* After devhost finished the bio,
					 * devhost pushes the bio into the end_queue
					 * to notify vfs
					 */
	struct raw_workqueue *bio_wq; 	/* worker to process bio */
};

#endif /* ULIBS_INCLUDE_LIBSYSIF_BLK_TYPES_H */
