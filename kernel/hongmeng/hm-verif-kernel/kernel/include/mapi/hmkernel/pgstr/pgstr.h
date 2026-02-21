/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Define type of pgstr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 07 18:05:13 2022
 */

#ifndef MAPI_HMKERNEL_PGSTR_PGSTR_H
#define MAPI_HMKERNEL_PGSTR_PGSTR_H

#include <hmkernel/kernel.h>
#include <uapi/hmkernel/pgstr/pgstr.h>

ENUM(56, pgstr_type_e, PGSTR_TYPE_,
	/* No memory area, which should not be used because an invalid pgstr entry represent NOMEM */
	NOMEM,
	/* Unused physical memory */
	NORMALPG,
	/* Used by page structure table */
	PGSTRTBL,
	/* Physical memory used by kernel */
	KERNEL,
	/* Used for ctrlmem: transparent kernel object and system call token */
	CTRLMEM,
	/* Used for IO */
	IOMEM,
	/* Reserved */
	RESERVED,
	/* Reserved memory during booting */
	BOOTDATA,
	/* Used for ctrlmem vspace */
	CTRLMEM_VSPACE,
	/*
	 * Reserved for contig memory. Can be mapped by uapp but don't allow trans to other types.
	 * We don't have to assign memory to CONTIG. Actually, we can manage CONTIG in sysmgr.
	 * Microkernel doesn't required to maintain CONTIG page type. In future we can remove
	 * CONTIG and merge it into NORMALPG.
	 */
	CONTIG,
	/* KShare udata. should be mapped to RO */
	KSHARE_UDATA,
	/* Page table: first level */
	PGTBL_FL,
	/* Page table: mid level 1 */
	PGTBL_ML1,
	/* Page table: mid level 2 */
	PGTBL_ML2,
	/* Page table: last level */
	PGTBL_ULL,
	/* Page table: last level in kernel mapping */
	PGTBL_KLL,
	/* IOMEM should not be mapped writable */
	IOMEM_RDONLY,
	/* IOMEM should not be mapped */
	IOMEM_NOMAP,
	/* Used for Virt-IO */
	IOMEM_VIRT,
	/* Kernel object container, RO */
	KOBJ_CONTAINER,
	/* SYSCONF */
	SYSCONF,
	/* SYSMGRPG, read write */
	SYSMGRPG,
	/* A page in sysmgr pagearray, read write */
	SYSPGARRPG,
	/* Sysmgr writable pcache page */
	SYSPCACHE,
	/* Global zero page */
	GLOBAL_ZERO,
	/* This page is used for oplog */
	OPLOG,
	/* GUEST_SHARED page */
	VM,
	/* Iterator, rw */
	ITERATOR,
	/* Tlb guarder, rdonly */
	TLBGATHER,
	/* sched info */
	COSCHED,
	/* Rdonly super vspace page */
	SUPERVS,
	/* Per-cpu channel pages */
	CHANNELPG,
	/* Writeable revents page */
	REVENTSPG,
	/*
	 * Hongmeng programmable system framework (hmpsf) page for modules and non-shared-memory maps.
	 * Sometimes abbreviated as PSF.
	 */
	HMPSF,
	/* Kernel code and data section */
	KERNELBIN,
	/*
	 * Hongmeng programmable system framework (hmpsf) page for shared-memory maps.
	 * Sometimes abbreviated as PSH.
	 */
	HMPSFSH,
	/* Writeable ULWT monitor page */
	ULWT,
	/* Rdonly percpu info page */
	PCPINFO,
	/* Used for track_info */
	TRACKINFO,
	/* Used for lastklog */
	LASTKLOG,
	/* Used for kbox */
	KBOX,
	/* Used for devhost coredump */
	COREDUMP,
	/* Used for public reserved memory */
	PUBRESV,
	/* Used for S4 snapshot trans from normal memory */
	SNAPSHOT_NML,
	/* Used for S4 snapshot trans from contig memory */
	SNAPSHOT_CTG,
	/* Iofast mem, trans from normal memmory */
	IOFAST,
	/* Iofast high mem, trans from highmem */
	IOFAST_HIGH,
	/* Iofast ctg mem, trans from contig memory */
	IOFAST_CTG,
	/* Smmu pt mem */
	SMMU_PT,
	/* Smmu pgd mem */
	SMMU_PGD,
	/* Rdonly verity hash page */
	VERITY_HASH,
	/*
	 * PAGES not mapped by kernel idmap, not protected by kdp.
	 */
	HIGHMEM,
	/* Rdonly fsverity signed code page */
	SIGNED_CODE,
	/* Rdonly dmverity signed code page */
	DM_CODE,

	/* Writeable pool's page, used for kernel page_pool */
	POOL_PAGE,

	/*
	 * Hongmeng probe (hmprobe) page for non-shared-memory maps.
	 */
	PROBE_PAGE,
)

#endif
