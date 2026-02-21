/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide API about trace_event in hmtrace framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <string.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <libhmtrace/hmtrace.h>
#include <libhmtrace/hmtrace_iter.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmtrace/uev_trace/uev_trace_template.h>

#include "core/hmtrace_core.h"
#include "../hmtracemgr/core/hmtracemgr_core.h"

#ifdef CONFIG_HMTRACE
void hm_trace_init(pid_t pid)
{
	int rc;

	if (pid < 0) {
		hm_warn("hmtrace init with invalid pid %d\n", pid);
		return;
	}

	/* hmtrace only needs to be initialized once */
	raw_mutex_init_recursive(&(hmtrace.mutex));
	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	if (hmtrace.initialized) {
		return;
	}

	rc = hmtracemgr_init(pid);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace init hmtracemgr failed: %s\n", hmstrerror(rc));
		return;
	}

	dlist_init(&(hmtrace.class_list));
	hmtrace.initialized = true;
	register_match_operations_fn(match_hmtrace_ipc_iter_operations);
	hm_info("pid=%d hmtrace init succeed\n", pid);
}

void hm_trace_init_for_premain(void)
{
	hm_trace_init(hm_getpid());
}

void event_entry_init(struct event_entry *event_entry)
{
	if (event_entry == NULL) {
		return;
	}

	event_entry->eid = -1;
	event_entry->class_entry = NULL;
	event_entry->status = TRACING_OFF;
	dlist_init(&event_entry->dlist);
}

int hm_trace_register_event(struct hmtrace_class *trace_class,
			    struct hmtrace_event *trace_event)
{
	int rc;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if ((trace_event == NULL) || (trace_class == NULL)) {
		hm_warn("hmtrace trace_event or trace_class is NULL\n");
		return E_HM_INVAL;
	}

	if (strlen(trace_event->name) == 0) {
		hm_warn("hmtrace register trace_event must have a name\n");
		return E_HM_INVAL;
	}

