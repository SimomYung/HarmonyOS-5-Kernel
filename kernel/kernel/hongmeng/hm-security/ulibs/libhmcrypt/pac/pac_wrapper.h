/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: PAC wrapper functions header
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 17 11:43:29 2024
 */

#ifndef SYSMGR_PAC_WRAPPER_H
#define SYSMGR_PAC_WRAPPER_H

#include <hongmeng/types.h>
#include <pthread.h>

void __wrap_qsort (void *, size_t, size_t, int (*)(const void *, const void *));
void __real_qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int __wrap_pthread_once(pthread_once_t *, void (*)(void));
int __real_pthread_once(pthread_once_t *, void (*)(void));

int __wrap_pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int __real_pthread_key_create(pthread_key_t *key, void (*destructor)(void*));

int __wrap_atexit(void (*func)(void));
int __real_atexit(void (*func)(void));
#endif
