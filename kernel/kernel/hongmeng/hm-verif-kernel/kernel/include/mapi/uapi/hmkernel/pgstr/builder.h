/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Builder of page structure handler functions - fix pgtblhdlr/builder.h
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 03 02:33:48 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_PGSTR_BUILDER_H
#define MAPI_UAPI_HMKERNEL_PGSTR_BUILDER_H

#include <hmasm/memory.h>
#include <hmkernel/types.h>
#include <hmasm/pgstr/basictypes.h>

#define __PGTBLHDLR_BASICTYPES_KEY		__paddr_t
#define __PGTBLHDLR_BASICTYPES_KEY_MAX		(~(__paddr_t)(0))
#define __PGTBLHDLR_BASICTYPES_VAL		__ARCH_PGSTR_VAL_TYPE
#define __PGTBLHDLR_BASICTYPES_INVAL		__ARCH_PGSTR_VAL_INVAL

#define __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE pgstr_
#define __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE PGSTR_

#include <hmkernel/pgtblhdlr/naming.h>

#define __PGTBLHDLR_GENERATOR

#define __PGTBL_DESC <hmasm/pgstr/pgstrdesc.h>
#include <hmkernel/pgtblhdlr/builder.h>

/*
 * cmake doesn't use gcc -Wp,-MD. Instead, it directly scan source file
 * with regexp  "^[ \t]*[#%][ \t]*(include|import)[ \t]*[<\"]([^\">]+)([\">])"
 * to find dependencies. To make dependency tracing correctly, we add a fake
 * include here. However, wrap it with a never defined macro so it would not
 * compiled.
 */
#ifdef CHEAT_CMAKE_C_INCLUDE_REGEX_SCAN
# include <hmasm/pgstr/pgstrdesc.h>
#endif

static __pgtblhdlr_inline struct pgstr_s *__PGTBLHDLR_NAMING_LOWERCASE(pgentry_pgstr_ptr)
	(enum ____PGENTRY_TYPE t, struct ____PGENTRY e)
{
	struct pgstr_s *ret = __NULL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		ret = __pgtblhdlr_##n##_pgstr_ptr(e.u.raw); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		ret = __NULL;
		break;
	}
	return ret;
}

static __pgtblhdlr_inline __ARCH_PGSTR_VAL_TYPE *__PGTBLHDLR_NAMING_LOWERCASE(pgentry_table_ptr)
	(enum ____PGENTRY_TYPE t, struct ____PGENTRY e)
{
	__ARCH_PGSTR_VAL_TYPE *ret = __NULL;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		ret = __pgtblhdlr_##n##_table_ptr(e.u.raw); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		ret = __NULL;
		break;
	}
	return ret;
}

static __pgtblhdlr_inline __u32 __PGTBLHDLR_NAMING_LOWERCASE(pgentry_decode_pgstr_val)
	(enum ____PGENTRY_TYPE t, __u32 oldv)
{
	__u32 newv = oldv;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		newv = __pgtblhdlr_##n##_decode_pgstr_val(oldv); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		newv = oldv;
		break;
	}
	return newv;
}

static __pgtblhdlr_inline __ARCH_PGSTR_VAL_TYPE __PGTBLHDLR_NAMING_LOWERCASE(pgentry_decode_table_val)
	(enum ____PGENTRY_TYPE t, __ARCH_PGSTR_VAL_TYPE oldv)
{
	__ARCH_PGSTR_VAL_TYPE newv = oldv;

	switch (t) {
#define PGTBLHDLR_RAWCFG(raw)
#define PGTBLHDLR_RAWFMT(raw)
#define PGTBLHDLR_PGENTRY(n, N, raw, a, s) \
	case (__PGTBLHDLR_NAMING_UPPERCASE(PGENTRY_TYPE_##N)): { \
		newv = __pgtblhdlr_##n##_decode_table_val(oldv); \
		break; \
	}
#define PGTBLHDLR_CONN(n, N, raw, nx, NX, rawx)
#include __PGTBL_DESC
#undef PGTBLHDLR_CONN
#undef PGTBLHDLR_PGENTRY
#undef PGTBLHDLR_RAWFMT
#undef PGTBLHDLR_RAWCFG
	default:
		newv = oldv;
		break;
	}
	return newv;
}

#undef __PGTBLHDLR_BASICTYPES_KEY
#undef __PGTBLHDLR_BASICTYPES_KEY_MAX
#undef __PGTBLHDLR_BASICTYPES_VAL
#undef __PGTBLHDLR_BASICTYPES_INVAL

#define __PGTBLHDLR_PGSTRENTRY_TYPE_INVALID	__PGTBLHDLR_PGSTR_PGENTRY_TYPE_INVALID

#undef __PGTBL_DESC
#undef __PGTBLHDLR_GENERATOR
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE
#undef __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE

#endif
