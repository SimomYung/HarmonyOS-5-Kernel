/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: per-file encryption
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 3 09:36:49 UTC 2023
 */
#ifndef __VFS_FSCRYPT_U_H
#define __VFS_FSCRYPT_U_H

#include <sys/ioctl.h>
#include <hongmeng/types.h>

/* Encryption algorithms */
#define FSCRYPT_MODE_AES_256_XTS 1 /* AES_256_XTS */
#define FSCRYPT_MODE_AES_256_CTS 4 /* AES_256_CTS */

/* Encryption policy flags: PAD */
#define FSCRYPT_POLICY_FLAGS_PAD_4		0x00
#define FSCRYPT_POLICY_FLAGS_PAD_8		0x01
#define FSCRYPT_POLICY_FLAGS_PAD_16		0x02
#define FSCRYPT_POLICY_FLAGS_PAD_32		0x03
#define FSCRYPT_POLICY_FLAGS_PAD_MASK		0x03

/* Encryption policy flags: DIRECT_KEY */
#define FSCRYPT_POLICY_FLAG_DIRECT_KEY		0x04

/* Encryption policy flags: IV_INO_LBLK */
#define FSCRYPT_POLICY_FLAG_IV_INO_LBLK_64	0x08
#define FSCRYPT_POLICY_FLAG_IV_INO_LBLK_32	0x10

#define FSCRYPT_MAX_IV_INO_LBLK_BIT 64

#define FSCRYPT_POLICY_V1		0
#define FSCRYPT_KEY_DESCRIPTOR_SIZE	8

struct fscrypt_policy_v1 {
	uint8_t version;
	uint8_t content_encrypt_mode;
	uint8_t filename_encrypt_mode;
	uint8_t flag;
	uint8_t mkey_desc[FSCRYPT_KEY_DESCRIPTOR_SIZE];
};

#define FSCRYPT_KEY_DESC_PREFIX		"fscrypt:"
#define FSCRYPT_KEY_DESC_PREFIX_SIZE	8
#define FSCRYPT_MAX_KEY_SIZE		64

/*
 * New policy version with HKDF and key verification (recommended).
 */
#define FSCRYPT_POLICY_V2		2
#define FSCRYPT_KEY_IDENTIFIER_SIZE	16
struct fscrypt_policy_v2 {
	uint8_t version;
	uint8_t content_encrypt_mode;
	uint8_t filename_encrypt_mode;
	uint8_t flag;
	uint8_t __reserved[4];
	uint8_t mkey_id[FSCRYPT_KEY_IDENTIFIER_SIZE];
};

union fscrypt_policy {
	uint8_t version;
	struct fscrypt_policy_v1 v1;
	struct fscrypt_policy_v2 v2;
};

/* Struct passed to FS_IOC_GET_ENCRYPTION_POLICY_EX */
struct fscrypt_get_policy_ex_arg {
	uint64_t policy_size; /* input/output */
	union fscrypt_policy policy; /* output */
};

#define FSCRYPT_KEY_SPEC_TYPE_DESCRIPTOR	1
#define FSCRYPT_KEY_SPEC_TYPE_IDENTIFIER	2

struct fscrypt_key_specifier {
	uint32_t type; /* one of FSCRYPT_KEY_SPEC_TYPE_* */
	uint32_t __reserved;
	union {
		uint64_t __reserved[4]; /* reserve some extra space */
		uint8_t descriptor[FSCRYPT_KEY_DESCRIPTOR_SIZE];
		uint8_t identifier[FSCRYPT_KEY_IDENTIFIER_SIZE];
	} u;
};

/*
 * Payload of Linux keyring key of type "fscrypt-provisioning", referenced by
 * fscrypt_add_key_arg::key_id as an alternative to fscrypt_add_key_arg::raw.
 */
struct fscrypt_provisioning_key_payload {
	uint32_t type;
	uint32_t __reserved;
	uint8_t raw[];
};

/* Struct passed to FS_IOC_ADD_ENCRYPTION_KEY */
struct fscrypt_add_key_arg {
	struct fscrypt_key_specifier key_spec;
	uint32_t raw_size;
	uint32_t key_id;
	uint32_t __reserved[8];
	uint8_t raw[];
};

/* Struct passed to FS_IOC_REMOVE_ENCRYPTION_KEY */
struct fscrypt_remove_key_arg {
	struct fscrypt_key_specifier key_spec;
#define FSCRYPT_KEY_REMOVAL_STATUS_FLAG_FILES_BUSY	0x00000001
#define FSCRYPT_KEY_REMOVAL_STATUS_FLAG_OTHER_USERS	0x00000002
	uint32_t removal_status_flags; /* output */
	uint32_t __reserved[5];
};

/* Struct passed to FS_IOC_GET_ENCRYPTION_KEY_STATUS */
struct fscrypt_get_key_status_arg {
	/* input */
	struct fscrypt_key_specifier key_spec;
	uint32_t __reserved[6];

	/* output */
#define FSCRYPT_KEY_STATUS_ABSENT		1
#define FSCRYPT_KEY_STATUS_PRESENT		2
#define FSCRYPT_KEY_STATUS_INCOMPLETELY_REMOVED	3
	uint32_t status;
#define FSCRYPT_KEY_STATUS_FLAG_ADDED_BY_SELF   0x00000001
	uint32_t status_flags;
	uint32_t user_count;
	uint32_t __out_reserved[13];
};

#define FS_IOC_SET_ENCRYPTION_POLICY		_IOR('f', 19, struct fscrypt_policy_v1)
#define FS_IOC_GET_ENCRYPTION_PWSALT		_IOW('f', 20, uint8_t[16])
#define FS_IOC_GET_ENCRYPTION_POLICY		_IOW('f', 21, struct fscrypt_policy_v1)
#define FS_IOC_GET_ENCRYPTION_POLICY_EX		_IOWR('f', 22, uint8_t[9]) /* size + version */
#define FS_IOC_ADD_ENCRYPTION_KEY		_IOWR('f', 23, struct fscrypt_add_key_arg)
#define FS_IOC_REMOVE_ENCRYPTION_KEY		_IOWR('f', 24, struct fscrypt_remove_key_arg)
#define FS_IOC_REMOVE_ENCRYPTION_KEY_ALL_USERS	_IOWR('f', 25, struct fscrypt_remove_key_arg)
#define FS_IOC_GET_ENCRYPTION_KEY_STATUS	_IOWR('f', 26, struct fscrypt_get_key_status_arg)
#define FS_IOC_GET_ENCRYPTION_NONCE		_IOR('f', 27, uint8_t[16])

#endif /* __VFS_FSCRYPT_U_H */
