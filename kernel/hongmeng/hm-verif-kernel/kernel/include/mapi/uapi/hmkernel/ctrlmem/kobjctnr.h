/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file of kernel object container (ctnr) - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 28 09:52:44 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJCTNR_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJCTNR_H

#include <hmkernel/const.h>
#include <hmkernel/types.h>
#include <lib/utils.h>

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_kobjctnr_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_kobjctnr_uatomic32_s __ctrlmem_kobjctnr_uatomic32_t;
#  define __uatomic32 __ctrlmem_kobjctnr_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif
#endif

/*
 * Internal structure:
 * +-------------------+-------------------------------------------------------------------------------------------+
 * | 1 bit for locking |                       31 bits for refcnt                                                  |
 * +-------------------+-------------------------------------------------------------------------------------------+
 */

#define __KOBJSTATEV_MASK_LOCK		0x80000000u
#define __KOBJSTATEV_MASK_REFCNT	0x7fffffffu

#define __KOBJSTATEV_SHIFT_LOCK		31u
#define __KOBJSTATEV_SHIFT_REFCNT	0u

struct __ctrlmem_kobjctnr_kobj_header_s {
	__uatomic32 state_refcnt;
};

struct __ctrlmem_kobjctnr_freelist_node_s {
	__u8 prev;
	__u8 next;
};

#define __CTRLMEM_KOBJCTNR_PAGE_SZ		4096
#define __CTRLMEM_KOBJCTNR_PIECE_SZ		16
#define __CTRLMEM_KOBJCTNR_HEADER_SZ		8

/*
 * Smallest controllable piece of memory. Size of kernel
 * object should be multiple of size of piece and aligned
 * to a piece.
 */
struct __ctrlmem_kobjctnr_piece_s {
	union {
		__u64 data[__CTRLMEM_KOBJCTNR_PIECE_SZ / sizeof(__u64)];
		struct __ctrlmem_kobjctnr_kobj_header_s kobj_header;
		struct __ctrlmem_kobjctnr_freelist_node_s freelist_node;
	} u;
};

/*
 * 8 bytes metadata, can be used as container
 * metadata and a pointer t next container.
 */
struct __ctrlmem_kobjctnr_page_header_s {
	union {
		__u8 data[__CTRLMEM_KOBJCTNR_HEADER_SZ];
		struct {
			__u64 v;
		} v64;
		struct {
			__u32 v1;
			__u32 v2;
		} v32;
		struct {
			void *v;
		} vptr;
		__u64 meta;
	} u;
};

/*
 * head is not *head* of linked list. freelist is a double linked
 * list linked as a circel, no head or tail defined. Following 'head'
 * points to any entry in that list. We call it head for trandition.
 */
#define __CTRLMEM_KOBJCTNR_FREELISTS_NR	8
#define __CTRLMEM_KOBJCTNR_NFREELIST_INVALID		__CTRLMEM_KOBJCTNR_FREELISTS_NR
#define __ctrlmem_kobjctnr_nfreelist_is_valid(n)	((n) < __CTRLMEM_KOBJCTNR_FREELISTS_NR)

struct __ctrlmem_kobjctnr_page_freelist_s {
	__u8 head;
};

/*
 * pieces_nr = ((page_sz - header_sz) - pieces_nr) / piece_sz
 * pieces_nr = (page_sz - header_sz) / (piece_sz + 1) = (4096 - 16) / (16 + 1) = 240
 */
#define __CTRLMEM_KOBJCTNR_PIECES_NR	240
#define __CTRLMEM_KOBJCTNR_NPIECE_INVALID	__CTRLMEM_KOBJCTNR_PIECES_NR
#define __ctrlmem_kobjctnr_npiece_is_valid(n)	((n) < __CTRLMEM_KOBJCTNR_PIECES_NR)

struct __ctrlmem_kobjctnr_meta_s {
	__u8 v;
};

#define __CTRLMEM_BKOBJCTNR_LIST_HEAD_NPIECE             0u
#define __CTRLMEM_BKOBJCTNR_LIST_HEAD_SZ_IN_PIECES       1u

