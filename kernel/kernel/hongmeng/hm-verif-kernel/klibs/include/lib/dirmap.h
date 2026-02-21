/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Dirmap
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 12 08:14:44 2018
 */
#ifndef KLIBS_DIRMAP_H
#define KLIBS_DIRMAP_H

#include <hmkernel/kernel.h>
#include <hmkernel/types.h>

/* All pointers stored in this map should be even */
/*
 * FREE        : a slot idx flaged as FREE is avaiable to be allocated
 * LAST_FREE   : a slot idx flaged as LAST_FREE is the tail of a link list of slots
 *               it should never be allocated nor used as storage
 */
#define DIRMAP_SLOT_BIT_FREE		(0x1U)	// 01b
#define DIRMAP_SLOT_BIT_LAST_FREE	(0x2U)	// 10b

/*
 * __mkmask generates a mask containing `bit` number of 1 and shift them w.r.t. `shift`
 * e.g. __mkmask(2,3) = 00000000000000000000000000011000b (32 bits in total)
 */
#define __mkmask(bit, shift)	((u32)((u64)(((u64)1 << ((unsigned int)(bit))) - (u64)(1)) << ((unsigned int)(shift))))

#define DIRMAP_BITS_FLAGS	2U
#define DIRMAP_SHIFT_FLAGS	0U
#define DIRMAP_MASK_FLAGS	__mkmask(DIRMAP_BITS_FLAGS, DIRMAP_SHIFT_FLAGS)

/*
 * __DIRMAP_MKSLOT joins two slot idxs and makes it a new slot header, during the process
 * the flags of both @self and @next are cleared, and the new flag @flags is attached.
 *
 * @param self     : a 32-bit idx
 * @param next     : a 32-bit idx
 * @param flags    : a 32-bit idx
 */
#define __DIRMAP_MKSLOT(self, next, flags)	((((u64)(self) & ~(u64)DIRMAP_MASK_FLAGS) << 32U) | \
						 ((u64)(next) & ~(u64)DIRMAP_MASK_FLAGS) | \
						 (u64)(flags))

/* similar to __DIRMAP_MKSOT but do not modify flags in @next */
#define __DIRMAP_MKSLOT_KEEP_FLAGS(self, next)	((((u64)(self) & ~(u64)DIRMAP_MASK_FLAGS) << 32U) | \
						 ((u64)(next) & 0xffffffffULL))
#define __DIRMAP_MKIDX(idx, flags)	(((u32)(idx) & ~DIRMAP_MASK_FLAGS) | (flags))
#define __DIRMAP_SLOT_SELF(slotval)	((u32)((u64)(slotval) >> 32U))
#define __DIRMAP_SLOT_NEXT(slotval)	((u32)((u64)(slotval) & 0xffffffffULL))

/*
 * __DECLARE_dirmap gives the declaration of a specific dirmap with
 * given suffix @suffix and maximum levels @levels supported
 *
 * @param suffix  : an identifier to label the dirmap which is semantic-unrelavent
 * @param levels   : ranges from 1 to 4
 */
#define __DECLARE_dirmap(suffix, levels)		\
	struct __dirmap_##suffix {			\
		u64 top_level;				\
		u32 freelists[levels + 1U];		\
		unsigned int slots[levels + 1U];		\
		unsigned int used[levels + 1U];		\
	};

/*
 * DIRMAP_LVS obtain the number of levels supported by the dirmap labelled by @suffix
 */
#define DIRMAP_LVS(suffix)	(({struct __dirmap_##suffix _____x; ARRAY_SIZE(_____x.freelists);}) - 1UL)

/*
 * __DIRMAP_INIT initializes a dirmap (with arbitrary suffix and parameters) whose
 * variable name is @name. In the initialized dirmap, on top level there is one slot
 * available but on other levels no slots are available. Users are supposed to manually
 * extend the dirmap through the extend functions declared by DECLARE_dirmap_extend
 * and DECLARE_dirmap_extend_page
 */
#define __DIRMAP_INIT(name)	\
{   \
	.top_level = __DIRMAP_MKSLOT(0U, 0U, (u64)DIRMAP_SLOT_BIT_LAST_FREE | (u64)DIRMAP_SLOT_BIT_FREE),	\
	.freelists = {		\
		[0 ... ARRAY_SIZE(name.freelists) - 2UL]	= __DIRMAP_MKIDX(0U, DIRMAP_SLOT_BIT_LAST_FREE | DIRMAP_SLOT_BIT_FREE),	\
		[ARRAY_SIZE(name.freelists) - 1UL]	= DIRMAP_SLOT_BIT_FREE,	\
	},							\
	.slots = {						\
		[0 ... ARRAY_SIZE(name.slots) - 2UL]	= 0U,	\
		[ARRAY_SIZE(name.slots) - 1UL]		= 1U,	\
	},							\
	.used = {						\
		[0 ... ARRAY_SIZE(name.slots) - 1UL] 	= 0U,	\
	},							\
}

