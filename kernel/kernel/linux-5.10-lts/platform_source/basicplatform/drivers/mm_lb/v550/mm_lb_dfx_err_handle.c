/*
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/delay.h>
#include <linux/oom.h>
#include <securec.h>
#include <linux/pagewalk.h>
#include <linux/interrupt.h>
#include "mm_lb_plat.h"

#define dfx_err_info_req_phy(info) \
	((((u64)info[1] & 0x7FUL) << 29) | (info[0] >> 3))
#define dfx_err_info_req_pgid(info) ((info[1] & 0x3F000UL) >> 12)
#define dfx_err_info_req_cgid(info) ((info[1] & 0xF00UL) >> 8)
#define dfx_err_info_hit_pgid(info) ((info[4] & 0x7E000UL) >> 13)

#if defined(CONFIG_MM_LB_ENG_DEBUG) && !defined(CONFIG_LIBLINUX)
#define PTE_PHY_MASK 0xFFFFFFFFF000
struct walk_private {
	struct page *page;
	struct task_struct *task;
};

static void print_kernel_map(unsigned long addr, u64 val, struct mm_walk *walk)
{
	pid_t mm_pid = 0, cur_pid = 0;
	char *mm_comm = "kernel", *cur_comm = "kernel";
	struct task_struct *mm_owner = walk->no_vma ? walk->mm->owner : walk->vma->vm_mm->owner;
	struct task_struct *cur_owner = ((struct walk_private*)walk->private)->task;
	struct page *page = ((struct walk_private*)walk->private)->page;

	if (mm_owner) {
		mm_pid = cur_pid = mm_owner->pid;
		mm_comm = cur_comm = mm_owner->comm;
	}

	if (cur_owner) {
		cur_pid = cur_owner->pid;
		cur_comm = cur_owner->comm;
	}

	if ((val & PTE_PHY_MASK) == page_to_phys(page))
		printk(KERN_ERR "0x%-16llx 0x%-16llx %-8d %-17s %-8d %-17s\n", \
			addr, val, cur_pid, cur_comm, mm_pid, mm_comm);
	return;
}

static int pageattr_pgd_entry(pgd_t *pgd, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pgd_t val = READ_ONCE(*pgd);
	if (pgd_leaf(val))
		print_kernel_map(addr, pgd_val(val), walk);
	return 0;
}

static int pageattr_p4d_entry(p4d_t *p4d, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	p4d_t val = READ_ONCE(*p4d);
	if (p4d_leaf(val))
		print_kernel_map(addr, p4d_val(val), walk);
	return 0;
}

static int pageattr_pud_entry(pud_t *pud, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pud_t val = READ_ONCE(*pud);
	if (pud_leaf(val))
		print_kernel_map(addr, pud_val(val), walk);
	return 0;
}

static int pageattr_pmd_entry(pmd_t *pmd, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pmd_t val = READ_ONCE(*pmd);
	if (pmd_leaf(val))
		print_kernel_map(addr, pmd_val(val), walk);
	return 0;
}

static int pageattr_pte_entry(pte_t *pte, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pte_t val = READ_ONCE(*pte);
	print_kernel_map(addr, pte_val(val), walk);
	return 0;
}

static int pageattr_pte_hole(unsigned long addr, unsigned long next,
			     int depth, struct mm_walk *walk)
{
	/* Nothing to do here */
	return 0;
}

static int pageattr_test_walk(unsigned long addr, unsigned long next,
			     struct mm_walk *walk)
{
	/* Nothing to do here */
	return 0;
}

static const struct mm_walk_ops kmap_walk_ops = {
	.pgd_entry = pageattr_pgd_entry,
	.p4d_entry = pageattr_p4d_entry,
	.pud_entry = pageattr_pud_entry,
	.pmd_entry = pageattr_pmd_entry,
	.pte_entry = pageattr_pte_entry,
	.pte_hole = pageattr_pte_hole,
	.test_walk = pageattr_test_walk,
};

