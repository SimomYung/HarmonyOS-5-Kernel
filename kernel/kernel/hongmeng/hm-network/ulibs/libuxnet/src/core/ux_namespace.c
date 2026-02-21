/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implements for net namespace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 7 10:31:47 2023
 */

#include <libhmsync/raw_mutex.h>
#include <libsec/cred.h>

#include "ux_socket.h"
#include "ux_namespace.h"

static struct dlist_node namespace_ops = DLIST_HEAD_INIT(namespace_ops);

#define DEFINE_NET_NAMESPACE_OPS(func, ...)	\
static int do_each_net_##func(ux_namespace_t *ns)	\
{	\
	ux_namespace_ops_t *ops = NULL;		\
	ux_namespace_ops_t *next = NULL;	\
	int ret = E_HM_OK;				\
									\
	dlist_for_each_entry_safe(ops, next, &namespace_ops, ux_namespace_ops_t, node) {	\
		if (ops->func != NULL) {			\
			ret = ops->func(ns);			\
		}									\
											\
		if (ret != E_HM_OK) {				\
			break;							\
		}									\
	}										\
											\
	return ret;								\
}

DEFINE_NET_NAMESPACE_OPS(ns_create)
DEFINE_NET_NAMESPACE_OPS(ns_destroy)

static ux_namespace_t *namespaces[UX_MAX_NAMESPACE_NR];
static struct raw_mutex namespace_lock;

static ux_namespace_t init_ns;

static void lock_namespace(void)
{
	raw_mutex_lock(&namespace_lock);
}

static void unlock_namespace(void)
{
	raw_mutex_unlock(&namespace_lock);
}

int ux_namespace_init(void)
{
	raw_mutex_init(&namespace_lock);

	init_ns.nsid = UX_ROOT_NS_ID;
	init_ns.destroy = NULL;
	(void)raw_atomic_int_set(&init_ns.refcnt, 1);

	/**
	 * @init_ns will never be destroy
	 */
	ux_namespace_get(&init_ns);
	namespaces[UX_ROOT_NS_ID] = &init_ns;

#ifndef CONFIG_NETFS_FUSION
	return do_each_net_ns_create(&init_ns);
#else
	return E_HM_OK;
#endif
}

static bool is_nsid_valid(unsigned int nsid)
{
	return nsid < UX_MAX_NAMESPACE_NR;
}

static bool is_nsid_root_ns(unsigned int nsid)
{
	return nsid == UX_ROOT_NS_ID;
}

static void __ux_namespace_destroy_real(struct ux_namespace *ns)
{
	unsigned int nsid;

	if (ns != NULL) {
		nsid = ns->nsid;
		free(ns);
		if (is_nsid_valid(nsid) && nsid != UX_ROOT_NS_ID) {
			namespaces[nsid] = NULL;
		}
	}
}

int ux_new_namespace(uint32_t cnode, unsigned int nsid)
{
	ux_sock_owner_t *owner = NULL;
	ux_namespace_t *ns = NULL;
	int rc = E_HM_OK;

	if (!is_nsid_valid(nsid)) {
		return E_HM_INVAL;
	}

	owner = ux_sock_owner_get(cnode, 0);
	if (owner == NULL) {
		owner = ux_sock_owner_get(cnode, 1);
		if (owner == NULL) {
			return E_HM_INVAL;
		}
	}

	lock_namespace();

	if (namespaces[nsid] != NULL) {
		rc = E_HM_EXIST;
	} else {
		ns = calloc(1, sizeof(ux_namespace_t));
		if (ns == NULL) {
			rc = E_HM_NOMEM;
		} else {
			(void)raw_atomic_int_set(&ns->refcnt, 1);
			ns->destroy = __ux_namespace_destroy_real;
			ns->nsid = nsid;
		}
	}

#ifndef CONFIG_NETFS_FUSION
	if (rc == E_HM_OK) {
		rc = do_each_net_ns_create(ns);
	}
#endif

	if (rc == E_HM_OK) {
		namespaces[nsid] = ns;
		if (owner->ns != NULL) {
			ux_namespace_put(owner->ns);
		}

		owner->ns = ns;
		ux_namespace_get(ns);
	}

	unlock_namespace();

	if (rc != E_HM_OK) {
		if (ns != NULL) {
			ux_namespace_put(ns);
		}
	}

	ux_sock_owner_put(owner);

	return rc;
}

int ux_destroy_namespace(unsigned int nsid)
{
	int rc = E_HM_OK;
	ux_namespace_t *ns = NULL;

	if (!is_nsid_valid(nsid)) {
		rc = E_HM_INVAL;
	}

	if (is_nsid_root_ns(nsid)) {
		// init_ns never to be destroyed
		if (rc == E_HM_OK) {
			net_error(HMNET_CORE, "trying to destroy root ns, seems to be crash!\n");
			rc = E_HM_INVAL;
		}
	} else {
		lock_namespace();

		if (rc == E_HM_OK) {
			if (namespaces[nsid] == NULL) {
				rc = E_HM_INVAL;
			} else {
				ns = namespaces[nsid];
				namespaces[nsid] = NULL;
			}
		}

		if (rc == E_HM_OK) {
			rc = do_each_net_ns_destroy(ns);
			ux_namespace_put(ns);
		}

		unlock_namespace();
	}

	return rc;
}

void ux_register_namespace_ops(ux_namespace_ops_t *ops)
{
	dlist_init(&ops->node);
	dlist_insert(&namespace_ops, &ops->node);
}

void ux_unregister_namespace_ops(ux_namespace_ops_t *ops)
{
	dlist_delete(&ops->node);
}

int ux_namespace_get_ns_by_nsid(unsigned int nsid, ux_namespace_t **ns)
{
	if (!is_nsid_valid(nsid)) {
		return E_HM_INVAL;
	}

	if (is_nsid_root_ns(nsid)) {
		// init_ns never be destroy, get without lock for fastpath
		ux_namespace_get(&init_ns);
		*ns = &init_ns;
	} else {
		lock_namespace();
		*ns = namespaces[nsid];
		ux_namespace_get(*ns);
		unlock_namespace();
	}

	return E_HM_OK;
}

int netns_id_of_current(unsigned int *nsid)
{
	int ret;
	struct sec_chk_cred cred = SEC_CHK_CRED_RESET;

	ret = sec_chk_cred_init_requester_simple(&cred);
	if (ret == E_HM_OK) {
		*nsid = netns_id_of_sec_cred(&cred);
	}

	return ret;
}
