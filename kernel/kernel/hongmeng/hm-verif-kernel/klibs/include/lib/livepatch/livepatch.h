/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Livepatch Elf
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun  6 11:06:22 2022
 */

#ifndef KLIB_LIVEPATCH_H
#define KLIB_LIVEPATCH_H

#include <lib/dlist.h>
#include <hmasm/types.h>
#include <hmasm/klivepatch_arch_insn.h>
#include <hmkernel/types.h>

#define FUNC_NAM_MAX_LEN 96

/* The same part of the function information */
struct livepatch_kfunc_info_s {
	/* name of the function to be patched */
	char name[FUNC_NAM_MAX_LEN];

	/* the addr and size of the function to be patched */
	__uptr_t old_addr;
	unsigned long old_size;

	/* the list of patched func chain */
	struct dlist_node patch_funcs;

	/* the func count of patched func chain */
	unsigned int patch_nr;

	/* store old instructions after patching */
	struct livepatch_old_insns_s old_insns;
};

/* struct livepatch_kfunc_s is the different part of the function info. */
struct livepatch_kfunc_s {
	/* the addr and size of the patched function */
	__uptr_t new_addr;
	unsigned long new_size;

	/* the func is patched or not */
	bool patched;

	/* pointer to the same part of the function info */
	struct livepatch_kfunc_info_s *info;

	/*
	 * link this func of enabled. if two livepatch patch the same function, only the last should
	 * in active status, thus we need a stack to keep the func chain
	 */
	struct dlist_node stack_node;
};

/*
 * used to track the livepatch module loaded into system
 */
struct livepatch_kpatch_s {
	/*
	 * the livepatch module info, should cast to 'struct module *'
	 * when we support unified format of loadable objects.
	 */
	void *module;

	/* livepatch exe code addr */
	char *layout_base;
	unsigned long layout_size;

	/* the functions in this livepatch module */
	struct livepatch_kfunc_s *funcs;
	unsigned long nr_funcs;

	/* the livepatch is enabled or not */
	bool enabled;

	/* link itself to process' patches list */
	struct dlist_node patch_node;
};

#endif
