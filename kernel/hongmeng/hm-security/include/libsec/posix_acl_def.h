/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Macros and structures of POSIX access control list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 16 00:29:24 2021
 */
#ifndef __POSIX_ACL_DEF_H
#define __POSIX_ACL_DEF_H

#include <unistd.h>
#include <libhmsync/raw_scopedptr.h>
#include "posix_acl_api.h"

struct sec_posix_acl_entry {
	uint16_t tag;
	uint16_t permission;
	union {
		uid_t	uid;
		gid_t	gid;
		uint32_t padding;
	} qualifier;
};

struct sec_posix_acl {
	struct raw_refcnt_nr refcnt;
	uint32_t entry_num;
	struct sec_posix_acl_entry entries[0];
};

/* trival ACLs can be described by file modes */
#define SEC_POSIX_ACL_TRIVIAL 1

#endif /* ifndef __POSIX_ACL_DEF_H */
