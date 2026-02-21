/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Physical memory area management
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 06 11:01:15 2020
 */
#ifndef SYSMGR_MEM_PMEM_AREA_H
#define SYSMGR_MEM_PMEM_AREA_H

#include <stdlib.h>
#include <lib/dlist.h>
#include <libmem/utils.h>
#include <libalgo/rbtree.h>
#include <intapi/process/process.h>

#define PMEM_AREA_FLAG_EXTEND_CTABLE 0x1U
#define PMEM_AREA_FLAG_NO_OOM_KILL 0x2U

struct pmem_area_s {
	uint64_t pfn;
	uint64_t nrpfn;
	cref_t self;
	rref_t cnode_ref;
	struct process_s *process;
	union {
		struct dlist_node dnode;
		struct rbt_node rnode;
	};
};

/*
 * pmem_area_alloc() - alloc one pmem
 *
 * @process: the process requests the sysmgr to allocate pmem
 * @target: rref of target process whose ctable indexes the created pmem
 * @size:   size of pmem to alloc
 * Returns: A struct pmem_area_s pointer
 *          if succeed, NULL when error
 *
 * The size after align up should be 2^n PAGE_SIZE. Generally, @target and @process
 * indicate the same process. Only when the ctable of a process is created, @target
 * indicates sysmgr, and @process indicates the created process.
 */
struct pmem_area_s *pmem_area_alloc(struct process_s *process, rref_t target, uint64_t size);

/*
 * The para are the same as the pmem_area_alloc()'s.
 * The pmem_area_alloc_no_oomkill() will return null instead of doing oom kill
 * when the pmem is not enough, but the pmem_area_alloc() will do.
 */
struct pmem_area_s *pmem_area_alloc_no_oomkill(struct process_s *process, rref_t target, uint64_t size);

/*
 * pmem_area_alloc_for_ctable() - alloc one pmem for extending ctable
 *
 * @process: the process requests the sysmgr to allocate pmem
 * @target: rref of target process whose ctable indexes the created pmem
 * @size:   size of pmem to alloc
 * Returns: A struct pmem_area_s pointer
 *          if succeed, NULL when error
 *
 * The size after align up should be 2^n PAGE_SIZE. Generally, @target and @process
 * indicate the same process.
 */
struct pmem_area_s *pmem_area_alloc_for_ctable(struct process_s *process, rref_t target,
					       uint64_t size);
/*
 * pmem_area_invalidate() - invalidate one pmem
 *
 * @pmem:    pmem will be invalidated
 *
 * Make pmem_cref in the pmem_area invalid
 */
static inline void pmem_area_invalidate(struct pmem_area_s *pmem)
{
	pmem->self = 0ULL;
}
/*
 * pmem_area_free() - free one pmem
 *
 * @pmem:    pmem will be free
 *
 * Return ok when free pmem success, or return error when free failed
 */
int pmem_area_free(struct pmem_area_s *pmem);

/*
 * pmem_area_free_by_cref() - free one pmem by its cref
 *
 * @target:     rref of target process
 * @pmem_cref:  pmem will be free
 * Returns:     E_HM_OK if succeed, error code
 *              likes E_HM_ when error
 */
int pmem_area_free_by_cref(rref_t target, cref_t pmem_cref);

uint64_t hm_ucap_uapps_kobj_dump(void);

#endif /* SYSMGR_MEM_PMEM_AREA_H */
