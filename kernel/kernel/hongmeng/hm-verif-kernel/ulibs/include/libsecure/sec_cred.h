/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Secure credential definition
 * Author: Huawei OS Kernel Lab
 * Create: Thi Nov 13 10:43:11 2019
 */

#ifndef ULIBS_INCLUDE_LIBSECURE_SEC_CRED_H
#define ULIBS_INCLUDE_LIBSECURE_SEC_CRED_H

enum sec_cred_selfcall_type {
	__SEC_CRED_NO_SELFCALL,
	__SEC_CRED_ACTV_SELFCALL
};

#define SEC_CRED_SELFCALL_FWD_USE_SELF 0x1
#define SEC_CRED_SELFCALL_FWD_NOT_USE_SELF 0x0

#define SEC_CRED_VALID 0x1
#define SEC_CRED_INVALID 0x0

struct sec_cred {
	unsigned long long sender;
	unsigned long credential;
	enum sec_cred_selfcall_type selfcall_type;
	union {
		struct {
			unsigned char selfcall_fwd_use_self :1;
			unsigned char fwd_base_cred_valid :1;
			unsigned char fwd_sh_cred_valid :1;
			unsigned char src_base_cred_valid :1;
			unsigned char src_sh_cred_valid :1;
			unsigned char reserved_flags :3;
		} cred_flags;
		unsigned char u_cred_flags;
	};
	unsigned long long self_cred_idx;
};

#endif
