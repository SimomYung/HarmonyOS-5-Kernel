/*
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/genalloc.h>
#include <linux/gfp.h>
#include <linux/mm_lb/mm_lb.h>
#include <linux/dma-direction.h>
#include <dsm/dsm_pub.h>
#include <asm/cacheflush.h>
#include "securec.h"
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include "mm_lb_priv.h"
#ifdef CONFIG_MM_LB_SLAVE
#include "mm_lb_ipc.h"
#endif
#include "mm_lb_common.h"
#include "mm_lb_driver.h"

#ifndef CONFIG_LIBLINUX
static int f(pte_t *pte, unsigned long addr, void *data)
{
	pte_t ***p = data;

	if (p) {
		*(*p) = pte;
		(*p)++;
	}
	return 0;
}


static struct vm_struct *__alloc_vm_area(size_t size, pte_t **ptes)
{
	struct vm_struct *area;

	area = get_vm_area_caller(size, VM_IOREMAP,
				__builtin_return_address(0));
	if (area == NULL)
		return NULL;

	/*
	 * This ensures that page tables are constructed for this region
	 * of kernel virtual address space and mapped into init_mm.
	 */
	if (apply_to_page_range(&init_mm, (unsigned long)area->addr,
				size, f, ptes ? &ptes : NULL)) {
		free_vm_area(area);
		return NULL;
	}

	return area;
}

int add_gid_vm(vm_info *vm, size_t sz)
{
	lb_area *vm_ar = NULL;

	vm_ar = kzalloc(sizeof(lb_area), GFP_KERNEL);
	if (!vm_ar) {
		lb_print(LB_ERROR, "%s kzalloc failed\n", __func__);
		goto err;
	}

	vm_ar->area = __alloc_vm_area(sz, NULL);
	if (!vm_ar->area) {
		lb_print(LB_ERROR, "%s alloc vm area failed\n", __func__);
		goto free_vm_ar;
	}

	if (gen_pool_add(vm->pool, (uintptr_t)vm_ar->area->addr, sz, -1)) {
		lb_print(LB_ERROR, "%s add gen pool failed\n", __func__);
		goto free_area;
	}

	list_add_tail(&vm_ar->next, &vm->list);

	return 0;

free_area:
	free_vm_area(vm_ar->area);

free_vm_ar:
	kfree(vm_ar);

err:
	return -ENOMEM;
}

static int map_page_range(unsigned long va, size_t sz,
		pgprot_t prot, struct page **pages)
{
	int err;

	err = map_kernel_range_noflush(va, sz, prot, pages);
	flush_cache_vmap(va, va + sz);

	return err > 0 ? 0 : err;
}

static unsigned long alloc_pool(vm_info *vm, size_t sz)
{
	unsigned long va;

	/* try alloc pool, gen_pool_alloc have lock */
	va = gen_pool_alloc(vm->pool, sz);
	if (!va) {
		/* add gid vm need mutex lock */
		mutex_lock(&vm->mutex);

		va = gen_pool_alloc(vm->pool, sz);
		if (!va && !add_gid_vm(vm, SZ_64M))
			va = gen_pool_alloc(vm->pool, sz);

		mutex_unlock(&vm->mutex);
	}

	return va;
}

static inline void free_pool(vm_info *vm, unsigned long va, size_t sz)
{
	gen_pool_free(vm->pool, va, sz);
}

void vm_unmap(vm_info *vm, const void *va, size_t sz)
{
	if (!vm) {
		lb_print(LB_ERROR, "vm not alloc\n");
		return;
	}

	unmap_kernel_range((uintptr_t)va, sz);
	free_pool(vm, (uintptr_t)va, sz);
}

void *vm_map(vm_info *vm, struct page **pages,
		size_t cnt, pgprot_t prot)
{
	void *va = NULL;
	size_t sz = cnt << PAGE_SHIFT;

	if (!vm) {
		lb_print(LB_ERROR, "vm not alloc\n");
		return NULL;
	}

	va = (void *)alloc_pool(vm, sz);
	if (!va) {
		lb_print(LB_ERROR, "alloc pool failed\n");
		return NULL;
	}

	if (map_page_range((uintptr_t)va, sz, prot, pages)) {
		free_pool(vm, (uintptr_t)va, sz);
		lb_print(LB_ERROR, "map page range failed\n");
		return NULL;
	}

	return va;
}
#endif

/* gpu uses this func for acquiring cpu pgtable gid prot */
int lb_prot_build(struct page *pages, pgprot_t *pageprot)
{
	unsigned long gid_idx;

	if (!is_lb_available() || !pages || !pageprot || !PageLB(pages))
		return 0;

	gid_idx = lb_page_to_gid(pages);
	*pageprot = pgprot_lb(*pageprot, gid_idx);

	return 0;
}

