/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Fscrypt Key's key_type implementation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 20 15:22:19 2022
 */
#ifndef KEYMGR_FSCRYPT_H
#define KEYMGR_FSCRYPT_H
/* libc */
#include <stdint.h>
#include <string.h>

/* hongmeng */
#include <libhmlog/hmlog.h>

#define FSCRYPT_MIN_KEY_SIZE 16
#define FSCRYPT_MAX_KEY_SIZE 64

#define FSCRYPT_KEY_SPEC_TYPE_DESCRIPTOR	1
#define FSCRYPT_KEY_SPEC_TYPE_IDENTIFIER	2

// fscrypt_ctrl argument
#define FSCRYPT_CTRL_ARG_MAXS_SIZE	4096

// create master keyring
#define FSCRYPT_CTRL_ADD_MKRING		1
// create master key
#define FSCRYPT_CTRL_ADD_MK		2
// get key payload by keyid and keytype
#define FSCRYPT_CTRL_GET_KEYDATA	3
// request key payload and size by name and type
#define FSCRYPT_CTRL_REQUEST_KEYDATA	4
// search key in keyring
#define FSCRYPT_CTRL_KEYRING_SEARCH	5
// delete master key from master-key keyring
#define FSCRYPT_CTRL_REMOVE_MK		6

#define FSCRYPT_CTRL_OP_MAX 		8

#define STRLEN(str)	(sizeof(str) - 1)
/* Max size of identifier and description */
#define MAX_OF_DESCRIPTOR_IDENTIFIER	16

/* This is for master-key keyring's name */
#define FSCRYPT_KEYRING_MK_NAME_SIZE \
	(STRLEN("fscrypt-") + (MAX_OF_DESCRIPTOR_IDENTIFIER << 1) + STRLEN("-users") + 1)

/* This is for master key's name, and 10 is for uid */
#define FSCRYPT_KEY_MK_NAME_SIZE \
	((MAX_OF_DESCRIPTOR_IDENTIFIER << 1) + STRLEN(".uid.") + 10 + 1)

#define FSCRYPT_LOGON_KEY_NAME_SIZE \
	(STRLEN("fscrypt:") + (MAX_OF_DESCRIPTOR_IDENTIFIER << 1) + 1)

#define FSCRYPT_MAX_PAYLOAD_SIZE	(148)
struct key_payload_res_s {
	size_t payload_size;
	uint8_t payload[FSCRYPT_MAX_PAYLOAD_SIZE];
};

// For FSCRYPT_CTRL_ADD_MKRING
struct fscrypt_ctl_add_mkring_arg {
	// identifier or description
	uint8_t info[MAX_OF_DESCRIPTOR_IDENTIFIER];
	/*
	 * @res points to a buffer, and keymgr will store
	 * master-key keyring addr in the buffer for caller
	 */
	void *res;
};

// For FSCRYPT_CTRL_ADD_MK
struct fscrypt_ctl_add_mk_arg {
	// identifier or description
	uint8_t info[MAX_OF_DESCRIPTOR_IDENTIFIER];
	// master-key keyring
	void *keyring;
	/*
	 * @res points to a buffer, and keymgr will store master key addr
	 * in the buffer for caller
	 */
	void *res;
};

/*
 * For FSCRYPT_CTRL_REMOVE_MK
 * Usage:
 * 1) Delete all master key in keyring
 *    struct fscrypt_ctl_rm_mk_arg arg = {true, keyring_addr, NULL}
 *
 * 2) Delete one key
 *    Delete one master key in keyring:
 *    struct fscrypt_ctl_rm_mk_arg arg = {false, keyring_addr, key_addr}
 *    Delete master-key keyring:
 *    struct fscrypt_ctl_rm_mk_arg arg = {false, NULL, keyring_addr}
 */
struct fscrypt_ctl_rm_mk_arg {
	bool del_all_keys;
	void *keyring;
	void *key;
};

// For FSCRYPT_CTRL_GET_KEYDATA
struct fscrypt_ctl_get_keydata_arg {
	int key_id;
	struct key_payload_res_s *res;
};

// For FSCRYPT_CTRL_REQUEST_KEYDATA
struct fscrypt_ctl_req_keydata_arg {
	char desc[FSCRYPT_LOGON_KEY_NAME_SIZE];
	struct key_payload_res_s *res;
};

struct key_search_res_s {
	int nr_leaves;
	void *key_addr;
};

struct fscrypt_ctrl_kring_search_arg {
	void *keyring;
	char desc[FSCRYPT_KEY_MK_NAME_SIZE];
	/*
	 * @res points to a buffer, and keymgr will store master key addr
	 * and nr_leaves count in the buffer for caller
	 */
	struct key_search_res_s *res;
};

struct key_fscrypt_provisioning_payload {
	uint32_t type;
	uint32_t __reserved;
	uint8_t raw[];
};

struct fscrypt_key {
	uint32_t mode;
	uint8_t raw[FSCRYPT_MAX_KEY_SIZE];
	uint32_t size;
};

#ifdef CONFIG_VFS_KEYMGR_DIRECT
int keymgr_fscrypt_ctrl(int operation, void *arg_addr, size_t arg_size);
#endif

#endif
