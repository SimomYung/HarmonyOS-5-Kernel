/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of vregion flags
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 22 11:44:53 2020
 */
#ifndef SYSMGR_MEMMGR_MEM_VR_FLAGS_H
#define SYSMGR_MEMMGR_MEM_VR_FLAGS_H

/*
vregion flags used and usable:
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 55             | 54            | 53             | 52             | 51         | 50        | 49           | 48         |
|                |               |                |                |            |HHP_PTE_RO | VR_SKIP_MERGE| VR_HHP     |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 47             | 46            | 45             | 44             | 43         | 42        | 41           | 40         |
| ONLINE         | VR_AMA_MIGRATE| VR_HUGEPAGE    | VMALLOC_REMAP  | ACTV_CACHE | VM_JIT    | IN_COPGTBL   | WIPEONFORK |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 39             | 38            | 37             | 36             | 35         | 34        | 33           | 32         |
| GROWSDOWN      |               | PERSISTED      | UNMAPPING      | USERXPTE   | PURGEABLE | VMUSER       | NOFORK     |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 31             | 30            | 29             | 28             | 27         | 26        | 25           | 24         |
| HKIP_REVOCABLE | HKIP_ROWM     | HKIP_PROTECTED | HKIP_INITED    | FILE_WR    | VM_XPM    | WRITETAINTED | SAFE       |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 23             | 22            | 21             | 20             | 19         | 18        | 17           | 16         |
| LOCK_ONFAULT   | STATIC_SHM_IO | ANON           | SHMM           | DONTDUMP   | POPULATE  | PA_4G        | STACK      |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 15             | 14            | 13             | 12             | 11         | 10        | 9            | 8          |
| HUGETLB        | COWED         | NOCOW          | IOMAP_INIT     | RAND_READ  | SEQ_READ  | backend      | backend    |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+
| 7              | 6             | 5              | 4              | 3          | 2         | 1            | 0          |
| backend        | backend       | ACTV           | VM             | LOCKED     | NORESERVE | PRIVATE      | SHARED     |
+----------------+---------------+----------------+----------------+------------+-----------+--------------+------------+

[6-9] bits represent 16 numbers for each backend, see comment in each VR_BACKEND_XXX
*/

#include <hmkernel/ctrlmem/kobjs/vregion.h>

#define VR_SHARED			__ULL(__VREGION_FLAGS_SHARED)  /* 0x1 shared changes */
#define VR_PRIVATE			__ULL(__VREGION_FLAGS_PRIVATE)  /* 0x2 private changes */
#define VR_NORESERVE			0x00000004ULL  /* don't check for reservations */
#define VR_LOCKED			__ULL(__VREGION_FLAGS_LOCKED) /* lock the mapping */

#define VR_VM				0x00000010ULL  /* mmap for virtual machine */
#define VR_ACTV				0x00000020ULL  /* actv stack mmap */
/* vregion flags of different backend types */
#define VR_BACKEND_MASK			(unsigned long long)(__VREGION_FLAGS_BACKEND_MASK)
#define VR_BACKEND_NONE			0x0ULL    /* reserve memory */
#define VR_BACKEND_ANON			(unsigned long long)(__VREGION_FLAGS_BACKEND_ANON)  /* 0x040 anonymous memory */
#define VR_BACKEND_FILE			(unsigned long long)(__VREGION_FLAGS_BACKEND_FILE)  /* 0x080 file backed memory */
#define VR_BACKEND_IOMM			(0x3ULL << 6)    /* 0x0C0 IO backed memory */
#define VR_BACKEND_PREH			(0x4ULL << 6)    /* 0x100 prehistoric memory, mapped before vspace created */
#define VR_BACKEND_KSHARE		(0x5ULL << 6)    /* 0x140 kshare memory */
#define VR_BACKEND_SHMM			(0x7ULL << 6)    /* 0x1C0 native share memory */
#define VR_BACKEND_CONTIG		(0x8ULL << 6)    /* 0x200 specific contig PA mapping */
#ifdef CONFIG_HMKIT_FDOFS
#define VR_BACKEND_VNODES		(0x9ULL << 6)    /* 0x240 fdofs: vnodes backend memory */
#endif
#define VR_BACKEND_IO_URING		(0xAULL << 6)    /* io_uring backed memory */
#define VR_BACKEND_VMAP			(0xBULL << 6) /* vmap backed memory */
#define VR_BACKEND_BINDER		(0xCULL << 6)    /* 0x300 binder backed memory */
#define VR_BACKEND_EXTRACT(x)		(unsigned long long)(__VREGION_FLAGS_BACKEND_EXTRACT(x))

/* flags used for madvise */
#define VR_SEQ_READ			0x00000400ULL  /* expect page references in sequential order */
#define VR_RAND_READ			0x00000800ULL  /* expect page references in random order */

/* flag for iomem, the corresponding vregion cant be released if set */
#define VR_IOMAP_INIT			0x00001000ULL

#define VR_NOCOW			__ULL(__VREGION_FLAGS_NOCOW)  /* 0x00002000 vregion is not CoWable */
#define VR_COWED			__ULL(__VREGION_FLAGS_COWED)  /* 0x00004000 vregion has been CoWed */
#define VR_HUGETLB			__ULL(__VREGION_FLAGS_HUGETLB) /* 0x00008000 flags for huge page */

