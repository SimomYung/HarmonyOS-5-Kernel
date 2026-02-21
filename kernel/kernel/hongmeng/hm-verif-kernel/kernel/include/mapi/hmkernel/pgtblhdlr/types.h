/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: HMKernel page table handler - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 06 18:02:09 2021
 */

#ifndef MAPI_HMKERNEL_PGTBLHDLR_TYPES_H
#define MAPI_HMKERNEL_PGTBLHDLR_TYPES_H

#include <uapi/hmkernel/pgtblhdlr/types.h>

/*
 * do not define special prefix in naming convension
 * for normal page table. If we need handle compat pagetable
 * like arm pagetable in an aarch64 kernel, set them to
 * compat_ and COMPAT_ then generate again.
 */
#define __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE
#define __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE

#include <hmkernel/pgtblhdlr/naming.h>
#include <hmkernel/strict.h>
#include <hmkernel/macro.h>

#define __PGTBLHDLR_GENERATOR

#define __PGTBL_DESC <hmasm/pgtbl/pgtbldesc.h>

/*
 * See comment in kernel/include/hmkernel/pgstr/types.h.
 * NEVER define this macro. This fake include is for cmake
 * dependencies tracing.
 */
#ifdef CHEAT_CMAKE_C_INCLUDE_REGEX_SCAN
# include <hmasm/pgtbl/pgtbldesc.h>
#endif

#define PGTBLHDLR_PGENTRY_ZERO { .u.invalid = NULL, }

/*
 * hlattr is attributes which is not able to determine statically. For example,
 * scenario definition (like kdata.h) knows this scenario is in kernel mode, but
 * va_bits (for aarch64) and ttbcr_n (for arm) is controlled internally.
 * For those settings we make hlattr and let scenario definition put them
 * into it. hlattr will be convert to cfg and reapplied to low level tmpl using
 * clrmsk and setmsk.
 */

#define ____PGCONFIG \
	__PGTBLHDLR_NAMING_LOWERCASE(pgcfg_s)
#define ____PGATTRIBUTE \
	__PGTBLHDLR_NAMING_LOWERCASE(pgattr_s)

struct ____PGCONFIG {
#define PGTBLHDLR_RAWCFG(raw) struct __pgtblhdlr_rawcfg_##raw##_s cfg;
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
};

struct ____PGATTRIBUTE {
#define PGTBLHDLR_RAWCFG(raw) \
	struct __pgtblhdlr_hlattr_##raw##_s hlattr;

#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	struct __pgtblhdlr_rawfmt_##raw##_s tmpl_##n;
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
};

static __pgtblhdlr_inline void __PGTBLHDLR_NAMING_LOWERCASE(pgentry_zero)
	(__maybe_unused const struct ____PGCONFIG *cfg, enum ____PGENTRY_TYPE t, struct ____PGENTRY e)
{
	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		__pgtblhdlr_rawfmt_##raw##_zero(e.u.raw); \
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
}

static __pgtblhdlr_inline void __PGTBLHDLR_NAMING_LOWERCASE(pgentry_populate_entry)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG *cfg,
	 struct ____PGENTRY e, ____TYPE_VAL val)
{
	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		__pgtblhdlr_##n##_populate_entry(&cfg->cfg, e.u.raw, val); \
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
}

