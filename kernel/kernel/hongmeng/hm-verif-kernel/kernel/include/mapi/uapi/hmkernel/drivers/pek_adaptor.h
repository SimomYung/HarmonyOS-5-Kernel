/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Define pek_adaptor macro and data struct
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 17 17:52:45 2022
 */

#ifndef UAPI_DRIVERS_PEK_ADAPTOR_H
#define UAPI_DRIVERS_PEK_ADAPTOR_H

#include <hmkernel/types.h>

#ifdef __HOST_LLT__
int llt_pek_adaptor_init(void);
int llt_pek_adaptor_handler(struct tcb_s *caller, unsigned long cmd, unsigned long arg0,
			    unsigned long arg1, unsigned long arg2, unsigned long arg3);
#endif

#endif
