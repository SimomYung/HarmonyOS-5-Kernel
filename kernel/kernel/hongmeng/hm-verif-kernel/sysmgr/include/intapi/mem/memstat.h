/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Memstat function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 14 11:03:40 2019
 */
#ifndef SYSMGR_MEM_MEMSTAT_H
#define SYSMGR_MEM_MEMSTAT_H

#include <libmem/mstat.h>
#include <libmem/memm_slab.h>
#include <intapi/process/process.h>

#define MEM_NODE_TYPE_MAX_NUMBER	4U

struct meminfo_s {
	uint64_t mlocked;
	uint64_t commit_as;
	uint64_t pagetables;
};

void memstat_write_bunch(const struct mem_size_info *mem_size,
			 struct bunch *bunch, bool is_uvmm);
void memstat_process_vregion(struct vspace_s *vspace, struct bunch *bunch);
void memstat_slab(struct memm_slab_info *slab_info);
void memstat_process(struct process_s *process, struct mem_size_info *pmem_size_info);
void memstat_process_shortcut(struct process_s *process, struct mem_size_info *pmem_size_info);
void memstat_process_and_summary(struct process_s *process, bool need_vrstat, struct bunch *bunch);
void memstat_process_nolock(struct vspace_s *vspace, struct process_s *process,
			    struct mem_size_info *pmem_size_info);
void memstat_calculate(struct memstat_deviation *deviation);
unsigned long long memstat_cal_kernel_stack(void);
void memstat_cal_free_avail(uint64_t *free, uint64_t *avail);

struct mem_stat_s {
	uint64_t total;
	uint64_t free;
	uint64_t cache;
};

struct memstat_mem_bank_stat_s {
	uint64_t node_count[MEM_NODE_TYPE_MAX_NUMBER];
};

struct memstat_vm_stat_s {
	uint64_t total_mem;
	uint64_t anon_mem;
	uint64_t file_mem;
	uint64_t static_shm;
	uint64_t dynamic_shm;
	uint64_t reserve_mem;
};

struct memstat_phy_range_s {
	uint64_t pstart;
	uint64_t pend;
};

struct memstat_hgtlb_ator_s {
	uint64_t nr_huge_pages_total;
	uint64_t nr_huge_pages_free;
	uint64_t nr_huge_pages_rsvd;
	uint64_t nr_huge_pages_surp;
	uint64_t nr_huge_pages_overcommit;
	uint64_t huge_page_size;
};

/*
 * Obtains information about the physical memory of system, for example, total
 * and free memory size.
 *
 * @param stat      [I] Pointer to memory used to store system physical memory
 *                      information.
 *
 * @note            The memory pointed by @stat must be cleared by the caller.
 */
void memstat_summary(struct memstat_summary_stat_s *stat);
void memstat_summary_snapshot(struct memstat_summary_stat_s *stat);
void memstat_bank(struct memstat_mem_bank_stat_s *stat);
void memstat_vm(struct memstat_vm_stat_s *stat);
void memstat_phy_range_of(struct memstat_phy_range_s *range);
void memstat_huge_tlb(unsigned int order, struct memstat_hgtlb_ator_s *huge_tlb_stat);
void memstat_huge_tlb_all(struct memstat_hgtlb_all_s *stat);
/*
 * Get total size of physical memory
 *
 * @return  Size of the memory sysmgr managed
 */
uint64_t memstat_total(void);
/*
 * Get size of available physical memory
 *
 * @return  Size of the memory available
 *
 * @note: Available memory is memory space that is either free or can be freed
 */
uint64_t memstat_avail(void);
uint64_t memstat_process_gpu_size(struct process_s *process);
#endif
