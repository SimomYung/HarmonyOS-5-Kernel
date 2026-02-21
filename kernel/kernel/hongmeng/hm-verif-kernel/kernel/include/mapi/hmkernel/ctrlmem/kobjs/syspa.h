/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Defs for syspa
 * Author: Huawei OS Kernel Lab
 * Created: Mon May 29 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_SYSPA_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_SYSPA_H

#include <uapi/hmkernel/ctrlmem/kobjs/syspa.h>
#include <hmkernel/pgtblhdlr.h>
#include <hmkernel/kernel.h>
#include <hmkernel/types.h>
#include <hmasm/memory.h>

struct ctrlmem_syspa32_s {
	u32 val;
};

struct ctrlmem_syspa64_s {
	u64 val;
};

struct ctrlmem_kobj_vspace_s;

typedef struct ctrlmem_syspa32_s ctrlmem_syspa32_t;
typedef struct ctrlmem_syspa64_s ctrlmem_syspa64_t;

#if __SIZEOF_POINTER__ == 4
typedef ctrlmem_syspa32_t ctrlmem_syspa_t;
#else
typedef ctrlmem_syspa64_t ctrlmem_syspa_t;
#endif

#define CTRLMEM_SYSPA_ZERO	{ \
	.val = 0u, \
}

/* type is only a mark to let reader know its original type. */
#define ctrlmem_syspa(type) ctrlmem_syspa_t

static inline void ctrlmem_syspa_set_ul(ctrlmem_syspa_t *syspa, unsigned long v)
{
	syspa->val = (typeof(syspa->val))(v);
}

static inline ctrlmem_syspa_t ctrlmem_mksyspa_ul(unsigned long v)
{
	ctrlmem_syspa_t syspa;
	ctrlmem_syspa_set_ul(&syspa, v);
	return syspa;
}

static inline ctrlmem_syspa_t ctrlmem_mksyspa_ptr(void *v)
{
	ctrlmem_syspa_t syspa;
	ctrlmem_syspa_set_ul(&syspa, ptr_to_ulong(v));
	return syspa;
}

static inline ctrlmem_syspa_t ctrlmem_syspa_minus(ctrlmem_syspa_t syspa, unsigned long off)
{
	ctrlmem_syspa_t ret = {
		.val = syspa.val - off,
	};
	return ret;
}

static inline ctrlmem_syspa_t ctrlmem_syspa_add(ctrlmem_syspa_t syspa, unsigned long off)
{
	ctrlmem_syspa_t ret = {
		.val = syspa.val + off,
	};
	return ret;
}

static inline bool ctrlmem_syspa_is_equal(ctrlmem_syspa_t a, ctrlmem_syspa_t b)
{
	return (a.val == b.val) ? true : false;
}

static inline bool ctrlmem_syspa_is_nonzero(ctrlmem_syspa_t syspa)
{
	return (syspa.val) != 0UL ? true : false;
}

#define CTRLMEM_SYSPA_INIT(v)	{ .val = (v), }

ENUM(4, ctrlmem_sysparule_e, CTRLMEM_SYSPARULE_,
	UNSET,
	/* page table walk, should provide pgd */
	PTWALK,
	/* linear mapping, should provide pvoffset */
	LINEAR,
	/* in SPIN, directly access its address */
	SPIN,
)

struct ctrlmem_sysparule_s {
	enum ctrlmem_sysparule_e type;
	u32 padding;
	union {
		struct ctrlmem_sysparule_pgtblwalk_data_s {
			struct ctrlmem_kobj_vspace_s *kobj_vspace;
		} pgtblwalk_data;
		struct ctrlmem_sysparule_linear_data_s {
			u64 pvoffset;
		} linear_data;
		struct ctrlmem_sysparule_spin_data_s {
		} spin_data;
	};
};

#endif
