/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Process-related resource operations of uprobe
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 7 15:50:16 2020
 */

#ifndef SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_PROCESS_H
#define SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_PROCESS_H

#include <lib/dlist.h>
#include <libhmsync/raw_rwlock.h>

struct uprobe_process_s {
	struct dlist_node uprobe_list;
	struct dlist_node uprobe_insn_buffer;
	uintptr_t trampoline;
	/*
	 * this rwlock protect all resource above, since uprobe_insn_buffer
	 * and trampoline should be accessed by uprobe inside the uprobe_list.
	 * rdlock to keep uprobe that find in uprobe list safe to use, and
	 * combine with uprobe get to prevent uprobe from going to release.
	 * wrlock to any possibly write ops to uprobe list or uprobe inside.
	 */
	struct raw_rwlock rw;
};

struct process_s;
#ifdef CONFIG_UPROBE
void uprobe_process_init(struct process_s *process);
#else
static inline void uprobe_process_init(struct process_s *process) { }
#endif

#endif