/*
 * DIRMAP creates a dirmap with given suffix @suffix and variable name @name,
 * and initializes it.
 *
 * @param suffix  : a valid suffix, i.e. __DECLARE_dirmap(suffix) must be called before
 * @param name     : a unique variable identifier
 */
#define DIRMAP(suffix, name) struct __dirmap_##suffix name = __DIRMAP_INIT(name)
#define DIRMAP_WRITABLE(suffix, name) struct __dirmap_##suffix name __rw_after_init = __DIRMAP_INIT(name)

#define __DECLARE_INIT_DIRMAP(suffix)		\
	static inline void					\
	INIT_DIRMAP_##suffix(struct __dirmap_##suffix *map)	\
	{							\
		DIRMAP(suffix, tmp);				\
								\
		*map = tmp;					\
	}

/*
 * __DECLARE_dirmap_unused_bits declares a function that computes how much bits
 * in total are required to represent the identifier of a slot in the given dirmap
 * suffixed by @suffix.
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param pgsb4    : size of a page on level 4 is denoted by 1 << pgsb[4]
 * @param sltsb4   : size of a slot on level 4 is denoted by 1 << sltsb[4]
 * @param pgsb3    : size of a page on level 3 is denoted by 1 << pgsb[3]
 * @param sltsb3   : size of a slot on level 3 is denoted by 1 << sltsb[3]
 * @param pgsb2    : size of a page on level 2 is denoted by 1 << pgsb[2]
 * @param sltsb2   : size of a slot on level 2 is denoted by 1 << sltsb[2]
 * @param pgsb1    : size of a page on level 1 is denoted by 1 << pgsb[1]
 * @param sltsb1   : size of a slot on level 1 is denoted by 1 << sltsb[1]
 * @param pgsb0    : size of a page on level 0 is denoted by 1 << pgsb[0]
 * @param sltsb0   : size of a slot on level 0 is denoted by 1 << sltsb[0]
 *
 * @returns        : number of unused bits in the dirmap
 *
 * the next version: name of the macro and name of the declared function do not match
 */
#define __DECLARE_dirmap_unused_bits(suffix, pgsb4, sltsb4, pgsb3, sltsb3, \
				     pgsb2, sltsb2, pgsb1, sltsb1, pgsb0, sltsb0)\
static inline unsigned int				\
__dirmap_##suffix##_unused_bits(void)			\
{							\
	unsigned long bits = 0UL;			\
							\
	bits += pgsb4 - sltsb4;				\
	bits += pgsb3 - sltsb3;				\
	bits += pgsb2 - sltsb2;				\
	bits += pgsb1 - sltsb1;				\
	bits += pgsb0 - sltsb0;				\
	return 32U - (unsigned int)bits;		\
}

#define __DECLARE_dirmap_bitops_top(suffix)			\
	static inline unsigned int				\
	__dirmap_##suffix##_shift_lv_top(void)			\
	{							\
		return (unsigned int)32;			\
	}							\
	static inline u32					\
	__dirmap_##suffix##_mask_lv_top(void)			\
	{							\
		return 0U;					\
	}							\
	static inline unsigned int				\
	__dirmap_##suffix##_decode_lv_top(u32 idx)		\
	{							\
		UNUSED(idx);					\
		return 0U;					\
	}							\
	static inline u32					\
	__dirmap_##suffix##_encode_lv_top(u32 _orig, u32 _val)	\
	{							\
		UNUSED(_val);					\
		return _orig;					\
	}