/* when call it set pte for cpu/iommu/gpu */
u64 lb_pte_attr(phys_addr_t phy_addr)
{
	unsigned long gid_idx;
	struct page *pg = NULL;

	if (!is_lb_available())
		return 0;

	/*
	 * pfn illegal judgment affects efficiency
	 * the current only gpu use lb_pte_attr
	 * gpu input a valid addree
	 */
	pg = phys_to_page(phy_addr);
	if (!PageLB(pg))
		return 0;

	gid_idx = lb_page_to_gid(pg);

	return pgprot_val(PAGE_LB_CACHEABLE(gid_idx));
}

/*
 * assert at set pte
 * assert for page's gid is diff pte gid
 */
void __lb_assert_pte(pte_t pte)
{
}

/* assert at page to virt */
void __lb_assert_phys(phys_addr_t phys)
{
	struct page *p = NULL;

	p = phys_to_page(phys);
	if (p && PageLB(p)) {
		lb_print(LB_ERROR, "page flag 0x%lx\n", p->flags);
		rdr_syserr_process_for_ap(MODID_AP_S_PANIC_LB, 0ULL, 0ULL);
	}
}

/*
 * assert at page to virt
 * assert at kmap
 * it is ok
 */
void __lb_assert_page(struct page *pg)
{
	if (!pg)
		return;

	if (PageLB(pg))
		rdr_syserr_process_for_ap(MODID_AP_S_PANIC_LB, 0ULL, 0ULL);
}

int lb_sg_attach(unsigned int pid, struct scatterlist *sgl, unsigned int nents)
{
	int i, j;
	struct scatterlist *sg = NULL;

	lb_print(LB_INFO, "begin! pid %u\n", pid);

	/* for gpu pid is normal call path */
	if (!is_lb_available() || !pid)
		return 0;

	if (!sgl)
		return -EINVAL;

	for_each_sg(sgl, sg, (int)nents, i) {
		if (!sg || lb_pages_attach(pid, phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT))
			goto err;
	}

	lb_print(LB_INFO, "end\n");

	return 0;

err:
	for_each_sg(sgl, sg, i, j) {
		if (!sg || lb_pages_detach(pid, phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT))
			lb_print(LB_ERROR, "page detach failed\n");
	}

	return -EINVAL;
}

int lb_sg_detach(unsigned int pid, struct scatterlist *sgl, unsigned int nents)
{
	int i;
	struct scatterlist *sg = NULL;

	lb_print(LB_INFO, "begin\n");

	/* for gpu pid is normal call path */
	if (!is_lb_available() || !pid)
		return 0;

	if (!sgl && sg_nents(sgl) != nents)
		return -EINVAL;

	for_each_sg(sgl, sg, (int)nents, i)
		if (lb_pages_detach(pid, phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT))
			lb_print(LB_ERROR, "page detach failed\n");

	lb_print(LB_INFO, "end\n");

	return 0;
}

#ifdef CONFIG_DMABUF_UDK_EN
int lb_sg_attach_from_udk(unsigned int pid, struct scatterlist *sgl, unsigned int nents)
{
	int i, j;
	struct scatterlist *sg = NULL;
 
	lb_print(LB_INFO, "%s begin! pid %u\n", __func__, pid);
 
	/* for gpu pid is normal call path */
	if (!is_lb_available() || !pid)
		return -EPERM;
 
	if (!sgl)
		return -EINVAL;
 
	for_each_sg(sgl, sg, (int)nents, i) {
		if (!sg || lb_pages_attach_from_udk(pid, phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT))
			goto err;
	}
 
	lb_print(LB_INFO, "%s end\n", __func__);
 
	return 0;
 
err:
	for_each_sg(sgl, sg, i, j) {
		if (!sg || lb_pages_detach_from_udk(pid, phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT))
			lb_print(LB_ERROR, "page detach failed\n");
	}
 
	return -EINVAL;
}

int lb_sg_detach_from_udk(unsigned int pid, struct scatterlist *sgl, unsigned int nents)
{
	int i;
	struct scatterlist *sg = NULL;
 
	lb_print(LB_INFO, "%s begin! pid %u\n", __func__, pid);
 
	/* for gpu pid is normal call path */
	if (!is_lb_available() || !pid)
		return -EPERM;
 
	if (!sgl && sg_nents(sgl) != nents)
		return -EINVAL;
 
	for_each_sg(sgl, sg, (int)nents, i)
		if (lb_pages_detach_from_udk(pid, phys_to_page(sg_phys(sg)),
			sg->length >> PAGE_SHIFT))
			lb_print(LB_ERROR, "lb_sg_detach_from_udk page detach failed\n");
 
	lb_print(LB_INFO, "%s end\n", __func__);
 
	return 0;
}
#endif /* CONFIG_DMABUF_UDK_EN */

