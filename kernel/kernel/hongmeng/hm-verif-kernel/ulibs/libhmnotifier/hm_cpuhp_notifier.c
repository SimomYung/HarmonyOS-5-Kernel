/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: cpu hotplug notifier
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 20 10:38:47 2021
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libsysif/pwrmgr/api.h>
#include <libhmnotifier/hm_cpuhp_notifier.h>

#include "../libhmpm/pm_sysif.h"

int cpuhp_add_event_handler(struct hm_event_handlers *handlers,
			    unsigned int priority,
			    handler_func func)
{
	int err;
	rref_t rref;
	struct hm_event_handler_info info;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	info.notifier_rref = rref;
	info.register_method_id = __pwrmgr_method_cpuhp_add_notifier;
	info.unregister_method_id = __pwrmgr_method_cpuhp_remove_notifier;
	info.event = EVENT_CPU_HOTPLUG;
	info.priority = priority;
	info.handler = func;

	return hm_event_add_handler(handlers, &info);
}

void cpuhp_remove_event_handler(struct hm_event_handlers *handlers)
{
	hm_event_remove_handler(handlers, EVENT_CPU_HOTPLUG);
}

int cpuhp_notifier_create_listener_helper(struct hm_notifier_cb *ops)
{
	int err;
	rref_t rref;

	if (ops == NULL) {
		hm_error("notifier create listener helper is invalid\n");
		return E_HM_INVAL;
	}
	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	ops->event = EVENT_CPU_HOTPLUG;
	err = actvcapcall_pmcall_cpuhp_add_notifier(rref, ops, sizeof(*ops));
	if (err != E_HM_OK) {
		hm_error("cpuhotplug add notifier failed: %s\n",
			 hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int cpuhp_notifier_remove_listener_helper(struct hm_notifier_cb *ops)
{
	int err;
	rref_t rref;

	if (ops == NULL) {
		hm_error("notifier remove listener helper is invalid\n");
		return E_HM_INVAL;
	}

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	ops->event = EVENT_CPU_HOTPLUG;
	err = actvcapcall_pmcall_cpuhp_remove_notifier(rref, ops, sizeof(*ops));
	if (err != E_HM_OK) {
		hm_error("cpuhotplug remove notifier failed: %s\n",
			 hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}


static int cpuhp_notifier_handle(const struct hm_listener *listener,
				 struct hm_cpuhp_notifier_args *args)
{
	int rc;

	hm_info("cpuhp: listener %s handling cpu%u event: %u enter\n",
		listener->cb.svpname, args->cpu, args->event);
	rc = hm_notify_listener(listener, args, sizeof(*args));
	hm_info("cpuhp: listener %s handling cpu%u event: %u return: %s\n",
		listener->cb.svpname, args->cpu, args->event, hmstrerror(rc));

	return rc;
}

/* notify low priority first */
int hm_cpuhp_notifier_cpu_add(struct hm_notifier *notifier, unsigned int cpu)
{
	int rc = E_HM_OK;
	int err;
	struct hm_cpuhp_notifier_args args;
	struct hm_listener *listener;

	args.e.event = EVENT_CPU_HOTPLUG;
	args.cpu = cpu;
	args.event = CPU_ADD;

	if (notifier == NULL) {
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &notifier->lock);
	iterate_listeners_reverse(listener, notifier) {
		rc = cpuhp_notifier_handle(listener, &args);
		if (rc != E_HM_OK) {
			break;
		}
	}

	if (rc != E_HM_OK) {
		args.event = CPU_ADD_FAILED;
		iterate_listeners_continue(listener, notifier) {
			err = hm_notify_listener(listener, &args, sizeof(args));
			if (err != E_HM_OK) {
				hm_warn("cpuhp: notifier listener rollback failed: %s\n",
					hmstrerror(err));
			}
		}
	}

	return rc;
}

/* notify high priority first */
int hm_cpuhp_notifier_cpu_remove(struct hm_notifier *notifier, unsigned int cpu)
{
	int rc = E_HM_OK;
	int err;
	struct hm_cpuhp_notifier_args args;
	struct hm_listener *listener;

	args.e.event = EVENT_CPU_HOTPLUG;
	args.cpu = cpu;
	args.event = CPU_REMOVE;

	if (notifier == NULL) {
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &notifier->lock);
	iterate_listeners(listener, notifier) {
		rc = cpuhp_notifier_handle(listener, &args);
		if (rc != E_HM_OK) {
			break;
		}
	}

	if (rc != E_HM_OK) {
		args.event = CPU_REMOVE_FAILED;
		iterate_listeners_reverse_continue(listener, notifier) {
			err = hm_notify_listener(listener, &args, sizeof(args));
			if (err != E_HM_OK) {
				hm_warn("cpuhp: notifier listener rollback failed: %s\n",
					hmstrerror(err));
			}
		}
	}

	return rc;
}
