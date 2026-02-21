/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header file for kobjs_list
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 29 15:29:11 2020
 */

DEFINE_KOBJ_ALLOCATOR(hmobj_Channel, chn, PAGE_SIZE * 2, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_CpuContext, cpuctx, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_DsuContext, dsuctx, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_Timer, timer, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_ActivationPool, actv_pool, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_Activation, actv, PAGE_SIZE, 100)

DEFINE_KOBJ_ALLOCATOR(hmobj_Livepatch, livepatch, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_EcEp, ec_ep, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_EcChn, ec_chn, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_Epoll, epoll, PAGE_SIZE, 0)

DEFINE_KOBJ_ALLOCATOR(hmobj_FDTable, fdtable, PAGE_SIZE, 25)
