/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of raw attributes (uapi).
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 15 12:16:00 2020
 */
#ifndef MAPI_UAPI_HMKERNEL_RAWATTRIBUTES_H
#define MAPI_UAPI_HMKERNEL_RAWATTRIBUTES_H

/******************************************************************************
 *                                                                            *
 *                              RAW ATTRUBUTES                                *
 *                                                                            *
 * Raw attributes are used as the flexible basis of other hm-attributes. Dev- *
 * elopers should not directly use these attributes. Instead, it is suggested *
 * to use the __hmattr prefixed hmattributes.                                 *
 *                                                                            *
 * Static analyzers and formal verifiers can extend the raw attributes with   *
 * their own implementations. In this case, they are expected to define this  *
 * macro, so the following blank macros will be overrided.                    *
 *                                                                            *
 *****************************************************************************/
#define __rawattr_function_private(...)

#define __rawattr_variable_range(lbound, ubound)

#define __rawattr_verifier_assert(expr)

#define __rawattr_verifier_assume(expr)

#define __rawattr_shadow_code(code)

/*
 * __rawattr_comment(...) is a macro that takes arbitrary number of strings
 * as comments. The macro does not have ANY operational semantics.
 */
#define __rawattr_comment(...)

/*
 * __rawattr_loop_invariant(expr) declares an invariant of a loop.
 *
 * An invariant is a boolean expression which evaluates to true in the
 * beginning of each loop iteration.
 *
 * @param expr the invariant expression
 */
#define __rawattr_loop_invariant(expr)

/*
 * __rawattr_loop_variant(expr) declares a variant of a loop.
 *
 * A loop variant is a numeric expression which evaluates to a non-negative and
 * decreasing integer. In each iteration, the loop variant must be updated to a
 * smaller value. Hence a valid loop variant ensures the termination of the
 * loop.
 *
 * Consider "expr" as a loop variant, it implicitly implies the loop invariant
 * "expr >= 0".
 *
 * @param expr the variant expression, -1 when the loop does not have any
 *             possible variant (it never terminates)
 */
#define __rawattr_loop_variant(expr)

/**
 * __rawattr_errorable__(return_t, error_t) is a type macro indicating that
 * this is a type `return_t`, but its underlying value is an error which has
 * type `error_t`.
 *
 * @param return_t the real type
 * @param error_t the binded underlying error type
 */
#define __rawattr_errorable__(return_t, error_t) return_t

#endif
