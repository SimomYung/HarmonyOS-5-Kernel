/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: hm notifier interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 19 11:00:02 2021
 */
#ifndef ULIBS_LIBHMNOTIFY_HM_NOTIFIER_H
#define ULIBS_LIBHMNOTIFY_HM_NOTIFIER_H

#include <lib/dlist.h>
#include <libhmsync/raw_mutex.h>
#include <hmkernel/capability.h>
#include <stddef.h>

#define SVP_NAME_MAX 32

struct hm_notifier_cb {
	char svpname[SVP_NAME_MAX];
	unsigned int notifier_method_id;
	unsigned int priority;
	unsigned int event;
};

struct hm_listener {
	struct dlist_node node;
	rref_t rref;
	struct hm_notifier_cb cb;
	unsigned int caller_cnode_idx;
};

struct hm_notifier {
	struct raw_mutex lock;
	struct dlist_node listeners;
};

#define	EVENT_CPU_HOTPLUG	(1U)
struct hm_notifier_event {
	unsigned int event;
};

/* the caller should hold hm_notifier.lock */
int hm_notify_listener(const struct hm_listener *listener,
		       void *args, size_t size);
int hm_notifier_add_listener(struct hm_notifier *notifier,
			     const struct hm_notifier_cb *ops,
			     unsigned int caller_cnode_idx);
int hm_notifier_remove_listener(struct hm_notifier *notifier,
				const char *svpname, unsigned int caller_cnode_idx);
int hm_notifier_remove_listener_by_cnode_idx(struct hm_notifier *notifier,
					     unsigned int cnode_idx);
void hm_notifier_init(struct hm_notifier *notifier);

#define iterate_listeners(listener, notifier)		\
	dlist_for_each_entry(listener, &(notifier)->listeners, struct hm_listener, node)

#define iterate_listeners_safe(listener, next, notifier)		\
	dlist_for_each_entry_safe(listener, next, &(notifier)->listeners, struct hm_listener, node)

#define iterate_listeners_reverse(listener, notifier)	\
	dlist_for_each_entry_reverse(listener, &(notifier)->listeners, struct hm_listener, node)

#define iterate_listeners_continue(listener, notifier)	\
	dlist_for_each_entry_continue(listener, &(notifier)->listeners, struct hm_listener, node)

#define iterate_listeners_reverse_continue(listener, notifier)	\
	dlist_for_each_entry_reverse_continue(listener, &(notifier)->listeners, struct hm_listener, node)

#endif
