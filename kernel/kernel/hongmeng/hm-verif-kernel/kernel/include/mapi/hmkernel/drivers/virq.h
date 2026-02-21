/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Export kernel virq.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 18 10:33:41 2023
 */
#ifndef MAPI_HMKERNEL_VIRQ_H
#define MAPI_HMKERNEL_VIRQ_H

#include <uapi/hmkernel/drivers/virq.h>

struct CNode_s;
int virq_bind(struct CNode_s *caller, __u32 virt_irq, cref_t cref, int retval);
int virq_unbind(struct CNode_s *caller, __u32 virt_irq);
int virq_grant(struct CNode_s *caller, __u32 virt_irq, __u32 cnode_idx);
int virq_translate(struct CNode_s *caller, __u32 hw_irq);

#endif
