/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Errno
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 01:29:43 2018
 */
#ifndef UAPI_HMKERNEL_ERRNO_H
#define UAPI_HMKERNEL_ERRNO_H

#ifndef __ASSEMBLY__

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

/* Support at most 512 errno for each cap */
/* we use 5 bits for captype (at most 32 caps). See kernel/capability/cap.h */
#define __HMK_ERR_CODE_BITS	9U
#define __HM_ERR_CAP_MASK	((1u << __CAP_BITS) - 1u)
#define __HM_MKERR(cap, code)	\
	(-1 * (__s32)((((__u32)(cap) & __HM_ERR_CAP_MASK) << __HMK_ERR_CODE_BITS) | (__u32)(code)))
#define __HM_ERR_CODE(err)	((~((__u32)(err) - 1U)) & ((1U << __HMK_ERR_CODE_BITS) - 1U))
#define __HM_ERR_CAP(err)	(((~((__u32)(err) - 1U)) >> __HMK_ERR_CODE_BITS) & __HM_ERR_CAP_MASK)

enum {
#define __HM_EDEF(name, posix, desc) __POS_ERRNO_##name,
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_common.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF
};

#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc)     __hm_kerrno_base_##type##_##ename,
#define CAPTYPE(type, num, grant, content)	\
	enum __hm_kerrno_base_##type {		\
		__hm_kerrno_base_##type##_NULL = 0,\
		content				\
		__hm_kerrno_base_##type##_MAX	\
	};
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD

/*
 * NOTE: Don't use __E_HM_XXX directly, use ERR_AS_ENUM(E_HM_XXX)
 * or ERR_AS_INT(E_HM_XXX).
 */
enum __hm_kerrno {
	/* kernel errno */
#define __HM_EDEF(name, posix, desc)			\
	__##name = __HM_MKERR(0, __POS_ERRNO_##name),
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_common.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF

	/* cap errno */
#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc)		\
	__E_HM_##ename = __HM_MKERR(hmobj_##type,	\
				    __hm_kerrno_base_##type##_##ename),
#define CAPTYPE(type, num, grant, content)		\
			content
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD
};

/* kernel errno */
#define __HM_EDEF(name, posix, desc) static const int name = (int)__##name;
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_common.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF

/* cap errno */
#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc)		\
	static const int E_HM_##ename = (int)__E_HM_##ename;
#define CAPTYPE(type, num, grant, content)		\
			content
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD

#define ERR_AS_ENUM(ename)	__##ename
#define ERR_AS_INT(ename)	((int)__##ename)

typedef __rawattr_errorable__(int, enum __hm_errno) __hmerr_t;

static inline __bool IS_REF_ERR(__u64 cref)
{
	return ((cref >> 32U) == 0xffffffffULL) || ((cref >> 32U) == 0ULL) ? __true : __false;
}

static inline __bool IS_REF_VALID(__u64 cref)
{
	return !IS_REF_ERR(cref);
}

static inline int REF_TO_ERR(__u64 cref)
{
	int err = E_HM_OK;
	__u64 _cref = 0ULL;

	if (IS_REF_ERR(cref)) {
		_cref = cref & 0xffffffffULL;
		err = (_cref != 0ULL && (_cref & ((1UL << (__HMK_ERR_CODE_BITS + __CAP_BITS)) - 1UL)) != 0ULL)
		       ? (int)_cref : E_HM_KERNEL_MAX;
	}
	return err;
}

static inline __u64 ERR_TO_REF(int err)
{
	unsigned int tmperr = (unsigned int)err & 0xffffffffU;

	return (__u64)tmperr;
}

static inline __bool is_cnode_idx_err(__u32 cnode_idx)
{
	return (((cnode_idx >> 31) == 0x1U) || (cnode_idx == CNODE_IDX_INVALD_ZERO)) ? __true : __false;
}

static inline __bool is_cnode_idx_valid(__u32 cnode_idx)
{
	return !is_cnode_idx_err(cnode_idx);
}

static inline int cnode_idx_to_err(__u32 cnode_idx)
{
	int err = E_HM_OK;

	if (is_cnode_idx_err(cnode_idx)) {
		err = (cnode_idx == CNODE_IDX_INVALD_ZERO) ?
		      E_HM_KERNEL_MAX : ((int)(unsigned int)(cnode_idx | 0x80000000U));
	}
	return err;
}

