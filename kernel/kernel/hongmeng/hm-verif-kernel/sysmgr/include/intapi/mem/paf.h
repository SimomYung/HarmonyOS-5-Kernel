/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Define of page allocation flags
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 17 19:53:25 2022
 */
#ifndef SYSMGR_INTAPI_MEM_PAF_H
#define SYSMGR_INTAPI_MEM_PAF_H

#include <hongmeng/types.h>

/*
 * page type
 * Note: If you modify the page type, the g_page_type array
 *       should be updated synchronously.
 */
enum pg_type {
	PAGE_VOID,     /* free page */
	PAGE_CACH,     /* free page in cache */
	PAGE_ANON,     /* used for normal mapping */
	PAGE_FILE,     /* used for fscache in fusion mode */
	PAGE_SLAB,     /* used in slab */
	PAGE_PTAB,     /* used in page table and use palloc_page_table instead */
	PAGE_KOBJ,     /* used as kernel objects */
	PAGE_KSHR,     /* used for kshare */
	PAGE_KSHM,     /* used for pages shared between kernel and system services */
	PAGE_HUGE,     /* used for huge page */
	PAGE_HUGE_FILE, /* used for huge file page */
	PAGE_RAW,      /* used for driver by direct pa */
	PAGE_PREH,     /* used before memmgr init */
#ifdef CONFIG_HHP
	PAGE_AHHP,     /* used for anonymous hybrid huge page */
#endif
#ifdef CONFIG_F_HHP
	PAGE_FHHP,
#endif
	PAGE_SMMU_PT,  /* pgtbl and pgd pages used for driver by direct pa */
	__NR_PAGE_TYPE,
};

/* memory zone type */
enum zone_type {
#ifdef CONFIG_HIGH_MEM_ZONE
	ZONE_HIGH, /* zone to highmem zone */
#endif
	ZONE_MAIN, /* normal zone */
#ifdef CONFIG_HIGH_MEM_ZONE
	ZONE_KLINEAR,
#endif
#ifdef CONFIG_DMA_MEM_ZONE
	ZONE_DMA,  /* zone to support 32-bit device */
#endif
#ifdef CONFIG_CONTIGUOUS_MEM_ZONE
	ZONE_CMA,  /* continous memory zone */
#endif
	ZONE_PLUG, /* hotplug zone */
#ifdef CONFIG_PAGE_CACHE_ZONE
	ZONE_PGCACHE, /* page cache zone */
#endif
#ifdef CONFIG_USE_KERN_HIGHMEM
	ZONE_KERN,
#endif
#ifdef CONFIG_HKIP_SEC_PAGE
	ZONE_HKIP,
#endif
	NR_ZONE,
};

/* bit feilds define for node id, zone, type in paf */
#define PAF_NSHIFT		(uint32_t)28
#define PAF_NMASK		(uint32_t)0xF0000000
#define PAF_ZSHIFT		(uint32_t)24
#define PAF_ZMASK		(uint32_t)0x0F000000
#define PAF_TSHIFT		(uint32_t)0
#define PAF_TMASK		(uint32_t)0x0000000F

/* transform between paf and node id, zone type an page type */
#define _from_paf(x, m, s)	(((uint32_t)(x) & (m)) >> (s))
#define _to_paf(x, m, s)	(((uint32_t)(x) << (s)) & (m))
#define nid_from_paf(x)		((uint32_t)(x) > 0U ? _from_paf(x, PAF_NMASK, PAF_NSHIFT): 0)
#define nid_to_paf(x)		_to_paf((uint32_t)(x), PAF_NMASK, PAF_NSHIFT)
#define zone_from_paf(x)	((uint32_t)(x) > 0U ? _from_paf(x, PAF_ZMASK, PAF_ZSHIFT) - 1U : NR_ZONE)
#define zone_to_paf(x)		_to_paf(((uint32_t)(x) + 1U), PAF_ZMASK, PAF_ZSHIFT)
#define type_from_paf(x)	((uint32_t)(x) & PAF_TMASK)
#define type_to_paf(x)		((uint32_t)(x) & PAF_TMASK)

