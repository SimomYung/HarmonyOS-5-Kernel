/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 12 10:49:14 2023
 */

#ifndef ULIBS_LIBSEC_MAC_CRED_H
#define ULIBS_LIBSEC_MAC_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

/* mac cred helpers */

#define sh_cred_to_mac(sh_cred) (&(((struct shared_cred *)(sh_cred))->cred_mac))

static inline
struct sec_cred_mac *mac_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
#ifdef ENABLE_PAC_HARD_CODE
	struct shared_cred *sh_cred = (struct shared_cred *)(cred->cred);
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// loacal
		hm_pacga_shcred_check(sh_cred, SEC_PAC_SHARED_MAC);
	} else {
		// shared_mem
		hm_pacdb_shcred_check(sh_cred, SEC_PAC_SHARED_MAC, b_cred->sh_idx);
	}
#endif
	return sh_cred_to_mac(cred->cred);
}

static inline
uint64_t sid_of_sec_cred(const struct sec_chk_cred *cred)
{
	return mac_cred_of_sec_cred(cred)->sid;
}

static inline
uint64_t inner_sid_of_sec_cred(const struct sec_chk_cred *cred)
{
	return mac_cred_of_sec_cred(cred)->inner_sid;
}

#endif