#define __DECLARE_dirmap_bitops_lv(suffix, prev_lv, lv, pgsb, sltsb)\
	static inline unsigned int				\
	__dirmap_##suffix##_shift_lv##lv(void)			\
	{							\
		return (unsigned long)lv >= DIRMAP_LVS(suffix) ? 32U\
			: (unsigned int)(__dirmap_##suffix##_shift_lv##prev_lv()\
			- (unsigned int)(pgsb - sltsb));	\
	}							\
	static inline u32					\
	__dirmap_##suffix##_mask_lv##lv(void)			\
	{							\
		return (unsigned long)lv >= DIRMAP_LVS(suffix) ? 0U\
			: __mkmask((pgsb - sltsb), __dirmap_##suffix##_shift_lv##lv());\
	}							\
	static inline unsigned int				\
	__dirmap_##suffix##_decode_lv##lv(u32 idx)		\
	{							\
		return (unsigned long)lv >= DIRMAP_LVS(suffix) ? 0U\
			: (unsigned int)(((__dirmap_##suffix##_mask_lv##lv() & (u64)(idx)) >> \
			      __dirmap_##suffix##_shift_lv##lv()));\
	}							\
	static inline u32					\
	__dirmap_##suffix##_encode_lv##lv(u32 _orig, u32 _val)	\
	{							\
		u64 orig = _orig;				\
		u64 val = _val;					\
		u64 __mask = __dirmap_##suffix##_mask_lv##lv();	\
		unsigned int shift = (unsigned int)__dirmap_##suffix##_shift_lv##lv();\
								\
		return (unsigned long)lv >= DIRMAP_LVS(suffix) ? _orig\
		       : (u32)((orig & ~__mask) | ((val << shift) & __mask));\
	}

#define __DECLARE_dirmap_lookup_slot_lv_TOP(suffix)		\
	static inline u64 *					\
	__dirmap_##suffix##_lookup_slot_lv_TOP(struct __dirmap_##suffix *map, u32 idx)\
	{							\
		UNUSED(idx);					\
		return (&map->top_level);			\
	}

/*
 * __DECLARE_dirmap_lookup_lv_n declares a set of functions to look up for a slot in a
 * specific level, with its index @idx given.
 *
 * Basically, @idx of a slot can be divided into various parts:
 *
 * High ----------------------------------------------------------------------------> Low
 * | index on level top | index on level top - 1 | ... | index on level 0 | unused bits |
 *
 * So the path in dirmap that leads to the slot is completely determined by @idx unless it
 * is invalid. __DECLARE_dirmap_lookup_lv_n is aimed at finding the corresponding slot on
 * the path with level n.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param sltsb    : slot size (1 << sltsb) of the level
 * @param lv_prev  : identifier of the previous level
 * @param lv       : identifier of the level
 *
 * parameters of the declared function:
 *
 * @param map      : pointer to the dirmap instance
 * @param idx      : idx of the slot to look up for
 *
 * @returns        : a pointer that points to the address of the slot on level n, whose index
 *                   on level n is equal to @idx (TODO double check)
 *                   or NULL if it cannot be located in the dirmap
 */
#define __DECLARE_dirmap_lookup_slot_lv_n(suffix, sltsb, lv_prev, lv)	\
	static inline u64 *						\
	__dirmap_##suffix##_lookup_slot_lv##lv(struct __dirmap_##suffix *map, u32 idx)\
	{								\
		unsigned int lv_idx = __dirmap_##suffix##_decode_lv##lv(idx);\
		struct __dirmap_slot {					\
			u8 data[1U << (sltsb)];				\
		} *ptr = NULL;						\
		u64 *prev_slot = NULL, *ret = NULL;			\
									\
		do {							\
			if ((unsigned long)lv >= DIRMAP_LVS(suffix)) {	\
				ret = (&map->top_level);		\
				break;					\
			}						\
			if (lv_idx == 0U) {				\
				ret = NULL;				\
				break;					\
			}						\
			prev_slot = __dirmap_##suffix##_lookup_slot_lv##lv_prev(map, idx);\
			if (prev_slot == NULL) {			\
				ret = NULL;				\
				break;					\
			}						\
			if ((*prev_slot & DIRMAP_SLOT_BIT_FREE) != 0ULL) { \
				ret = NULL;				\
				break;					\
			}						\
			ptr = (struct __dirmap_slot *)(uptr_t)(*prev_slot);\
			ret = (u64 *)(uptr_t)(&ptr[lv_idx]);		\
		} while (false);					\
									\
		return ret;						\
	}

/*
 * __DECLARE_dirmap_can_alloc_slot_lv_n declares a function that checks whether
 * there are available slots on level @lv.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param lv       : level of the dirmap that the declared function aims to check
 *
 * parameters of the declared function:
 *
 * @param map      : pointer to the dirmap instance
 * @returns        : true if at least one free slot exists, and false otherwise
 */
#define __DECLARE_dirmap_can_alloc_slot_lv_n(suffix, lv)	\
	static inline bool					\
	__dirmap_##suffix##_can_alloc_slot_lv##lv(struct __dirmap_##suffix *map)\
	{							\
		bool ret = false;				\
		unsigned long __lv  = UL(lv);			\
		unsigned long __lvs = (unsigned long)DIRMAP_LVS(suffix); \
								\
		if (__lv <= __lvs) {				\
			ret = (map->freelists[lv] & DIRMAP_SLOT_BIT_LAST_FREE) == 0U;\
		}						\
								\
		return ret;					\
	}

#define __DECLARE_dirmap_alloc_slot_lv_TOP(suffix)\
	static inline u64 *				\
	__dirmap_##suffix##_alloc_slot_lv_TOP(struct __dirmap_##suffix *map, u32 *pidx)\
	{							\
		u64 *slot = NULL;				\
								\
		do {						\
			if ((map->freelists[DIRMAP_LVS(suffix)] & DIRMAP_SLOT_BIT_LAST_FREE) != 0U) {\
				break;				\
			}					\
			slot = (&map->top_level);		\
			if (((*slot) & DIRMAP_SLOT_BIT_FREE) == 0U) {\
				slot = NULL;			\
				break;				\
			}					\
			map->freelists[DIRMAP_LVS(suffix)] = __DIRMAP_SLOT_NEXT(*slot);\
			if (pidx != NULL) {			\
				*pidx = __DIRMAP_SLOT_SELF(*slot);\
			}					\
			map->used[DIRMAP_LVS(suffix)]++;	\
			*slot = 0UL;				\
		} while (false);				\
								\
		return slot;					\
	}

/*
 * __DECLARE_dirmap_alloc_slot_lv_n declares a function that allocates slots for dirmaps with
 * suffix @suffix and on level @lv. When a slot is successfully allocated, it is removed from
 * the head of @map -> freelists[@lv].
 *
 * When a slot is allocated, its header is erased. Because now it is taken out of the freelist,
 * the self/next index become useless. But a copy of its index should ALWAYS be kept by the user.
 * They can recover the address of the slot through @pidx, but not vice versa.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param lv       : the level to allocate on
 *
 * parameters of the declared function:
 * @param map      : pointer to the dirmap instance
 * @param pidx     : content of pidx is rewritten by the index of allocated slot
 *                   if the allocation succeeds
 * @returns        : pointer to the starting address of the allocated slot, or
 *                   NULL if the alllocation fails
 */
#define __DECLARE_dirmap_alloc_slot_lv_n(suffix, lv)	\
	static inline u64 *				\
	__dirmap_##suffix##_alloc_slot_lv##lv(struct __dirmap_##suffix *map, u32 *pidx)\
	{								\
		u64 *slot = NULL;					\
		do {							\
			if ((unsigned long)lv > DIRMAP_LVS(suffix)) {	\
				break;					\
			}						\
			if ((map->freelists[lv] & DIRMAP_SLOT_BIT_LAST_FREE) != 0U) {\
				break;					\
			}						\
			slot = __dirmap_##suffix##_lookup_slot_lv##lv(map, map->freelists[lv]);\
			if (slot == NULL || ((*slot) & DIRMAP_SLOT_BIT_FREE) == 0U) {\
				slot = NULL;				\
				break;					\
			}						\
			map->freelists[lv] = __DIRMAP_SLOT_NEXT(*slot);	\
			if (pidx != NULL) {				\
				*pidx = __DIRMAP_SLOT_SELF(*slot);	\
			}						\
			map->used[lv]++;				\
			*slot = 0UL;					\
		} while (false);					\
									\
		return slot;						\
	}

/*
 * #define __DECLARE_dirmap_free_slot_lv_n declares a function that free a slot in a dirmap
 * with suffix @suffix and on level @lv. When the slot is successfully freed, it is added
 * to the head of @map->freelists[@lv].
 *
 * When a slot is freed, its header is recovered according to the given index @idx.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param lv       : the level to free on
 *
 * parameters of the declared function:
 * @param map      : pointer to the dirmap instance
 * @param idx      : index of the slot-to-free
 */
#define __DECLARE_dirmap_free_slot_lv_n(suffix, lv)	\
	static inline void					\
	__dirmap_##suffix##_free_slot_lv##lv(struct __dirmap_##suffix *map, u32 idx)\
	{							\
		u64 *slot;					\
		do {						\
			if ((unsigned long)lv > DIRMAP_LVS(suffix)) {\
				break;				\
			}					\
			slot = __dirmap_##suffix##_lookup_slot_lv##lv(map, idx);\
			if (slot == NULL || ((*slot) & DIRMAP_SLOT_BIT_FREE) == DIRMAP_SLOT_BIT_FREE) {\
				break;				\
			}					\
			*slot = __DIRMAP_MKSLOT_KEEP_FLAGS(idx, map->freelists[lv]);	\
			map->freelists[lv] = __DIRMAP_MKIDX(idx, DIRMAP_SLOT_BIT_FREE);\
			map->used[lv]--;			\
		} while (false);				\
	}

/*
 * __DECLARE_dirmap_init_page_lv_n declares a function that initializes a page in a dirmap
 * with suffix @suffix and on level @lv. The size of pages on this level is specified by
 * (1 << @pgsb) and the size of slots on this level is specified by (1 << @sltsb).
 *
 * A page is initialized as an array of slots, of which each contains a slot header area
 * and a slot data area. A slot header is located in the lower 64 bits and contains two
 * idxs indicating the index of the current slot (higher 32 bits) and the the index of the
 * next slot (lower 32 bits).
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param pgsb     : size of the page (actual size is 1 << @pgsb)
 * @param sltsb    : size of the slot (actual size is 1 << @sltsb)
 * @param lv       : the level to initialize page on
 *
 * parameters of the decalred function:
 *
 * @param freelist : pointer the head element in the freelist of the level @lv
 * @param totol_nr : pointer to the total number of available slots in the level @lv
 *                   @total_nr is updated after a new page is successfully initialized
 * @param slot     : pointer to the array of slots newly allocated
 * @param base_idx : the base index of this page, i.e. the index of the parent slot where
 *                   the page is linked with
 * @param page     : pointer to the page-to-initialize
 */
#define __DECLARE_dirmap_init_page_lv_n(suffix, pgsb, sltsb, lv)			\
	static inline void								\
	__dirmap_##suffix##_init_page_lv##lv(u32 *free_list, unsigned int *total_nr,	\
					     u64 *slot, u32 base_idx, void *page)	\
	{ 										\
		unsigned int page_sz = 1U << (pgsb);					\
		unsigned int slot_sz = 1U << (sltsb);					\
		struct __dirmap_slot {							\
			u64 header;							\
			u8 data[sltsb == 0U ? 0U : (1U << (sltsb)) - sizeof(u64)];	\
		} __attribute__((packed));						\
		struct __dirmap_slot *pslot = (struct __dirmap_slot *)(uptr_t)page;	\
		struct __dirmap_slot *lastone = &pslot[page_sz / slot_sz - 1U];		\
		unsigned int i = 1U;							\
		u32 self, next;								\
											\
		pslot++;								\
		while (pslot < lastone) {						\
			self = __dirmap_##suffix##_encode_lv##lv(base_idx, i);		\
			next = __dirmap_##suffix##_encode_lv##lv(base_idx, i + 1U);	\
											\
			i = i + 1U;							\
			pslot->header = __DIRMAP_MKSLOT(self, next, DIRMAP_SLOT_BIT_FREE);\
			pslot++;							\
		}									\
											\
		self = __dirmap_##suffix##_encode_lv##lv(base_idx, i);			\
		next = *free_list;							\
		lastone->header = __DIRMAP_MKSLOT_KEEP_FLAGS(self, next);		\
		*free_list = __dirmap_##suffix##_encode_lv##lv(base_idx, 1U) | DIRMAP_SLOT_BIT_FREE;\
		*slot = (u64)(uptr_t)page;						\
		*total_nr += ((page_sz) / (slot_sz)) - 1U;				\
											\
		((void **)page)[0] = (void *)slot;					\
	}

#define __DECLARE_dirmap_extend_lv_TOP(suffix)			\
	static inline unsigned int __dirmap_##suffix##_extend_lv_TOP(struct __dirmap_##suffix *map, \
								     void *page, unsigned int sz)\
	{								\
		UNUSED(map, page, sz);					\
		return 0U;						\
	}

/*
 * __DECLARE_dirmap_extend_lv_n declares a function to extend level @lv of the dirmap
 * @dirmap by adding one page using externally allocated memory @page whose size is @sz.
 *
 * In dirmaps, each page is linked with a slot in the previous level. So firstly the
 * function looks up for an available slot in the previous level. If no available slot
 * is found, the function then extends the previous level instead.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param pgsb     : size of the page (actual size is 1 << @pgsb)
 * @param sltsb    : size of the slot (actual size is 1 << @sltsb)
 * @param lv       : the level to initialize page on
 *
 * parameters of the decalred function:
 *
 * @param map      : pointer to the dirmap instance
 * @param page     : pointer to the available memory page (NOT the extended pages in dirmap!)
 * @param sz       : size of the available memory page (number of bytes)
 *
 * @returns        : number of bytes that are used in this extending process
 */
#define __DECLARE_dirmap_extend_lv_n(suffix, pgsb, lv_prev, lv)\
	static inline unsigned int __dirmap_##suffix##_extend_lv##lv(struct __dirmap_##suffix *map, \
								     void *page, unsigned int sz)\
	{								\
		u32 uplv_idx;						\
		u64 *slot;						\
		unsigned int ret = 0U;					\
		do {							\
			if (sz < (1U << pgsb)) {			\
				break;					\
			}						\
			if ((unsigned long)lv >= DIRMAP_LVS(suffix)) {	\
				break;					\
			}						\
			slot = __dirmap_##suffix##_alloc_slot_lv##lv_prev(map, &uplv_idx);\
			if (slot != NULL) {				\
				__dirmap_##suffix##_init_page_lv##lv(&map->freelists[lv], &map->slots[lv],\
						(u64 *)slot, uplv_idx, page);\
				ret = (1U << pgsb);			\
				break;					\
			}						\
			ret = __dirmap_##suffix##_extend_lv##lv_prev(map, page, sz);\
		} while (false);					\
		return ret;						\
	}

