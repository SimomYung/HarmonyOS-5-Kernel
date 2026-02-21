/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Permission
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 21 10:12:35 2020
 */
#ifndef MAPI_CAPABILITY_CAP_PERMISSION_H
#define MAPI_CAPABILITY_CAP_PERMISSION_H

#include <hmkernel/capability.h>

struct CNode_s;

int permission_check_cref(struct CNode_s *caller_cnode,
			  cref_t permission_cref,
			  u64 req_permission);

int permission_check_cref_nolock(struct CNode_s *caller_cnode,
			  cref_t permission_cref,
			  u64 req_permission);

#endif
