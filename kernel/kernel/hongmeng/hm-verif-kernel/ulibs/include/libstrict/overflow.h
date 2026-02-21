/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: No-overflow arithmetic macros
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 22 15:56:32 CST 2019
 */
#ifndef ULIBS_INCLUDE_LIBSTRICT_OVERFLOW_H
#define ULIBS_INCLUDE_LIBSTRICT_OVERFLOW_H

#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

/* This header generate following macro, which is used to wrap up
 * MUL, ADD and SUB operation between two INTEGER number. Purpose
 * is avoid INTEGER_OVERFLOW CodexDex warning.
 *
 * 1. noovfl_sadd(rs, op1, op2, message), noovfl_uadd(rs, op1, op2, message):
 *    res = op1 + op2, op1 and op2 must be (unsigned)int, (unsigned)long
 *    or (unsigned)long long type. It make sure no INTEGER_OVERFLOW take
 *    place, otherwise print message and panic.
 *
 * 2. noovfl_ssub(res, op1, op2, message), noovfl_usub(rs, op1, op2, message):
 *    res = op1 - op2, op1 and op2 must be (unisgned)int, (unsigned)long
 *    or (unsigned)long long type. It make sure no INTEGER_OVERFLOW take
 *    place, otherwise print message and panic.
 *
 * 3. noovfl_umul(res, op1, op2, message):
 *    res = op1 * op2, op1 and op2 must be unsigned int, unsigned long
 *    or unsigned long long. It make sure no INTEGER_OVERFLOW take place,
 *    otherwise print message and panic.
 *
 * Some macro spell:
 * __noovfl_not, __noovfl_bool, __noovfl_if_else,
 * __noovfl_eval, __noovfl_defer and __noovfl_map
 *
 * NOT: Cast no comma expression to 0, and cast 0 to 1
 * Usage:
 * NOT(no comma expression)
 *
 * Flow:
 * 1. NOT(foo bar) --> SECOND(NO_foo bar) --> SECOND(No_foo bar, 0) --> 0
 * or
 * 2. NOT(0) --> SECOND(NO_0) --> SECOND(!, 1, 0) --> 1
 *
 * Example:
 * 1. NOT(foo)
 * --> MAKE_BOOL(CAT(NOT_, foo))
 * --> MAKE_BOOL(NOT_## foo)
 * --> MAKE_BOOL(NOT_foo)
 * --> SECOND(NOT_foo, 0)
 * --> 0
 *
 * 2. NOT(0)
 * --> MAKE_BOOL(NOT_## 0)
 * --> MAKE_BOOL(NOT_0)
 * --> MAKE_BOOL(!, 1)
 * --> SECOND(!, 1, 0)
 * --> 1
 *
 * BOOL: Cast no comma expression to 1, and 0 to 0
 * Usage:
 * BOOL(no comma expression)
 *
 * Flow:
 * 1. BOOL(foo bar) --> NOT(NOT(foo bar)) --> NOT(0) --> 1
 * or
 * 2. BOOL(0) --> NOT(NOT(0)) --> NOT(1) --> 0
 */
#define __noovfl_cat(a, b) a ## b
#define __noovfl_second(a, b, ...) b
#define __noovfl_make_bool(...) __noovfl_second(__VA_ARGS__, 0)
#define __NOOVFL_NOT_0 !, 1
#define __noovfl_not(v)  __noovfl_make_bool(__noovfl_cat(__NOOVFL_NOT_, v))
#define __noovfl_bool(v) __noovfl_not(__noovfl_not(v))

/* IF_ELSE: C-style if...else... statement
 * Usage:
 * IF_ELSE(no comma expression)(true statement)(false statement)
 *
 * Flow:
 * 1. IF_ELSE(...) --> IF_ELSE(1) or IF_ELSE(0)
 * then
 * 2. IF_ELSE(1)(if-code)(else-code) --> IF_1(if-code)(else-code)
 * --> if-code IF_1_ELSE(else-code) --> if-code
 * or
 * 3. IF_ELSE(0)(if-code)(else-code) --> IF_0(if-code)(else-code)
 * --> IF_0_ELSE(else-code) --> else-code
 *
 * Example:
 * 1. IF_ELSE(foo bar)(if-code)(else-code)
 * --> IF_BOOL(BOOL(foo bar))(if-code)(else-code)
 * --> IF_BOOL(1)(if-code)(else-code)
 * --> CAT(IF_, 1)(if-code)(else-code)
 * --> IF_## 1(if-code)(else-code)
 * --> IF_1(if-code)(else-code)
 * --> IF_1(if-code)(else-code)
 * --> if-code IF_1_ELSE(else-code)
 * --> if-code
 *
 * 2. IF_ELSE(0)(if-code)(else-code)
 * --> IF_BOOL(BOOL(0))(if-code)(else-code)
 * --> IF_BOOL(0)(if-code)(else-code)
 * --> CAT(IF_, 0)(if-code)(else-code)
 * --> IF_## 0(if-code)(else-code)
 * --> IF_0(if-code)(else-code)
 * --> IF_0_ELSE(else-code)
 * --> else-code
 */
