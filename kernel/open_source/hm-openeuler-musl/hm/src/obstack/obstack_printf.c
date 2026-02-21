/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei RTOS
 * Create: Tues Aug 23 11:09:00 2021
 */
#include <stdio.h>
#include <stdarg.h>
#include "obstack.h"

int obstack_printf(struct obstack *h, const char *fmt, ...)
{
    va_list args;
    char buf[1024];

    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    if (len > 0) {
        obstack_grow(h, buf, (size_t)len);
    }

    va_end(args);
    return len;
}
