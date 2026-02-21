/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: intapi of actv binder
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 13 18:58:19 2023
 */

#ifndef INTAPI_ACTIVATION_ACTV_BINDER_H
#define INTAPI_ACTIVATION_ACTV_BINDER_H

#include <libstrict/strict.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libalgo/rbtree.h>

#include <lib/dlist.h>

struct process_s;

#ifdef CONFIG_ACTV_BINDER
struct actv_binder_s;

struct process_actv_binder_s {
	struct raw_mutex mutex;
	struct actv_binder_s *actv_binder;
};

void process_actv_binder_init(struct process_s *process);
void process_clean_actv_binder(struct process_s *process);
#endif /* CONFIG_ACTV_BINDER */

#endif /* !INTAPI_ACTIVATION_ACTV_BINDER_H */
