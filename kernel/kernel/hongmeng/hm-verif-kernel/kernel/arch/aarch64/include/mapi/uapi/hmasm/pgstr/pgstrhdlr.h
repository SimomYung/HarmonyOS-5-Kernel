/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: arch page struct table handler for aarch64 - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 03 12:02:56 2022
 */

#ifndef AARCH64_INCLUDE_MAPI_UAPI_HMASM_PGSTR_PGSTRHDLR_H
#define AARCH64_INCLUDE_MAPI_UAPI_HMASM_PGSTR_PGSTRHDLR_H

#include <hmkernel/pgstr/builder.h>

#define __PGTBLHDLR_PGSTRENTRY_TYPE_ROOT		__PGTBLHDLR_PGSTR_PGENTRY_TYPE_A64PGSTR_ROOT
#define __PGTBLHDLR_PGSTRENTRY_TYPE_SMALLEST_BLK	__PGTBLHDLR_PGSTR_PGENTRY_TYPE_A64PGSTR_P4K

struct __pgstrentry_root_s {
	struct __pgtblhdlr_rawfmt_a64pgstr_top_s root;
};

#define __PGSTRENTRY_ROOT_INIT { .root.u.rawv = (__u64)(0), }

#endif
