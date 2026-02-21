/*
 * HiStar Oisp driver
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/firmware.h>
#include <linux/iommu.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <securec.h>
#include <platform_include/isp/linux/hoisp.h>
#include <platform_include/isp/linux/hisp_mempool.h>
#ifdef CONFIG_MM_AMA_V2
#include <platform_include/basicplatform/linux/mem_ama.h>
#endif

#include <linux/pinctrl/consumer.h>

#ifdef CONFIG_MM_LB
#include <linux/mm_lb/mm_lb.h>
#define OISP_USE_SC 1
#else
#define OISP_USE_SC 0
#endif

#define OISP_MEM_FALSE 0
#define OISP_MEM_TRUE 1

enum oisp_mem_status {
	OISP_MEM_MAP,
	OISP_MEM_USING,
	OISP_MEM_UNMAP,
	OISP_MEM_NULL,
};

struct memory_status {
	unsigned int is_mapped;
	unsigned int is_using;
	unsigned long long shared_addr;
	unsigned long size;
};

struct hoisp_mem_info {
	unsigned int mem_addr;
	unsigned int mem_size;
	enum oisp_mem_status mem_status;
};

struct hoisp_device {
	struct device *dev;
	struct hoisp_mem_info text_mem;
	struct hoisp_mem_info data_mem;
	struct hoisp_mem_info msg_mem;
	struct hoisp_mem_info share_mem;
	struct memory_status *mem_status;
	const char *firmware;
	struct iommu_domain *domain;
	struct sg_table *text_table;
	struct sg_table *data_table;
	atomic_t oisp_prepare_status;
	struct pinctrl *pinctrls;
	struct pinctrl_state *pinctrls_default;
	struct pinctrl_state *pinctrls_idle;
};

struct page_info {
	struct page *page;
	unsigned int order;
	struct list_head node;
};

struct hoisp_device hoisp_dev = {0};

static const unsigned int orders[] = {10, 8, 4, 0};
#define OISP_NUM_ORDERS ARRAY_SIZE(orders)

static void *hoisp_mem_map_kernel(struct sg_table *table, unsigned long length);
static void hoisp_mem_unmap_kernel(const void *va);

struct hisp_mem_info_t {
	unsigned long long cpu_addr;
	unsigned long size;
};

struct hisp_mem_info_t g_isp_msg_mem_info = {0};
struct hisp_mem_info_t g_isp_shared_mem_info = {0};

static int hoisp_msg_mem_alloc(void)
{
	struct hisp_mem_type type = { 0 };
	struct hisp_map_info_s map_info = { 0 };
	struct hisp_mem_info_s mem_info = { 0 };

	type.master = ISP_CPU | OISP_CPU;
	type.mem_type = ISP_PRIVATE_MEM;
	map_info.fd = -1;
	map_info.size = OISP_MSG_MEM_SIZE;
	map_info.prot = IOMMU_READ | IOMMU_WRITE;
	if (hisp_mem_map_addr(type, &map_info, &mem_info) != 0) {
		pr_err("%s: hisp_alloc_cpu_map_addr failed", __func__);
		return -EINVAL;
	}
	g_isp_msg_mem_info.cpu_addr = mem_info.cpu_addr;
	g_isp_msg_mem_info.size = mem_info.cpu_size;
	return 0;
}

void hoisp_msg_mem_free(void)
{
	struct hisp_mem_type type = { 0 };
	struct hisp_mem_info_s mem_info = { 0 };

	if (g_isp_msg_mem_info.size != 0) {
		type.master = ISP_CPU | OISP_CPU;
		type.mem_type = ISP_PRIVATE_MEM;
		mem_info.cpu_addr = g_isp_msg_mem_info.cpu_addr;
		mem_info.cpu_size = g_isp_msg_mem_info.size;

		if(hisp_mem_unmap_addr(type, mem_info) != 0) {
			pr_err("%s: hisp_mem_unmap_addr failed", __func__);
			return;
		}
		if (hoisp_dev.msg_mem.mem_status == OISP_MEM_MAP) {
			hoisp_msg_mem_unmap();
		}
		g_isp_msg_mem_info.cpu_addr = 0;
		g_isp_msg_mem_info.size = 0;
	}
}

unsigned int hoisp_check_mem_ready(void)
{
	if (hoisp_dev.text_mem.mem_status != OISP_MEM_MAP) {
		return OISP_MEM_FALSE;
	}
	if (hoisp_dev.data_mem.mem_status != OISP_MEM_MAP) {
		return OISP_MEM_FALSE;
	}
	if (hoisp_dev.msg_mem.mem_status != OISP_MEM_MAP) {
		return OISP_MEM_FALSE;
	}
	if (hoisp_dev.share_mem.mem_status != OISP_MEM_MAP) {
		return OISP_MEM_FALSE;
	}
	return OISP_MEM_TRUE;
}

int hoisp_mem_status_init(struct sg_table *sg_table)
{
	void *msg_vaddr = NULL;
	int ret = 0;

	msg_vaddr = hoisp_mem_map_kernel(sg_table, hoisp_dev.msg_mem.mem_size);
	if (msg_vaddr == NULL) {
		pr_err("%s:map mem failed\n", __func__);
		return -EINVAL;
	}

	ret = memset_s(msg_vaddr, hoisp_dev.msg_mem.mem_size, 0,
		hoisp_dev.msg_mem.mem_size);
	if (ret != 0) {
		pr_err("%s:memset oisp msg failed\n", __func__);
	}

	hoisp_dev.mem_status = msg_vaddr;
	hoisp_dev.mem_status->is_mapped = hoisp_check_mem_ready();

	__flush_dcache_area(msg_vaddr, hoisp_dev.msg_mem.mem_size);
	return 0;
}

void hoisp_mem_status_deinit(void)
{
	hoisp_mem_unmap_kernel((void*)hoisp_dev.mem_status);
	hoisp_dev.mem_status = NULL;
}

void hoisp_mem_status_updata(void)
{
	if (hoisp_dev.mem_status == NULL) {
		pr_info("%s, hoisp_dev.mem_status is NULL\n", __func__);
		return;
	}
	hoisp_dev.mem_status->is_mapped = hoisp_check_mem_ready();
	hoisp_dev.mem_status->shared_addr = g_isp_shared_mem_info.cpu_addr;
	hoisp_dev.mem_status->size = g_isp_shared_mem_info.size;
	__flush_dcache_area((void*)hoisp_dev.mem_status, hoisp_dev.msg_mem.mem_size);
}

int hoisp_msg_mem_map(struct sg_table *sg_table)
{
	struct iommu_domain *domain = hoisp_dev.domain;
	size_t map_len;
	struct scatterlist *sgl =NULL;
	if (sg_table == NULL) {
		pr_err("%s, hoisp_dev.domain is NULL\n", __func__);
		return -EINVAL;
	}

	sgl = sg_table->sgl;
	if (sgl == NULL || domain == NULL) {
		pr_err("%s, hoisp_dev.domain is NULL\n", __func__);
		return -EINVAL;
	}

	map_len = iommu_map_sg(domain, hoisp_dev.msg_mem.mem_addr, sgl,
		(unsigned int)sg_nents(sgl), IOMMU_READ | IOMMU_WRITE);
	if (map_len != hoisp_dev.msg_mem.mem_size) {
		pr_err("%s: iommu_map_sg failed: map_len %lx len 0x%lx\n",
				__func__, map_len, hoisp_dev.msg_mem.mem_size);
		return -EINVAL;
	}
	hoisp_dev.msg_mem.mem_status = OISP_MEM_MAP;
	if (hoisp_mem_status_init(sg_table) != 0) {
		pr_err("%s, hoisp_mem_status_init failed\n", __func__);
		return -EINVAL;
	}
	pr_info("%s, hoisp msg mem map succ\n", __func__);
	return 0;
}

int hoisp_shared_mem_map(struct scatterlist *sgl, unsigned long isp_addr, unsigned long isp_size)
{
	struct iommu_domain *domain = hoisp_dev.domain;
	size_t map_len;

	if (domain == NULL) {
		pr_err("%s, hoisp_dev.domain is NULL\n", __func__);
		return -EINVAL;
	}

	map_len = iommu_map_sg(domain, hoisp_dev.share_mem.mem_addr, sgl,
		(unsigned int)sg_nents(sgl),
		IOMMU_READ | IOMMU_WRITE);
	if (map_len != hoisp_dev.share_mem.mem_size) {
		pr_err("%s: iommu_map_sg failed: map_len %lx len 0x%lx\n",
				__func__, map_len, hoisp_dev.share_mem.mem_size);
		return -EINVAL;
	}
	hoisp_dev.share_mem.mem_status = OISP_MEM_MAP;
	g_isp_shared_mem_info.cpu_addr = isp_addr;
	g_isp_shared_mem_info.size = isp_size;
	hoisp_mem_status_updata();
	pr_info("%s, hoisp shard mem map succ\n", __func__);
	return 0;
}

int hoisp_msg_mem_unmap(void)
{
	size_t unmap_len = 0;
	if (hoisp_dev.dev == NULL) {
		pr_err("%s, hoisp_dev.dev is NULL\n", __func__);
		return -EINVAL;
	}

	if (hoisp_dev.msg_mem.mem_status != OISP_MEM_MAP) {
		return 0;
	}

	if ((hoisp_dev.mem_status != NULL) && (hoisp_dev.mem_status->is_mapped == OISP_MEM_TRUE)) {
		__inval_dcache_area(hoisp_dev.mem_status, hoisp_dev.msg_mem.mem_size);
		if (hoisp_dev.mem_status->is_using == OISP_MEM_TRUE) {
			pr_err("%s, hoisp msg mem is using\n", __func__);
			return -EINVAL;
		}
	}

	unmap_len = mm_iommu_unmap_fast(hoisp_dev.dev, hoisp_dev.msg_mem.mem_addr,
		hoisp_dev.msg_mem.mem_size);
	if (unmap_len != hoisp_dev.msg_mem.mem_size) {
		pr_err("%s: iommu_unmap failed: unmap_len %lx len 0x%lx\n",
				__func__, unmap_len, hoisp_dev.msg_mem.mem_size);
		return -EINVAL;
	}
	hoisp_dev.msg_mem.mem_status = OISP_MEM_NULL;
	hoisp_mem_status_updata();
	hoisp_mem_status_deinit();
	pr_info("%s, hoisp msg mem unmap succ\n", __func__);
	return 0;
}

int hoisp_shared_mem_unmap(void)
{
	size_t unmap_len = 0;
	if (hoisp_dev.dev == NULL) {
		pr_err("%s, hoisp_dev.dev is NULL\n", __func__);
		return -EINVAL;
	}
	if (hoisp_dev.share_mem.mem_status != OISP_MEM_MAP &&
		hoisp_dev.share_mem.mem_status != OISP_MEM_UNMAP) {
		return 0;
	}

	if ((hoisp_dev.mem_status != NULL) && (hoisp_dev.mem_status->is_mapped == OISP_MEM_TRUE)) {
		__inval_dcache_area(hoisp_dev.mem_status, hoisp_dev.msg_mem.mem_size);
		if (hoisp_dev.mem_status->is_using == OISP_MEM_TRUE) {
			pr_err("%s, hoisp share mem is using\n", __func__);
			return -EINVAL;
		}
	}

	unmap_len = mm_iommu_unmap_fast(hoisp_dev.dev, hoisp_dev.share_mem.mem_addr,
		hoisp_dev.share_mem.mem_size);
	if (unmap_len != hoisp_dev.share_mem.mem_size) {
		pr_err("%s: iommu_unmap failed: unmap_len %lx len 0x%lx\n",
				__func__, unmap_len, hoisp_dev.share_mem.mem_size);
		return -EINVAL;
	}
	hoisp_dev.share_mem.mem_status = OISP_MEM_NULL;
	g_isp_shared_mem_info.cpu_addr = 0;
	g_isp_shared_mem_info.size = 0;
	hoisp_mem_status_updata();
	pr_info("%s, hoisp share mem unmap succ\n", __func__);
	return 0;
}

static inline unsigned int hoisp_order_to_size(unsigned int order)
{
	return PAGE_SIZE << order;
}

/* try to alloc largest page orders */
static struct page *hoisp_alloc_largest_pages(gfp_t gfp_mask, unsigned long size,
				unsigned int max_order, unsigned int *order)
{
	struct page *page   = NULL;
	gfp_t gfp_flags     = 0;
	unsigned long i     = 0;

	if (order == NULL) {
		pr_err("%s: order is NULL\n", __func__);
		return NULL;
	}

	for (i = 0; i < OISP_NUM_ORDERS; i++) {
		if ((unsigned int)size < hoisp_order_to_size(orders[i]))
			continue;
		if (max_order < orders[i])
			continue;

		if (orders[i] >= 8) {
			gfp_flags = gfp_mask & (~__GFP_RECLAIM);
			gfp_flags |= __GFP_NOWARN;
			gfp_flags |= __GFP_NORETRY;
		} else if (orders[i] >= 4) {
			gfp_flags = gfp_mask & (~__GFP_DIRECT_RECLAIM);
			gfp_flags |= __GFP_NOWARN;
			gfp_flags |= __GFP_NORETRY;
		} else {
			gfp_flags = gfp_mask;
		}

		gfp_flags |= __GFP_ZERO;
#ifdef CONFIG_MM_AMA_V2
		page = ama_alloc_bigbw_pages(gfp_flags, orders[i]);
#else
		page = alloc_pages(gfp_flags, orders[i]);
#endif
		if (page == NULL)
			continue;
		__inval_dcache_area(page_address(page), PAGE_SIZE * (1 << orders[i]));
		*order = orders[i];

		return page;
	}

	return NULL;
}

