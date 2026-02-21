/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: FS Error Pointer Definitions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 30 12:08:33 2024
 */
#ifndef __FS_ERR_H
#define __FS_ERR_H

#define MAX_ERRNO 4095
#define IS_ERR(ptr)    ((unsigned long)(ptr) >= (unsigned long)(-MAX_ERRNO))
#define IS_ERR_OR_NULL(ptr)    (unlikely((ptr) == NULL) || IS_ERR(ptr))

static inline void *ERR_PTR(long error)
{
       return (void*)error;
}

static inline long PTR_ERR(const void *ptr)
{
       return (long)ptr;
}

#endif /* __FS_ERR_H */
