/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The implement of hmposix
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 16:50:16 2022
 */
#include <errno.h>
#include <malloc.h>
#include <stddef.h>
#include "himalloc_hmposix.h"

int posix_memalign(void **res, size_t align, size_t len)
{
    if (res == NULL) {
        return EINVAL;
    }

    if (align < sizeof(void *)) {
        return EINVAL;
    }

    if ((align & (align - 1UL)) != 0UL) {
        return EINVAL;
    }

    void *p = memalign(align, len);

    if ((p == NULL) && (len != 0UL)) {
        return ENOMEM;
    }

    *res = p;

    return 0;
}

void *aligned_alloc(size_t align, size_t len)
{
    return memalign(align, len);
}

int mallopt(int param, int value)
{
    (void)param;
    (void)value;

    return 0;
}