static int hoisp_alloc_mem_page(unsigned long length,
	struct list_head *pages_list, int *alloc_pages_num)
{
	struct page_info *page_info = NULL;
	unsigned int max_order = orders[0];
	long size_remaining = PAGE_ALIGN(length);

	INIT_LIST_HEAD(pages_list);

	while (size_remaining > 0) {
		page_info = (struct page_info *)kzalloc(sizeof(struct page_info),
			GFP_KERNEL);
		if (page_info == NULL)
			return -ENOMEM;

		page_info->page = hoisp_alloc_largest_pages(GFP_KERNEL,
			(unsigned long)size_remaining, max_order, &page_info->order);
		if (!page_info->page) {
			pr_err("%s: alloc largest pages failed!\n", __func__);
			kfree(page_info);
			return -ENOMEM;
		}

		list_add_tail(&page_info->node, pages_list);

		max_order = page_info->order;
		(*alloc_pages_num)++;
		size_remaining -= (PAGE_SIZE << page_info->order);
	}

	return 0;
}

static struct sg_table *hoisp_alloc_dynmem_sg_table(unsigned long length)
{
	struct list_head pages_list             = { 0 };
	struct page_info *page_info       = NULL;
	struct page_info *tmp_page_info   = NULL;
	struct sg_table *table       = NULL;
	struct scatterlist *sg       = NULL;
	int alloc_pages_num          = 0;
	int ret;

