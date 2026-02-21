/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Handler for tracefs sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 12:32:02 2023
 */
#include <libsysif/devhost/server.h>
#include <libstrict/strict.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libsysif/sysmgr/api.h>
#include <libmem/umem.h>
#include <hongmeng/errno.h>
#include <devhost/log.h>
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <libdevhost/devhost.h>
#include <lib/trace/tracepoint_defs.h>
#include <intapi/tracemgr/trace.h>
#include <libhmtrace_ng/hmtrace.h>

#include "tracefs.h"

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_trace_enable_disable_ldk_event, dh_trace_enable_disable_ldk_event,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tp_index,
			 bool, enable)
{
	UNUSED(sender, credential);

	return devhost_trace_enable_disable_ldk_event(tp_index, enable);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_trace_ldk_event_format, dh_trace_ldk_event_format,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tp_index,
			 char *, format_user)
{
	UNUSED(sender, credential);

	int rc = E_HM_OK;

	char *format = calloc(1, TRACE_ITER_BUF_SIZE);
	if (format == NULL) {
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		rc = devhost_trace_ldk_event_format(tp_index, format);
	}

	if (rc == E_HM_OK) {
		rc = hm_copy_to_user(format_user, format,
				     strnlen(format, TRACE_ITER_BUF_SIZE - 1) + (size_t)1);
	}

	if (format != NULL) {
		free(format);
	}

	return rc;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_trace_print_ldk_event, dh_trace_print_ldk_event,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tp_index,
			 void *, entry_user,
			 size_t, entry_size,
			 char *, event_str_user)
{
	UNUSED(sender, credential);

	int rc = E_HM_OK;
	void *entry = NULL;
	char *event_str = NULL;

	if (entry_size > TRACE_ITER_BUF_SIZE) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		event_str = calloc(1, TRACE_ITER_BUF_SIZE);
		if (event_str == NULL) {
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		entry = calloc(1, entry_size);
		if (entry == NULL) {
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		rc = hm_copy_from_user(entry, entry_user, entry_size);
	}

	if (rc == E_HM_OK) {
		rc = devhost_trace_print_ldk_event(tp_index, entry, event_str);
	}

	if (rc == E_HM_OK) {
		rc = hm_copy_to_user(event_str_user, event_str,
				     strnlen(event_str, TRACE_ITER_BUF_SIZE - 1) + (size_t)1);
	}

	if (event_str != NULL) {
		free(event_str);
	}
	if (entry != NULL) {
		free(entry);
	}

	return rc;
}

static int devhost_tracefs_create_early_events(void)
{
	int ret;
	uref_t dh_uref = 0ULL;

	dh_uref = libdh_ap_export(hmsrv_sysmgr_cnode_idx());
	if (IS_REF_ERR(dh_uref)) {
		dh_error("export actv pool to sysmgr failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(dh_uref)));
		return REF_TO_ERR(dh_uref);
	}
	ret = actvcall_hmcall_tracemgr_register_devhost(dh_uref);
	if (ret < 0) {
		(void)hm_ucap_revoke(0, dh_uref);
		dh_error("actvcall_hmcall_tracemgr_register_devhost failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
	return ret;
}

/* load tracefs module */
static int tracefs_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("tracefs.ko");
	if (ret < 0) {
		dh_error("lnxbase: load tracefs.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}
	ret = devhost_tracefs_create_early_events();
	if (ret < 0) {
		dh_error("devhost_tracefs_create_early_events failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, tracefs_load_module, NULL);

/*
 * tracefs sysif module
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_trace_enable_disable_ldk_event`
 *  - target sysif-ep: `drvhandler_devhost_trace_ldk_event_format`
 *  - target sysif-ep: `drvhandler_devhost_trace_print_ldk_event`
 *
 * dependence: NULL
 */
DEFINE_DSO_PLUGIN(tracefs, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_trace_enable_disable_ldk_event, dh_trace_enable_disable_ldk_event),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_trace_ldk_event_format, dh_trace_ldk_event_format),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_trace_print_ldk_event, dh_trace_print_ldk_event),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
