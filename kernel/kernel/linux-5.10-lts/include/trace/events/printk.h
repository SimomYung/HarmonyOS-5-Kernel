/* SPDX-License-Identifier: GPL-2.0 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM printk

#if !defined(_TRACE_PRINTK_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_PRINTK_H

#include <linux/tracepoint.h>

TRACE_EVENT(console,
	TP_PROTO(const char *text, size_t len),

	TP_ARGS(text, len),

	TP_STRUCT__entry(
		__dynamic_array(char, msg, len + 1)
	),

	TP_fast_assign(
		/*
		 * Each trace entry is printed in a new line.
		 * If the msg finishes with '\n', cut it off
		 * to avoid blank lines in the trace.
		 */
		if ((len > 0) && (text[len-1] == '\n'))
			len -= 1;

		memcpy(__get_str(msg), text, len);
		__get_str(msg)[len] = 0;
	),

	TP_printk("%s", __get_str(msg))
);

TRACE_EVENT(console_with_ip,
	TP_PROTO(unsigned long ip, const char *text, size_t len),

	TP_ARGS(ip, text, len),

	TP_STRUCT__entry(
		__field(unsigned long, ip)
		__dynamic_array(char, msg, len + 1)
	),

	TP_fast_assign(
		/*
		 * Each trace entry is printed in a new line.
		 * If the msg finishes with '\n', cut it off
		 * to avoid blank lines in the trace.
		 */
		if ((len > 0) && (text[len-1] == '\n'))
			len -= 1;

		__entry->ip = ip;
		memcpy(__get_str(msg), text, len);
		__get_str(msg)[len] = 0;
	),

	TP_printk("%ps: %s", __entry->ip, __get_str(msg))
);
#endif /* _TRACE_PRINTK_H */

void trace_write_console_with_ip(unsigned long ip, const char *fmt, ...);
#define trace_console_write(fmt, args...)					\
do {									\
	trace_write_console_with_ip(_THIS_IP_, fmt, ##args);		\
} while (0)

/* This part must be outside protection */
#include <trace/define_trace.h>
