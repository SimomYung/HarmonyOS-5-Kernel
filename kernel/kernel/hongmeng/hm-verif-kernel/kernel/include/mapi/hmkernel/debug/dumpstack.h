/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Dumpstack for modules
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 12 09:54:34 2020
 */
#ifndef MAPI_HMKERNEL_DUMPSTACK_H
#define MAPI_HMKERNEL_DUMPSTACK_H

#include <hmkernel/types.h>
#include <hmasm/stackunwind.h>

struct stack_frame_s;
void dump_kernel_stack(struct stack_frame_s frame, bool nolock, bool is_panic);
void dump_current_kernel_stack(bool dump_locktrace, bool nolock, bool is_panic);
#endif
