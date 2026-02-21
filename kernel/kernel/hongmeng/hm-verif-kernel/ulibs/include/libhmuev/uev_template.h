/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Introduce generating macro for uev
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 18 19:46:33 2019
 */

#ifndef ULIBS_LIBHMUEV_UEV_TEMPLATE_H
#define ULIBS_LIBHMUEV_UEV_TEMPLATE_H

#include <lib/dlist.h>
#include <libhmsync/raw_rwlock.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/macro.h>
#include <stdbool.h>

#define UEV_MAX_NAME 64

#define UEV_TRACE_DATA_BUF 1024
#define UEV_PRIORITY_DEFAULT 100
#define UEV_PRIORITY_MAX 256

struct uev_header {
	struct dlist_node list;
	char name[UEV_MAX_NAME];
	struct raw_rwlock rwlock;
	unsigned int refcnt;
	/* replace 'enable' as jump lable in future */
	bool enable;
};

struct uev_request_obj {
	struct dlist_node list;
	void *handler;
	unsigned char prio;
	bool enable;
};

struct uev_trace_data {
	size_t size;
	size_t offset;
	char buf[UEV_TRACE_DATA_BUF];
};

#define UEV_STRUCT_PROTO(n, ...)	__SC_MAPSTAT(n, __SC_DECL, __VA_ARGS__)
#define UEV_FUNC_PROTO(n, ...)		__SC_MAP(n, __SC_DECL, __VA_ARGS__)
#define UEV_FUNC_CALL(n, ...)		__SC_MAP(n, __SC_USE, __VA_ARGS__)

#define UEV_ASSIGNS(code)	code
#define UEV_STRUCT(...)		__VA_ARGS__
#define UEV_ARG(...)		__VA_ARGS__
#define	UEV_PRINT(code)		code

int _uev_common_register(struct uev_header *header,
			 struct uev_request_obj *obj);
int _uev_common_unregister(struct uev_header *header,
			   struct uev_request_obj *obj);
struct hmtrace_event;
struct hmtrace_class;
struct bunch;
void uev_data_write_int(struct uev_trace_data *data, int v);
void uev_data_write_u64(struct uev_trace_data *data, uint64_t v);
void uev_data_write_string(struct uev_trace_data *data, const void* src);
void uev_data_write(struct uev_trace_data *data, const void *buf, size_t sz);
int uev_enable_event(const struct hmtrace_event *trace_event);
int uev_disable_event(const struct hmtrace_event *trace_event);
int uev_get_available_events(struct bunch *bunch, void *data);
int uev_register_event(struct hmtrace_class *trace_class,
		       const void *buffer, unsigned int buffer_size);
int uev_unregister_event(struct hmtrace_class *trace_class,
			 const void *buffer, unsigned int buffer_size);

static inline void uev_enable_request(struct uev_request_obj *uobj)
{
	uobj->enable = true;
}

static inline void uev_disable_request(struct uev_request_obj *uobj)
{
	uobj->enable = false;
}

/*
 * ulibs can used by different apps, which have their own configs,
 * So you need define '__HM_UEV__' before including this head file,
 * if you want enable hmuev in your apps.
 */
#ifdef __HM_UEV__

#define UEV_HEAD_DECLARE(name)	\
extern struct uev_header uev_head_##name;

#define UEV_HEAD_DEFINE(_name)	\
	struct uev_header uev_head_##_name = {						\
		.list = DLIST_HEAD_INIT(uev_head_##_name.list),				\
		.refcnt = 0,								\
		.enable = false,							\
		.rwlock = RAW_RWLOCK_INITIALIZER_NOTRACE(uev_head_##_name.rwlock),	\
		.name = #_name,								\
	};

#define UEV_FUNC_DECLARE(name, args)	\
static inline int uev_##name##_register(struct uev_request_obj *uev_nobj)	\
{										\
	return _uev_common_register(&uev_head_##name, uev_nobj);		\
}										\
										\
static inline int uev_##name##_unregister(struct uev_request_obj *uev_nobj)	\
{										\
	return _uev_common_unregister(&uev_head_##name, uev_nobj);		\
}										\
extern void _uev_trace_##name(UEV_FUNC_PROTO(args));		\
static inline void uev_trace_##name(UEV_FUNC_PROTO(args))	\
{								\
	/* use jump lable to reduce cost in future */		\
	if (uev_enabled(&uev_head_##name)) {			\
		_uev_trace_##name(UEV_FUNC_CALL(args));		\
	}							\
}								\
static inline	\
void _uev_trace_##name##_handler(void * handler, UEV_FUNC_PROTO(args))	\
{									\
	((void(*)(UEV_FUNC_PROTO(args)))handler)(UEV_FUNC_CALL(args));	\
}

#define UEV_FUNC_DEFINE(name, args)	\
void _uev_trace_##name(UEV_FUNC_PROTO(args))			\
{								\
	struct uev_header *head = &uev_head_##name;		\
	struct uev_request_obj *pos = NULL;			\
								\
	(void)raw_rwlock_rdlock(&uev_head_##name.rwlock);		\
	dlist_for_each_entry(pos, &head->list, struct uev_request_obj, list) { \
		if (uev_request_enabled(pos)) {			\
			_uev_trace_##name##_handler(pos->handler, \
						    UEV_FUNC_CALL(args)); \
		}						\
	}							\
	(void)raw_rwlock_unlock(&uev_head_##name.rwlock);		\
}

#define DECLARE_UEV(NAME, name, args, ...)	\
	UEV_HEAD_DECLARE(name)	\
	UEV_FUNC_DECLARE(name, UEV_ARG(args))

#define DEFINE_UEV(NAME, name, args, ...)	\
	UEV_HEAD_DEFINE(name)	\
	UEV_FUNC_DEFINE(name, UEV_ARG(args))

static inline bool uev_request_enabled(struct uev_request_obj *uobj)
{
	return uobj->enable;
}

static inline bool uev_enabled(struct uev_header *head)
{
	return head->enable;
}

#else	/* !__HM_UEV__ */
#define UEV_HEAD_DECLARE(name)
#define UEV_HEAD_DEFINE(_name)
#define UEV_FUNC_DEFINE(name, args)

#define UEV_FUNC_DECLARE(name, args)	\
static inline int uev_##name##_register(struct uev_request_obj *uev_nobj)	\
{										\
	return E_HM_NOSYS;							\
}										\
static inline int uev_##name##_unregister(struct uev_request_obj *uev_nobj)	\
{										\
	return E_HM_NOSYS;							\
}										\
static inline void uev_trace_##name(UEV_FUNC_PROTO(args))			\
{}

#define DECLARE_UEV(NAME, name, args, ...)	\
	UEV_FUNC_DECLARE(name, UEV_ARG(args))
#define DEFINE_UEV(NAME, name, args, ...)

#endif /* __HM_UEV__ */
#endif /* ULIBS_LIBHMUEV_UEV_TEMPLATE_H */
