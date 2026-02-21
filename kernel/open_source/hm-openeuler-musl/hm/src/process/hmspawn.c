/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implement hm_process_spawn
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 19 16:21:16 2021
 */
#include <string.h>
#include "hmsyscall.h"
#include "hm_internal.h"

int hm_process_spawn(pid_t *restrict res, const char *restrict path,
		     const posix_spawn_file_actions_t *fa,
		     const posix_spawnattr_t *restrict attr,
		     char *const argv[restrict], char *const envp[restrict])
{
	pid_t pid = 0;
	int rc;

	rc = -(int)__syscall(SYS_process_spawn, (uintptr_t)&pid, (uintptr_t)path, (uintptr_t)fa,
			     (uintptr_t)attr, (uintptr_t)argv, (uintptr_t)envp);
	if (rc == 0 && res != NULL) {
		(void)safe_copy(res, &pid, sizeof(pid_t));
	}

	return rc;
}
