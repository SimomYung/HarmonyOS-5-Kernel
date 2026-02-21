/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Descriptions: Definition of livepatch
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 31 15:06:09 2021
 */
#ifndef SYSMGR_INTAPI_LIVEPATCH_H
#define SYSMGR_INTAPI_LIVEPATCH_H

#include <intapi/process/process.h>

/*
 * a process who want to support livepatch should call this interface to register
 * itself and all dso.
 *
 * @param p	the process who invoked this interface, filled by sysif.
 */
int livepatch_register(struct process_s *p);

/*
 * a process who want to unregister livepatch support should call this interface.
 *
 * @param p		the process who invoked this interface, filled by sysif.
 * @param object_name	the name of the dso, if NULL, unregister the process elf
 * 			and all dso.
 */
int livepatch_unregister(struct process_s *p);
#ifdef CONFIG_LIVEPATCH
bool is_lp_stopmachine(void);
#else
static inline bool is_lp_stopmachine(void)
{
	return false;
}
#endif
#endif
