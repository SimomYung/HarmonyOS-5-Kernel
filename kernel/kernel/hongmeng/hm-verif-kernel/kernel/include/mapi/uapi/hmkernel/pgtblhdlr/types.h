/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HMKernel page table handler
 * Author: Huawei OS Kernel Lab
 * Created: Tue Aug 02 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_PGTBLHDLR_TYPES_H
#define MAPI_UAPI_HMKERNEL_PGTBLHDLR_TYPES_H

#define __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE
#define __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE

#include <hmkernel/types.h>
#include <hmkernel/macro.h>
#include <hmkernel/pgtblhdlr/naming.h>

#define __PGTBLHDLR_GENERATOR

#define __PGTBL_DESC <hmasm/pgtbl/pgtbldesc.h>
#include <hmkernel/pgtblhdlr/builder.h>

/*
 * Bring value of entry out through *pv, so callbacks is able to directly use value, no need to
 * extract it from entry pointer again.
 * For example, in inv handler there is no need to pgentry_is_zero. We use (lp->v == 0) to check if
 * it is swap entry.
 */
static __pgtblhdlr_inline enum ____PGENTRY_TYPE __PGTBLHDLR_NAMING_LOWERCASE(pgentry_lookup_nextlv_ex)
	(const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY *p_next_entry,
	 enum ____PGENTRY_TYPE curr_type, void *_table, unsigned long idx, __pgtblhdlr_rawfmt_base_t *pv)
{
	enum ____PGENTRY_TYPE next_type = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_INVALID);
	__pgtblhdlr_rawfmt_base_t *p = NULL;
	__pgtblhdlr_rawfmt_base_t v = 0UL;
	bool loaded = false;
	if (__false) {
	}
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx) \
	else if ((curr_type == __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)) && \
		 __pgtblhdlr_##n##_accept_##nx(&cfg->cfg) && \
		 (p = (loaded ? p : ((__pgtblhdlr_rawfmt_base_t *)((unsigned long)(_table) + \
			 sizeof(struct __pgtblhdlr_rawfmt_##rawx##_s) * idx))), \
		  v = (loaded ? v : *p), loaded = true, true) && \
		 __pgtblhdlr_##nx##_val_is_present(&cfg->cfg, v)) { \
			p_next_entry->u.rawx = (struct __pgtblhdlr_rawfmt_##rawx##_s *)(unsigned long)(p); \
			next_type = __PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##NX); \
			if (pv != NULL) {  \
				*pv = v; \
			} \
		}

#include __PGTBL_DESC
	else {
		p_next_entry->u.invalid = ((void *)(unsigned long)(p));
		if (pv != NULL) {
			*pv = v;
		}
	}
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	return next_type;
}

static __pgtblhdlr_inline ____TYPE_VAL __PGTBLHDLR_NAMING_LOWERCASE(pgentry_decode_val)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_rawfmt_base_t v)
{
	/*
	 * If ____TYPE_VAL is structure we cannot
	 * write res = { .a = 0, }..
	 */
	____TYPE_VAL res;
	____TYPE_VAL inval = ____VAL_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_##n##_decode_val(&cfg->cfg, v); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		res = inval;
		break;
	}
	return res;
}

static __pgtblhdlr_inline ____TYPE_VAL __PGTBLHDLR_NAMING_LOWERCASE(pgentry_decode_entry)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e)
{
	/*
	 * If ____TYPE_VAL is structure we cannot
	 * write res = { .a = 0, }..
	 */
	____TYPE_VAL res;
	____TYPE_VAL inval = ____VAL_INVAL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_##n##_decode_entry(&cfg->cfg, e.u.raw); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		res = inval;
		break;
	}
	return res;
}

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_val_is_valid)
	(enum ____PGENTRY_TYPE t, __pgtblhdlr_rawfmt_base_t v)
{
	__bool res = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_rawfmt_##raw##_val_is_valid(v); \
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

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_val_is_present)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_rawfmt_base_t v)
{
	__bool res = __false;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		res = __pgtblhdlr_##n##_val_is_present(&cfg->cfg, v); \
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

/*
 * This macro could be canceled by CONFIG_PGTBL_FRAGILE_MMAP (which can be turned
 * off when arch is supported). In kernel/include/mapi/hmkernel/pgtblhdlr/types.h
 * it is undefed and redefined according to CONFIG_PGTBL_FRAGILE_MMAP.
 */
#ifdef __PGTBLATTR_ARCH_SUPPORT_SW_FRAGILE_MAPPING
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_LIST , fragile
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_SETABLE_LIST , fragile, nofragile
#else
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_LIST
#define __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_SETABLE_LIST
#endif

#define __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST \
		uexecutable, kexecutable, executable, \
		uwritable, ureadable, kwritable, kreadable, writable, readable, \
		nonemap \
		__PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_LIST

#define __pgtblhdlr_pgentry_extractable_define_attr(n) \
		__PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_##n),

enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_e) {
	__hmkernel_macro_simple_map_list(__pgtblhdlr_pgentry_extractable_define_attr,
					 __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST)
};

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_val_chkattr)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_rawfmt_base_t v,
	 enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_e) attr)
{
	__bool res = __false;

#define __pgtblhdlr_pgentry_val_chkattr(n, a) \
	case (__PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_##a)): { \
		res = __pgtblhdlr_##n##_val_is_##a(&cfg->cfg, v); \
		break; \
	}

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		switch (attr) { \
			__hmkernel_macro_exarg1_map_list(__pgtblhdlr_pgentry_val_chkattr, n, \
							 __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST) \
			default: \
				res = __false; \
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
		res = __false;
		break;
	}
	return res;
}

