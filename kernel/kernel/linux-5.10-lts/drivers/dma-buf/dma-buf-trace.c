/* SPDX-License-Identifier: GPL-2.0-only */

#include <securec.h>
#include <trace/events/dmabuf_trace.h>

#define MAX_TRACE_TAG_LEN 512

#ifdef CONFIG_DMABUF_TRACE_SUPPORT
void dmabuf_systrace_begin(const char *fmt, ...)
{
	if (!trace_tracing_mark_write_enabled())
		return;

	char trace_tag[MAX_TRACE_TAG_LEN];
	va_list ap;

	va_start(ap, fmt);
	(void)vsprintf_s(trace_tag, MAX_TRACE_TAG_LEN, fmt, ap);
	va_end(ap);

	trace_tracing_mark_write(current->tgid, trace_tag, true);
}

void dmabuf_systrace_end(void)
{
	if (!trace_tracing_mark_write_enabled())
		return;

	trace_tracing_mark_write(current->tgid, "", false);
}
#else
void dmabuf_systrace_begin(const char *fmt, ...)
{
}
void dmabuf_systrace_end(void)
{
}
#endif
