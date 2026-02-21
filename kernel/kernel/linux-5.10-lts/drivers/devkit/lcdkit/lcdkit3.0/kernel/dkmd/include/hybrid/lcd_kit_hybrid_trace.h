/* SPDX-License-Identifier: GPL-2.0 */
/*
 * lcd_kit_hybrid_trace.h
 *
 * head file for hybrid systrace
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#if !defined(__LCD_KIT_HYBRID_TRACE_H_) || defined(TRACE_HEADER_MULTI_READ)
#define __LCD_KIT_HYBRID_TRACE_H_

#include <linux/tracepoint.h>

#undef TRACE_SYSTEM
#define TRACE_SYSTEM lcd_hybrid
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE lcd_kit_hybrid_trace

TRACE_EVENT(tracing_mark_write,
	TP_PROTO(const char *name, bool trace_begin),
	TP_ARGS(name, trace_begin),
	TP_STRUCT__entry(
		__field(u64, pid)
		__string(trace_name, name)
		__field(bool, trace_begin)
	),
	TP_fast_assign(
		__entry->pid = current->pid;
		__assign_str(trace_name, name);
		__entry->trace_begin = trace_begin;
	),
	TP_printk("%s|%d|%s", __entry->trace_begin ? "B" : "E",
		__entry->pid, __get_str(trace_name))
);

#define LCD_HYBRID_TRACE_BEGIN(name) trace_tracing_mark_write(name, true)
#define LCD_HYBRID_TRACE_END(name) trace_tracing_mark_write(name, false)

void lcd_hybrid_trace_begin(const char *str);
void lcd_hybrid_trace_end(const char *str);

#endif /* __LCD_KIT_HYBRID_TRACE_H_ */

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .

#include <trace/define_trace.h>
