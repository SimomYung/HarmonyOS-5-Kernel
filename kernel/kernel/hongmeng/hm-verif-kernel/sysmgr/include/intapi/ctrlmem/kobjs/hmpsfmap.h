/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmpsfmap kobj
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 9 16:35:40 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_HMPSFMAP_H
#define INTAPI_CTRLMEM_KOBJS_HMPSFMAP_H

#include <intapi/ctrlmem/kobjctnr.h>
#include <intapi/ctrlmem/kobjs/hmpsf.h>
#include <hmasm/memory.h>
#include <hmkernel/ctrlmem/kobjs/hmpsfmap.h>
#include <hmkernel/pgstr/pgstr.h>

struct __ctrlmem_kobj_hmpsfmap_s *ctrlmem_kobj_hmpsfmap_spawn(struct pgstr_s *pages_pgstr, __paddr_t pages_pa,
							      unsigned int pages_num, uint64_t flags);
int ctrlmem_kobj_hmpsfmap_destroy(struct __ctrlmem_kobj_hmpsfmap_s *kobj);

#endif
