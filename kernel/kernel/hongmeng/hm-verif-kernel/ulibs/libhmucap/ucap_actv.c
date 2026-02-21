/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces for ucap_actv
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 14 09:54:16 2023
 */

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <hmkernel/activation/local_area.h>
#include <libhmlog/hmlog.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libstrict/strict.h>
#include <libhmucap/ucap.h>
#include <libhmactv/actv.h>
#include <libhmucap/kobjpool.h>
#include <libhmucap/actv_kobjpool.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_hmsignal.h>

#include "ucap_internal.h"

static cref_t new_actv_pool(cref_t base_pmem, unsigned long name, unsigned long flags)
{
	cref_t ap_cref;
	ap_cref = hm_ucap_new_cap(0, base_pmem, hmobj_ActivationPool, 0xff, name, flags, 0UL, 0UL);
	if (IS_REF_ERR(ap_cref)) {
		return ap_cref;
	}
#ifdef CONFIG_PAC
	int err;
	err = hm_actv_pool_pac_update(ap_cref);
	if (err != E_HM_OK) {
		(void)hm_ucap_delete_cap(0, ap_cref);
		return ERR_TO_REF(err);
	}
#endif
	return ap_cref;
}

/* The caller must ensure that the name is a string ending with '/0' */
cref_t hm_ucap_new_actvpool_with_flags(const char *name, unsigned long flags)
{
	char pool_name[HMCAP_ACTVPOOL_NAME_SIZE] = "default";
	struct kobj_slot *slot = NULL;
	cref_t ap_cref;
	int err;

	if (name != NULL) {
		size_t copy_sz = min(strlen(name), sizeof(pool_name) - 1U);
		err = strncpy_s(pool_name, sizeof(pool_name), name, copy_sz);
		if (err != 0) {
			hm_warn("failed to read name for actvpool, err=%d\n", err);
			__hmattr_sideff_resolved("no relative side effect");
			return ERR_TO_REF(E_HM_INVAL);
		} else {
			pool_name[copy_sz] = '\0';
		}
	}

	slot = kobj_alloc_slot(0, hmobj_ActivationPool, (uint64_t)__CAP_ACTIVATION_POOL_OBJ_SIZE);
	if (slot == NULL) {
		hm_warn("failed to alloc pmem for actvpool\n");
		__hmattr_sideff_resolved("no relative side effect");
		return ERR_TO_REF(E_HM_NOMEM);
	}
	IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");

	ap_cref = new_actv_pool(slot->pmem->mem.pmem, ptr_to_ulong(&pool_name[0]), flags);
	if (IS_REF_ERR(ap_cref)) {
		hm_warn("failed to create actvpool, err=%s\n", hmstrerror(REF_TO_ERR(ap_cref)));
		kobj_free_slot(0, slot);
		__hmattr_sideff_resolved("all slots has been freed, no size effect");
		return ap_cref;
	}

	err = hm_proc_sync_cred_for_ap(ap_cref);
	if (err != E_HM_OK) {
		hm_error("sync cred for ap error\n");
		(void)hm_ucap_delete_cap(0, ap_cref);
		kobj_free_slot(0, slot);
		ap_cref = ERR_TO_REF(err);
		return ap_cref;
	}

	err = kobj_record_slot(ap_cref, slot);
	if (err != E_HM_OK) {
		(void)hm_ucap_delete_cap(0, ap_cref);
		kobj_free_slot(0, slot);
		ap_cref = ERR_TO_REF(err);
	}

	return ap_cref;
}

/* The caller must ensure that the name is a string ending with '\0' */
cref_t hm_ucap_new_actvpool(const char *name)
{
	return hm_ucap_new_actvpool_with_flags(name, 0UL);
}

