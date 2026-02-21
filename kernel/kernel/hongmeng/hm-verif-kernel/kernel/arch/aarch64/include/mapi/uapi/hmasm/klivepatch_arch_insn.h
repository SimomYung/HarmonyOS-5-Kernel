/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Aarch64 Livepatch instructions struct
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 18 17:15:23 2022
 */

#ifndef ARCH64_LIVEPATCH_ARCH_INSN_H
#define ARCH64_LIVEPATCH_ARCH_INSN_H

#include <hmasm/types.h>

#define LJMP_INSN_SIZE 5
struct livepatch_old_insns_s {
	__u32 old_insns[LJMP_INSN_SIZE];
};

#endif
