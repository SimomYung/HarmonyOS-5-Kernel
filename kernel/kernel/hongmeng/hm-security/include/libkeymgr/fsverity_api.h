/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Fsverity Key's key_type implementation
 * Author: Huawei OS Kernel Lab
 * Create: Tue July 25 12:10:24 2023
 */

#ifndef KEYMGR_FSVERITY_H
#define KEYMGR_FSVERITY_H
/* libc */
#include <stdint.h>
#include <string.h>

#define FSVERITY_CERT_MAX_SIZE		(4096)

#define FSVERITY_CTRL_CREATE_KEYRING	(1)
#define FSVERITY_CTRL_GET_CERTDATA	(2)
#define FSVERITY_CTRL_GET_KEYRINGID	(3)
#define FSVERITY_CTRL_SEARCH_KEYDATA	(4)
#define FSVERITY_CTRL_GET_ALLCERTS	(5)
#define FSVERITY_CTRL_OP_MAX		(7)

struct fsverity_ctl_get_certdata_arg {
	int cert_id;
	uint8_t *res;
};

struct fsverity_ctl_create_keyring_arg {
	void *res;
};

struct fsverity_ctl_get_keyringid_arg {
	size_t res_len;
	void *res;
};

struct fsverity_ctl_get_allcerts_arg {
	uint8_t *out_certs_num;
	void *certs;
	uint8_t certs_num;
};

#define FSVERITY_MAX_KEY_SIZE	(128)
struct fsverity_ctl_search_keydata_arg {
	char desc[FSVERITY_MAX_KEY_SIZE];
	size_t cmp_len;
	uint8_t *payload;
};

#ifdef CONFIG_VFS_KEYMGR_DIRECT
int keymgr_fsverity_ctrl(int operation, void *arg_addr, size_t arg_size);
#endif
#endif
