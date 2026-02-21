/*
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
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

#include <platform_include/basicplatform/linux/rdr_platform.h>
#ifdef CONFIG_MM_AMA
#include <platform_include/basicplatform/linux/mem_ama.h>
#endif
#include <linux/mm_lb/mm_lb.h>
#include <linux/ion/mm_ion.h>

#include "mm_lb_drv_v550.h"
#include "mm_lb_plat_v550.h"

u32 lb_get_available_size(void)
{
	if (!lbdev)
		return 0;

	if (!is_lb_available())
		return 0;

	return lbdev->max_size;
}

/*
 * this API is called by HIFI, Tiny, GPU
 * size is not used
 */
int lb_request_quota(u32 pid)
{
	return 0;
}

int lb_release_quota(u32 pid)
{
	return 0;
}

int lb_up_policy_prio(u32 pid)
{
	return 0;
}

int lb_down_policy_prio(u32 pid)
{
	return 0;
}

u32 lb_get_master_policy(u32 pid, enum policy_type type)
{
	return 0;
}

void *lb_page_to_virt(struct page *page)
{
	u64 virt;

	if (!is_lb_available() || !page)
		return NULL;

	WARN(!PageLB(page), "fail pagelb");
	WARN(!PagePrivate(page), "fail pageprivate");
	virt = page->private & LB_VA_MASK;

	return (void *)virt;
}
EXPORT_SYMBOL(lb_page_to_virt);

/* This interface is used for iommu pgtable pte prot build */
u64 lb_pid_to_gidphys(u32 pgid)
{
	if (!is_lb_available())
		return 0;

#ifdef CONFIG_MM_LB_GID_PBHA
	return PTE_LB(pgid & PGID_LOW_4_BITS_MASK) | PBHA_GID_HIGH_BIT((pgid & PGID_HIGH_2_BITS_MASK) >> 4);
#else
	return PTE_LB(pgid);
#endif
}

u32 lb_get_page_osh_flag(struct page *page)
{
	return page->private & BIT(LB_PRIV_OSH_SHIFT);
}

u32 lb_page_to_gid(struct page *page)
{
	u32 gid_idx;

	if (!is_lb_available() || !page || !PageLB(page))
		return 0;

	WARN(!PagePrivate(page), "fail pageprivate");
	gid_idx = page->private & LB_PRIV_GID_MASK;

	return gid_idx;
}
EXPORT_SYMBOL(lb_page_to_gid);

/*
 * in order to avoid GPU mapping framebuf with dss'gid,
 * only is called by GPU r18,
 * only is a circumvention plan.
 */
u64 lb_pte_attr_gpu(phys_addr_t phy_addr)
{
	return lb_pte_attr(phy_addr);
}

/*
 * when call it @ ion
 * @ map to cpu user/kernel space
 */
int lb_pid_prot_build(u32 pgid, pgprot_t *pageprot)
{
	if (!is_lb_available())
		return 0;

	if (!pageprot)
		return -EINVAL;

	*pageprot = pgprot_lb(*pageprot, pgid);

	return 0;
}

int lb_pages_attach(u32 pgid, struct page *pg, size_t count)
{
	int ret;
	int cpu_pgid;
	unsigned long flags = 0;
	pgid_info *p_info = NULL;
	pgid_info *cpu_p_info = NULL;

	lb_print(LB_INFO, "begin\n");

	if (!is_lb_available()) {
		lb_print(LB_ERROR, "lb not available");
		return 0;
	}

	if (!pg) {
		lb_print(LB_ERROR, "pages invalid\n");
		return -EINVAL;
	}

	cpu_pgid = get_cpu_pgid(pgid);
	if (cpu_pgid < 0)
		return -EINVAL;

	/* set pages */
	ret = set_page(cpu_pgid, pg, count);
	if (ret) {
		lb_print(LB_ERROR, "set page failed\n");
		return -EINVAL;
	}

	cpu_p_info = get_pgid_info(cpu_pgid);
	spin_lock_irqsave(&cpu_p_info->lock, flags);
	cpu_p_info->page_count += count;
	spin_unlock_irqrestore(&cpu_p_info->lock, flags);

	if (pgid != cpu_pgid)
		p_info = get_pgid_info(pgid);

	if (p_info) {
		spin_lock_irqsave(&p_info->lock, flags);
		p_info->page_count += count;
		spin_unlock_irqrestore(&p_info->lock, flags);
	}

	lb_gid_enable(cpu_pgid);
	if (p_info)
		lb_gid_enable(pgid);

	lb_print(LB_INFO, "end\n");

	return 0;
}