	INIT_LIST_HEAD(&pages_list);

	ret = hoisp_alloc_mem_page(length, &pages_list, &alloc_pages_num);
	if (ret < 0) {
		pr_err("%s: hisp_alloc_mem_page fail\n", __func__);
		goto free_pages;
	}

	table = (struct sg_table *)kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (table == NULL)
		goto free_pages;

	if (sg_alloc_table(table, (unsigned int)alloc_pages_num, GFP_KERNEL))
		goto free_table;

	sg = table->sgl;
	list_for_each_entry_safe(page_info, tmp_page_info, &pages_list, node) {
		sg_set_page(sg, page_info->page,
			PAGE_SIZE << page_info->order, 0);
		sg = sg_next(sg);
		list_del(&page_info->node);
		kfree(page_info);
	}
	pr_debug("%s: pages num.%d, length.0x%lx\n",
		__func__, alloc_pages_num, length);
	return table;

free_table:
	kfree(table);

free_pages:
	list_for_each_entry_safe(page_info, tmp_page_info, &pages_list, node) {
		__free_pages(page_info->page, page_info->order);
		list_del(&page_info->node);
		kfree(page_info);
	}

	return NULL;
}

static void hoisp_free_dynmem_sg_table(struct sg_table *table)
{
	struct scatterlist *sg  = NULL;
	unsigned int i          = 0;

	if (table == NULL) {
		pr_err("%s: table is NULL\n", __func__);
		return;
	}
	pr_debug("%s: table = 0x%pK\n", __func__, table);
	for_each_sg(table->sgl, sg, table->nents, i) {
		__free_pages(sg_page(sg), get_order((unsigned int)PAGE_ALIGN(sg->length)));
	}
	sg_free_table(table);
	kfree(table);
}

