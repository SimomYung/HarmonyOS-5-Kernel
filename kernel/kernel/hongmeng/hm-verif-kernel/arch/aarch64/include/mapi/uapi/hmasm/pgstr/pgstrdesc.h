/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Desc of pgstr tbl for arm64 - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 02 16:24:14 2022
 */

#ifndef A64_MAPI_UAPI_HMASM_PGSTR_PGSTRDESC_H
#define A64_MAPI_UAPI_HMASM_PGSTR_PGSTRDESC_H

#undef A64_MAPI_UAPI_HMASM_PGSTR_PGSTRDESC_H

#ifdef __PGTBLHDLR_CODE

#ifndef A64_MAPI_UAPI_HMASM_PGSTR_PGSTRDESC_H__CODE
#define A64_MAPI_UAPI_HMASM_PGSTR_PGSTRDESC_H__CODE

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/strict.h>
#include <hmasm/memory.h>

#include <hmkernel/pgstr/pgstr.h>

struct __pgtblhdlr_rawucfg_a64pgstr_s { };

#define __A64_PGSTR_MARK_MASK	3

#define __A64_PGSTR_MARK_NONE		0
#define __A64_PGSTR_MARK_TABLE		1
#define __A64_PGSTR_MARK_BLOCK		2
#define __A64_PGSTR_MARK_INTERNAL	3

/*
 * Consider to match pgstr table to pagetable large table, the table size would be:
 *    p4k, t2m, t1g, t512g (t512g is 39 bits). There would be a problem:
 * For root, t512g and t1g, simply use 64bit to encode next level table.
 * For t2m it is a little bit complex. Each entry points to a table of 512 p4k entries.
 * Since each pk4 entry is u32 (4bytes), 512 p4k entry totally has 2048 bytes, less than 4k.
 *
 * Therefore, for ARM64 we don't try to match size of pgstr table to physical large page.
 * Size would be:
 *  p4k, t4m (1024 entry * 4k), t2g, t1t (1t would be 40 bits)
 */

struct __pgtblhdlr_rawfmt_a64pgstr_top_s {
	union {
		struct {
			__u64 ptrv;
		} ptr;
		__u64 rawv;
	} u;
};

struct __pgtblhdlr_rawfmt_a64pgstr_mid_s {
	union {
		struct {
			/* Make sure the LSB of e.u.v is also LSB of e.u.str.v */
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			__u32 padding;
			struct pgstr_s pgstr;
#else
			struct pgstr_s pgstr;
			__u32 padding;
#endif
		} str;
		struct {
			__u64 ptrv;
		} ptr;
		__u64 rawv;
	} u;
};

struct __pgtblhdlr_rawfmt_a64pgstr_p4k_s {
	union {
		struct {
			struct pgstr_s pgstr;
		} str;
		__u32 rawv;
	} u;
};

