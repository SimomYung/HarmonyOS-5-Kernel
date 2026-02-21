/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for vregion object
 * Author: Huawei OS Kernel Lab
 * Created: Fri Apr 21 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VREGION_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VREGION_H

#include <hmkernel/ctrlmem/kobjs/vrcache.h>
#include <hmkernel/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>
#include <hmasm/pgtbl/pgtblattr/model.h>
#include <hmkernel/mm/pgtable.h>
#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmasm/memory.h>

/*
 * Protection attribute for hongmeng memory.
 * Compatible with POSIX PROT.
 */
#define __VREGION_MPROT_NONE			0    /* 0, PROT_NONE */
#define __VREGION_MPROT_READ			1    /* 1, PROT_READ */
#define __VREGION_MPROT_WRITE			2    /* 2, PROT_WRITE */
#define __VREGION_MPROT_EXEC			4    /* 4, PROT_EXEC */
#define __VREGION_MPROT_RWX_MASK		0x7  /* mask for rwx properties */
/* hongmeng PROT extensions */
#define __VREGION_MPROT_NOCACHE			0x00000040
#define __VREGION_MPROT_BTI				0x20
#define __VREGION_MPROT_MASK			0xf    /* mask for vregion prot properties */
#define __VREGION_MPROT_BITLOCK			0x100  /* mprot shared with bitlock: lockbit */
#define __VREGION_MPROT_WAITER			0x200  /* mprot shared with bitlock: waiter bit */

#define __VREGION_FLAGS_SHARED			0x00000001  /* shared changes */
#define __VREGION_FLAGS_PRIVATE			0x00000002  /* private changes */
#define __VREGION_FLAGS_LOCKED			0x00000008  /* lock the mapping */

#define __VREGION_FLAGS_HHP			0x00001000  /* vregion support hhp */
#define __VREGION_FLAGS_NOCOW			0x00002000  /* vregion is not CoWable */
#define __VREGION_FLAGS_COWED			0x00004000  /* vregion has been CoWed */

/* flags for huge page */
#define __VREGION_FLAGS_HUGETLB			0x00008000

#define __VREGION_FLAGS_SHMM			0x00100000  /* vregion is shmm */
#define __VREGION_FLAGS_ANON			0x00200000  /* vregion is anonymous */

#define __VREGION_FLAGS_BACKEND_MASK		(0xf << 6)
#define __VREGION_FLAGS_BACKEND_ANON		(0x1 << 6)  /* anonymous memory */
#define __VREGION_FLAGS_BACKEND_FILE		(0x2 << 6)  /* file backed memory */
#define __VREGION_FLAGS_BACKEND_EXTRACT(x)	((x) & __VREGION_FLAGS_BACKEND_MASK)

#define __VREGION_FLAGS_UNMAPPING		0x1000000000ULL

#define __VREGION_VROBJCACHE_LOCKABLE		0UL
#define __VREGION_VROBJCACHE_UNLOCKABLE		1UL

#define __VREGION_FLAGS_GZP_1		0x200042
#define __VREGION_FLAGS_GZP_2		0x204042

#define __VREGION_FLAGS_ANON_1	0x200042
#define __VREGION_FLAGS_ANON_2	0x204042
#define __VREGION_FLAGS_ANON_3	0x202042
#define __VREGION_FLAGS_ANON_4	0x200046
#define __VREGION_FLAGS_ANON_5	0x204046
#define __VREGION_FLAGS_ANON_6	0x202046

#define __VREGION_FLAGS_HHP_1	0x201042

#define __VREGION_FLAGS_FILE_1	0x81
#define __VREGION_FLAGS_FILE_2	0x82
#define __VREGION_FLAGS_FILE_3	0x4082

#define __VREGION_KPF_NOT_CARE_FLAG_MASK 0xf1ffffff
#define __VREGION_FLAGS_HHP_MASK 0x201042

struct __ctrlmem_kobj_vregion_node_s {
#if __SIZEOF_LONG__ == 4
	__u32 padding;
	__u32 left;
	__u32 right;
#else
	__u64 padding;
	__u64 left;
	__u64 right;
#endif
};

struct __ctrlmem_kobj_vregion_data_anonbk_s {
#if __SIZEOF_LONG__ == 4
	__u32 vrgptr;
	__u32 padding[2];
#else
	__u64 vrgptr;
	__u64 padding[2];
#endif
};

struct __ctrlmem_kobj_vregion_data_filebk_fscache_s {
#if __SIZEOF_LONG__ == 4
	__u32 vrgptr;
#else
	__u64 vrgptr;
#endif

#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
#if __SIZEOF_LONG__ == 4
	__u32 mapping;
#else
	__u64 mapping;
#endif
#endif
};

struct __ctrlmem_kobj_vregion_data_filebk_s {
#if __SIZEOF_LONG__ == 4
	__u32 vrgptr;
	__u32 fcache_ptr;
	__bool is_dax;
	__bool is_seal_future_write;
	__bool hhp;
	__u32 cnode_idx;
	__u32 fid;
#else
	__u64 vrgptr;
	__u64 fcache_ptr;
	__bool is_dax;
	__bool is_seal_future_write;
	__bool hhp;
	__u32 cnode_idx;
	__u64 fid;
#endif
};

union __ctrlmem_kobj_vregion_data_backend_u {
	struct __ctrlmem_kobj_vregion_data_anonbk_s anonbk;
	struct __ctrlmem_kobj_vregion_data_filebk_s filebk;
	struct {
		__u32 padding1;
		__u64 padding2;
		__u64 padding3;
		__u64 padding4;
		__u64 padding5;
	} iomem_padding;
};