struct __ctrlmem_kobjctnr_page_s {
	struct __ctrlmem_kobjctnr_page_header_s		header;
	struct __ctrlmem_kobjctnr_page_freelist_s	freelists[__CTRLMEM_KOBJCTNR_FREELISTS_NR];
	struct __ctrlmem_kobjctnr_meta_s		metas[__CTRLMEM_KOBJCTNR_PIECES_NR];
	union {
		struct __ctrlmem_kobjctnr_piece_s	pieces[__CTRLMEM_KOBJCTNR_PIECES_NR];
		/*
		 * When booting, kernel deliver first bkobjctnr page to sysmgr through
		 * CTRLMEM section. Following pages are linked in a list through
		 * bkobjctnr_next_page_pa.
		 */
		__u64					bkobjctnr_next_page_pa;
	};
};

/*
 * When page is empty, default freelists viewed as u64.
 */
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
# define __CTRLMEM_KOBJCTNR_FREELISTS_ALLCLEAR_U64	0x70301000f0f0f0f0
#else
# define __CTRLMEM_KOBJCTNR_FREELISTS_ALLCLEAR_U64	0xf0f0f0f000103070
#endif
/*
 * Call it ceiling, not MAX because MAX are all exclusive in this header.
 * kobj can have at most 128 pieces in size, __CTRLMEM_KOBJCTNR_KOBJSZ_MAX
 * would be 129, but it is confusing.
 */
#define __CTRLMEM_KOBJCTNR_KOBJSZ_CEILING		128

#define __CTRLMEM_KOBJCTNR_KOBJSZ_VSPACE	8
#define __CTRLMEM_KOBJCTNR_KOBJNFL_VSPACE	3

#define __CTRLMEM_KOBJCTNR_KOBJSZ_PROCESS	16
#define __CTRLMEM_KOBJCTNR_KOBJNFL_PROCESS	4

#define __CTRLMEM_KOBJCTNR_KOBJSZ_THREAD	128
#define __CTRLMEM_KOBJCTNR_KOBJNFL_THREAD	7

#define __CTRLMEM_KOBJCTNR_KOBJSZ_COFILE	64
#define __CTRLMEM_KOBJCTNR_KOBJNFL_COFILE	6

#define __CTRLMEM_KOBJCTNR_KOBJSZ_PCACHE	4
#define __CTRLMEM_KOBJCTNR_KOBJNFL_PCACHE	2

#define __CTRLMEM_KOBJCTNR_KOBJSZ_SYSPGARR	2
#define __CTRLMEM_KOBJCTNR_KOBJNFL_SYSPGARR	1

#define __CTRLMEM_KOBJCTNR_KOBJSZ_VSPACEEXT	2
#define __CTRLMEM_KOBJCTNR_KOBJNFL_VSPACEEXT	1

#define __CTRLMEM_KOBJCTNR_KOBJSZ_POLLLIST	4
#define __CTRLMEM_KOBJCTNR_KOBJNFL_POLLLIST	2

#define __CTRLMEM_KOBJCTNR_KOBJSZ_ULWT		64
#define __CTRLMEM_KOBJCTNR_KOBJNFL_ULWT		6

#define __CTRLMEM_KOBJCTNR_KOBJSZ_SYSEVENT	1
#define __CTRLMEM_KOBJCTNR_KOBJNFL_SYSEVENT	0

#define __CTRLMEM_KOBJCTNR_KOBJSZ_HMPSFMOD	16
#define __CTRLMEM_KOBJCTNR_KOBJNFL_HMPSFMOD	4

#define __CTRLMEM_KOBJCTNR_KOBJSZ_HMPSFMAP	16
#define __CTRLMEM_KOBJCTNR_KOBJNFL_HMPSFMAP	4

#define __CTRLMEM_KOBJCTNR_KOBJSZ_ACTV_FRAME	8
#define __CTRLMEM_KOBJCTNR_KOBJNFL_ACTV_FRAME	3

#define __CTRLMEM_KOBJCTNR_KOBJSZ_FSVERITY	32
#define __CTRLMEM_KOBJCTNR_KOBJNFL_FSVERITY	5