static void hoisp_get_alloced_page(struct sg_table *table,
	unsigned int npages, struct page **pages)
{
	unsigned int i, j;
	struct scatterlist *sg  = NULL;
	unsigned int npages_this_entry   = 0;
	struct page *page       = NULL;

	(void)npages;

	if ((table == NULL) || (pages == NULL)) {
		pr_err("%s: table or pages is NULL\n", __func__);
		return;
	}

	for_each_sg(table->sgl, sg, table->nents, i) {
		npages_this_entry = (unsigned int)(PAGE_ALIGN(sg->length) / PAGE_SIZE);
		page = sg_page(sg);
		for (j = 0; j < npages_this_entry; j++)
			*(pages++) = page++;
	}
}

static void *hoisp_mem_map_kernel(struct sg_table *table, unsigned long length)
{
	void *vaddr             = NULL;
	unsigned int npages = (unsigned int)(PAGE_ALIGN(length) / PAGE_SIZE);
	struct page **pages;

	pages = vmalloc(sizeof(struct page *) * npages);
	if (pages == NULL)
		return NULL;

	hoisp_get_alloced_page(table, npages, pages);

	vaddr = vmap(pages, npages, VM_MAP, PAGE_KERNEL);
	vfree(pages);
	if (vaddr == NULL) {
		pr_err("%s: vmap failed.\n", __func__);
		return NULL;
	}
	return vaddr;
}