static __pgtblhdlr_inline enum pgstr_type_e __PGTBLHDLR_NAMING_LOWERCASE(pgentry_tbl_pgstr_type)
	(enum ____PGENTRY_TYPE t, bool is_kmap)
{
	enum pgstr_type_e result = PGSTR_TYPE_INVALID;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_##n##_tbl_pgstr_type(is_kmap); \
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
	return result;
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t __PGTBLHDLR_NAMING_LOWERCASE(pgentry_prepare_val)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG *cfg, __paddr_t pa)
{
	__pgtblhdlr_rawfmt_base_t result = __PGTBLHDLR_RAWFMT_BASE_T_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_##n##_prepare_val(&cfg->cfg, pa); \
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
	return result;
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t __PGTBLHDLR_NAMING_LOWERCASE(pgentry_prepare_soft_val)
	(enum ____PGENTRY_TYPE t, __pgtblhdlr_rawfmt_base_t soft_val)
{
	__pgtblhdlr_rawfmt_base_t result = __PGTBLHDLR_RAWFMT_BASE_T_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_##n##_prepare_soft_val(soft_val); \
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
	return result;
}

enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_op_e) {
	__PGTBLHDLR_NAMING_UPPERCASE(SEQ),
	__PGTBLHDLR_NAMING_UPPERCASE(ACQ),
	__PGTBLHDLR_NAMING_UPPERCASE(REL),
	__PGTBLHDLR_NAMING_UPPERCASE(RLX),
	__PGTBLHDLR_NAMING_UPPERCASE(NOATOMIC),
};

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__PGTBLHDLR_NAMING_LOWERCASE(__pgentry_atomic_read)(enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_op_e) mo,
						    enum ____PGENTRY_TYPE t, struct ____PGENTRY e)
{
	__pgtblhdlr_rawfmt_base_t result = __PGTBLHDLR_RAWFMT_BASE_T_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		switch (mo) { \
		case __PGTBLHDLR_NAMING_UPPERCASE(SEQ): \
			result = __pgtblhdlr_rawfmt_##raw##_atomic_read_seq(e.u.raw); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(ACQ): \
			result = __pgtblhdlr_rawfmt_##raw##_atomic_read_acq(e.u.raw); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(RLX): \
			result = __pgtblhdlr_rawfmt_##raw##_atomic_read_rlx(e.u.raw); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(NOATOMIC): \
			result = __pgtblhdlr_rawfmt_##raw##_atomic_read_noatomic(e.u.raw); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(REL): \
		default: \
			 break; \
		} \
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
	return result;
}

#define define_pgtblhdlr_pgentry_atomic_read(mo, MO) \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t \
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_read##mo)(enum ____PGENTRY_TYPE t, struct ____PGENTRY e) \
{ \
	return __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_atomic_read)(__PGTBLHDLR_NAMING_UPPERCASE(MO), t, e); \
}

define_pgtblhdlr_pgentry_atomic_read(_seq, SEQ)
define_pgtblhdlr_pgentry_atomic_read(_acq, ACQ)
define_pgtblhdlr_pgentry_atomic_read(_rlx, RLX)
define_pgtblhdlr_pgentry_atomic_read(_noatomic, NOATOMIC)

static __pgtblhdlr_inline int
__PGTBLHDLR_NAMING_LOWERCASE(__pgentry_atomic_write)(enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_op_e) mo,
						     enum ____PGENTRY_TYPE t, struct ____PGENTRY e,
						     __pgtblhdlr_rawfmt_base_t v)
{
	int err = E_HM_NOSYS;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		switch (mo) { \
		case __PGTBLHDLR_NAMING_UPPERCASE(SEQ): \
			err = __pgtblhdlr_rawfmt_##raw##_atomic_write_seq(e.u.raw, v); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(REL): \
			err = __pgtblhdlr_rawfmt_##raw##_atomic_write_rel(e.u.raw, v); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(RLX): \
			err = __pgtblhdlr_rawfmt_##raw##_atomic_write_rlx(e.u.raw, v); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(NOATOMIC): \
			err = __pgtblhdlr_rawfmt_##raw##_atomic_write_noatomic(e.u.raw, v); \
			break; \
		case __PGTBLHDLR_NAMING_UPPERCASE(ACQ): \
		default: \
			 break; \
		} \
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
	return err;
}

#define define_pgtblhdlr_pgentry_atomic_write(mo, MO) \
static __pgtblhdlr_inline int \
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_write##mo)(enum ____PGENTRY_TYPE t, struct ____PGENTRY e, \
						       __pgtblhdlr_rawfmt_base_t v) \
{ \
	return __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_atomic_write)(__PGTBLHDLR_NAMING_UPPERCASE(MO), t, e, v); \
}

