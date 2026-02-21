/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/fscrypt.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 15:01:53 2021
 */

#ifndef LINUX_FSCRYPT_H
#define LINUX_FSCRYPT_H

#include <linux/types.h>

#define FSCRYPT_KEY_DESCRIPTOR_SIZE	8
struct fscrypt_policy_v1 {
	__u8 version;
	__u8 contents_encryption_mode;
	__u8 filenames_encryption_mode;
	__u8 flags;
	__u8 master_key_descriptor[FSCRYPT_KEY_DESCRIPTOR_SIZE];
};

#define FSCRYPT_KEY_IDENTIFIER_SIZE	16
struct fscrypt_policy_v2 {
	__u8 version;
	__u8 contents_encryption_mode;
	__u8 filenames_encryption_mode;
	__u8 flags;
	__u8 __reserved[4];
	__u8 master_key_identifier[FSCRYPT_KEY_IDENTIFIER_SIZE];
};

struct fscrypt_get_policy_ex_arg {
	__u64 policy_size;
	union {
		__u8 version;
		struct fscrypt_policy_v1 v1;
		struct fscrypt_policy_v2 v2;
	} policy;
};

#endif