int lb_pages_detach(u32 pgid, struct page *pages, size_t count)
{
	int cpu_pgid;
	pgid_info *p_info = NULL;
	pgid_info *cpu_p_info = NULL;
	unsigned long flags = 0;

	lb_print(LB_INFO, "begin\n");

	if (!is_lb_available())
		return 0;

	if (!pages) {
		lb_print(LB_ERROR, "pages invalid\n");
		return -EINVAL;
	}

	cpu_pgid = get_cpu_pgid(pgid);
	if (cpu_pgid < 0)
		return -EINVAL;

	cpu_p_info = get_pgid_info(cpu_pgid);
	spin_lock_irqsave(&cpu_p_info->lock, flags);
	if (WARN_ON(cpu_p_info->page_count < count))
		cpu_p_info->page_count = 0;
	else
		cpu_p_info->page_count -= count;
	spin_unlock_irqrestore(&cpu_p_info->lock, flags);

	if (pgid != cpu_pgid)
		p_info = get_pgid_info(pgid);

	if (p_info) {
		spin_lock_irqsave(&p_info->lock, flags);
		if (WARN_ON(p_info->page_count < count))
			p_info->page_count = 0;
		else
			p_info->page_count -= count;
		spin_unlock_irqrestore(&p_info->lock, flags);
	}

	if (!cpu_pgid && !cpu_p_info->page_count)
		lb_gid_bypass(cpu_pgid);

	if (p_info && !pgid && !p_info->page_count)
		lb_gid_bypass(pgid);

	reset_page(cpu_pgid, pages, count);

	lb_print(LB_INFO, "end\n");

	return 0;
}
EXPORT_SYMBOL(lb_pages_detach);

#ifdef CONFIG_DMABUF_UDK_EN
int lb_pages_attach_from_udk(u32 pgid, struct page *pg, size_t count)
{
	int ret;
	int cpu_pgid;
	unsigned long flags = 0;
	pgid_info *p_info = NULL;
	pgid_info *cpu_p_info = NULL;

	lb_print(LB_INFO, "begin\n");

	if (!is_lb_available()) {
		lb_print(LB_ERROR, "lb not available");
		return -EPERM;
	}

	if (!pg) {
		lb_print(LB_ERROR, "pages invalid\n");
		return -EINVAL;
	}

	cpu_pgid = get_cpu_pgid(pgid);
	if (cpu_pgid < 0)
		return -EINVAL;

	/* set pages */
	ret = set_page_from_udk(cpu_pgid, pg, count);
	if (ret) {
		lb_print(LB_ERROR, "set page failed\n");
		return -EINVAL;
	}

	cpu_p_info = get_pgid_info(cpu_pgid);
	spin_lock_irqsave(&cpu_p_info->lock, flags);
	cpu_p_info->page_count += count;
	spin_unlock_irqrestore(&cpu_p_info->lock, flags);

	if (pgid != cpu_pgid)
		p_info = get_pgid_info(pgid);

	if (p_info) {
		spin_lock_irqsave(&p_info->lock, flags);
		p_info->page_count += count;
		spin_unlock_irqrestore(&p_info->lock, flags);
	}

	lb_gid_enable(cpu_pgid);
	if (p_info)
		lb_gid_enable(pgid);
 
	lb_print(LB_INFO, "end\n");

	return 0;
}

