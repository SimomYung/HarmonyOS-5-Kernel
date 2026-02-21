/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header of power management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 22 11:17:28 2020
 */
#ifndef MAPI_HMKERNEL_PM_SUSPEND_H
#define MAPI_HMKERNEL_PM_SUSPEND_H

#include <hmkernel/errno.h>
#include <lib/dlist.h>

#define PM_DEV_NAME_LEN 32

struct pm_dev_s {
	char name[PM_DEV_NAME_LEN];
	struct dlist_node list;

	int (*suspend)(struct pm_dev_s *pm_dev);
	void (*resume)(struct pm_dev_s *pm_dev);
#ifdef CONFIG_KERNEL_PM_HIBERNATION
	int (*freeze)(struct pm_dev_s *pm_dev);
	void (*thaw)(struct pm_dev_s *pm_dev);
	void (*restore)(struct pm_dev_s *pm_dev);
#endif
};

#ifdef CONFIG_KERNEL_PM_SUSPEND
int pm_suspend_cnt_read(void);
int pm_suspend_kernel(void);
int pm_dev_register(struct pm_dev_s *pm_dev);
int pm_cpu_dev_register(struct pm_dev_s *pm_dev);
int pm_suspend_enter(unsigned long suspend_state);
unsigned long pm_suspend_state(void);
struct dlist_node *pm_get_dev_list(void);
struct dlist_node *pm_get_cpu_dev_list(void);
#else
static inline int pm_suspend_cnt_read(void)
{
	return E_HM_NOSYS;
}

static inline int pm_suspend_kernel(void)
{
	return E_HM_NOSYS;
}

static inline int pm_dev_register(struct pm_dev_s *pm_dev)
{
	UNUSED(pm_dev);
	return 0;
}

static inline int pm_cpu_dev_register(struct pm_dev_s *pm_dev)
{
	UNUSED(pm_dev);
	return 0;
}

static inline int pm_suspend_enter(unsigned long suspend_state)
{
	UNUSED(suspend_state);
	return 0;
}

static inline unsigned long pm_suspend_state(void)
{
	return 0;
}

static inline struct dlist_node *pm_get_dev_list(void)
{
	return NULL;
}

static inline struct dlist_node *pm_get_cpu_dev_list(void)
{
	return NULL;
}
#endif

struct __hibernate_memory_bitmap;

#ifdef CONFIG_KERNEL_PM_HIBERNATION
int pm_hibernate_kernel(struct __hibernate_memory_bitmap *orig_bm, struct __hibernate_memory_bitmap *copy_bm,
			struct __hibernate_memory_bitmap *zero_bm, paddr_t *pgd,
			vaddr_t *r_enter, int *hibernating);
int pm_software_resume_kernel(paddr_t pginfo_pa, paddr_t r_pgd,
			      vaddr_t r_enter, u64 *safe_pages, unsigned int len);
int pm_get_safepage_nums(void);
#else
static inline int pm_hibernate_kernel(struct __hibernate_memory_bitmap *orig_bm,
				      struct __hibernate_memory_bitmap *copy_bm,
				      struct __hibernate_memory_bitmap *zero_bm,
				      unsigned long long *kpgd, unsigned long *r_enter, int *hibernating)
{
	UNUSED(orig_bm, copy_bm, zero_bm, kpgd, r_enter, hibernating);
	return E_HM_NOSYS;
}

static inline int pm_software_resume_kernel(paddr_t pginfo_pa, paddr_t r_pgd,
					    vaddr_t r_enter, u64 *safe_pages, unsigned int len)
{
	UNUSED(pginfo_pa, r_pgd, r_enter, safe_pages, len);
	return E_HM_NOSYS;
}

static inline int pm_get_safepage_nums(void)
{
	return E_HM_NOSYS;
}
#endif

#ifdef __HOST_LLT__
void llt_pm_dev_unregister(void);
#endif

#endif
