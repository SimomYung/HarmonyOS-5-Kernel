/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmpsfmod kobj
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 6 11:32:09 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_HMPSFMOD_H
#define INTAPI_CTRLMEM_KOBJS_HMPSFMOD_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <intapi/ctrlmem/kobjs/hmpsf.h>
#include <hmasm/memory.h>
#include <hmkernel/ctrlmem/kobjs/hmpsfmod.h>
#include <hmkernel/pgstr/pgstr.h>

struct __ctrlmem_kobj_hmpsfmod_s *ctrlmem_kobj_hmpsfmod_spawn(struct pgstr_s *pages_pgstr, __paddr_t pages_pa,
							      unsigned int pages_num);
int ctrlmem_kobj_hmpsfmod_destroy(struct __ctrlmem_kobj_hmpsfmod_s *kobj);

#endif
