/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of ulibs flags
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 15:37:48 2019
 */

#ifndef ULIBS_INCLUDE_LIBMEM_MEM_FLAGS_H
#define ULIBS_INCLUDE_LIBMEM_MEM_FLAGS_H

#include <hmkernel/ctrlmem/kobjs/vregion.h>
#include <hongmeng/compiler.h>
#include <sys/mman.h>

/*
 * Protection attribute for hongmeng memory.
 * Compatible with POSIX PROT.
 */
#define MPROT_NONE			(unsigned int)PROT_NONE			/* 0 */
#define MPROT_READ			(unsigned int)PROT_READ			/* 1 */
#define MPROT_WRITE			(unsigned int)PROT_WRITE		/* 2 */
#define MPROT_EXEC			(unsigned int)PROT_EXEC			/* 4 */
#define MPROT_RWX_MASK			(unsigned int)__VREGION_MPROT_RWX_MASK /* mask for rwx properties */
#define MPROT_RX_MASK(prot)		((prot) & (MPROT_EXEC | MPROT_READ))

#define MPROT_SEM			(unsigned int)0x8

#define MPROT_NOCACHE			(unsigned int)__VREGION_MPROT_NOCACHE /* hongmeng PROT extensions 0x00000040 */

/*
 * In XPM feature, the anon is not allow mmap to PROT_EXEC. However, VDSO is a
 * special scenario, it allows anon flags memmgr_mmap to PROT_EXEC. In order to
 * handle this exceptional scenario, we add MPROT_VDSO, and XPM will not check
 * the memmgr_mmap with MPROT_VDSO prot. Note that, MPROT_VDSO is not allow use
 * in the user mode.
 */
#define MPROT_VDSO			0x00000020u
/* Keep compatible with POSIX PROT_BTI map flag */
#define MPROT_BTI			0x00000010u

/* hongmeng PROT bitlock */
#define MPROT_BITLOCK			0x00000100u /* prot shared with bitlock: lockbit */
#define MPROT_WAITER			0x00000200u /* prot shared with bitlock: waiter bit */

/* hongmeng PROT hkip */
#define MPROT_HKIP_ROWM			0x00001000u /* EL2 writable, EL1/EL0 RO */
#define MPROT_HKIP_REVOCABLE		0x00002000u
#define MPROT_HKIP_INIT_ONLY		0x00004000u
#define MPROT_HKIP			0x00008000u
#define MPROT_HKIP_MASK			0x0000F000u

#define MPROT_HKIP_SEC_PAGE		0x00010000u
#define MPROT_HKIP_SEC_PAGE_COW		0x00020000u
#define MPROT_HKIP_SEC_PAGE_MASK	0x00030000u

/* hongmeng PROT may_not xxx for mprotect */
#define MPROT_MAYNOT_READ		0x01000000u
#define MPROT_MAYNOT_WRITE		0x02000000u
#define MPROT_MAYNOT_EXEC		0x04000000u
#define MPROT_MAYNOT_SHARE		0x08000000u
#define MPROT_MAYNOT_RWX_MASK		0x07000000u
#define MPROT_MAYNOT_MASK		0x0F000000u

#define MPROT_DMA			0x80000000u

#ifdef CONFIG_MM_LB
#define PGPROT_LB_MASK 0xf000000000
#endif

/*
 * Hongmeng map flags.
 * Compatible with POSIX map flags, more needs to be added.
 */
#define MFLAG_MAP_FIXED			(unsigned int)MAP_FIXED			/* 0x10 */
#define MFLAG_MAP_FIXED_NOREPLACE	(unsigned int)MAP_FIXED_NOREPLACE	/* 0x100000 */
#define MFLAG_MAP_ANON			(unsigned int)MAP_ANON			/* 0x20 */
#define MFLAG_MAP_ANONYMOUS		(unsigned int)MAP_ANONYMOUS		/* 0x20 */
#define MFLAG_MAP_XPM			(unsigned int)0x40
#define MAP_XPM				(unsigned int)MFLAG_MAP_XPM
#define MFLAG_MAP_SHARED		(unsigned int)MAP_SHARED		/* 0x01 */
#define MFLAG_MAP_PRIVATE		(unsigned int)MAP_PRIVATE		/* 0x02 */
#define MFLAG_MAP_SHARED_VALIDATE   (unsigned int)MAP_SHARED_VALIDATE   /* 0x03 */
#define MFLAG_MAP_POPULATE		(unsigned int)MAP_POPULATE		/* 0x8000 */
#define MFLAG_MAP_JIT			(unsigned int)0x1000			/* 0x1000, reuse deprecated MAP_EXECUTABLE in posix */
#define MFLAG_MAP_LOCKED		(unsigned int)MAP_LOCKED		/* 0x2000 */
#define MFLAG_MAP_NORESERVE		(unsigned int)MAP_NORESERVE		/* 0x4000 */
#define MFLAG_MAP_NONBLOCK      (unsigned int)MAP_NONBLOCK      /* 0x10000 */
#define MFLAG_MAP_STACK			(unsigned int)MAP_STACK			/* 0x20000 */
#define MFLAG_MAP_SYNC          (unsigned int)MAP_SYNC          /* 0x80000 */
#define MFLAG_MAP_PURGEABLE		(unsigned int)0x4
#define MFLAG_MAP_USEREXPTE		(unsigned int)0x8
#define MFLAG_MAP_NOPOPULATE		(unsigned int)0x80000000 /* Do not allocate memory */

