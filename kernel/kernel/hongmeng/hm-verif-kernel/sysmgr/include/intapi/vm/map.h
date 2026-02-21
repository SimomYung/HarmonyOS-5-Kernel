/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Internel API for vm memory management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 02 19:27:52 2021
 */

#ifndef SYSMGR_INTAPI_VM_MAP_H
#define SYSMGR_INTAPI_VM_MAP_H

#include <intapi/mem/vspace.h>
#include <sec/ctx.h>

struct gpa_list_s;

struct vm_map_params_s {
	struct vspace_s *vm_vspace;
	uint32_t fd_type;
	struct gpa_list_s *gpa_list;
	uint32_t prot;
	uint32_t flags;
	unsigned long fd_file;
	const char *fs_service;
	uint64_t offset;
};

struct vm_map_shm_params_s {
	struct vspace_s *vm_vspace;
	struct gpa_list_s *gpa_lists;
	uint32_t nr_gpa_lists;
	uint32_t prot;
};

int mmap_gpa_list(struct vm_map_params_s *params);
int munmap_gpa_list(struct vspace_s *vm_vspace, struct gpa_list_s *gpa_list);

struct static_shm_sec_info_s {
	struct sec_natv_obj_ctx sec_ctx;
};

#endif
