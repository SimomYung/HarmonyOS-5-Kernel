/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 12 10:49:14 2023
 */

#ifndef ULIBS_LIBSEC_DAC_CRED_H
#define ULIBS_LIBSEC_DAC_CRED_H

#ifndef ULIBS_LIBSEC_CRED_H
#error you should include cred.h instead
#endif

#define base_cred_to_dac(b_cred) (&(((struct base_cred *)(b_cred))->cred_dac))

int sec_cred_groups_search(const gid_t *group_list, uint32_t size, gid_t gid);

static inline
struct sec_cred_dac *dac_cred_of_sec_cred(const struct sec_chk_cred *cred)
{
	return base_cred_to_dac(cred->base_cred);
}

#define sh_cred_to_umask(sh_cred) (((struct shared_cred *)(sh_cred))->umask)

static inline
mode_t umask_of_sec_cred(const struct sec_chk_cred *cred)
{
	return sh_cred_to_umask(cred->cred);
}

static inline int gid_in_group(const struct sec_cred_dac *dac_cred, gid_t gid)
{
	int err = E_HM_OK;
	gid_t *groups = NULL;

	if (dac_cred->groups.size == 0u) {
		err = E_HM_POSIX_NOENT;
	} else {
		groups = sec_cred_idx_to_ptr(dac_cred->groups.idx);
		if (groups == NULL)
			return E_HM_INVAL;
		err = sec_cred_groups_search(groups, dac_cred->groups.size, gid);
	}

	return err;
}

static inline int in_group(const struct sec_cred_dac *dac_cred, gid_t gid, gid_t p_gid)
{
	int err = E_HM_OK;
	if (p_gid != gid) {
		err = gid_in_group(dac_cred, gid);
	}
	return err;
}

#endif
