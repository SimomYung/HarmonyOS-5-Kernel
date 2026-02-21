/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Description: Implementation of errno
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 30 17:34:28 2018
 */

#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <hmkernel/capability.h>

#ifdef __KERNEL__

#include <hmkernel/kernel.h>
static_assert((VA_KERNEL_VSPACE_END <= __ERR_PTR_BASE_UPTR), small_ptr_base);

#else

#include <errno.h>
#include <string.h>
#include <lib/utils.h>
#include <hongmeng/errno.h>
#include <libsecure/bounds_check.h>

#endif /* __KERNEL__ */

#define HM_EDEF(name, posix, desc)     __errstr_BASE_##name,
#define HM_EDEF_P(name, posix, desc)   __errstr_BASE_##name,
enum __errstr_BASE_COUNTER {
#include <hmkernel/__errno_common.h>
#ifndef __KERNEL__
#include <hmkernel/__errno_user.h>
#endif
__errstr_BASE_NR,
};
#undef HM_EDEF_P
#undef HM_EDEF

/*
 * error code illustration
 */
#define __HM_EDEF(name, posix, desc) [__HM_ERR_CODE(__##name)] = (desc),
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
static const char *__errstr_BASE[(int)__errstr_BASE_NR] = {
#include <hmkernel/__errno_common.h>
#ifndef __KERNEL__
#include <hmkernel/__errno_user.h>
#endif
};
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF

#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc) [__hm_kerrno_base_##type##_##ename] = (desc),
#define CAPTYPE(type, num, grant, content) \
	static const char *__errstr_##type[__hm_kerrno_base_##type##_MAX] __attribute__((unused)) = { \
									 [0] = "Unknown hmobj_" #type " error",	\
									 content \
								       };
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD

static struct {
	const char **array;
	size_t array_sz;
} __errstr_matrix[(int)hmobj_MAX] = {
	[hmobj_NULL] = {__errstr_BASE, ARRAY_SIZE(__errstr_BASE)},
#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc)
#define CAPTYPE(type, num, grant, content) \
	[hmobj_##type] = {__errstr_##type, ARRAY_SIZE(__errstr_##type)},
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD
};

const char *hmstrerror(int error)
{
	enum hmobj cap = enum_hmobj_from_uint(__HM_ERR_CAP(error));
	unsigned int code = __HM_ERR_CODE(error);
	const char *p_ret = NULL;

	if (error >= 0) {
		p_ret = "Success";
	} else if (cap >= hmobj_MAX || (__errstr_matrix[cap].array_sz == 0UL)) {
		p_ret = "Unknwon error: wrongcap";
	} else {
		if (code >= __errstr_matrix[cap].array_sz) {
			code = 0U;
		}
		p_ret = __errstr_matrix[cap].array[code];
	}

	return p_ret;
}

#ifndef __KERNEL__
/*
 * check whether the error code definition in the kernel is consistent with that in the user space
 */
enum {
#define POSIX_ERRNO_DEF(name, posix) __TMP_##name = (posix),
#include <lib/__posix_errno.h>
#undef POSIX_ERRNO_DEF
};

#include <hongmeng/compiler.h>
#define POSIX_ERRNO_DEF(name, posix) raw_static_assert((name) == __TMP_##name, errno_not_consistent_##name);
#include <lib/__posix_errno.h>
#undef POSIX_ERRNO_DEF
#endif

#ifdef __KERNEL__
enum {
#define POSIX_ERRNO_DEF(name, posix) name = (posix),
#include <lib/__posix_errno.h>
#undef POSIX_ERRNO_DEF
};

enum {
	__POS_ERRNO_E_HM_USER_START = __POS_ERRNO_E_HM_KERNEL_MAX,
#define __HM_EDEF(name, posix, desc)  __POS_ERRNO_##name,
#define HM_EDEF(...)    __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...)  __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_user.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF
};

/*
 * NOTE: Don't use __E_HM_XXX directly, use ERR_AS_ENUM(E_HM_XXX)
 * or ERR_AS_INT(E_HM_XXX).
 */
enum {
#define __HM_EDEF(name, posix, desc) __##name = __HM_MKERR(0, __POS_ERRNO_##name),
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_user.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF
};
#endif

#define HM_EDEF(name, posix, desc)     __hm2p_errno_trans_BASE_##name,
#define HM_EDEF_P(name, posix, desc)   __hm2p_errno_trans_BASE_##name,
enum __hm2p_errno_trans_BASE_COUNTER {
#include <hmkernel/__errno_common.h>
#include <hmkernel/__errno_user.h>
__hm2p_errno_trans_BASE_NR,
};
#undef HM_EDEF_P
#undef HM_EDEF

/* mapping from hm errno to POSIX errno */
#define __HM_EDEF(name, posix, desc) [__HM_ERR_CODE(__##name)] = (posix),
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
static const int __hm2p_errno_trans_BASE[(int)__hm2p_errno_trans_BASE_NR] = {
#include <hmkernel/__errno_common.h>
#include <hmkernel/__errno_user.h>
};
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF

#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc)  [__hm_kerrno_base_##type##_##ename] = (perrno),
#define CAPTYPE(type, num, grant, content) \
static const int __hm2p_errno_trans_##type[__hm_kerrno_base_##type##_MAX] __attribute__((unused)) = { \
								    [0] = 0,	\
								    content	\
								  };
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD

