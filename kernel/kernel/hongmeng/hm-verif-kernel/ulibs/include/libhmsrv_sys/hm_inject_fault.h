/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Inject fault into sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 24 18:59:03 CST 2020
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_HM_INJECT_FAULT_H
#define ULIBS_INCLUDE_LIBHMSRV_HM_INJECT_FAULT_H

int hm_inject_fault(int type, int subtype, const void *args, unsigned int length);

#endif
