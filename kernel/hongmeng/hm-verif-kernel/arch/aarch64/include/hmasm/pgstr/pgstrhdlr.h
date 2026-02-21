/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: arch page struct table handler for aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 02 06:41:40 2022
 */

#ifndef AARCH64_INCLUDE_HMASM_PGSTR_PGSTRHDLR_H
#define AARCH64_INCLUDE_HMASM_PGSTR_PGSTRHDLR_H

#ifndef __BUILD_ARCH_PGSTRTBL__
#error This header should not be used alone
#endif

#include <hmkernel/compiler.h>

#include <mapi/hmasm/pgstr/pgstrhdlr.h>

#define PGTBLHDLR_PGSTRENTRY_TYPE_ROOT		__PGTBLHDLR_PGSTRENTRY_TYPE_ROOT
#define PGTBLHDLR_PGSTRENTRY_TYPE_SMALLEST_BLK	__PGTBLHDLR_PGSTRENTRY_TYPE_SMALLEST_BLK

struct pgstrentry_root_s {
	struct __pgstrentry_root_s root;
};

#define PGSTRENTRY_ROOT_INIT { .root = __PGSTRENTRY_ROOT_INIT }

static inline __always_inline struct pgstrentry_s
pgstrentry_make_root(struct pgstrentry_root_s *s)
{
	struct pgstrentry_s e = {
		.e.u.a64pgstr_top = &s->root.root,
	};
	return e;
}

#endif