#define __DEFINE_a64pgstr_rawfmt(fmt, t) \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_rawfmt_##fmt##_val_is_valid(__u32 v) \
{ \
	return ____IS((v & (t)(__UL(__A64_PGSTR_MARK_MASK))) != (t)(__UL(__A64_PGSTR_MARK_NONE))); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_rawfmt_##fmt##_is_valid(const struct __pgtblhdlr_rawfmt_##fmt##_s *raw) \
{ \
	return ____pgtblhdlr_rawfmt_##fmt##_val_is_valid(raw->u.rawv); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_rawfmt_##fmt##_is_zero(const struct __pgtblhdlr_rawfmt_##fmt##_s *raw) \
{ \
	return ____IS(raw->u.rawv == (t)(0x0UL)); \
}

__DEFINE_a64pgstr_rawfmt(a64pgstr_top, __u64)
__DEFINE_a64pgstr_rawfmt(a64pgstr_mid, __u64)
__DEFINE_a64pgstr_rawfmt(a64pgstr_p4k, __u32)

/*
 * For a64pgstr table, we direct write 256TB in entry table and needn't
 * if (reference == 0) ... condition check.
 */
static __pgtblhdlr_inline unsigned long
____pgtblhdlr_pgstr_key_align(unsigned long reference)
{
	return reference;
}

/*
 * Same as key_align.
 */
static __pgtblhdlr_inline unsigned long
____pgtblhdlr_pgstr_key_range(unsigned long reference)
{
	return reference;
}

/*
 * Following functions are required to be defined
 * for each type:
 * is_present, is_lastlv, tblentry_size, decode_x_val,
 * prepare_val, key_is_aligned, tblidx_from_key.
 *
 * Where: for all table (r, t1t, t2g) type,
 * is_present(v&3==1), is_lastlv(false), tblentry_size(8), decode_x_val(v&(~3))
 *   and prepare_val (v | 1) key_is_aligned(always true) are same. tblidx_from_key
 *   should be defined for each level.
 * For table 4m, tblentry_size is 4.
 * For all blocks (b1t, b2g, b4m, p4k) type,
 * is_present(v&3==2), is_lastlv(true), tblentry_size(1), decode_x_val(v&(~3)) and
 *   prepare_val(v | 2) are same code.
 *   tblidx_from_key and key_is_aligned should be defined for each level.
 */
#define __DEFINE_pgstrdesc_table(n, fmt, nxtfmt) \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_val_is_present(__u64 v) \
{ \
	return ____IS((v & (__u64)(__U(__A64_PGSTR_MARK_MASK))) == (__u64)(__U(__A64_PGSTR_MARK_TABLE))); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_entry_is_present(const struct __pgtblhdlr_rawfmt_a64pgstr_##fmt##_s *raw) \
{ \
	return ____pgtblhdlr_a64pgstr_##n##_val_is_present(raw->u.rawv); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_is_lastlv(void) \
{ \
	return __false; \
} \
static __pgtblhdlr_inline unsigned long \
____pgtblhdlr_a64pgstr_##n##_tblentry_size(void) \
{ \
	return sizeof(struct __pgtblhdlr_rawfmt_a64pgstr_##nxtfmt##_s); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_key_is_aligned(__maybe_unused __paddr_t pa_start, __maybe_unused __paddr_t pa_end) \
{ \
	return __true; \
} \
static __pgtblhdlr_inline struct pgstr_s * \
__pgtblhdlr_a64pgstr_##n##_pgstr_ptr(__maybe_unused struct __pgtblhdlr_rawfmt_a64pgstr_##fmt##_s *r) \
{ \
	return __NULL; \
} \
static __pgtblhdlr_inline __u64 * \
__pgtblhdlr_a64pgstr_##n##_table_ptr(struct __pgtblhdlr_rawfmt_a64pgstr_##fmt##_s *r) \
{ \
	return &r->u.ptr.ptrv; \
} \
static __pgtblhdlr_inline __u32 \
__pgtblhdlr_a64pgstr_##n##_decode_pgstr_val(__u32 oldv) \
{ \
	return oldv; \
} \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64pgstr_##n##_decode_table_val(__u64 oldv) \
{ \
	__u64 hi = oldv >> 32u; \
	__u64 lo = oldv & (__u64)0xffffffffULL; \
	__u64 new_hi = (hi >> 2u); \
	__u64 new_lo = lo & (__u64)0xfffffffcULL; \
	return (new_hi << 32u) | new_lo; \
}

#define __DEFINE_pgstrdesc_block(n, fmt, align, type, last) \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_val_is_present(type v) \
{ \
	return ____IS((v & (type)(__U(__A64_PGSTR_MARK_MASK))) == (type)(__U(__A64_PGSTR_MARK_BLOCK))) || \
		((last) && ____IS((v & (type)(__U(__A64_PGSTR_MARK_MASK))) == \
				       (type)(__U(__A64_PGSTR_MARK_INTERNAL)))); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_entry_is_present(const struct __pgtblhdlr_rawfmt_a64pgstr_##fmt##_s *raw) \
{ \
	return ____pgtblhdlr_a64pgstr_##n##_val_is_present(raw->u.rawv); \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_is_lastlv(void) \
{ \
	return __true; \
} \
static __pgtblhdlr_inline unsigned long \
____pgtblhdlr_a64pgstr_##n##_tblentry_size(void) \
{ \
	return 1UL; \
} \
static __pgtblhdlr_inline __bool \
____pgtblhdlr_a64pgstr_##n##_key_is_aligned(__paddr_t pa_start, __paddr_t pa_end) \
{ \
	__bool r = __true; \
 \
	if ((align) == 0x1000UL) { \
		r = __true; \
	} else { \
		r = ((pa_start % (align) == (__paddr_t)(0UL)) && \
		     (pa_end % (align) == (__paddr_t)(0UL))); \
	} \
	return r; \
} \
static __pgtblhdlr_inline __paddr_t \
____pgtblhdlr_a64pgstr_##n##_tblidx_from_key(__paddr_t pa) \
{ \
	return pa & (__paddr_t)((align) - 1UL); \
} \
static __pgtblhdlr_inline struct pgstr_s * \
__pgtblhdlr_a64pgstr_##n##_pgstr_ptr(struct __pgtblhdlr_rawfmt_a64pgstr_##fmt##_s *r) \
{ \
	return &r->u.str.pgstr; \
} \
static __pgtblhdlr_inline __u64 * \
__pgtblhdlr_a64pgstr_##n##_table_ptr(__maybe_unused struct __pgtblhdlr_rawfmt_a64pgstr_##fmt##_s *r) \
{ \
	return __NULL; \
} \
static __pgtblhdlr_inline __u32 \
__pgtblhdlr_a64pgstr_##n##_decode_pgstr_val(__u32 oldv) \
{ \
	return oldv & 0xfffffffcu; \
} \
static __pgtblhdlr_inline __u64 \
__pgtblhdlr_a64pgstr_##n##_decode_table_val(__u64 oldv) \
{ \
	return oldv; \
}

__DEFINE_pgstrdesc_table(r,	top,	mid)
__DEFINE_pgstrdesc_table(t1t,	mid,	mid)
__DEFINE_pgstrdesc_table(t2g,	mid,	mid)
__DEFINE_pgstrdesc_table(t4m,	mid,	p4k)

__DEFINE_pgstrdesc_block(b1t,	mid,	0x10000000000UL, __u64, __false)
__DEFINE_pgstrdesc_block(b2g,	mid,	0x80000000UL, __u64, __false)
__DEFINE_pgstrdesc_block(b4m,	mid,	0x400000UL, __u64, __false)
__DEFINE_pgstrdesc_block(p4k,	p4k,	0x1000UL, __u32, __true)

/*
 * Each entry in root table controls 1t, totally 512(1<<9) entries.
 */
static __pgtblhdlr_inline __paddr_t
____pgtblhdlr_a64pgstr_r_tblidx_from_key(__paddr_t pa)
{
	return (pa >> 40u) & (__paddr_t)(0x1ffu);
}

/*
 * each entry in t1t controls 2g, totally 512 entries
 */
static __pgtblhdlr_inline __paddr_t
____pgtblhdlr_a64pgstr_t1t_tblidx_from_key(__paddr_t pa)
{
	return (pa >> 31u) & (__paddr_t)(0x1ffu);
}

/*
 * each entry in t2g controls 4m
 */
static __pgtblhdlr_inline __paddr_t
____pgtblhdlr_a64pgstr_t2g_tblidx_from_key(__paddr_t pa)
{
	return (pa >> 22u) & (__paddr_t)(0x1ffu);
}

/*
 * each entry in t4m controls 4k, totally 1024 entries
 */
static __pgtblhdlr_inline __paddr_t
____pgtblhdlr_a64pgstr_t4m_tblidx_from_key(__paddr_t pa)
{
	return (pa >> 12u) & (__paddr_t)(0x3ffu);
}

#define __pgtblhdlr_rawfmt_a64pgstr_top_val_is_valid(r)		____pgtblhdlr_rawfmt_a64pgstr_top_val_is_valid((r))
#define __pgtblhdlr_rawfmt_a64pgstr_top_is_valid(r)		____pgtblhdlr_rawfmt_a64pgstr_top_is_valid((r))
#define __pgtblhdlr_rawfmt_a64pgstr_top_is_zero(c, r)		____pgtblhdlr_rawfmt_a64pgstr_top_is_zero((r))
#define __pgtblhdlr_rawfmt_a64pgstr_mid_val_is_valid(r) 	____pgtblhdlr_rawfmt_a64pgstr_mid_val_is_valid((r))
#define __pgtblhdlr_rawfmt_a64pgstr_mid_is_valid(r)  		____pgtblhdlr_rawfmt_a64pgstr_mid_is_valid((r))
#define __pgtblhdlr_rawfmt_a64pgstr_mid_is_zero(c, r)		____pgtblhdlr_rawfmt_a64pgstr_mid_is_zero((r))
#define __pgtblhdlr_rawfmt_a64pgstr_p4k_val_is_valid(r)		____pgtblhdlr_rawfmt_a64pgstr_p4k_val_is_valid((r))
#define __pgtblhdlr_rawfmt_a64pgstr_p4k_is_valid(r)		____pgtblhdlr_rawfmt_a64pgstr_p4k_is_valid((r))
#define __pgtblhdlr_rawfmt_a64pgstr_p4k_is_zero(c, r)		____pgtblhdlr_rawfmt_a64pgstr_p4k_is_zero((r))

#define __pgtblhdlr_pgstr_key_align(c, r)			____pgtblhdlr_pgstr_key_align((r))
#define __pgtblhdlr_pgstr_key_range(c, r)			____pgtblhdlr_pgstr_key_range((r))

#define __pgtblhdlr_a64pgstr_r_entry_is_present(c, r)			____pgtblhdlr_a64pgstr_r_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_r_val_is_present(c, r)			____pgtblhdlr_a64pgstr_r_val_is_present((r))
#define __pgtblhdlr_a64pgstr_r_is_lastlv(c)			____pgtblhdlr_a64pgstr_r_is_lastlv()
#define __pgtblhdlr_a64pgstr_r_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_r_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_r_tblentry_size(c)			____pgtblhdlr_a64pgstr_r_tblentry_size()
#define __pgtblhdlr_a64pgstr_r_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_r_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_t1t_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_t1t_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_t1t_val_is_present(c, r)		____pgtblhdlr_a64pgstr_t1t_val_is_present((r))
#define __pgtblhdlr_a64pgstr_t1t_is_lastlv(c)			____pgtblhdlr_a64pgstr_t1t_is_lastlv()
#define __pgtblhdlr_a64pgstr_t1t_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_t1t_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_t1t_tblentry_size(c)		____pgtblhdlr_a64pgstr_t1t_tblentry_size()
#define __pgtblhdlr_a64pgstr_t1t_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_t1t_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_b1t_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_b1t_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_b1t_val_is_present(c, r)		____pgtblhdlr_a64pgstr_b1t_val_is_present((r))
#define __pgtblhdlr_a64pgstr_b1t_is_lastlv(c)			____pgtblhdlr_a64pgstr_b1t_is_lastlv()
#define __pgtblhdlr_a64pgstr_b1t_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_b1t_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_b1t_tblentry_size(c)		____pgtblhdlr_a64pgstr_b1t_tblentry_size()
#define __pgtblhdlr_a64pgstr_b1t_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_b1t_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_t2g_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_t2g_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_t2g_val_is_present(c, r)		____pgtblhdlr_a64pgstr_t2g_val_is_present((r))
#define __pgtblhdlr_a64pgstr_t2g_is_lastlv(c)			____pgtblhdlr_a64pgstr_t2g_is_lastlv()
#define __pgtblhdlr_a64pgstr_t2g_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_t2g_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_t2g_tblentry_size(c)		____pgtblhdlr_a64pgstr_t2g_tblentry_size()
#define __pgtblhdlr_a64pgstr_t2g_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_t2g_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_b2g_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_b2g_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_b2g_val_is_present(c, r)		____pgtblhdlr_a64pgstr_b2g_val_is_present((r))
#define __pgtblhdlr_a64pgstr_b2g_is_lastlv(c)			____pgtblhdlr_a64pgstr_b2g_is_lastlv()
#define __pgtblhdlr_a64pgstr_b2g_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_b2g_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_b2g_tblentry_size(c)		____pgtblhdlr_a64pgstr_b2g_tblentry_size()
#define __pgtblhdlr_a64pgstr_b2g_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_b2g_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_t4m_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_t4m_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_t4m_val_is_present(c, r)		____pgtblhdlr_a64pgstr_t4m_val_is_present((r))
#define __pgtblhdlr_a64pgstr_t4m_is_lastlv(c)			____pgtblhdlr_a64pgstr_t4m_is_lastlv()
#define __pgtblhdlr_a64pgstr_t4m_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_t4m_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_t4m_tblentry_size(c)		____pgtblhdlr_a64pgstr_t4m_tblentry_size()
#define __pgtblhdlr_a64pgstr_t4m_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_t4m_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_b4m_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_b4m_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_b4m_val_is_present(c, r)		____pgtblhdlr_a64pgstr_b4m_val_is_present((r))
#define __pgtblhdlr_a64pgstr_b4m_is_lastlv(c)			____pgtblhdlr_a64pgstr_b4m_is_lastlv()
#define __pgtblhdlr_a64pgstr_b4m_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_b4m_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_b4m_tblentry_size(c)		____pgtblhdlr_a64pgstr_b4m_tblentry_size()
#define __pgtblhdlr_a64pgstr_b4m_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_b4m_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_p4k_entry_is_present(c, r)		____pgtblhdlr_a64pgstr_p4k_entry_is_present((r))
#define __pgtblhdlr_a64pgstr_p4k_val_is_present(c, r)		____pgtblhdlr_a64pgstr_p4k_val_is_present((r))
#define __pgtblhdlr_a64pgstr_p4k_is_lastlv(c)			____pgtblhdlr_a64pgstr_p4k_is_lastlv()
#define __pgtblhdlr_a64pgstr_p4k_tblidx_from_key(c, k)		____pgtblhdlr_a64pgstr_p4k_tblidx_from_key((k))
#define __pgtblhdlr_a64pgstr_p4k_tblentry_size(c)		____pgtblhdlr_a64pgstr_p4k_tblentry_size()
#define __pgtblhdlr_a64pgstr_p4k_key_is_aligned(st, ed)		____pgtblhdlr_a64pgstr_p4k_key_is_aligned((st), (ed))

#define __pgtblhdlr_a64pgstr_r_accept_a64pgstr_b1t(c)	__true
#define __pgtblhdlr_a64pgstr_r_accept_a64pgstr_t1t(c)	__true
#define __pgtblhdlr_a64pgstr_t1t_accept_a64pgstr_b2g(c)	__true
#define __pgtblhdlr_a64pgstr_t1t_accept_a64pgstr_t2g(c)	__true
#define __pgtblhdlr_a64pgstr_t2g_accept_a64pgstr_b4m(c)	__true
#define __pgtblhdlr_a64pgstr_t2g_accept_a64pgstr_t4m(c)	__true
#define __pgtblhdlr_a64pgstr_t4m_accept_a64pgstr_p4k(c)	__true

#endif

#else /* __PGTBLHDLR_CODE */

PGTBLHDLR_RAWCFG(a64pgstr)

PGTBLHDLR_RAWFMT(a64pgstr_top)
PGTBLHDLR_RAWFMT(a64pgstr_mid)
PGTBLHDLR_RAWFMT(a64pgstr_p4k)

/*
 * ARM 64 hardware support at most 48-bit physical address. Different from
 * a32pgstr and a64pgtbl which set size and align of root/ttbrreg to 0
 * because of different reasons, for arm64, u64 is able to encode all 48-bit
 * addresses with no overflow, and no cfg for pgstrtable to affect the size
 * of physical address.
 *
 * Since size of pgstrtbl is not match from hardware, totally we support 512t (49 bits).
 * actually t1t table and b1t table is possible to have only 2048 bytes.
 */
PGTBLHDLR_PGENTRY(a64pgstr_r,		A64PGSTR_ROOT,	a64pgstr_top,	0x2000000000000,	0x2000000000000)
PGTBLHDLR_PGENTRY(a64pgstr_b1t,		A64PGSTR_B1T,	a64pgstr_mid,	0x10000000000,		0x10000000000)
PGTBLHDLR_PGENTRY(a64pgstr_t1t,		A64PGSTR_T1T,	a64pgstr_mid,	0x10000000000,		0x10000000000)
PGTBLHDLR_PGENTRY(a64pgstr_b2g,		A64PGSTR_B2G,	a64pgstr_mid,	0x80000000,		0x80000000)
PGTBLHDLR_PGENTRY(a64pgstr_t2g,		A64PGSTR_T2G,	a64pgstr_mid,	0x80000000,		0x80000000)
PGTBLHDLR_PGENTRY(a64pgstr_b4m,		A64PGSTR_B4M,	a64pgstr_mid,	0x400000,		0x400000)
PGTBLHDLR_PGENTRY(a64pgstr_t4m,		A64PGSTR_T4M,	a64pgstr_mid,	0x400000,		0x400000)
PGTBLHDLR_PGENTRY(a64pgstr_p4k,		A64PGSTR_P4K,	a64pgstr_p4k,	0x1000,			0x1000)


PGTBLHDLR_CONN(a64pgstr_r,	A64PGSTR_ROOT,	a64pgstr_top,	a64pgstr_b1t,	A64PGSTR_B1T,	a64pgstr_mid)
PGTBLHDLR_CONN(a64pgstr_r,	A64PGSTR_ROOT,	a64pgstr_top,	a64pgstr_t1t,	A64PGSTR_T1T,	a64pgstr_mid)
PGTBLHDLR_CONN(a64pgstr_t1t,	A64PGSTR_T1T,	a64pgstr_mid,	a64pgstr_b2g,	A64PGSTR_B2G,	a64pgstr_mid)
PGTBLHDLR_CONN(a64pgstr_t1t,	A64PGSTR_T1T,	a64pgstr_mid,	a64pgstr_t2g,	A64PGSTR_T2G,	a64pgstr_mid)
PGTBLHDLR_CONN(a64pgstr_t2g,	A64PGSTR_T2G,	a64pgstr_mid,	a64pgstr_b4m,	A64PGSTR_B4M,	a64pgstr_mid)
PGTBLHDLR_CONN(a64pgstr_t2g,	A64PGSTR_T2G,	a64pgstr_mid,	a64pgstr_t4m,	A64PGSTR_T4M,	a64pgstr_mid)
PGTBLHDLR_CONN(a64pgstr_t4m,	A64PGSTR_T4M,	a64pgstr_mid,	a64pgstr_p4k,	A64PGSTR_P4K,	a64pgstr_p4k)

#endif /* __PGTBLHDLR_CODE */

#endif