#define VR_STACK			0x00010000ULL  /* map the stack */
#define VR_PA_4G			0x00020000ULL  /* mmap for below 4G physical address */
#define VR_POPULATE			0x00040000ULL  /* populate (prefault) pagetables */
#define VR_DONTDUMP			0x00080000ULL  /* don't include vregion in core dump */

#define VR_SHMM				__ULL(__VREGION_FLAGS_SHMM)  /* 0x00100000 vregion is shmm */
#define VR_ANON				__ULL(__VREGION_FLAGS_ANON)  /* 0x00200000 vregion is anonymous */
#define VR_STATIC_SHM_IO		0x00400000ULL  /* IOMEM type with static shmm */
#define VR_LOCK_ONFAULT			0x00800000ULL  /* lock the mapping and popualte page on page fault */

#define VR_SAFE				0x01000000ULL  /* vregion is in sfi safe region */

#ifdef CONFIG_MEM_PURGEABLE
/* flags for purgeable memory */
#define VR_PURGEABLE			0x400000000ULL  /* vregion is purgeable */
#define VR_USEREXPTE			0x800000000ULL  /* vregion is user expte for purgeable page */
#else /* !CONFIG_MEM_PURGEABLE */
#define VR_PURGEABLE			0x0ULL  /* vregion is purgeable */
#define VR_USEREXPTE			0x0ULL  /* vregion is user expte for purgeable page */
#endif /* CONFIG_MEM_PURGEABLE */

/* flags for xpm */
#define VR_WRITETAINTED			0x02000000ULL  /* vregion is write tainted */
#define VR_VM_XPM			0x04000000ULL  /* vregion flags of mapped into xpm region */
#define VR_FILE_WR			0x08000000ULL  /* file vregion is write tainted */

#ifdef CONFIG_HKIP_PROTECT_EL0
/* vregion flags for different hkip protect, rowm: EL2 writable, EL1/EL0 RO */
#define VR_HKIP_MASK            (0xfULL << 28)
#define VR_HKIP_STATUS_MASK     (0x3ULL << 28)
#define VR_HKIP_TYPE_MASK       (0xcULL << 28)
#define VR_HKIP_DISABLED        (0x0ULL)        /* vregion cannot invoke hkip api */
#define VR_HKIP_INITED          (0x1ULL << 28)  /* 0x10000000 vregion can invoke hkip api */
#define VR_HKIP_PROTECTED       (0x2ULL << 28)  /* 0x20000000 vregion protected hkip api */
#define VR_HKIP_ROWM            (0x4ULL << 28)  /* 0x40000000 bitfield: vregion is mapped as 1: rowm, 0: ro */
#define VR_HKIP_REVOCABLE       (0x8ULL << 28)  /* 0x80000000 bitfield: vregion is 1: revocable, 0: irrevocable */
#define VR_HKIP_STATUS(x)       ((x) & VR_HKIP_STATUS_MASK)
#define VR_HKIP_TYPE(x)         ((x) & VR_HKIP_TYPE_MASK)
#endif

#define VR_NOFORK			0x100000000ULL /* vregion that shuold not be forked */
/* vregion flags of vmalloc user */
#define VR_VMUSER			0x200000000ULL

#define VR_UNMAPPING			__VREGION_FLAGS_UNMAPPING /* 0x1000000000ULL */
#define VR_PERSISTED_MAP		0x2000000000ULL /* persisted mapping */
#define VR_GROWSDOWN			0x8000000000ULL /* see MAP_GROWSDOWN */
#define VR_WIPEONFORK			0x10000000000ULL
#define VR_IN_COPGTBL			0x20000000000ULL /* vregion has page in jitfort copgtable */
#define VR_VM_JIT			0x40000000000ULL /* vregion that holds jit memory */
#define VR_ACTV_CACHE			0x80000000000ULL /* actv cache stack mmap */
#define VR_VMALLOC_REMAP		0x100000000000ULL /* for vmalloc remap */
#define VR_HUGEPAGE			0x200000000000ULL /* transparent huge page */
#define VR_AMA_MIGRATE			0x400000000000ULL /* ama2.0 migrate */

#define VR_ONLINE			0x800000000000ULL /* vregion is on the rbtree or not */
#define VR_HHP				0x1000000000000ULL /* vregion supports hhp */
#define VR_SKIP_MERGE			0x2000000000000ULL /* skip merge, workaround, can be removed when the pgtable lock support */
/* the ptes of the vregion are all read only, remove the flags when forkvs online */
#define VR_HHP_PTE_RO			0x4000000000000ULL
/* vregion is or not accounted as committed memory */
#define VR_IS_COMMITTED_ACCOUNTABLE(x)   (!((x) & (VR_HUGETLB | VR_NORESERVE | VR_SHARED)))

/*
 * Optimisation macro.  It is equivalent to: ((((x) & (bit1)) != 0U) ? (bit2) : 0U)
 * but this version is faster.
 * ("bit1" and "bit2" must be single bits)
 */
#define _convert_flag(x, bit1, bit2) \
	((((bit1) == 0U) || ((bit2) == 0U)) ? 0 : \
	 ((bit1) <= (bit2) ? ((x) & (bit1)) * ((bit2) / (bit1)) \
	  : ((x) & (bit1)) / ((bit1) / (bit2))))
#endif
