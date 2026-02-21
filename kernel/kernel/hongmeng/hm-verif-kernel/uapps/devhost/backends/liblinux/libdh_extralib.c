/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Init func for extra lib
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 17 02:24:01 2020
 */

#include "libdh_linux.h"
#include <devhost/log.h>
#include <hongmeng/compiler.h>

/* Gather the init funcs inside section .extralib.init.data and execute them */
int dh_extralib_init(void)
{
	int ret;
	int i;
	initcall_t *pn = NULL;
	if (unlikely(__extralib_initcall_start == __extralib_initcall_end)) {
		return 0;
	}

	dh_info("start to initialize post extra lib initcalls\n");
	for (pn = __extralib_initcall_start, i = 0;
	     pn < __extralib_initcall_end;
	     pn++, i++) {
		ret = (*pn)();
		if (ret != 0) {
			dh_error("extra lib init failed. i=%d, ret=%d\n", i, ret);
		}
	}
	dh_info("finish initializing extra lib initcalls\n");
	return 0;
}
