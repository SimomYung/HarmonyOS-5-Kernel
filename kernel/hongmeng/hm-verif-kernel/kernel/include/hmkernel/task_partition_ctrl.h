/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: task partition ctrl func
 * Create: Wed Nov 2 17:13:00 CST 2024
 */
#ifndef HMKERNEL_TASK_PARTITION_CTRL_H
#define HMKERNEL_TASK_PARTITION_CTRL_H
 
#if defined(CONFIG_L3CACHE_PARTITION_CTRL)
#include <hmkernel/l3c_part_ctrl.h>
#elif defined(CONFIG_MPAM_SCHED)
#include <hmkernel/mpam_sched.h>
#endif
#endif /* !HMKERNEL_TASK_PARTITION_CTRL_H */