#define __CTRLMEM_KOBJCTNR_KOBJSZ_ACTV_BINDER_CONTEXT  4
#define __CTRLMEM_KOBJCTNR_KOBJNFL_ACTV_BINDER_CONTEXT 2

#define __CTRLMEM_KOBJCTNR_KOBJSZ_ACTV_BINDER_REFER 4
#define __CTRLMEM_KOBJCTNR_KOBJNFL_ACTV_BINDER_REFER 2

/*
 * Meaning of meta.
 *
 * First 9 number assigned to freelist. 0-7 represent corresponding piece is
 * head of freelist with order x.
 *
 * Last 127 number represent to the offset to the first piece of the object.
 * We allow the largest object takes 128 pieces, so offset is -1 ~ -127,
 * which is 0xff, 0xfe, ..., 0x81.
 *
 * Number 0x09 - 0x80 (inclusive) can be assigned to object. We left 0x80
 * to special use, so totally we support at most 119 kobj types.
 *
 * We assign 0x80 specilly as uninited kernel object.
 */

/*
 * Rules locking:
 *
 * Use pgstr lock to protect freelist, use meta to protect each kobj's lifecycle.
 * *FIXME* vsync doesn't support atomic bit8, so temporary use pgstr's lock to
 * protect kobj.
 *
 * 1.freelist must be protected by pgstr lock. In critical section, only freelist.c handles
 *   all metas. In pgstr lock critical section, meta can be moved from FREE to KOBJ_UNINITED
 *   and vise versa, but not allowed to move from KOBJ_UNINITED to any other kobj.
 *
 * 2.KOBJ_UNINITED is a temporary state.
 *   1. ctrlmem write on meta should return E_HM_INVAL when meta->v is KOBJ_UNINITED
 *   2. The one who direct freelist moving meta to KOBJ_UNINITED should continue move it
 *      to other kobj (with pgstr lock released) or FREE (with pgstr locked).
 *      NOTE that even "free" or "destroy" is not allowed to move meta from KOBJ_UNINITED
 *      to free. Only the thread who moves meta from kobj to KOBJ_UNINITED is allowed to
 *      give it back to freelist.
 *
 * 3.After meta moved to kobj, meta is worked as a atomic8. When ctrlmem syscall see a
 *   meta with inited kobj, it first use cmpxchg to lock it then fetch kobjheader from
 *   pieces, acquire its refcnt then unlock it. Which means we must alloc an extra bit
 *   from meta to makring this locking. However, since some kobj is only shared data and no
 *   need to invoke, or kobj is not allow to be destroyed, or some other condition that
 *   kernel is ensure kobj is impossible to be invoked, for those kobj we don't need such
 *   locking. So we don't assign a bit for locking. Instead, we create 2 kobjs for those
 *   kobj need this locking, and make itself control its invocation and destroying.
 *
 * *HOWEVER* Since currently libvsync doesn't support vatomic8_t, we still use pgstr lock
 * to protoct meta.
 *
 * Creating kobj:
 *
 *  pgstr lock is required:
 *  pgstr_lock()
 *  occupy()   <-- meta is KOBJ_UNINITED
 *  pgstr_unlock()
 *  kobj_init()   <-- lock not required
 *  set meta to KOBJ   <-- a store-release is required
 *
 * After libvsync support vatomic8_t:
 *
 * Use of kobj:
 *   cmpxchg(&meta, KOBJ, KOBJ_LOCKED) <-- if fail, race against kobj destroy
 *   if success:
 *     acquire refcnt state in pieces
 *     write(&meta, KOBJ)
 *
 * Destroy of kobj:
 *   cmpxchg(&meta, KOBJ, KOBJ_UNINITED) <-- if fail, race against kobj usage
 *   if success:
 *     destroy kobj
 *     freelists_release
 *
 * Before libvsync support vatomic8_t we rely on pgstr lock,
 * no need to assign KOBJ_LOCKED type:
 *
 * Ctrlmem operation of kobj:
 *
 *   pgstr_lock();
 *   read and check meta
 *   acquire refcnt state in pieces
 *   pgstr_unlock();
 *
 * Destroy of kobj:
 *
 *   pgstr_lock();
 *   refcnt state to make sure no more reference
 *      (in kernel. references in sysmgr should be considered by sysmgr itself)
 *   check and update meta to KOBJ_UNINITED
 *   pgstr_unlock();
 *   cleanup
 *   release to freelists.
 *
 * Single *LOCKED_KOBJ* __CTRLMEM_KOBJCTNR_META_LOCKED_KOBJ or per-kobj-type LOCKED
 * (__CTRLMEM_KOBJCTNR_META_KOBJ_VSPACE_LOCKED)?
 *
 * We choose __CTRLMEM_KOBJCTNR_META_LOCKED_KOBJ because knowing type of the original
 * kobj but unable to lock it (through cmpxchg) is meaningless.
 */
