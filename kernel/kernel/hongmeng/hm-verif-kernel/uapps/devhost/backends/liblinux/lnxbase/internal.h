/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost backend APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 4 09:40:43 2021
 */
#ifndef LNXBASE_INTERNAL_H
#define LNXBASE_INTERNAL_H

#include <lib/dlist.h>
#include <devhost/backend.h>
#include <lnxbase/lnxbase.h>

struct extension_node {
	struct dlist_node node;
	const char *extralib_name;
	void *extralib_sohandle;
};

/* lnxbase context */
struct lnxbase_ctx {
	struct dh_backend_ctor *ctor;
	unsigned long features;

	/* module related */
	struct devhost_loader mod_loader;
	struct lnxbase_module_ops *mod_ops;
	struct dlist_node extensions;
	int mod_loaded;

	/* postinit */
	lnxbase_postinit_fn_t postinit_fn;
	void *postinit_args;

	/* optional: post root notifier */
	lnxbase_post_populate_fn_t post_dev_populate_fn;

	void (*time_sync_hdlr)(void);
};

/* lnxbase feature bits */
#define LNXBASE_FEATURE_MODULE		(1U << 1)
#define LNXBASE_FEATURE_NETLINK		(1U << 2)
#define LNXBASE_FEATURE_TIMESYNC	(1U << 3)
#define LNXBASE_FEATURE_BACKTRACE	(1U << 4)
#define LNXBASE_FEATURE_DEV_AUTOPROBE	(1U << 5)
#define LNXBASE_FEATURE_BOOTFDT		(1U << 6)
#define LNXBASE_FEATURE_SKIP_FIRST_TIMESYNC	(1U << 7)

/* cache */
int lnxbase_cache_init(void);
void lnxbase_flush_cache_range(unsigned int cmd, unsigned long start, unsigned long end);
void lnxbase_flush_pgtbl_cache(unsigned int pid, unsigned long vstart, unsigned long vend);

int lnxbase_args_init(void);

/* page pool */
int lnxbase_page_pool_init(unsigned int page_sizeorder);
int lnxbase_page_pool_watermark_reached(void);

/* module */
#define MODULE_RSVD_ANON_NAME	"[liblinux/MODULE-RSVD]"
#define MODULE_ANON_NAME_FMT	"[liblinux/%s.ko]"
#define MODULE_NONAME		"[liblinux/<module>]"

int lnxbase_load_module_chk(const struct lnxbase_ctx *ctx, const char *modname,
			    const char *chksym);
static inline
int lnxbase_load_module(const struct lnxbase_ctx *ctx, const char *modname)
{
	return lnxbase_load_module_chk(ctx, modname, NULL);
}

/* vmallocinfo */
int lnxbase_vmallocinfo_init(void);
bool lnxbase_vmallocinfo_enabled(void);

/* highmem */
bool lnxbase_is_vmemmap(const void *start, unsigned long size);
bool lnxbase_is_vmemmap_populate_all(const void *start, unsigned long size);

struct vmap_node *vmemmap_get_vnode(void *start, unsigned long size, uint32_t prot);

enum vmemmap_ops_type {
	VMEMMAP_OPS_INIT,
	VMEMMAP_OPS_MAP,
	VMEMMAP_OPS_DONE
};

struct vmemmap_ops {
	int (*map_init)(void *priv);
	int (*map)(void *priv, void *addr, unsigned long size);
	int (*map_done)(void *priv);
	unsigned int flags;
};

struct vmemmap_init_ops {
	struct vmemmap_ops *map_ops;
	unsigned int ops_num;
	unsigned int ops_flags;
};

int lnxbase_vmemmap_init(void *vmemmap_start, unsigned long page_size,
			 struct vmemmap_init_ops *ops, void *priv);

#define max(x, y) ({				\
	typeof(x) ____x = (x);			\
	typeof(y) ____y = (y);			\
	(____x > ____y) ? ____x : ____y;	\
})

#define min(x, y) ({				\
	typeof(x) ____x = (x);			\
	typeof(y) ____y = (y);			\
	(____x < ____y) ? ____x : ____y;	\
})

#endif	/* LNXBASE_INTERNAL_H */
