/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Kernel interface defination of modif
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 10:56:47 2020
 */
#ifndef MAPI_HMKERNEL_MODIF___KERNEL_H
#define MAPI_HMKERNEL_MODIF___KERNEL_H

/*
 * Interfaces in this files are mapi implemnted in
 * kernel/modif.c. The purpose of this is to avoid
 * promoting neccessary hmkernel api to mapi
 */
#include <hmkernel/tcb.h>
#include <hmkernel/types.h>

struct arg_type_info {
	bool is_signed;
	bool is_fixed64;
	size_t argsz;
};

int modif_sysarg(void *parg,
		 struct tcb_s *caller_tcb,
		 unsigned long argno,
		 struct arg_type_info type_info);

#endif