static void hoisp_mem_unmap_kernel(const void *va)
{
	if (va == NULL) {
		pr_err("%s: va is NULL\n", __func__);
		return;
	}
	vunmap(va);
}

static int hoisp_load_text(const struct firmware *firmware_p)
{
	struct iommu_domain *domain = NULL;
	struct sg_table *text_table = NULL;
	void *text_vaddr = NULL;
	size_t map_len = 0;
	int ret = 0;

	domain = hoisp_dev.domain;
	if (domain == NULL) {
		pr_err("%s, hoisp_dev.domain is NULL\n", __func__);
		return -EINVAL;
	}

	text_table = hoisp_alloc_dynmem_sg_table(hoisp_dev.text_mem.mem_size);
	if (text_table == NULL) {
		pr_err("%s:alloc mem failed\n", __func__);
		return -EINVAL;
	}

	map_len = iommu_map_sg(domain, hoisp_dev.text_mem.mem_addr, text_table->sgl,
		(unsigned int)sg_nents(text_table->sgl),
		IOMMU_READ | IOMMU_WRITE | IOMMU_EXEC);
	if (map_len != hoisp_dev.text_mem.mem_size) {
		pr_err("%s: iommu_map_sg failed: map_len %lx len 0x%lx\n",
			__func__, map_len, hoisp_dev.text_mem.mem_size);
		goto free_table;
	}

	text_vaddr = hoisp_mem_map_kernel(text_table, hoisp_dev.text_mem.mem_size);
	if (text_vaddr == NULL) {
		pr_err("%s:map mem failed\n", __func__);
		goto unmap_text_addr;
	}

	ret = memcpy_s(text_vaddr, hoisp_dev.text_mem.mem_size, firmware_p->data,
		hoisp_dev.text_mem.mem_size);
	if (ret != 0) {
		pr_err("%s:copy oisp text failed\n", __func__);
		goto free_mapping;
	}
	__flush_dcache_area(text_vaddr, hoisp_dev.text_mem.mem_size);

	hoisp_mem_unmap_kernel(text_vaddr);
	text_vaddr = NULL;

#if OISP_USE_SC
	ret = lb_sg_attach(PID_OISP, text_table->sgl,
		(unsigned int)sg_nents(text_table->sgl));
	if (ret < 0) {
		pr_err("%s:map sc failed\n", __func__);
		goto free_mapping;
	}
#endif

	hoisp_dev.text_table = text_table;
	hoisp_dev.text_mem.mem_status = OISP_MEM_MAP;
	hoisp_mem_status_updata();
	return ret;

free_mapping:
	hoisp_mem_unmap_kernel(text_vaddr);

unmap_text_addr:
	if (hoisp_dev.dev != NULL) {
		mm_iommu_unmap_fast(hoisp_dev.dev, hoisp_dev.text_mem.mem_addr, hoisp_dev.text_mem.mem_size);
	} else {
		pr_err("%s, hoisp_dev.dev is NULL\n", __func__);
	}

free_table:
	hoisp_free_dynmem_sg_table(text_table);

	return -EINVAL;
}

