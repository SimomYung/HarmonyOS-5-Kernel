/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: dfx to catch unexpected process death
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 29 15:25:50 2023
 */

#ifndef SYSMGR_INSPECTOR_DIE_CATCH_H
#define SYSMGR_INSPECTOR_DIE_CATCH_H

#include <vsync/atomic.h>
#include <stdbool.h>
#include <libstrict/strict.h>

struct process_s;
struct die_catch_s {
	vatomic16_t catch_flags;
	bool die_catch_handled;
	pid_t killer_pid;
	unsigned int killer_uid;
};

#ifdef CONFIG_DFX_DIE_CATCH
void process_die_catch_init(struct process_s *process);

int procfs_pid_unexpected_die_catch_read(struct process_s *process, char *data, size_t data_len);
int procfs_pid_unexpected_die_catch_write(struct process_s *process, uint64_t pos,
					  const char *src, size_t size, size_t *wsize);

bool try_catch_unexpected_die(struct process_s *process, int exit_status);

bool process_die_catch_restrict_dump_abort(const struct process_s *process);
#else /* !CONFIG_DFX_DIE_CATCH */
static inline void process_die_catch_init(struct process_s *process)
{
	UNUSED(process);
}

static inline bool try_catch_unexpected_die(struct process_s *process, int exit_status)
{
	UNUSED(process, exit_status);
	return false;
}

static inline bool process_die_catch_restrict_dump_abort(const struct process_s *process)
{
	UNUSED(process);
	return false;
}
#endif /* CONFIG_DFX_DIE_CATCH */

#endif
