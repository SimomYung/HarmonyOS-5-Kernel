/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces for ucap_dsuctx
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 20 10:19:12 2023
 */

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <libhmucap/ucap.h>
#include <libhmucap/kobjpool.h>
#include <libsysif/sysmgr/api.h>

#include "ucap_internal.h"

static cref_t
ucap_new_dsuctx_with_security_check(cref_t base_pmem)
{
	int err;
	cref_t dsuctx_cref;
	struct __actvret_hmcall_proc_new_dsucontext ret;

	mem_zero_s(ret);
	err = actvcall_hmcall_proc_new_dsucontext(base_pmem, &ret);
	if (err < 0) {
		dsuctx_cref = ERR_TO_REF(err);
	} else {
		dsuctx_cref = ret.cref;
	}

	return dsuctx_cref;
}

static cref_t ucap_new_dsuctx(const struct kobj_slot *slot)
{
	IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");

	cref_t base_pmem = slot->pmem->mem.pmem;

	return ucap_new_dsuctx_with_security_check(base_pmem);
}

cref_t hm_ucap_new_dsuctx(void)
{
	int err = E_HM_OK;
	struct kobj_slot *slot = NULL;
	cref_t dsuctx_cref = ERR_TO_REF(E_HM_INVAL);

	slot = kobj_alloc_slot(0, hmobj_DsuContext, CPUCONTEXT_ARM_DSU_STRUCT_SIZE);
	if (slot == NULL) {
		hm_warn("failed to alloc pmem for dsu context cap\n");
		err = E_HM_NOMEM;
	}

	if (err == 0) {
		dsuctx_cref = ucap_new_dsuctx(slot);
		if (IS_REF_ERR(dsuctx_cref)) {
			err = REF_TO_ERR(dsuctx_cref);
			hm_warn("failed to create dsu context cap: %s\n", hmstrerror(err));
			kobj_free_slot(0, slot);
		}
	}

	if (err == 0) {
		err = kobj_record_slot(dsuctx_cref, slot);
		if (err != E_HM_OK) {
			(void)do_delete_cap(0, dsuctx_cref);
			kobj_free_slot(0, slot);
			dsuctx_cref = ERR_TO_REF(err);
		}
	} else {
		dsuctx_cref = ERR_TO_REF(err);
	}

	return dsuctx_cref;
}

int hm_ucap_delete_dsuctx(cref_t dsu_ctx)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(0, dsu_ctx, hmobj_DsuContext);
	if (err >= 0) {
		slot = kobj_remove_slot(dsu_ctx);
		if (slot == NULL) {
			err = E_HM_NOENT;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(0, dsu_ctx);
		if (err < 0) {
			hm_warn("failed to delete dsu context, %s\n", hmstrerror(err));
			if (kobj_record_slot(dsu_ctx, slot) != E_HM_OK) {
				(void)do_delete_cap(0, dsu_ctx);
				kobj_free_slot(0, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(0, slot);
	}

	return err < 0 ? err : E_HM_OK;
}