define_pgtblhdlr_pgentry_atomic_write(_seq, SEQ)
define_pgtblhdlr_pgentry_atomic_write(_rel, REL)
define_pgtblhdlr_pgentry_atomic_write(_rlx, RLX)
define_pgtblhdlr_pgentry_atomic_write(_noatomic, NOATOMIC)

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_cmpxchg_seq)(enum ____PGENTRY_TYPE t, struct ____PGENTRY e,
							 __pgtblhdlr_rawfmt_base_t oldv,
							 __pgtblhdlr_rawfmt_base_t newv)
{
	__pgtblhdlr_rawfmt_base_t result = __PGTBLHDLR_RAWFMT_BASE_T_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_rawfmt_##raw##_atomic_cmpxchg_seq(e.u.raw, oldv, newv); \
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
	return result;
}

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_atomic_cmpxchg_noatomic)(enum ____PGENTRY_TYPE t, struct ____PGENTRY e,
							      __pgtblhdlr_rawfmt_base_t oldv,
							      __pgtblhdlr_rawfmt_base_t newv)
{
	__pgtblhdlr_rawfmt_base_t result = __PGTBLHDLR_RAWFMT_BASE_T_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_rawfmt_##raw##_atomic_cmpxchg_noatomic(e.u.raw, oldv, newv); \
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
	return result;
}

#ifdef CONFIG_ARCH_SUPPORT_FRAGILE_MAPPING

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t __PGTBLHDLR_NAMING_LOWERCASE(pgentry_prepare_val_fragile)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG *cfg, __paddr_t pa)
{
	__pgtblhdlr_rawfmt_base_t result = __PGTBLHDLR_RAWFMT_BASE_T_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		result = __pgtblhdlr_##n##_prepare_val_fragile(&cfg->cfg, pa); \
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
	return result;
}

#endif

#undef __PGTBL_DESC
#undef __PGTBLHDLR_GENERATOR
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE

#include <hmkernel/compiler.h>

/* To make things simpler... */

struct pgentry_s {
	struct __pgtblhdlr_pgentry_s e;
};

#define PGENTRY_ZERO { \
	.e = PGTBLHDLR_PGENTRY_ZERO, \
}

struct pgcfg_s {
	struct __pgtblhdlr_pgcfg_s c;
};

#define PGCFG_ZERO { \
	.c.cfg = PGTBLHDLR_CFG_ZERO, \
}

struct pgattr_s {
	struct __pgtblhdlr_pgattr_s a;
};

struct pgentry_type_s {
	enum __pgtblhdlr_pgentry_type_e t;
};

#define PGENTRY_TYPE_BASE	{ \
	.t = PGTBLHDLR_PGENTRY_TYPE_BASE, \
}

#define PGENTRY_TYPE_SMALLEST_BLK	{ \
	.t = PGTBLHDLR_PGENTRY_TYPE_SMALLEST_BLK, \
}

#define PGENTRY_TYPE_SMALLEST_TBL	{ \
	.t = PGTBLHDLR_PGENTRY_TYPE_SMALLEST_TBL, \
}

#define PGENTRY_TYPE_SMALLEST_HUGE	{ \
	.t = PGTBLHDLR_PGENTRY_TYPE_SMALLEST_HUGE, \
}

/*
 * Namespace problem: grep PGENTRY_TYPE_INVALID to
 * see where it is.
 */
#define PGENTRY_TYPE_INVAL	{ \
	.t = __PGTBLHDLR_PGENTRY_TYPE_INVALID, \
}

static inline __always_inline bool
pgentry_type_is_valid(struct pgentry_type_s type)
{
	return __pgtblhdlr_pgentry_type_is_valid(type.t);
}

static inline __always_inline __pgtblhdlr_rawfmt_base_t *pgentry_to_ptr(struct pgentry_s e)
{
	return __pgtblhdlr_pgentry_to_ptr(e.e);
}

static inline __always_inline struct pgentry_s pgentry_from_ptr(__pgtblhdlr_rawfmt_base_t *ptr)
{
	struct pgentry_s e = {
		.e = __pgtblhdlr_pgentry_from_ptr(ptr),
	};
	return e;
}

static inline __always_inline bool
pgentry_is_valid(struct pgentry_type_s type, struct pgentry_s e)
{
	return __pgtblhdlr_pgentry_is_valid(type.t, e.e);
}

