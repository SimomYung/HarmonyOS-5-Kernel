/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Sun Aug 20 17:50:23 2023
 */

#ifndef ULIBS_LIBSEC_CAPS_CRED_H
#define ULIBS_LIBSEC_CAPS_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#define base_cred_to_caps(b_cred) (&(((struct base_cred *)(b_cred))->cred_caps))

static inline
struct sec_cred_caps *caps_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
	hm_pac_shared_bcred_check(cred, SEC_PAC_BASE_CAPS, PAC_CHECK_NO_RETRY);
	return base_cred_to_caps(cred->base_cred);
}

#endif
