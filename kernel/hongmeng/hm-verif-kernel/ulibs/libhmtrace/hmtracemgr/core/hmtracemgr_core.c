/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide core functions for hmtracemgr sysif handler
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <hongmeng/errno.h>
#include <libhmactv/actv.h>
#include <libsysif/sysmgr/api.h>
#include <libhmtrace/hmtrace.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex_guard.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmucap/ucap.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>

#include "../../hmtrace/core/hmtrace_core.h"
#include "hmtracemgr_core.h"

int hmtracemgr_get_registered_class_list(struct bunch_ipc_attr *attr)
{
	int rc;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if (attr == NULL) {
		hm_warn("invalid bunch ipc attribute\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	rc = get_registered_class_list(attr);
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr get registered class list failed: %s\n",
			hmstrerror(rc));
	}

	return rc;
}

int hmtracemgr_get_registered_events_list(const char *class_name, struct bunch_ipc_attr *attr)
{
	int rc;
	struct hmtrace_class *trace_class = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if (attr == NULL) {
		hm_warn("invalid bunch ipc attribute\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n", class_name);
		return E_HM_INVAL;
	}

	rc = get_registered_events_list(&trace_class->class_entry, attr);
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr get class %s registered events list failed: %s\n",
			class_name, hmstrerror(rc));
	}

	return rc;
}

int hmtracemgr_get_available_events_list(const char *class_name, struct bunch_ipc_attr *attr)
{
	int rc;
	struct hmtrace_class *trace_class = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if (attr == NULL) {
		hm_warn("invalid bunch ipc attribute\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n", class_name);
		return E_HM_INVAL;
	}

	rc = get_available_events_list(trace_class, class_name, attr);
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr get class %s available events list failed: %s\n",
			class_name, hmstrerror(rc));
	}

	return rc;
}

int hmtracemgr_register_event(const char *class_name, const void *buffer,
			      unsigned int buffer_size)
{
	int rc;
	struct hmtrace_class *trace_class = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n",
			class_name);
		return E_HM_INVAL;
	}

	if (trace_class->register_event == NULL) {
		hm_warn("hmtrace trace_class %s not support dynamic register event\n",
			class_name);
		return E_HM_NOSYS;
	}

	if (buffer_size == 0) {
		return E_HM_INVAL;
	}

	rc = trace_class->register_event(trace_class, buffer, buffer_size);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace register event dynamically failed: %s\n",
			hmstrerror(rc));
	}

	return rc;
}

int hmtracemgr_unregister_event(const char *class_name, const void *buffer,
				unsigned int buffer_size)
{
	int rc;
	struct hmtrace_class *trace_class = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n",
			class_name);
		return E_HM_INVAL;
	}

	if (trace_class->unregister_event == NULL) {
		hm_warn("hmtrace trace_class %s not support dynamic unregister event\n",
			class_name);
		return E_HM_NOSYS;
	}

	if (buffer_size == 0) {
		return E_HM_INVAL;
	}

	rc = trace_class->unregister_event(trace_class, buffer, buffer_size);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace unregister event dynamically failed: %s\n",
			hmstrerror(rc));
	}

	return rc;
}

int hmtracemgr_set_event_on(const char *class_name, const char *event_name)
{
	int rc = E_HM_OK;
	struct hmtrace_class *trace_class = NULL;
	struct hmtrace_event *trace_event = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n",
			class_name);
		return E_HM_INVAL;
	}

	if (strcmp(trace_class->name, "function_trace") == 0) {
		if (*event_name == '\0') {
			*(trace_class->class_entry.rb->private.ftrace_enable) = 0;
		} else {
			rc = E_HM_INVAL;
		}
	} else {
		trace_event = trace_event_of_event_name(&trace_class->class_entry,
							event_name);
		if (trace_event == NULL) {
			hm_warn("hmtracemgr set tracing on event %s not registered\n",
				event_name);
			rc = E_HM_INVAL;
		}

		if (rc == E_HM_OK) {
			rc = set_event_tracing_on(trace_event);
			if (rc != E_HM_OK) {
				hm_warn("hmtracemgr set event %s tracing on failed: %s\n",
					event_name, hmstrerror(rc));
			}
		}
	}

	return rc;
}