#define __pgtblhdlr_pgentry_val_define_attr(n) \
static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_val_is_##n) \
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_rawfmt_base_t v) \
{ \
	return __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_val_chkattr(t, cfg, v, \
							      __PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_##n))); \
}

__hmkernel_macro_simple_map_list(__pgtblhdlr_pgentry_val_define_attr, __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST)

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_entry_chkattr)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e,
	 enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_e) attr)
{
	__bool res = __false;

#define __pgtblhdlr_pgentry_entry_chkattr(n, raw, a) \
	case (__PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_##a)): { \
		res = __pgtblhdlr_##n##_entry_is_##a(&cfg->cfg, e.u.raw); \
		break; \
	}

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		switch (attr) { \
			__hmkernel_macro_exarg2_map_list(__pgtblhdlr_pgentry_entry_chkattr, n, raw, \
							 __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST) \
			default: \
				res = __false; \
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
		res = __false;
		break;
	}
	return res;
}

#define __pgtblhdlr_pgentry_entry_define_attr(n) \
static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_entry_is_##n) \
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e) \
{ \
	return __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_entry_chkattr(t, cfg, e, \
								    __PGTBLHDLR_NAMING_LOWERCASE(pgentry_extractable_attr_##n))); \
}

__hmkernel_macro_simple_map_list(__pgtblhdlr_pgentry_entry_define_attr,
				 __PGTBLHDLR_PGENTRY_EXTRACTABLE_ATTR_LIST)


#define __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST rdonly, noaf __PGTBLHDLR_PGENTRY_FRAGILE_ATTR_IN_SETABLE_LIST

#define __pgtblhdlr_pgentry_setable_define_attr(n) \
		__PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_##n),

enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_e) {
	__hmkernel_macro_simple_map_list(__pgtblhdlr_pgentry_setable_define_attr,
					 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST)
};

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_val_set_attr)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_rawfmt_base_t v,
	 enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_e) attr)
{
	__pgtblhdlr_rawfmt_base_t res = v;

#define __pgtblhdlr_pgentry_val_set_attr(n, a) \
	case (__PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_##a)): { \
		res = __pgtblhdlr_##n##_val_set_##a(&cfg->cfg, v); \
		break; \
	}

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		switch (attr) { \
			__hmkernel_macro_exarg1_map_list(__pgtblhdlr_pgentry_val_set_attr, n, \
							 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST) \
			default: \
				res = __false; \
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
		res = v;
		break;
	}
	return res;
}