static inline __always_inline bool
pgentry_entry_is_present(struct pgentry_type_s type, const struct pgcfg_s *cfg, struct pgentry_s e)
{
	return __pgtblhdlr_pgentry_entry_is_present(type.t, container_of(&cfg->c.cfg.ucfg,
								   struct __pgtblhdlr_pgucfg_s, cfg), e.e);
}

static inline __always_inline bool
pgentry_val_is_present(struct pgentry_type_s type, const struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t v)
{
	return __pgtblhdlr_pgentry_val_is_present(type.t, container_of(&cfg->c.cfg.ucfg,
								       struct __pgtblhdlr_pgucfg_s, cfg), v);
}

#define pgentry_val_define_attr(n) \
static inline __always_inline paddr_t \
pgentry_val_is_##n(struct pgentry_type_s type, const struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t v) \
{ \
	return __pgtblhdlr_pgentry_val_is_##n(type.t, container_of(&cfg->c.cfg.ucfg, \
								   struct __pgtblhdlr_pgucfg_s, cfg), v); \
}

#define pgentry_val_define_setable_attr(n) \
static inline __always_inline __pgtblhdlr_rawfmt_base_t \
pgentry_val_set_##n(struct pgentry_type_s type, const struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t v) \
{ \
	return __pgtblhdlr_pgentry_val_set_##n(type.t, container_of(&cfg->c.cfg.ucfg, \
								    struct __pgtblhdlr_pgucfg_s, cfg), v); \
}

#define pgentry_entry_define_attr(n) \
static inline __always_inline __bool \
pgentry_is_##n(struct pgentry_type_s type, const struct pgcfg_s *cfg, struct pgentry_s e) \
{ \
	return __pgtblhdlr_pgentry_entry_is_##n(type.t, container_of(&cfg->c.cfg.ucfg, \
								     struct __pgtblhdlr_pgucfg_s, cfg), e.e); \
}

#define pgentry_entry_define_setable_attr(n) \
static inline __always_inline void \
pgentry_set_##n(struct pgentry_type_s type, const struct pgcfg_s *cfg, struct pgentry_s e) \
{ \
	__pgtblhdlr_pgentry_entry_set_##n(type.t, container_of(&cfg->c.cfg.ucfg, \
							       struct __pgtblhdlr_pgucfg_s, cfg), e.e); \
}

#undef __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_LIST
#undef __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_SETABLE_LIST
#ifdef CONFIG_PGTBL_FRAGILE_MMAP
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_LIST , fragile
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_SETABLE_LIST , fragile, nofragile
#else
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_LIST
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_SETABLE_LIST
#endif