static inline __u32 err_to_cnode_idx(int err)
{
	return ((__u32)(((unsigned int)err) | 0x80000000U));
}

/*
 * Using macros to define three HM error codes with ambiguous semantics.
 * Follwing macro defined errnos(EXIST/FAULT/NOENT) are deprecated, do not use.
 */
#if !defined(E_HM_EXIST) || !defined(__E_HM_EXIST)
#define E_HM_EXIST		E_HM_POSIX_EXIST
#define __E_HM_EXIST		__E_HM_POSIX_EXIST
#endif /* E_HM_EXIST */

#if !defined(E_HM_FAULT) || !defined(__E_HM_FAULT)
#define E_HM_FAULT		E_HM_POSIX_FAULT
#define __E_HM_FAULT		__E_HM_POSIX_FAULT
#endif /* E_HM_FAULT */

#if !defined(E_HM_NOENT) || !defined(__E_HM_NOENT)
#define E_HM_NOENT		E_HM_POSIX_NOENT
#define __E_HM_NOENT		__E_HM_POSIX_NOENT
#endif /* E_HM_NOENT */

#endif /* !__ASSEMBLY__ */

#define __M_E_HM_OK			0
#define __M_E_HM_POSIX_EXIST		-1
#define __M_E_HM_POSIX_FAULT		-2
#define __M_E_HM_POSIX_NOENT		-3
#define __M_E_HM_INVAL			-4
#define __M_E_HM_ALIGN			-5
#define __M_E_HM_OVERFLOW		-6
#define __M_E_HM_FAULT_SRC		-7
#define __M_E_HM_FAULT_DST		-8
#define __M_E_HM_PERM			-9
#define __M_E_HM_DEAD			-10
#define __M_E_HM_INTR			-11
#define __M_E_HM_NOMEM			-12
#define __M_E_HM_AGAIN			-13
#define __M_E_HM_IDLE			-14
#define __M_E_HM_NOEXEC			-15
#define __M_E_HM_TOOMANY		-16
#define __M_E_HM_NOSYS			-17
#define __M_E_HM_SRCH			-18
#define __M_E_HM_CHILD			-19
#define __M_E_HM_IO			-20
#define __M_E_HM_BUSY			-21
#define __M_E_HM_TIMEDOUT		-22
#define __M_E_HM_DEADLK			-23
#define __M_E_HM_HMSIGINTR		-24
#define __M_E_HM_TRIGGER_STATE		-25
#define __M_E_HM_TRIGGER_NO_CURR	-26
#define __M_E_HM_PAGE_IN_USE		-27
#define __M_E_HM_PAGE_SRC_STATE		-28
#define __M_E_HM_PAGE_DST_STATE		-29
#define __M_E_HM_NON_UNIFORM		-30
#define __M_E_HM_FORMAT			-31
#define __M_E_HM_INCONSISTENT		-32
#define __M_E_HM_TAILCALL		-33
#define __M_E_HM_PIPE			-34
#define __M_E_HM_OBJEXIST		-35
#define __M_E_HM_RESFAULT		-36
#define __M_E_HM_NOOBJ			-37
#define __M_E_HM_CTRLMEM_ARGFMT		-38
#define __M_E_HM_RANGE			-39
#define __M_E_HM_BADF			-40
#define __M_E_HM_MFILE			-41
#define __M_E_HM_NOTSOCK		-42
#define __M_E_HM_SPIPE			(-43)
#define __M_E_HM_OPNOTSUPP		(-44)
#define __M_E_HM_NODEV			(-45)
#define __M_E_HM_LOOP		(-46)
#define __M_E_HM_NODATA		(-47)
#define __M_E_HM_RESTARTSYS		(-48)
#define __M_E_HM_RESTARTBLOCK		(-49)
#define __M_E_HM_RESTARTNOHAND		(-50)
#define __M_E_HM_RESTARTNOINTR		(-51)
#define __M_E_HM_KERNEL_MAX		(-52)
/*
 * NOTE: Don't use __M_E_HM_XXX directly, use ERR_AS_MACRO(E_HM_XXX).
 */
#define ERR_AS_MACRO(ename)	__M_##ename

#endif
