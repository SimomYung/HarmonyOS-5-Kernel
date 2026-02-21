/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: User interfaces of POSIX access control list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 05:22:12 2021
 */
#ifndef __POSIX_ACL_API_H
#define __POSIX_ACL_API_H

#include <stdint.h>
#include <hmkernel/const.h>

/* version of header */
#define HM_POSIX_ACL_VERSION		2u

/* type */
#define HM_POSIX_ACL_TYPE_ACCESS	0x8000u
#define HM_POSIX_ACL_TYPE_DEFAULT	0x4000u

/* tag */
#define HM_POSIX_ACL_UNDEFINED_TAG	0x0u
#define HM_POSIX_ACL_USER_OBJ		0x1u
#define HM_POSIX_ACL_USER		0x2u
#define HM_POSIX_ACL_GROUP_OBJ		0x4u
#define HM_POSIX_ACL_GROUP		0x8u
#define HM_POSIX_ACL_MASK		0x10u
#define HM_POSIX_ACL_OTHER		0x20u

/* permission */
#define HM_POSIX_ACL_READ	4u
#define HM_POSIX_ACL_WRITE	2u
#define HM_POSIX_ACL_EXEC	1u

/* qualifier */
#define HM_POSIX_ACL_UNDEFINED_ID	0xffffffffu

struct hm_posix_acl_xattr_entry {
	uint16_t tag;
	uint16_t permission;
	uint32_t qualifier; /* uid or gid */
};

struct hm_posix_acl_xattr_header {
	uint32_t version;
};

#endif /* ifndef __POSIX_ACL_API_H */