int hmtracemgr_set_event_off(const char *class_name, const char *event_name)
{
	int rc = E_HM_OK;
	struct hmtrace_class *trace_class = NULL;
	struct hmtrace_event *trace_event = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n",
			class_name);
		return E_HM_INVAL;
	}

	if (strcmp(trace_class->name, "function_trace") == 0) {
		if (*event_name == '\0') {
			*(trace_class->class_entry.rb->private.ftrace_enable) = 1;
		} else {
			rc = E_HM_INVAL;
		}
	} else {
		trace_event = trace_event_of_event_name(&trace_class->class_entry,
							event_name);
		if (trace_event == NULL) {
			hm_warn("hmtracemgr set tracing off event %s not registered\n",
				event_name);
			return E_HM_INVAL;
		}

		rc = set_event_tracing_off(trace_event);
		if (rc != E_HM_OK) {
			hm_warn("hmtracemgr set event %s tracing off failed: %s\n",
				event_name, hmstrerror(rc));
		}
	}

	return rc;
}

int hmtracemgr_get_event_status(const char *class_name,
				const char *event_name, int *status)
{
	int rc;
	struct hmtrace_class *trace_class = NULL;
	struct hmtrace_event *trace_event = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtrace trace_class %s not supported\n",
			class_name);
		return E_HM_INVAL;
	}

	trace_event = trace_event_of_event_name(&trace_class->class_entry,
						event_name);
	if (trace_event == NULL) {
		hm_warn("hmtracemgr get status event %s not registered\n",
			event_name);
		return E_HM_INVAL;
	}

	rc = get_event_tracing_status(trace_event, status);
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr get event %s tracing status failed: %s\n",
			event_name, hmstrerror(rc));
	}

	return rc;
}

int hmtracemgr_get_event_eid(const char *class_name,
			     const char *event_name, int *eid)
{
	int err = E_HM_OK;
	struct hmtrace_class *trace_class = NULL;
	struct hmtrace_event *trace_event = NULL;

	if ((class_name == NULL) || (event_name == NULL) || (eid == NULL)) {
		err = E_HM_INVAL;
	}
	/* check hmtrace eid */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		err = E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	if (err == E_HM_OK) {
		trace_class = trace_class_of_class_name(class_name);
		if (trace_class == NULL) {
			hm_warn("hmtrace trace_class %s not supported\n",
				class_name);
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		trace_event = trace_event_of_event_name(&trace_class->class_entry,
							event_name);
		if (trace_event == NULL) {
			hm_warn("hmtracemgr get eid event %s not registered\n",
				event_name);
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		*eid = trace_event->event_entry.eid;
	}
	return err;
}

int hmtracemgr_get_rb_info(const char *class_name, struct rb *rb)
{
	int rc;
	uint64_t key;
	struct hmtrace_class *trace_class = NULL;

	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("hmtracemgr get shm id class %s not registered\n",
			class_name);
		return E_HM_INVAL;
	}

	if (trace_class->prepare_for_show != NULL) {
		rc = trace_class->prepare_for_show(trace_class);
		if (rc != E_HM_OK) {
			hm_warn("hmtrace class %s prepare for show failed\n",
				class_name);
			return rc;
		}
	}

	rb->blk_num = trace_class->class_entry.rb->blk_num;
	rb->rb_sz = trace_class->class_entry.rb->rb_sz;

	hm_debug("server try to grant shm to caller\n");
	rc = hm_ashm_grant_to_caller(trace_class->class_entry.rb->shm_id, SHM_GRANT_RDWR, &key);
	if (rc < 0) {
		hm_warn("grant shm to caller failed: %s\n", hmstrerror(rc));
		return rc;
	}
	rb->key = key;
	rb->cnode_idx = hm_ucap_self_cnode_idx();

	return E_HM_OK;
}

static int check_hmtrace_inited(void)
{
	int err = E_HM_OK;

	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		err = E_HM_NOOBJ;
	}
	return err;
}

