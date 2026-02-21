/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: per-file verity
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 3 09:36:49 UTC 2023
 */

#ifndef __VFS_VERITY_U_H
#define __VFS_VERITY_U_H

#include <sys/ioctl.h>
#include <hongmeng/types.h>

#define FS_VERITY_HASH_ALG_SHA256	1
#define FS_VERITY_HASH_ALG_SHA512	2

#define CODE_SIGN_VERSION	5

struct fsverity_enable_arg {
	uint32_t version;
	uint32_t hash_algorithm;
	uint32_t block_size;
	uint32_t salt_size;
	uint64_t salt_ptr;
	uint32_t sig_size;
	uint32_t __reserved1;
	uint64_t sig_ptr;
	uint64_t __reserved2[11];
};

struct code_sign_enable_arg {
	uint32_t version;
	uint32_t hash_algorithm;
	uint32_t block_size;
	uint32_t salt_size;
	uint64_t salt_ptr;
	uint32_t sig_size;
	uint32_t __reserved1;
	uint64_t sig_ptr;
	uint64_t __reserved2[5];
	uint32_t __reserved3;
	uint32_t pgtypeinfo_size; /* size of page type info in number of bits */
	uint64_t pgtypeinfo_offset; /* page type info offset */
	uint64_t tree_offset;
	uint64_t root_hash_ptr;
	uint64_t data_size;
	uint32_t flags;
	uint32_t cs_version;
};

struct fsverity_digest {
	uint16_t digest_algorithm;
	uint16_t digest_size; /* input/output */
	uint8_t digest[];
};

#define FS_IOC_ENABLE_VERITY	_IOW('f', 133, struct fsverity_enable_arg)
#define FS_IOC_MEASURE_VERITY	_IOWR('f', 134, struct fsverity_digest)
#define FS_IOC_ENABLE_CODE_SIGN	_IOW('f', 200, struct code_sign_enable_arg)

#endif
