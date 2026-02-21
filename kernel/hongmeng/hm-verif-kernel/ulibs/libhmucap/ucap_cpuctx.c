/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces for ucap cpuctx
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 20 09:06:28 2023
 */

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <libhmucap/ucap.h>
#include <libhmucap/kobjpool.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libsysif/sysmgr/api.h>

#include "ucap_internal.h"

struct cpuctx_size {
	const char *name;
	uint64_t size;
};

static struct cpuctx_size cpuctx_sizes[HM_UCAP_CPUCTX_MAX] = {
	[HM_UCAP_CPUCTX_ARM_PMU] = {
		.name = "perf",
		/*
		 * CPUCONTEXT_ARM_PMU_STRUCT_SIZE >= sizeof(struct perf_ctx_s).
		 * The size of (struct perf_cpu_ctx_s) is relate to CONFIG_NR_CPUS in the kernel currently,
		 * on Charlotte platform which cpu number is 8, the size is about to 14520.
		 */
		.size = CPUCONTEXT_ARM_PMU_STRUCT_SIZE,
	},
	[HM_UCAP_CPUCTX_ARM_DEBUG] = {
		.name = "debug",
		/*
		 * CPUCONTEXT_ARM_DEBUG_STRUCT_SIZE >= max (sizeof(struct debugv8_ctx_s),
		 * sizeof(struct debugv7_ctx_s))
		 */
		.size = CPUCONTEXT_ARM_DEBUG_STRUCT_SIZE,
	},
};

static cref_t
ucap_new_cpuctx_with_security_check(cref_t base_pmem,
				    struct cpucontext_name name)
{
	int err;
	cref_t cpuctx_cref;
	struct __actvret_hmcall_proc_new_cpucontext ret;

	mem_zero_s(ret);
	err = actvcall_hmcall_proc_new_cpucontext(base_pmem, name, &ret);
	if (err < 0) {
		cpuctx_cref = ERR_TO_REF(err);
	} else {
		cpuctx_cref = ret.cref;
	}

	return cpuctx_cref;
}

static cref_t ucap_new_cpuctx(const struct kobj_slot *slot,
			      struct cpucontext_name name,
			      bool need_security_check)
{
	cref_t cpuctx_cref;

	IMPOSSIBLE(slot->pmem == NULL, "pmem in slot can't be NULL");

	cref_t base_pmem = slot->pmem->mem.pmem;

	if (need_security_check) {
		cpuctx_cref = ucap_new_cpuctx_with_security_check(base_pmem, name);
	} else {
		cpuctx_cref = hm_ucap_new_cap(0, base_pmem,
					      hmobj_CpuContext,
					      __CAP_RIGHTS_ALL,
					      ptr_to_ulong(name.string),
					      0UL, 0UL, 0UL);
	}

	return cpuctx_cref;
}

static cref_t get_cpuctx_cref_by_type(enum hm_ucap_cpuctx_type type,
				      const struct kobj_slot *slot,
				      struct cpucontext_name name)
{
	cref_t cpuctx_cref = ERR_TO_REF(E_HM_INVAL);

	switch (type) {
	case HM_UCAP_CPUCTX_ARM_PMU:
	case HM_UCAP_CPUCTX_ARM_DEBUG:
		cpuctx_cref = ucap_new_cpuctx(slot, name, true);
		break;
	case HM_UCAP_CPUCTX_MAX:
		/* fall-through */
	default:
		break;
	}

	return cpuctx_cref;
}

cref_t hm_ucap_new_cpuctx(enum hm_ucap_cpuctx_type type)
{
	int err = E_HM_INVAL;
	size_t copy_sz;
	struct cpucontext_name name;
	struct kobj_slot *slot = NULL;
	cref_t cpuctx_cref = ERR_TO_REF(E_HM_INVAL);

	if (type < HM_UCAP_CPUCTX_MAX) {
		copy_sz = min(strlen(cpuctx_sizes[type].name), sizeof(name.string) - 1);
		err = strncpy_s(name.string, sizeof(name.string), cpuctx_sizes[type].name, copy_sz);
		if (err != 0) {
			hm_warn("failed to read name for cpu context, err=%d\n", err);
			err = posix2hmerrno(err);
		} else {
			name.string[copy_sz] = '\0';
		}
	}

	if (err == 0) {
		slot = kobj_alloc_slot(0, hmobj_CpuContext, cpuctx_sizes[type].size);
		if (slot == NULL) {
			hm_warn("failed to alloc pmem for cpu context cap\n");
			err = E_HM_NOMEM;
		}
	}

	if (err == 0) {
		cpuctx_cref = get_cpuctx_cref_by_type(type, slot, name);
		if (IS_REF_ERR(cpuctx_cref)) {
			err = REF_TO_ERR(cpuctx_cref);
			hm_warn("failed to create cpu context cap, err=%s\n", hmstrerror(err));
			kobj_free_slot(0, slot);
		}
	}

	if (err == 0) {
		err = kobj_record_slot(cpuctx_cref, slot);
		if (err != E_HM_OK) {
			(void)do_delete_cap(0, cpuctx_cref);
			kobj_free_slot(0, slot);
			cpuctx_cref = ERR_TO_REF(err);
		}
	} else {
		cpuctx_cref = ERR_TO_REF(err);
	}

	return cpuctx_cref;
}

int hm_ucap_delete_cpuctx(cref_t cpu_ctx)
{
	int err;
	struct kobj_slot *slot = NULL;

	err = kobj_check_captype(0, cpu_ctx, hmobj_CpuContext);
	if (err >= 0) {
		slot = kobj_remove_slot(cpu_ctx);
		if (slot == NULL) {
			err = E_HM_NOOBJ;
		}
	}

	if (err >= 0) {
		err = do_delete_cap(0, cpu_ctx);
		if (err < 0) {
			hm_warn("failed to delete cpu context, err=%s\n", hmstrerror(err));
			if (kobj_record_slot(cpu_ctx, slot) != E_HM_OK) {
				(void)do_delete_cap(0, cpu_ctx);
				kobj_free_slot(0, slot);
			}
		}
	}

	if (err >= 0) {
		kobj_free_slot(0, slot);
	}

	return err < 0 ? err : E_HM_OK;
}
