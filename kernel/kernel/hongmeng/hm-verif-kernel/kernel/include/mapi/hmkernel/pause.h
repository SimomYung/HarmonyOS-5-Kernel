/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: __NR_pause header file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 20 09:47:46 2023
 */

#ifndef MAPI_HMKERNEL_PAUSE_H
#define MAPI_HMKERNEL_PAUSE_H

struct tcb_s;
extern bool pause_intr(int *retval);

#endif /* MAPI_HMKERNEL_PAUSE_H */
