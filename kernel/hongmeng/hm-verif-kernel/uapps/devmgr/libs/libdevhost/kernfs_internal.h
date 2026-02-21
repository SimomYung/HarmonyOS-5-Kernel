/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Libdh kernfs prototypes and type definitions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 18 17:11:57 2020
 */
#ifndef ULIBS_LIBDEVHOST_KERNFS_INTERNAL_H
#define ULIBS_LIBDEVHOST_KERNFS_INTERNAL_H

#include <sys/types.h>
#include <libalgo/idr.h>
#include <libalgo/rbtree.h>
#include <libhmsync/raw_rwlock.h>
#include <libdevhost/kernfs.h>
#include <libdevhost/uobject.h>

struct kernfs_entry {
	struct uobject uobj;
	union {
		struct libdh_kernfs_ops *ops;
		char *pathname;
	};
	void (*release)(struct kernfs_entry *entry);
};

#define KERNFS_CONTEXT_BASE_MAX	64U

struct libdh_kernfs_context {
	char base[KERNFS_CONTEXT_BASE_MAX];
	struct idr idr;
	struct raw_rwlock mutex;
};

struct libdh_kernfs_notify_node {
	struct rbt_node node; /* node in kernfs_notify_tree */
	uintptr_t ctx; /* kernfs_node_ctx */
};

static inline void libdh_kernfs_get(struct kernfs_entry *entry)
{
	(void)uobject_get(&entry->uobj);
}

static inline void libdh_kernfs_put(struct kernfs_entry *entry)
{
	(void)uobject_put(&entry->uobj);
}

int libdh_kernfs_init(struct libdh_kernfs_context *fs_ctx);

int libdh_kernfs_insert(struct libdh_kernfs_context *fs_ctx,
			struct kernfs_entry *entry, int *tagid_out);

int libdh_kernfs_entry_acquire(struct libdh_kernfs_context *fs_ctx,
				  unsigned int tagid, struct kernfs_entry **entry_out);

void libdh_kernfs_release(struct libdh_kernfs_context *fs_ctx, int tagid);
/* remove entry from idr and put entry */
void libdh_kernfs_remove(struct libdh_kernfs_context *fs_ctx, int tagid);

int libdh_kernfs_read(struct libdh_kernfs_context *fs_ctx, unsigned int tagid,
		      const struct libdh_kernfs_rw_args *read_args,
		      uintptr_t ctx, size_t *rsize /* out */);

int libdh_kernfs_write(struct libdh_kernfs_context *fs_ctx, unsigned int tagid,
		       const struct libdh_kernfs_rw_args *write_args,
		       uintptr_t ctx, size_t *wsize /* out */);

int libdh_kernfs_poll(struct libdh_kernfs_context *fs_ctx, unsigned int tagid,
		      uintptr_t ctx, bool pollable);

void libdh_kernfs_tagid_invalid(struct libdh_kernfs_context *fs_ctx, int tagid);

int libdh_kernfs_notify_insert(uintptr_t kernfs_node_ctx);

void libdh_kernfs_notify_remove(uintptr_t kernfs_node_ctx);

int libdh_kernfs_notify_search(uintptr_t kernfs_node_ctx);

#endif	/* ULIBS_LIBDEVHOST_KERNFS_INTERNAL_H */
