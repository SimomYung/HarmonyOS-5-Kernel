/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Read and extract pgstr in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 20 18:07:34 2022
 */

#ifndef SYSMGR_INCLUDE_INTAPI_CTRLMEM_PGSTR_H
#define SYSMGR_INCLUDE_INTAPI_CTRLMEM_PGSTR_H

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/pgstr/pgstr.h>

static inline __u32 pgstr_val_of(struct pgstr_s *pgstr)
{
	return vatomic32_read_rlx(&pgstr->v);
}

static inline __u32 pgstrv_type_of(__u32 pgstrv)
{
	return (pgstrv & (__u32)(__PGSTRV_MASK_TYPE)) >> (__PGSTRV_SHIFT_TYPE);
}

static inline __u32 pgstrv_refcnt_of(__u32 pgstrv)
{
	return (pgstrv & (__u32)(__PGSTRV_MASK_REFCNT)) >> (__PGSTRV_SHIFT_REFCNT);
}

extern const char *pgstrv_strtype(__u32 type);

static inline __u32 pgstr_type_of(struct pgstr_s *pgstr)
{
	return pgstrv_type_of(pgstr_val_of(pgstr));
}

static inline __u32 pgstr_refcnt_of(struct pgstr_s *pgstr)
{
	return pgstrv_refcnt_of(pgstr_val_of(pgstr));
}

static inline __bool pgstr_cmp_type_refcnt(struct pgstr_s *pgstr,
					   __u32 expect_type,
					   __u32 expect_refcnt)
{
	__u32 pgstrv	= pgstr_val_of((pgstr));
	__u32 type	= pgstrv_type_of(pgstrv);
	__u32 refcnt	= pgstrv_refcnt_of(pgstrv);

	return ((type == expect_type) && (refcnt == expect_refcnt)) ? __true : __false;
}

static inline __bool pgstr_cmp_type(struct pgstr_s *pgstr, __u32 expect_type)
{
	__u32 pgstrv	= pgstr_val_of((pgstr));
	__u32 type	= pgstrv_type_of(pgstrv);

	return (type == expect_type) ? __true : __false;
}

static inline __bool pgstr_type_need_refcnt(__u32 type)
{
	return !((type == __PGSTR_TYPE_HIGHMEM) || (type == __PGSTR_TYPE_CONTIG));
}

#endif