struct __ctrlmem_kobj_vregion_s {
	struct __ctrlmem_kobj_vregion_node_s node;
	__vaddr_t start;
	__vaddr_t end;
	__u64 flags;
	__u32 prot;
	__u64 pos;
	union __ctrlmem_kobj_vregion_data_backend_u bkdata;
};

/*
 * Give userspace a way to inject its own atomic type.
 * Kernel use its own "struct ctrlmem_vspace_slot_s".
 */
#ifndef __uatomic64
# ifndef __KERNEL__
/*
 * Set a struct to missing atomic type to make sure code
 * which doesn't care about type of it generate compiling
 * error if it access this type.
 */
struct __ctrlmem_kobj_vregion_uatomic64_s {
	__u64 donttouch;
};
typedef struct __ctrlmem_kobj_vregion_uatomic64_s __ctrlmem_kobj_vregion_uatomic64_t;
#  define __uatomic64 __ctrlmem_kobj_vregion_uatomic64_t
# else
#  include <lib/atomic.h>
#  define __uatomic64 vatomic64_t
# endif
#endif

#ifndef __uatomic32
# ifndef __KERNEL__
/*
 * Set a struct to missing atomic type to make sure code
 * which doesn't care about type of it generate compiling
 * error if it access this type.
 */
struct __ctrlmem_kobj_vregion_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_kobj_vregion_uatomic32_s __ctrlmem_kobj_vregion_uatomic32_t;
#  define __uatomic32 __ctrlmem_kobj_vregion_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif
#endif

enum __ctrlmem_kobj_phymem_type {
	__CTRLMEM_MEM_PRIVATE_ANON,    /* anon private physical memory size */
	__CTRLMEM_MEM_COW_FILE,        /* file copy on write physical memory size */
	__CTRLMEM_MEM_SHARED_FILE,     /* shared file physical memory size */
	__CTRLMEM_MEM_SHMEM,           /* size of share physical memory size */
	__CTRLMEM_NR_MEM_TYPES,
};

struct __ctrlmem_kobj_phymem_stat_s {
	__u64 counter[__CTRLMEM_NR_MEM_TYPES];
};

/*
 * a per vspace structure, usage: store rbt root, kernel/sysmgr lock, statistic data, etc.
*/
struct __ctrlmem_kobj_vregion_shared_s {
#if __SIZEOF_POINTER__ == 4
	__u32 vmem_list_prev;
	__u32 vmem_list_next;
	__u32 rbt_root;
	__u32 sumry_upd_cb;
#else
	__u64 vmem_list_prev;
	__u64 vmem_list_next;
	__u64 rbt_root;
	__u64 sumry_upd_cb;
#endif
	struct __ctrlmem_kobj_rwlock_s rwlock;
	struct __ctrlmem_kobj_vrcache_s vrcache;
	__bool mlockall;
	__bool mlockall_onfault;
	__bool swapless;
	__uatomic32 kpfskip;
	struct __ctrlmem_kobj_phymem_stat_s phymem_stat;
	struct __ctrlmem_kobj_fspgcache_fscache_stat_s *fscache_stat;
};

static inline __always_inline enum __pgtblattr_modelid_e __prot_to_modelid(__u32 prot)
{
	enum __pgtblattr_modelid_e modelid;
	switch (prot & __VREGION_MPROT_MASK) {
		case (__VREGION_MPROT_EXEC | __VREGION_MPROT_WRITE | __VREGION_MPROT_READ):
		case (__VREGION_MPROT_EXEC | __VREGION_MPROT_WRITE):
			modelid = __PGTBLATTR_MODELID(UWCODE);
			break;
		case (__VREGION_MPROT_EXEC | __VREGION_MPROT_READ):
#if __SIZEOF_POINTER__ == 4
			modelid = __PGTBLATTR_MODELID(UCODE);
#else
			modelid = (prot & __VREGION_MPROT_BTI) ? __PGTBLATTR_MODELID(UPCODE) : __PGTBLATTR_MODELID(UCODE);
#endif
			break;
		case (__VREGION_MPROT_EXEC):
#if __SIZEOF_POINTER__ == 4
			modelid = __PGTBLATTR_MODELID(UCODE);
#else
			modelid = __PGTBLATTR_MODELID(XOM);
#endif
			break;
		case (__VREGION_MPROT_NOCACHE | __VREGION_MPROT_WRITE | __VREGION_MPROT_READ):
		case (__VREGION_MPROT_NOCACHE | __VREGION_MPROT_WRITE):
			modelid = __PGTBLATTR_MODELID(UNCDATA);
			break;
		case (__VREGION_MPROT_NOCACHE | __VREGION_MPROT_READ):
			modelid = __PGTBLATTR_MODELID(UNCRODATA);
			break;
		case (__VREGION_MPROT_WRITE | __VREGION_MPROT_READ):
		case __VREGION_MPROT_WRITE:
			modelid = __PGTBLATTR_MODELID(UDATA);
			break;
		case __VREGION_MPROT_READ:
			modelid = __PGTBLATTR_MODELID(URODATA);
			break;
		case __VREGION_MPROT_NONE:
			modelid = __PGTBLATTR_MODELID(NONEMAP);
			break;
		default:
			modelid = __PGTBLATTR_MODELID(INVAL);
			break;
	}

	return modelid;
}

#endif