#define __noovfl_if_else(condition)  __noovfl_if_bool(__noovfl_bool(condition))
#define __noovfl_if_bool(boolean) __noovfl_cat(__noovfl_if_, boolean)
#define __noovfl_if_1(...) __VA_ARGS__ __noovfl_if_1_else
#define __noovfl_if_0(...) __noovfl_if_0_else
#define __noovfl_if_1_else(...)
#define __noovfl_if_0_else(...) __VA_ARGS__

/* HAS_ARGS: Dectect empty argument list
 * Usage:
 * HAS_ARGS(argument list)
 *
 * Flow:
 * 1. HAS_ARGS(foo, bar) --> FIRST(0 foo, bar) --> BOOL(0_foo) --> 1
 * or
 * 2. HAS_ARGS() --> FIRST(0) --> BOOL(0) --> 0
 *
 * Example:
 * 1. HAS_ARGS(foo, bar)
 * --> BOOL(FIRST(EMPTY_ARGS foo, bar))
 * --> BOOL(EMPTY_ARGS foo)
 * --> NOT(NOT(EMPTY_ARGS foo))
 * --> NOT(0)
 * --> 1
 *
 * 2. HAS_ARGS()
 * --> BOOL(FIRST(EMPTY_ARGS))
 * --> BOOL(EMPTY_ARGS)
 * --> BOOL(0)
 * --> 0
 */
#define __noovfl_first(a, ...) a
#define __noovfl_has_args(...) __noovfl_bool(__noovfl_first(__noovfl_empty_args __VA_ARGS__)())
#define __noovfl_empty_args() 0

/* EVAL: Force C-Preprocessor to make new pass on recursive expression
 * Usage:
 * EVAL(recursive expression)
 *
 * How to work:
 * 1. CPP recursively expands macro's arugments before substituting the
 *    macro' body and expanding then.
 * 2. CPP doesn't allow to expand itself in macro' body. But expansion
 *    takes place in new pass.
 *
 * Example:
 * #define Bar()
 * #define Foo(v) Hello v!
 * Given a expression: Foo Bar() (Mike)
 * Then CPP scan from the first token:
 * @Foo Bar() (Hello)     # @ means the scan indexer of CPP.
 * --> Foo @Bar()(Hello)  # expand Bar()
 * --> Foo @(Hello)
 * --> Foo (Hello)@       # scan stop, Foo doesn't expand out
 *
 * But make use of EVAL():
 * @EVAL(Foo Bar()(Mike))      # expand arguments firstly.
 * --> EVAL(Foo @Bar() (Mike)) # expand Bar()
 * --> EVAL(Foo @(Mike))
 * --> EVAL(Foo (Mike)@)
 * --> @Foo (Mike)             # force CPP to scan from start again.
 * --> Hello Mike!             # expand Foo()
 *
 * DEFER: Defer macro expansion to next scan pass
 * Usage:
 * DEFER(expression)
 *
 * How to work:
 * Force CPP scan indexer skip the macro token to ()
 * #define Skip()
 * #define Foo(v) Hello v!
 * #define DEFER(m) m Skip()
 * Given a expression: Foo(Mike)
 * @Foo(Mike)             # @ means the scan indexer of CPP.
 * --> Hello Mike!        # expand Foo()
 * Given another expression: DEFER(Foo)(Mike)
 * @DEFER(Foo)(Mike)      # @ means the scan indexer of CPP
 * -->@Foo Skip()(Mike)   # expand DEFER()
 * -->Foo @SKip()(Mike)   # expand Skip()
 * -->Foo @(Mike)
 * -->Foo (Mike)          # scan stop, Foo doesn't expand out
 */
