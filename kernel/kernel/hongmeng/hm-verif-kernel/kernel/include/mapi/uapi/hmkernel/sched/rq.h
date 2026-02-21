/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: HongMeng specific runqueues definitions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 12 15:18:01 2020
 */
#ifndef UAPI_HMKERNEL_SCHED_RQ_H
#define UAPI_HMKERNEL_SCHED_RQ_H

/* module managed soft rt rq range [2, 31], different rq means different policy */
#define HM_RUNQUEUE_MODULE_IDLE 2U
#define HM_RUNQUEUE_MODULE_OTHER 6U
#define HM_RUNQUEUE_MODULE_RT 7U
#define HM_RUNQUEUE_MODULE_EDF 8U
/*
 * HM_RUNQUEUE_MODULE_STOP is deferent from linux stop_class.
 * Currently, it is only used for the cpuhp thread.
 */
#define HM_RUNQUEUE_MODULE_STOP	30U
#define HM_RUNQUEUE_MODULE_BOOST 31U

#endif