#define __CTRLMEM_KOBJCTNR_META_FREELIST_0	0
#define __CTRLMEM_KOBJCTNR_META_FREELIST_1	1
#define __CTRLMEM_KOBJCTNR_META_FREELIST_2	2
#define __CTRLMEM_KOBJCTNR_META_FREELIST_3	3
#define __CTRLMEM_KOBJCTNR_META_FREELIST_4	4
#define __CTRLMEM_KOBJCTNR_META_FREELIST_5	5
#define __CTRLMEM_KOBJCTNR_META_FREELIST_6	6
#define __CTRLMEM_KOBJCTNR_META_FREELIST_7	7
#define __CTRLMEM_KOBJCTNR_META_FREEPIECE	__CTRLMEM_KOBJCTNR_FREELISTS_NR
#define __CTRLMEM_KOBJCTNR_META_KOBJ_DATA	9
#define __CTRLMEM_KOBJCTNR_META_KOBJ_VSPACE	10
#define __CTRLMEM_KOBJCTNR_META_KOBJ_PCACHE	11
#define __CTRLMEM_KOBJCTNR_META_KOBJ_SYSPGARR	12
#define __CTRLMEM_KOBJCTNR_META_KOBJ_VSPACEEXT	13
#define __CTRLMEM_KOBJCTNR_META_KOBJ_COFILE	14
#define __CTRLMEM_KOBJCTNR_META_KOBJ_POLLLIST	15
#define __CTRLMEM_KOBJCTNR_META_KOBJ_PROCESS	16
#define __CTRLMEM_KOBJCTNR_META_KOBJ_THREAD	17
#define __CTRLMEM_KOBJCTNR_META_KOBJ_ULWT	18
#define __CTRLMEM_KOBJCTNR_META_KOBJ_SYSEVENT	19
#define __CTRLMEM_KOBJCTNR_META_KOBJ_HMPSFMOD	20
#define __CTRLMEM_KOBJCTNR_META_KOBJ_HMPSFMAP	21
#define __CTRLMEM_KOBJCTNR_META_KOBJ_ACTV_FRAME	22
#define __CTRLMEM_KOBJCTNR_META_KOBJ_FSVERITY	23
#define __CTRLMEM_KOBJCTNR_META_KOBJ_ACTV_BINDER_CONTEXT 24
#define __CTRLMEM_KOBJCTNR_META_KOBJ_ACTV_BINDER_REFER 25

/*
 * __CTRLMEM_KOBJCTNR_META_KOBJ_MAX is exclusive
 */
#define __CTRLMEM_KOBJCTNR_META_KOBJ_MAX	26

/*
 * A special kobject type, which represent the kobject is being
 * acquired or being destroyed. Should try again if observe this type.
 */
#define __CTRLMEM_KOBJCTNR_META_LOCKED_KOBJ	0x7f

/*
 * About meta of kernel object:
 *
 * The first byte of meta is kobj type (from 9 to 127 (0x7f)).
 * Following bytes set as offset from this meta to first meta, from -1 (0xff) to -127 (0x81).
 * 0x81 is the largest (lowest) possible offset, means the largest kobj
 * size is 128 pieces (2048 bytes).
 */

#define __CTRLMEM_KOBJCTNR_META_KOBJ_UNINITED	0x80

