/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide operations about ring buffer in hmtrace framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libhmtrace/hmtrace.h>
#include "hmtrace_core.h"

#define RB_EVENT_VECTORS_SIZE 2
static int write_event_prepare(struct hmtrace_event *trace_event,
			       struct event_data *event_data)
{
	int rc;
	struct event_entry *event_entry = NULL;
	struct timespec timespec;

	event_entry = &trace_event->event_entry;
	if (!is_event_registered(event_entry)) {
		hm_warn("event not registered yet\n");
		return E_HM_INVAL;
	}

	if (event_entry->status == TRACING_OFF) {
		hm_debug("event tracing is off\n");
		return E_HM_PERM;
	}

	event_data->tid = hm_actv_local_thread_tid_of();
	event_data->pid = hm_actv_local_thread_pid_of();
	event_data->cpuid = hm_actv_local_thread_cpu_of();
	const char *thread_name = hm_actv_local_thread_name_of();
	if (thread_name != NULL) {
		NOFAIL(memcpy_s(event_data->name, HMCAP_THREAD_NAME_SIZE,
				thread_name,
				HMCAP_THREAD_NAME_SIZE));
	}

	event_data->eid = trace_event->event_entry.eid;

	rc = hm_clock_gettime(CLOCK_MONOTONIC, &timespec);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace get timestamp failed while write event\n");
		return rc;
	}
	event_data->timestamp = (uint64_t)(unsigned long)timespec.tv_sec * USEC_PER_SEC +
				(uint64_t)(unsigned long)timespec.tv_nsec / NSEC_PER_USEC;
	return E_HM_OK;
}

static int do_write_event(struct hmtrace_event *trace_event,
			  const void *data, size_t data_size, bool wake)
{
	ssize_t rc;
	struct event_data event_data;
	struct rb_vectors_s rb_vectors[RB_EVENT_VECTORS_SIZE];

	mem_zero_s(event_data);
	rc = (ssize_t)write_event_prepare(trace_event, &event_data);
	if (rc == E_HM_OK) {
		rb_vectors[0].vector_buf = &event_data;
		rb_vectors[0].vector_len = EVENT_DATA_HDR_SIZE;
		rb_vectors[1].vector_buf = data;
		rb_vectors[1].vector_len = data_size;
		struct rb *rb = hm_trace_rb_get(trace_event->event_entry.class_entry);
		rc = hm_ring_buffer_write_vector(rb,
						 rb_vectors, RB_EVENT_VECTORS_SIZE,
						 HMTRACE_DEFAULT_TRY_TIME,
						 wake);
		hm_trace_rb_put(rb);
		if (rc > 0) {
			rc = E_HM_OK;
		}
	}
	return (int)rc;
}

int write_event(struct hmtrace_event *trace_event,
		const void *data, size_t data_size)
{
	return do_write_event(trace_event, data, data_size, false);
}

#ifdef CONFIG_HMTRACE_UNBLOCK_WRITE
int write_event_unblock(struct hmtrace_event *trace_event,
			const void *data, size_t data_size)
{
	return do_write_event(trace_event, data, data_size, true);
}
#endif

#ifndef __HOST_UT__
static void _hm_trace_rb_get(struct rb *rb)
{
	int err = raw_refcnt_nr_get(&rb->refcnt_nr);
	BUG_ON(err <= 0, "raw_refcnt_nr_get failed, err=%d\n", err);

	return;
}
#else
static void _hm_trace_rb_get(struct rb *rb)
{
	UNUSED(rb);
	return;
}
#endif

struct rb *hm_trace_rb_get(struct class_entry *entry)
{
	RAW_MUTEX_GUARD(_, &(entry->mutex));
	struct rb *rb = entry->rb;
	BUG_ON(rb == NULL, "rb is NULL\n");
	_hm_trace_rb_get(rb);
	return rb;
}

#ifndef __HOST_UT__
void hm_trace_rb_put(struct rb *rb)
{
	int cnt = raw_refcnt_nr_put(&rb->refcnt_nr);
	if (cnt == 0) {
		(void)hm_mem_munmap(rb->map_buf, rb->rb_sz);
		(void)hm_ashm_close(rb->shm_id);
		(void)hm_ashm_unlink(rb->shm_id);
		free(rb->data);
		free(rb);
	}
}
#else
void hm_trace_rb_put(struct rb *rb)
{
	UNUSED(rb);
	return;
}
#endif
