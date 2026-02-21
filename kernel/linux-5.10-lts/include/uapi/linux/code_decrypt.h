/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 *
 * code decrypt user API
 */

#ifndef _UAPI_LINUX_CODE_DECRYPT_H
#define _UAPI_LINUX_CODE_DECRYPT_H

#include <linux/ioctl.h>
#include <linux/types.h>

struct code_decrypt_arg {
	int arg1_len;
	int arg2_len;
	void *arg1;
	void *arg2;
};

#ifndef CIPHER_IV_LEN
#define CIPHER_IV_LEN (128 / 8)
#endif
struct code_decrypt_arg_range {
	unsigned char iv[CIPHER_IV_LEN];
	unsigned long off;
	unsigned long size;
};
#undef CIPHER_IV_LEN

struct code_decrypt_inode_metadata {
	int version;
	int metadata_size;
	int app_id_off;
	int app_id_size;
	int range_off;
	int range_size;
};

/* Common commands. */
#define CODE_DECRYPT_CMD_SET_KEY						_IOW('c', 0x01, struct code_decrypt_arg)
#define CODE_DECRYPT_CMD_REMOVE_KEY						_IOW('c', 0x02, struct code_decrypt_arg)
#define CODE_DECRYPT_CMD_SET_ASSOCIATE_KEY				_IOW('c', 0x03, struct code_decrypt_arg)
#define CODE_DECRYPT_CMD_REMOVE_ASSOCIATE_KEY			_IOW('c', 0x04, struct code_decrypt_arg)
#define CODE_DECRYPT_CMD_CLEAR_CACHE_BY_KEY				_IOW('c', 0x05, struct code_decrypt_arg)
#define CODE_DECRYPT_CMD_CLEAR_CACHE_BY_FILE			_IOW('c', 0x06, struct code_decrypt_arg)
#define CODE_DECRYPT_CMD_CLEAR_SPECIFIC_CACHE			_IOW('c', 0x07, struct code_decrypt_arg)

#endif
