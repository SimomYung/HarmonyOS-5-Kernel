/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: sysevent
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 25 07:52:50 PM 2023
 */

#ifndef ULIBS_LIBSYSEVENT_SYSEVENT_H
#define ULIBS_LIBSYSEVENT_SYSEVENT_H

#include <stddef.h>
#include <stdint.h>
#include <hmkernel/types.h>
#include <lib/sysevent/sysevent_macro.h>
#include <lib/sysevent/sysevent_enum.h>

#ifdef __cplusplus
extern "C" {
#endif

int hm_sysevent_report_raw_data(const char *data, size_t data_len);
int hm_sysevent_report_raw(const char *domain, const char *evt_name,
			   enum sysevent_type evt_type, __u32 params_count, ...);

#ifdef __cplusplus
}
#endif

#endif
