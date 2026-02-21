/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: The Actv Upcall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 16 18:02:01 2021
 */

#ifndef AARCH64_ASM_ACTIVATION_UPCALL_H
#define AARCH64_ASM_ACTIVATION_UPCALL_H

#include <mapi/hmasm/activation/upcall.h>

#include <hmkernel/compiler.h>

static_assert(sizeof(union __arch_actv_upcall_args_u) <=
	      sizeof(__u64) * __U(__ARCH_ACTV_UPCALL_ARG_NR),
	      size_of_arch_actv_upcall_args_too_large);

#endif /* !AARCH64_ASM_ACTIVATION_UPCALL_H */
