/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide API about ring buffer in hmtrace framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmtrace/hmtrace.h>
#include <libhwsecurec/securec.h>

#include "core/hmtrace_core.h"

#ifdef CONFIG_HMTRACE
static int check_event(const struct hmtrace_event *trace_event,
		       const void *data, size_t data_size)
{
	/* check hmtrace status */
	if (!hmtrace.initialized) {
		hm_warn("hmtrace need init first\n");
		return E_HM_POSIX_FAULT;
	}

	if ((trace_event == NULL) || (data == NULL) || (data_size == 0)) {
		hm_warn("hmtrace write event input invalid\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hm_trace_write_event(struct hmtrace_event *trace_event,
			 const void *data, size_t data_size)
{
	int rc;

	rc = check_event(trace_event, data, data_size);
	if (rc != E_HM_OK) {
		return rc;
	}

	return write_event(trace_event, data, data_size);
}

#else
int hm_trace_write_event(struct hmtrace_event *trace_event,
			 const void *data, size_t data_size)
{
	UNUSED(trace_event, data, data_size);
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_HMTRACE_UNBLOCK_WRITE
int hm_trace_write_event_unblock(struct hmtrace_event *trace_event,
				 const void *data, size_t data_size)
{
	int rc;

	rc = check_event(trace_event, data, data_size);
	if (rc != E_HM_OK) {
		return rc;
	}

	return write_event_unblock(trace_event, data, data_size);
}
#else
int hm_trace_write_event_unblock(struct hmtrace_event *trace_event,
				 const void *data, size_t data_size)
{
	UNUSED(trace_event, data, data_size);
	return E_HM_NOSYS;
}
#endif
