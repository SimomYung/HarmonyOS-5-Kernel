/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: clone
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 27 11:10:14 2024
 */

#ifndef AARCH64_ULIBS_ASM_LIBHMSYNC_CLONE_H
#define AARCH64_ULIBS_ASM_LIBHMSYNC_CLONE_H

#define ARGS_IN_STACK 0

__attribute__((visibility("hidden")))
int ____clone(unsigned long long rpcinfo, unsigned long buf,
	      unsigned long long clone_flags, unsigned long stack,
	      unsigned long ptid, unsigned long tls, unsigned long ctid,
	      unsigned long func, unsigned long arg, unsigned long modifier);
#endif
