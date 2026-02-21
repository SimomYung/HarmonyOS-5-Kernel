/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide operations about trace_class in hmtrace framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <hongmeng/errno.h>
#include <libhmactv/actv.h>
#include <libhmtrace/hmtrace.h>
#include <libhwsecurec/securec.h>

#include "hmtrace_core.h"

struct hmtrace_class *trace_class_of_class_name(const char* class_name)
{
	struct class_entry *class_entry = NULL;
	struct hmtrace_class *trace_class = NULL;

	dlist_for_each_entry(class_entry, &(hmtrace.class_list),
			     struct class_entry, dlist) {
		trace_class = container_of(class_entry, struct hmtrace_class,
					   class_entry);
		if (strncmp(class_name, trace_class->name,
		    HMTRACE_MAX_CLASS_NAME_SIZE) == 0) {
			return trace_class;
		}
	}

	return NULL;
}

bool is_class_registered(const struct class_entry *class_entry)
{
	return !dlist_empty(&class_entry->dlist);
}

static int do_register_class(struct hmtrace_class *trace_class)
{
	int err = E_HM_OK;
	struct class_entry *class_entry = NULL;
	class_entry = &trace_class->class_entry;

	class_entry->rb = hm_ring_buffer_create(HMTRACE_DEFAULT_LOCAL_BUFFER_SIZE, RB_OVERWRITE_MODE);
	if (class_entry->rb == NULL) {
		hm_warn("hmtrace create ring buffer for %s failed\n",
			trace_class->name);
		err = E_HM_POSIX_FAULT;
	}

	if (err == E_HM_OK) {
		/* add to class_list */
		dlist_insert_tail(&(hmtrace.class_list), &class_entry->dlist);
		hmtrace.class_num++;
		raw_refcnt_nr_init(&class_entry->rb->refcnt_nr, 1);
	}

	return err;
}

static int do_register_uev_event_table(struct hmtrace_class *registered_class,
		struct hmtrace_class *new_class)
{
	int err = E_HM_OK;

	struct uev_trace_event_table *table = new_class->event_table.uev_table;
	if (table != NULL) {
		BUG_ON(registered_class->event_table.uev_table == NULL);
		registered_class->event_table.uev_table->next_table = table;
	}

	return err;
}

static bool is_uev_trace_class(struct hmtrace_class *trace_class)
{
	return (strcmp(trace_class->name, UEV_TRACE) == 0);
}

static bool should_register_on_event_table(struct hmtrace_class *registered_class,
		const struct hmtrace_class *new_class)
{
	bool ret = false;
	/* register a new event table on uev_trace */
	if (registered_class != NULL && registered_class != new_class
			&& is_uev_trace_class(registered_class) &&
			new_class->event_table.uev_table != NULL &&
			(new_class->register_event == NULL ||
			 new_class->register_event == registered_class->register_event)) {
		ret = true;
	}

	return ret;
}

int register_class(struct hmtrace_class *trace_class)
{
	int err = E_HM_OK;
	struct class_entry *class_entry = NULL;
	struct hmtrace_class *registered_class = NULL;

	/* look up class_list, check if class has already been registered */
	class_entry = &trace_class->class_entry;
	if (is_class_registered(class_entry)) {
		hm_warn("hmtrace class %s already registered\n",
			trace_class->name);
		err = E_HM_OBJEXIST;
	}

	if (err == E_HM_OK) {
		/* all trace_class can only have one unique name */
		registered_class = trace_class_of_class_name(trace_class->name);
		if (registered_class != NULL &&
				!should_register_on_event_table(registered_class, trace_class)) {
			hm_warn("hmtrace already registered class same name %s\n",
				registered_class->name);
			err = E_HM_OBJEXIST;
		}
	}

	if (err == E_HM_OK) {
		if (registered_class != NULL) {
			err = do_register_uev_event_table(registered_class, trace_class);
		} else {
			err = do_register_class(trace_class);
		}
	}

	return err;
}

static void do_unregister_uev_event_table(struct hmtrace_class *main_class,
		struct hmtrace_class *trace_class)
{
	struct uev_trace_event_table *table_to_remove = trace_class->event_table.uev_table;
	struct uev_trace_event_table *table = main_class->event_table.uev_table;

	while (table != NULL) {
		struct uev_trace_event_table *next_table = table->next_table;
		if (table_to_remove == next_table) {
			table->next_table = table_to_remove->next_table;
			break;
		}
		table = next_table;
	}
}

static int do_unregister_class(struct hmtrace_class *registered_class,
		struct hmtrace_class *trace_class)
{
	int err = E_HM_OK;
	struct class_entry *class_entry = NULL;
	class_entry = &trace_class->class_entry;
	if (registered_class == trace_class) {
		hmtrace.class_num--;
	} else {
		if (registered_class != NULL) {
			do_unregister_uev_event_table(registered_class, trace_class);
		}
	}

	/* delete from class_list */
	dlist_delete(&class_entry->dlist);

	if (class_entry->rb != NULL) {
		hm_ring_buffer_destroy(class_entry->rb);
		class_entry->rb = NULL;
	}

	return err;
}

