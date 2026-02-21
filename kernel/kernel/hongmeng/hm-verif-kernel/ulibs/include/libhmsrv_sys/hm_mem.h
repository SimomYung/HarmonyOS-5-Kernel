/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Interface for memory management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 27 06:26:16 2019
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MEM_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MEM_H

#include <stdint.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <libbunch/bunch.h>
#include <libmem/types.h>
#include <libmem/flags.h>
#include <libsysif/sysmgr/api.h>
#include <hmkernel/capability.h>
#include "hm_stat.h"

/*
 * struct hm_mem_fnode_s - Unique identification file
 * @index: inode index in fs
 * @dev_id: device id
 */
struct hm_mem_fnode_s {
	uint32_t index;
	uint64_t dev_id;
};

enum __hmmem_tests_type {
	__TEST_LINEAR_RO_SYS_PART,
	__TEST_LINEAR_RO_KER_PART,
};

/*
 * This interface is designed for fixed map, the `flag` parameter
 * will be marked with MAP_FIXED by default. It also provides an
 * extra return parameter `unmapped`. According to the POSIX spec,
 * MAP_FIXED will remove old mappings then build new mapping,
 * which would lead to memory problems in concurrent environment.
 * Thus the `unmapped` is used to indicate whether the original
 * virtual address has been unmapped or not when mmap failed.
 * Note if mmap succeeds, `unmapped` will not be modified.
 *
 * There are several reasons for adding this new interface.
 * 1.Both the hm_mem_mmap and hm_mem_mmap_posix interfaces do not
 * provide the functionality to show if the original address has
 * been unmapped.
 * 2.Whether the address has been unmapped is a behavior rather
 * than an error, thus can not be represented by a single error code.
 */
void *hm_mem_mmap_fixed(void *addr, size_t len,
			uint32_t prot, uint32_t flags,
			int32_t fd, int64_t offset,
			bool *unmapped, int *ret_err);
/* NOTE: this function is only be used in libvsyscall */
void *hm_mem_mmap_posix(void *addr, size_t len,
			uint32_t prot, uint32_t flags,
			int32_t fd, int64_t offset, int *ret_err);
void *hm_server_mem_mmap_posix(rref_t server,
			       const void *addr, size_t len,
			       uint32_t prot, uint32_t flags,
			       int32_t fd, int64_t offset, int *ret_err);
int hm_server_mem_munmap(rref_t server, const void *addr, size_t len);

void *hm_mem_mmap(void *addr, size_t len,
		  uint32_t prot, uint32_t flags,
		  int32_t fd, int64_t offset);

void *hm_mem_mmap_ex(void *addr, size_t len,
		  uint32_t prot, uint32_t flags,
		  int32_t fd, int64_t offset, size_t guard_size);

long hm_mem_mremap(void *old_addr, size_t old_len, size_t new_len,
		    uint32_t flags, void *new_addr);

int hm_mem_munmap(void *addr, size_t len);
int hm_mem_rmap_unmap_page(const void *addr, uint32_t *flags);
void *hm_mem_alloc_range_restricted(size_t length, unsigned long low_limit,
				    unsigned long high_limit, uint32_t flags,
				    unsigned long align_mask,
				    unsigned long align_offset);
/* NOTE: this function is only be used in libvsyscall */
long hm_mem_brk(const void *brk);

int hm_mem_madvise(const void *addr, size_t len, uint32_t flags);

int hm_mem_mprotect(const void *addr, size_t len, uint32_t prot);

int hm_mem_amend_prot(const void *addr, size_t len, uint32_t set_prot,
			  uint32_t clear_prot);

long hm_mem_msync(const void *addr, size_t len, uint32_t flags);

long hm_mem_mlock(const void *addr, size_t len);

long hm_mem_mlock2(const void *addr, size_t len, uint32_t flags);

long hm_mem_munlock(const void *addr, size_t len);

long hm_mem_mlockall(uint32_t flags);

long hm_mem_munlockall(void);

cref_t hm_mem_alloc_pmem(size_t size);

int hm_mem_free_pmem(cref_t cref);

int hm_mem_shm_open(const char *name, size_t len, uint64_t oflags,
		    size_t *ret_len);

int hm_mem_shm_open_with_mode(const char *name, size_t len, uint64_t oflags,
			      mode_t mode, size_t *ret_len);

int hm_mem_shm_unlink(const char *name);

int hm_mem_shm_close(int32_t shm_id);

int hm_mem_shm_reclaim_pages(int32_t shm_id, const void *start_addr,
			     unsigned long nr_pages);

int hm_mem_query_shm_info(int32_t shm_id,
			  struct shm_node_info *shm_info);
int hm_mem_query_shm_paddr(int32_t shm_id,
			   unsigned long long *dst,
			   unsigned long size);

int hm_mem_shm_dump_info(void);

int hm_mem_stat(unsigned int flags, struct bunch *bunch);

int hm_server_mem_stat(rref_t server, unsigned int flags, struct bunch *bunch);

int hm_mem_set_anon_name(const void *start, size_t len, const char *name);

long hm_mem_mincore(const void *addr, size_t length, const unsigned char *vec);
int hm_mem_calculate(struct memstat_deviation *div);
int hm_mem_fs_stat_register(const void *stat_addr);
int hm_mem_hugetlbfs_mount(const struct hgtlb_mount_args_s *args, unsigned int *order);
int hm_mem_file_mapping_stat(struct hm_mem_fnode_s fnode,
			     struct file_map_info_s *map_info);

int hm_mem_shm_chmod(const char* name, mode_t mode);

int hm_mem_cma_dyn_init(size_t size, unsigned int *cma_id);
int hm_mem_cma_alloc(unsigned int cma_id, size_t size,
		     unsigned int align_order, unsigned long long *paddr);
int hm_mem_cma_free(unsigned int cma_id, unsigned long long paddr, size_t size);
int hm_mem_cma_raw_map(uint64_t *vaddr, unsigned long long paddr,
		       unsigned long len, unsigned long prot, unsigned int flags);
int hm_mem_cma_setup(uint64_t *vaddr, unsigned long len);

size_t hm_mem_drop_file_cache(void);
int hm_mem_enable_rsv_mem(uint64_t pfn_start, uint64_t pfn_end);
int hm_mem_devhost_stat_register(const void *stat_addr, size_t stat_size);
int hm_mem_extfrag_index_read(void *buf, size_t len);
int hm_mem_compact(void);

int hm_mem_cpa_prepare_memory(unsigned int flags, unsigned long water_mark_page,
			      unsigned long size, unsigned long heap_size,
			      unsigned long allocated_size);

void *hm_mem_mmap_lsyscall(void *addr, size_t len,
			   uint32_t prot, uint32_t flags,
			   int32_t fd, int64_t offset);
int hm_mem_munmap_lsyscall(void *addr, size_t len);
int hm_mem_dump_pte_with_pgd(pid_t pid, uint64_t pgd, uintptr_t va);
#endif /* ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MEM_H */