	if (strchr(trace_event->name, ' ') != NULL) {
		hm_warn("hmtrace trace_event name cannot contain space char\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	rc = register_event(trace_class, trace_event);
	if (rc != E_HM_OK) {
		hm_debug("hmtrace register trace_event %s failed: %s\n",
			trace_event->name, hmstrerror(rc));
	}

	return rc;
}

int hm_trace_unregister_event(struct hmtrace_event *trace_event)
{
	int rc;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if ((trace_event == NULL) || strlen(trace_event->name) == 0)  {
		hm_warn("hmtrace trace_event or event_name invalid\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	rc = unregister_event(trace_event);
	if (rc != E_HM_OK) {
		hm_debug("hmtrace unregister trace_event %s failed: %s\n",
			trace_event->name, hmstrerror(rc));
	}

	return rc;
}

struct hmtrace_event *hm_trace_event_of_event_name(const struct hmtrace_class *trace_class,
						   const char *event_name)
{
	struct hmtrace_event *trace_event = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return NULL;
	}

	if (trace_class == NULL) {
		hm_warn("hmtrace acquire event struct with invalid trace_class\n");
		return NULL;
	}

	if (event_name == NULL || strlen(event_name) == 0) {
		hm_warn("hmtrace acquire event struct with invalid event_name\n");
		return NULL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	if (!is_class_registered(&trace_class->class_entry)) {
		hm_warn("hmtrace class %s not registered\n", trace_class->name);
		return NULL;
	}

	trace_event = trace_event_of_event_name(&trace_class->class_entry,
						event_name);
	if (trace_event == NULL) {
		hm_warn("hmtrace event %s not registered\n", event_name);
	}

	return trace_event;
}

int hm_trace_set_event_tracing_on(struct hmtrace_event *trace_event)
{
	int err;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if (trace_event == NULL || strlen(trace_event->name) == 0)  {
		hm_warn("hmtrace trace_event or event_name invalid\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	err = set_event_tracing_on(trace_event);
	if (err != E_HM_OK) {
		hm_warn("hmtrace set event %s tracing on failed: %s\n",
			trace_event->name, hmstrerror(err));
	}

	return err;
}

int hm_trace_set_event_tracing_off(struct hmtrace_event *trace_event)
{
	int err;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if (trace_event == NULL || strlen(trace_event->name) == 0)  {
		hm_warn("hmtrace trace_event or event_name invalid\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	err = set_event_tracing_off(trace_event);
	if (err != E_HM_OK) {
		hm_warn("hmtrace set event %s tracing off failed: %s\n",
			trace_event->name, hmstrerror(err));
	}

	return err;
}

int hm_trace_get_event_tracing_status(struct hmtrace_event *trace_event,
				      int *status)
{
	int err;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if (trace_event == NULL || strlen(trace_event->name) == 0)  {
		hm_warn("hmtrace trace_event or event_name invalid\n");
		return E_HM_INVAL;
	}

	if (status == NULL) {
		hm_warn("hmtrace get event status with invalid status\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	err = get_event_tracing_status(trace_event, status);
	if (err != E_HM_OK) {
		hm_warn("hmtrace get event %s tracing status failed: %s\n",
			trace_event->name, hmstrerror(err));
	}

	return err;
}

typedef int (*register_event_fn)(struct hmtrace_class *trace_class,
				 const void *buffer, unsigned int buffer_size);
static void __tracing_register_events(struct hmtrace_class *trace_class, register_event_fn register_fn)
{
	int err;
	struct uev_trace_event *uev = NULL;
	struct hmtrace_event *event = NULL;
	const struct uev_trace_event_table *table;
	unsigned int buffer_size;
	size_t idx;

	for_each_uev_event_table(trace_class, table) {
		for_each_uev_event(table, uev, idx) {
			event = &uev->evt;
			buffer_size = (unsigned int)strlen(event->name) + 1;
			err = register_fn(trace_class, (const void*)event->name, buffer_size);
			if ((err != E_HM_OK) && (err != E_HM_EXIST)) {
				hm_warn("hmtrace register event :%s failed\n", event->name);
				break;
			}
		}
	}
}

static int __tracing_on_events(const struct class_entry *class_entry,
			       enum hmtrace_tracing_status status)
{
	int err = E_HM_OK;
	struct hmtrace_class *trace_class = NULL;
	struct event_entry *event_entry = NULL;
	struct hmtrace_event *event;

	trace_class = container_of(class_entry, struct hmtrace_class, class_entry);
	/* function_trace doesn't have any trace event */
	if (strcmp(trace_class->name, "function_trace") == 0) {
		*(trace_class->class_entry.rb->private.ftrace_enable) = (status == TRACING_OFF) ? 1 : 0;
		return err;
	}

	if (status == TRACING_ON) {
		__tracing_register_events(trace_class, trace_class->register_event);
	}

	dlist_for_each_entry(event_entry, &class_entry->events_list,
			     struct event_entry, dlist) {
		event = container_of(event_entry, struct hmtrace_event, event_entry);

		if (trace_class->enable_event != NULL) {
			if (status == TRACING_ON) {
				err = trace_class->enable_event(event);
			} else if (status == TRACING_OFF) {
				err = trace_class->disable_event(event);
			} else {
				/* do nothing */
			}
			if (err != E_HM_OK) {
				hm_warn("hmtrace enable event :%s failed\n", event->name);
				break;
			}
		}
		hm_debug("name:%s, status:%u\n", event->name, (unsigned int)status);
		event_entry->status = status;
	}

	if (status == TRACING_OFF) {
		__tracing_register_events(trace_class, trace_class->unregister_event);
	}
	return err;
}

int hm_trace_tracing_on(enum hmtrace_tracing_status status)
{
	int err = E_HM_OK;
	struct class_entry *class_entry = NULL;

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		dlist_for_each_entry(class_entry, &(hmtrace.class_list),
				     struct class_entry, dlist) {
			err = __tracing_on_events(class_entry, status);
			if (err != E_HM_OK) {
				break;
			}
		}
	}

	return err;
}

#else /* CONFIG_HMTRACE */

void hm_trace_init(pid_t pid)
{
	UNUSED(pid);
}

void event_entry_init(struct event_entry *event_entry)
{
	UNUSED(event_entry);
}

int hm_trace_register_event(struct hmtrace_class *trace_class,
			    struct hmtrace_event *trace_event)
{
	UNUSED(trace_class, trace_event);
	return E_HM_NOSYS;
}

int hm_trace_unregister_event(struct hmtrace_event *trace_event)
{
	UNUSED(trace_event);
	return E_HM_NOSYS;
}

struct hmtrace_event *hm_trace_event_of_event_name(const struct hmtrace_class *trace_class,
						   const char *event_name)
{
	UNUSED(trace_class, event_name);
	return NULL;
}

int hm_trace_set_event_tracing_on(struct hmtrace_event *trace_event)
{
	UNUSED(trace_event);
	return E_HM_NOSYS;
}

int hm_trace_set_event_tracing_off(struct hmtrace_event *trace_event)
{
	UNUSED(trace_event);
	return E_HM_NOSYS;
}

int hm_trace_get_event_tracing_status(struct hmtrace_event *trace_event,
				      int *status)
{
	UNUSED(trace_event, status);
	return E_HM_NOSYS;
}

int hm_trace_tracing_on(enum hmtrace_tracing_status status)
{
	UNUSED(status);
	return E_HM_NOSYS;
}
#endif
