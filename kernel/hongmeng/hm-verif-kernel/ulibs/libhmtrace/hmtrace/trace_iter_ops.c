/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide each hmtrace iterator operations
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 12 17:36:15 2020
 */

#include "core/trace_iter.h"

#include <string.h>
#include <sys/types.h>
#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmtrace/efmt.h>
#include <libhmtrace/hmtrace.h>
#include <libhwsecurec/securec.h>
#include <libhmtrace/hmtrace_iter.h>

#ifdef CONFIG_HMTRACE
static void *efmt_iter_begin(const char *param)
{
	char *class_name = NULL;
	struct hmtrace_class *trace_class = NULL;
	struct class_entry *class_entry = NULL;
	struct event_entry *event_entry = NULL;

	class_name = strchr(param, ' ');
	if (class_name == NULL) {
		return NULL;
	}
	class_name++;

	trace_class = hm_trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		return NULL;
	}
	class_entry = &trace_class->class_entry;

	if (dlist_empty(&class_entry->events_list)) {
		return NULL;
	}

	event_entry = dlist_first_entry(&class_entry->events_list,
					struct event_entry, dlist);
	return (void *)container_of(event_entry, struct hmtrace_event,
				    event_entry);
}

static void *efmt_iter_next(const char *param, void *pos)
{
	char *class_name = NULL;
	struct hmtrace_class *trace_class = NULL;
	struct dlist_node *events_list_head = NULL;
	struct event_entry *curr_event_entry = NULL;
	struct event_entry *next_event_entry = NULL;

	if (param == NULL || pos == NULL) {
		return NULL;
	}

	class_name = strchr(param, ' ');
	if (class_name == NULL) {
		return NULL;
	}
	class_name++;

	trace_class = hm_trace_class_of_class_name(class_name);
	if (trace_class == NULL) {
		return NULL;
	}
	events_list_head = &trace_class->class_entry.events_list;

	curr_event_entry = &((struct hmtrace_event *)pos)->event_entry;

	next_event_entry = dlist_next_entry(curr_event_entry,
					    struct event_entry, dlist);

	if (&next_event_entry->dlist != events_list_head) {
		return (void *)container_of(next_event_entry,
					    struct hmtrace_event, event_entry);
	}

	return NULL;
}

static void efmt_iter_end(const char *param)
{
	UNUSED(param);
}

static int efmt_iter_encode(void *buffer, size_t buf_size, void *pos)
{
	int err;
	int eid;
	void *efmt_buf = NULL;
	unsigned int efmt_size = 0;
	size_t data_size;
	struct hmtrace_event *trace_event = NULL;

	/* judge buffer and pos only, and buf_size maybe zero */
	if (buffer == NULL || pos == NULL) {
		return E_HM_INVAL;
	}

	trace_event = (struct hmtrace_event *)pos;
	err = marshal_event_format(trace_event->efmt, (uintptr_t *)&efmt_buf, &efmt_size);
	if (err != E_HM_OK) {
		return err;
	}

	/* return E_HM_OK while buf_size is not enough, and it will be called later */
	data_size = sizeof(eid) + efmt_size;
	if (buf_size < data_size) {
		err = E_HM_OK;
		goto err_out;
	}

	/* write eid first */
	eid = trace_event->event_entry.eid;
	err = memcpy_s(buffer, buf_size, &eid, sizeof(eid));
	if (err != E_HM_OK) {
		err = E_HM_POSIX_FAULT;
		goto err_out;
	}

	/* write marshaled efmt secondly */
	err = memcpy_s((void *)((uintptr_t)buffer + sizeof(eid)), buf_size - sizeof(eid),
		       efmt_buf, efmt_size);
	if (err != E_HM_OK) {
		err = E_HM_POSIX_FAULT;
		goto err_out;
	}

	err = (int)data_size;
err_out:
	free(efmt_buf);
	return err;
}

static int efmt_iter_decode(void *buffer, size_t buf_size, void **out)
{
	int eid;
	unsigned int efmt_size = 0;
	struct event_format *efmt = NULL;
	struct efmt_iter_data *efmt_iter_data = NULL;

	if (buffer == NULL || buf_size == 0 || out == NULL) {
		return E_HM_INVAL;
	}

	/* acquire eid first */
	eid = *(int *)buffer;

	/* unmarshal the marshaled efmt in ipc buf */
	efmt = unmarshal_event_format((uintptr_t)buffer + (uint32_t)sizeof(eid),
				      &efmt_size);
	if (efmt == NULL) {
		return E_HM_POSIX_FAULT;
	}

	efmt_iter_data = (struct efmt_iter_data *)malloc(sizeof(struct efmt_iter_data));
	if (efmt_iter_data == NULL) {
		free_event_format(efmt);
		return E_HM_NOMEM;
	}
	efmt_iter_data->eid = eid;
	efmt_iter_data->efmt = efmt;
	*out = efmt_iter_data;

	return (int)(unsigned int)sizeof(eid) + (int)efmt_size;
}

static struct iter_operations efmt_iter = {
	.name = "efmt",
	.begin = efmt_iter_begin,
	.next = efmt_iter_next,
	.end = efmt_iter_end,
	.encode_write = efmt_iter_encode,
	.decode_read = efmt_iter_decode,
};

static struct iter_operations *hmtrace_ipc_iter_array[] = {
	&efmt_iter,
};

const struct iter_operations *match_hmtrace_ipc_iter_operations(const char *name)
{
	unsigned int i;
	char *ret = NULL;
	char *iter_name = NULL;

	if (name == NULL) {
		return NULL;
	}

	/* the format of name should be "iter_name class_name"
	 * iter_name is necessary and used to match the iter_operation
	 * class_name is unnecessary and used as parameter for iterator */
	iter_name = strdup(name);
	if (iter_name == NULL) {
		return NULL;
	}
	/* judge whether there is class_name */
	ret = strchr(iter_name, ' ');
	if (ret != NULL) {
		/* truncate iter_name */
		iter_name[ret - iter_name] = '\0';
	}

	for (i = 0 ; i < __ARRAY_SIZE(hmtrace_ipc_iter_array); i++) {
		if (strcmp(hmtrace_ipc_iter_array[i]->name, iter_name) == 0) {
			free(iter_name);
			return hmtrace_ipc_iter_array[i];
		}
	}

	free(iter_name);
	return NULL;
}

void register_match_operations_fn(match_operations_fn_t func)
{
	trace_iter_register(func);
}
#else
const struct iter_operations *match_hmtrace_ipc_iter_operations(const char *name)
{
	UNUSED(name);
	return NULL;
}

void register_match_operations_fn(match_operations_fn_t func)
{
	UNUSED(func);
}
#endif
