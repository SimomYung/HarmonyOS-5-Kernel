/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide API about trace_class in hmtrace framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <string.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <libhmtrace/hmtrace.h>
#include <libhmsync/raw_mutex_guard.h>

#include "core/hmtrace_core.h"

struct hmtrace_ctrl hmtrace = {
	.initialized = false,
	.mutex = RAW_MUTEX_INITIALIZER,
	.class_num = 0,
	.class_list = DLIST_HEAD_INIT(hmtrace.class_list),
};

#ifdef CONFIG_HMTRACE
void class_entry_init(struct class_entry *class_entry)
{
	if (class_entry == NULL) {
		return;
	}

	dlist_init(&class_entry->dlist);
	dlist_init(&class_entry->events_list);
	class_entry->rb = NULL;
	class_entry->event_num = 0;
}

int hm_trace_register_class(struct hmtrace_class *trace_class)
{
	int rc;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if ((trace_class == NULL) || strlen(trace_class->name) == 0) {
		hm_warn("hmtrace register trace class invalid\n");
		return E_HM_INVAL;
	}

	if (strchr(trace_class->name, ' ') != NULL) {
		hm_warn("hmtrace class name cannot contain space character\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	rc = register_class(trace_class);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace register trace class %s failed: %s\n",
			trace_class->name, hmstrerror(rc));
	}

	return rc;
}

int hm_trace_unregister_class(struct hmtrace_class *trace_class)
{
	int rc;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if ((trace_class == NULL) || strlen(trace_class->name) == 0) {
		hm_warn("hmtrace unregister trace class is NULL\n");
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	rc = unregister_class(trace_class);
	if (rc != E_HM_OK) {
		hm_warn("hmtrace unregister trace class %s failed: %s\n",
			trace_class->name, hmstrerror(rc));
	}

	return rc;
}

struct hmtrace_class *hm_trace_class_of_class_name(const char *class_name)
{
	struct hmtrace_class *trace_class = NULL;

	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return NULL;
	}

	if (class_name == NULL || strlen(class_name) == 0) {
		hm_warn("hmtrace acquire class struct with invalid class_name\n");
		return NULL;
	}

	RAW_MUTEX_GUARD(_, &(hmtrace.mutex));
	trace_class = trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		hm_warn("trace class %s not registered\n", class_name);
	}

	return trace_class;
}
#else
void class_entry_init(struct class_entry *class_entry)
{
	UNUSED(class_entry);
}

int hm_trace_register_class(struct hmtrace_class *trace_class)
{
	UNUSED(trace_class);
	return E_HM_NOSYS;
}

int hm_trace_unregister_class(struct hmtrace_class *trace_class)
{
	UNUSED(trace_class);
	return E_HM_NOSYS;
}

struct hmtrace_class *hm_trace_class_of_class_name(const char* class_name)
{
	UNUSED(class_name);
	return NULL;
}
#endif
