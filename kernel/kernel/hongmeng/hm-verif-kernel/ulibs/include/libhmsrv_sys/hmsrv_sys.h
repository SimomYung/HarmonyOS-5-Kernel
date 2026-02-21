/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of sysif server hmsrv sys
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 08:16:29 2019
 */

#ifndef ULIBS_INCLUDE_HMSRV_SYS_H
#define ULIBS_INCLUDE_HMSRV_SYS_H

#include <stdint.h>
#include <hongmeng/types.h>
#include <hmkernel/capability.h>

struct hmsrv_sys_version {
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
	uint16_t build;
};

void hmsrv_sys_version_get(struct hmsrv_sys_version *version);
int hmsrv_sys_version_check(const struct hmsrv_sys_version *expected);

__u32 hmsrv_sysmgr_cnode_idx(void);
void hmsrv_set_sysmgr_cnode_idx(__u32 cnode_idx);

rref_t hmsrv_module_sched_rref_of(void);
void hmsrv_update_sched_class_module_rref(void);

#endif
