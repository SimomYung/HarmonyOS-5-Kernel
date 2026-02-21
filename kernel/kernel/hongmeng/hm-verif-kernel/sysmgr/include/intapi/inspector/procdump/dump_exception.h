/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Dump exception information
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 18 13:20:59 2020
 */
#ifndef SYSMGR_INTAPI_INSPECTOR_PROCDUMP_DUMP_EXCEPTION_H
#define SYSMGR_INTAPI_INSPECTOR_PROCDUMP_DUMP_EXCEPTION_H

#include "procdump.h"

#include <intapi/process/service.h>
#ifdef CONFIG_OPENHARMONY
#include <intapi/process/procmgr_cred.h>
#endif
#include <hmasm/exception.h>

struct exception_info {
	struct thread_s *thread;
	struct process_s *process;
	const struct __arch_exception_info *ex_info;
};

#ifdef CONFIG_OPENHARMONY
/* user apps has uid greater than 10000 */
#define MAX_KEY_SYSTEM_PROCESS_UID 10000U
static inline bool is_key_system_process(struct process_s *process)
{
	int err = E_HM_OK;
	bool res = false;
	uid_t uid = 0;
	err = process_getuid(process, &uid);
	if (err == E_HM_OK && uid < MAX_KEY_SYSTEM_PROCESS_UID) {
		res = true;
	}
	return res;
}
#else
static inline bool is_key_system_process(struct process_s *process)
{
	UNUSED(process);
	return false;
}
#endif

#ifdef CONFIG_USER_DUMP_PROCFS

#define USER_DUMP_MAX_SIZE	2
#define USER_DUMP_MAX		1

uint32_t user_process_dump_read(void);
void user_process_dump_write(uint32_t new_dump);

static inline bool
is_dump_process(struct process_s *process)
{
	if (!process_is_system_service(process) && !is_key_system_process(process)
			&& (user_process_dump_read() == 0)) {
		return false;
	} else {
		return true;
	}
}
#else
static inline bool
is_dump_process(struct process_s *process)
{
	return true;
}
#endif
int dump_exception(unsigned int signum, const struct exception_info *info, bool emergency);
void dump_exception_basic(unsigned int signum, const struct exception_info *info);
#endif