/* The func is guaranteed by the hmtrace.mutex */
int hmtracemgr_trace_buffer_size(const char *class_name, size_t *buffer_size)
{
	int err;
	struct hmtrace_class *trace_class = NULL;

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	err = check_hmtrace_inited();
	if (err == E_HM_OK) {
		trace_class = trace_class_of_class_name(class_name);
		if (trace_class == NULL) {
			hm_warn("hmtrace trace_class %s not supported\n", class_name);
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		struct class_entry *class_entry = &trace_class->class_entry;
		BUG_ON(class_entry == NULL, "class_entry of %s is NULL\n", class_name);
		BUG_ON(class_entry->rb == NULL, "rb of class_entry of %s is NULL\n", class_name);
		*buffer_size = class_entry->rb->rb_sz;
	}
	return err;
}

static int realloc_trace_buffer(struct class_entry *class_entry, size_t size)
{
	int err = E_HM_OK;
	struct rb *rb = NULL;
	struct rb *old_rb = NULL;

	old_rb = hm_trace_rb_get(class_entry);
	if (old_rb->rb_sz != size) {
		rb = hm_ring_buffer_create(size, RB_OVERWRITE_MODE);
		if (rb == NULL) {
			err = E_HM_POSIX_FAULT;
		} else {
			raw_refcnt_nr_init(&rb->refcnt_nr, 1);
			raw_mutex_lock(&(class_entry->mutex));
			class_entry->rb = rb;
			raw_mutex_unlock(&(class_entry->mutex));
			/*
			 * The first put corresponds to get in the function.
			 * The second put indicates that the rb needs to be released.
			 */
			hm_trace_rb_put(old_rb);
			hm_trace_rb_put(old_rb);
		}
	}
	return err;
}

#define MAX_BUFFER_SIZE 1073741824U
#define MIN_BUFFER_SIZE 4096U
static int check_buffer_size(const struct hmtrace_class *trace_class, size_t size)
{
	int err = E_HM_OK;
	size_t freemem = 0UL;

	if (__builtin_popcountl((unsigned long)size) != 1) {
		hm_warn("invalid size %zu, size must be a power of 2\n", size);
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if ((size < MIN_BUFFER_SIZE) || (size > MAX_BUFFER_SIZE)) {
			hm_warn("invalid size %zu, range is [%u, %u]\n", size,
				MIN_BUFFER_SIZE, MAX_BUFFER_SIZE);
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		if (trace_class->freeram != NULL) {
			freemem = trace_class->freeram();
		} else {
#ifndef __HOST_UT__
			struct sysinfo info;
			err = lsyscall_sysctrl_sysinfo(&info);
			if (err == E_HM_OK) {
				freemem = (size_t)info.freeram;
			} else {
				hm_warn("sysinfo failed, err=%d\n", err);
			}
#else
			freemem = MIN_BUFFER_SIZE;
#endif
		}
	}

	if ((err == E_HM_OK) && (size > freemem)) {
		hm_warn("must less than freemem, size is %zu, freemem is %zu\n", size, freemem);
		err = E_HM_NOMEM;
	}
	return err;
}

/* The func is guaranteed by the hmtrace.mutex */
int hmtracemgr_realloc_trace_buffer(const char *class_name, size_t size)
{
	int err;
	struct hmtrace_class *trace_class = NULL;

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	err = check_hmtrace_inited();
	if (err == E_HM_OK) {
		trace_class = trace_class_of_class_name(class_name);
		if (trace_class == NULL) {
			hm_warn("hmtrace trace_class %s not supported\n", class_name);
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		err = check_buffer_size(trace_class, size);
	}

	if (err == E_HM_OK) {
		err = realloc_trace_buffer(&trace_class->class_entry, size);
		if (err != E_HM_OK) {
			hm_warn("hmtrace realloc trace buffer failed %s,trace class is %s\n",
				hmstrerror(err), trace_class->name);
		}
	}
	return err;
}
