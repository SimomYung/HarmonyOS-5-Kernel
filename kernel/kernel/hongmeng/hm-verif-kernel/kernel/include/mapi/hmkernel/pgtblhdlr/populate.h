/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: MAPI mapping of pgtblhdlr/populate.h
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 29 17:08:51 2022
 */

#ifndef MAPI_HMKERNEL_PGTBLHDLR_POPULATE_H
#define MAPI_HMKERNEL_PGTBLHDLR_POPULATE_H

#include <hmkernel/pgtblhdlr/types.h>
#include <hmasm/pgtbl/pgtblattr/zero.h>
#include <hmasm/pgtbl/pgtblattr/model.h>
#include <hmasm/pgtbl/populate.h>
#include <hmasm/memory.h>

/*
 * In builder.h there is a naming conversion layer, which allow programmer
 * add prefix and suffix to function and type name. For example,
 * without definition of __PGTBLHDLR_NAMING_COMMON_PREFIX_{LOWER,UPPER}CASE,
 * pgcfg_s is converted to __pgtblhdlr_pgcfg_s, where __pgtblhdlr_ is common
 * prefix of pgtblhdlr. See naming.h.
 *
 * If lifted into uapi, function defined here should also have this layer:
 * like builder.h, avoid using rawname. Instead, use
 * __PGTBLHDLR_NAMING_LOWERCASE(pgentry_amend_cfg), ____PGCONFIG and so on.
 * By naming.h it is possible to process different type of page tables in
 * a same file.
 *
 * Fortunately, currently there is no need to process different pgtable in mixing
 * manner. In pgtblhdlr/types.h we define these __pgtblhdlr_ names back and remove
 * leading `__` because it is not part of UAPI.
 *
 * In this function we provide similar things. At first we use __pgtblhdlr_xx
 * naming to define functions. Since we know
 * __PGTBLHDLR_NAMING_COMMON_PREFIX_{LOWER,UPPER}CASE are all non, we get
 * rid from __PGTBLHDLR_NAMING_LOWERCASE and directly write their names.
 * Then we define functions without __pgtblhdlr_ prefix as kernel API.
 */
static __pgtblhdlr_inline void __pgtblhdlr_pgcfg_init
	(struct __pgtblhdlr_pgcfg_s *cfg, const struct __pgtblhdlr_pgattr_s *attribute)
{
#define PGTBLHDLR_RAWCFG(raw)	\
	__pgtblhdlr_##raw##_init_cfg_hl(&cfg->cfg, &attribute->hlattr);

#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	__pgtblhdlr_##n##_init_cfg_partial(&cfg->cfg, &attribute->tmpl_##n);
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
}

static __pgtblhdlr_inline void __pgtblhdlr_pgcfg_init_hl
	(struct __pgtblhdlr_pgcfg_s *cfg, const struct __pgtblhdlr_pgattr_s *attribute)
{
#define PGTBLHDLR_RAWCFG(raw)	\
	__pgtblhdlr_##raw##_init_cfg_hl(&cfg->cfg, &attribute->hlattr);

#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
}

/*
 * FIXME: only pgtable_map_ng and pgtable_remap_one_page_ng calls pgattr_init_by_va,
 * because it require hint API and unable to use fixed attributes. Use
 * __pgtblhdlr_pgattr_init_by_va to initialize hlattr and use pgtblhdlr_parse_hint
 * to initialize other attributes.
 *
 * When we finally remove hints from API we should totally remove all _by_va functions.
 */
static __pgtblhdlr_inline void __pgtblhdlr_pgattr_init_by_va
	(struct __pgtblhdlr_pgattr_s *attr, vaddr_t va)
{
	struct pgattr_s zero_attr = PGATTR_ZERO;

	*attr = zero_attr.a;

#define PGTBLHDLR_RAWCFG(raw)	\
	__pgtblhdlr_##raw##_init_hlattr_by_va(&attr->hlattr, va); \

#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
}