int lb_pages_detach_from_udk(u32 pgid, struct page *pages, size_t count)
{
	int cpu_pgid;
	pgid_info *p_info = NULL;
	pgid_info *cpu_p_info = NULL;
	unsigned long flags = 0;

	lb_print(LB_INFO, "begin\n");

	if (!is_lb_available())
		return -EPERM;

	if (!pages) {
		lb_print(LB_ERROR, "pages invalid\n");
		return -EINVAL;
	}

	cpu_pgid = get_cpu_pgid(pgid);
	if (cpu_pgid < 0)
		return -EINVAL;

	reset_page_from_udk(cpu_pgid, pages, count);

	cpu_p_info = get_pgid_info(cpu_pgid);
	spin_lock_irqsave(&cpu_p_info->lock, flags);
	if (WARN_ON(cpu_p_info->page_count < count))
		cpu_p_info->page_count = 0;
	else
		cpu_p_info->page_count -= count;
	spin_unlock_irqrestore(&cpu_p_info->lock, flags);

	if (pgid != cpu_pgid)
		p_info = get_pgid_info(pgid);

	if (p_info) {
		spin_lock_irqsave(&p_info->lock, flags);
		if (WARN_ON(p_info->page_count < count))
			p_info->page_count = 0;
		else
			p_info->page_count -= count;
		spin_unlock_irqrestore(&p_info->lock, flags);
	}

	if (!cpu_pgid && !cpu_p_info->page_count)
		lb_gid_bypass(cpu_pgid);

	if (p_info && !pgid && !p_info->page_count)
		lb_gid_bypass(pgid);

	lb_print(LB_INFO, "end\n");

	return 0;
}
EXPORT_SYMBOL(lb_pages_detach_from_udk);
#endif /* CONFIG_DMABUF_UDK_EN */

void lb_pages_set_osh(struct page *pg, size_t count, u32 osh)
{
	int i;

	for (i = 0; i < count; i++) {
		if (unlikely(!pg)) {
			lb_print(LB_ERROR, "pages is NULL, i = %d\n", i);
			return;
		}
		set_page_private(pg,
			osh ?
			page_private(pg) | BIT(LB_PRIV_OSH_SHIFT) :
			page_private(pg) & (~BIT(LB_PRIV_OSH_SHIFT)));
		pg++;
	}
}

void lb_sg_set_osh(struct scatterlist *sgl, u32 nents, u32 osh)
{
	struct scatterlist *sg = NULL;
	int i;

	if (!sgl)
		return;

	for_each_sg(sgl, sg, (int)nents, i) {
		if (!sg)
			return;

		lb_pages_set_osh(phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT, osh);
	}
}

struct page *lb_alloc_pages(u32 pgid, gfp_t gfp_mask, u32 order)
{
	struct page *lb_page = NULL;
	/*
	 * alloc pages
	 * pgid = 0 for GPU
	 */
#ifdef CONFIG_MM_AMA
	lb_page = ama_alloc_meida_pages(gfp_mask, order);
#else
	lb_page = alloc_pages(gfp_mask, order);
#endif
	if (!lb_page) {
		lb_print(LB_ERROR, "pgid %u:alloc pages failed\n", pgid);
		goto err;
	}

	if (!is_lb_available() || !pgid)
		return lb_page;

	if (lb_pages_attach(pgid, lb_page, 1ULL << order)) {
		lb_print(LB_ERROR, "pgid %u:lb pages attach failed\n", pgid);
		goto err;
	}

	return lb_page;

err:
	if (lb_page)
		__free_pages(lb_page, order);
	return NULL;
}
EXPORT_SYMBOL(lb_alloc_pages);

struct page *lb_alloc_pages_bigbw(u32 pgid, gfp_t gfp_mask, u32 order)
{
	struct page *lb_page = NULL;
	/*
	 * alloc pages
	 * pgid = 0 for GPU
	 */
#ifdef CONFIG_MM_AMA
	lb_page = ama_alloc_bigbw_pages(gfp_mask, order);
#else
	lb_page = alloc_pages(gfp_mask, order);
#endif
	if (!lb_page) {
		lb_print(LB_ERROR, "pgid %u:alloc pages failed\n", pgid);
		goto err;
	}

	if (!is_lb_available() || !pgid)
		return lb_page;

