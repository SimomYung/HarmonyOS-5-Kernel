/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Credential define header file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 20 11:28:21 2023
 */

#ifndef ULIBS_LIBSEC_NS_CRED_H
#define ULIBS_LIBSEC_NS_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#include <libsec/chk_cred.h>

#define base_cred_to_usr_ns_cred(b_cred) (sec_cred_idx_to_ptr(((struct base_cred *)(b_cred))->usr_ns_idx))

static inline
struct usr_ns_cred *usr_ns_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
	return base_cred_to_usr_ns_cred(cred->base_cred);
}

#ifdef CONFIG_NET_NS

#define base_cred_to_nsid_group(b_cred) (&(((struct base_cred *)(b_cred))->nsid_group))

static inline
struct nsid_group *nsid_group_of_sec_cred(const struct sec_chk_cred *cred)
{
#ifdef ENABLE_PAC_HARD_CODE
	struct base_cred *b_cred = (struct base_cred *)(cred->base_cred);
	if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
		// loacal
		hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_NSID_GROUP);
	} else {
		// shared_mem
		hm_pacdb_bcred_check(b_cred, SEC_PAC_BASE_NSID_GROUP, cred->cred_idx);
	}
#endif
	return base_cred_to_nsid_group(cred->base_cred);
}

static inline uint32_t netns_id_of_sec_cred(const struct sec_chk_cred *cred)
{
	return nsid_group_of_sec_cred(cred)->netns_id;
}
#else
static inline uint32_t netns_id_of_sec_cred(const struct sec_chk_cred *cred)
{
	UNUSED(cred);
	return 0;
}
#endif

#endif