static __pgtblhdlr_inline void __pgtblhdlr_init_cfg_partial
	(enum __pgtblhdlr_pgentry_type_e t, struct __pgtblhdlr_pgcfg_s *cfg,
	 const struct __pgtblhdlr_pgattr_s *attr)
{
	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_PGENTRY_TYPE_##N): { \
		__pgtblhdlr_##n##_init_cfg_partial(&cfg->cfg, &attr->tmpl_##n); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		break;
	}
}

static __pgtblhdlr_inline void __pgtblhdlr_amend_cfg_preva
	(enum __pgtblhdlr_pgentry_type_e t, struct __pgtblhdlr_pgcfg_s *cfg,
	 const struct __pgtblhdlr_pgattr_s *attribute)
{
	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_PGENTRY_TYPE_##N): { \
		__pgtblhdlr_##n##_amend_cfg_preva(&cfg->cfg, &attribute->hlattr, &attribute->tmpl_##n); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		break;
	}
}

static __pgtblhdlr_inline void __pgtblhdlr_amend_cfg_postva
	(enum __pgtblhdlr_pgentry_type_e t, struct __pgtblhdlr_pgcfg_s *cfg, vaddr_t va)
{
	/*
	 * It seems we can do init_hlattr_by_va and init_cfg_hl like
	 * __pgtblhdlr_amend_cfg_postva_all, but wait. Don't do this: it is harmful
	 * and useless for optimization. When we call __pgtblhdlr_amend_cfg_postva with
	 * known type, we should have setup hlattr and cfg. It is important for some
	 * attribute which is not able to fixed by only vaddr. Like arm trampoline code,
	 * there are different attributes during booting, but the vaddress is fixed.
	 */
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_PGENTRY_TYPE_##N): { \
		__pgtblhdlr_##n##_amend_cfg_postva(&cfg->cfg, va); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		break;
	}
}

static __pgtblhdlr_inline void __pgtblhdlr_amend_cfg_entval
	(enum __pgtblhdlr_pgentry_type_e t, struct __pgtblhdlr_pgcfg_s *cfg,
	 __pgtblhdlr_rawfmt_base_t entval)
{
	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_PGENTRY_TYPE_##N): { \
		__pgtblhdlr_##n##_amend_cfg_entval(&cfg->cfg, entval); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		break;
	}
}

static __pgtblhdlr_inline void __pgtblhdlr_amend_cfg_preva_all
	(struct __pgtblhdlr_pgcfg_s *cfg, const struct __pgtblhdlr_pgattr_s *attribute)
{
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	__pgtblhdlr_##n##_amend_cfg_preva(&cfg->cfg, &attribute->hlattr, &attribute->tmpl_##n);
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
}

static inline __always_inline void
pgcfg_init_preva(struct pgcfg_s *cfg, const struct pgattr_s *attr)
{
	__pgtblhdlr_pgcfg_init(&cfg->c, &attr->a);
	__pgtblhdlr_amend_cfg_preva_all(&cfg->c, &attr->a);
}

static __pgtblhdlr_inline void __pgtblhdlr_amend_cfg_postva_all
	(struct __pgtblhdlr_pgcfg_s *cfg, vaddr_t va)
{
	/*
	 * Rewrite to cfg->hlattr according to va, so compiler knows the va_bits and setting
	 * in following iteratore. With this overwriting, codesize can be reduced nearly 50%
	 * when cfg is loaded from modelid array.
	 */
#define PGTBLHDLR_RAWCFG(raw) \
	struct __pgtblhdlr_hlattr_##raw##_s attribute; \
	__pgtblhdlr_##raw##_init_hlattr_by_va(&attribute, va); \
	__pgtblhdlr_##raw##_init_cfg_hl(&cfg->cfg, &attribute);

#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s)
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG

#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	__pgtblhdlr_##n##_amend_cfg_postva(&cfg->cfg, va);
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include <hmasm/pgtbl/pgtbldesc.h>
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
}