#define __noovfl_eval(...) __noovfl_eval1(__noovfl_eval1(__VA_ARGS__))
#define __noovfl_eval1(...) __VA_ARGS__
#define __noovfl_skip()
#define __noovfl_defer(m) m __noovfl_skip __noovfl_skip() ()

/* MAP: Python-style Lambda, apply clause to arguments list, clause fetch
 *      two elemnts from list each time.
 * Usage:
 * MAP(Clause, ending clause, v1, v2, t1, t2, ...)
 * And,
 * Clause(v1, v2, t1, t2)
 *
 * Example:
 * MAP(m, e, v1, v2, t1, t2, t3, t4, t5, t6)
 * --> m(v1, v2, t1, t2) IF_ELSE(HAS_ARGS)(MAP(m, e, t1, t2, t3, t4))(e)
 * --> m(v1, v2, t1, t2) IF_ELSE(1)(MAP(m, e, t1, t2, t3, t4))(e)
 * --> m(v1, v2, t1, t2) (MAP(m, e, t1, t2, t3, t4))
 * Or
 * --> m(v1, v2, t1, t2) IF_ELSE(0)(MAP(m, e, t1, t2, t3, t4))(e)
 * --> m(v1, v2, t1, t2) (e)
 *
 * How to work:
 * It makes use of EVAL and DEFER to implement a recursive macro, like
 * #define Calendar(n, vi, n, aka) Hello n vi n<aka>!
 * Given a expression: MAP(Foo, Tuesday, Today, Is, Monday, MON, Friday, FRI, Sunday, SUN)
 * @EVAL(MAP(Foo, Tuesday, Today, Is, Monday, MON, Friday, FRI, Sunday, SUN))
 * --> EVAL(@Foo(...) IF_ELSE(__VA_ARGS)(DEFER(_MAP)()(..., __VA_ARGS__))(Tuesday))
 * --> EVAL(... @IF_ELSE(__VA_ARGS__)(DEFER(_MAP)()(..., __VA_ARGS__))(Tuesday))
 * --> EVAL(... @IF_ELSE(1)(DEFER(_MAP)()(..., __VA_ARGS__))(Tuesday))
 * --> EVAL(... (@DEFER(_MAP)()(..., __VA_ARGS__)))
 * --> EVAL(... (@_MAP SKIP() ()(..., __VA_ARGS__)))                  # expand DEFER
 * --> EVAL(... (_MAP @SKIP() ()(..., __VA_ARGS__)))                  # defer _MAP
 * --> EVAL(... (_MAP @()(..., __VA_ARGS__)))                         # expand SKIP
 * --> EVAL(... (_MAP ()@(..., __VA_ARGS__)))
 * --> EVAL(... (_MAP ()(..., __VA_ARGS__))@)
 * --> EVAL1(@Hello, Today Is Monday<MON> (_MAP()(..., __VA_ARGS__))) # second pass
 * --> EVAL1(... (@_MAP()(..., __VA_ARGS__)))                         # expand _MAP
 * --> EVAL1(... (@MAP(..., __VA_ARGS__)))                            # expand MAP() again!
 * ....
 */
#define __noovfl_map(m, e, v1, v2, t1, t2, ...) \
	m(v1, v2, t1, t2) \
	__noovfl_if_else(__noovfl_has_args(__VA_ARGS__))\
	(__noovfl_defer(__noovfl_map_defer)()(m, e, v1, v2, __VA_ARGS__))(e)
#define __noovfl_map_defer() __noovfl_map

/* Clause: Chooser */
#define __noovfl_choose_opr(opr, t, tr, trn) \
	(__builtin_types_compatible_p(t, tr)  == 1) ? \
	(t)__noovfl_## opr ##_## trn(__op1, __op2, __message) :
#define __noovfl_choose_clz(val, t, tr, trn) \
	(__builtin_types_compatible_p(t, tr)  == 1) ? \
	__builtin_## trn((tr)(val)) :
#define __noovfl_clz(val) ({ \
	__noovfl_eval(__noovfl_map(__noovfl_choose_clz, -1, val, typeof(val), \
				   unsigned int, clz, unsigned long, clzl, \
				   unsigned long long, clzll)); })

/* Overflow constants */
#define __noovfl_is_signed(T)		((T)(-1) < (T)1)
#define __noovfl_bits_type(T)		(sizeof(T) * 8UL)
#define __noovfl_signed_type_max(T) \
	(((((T)1 << (__noovfl_bits_type(T) - 2UL)) - (T)1) << 1) + (T)1)
