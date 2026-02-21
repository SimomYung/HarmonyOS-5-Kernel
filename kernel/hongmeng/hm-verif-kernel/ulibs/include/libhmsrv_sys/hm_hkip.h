/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The interface of hkip
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 19 19:04:31 2023
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_HKIP_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_HKIP_H

#include <libsysif/sysmgr/api.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hongmeng/panic.h>

enum __hkip_protect_type {
	/* read only protection */
	__HKIP_RO_REGISTER,
	__HKIP_RO_REGISTER_REVOCABLE,
	__HKIP_RO_REVOKE,
	/* read only and execute protection */
	__HKIP_ROX_TEXT_REGISTER,
	/* read only in el1, allow write and execute in el2 */
	__HKIP_ROWM_REGISTER,
	__HKIP_ROWM_REGISTER_REVOCABLE,
	__HKIP_ROWM_REVOKE,
	__HKIP_PROTECT_KERNEL_CODE,
	__HKIP_PROTECT_USER_CODE,
};

enum __hkip_tests_type {
	__TEST_HKIP_SYS_RO,
	__TEST_HKIP_SYS_RO_AFTER_INIT,
	__TEST_HKIP_SYS_CONST_RO,
	__TEST_HKIP_SYS_RO_REVOCABLE,
	__TEST_HKIP_SYS_ROWM_REVOCABLE,
	__TEST_HKIP_SYS_ROWM_CPY,
	__TEST_HKIP_SYS_ROWM_REVOCABLE_CPY,
	__TEST_HKIP_SYS_ROWM_SET,
	__TEST_HKIP_SYS_ROWM_REVOCABLE_SET,
	__TEST_HKIP_SYS_REG_SCTLR,
	__TEST_HKIP_SYS_REG_TTBR,
	__TEST_HKIP_SYS_CODE_DATA,
	__TEST_HKIP_SYS_REG_TCR,
	__TEST_HKIP_SYS_REG_MAIR,
	__TEST_HKIP_SYS_REG_AFSR,
	__TEST_HKIP_SYS_EL1_REG,
	__TEST_HKIP_SMC_FILTER,
	__TEST_HKIP_SYS_RO_FAIL,
	__TEST_HKIP_RO,
	__TEST_HKIP_ROX,
	__TEST_HKIP_ROWM,
	__TEST_HKIP_ROWM_CPY,
	__TEST_HKIP_ROWM_SET,
	__TEST_HKIP_CODE,
	__TEST_HKIP_CONST_RO,
        __TEST_HKIP_FGT_ENABLE,
};

int hm_hkip_protect_ro(const void *dst, size_t size);
int hm_hkip_protect_ro_revocable(const void *dst, size_t size);
int hm_hkip_protect_ro_revoke(const void *dst, size_t size);
int hm_hkip_protect_rox(const void *dst, size_t size);
int hm_hkip_protect_rowm(const void *dst, size_t size);
int hm_hkip_protect_rowm_revocable(const void *dst, size_t size);
int hm_hkip_protect_rowm_revoke(const void *dst, size_t size);

int hm_hkip_rowm_cpy(const void *dst, const void *src, size_t size);
int hm_hkip_rowm_set(const void *dst, uint8_t val, size_t size);
int hm_hkip_get_status(void);
int hm_hkip_protect_proc(void);

#ifdef CONFIG_SYSMGR_FAULT_INJECT
int hm_hkip_write_evil_data(const void *vaddr, int value);
#else
static inline int hm_hkip_write_evil_data(const void *vaddr, int value)
{
	UNUSED(vaddr, value);
	return E_HM_OK;
}
#endif

#define hm_hkip_rowm_cpy_nofail(dst, src, size)			\
do {													\
	int ___err;											\
	___err = hm_hkip_rowm_cpy(dst, src, size);			\
	if (___err < 0)										\
		panic("hkip rowm cpy failed, err=%d", ___err);	\
} while (1 != 1)

#endif  // ULIBS_INCLUDE_LIBHMSRV_SYS_HM_HKIP_H
