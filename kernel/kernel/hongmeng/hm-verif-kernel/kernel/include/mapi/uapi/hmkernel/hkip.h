/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: kip protection header for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 01 16:48:30 2023
 */

#ifndef UAPI_HMKERNEL_HKIP_H
#define UAPI_HMKERNEL_HKIP_H

#define HKIP_ENABLE 1
#define HKIP_DISABLE 0

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
	__HKIP_SEC_REGION_PROTECT_SYSMGR,
	__HKIP_SEC_REGION_PROTECT_SERVICES,
	__HKIP_SEC_REGION_DISABLE_FORBIDDEN,
	__HKIP_SEC_REGION_DISABLE,
};

enum __hkip_rowm_type {
	/* write memory in el2 after rowm register */
	__HKIP_ROWM_WRITE,
	__HKIP_ROWM_SET,
};

#endif
