/* SPDX-License-Identifier: GPL-2.0 */
/*
 * liblinux Vendor Hooks
 */
#include <asm/sections.h>

#if !defined(_ARCH_TRACE_HOOK_LIBLINUX_H) || defined(TRACE_HEADER_MULTI_READ)
#define _ARCH_TRACE_HOOK_LIBLINUX_H

#define DEFINE_INIT_VENDOR_HOOK(name, hook_f, cond_no_stop)				\
	static void __init register_##name(void)					\
	{										\
		int ret;								\
		ret = register_trace_##name(hook_f, NULL);				\
		if (ret) {								\
			if (cond_no_stop) {						\
				pr_warn("liblinux: register_trace_"#name" failed, ret=%d\n",\
					ret);						\
			} else {							\
				panic("liblinux: register_trace_"#name" failed, ret=%d\n",\
					ret);						\
			}								\
		}									\
	}										\
	static inithook_t __initcall_liblinux_vh_##name __used				\
	__attribute__((__section__(".liblinux.vh.init.data"))) = register_##name

#define INIT_VENDOR_HOOK(name, hook_f) DEFINE_INIT_VENDOR_HOOK(name, hook_f, 0)
#define INIT_VENDOR_HOOK_NOSTOP(name, hook_f) DEFINE_INIT_VENDOR_HOOK(name, hook_f, 1)

/* Ensure define_trace.h include original liblinux.h */
#ifdef TRACE_HEADER_MULTI_READ
#include_next <trace/hooks/liblinux.h>
#else

#if !defined(CONFIG_TRACING)

/* Ensure liblinux hook opened */
#ifndef TRACEPOINTS_ENABLED
#define TRACEPOINTS_ENABLED
#endif
/* Ensure liblinux hook define its own DEFINE_TRACE*/
#ifdef DEFINE_TRACE
#define UNDEFINE_DEFINE_TRACE
#undef DECLARE_TRACE
#undef DEFINE_TRACE
#undef EXPORT_TRACEPOINT_SYMBOL_GPL
#undef EXPORT_TRACEPOINT_SYMBOL
#undef __DECLARE_TRACE
#undef DEFINE_TRACE_FN
#endif

#endif /* !defined(CONFIG_TRACING) */

#include_next <trace/hooks/liblinux.h>

#if !defined(CONFIG_TRACING)

/* Do not break following trace events */
#undef DECLARE_TRACE
#undef DEFINE_TRACE
#undef __DECLARE_TRACE
#undef DEFINE_TRACE_FN
#undef TRACEPOINTS_ENABLED

/* Do not break following EXPORT_TRACEPOINT_SYMBOL */
#ifndef CREATE_TRACE_POINTS
#undef EXPORT_TRACEPOINT_SYMBOL_GPL
#undef EXPORT_TRACEPOINT_SYMBOL
#endif

/* Do not break previous NULL EXPORT_TRACEPOINT_SYMBOL_GPL */
#ifdef UNDEFINE_DEFINE_TRACE
#define EXPORT_TRACEPOINT_SYMBOL_GPL(name)
#define EXPORT_TRACEPOINT_SYMBOL(name)
#endif

#endif /* !defined(CONFIG_TRACING) */

#endif /* TRACE_HEADER_MULTI_READ */
#endif /* !defined(_ARCH_TRACE_HOOK_LIBLINUX_H) */
