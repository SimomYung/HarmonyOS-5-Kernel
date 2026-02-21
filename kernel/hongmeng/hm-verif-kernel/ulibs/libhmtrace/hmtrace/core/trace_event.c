/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide operations about trace_event in hmtrace framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <string.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmtrace/hmtrace.h>
#include <libsysif/sysmgr/api.h>

#include "hmtrace_core.h"

struct hmtrace_event *trace_event_of_event_name(const struct class_entry *class_entry,
						const char *event_name)
{
	struct event_entry *event_entry = NULL;
	struct hmtrace_event *trace_event = NULL;

	dlist_for_each_entry(event_entry, &class_entry->events_list,
			     struct event_entry, dlist) {
		trace_event = container_of(event_entry, struct hmtrace_event,
					   event_entry);
		if (strncmp(event_name, trace_event->name,
			    HMTRACE_MAX_EVENT_NAME_SIZE_RESTRICTED) == 0) {
			return trace_event;
		}
	}

	return NULL;
}

bool is_event_registered(const struct event_entry *event_entry)
{
	return !dlist_empty(&event_entry->dlist);
}

int register_event(struct hmtrace_class *trace_class,
		   struct hmtrace_event *trace_event)
{
	int err;
	struct class_entry *class_entry = NULL;
	struct event_entry *event_entry = NULL;
	struct hmtrace_event *other_trace_event = NULL;
	struct __actvret_hmcall_hmtrace_alloc_eid ret;

	mem_zero_s(ret);
	/* check if trace_class in the class_list */
	class_entry = &trace_class->class_entry;
	if (!is_class_registered(class_entry)) {
		hm_warn("hmtrace trace_class %s belongs not supported\n",
			trace_class->name);
		return E_HM_INVAL;
	}

	/* event can only be registered once */
	event_entry = &trace_event->event_entry;
	if (is_event_registered(event_entry)) {
		hm_warn("hmtrace event %s already registered\n", trace_event->name);
		return E_HM_OBJEXIST;
	}

	/* the same name event is not allowed */
	other_trace_event = trace_event_of_event_name(class_entry, trace_event->name);
	if (other_trace_event != NULL) {
		hm_debug("hmtrace class %s has registered the same name event %s\n",
			trace_class->name, other_trace_event->name);
		return E_HM_POSIX_FAULT;
	}

	err = actvcall_hmcall_hmtrace_alloc_eid(&ret);
	if (err != E_HM_OK) {
		hm_warn("hmtrace alloc eid for event %s failed\n",
			trace_event->name);
		return err;
	}
	event_entry->eid = ret.eid;

	/* add to events list */
	event_entry->class_entry = class_entry;
	dlist_insert_tail(&class_entry->events_list, &event_entry->dlist);
	class_entry->event_num++;
	return E_HM_OK;
}

int unregister_event(struct hmtrace_event *trace_event)
{
	struct event_entry *event_entry = NULL;

	event_entry = &trace_event->event_entry;
	if (!is_event_registered(event_entry)) {
		hm_debug("event %s not registered yet\n", trace_event->name);
		return E_HM_NOOBJ;
	}

	/* if event is tracing_on, return error */
	if (event_entry->status == TRACING_ON) {
		hm_warn("hmtrace event %s is tracing on\n", trace_event->name);
		return E_HM_BUSY;
	}

	/* delete from events list */
	(void)actvcall_hmcall_hmtrace_free_eid(event_entry->eid);
	event_entry->class_entry->event_num--;
	dlist_delete(&event_entry->dlist);
	return E_HM_OK;
}

int set_event_tracing_on(struct hmtrace_event *trace_event)
{
	int err;
	struct hmtrace_class *trace_class = NULL;
	struct event_entry *event_entry = NULL;

	event_entry = &trace_event->event_entry;
	if (!is_event_registered(event_entry)) {
		hm_warn("event %s not registered yet\n", trace_event->name);
		return E_HM_INVAL;
	}

	trace_class = container_of(event_entry->class_entry,
				   struct hmtrace_class,
				   class_entry);
	if (event_entry->status == TRACING_ON &&
	    strncmp(trace_class->name, KEV_TRACE, strlen(KEV_TRACE)) != 0) {
		return E_HM_OK;
	}

	if (trace_class->enable_event != NULL) {
		err = trace_class->enable_event(trace_event);
		if (err != E_HM_OK) {
			hm_warn("hmtrace enable event %s failed\n",
				trace_event->name);
			return err;
		}
	}
	event_entry->status = TRACING_ON;

	return E_HM_OK;
}

int set_event_tracing_off(struct hmtrace_event *trace_event)
{
	int err;
	struct hmtrace_class *trace_class = NULL;
	struct event_entry *event_entry = NULL;

	event_entry = &trace_event->event_entry;
	if (!is_event_registered(event_entry)) {
		hm_warn("event %s not registered yet\n", trace_event->name);
		return E_HM_INVAL;
	}

	trace_class = container_of(event_entry->class_entry,
				   struct hmtrace_class,
				   class_entry);
	if (event_entry->status == TRACING_OFF &&
	    strncmp(trace_class->name, KEV_TRACE, strlen(KEV_TRACE)) != 0) {
		return E_HM_OK;
	}

	if (trace_class->disable_event != NULL) {
		err = trace_class->disable_event(trace_event);
		if (err != E_HM_OK) {
			hm_warn("hmtrace disable event %s failed\n",
				trace_event->name);
			return err;
		}
	}
	event_entry->status = TRACING_OFF;

	return E_HM_OK;
}

int get_event_tracing_status(struct hmtrace_event *trace_event, int *status)
{
	struct event_entry *event_entry = NULL;

	event_entry = &trace_event->event_entry;
	if (!is_event_registered(event_entry)) {
		hm_warn("event %s not registered yet\n", trace_event->name);
		return E_HM_INVAL;
	}

	*status = (int)(trace_event->event_entry.status);
	return E_HM_OK;
}