static int hoisp_load_data(const struct firmware *firmware_p)
{
	struct sg_table *data_table;
	struct iommu_domain *domain = hoisp_dev.domain;
	void *data_vaddr = NULL;
	size_t map_len;
	int ret = 0;

	if (domain == NULL) {
		pr_err("%s, hoisp_dev.domain is NULL\n", __func__);
		return -EINVAL;
	}

	data_table = hoisp_alloc_dynmem_sg_table(hoisp_dev.data_mem.mem_size);
	if (data_table == NULL) {
		pr_err("%s:alloc mem failed\n", __func__);
		return -EINVAL;
	}

	map_len = iommu_map_sg(domain, hoisp_dev.data_mem.mem_addr, data_table->sgl,
		(unsigned int)sg_nents(data_table->sgl),
		IOMMU_READ | IOMMU_WRITE);
	if (map_len != hoisp_dev.data_mem.mem_size) {
		pr_err("%s: iommu_map_sg failed: map_len %lx len 0x%lx\n",
				__func__, map_len, hoisp_dev.data_mem.mem_size);
		goto free_mapping;
	}

	data_vaddr = hoisp_mem_map_kernel(data_table, hoisp_dev.data_mem.mem_size);
	if (data_vaddr == NULL) {
		pr_err("%s:map mem failed\n", __func__);
		goto free_mapping;
	}

	ret = memcpy_s(data_vaddr, hoisp_dev.data_mem.mem_size,
		firmware_p->data + hoisp_dev.text_mem.mem_size,
		firmware_p->size - hoisp_dev.text_mem.mem_size);
	if (ret != 0) {
		pr_err("%s:copy oisp text failed\n", __func__);
		return ret;
	}
	__flush_dcache_area(data_vaddr, hoisp_dev.data_mem.mem_size);

#if OISP_USE_SC
	ret = lb_sg_attach(PID_OISP, data_table->sgl,
		(unsigned int)sg_nents(data_table->sgl));
	if (ret < 0) {
		pr_err("%s:map sc failed\n", __func__);
		goto free_mapping;
	}
#endif

	hoisp_dev.data_table = data_table;
	hoisp_dev.data_mem.mem_status = OISP_MEM_MAP;
	hoisp_mem_status_updata();

free_mapping:
	hoisp_mem_unmap_kernel(data_vaddr);

	return ret;
}

static int hoisp_release_loadmem(struct device *dev, unsigned long iova, size_t size, struct sg_table **sg_table)
{
	size_t unmapped = 0;
	pr_info("hoisp_release_loadmem start\n");

	if (dev == NULL || sg_table == NULL) {
		pr_err("%s, dev or sg_table is NULL\n", __func__);
		return -EINVAL;
	}
	unmapped = mm_iommu_unmap_fast(dev, iova, size);
	if (unmapped != size) {
		pr_err("%s, unmap fail, len 0x%lx\n", __func__, unmapped);
		return -EINVAL;
	}

	if (*sg_table != NULL) {
#if OISP_USE_SC
		(void)lb_sg_detach(PID_OISP, (*sg_table)->sgl, (unsigned int)sg_nents((*sg_table)->sgl));
#endif
		hoisp_free_dynmem_sg_table(*sg_table);
		*sg_table = NULL;
	}
	pr_info("hoisp_release_loadmem end\n");

	return 0;
}

void hoisp_pre_deinit(void)
{
	if (hoisp_dev.share_mem.mem_status != OISP_MEM_NULL) {
		hoisp_dev.share_mem.mem_status = OISP_MEM_UNMAP;
		hoisp_mem_status_updata();
		(void)hoisp_shared_mem_unmap();
	}
}