int hm_ucap_delete_actvpool(cref_t ap_cref)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(0, ap_cref, hmobj_ActivationPool);
	if (err >= 0) {
		slot = kobj_remove_slot(ap_cref);
		if (slot == NULL) {
			err = E_HM_NOOBJ;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(0, ap_cref);
		if (err < 0) {
			hm_warn("failed to delete actv pool, err=%s\n", hmstrerror(err));
			if (kobj_record_slot(ap_cref, slot) != E_HM_OK) {
				(void)do_delete_cap(0, ap_cref);
				kobj_free_slot(0, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(0, slot);
	}

	return err < 0 ? err : E_HM_OK;
}

int hm_ucap_new_actv(cref_t actv_pool_cref, struct kobj_self_mng *obj)
{
	int err = E_HM_OK;
	cref_t a_cref = ERR_TO_REF(E_HM_INVAL);
	DEFINE_HMSIGMASK_ACTV_DEFAULT(mask_default);
	struct kobj_pmem *pmem = NULL;

	pmem = kobj_alloc_pmem_slot(hm_ucap_self_cnode_cref(), hmobj_Activation, __CAP_ACTIVATION_OBJ_SIZE);
	if (pmem == NULL) {
		err = E_HM_NOMEM;
		hm_warn("failed to alloc pmem, err=%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		a_cref = hm_ucap_new_cap(0, pmem->mem.pmem, hmobj_Activation,
					 0xff, ptr_to_ulong(&actv_pool_cref),
					 0UL, ptr_to_ulong(&mask_default), 0UL);
		if (IS_REF_ERR(a_cref)) {
			err = REF_TO_ERR(a_cref);
			hm_warn("failed to create actv, err=%s\n", hmstrerror(err));
			kobj_free_pmem_slot(hm_ucap_self_cnode_cref(), pmem);
		} else {
			obj->cref = a_cref;
			obj->pmem = pmem;
		}
	}

	return err;
}

int hm_ucap_delete_actv(struct kobj_self_mng *obj)
{
	int err;

	err = do_delete_cap(0, obj->cref);
	if (err < 0) {
		hm_warn("failed to delete actv, err=%s\n", hmstrerror(err));
	} else {
		kobj_free_pmem_slot(hm_ucap_self_cnode_cref(), obj->pmem);
	}

	return err < 0 ? err : E_HM_OK;
}

cref_t hm_ucap_new_actv_pure(cref_t actv_pool_cref, void *actv_kobj_pool, void *p_actv)
{
	int err = E_HM_OK;
	struct actv_kobj_slot *slot = NULL;
	cref_t a_cref = ERR_TO_REF(E_HM_INVAL);
	DEFINE_HMSIGMASK_ACTV_DEFAULT(mask_default);

	slot = actv_kobj_slot_alloc(actv_kobj_pool, p_actv);
	if (slot == NULL) {
		err = E_HM_NOMEM;
		hm_warn_ratelimited("failed to alloc pmem for actv\n");
	} else {
		a_cref = hm_ucap_new_cap(0, slot->mem.pmem, hmobj_Activation, 0xff,
					 ptr_to_ulong(&actv_pool_cref), 0UL, ptr_to_ulong(&mask_default), 0UL);
		if (IS_REF_ERR(a_cref)) {
			err = REF_TO_ERR(a_cref);
			hm_warn("failed to create actv, err=%s\n", hmstrerror(err));
			actv_kobj_pool_free_one_slot(actv_kobj_pool, slot);
		}
	}

	if (err == E_HM_OK) {
		/* record inner idx into new slot */
		slot->inner_idx = __REF_INNER_IDX_OF(a_cref);
	} else {
		a_cref = ERR_TO_REF(err);
	}

	return a_cref;
}

void *hm_ucap_actv_pure_res_of(cref_t a_cref, void *actv_kobj_pool)
{
	struct actv_kobj_slot *slot = NULL;
	uint32_t inner_idx = __REF_INNER_IDX_OF(a_cref);

	slot = actv_kobj_slot_search(actv_kobj_pool, inner_idx);
	return actv_kobj_pool_res_of(slot);
}

int hm_ucap_delete_actv_pure(cref_t a_cref, void *actv_kobj_pool)
{
	int err;
	uint32_t inner_idx = __REF_INNER_IDX_OF(a_cref);

	err = do_delete_cap(0, a_cref);
	if (err < 0) {
		hm_warn("failed to delete actv pure, err=%s\n", hmstrerror(err));
	} else {
		/* actv kobj slot must be free after deleting actv cap */
		actv_kobj_slot_free(actv_kobj_pool, inner_idx);
	}

	return err < 0 ? err : E_HM_OK;
}

#ifdef CONFIG_NEED_SHADOW_STACK
void *hm_ucap_actv_pure_sstack_of(cref_t a_cref, void *actv_kobj_pool)
{
	struct actv_kobj_slot *slot = NULL;
	uint32_t inner_idx = __REF_INNER_IDX_OF(a_cref);

	slot = actv_kobj_slot_search(actv_kobj_pool, inner_idx);
	return actv_kobj_pool_sstack_of(slot);
}
#endif