#ifndef CONFIG_MM_LB_V550
#ifdef CONFIG_HISYSEVENT
typedef struct {
	int errorno;
	char *name;
} errorno_map;

static int lb_errorno_to_str(int errorno, char *str, int bufflen)
{
	int i, num;

	errorno_map ecc_map[] = {
		{DSM_LB_ECC_SINGLE_BIT, "LB_ECC_1BIT"},
		{DSM_LB_ECC_MUTIL_BIT, "LB_ECC_MULTI_BIT"},
	};

	num = sizeof(ecc_map) / sizeof(ecc_map[0]);
	for (i = 0; i < num; i++) {
		if (ecc_map[i].errorno == errorno) {
			if (strlen(ecc_map[i].name) >= bufflen)
				return -1;
			strncpy_s(str, bufflen, ecc_map[i].name, bufflen - 1);
			return 0;
		}
	}
	return -1;
}

static struct dsm_client_ops lb_dsm_ops = {
	.poll_state = NULL,
	.dump_func = NULL,
	.errorno_to_str = lb_errorno_to_str,
};
#endif

struct dsm_dev lb_dsm_dev = {
	.name        = "MM_LB",
	.device_name = "LB",
	.ic_name     = NULL,
	.module_name = NULL,
#ifdef CONFIG_HISYSEVENT
	.fops        = &lb_dsm_ops,
#else
	.fops        = NULL,
#endif
	.buff_size   = 1024,
};

static void dsm_lb_handle_work(struct work_struct *work)
{
	struct dsm_client *ai_client;

	ai_client = dsm_find_client("MM_LB");
	if (!ai_client) {
		lb_print(LB_ERROR, "lb dsm is null\n");
		return;
	}
	lbdev->dsm.ai_client = ai_client;
}

int dsm_init(struct lb_device *lbd)
{
	lbd->dsm.ai_client = dsm_register_client(&lb_dsm_dev);
	INIT_WORK(&(lbd->dsm.dsm_work), dsm_lb_handle_work);

	return 0;
}

int soc_init(struct platform_device *pdev, struct lb_device *lbd)
{
	int irq;

	lb_print(LB_INFO, "into %s\n", __func__);

	/* register intr */
	irq = platform_get_irq_byname(pdev, "dfx-intr");
	if (irq < 0) {
		lb_print(LB_ERROR, "get dfx irq failed\n");
		return -EINVAL;
	}
	lbd->dfx_irq = irq;

	irq = platform_get_irq_byname(pdev, "cmd-intr");
	if (irq < 0) {
		lb_print(LB_ERROR, "get cmd-intr failed\n");
		return -EINVAL;
	}
	lbd->cmd_irq = irq;

	/* enable intr reg */
	if (reset_ip(lbd)) {
		lb_print(LB_ERROR, "reset ip failed\n");
		return -EINVAL;
	}

	lb_print(LB_INFO, "out %s\n", __func__);

	return 0;
}

#ifndef CONFIG_LIBLINUX
void vm_uninit(struct lb_device *lbd)
{
	int i;
	vm_info *vm = NULL;
	lb_area *vm_ar = NULL;
	lb_area *tmp_ar = NULL;

	for (i = 0; i < lbd->gdplc.nr; i++) {
		vm = lbd->gdplc.ptbl[i].vm;
		if (!vm)
			continue;

		list_for_each_entry_safe(vm_ar, tmp_ar, &vm->list, next) {
			list_del(&vm_ar->next);

			if (vm_ar->area)
				free_vm_area(vm_ar->area);

			kfree(vm_ar);
		}

		if (vm->pool)
			gen_pool_destroy(vm->pool);

		mutex_destroy(&vm->mutex);

		kfree(vm);
		lbd->gdplc.ptbl[i].vm = NULL;
	}
}

int vm_init(struct lb_device *lbd)
{
	int i;
	vm_info *vm = NULL;

	for (i = 0; i < lbd->gdplc.nr; i++) {
		if (!(lbd->gdplc.ptbl[i].stat & GID_VM_STATE))
			continue;

		vm = kzalloc(sizeof(vm_info), GFP_KERNEL);
		if (!vm) {
			lb_print(LB_ERROR, "kzalloc failed\n");
			goto vm_free;
		}
		lbd->gdplc.ptbl[i].vm = vm;

		mutex_init(&(vm->mutex));
		INIT_LIST_HEAD(&(vm->list));

		vm->pool = gen_pool_create(PAGE_SHIFT, -1);
		if (!vm->pool) {
			lb_print(LB_ERROR, "gen pool create failed\n");
			goto vm_free;
		}

		if (add_gid_vm(vm, SZ_64M)) {
			lb_print(LB_ERROR, "add pool failed\n");
			goto vm_free;
		}
	}

	return 0;

vm_free:
	vm_uninit(lbd);

	return -ENOMEM;
}
#endif

