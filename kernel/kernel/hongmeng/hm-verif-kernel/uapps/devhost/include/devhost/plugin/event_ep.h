/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Struct and macro for event extension point
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 15:12:10 2020
 */
#ifndef DEVHOST_PLUGIN_EVENT_EP_H
#define DEVHOST_PLUGIN_EVENT_EP_H

#include <devhost/plugin/base.h>

/* event extension point */
struct event_extension_point {
	struct extension_point base;
	struct dlist_node before_event_list;
	struct dlist_node after_event_list;
};

struct event_node {
	struct dlist_node dnode;
	void (*fn)(void /* ... */);
	struct event_extension_point *ep;
	struct libdh_extension *ext;
};

struct event_extension_info {
	void (*before)(void /* ... */);
	void (*after)(void /* ... */);
	struct event_node *before_inst;
	struct event_node *after_inst;
};

int __event_ep_init(struct extension_point *_ep);
int __event_ep_attach(struct extension_point *_ep, struct libdh_extension *_ext);
void __event_ep_detach(struct extension_point *_ep, struct libdh_extension *_ext);

#define __EVENT_EP_CALLCHAIN(_ep, status, default_fn, ...) do {	\
	struct event_extension_point *ep = (_ep);		\
	struct event_node *event = NULL;			\
	struct dlist_node *pos = NULL;				\
	/* call before event handlers */			\
	dh_ep_rdlock(&ep->base);				\
	dlist_for_each(pos, &ep->before_event_list) {		\
		event = dlist_entry(pos, struct event_node, dnode);		\
		__ARGS_CALL_FUNC_PTR(event->fn, void, ##__VA_ARGS__);		\
	}							\
	/* call default_fn if not NULL */			\
	status = E_HM_OK;					\
	if (default_fn != NULL) {				\
		status = __ARGS_CALL_FUNC_PTR(default_fn, int, ##__VA_ARGS__);	\
	}							\
	if (status < 0)	{					\
		/* don't trigger after-event */			\
		dh_ep_unlock(&ep->base);			\
		break;						\
	}							\
	/* call after event handlers */				\
	dlist_for_each(pos, &ep->after_event_list) {		\
		event = dlist_entry(pos, struct event_node, dnode);		\
		__ARGS_CALL_FUNC_PTR(event->fn, void, ##__VA_ARGS__);		\
	}							\
	dh_ep_unlock(&ep->base);				\
} while (0)

#define DECLARE_EVENT_EP(name, status, fn, ...) do {		\
	/* declare base extension point */			\
	__DECLARE_EXTENSION_POINT(name,				\
				  struct event_extension_point, base,	\
				  __event_ep_init, NULL,	\
				  __event_ep_attach, __event_ep_detach,	\
				  fn, DH_EP_TYPE_EVENT, 0,	\
				  __SUBCLASS_MEMBER_SET(before_event_list, DLIST_HEAD_INIT(__ep_##name.before_event_list)),	\
				  __SUBCLASS_MEMBER_SET(after_event_list, DLIST_HEAD_INIT(__ep_##name.after_event_list))	\
				  );					\
	/* call event extension point callchain */		\
	__EVENT_EP_CALLCHAIN(&__ep_##name, status, fn, ##__VA_ARGS__);	\
} while (0)

#endif /* DEVHOST_PLUGIN_EVENT_EP_H */