#define __DECLARE_dirmap_idx_of_slot_lv_TOP(suffix)		\
	static inline u32					\
	__dirmap_##suffix##_idx_of_slot_lv_TOP(void *slot, struct __dirmap_##suffix **pmap)\
	{							\
		*pmap = container_of(slot, struct __dirmap_##suffix, top_level); \
		return 0U;					\
	}

#define __DECLARE_dirmap_idx_of_slot_lv_n(suffix, pgsb, sltsb, lv_prev, lv)\
	static inline u32					\
	__dirmap_##suffix##_idx_of_slot_lv##lv(void *slot, struct __dirmap_##suffix **pmap) \
	{							\
		unsigned int page_sz = 1U << (pgsb);		\
		unsigned int slot_sz = 1U << (sltsb);		\
		unsigned long slot_ptr = ptr_to_ulong(slot);	\
		unsigned long offset = slot_ptr % page_sz;	\
		void *prev_slot;				\
		u32 prev_idx, ret;				\
								\
		if ((unsigned long)lv >= DIRMAP_LVS(suffix)) {	\
			*pmap = container_of(slot, struct __dirmap_##suffix, top_level); \
			ret = 0U;				\
		} else {					\
			u32 idx_lv = (u32)(offset / slot_sz);	\
			prev_slot = *(ulong_to_ptr(slot_ptr - offset, void *));\
			prev_idx = __dirmap_##suffix##_idx_of_slot_lv##lv_prev(prev_slot, pmap); \
			ret = __dirmap_##suffix##_encode_lv##lv(prev_idx, idx_lv);\
		}						\
		return ret;					\
	}
/*
 * DECLARE_dirmap_extend declares a function that AIMS FOR EXTERNAL USE. It extends a
 * dirmap @map with a given externally allocated memory @page whose size is @sz.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 *
 * parameters of the declared function:
 *
 * @param map      : pointer to the dirmap instance
 * @param page     : pointer to the available memory page
 * @param sz       : size of the available memory page (number of bytes)
 *
 * @returns        : number of bytes that are left in this extending process
 */
#define DECLARE_dirmap_extend(suffix)				\
	static inline unsigned int					\
	dirmap_##suffix##_extend(struct __dirmap_##suffix *map, void *page, size_t sz)\
	{								\
		return (unsigned int)sz - __dirmap_##suffix##_extend_lv0(map, page, (unsigned int)sz);\
	}

/*
 * DECLARE_dirmap_extend_page declares a function that AIMS FOR EXTERNAL USE. It
 * extends a dirmap @map with a given externally allocated memory @page whose size
 * is exactly PAGE_SIZE.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 *
 * parameters of the declared function:
 *
 * @param map      : pointer to the dirmap instance
 * @param page     : pointer to the available memory page
 * @param sz       : size of page
 *
 * @returns        : number of free bytes that are still available in @page
 */
#define DECLARE_dirmap_extend_page(suffix)			\
	static inline unsigned int					\
	dirmap_##suffix##_extend_page(struct __dirmap_##suffix *map,	\
				      void *page, unsigned int sz)	\
	{								\
		unsigned int consumed = 0U, ret = 0U;			\
		while (consumed < sz) {					\
			unsigned int c;					\
			c = __dirmap_##suffix##_extend_lv0(map, &((u8 *)(uptr_t)page)[consumed],\
							   sz - consumed);\
			if (c == 0U) {					\
				ret = sz - consumed;			\
				break;					\
			}						\
			consumed += c;					\
		}							\
		return ret;						\
	}

