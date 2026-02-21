/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Builder of page table handler functions
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 04 12:34:46 2021
 */

#ifndef MAPI_UAPI_HMKERNEL_PGTBLHDLR_BUILDER_H
#define MAPI_UAPI_HMKERNEL_PGTBLHDLR_BUILDER_H

#ifndef __PGTBLHDLR_GENERATOR
# error mapi/uapi/pgtblhdlr.h should not be used standalone.
#endif

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/compiler.h>
#include <hmkernel/pgtblhdlr/naming.h>
#include <hmkernel/pgtblhdlr/basictypes.h>

#ifndef __pgtblhdlr_inline
# define __pgtblhdlr_inline inline __always_inline
#endif

#define __PGTBLHDLR_CODE
#include __PGTBL_DESC
#undef __PGTBLHDLR_CODE

/*
 * Undefine header guard so this header can be include multiple time.
 * Keep the guard to mute code scanner.
 */
#undef MAPI_UAPI_HMKERNEL_PGTBLHDLR_BUILDER_H

#define ____PGENTRY \
	__PGTBLHDLR_NAMING_LOWERCASE(pgentry_s)
#define ____PGENTRY_TYPE \
	__PGTBLHDLR_NAMING_LOWERCASE(pgentry_type_e)
#define ____PGENTRY_TYPE_NR \
	__PGTBLHDLR_NAMING_LOWERCASE(pgentry_type_nr_e)
#define ____PGCONFIG_USER \
	__PGTBLHDLR_NAMING_LOWERCASE(pgucfg_s)

#define PGTBLHDLR_RAWCFG(raw) struct __pgtblhdlr_rawcfg_##raw##_s;
#define PGTBLHDLR_RAWFMT(raw) struct __pgtblhdlr_rawfmt_##raw##_s;
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG

/*
 * pgentry_s is abstract type of a pagetable entry, which is
 * a union of possible physical page table entry pointer so
 * caller is able to fetch page table entry through:
 *
 * pgentry.u->__pgentry_arm_lv1[0]
 *
 * Internal type of union is pointer, so pass pgentry from argument,
 * donot use its pointer.
 */
struct ____PGENTRY {
	union {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw) struct __pgtblhdlr_rawfmt_##raw##_s *raw;
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
		void *invalid;
	} u;
};

/*
 * pgconfig_s is a wrapper to concrete rawcfg to uniform
 * typename. Do not need passing its pointer.
 */
struct ____PGCONFIG_USER {
#define PGTBLHDLR_RAWCFG(raw) struct __pgtblhdlr_rawucfg_##raw##_s cfg;
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
};

/*
 * Key of type
 */
enum ____PGENTRY_TYPE {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)	__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N),
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID),
};

enum ____PGENTRY_TYPE_NR {
	__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_NR_TYPES) = (unsigned int)__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID)
};

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_type_is_valid)
	(enum ____PGENTRY_TYPE t)
{
	return (t == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID)) ? __false : __true;
}

/*
 * is_present: check if the entry is specific type. PTE table type, check sec type,
 * return false
 * is_valid: check if the entry has a valid value for its level. PTE table type,
 * check sec type, return true
 * is_valid in descs should be adapted when the invalid formats are defined
 */
static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_is_valid)
	(enum ____PGENTRY_TYPE t, struct ____PGENTRY e)
{
	__bool ret = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		ret = __pgtblhdlr_rawfmt_##raw##_is_valid(e.u.raw); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		ret = __false;
		break;
	}
	return ret;
}

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_entry_is_present)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e)
{
	__bool rc = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		rc = __pgtblhdlr_##n##_entry_is_present(&cfg->cfg, e.u.raw); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		rc = __false;
		break;
	}
	return rc;
}

/*
 * is_zero and is_present are different. is_zero can be used to determin if
 * this entry is empty. Some entry like ttbrreg is present even if it is zero.
 */
static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_is_zero)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e)
{
	__bool result = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_rawfmt_##raw##_is_zero(&cfg->cfg, e.u.raw); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		result = __false;
		break;
	}
	return result;
}

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_is_lastlv)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	__bool res = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_##n##_is_lastlv(&cfg->cfg); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		res = __false;
		break;
	}
	return res;
}

static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_tblidx_from_key)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg, ____TYPE_KEY key)
{
	____TYPE_KEY res = (____TYPE_KEY)(key);

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_##n##_tblidx_from_key(&cfg->cfg, key); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		res = (____TYPE_KEY)(__UL(0));
		break;
	}
	return (unsigned long)res;
}

