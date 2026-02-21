/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: The implement of printing error message
 * Author:  Huawei RTOS
 * Create: 2021-09-13 11:05:50
 */
#define _GNU_SOURCE
#include <errno.h>
#include <error.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <atomic.h>
#include "stdio_impl.h"

int error_message_count = 0;
void error(int status, int errnum, const char *format, ...)
{
    va_list ap;
    FLOCK(stderr);
    (void)fprintf(stderr, "%s: ", program_invocation_name);
    va_start(ap, format);
    (void)vfprintf(stderr, format, ap);
    va_end(ap);

    if (errnum != 0) {
        (void)fprintf(stderr, ": %s\n", strerror(errnum));
    }
    FUNLOCK(stderr);
    a_inc(&error_message_count);
    if (status != 0) {
        exit(status);
    }
}
