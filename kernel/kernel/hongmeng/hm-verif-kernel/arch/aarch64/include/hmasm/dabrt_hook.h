/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: data abort hook
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 19 08:11:53 2022
 */

#ifndef A64_ASM_DABRT_HOOK_H
#define A64_ASM_DABRT_HOOK_H

#include <hmkernel/kernel.h>

ENUM(3, dabrt_hook_type, DABRT_HOOK_TYPE_,
	NO_FIXUP,
	FASTPATH,
	ROLLBACK,
)

#ifdef CONFIG_DABRT_ROLLBACK_FIXUP
#define DABRT_ROLLBACK_BUFSIZE 13U

struct dabrt_rollback_buf {
	u64 buffer[DABRT_ROLLBACK_BUFSIZE];
};

extern const unsigned long dabrt_rollback[];
extern int dabrt_rollback_prepare(struct dabrt_rollback_buf *buf);
#endif

struct dabrt_hook {
	enum dabrt_hook_type type;
#ifdef CONFIG_DABRT_ROLLBACK_FIXUP
	struct dabrt_rollback_buf rollback_buf __aligned(16U);
#endif
};
#endif