#define __noovfl_unsigned_type_max(T)	(~(T)0)
#define __noovfl_type_max(T) \
	(__noovfl_is_signed(T) ? \
	 __noovfl_signed_type_max(T) : __noovfl_unsigned_type_max(T))
#define __noovfl_type_min(T) \
	(__noovfl_is_signed(T) ? \
	 (T)((T)-1 - __noovfl_signed_type_max(T)) : 0)

/*
 * Check for the mul overflow between two unsigned integer number
 */
#ifdef CONFIG_STRICT_NO_OVERFLOW
#define noovfl_umul(rs, op1, op2, message) do { \
	BUG_ON((sizeof(rs) < sizeof(op1)) || (sizeof(rs) < sizeof(op2))); \
	typeof(rs) __op1 = (typeof(rs))(op1); \
	typeof(rs) __op2 = (typeof(rs))(op2); \
	int __op1_clz = __noovfl_clz(__op1); \
	BUG_ON(__op1_clz == -1); \
	int __op2_clz = __noovfl_clz(__op2); \
	BUG_ON(__op2_clz == -1); \
	int __res_bits = (int)__noovfl_bits_type(typeof(rs)); \
	if (__op1 != 0U && __op2 != 0U && __res_bits > (__op1_clz + __op2_clz)) { \
		hm_panic("ovfl: %s", message); \
	} \
	if (sizeof(rs) >= sizeof(uint64_t)) { \
		(rs) = (__op1 * __op2); \
	} else { \
		uint64_t u64_rs = ((uint64_t)__op1 * (uint64_t)__op2); \
		(rs) = (typeof(rs))(u64_rs); }} while (__false)
#else
#define noovfl_umul(rs, op1, op2, message) do { (rs) = (op1) * (op2); } while (__false)
#endif

/*
 * Check for the add overflow between two unsigned integer number
 */
#define __noovfl_gen_uadd(s, op, tr, trn) \
static inline tr \
__noovfl_uadd_## trn(tr __op1, tr __op2, const char *message) { \
	tr __max = __noovfl_type_max(tr); \
	if (__op1 > (__max - __op2)) { \
		hm_panic("ovfl: %s", message); \
	} \
	return __op1 + __op2; \
}
__noovfl_eval(__noovfl_map(__noovfl_gen_uadd, , , ,
		  unsigned int, ui, unsigned long, ul,
		  unsigned long long, ull))

#ifdef CONFIG_STRICT_NO_OVERFLOW
#define noovfl_uadd(rs, op1, op2, message) \
do { \
	BUG_ON(sizeof(rs) < sizeof(op1) || sizeof(rs) < sizeof(op2)); \
	typeof(rs) __op1 = (typeof(rs))(op1); \
	typeof(rs) __op2 = (typeof(rs))(op2); \
	const char *__message = message; \
	(rs) = __noovfl_eval(__noovfl_map(__noovfl_choose_opr, \
			       ({hm_panic("ovfl: unknown type\n"); (typeof(rs))0;}), \
			       uadd, typeof(rs), \
			       unsigned int, ui, unsigned long, ul, \
			       unsigned long long, ull)); \
} while (__false)
#else
#define noovfl_uadd(rs, op1, op2, message) do { (rs) = (op1) + (op2); } while (__false)
#endif

/*
 * Check for the add overflow between two signed integer number
 */
#define __noovfl_gen_sadd(s, op, tr, trn) \
static inline tr \
__noovfl_sadd_## trn(tr __op1, tr __op2, const char *message) { \
	tr __max = __noovfl_type_max(tr); \
	tr __min = __noovfl_type_min(tr); \
	if ((__op1 > 0 && __op2 > 0 && __op1 > (__max - __op2)) || \
	    (__op1 < 0 && __op2 < 0 && __op1 < (__min - __op2))) { \
		hm_panic("ovfl: %s", message); \
	} \
	return __op1 + __op2; \
}
__noovfl_eval(__noovfl_map(__noovfl_gen_sadd, , , ,
		  int, si, long, sl, long long, sll))

