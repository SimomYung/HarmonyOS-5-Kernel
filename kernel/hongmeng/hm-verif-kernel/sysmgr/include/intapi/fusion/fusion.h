/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of system services fusion
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 9 20:19:21 2023
 */
#ifndef SYSMGR_INTAPI_FUSION_FUSION_H
#define SYSMGR_INTAPI_FUSION_FUSION_H

struct raw_work;

#ifdef CONFIG_UDK_FUSION
extern int udk_fusion_entry(int argc, char **argv);
#else
static inline int udk_fusion_entry(int argc, char **argv)
{
	UNUSED(argc, argv);
	return 0;
}
#endif

uint64_t fusion_sys_va_to_pa(uintptr_t vaddr);
uintptr_t fusion_pa_to_sys_va(uint64_t paddr);
void fusion_flush_cache_range_by_pa(unsigned long pstart, unsigned long pend,
				    unsigned int cmd);

#ifdef CONFIG_VFS_FUSION
struct hm_execve_req;
int fusion_spawn(const struct hm_execve_req *req);
int fusion_rmap_clean_dirty_page(uintptr_t vaddr);
extern int flush_dcache_range(uint64_t start, uint64_t end);
extern int clean_dcache_range(uint64_t start, uint64_t end);
extern int inval_dcache_range(uint64_t start, uint64_t end);

int fusion_sysmgr_workqueue_add_work(struct raw_work *work);
bool fusion_thread_is_graphic_high(void);
#else
static inline int fusion_rmap_clean_dirty_page(uintptr_t vaddr)
{
	UNUSED(vaddr);
	return E_HM_NOSYS;
}
static inline int fusion_spawn(const struct hm_execve_req *req)
{
	UNUSED(req);
	return E_HM_NOSYS;
}

static inline int fusion_sysmgr_workqueue_add_work(struct raw_work *work)
{
	UNUSED(work);
	return E_HM_NOSYS;
}

static inline bool fusion_thread_is_graphic_high(void)
{
	return false;
}
#endif /* CONFIG_VFS_FUSION */
#endif /* SYSMGR_INTAPI_FUSION_FUSION_H */