/* alloc zero page */
#define PAF_ZERO		(uint32_t)0x00000010
/* page cannot be migrated */
#define PAF_UNMOVE		(uint32_t)0x00000020
/* page cannot allocated from high zone */
#define PAF_UNHIGH		(uint32_t)0x00000040
/* Do not use this PAF directly!! */
/* able to use reserve memory for emergency */
#define __PAF_EMRG		(uint32_t)0x00000100
/* able to use reserve memory for core modules */
#define __PAF_CORE		(uint32_t)0x00000200
/* able to use reserve memory for system services */
#define __PAF_SERV		(uint32_t)0x00000400
/* it should add new scence in list below */
/* use (0, low] mem	EMGR scenes */
#define PAF_COREDUMP	__PAF_EMRG
#define PAF_SNAPSHOT	__PAF_EMRG
/* use (low, norm] mem	CORE scenes */
#define PAF_SLAB	__PAF_CORE
#define PAF_PGTABLE	__PAF_CORE
#define PAF_VR	__PAF_CORE
#define PAF_HIBERNATE	__PAF_CORE
#define PAF_MEMMGR	__PAF_CORE
/* use (norm, high] mem	SERV scenes */
#define PAF_PREH	__PAF_SERV

/* fail return instead of kill process when OOM */
#define PAF_RETRY_MAYFAIL	(uint32_t)0x00000800
/* alloc may come from or for kernel */
#define PAF_CTX_KERNEL		(uint32_t)(type_to_paf(PAGE_KOBJ))
/* alloc may on call chain of VFS */
#define PAF_CTX_VFS		(uint32_t)0x00001000
/* alloc may on call chain of devhost */
#define PAF_CTX_DEV		(uint32_t)0x00002000
/* not allow slow alloc */
#define PAF_NO_SLOW		(uint32_t)0x00004000
/* not allow io for alloc */
#define PAF_NO_IO		(uint32_t)0x00008000
/* could use cma zone to alloc */
#define PAF_USE_CMA		(uint32_t)0x00010000
/*
 * Do not exit until success for alloc memory
 * !!! WARNINGS: THIS FLAG IS VERY DANGEROUS !!!
 * In low-memory scenarios, the memory application
 * thread may be deadloop, causing the system to be hung
 * Do not use it if you don't know what consequences of it
 */
#define PAF_NO_FAIL		(uint32_t)0x00020000
/* not allow for actv call */
#define PAF_CTX_ACTV		(uint32_t)0x00040000
/* alloc for kernelmem */
#define PAF_KERN		(uint32_t)0x00080000
/* suppresses page alloc failed reports */
#define PAF_NOWARN		(uint32_t)0x00100000

/* not allowed to alloc from kernelmem when migrate a file page in kernel zone */
#define PAF_FILE_MIG_NOKERN	(uint32_t)0x00200000

/* alloc from compact */
#define PAF_COMPACT		(uint32_t)0x00400000
/*
 * Page allocation with PAF_ATOMIC avoids triggering reclaim, oom or sleep
 * and is high priority.
 */
#define PAF_ATOMIC		(__PAF_SERV | PAF_NO_SLOW | PAF_RETRY_MAYFAIL)

#ifdef CONFIG_DMA_MEM_ZONE
static inline uint32_t paf_from_dma_zone(void)
{
	return zone_to_paf(ZONE_DMA);
}
#elif defined(CONFIG_HIGH_MEM_ZONE)
static inline uint32_t paf_from_dma_zone(void)
{
	return zone_to_paf(ZONE_MAIN);
}
#else
static inline uint32_t paf_from_dma_zone(void)
{
	return 0;
}
#endif

#ifdef CONFIG_HKIP_SEC_PAGE
#define PAF_HKIP		(zone_to_paf(ZONE_HKIP) | PAF_UNMOVE | PAF_UNHIGH | PAF_NO_SLOW)
#define PAF_HKIP_EXECLUDED	(PAF_ZMASK | PAF_USE_CMA)
#else
#define PAF_HKIP (0)
#define PAF_HKIP_EXECLUDED (0)
#endif

#ifdef CONFIG_USE_KERN_HIGHMEM
#define PAF_KERN_EXECLUDED	(PAF_ZMASK)
#else
#define PAF_KERN_EXECLUDED	(0u)
#endif
#endif /* SYSMGR_INTAPI_MEM_PAF_H */