/* flags for huge page */
#define MFLAG_MAP_HUGETLB		(unsigned int)MAP_HUGETLB		/* 0x40000 */
#define MFLAG_MAP_HUGE_2MB		(unsigned int)MAP_HUGE_2MB		/* 21 << 26 */
#define MFLAG_MAP_HUGE_1GB		(unsigned int)MAP_HUGE_1GB		/* 30 << 26 */
#define MFLAG_MAP_HUGE_SHIFT		(unsigned int)MAP_HUGE_SHIFT		/* 26 */
#define MFLAG_MAP_HUGE_MASK		(unsigned int)MAP_HUGE_MASK		/* 0x3f */

/*
 * Hongmeng specific map flags.
 *
 * Cannot conflict with POSIX map flags.
 * Bits [26:31] encode the hugetlb page size.
 */
#define MFLAG_MAP_SHMM			(unsigned int)0x400000 /* map native shm of IOMEM type */
#define MFLAG_MAP_IOMM			(unsigned int)0x1000000 /* IOMEM type */
#define MFLAG_MAP_STATIC_SHM_IO		(unsigned int)0x2000000 /* IOMEM type with static shmm */
#define MFLAG_MAP_FILE			(unsigned int)0x800000  /* FILE type */
/*
 * MFLAG_MAP_IO_URING uses Bit[30] which conflicts with hugetlb reserve flags,
 * we must call MAP_IO_URING to make sure we are mapping io_uring.
 */
#define MFLAG_MAP_IO_URING		(unsigned int)0x40000000  /* IO_URING type */
#define MFLAG_MAP_VM			(unsigned int)0x400      /* mmap for virtual machine */
#define MFLAG_RLIMIT			(unsigned int)0x10000       /* whether need rlimit check */
#define MFLAG_MAP_ACTV			(unsigned int)0x80 /* actv stack mmap */
#define MFLAG_NOCOW			(unsigned int)0x200000  /* vregion is not CoWable */
#define MFLAG_MAP_GAP			(unsigned int)0x200     /* insert random gap between unfixed mmap vregions */
#define MFLAG_MAP_SAFE			(unsigned int)0x800     /* mapped into safe region */

/* flags used only when hughtlb is off */
#define MFLAG_MAP_RETRY			(unsigned int)0x10000000  /* try mmap retry when memory not enough */
/* flags used only when hughtlb is off */
#define MFLAG_MAP_PA_4G			(unsigned int)0x20000000 /* mmap for below 4G physical address */
/* flags used only when hughtlb is off */
#define MFLAG_MAP_VMUSER		(unsigned int)0x4000000   /* vmalloc_user */
#define MFLAG_MAP_GROWSDOWN		(unsigned int)0x100 /* mapping can grows down in specific situation */
#define MFLAG_MAP_ACTV_CACHE		(unsigned int)0x8000000			/* actv stack cache mmap */

#define MAP_FLAG_IS_VMUSER(flags) 	(((flags) & MFLAG_MAP_HUGETLB) == 0 && ((flags) & MFLAG_MAP_VMUSER) != 0)

#define MAP_IO_URING(flags) ((((flags) & MFLAG_MAP_IO_URING) != 0U) && (((flags) & MFLAG_MAP_HUGETLB) == 0U))

#define MAP_FLAG_IS_ACTV_CACHE(flags)						\
((((flags) & MFLAG_MAP_ACTV_CACHE) != 0U) &&					\
 (((flags) & MFLAG_MAP_HUGETLB) == 0U))						\