/*
 * DECLARE_dirmap_alloc declares a function that AIMS FOR EXTERNAL USE. The declared
 * function allocates a available slot on level 0. And the return value is a pointer
 * pointing to the address of the slot. Type of the pointer is specified by @type.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param type     : the datatype that the slot will be used as
 *
 * parameters of the declared function:
 *
 * @param map      : the dirmap from which the slot is allocated
 * @param pidx     : a pointer to which the function will save the index of the allocated
 *                   slot
 *
 * @returns        : the pointer of the allocated slot, or NULL if the allocation fails
 */
#define DECLARE_dirmap_alloc(suffix, type)			\
	static inline type*					\
	dirmap_##suffix##_alloc(struct __dirmap_##suffix *map, u32 *pidx)\
	{							\
		u32 idx;					\
		void *ret;					\
								\
		ret = __dirmap_##suffix##_alloc_slot_lv0(map, &idx);\
		if (ret != NULL && pidx != NULL) {		\
			*pidx = idx >> __dirmap_##suffix##_unused_bits();\
		}						\
		if (ret != NULL) {					\
			*((u64 *)(uptr_t)(ret)) = 0UL;		\
		}						\
		return (type *)(uptr_t)ret;			\
	}

/*
 * DECLARE_dirmap_can_alloc declares a function that AIMS FOR EXTERNAL USE. The declared
 * function checks whether there are available free slots in a dirmap @map with suffix
 * @suffix.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 *
 * parameters of the declared function:
 *
 * @param map      : the dirmap from which the slot is allocated
 *
 * @returns        : a boolean value indicating whether free slots are available
 */
