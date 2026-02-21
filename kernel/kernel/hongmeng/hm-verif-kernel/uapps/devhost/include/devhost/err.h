/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Help functions for devhost err and ptr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 12 14:31:35 2019
 */
#ifndef __DEVHOST_API_ERR_H__
#define __DEVHOST_API_ERR_H__

#ifndef IS_ERR
static inline void *ERR_PTR(long error)
{
	return (void*)error;
}

static inline long PTR_ERR(const void *ptr)
{
	return (long)(uintptr_t)ptr;
}

#define IS_ERR(ptr)	((unsigned long)(ptr) > (unsigned long)(-4096))
#define IS_ERR_OR_NULL(ptr)	(unlikely((ptr) == NULL) || IS_ERR(ptr))
#endif

#endif /* __DEVHOST_API_ERR_H__ */
