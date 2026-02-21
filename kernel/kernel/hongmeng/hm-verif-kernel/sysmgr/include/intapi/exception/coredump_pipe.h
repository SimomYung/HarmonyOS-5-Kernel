/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Definition for coredump pipe file operations
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 20:16:38 2022
 */
#ifndef SYSMGR_INTAPI_EXCEPTION_COREDUMP_PIPE_H
#define SYSMGR_INTAPI_EXCEPTION_COREDUMP_PIPE_H

#include <hongmeng/errno.h>

#ifdef CONFIG_COREDUMP_PIPE
unsigned int coredump_pipe_limit_read(void);
void coredump_pipe_limit_write(unsigned int limit);
#else
static inline unsigned int coredump_pipe_limit_read(void)
{
	return 0;
}

static inline void coredump_pipe_limit_write(unsigned int limit)
{
	UNUSED(limit);
}
#endif

#endif