#define __CTRLMEM_KOBJCTNR_META_OFFSET_1	255u
#define __ctrlmem_kobjctnr_meta_of_offset(n)	(__CTRLMEM_KOBJCTNR_META_OFFSET_1 - ((n) - 1))
#define __CTRLMEM_KOBJCTNR_META_OFFSET_MIN	__ctrlmem_kobjctnr_meta_of_offset(__CTRLMEM_KOBJCTNR_KOBJSZ_CEILING - 1u)

#define __ctrlmem_kobjctnr_meta_freelist_order(v) \
	(((v) <= __CTRLMEM_KOBJCTNR_META_FREELIST_7) ? \
	 (v) : __CTRLMEM_KOBJCTNR_NFREELIST_INVALID)

#define __ctrlmem_kobjctnr_meta_is_freelist_head(v) \
	__ctrlmem_kobjctnr_nfreelist_is_valid(__ctrlmem_kobjctnr_meta_freelist_order(v))

#define __ctrlmem_kobjctnr_meta_is_freepiece(v) \
	((v) == __CTRLMEM_KOBJCTNR_META_FREEPIECE)

#define __ctrlmem_kobjctnr_meta_is_in_freelist(v) \
	(__ctrlmem_kobjctnr_meta_is_freepiece(v) || \
	 (__ctrlmem_kobjctnr_meta_is_freelist_head(v)))

#define __ctrlmem_kobjctnr_meta_is_kobj(v) \
	((!__ctrlmem_kobjctnr_meta_is_in_freelist(v)) && \
	 ((v) < __CTRLMEM_KOBJCTNR_META_KOBJ_MAX))

#define __ctrlmem_kobjctnr_meta_is_locked_kobj(v) \
	 ((v) == __CTRLMEM_KOBJCTNR_META_LOCKED_KOBJ)

#define __ctrlmem_kobjctnr_meta_is_generic_kobj(v) \
	 (__ctrlmem_kobjctnr_meta_is_kobj(v) || __ctrlmem_kobjctnr_meta_is_locked_kobj(v))


#define __ctrlmem_kobjctnr_meta_is_uninited_kobj(v) \
	 ((v) == __CTRLMEM_KOBJCTNR_META_KOBJ_UNINITED)

#define __ctrlmem_kobjctnr_meta_is_kobjbody(v) \
	 ((v) >= (__CTRLMEM_KOBJCTNR_META_KOBJ_UNINITED + 1u))

#define __ctrlmem_kobjctnr_meta_is_in_kobj(v)  \
	(__ctrlmem_kobjctnr_meta_is_generic_kobj(v) || \
	 __ctrlmem_kobjctnr_meta_is_kobjbody(v))

/*
 * NPIECE related computations
 */
#define __ctrlmem_kobjctnr_page_npiece_to_meta(page, npiece)	\
	(((npiece) < __CTRLMEM_KOBJCTNR_PIECES_NR) ? &((page)->metas[(npiece)]) : NULL)

/*
 * Offset related computations
 */
#define __ctrlmem_kobjctnr_pieces_offset_to_npiece(offset) \
	(((offset) >= (sizeof(struct __ctrlmem_kobjctnr_piece_s) * __CTRLMEM_KOBJCTNR_PIECES_NR)) ? \
	 __U(__CTRLMEM_KOBJCTNR_NPIECE_INVALID) : \
	 (unsigned int)((offset) / sizeof(struct __ctrlmem_kobjctnr_piece_s)))

#define __CTRLMEM_KOBJCTNR_PIECES_OFFSET	(sizeof(struct __ctrlmem_kobjctnr_page_header_s) + \
						 sizeof(struct __ctrlmem_kobjctnr_page_freelist_s) * __CTRLMEM_KOBJCTNR_FREELISTS_NR + \
						 sizeof(struct __ctrlmem_kobjctnr_meta_s) * __CTRLMEM_KOBJCTNR_PIECES_NR)

#define __ctrlmem_kobjctnr_page_offset_to_npiece(offset)	\
	(((offset) <= __CTRLMEM_KOBJCTNR_PAGE_SZ) ? \
	 __ctrlmem_kobjctnr_pieces_offset_to_npiece((offset) - __CTRLMEM_KOBJCTNR_PIECES_OFFSET) : \
	 __U(__CTRLMEM_KOBJCTNR_NPIECE_INVALID))

