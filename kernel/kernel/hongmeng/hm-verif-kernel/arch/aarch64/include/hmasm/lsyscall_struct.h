/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: definition of lsyscall private args
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 30 16:22:21 2023
 */

#ifndef ASM_LSYSCALL_STRUCT_H
#define ASM_LSYSCALL_STRUCT_H

#include <hmkernel/compiler.h>
#include <hmkernel/fdtable/struct.h>
#include <hmkernel/restart_block.h>

#define ARCH_LSYSCALL_ARGS_SIZE_MAX 64

union lsyscall_args {
	struct {
		u64 fd;
		struct fd_s fde;
	} common;

	struct {
		void *epoll_file;
		void *common_file;
	} epoll_ctl_hook_args;

	struct {
		u64 demo_arg;
	} demo_hook_args;

	/*
	 * If file_close upcall info stores in rlambda_extra, it will be
	 * occurs error in the following scenario.
	 *        uapp        fs       kernel            kernel
	 * dup2:lsyscall-->capcall-->upcall set-->...-->retry--->panic
	 *                                          |
	 *                                          V
	 *                                       vsfault
	 *                           (overwrites original file_close value)
	 * so we move file_close info into lsyscall_info_args. Since server
	 * initiate capcall, independent of other lsyscall args.
	 */
	struct {
		unsigned int fd;
		unsigned int cnode_idx;
		unsigned int server;
		unsigned int rpccnt;
	} file_close;

	/*
	 * Use union to save the restart block parameter. We can ensure that no
	 * other lsyscall is invoked when the handle_hmsignals initiates
	 * __NR_restart_syscall.
	 * If the corresponding handler function is registered for the signal,
	 * handle_hmsignals ensures that syscall will not be retried through
	 * __NR_restart_syscall. So we can use this union to store
	 * restart_block. In addition, actv_lsyscall_arg_type_of ensures that no
	 * invalid pointer is accessed in sys_restart_syscall.
	 */
	struct restart_block restart_block;
};

static_assert(sizeof(union lsyscall_args) <= ARCH_LSYSCALL_ARGS_SIZE_MAX,
	      sizeof_of_lsyscall_args_too_large);
#endif
