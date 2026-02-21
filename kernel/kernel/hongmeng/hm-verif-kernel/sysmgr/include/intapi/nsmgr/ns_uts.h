/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UTS namespace header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 19 17:01:24 2021
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_UTS_H
#define SYSMGR_INCLUDE_NSMGR_NS_UTS_H

#include <sysif/server.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <intapi/nsmgr/ns_common.h>

#ifndef CLONE_NEWUTS
#define CLONE_NEWUTS	(0x4000000)
#endif

struct thread_s;
struct uts_ns {
	struct ns_common ns_common;

	/* protect updating uname */
	struct raw_mutex mutex;
	struct uname name;
#ifdef CONFIG_FAKE_UNAME
	bool fake_uname;
#endif
};

static inline int
uts_ns_get(struct uts_ns *ns)
{
	return ns_common_get(&ns->ns_common);
}

static inline void
uts_ns_put(struct uts_ns *ns)
{
	return ns_common_put(&ns->ns_common);
}

static inline struct uts_ns *
common_to_uts_ns(struct ns_common *ns)
{
	return container_of(ns, struct uts_ns, ns_common);
}

static inline int
uts_ns_set_hostname(struct uts_ns *utsns, const char *name, size_t len)
{
	int err = E_HM_FAULT;

	if (utsns != NULL && name != NULL) {
		err = E_HM_OK;
		raw_mutex_lock(&utsns->mutex);
		NOFAIL(strncpy_s(utsns->name.nodename, __SYSCONF_STR_LEN_MAX, name, __SYSCONF_STR_LEN_MAX - 1));
		raw_mutex_unlock(&utsns->mutex);
	} else if (utsns != NULL && name == NULL && len == 0) {
		err = E_HM_OK;
		raw_mutex_lock(&utsns->mutex);
		mem_zero_a(utsns->name.nodename);
		raw_mutex_unlock(&utsns->mutex);
	} else {
		/* do nothing */
	}

	return err;
}

static inline int
uts_ns_get_uname(struct uts_ns *utsns, struct uname *name)
{
	int err = E_HM_INVAL;

	if (utsns != NULL && name != NULL) {
		err = E_HM_OK;
		raw_mutex_lock(&utsns->mutex);
		NOFAIL(memcpy_s(name, sizeof(struct uname),
				&utsns->name, sizeof(struct uname)));
		raw_mutex_unlock(&utsns->mutex);
	}
	return err;
}

static inline int
uts_ns_set_domainname(struct uts_ns *utsns, const char *name, size_t len)
{
	int err = E_HM_FAULT;

	if (utsns != NULL && name != NULL) {
		err = E_HM_OK;
		raw_mutex_lock(&utsns->mutex);
		NOFAIL(strncpy_s(utsns->name.domainname, __SYSCONF_STR_LEN_MAX, name, __SYSCONF_STR_LEN_MAX - 1));
		raw_mutex_unlock(&utsns->mutex);
	} else if (utsns != NULL && name == NULL && len == 0) {
		err = E_HM_OK;
		raw_mutex_lock(&utsns->mutex);
		mem_zero_a(utsns->name.domainname);
		raw_mutex_unlock(&utsns->mutex);
	} else {
		/* do nothing */
	}

	return err;
}

struct ns_operation *fetch_uts_op(void);

#ifdef CONFIG_FAKE_UNAME
bool uts_ns_is_fake_uname(struct uts_ns *utsns);
static inline void uts_ns_set_fake_uname(struct uts_ns *utsns, bool flag)
{
	utsns->fake_uname = flag;
}
#else
static inline bool uts_ns_is_fake_uname(struct uts_ns *utsns)
{
	UNUSED(utsns);
	return false;
}
#endif

#ifdef CONFIG_UTS_NS
int copy_uts_ns(struct uts_ns *ns_old, unsigned int flags, struct uts_ns **ns_new);
#else
static inline int copy_uts_ns(struct uts_ns *ns_old, unsigned int flags, struct uts_ns **ns_new)
{
	int err = E_HM_OK;

	if ((flags & (unsigned int)CLONE_NEWUTS) != 0U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*ns_new = ns_old;
		int rc = uts_ns_get(ns_old);
		BUG_ON(rc <= 0);
	}

	return err;
}
#endif

#endif
