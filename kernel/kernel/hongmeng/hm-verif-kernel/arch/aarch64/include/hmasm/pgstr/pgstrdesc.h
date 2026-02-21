/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: struct page table description (support 48-bits physical address)
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 02 06:43:13 2022
 */

#ifndef AARCH64_HMASM_PGSTR_PGSTRDESC_H
#define AARCH64_HMASM_PGSTR_PGSTRDESC_H

/* undefine guard to mute code scanner */
#undef AARCH64_HMASM_PGSTR_PGSTRDESC_H

#include <mapi/hmasm/pgstr/pgstrdesc.h>

#ifdef __PGTBLHDLR_CODE

/* Real guard in __PGTBLHDLR_CODE block */
#ifndef A64_HMASM_PGSTR_PGSTRDESC_H__CODE
#define A64_HMASM_PGSTR_PGSTRDESC_H__CODE

struct __pgtblhdlr_hlattr_a64pgstr_s { };

#define DEFINE_a64pgstr_rawfmt(fmt, t) \
static __pgtblhdlr_inline \
void ____pgtblhdlr_rawfmt_##fmt##_zero(struct __pgtblhdlr_rawfmt_##fmt##_s *r) \
{ \
	r->u.rawv = (t)(0UL); \
}

DEFINE_a64pgstr_rawfmt(a64pgstr_top, __u64)
DEFINE_a64pgstr_rawfmt(a64pgstr_mid, __u64)
DEFINE_a64pgstr_rawfmt(a64pgstr_p4k, __u32)

/*
 * Encoding of physical address in mid-level table:
 * for mit-level table a raw u64 makes ctrlmem syscall hard to process:
 *  ctrlmem syscall decide syscall semantic based on 3 factors:
 *    1. address of physical address
 *    2. value of argument to be written
 *    3. original value of the ctrlmem object.
 *  For pgtbl, it could be 2 system call types:
 *    1. writing to pgstr_s: change page type
 *    2. writing to mid-level entries of pgstrtbl: manage the pgstrtbl table.
 *  We can only distinguish these 2 system call based on original value of
 *  the entry to be written. The 2-bits MARK at the lowest significant bits
 *  can help ctrlmem understand the semantic.
 * However, it becomes a little bit harder on aarch64 because on aarch64, the
 * mid-level entry is 64-bits physical address, when pgstrv itself is 32-bits.
 * If dest of writing is the high 32-bit of the physical address, we can not
 * sure if the lowest 2 bits are really MARK. It is possible to wrongly understand
 * the meaning of system call, until we dereference the correct pgstr_s from
 * pa which should be provided in case the syscall wants to change page type,
 * and found the pgstr is not match.
 *
 * To avoid the above problem, we use special decoding of highest 32 bit of physical
 * address.
 *
 * Since arm64 supports at most 52 bits physical memory, the highest 12 bits are always
 * 0. we use extra 2 bits to mark the higiest part of physical memory, which means
 * we encode physical address 0x000a5a5a:a5a5a000 to: 0xa5a5a000|3 : a5a5a000|1, where
 * 1 is __A64_PGSTR_MARK_TABLE which means a5a5a000|1 is a u32 at the lowest part of
 * physical memory, 0xa5a5a000|3 is a u32 at the highest part of phycal memory.
 * merge them together, 0x000a5a5aa5a5a000 would be encoded to 0xa5a5a003a5a5a001.
 */
#define DEFINE_pgstrdesc_table(n, fmt, nxtfmt) \
static __pgtblhdlr_inline __u32 \
__pgtblhdlr_a64pgstr_##n##_prepare_pgstr_val(__u32 oldv) \
{ \
	return oldv; \
} \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64pgstr_##n##_prepare_table_val(__u64 oldv) \
{ \
	__u64 hi = oldv >> 32u; \
	__u64 lo = oldv & (__u64)0xfffffffcULL; \
	__u64 new_hi = (hi << 2u) | __ULL(__A64_PGSTR_MARK_INTERNAL); \
	__u64 new_lo = lo | __ULL(__A64_PGSTR_MARK_TABLE); \
	return (new_hi << 32u) | new_lo; \
}

#define DEFINE_pgstrdesc_block(n, fmt, align) \
static __pgtblhdlr_inline __u32 \
__pgtblhdlr_a64pgstr_##n##_prepare_pgstr_val(__u32 oldv) \
{ \
	return (oldv & 0xfffffffcu) | __U(__A64_PGSTR_MARK_BLOCK); \
} \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64pgstr_##n##_prepare_table_val(__u64 oldv) \
{ \
	return oldv; \
}

DEFINE_pgstrdesc_table(r,	top,	mid)
DEFINE_pgstrdesc_table(t1t,	mid,	mid)
DEFINE_pgstrdesc_table(t2g,	mid,	mid)
DEFINE_pgstrdesc_table(t4m,	mid,	p4k)

DEFINE_pgstrdesc_block(b1t,	mid,	0x10000000000ul)
DEFINE_pgstrdesc_block(b2g,	mid,	0x80000000ul)
DEFINE_pgstrdesc_block(b4m,	mid,	0x400000ul)
DEFINE_pgstrdesc_block(p4k,	p4k,	0x1000ul)

#define __pgtblhdlr_rawfmt_a64pgstr_top_zero(r)			____pgtblhdlr_rawfmt_a64pgstr_top_zero((r))
#define __pgtblhdlr_rawfmt_a64pgstr_mid_zero(r)			____pgtblhdlr_rawfmt_a64pgstr_mid_zero((r))
#define __pgtblhdlr_rawfmt_a64pgstr_p4k_zero(r)			____pgtblhdlr_rawfmt_a64pgstr_p4k_zero((r))

#endif

#else /* __PGTBLHDLR_CODE */

#endif /* __PGTBLHDLR_CODE */

#endif