/* flags for mremap */
#define MREMAP_FLAG_MAYMOVE		MREMAP_MAYMOVE /* 1 */
#define MREMAP_FLAG_FIXED		MREMAP_FIXED   /* 2 */
#define MREMAP_FLAG_DONTUNMAP		MREMAP_DONTUNMAP   /* 4 */

/* flags for msync */
#define MSYNC_FLAG_SYNC			(unsigned int)MS_SYNC       /* 4 */
#define MSYNC_FLAG_ASYNC		(unsigned int)MS_ASYNC      /* 1 */
#define MSYNC_FLAG_INVALIDATE		(unsigned int)MS_INVALIDATE /* 2 */

/* flags for mlock2 */
#define MLOCK2_FLAG_ONFAULT		(unsigned int)MLOCK_ONFAULT     /* 0x01 */
/* flags for mlockall */
#define MLOCK_FLAG_CURRENT		(unsigned int)MCL_CURRENT	/* 1 */
#define MLOCK_FLAG_FUTURE		(unsigned int)MCL_FUTURE	/* 2 */
#define MLOCK_FLAG_ONFAULT		(unsigned int)MCL_ONFAULT /* 4 lock all pages that are faulted in */
#define MLOCK_FLAG_MASK			(unsigned int)0x7

/* flags for madvise */
#define MADV_FLAG_DONTNEED		POSIX_MADV_DONTNEED	/* 4 */
#define MADV_FLAG_FREE			MADV_FREE               /* 8 */
#define MADV_FLAG_COLD			MADV_COLD               /* 20 */
#define MADV_FLAG_PAGEOUT		MADV_PAGEOUT            /* 21 */
#define MADV_FLAG_REMOVE		MADV_REMOVE		/* 9 */
#define MADV_FLAG_DONTFORK		MADV_DONTFORK           /* 10 */
#define MADV_FLAG_DOFORK		MADV_DOFORK             /* 11 */
#define MADV_FLAG_DONTDUMP		MADV_DONTDUMP		/* 16 */
#define MADV_FLAG_DODUMP		MADV_DODUMP		/* 17 */
#define MADV_FLAG_WIPEONFORK		MADV_WIPEONFORK		/* 18 */
#define MADV_FLAG_KEEPONFORK		MADV_KEEPONFORK		/* 19 */
#define MADV_FLAG_NORMAL		POSIX_MADV_NORMAL       /* 0x00 */
#define MADV_FLAG_RANDOM		POSIX_MADV_RANDOM       /* 0x01 */
#define MADV_FLAG_SEQUENTIAL		POSIX_MADV_SEQUENTIAL   /* 0x02 */
#define MADV_FLAG_WILLNEED		POSIX_MADV_WILLNEED     /* 0x03 */
/*
 * reuse MFLAG_NOCOW as a madvise flag, MFLAG_NOCOW is a very
 * large number that will not conflict with future madvise flags
 */
#define MADV_FLAG_NOCOW			MFLAG_NOCOW /* 0x200000 */

/* flags for process_vm_readv/process_vm_writev */
#define MACCESS_VM_READ			0x00
#define MACCESS_VM_WRITE		0x01

/* native shm open flags, compatible with POSIX */
#define SHM_O_ACCMODE			(unsigned long long)0x03
#define SHM_O_RDONLY			(unsigned long long)0x00
#define SHM_O_WRONLY			(unsigned long long)0x01
#define SHM_O_RDWR			(unsigned long long)0x02
#define SHM_O_VM			(unsigned long long)0x04 /* Memory shared with virtual machine */
#define SHM_O_SYSVSHM			(unsigned long long)0x08 /* is sysvshm */

/* Hongmeng specific shm flags */
/* create shm with the specified physical address */
#define SHM_O_WITH_PA			(unsigned long long)0x10
/* keep the page in cell shm even though all mappings are remove */
#define SHM_O_CELL_KEEP			(unsigned long long)0x20
#define SHM_O_CREAT			(unsigned long long)0x40
#define SHM_O_EXCL			(unsigned long long)0x80
/* the shm_node won't be auto unlinked when creator exits,
 * which means the shm_node will be unlinked by someone else,
 * and creator itself should not unlink it.
 */
