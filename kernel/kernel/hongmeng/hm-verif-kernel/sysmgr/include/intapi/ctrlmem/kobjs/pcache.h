/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj pcache
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 01 18:45:05 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_PCACHE_H
#define INTAPI_CTRLMEM_KOBJS_PCACHE_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/pcache.h>
#include <hmkernel/ctrlmem/kobjs/syspgarr.h>
#include <hmkernel/ctrlmem/pcache.h>

#define ZPAGE_INIT_REFCNT 3 /* lru need an extra refcnt */
#define ZPAGE_INIT_MAPCNT 1

extern struct __ctrlmem_kobj_pcache_s *ctrlmem_kobj_pcache_spawn(struct __ctrlmem_pcache_s *pcpg);

#endif
