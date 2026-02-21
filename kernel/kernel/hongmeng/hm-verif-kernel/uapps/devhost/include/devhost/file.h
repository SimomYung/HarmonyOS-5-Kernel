/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for file table management
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 14:17:03 2019
 */

#ifndef __DEVHOST_API_FILE_H__
#define __DEVHOST_API_FILE_H__

#include <fcntl.h>
#include <lib/dlist.h>
#include <lib/utils.h>
#include <devhost/device.h>
#ifdef CONFIG_SEHARMONY
#include <devhost/security_seharmony.h>
#endif
#include <hmkernel/capability.h>
#include <libhmsync/raw_mutex.h>
#include <librb/shm_ring_buffer.h>
#include <libhmsync/raw_scoped_coptr.h>
#include <libhwsecurec/securec.h>
#include <libsysif/utils.h>

struct block_ctx {
	uint64_t rref_notify;
	uint64_t service_id;
	unsigned int method;
	struct sring_buf *end_srbuf;
};

struct devhost_poll_list {
	struct dlist_node node;
	struct raw_mutex lock;
};

struct hmkobj_ctx {
	void *kobj;
	struct raw_refcnt refcnt;
};

struct filp_node {
	__u32 cnode_idx;
	struct raw_compounded_auto_refcnt refcnt;
	struct device_info *devinfo;
	void *filp;
	struct raw_mutex ctx_lock;
	union {
		void *private_ctx;
		struct block_ctx *blk_ctx; /* `devinfo->vfs_mode & MODE_BLK` */
	};
	struct devhost_poll_list wait_head;  /* for ppoll */
	struct devhost_poll_list ep_list;    /* for epoll */
	unsigned int f_flags;
	unsigned int f_modes;
	int64_t pos;
	void *hmkobj; /* for ctrlmem kobj with traditional way */
	struct hmkobj_ctx *kobj_ctx; /* for ctrlmem kobj */
#ifdef CONFIG_SEHARMONY
	struct dh_file_sec security;
#endif
};

static inline struct filp_node *refcnt_to_filp_node(struct raw_compounded_auto_refcnt *refcnt)
{
	return container_of(refcnt, struct filp_node, refcnt);
}

static inline void devhost_put_file_rpccnt(struct filp_node *node)
{
	if (node == NULL) {
		return;
	}
	NOFAIL(raw_compounded_auto_rpccnt_put(&node->refcnt));
}

static inline void devhost_put_file(struct filp_node *node)
{
	if (node == NULL) {
		return;
	}
	(void)raw_compounded_auto_refcnt_put(&node->refcnt);
}

static inline int devhost_get_file(struct filp_node *node)
{
	if (node == NULL) {
		return E_HM_INVAL;
	}
	return raw_compounded_auto_refcnt_get(&node->refcnt);
}

static inline void devhost_put_kobj(struct hmkobj_ctx *kobj_ctx)
{
	if (kobj_ctx == NULL) {
		return;
	}
	(void)raw_refcnt_put(&kobj_ctx->refcnt);
}

static inline void devhost_get_kobj(struct hmkobj_ctx *kobj_ctx)
{
	if (kobj_ctx == NULL) {
		return;
	}
	(void)raw_refcnt_get(&kobj_ctx->refcnt);
}

int devhost_register_file(struct filp_node *node, unsigned int *file_id);
int devhost_unregister_file(uint64_t cnode_idx, unsigned int file_id);
struct filp_node *devhost_alloc_filp_node(struct device_info *devinfo,
					  __u32 cnode_idx);
struct filp_node *devhost_alloc_filp_node_fast(struct device_info *devinfo);

/* The function is called by 'acquire_devhost_filp_node'.
 * The function name is defined according to a certain rules which
 * can be known in 'acquire_scopedptr'.
*/
struct filp_node *acquire_filp_node_ptr_coref(__u32 cnode_idx,
					      unsigned int fastpath,
					      unsigned long long file_id);
declare_scoped_coptr_type(filp_node);
define_scoped_coptr_release(filp_node, NULL)
#define acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id) \
	acquire_scoped_coptr(filp_node, node, \
			     offset_of(struct filp_node, refcnt), \
			     cnode_idx, fastpath, file_id)

static inline void filp_poll_list_init(struct filp_node *node)
{
	dlist_init(&node->wait_head.node);
	raw_mutex_init(&node->wait_head.lock);

	dlist_init(&node->ep_list.node);
	raw_mutex_init(&node->ep_list.lock);
}

static inline struct dlist_node *wait_head_of_filp(struct filp_node *node)
{
	return &node->wait_head.node;
}

static inline void filp_poll_list_lock(struct filp_node *node)
{
	raw_mutex_lock(&node->wait_head.lock);
}

static inline void filp_poll_list_unlock(struct filp_node *node)
{
	raw_mutex_unlock(&node->wait_head.lock);
}

static inline struct dlist_node *epoll_list_of_filp(struct filp_node *node)
{
	return &node->ep_list.node;
}

static inline void filp_epoll_list_lock(struct filp_node *node)
{
	raw_mutex_lock(&node->ep_list.lock);
}

static inline void filp_epoll_list_unlock(struct filp_node *node)
{
	raw_mutex_unlock(&node->ep_list.lock);
}

#define __O_ACCMODE		(O_RDONLY | O_WRONLY | O_RDWR)
#define get_accmode(flag)	((flag) & __O_ACCMODE)
#define trans_acc_mode(flag)	get_accmode((flag) + 1)

#define ubit(n)		(1U << (n))
#define MODE_READ	1
#define MODE_WRITE	ubit(1)

DECLARE_UPCALL_ACTVHDLR(handle_rpccnt_put);
#endif /* __DEVHOST_API_FILE_H__ */