	if (lb_pages_attach(pgid, lb_page, 1ULL << order)) {
		lb_print(LB_ERROR, "pgid %u:lb pages attach failed\n", pgid);
		goto err;
	}

	return lb_page;

err:
	if (lb_page)
		__free_pages(lb_page, order);
	return NULL;
}

/*
 * alloc lb pages with osh flag for gpu
 */
struct page *lb_alloc_pages_osh(u32 pgid, gfp_t gfp_mask, u32 order)
{
	struct page *lb_page = NULL;
	/*
	 * alloc pages
	 * pgid = 0 for GPU
	 */
#ifdef CONFIG_MM_AMA
	lb_page = ama_alloc_meida_pages(gfp_mask, order);
#else
	lb_page = alloc_pages(gfp_mask, order);
#endif
	if (!lb_page) {
		lb_print(LB_ERROR, "pgid %u:alloc pages failed\n", pgid);
		goto err;
	}

	if (!is_lb_available() || !pgid)
		return lb_page;

	lb_pages_set_osh(lb_page, 1ULL << order, TRUE);

	if (lb_pages_attach(pgid, lb_page, 1ULL << order)) {
		lb_print(LB_ERROR, "pgid %u:lb pages attach failed\n", pgid);
		goto err;
	}

	return lb_page;

err:
	if (lb_page)
		__free_pages(lb_page, order);
	return NULL;
}
EXPORT_SYMBOL(lb_alloc_pages_osh);

/*
 * alloc lb pages with osh flag for gpu
 */
struct page *lb_alloc_pages_osh_bigbw(u32 pgid, gfp_t gfp_mask, u32 order)
{
	struct page *lb_page = NULL;
	/*
	 * alloc pages
	 * pgid = 0 for GPU
	 */
#ifdef CONFIG_MM_AMA
	lb_page = ama_alloc_bigbw_pages(gfp_mask, order);
#else
	lb_page = alloc_pages(gfp_mask, order);
#endif
	if (!lb_page) {
		lb_print(LB_ERROR, "pgid %u:alloc pages failed\n", pgid);
		goto err;
	}

	if (!is_lb_available() || !pgid)
		return lb_page;

	lb_pages_set_osh(lb_page, 1ULL << order, TRUE);

	if (lb_pages_attach(pgid, lb_page, 1ULL << order)) {
		lb_print(LB_ERROR, "pgid %u:lb pages attach failed\n", pgid);
		goto err;
	}

	return lb_page;

err:
	if (lb_page)
		__free_pages(lb_page, order);
	return NULL;
}

int lb_free_pages(u32 pgid, struct page *pages, u32 order)
{
	if (!is_lb_available() || !pgid)
		goto succ;

	if (lb_pages_detach(pgid, pages, 1UL << order)) {
		lb_print(LB_ERROR, "lb pages detach failed\n");
		goto err_nofree;
	}

succ:
	if (pages)
		__free_pages(pages, order);
	return 0;

err_nofree:
	return -EINVAL;
}
EXPORT_SYMBOL(lb_free_pages);

int lb_gid_enable(u32 pgid)
{
	pgid_info *p_info = NULL;
	unsigned long flags = 0;

	if (!is_lb_available())
		return 0;

	p_info = get_pgid_info(pgid);
	if (!p_info)
		return -EINVAL;

	spin_lock_irqsave(&p_info->lock, flags);
	if (p_info->pgid.disable)
		lb_pgid_enable(pgid);
	else
		lb_print(LB_INFO, "pgid %u already enabled!\n", pgid);
	spin_unlock_irqrestore(&p_info->lock, flags);

	return 0;
}

int lb_gid_bypass(u32 pgid)
{
	pgid_info *p_info = NULL;
	unsigned long flags = 0;

	if (!is_lb_available())
		return 0;

	p_info = get_pgid_info(pgid);
	if (!p_info)
		return -EINVAL;

	spin_lock_irqsave(&p_info->lock, flags);
	if (!p_info->pgid.disable)
		lb_pgid_bypass(pgid);
	else
		lb_print(LB_INFO, "pgid %u already disabled!\n", pgid);
	spin_unlock_irqrestore(&p_info->lock, flags);
	return 0;
}