#ifdef CONFIG_STRICT_NO_OVERFLOW
#define noovfl_sadd(rs, op1, op2, message) \
do { \
	BUG_ON(sizeof(rs) < sizeof(op1) || sizeof(rs) < sizeof(op2)); \
	typeof(rs) __op1 = (typeof(rs))(op1); \
	typeof(rs) __op2 = (typeof(rs))(op2); \
	const char *__message = message; \
	(rs) = __noovfl_eval(__noovfl_map(__noovfl_choose_opr, \
			       ({hm_panic("ovfl: unknown type\n"); (typeof(rs))0;}), \
			       sadd, typeof(rs), \
			       int, si, long, sl, long long, sll)); \
} while (__false)
#else
#define noovfl_sadd(rs, op1, op2, message) do { (rs) = (op1) + (op2); } while (__false)
#endif

/* noovfl_madd64 with positive signed value */
static inline uint64_t __noovfl_madd64_p(uint64_t op1, int64_t op2,
					 const char *message)
{
#ifdef CONFIG_STRICT_NO_OVERFLOW
	uint64_t __max = __noovfl_type_max(uint64_t);

	if (__max - op1 < (uint64_t)op2) {
		hm_panic("ovfl: %s\n", message);
	}
#else
	UNUSED(message);
#endif

	return op1 + (uint64_t)op2;
}

/* no overflow addition of mixed uint64_t and int64_t */
static inline uint64_t noovfl_madd64(uint64_t op1, int64_t op2,
				     const char *message)
{
	uint64_t rc;

	if (op2 >= 0ll) {
		rc = __noovfl_madd64_p(op1, op2, message);
	} else {
		/* negative op2 won't cause overflow */
		rc = op1 - (uint64_t)(-op2);
	}

	return rc;
}

/*
 * Check for the sub overflow between two unsigned integer number
 */
#define __noovfl_gen_usub(s, op, tr, trn) \
static inline tr \
__noovfl_usub_## trn(tr __op1, tr __op2, const char *message) { \
	if (__op1 < __op2) {\
		hm_panic("ovfl: %s", message); \
	} \
	return __op1 - __op2; \
}
__noovfl_eval(__noovfl_map(__noovfl_gen_usub, , , ,
		  unsigned int, ui, unsigned long, ul,
		  unsigned long long, ull))

#ifdef CONFIG_STRICT_NO_OVERFLOW
#define noovfl_usub(rs, op1, op2, message) \
do { \
	BUG_ON(sizeof(rs) < sizeof(op1) || sizeof(rs) < sizeof(op2)); \
	typeof(rs) __op1 = (typeof(rs))(op1); \
	typeof(rs) __op2 = (typeof(rs))(op2); \
	const char *__message = message; \
	(rs) = __noovfl_eval(__noovfl_map(__noovfl_choose_opr, \
			       ({hm_panic("ovfl: unknown type\n"); (typeof(rs))0;}), \
			       usub, typeof(rs), \
			       unsigned int, ui, unsigned long, ul, \
			       unsigned long long, ull)); \
} while (__false)
#else
#define noovfl_usub(rs, op1, op2, message) do { (rs) = (op1) - (op2); } while (__false)
#endif

/*
 * Check for the sub overflow between two signed integer number
 */
#define __noovfl_gen_ssub(s, op, tr, trn) \
static inline tr \
__noovfl_ssub_## trn(tr __op1, tr __op2, const char *message) { \
	tr __max = __noovfl_type_max(tr); \
	tr __min = __noovfl_type_min(tr); \
	if ((__op1 > 0 && __op2 < 0 && __op1 > (__max + __op2)) || \
	    (__op1 < 0 && __op2 > 0 && __op1 < (__min + __op2))) {\
		hm_panic("ovfl: %s", message); \
	} \
	return __op1 - __op2; \
}
__noovfl_eval(__noovfl_map(__noovfl_gen_ssub, , , ,
		  int, si, long, sl, long long, sll))

#ifdef CONFIG_STRICT_NO_OVERFLOW
#define noovfl_ssub(rs, op1, op2, message) \
do { \
	BUG_ON(sizeof(rs) < sizeof(op1) || sizeof(rs) < sizeof(op2)); \
	typeof(rs) __op1 = (typeof(rs))(op1); \
	typeof(rs) __op2 = (typeof(rs))(op2); \
	const char *__message = message; \
	(rs) = __noovfl_eval(__noovfl_map(__noovfl_choose_opr, \
			       ({hm_panic("ovfl: unknown type\n"); (typeof(rs))0;}), \
			       ssub, typeof(rs), \
			       int, si, long, sl, long long, sll)); \
} while (__false)

#else
#define noovfl_ssub(rs, op1, op2, message) do { (rs) = (op1) - (op2); } while (__false)
#endif

#endif
