/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:  Header file for network namespace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb  7 10:31:47 2023
 */

#ifndef __LIBUXNET_UX_NAMESPACE_
#define __LIBUXNET_UX_NAMESPACE_

#include <lib/dlist.h>
#include <libhmsync/atomic.h>

#define UX_ROOT_NS_ID		8189
#define UX_MAX_NAMESPACE_NR	8192

#ifndef CLONE_NEWNET
#define CLONE_NEWNET	(0x40000000)
#endif

typedef struct ux_namespace {
	raw_atomic_int_t refcnt;
	unsigned int nsid;
	void (*destroy)(struct ux_namespace *ns);
} ux_namespace_t;

typedef struct ux_namespace_ops {
	struct dlist_node node;

	int (*ns_create)(ux_namespace_t *ns);
	int (*ns_destroy)(ux_namespace_t *ns);
} ux_namespace_ops_t;

int ux_new_namespace(uint32_t cnode, unsigned int nsid);
int ux_destroy_namespace(unsigned int nsid);

#ifdef CONFIG_NET_NS
int ux_namespace_init(void);

static inline void ux_namespace_get(ux_namespace_t *ns)
{
	if (ns != NULL) {
		(void)raw_atomic_int_inc(&ns->refcnt);
	}
}

static inline void ux_namespace_put(ux_namespace_t *ns)
{
	if (ns != NULL && raw_atomic_int_dec(&ns->refcnt) == 0) {
		if (ns->destroy != NULL) {
			ns->destroy(ns);
		}
	}
}

int ux_namespace_get_ns_by_nsid(unsigned int nsid, ux_namespace_t **ns);
int netns_id_of_current(unsigned int *nsid);
#else
static inline int ux_namespace_init(void)
{
	return E_HM_OK;
}

static inline int ux_namespace_get_ns_by_nsid(unsigned int nsid, ux_namespace_t **ns)
{
	UNUSED(nsid);
	*ns = NULL;
	return E_HM_OK;
}

static inline void ux_namespace_get(ux_namespace_t *ns)
{
	UNUSED(ns);
	return;
}

static inline void ux_namespace_put(ux_namespace_t *ns)
{
	UNUSED(ns);
	return;
}

static inline int netns_id_of_current(unsigned int *nsid)
{
	UNUSED(nsid);
	return E_HM_INVAL;
}
#endif

void ux_register_namespace_ops(ux_namespace_ops_t *ops);
void ux_unregister_namespace_ops(ux_namespace_ops_t *ops);
#endif
