/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: hm event handler module
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 12 17:19:14 2022
 */

#include <libhwsecurec/securec.h>
#include <stdbool.h>

#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex_guard.h>

#include <libhmnotifier/hm_event_handler.h>

struct hm_event_handler {
	struct hm_event_handler_info info;
	struct dlist_node node;
	const struct hm_event_handlers *handlers;
	unsigned int caller_cnode_idx;
};

static struct hm_event_handler *
hm_event_find_handler(struct hm_event_handlers *handlers, unsigned int event)
{
	struct hm_event_handler *handler;

	dlist_for_each_entry(handler, &handlers->handlers, struct hm_event_handler, node) {
		if (handler->info.event != event) {
			continue;
		}
		return handler;
	}

	return NULL;
}

static struct hm_event_handler *
hm_event_new_handler(const struct hm_event_handlers *handlers,
		     const struct hm_event_handler_info *info)
{
	struct hm_event_handler *handler = NULL;

	handler = malloc(sizeof(struct hm_event_handler));
	if (handler == NULL) {
		return NULL;
	}
	mem_zero_s(*handler);

	handler->info = *info;
	handler->handlers = handlers;

	return handler;
}

static int hm_event_send_handler(struct hm_event_handler *handler, bool registered)
{
	int rc;
	unsigned long long rpc_info;
	struct hm_notifier_cb cb;
	unsigned int method_id;
	rref_t rref = handler->info.notifier_rref;

	rc = strcpy_s(cb.svpname, SVP_NAME_MAX, handler->handlers->svpname);
	if (rc != E_HM_OK) {
		return E_HM_INVAL;
	}
	cb.notifier_method_id = handler->handlers->notifier_method_id;
	cb.priority = handler->info.priority;
	cb.event = handler->info.event;

	if (registered) {
		method_id = handler->info.register_method_id;
	} else {
		method_id = handler->info.unregister_method_id;
	}

	rpc_info = __RPC_INFO_ENCODE_XACT(true, true, method_id, sizeof(cb), 0);
	do {
		rc = syscap_ActivationPoolSendBufCall(rref, rpc_info, &cb);
	} while (rc ==  E_HM_ACTIVATION_RPC_RETRY);

	return rc;
}

static int hm_event_register_handler(struct hm_event_handler *handler)
{
	int ret = hm_event_send_handler(handler, true);
	struct sysarg_cnode_statrref stat;
	rref_t rref = handler->info.notifier_rref;
	if (ret == E_HM_OK) {
		ret = sysfast_cnode_stat_rref(&stat, rref, CNODE_STATRREF_SRC_CNODE_IDX);
		if (ret != E_HM_OK) {
			/* rref is invalid, just try unregister it */
			(void)hm_event_send_handler(handler, false);
		}
	}

	if (ret == E_HM_OK) {
		handler->caller_cnode_idx = stat.src_cnode_idx;
	}

	return ret;
}

static void hm_event_unregister_handler(struct hm_event_handler *handler)
{
	int ret;

	ret = hm_event_send_handler(handler, false);
	if (ret != E_HM_OK) {
		hm_warn("unregister event handler failed: %s\n", hmstrerror(ret));
	}
}

static void hm_event_free_handler(struct hm_event_handler *handler)
{
	free(handler);
}

int hm_event_add_handler(struct hm_event_handlers *handlers,
			 const struct hm_event_handler_info *info)
{
	int ret = E_HM_OK;
	struct hm_event_handler *handler = NULL;

	RAW_MUTEX_GUARD(_, &handlers->lock);
	handler = hm_event_find_handler(handlers, info->event);
	if (handler != NULL) {
		ret = E_HM_POSIX_EXIST;
	}

	if (ret == E_HM_OK) {
		handler = hm_event_new_handler(handlers, info);
		if (handler == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		ret = hm_event_register_handler(handler);
		if (ret != E_HM_OK) {
			hm_event_free_handler(handler);
		}
	}

	if (ret == E_HM_OK) {
		dlist_insert_tail(&handlers->handlers, &handler->node);
	}

	return ret;
}

void hm_event_remove_handler(struct hm_event_handlers *handlers,
			     unsigned int event)
{
	struct hm_event_handler *handler = NULL;

	RAW_MUTEX_GUARD(_, &handlers->lock);
	handler = hm_event_find_handler(handlers, event);
	if (handler == NULL) {
		return;
	}

	hm_event_unregister_handler(handler);
	dlist_delete(&handler->node);
	hm_event_free_handler(handler);
}

int hm_event_handler_dispatch(struct hm_event_handlers *handlers, void *args,
			      unsigned long long sender)
{
	struct hm_event_handler *handler;
	int ret = E_HM_OK;
	struct hm_notifier_event *e = args;
	handler_func func = NULL;
	unsigned int caller_cnode_idx;

	RAW_MUTEX_GUARD_START(_, &handlers->lock);
	handler = hm_event_find_handler(handlers, e->event);
	if (handler == NULL) {
		ret = E_HM_NOSYS;
	}

	if (ret == E_HM_OK) {
		/*
		 * we know who will send the event to us, so no security
		 * check here.
		 */
		caller_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
		if (caller_cnode_idx != handler->caller_cnode_idx) {
			ret = E_HM_NOSYS;
		}
	}

	if (ret == E_HM_OK) {
		func = handler->info.handler;
	}

	/*
	 * unlock before calling func, so we can handle more than
	 * one event at the same time.
	 */
	RAW_MUTEX_GUARD_END(_, &handlers->lock);

	if (ret == E_HM_OK) {
		ret = func(args);
	}

	return ret;
}

int hm_event_handlers_init(struct hm_event_handlers *handlers,
			   unsigned int notifier_method_id,
			   const char *svpname)
{
	int ret;

	ret = strcpy_s(handlers->svpname, SVP_NAME_MAX, svpname);
	if (ret != E_HM_OK) {
		return E_HM_INVAL;
	}

	handlers->notifier_method_id = notifier_method_id;
	dlist_init(&handlers->handlers);
	raw_mutex_init(&handlers->lock);

	return E_HM_OK;
}
