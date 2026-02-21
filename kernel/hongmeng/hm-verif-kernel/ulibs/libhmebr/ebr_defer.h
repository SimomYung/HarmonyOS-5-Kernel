/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declarations for ebr deferring synchronization
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 29 14:05:30 2020
 */
#ifndef ULIBS_LIBHMEBR_EBR_DEFER_H
#define ULIBS_LIBHMEBR_EBR_DEFER_H

#include <libhmebr/ebr.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_thread_cond.h>

enum worker_state {
	EBR_WORKER_STOP,
	EBR_WORKER_START,
};

struct call_ebr_obj {
	struct cb_stack cb_head;
	raw_atomic_int_t call_watermark;

	/* worker thread related */
	struct raw_mutex worker_lock;
	raw_thread_t wtid;
	enum worker_state state;

	/* cond variable used for sync between call_ebr and worker thread */
	struct raw_thread_cond call_cond;
	struct raw_mutex call_lock;
};

void call_ebr_obj_destroy(struct epoch_set *es);

#endif