static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_nr_tblidx)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg)
{
	return __PGTBLHDLR_NAMING_LOWERCASE(pgentry_tblidx_from_key)(t, cfg, ____KEY_MAX) + __UL(1);
}

static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_tblentry_size)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	unsigned long res = __UL(0);

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_##n##_tblentry_size(&cfg->cfg); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		res = (__UL(0));
		break;
	}
	return res;
}

static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_table_size)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	unsigned long res = __UL(0);

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		unsigned long nr_entries = __pgtblhdlr_##n##_tblidx_from_key(&cfg->cfg, ____KEY_MAX) + __UL(1); \
		res = __pgtblhdlr_##n##_tblentry_size(&cfg->cfg) * nr_entries; \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		res = (__UL(0));
		break;
	}
	return res;
}

static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_key_align)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	unsigned long align = __UL(0);

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		align =  __PGTBLHDLR_NAMING_LOWERCASE(key_align)(&cfg->cfg, __UL(a)); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
	default:
		align = __UL(0);
		break;
	}
	return align;
}

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_key_is_aligned)
	(enum ____PGENTRY_TYPE t, ____TYPE_KEY key_start, ____TYPE_KEY key_end)
{
	__bool aligned = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		aligned = __pgtblhdlr_##n##_key_is_aligned(key_start, key_end); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
	default:
		aligned = __false;
		break;
	}
	return aligned;
}

/*
 * An alternative way to implement this key_range and key_align is to define
 *  __pgtblhdlr_##n##_key_{range,align} for each type like other. However,
 * only root can be affected by cfg. For other type of entries, range and alignment
 * are fixed and can be listed in the macro.
 *
 * Therefore we reqire lowlevel give an key_range function for all cases.
 */
static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_key_range)
	(enum ____PGENTRY_TYPE t, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	unsigned long sz = __UL(0);

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		sz = __PGTBLHDLR_NAMING_LOWERCASE(key_range)(&cfg->cfg, __UL(s)); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
	default:
		sz = __UL(0);
		break;
	}
	return sz;
}

static __pgtblhdlr_inline unsigned long __PGTBLHDLR_NAMING_LOWERCASE(pgentry_nextlv_key_range)
	(enum ____PGENTRY_TYPE parent_type, const struct ____PGCONFIG_USER *cfg)
{
	unsigned long sz = __UL(0);

#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx) \
	if (parent_type == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N) && \
		 __pgtblhdlr_##n##_accept_##nx(&cfg->cfg)) \
	{ \
		sz = __PGTBLHDLR_NAMING_LOWERCASE(pgentry_key_range)( \
		 __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX), cfg); \
		return sz; \
	}

#include __PGTBL_DESC

#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return sz;
}

static __pgtblhdlr_inline enum ____PGENTRY_TYPE __PGTBLHDLR_NAMING_LOWERCASE(pgentry_lookup_nextlv)
	(__maybe_unused const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY *p_next_entry,
	 enum ____PGENTRY_TYPE curr_type, void *_table, unsigned long idx)
{
	enum ____PGENTRY_TYPE next_type = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);

	if (__false) {
	}
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
	/* use else to help gcc optimization. One may think we
	 * can make this macro looks better by setting
	 * an indicator:
	 * if (!found) {
	 *     ....
	 *     if (match_a()) {
	 *         found = true;
	 *     }
	 * }
	 * if (!found) {
	 *     ....
	 *     if (match_b()) {
	 *         found = true;
	 *     }
	 * }
	 *
	 * However, it prevent gcc optimizing because 'found' is set in
	 * a different levelanother if branch.
	 */
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx) \
	else if ((curr_type == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)) && \
		 __pgtblhdlr_##n##_accept_##nx(&cfg->cfg) && \
		 __pgtblhdlr_##nx##_entry_is_present(&cfg->cfg, \
					       &((struct __pgtblhdlr_rawfmt_##rawx##_s *) \
						  (unsigned long)(_table))[idx])) { \
			p_next_entry->u.rawx = &((struct __pgtblhdlr_rawfmt_##rawx##_s *) \
						  (unsigned long)(_table))[idx]; \
			next_type = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX); \
		}

#include __PGTBL_DESC
	else {
		p_next_entry->u.invalid = ((void *)__UL(0));
	}
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return next_type;
}