int hoisp_start_prepare(void)
{
	const struct firmware *firmware_p;
	struct device *dev = hoisp_dev.dev;
	int ret = 0;

	pr_info("hoisp_start_prepare start\n");

	if (atomic_read(&hoisp_dev.oisp_prepare_status) > 0) {
		pr_err("[%s] hoisp had been prepared\n", __func__);
		pr_err("[%s] oisp_prepare_status.0x%x\n", __func__,
				atomic_read(&hoisp_dev.oisp_prepare_status));
		return -ENODEV;
	}

	/* load firmware */
	ret = request_firmware(&firmware_p, hoisp_dev.firmware, dev);
	if (ret < 0) {
		dev_err(dev, "request_firmware failed: %d\n", ret);
		return ret;
	}

	if (firmware_p->size <= hoisp_dev.text_mem.mem_size) {
		dev_err(dev, "firmware size error : %d\n", firmware_p->size);
		goto ERR_RET;
	}

	ret = hoisp_load_text(firmware_p);
	if (ret < 0) {
		dev_err(dev, "load text failed : %d\n", ret);
		goto ERR_RET;
	}

	ret = hoisp_load_data(firmware_p);
	if (ret < 0) {
		dev_err(dev, "load data failed : %d\n", ret);
		goto FREE_TEXT;
	}

	ret = hoisp_msg_mem_alloc();
	if (ret < 0) {
		dev_err(dev, "alloc msg mem failed : %d\n", ret);
		goto FREE_DATA;
	}

	if (hoisp_dev.pinctrls != NULL && hoisp_dev.pinctrls_default != NULL) {
		ret = pinctrl_select_state(hoisp_dev.pinctrls,
			hoisp_dev.pinctrls_default);
		if (ret) {
			dev_err(dev, "could not set default pins : %d\n", ret);
			goto FREE_MSG;
		}
	} else {
		dev_err(dev, "pinctrls is null\n");
		goto FREE_MSG;
	}

	atomic_set(&hoisp_dev.oisp_prepare_status, 1);
	release_firmware(firmware_p);
	pr_info("hoisp_start_prepare end\n");
	return 0;
FREE_MSG:
	hoisp_msg_mem_free();
FREE_DATA:
	hoisp_release_loadmem(hoisp_dev.dev, hoisp_dev.data_mem.mem_addr,
		hoisp_dev.data_mem.mem_size, &hoisp_dev.data_table);
	hoisp_dev.data_mem.mem_status = OISP_MEM_NULL;
FREE_TEXT:
	hoisp_release_loadmem(hoisp_dev.dev, hoisp_dev.text_mem.mem_addr,
		hoisp_dev.text_mem.mem_size, &hoisp_dev.text_table);
	hoisp_dev.text_mem.mem_status = OISP_MEM_NULL;
ERR_RET:
	release_firmware(firmware_p);
	pr_info("hoisp_start_prepare error end\n");
	return ret;
}
EXPORT_SYMBOL(hoisp_start_prepare);

static int hoisp_release_dynmem(void)
{
	int retFreeText, retFreeData;
	int ret = 0;
	pr_info("hoisp_release_dynmem start\n");

	if (hoisp_dev.dev == NULL) {
		pr_err("%s, hoisp_dev.dev is NULL\n", __func__);
		return -EINVAL;
	}

	hoisp_msg_mem_free();

	retFreeData = hoisp_release_loadmem(hoisp_dev.dev, hoisp_dev.data_mem.mem_addr,
		hoisp_dev.data_mem.mem_size, &hoisp_dev.data_table);
	if (retFreeData != 0) {
		pr_err("%s, hoisp release data failed\n", __func__);
		ret = -EINVAL;
	}
	hoisp_dev.data_mem.mem_status = OISP_MEM_NULL;
	hoisp_mem_status_updata();

	retFreeText = hoisp_release_loadmem(hoisp_dev.dev, hoisp_dev.text_mem.mem_addr,
		hoisp_dev.text_mem.mem_size, &hoisp_dev.text_table);
	if (retFreeText != 0) {
		pr_err("%s, hoisp release text failed\n", __func__);
		ret = -EINVAL;
	}
	hoisp_dev.text_mem.mem_status = OISP_MEM_NULL;
	hoisp_mem_status_updata();

	pr_info("hoisp_release_dynmem end\n");

	return ret;
}

int hoisp_stop_cleanup(void)
{
	int ret;

	pr_info("hoisp_stop_cleanup start\n");

	if (atomic_read(&hoisp_dev.oisp_prepare_status) == 0) {
		pr_err("[%s] hoisp had not been prepared\n", __func__);
		return -ENODEV;
	}

	hoisp_pre_deinit();
	ret = hoisp_release_dynmem();

	if (hoisp_dev.pinctrls != NULL && hoisp_dev.pinctrls_idle != NULL) {
		ret = pinctrl_select_state(hoisp_dev.pinctrls,
			hoisp_dev.pinctrls_idle);
		if (ret) {
			pr_err("could not set idle pins : %d\n", ret);
		}
	} else {
		pr_err("pinctrls is null\n");
	}

	atomic_set(&hoisp_dev.oisp_prepare_status, 0);

	pr_info("hoisp_stop_cleanup end\n");

	return ret;
}
EXPORT_SYMBOL(hoisp_stop_cleanup);