static struct {
	const int *array;
	size_t array_sz;
} __hm2p_errno_trans_matrix[(int)hmobj_MAX] = {
	[hmobj_NULL] = {__hm2p_errno_trans_BASE, ARRAY_SIZE(__hm2p_errno_trans_BASE)},
#define CAPMETHOD(type, method)
#define CAPERRNO(type, perrno, ename, desc)
#define CAPTYPE(type, num, grant, content) \
	[hmobj_##type] = {__hm2p_errno_trans_##type, ARRAY_SIZE(__hm2p_errno_trans_##type)},
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD
};

/* the return value -1 means it's not a POSIX errno. */
int hmerrno2posix(int error)
{
	enum hmobj cap = enum_hmobj_from_uint(__HM_ERR_CAP(error));
	unsigned int code = __HM_ERR_CODE(error);

	if (error >= 0) {
		/* assume positive errno to be POSIX errno */
		__hmattr_sideff_resolved("no relative side effect");
		return error;
	}

	if (cap >= hmobj_MAX || (__hm2p_errno_trans_matrix[cap].array_sz == 0)) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	if (code >= __hm2p_errno_trans_matrix[cap].array_sz) {
		__hmattr_sideff_resolved("no relative side effect");
		return -1;
	}

	/*
	 * The error codes defined in macra HM_EDEF cannot be converted
	 * using the hmerrno2posix function.
	 * The case of ret == EHMERROR(EINVAL) and error != E_HM_INVAL.
	 */
	return __hm2p_errno_trans_matrix[cap].array[code];
}
#ifndef __KERNEL__
/*
 * mapping from POSIX errno to hm errno, only used in user space.
 *
 * For hm2p errno trans, different hm errno can result in the same
 * POSIX errno (hm kernel/user errno and cap errno could share the
 * same POSIX errno), for the backward trans, we don't account cap
 * errno, so we can still have the 1:1 errno mapping.
 *
 * Note that the target errno is E_HM_XXX, not the real error code,
 * because this is the right input for most errno trans functions
 * (for example hmstrerror()).
 */
#define HM_EDEF(name, posix, desc)
#define HM_EDEF_P(name, posix, desc) [posix] = __##name,
static const int __p2hm_errno_trans_BASE[] = {
#include <hmkernel/__errno_common.h>
#include <hmkernel/__errno_user.h>
};
#undef HM_EDEF_P
#undef HM_EDEF

/* the return value E_HM_USER_MAX means it's an invalid errno. */
int posix2hmerrno(int error)
{
	static const unsigned int array_sz = (const unsigned int)ARRAY_SIZE(__p2hm_errno_trans_BASE);
	unsigned int index = 0;
	int hmerror;

	if (error <= 0) {
		/* assume negative errno to be hm errno */
		__hmattr_sideff_resolved("no relative side effect");
		return error;
	}

	index = (unsigned int)error;
	if (index >= array_sz) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_USER_MAX;
	}

	index = array_index_fixup(index, array_sz);
	hmerror = __p2hm_errno_trans_BASE[index];
	if (hmerror == 0) {
		return E_HM_USER_MAX;
	}
	return hmerror;
}

#endif /* __KERNEL__ */
