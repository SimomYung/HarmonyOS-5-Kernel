/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The api of the ucap
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 14 19:47:09 2023
 */

#include <libhmucap/ucap.h>
#include <libhmucap/kobjpool.h>
#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>

cref_t hm_ucap_new_ec_ep(cref_t cnode, struct __ec_ep_args *args,
			 cref_t vspace_cref)
{
	int err = E_HM_OK;
	struct kobj_slot *slot = NULL;
	cref_t ep_cref = ERR_TO_REF(E_HM_INVAL);

	slot = kobj_alloc_slot(cnode, hmobj_EcEp, __CAP_EC_EP_OBJ_SIZE);
	if (slot == NULL) {
		err = E_HM_NOMEM;
		ep_cref = ERR_TO_REF(err);
		hm_warn("failed to alloc pmem for ec ep\n");
	}

	if (err == E_HM_OK) {
		IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");
		ep_cref = hm_ucap_new_cap(cnode, slot->pmem->mem.pmem,
					  hmobj_EcEp, __CAP_RIGHTS_ALL,
					  ptr_to_ulong(args),
					  ptr_to_ulong(&vspace_cref),
					  0UL, 0UL);
		if (IS_REF_ERR(ep_cref)) {
			err = REF_TO_ERR(ep_cref);
			kobj_free_slot(cnode, slot);
		}
	}

	if (err == E_HM_OK) {
		err = kobj_record_slot(ep_cref, slot);
		if (err != E_HM_OK) {
			(void)hm_ucap_delete_cap(cnode, ep_cref);
			kobj_free_slot(cnode, slot);
			ep_cref = ERR_TO_REF(err);
		}
	}

	return ep_cref;
}

int hm_ucap_delete_ec_ep(cref_t cnode, cref_t ep_cref)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(cnode, ep_cref, hmobj_EcEp);
	if (err >= 0) {
		slot = kobj_remove_slot(ep_cref);
		if (slot == NULL) {
			err = E_HM_NOOBJ;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(cnode, ep_cref);
		if (err < 0) {
			if (kobj_record_slot(ep_cref, slot) != E_HM_OK) {
				(void)do_delete_cap(cnode, ep_cref);
				kobj_free_slot(cnode, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(cnode, slot);
	}

	return err < 0 ? err : E_HM_OK;
}

cref_t hm_ucap_new_ec_chn(cref_t cnode)
{
	int err = E_HM_OK;
	struct kobj_slot *slot = NULL;
	cref_t chn_cref = ERR_TO_REF(E_HM_INVAL);

	slot = kobj_alloc_slot(cnode, hmobj_EcChn, __CAP_EC_CHN_OBJ_SIZE);
	if (slot == NULL) {
		err = E_HM_NOMEM;
		chn_cref = ERR_TO_REF(err);
		hm_warn("failed to alloc pmem for chn\n");
	}

	if (err == E_HM_OK) {
		IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");
		chn_cref = hm_ucap_new_cap(cnode, slot->pmem->mem.pmem,
					   hmobj_EcChn, __CAP_RIGHTS_ALL,
					   0UL, 0UL, 0UL, 0UL);
		if (IS_REF_ERR(chn_cref)) {
			err = REF_TO_ERR(chn_cref);
			kobj_free_slot(cnode, slot);
		}
	}

	if (err == E_HM_OK) {
		err = kobj_record_slot(chn_cref, slot);
		if (err != E_HM_OK) {
			(void)hm_ucap_delete_cap(cnode, chn_cref);
			kobj_free_slot(cnode, slot);
			chn_cref = ERR_TO_REF(err);
		}
	}

	return chn_cref;
}

int hm_ucap_delete_ec_chn(cref_t cnode, cref_t chn_cref)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(cnode, chn_cref, hmobj_EcChn);
	if (err >= 0) {
		slot = kobj_remove_slot(chn_cref);
		if (slot == NULL) {
			err = E_HM_NOOBJ;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(cnode, chn_cref);
		if (err < 0) {
			if (kobj_record_slot(chn_cref, slot) != E_HM_OK) {
				(void)do_delete_cap(cnode, chn_cref);
				kobj_free_slot(cnode, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(cnode, slot);
	}

	return err < 0 ? err : E_HM_OK;
}
