/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Inject fault into sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 24 18:59:03 CST 2020
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_inject_fault.h>
#include <libsysif/sysmgr/api.h>

#ifdef CONFIG_DEBUG_BUILD
int hm_inject_fault(int type, int subtype, const void *args, unsigned int length)
{
	int err;

	err = actvcall_hmcall_inject_fault(type, subtype, args, length);
	if (err < 0) {
		hm_warn("actvcapcall_hmcall_inject exception failed, err=%s\n",
			hmstrerror(err));
	}

	return err;
}
#endif
