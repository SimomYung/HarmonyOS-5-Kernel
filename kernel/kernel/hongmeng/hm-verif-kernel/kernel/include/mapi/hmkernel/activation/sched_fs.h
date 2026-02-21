/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Sched fs info interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 18 17:07:09 2023
 */

#ifndef MAPI_HMKERNEL_ACTIVATION_SCHED_FS_H
#define MAPI_HMKERNEL_ACTIVATION_SCHED_FS_H

#include <hmkernel/activation/sched.h>

#ifdef CONFIG_ACTV_SCHED_FS
JAM_INLINE u64
actv_fs_info_sync(struct actv_s *actv);
JAM_INLINE u64
actv_fs_saved_uctx_of(struct actv_s *actv);
JAM_INLINE u64
actv_fs_callback_stk_of(struct actv_s *actv);
#endif

#endif
