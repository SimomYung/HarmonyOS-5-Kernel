/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: hhee hibernate save and restore
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 25 16:54:55 2025
 */
#ifndef HMKERNEL_HHEE_HIBERNATE_H
#define HMKERNEL_HHEE_HIBERNATE_H

#include <hmkernel/errno.h>
#include <hmkernel/strict.h>
#include <hmkernel/drivers/hibernate.h>

#ifdef CONFIG_HHEE_HIBERNATE_V2
int hhee_hibernate_save(void);
int hhee_hibernate_restore(void);
int hhee_hibernate_hkip_disable(void);
int hhee_hibernate_smc_trap_enable(void);
#else
static inline int hhee_hibernate_save(void)
{
	return E_HM_OPNOTSUPP;
}

static inline int hhee_hibernate_restore(void)
{
	return E_HM_OPNOTSUPP;
}

static inline int hhee_hibernate_hkip_disable(void)
{
	return E_HM_OPNOTSUPP;
}

static inline int hhee_hibernate_smc_trap_enable(void)
{
	return E_HM_OPNOTSUPP;
}
#endif

#endif