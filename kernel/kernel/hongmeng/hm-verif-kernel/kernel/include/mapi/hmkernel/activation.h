/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Activation interfaces for modules
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 13 16:29:02 2021
 */
#ifndef MAPI_HMKERNEL_ACTIVATION_H
#define MAPI_HMKERNEL_ACTIVATION_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>

struct tcb_s;
struct actv_s;
struct actv_pool_s;
struct CNode_s;

JAM_EXTERN_INLINE struct CNode_s *actv_cnode_of(const struct actv_s *actv);

JAM_EXTERN_INLINE struct tcb_s *actv_tcb_of(const struct actv_s *actv);

JAM_EXTERN_INLINE struct tcb_s *actv_base_tcb_of(const struct actv_s *actv);

JAM_EXTERN_INLINE struct actv_pool_s *
actv_pool_of(const struct actv_s *actv);

#endif
