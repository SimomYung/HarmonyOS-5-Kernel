/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Declare tsm interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 10 11:35:32 2025
 */
#ifndef SYSMGR_INTAPI_TSM_H
#define SYSMGR_INTAPI_TSM_H

#include <stdint.h>
#include <libhmsync/raw_mutex.h>
#include <hmkernel/ctrlmem/kobjs/ulwt.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(SILK);

struct process_ulwt_tsm_kres_s {
	struct raw_mutex mutex;
	int kinfopage_sysmgr_kshmd;
	int kinfopage_u_kshmd;
	uintptr_t kinfopage_uva;
	struct __ctrlmem_kobj_ulwt_s *ukobjulwt_kinfo;
};

#endif
