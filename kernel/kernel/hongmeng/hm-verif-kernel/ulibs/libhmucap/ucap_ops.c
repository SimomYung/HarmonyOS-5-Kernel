/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of ulibs ucap operation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 22 17:35:04 2019
 */
#include <libhmucap/ucap.h>

#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>

/*
 * Note we don't separate alloc and free in two files, because we want to
 * couple them together, so there will be no case we alloc with one version and
 * free with another version
 */
static cref_t
alloc_pmem(cref_t cnode, uint64_t size, struct ucap_mem *mem)
{
	cref_t pmem_cref = 0;

	if (__REF_IS_RREF(cnode)) {
		/*
		 * XXX: call this via a cnode rref, the caller needs to alloc
		 * and move the pmem for extend table. We may not support this
		 * for uapps.
		 */
		pmem_cref = ERR_TO_REF(E_HM_NOSYS);
	} else if (cnode != 0U &&
		   (__REF_CNODE_IDX_OF(cnode) != hm_ucap_self_cnode_idx())) {
		pmem_cref = ERR_TO_REF(E_HM_INVAL);
	} else {
		pmem_cref = hm_mem_alloc_pmem((size_t)size);
		if (IS_REF_ERR(pmem_cref)) {
			hm_warn("hm_mem_alloc_pmem failed, err=%s\n",
				hmstrerror(REF_TO_ERR(pmem_cref)));
		} else if (mem != NULL) {
			mem->pmem = pmem_cref;
			mem->page = NULL;
		} else {
			/* do nothing */
		}
	}

	return pmem_cref;
}

static int
free_pmem(cref_t cnode, const struct ucap_mem *mem)
{
	int err = E_HM_OK;

	__u32 cnode_idx = hm_ucap_self_cnode_idx();

	if (mem == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK && is_cnode_idx_err(cnode_idx)) {
		hm_panic("Unable to get cnode idx due to: %s\n",
			 hmstrerror(cnode_idx_to_err(cnode_idx)));
	}

	if (err == E_HM_OK) {
		if (__REF_IS_RREF(cnode)) {
			/*
			 * XXX: call this via a cnode rref, the caller needs to move
			 * back and free the pmem.
			 */
			err = E_HM_NOSYS;
		} else if (cnode != 0U &&
			   (__REF_CNODE_IDX_OF(cnode) != cnode_idx)) {
			err = E_HM_INVAL;
		} else {
			err = hm_mem_free_pmem(mem->pmem);
			if (err < 0) {
				hm_warn("hm_mem_free_pmem failed, err=%s\n",
					hmstrerror(err));
			}
		}
	}

	return err;
}

static int
cslots_water(cref_t cnode, enum hmobj cap_type)
{
	int water;

	switch (cap_type) {
	case hmobj_Activation:
		water = CTABLE_SLOTS_WATER_NEWCAP_ACTV;
		break;
	case hmobj_PMEM:
		water = CTABLE_SLOTS_WATER_NEWCAP_PMEM;
		break;
	case hmobj_CpuContext:
		water = CTABLE_SLOTS_WATER_NEWCAP_CPUCONTEXT;
		break;
	case hmobj_IrqCtrl:
		/* fall-through */
	case hmobj_Timer:
		/* fall-through */
	case hmobj_CNode:
		/* fall-through */
	case hmobj_Thread:
		/* fall-through */
	case hmobj_Channel:
		/* fall-through */
	case hmobj_ActivationPool:
		/* fall-through */
	case hmobj_VSpace:
		/* fall-through */
	case hmobj_SysCtrl:
		/* fall-through */
	case hmobj_Scheduler:
		/* fall-through */
	case hmobj_VM:
		/* fall-through */
	case hmobj_Module:
		/* fall-through */
	case hmobj_Permission:
		/* fall-through */
	case hmobj_MAX:
		/* fall-through */
	case hmobj_NULL:
		/* fall-through */
	default:
		water = __REF_IS_RREF(cnode) ? E_HM_NOSYS
					     : CTABLE_SLOTS_WATER_UAPP;
		break;
	}

	return water;
}

static int
extend_ctable(cref_t cnode, unsigned int water)
{
	UNUSED(water);
	int err;

	if (__REF_IS_RREF(cnode)) {
		err = E_HM_NOSYS;
	} else {
		err = actvcall_hmcall_mem_extend_ctable();
	}

	return err;
}

static int
record_table_pmem(cref_t cnode, const struct ucap_mem *mem)
{
	UNUSED(cnode);
	int err = E_HM_OK;

	if (mem == NULL) {
		err = E_HM_INVAL;
	}

	/* For uapp, no need to record table pmem */
	return err;
}

struct ucap_ops ucap_default_ops __read_mostly = {
	.alloc_pmem = alloc_pmem,
	.free_pmem = free_pmem,
	.cslots_water = cslots_water,
	.extend_ctable = extend_ctable,
	.record_table_pmem = record_table_pmem,
};
