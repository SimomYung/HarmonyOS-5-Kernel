/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide hooks of function_graph for trace framework
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 24 19:54:44 2019
 */
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmelf/elf_load.h>
#include <libhmtrace/hmtrace.h>
#include <libhmtrace/ftrace/function_graph.h>

#ifdef CONFIG_FUNCTION_GRAPH
#include "function_graph.h"

#define INDENTATION_SIZE		2
#define NANOSECOND_EVERY_SECOND		1000000000

/* global_depth means the depth of function call */
static int global_depth = 0;
static bool tracing_on = false;
static struct dlist_node *graph_handler_record_list = NULL;
static struct dlist_node *graph_show_record_list = NULL;

static int function_graph_stop(struct ftrace_tracer_s *tracer)
{
	UNUSED(tracer);
	tracing_on = false;
	return E_HM_OK;
}

static int function_graph_start(struct ftrace_tracer_s *tracer)
{
	UNUSED(tracer);
	tracing_on = true;
	return E_HM_OK;
}

static int function_graph_prepare_for_show(struct ftrace_tracer_s *tracer)
{
	UNUSED(tracer);
	graph_show_record_list = (struct dlist_node *)malloc(sizeof(struct dlist_node));
	if (graph_show_record_list == NULL) {
		hm_warn("graph_show_record_list alloc failed\n");
		return E_HM_NOMEM;
	}
	dlist_init(graph_show_record_list);

	return function_trace_prepare_for_show(NULL);
}

static int function_graph_clean_after_show(struct ftrace_tracer_s *tracer)
{
	UNUSED(tracer);

	struct graph_show_record *show_record = NULL;
	struct graph_show_record *tmp = NULL;

	dlist_for_each_entry_safe(show_record, tmp, graph_show_record_list,
				  struct graph_show_record, dlist) {
		dlist_del_entry(show_record, struct graph_show_record, dlist);
		free(show_record);
	}
	free(graph_show_record_list);
	graph_show_record_list = NULL;

	return function_trace_clean_after_show(NULL);
}

static char *indentation_space_alloc(int depth)
{
	int i;
	int ret;
	size_t indentation_space_len;
	char *indentation_space = NULL;

	indentation_space_len = (size_t)(unsigned int)(INDENTATION_SIZE * depth + 1);
	indentation_space = (char *)malloc(indentation_space_len);
	if (indentation_space == NULL) {
		hm_warn("indentation_space alloc failed\n");
	}

	if (indentation_space != NULL) {
		ret = strncpy_s(indentation_space, indentation_space_len, "", 1);
		if (ret != 0) {
			hm_warn("strncpy_s to indentation_space failed\n");
		}

		for (i = 0; i < depth; i++) {
			ret = strcat_s(indentation_space, indentation_space_len, "  ");
			if (ret != 0) {
				hm_warn("strcat_s to indentation space failed\n");
				free(indentation_space);
				indentation_space = NULL;
				break;
			}
		}
	}

	return indentation_space;
}

static int snprintf_show_record(char *buffer, unsigned int buffer_size,
				enum graph_show_format format, int depth,
				const struct graph_show_record *show_record)
{
	int ret = 0;
	int err = E_HM_OK;
	char *tmp = NULL;
	char *indentation_space = NULL;

	tmp = indentation_space_alloc(depth);
	indentation_space = (tmp == NULL) ? "" : tmp;

	switch (format) {
	case NOT_MATCH_FMT:
		ret = snprintf_s(buffer, buffer_size, buffer_size - 1,
				 "[%3ld.%09lds]%s} /* %s */",
				 show_record->record.time.tv_sec,
				 show_record->record.time.tv_nsec,
				 indentation_space, show_record->func_name);
		break;
	case LEAF_NODE_FMT:
		ret = snprintf_s(buffer, buffer_size, buffer_size - 1,
				 "[%3ld.%09lds]%s%s();",
				 show_record->record.time.tv_sec,
				 show_record->record.time.tv_nsec,
				 indentation_space, show_record->func_name);
		break;
	case NESTED_ENT_FMT:
		ret = snprintf_s(buffer, buffer_size, buffer_size - 1,
				 "[              ]%s%s(){",
				 indentation_space, show_record->func_name);
		break;
	case NESTED_RET_FMT:
		ret = snprintf_s(buffer, buffer_size, buffer_size - 1,
				 "[%3ld.%09lds]%s};",
				 show_record->record.time.tv_sec,
				 show_record->record.time.tv_nsec,
				 indentation_space);
		break;
	default:
		err = E_HM_INVAL;
		break;
	}

	/* if snprintf_s failed, set buffer to zero, and print nothing */
	if (ret < 0) {
		mem_zero_p(buffer, buffer_size, buffer_size);
		err = E_HM_POSIX_FAULT;
	}
	if (tmp != NULL) {
		free(tmp);
	}
	return err;
}

static int not_print_format(char *buffer, unsigned int buffer_size,
			    struct graph_show_record *curr_show_record)
{
	curr_show_record->is_printed = false;
	dlist_insert_tail(graph_show_record_list, &curr_show_record->dlist);
	mem_zero_p(buffer, buffer_size, buffer_size);

