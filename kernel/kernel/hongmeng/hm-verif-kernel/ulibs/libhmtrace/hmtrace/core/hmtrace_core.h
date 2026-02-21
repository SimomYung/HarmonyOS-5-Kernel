/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of data struct and declaration of hmtrace inner interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#ifndef ULIBS_LIBHMTRACE_HMTRACE_UTILS_H
#define ULIBS_LIBHMTRACE_HMTRACE_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <hmkernel/compiler.h>
#include <libhmsync/raw_mutex.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libhmtrace/hmtrace.h>
#include <libbunch/bunch_ipc.h>

extern struct hmtrace_ctrl hmtrace;

struct hmtrace_ctrl {
	/* external api entries protects the hmtrace list by locking */
	struct raw_mutex mutex;

	/* holds a link list of registered class */
	struct dlist_node class_list;

	/* the number of registered class */
	int class_num;

	/* external api function entries need to check if initialized */
	bool initialized;
};

struct hmtrace_class *trace_class_of_class_name(const char* class_name);
struct hmtrace_event *trace_event_of_event_name(const struct class_entry *class_entry,
						const char *event_name);

/* trace_class functions */
bool is_class_registered(const struct class_entry *class_entry);
int register_class(struct hmtrace_class *trace_class);
int unregister_class(struct hmtrace_class *trace_class);
int get_registered_class_list(struct bunch_ipc_attr *attr);
int get_registered_events_list(const struct class_entry *class_entry,
			       struct bunch_ipc_attr *attr);
int get_available_events_list(struct hmtrace_class *trace_class,
			      const char *class_name,
			      struct bunch_ipc_attr *attr);

/* trace_event functions */
bool is_event_registered(const struct event_entry *event_entry);
void hm_trace_init_for_premain(void);
int register_event(struct hmtrace_class *trace_class,
		   struct hmtrace_event *trace_event);
int unregister_event(struct hmtrace_event *trace_event);
int set_event_tracing_on(struct hmtrace_event *trace_event);
int set_event_tracing_off(struct hmtrace_event *trace_event);
int get_event_tracing_status(struct hmtrace_event *trace_event, int *status);

/* trace_buffer functions */
int write_event(struct hmtrace_event *trace_event,
		const void *data, size_t data_size);
#ifdef CONFIG_HMTRACE_UNBLOCK_WRITE
int write_event_unblock(struct hmtrace_event *trace_event,
			const void *data, size_t data_size);
#else
static inline
int write_event_unblock(const struct hmtrace_event *trace_event,
			const void *data, size_t data_size)
{
	UNUSED(trace_event, data, data_size);
	return E_HM_NOSYS;
}
#endif
#endif