#define SHM_O_KEEP			(unsigned long long)0x100
/* allocate contiguous physical pages when creating shm */
#define SHM_O_CELL			(unsigned long long)0x200
/* allocate non-contiguous physical pages when creating shm */
#define SHM_O_POPULATE			(unsigned long long)0x400
/* shm hugetlb flags */
#define SHM_O_HUGETLB			(unsigned long long)0x800 /* compatible with SHM_HUGETLB */
#define SHM_O_NORESERVE			(unsigned long long)0x1000 /* compatible with SHM_NORESERVE */
/* allow page allocation failure and will not trigger oom if failed */
#define SHM_O_NO_OOM			(unsigned long long)0x2000
/* allow granted server to regrant shm to other processes */
#define SHM_O_GRANT_TRANS		(unsigned long long)0x4000
/*
 * Retry open shm node when memory not enough
 *
 * The open implementation _must_ retry infinitely: the caller
 * cannot handle allocation failures. The allocation could block
 * indefinitely but will never return with E_HM_NOMEM failure.
 * New users should evaluate carefullly(and the flag should be
 * used only when there is no reasonable failure policy).
 */
#define SHM_O_NOFAIL			(unsigned long long)0x8000
#define SHM_O_CONSUME_RESERVE		(unsigned long long)0x10000 /* consume reserve huge page when fill file page */
/* checks whether all physical memory of the shm is allocated */
#define SHM_O_PHYMEM_ALLOCATED		(unsigned long long)0x20000
/* allocate page shared between kernel and system services */
#define SHM_O_KSHM			(unsigned long long)0x80000
/* Do not allog from fs io */
#define SHM_O_NO_IO			(unsigned long long)0x100000
#define SHM_O_DMA			(unsigned long long)0x80000000
#define SHM_O_HUGE_SHIFT		(unsigned long long)MFLAG_MAP_HUGE_SHIFT /* 26 */
#define SHM_O_HUGE_MASK			(unsigned long long)MFLAG_MAP_HUGE_MASK
#define SHM_O_HUGE_2MB			(unsigned long long)MFLAG_MAP_HUGE_2MB
#define SHM_O_HUGE_1GB			(unsigned long long)MFLAG_MAP_HUGE_1GB

/* Shm node for binder share memory */
#define SHM_O_BINDER			(unsigned long long)0x1000000

/* native shm grant flags */
#define SHM_GRANT_ACCMODE		(unsigned int)0x03
#define SHM_GRANT_RDONLY		(unsigned int)0x00
#define SHM_GRANT_WRONLY		(unsigned int)0x01
#define SHM_GRANT_RDWR			(unsigned int)0x02
#define SHM_GRANT_UNLINK		(unsigned int)0x04   /* permission to unlink shm */
#define SHM_GRANT_TRANS			(unsigned int)0x08   /* permission to transfer authority to others */
#define SHM_GRANT_MASK 0xF
/* native shm alloc flags */
#define SHM_ALLOC_POPULATE		(unsigned long long)0x1
#define SHM_ALLOC_GLOBAL		(unsigned long long)0x2
/*
 * flags for iofast interface
 */
#define IOFAST_CONTINUOUS                0x1U /* pages must be continuity */
#define IOFAST_DETACH                    0x2U /* pages not map to vregion */
#define IOFAST_DMA                       0x4U /* pages are in low 4G */
#define IOFAST_NO_IO                     0x8U /* pages alloc no IO occurs */
#define IOFAST_ZERO                      0x10U /* alloc zero pages */
#define IOFAST_PGTBL                     0x20U /* alloc pgtbl pages */
#define IOFAST_PGD                       0x40U /* alloc pgd pages */
#define IOFAST_NO_SLOW                   0x80U /* maps to PAF_NO_SLOW */
#define IOFAST_NOWARN                    0x100U /* maps to PAF_NOWARN */
#define IOFAST_MAYSLOW                   0x400U /* allow slow alloc */
/*
 * flag for iomap
 *
 * devhost uses MFLAG_IOMAP_RECLAIM to notify sysmgr that
 * the virtual memory prepared in mmap can be reclaimed.
 */
#define MFLAG_IOMAP_RECLAIM		(unsigned int)0x1
/*
 * flag for iomap
 *
 * MFLAG_IOMAP_UPDATE is used to update iomem_id and rref in VR after mmap.
 */
#define MFLAG_IOMAP_UPDATE              (unsigned int)0x2
/*
 * flag for iomap type
 */
#define MFLAG_IOMAP_TYPE_DEFAULT	(unsigned int)0x1
#define MFLAG_IOMAP_TYPE_DMA_BUF	(unsigned int)0x2
#define MFLAG_IOMAP_TYPE_HVGR		(unsigned int)0x4

#endif