#define DECLARE_dirmap_can_alloc(suffix)		\
	static inline bool					\
	dirmap_##suffix##_can_alloc(struct __dirmap_##suffix *map)\
	{							\
		return __dirmap_##suffix##_can_alloc_slot_lv0(map);\
	}

/*
 * DECLARE_dirmap_free declares a function that AIMS FOR EXTERNAL USE. The declared
 * function frees a slot with given index @_idx in a dirmap @map with suffix
 * @suffix.
 *
 * The index is named _idx since it does not contains unused bits. When processing
 * the index through the internal free functions, first the full index is recovered
 * by _idx << unused_bits.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 *
 * parameters of the declared function:
 *
 * @param map      : the dirmap from which the slot is allocated
 * @param _idx     : the index (without unused bits) of the slot-to-free
 */
#define DECLARE_dirmap_free(suffix)			\
	static inline void					\
	dirmap_##suffix##_free(struct __dirmap_##suffix *map, u32 _idx)\
	{							\
		u32 idx = _idx << __dirmap_##suffix##_unused_bits();\
								\
		__dirmap_##suffix##_free_slot_lv0(map, idx);	\
	}

/*
 * DECLARE_dirmap_lookup declares a function that AIMS FOR EXTERNAL USE. The declared
 * function looks up for a slot with given index @_idx in a dirmap @map with suffix
 * @suffix.
 *
 * The index is named _idx since it does not contains unused bits. When processing
 * the index through the internal free functions, first the full index is recovered
 * by _idx << unused_bits.
 *
 * parameters of the macro:
 *
 * @param suffix  : the suffix of dirmaps it supports
 * @param type     : the datatype that the slot will be used as
 *
 * parameters of the declared function:
 *
 * @param map      : the dirmap from which the slot is allocated
 * @param _idx     : the index (without unused bits) of the slot-to-free
 *
 * @returns        : a pointer pointing to the address of the corresponding slot, or
 *                   NULL if the slot is not found or it is free (i.e. not allocated)
 */
