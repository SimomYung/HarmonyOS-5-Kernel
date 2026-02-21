/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of wake lock native api
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 5 15:36:45 2019
 */
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/wakelock.h>
#include <libhwsecurec/securec.h>
#include <libsysif/pwrmgr/api.h>

#include "pm_sysif.h"

static int set_wakelock_name(struct wake_lock_name *wl_name, const char *name)
{
	size_t len;

	if (name == NULL) {
		hm_warn("invalid name\n");
		return E_HM_INVAL;
	}
	len = strnlen(name, WAKELOCK_NAME_MAX);
	if (len >= WAKELOCK_NAME_MAX || len == 0) {
		hm_warn("invalid name len=%zu\n", len);
		return E_HM_INVAL;
	}
	NOFAIL(strncpy_s(wl_name->string, sizeof(wl_name->string), name, len));

	return E_HM_OK;
}

static int do_wakelock_action(const char *name, unsigned int action)
{
	int err;
	rref_t rref;
	struct wake_lock_name wl_name;

	err = set_wakelock_name(&wl_name, name);
	if (err != E_HM_OK) {
		return err;
	}
	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n",
			 hmstrerror(err));
		return err;
	}

	return actvcapcall_pmcall_wakelock_action(rref, action, wl_name);
}

int hm_wake_lock_add(const char *name)
{
	return do_wakelock_action(name, WL_ACTION_ADD);
}

int hm_wake_lock_remove(const char *name)
{
	return do_wakelock_action(name, WL_ACTION_REMOVE);
}

int hm_wake_lock_acquire(const char *name)
{
	return do_wakelock_action(name, WL_ACTION_ACQUIRE);
}

int hm_wake_lock_release(const char *name)
{
	return do_wakelock_action(name, WL_ACTION_RELEASE);
}

int hm_wake_lock_acquire_timeout(const char *name, unsigned long msec)
{
	int err;
	struct wake_lock_name wl;
	rref_t rref;

	err = set_wakelock_name(&wl, name);
	if (err != E_HM_OK) {
		return err;
	}
	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}

	return actvcapcall_pmcall_wakelock_acquire_timeout(rref, wl, msec);
}

int hm_wake_lock_get_count(unsigned int *count)
{
	int err;
	rref_t rref;
	struct __actvret_pmcall_wakelock_get_count ret;

	if (count == NULL) {
		hm_error("invalid count\n");
		return E_HM_INVAL;
	}
	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n",
			 hmstrerror(err));
		return err;
	}
	mem_zero_s(ret);
	err = actvcapcall_pmcall_wakelock_get_count(rref, &ret);
	if (err != E_HM_OK) {
		hm_error("wake lock get count failed: %s\n",
			 hmstrerror(err));
		return err;
	}
	*count = ret.count;

	return E_HM_OK;
}

int hm_wake_lock_save_count(unsigned int count)
{
	int err;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}
	err = actvcapcall_pmcall_wakelock_save_count(rref, count);
	if (err != E_HM_OK) {
		hm_error("wake lock save count failed: %s\n",
			 hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

struct wake_lock_stat_config {
	rref_t rref;
	struct wake_lock_name name;
};

static int wake_lock_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct wake_lock_stat_config *config = NULL;

	config = (struct wake_lock_stat_config *)data;
	return actvxactcapcall_pmcall_wakelock_stat(false, true, config->rref,
						    config->name, *attr);
}

int hm_wake_lock_stat(const char *name, struct bunch *bunch)
{
	int err = E_HM_OK;
	rref_t rref = 0;
	struct sysarg_cnode_statrref statrref = { 0 };
	struct wake_lock_name wl_name;
	struct wake_lock_stat_config config;
	struct bunch_ipc_reader_ctx ctx;

	if (bunch == NULL) {
		hm_error("invalid bunch\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = set_wakelock_name(&wl_name, name);
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("wakelock: pm sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(config);
		config.name = wl_name;
		config.rref = rref;
		/* query tppmgr cnode_idx by rref */
		err = sysfast_cnode_stat_rref(&statrref, rref,
					CNODE_STATRREF_SRC_CNODE_IDX);
		if (err < 0) {
			hm_error("query tppmgr cnode_idx failed:%s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		ctx.ipc = wake_lock_stat_ipc;
		ctx.private_data = ptr_to_void(&config);
		ctx.target_info = rref;
		err = bunch_ipc_request(bunch, &ctx);
	}

	return err;
}
