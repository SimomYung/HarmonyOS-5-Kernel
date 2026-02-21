/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: device header file for security device
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 21 14:40:09 2023
 */

#ifndef VFS_SECFD_H
#define VFS_SECFD_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include <libalgo/rbtree.h>

#define SEC_ACCESS_TOKEN_FD_MINOR	0x10

#define HM_ACCESS_TOKEN_ID_IOCTL_BASE 'A'
#define MAX_PERM_GROUP_NUM 64

enum {
	HM_GET_TOKEN_ID = 1,
	HM_SET_TOKEN_ID,
	HM_GET_FTOKEN_ID,
	HM_SET_FTOKEN_ID,
	HM_ADD_PERMISSION,
	HM_REMOVE_PERMISSION,
	HM_GET_PERMISSION,
	HM_SET_PERMISSION,
	HM_ACCESS_TOKENID_MAX_NR
};

struct access_token_inner {
	unsigned int token_uniqueid : 20;
	unsigned int res : 5;
	unsigned int render_flag : 1;
	unsigned int dlp_flag : 1;
	unsigned int type : 2;
	unsigned int version : 3;
};

struct access_token_perm_op {
	uint32_t token;
	uint32_t op_code;
	bool is_granted;
};

struct access_token_perm_data {
	uint32_t token;
	uint32_t perm[MAX_PERM_GROUP_NUM];
};

struct access_token_perm {
	struct rbt_node node;
	struct access_token_perm_data data;
};

#define ACCESS_TOKENID_GET_TOKENID _IOR(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_GET_TOKEN_ID, uint64_t)
#define ACCESS_TOKENID_SET_TOKENID _IOW(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_SET_TOKEN_ID, uint64_t)
#define ACCESS_TOKENID_GET_FTOKENID _IOR(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_GET_FTOKEN_ID, uint64_t)
#define ACCESS_TOKENID_SET_FTOKENID _IOW(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_SET_FTOKEN_ID, uint64_t)
#define ACCESS_TOKENID_ADD_PERMISSION _IOW(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_ADD_PERMISSION, \
	struct access_token_perm_data)
#define ACCESS_TOKENID_REMOVE_PERMISSION _IOW(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_REMOVE_PERMISSION, uint32_t)
#define ACCESS_TOKENID_GET_PERMISSION _IOW(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_GET_PERMISSION, \
	struct access_token_perm_op)
#define ACCESS_TOKENID_SET_PERMISSION _IOW(HM_ACCESS_TOKEN_ID_IOCTL_BASE, HM_SET_PERMISSION, \
	struct access_token_perm_op)

#endif // VFS_SECFD_H
