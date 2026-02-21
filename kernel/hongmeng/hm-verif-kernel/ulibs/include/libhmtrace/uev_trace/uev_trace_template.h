/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: template macro for uev trace
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 28 20:46:33 2020
 */

#ifndef ULIBS_UEV_TRACE_TEMPLATE_H
#define ULIBS_UEV_TRACE_TEMPLATE_H

#include <hongmeng/types.h>
#include <hongmeng/macro.h>
#include <libhmtrace/hmtrace.h>
#include <libhmuev/uev_template.h>

#define UEV_TRACE_EXPAND_FIELD(n, ...)		__SC_MAPSTAT(n, __SC_DECL, __VA_ARGS__)
#define UEV_TRACE_FUNC_PROTO(n, ...)		__SC_MAP(n, __SC_DECL, __VA_ARGS__)
#define UEV_TRACE_FUNC_CALL(n, ...)		__SC_MAP(n, __SC_USE, __VA_ARGS__)

#define UEV_TRACE_ASSIGN(code)	code
#define UEV_TRACE_STRUCT(...)	__VA_ARGS__
#define UEV_TRACE_ARG(...)	__VA_ARGS__
#define	UEV_TRACE_SHOW(...)	__VA_ARGS__
#define UEV_TRACE_FIELD(...)	__VA_ARGS__
#define UEV_TRACE_STRING(name, size)        name[size]
#define UEV_TRACE_EFMT_FIELD(...) __VA_ARGS__

struct uev_trace_event {
	unsigned int id;
	struct hmtrace_event evt;
	struct uev_request_obj uev_obj;
	int (*register_handler) (struct uev_request_obj *);
	int (*unregister_handler) (struct uev_request_obj *);
};

#define UEV_GEN_TRACE_DATA(name, ...)		\
struct uevtrace_##name##_data {			\
	unsigned int id;			\
	UEV_TRACE_EXPAND_FIELD(__VA_ARGS__);	\
};

#define UEV_MAX_BUF_SIZE	128
/* use print handler before efmt introduced */
#define UEV_GEN_TRACE_STRUCT(sname, args, efmt_fields)	\
DEFINE_EVENT_FORMAT(sname, UEV_TRACE_ARG(efmt_fields));		\
static void uevtrace_##sname##_print(UEV_TRACE_FUNC_PROTO(args));	\
static struct uev_trace_event uevtrace_##sname##_event = {		\
	.id = (unsigned int)etype_##sname,	\
	.evt = {				\
		.name = #sname,			\
		.event_entry = HMTRACE_EVENT_ENTRY_INIT(uevtrace_##sname##_event.evt.event_entry),	\
		.efmt = TRACE_EVENT_FORMAT(sname),	\
	},					\
	.uev_obj = {				\
		.list = DLIST_HEAD_INIT(uevtrace_##sname##_event.uev_obj.list),	\
		.handler = uevtrace_##sname##_print,	\
		.prio = UEV_PRIORITY_DEFAULT,		\
		.enable = false,			\
	},						\
	.register_handler = uev_##sname##_register,	\
	.unregister_handler = uev_##sname##_unregister,	\
};

#define UEV_GEN_TRACE_FUNC(name, args, assigns)						\
static void uevtrace_##name##_print(UEV_TRACE_FUNC_PROTO(args))				\
{											\
	struct uev_trace_data trace_data = {0};						\
	trace_data.size = UEV_TRACE_DATA_BUF;						\
	assigns;									\
	(void)hm_trace_write_event(&(uevtrace_##name##_event.evt), trace_data.buf,	\
				   trace_data.offset);					\
}

#define UEV_DEFINE_EVENT_TYPE(module, name, ...) etype_##name,
#define UEV_REFERENCE_EVENT(module, name, ...)	\
[(unsigned int)etype_##name] = &uevtrace_##name##_event,

#define UEV_DEFINE_TRACE_EVENT(module, name, args, fields, assigns, efmt_fields)	\
UEV_GEN_TRACE_DATA(name, UEV_TRACE_ARG(fields))	\
UEV_GEN_TRACE_STRUCT(name, UEV_TRACE_ARG(args), UEV_TRACE_ARG(efmt_fields))	\
UEV_GEN_TRACE_FUNC(name, UEV_TRACE_ARG(args), UEV_TRACE_ARG(assigns))

#endif