#define __pgtblhdlr_pgentry_val_define_setable_attr(n) \
static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t __PGTBLHDLR_NAMING_LOWERCASE(pgentry_val_set_##n) \
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_rawfmt_base_t v) \
{ \
	return __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_val_set_attr(t, cfg, v, \
				__PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_##n))); \
}

__hmkernel_macro_simple_map_list(__pgtblhdlr_pgentry_val_define_setable_attr,
				 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST)

static __pgtblhdlr_inline void __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_entry_set_attr)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e,
	 enum __PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_e) attr)
{
#define __pgtblhdlr_pgentry_entry_set_attr(n, raw, a) \
	case (__PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_##a)): { \
		__pgtblhdlr_##n##_entry_set_##a(&cfg->cfg, e.u.raw); \
		break; \
	}

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		switch (attr) { \
			__hmkernel_macro_exarg2_map_list(__pgtblhdlr_pgentry_entry_set_attr, n, raw, \
							 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST) \
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
}

#define __pgtblhdlr_pgentry_entry_define_setable_attr(n) \
static __pgtblhdlr_inline void __PGTBLHDLR_NAMING_LOWERCASE(pgentry_entry_set_##n) \
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, struct ____PGENTRY e) \
{ \
	return __PGTBLHDLR_NAMING_LOWERCASE(__pgentry_entry_set_attr(t, cfg, e, \
								    __PGTBLHDLR_NAMING_LOWERCASE(pgentry_setable_attr_##n))); \
}

__hmkernel_macro_simple_map_list(__pgtblhdlr_pgentry_entry_define_setable_attr,
				 __PGTBLHDLR_PGENTRY_SETABLE_ATTR_LIST)

static __pgtblhdlr_inline __pgtblhdlr_rawfmt_base_t *__PGTBLHDLR_NAMING_LOWERCASE(pgentry_to_ptr)
	(struct ____PGENTRY e)
{
	return (__pgtblhdlr_rawfmt_base_t *)(e.u.invalid);
}

static __pgtblhdlr_inline struct ____PGENTRY
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_from_ptr)(__pgtblhdlr_rawfmt_base_t *ptr)
{
	struct ____PGENTRY e = {
		.u.invalid = ptr,
	};
	return e;
}

static inline __always_inline __PGTBLHDLR_BASICTYPES_KEY
__PGTBLHDLR_NAMING_LOWERCASE(pgentry_va_next)(enum ____PGENTRY_TYPE parent_type,
					      const struct ____PGCONFIG_USER *cfg,
					      __PGTBLHDLR_BASICTYPES_KEY va,
					      __PGTBLHDLR_BASICTYPES_KEY va_end)
{
	unsigned long nlv_size = __pgtblhdlr_pgentry_nextlv_key_range(parent_type, cfg);
	__PGTBLHDLR_BASICTYPES_KEY va_down = (va & ~(__PGTBLHDLR_BASICTYPES_KEY)(nlv_size - 1ul));
	return ((va_end - va_down) > nlv_size) ? (va_down + nlv_size) : va_end;
}


#undef __PGTBL_DESC
#undef __PGTBLHDLR_GENERATOR
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE

/*
 * Iterator generator for userspace
 */
struct __pgtbliter_ulparams_s {
	const struct __pgtblhdlr_pgucfg_s *cfg;
	enum __pgtblhdlr_pgentry_type_e t;
	struct __pgtblhdlr_pgentry_s e;
	__vaddr_t st;
	__vaddr_t ed;
	__pgtblhdlr_rawfmt_base_t v;
};

#define __pgtbliter_ulparams_init(_cfg, _t, _e, _st, _ed) { \
	.cfg	= (_cfg), \
	.t	= (_t), \
	.e	= (_e), \
	.st	= (_st), \
	.ed	= (_ed), \
}

#define __pgtbliter_ulparams_set_range(x, a, b) \
do { \
	(x).st = (a); \
	(x).ed = (b); \
} while(0)


#define __pgtbliter_ulparams_set(x, tp, et) \
do { \
	(x).t = (tp); \
	(x).e = (et); \
} while(0)

#define __pgtbliter_args_list(...)	, ##__VA_ARGS__
#define __pgtbliter_params_list(...)	, ##__VA_ARGS__
#define __pgtbliter_params_local(...)	__VA_ARGS__
#define __PGTBLITER_PARAMS_LOCAL_ALL	__pgtbliter_params_local(lparams.t, lparams.cfg, lparams.e, \
								 lparams.st, lparams.ed)
#define __pgtbliter_stop(...)		false
#define __pgtbliter_cont_pr(...)	true
#define __pgtbliter_cont_po(cont, ...)	cont

/*
 * Give level, if it is not 'bad', return integer of level, else return -1
 */
#define ________pgtbliter_level(a, b, ...)	b
#define ______pgtbliter_level0	, 0
#define ______pgtbliter_level1	, 1
#define ______pgtbliter_level2	, 2
#define ______pgtbliter_level3	, 3
#define ______pgtbliter_level4	, 4
#define ______pgtbliter_level5	, 5
#define ______pgtbliter_level6	, 6
#define ____pgtbliter_level(arg)	________pgtbliter_level(arg, -1)
#define __pgtbliter_level(lv) ____pgtbliter_level(______pgtbliter_level##lv)

/*
 * Usage:
 * __define_pgtbliter_bad(...)
 * {
 *     __invoke_an_inexist_function();
 *    panic(...);
 * }
 */
#define __define_pgtbliter_bad(func, args, params) \
static inline __always_inline __bool __pgtbliter_##func##_bad(enum __pgtblhdlr_pgentry_type_e t, \
							      const struct __pgtblhdlr_pgucfg_s *cfg, \
							      struct __pgtblhdlr_pgentry_s e, \
							      __vaddr_t st, __vaddr_t ed args)

