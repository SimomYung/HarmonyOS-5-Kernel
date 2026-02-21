/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Interfaces of kbox in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 28 20:36:33 2022
 */

#ifndef SYSYMGR_INSPECTOR_KBOX_H
#define SYSYMGR_INSPECTOR_KBOX_H

#ifdef CONFIG_KBOX
int kbox_sec_chk_paddr(uint64_t paddr, uint64_t paddr_end);
#else
static inline int kbox_sec_chk_paddr(uint64_t paddr, uint64_t paddr_end)
{
	UNUSED(paddr, paddr_end);
	return E_HM_ACCES;
}
#endif

#endif
