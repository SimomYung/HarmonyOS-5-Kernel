/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: kip protection header for mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 01 16:48:30 2023
 */

#ifndef MAPI_HMKERNEL_HKIP_H
#define MAPI_HMKERNEL_HKIP_H

#include <uapi/hmkernel/hkip.h>

/*
 * Definitions for HM_KIP HVC function ids.
 */
#define HKIP_RO_REGISTER                0xC6001030
#define HKIP_RO_REGISTER_REVOCABLE      0xC6001032
#define HKIP_RO_REVOKE                  0xC6001033
#define HKIP_ROX_TEXT_REGISTER          0xC6001086
#define HKIP_ROWM_REGISTER		0xC6001040
#define HKIP_ROWM_REGISTER_REVOCABLE	0xC6001042
#define HKIP_ROWM_REVOKE		0xC6001043
#define HKIP_ROWM_WRITE 		0xC6001051
#define HKIP_ROWM_SET                   0xC6001058
#define HKIP_PROTECT_KERNEL_CODE        0Xc6001084
#define HKIP_PROTECT_USER_CODE		0Xc6001085
#define HKIP_HHEE_RESVMEM_RANGE		0xC60010A3

#endif