/*
 * NOTE that following '*pgentry_table_ptr(...)' is not safe and the whole iterator
 * should be protected by some lock. We should not depend on this iterator.
 * But it is ok for user page table walker because page table is always readonly
 * for it.
 *
 * NOTE that pa2va should be injected to these macro.
 */
#define __define_pgtbliter(lv, nlv, blk, tbl_pre, inv, tbl_post, pa2va, args, local, params) \
static inline __always_inline bool \
__pgtbliter_##blk##_##lv(enum __pgtblhdlr_pgentry_type_e t, \
			 const struct __pgtblhdlr_pgucfg_s *cfg, \
			 struct __pgtblhdlr_pgentry_s e, \
			 __vaddr_t st, __vaddr_t ed args) \
{ \
	__vaddr_t va = st; \
	struct __pgtbliter_ulparams_s lparams = __pgtbliter_ulparams_init(cfg, t, e, st, ed); \
	__bool cont = __true; \
	__pgtblhdlr_rawfmt_base_t *ptr = __pgtblhdlr_pgentry_to_ptr(e); \
	__pgtblhdlr_rawfmt_base_t val = ptr[0]; \
	if ((t == __PGTBLHDLR_PGENTRY_TYPE_BASE) && \
	    (!__pgtblhdlr_pgentry_val_is_present(t, cfg, val))) { \
		cont = inv(local params); \
	} else { \
		lparams.v = val; \
		cont = tbl_pre(local params); \
	} \
	void *tb = pa2va(__pgtblhdlr_pgentry_decode_val(t, cfg, val)); \
	while (cont && (va < ed)) { \
		unsigned long idx = __pgtblhdlr_pgentry_tblidx_from_key(t, cfg, va); \
		struct __pgtblhdlr_pgentry_s ne; \
		enum __pgtblhdlr_pgentry_type_e nt = __pgtblhdlr_pgentry_lookup_nextlv_ex(cfg, &ne, t, tb, \
											  idx, &lparams.v); \
		__vaddr_t _ed = __pgtblhdlr_pgentry_va_next(t, cfg, va, ed); \
		__pgtbliter_ulparams_set_range(lparams, va, _ed); \
		if (__pgtblhdlr_pgentry_is_lastlv(nt, cfg)) { \
			__pgtbliter_ulparams_set(lparams, nt, ne); \
			cont = blk(local params); \
		} else if (!__pgtblhdlr_pgentry_type_is_valid(nt)) { \
			nt = __pgtblhdlr_pgentry_choose_nextlv_blkfit(t, cfg); \
			ne = __pgtblhdlr_pgentry_from_table(nt, tb, idx); \
			__pgtbliter_ulparams_set(lparams, nt, ne); \
			cont = inv(local params); \
		} else { \
			cont = __pgtbliter_##blk##_##nlv(nt, cfg, ne, va, _ed params); \
		} \
		va = _ed; \
	} \
	__pgtbliter_ulparams_set_range(lparams, st, ed); \
	__pgtbliter_ulparams_set(lparams, t, e); \
	cont = tbl_post(cont, local params); \
	return cont; \
}

#endif
