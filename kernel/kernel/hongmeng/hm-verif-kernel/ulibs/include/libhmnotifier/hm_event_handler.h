/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: hm event handler interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 12 17:19:14 2022
 */
#ifndef ULIBS_LIBHMNOTIFY_HM_EVENT_HANDLER_H
#define ULIBS_LIBHMNOTIFY_HM_EVENT_HANDLER_H

#include <libhmnotifier/hm_notifier.h>

typedef int (*handler_func)(void *args);

struct hm_event_handler_info {
	rref_t notifier_rref;
	unsigned int register_method_id;
	unsigned int unregister_method_id;
	unsigned int event;
	unsigned int priority;
	handler_func handler;
};

struct hm_event_handlers {
	char svpname[SVP_NAME_MAX];
	unsigned int notifier_method_id;
	struct raw_mutex lock;
	struct dlist_node handlers;
};

extern int hm_event_handlers_init(struct hm_event_handlers *handler,
				  unsigned int notifier_method_id,
				  const char *svpname);
extern int hm_event_add_handler(struct hm_event_handlers *handlers,
				const struct hm_event_handler_info *info);
extern void hm_event_remove_handler(struct hm_event_handlers *handlers,
				    unsigned int event);
extern int hm_event_handler_dispatch(struct hm_event_handlers *handlers, void *args,
				     unsigned long long sender);


#endif