	return E_HM_OK;
}

static int not_match_format(char *buffer, unsigned int buffer_size,
			    struct graph_show_record *curr_show_record)
{
	int err;

	err = snprintf_show_record(buffer, buffer_size, NOT_MATCH_FMT,
				   curr_show_record->record.depth,
				   curr_show_record);
	free(curr_show_record);
	curr_show_record = NULL;

	return err;
}

static int nested_entry_format(char *buffer, unsigned int buffer_size,
			       struct graph_show_record *prev_show_record,
			       struct graph_show_record *curr_show_record)
{
	int err;

	err = snprintf_show_record(buffer, buffer_size, NESTED_ENT_FMT,
				   prev_show_record->record.depth,
				   prev_show_record);
	prev_show_record->is_printed = true;
	curr_show_record->is_printed = false;
	dlist_insert_tail(graph_show_record_list, &curr_show_record->dlist);

	return err;
}

static int leaf_node_format(char *buffer, unsigned int buffer_size,
			    struct graph_show_record *prev_show_record,
			    struct graph_show_record *curr_show_record)
{
	int err;

	err = snprintf_show_record(buffer, buffer_size, LEAF_NODE_FMT,
				   curr_show_record->record.depth,
				   curr_show_record);

	dlist_del_entry(prev_show_record, struct graph_show_record, dlist);
	free(prev_show_record);
	free(curr_show_record);
	prev_show_record = NULL;
	curr_show_record = NULL;

	return err;
}

static int nested_return_format(char *buffer, unsigned int buffer_size,
				struct graph_show_record *prev_show_record,
				struct graph_show_record *curr_show_record)
{
	int err;

	err = snprintf_show_record(buffer, buffer_size, NESTED_RET_FMT,
				   curr_show_record->record.depth,
				   curr_show_record);

	dlist_del_entry(prev_show_record, struct graph_show_record, dlist);
	free(prev_show_record);
	free(curr_show_record);
	prev_show_record = NULL;
	curr_show_record = NULL;

	return err;
}

static int show_format_distribution(char *buffer, unsigned int buffer_size,
				    struct graph_show_record *prev_show_record,
				    struct graph_show_record *curr_show_record)
{
	if (prev_show_record == NULL) {
		if (curr_show_record->record.flag == GRAPH_ENT) {
			return not_print_format(buffer, buffer_size, curr_show_record);
		} else {
			return not_match_format(buffer, buffer_size, curr_show_record);
		}
	}

	/* this circumstances is not hoped */
	if (prev_show_record->record.flag == GRAPH_RET) {
		mem_zero_p(buffer, buffer_size, buffer_size);
		return E_HM_OK;
	}

	if (curr_show_record->record.flag == GRAPH_ENT) {
		if (prev_show_record->is_printed == false) {
			return nested_entry_format(buffer, buffer_size,
						   prev_show_record, curr_show_record);
		} else {
			return not_print_format(buffer, buffer_size, curr_show_record);
		}
	} else {
		if (prev_show_record->is_printed == false) {
			return leaf_node_format(buffer, buffer_size,
						prev_show_record, curr_show_record);
		} else {
			return nested_return_format(buffer, buffer_size,
						    prev_show_record, curr_show_record);
		}
	}
}

static int function_graph_show(struct ftrace_tracer_s *tracer,
			       void *data, unsigned int data_size,
			       void *buffer, unsigned int buffer_size)
{
	UNUSED(tracer);
	UNUSED(data_size);

	int err;
	struct symbol_entry ip_entry;
	struct function_graph_record *record = NULL;
	struct graph_show_record *prev_show_record = NULL;
	struct graph_show_record *curr_show_record = NULL;

	if (!dlist_empty(graph_show_record_list)) {
		prev_show_record = dlist_entry(dlist_get_last(graph_show_record_list),
					       struct graph_show_record, dlist);
	}

	curr_show_record = (struct graph_show_record *)malloc(sizeof(struct graph_show_record));
	if (curr_show_record == NULL) {
		hm_warn("struct graph_show_record alloc failed\n");
		return E_HM_NOMEM;
	}

	record = (struct function_graph_record *)data;
	err = symbol_lookup_by_addr(elf_sym_desc, sym_mode,
				    record->ip, &ip_entry);
	if (err) {
		NOFAIL(strncpy_s(ip_entry.name, SYMBOL_NAME_MAX_LEN,
				 UNKNOWN_NAME, SYMBOL_NAME_MAX_LEN - 1));
	}

	curr_show_record->func_name = ip_entry.name;
	err = memcpy_s(&(curr_show_record->record), sizeof(struct function_graph_record),
		       record, sizeof(struct function_graph_record));
	if (err) {
		hm_warn("memcpy_s failed\n");
	}

	return show_format_distribution(buffer, buffer_size,
					prev_show_record, curr_show_record);
}

