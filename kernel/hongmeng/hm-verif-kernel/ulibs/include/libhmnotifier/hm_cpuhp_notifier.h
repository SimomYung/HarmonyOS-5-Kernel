/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: cpu hotplug notifier interface
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 20 10:38:47 2021
 */
#ifndef ULIBS_LIBHMNOTIFY_HM_LISTENER_H
#define ULIBS_LIBHMNOTIFY_HM_LISTENER_H

#include <libhmnotifier/hm_notifier.h>
#include <libhmnotifier/hm_event_handler.h>

enum cpuhp_event {
	CPU_ADD,
	CPU_ADD_FAILED,
	CPU_REMOVE,
	CPU_REMOVE_FAILED,
};

struct hm_cpuhp_notifier_args {
	struct hm_notifier_event e;
	unsigned int cpu;
	unsigned int event;
}__attribute((packed));

int cpuhp_add_event_handler(struct hm_event_handlers *handlers,
			    unsigned int priority,
			    handler_func func);
void cpuhp_remove_event_handler(struct hm_event_handlers *handlers);
/* The following help func is deprecated, and will be removed in the future */
int cpuhp_notifier_create_listener_helper(struct hm_notifier_cb *ops);
int cpuhp_notifier_remove_listener_helper(struct hm_notifier_cb *ops);

int hm_cpuhp_notifier_cpu_add(struct hm_notifier *notifier, unsigned int cpu);
int hm_cpuhp_notifier_cpu_remove(struct hm_notifier *notifier, unsigned int cpu);

#endif
