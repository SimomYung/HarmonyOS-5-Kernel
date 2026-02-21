/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of sched proto for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 02 20:41:17 2020
 */
#ifndef UAPI_SCHED_H
#define UAPI_SCHED_H

#define SCHED_OTHER	0
#define SCHED_FIFO	1
#define SCHED_RR	2
#define SCHED_BATCH	3
#define SCHED_IDLE	5
#define SCHED_EDF	6
#define SCHED_NULL	7

#define SCHED_USER_IDLE	9 /* policy of HM_RUNQUEUE_MODULE_IDLE */
#define SCHED_STOP	10 /* policy of HM_RUNQUEUE_MODULE_STOP */

#define SCHED_RESET_ON_FORK	0x40000000

#define SCHED_FLAG_RESET_ON_FORK	0x01
#define SCHED_FLAG_KEEP_POLICY		0x08
#define SCHED_FLAG_KEEP_PARAMS		0x10
#define SCHED_FLAG_UTIL_CLAMP_MIN	0x20
#define SCHED_FLAG_UTIL_CLAMP_MAX	0x40

#define SCHED_FLAG_UTIL_CLAMP	(SCHED_FLAG_UTIL_CLAMP_MIN | \
				 SCHED_FLAG_UTIL_CLAMP_MAX)

#define SCHED_FLAG_ALL		(SCHED_FLAG_RESET_ON_FORK	| \
				 SCHED_FLAG_KEEP_POLICY		| \
				 SCHED_FLAG_KEEP_PARAMS		| \
				 SCHED_FLAG_UTIL_CLAMP)
#endif
