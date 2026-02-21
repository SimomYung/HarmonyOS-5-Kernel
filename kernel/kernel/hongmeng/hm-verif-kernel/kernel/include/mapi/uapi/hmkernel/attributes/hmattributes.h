/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of hmattributes (uapi)
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 14 19:33:02 2020
 */
#ifndef MAPI_UAPI_HMKERNEL_HMATTRIBUTES_H
#define MAPI_UAPI_HMKERNEL_HMATTRIBUTES_H

#include <hmkernel/attributes/rawattributes.h>

/*
 * __hmattr_runtime_const(initializer, ...) decorates a global variable
 * declaration. It claims that this variable is a RUNTIME CONSTANT and is
 * initialized by the function `initializer`.
 *
 * A runtime constant is a variable whose value can be determined only in
 * runtime. Once the variable is initialized, no further changes can take
 * place. Natually, such varibles can be assigned only in its initializer.
 *
 * For example,
 *
 * >    int __hmattr_runtime_const(init) rconst;
 * >
 * >    void init() {
 * >        rconst = 1;
 * >    }
 * >
 * >    void f () {
 * >        __hmattr_loop_upper_bound(rconst)
 * >        for (int i = 0; i < rconst; i ++) {
 * >            ...
 * >        }
 * >    }
 *
 * @param initializer A function where the runtime constant variable is
 *                    initialized
 * @param ...         All the following parameters are taken as comments that
 *                    describes the reason why the given bound is correct.
 */
#define __hmattr_runtime_const(...)

/*
 * __hmattr_loop_upper_bound(bound, ...) declares a constant upper bound for
 * the decorated loop. It is suitebale for all kind of loops (for, while,
 * do-while) and iterators (dlist_for_each, etc.).
 *
 * It is IMPORTANT to notice that the upper bound is not the exact number of
 * iterations. For example, given bound = 10, we can use
 * __hmattr_loop_upper_bound(10) to decorate the loop once its iteration
 * number is LESS THAN OR EQUAL TO 10.
 *
 * @param bound A contant expression representing the maximal number of
 *              iterations in this loop. It can be numeric literal (10, 100,
 *              etc.) or expressions where only const integers are involved.
 *
 * @param ...   All the following parameters are taken as comments that
 *              describes the reason why the given bound is correct.
 */
