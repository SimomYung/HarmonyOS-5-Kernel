/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: APIs for actv binder context kobj
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 19 06:51:01 2024
 */
#ifndef INTAPI_CTRLMEM_KOBJS_ACTV_BINDER_CONTEXT_H
#define INTAPI_CTRLMEM_KOBJS_ACTV_BINDER_CONTEXT_H

#include <intapi/ctrlmem/kobjctnr.h>

#include <hmkernel/ctrlmem/kobjs/actv_binder_context.h>

struct __ctrlmem_kobj_actv_binder_context_s *
ctrlmem_kobj_actv_binder_context_spawn(unsigned int devno, unsigned int flags, cref_t actv_pool_cref);

int ctrlmem_kobj_actv_binder_context_destroy(struct __ctrlmem_kobj_actv_binder_context_s *kobj);

#endif /* INTAPI_CTRLMEM_KOBJS_ACTV_BINDER_CONTEXT_H */