static void kmap_walk_mm(struct mm_struct *mm, struct walk_private *private)
{
	struct vm_area_struct *vma;

	if (!mm->mmap)
		(void)walk_page_range_novma(mm, 0, PAGE_OFFSET, &kmap_walk_ops, NULL, private);

	for (vma = mm->mmap; vma != NULL; vma = vma->vm_next)
		(void)walk_page_vma(vma, &kmap_walk_ops, private);
}

static void task_mm_walk(struct page* page)
{
	struct task_struct *p, *t;
	struct walk_private private = {
		.page = page,
	};

	mmap_read_lock(&init_mm);
	(void)walk_page_range_novma(&init_mm, PAGE_OFFSET, ~0UL, &kmap_walk_ops, NULL, &private);
	mmap_read_unlock(&init_mm);

	rcu_read_lock();
	for_each_process(p) {
		if (p->flags & PF_KTHREAD)
			continue;

		t = find_lock_task_mm(p);
		if (!t)
			continue;

		private.task = t;
		(void)kmap_walk_mm(t->mm, &private);
		task_unlock(t);
	};
	rcu_read_unlock();
}

void post_process_pgid_hit_cgid_mismatch(u32 *err_info, u32 info_num)
{
	u64 phys;
	struct page *page;

	if (info_num < DFX_INFO_NUM)
		return;

	phys = dfx_err_info_req_phy(err_info);

	if (!pfn_valid(__phys_to_pfn(phys)))
		return;

	page = phys_to_page(phys);
	printk(KERN_ERR "phys[0x%llx] page[0x%llx] req cgid[0x%x] req pgid[0x%x] req mid[0x%x] hit pgid[0x%x]\n", \
		phys, page, dfx_err_info_req_cgid(err_info),dfx_err_info_req_pgid(err_info), \
		dfx_err_info_req_mid(err_info), dfx_err_info_hit_pgid(err_info));
	printk(KERN_ERR "virt               pte                cur pid  cur task          mm pid   mm task\n");
	task_mm_walk(page);

	return;
}
#else
void post_process_pgid_hit_cgid_mismatch(u32 *err_info, u32 info_num)
{
	if (info_num < DFX_INFO_NUM)
		return;

	printk(KERN_ERR "req cgid[0x%x] req pgid[0x%x] req mid[0x%x] hit pgid[0x%x]\n", dfx_err_info_req_cgid(err_info),\
		dfx_err_info_req_pgid(err_info),dfx_err_info_req_mid(err_info), dfx_err_info_hit_pgid(err_info));
	
	return;
}
#endif

#define dfx_err_info_buff_alloc(info) info[0]
#define dfx_err_info_req_dmem0(info) (info[0] | info[1])
#define dfx_err_info_req_dmem1(info) (info[2] & 0x3FFUL)
#define dfx_err_info_req_snpattr(info) (info[2] & 0x80000UL)

void lb_dfx_dmem_err_handle(u32 *err_info, u32 info_num)
{
	if (info_num < DFX_INFO_NUM)
		return;

	printk(KERN_ERR "req dmem0[0x%x] req dmem1[0x%x]\n", dfx_err_info_req_dmem0(err_info),dfx_err_info_req_dmem1(err_info));

	return;
}

void lb_dfx_buffer_alloc_err_handle (u32 *err_info, u32 info_num)
{
	if (info_num < DFX_INFO_NUM)
		return;
	
	printk(KERN_ERR "req alloc info [0x%x]\n", dfx_err_info_buff_alloc(err_info));

	return;
}

void lb_dfx_multi_dirty_err_handle (u32 *err_info, u32 info_num)
{
	if (info_num < DFX_INFO_NUM)
		return;

	printk(KERN_ERR "req cgid[0x%x] req pgid[0x%x] req mid[0x%x] snoop attr[0x%x] phy addr[0x%x]\n", dfx_err_info_req_cgid(err_info),\
		dfx_err_info_req_pgid(err_info),dfx_err_info_req_mid(err_info), dfx_err_info_req_snpattr(err_info),dfx_err_info_req_phy(err_info));
	
	return;
}