#define __hmattr_loop_upper_bound(bound, ...)					\
	__rawattr_loop_variant(bound - LOOP_COUNTER)				\
	__rawattr_comment(#__VA_ARGS__)


/*
 * __hmattr_infinite_loop(...) explicitly tells that the decorated loop never
 * finishes. It is suitable for all kind of loops (for, while, do-while).
 *
 * We also suggest that this macro should be used together with the function
 * attribute "__noreturn" and the statement attribute "unreachable()" after
 * the loop body.
 *
 * @param ... All the parameters of this macro are taken as comments that
 *            describe why an infinite loop is required here.
 */
#define __hmattr_infinite_loop(...)						\
	__rawattr_loop_variant(-1)						\
	__rawattr_comment("this loop never terminates because " #__VA_ARGS__)

/*
 * __hmattr_finite_loop(...) explicitly tells that the declrated loop DOES
 * terminate within a finite number of iterations. It is suitable for
 * all kind of loops (for, while, do-while) and iterators (dlist_for_each,
 * etc.).
 *
 * This macro should be used ONLY when you don't know the precise boundary
 * of the decorated loop, or we lacks an approach to represent the boundary
 * of the loop. Otherwise it is suggested to use __hmattr_loop_upper_bound
 * instead.
 *
 * Whoever use this macro should consider whether we need to define a new
 * loop attribute macro to capture the termination condition of the loop
 * in a more precise manner.
 *
 * @param ... All parameters of this macro are taken as comments that describe
 *            why the decorated loop finally terminates.
 */
#define __hmattr_finite_loop(...)						\
	__rawattr_comment("this loop finally terminates because " #__VA_ARGS__)

/*
 * __hmattr_const_for_loop(bound, i, ...) is used to decorate a for loop with
 * a simple incremental counter `i` and a constant upper bound `bound`. e.g.
 *
 * >    for (i = 0; i < bound; i ++) { .... }
 *
 * @param bound The constant boundary of the loop
 * @param i     The loop counter
 * @param ...   All the rest parameters are taken as comments
 */
#define __hmattr_const_for_loop(bound, i, ...)					\
	__rawattr_loop_variant(bound - i)					\
	__rawattr_comment("this is 'for (" #i " = 0; i < " #bound "; i++) { }'"	\
			  "loop, where " #bound " is constant)." #__VA_ARGS__)

/*
 * __hmattr_simple_for_loop(bound, lim, i, ...) is used to decorate a for loop
 * with a simple incremental counter `i` and an over-approximated constant
 * boundary `bound`.
 *
 * >    for (i = 0; i < lim; i += STEP) { ... }
 * >                   ~~~~~
 * >             `lim` can be a complex expression (and the expression)
 * >             and whose evaluation may changes during the loop execution.
 * >             But it should always satisfies lim <= bound.
 *
 * where STEP must be a positive constant number.
 *
 * @param bound The over-approximated constant boundary of the loop
 * @param lim   The literal boundary of the loop
 * @param i     The loop counter
 * @param ...   All the rest parameters are taken as comments
 */
#define __hmattr_simple_for_loop(bound, lim, i, ...)				\
	__rawattr_loop_variant(bound - i)					\
	__rawattr_loop_invariant(i < lim)					\
	__rawattr_comment("this is 'for (" #i " = 0; " #i " < " #lim "; " #i	\
			  "++) { }' loop, where " #lim " is an variable and "	\
			  "has an upper bound " #bound "." #__VA_ARGS__)

/*
 * __hmattr_const_while_loop(bound, i) is used to decorate a while loop
 * with a simple incremental counter `i` and a constant upper bound `bound`. e.g.
 *
 * >    i = 0;
 * >    while (i < bound) {
 * >        ...
 * >        i += 1;
 * >        ...
 * >    }
 *
 * @param bound The constant boundary of the loop
 * @param i     The loop counter
 * @param ...   All the rest parameters are taken as comments
 */
#define __hmattr_const_while_loop(bound, i, ...)				\
	__rawattr_loop_variant(bound - i)					\
	__rawattr_comment("this is 'while (" #i " < " #bound ") { }' where "	\
			  "the boundary " #bound " is constant." #__VA_ARGS__)

/*
 * __hmattr_simple_while_loop(bound, lim, i, ...) is used to decorate a while loop
 * with a simple incremental counter `i` and an over-approximated constant
 * boundary `bound`.
 *
 * >    i = 0;
 * >    while (i < lim) {
 * >              ~~~~~
 * >             `lim` can be a complex expression (and the expression)
 * >             and whose evaluation may changes during the loop execution.
 * >             But it should always satisfies lim <= bound.
 * >
 * >        ...
 * >        i += STEP;
 * >        ...
 * >    }
 *
 * where step must be a positive constant number.
 *
 * @param bound The over-approximated constant boundary of the loop
 * @param lim   The literal boundary of the loop
 * @param i     The loop counter
 * @param ...   All the rest parameters are taken as comments
 */
#define __hmattr_simple_while_loop(bound, lim, i, ...)				\
	__rawattr_loop_variant(bound - i)					\
	__rawattr_loop_invariant(i < lim)					\
	__rawattr_comment("this is 'while (" #i " < " #bound ") { }' where "	\
			  #lim " is an variable and has an upper bound " #bound	\
			  "." #__VA_ARGS__)

/*
 * __hmattr_dlist_for_each(bound, list, ...) is used to decorate a dlist
 * iterator where `bound` is the upper bound of `list`'s size.
 *
 * @param bound The constant boundary of the list size
 * @param list  The dlist to iterate on
 * @param ...   All the rest parameters are taken as comments
 */
#define __hmattr_dlist_for_each(bound, list, ...)				\
	__hmattr_loop_upper_bound(bound, "this is dlist for each iterator on "	\
				  "list " #list ". list length upper bound is "	\
				  #bound "." #__VA_ARGS__)

#ifndef __ASSEMBLY__

/*
 * __hmattr_sideff_resolved(reason) is a semantically trivial function aiming to
 * tell the code reviewers that before an early return statement, all the
 * side-effects (including locks, temporarily allocated objects, etc.) are
 * carefully checked and resolved (e.g. released). Any invocation of this function
 * is expected to be erased by the compilers.
 *
 * The function has no return value.
 *
 * @param reason a constant string telling the reviewer why and how the side
                 effect is resolved, or "no relative side effect"
 */
static inline void __hmattr_sideff_resolved(const char *reason)
{
	const char *_reason = reason;
	(void)(_reason);
}

#endif

#ifdef __CALIBUR_COMPILE__
#define __kitattr_base(n)                       __attribute__((decl_tags(n)))
#else
#define __kitattr_base(n)
#endif

#define __kitattr(name, value)                  __kitattr_base(#name ":" #value)

#define __kit_state(n)                          __kitattr(state, n)

#define __kit_shape_dlist(n)                    __kitattr(shape.dlist, n)
#define __kit_shape_dlist_node(n)               __kitattr(shape.dlist_node, n)
#define __kit_shape_rbtree(n)			__kitattr(shape.rbtree, n)
#define __kit_shape_rbtree_node(n)		__kitattr(shape.rbtree_node, n)

#define __hmattr_tag(...)
#ifndef __ASSEMBLY__
#define __hmattr_tag_begin(...)			do { } while(0)
#define __hmattr_tag_end(...)			do { } while(0)
#else
#define __hmattr_tag_begin(...)
#define __hmattr_tag_end(...)
#endif

#endif