uintptr_t return_handler_hook(void)
{
	int err;
	struct timespec curr_time_stamp;
	struct function_graph_record record;
	struct graph_handler_record *last_handler_record = NULL;
	uintptr_t ret_addr;

	/* get the timestamp in the ret of traced function */
	err = hm_clock_gettime(CLOCK_MONOTONIC, &curr_time_stamp);
	if (err < 0) {
		hm_warn("function graph get ret time failed: %s\n", hmstrerror(err));
		mem_zero_s(curr_time_stamp);
	}

	/* acquire the last node in list and remove it, like pop stack */
	last_handler_record = dlist_entry(dlist_get_last(graph_handler_record_list),
					  struct graph_handler_record, dlist);
	dlist_del_entry(last_handler_record, struct graph_handler_record, dlist);

	/* calculate the total time consumed by traced function */
	if ((curr_time_stamp.tv_nsec == 0 && curr_time_stamp.tv_sec == 0) ||
	    (last_handler_record->record.time.tv_nsec == 0 &&
	    last_handler_record->record.time.tv_sec == 0)) {
		record.time.tv_nsec = 0;
		record.time.tv_sec = 0;
	} else {
		record.time.tv_nsec = curr_time_stamp.tv_nsec -
				      last_handler_record->record.time.tv_nsec;
		record.time.tv_sec = curr_time_stamp.tv_sec -
				     last_handler_record->record.time.tv_sec;
		if (record.time.tv_nsec < 0) {
			record.time.tv_nsec = record.time.tv_nsec + NANOSECOND_EVERY_SECOND;
			record.time.tv_sec = record.time.tv_sec - 1;
		}
	}

	record.ip = last_handler_record->record.ip;
	record.depth = --global_depth;
	record.flag = GRAPH_RET;

	ret_addr = last_handler_record->ret_addr;
	err = hm_ftrace_write_event(&function_graph, &record,
				    sizeof(struct function_graph_record));
	if (err) {
		hm_warn("function graph write return record data failed: %s\n", hmstrerror(err));
		hm_ftrace_function_graph_set_tracing_off();
	}

	free(last_handler_record);
	last_handler_record = NULL;

	return ret_addr;
}

void function_graph_handler_hook(void)
{
	int err;
	uintptr_t reg_current_fp;
	uintptr_t stack_frame_fp;
	uintptr_t stack_frame_pc;
	struct graph_handler_record *handler_record = NULL;

	if (tracing_on) {
		handler_record = (struct graph_handler_record *)malloc(sizeof(struct graph_handler_record));
		if (handler_record == NULL) {
			hm_warn("struct graph_handler_record alloc failed\n");
			return;
		}
		dlist_insert_tail(graph_handler_record_list, &handler_record->dlist);
		handler_record->record.depth = global_depth++;
		handler_record->record.flag = GRAPH_ENT;

		/* acquire current fp register */
		asm volatile("mov %0, fp" : "=r"(reg_current_fp));

		/* get the ip of traced function */
		stack_frame_fp = *(uintptr_t *)reg_current_fp;
		handler_record->record.ip = *(uintptr_t *)(stack_frame_fp + sizeof(unsigned long));

		/* get the return addr of traced function and store in graph_handler_record_list */
		stack_frame_fp = *(uintptr_t *)stack_frame_fp;
		stack_frame_pc = stack_frame_fp + sizeof(unsigned long);
		handler_record->ret_addr = *(uintptr_t *)stack_frame_pc;

		/* replace the return addr of traced function with addr of return_handler_trampoline */
		*(uintptr_t *)stack_frame_pc = (uintptr_t)return_handler_trampoline;

		/* get the timestamp in the entry of traced function */
		err = hm_clock_gettime(CLOCK_MONOTONIC, &(handler_record->record.time));
		if (err < 0) {
			hm_warn("function graph get entry time failed: %s\n", hmstrerror(err));
			mem_zero_s(handler_record->record.time);
		}

		err = hm_ftrace_write_event(&function_graph, &(handler_record->record),
					    sizeof(struct function_graph_record));
		if (err) {
			hm_warn("function graph write entry record data failed: %s\n", hmstrerror(err));
			hm_ftrace_function_graph_set_tracing_off();
		}
	}
}

static int function_graph_init(struct ftrace_tracer_s *tracer)
{
	UNUSED(tracer);

	/* init the graph_handler_record_list only the first time register function graph */
	if (graph_handler_record_list == NULL) {
		graph_handler_record_list = (struct dlist_node *)malloc(sizeof(struct dlist_node));
		if (graph_handler_record_list == NULL) {
			hm_warn("graph_handler_record_list alloc failed\n");
			return E_HM_NOMEM;
		}
		dlist_init(graph_handler_record_list);
	}

	return E_HM_OK;
}

static int function_graph_exit(struct ftrace_tracer_s *tracer)
{
	UNUSED(tracer);
	return E_HM_OK;
}

struct ftrace_tracer_s function_graph = {
	.name = "function_graph",
	.init = function_graph_init,
	.release = function_graph_exit,
	.start = function_graph_start,
	.stop = function_graph_stop,
	.handler_hook = NULL,
	.show = function_graph_show,
	.pre_report_events = function_graph_prepare_for_show,
	.post_report_events = function_graph_clean_after_show,
};
#endif
