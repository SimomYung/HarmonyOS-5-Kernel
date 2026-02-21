/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: definitions for uev trace
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 10 08:56:22 2023
 */

#ifndef ULIBS_UEV_TRACE_H
#define ULIBS_UEV_TRACE_H

struct uev_trace_event;

struct uev_trace_event_table {
	struct uev_trace_event **event_array;
	size_t array_size;
	struct uev_trace_event_table *next_table;
};

#define DEFINE_UEV_EVENT_TABLE(__name, __table) \
static struct uev_trace_event_table __name = { \
	.event_array = (__table), \
	.array_size = (sizeof(__table) / sizeof((__table)[0])), \
	.next_table = NULL, \
}

#define for_each_uev_event_table(__class, __table) \
	for ((__table) = (__class)->event_table.uev_table; \
		(__table) != NULL; \
			(__table) = (__table)->next_table)

#define for_each_uev_event(__table, __event, __idx) \
	    for(__idx = 0, (__event) = (__table)->event_array[0]; \
			__idx < (__table)->array_size; \
			idx++, (__event) = (__table)->event_array[__idx]) \

#endif
