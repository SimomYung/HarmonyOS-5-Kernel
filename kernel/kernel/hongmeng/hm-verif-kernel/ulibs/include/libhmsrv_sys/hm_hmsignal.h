/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Interfaces for hmsignals
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 21 16:32:46 2018
 */
#ifndef ULIBS_INCLUDE_LIBVSYSCALL_HM_HMSIGNAL_H
#define ULIBS_INCLUDE_LIBVSYSCALL_HM_HMSIGNAL_H

#include <hmkernel/capability.h>
#include <hmkernel/ipc/hmsignal.h>

#define HMSIGMASK_NONE		(0ULL)

/* NonMaskableSignals */
#define HMSIGNAL_NONMASKABLE ((unsigned long long)1u << (unsigned long long)(HMSIGNAL_KILL))
#define HMSIGNAL_ACTV_MASK_DEFAULT (~HMSIGNAL_DEFAULT_NONMASK)

#define hmsignal_mask(signum)	(1ULL << (signum))

#define DEFINE_HMSIGMASK_NONE(x) struct __hmsigmask x = {0}
#define DEFINE_HMSIGMASK_ACTV_DEFAULT(x) struct __hmsigmask x = {HMSIGNAL_ACTV_MASK_DEFAULT}

int hm_actv_set_hmsignal_mask(cref_t actv_cref, const struct __hmsigmask *uin,
			      struct __hmsigmask *uout);

int hm_thread_set_hmsignal_mask(cref_t tcb_cref, const struct __hmsigmask *uin,
				struct __hmsigmask *uout);

#endif /* ULIBS_INCLUDE_LIBVSYSCALL_HM_HMSIGNAL_H */