static int hoisp_dev_getdts(struct device *pdev,
			struct hoisp_device *dev)
{
	struct device_node *np = pdev->of_node;
	int ret;

	ret = of_property_read_string(np, "oisp_bin_name", &dev->firmware);
	if (ret < 0)
		return -EINVAL;

	ret = of_property_read_u32(np, "oisp_text_addr", &dev->text_mem.mem_addr);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_text_addr ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_text_size", &dev->text_mem.mem_size);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_boot_size ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_data_addr", &dev->data_mem.mem_addr);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_data_addr ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_data_size", &dev->data_mem.mem_size);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_data_size ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_share_mem_addr", &dev->share_mem.mem_addr);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_share_mem_addr ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_share_mem_size", &dev->share_mem.mem_size);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_share_mem_size ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_msg_size", &dev->msg_mem.mem_size);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_msg_size ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	ret = of_property_read_u32(np, "oisp_msg_addr", &dev->msg_mem.mem_addr);
	if (ret < 0) {
		pr_err("[%s] Failed: get oisp_msg_addr ret.%d\n",
			__func__, ret);
		return -EINVAL;
	}

	dev->pinctrls = devm_pinctrl_get(pdev);
	if (dev->pinctrls == NULL) {
		pr_err("[%s] Failed: get pinctrls failed.\n", __func__);
		return -EINVAL;
	}

	dev->pinctrls_default = pinctrl_lookup_state(dev->pinctrls,
		PINCTRL_STATE_DEFAULT);
	if (dev->pinctrls_default == NULL) {
		pr_err("[%s] Failed: get pinctrls_default failed.\n", __func__);
		return -EINVAL;
	}

	dev->pinctrls_idle = pinctrl_lookup_state(dev->pinctrls,
		PINCTRL_STATE_IDLE);
	if (dev->pinctrls_idle == NULL) {
		pr_err("[%s] Failed: get pinctrls_idle failed.\n", __func__);
		return -EINVAL;
	}

	return ret;
}

static int hoisp_device_probe(struct platform_device *pdev)
{
	int ret;
	struct hoisp_device *dev = &hoisp_dev;

	pr_info("hoisp_device_probe start\n");

	hoisp_dev.text_table = NULL;
	hoisp_dev.text_mem.mem_status = OISP_MEM_NULL;
	hoisp_dev.data_table = NULL;
	hoisp_dev.data_mem.mem_status = OISP_MEM_NULL;
	hoisp_dev.msg_mem.mem_status = OISP_MEM_NULL;
	hoisp_dev.share_mem.mem_status = OISP_MEM_NULL;

	ret = hoisp_dev_getdts(&pdev->dev, dev);
	if (ret < 0)
		return 0;

	hoisp_dev.domain = iommu_get_domain_for_dev(&pdev->dev);
	hoisp_dev.dev = &pdev->dev;

	if (hoisp_dev.domain == NULL) {
		pr_err("%s, hoisp_dev.domain is NULL\n", __func__);
		return -EINVAL;
	}

	atomic_set(&hoisp_dev.oisp_prepare_status, 0);

	pr_info("hoisp_device_probe end\n");

	return ret;
}

static int hoisp_device_remove(struct platform_device *pdev)
{
	(void)pdev;
	return 0;
}

static const struct of_device_id hoisp_of_match[] = {
	{ .compatible = "hisilicon,oisp"},
	{ },
};
MODULE_DEVICE_TABLE(of, hoisp_of_match);

static struct platform_driver hoisp_driver = {
	.driver = {
		.owner      = THIS_MODULE,
		.name       = "oisp",
		.of_match_table = of_match_ptr(hoisp_of_match),
	},
	.probe  = hoisp_device_probe,
	.remove = hoisp_device_remove,
};
module_platform_driver(hoisp_driver);
MODULE_LICENSE("GPL v2");