static __pgtblhdlr_inline struct ____PGENTRY __PGTBLHDLR_NAMING_LOWERCASE(pgentry_from_table)
	(enum ____PGENTRY_TYPE type, void *_table, unsigned long idx)
{
	struct ____PGENTRY entry;

	switch (type) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		entry.u.raw = &((struct __pgtblhdlr_rawfmt_##raw##_s *)(unsigned long)(_table))[idx]; \
		break;\
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
	default:
		/* NULL */
		entry.u.invalid = ((void *)__UL(0));
		break;
	}
	return entry;
}

/*
 * Find the table type which has the same key_size with the input blk
 * type according to the order of PGTBLHDLR_PGENTRY
 * The input type must be blk type, ttbr and tbl types are forbidden
 */
static __pgtblhdlr_inline enum ____PGENTRY_TYPE
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_choose_curlv_tblfit)
	(enum ____PGENTRY_TYPE type, const struct ____PGCONFIG_USER *cfg)
{
	enum ____PGENTRY_TYPE res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);
	unsigned long sz = __PGTBLHDLR_NAMING_LOWERCASE(pgentry_key_range)(type, cfg);

#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	if (sz == __UL(s)) { \
		res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N); \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return res;
}

/*
 * Find the best acceptable nextlv according to the order of PGTBLHDLR_CONN
 * blk types are considered better than tbl types
 */
static __pgtblhdlr_inline enum ____PGENTRY_TYPE
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_choose_nextlv_bestfit)
	(enum ____PGENTRY_TYPE parent_type, __maybe_unused const struct ____PGCONFIG_USER *cfg,
	 ____TYPE_KEY key_start, ____TYPE_KEY key_end)
{
	enum ____PGENTRY_TYPE res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);

#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx) \
	if (parent_type == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N) && \
		 __pgtblhdlr_##n##_accept_##nx(&cfg->cfg) && \
		__PGTBLHDLR_NAMING_LOWERCASE(pgentry_key_is_aligned)( \
		 __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX), key_start, key_end)) \
	{ \
		res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX); \
	} else

#include __PGTBL_DESC
	{ }
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return res;
}

/*
 * Find the first acceptable nextlv according to the order of PGTBLHDLR_CONN
 */
static __pgtblhdlr_inline enum ____PGENTRY_TYPE
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_choose_nextlv_blkfit)
	(enum ____PGENTRY_TYPE parent_type, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	enum ____PGENTRY_TYPE res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);

#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx) \
	if (parent_type == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N) && \
		 __pgtblhdlr_##n##_accept_##nx(&cfg->cfg)) \
	{ \
		res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX); \
	} else

#include __PGTBL_DESC
	{ }
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return res;
}

/*
 * Find the last acceptable nextlv according to the order of PGTBLHDLR_CONN
 */
static __pgtblhdlr_inline enum ____PGENTRY_TYPE __PGTBLHDLR_NAMING_LOWERCASE(pgentry_choose_nextlv_tblfit)
	(enum ____PGENTRY_TYPE parent_type, __maybe_unused const struct ____PGCONFIG_USER *cfg)
{
	enum ____PGENTRY_TYPE res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);

#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx) \
	if (parent_type == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N) && \
		 __pgtblhdlr_##n##_accept_##nx(&cfg->cfg)) \
	{ \
		res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX); \
	}

#include __PGTBL_DESC

#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return res;
}

/*
 * Give a block, return a block with same size.
 *
 * The resulting code would be:
 *
 * switch () {                                    |
 *    case INVALID:                               |
 *        result = A64PGSTR_ROOT;                 |
 *        break;                                  |
 *    case A64PGSTR_ROOT:                         |
 *        result = A64PGSTR_B1T;                  |
 *        break;                                  |
 *    case A64PGSTR_B1T:                          |
 *   -------- Generated by A64PGSTR_T1T   --------|
 *        result = A64PGSTR_T1T;                  |
 *        break;                                  |
 *    case A64PGSTR_T1T:                          |
 *   ---------------------------------------------|
 *        result = A64PGSTR_B2G;                  |
 *        break;                                  |
 *
 * return next entry name listed in desc.h
 * }
 */
static __pgtblhdlr_inline enum ____PGENTRY_TYPE __PGTBLHDLR_NAMING_LOWERCASE(pgentry_type_next)
	(enum ____PGENTRY_TYPE type)
{
	enum ____PGENTRY_TYPE res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);

	switch (type) {
	case __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID):
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
		res = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N); \
		break; \
	case __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N):
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)

#include __PGTBL_DESC

#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		break;
	}
	return res;
}

static __pgtblhdlr_inline const char *__PGTBLHDLR_NAMING_LOWERCASE(pgentry_strtype)
	(enum ____PGENTRY_TYPE t)
{
	const char *str = "Unknown";

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		str = #n; \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		break;
	}
	return str;
}

#endif