int set_page(struct lb_plc_info *policy,
		struct page *page_alloc, size_t count)
{
	void *vaddr = NULL;
	struct page *pg = page_alloc;
	struct page **pages = NULL;
	struct page **tmp = NULL;
	size_t i;

#ifdef CONFIG_MM_LB_L3_EXTENSION
	change_secpage_range(page_to_phys(page_alloc),
		(uintptr_t)page_address(page_alloc),
		count << PAGE_SHIFT, __pgprot(PROT_DEVICE_nGnRE));
	flush_tlb_kernel_range((uintptr_t)page_to_virt(page_alloc),
		(uintptr_t)page_to_virt(pg) + (count << PAGE_SHIFT));
#endif

	/*
	 * invalid acpu cache by normal va
	 * it is necessary by normal va
	 * cache |--va--| |--va with gid--|
	 * lb             |-------lb------|
	 * ddr   |------|
	 */
	__dma_unmap_area(page_to_virt(pg), count << PAGE_SHIFT,
			DMA_FROM_DEVICE);

	pages = kcalloc(count, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		lb_print(LB_ERROR, "zalloc page\n");
		goto change_mapping;
	}

	tmp = pages;
	for (i = 0; i < count; i++)
		*(tmp++) = pg++;

#ifndef CONFIG_LIBLINUX
	vaddr = vm_map(policy->vm, pages, count,
			pgprot_lb(PAGE_KERNEL, policy->gid));
#else
	vaddr = vmap(pages, count,
	   VM_MAP, pgprot_lb(PAGE_KERNEL, policy->gid));
#endif
	if (!vaddr) {
		lb_print(LB_ERROR, "%s:vm map failed\n", __func__);
		goto free_page;
	}

	kfree(pages);

	/* set the page privite */
	pg = page_alloc;

	for (i = 0; i < count; i++) {
		set_page_private(pg, (uintptr_t)vaddr | policy->gid);
		SetPagePrivate(pg);
		SetPageLB(pg);
		pg++;
		vaddr += PAGE_SIZE;
	}

	return 0;

free_page:
	kfree(pages);

change_mapping:
#ifdef CONFIG_MM_LB_L3_EXTENSION
	change_secpage_range(page_to_phys(page_alloc),
		(uintptr_t)page_address(page_alloc),
		count << PAGE_SHIFT, PAGE_KERNEL);
	flush_tlb_kernel_range((uintptr_t)page_to_virt(page_alloc),
		(uintptr_t)page_to_virt(page_alloc) + (count << PAGE_SHIFT));
#endif

	return -ENOMEM;
}

void reset_page(struct lb_plc_info *policy,
		struct page *page, size_t count)
{
	int i;
	unsigned int gid;
	void *vaddr = NULL;
	struct page *pg = page;
	phys_addr_t phys = page_to_phys(pg);
	unsigned long size = count << PAGE_SHIFT;

	if (!page) {
		lb_print(LB_ERROR, "%s:reset page falied\n", __func__);
		return;
	}

	WARN_ON(!PageLB(pg));
	gid = lb_page_to_gid(pg);
	WARN(policy->gid != gid, "pgid %u ggid %u\n", policy->gid, gid);

	/*
	 * invalid acpu cache by lb va
	 * it is necessary by lb va
	 * cache |--va--| |--va with gid--|
	 * lb             |-------lb------|
	 * ddr   |------|
	 */
	vaddr = lb_page_to_virt(page);
	__dma_unmap_area(vaddr, size, DMA_FROM_DEVICE);

#ifndef CONFIG_LIBLINUX
	vm_unmap(policy->vm, vaddr, size);
#else
	vunmap(vaddr);
#endif

	for (i = 0; i < (int)count; i++) {
		set_page_private(pg, 0);
		ClearPagePrivate(pg);
		ClearPageLB(pg);
		pg++;
	}

#ifdef CONFIG_MM_LB_SLAVE
	if (get_pid_status(policy->pid) == PID_STATUS_ENABLE)
#endif
	/* invalid system cache */
	lb_invalid_cache(gid, phys, size);

#ifdef CONFIG_MM_LB_L3_EXTENSION
	change_secpage_range(page_to_phys(page),
		(uintptr_t)page_address(page),
		count << PAGE_SHIFT, PAGE_KERNEL);
	flush_tlb_kernel_range((uintptr_t)page_to_virt(page),
		(uintptr_t)page_to_virt(page) + (count << PAGE_SHIFT));
#endif
}

#endif