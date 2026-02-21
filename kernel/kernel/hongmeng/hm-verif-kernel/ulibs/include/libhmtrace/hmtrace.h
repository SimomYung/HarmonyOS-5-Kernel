/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of data struct and declaration of hmtrace API
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#ifndef ULIBS_LIBHMTRACE_HMTRACE_H
#define ULIBS_LIBHMTRACE_HMTRACE_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <lib/dlist.h>
#include <libhmsync/raw_mutex.h>
#include <libhmtrace/efmt.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libbunch/bunch_ipc.h>
#include <libhmtrace/uev_trace/uev_trace.h>

#define HMTRACEMGR_PATH_FMT "hmtracemgr_ap_%010d"
#define HMTRACEMGR_PATH_MAX_SIZE 128

#define HMTRACE_MAX_CLASS_NAME_SIZE			32
#define HMTRACE_MAX_EVENT_NAME_SIZE			500
/* the following macro is for the size of hmtrace_eventname in sysif */
#define HMTRACE_MAX_EVENT_NAME_SIZE_RESTRICTED		64
#define HMTRACE_DEFAULT_LOCAL_BUFFER_SIZE		(1 << 16)
#define EVENT_DATA_HDR_SIZE				sizeof(struct event_data)
#define trace_info(format, ...) (void)printf(format, ##__VA_ARGS__)
#define trace_warn(format, ...) (void)fprintf(stderr, "[hmtrace|%s]"format, \
					      __func__, ##__VA_ARGS__)

#define UPROBE_TRACE	"uprobe_trace"
#define FUNCTION_TRACE	"function_trace"
#define UEV_TRACE	"uev_trace"
#define KEV_TRACE	"kev_trace"

enum hmtrace_tracing_status {
	TRACING_OFF = 0x0,
	TRACING_ON,
};

struct event_data {
	int eid;
	pid_t tid;
	pid_t pid;
	unsigned int cpuid;
	char name[HMCAP_THREAD_NAME_SIZE];
	uint64_t timestamp;
	unsigned char data[0];
};

struct event_entry {
	int eid;
	struct class_entry *class_entry;
	struct dlist_node dlist;

	/* event status, reference enumeration value */
	enum hmtrace_tracing_status status;
};

struct hmtrace_event {
	struct event_entry event_entry;

	/* specify event name */
	char name[HMTRACE_MAX_EVENT_NAME_SIZE];
	struct event_format *efmt;
};

struct class_entry {
	struct dlist_node dlist;

	/* holds a link list of registered events */
	struct dlist_node events_list;

	/* this class's ring buffer */
	struct rb *rb;

	/* the number of events registered on this class */
	int event_num;

	/* mutex used to protect rb */
	struct raw_mutex mutex;
};

struct hmtrace_class {
	struct class_entry class_entry;

	/* specify hmtrace class name */
	char name[HMTRACE_MAX_CLASS_NAME_SIZE];

	union {
		struct uev_trace_event_table *uev_table;
	} event_table;
	/* called when register the event by hmtrace.elf */
	int (*register_event)(struct hmtrace_class *trace_class,
			      const void *buffer, unsigned int buffer_size);

	/* called when unregister the event by hmtrace.elf */
	int (*unregister_event)(struct hmtrace_class *trace_class,
				const void *buffer, unsigned int buffer_size);

	/* called when enable the event by hmtrace.elf */
	int (*enable_event)(const struct hmtrace_event *trace_event);

	/* called when disable the event by hmtrace.elf */
	int (*disable_event)(const struct hmtrace_event *trace_event);

	/* called when get_available_events_list by hmtrace.elf */
	int (*get_available_events_list)(struct bunch *bunch, void *data);

	/* called when show event by hmtrace.elf */
	int (*prepare_for_show)(const struct hmtrace_class *trace_class);

	/* called when realloc buffer size by hmtrace.elf */
	size_t (*freeram)(void);
};

#define DEFINE_UEV_TRACE_CLASS(__trace_class_name, __table) \
DEFINE_UEV_EVENT_TABLE(__trace_class_name##_table, __table); \
static struct hmtrace_class __trace_class_name = { \
		.name = "uev_trace", \
			.event_table = { \
			.uev_table = &__trace_class_name##_table, \
			}, \
		.register_event = uev_register_event, \
		.unregister_event = uev_unregister_event, \
		.get_available_events_list = uev_get_available_events, \
		.class_entry = HMTRACE_CLASS_ENTRY_INIT(__trace_class_name.class_entry), \
		.enable_event = uev_enable_event, \
		.disable_event = uev_disable_event, \
		.prepare_for_show = NULL, \
		.freeram = NULL \
}

#define HMTRACE_CLASS_ENTRY_INIT(class_entry) { DLIST_HEAD_INIT((class_entry).dlist),\
						DLIST_HEAD_INIT((class_entry).events_list),\
						NULL, 0 , RAW_MUTEX_INITIALIZER_RECURSIVE_NOTRACE}

#define HMTRACE_EVENT_ENTRY_INIT(event_entry) { -1, NULL, \
						DLIST_HEAD_INIT((event_entry).dlist),\
						TRACING_OFF }

/* init hmtrace framework */
void hm_trace_init(pid_t pid);

/* init class entry before register class */
void class_entry_init(struct class_entry *class_entry);

/* register a class */
int hm_trace_register_class(struct hmtrace_class *trace_class);

/* unregister a class */
int hm_trace_unregister_class(struct hmtrace_class *trace_class);

/* acquire the struct of trace_class by class_name */
struct hmtrace_class *hm_trace_class_of_class_name(const char* class_name);

/* init event entry before register event */
void event_entry_init(struct event_entry *event_entry);

/* register a event, a event can only be registered to a unique class */
int hm_trace_register_event(struct hmtrace_class *trace_class,
			    struct hmtrace_event *trace_event);

/* unregister a event, and remove from events_class belongs to */
int hm_trace_unregister_event(struct hmtrace_event *trace_event);

/* acquire the struct of trace_event by class_name and event_name */
struct hmtrace_event *hm_trace_event_of_event_name(const struct hmtrace_class *trace_class,
						   const char *event_name);

/* set event tracing on by server */
int hm_trace_set_event_tracing_on(struct hmtrace_event *trace_event);

/* set event tracing on by server */
int hm_trace_set_event_tracing_off(struct hmtrace_event *trace_event);

/* get event tracing status by server */
int hm_trace_get_event_tracing_status(struct hmtrace_event *trace_event,
				      int *status);

/* write event data to buffer, with checking validity of event */
int hm_trace_write_event(struct hmtrace_event *trace_event,
			 const void *data, size_t data_size);

/* turn on/off all the trace event in hmtrace */
int hm_trace_tracing_on(enum hmtrace_tracing_status status);

int hm_trace_write_event_unblock(struct hmtrace_event *trace_event,
				 const void *data, size_t data_size);
struct rb *hm_trace_rb_get(struct class_entry *entry);
void hm_trace_rb_put(struct rb *rb);
#endif
