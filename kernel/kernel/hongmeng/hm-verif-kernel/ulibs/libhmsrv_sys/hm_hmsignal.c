/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementations of some interfaces about hmsignals
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 2 16:32:46 2019
 */
#include <sys/time.h>
#include <stdarg.h>
#include <errno.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_hmsignal.h>
#include <hmkernel/ipc/hmsignal.h>

int hm_actv_set_hmsignal_mask(cref_t actv_cref, const struct __hmsigmask *uin,
			      struct __hmsigmask *uout)
{
	int err;
	struct __hmsigmask in = {0};

	if (uin != NULL) {
		in.__mask = (uin->__mask) & (~HMSIGNAL_NONMASKABLE);
	}

	err = syscap_ActivationHMSignalMask(actv_cref, uin == NULL ? NULL : &in, uout);
	return err;
}

int hm_thread_set_hmsignal_mask(cref_t tcb_cref, const struct __hmsigmask *uin,
				struct __hmsigmask *uout)
{
	int err = E_HM_OK;
	struct __hmsigmask in = {0};

	/* NOTE:
	 * 1. Some hmsignals cannot be masked and
	 * 2. Change hmsignal mask should has the permission
	 * 3. Changing other's thread.hmsignalmask is complex:
	 *    needs to consider whether an original masked signal get
	 *    unmasked, in this case, tcb_hmsignal() like operation
	 *    needs to be done. To reduce this complexity, only allows
	 *    tcb itself calls HMSignalMask. In this case, the tcb
	 *    must in RUNNING state.
	 */
	if (tcb_cref != 0U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (uin != NULL) {
			in.__mask = (uin->__mask) & (~HMSIGNAL_NONMASKABLE);
		}

		err = sysfast_thread_hmsignal_mask(uin == NULL ? NULL : &in, uout);
	}
	return err;
}
