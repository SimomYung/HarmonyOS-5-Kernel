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
#ifdef PAC_DFI_CHECK_CRED_SEHARMANY_WITH_LOCK
	hm_pac_shared_bcred_check_with_lock(cred, SEC_PAC_BASE_SEHARMONY);
#else
	hm_pac_shared_bcred_check(cred, SEC_PAC_BASE_SEHARMONY, PAC_CHECK_NO_RETRY);
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
