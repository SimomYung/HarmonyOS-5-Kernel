/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: APIs for actv binder refer kobj
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 20 12:46:33 2024
 */

#ifndef INTAPI_CTRLMEM_KOBJS_ACTV_BINDER_REFER_H
#define INTAPI_CTRLMEM_KOBJS_ACTV_BINDER_REFER_H

#include <intapi/ctrlmem/kobjctnr.h>

#include <hmkernel/ctrlmem/kobjs/actv_binder_context.h>
#include <hmkernel/ctrlmem/kobjs/actv_binder_refer.h>

struct __ctrlmem_kobj_actv_binder_refer_s *
ctrlmem_kobj_actv_binder_refer_spawn(struct __ctrlmem_kobj_actv_binder_context_s *kobj_ab_ctx);

int ctrlmem_kobj_actv_binder_refer_destroy(struct __ctrlmem_kobj_actv_binder_refer_s *kobj);

int ctrlmem_kobj_actv_binder_refer_link(struct __ctrlmem_kobj_actv_binder_refer_s *kobj_ab_refer,
					cref_t actv_pool_cref, unsigned int handle);

int ctrlmem_kobj_actv_binder_refer_unlink(struct __ctrlmem_kobj_actv_binder_refer_s *kobj_ab_refer);

int ctrlmem_kobj_actv_binder_refer_revoke(struct __ctrlmem_kobj_actv_binder_refer_s *kobj_ab_refer);

#endif /* INTAPI_CTRLMEM_KOBJS_ACTV_BINDER_REFER_H */
