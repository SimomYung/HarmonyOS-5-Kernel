/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 12 10:49:14 2023
 */
#ifndef ULIBS_LIBSEC_SEHARMONY_CRED_H
#define ULIBS_LIBSEC_SEHARMONY_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

/* seharmony cred helpers */
#define current_seharmony_cred() (&((struct base_cred *)(current_base_cred()))->cred_sel)

#define base_cred_to_sel(b_cred) (&(((struct base_cred *)(b_cred))->cred_sel))

static inline
struct sec_cred_seharmony *sel_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
#ifdef ENABLE_PAC_HARD_CODE
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// loacal
		hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_SEHARMONY);
	} else {
		// shared_mem
		hm_pacdb_bcred_check(b_cred, SEC_PAC_BASE_SEHARMONY, cred->cred_idx);
	}
#endif
	return base_cred_to_sel(cred->base_cred);
}

static inline
hmsel_sid_t sel_sid_of_sec_cred(const struct sec_chk_cred *cred)
{
	return sel_cred_of_sec_cred(cred)->sid;
}

static inline
hmsel_sid_t current_sel_sid(void)
{
	return current_seharmony_cred()->sid;
}

#endif
