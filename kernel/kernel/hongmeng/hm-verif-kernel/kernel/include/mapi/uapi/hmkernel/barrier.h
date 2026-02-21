/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of barrier
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 7 11:06:49 2018
 */
#ifndef UAPI_HMKERNEL_BARRIER_H
#define UAPI_HMKERNEL_BARRIER_H

#include <hmkernel/compiler.h>
#include <hmasm/barrier.h>

#ifdef CONFIG_SMP

#define smp_mb()  __smp_mb()
#define smp_rmb() __smp_rmb()
#define smp_wmb() __smp_wmb()

#ifndef smp_write_rel
#define smp_write_rel(p, v)	\
do {				\
	smp_mb();		\
	__write_once(p, v);	\
} while (0)
#endif

#else
/* if no smp support */
#define smp_mb()  barrier()
#define smp_rmb() barrier()
#define smp_wmb() barrier()

#ifndef smp_write_rel
#define smp_write_rel(p, v)	\
do {				\
	barrier();		\
	__write_once(p, v);	\
} while (0)
#endif

#endif

#define MEMBARRIER_CMD_QUERY					(0)
#define MEMBARRIER_CMD_SHARED					(1)
#define MEMBARRIER_CMD_PRIVATE_EXPEDITED		(1 << 3)
#define MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED	(1 << 4)

/* Currently supported command */
#define MEMBARRIER_SUPPORTED_CMDS (MEMBARRIER_CMD_PRIVATE_EXPEDITED | \
		MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED | MEMBARRIER_CMD_SHARED)

#endif
