/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 17 20:51:10 2023
 */

#ifndef ULIBS_LIBSEC_AUDIT_CRED_H
#define ULIBS_LIBSEC_AUDIT_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#define sh_cred_to_audit(sh_cred) (&(((struct shared_cred *)(sh_cred))->cred_audit))

static inline
struct sec_cred_audit *audit_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
#ifdef ENABLE_PAC_HARD_CODE
	struct shared_cred *sh_cred = (struct shared_cred *)(cred->cred);
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// loacal
		hm_pacga_shcred_check(sh_cred, SEC_PAC_SHARED_AUDIT);
	} else {
		// sh_mem
		hm_pacdb_shcred_check(sh_cred, SEC_PAC_SHARED_AUDIT, b_cred->sh_idx);
	}
#endif
	return sh_cred_to_audit(cred->cred);
}

#define TTY_OF_SH_CRED(sh_cred) (sh_cred_to_audit(sh_cred)->tty_idx)

#endif
