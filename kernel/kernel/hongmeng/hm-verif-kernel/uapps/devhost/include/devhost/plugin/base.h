/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Base struct and function of plugin system
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 11:10:51 2020
 */
#ifndef ULIBS_LIBDEVHOST_PLUGIN_BASE_H
#define ULIBS_LIBDEVHOST_PLUGIN_BASE_H

#include <stddef.h>
#include <lib/dlist.h>
#include <hmkernel/compiler.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_scopedptr.h>

#include <devhost/plugin/helper.h>

enum libdh_plugin_ep_type {
	DH_EP_TYPE_INVALID,
	DH_EP_TYPE_SYSIF,
	DH_EP_TYPE_EVENT,
	DH_EP_TYPE_ADVICE,
};

enum libdh_plugin_ext_flag_bits {
	__DH_EXT_FLAG_SELFCALL,
};

#define DH_EXT_FLAG_NONE    		0U
#define DH_EXT_FLAG_SELFCALL		(1U << (__DH_EXT_FLAG_SELFCALL))

#define DH_EP_FLAGS_STATIC	1U

struct libdh_plugin;
/* extension point target, description of one extension */
struct libdh_extension {
	const char *name; /* target extension point name to hook */
	enum libdh_plugin_ep_type type;
	unsigned int ext_flag;
	struct libdh_plugin *(*owner)(void);
	struct raw_mutex ext_lock;
	void *ext_info; /* ptr of 'type' extension info */
};

enum libdh_plugin_type {
	DH_PLUGIN_TYPE_INVALID,
	DH_PLUGIN_TYPE_BUILTIN,
	DH_PLUGIN_TYPE_DYNAMIC,
};

struct extension_point;
/* plugin */
struct libdh_plugin {
	struct dlist_node dnode;
	const char *name;
	enum libdh_plugin_type type;
	struct libdh_extension *extensions;
	struct dlist_node ext_points;
	struct raw_refcnt refcnt;
	int (*init)(struct libdh_plugin *plugin);
	void (*deinit)(struct libdh_plugin *plugin);
};

/* base extension point define, user should not use this */
struct extension_point {
	struct dlist_node dnode;
	const char *name;
	enum libdh_plugin_ep_type type;
	unsigned int flags;
	size_t size; /* subclass size */
	struct libdh_plugin *(*owner)(void);
	struct dlist_node owner_dnode;
	struct raw_refcnt refcnt;
	struct raw_rwlock rwlock;
	/* called when init or deinit */
	int (*init)(struct extension_point *ep);
	void (*deinit)(struct extension_point *ep);
	/* called when new plugin try to attach or detach to this extension point */
	int (*attach)(struct extension_point *ep, struct libdh_extension *ext);
	void (*detach)(struct extension_point *ep, struct libdh_extension *ext);
	/* private data for different extension point type */
	void *_private;
};

#ifndef _DH_PLUGIN_NAME
static __used struct libdh_plugin *__dh_elf_null_plugin__(void)
{
	return NULL;
}
#define __DH_PLUGIN_NAME__	__dh_elf_null_plugin__
#define __DEFINE_PLUGIN(plugin_name, s_type, member,	\
			exts, init_fn, deinit_fn,	\
			plugin_type, ...)
#else
#define __DH_PLUGIN_ADD_PREFIX_(a)	__##a##_plugin__
#define __DH_PLUGIN_ADD_PREFIX(a)	__DH_PLUGIN_ADD_PREFIX_(a)
#define __DH_PLUGIN_NAME__	__DH_PLUGIN_ADD_PREFIX(_DH_PLUGIN_NAME)

struct libdh_plugin *__DH_PLUGIN_NAME__(void);

#define __DEFINE_PLUGIN(plugin_name, s_type, member,	\
			exts, init_fn, deinit_fn,	\
			plugin_type, ...)		\
	static __used					\
	__attribute__((section(".libdh.plugins")))	\
	s_type __##plugin_name = {			\
		.member = {				\
			.dnode = DLIST_HEAD_INIT(__##plugin_name.member.dnode),	\
			.name = #plugin_name,		\
			.type = plugin_type,		\
			.init = init_fn,		\
			.deinit = deinit_fn,		\
			.refcnt = __RAW_REFCNT_INITIALIZER(0),\
			.extensions = exts,		\
			.ext_points = DLIST_HEAD_INIT(__##plugin_name.member.ext_points),	\
		},					\
		__VA_ARGS__				\
	};						\
	struct libdh_plugin *__DH_PLUGIN_NAME__(void)	\
	{						\
		return &__##plugin_name.member;		\
	}
#endif

#define __SUBCLASS_MEMBER_SET(member, val)	\
	.member = val

#define __DECLARE_EXTENSION_POINT(ep_name, s_type, member, 	\
				  init_fn, deinit_fn,		\
				  extend_fn, detach_fn,		\
				  private, ep_type, ep_attr, ...)	\
	static __used						\
	__attribute__((section(".libdh.extensionpoints"), aligned(1)))		\
	s_type __ep_##ep_name = {				\
		.member = {					\
			.dnode = DLIST_HEAD_INIT(__ep_##ep_name.member.dnode),	\
			.name = #ep_name,			\
			.type = ep_type,			\
			.flags = ep_attr,			\
			.size = sizeof(s_type),			\
			.owner = __DH_PLUGIN_NAME__,		\
			.owner_dnode = DLIST_HEAD_INIT(__ep_##ep_name.member.owner_dnode),	\
			.refcnt = __RAW_REFCNT_INITIALIZER(0),	\
			.rwlock = RAW_RDLOCK_INITIALIZER,	\
			.init = init_fn,			\
			.deinit = deinit_fn,			\
			.attach = extend_fn,			\
			.detach = detach_fn,			\
			._private = (void *)(uintptr_t)(private),	\
		},						\
		__VA_ARGS__					\
	}

static inline void dh_ep_rdlock(struct extension_point *ep)
{
	if ((ep->flags & DH_EP_FLAGS_STATIC) == 0) {
		(void)raw_rwlock_rdlock(&ep->rwlock);
	}
}

static inline void dh_ep_rwlock(struct extension_point *ep)
{
	if ((ep->flags & DH_EP_FLAGS_STATIC) == 0) {
		(void)raw_rwlock_wrlock(&ep->rwlock);
	}
}

static inline void dh_ep_unlock(struct extension_point *ep)
{
	if ((ep->flags & DH_EP_FLAGS_STATIC) == 0) {
		(void)raw_rwlock_unlock(&ep->rwlock);
	}
}

#endif /* ULIBS_LIBDEVHOST_PLUGIN_BASE_H */
