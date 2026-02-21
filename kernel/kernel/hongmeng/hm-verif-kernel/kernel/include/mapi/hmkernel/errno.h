/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Commonly used error handling utilities exported for module.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 17 16:57:05 2019
 */
#ifndef MAPI_HMKERNEL_ERRNO_H
#define MAPI_HMKERNEL_ERRNO_H

#include <uapi/hmkernel/errno.h>

#ifndef __ASSEMBLY__

#define __ERR_PTR_BASE_UPTR (((uptr_t)(-1) - (1UL << (__HMK_ERR_CODE_BITS + __CAP_BITS))) + 1UL)
#define __ERR_PTR_BASE ((void *)__ERR_PTR_BASE_UPTR)

/*
 * Following error num is only used
 * in kernel and module, and they are behind E_HM_KERNEL_MAX.
 * In the future, if more error num should be define for kernel,
 * we can use HM_EDEF_K in __errno_common.h to generate them.
 */
#define __E_HM_RETVAL __HM_MKERR(0, (__POS_ERRNO_E_HM_KERNEL_MAX + 1))
#define __E_HM_AGAIN_INTERNAL __HM_MKERR(0, (__POS_ERRNO_E_HM_KERNEL_MAX + 2))
#define __E_HM_PAGE_FAULT __HM_MKERR(0, (__POS_ERRNO_E_HM_KERNEL_MAX + 3))

static const int E_HM_RETVAL = (int)__E_HM_RETVAL;
static const int E_HM_AGAIN_INTERNAL = (int)__E_HM_AGAIN_INTERNAL;
static const int E_HM_PAGE_FAULT = (int)__E_HM_PAGE_FAULT;

static inline bool IS_PTR_ERR(const void *ptr)
{
	return ptr > __ERR_PTR_BASE;
}

static inline int PTR_TO_ERR(const void *ptr)
{
	int ret = 0;
	uptr_t tmp_ptr = 0UL;

	if (IS_PTR_ERR(ptr)) {
		tmp_ptr = (uptr_t)(ptr) - (uptr_t)(__ERR_PTR_BASE);
		ret = -1 * (int)tmp_ptr;
	}

	return ret;
}

static inline void *__ERR_TO_PTR(int err)
{
	void *ret = NULL;

	if (err != E_HM_OK) {
		unsigned int uerr;

		uerr = (unsigned int)(-err);
		uerr = uerr & ((1u << (__HMK_ERR_CODE_BITS + __CAP_BITS)) - 1u);
		ret = (void *)((uptr_t)__ERR_PTR_BASE + uerr);
	}
	return ret;
}

#define __ERR_TO_PTR_TYPE(err, type) ({type *__p; __p = (type *)(uptr_t)(__ERR_TO_PTR(err)); __p;})
#define ERR_TO_TCB(err) __ERR_TO_PTR_TYPE(err, struct tcb_s)

#endif /* !__ASSEMBLY__ */
#endif
