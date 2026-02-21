/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Pal for capabilities
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 13 15:25:30 2021
 */

#include <liblinux/pal.h>

#include <stdbool.h>
#include <libsec/cred.h>
#include <libsec/chk_cred.h>
#include <libsec/posix_cap.h>

#include <devhost/log.h>

#define IN_GPROUPS 1
#define NOT_IN_GPROUPS 0

static int __liblinux_pal_group_check(unsigned int gid,
				      struct sec_cred_dac *cred_dac,
				      unsigned int *proc_gid)
{
	struct sec_chk_cred cred = {0};
	int ret;

	ret = sec_chk_cred_init_requester(&cred);
	if (ret != 0) {
		dh_debug("gid %u failed get cred\n", gid);
		return NOT_IN_GPROUPS;
	}

	*cred_dac = *dac_cred_of_sec_cred(&cred);
	ret = sec_cred_check_dac_groups(&cred, gid, *proc_gid);
	if (ret != 0) {
		dh_debug("gid %u NO access\n", gid);
		return NOT_IN_GPROUPS;
	}

	return IN_GPROUPS;
}

int liblinux_pal_in_group_p(unsigned int gid)
{
	struct sec_cred_dac cred_dac = {0};
	return __liblinux_pal_group_check(gid, &cred_dac, &cred_dac.fsgid);
}

int liblinux_pal_in_egroup_p(unsigned int gid)
{
	struct sec_cred_dac cred_dac = {0};
	return __liblinux_pal_group_check(gid, &cred_dac, &cred_dac.egid);
}

int liblinux_pal_capable(int cap, unsigned int opts)
{
	struct sec_chk_cred cred = {0};
	__u64 idx = 0;
	int ret;

	ret = sysfast_miscdata_read(&idx, __MISCDATA_TCB_CRED, __MISCACCESS_SELF);
	if (ret != E_HM_OK) {
		dh_error("failed to read tcb cred misc data err = %s\n", hmstrerror(ret));
		return (int)false;
	}

	struct base_cred *b_cred = (struct base_cred *)sec_cred_idx_to_ptr(idx);
	if (b_cred == NULL) {
		dh_error("failed to get base cred pointer\n");
		return (int)false;
	}

	cred.cred_idx = idx;
	cred.base_cred = b_cred;
	if ((opts & LIBLINUX_CAP_OPT_NOAUDIT) == 0) {
		ret = (int)sec_capable(&cred, (unsigned int)cap);
	} else {
		ret = (int)sec_capable_noaudit(&cred, (unsigned int)cap);
	}

	return ret;
}