/*
 * Don't use __always_inline: if we call this function, we are
 * trying to create a cfg which is expected to be passed through
 * map iterator and don't have chance to optimize (no type information).
 */
static inline void
pgcfg_amend_postva(struct pgcfg_s *cfg, vaddr_t va)
{
	__pgtblhdlr_amend_cfg_postva_all(&cfg->c, va);
}

static inline __always_inline struct pgcfg_s
__pgcfg_of_attr_preva(const struct pgattr_s *pattr)
{
	struct pgcfg_s cfg = PGCFG_ZERO;

	pgcfg_init_preva(&cfg, pattr);
	return cfg;
}

#define pgcfg_of_attr_preva(A) ({struct pgattr_s __attr = A; __pgcfg_of_attr_preva(&__attr);})

#define pgcfg_udecode()	pgcfg_of_attr_preva(PGATTR_UDATA_MAPPING)
#define pgcfg_kdecode()	pgcfg_of_attr_preva(PGATTR_KDATA_MAPPING)
#define pgcfg_uscan(privileged) ({ \
	struct pgcfg_s c_u = pgcfg_of_attr_preva(PGATTR_UDATA_MAPPING); \
	struct pgcfg_s c_s = pgcfg_of_attr_preva(PGATTR_SDATA_MAPPING); \
	(privileged) ? c_s : c_u; \
})
#define pgcfg_kscan() pgcfg_of_attr_preva(PGATTR_KDATA_MAPPING)

static inline __always_inline void
pgtblhdlr_init_cfg_preva_partial(struct pgentry_type_s type, struct pgcfg_s *cfg,
				 const struct pgattr_s *attr)
{
	__pgtblhdlr_pgcfg_init_hl(&cfg->c, &attr->a);
	__pgtblhdlr_init_cfg_partial(type.t, &cfg->c, &attr->a);
	__pgtblhdlr_amend_cfg_preva(type.t, &cfg->c, &attr->a);
}

static inline __always_inline void pgattr_init_by_va(struct pgattr_s *attr, vaddr_t va)
{
	__pgtblhdlr_pgattr_init_by_va(&attr->a, va);
}

static inline __always_inline void
__pgentry_populate_pa(struct pgentry_type_s type, const struct pgcfg_s *cfg,
		      struct pgentry_s e, paddr_t pa)
{
	__pgtblhdlr_pgentry_populate_entry(type.t, &cfg->c, e.e, pa);
}

static inline __always_inline void
pgentry_populate_pa(struct pgentry_type_s type, const struct pgcfg_s *cfg, vaddr_t va,
		    struct pgentry_s e, paddr_t pa)
{
	struct pgcfg_s precfg = *cfg;
	__pgtblhdlr_amend_cfg_postva(type.t, &precfg.c, va);
	__pgtblhdlr_pgentry_populate_entry(type.t, &precfg.c, e.e, pa);
}

static inline __always_inline void
pgentry_populate_pa_barrier(struct pgentry_type_s type, const struct pgcfg_s *cfg, vaddr_t va,
			    struct pgentry_s e, paddr_t pa)
{
	pgentry_populate_pa(type, cfg, va, e, pa);
	pgtblhdlr_barrier();
}

static inline __always_inline void
pgentry_amend_cfg_entval(struct pgentry_type_s type, struct pgcfg_s *cfg, __pgtblhdlr_rawfmt_base_t entval)
{
	__pgtblhdlr_amend_cfg_entval(type.t, &cfg->c, entval);
}

static inline __always_inline void
pgentry_amend_cfg_postva(struct pgentry_type_s type, struct pgcfg_s *cfg, vaddr_t va)
{
	__pgtblhdlr_amend_cfg_postva(type.t, &cfg->c, va);
}

#endif
