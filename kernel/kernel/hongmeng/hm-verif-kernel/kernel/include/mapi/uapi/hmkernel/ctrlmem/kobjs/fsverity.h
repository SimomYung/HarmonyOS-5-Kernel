/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Definitions for fsverity object
 * Author: Huawei OS Kernel Lab
 * Created: Mon May 6 13:36:49 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_FSVERITY_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_FSVERITY_H

#include <hmkernel/types.h>

/* 8 levels is enough */
#define __CTRLMEM_FSVERITY_MAX_LEVELS                     8
#define __CTRLMEM_FSVERITY_MAX_SALT_SIZE                  32
#define __CTRLMEM_FSVERITY_MAX_ALG_NUM                    3
#define __CTRLMEM_FSVERITY_MIN_MERKLE_TREE_BLOCK_SHIFT    10

#define __CTRLMEM_FSVERITY_SHA256_DIGEST_SIZE      32
#define __CTRLMEM_FSVERITY_SHA256_BLOCK_SIZE       64
#define __CTRLMEM_FSVERITY_SHA512_DIGEST_SIZE      64
#define __CTRLMEM_FSVERITY_SHA512_BLOCK_SIZE       128
#define __CTRLMEM_FSVERITY_HASH_ALG_NAME_SHA256    "sha256"
#define __CTRLMEM_FSVERITY_HASH_ALG_NAME_SHA512    "sha512"

#define __CTRLMEM_FSVERITY_MAX_DIGEST_SIZE       __CTRLMEM_FSVERITY_SHA512_DIGEST_SIZE

struct __ctrlmem_kobj_fsverity_hash_alg_s {
	__u32 index;
	union {
		const char *name;	   /* hash alg name */
		__u64 padding1;
	};
	__u32 digest_size;  /* digest size(bytes) */
	__u32 block_size;   /* block size(bytes) */
	__u32 mb_max_msgs;  /* the maximum supported interleaving factor for multibuffer hashing */
};

struct __ctrlmem_kobj_fsverity_merkle_tree_params_s {
	struct __ctrlmem_kobj_fsverity_hash_alg_s hash_alg; /* hash algorithm */
	__u8 salt_size;                           /* size of salt in bytes; 0 if none */
	__u8 salt[__CTRLMEM_FSVERITY_MAX_SALT_SIZE];        /* salt prepended to each hashed block */
	__u32 digest_size;                    /* digest of the hash alg */
	__u32 block_size;                     /* size of merkle tree blocks */
	__u32 hashes_per_block;               /* arity of the hash alg */
	__u32 log_blocksize;                  /* equals to log2(block_size) */
	__u32 log_arity;                      /* equals to log2(hashes_per_block) */
	__u32 num_levels;                     /* number of Merkle tree levels */
	__u64 tree_size;                          /* merkle tree size(bytes) */
	__u64 level0_blocks;                      /* number of blocks in tree level 0 */
	__u64 level_start[__CTRLMEM_FSVERITY_MAX_LEVELS];  /*  Starting block index for each tree level */
};

#define __CTRLMEM_FSVERITY_INFO_CORE_TYPE_KOBJ 1

struct __ctrlmem_kobj_fsverity_info_s {
	__u8 type;
	struct __ctrlmem_kobj_fsverity_merkle_tree_params_s tree_params;
	__u8 root_hash[__CTRLMEM_FSVERITY_MAX_DIGEST_SIZE];
	__u8 measurement[__CTRLMEM_FSVERITY_MAX_DIGEST_SIZE];
};

struct __ctrlmem_kobj_fsverity_single_hash_s {
	union {
		void *page;
		__u64 padding1;
	};
	union {
		__u64 page_va;
		__u64 page_pa;
	};
	__u64 index;
	__u32 hoff_in_page;
	__u32 doff_in_page;
	__u8 will_verify;
};

#define __CTRLMEM_FSVERITY_MAX_PENDING_PAGES 2
struct __ctrlmem_kobj_fsverity_task_s {
	union {
		__u64 page_va;
		__u64 page_pa;
	};
	__u32 data_offset;
	struct ___ctrlmem_kobj_fsverity_single_hash_s *hbuffer;
	__u32 result;
};

struct __ctrlmem_kobj_fsverity_mb_task_s {
	struct __ctrlmem_kobj_fsverity_task_s tasks[__CTRLMEM_FSVERITY_MAX_PENDING_PAGES];
	__u32 num_pending;
};

struct __ctrlmem_kobj_fsverity_s {
	__u32 op;
	struct __ctrlmem_kobj_fsverity_info_s verity_info;
};

#define __CTRLMEM_KOBJ_FSVERITY_OP_NULL 0
#define __CTRLMEM_KOBJ_FSVERITY_OP_VERIFY 1
#define __CTRLMEM_KOBJ_FSVERITY_OP_VERIFY_MB 2
#define __CTRLMEM_KOBJ_FSVERITY_OP_UPDATE 3
#define __CTRMMEM_KOBJ_FSVERITY_OP_INVALID 4

#define __CTRLMEM_KOBJ_FSVERITY_VERIFY_E_OK 0
#define __CTRLMEM_KOBJ_FSVERITY_VERIFY_E_INVAL (-1)
#define __CTRLMEM_KOBJ_FSVERITY_VERIFY_E_OVERFLOW (-2)
#define __CTRLMEM_KOBJ_FSVERITY_VERIFY_E_NOTSUPP (-3)
#define __CTRLMEM_KOBJ_FSVERITY_VERIFY_E_BAD_HASH (-4)

#endif
