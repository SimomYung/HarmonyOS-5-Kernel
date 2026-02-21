/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: thread ref interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 16:35:22 2024
 */

#ifndef H_SYSMGR_INCLUDE_THREAD_REF_H
#define H_SYSMGR_INCLUDE_THREAD_REF_H

int thread_ref_sched_trans(cref_t src_cref, cref_t dst_cref, bool restore);
#endif
