// SPDX-License-Identifier: GPL-2.0
#include <linux/types.h>
#include <linux/tracefs.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/export.h>
#include <linux/security.h>
#include <linux/bug.h>
#include <linux/trace_events.h>
#include <securec.h>

#include "tracefs_proxy.h"
#include "tracefs_rmqk.h"
#include "tracefs_helper.h"
#include "tracefs.h"

extern struct static_key global_trace_key;

static DEFINE_MUTEX(trace_printk_list_mutex);
static LIST_HEAD(g_printk_format_list);

extern struct trace_event_call *__start_ftrace_events[];
extern struct trace_event_call *__stop_ftrace_events[];
#define for_each_event(event, start, end)			\
	for (event = start;					\
	     (unsigned long)event < (unsigned long)end;		\
	     event++)

static int enable_ldk_event(struct trace_event_call *call)
{
	int err = 0;

	if (call->event.file)
		return 0;

	struct trace_event_file *file = kzalloc(sizeof(struct trace_event_file), GFP_KERNEL);
	if (file == NULL)
		return -ENOMEM;

	file->event_call = call;
	call->event.file = file;
	err = tracepoint_probe_register(call->tp, call->class->probe, call->event.file);
	if (err != 0) {
		kfree(file);
		call->event.file = NULL;
	}
	return err;
}

static int disable_ldk_event(struct trace_event_call *call)
{
	if (!call->event.file)
		return 0;

	int ret = tracepoint_probe_unregister(call->tp, call->class->probe, call->event.file);
	kfree(call->event.file);
	call->event.file = NULL;

	return ret;
}

int hmtrace_enable_disable_ldk_event(unsigned int tp_index, bool enable)
{
	struct trace_event_call *call;
	unsigned int tp_num = __stop_ftrace_events - __start_ftrace_events;

	if (tp_index >= tp_num) {
		return -EINVAL;
	}
	call = __start_ftrace_events[tp_index];
	if (strcmp(call->class->system, "ftrace") == 0)
		return 0;

	if (enable) {
#ifdef CONFIG_TRACE_PERF
		static_key_enable(&global_trace_key);
#endif
		return enable_ldk_event(call);
	}

	return disable_ldk_event(call);
}
EXPORT_SYMBOL(hmtrace_enable_disable_ldk_event);

struct ftrace_event_field {
	struct list_head	link;
	const char		*name;
	const char		*type;
	int			filter_type;
	int			offset;
	int			size;
	int			is_signed;
};

int hmtrace_ldk_event_format(unsigned int tp_index, char *format)
{
	struct trace_event_call *call = NULL;
	unsigned int tp_num = __stop_ftrace_events - __start_ftrace_events;
	struct seq_buf seq = {0};
	const char *array_descriptor = NULL;
	struct list_head *head = NULL;
	struct ftrace_event_field *field = NULL;

	if (tp_index >= tp_num) {
		return -EINVAL;
	}

	call = __start_ftrace_events[tp_index];
	seq_buf_init(&seq, format, PAGE_SIZE);
	head = trace_get_fields(call);
	list_for_each_entry(field, head, link) {
		/*
		 * Smartly shows the array type(except dynamic array).
		 * Normal:
		 *	field:TYPE VAR
		 * If TYPE := TYPE[LEN], it is shown:
		 *	field:TYPE VAR[LEN]
		 */
		array_descriptor = strchr(field->type, '[');

		if (str_has_prefix(field->type, "__data_loc"))
			array_descriptor = NULL;

		if (!array_descriptor)
			seq_buf_printf(&seq, "\tfield:%s %s;\toffset:%u;\tsize:%u;\tsigned:%d;\n",
				       field->type, field->name, field->offset,
				       field->size, !!field->is_signed);
		else
			seq_buf_printf(&seq, "\tfield:%.*s %s%s;\toffset:%u;\tsize:%u;\tsigned:%d;\n",
				       (int)(array_descriptor - field->type),
				       field->type, field->name,
				       array_descriptor, field->offset,
				       field->size, !!field->is_signed);
	}
	seq_buf_printf(&seq, "\nprint fmt: %s\n",
		       call->print_fmt);

	return 0;
}
EXPORT_SYMBOL(hmtrace_ldk_event_format);

