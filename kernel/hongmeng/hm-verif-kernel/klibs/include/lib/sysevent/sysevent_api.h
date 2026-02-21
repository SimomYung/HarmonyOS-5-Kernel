/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */
#ifndef KLIBS_SYSEVENT_API_H
#define KLIBS_SYSEVENT_API_H

#include <stddef.h>
#include <hmkernel/types.h>
#include <hmkernel/macro.h>
#include <hmkernel/errno.h>
#include "sysevent_enum.h"

#ifdef __KERNEL__
#ifdef CONFIG_SYSEVENT

int hm_sysevent_report_raw(const char *domain, const char *evt_name,
			   enum sysevent_type evt_type, __u32 params_count, ...);

#else /* !CONFIG_SYSEVENT */

static inline int hm_sysevent_report_raw(const char *domain, const char *evt_name,
					 enum sysevent_type evt_type, __u32 params_count, ...)
{
	UNUSED(domain, evt_name, evt_type, params_count);
	return E_HM_NOSYS;
}

#endif /* CONFIG_SYSEVENT */

#include "sysevent_macro.h"

#endif /* __KERNEL__ */

#endif
