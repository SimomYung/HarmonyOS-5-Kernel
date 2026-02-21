/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: fdtbale cap ops
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 26 09:58:28 2023
 */

#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>

#include <lib/errno.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <hmkernel/types.h>
#include <libhmucap/ucap.h>
#include <hongmeng/panic.h>
#include <hongmeng/syscall.h>
#include <libfdtable/fdtable.h>
#include <libsysif/sysmgr/api.h>
#include <hmasm/lsyscall.h>

int hm_fdtable_require_grant(unsigned int cnode_idx, rref_t *result)
{
	int err = E_HM_OK;
	rref_t fdtable_rref;

	fdtable_rref = hm_ucap_require(0, hmsrv_sysmgr_cnode_idx(), 0UL);
	if (!IS_REF_ERR(fdtable_rref)) {
		/* call sysmgr for grant */
		err = actvcall_hmcall_proc_acquire_fdtable_rref(cnode_idx, fdtable_rref);
		if (err < 0) {
			hm_warn("acquire fdtable rref failed, cnode_idx=0x%x, err=%d,%s\n",
				cnode_idx, err, hmstrerror(err));
			/* impossible to failed */
			int ret = hm_ucap_reject(0, fdtable_rref);
			BUG_ON(ret < 0);
		}
	} else {
		err = REF_TO_ERR(fdtable_rref);
		hm_warn("ucap require failed, cnode_idx=0x%x, err=%d,%s\n",
			cnode_idx, err, hmstrerror(err));
	}

	if (err == E_HM_OK) {
		*result = fdtable_rref;
	}

	return err;
}
