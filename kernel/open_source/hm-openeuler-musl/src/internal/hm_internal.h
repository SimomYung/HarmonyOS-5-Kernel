/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Desctiption: Hognmeng internal api
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 20 11:25:47 2021
 */
#ifndef _HM_INTERNAL_H
#define _HM_INTERNAL_H

#include <unistd.h>
#include <spawn.h>
#include <signal.h>
#include "pthread_impl.h"

#define HM_INTERNAL_API  __attribute__((__visibility__("hidden")))
#define HM_NATIVE_API  __attribute__((__visibility__("default")))

HM_INTERNAL_API int hm_process_spawn(pid_t *restrict res, const char *restrict path,
				     const posix_spawn_file_actions_t *fa,
				     const posix_spawnattr_t *restrict attr,
				     char *const argv[restrict], char *const envp[restrict]);

#endif