int unregister_class(struct hmtrace_class *trace_class)
{
	int err = E_HM_OK;
	struct hmtrace_class *registered_class = NULL;
	struct class_entry *class_entry = NULL;

	class_entry = &trace_class->class_entry;
	registered_class = trace_class_of_class_name(trace_class->name);
	if (!is_class_registered(class_entry)
			&& !should_register_on_event_table(registered_class, trace_class)) {
		hm_warn("hmtrace class %s not registered yet\n", trace_class->name);
		err = E_HM_INVAL;
	}
	/* check if events_list in trace_class is empty */
	if (err == E_HM_OK && class_entry->event_num != 0) {
		hm_warn("hmtrace class %s events_list is not empty\n",
			trace_class->name);
		err = E_HM_BUSY;
	}
	if (err == E_HM_OK) {
		(void)do_unregister_class(registered_class, trace_class);
	}

	return err;
}

static int create_registered_class_list(struct bunch *bunch)
{
	struct class_entry *class_entry = NULL;
	struct hmtrace_class *trace_class = NULL;
	int32_t total_size = 0;
	int32_t total_len = 0;

	dlist_for_each_entry(class_entry, &(hmtrace.class_list),
			     struct class_entry, dlist) {
		trace_class = container_of(class_entry, struct hmtrace_class,
					   class_entry);
		total_size++;
		total_len += (int32_t)strlen(trace_class->name) + 1;
	}
	bunch_write_int32(bunch, total_size);
	bunch_write_int32(bunch, total_len);

	dlist_for_each_entry(class_entry, &(hmtrace.class_list),
			     struct class_entry, dlist) {
		trace_class = container_of(class_entry, struct hmtrace_class,
					   class_entry);
		bunch_write_string(bunch, trace_class->name);
	}

	return E_HM_OK;
}

int get_registered_class_list(struct bunch_ipc_attr *attr)
{
	int rc = E_HM_OK;
	struct bunch *bunch = NULL;

	bunch = bunch_ipc_open(attr);
	if (bunch == NULL) {
		rc = E_HM_POSIX_FAULT;
		hm_warn("bunch ipc bunch open attr failed\n");
	}

	if (rc == E_HM_OK) {
		rc = create_registered_class_list(bunch);
		if (rc == E_HM_OK && bunch->status == BUNCH_STATUS_WOF) {
			/* handle write overflow */
			rc = E_HM_NOMEM;
		}
		bunch_ipc_close(bunch);
	}

	return rc;
}

static int create_registered_events_list(struct bunch *bunch,
					 const struct class_entry *class_entry)
{
	struct event_entry *event_entry = NULL;
	struct hmtrace_event *trace_event = NULL;
	int32_t total_size = 0;
	int32_t total_len = 0;

	dlist_for_each_entry(event_entry, &class_entry->events_list,
			     struct event_entry, dlist) {
		trace_event = container_of(event_entry, struct hmtrace_event,
					   event_entry);
		total_size++;
		total_len += (int32_t)strlen(trace_event->name) + 1;
	}
	bunch_write_int32(bunch, total_size);
	bunch_write_int32(bunch, total_len);

	dlist_for_each_entry(event_entry, &class_entry->events_list,
			     struct event_entry, dlist) {
		trace_event = container_of(event_entry, struct hmtrace_event,
					   event_entry);
		bunch_write_string(bunch, trace_event->name);
	}

	return E_HM_OK;
}

int get_registered_events_list(const struct class_entry *class_entry,
			       struct bunch_ipc_attr *attr)
{
	int rc = E_HM_OK;
	struct bunch *bunch = bunch_ipc_open(attr);

	if (bunch == NULL) {
		rc = E_HM_POSIX_FAULT;
		hm_warn("bunch ipc bunch open attr failed\n");
	}

	if (rc == E_HM_OK) {
		rc = create_registered_events_list(bunch, class_entry);
		if (rc == E_HM_OK && bunch->status == BUNCH_STATUS_WOF) {
			rc = E_HM_NOMEM;
		}
		bunch_ipc_close(bunch);
	}

	return rc;
}

int get_available_events_list(struct hmtrace_class *trace_class,
			      const char *class_name,
			      struct bunch_ipc_attr *attr)
{
	int rc = E_HM_OK;
	struct bunch *bunch = NULL;

	BUG_ON(trace_class == NULL);
	if (trace_class->get_available_events_list == NULL) {
		hm_warn("hmtrace trace_class %s not support get available_events_list\n",
			class_name);
		rc = E_HM_NOSYS;
	}

	if (rc == E_HM_OK) {
		bunch = bunch_ipc_open(attr);
		if (bunch == NULL) {
			rc = E_HM_POSIX_FAULT;
			hm_warn("bunch ipc bunch open attr failed\n");
		}
	}

	if (rc == E_HM_OK) {
		rc = trace_class->get_available_events_list(bunch, (void *)trace_class);
		if (rc == E_HM_OK && bunch->status == BUNCH_STATUS_WOF) {
			rc = E_HM_NOMEM;
		}
		/* reach here means bunch open successfully, close it */
		bunch_ipc_close(bunch);
	}

	return rc;
}