/* This interface does not support concurrency. */
extern struct trace_event *ftrace_find_event(int type);
static struct trace_seq g_tmp_seq;
int hmtrace_print_ldk_event(unsigned int tp_index, void *entry, char *event_str)
{
	struct seq_buf seq = {0};
	struct trace_event_call *call = NULL;
	struct trace_event *event = NULL;
	unsigned int tp_num = __stop_ftrace_events - __start_ftrace_events;

	if (tp_index >= tp_num) {
		return -EINVAL;
	}

	call = __start_ftrace_events[tp_index];
	event = &call->event;
	if ((event != NULL) && (event->hmfuncs != NULL)) {
#ifdef CONFIG_LIBLINUX_HMTRACE
		seq_buf_init(&seq, event_str, PAGE_SIZE);
		trace_seq_init(&g_tmp_seq);
		return event->hmfuncs->print_event(entry, &seq, &g_tmp_seq);
#else
		return -ENOSYS;
#endif
	} else {
		pr_warn("unknown trace type %u", tp_index);
		return -EINVAL;
	}
}
EXPORT_SYMBOL(hmtrace_print_ldk_event);

struct printk_format_s {
	struct list_head list;
	char *str;
};

void trace_get_printk_format_list(struct list_head *printk_format_list);

static void add_ldk_printk_format_list(void)
{
	struct printk_format_s *printk_format, *n;
	int err;

	mutex_lock(&trace_printk_list_mutex);
	if (!list_empty(&g_printk_format_list)) {
		mutex_unlock(&trace_printk_list_mutex);
		return;
	}
	trace_get_printk_format_list(&g_printk_format_list);
	list_for_each_entry(printk_format, &g_printk_format_list, list) {
		if (printk_format->str == NULL) {
			continue;
		}
		err = hmtrace_add_ldk_printk_format(printk_format->str);
		if (err) {
			pr_warn("hmtrace_add_ldk_printk_format failed, err=%d\n", err);
		}
	}
	list_for_each_entry_safe(printk_format, n, &g_printk_format_list, list) {
		list_del(&printk_format->list);
		kfree(printk_format);
	}
	mutex_unlock(&trace_printk_list_mutex);
}

static int trace_register_ldk_events(void)
{
	unsigned int tp_num = __stop_ftrace_events - __start_ftrace_events;
	struct tracepoint_config *config = NULL;
	unsigned int index = 0;
	unsigned int *tp_type;
	struct trace_event_call **iter, *call;
	int err;

	config = kzalloc(sizeof(struct tracepoint_config) * tp_num, GFP_KERNEL);
	if (config == NULL) {
		return -ENOMEM;
	}

	tp_type = kzalloc(sizeof(unsigned int) * tp_num, GFP_KERNEL);
	if (tp_type == NULL) {
		kfree(config);
		return -ENOMEM;
	}

	add_ldk_printk_format_list();

	for_each_event(iter, __start_ftrace_events, __stop_ftrace_events) {
		call = *iter;
		(void)strncpy_s(config[index].group_name, TRACE_EVENT_GROUP_NAME_SIZE,
				call->class->system, TRACE_EVENT_GROUP_NAME_SIZE - 1U);
		(void)strncpy_s(config[index].event_name, TRACE_EVENT_NAME_SIZE,
				trace_event_name(call), TRACE_EVENT_NAME_SIZE - 1U);
		index++;
	}
	err = hmtrace_register_ldk_events(config, tp_num, tp_type);
	if (!err) {
		index = 0;
		for_each_event(iter, __start_ftrace_events, __stop_ftrace_events) {
			call = *iter;
			call->event.type = tp_type[index];
			index++;
		}
	}
	kfree(config);
	kfree(tp_type);
	return err;
}

static int __init hmtracefs_init(void)
{
	int ret;

	ret = trace_register_ldk_events();
	if (ret) {
		pr_warn("trace_register_ldk_events failed");
		return ret;
	}

	return 0;
}
late_initcall(hmtracefs_init);
