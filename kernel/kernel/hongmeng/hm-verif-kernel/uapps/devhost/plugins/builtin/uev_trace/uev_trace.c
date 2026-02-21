/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Uev trace server
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 7 10:26:01 CST 2022
 */
#include <libhmsrv_sys/hm_procmgr.h>
#include <devhost/plugin.h>
/* this should be included first before uev_trace_template.h */
#include "sysif_uev.h"
#include <libhmtrace/uev_trace/uev_trace_template.h>

#define __SYSIFUEV_IFACE_INC "sysif_uev_list.h"

/* generate event type */
enum {
#define UEV_POINT UEV_DEFINE_EVENT_TYPE
#include __SYSIFUEV_IFACE_INC
#undef  UEV_POINT
};

/* generate event instance */
#define UEV_POINT       UEV_DEFINE_TRACE_EVENT
#include __SYSIFUEV_IFACE_INC
#undef  UEV_POINT

/* generate event instance table */
static struct uev_trace_event *uev_table[] = {
#define UEV_POINT	UEV_REFERENCE_EVENT
#include __SYSIFUEV_IFACE_INC
#undef	UEV_POINT
};

DEFINE_UEV_TRACE_CLASS(devhost_uev_class, uev_table);

static int __uev_trace_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	int ret = E_HM_OK;

	UNUSED(plugin);

	pid_t pid = hm_getpid();
	if (pid < 0) {
		hm_error("get pid failed, err=%d\n", pid);
		ret = pid;
	}

	if (ret == E_HM_OK) {
		hm_trace_init(pid);
		ret = hm_trace_register_class(&devhost_uev_class);
	}
	return ret;
}

DEFINE_BUILTIN_PLUGIN(uev_trace, __uev_trace_plugin_init);
