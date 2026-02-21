/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 20 14:58:00 2019
 */

#ifndef ARCH_AARCH64_INCLUDE_MAPI_UAPI_ASM_ARCH_UPROBE_INSN_H
#define ARCH_AARCH64_INCLUDE_MAPI_UAPI_ASM_ARCH_UPROBE_INSN_H

#include <hmasm/types.h>

/* relocate the PC-relative insntruction is mandatory */
#define ARCH_RELOCATE_MANDATORY		0
/*
 * relocate the PC-relative insntruction is optional,
 * copy raw instruction if relocate failed or unsupported.
 */
#define ARCH_RELOCATE_OPTIONAL		1

#define ARCH_RELOCATE_DEFAULT		ARCH_RELOCATE_MANDATORY

struct arch_callback_info_s {
	const void *trap_point;
	__u32 trap_point_size;
	const void *callback_buffer;
	__u32 callback_buffer_size;
	int relocate_option;
};

struct arch_trap_info_s {
	const void *trap_point;
	__u32 trap_point_size;
	const void *trap_entry;
};

__u32 arch_make_callback_insn(const struct arch_callback_info_s *cb_info,
			      __u32 *result, __u32 size);
__u32 arch_make_trap_insn(const struct arch_trap_info_s *trap_info,
			  __u32 *result, __u32 size);

#endif
