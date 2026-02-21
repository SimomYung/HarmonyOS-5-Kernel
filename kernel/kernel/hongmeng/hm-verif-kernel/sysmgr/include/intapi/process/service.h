/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Process service judge interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 14 20:30:46 2019
 */
#ifndef SYSMGR_INTAPI_PROCESS_SERVICE_H
#define SYSMGR_INTAPI_PROCESS_SERVICE_H
#include <intapi/process/process.h>
#include <intapi/process/pid.h>
#include <procmgr/procmgr_core.h>

#define VFS_SERVICE		"vfs.elf"
#define VFS_BIN			"/bin/vfs.elf"

static inline bool process_is_sysmgr(const struct process_s *process)
{
	return process == process_of_sysmgr();
}

static inline bool process_is_hm_test(const struct process_s *process)
{
	BUG_ON(process_name_of(process) == NULL);
	return strncmp(process_name_of(process), "/usr/bin/hm-test.elf", sizeof("/usr/bin/hm-test.elf")) == 0;
}

static inline bool process_is_sec_test(const struct process_s *process)
{
	BUG_ON(process_name_of(process) == NULL);
	return strncmp(process_name_of(process), "/usr/bin/sec-test.elf", sizeof("/usr/bin/sec-test.elf")) == 0;
}

static inline bool process_is_hm_test_dynamic(const struct process_s *process)
{
	BUG_ON(process_name_of(process) == NULL);
	return strncmp(process_name_of(process), "/usr/bin/hm-test-dynamic.elf", sizeof("/usr/bin/hm-test-dynamic.elf")) == 0;
}

static inline bool process_is_llt(const struct process_s *process)
{
	BUG_ON(process_name_of(process) == NULL);
	return strncmp(process_name_of(process), "llt_process", sizeof("llt_process")) == 0;
}

static inline bool process_is_proc_affinity_test(const struct process_s *process)
{
	BUG_ON(process_name_of(process) == NULL);
	return strncmp(process_name_of(process), "/usr/bin/hm-spawn-test.elf", sizeof("/usr/bin/hm-spawn-test.elf")) == 0;
}
bool process_is_system_service(const struct process_s *process);

#ifdef CONFIG_HKIP_PROTECT_EL0
bool process_support_hkip(const struct process_s *process);
#endif

bool process_is_devhost(const struct process_s *process);

static inline bool is_process_privileged_affinity(const struct process_s *process)
{
	hm_debug("service %s, is_process_privileged_affinity is %d\n", process_name_of(process), process->is_privileged_affinity);
	return process->is_privileged_affinity;
}

static inline bool process_is_native_process(struct process_s *process)
{
	return process_loader_of(process)->native_apps;
}

bool process_is_init(const struct process_s *process);
void launch_init(void);
bool process_is_vfs(const struct process_s *process);
void launch_vfs(void);
#endif
