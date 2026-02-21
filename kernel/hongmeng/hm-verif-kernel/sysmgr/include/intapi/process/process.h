/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition for process
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 14 04:43:56 2018 -0400
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROCESS_H
#define SYSMGR_INTAPI_PROCESS_PROCESS_H

#include <intapi/process/proc_core.h>
#include <intapi/process/proc_info.h>
#include <intapi/process/procgroup.h>
#include <intapi/process/proc_scopedptr.h>

struct file;
struct execve_info {
	struct hm_execve_req *exec_req;
	struct vspace_s *src;
	struct vspace_s *dst;
	char *script_str;
	unsigned int script_strcnt;
	size_t script_len;
	size_t src_pointer_sz;
	size_t dst_pointer_sz;
	bool replace_argv0;
	bool need_install_execfd;
	int execfd;
	 /*
	  * different from the file of filename,
	  * it is the file pass to the interpreter,
	  * only used for need_install_execfd case.
	  */
	struct file *binary;
	uint32_t cnode_idx;
};

struct spawn_info {
	struct hm_spawn_req *spawn_req;
	struct execve_info exec_info;
};

#endif
