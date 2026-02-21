/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: ulibs reserve memory operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 08 17:10:53 2025
 */

#include <hongmeng/errno.h>
#include <libmem/reserv_mem.h>
#include <libsysif/sysmgr/api.h>

int hm_reserved_memory_read_config(unsigned int type, __u64 *paddr, __u32 *size)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_sysctrl_reserved_memory_read_config ret;

	if (paddr == NULL || size == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		mem_zero_s(ret);
		err = actvcall_hmcall_sysctrl_reserved_memory_read_config(type, &ret);
	}
	if (err == E_HM_OK) {
		*paddr = ret.paddr;
		*size = ret.size;
	} else {
		hm_warn("read reserved memory config failed, type=%x, err=%s\n", type, hmstrerror(err));
	}
	return err;
}
