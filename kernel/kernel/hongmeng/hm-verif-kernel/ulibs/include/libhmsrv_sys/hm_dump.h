/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: User interface of dump function
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 11 21:45:18 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HMDUMP_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HMDUMP_H

#include <unistd.h>
#include <libbunch/bunch.h>
#include <hmasm/types.h>
#include <hmsysmgr/dumpstack.h>

#define SYMBOL_SIZE 128

struct symbol_s {
	char symbol[SYMBOL_SIZE];
	pid_t pid;
};

int hm_dump_thread_to_bunch(pid_t tid, unsigned int flags, struct bunch *bunch);
int hm_dump_thread_to_klog(pid_t tid, unsigned int flags);
int hm_dump_addr2sym_by_cnode_idx(__u32 cnode_idx, unsigned long addr, struct symbol_s *symbol);

/* dump callstack of current thread */
int dump_current_thread_stack(unsigned int flags);
/* dump callstack of for a thread */
int dump_thread_stack(pid_t tid, unsigned int flags);
#endif
