/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 26 18:59:10 2023
 */

#ifndef ULIBS_LIBSEC_KEY_CRED_H
#define ULIBS_LIBSEC_KEY_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#define base_cred_to_keyring(b_cred) (&(((struct base_cred *)(b_cred))->cred_key))

static inline
struct sec_cred_keyring *keyring_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
	hm_pac_shared_bcred_check(cred, SEC_PAC_BASE_KEYRING, PAC_CHECK_NO_RETRY);
	return base_cred_to_keyring(cred->base_cred);
}

#endif
