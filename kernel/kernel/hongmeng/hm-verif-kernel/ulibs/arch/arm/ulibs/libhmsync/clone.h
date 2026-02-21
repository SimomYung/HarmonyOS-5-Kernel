/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: clone
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 27 11:12:22 2024
 */
#ifndef ARM_ULIBS_ASM_LIBHMSYNC_CLONE_H
#define ARM_ULIBS_ASM_LIBHMSYNC_CLONE_H

#define ARGS_IN_STACK 6 /* include clone_flags~ctid, 6 unsigned long */

__attribute__((visibility("hidden")))
int ____clone(unsigned long long rpcinfo, unsigned long buf,
	      unsigned long long flags, unsigned long stack,
	      unsigned long ptid, unsigned long newtls,
	      unsigned long ctid, unsigned long entry,
	      unsigned long args, unsigned modifier);
#endif
