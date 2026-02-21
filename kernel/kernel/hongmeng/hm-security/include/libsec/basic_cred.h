/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 15 18:48:44 2023
 */

#ifndef ULIBS_LIBSEC_BASIC_CRED_H
#define ULIBS_LIBSEC_BASIC_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#define sh_cred_to_basic(sh_cred) (&(((struct shared_cred *)(sh_cred))->cred_basic))

static inline
struct sec_cred_basic *basic_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
#ifdef ENABLE_PAC_HARD_CODE
	struct shared_cred *sh_cred = (struct shared_cred *)(cred->cred);
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// loacal
		hm_pacga_shcred_check(sh_cred, SEC_PAC_SHARED_BASIC);
	} else {
		// shared_mem
		hm_pacdb_shcred_check(sh_cred, SEC_PAC_SHARED_BASIC, b_cred->sh_idx);
	}
#endif
	return sh_cred_to_basic(cred->cred);
}

#endif
