/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Modif user mode interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 16:29:07 HKT 2020
 */
#ifndef UAPI_HMKERNEL_MODIF_API_H
#define UAPI_HMKERNEL_MODIF_API_H

extern int __syscall_fast_module_modif_call(unsigned int, ...);

static int modif_call(unsigned long cmd,
		      void *args, unsigned long args_size,
		      void *result, unsigned long result_size)
{
	return __syscall_fast_module_modif_call(cmd, args, args_size,
						result, result_size);
}

#endif
