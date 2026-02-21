/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 13 14:45:40 2023
 */
#ifndef H_SYSMGR_INCLUDE_HMPSFMGR_H
#define H_SYSMGR_INCLUDE_HMPSFMGR_H

#include <hmasm/types.h>
#include <lib/hmpsf/hmpsf_def.h>
#include <intapi/process/process.h>
#ifdef CONFIG_HMXVM
#include <xvm/xvm_types/std/map/int_hash_map.h>
#include <xvm/xvm_types/std/map/bytes_hash_map.h>
#include <xvm/xvm_types/std/map/perfbuf.h>
#endif

struct hmpsfmgr_module {
	unsigned int		id;
	enum hmpsf_vspaceid	vspaceid;
	enum hmpsf_vm_type	vm_type;
	atomic64_t		refcnt;
#ifdef CONFIG_HMXVM
	/*
	 * maps bound to the module
	 */
	struct dlist_node	map_use_list; /* the list of map use */
#endif

	/*
	 * An opaque pointer of host-specific data.
	 *
	 * We have to copy map struct into hmpsfmgr_module before we can find out the target
	 * vspaceid, for which we need to malloc hmpsfmgr_module first. So, host specific data
	 * shall be later allocated and attached to the host_specific field here.
	 */
	void			*host_specific;
};

struct hmpsfmgr_module_kernel_specific_s {
	__paddr_t pages_pa;
	unsigned int pages_num;
	struct __ctrlmem_kobj_hmpsfmod_s *hmpsfmod_kobj;
};

/* Presents the usage of a module by a client processs */
struct hmpsfmgr_module_use_s {
	struct dlist_node	dnode;
	struct hmpsfmgr_module	*module;
	atomic64_t		refcnt;
};

#ifdef CONFIG_HMXVM
struct hmpsfmgr_map_use_s {
	struct dlist_node	dnode;
	struct hmpsfmgr_map_s	*map;
	atomic64_t		refcnt;
};

#define HMPSFMGR_MAP_HEADER \
	atomic64_t refcnt __attribute__((aligned(sizeof(uintptr_t)))); \
	unsigned long long pfn; \
	__u64 pages_pa; \
	unsigned int pages_num; \
	uintptr_t pages_client_va; \
	struct process_s *owner_process; \
	struct __ctrlmem_kobj_hmpsfmap_s *hmpsfmap_kobj; \
	struct hmpsf_vm_if_s *vm_ifs;

struct hmpsfmgr_map_s {
	HMPSFMGR_MAP_HEADER
	xvm_map_t base;
};

struct hmpsfmgr_int_hash_map_s {
	HMPSFMGR_MAP_HEADER
	xvm_int_hash_map_t map;
};

struct hmpsfmgr_bytes_hash_map_s {
	HMPSFMGR_MAP_HEADER
	xvm_bytes_hash_map_t map;
};

struct hmpsfmgr_perfbuf_s {
	HMPSFMGR_MAP_HEADER
	xvm_perfbuf_t map;
};
#endif

struct hmpsfmgr_module_use_s *hmpsf_module_use_get_by_id(struct process_s *process,
							 unsigned int id);
void hmpsf_module_use_put(struct hmpsfmgr_module_use_s *ref);
void hmpsfmgr_clean_module_use(struct process_s *process);

#ifdef CONFIG_HMXVM
struct hmpsfmgr_map_use_s *hmpsf_map_use_get_by_id(struct process_s *process, unsigned int id);
void hmpsf_map_use_put(struct hmpsfmgr_map_use_s *map_use);
void hmpsfmgr_clean_map_use(struct process_s *process);
#endif

#endif