/*
 * ptr related computations
 */
#define __ctrlmem_kobjctnr_page_ptr_to_page(ptr)	\
	((struct __ctrlmem_kobjctnr_page_s *)(ALIGN_DOWN_CONST( \
						   (unsigned long)(ptr), \
						   __UL(__CTRLMEM_KOBJCTNR_PAGE_SZ))))

#define __ctrlmem_kobjctnr_page_ptr_to_offset(ptr) \
	((unsigned long)(ptr) - ALIGN_DOWN_CONST((unsigned long)(ptr), \
						 __UL(__CTRLMEM_KOBJCTNR_PAGE_SZ)))

#define __ctrlmem_kobjctnr_page_ptr_to_npiece(ptr) \
	__ctrlmem_kobjctnr_page_offset_to_npiece( \
			__ctrlmem_kobjctnr_page_ptr_to_offset(ptr))

/*
 * nblk used to form freelist to make sure aligned address has aligned nblk number.
 * It is used internally. Put it in uapi because it controls the format of freelist.
 */
#define __ctrlmem_kobjctnr_page_offset_to_nblk(offset)	\
	((offset) / (sizeof(struct __ctrlmem_kobjctnr_piece_s)))

#define __CTRLMEM_KOBJCTNR_NBLK_MIN		__ctrlmem_kobjctnr_page_offset_to_nblk( \
								__CTRLMEM_KOBJCTNR_PIECES_OFFSET)
#define __CTRLMEM_KOBJCTNR_NBLK_MAX		__ctrlmem_kobjctnr_page_offset_to_nblk(\
								__CTRLMEM_KOBJCTNR_PAGE_SZ)
#define __CTRLMEM_KOBJCTNR_NBLK_INVALID		__CTRLMEM_KOBJCTNR_NBLK_MAX
#define __ctrlmem_kobjctnr_nblk_is_valid(n)	(((n) >= __CTRLMEM_KOBJCTNR_NBLK_MIN) && \
							((n) < __CTRLMEM_KOBJCTNR_NBLK_MAX))

#define __ctrlmem_kobjctnr_nblk_to_npiece(nblk) \
	(__ctrlmem_kobjctnr_nblk_is_valid(nblk) ? ((unsigned int)(nblk) - __CTRLMEM_KOBJCTNR_NBLK_MIN) : \
							__U(__CTRLMEM_KOBJCTNR_NPIECE_INVALID))

#define __ctrlmem_kobjctnr_npiece_to_nblk(npiece) \
	(__ctrlmem_kobjctnr_npiece_is_valid(npiece) ? ((npiece) + __CTRLMEM_KOBJCTNR_NBLK_MIN) : \
							(__CTRLMEM_KOBJCTNR_NBLK_INVALID))

/*
 * In ctrlmem write, when writing to meta, 'v' represents what to do (to create a new kobj
 * or to destroy a kobj), 'sz' represent how many pieces for it when creating a new kobj.
 * if 'sz' == 0, use kobj defalt configuration or return E_HM_INVAL.
 */
#define __ctrlmem_kobjctnr_write_meta_mkval(vl, sz)	(((vl) & 0xffu) | ((sz) << 8))

/*
 * Trust compiler, don't use smart bitops.
 */
static inline unsigned int __ctrlmem_kobjpool_sz_to_order(unsigned int sz)
{
	unsigned int hi_bit = sizeof(unsigned int) * __CHAR_BIT__ - __builtin_clz(sz) - 1;
	return (sz > (1u << hi_bit)) ? hi_bit + 1 : hi_bit;
}

static inline struct __ctrlmem_kobjctnr_page_freelist_s*
__ctrlmem_kobjpool_freelist_of_sz(struct __ctrlmem_kobjctnr_page_s *page, unsigned int sz)
{
	unsigned int nfreelist = __ctrlmem_kobjpool_sz_to_order(sz);
	struct __ctrlmem_kobjctnr_page_freelist_s *fl = __NULL;

	if (__ctrlmem_kobjctnr_nfreelist_is_valid(nfreelist)) {
		fl = &page->freelists[nfreelist];
	}
	return fl;
}

#endif
