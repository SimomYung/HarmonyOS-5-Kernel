/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces for ucap_channel
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 14 11:40:20 2023
 */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <libhmucap/ucap.h>
#include <libhmucap/kobjpool.h>
#include <libhmsrv_sys/hm_mem.h>

#include "ucap_internal.h"

cref_t hm_ucap_new_channel(void)
{
	int err = E_HM_OK;
	cref_t chn_cref = ERR_TO_REF(E_HM_INVAL);
	struct kobj_slot *slot;

	slot = kobj_alloc_slot(0, hmobj_Channel, UCAP_NEW_CHANNEL_SIZE);
	if (slot == NULL) {
		err = E_HM_NOMEM;
		chn_cref = ERR_TO_REF(E_HM_NOMEM);
		hm_warn("failed to alloc pmem for channel\n");
	}

	if (err == E_HM_OK) {
		IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");

		chn_cref = hm_ucap_new_cap(0, slot->pmem->mem.pmem, hmobj_Channel, 0xff,
					   0, 0, 0, 0);
		if (IS_REF_ERR(chn_cref)) {
			err = REF_TO_ERR(chn_cref);
			hm_warn("failed to create channel, err=%s\n", hmstrerror(err));
			kobj_free_slot(0, slot);
		}
	}

	if (err == E_HM_OK) {
		err = kobj_record_slot(chn_cref, slot);
		if (err != E_HM_OK) {
			(void)hm_ucap_delete_cap(0, chn_cref);
			kobj_free_slot(0, slot);
			chn_cref = ERR_TO_REF(err);
		}
	}

	return chn_cref;
}

int hm_ucap_delete_channel(cref_t chn_cref)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(0, chn_cref, hmobj_Channel);
	if (err >= 0) {
		slot = kobj_remove_slot(chn_cref);
		if (slot == NULL) {
			err = E_HM_NOOBJ;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(0, chn_cref);
		if (err < 0) {
			hm_warn("failed to delete channel, err=%s\n", hmstrerror(err));
			if (kobj_record_slot(chn_cref, slot) != E_HM_OK) {
				(void)do_delete_cap(0, chn_cref);
				kobj_free_slot(0, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(0, slot);
	}

	return err < 0 ? err : E_HM_OK;
}