#define DECLARE_dirmap_lookup(suffix, type)			\
	static inline type *					\
	dirmap_##suffix##_lookup(struct __dirmap_##suffix *map, u32 _idx)\
	{							\
		u32 idx = _idx << __dirmap_##suffix##_unused_bits();\
		u64 *slot_ptr;					\
								\
		slot_ptr = __dirmap_##suffix##_lookup_slot_lv0(map, idx);\
		if (slot_ptr != NULL && (*slot_ptr & DIRMAP_SLOT_BIT_FREE) != 0U) {\
			slot_ptr = NULL;			\
		}						\
		return (type *)(uptr_t)(slot_ptr);		\
	}

/*
 * DECLARE_dirmap_total_slots declares a function to compute number of all slots
 * (either allocated or free) in a dirmap
 */
#define DECLARE_dirmap_total_slots(suffix)\
	static inline unsigned int				\
	dirmap_##suffix##_total_slots(struct __dirmap_##suffix *map)\
	{							\
		return map->slots[0];				\
	}

/*
 * DECLARE_dirmap_total_slots declares a function to compute number of all used slots
 * in a dirmap
 */
#define DECLARE_dirmap_used_slots(suffix)\
	static inline unsigned int				\
	dirmap_##suffix##_used_slots(struct __dirmap_##suffix *map)\
	{							\
		return map->used[0];				\
	}

/*
 * DECLARE_dirmap_total_slots declares a function to compute number of all free slots
 * in a dirmap
 */
#define DECLARE_dirmap_free_slots(suffix)\
	static inline unsigned int				\
	dirmap_##suffix##_free_slots(struct __dirmap_##suffix *map)\
	{							\
		return map->slots[0] - map->used[0];		\
	}

#define DECLARE_dirmap_idx_of_slot(suffix, type)		\
	static inline unsigned int				\
	dirmap_##suffix##_idx_of_slot(type *slot, struct __dirmap_##suffix **pmap)\
	{							\
		return __dirmap_##suffix##_idx_of_slot_lv0((void *)slot, pmap) >> \
			__dirmap_##suffix##_unused_bits();	\
	}

