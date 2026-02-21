/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ARM64 stage 2 page table handler builder
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 23 22:36:08 2023
 */

#ifndef MAPI_UAPI_HMASM_PGTBLA64S2_H
#define MAPI_UAPI_HMASM_PGTBLA64S2_H

#include <hmasm/memory.h>
#include <hmkernel/types.h>

#define __ARCH_PGTBLA64S2_VAL_TYPE   __u64
#define __ARCH_PGTBLA64S2_VAL_INVAL ((__u64)(0u))
typedef __ARCH_PGTBLA64S2_VAL_TYPE __arch_pgtbla64s2_val_t;

#define __PGTBLHDLR_BASICTYPES_KEY		__paddr_t
#define __PGTBLHDLR_BASICTYPES_KEY_MAX		(~(__paddr_t)(0))
#define __PGTBLHDLR_BASICTYPES_VAL		__ARCH_PGTBLA64S2_VAL_TYPE
#define __PGTBLHDLR_BASICTYPES_INVAL		__ARCH_PGTBLA64S2_VAL_INVAL

#define __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE pgtbla64s2_
#define __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE PGTBLA64S2_

#include <hmkernel/pgtblhdlr/naming.h>

#define __PGTBLHDLR_GENERATOR

#define __PGTBL_DESC <hmasm/pgtbla64s2/desc.h>
#include <hmkernel/pgtblhdlr/builder.h>

/*
 * cmake doesn't use gcc -Wp,-MD. Instead, it directly scan source file
 * with regexp  "^[ \t]*[#%][ \t]*(include|import)[ \t]*[<\"]([^\">]+)([\">])"
 * to find dependencies. To make dependency tracing correctly, we add a fake
 * include here. However, wrap it with a never defined macro so it would not
 * compiled.
 */
#ifdef CHEAT_CMAKE_C_INCLUDE_REGEX_SCAN
# include <hmasm/pgtbla64s2/desc.h>
#endif

/*
 * arm manual:
 * https://developer.arm.com/documentation/ddi0487/ia
 * Document number ID081822, Document version I.a
 *
 * See Table (D8-40 Data access permissions for a stage 2 translation) in ARM64 manual
 */
#define PGTBLA64S2_ATTR_S2AP_SHIFT	6U
#define PGTBLA64S2_ATTR_S2AP_MASK	0x3UL
#define PGTBLA64S2_ATTR_S2AP_NO_ACCESS	0
#define PGTBLA64S2_ATTR_S2AP_READ_ONLY	1
#define PGTBLA64S2_ATTR_S2AP_WRITE_ONLY	2
#define PGTBLA64S2_ATTR_S2AP_READ_WRITE	3

static inline __u64 __pgtblhdlr_a64s2_get_s2ap(__u64 v)
{
	return ((v >> PGTBLA64S2_ATTR_S2AP_SHIFT) & PGTBLA64S2_ATTR_S2AP_MASK);
}

static __bool __pgtblhdlr__pgtbla64s2_check_s2ap(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e type,
						 __u64 v, __u64 s2ap)
{
	__bool is_attr = __false;
	switch (type) {
	case __PGTBLHDLR_PGTBLA64S2_PGENTRY_TYPE_A64S2BLK1G:
	case __PGTBLHDLR_PGTBLA64S2_PGENTRY_TYPE_A64S2BLK2M:
	case __PGTBLHDLR_PGTBLA64S2_PGENTRY_TYPE_A64S2PAGE4K:
		is_attr = (__pgtblhdlr_a64s2_get_s2ap(v) == s2ap);
		break;
	default:
		break;
	}
	return is_attr;
}

static inline __bool __pgtblhdlr__pgtbla64s2_is_readwrite(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e type,
							  struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return __pgtblhdlr__pgtbla64s2_check_s2ap(type, raw->v, PGTBLA64S2_ATTR_S2AP_READ_WRITE);
}

static inline __bool __pgtblhdlr__pgtbla64s2_is_readable(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e type,
							 struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return (__pgtblhdlr__pgtbla64s2_check_s2ap(type, raw->v, PGTBLA64S2_ATTR_S2AP_READ_ONLY) ||
		__pgtblhdlr__pgtbla64s2_check_s2ap(type, raw->v, PGTBLA64S2_ATTR_S2AP_READ_WRITE));
}

static inline __bool __pgtblhdlr__pgtbla64s2_is_writable(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e type,
							 struct __pgtblhdlr_rawfmt_a64s2pgentry_s *raw)
{
	return (__pgtblhdlr__pgtbla64s2_check_s2ap(type, raw->v, PGTBLA64S2_ATTR_S2AP_WRITE_ONLY) ||
		__pgtblhdlr__pgtbla64s2_check_s2ap(type, raw->v, PGTBLA64S2_ATTR_S2AP_READ_WRITE));
}

static __pgtblhdlr_inline ____TYPE_VAL __PGTBLHDLR_NAMING_LOWERCASE(pgentry_decode_val)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_a64s2_rawfmt_base_t v)
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

static __pgtblhdlr_inline __bool __PGTBLHDLR_NAMING_LOWERCASE(pgentry_val_is_present)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __pgtblhdlr_a64s2_rawfmt_base_t v)
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

static __pgtblhdlr_inline __pgtblhdlr_a64s2_rawfmt_base_t __PGTBLHDLR_NAMING_LOWERCASE(pgentry_prepare_val)
	(enum ____PGENTRY_TYPE t, const struct ____PGCONFIG_USER *cfg, __paddr_t pa)
{
	__pgtblhdlr_a64s2_rawfmt_base_t result = __PGTBLHDLR_A64S2_RAWFMT_BASE_T_INVAL;

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

static inline __always_inline __pgtblhdlr_a64s2_rawfmt_base_t *
__pgtblhdlr_pgtbla64s2_pgentry_to_ptr(struct __pgtblhdlr_pgtbla64s2_pgentry_s e)
{
	return e.u.invalid;
}

#define __pgtblhdlr_pgtbla64s2_align_down(v, a) ((__paddr_t)(v) & ~((__paddr_t)(a) - (__paddr_t)(1ull)))

static inline __always_inline __paddr_t
__pgtblhdlr_pgtbla64s2_key_next(enum __pgtblhdlr_pgtbla64s2_pgentry_type_e parent_type,
				const struct __pgtblhdlr_pgtbla64s2_pgucfg_s *cfg,
				__paddr_t ipa, __paddr_t ipa_end)
{
	unsigned long nlv_size = __pgtblhdlr_pgtbla64s2_pgentry_nextlv_key_range(parent_type, cfg);
	__paddr_t ipa_down = __pgtblhdlr_pgtbla64s2_align_down(ipa, nlv_size);
	return ((ipa_end - ipa_down) > nlv_size) ? (ipa_down + nlv_size) : ipa_end;
}

#undef __PGTBLHDLR_BASICTYPES_KEY
#undef __PGTBLHDLR_BASICTYPES_KEY_MAX
#undef __PGTBLHDLR_BASICTYPES_VAL
#undef __PGTBLHDLR_BASICTYPES_INVAL

#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE
#undef __PGTBL_DESC
#endif
