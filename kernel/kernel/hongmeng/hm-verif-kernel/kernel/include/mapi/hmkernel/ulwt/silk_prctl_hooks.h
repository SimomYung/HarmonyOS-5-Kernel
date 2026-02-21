/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ULWT prctl hooks
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 12 11:49:27 2024
 */

#ifndef MAPI_HMKERNEL_ULWT_SILK_PRCTL_HOOKS_H
#define MAPI_HMKERNEL_ULWT_SILK_PRCTL_HOOKS_H

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(SILK);

#ifdef CONFIG_ULWT

int handle_silk_syscall_ops(struct arch_uctx *uctx, int op, bool *hooked);

#endif /* CONFIG_ULWT */

#endif
