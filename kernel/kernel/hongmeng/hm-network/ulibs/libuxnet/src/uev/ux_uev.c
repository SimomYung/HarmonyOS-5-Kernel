/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  UEV tracepoint implementations
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 27 14:16:32 2021
 */

#include "uev/ux_uev.h"
#define __UEV_IFACE_INC "uev/ux_uev_list.h"

/* generate tracepoint implementations */
#define UEV_POINT       DEFINE_UEV
#include __UEV_IFACE_INC
#undef UEV_POINT

#include <libhmtrace/hmtrace.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmtrace/uev_trace/uev_trace_template.h>

/* generate event type */
enum {
#define UEV_POINT UEV_DEFINE_EVENT_TYPE
#include __UEV_IFACE_INC
#undef UEV_POINT
	UEV_POINT_MAX,
};

/* generate event instance */
#define UEV_POINT UEV_DEFINE_TRACE_EVENT
#include __UEV_IFACE_INC
#undef UEV_POINT

/* generate event instance table */
static struct uev_trace_event *__uev_table[UEV_POINT_MAX] = {
#define UEV_POINT UEV_REFERENCE_EVENT
#include __UEV_IFACE_INC
#undef UEV_POINT
};

DEFINE_UEV_TRACE_CLASS(uev_class, __uev_table);

int uev_trace_init(void)
{
	pid_t pid = hm_getpid();

	BUG_ON(pid < 0);
	hm_trace_init(pid);

	if (hm_trace_register_class(&uev_class) < 0) {
		/* ignore uev initialization failure */
		net_debug(HMNET_CORE, "failed to register uev trace\n");
	}

	return 0;
}