__hmkernel_macro_simple_map_list(pgentry_val_define_attr,
				 __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(pgentry_entry_define_attr,
				 __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(pgentry_val_define_setable_attr,
				 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST)
__hmkernel_macro_simple_map_list(pgentry_entry_define_setable_attr,
				 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST)

static inline __always_inline bool
pgentry_is_zero(struct pgentry_type_s type, const struct pgcfg_s *cfg, struct pgentry_s e)
{
	return __pgtblhdlr_pgentry_is_zero(type.t, container_of(&cfg->c.cfg.ucfg,
								struct __pgtblhdlr_pgucfg_s, cfg), e.e);
}

static inline __always_inline bool
pgentry_is_lastlv(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	return __pgtblhdlr_pgentry_is_lastlv(type.t, container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
}

static inline __always_inline unsigned long
pgentry_tblidx_from_va(struct pgentry_type_s type, const struct pgcfg_s *cfg, vaddr_t va)
{
	return __pgtblhdlr_pgentry_tblidx_from_key(type.t,
	       container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg), va);
}

static inline __always_inline unsigned long
pgentry_nr_tblidx(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	return __pgtblhdlr_pgentry_nr_tblidx(type.t, container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
}

static inline __always_inline paddr_t
pgentry_decode_entry(struct pgentry_type_s type, const struct pgcfg_s *cfg, struct pgentry_s e)
{
	return __pgtblhdlr_pgentry_decode_entry(type.t, container_of(&cfg->c.cfg.ucfg,
								     struct __pgtblhdlr_pgucfg_s, cfg), e.e);
}

static inline __always_inline paddr_t
pgentry_decode_val(struct pgentry_type_s type, const struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t v)
{
	return __pgtblhdlr_pgentry_decode_val(type.t, container_of(&cfg->c.cfg.ucfg,
								   struct __pgtblhdlr_pgucfg_s, cfg), v);
}

static inline __always_inline unsigned long
pgentry_va_align(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	return __pgtblhdlr_pgentry_key_align(type.t, container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
}

static inline __always_inline bool
pgentry_va_is_aligned(struct pgentry_type_s type, vaddr_t va_start, vaddr_t va_end)
{
	return __pgtblhdlr_pgentry_key_is_aligned(type.t, va_start, va_end);
}

static inline __always_inline bool
pgentry_type_is(struct pgentry_type_s type,
		enum __pgtblhdlr_pgentry_type_e t)
{
	return (type.t == t) ? true : false;
}

static inline __always_inline unsigned long
pgentry_va_size(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	return __pgtblhdlr_pgentry_key_range(type.t, container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
}

static inline __always_inline unsigned long
pgentry_nextlv_va_size(struct pgentry_type_s parent_type, const struct pgcfg_s *cfg)
{
	return __pgtblhdlr_pgentry_nextlv_key_range(parent_type.t,
	       container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
}

static inline __always_inline struct pgentry_type_s
pgentry_lookup_nextlv(const struct pgcfg_s *cfg, struct pgentry_s *p_next_entry,
		      struct pgentry_type_s curr_type, struct pgentry_s curr_entry,
		      void *_table, unsigned long idx)
{
	struct pgentry_type_s rettype;

	UNUSED(curr_entry);
	rettype.t = __pgtblhdlr_pgentry_lookup_nextlv(container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg),
						      &p_next_entry->e,
						      curr_type.t, _table, idx);
	return rettype;
}

static inline __always_inline struct pgentry_type_s
pgentry_lookup_nextlv_ex(const struct pgcfg_s *cfg, struct pgentry_s *p_next_entry,
			 struct pgentry_type_s curr_type, struct pgentry_s curr_entry,
			 void *_table, unsigned long idx, __pgtblhdlr_rawfmt_base_t *pv)
{
	struct pgentry_type_s rettype;

	UNUSED(curr_entry);
	rettype.t = __pgtblhdlr_pgentry_lookup_nextlv_ex(container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg),
							 &p_next_entry->e,
							 curr_type.t, _table, idx, pv);
	return rettype;
}

static inline __always_inline struct pgentry_s
pgentry_from_table(struct pgentry_type_s type, void *_table, unsigned long idx)
{
	struct pgentry_s ret;

	ret.e = __pgtblhdlr_pgentry_from_table(type.t, _table, idx);
	return ret;
}

static inline __always_inline struct pgentry_type_s
pgentry_choose_nextlv_bestfit(struct pgentry_type_s parent_type, const struct pgcfg_s *cfg,
			       vaddr_t va_start, vaddr_t va_end)
{
	struct pgentry_type_s rettype;

	rettype.t = __pgtblhdlr_pgentry_choose_nextlv_bestfit(parent_type.t,
		    container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg), va_start, va_end);
	return rettype;
}

static inline __always_inline struct pgentry_type_s
pgentry_choose_nextlv_blkfit(struct pgentry_type_s parent_type, const struct pgcfg_s *cfg)
{
	struct pgentry_type_s rettype;

	rettype.t = __pgtblhdlr_pgentry_choose_nextlv_blkfit(parent_type.t,
		    container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
	return rettype;
}

static inline __always_inline struct pgentry_type_s
pgentry_choose_nextlv_tblfit(struct pgentry_type_s parent_type, const struct pgcfg_s *cfg)
{
	struct pgentry_type_s rettype;

	rettype.t = __pgtblhdlr_pgentry_choose_nextlv_tblfit(parent_type.t,
		    container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
	return rettype;
}

/*
 * Find type listed after `type` in desc.
 * desc list blk type prior its corresponding tbl type.
 */
static inline __always_inline struct pgentry_type_s
pgentry_type_next(struct pgentry_type_s type)
{
	struct pgentry_type_s rettype;

	rettype.t = __pgtblhdlr_pgentry_type_next(type.t);
	return rettype;
}

static inline __always_inline struct pgentry_type_s
pgentry_type_convert_b2t(struct pgentry_type_s blktype, const struct pgcfg_s *cfg)
{
	struct pgentry_type_s tbltype = {.t = __PGTBLHDLR_PGENTRY_TYPE_INVALID };

	tbltype = pgentry_type_next(blktype);
	if (pgentry_type_is_valid(tbltype)) {
		if ((pgentry_is_lastlv(tbltype, cfg)) ||
		    (pgentry_va_size(tbltype, cfg) != pgentry_va_size(blktype, cfg))) {
			tbltype.t = __PGTBLHDLR_PGENTRY_TYPE_INVALID;
		}
	}

	return tbltype;
}

static inline __always_inline void
pgentry_zero(const struct pgcfg_s *cfg, struct pgentry_type_s type, struct pgentry_s e)
{
	__pgtblhdlr_pgentry_zero(&cfg->c, type.t, e.e);
}

static inline __always_inline unsigned long
pgentry_table_size_unaligned(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	return __pgtblhdlr_pgentry_table_size(type.t,
				container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
}

static inline __always_inline unsigned long
pgentry_table_size(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	return PAGE_ALIGN_UP(pgentry_table_size_unaligned(type, cfg));
}

static inline __always_inline vaddr_t
pgentry_va_next(struct pgentry_type_s parent_type, const struct pgcfg_s *cfg,
		    vaddr_t va, vaddr_t va_end)
{
	return __pgtblhdlr_pgentry_va_next(parent_type.t,
					   container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg),
					   va, va_end);
}

static inline __always_inline struct pgentry_type_s
pgentry_choose_curlv_tblfit(struct pgentry_type_s type, const struct pgcfg_s *cfg)
{
	struct pgentry_type_s rettype;

	rettype.t = __pgtblhdlr_pgentry_choose_curlv_tblfit(type.t,
		    container_of(&cfg->c.cfg.ucfg, struct __pgtblhdlr_pgucfg_s, cfg));
	return rettype;
}

static inline __always_inline enum pgstr_type_e
pgentry_type_tbl_pgstr_type(struct pgentry_type_s type, bool is_kmap)
{
	return __pgtblhdlr_pgentry_tbl_pgstr_type(type.t, is_kmap);
}

/*
 * Doesn't like pgentry_decode_val which use container_of to cast pgcfg to pgucfg, here we simply
 * use &cfg->c. The reason is __pgtblhdlr_pgentry_decode_val is defined in UAPI which require UCFG
 * but __pgtblhdlr_pgentry_prepare_val is defined in MAPI which accept CFG.
 */
static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_prepare_val(struct pgentry_type_s type, const struct pgcfg_s *cfg, __paddr_t pa)
{
	return __pgtblhdlr_pgentry_prepare_val(type.t, &cfg->c, pa);
}

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_prepare_soft_val(struct pgentry_type_s type, __pgtblhdlr_rawfmt_base_t soft_val)
{
	return __pgtblhdlr_pgentry_prepare_soft_val(type.t, soft_val);
}

static inline __always_inline const char *
pgentry_strtype(struct pgentry_type_s type)
{
	return __pgtblhdlr_pgentry_strtype(type.t);
}

#define define_pgentry_atomic_read(mo) \
static inline __always_inline __pgtblhdlr_rawfmt_base_t \
pgentry_atomic_read##mo(struct pgentry_type_s t, struct pgentry_s e) \
{ \
	return __pgtblhdlr_pgentry_atomic_read##mo(t.t, e.e); \
}

define_pgentry_atomic_read(_seq)
define_pgentry_atomic_read(_acq)
define_pgentry_atomic_read(_rlx)
define_pgentry_atomic_read(_noatomic)

#define define_pgentry_atomic_write(mo) \
static inline __always_inline int \
pgentry_atomic_write##mo(struct pgentry_type_s t, struct pgentry_s e, __pgtblhdlr_rawfmt_base_t v) \
{ \
	return __pgtblhdlr_pgentry_atomic_write##mo(t.t, e.e, v); \
}

define_pgentry_atomic_write(_seq)
define_pgentry_atomic_write(_rel)
define_pgentry_atomic_write(_rlx)
define_pgentry_atomic_write(_noatomic)

static inline __always_inline __pgtblhdlr_rawfmt_base_t \
pgentry_atomic_cmpxchg_seq(struct pgentry_type_s t, struct pgentry_s e, __pgtblhdlr_rawfmt_base_t oldv,
			   __pgtblhdlr_rawfmt_base_t newv)
{
	return __pgtblhdlr_pgentry_atomic_cmpxchg_seq(t.t, e.e, oldv, newv);
}

static inline __always_inline __pgtblhdlr_rawfmt_base_t \
pgentry_atomic_cmpxchg_noatomic(struct pgentry_type_s t, struct pgentry_s e, __pgtblhdlr_rawfmt_base_t oldv,
				__pgtblhdlr_rawfmt_base_t newv)
{
	return __pgtblhdlr_pgentry_atomic_cmpxchg_noatomic(t.t, e.e, oldv, newv);
}

/*
 * asm/pgtbl/pgtblhdlr.h provides some definition to give
 * uniformed naming for different archs.
 */
#include <hmasm/pgtbl/pgtblhdlr.h>

static inline __always_inline bool
pgentry_type_is_smallest_page(struct pgentry_type_s type)
{
	return pgentry_type_is(type, PGTBLHDLR_PGENTRY_TYPE_SMALLEST_BLK);
}

static inline __always_inline bool
pgentry_type_is_base(struct pgentry_type_s type)
{
	return pgentry_type_is(type, PGTBLHDLR_PGENTRY_TYPE_BASE);
}

#ifdef CONFIG_PGTBL_FRAGILE_MMAP

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_prepare_val_fragile(struct pgentry_type_s type, const struct pgcfg_s *cfg, __paddr_t pa)
{
	return __pgtblhdlr_pgentry_prepare_val_fragile(type.t, &cfg->c, pa);
}

#else

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_prepare_val_fragile(struct pgentry_type_s type, const struct pgcfg_s *cfg, __paddr_t pa)
{
	return pgentry_prepare_val(type, cfg, pa);
}

static inline __always_inline bool
pgentry_val_is_fragile(__maybe_unused struct pgentry_type_s type,
				__maybe_unused const struct pgcfg_s *cfg,
				__maybe_unused __pgtblhdlr_rawfmt_base_t v)
{
	return false;
}

static inline __always_inline paddr_t
pgentry_is_fragile(__maybe_unused struct pgentry_type_s type,
				__maybe_unused const struct pgcfg_s *cfg,
				__maybe_unused struct pgentry_s e)
{
	return false;
}

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_val_set_fragile(__maybe_unused struct pgentry_type_s type,
			__maybe_unused const struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t v)
{
	return v;
}

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_val_set_nofragile(__maybe_unused struct pgentry_type_s type,
			__maybe_unused const struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t v)
{
	return v;
}

#endif

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_prepare_val_inherit(struct pgentry_type_s type, const struct pgcfg_s *cfg,
			    __paddr_t paddr, __pgtblhdlr_rawfmt_base_t ptev)
{
	__pgtblhdlr_rawfmt_base_t newptev = 0UL;
	if (pgentry_val_is_fragile(type, cfg, ptev)) {
		newptev = pgentry_prepare_val_fragile(type, cfg, paddr);
	} else {
		newptev = pgentry_prepare_val(type, cfg, paddr);
	}
	return newptev;
}

static inline __always_inline __pgtblhdlr_rawfmt_base_t
pgentry_prepare_val_fragile_ifdef(struct pgentry_type_s type, const struct pgcfg_s *cfg,
				  __paddr_t pa)
{
#ifdef CONFIG_PGTBL_FRAGILE_MMAP
	return pgentry_prepare_val_fragile(type, cfg, pa);
#else
	return pgentry_prepare_val(type, cfg, pa);
#endif
}

#endif
