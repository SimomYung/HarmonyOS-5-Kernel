/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces for ucap livepatch
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 14 12:52:41 2023
 */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <libhmucap/ucap.h>
#include <libhmucap/kobjpool.h>

#include "ucap_internal.h"

cref_t hm_ucap_new_livepatch(unsigned long cap_size, unsigned long func_nr)
{
	int err = E_HM_OK;
	cref_t lp_cref = ERR_TO_REF(E_HM_INVAL);
	struct kobj_slot *cap_slot = NULL;

	cap_slot = kobj_alloc_slot(0, hmobj_Livepatch, cap_size);
	if (cap_slot == NULL) {
		err = E_HM_NOMEM;
		lp_cref = ERR_TO_REF(E_HM_NOMEM);
		hm_warn("failed to alloc pmem for livepatch\n");
	}

	if (err == E_HM_OK) {
		IMPOSSIBLE(cap_slot->pmem == NULL, "pmem in slot can't be NULL");

		lp_cref = hm_ucap_new_cap(0, cap_slot->pmem->mem.pmem, hmobj_Livepatch, 0xff,
					  func_nr, 0, 0, 0);
		if (IS_REF_ERR(lp_cref)) {
			err = REF_TO_ERR(lp_cref);
			hm_warn("failed to create livepatch, err=%s\n", hmstrerror(err));
			kobj_free_slot(0, cap_slot);
		}
	}

	if (err == E_HM_OK) {
		err = kobj_record_slot(lp_cref, cap_slot);
		if (err != E_HM_OK) {
			(void)hm_ucap_delete_cap(0, lp_cref);
			kobj_free_slot(0, cap_slot);
			lp_cref = ERR_TO_REF(err);
		}
	}

	return lp_cref;
}

int hm_ucap_delete_livepatch(cref_t lp_cref)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(0, lp_cref, hmobj_Livepatch);
	if (err >= 0) {
		slot = kobj_remove_slot(lp_cref);
		if (slot == NULL) {
			err = E_HM_NOOBJ;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(0, lp_cref);
		if (err < 0) {
			hm_warn("failed to delete livepatch cap, err=%s\n", hmstrerror(err));
			if (kobj_record_slot(lp_cref, slot) != E_HM_OK) {
				(void)do_delete_cap(0, lp_cref);
				kobj_free_slot(0, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(0, slot);
	}

	return err < 0 ? err : E_HM_OK;
}
