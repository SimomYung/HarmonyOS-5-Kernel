/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: The interface of printing error message
 * Author:  Huawei RTOS
 * Create: 2021-09-13 11:05:50
 */
#ifndef _ERROR_H
#define _ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <errno.h>

extern int error_message_count;
extern void error(int status, int errnum, const char *format, ...);

#ifdef __cplusplus
}
#endif


#endif
