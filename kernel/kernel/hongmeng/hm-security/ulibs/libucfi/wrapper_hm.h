/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Delcaration of wrapped hongmeng function
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 30 11:30:11 2022
 */
#ifndef __WRAPPER_HM_H_
#define __WRAPPER_HM_H_
#include <stddef.h>
#include <stdint.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

typedef void (*hmcrt_posthook_type)(void);
typedef void *(*dlopen_type)(const char *file, int mode);
typedef int (*dlclose_type)(void *p);
void hmcrt_posthook(void);
void *dlopen(const char *file, int mode);
int dlclose(void *p);

#endif

