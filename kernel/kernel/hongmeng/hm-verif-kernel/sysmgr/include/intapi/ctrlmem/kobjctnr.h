/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Sysmgr ctrlmem kobj container handler
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 01 18:27:41 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJCTNR_H
#define INTAPI_CTRLMEM_KOBJCTNR_H

#include <hongmeng/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>

#include <hmkernel/ctrlmem/kobjctnr.h>

void ctrlmem_bkobjctnr_init(const __u64 *meta);
void ctrlmem_bkobjthreadctnr_init(void);
void ctrlmem_bkobjctnr_collect(void);

struct __ctrlmem_kobj_vspace_s;
struct __ctrlmem_kobj_vspace_s *ctrlmem_bkobjctnr_sysmgr_kobjvs(void);
struct __ctrlmem_kobj_process_s;
struct __ctrlmem_kobj_process_s *ctrlmem_bkobjctnr_sysmgr_kobjpr(void);

struct ctrlmem_kobjpool_s;

int ctrlmem_kobjpool_reclaim(struct ctrlmem_kobjpool_s *_pool);

#endif