#define \
__DECLARE_DIRMAP(suffix, levels, pgsb4, sltsb4, pgsb3, sltsb3, pgsb2, sltsb2, pgsb1, sltsb1, pgsb0, sltsb0, type) \
	__DECLARE_dirmap(suffix, levels)		\
	__DECLARE_INIT_DIRMAP(suffix)			\
	__DECLARE_dirmap_unused_bits(suffix, pgsb4, sltsb4,		\
				     pgsb3, sltsb3, pgsb2, sltsb2,	\
				     pgsb1, sltsb1, pgsb0, sltsb0)	\
	__DECLARE_dirmap_bitops_top(suffix)		\
	__DECLARE_dirmap_bitops_lv(suffix, _top, 4, pgsb4, sltsb4)\
	__DECLARE_dirmap_bitops_lv(suffix, 4, 3, pgsb3, sltsb3)\
	__DECLARE_dirmap_bitops_lv(suffix, 3, 2, pgsb2, sltsb2)\
	__DECLARE_dirmap_bitops_lv(suffix, 2, 1, pgsb1, sltsb1)\
	__DECLARE_dirmap_bitops_lv(suffix, 1, 0, pgsb0, sltsb0)\
	__DECLARE_dirmap_lookup_slot_lv_TOP(suffix)		\
	__DECLARE_dirmap_lookup_slot_lv_n(suffix, sltsb4, _TOP, 4)\
	__DECLARE_dirmap_lookup_slot_lv_n(suffix, sltsb3, 4, 3)\
	__DECLARE_dirmap_lookup_slot_lv_n(suffix, sltsb2, 3, 2)\
	__DECLARE_dirmap_lookup_slot_lv_n(suffix, sltsb1, 2, 1)\
	__DECLARE_dirmap_lookup_slot_lv_n(suffix, sltsb0, 1, 0)\
	__DECLARE_dirmap_can_alloc_slot_lv_n(suffix, 4)	\
	__DECLARE_dirmap_can_alloc_slot_lv_n(suffix, 3)	\
	__DECLARE_dirmap_can_alloc_slot_lv_n(suffix, 2)	\
	__DECLARE_dirmap_can_alloc_slot_lv_n(suffix, 1)	\
	__DECLARE_dirmap_can_alloc_slot_lv_n(suffix, 0)	\
	__DECLARE_dirmap_alloc_slot_lv_TOP(suffix)		\
	__DECLARE_dirmap_alloc_slot_lv_n(suffix, 4)		\
	__DECLARE_dirmap_alloc_slot_lv_n(suffix, 3)		\
	__DECLARE_dirmap_alloc_slot_lv_n(suffix, 2)		\
	__DECLARE_dirmap_alloc_slot_lv_n(suffix, 1)		\
	__DECLARE_dirmap_alloc_slot_lv_n(suffix, 0)		\
	__DECLARE_dirmap_free_slot_lv_n(suffix, 4)		\
	__DECLARE_dirmap_free_slot_lv_n(suffix, 3)		\
	__DECLARE_dirmap_free_slot_lv_n(suffix, 2)		\
	__DECLARE_dirmap_free_slot_lv_n(suffix, 1)		\
	__DECLARE_dirmap_free_slot_lv_n(suffix, 0)		\
	__DECLARE_dirmap_init_page_lv_n(suffix, pgsb4, sltsb4, 4)\
	__DECLARE_dirmap_init_page_lv_n(suffix, pgsb3, sltsb3, 3)\
	__DECLARE_dirmap_init_page_lv_n(suffix, pgsb2, sltsb2, 2)\
	__DECLARE_dirmap_init_page_lv_n(suffix, pgsb1, sltsb1, 1)\
	__DECLARE_dirmap_init_page_lv_n(suffix, pgsb0, sltsb0, 0)\
	__DECLARE_dirmap_extend_lv_TOP(suffix)			\
	__DECLARE_dirmap_extend_lv_n(suffix, pgsb4, _TOP, 4)	\
	__DECLARE_dirmap_extend_lv_n(suffix, pgsb3, 4, 3)	\
	__DECLARE_dirmap_extend_lv_n(suffix, pgsb2, 3, 2)	\
	__DECLARE_dirmap_extend_lv_n(suffix, pgsb1, 2, 1)	\
	__DECLARE_dirmap_extend_lv_n(suffix, pgsb0, 1, 0)	\
	__DECLARE_dirmap_idx_of_slot_lv_TOP(suffix)		\
	__DECLARE_dirmap_idx_of_slot_lv_n(suffix, pgsb4, sltsb4, _TOP, 4)\
	__DECLARE_dirmap_idx_of_slot_lv_n(suffix, pgsb3, sltsb3, 4, 3)	\
	__DECLARE_dirmap_idx_of_slot_lv_n(suffix, pgsb2, sltsb2, 3, 2)	\
	__DECLARE_dirmap_idx_of_slot_lv_n(suffix, pgsb1, sltsb1, 2, 1)	\
	__DECLARE_dirmap_idx_of_slot_lv_n(suffix, pgsb0, sltsb0, 1, 0)	\
	DECLARE_dirmap_extend(suffix)				\
	DECLARE_dirmap_extend_page(suffix)			\
	DECLARE_dirmap_alloc(suffix, type)			\
	DECLARE_dirmap_can_alloc(suffix)			\
	DECLARE_dirmap_free(suffix)				\
	DECLARE_dirmap_lookup(suffix, type)			\
	DECLARE_dirmap_total_slots(suffix)			\
	DECLARE_dirmap_used_slots(suffix)			\
	DECLARE_dirmap_free_slots(suffix)			\
	DECLARE_dirmap_idx_of_slot(suffix, type)		\

#define GEN_DIRMAP_ITERATE_API
# include <lib/dirmap_iterate.h>
#undef GEN_DIRMAP_ITERATE_API

#endif
