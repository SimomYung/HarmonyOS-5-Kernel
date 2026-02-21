/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Audit kernel uapi interface header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 29 10:17:33 CST 2021
 */

#ifndef UAPI_HMKERNEL_AUDIT_H
#define UAPI_HMKERNEL_AUDIT_H

#include <hmkernel/types.h>

#define __AUDIT_SYSCALL_ARGC	4
#define __AUDIT_TYPE_CTX_DATA	0
#define __AUDIT_TYPE_CTX_DEL	1
#define __AUDIT_TYPE_AVC_INVALID	0
#define __AUDIT_TYPE_AVC_CRIGHT	1
#define __AUDIT_TYPE_AVC_PERMISSION	2
#define __AUDIT_DATA_LEN	sizeof(struct __audit_syscall_data)

enum __audit_ops {
	__AUDIT_CONTEXT_DISABLE,
	__AUDIT_CONTEXT_ENABLE,
	__AUDIT_CONTEXT_DELETE,
};

enum __audit_cmd {
	__AUDIT_CMD_DISABLE,
	__AUDIT_CMD_ENABLE,
};

struct __audit_syscall_avc {
	__u32 avc_type;
	union {
		struct {
			__u32 hmobj_type;
			__u8 req_cright;
		} cright;
		__u64 req_cpermission;
	};
};

struct __audit_syscall_data {
	__u32 audit_data_type;
	__u32 capno;
	__u32 method;
	__u32 proxy_call_flag;
	__u64 timestamp;
	__u64 argv[__AUDIT_SYSCALL_ARGC];
	__s64 ret_code;
	__u32 cnode_idx;
	__u32 arch;
	__uptr_t context_id;
	struct __audit_syscall_avc avc_info;
};
#endif
