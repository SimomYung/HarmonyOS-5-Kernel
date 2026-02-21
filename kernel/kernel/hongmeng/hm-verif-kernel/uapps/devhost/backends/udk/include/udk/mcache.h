/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: mcache for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thur Dec 14 19:27:05 2023
 */
#ifndef __UDK_CACHE_H__
#define __UDK_CACHE_H__

#include <stdint.h>
#include <stddef.h>

#include <libhmsync/bitops/generic.h>

struct udk_mcache_s;
typedef int (*udk_mcache_foreach_func)(void *);
struct udk_mcache_s *udk_mcache_create(char *name, unsigned int num, unsigned int obj_size);
void udk_mcache_destroy(struct udk_mcache_s *mcache);
void *udk_mcache_alloc(struct udk_mcache_s *mcache);
void udk_mcache_free(struct udk_mcache_s *mcache, void *va);
int udk_mcache_foreach(struct udk_mcache_s *mcache, udk_mcache_foreach_func f);
#endif
