/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj syspgarr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 02 11:47:05 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_SYSPGARR_H
#define INTAPI_CTRLMEM_KOBJS_SYSPGARR_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/syspgarr.h>

extern struct __ctrlmem_kobj_syspgarr_s *ctrlmem_kobj_syspgarr_spawn_sparse(unsigned long array_start);

extern struct __ctrlmem_kobj_syspgarr_s *ctrlmem_kobj_syspgarr_spawn_dpa(void);

extern void ctrlmem_kobj_syspgarr_set(struct __ctrlmem_kobj_syspgarr_s *syspgarr);

extern struct __ctrlmem_kobj_syspgarr_s *ctrlmem_kobj_syspgarr_global(void);

#endif
