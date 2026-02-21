/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces for ucap_fdtable
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 31 10:16:23 2023
 */

#include <libhmucap/ucap.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <hmkernel/capability.h>

#include <libhmlog/hmlog.h>
#include <libmem/types.h>
#include <libhmucap/kobjpool.h>

int hm_ucap_new_fdtable(cref_t cnode, __u64 rlimit, struct kobj_self_mng *obj, __u64 process_cnode_idx)
{
	int err = E_HM_OK;
	__u64 _rlimit = rlimit;
	cref_t fdtable_cref = ERR_TO_REF(E_HM_INVAL);
	struct kobj_pmem *pmem = NULL;

	pmem = kobj_alloc_pmem_slot(cnode, hmobj_FDTable, __CAP_FDTABLE_OBJ_SIZE);
	if (pmem == NULL) {
		err = E_HM_NOMEM;
		hm_warn("failed to alloc pmem, err=%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		fdtable_cref = hm_ucap_new_cap(cnode, pmem->mem.pmem, hmobj_FDTable, 0xff,
									   ptr_to_ulong(&_rlimit), 0, process_cnode_idx, 0);
		if (IS_REF_ERR(fdtable_cref)) {
			err = REF_TO_ERR(fdtable_cref);
			hm_warn("failed to create fdtable, err=%s\n", hmstrerror(err));
			kobj_free_pmem_slot(cnode, pmem);
		} else {
			obj->cref = fdtable_cref;
			obj->pmem = pmem;
		}
	}

	return err;
}

int hm_ucap_delete_fdtable(cref_t cnode, struct kobj_self_mng *obj)
{
	int err;

	err = do_delete_cap(cnode, obj->cref);
	if (err < 0) {
		hm_warn("failed to delete fdtable, err=%s\n", hmstrerror(err));
	} else {
		kobj_free_pmem_slot(cnode, obj->pmem);
	}

	return err < 0 ? err : E_HM_OK;
}
