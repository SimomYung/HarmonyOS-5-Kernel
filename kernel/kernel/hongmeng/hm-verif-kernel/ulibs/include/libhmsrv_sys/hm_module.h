/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declare cap_modlue interface for user
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 11:45:32 2019
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MODULE_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MODULE_H

#include <stdint.h>
#include <stdlib.h>
#include <hmkernel/capability.h>

rref_t hm_sched_module_rref_of(void);
rref_t hm_module_open(const char *module_name);
int hm_module_close(rref_t module_rref);
int hm_module_ioctl(rref_t module_rref, uint32_t cmd,
		    void *cfg, size_t cfg_len,
		    void *rlt, size_t rlt_len);
int hm_module_bind_modif(rref_t module_rref);
int hm_module_unbind_modif(rref_t module_rref);

#endif
