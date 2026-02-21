/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define coobj related functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 9 20:48:28 2023
 */
#include <libhmsrv_sys/hm_coobj.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>

uintptr_t coobj_resv_va __ro_after_init;

#if defined(__HOST_LLT__) || defined(__HOST_UT__)
void *coobj_idx_to_ptr(uintptr_t ator_addr, size_t idx)
{
	void *ret = NULL;
	struct coobj_var_user_s *ator = ulong_to_ptr(ator_addr, struct coobj_var_user_s);

	if(ator != NULL) {
#ifdef __aarch64__
		ret = coobj_user_idx_to_ptr(ator, (__relpos_t)(idx >> COOBJ_RESERVED_MEM_ORDER));
#else
		ret = coobj_user_idx_to_ptr(ator, (__relpos_t)idx);
#endif
	}
	return ret;
}

#ifdef __aarch64__
void* coobj_offset_to_ptr(size_t idx)
{
	void *ret = NULL;
	size_t addr = (idx & COOBJ_RESERVED_MEM_MASK) + coobj_resv_va;
	ret = size_t_to_ptr(addr, void);
	return ret;
}
#endif
#endif

void coobj_free(uintptr_t ator_addr, size_t idx)
{
	struct coobj_var_uholder_s *ator = ulong_to_ptr(ator_addr, struct coobj_var_uholder_s);

	if (ator != NULL) {
#ifdef __aarch64__
		(void)coobj_var_free(ator, (__relpos_t)(idx >> COOBJ_RESERVED_MEM_ORDER));
#else
		(void)coobj_var_free(ator, (__relpos_t)idx);
#endif
	}
	return;
}

size_t coobj_malloc(uintptr_t ator_addr, size_t size)
{
	__relpos_t ret = ____COOBJ_INVALID_POS;
	struct coobj_var_uholder_s *ator = ulong_to_ptr(ator_addr, struct coobj_var_uholder_s);

	if (ator != NULL) {
		ret = coobj_var_malloc(ator, (__relpos_t)size);
	}
#ifdef __aarch64__
	if (ret != ____COOBJ_INVALID_POS) {
		struct coobj_var_user_s *uator = ulong_to_ptr(ator_addr, struct coobj_var_user_s);
		void *ptr = coobj_user_idx_to_ptr(uator, ret);
		ret = (ret << COOBJ_RESERVED_MEM_ORDER) +
		      ((ptr_to_size_t(ptr) - coobj_resv_va) & COOBJ_RESERVED_MEM_MASK);
	}
#endif
	return ret;
}
