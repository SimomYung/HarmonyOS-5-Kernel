/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Mirror struct of sysmgr page_s, for comanage
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 24 09:53:25 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_SYSPG_SYSPG_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_SYSPG_SYSPG_H

#include <hmkernel/types.h>
#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif

struct __syspg_knode_s {
#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
	__u64 flags;
#else
	__u32 flags;
	__u32 lock;
#endif
	__u32 refcnt;
	__u32 mapcnt;
	__uptr_t vrg;
#if defined(CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE) || defined(CONFIG_PAGE_META_MERGE)
	__u64 index;
#else
	__u64 pos;
#endif
	__u64 ubdqnode;
};

#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
#define CTRLMEM_PAGE_HIGGBITS_SHIFT	32
#define CTRLMEM_BITLOCK_LOCKBIT		2
#define SYSMGR_PAGE_CTRLMEM_BITLOCK_MASK	(~(1UL << (CTRLMEM_PAGE_HIGGBITS_SHIFT + CTRLMEM_BITLOCK_LOCKBIT)))

#define CTRLMEM_PAGE_F_UPTODATE		((0x00000002UL) << CTRLMEM_PAGE_HIGGBITS_SHIFT)
#define CTRLMEM_PAGE_F_READAHEAD	((0x00000010UL) << CTRLMEM_PAGE_HIGGBITS_SHIFT)
#endif

#define __SYSPG_CACH_TYPE		1
#define __SYSPG_ANON_TYPE		2
#define __SYSPG_FILE_TYPE		3
#define __SYSPG_PTAB_TYPE		5
#define __SYSPG_HUGE_TYPE		9
#define __SYSPG_AHHP_TYPE		13
#define __SYSPG_FHHP_TYPE		14

#define __SYSPG_TYPE_MASK		0x0000000FU

#define __SYSPG_REFCOUNT_COUNTER_MAX	0x3fffffff
#define __SYSPG_REFCOUNT_COUNTER_MASK	0x3fffffffU

#define __SYSPG_F_FILEBACKED	0x00001000U

#define __SYSPG_F_SHMMBACKED	0x02000000U    /* page is shmm-backed page */

#define __SYSPG_L_CTRLMEM_MASK  0xffffff7fU

#define __SYSPG_F_MLOCKED		0x00000020U

#define __SYSPG_F_DCACHE_CLEAN  0x00002000U    /* page is markd as dcache clean */

#define __SYSPG_F_UNMOVABLE	0x00008000U    /* page can not be migrated */
#endif
