/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: hm notifier module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 19 11:00:02 2021
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/pwrmgr/api.h>
#include <libhmsrv_sys/hm_path.h>

#include <libhmnotifier/hm_notifier.h>

int hm_notify_listener(const struct hm_listener *listener,
		       void *args, size_t size)
{
	int rc = E_HM_OK;
	unsigned long long rpc_info;

	if (listener == NULL || args == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rpc_info = __RPC_INFO_ENCODE_XACT(false, false, listener->cb.notifier_method_id, size, 0);
		do {
			rc = syscap_ActivationPoolSendBufCall(listener->rref, rpc_info, args);
		} while (rc == E_HM_ACTIVATION_RPC_RETRY);
	}

	return rc;
}

static struct hm_listener *
hm_notifier_find_listener(const struct hm_notifier *notifier,
			  const char *svpname)
{
	struct hm_listener *listener;

	iterate_listeners(listener, notifier) {
		if (strcmp(listener->cb.svpname, svpname) == 0) {
			return listener;
		}
	}

	return NULL;
}

static void hm_notifier_insert_listener(struct hm_notifier *notifier,
					struct hm_listener *listener)
{
	struct hm_listener *next_listener = NULL;
	struct dlist_node *prev_node = NULL;

	iterate_listeners(next_listener, notifier) {
		if (listener->cb.priority > next_listener->cb.priority) {
			break;
		}
	}

	/*
	 * If the list is empty, or listener's priority is lowest,
	 * &next_listener->node is &notifier->listeners.
	 */
	prev_node = dlist_get_prev(&next_listener->node);
	dlist_insert(prev_node, &listener->node);
}

static void hm_free_listener(struct hm_listener *listener)
{
	(void)hm_path_release(listener->cb.svpname, listener->rref);
	free(listener);
}

static int verify_caller_cnode_idx(rref_t rref, unsigned int caller_cnode_idx)
{
	struct sysarg_cnode_statrref stat;
	int ret;

	ret = sysfast_cnode_stat_rref(&stat, rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (stat.src_cnode_idx != caller_cnode_idx) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hm_notifier_add_listener(struct hm_notifier *notifier,
			     const struct hm_notifier_cb *ops,
			     unsigned int caller_cnode_idx)
{
	struct hm_listener *listener = NULL;
	struct hm_listener *tmp_listener = NULL;
	int rc;
	rref_t rref;
	size_t len;

	if (ops == NULL || notifier == NULL) {
		return E_HM_INVAL;
	}

	len = strnlen(ops->svpname, SVP_NAME_MAX);
	if (len == 0 || len == SVP_NAME_MAX) {
		return E_HM_INVAL;
	}

	rc = hm_path_acquire(ops->svpname, &rref);
	if (rc != E_HM_OK) {
		return rc;
	}

	rc = verify_caller_cnode_idx(rref, caller_cnode_idx);
	if (rc != E_HM_OK) {
		(void)hm_path_release(ops->svpname, rref);
		return rc;
	}

	listener = malloc(sizeof(*listener));
	if (listener == NULL) {
		(void)hm_path_release(ops->svpname, rref);
		return E_HM_NOMEM;
	}

	listener->rref = rref;
	listener->cb = *ops;
	listener->caller_cnode_idx = caller_cnode_idx;

	RAW_MUTEX_GUARD(_, &notifier->lock);
	tmp_listener = hm_notifier_find_listener(notifier, ops->svpname);
	if (tmp_listener != NULL) {
		if (tmp_listener->caller_cnode_idx == caller_cnode_idx) {
			hm_free_listener(listener);
			return E_HM_POSIX_EXIST;
		} else {
			/* old listener is dead */
			dlist_delete(&tmp_listener->node);
			hm_free_listener(tmp_listener);
		}
	}

	hm_notifier_insert_listener(notifier, listener);

	return E_HM_OK;
}

int hm_notifier_remove_listener(struct hm_notifier *notifier,
				const char *svpname, unsigned int caller_cnode_idx)
{
	size_t len;
	struct hm_listener *listener;

	if (svpname == NULL || notifier == NULL) {
		return E_HM_INVAL;
	}

	len = strnlen(svpname, SVP_NAME_MAX);
	if (len == 0 || len == SVP_NAME_MAX) {
		return E_HM_INVAL;
	}

	RAW_MUTEX_GUARD(_, &notifier->lock);
	listener = hm_notifier_find_listener(notifier, svpname);
	if (listener == NULL) {
		return E_HM_OK;
	}

	if (listener->caller_cnode_idx != caller_cnode_idx) {
		return E_HM_INVAL;
	}

	dlist_delete(&listener->node);
	hm_free_listener(listener);

	return E_HM_OK;
}

int hm_notifier_remove_listener_by_cnode_idx(struct hm_notifier *notifier,
					     unsigned int cnode_idx)
{
	int ret = E_HM_OK;
	struct hm_listener *next = NULL;
	struct hm_listener *listener = NULL;

	if (notifier == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		RAW_MUTEX_GUARD(_, &notifier->lock);
		iterate_listeners_safe(listener, next, notifier) {
			if (listener->caller_cnode_idx == cnode_idx) {
				dlist_delete(&listener->node);
				hm_free_listener(listener);
			}
		}
	}

	return ret;
}

void hm_notifier_init(struct hm_notifier *notifier)
{
	if (notifier != NULL) {
		dlist_init(&notifier->listeners);
		raw_mutex_init(&notifier->lock);
	}
}