int perf_ctrl_lb_set_policy(void __user *uarg)
{
	int res;
	struct lb_policy_config lb_config;

	if (uarg == NULL)
		return -EINVAL;

	if (!is_lb_available())
		return 0;

	if (copy_from_user(&lb_config, uarg, sizeof(struct lb_policy_config))) {
		lb_print(LB_ERROR, "set master prio copy_to_user fail.\n");
		return -EFAULT;
	}

	res = lb_set_master_policy(lb_config.pid, lb_config.quota, lb_config.prio);

	return res;
}

int perf_ctrl_lb_reset_policy(void __user *uarg)
{
	int res;
	struct lb_policy_config lb_config;

	if (uarg == NULL)
		return -EINVAL;

	if (!is_lb_available())
		return 0;

	if (copy_from_user(&lb_config, uarg, sizeof(struct lb_policy_config))) {
		lb_print(LB_ERROR, "reset master prio copy_to_user fail.\n");
		return -EFAULT;
	}

	res = lb_reset_master_policy(lb_config.pid);

	return res;
}

int perf_ctrl_lb_set_quota(void __user *uarg)
{
	u32 cgid;
	cgid_info *c_info = NULL;
	struct lb_policy_config lb_config = {0};

	if (uarg == NULL)
		return -EINVAL;

	if (!is_lb_available)
		return 0;

	if (copy_from_user(&lb_config, uarg, sizeof(struct lb_policy_config))) {
		pr_err("set quota copy_from_user fail.\n");
		return -EFAULT;
	}

	if (lb_config.pid >= PID_MAX) {
		lb_print(LB_ERROR, "pid %u invalid\n", lb_config.pid);
		return -EINVAL;
	}

	cgid = get_pgid_cgid_idx(lb_config.pid);
	c_info = get_cgid_info(cgid);
	spin_lock(&c_info->lock);
	set_cgid_quota(cgid, lb_config.quota, FALSE);
	set_quota(cgid, lb_config.quota);
	spin_unlock(&c_info->lock);

	return 0;
}

int perf_ctrl_lb_reset_quota(void __user *uarg)
{
	u32 cgid;
	cgid_info *c_info = NULL;
	struct lb_policy_config lb_config = {0};

	if (uarg == NULL)
		return -EINVAL;

	if (!is_lb_available())
		return 0;

	if (copy_from_user(&lb_config, uarg, sizeof(struct lb_policy_config))) {
		lb_print(LB_ERROR, "reset quota copy_from_user fail.\n");
		return -EFAULT;
	}

	if (lb_config.pid >= PID_MAX) {
		lb_print(LB_ERROR, "pid %u invalid\n", lb_config.pid);
		return -EINVAL;
	}

	cgid = get_pgid_cgid_idx(lb_config.pid);
	c_info = get_cgid_info(cgid);
	spin_lock(&c_info->lock);
	set_cgid_quota(cgid, c_info->org_quota, FALSE);
	set_quota(cgid, c_info->org_quota);
	spin_unlock(&c_info->lock);

	return 0;
}

int perf_ctrl_lb_lite_info(void __user *uarg)
{
	struct lb_policy_config lb_config = {};

	if (uarg == NULL)
		return -EINVAL;

	lb_config.available = is_lb_available();

	if (copy_to_user(uarg, &lb_config, sizeof(struct lb_policy_config))) {
		lb_print(LB_ERROR, "get lite info copy_to_user fail.\n");
		return -EFAULT;
	}

	return 0;
}

int get_flow_stat(struct lb_policy_flow *flow)
{
	return 0;
}

u32 get_sc_size(void)
{
	if (!is_lb_available())
		return 0;

	return lbdev->max_size;
}

u32 lb_get_pid_by_userpid(u32 user_pid)
{
	u32 pid;

	switch (user_pid) {
	case ION_LB_VENC:
		pid = PID_VENC;
		break;
	default:
		pid = user_pid;
		break;
	}

	return pid;
}