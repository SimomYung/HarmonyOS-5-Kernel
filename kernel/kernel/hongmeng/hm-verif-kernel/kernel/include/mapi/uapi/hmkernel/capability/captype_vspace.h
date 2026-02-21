/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of vspace capability
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 20 07:06:46 2018
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(VSpace, 7, CAPALLGRANT(VSpace)
		   CAPNOGRANTMOVE(VSpace)
		   CAPRAWCREFRLOOKUP(VSpace)
		   CAPDEFAULTSIZE(VSpace),
	CAPMETHOD(VSpace, MapNormalPage)
	CAPMETHOD(VSpace, ReMapOnePfn)
	CAPMETHOD(VSpace, UnMap)
	CAPMETHOD(VSpace, PreparePgtbl)
	CAPMETHOD(VSpace, Shrink)
	CAPMETHOD(VSpace, MProtect)
	CAPMETHOD(VSpace, Scan)
	CAPMETHOD(VSpace, WriteTo)
	CAPMETHOD(VSpace, ReadFrom)
	CAPMETHOD(VSpace, FlushCache)
	CAPMETHOD(VSpace, MapKSharePage)
	CAPMETHOD(VSpace, FutexWake)

	CAPERRNO(VSpace, EHMERROR,	VSPACE_NOPT,		"E_HM_VSPACE_NOPT")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_NOPG,		"E_HM_VSPACE_NOPG")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_PG,		"E_HM_VSPACE_PG")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_PT,		"E_HM_VSPACE_PT")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_PG_IN_USE,	"E_HM_VSPACE_PG_IN_USE")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_PT_IN_USE,	"E_HM_VSPACE_PT_IN_USE")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_HINT,		"E_HM_VSPACE_HINT")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_LAYOUT,		"E_HM_VSPACE_LAYOUT")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_ONLINE,		"E_HM_VSPACE_ONLINE")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_IN_USE,		"E_HM_VSPACE_IN_USE")
	CAPERRNO(VSpace, EHMERROR,	VSPACE_NO_MASTER,	"E_HM_VSPACE_NO_MASTER")
	)

#ifdef CAP_EXPORT_API
#ifndef CAP_VSPACE_API_EXPORTED
#define CAP_VSPACE_API_EXPORTED

#include <hmasm/pgtbl/pgtblattr/model.h>
#include <hmasm/vspace.h>
#include <hmkernel/capability.h>
#include <hmkernel/mm/pgtable_layer.h>

#define __VSPACE_MPROTECTNG_FLAG_TRYBEST	1UL

enum {
	CRIGHT_VSPACE_SHRINK_BIT,
	CRIGHT_VSPACE_MPROTECT_BIT,
	CRIGHT_VSPACE_WRITE_TO_BIT,
	CRIGHT_VSPACE_FUTEX_WAKE_BIT,
	CRIGHT_VSPACE_IPCPROTECT_BIT,
	CRIGHT_VSPACE_MAX
};

#define CRIGHT_VSPACE_SHRINK	(1U << CRIGHT_VSPACE_SHRINK_BIT)
#define CRIGHT_VSPACE_MPROTECT	(1U << CRIGHT_VSPACE_MPROTECT_BIT)
#define CRIGHT_VSPACE_WRITE_TO	(1U << CRIGHT_VSPACE_WRITE_TO_BIT)
#define CRIGHT_VSPACE_FUTEX_WAKE	(1U << CRIGHT_VSPACE_FUTEX_WAKE_BIT)
#define CRIGHT_VSPACE_IPC_PROTECTED	(1U << CRIGHT_VSPACE_IPCPROTECT_BIT)

struct sysarg_vspace_config {
	__u32  is_privileged;
	__u32  has_linear_map;
	__u64  ctrlmem_vspace_slot_ptr;
	__u64  kobj_vspace_ptr;
	__u32  is_vm;
};

struct sysarg_vspace_pages {
	__u64 page_array;
	__u64 offset;
	__u32 array_size;
};

struct sysarg_vspace_map_pages {
	struct sysarg_vspace_pages pg;
	struct sysarg_vspace_pages pt;
};

struct sysarg_vspace_map {
	/*
	 * pfn_array is not exactly an array of page frame num,
	 * it contains a serial number of:
	 * pfn1, nrpfn1, pfn2, nrpfn2, ... pfnn, nrpfnn,
	 * we call the two u64 "pfn + nrpfn" a "pfn slot".
	 *
	 * array_size is the size of this array.
	 */
	__u64 pfn_array;
	__u32 array_size;

	/*
	 * When a pfn slot stands for a struct page who has an large
	 * order, it could be partially consumed because we limitted
	 * the max length for one map, then the next map will use
	 * this `offset`.
	 *
	 * `Offset` will also be used for output residue(kernel->user),
	 * for example, input contains 3 slots, each with 4 pages,
	 * | pfn1 |  4  | pfn2 |  4  | pfn3 |  4  |
	 * kernel consumed 10 pages, which is the first 2 slots and 2
	 * pages in the third slot, then output residue will contain:
	 * - pfn_array: address of the third slot
	 * - array_size: 2
	 * - offset: 2
	 */
	__u64 offset;
};

struct sysarg_vspace_range {
	__u64 start;
	__u64 end;
	struct __pgtable_key_attr_s keyattr;
};

struct sysarg_vspace_pgtblattr {
	enum __pgtblattr_modelid_e modelid;
	__bool try_best;
};

struct sysarg_vspace_shrink {
	__u64 stop_vaddr;
};

struct sysarg_vspace_unmap {
	__u64 next_vaddr;
};

struct sysarg_vspace_remap_one_pfn {
	cref_t vspace_cref;
	__u64 va;
	__u64 pfn;
	enum __pgtblattr_modelid_e modelid;
};

struct sysarg_vspace_mprotect {
	cref_t vspace_cref;
	enum __pgtblattr_modelid_e modelid;
	__u64 start;
	__u64 end;
	__u64 flags;
};

enum sysarg_page_type {
	__PAGE_TRANS_USER,
	__PAGE_TRANS_KSHARE_UDATA,
	__PAGE_TRANS_KOBJ,
	__PAGE_TRANS_NR_TYPE,
};

#endif
#endif
