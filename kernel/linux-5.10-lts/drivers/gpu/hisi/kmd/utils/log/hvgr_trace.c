#define CREATE_TRACE_POINTS
#include "hvgr_trace.h"
#include <linux/sched/task.h>
#include <securec.h>
#include "hvgr_defs.h"

#define MAX_TRACE_TAG_LEN 512

void hvgr_systrace_begin(const char *fmt, ...)
{
	unused(fmt);
}

void hvgr_systrace_end(void)
{
}
