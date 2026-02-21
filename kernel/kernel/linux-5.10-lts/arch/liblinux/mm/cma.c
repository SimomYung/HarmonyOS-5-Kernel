// SPDX-License-Identifier: GPL-2.0-or-later
#include <linux/memblock.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/cma.h>
#include <linux/io.h>

#include "../../../mm/cma.h"
#include <trace/hooks/liblinux.h>

static void hook_cma_add_section(void *args, unsigned long pfn, unsigned long count)
{
	int ret = 0;
	unsigned long cur_nr_pages;
	unsigned long end_pfn = pfn + count;

	for (; pfn < end_pfn; pfn += cur_nr_pages) {
		cur_nr_pages = min(end_pfn - pfn,
				   SECTION_ALIGN_UP(pfn + 1) - pfn);
		ret = sparse_add_section(0, pfn, cur_nr_pages, NULL);
		if (ret < 0)
			pr_err("cma add section failed. pfn=0x%lx count=0x%llx\n", pfn, count);
	}
}
INIT_VENDOR_HOOK(ldk_vh_cma_add_section, hook_cma_add_section);

static void hook_cma_alloc(void *args, unsigned long *bitmap_no, const struct cma *cma,
			   size_t count, unsigned int align_order, int *ret)
{
	unsigned long paddr;
	unsigned long bitmap_tmp = -1;
	unsigned int cma_id = cma - &cma_areas[0];

	*ret = liblinux_pal_cma_alloc(cma_id, count << PAGE_SHIFT, align_order, &paddr);
	if (*ret) {
		pr_err("cma alloc failed ret:%d cm_id %d align %u\n",
		       *ret, cma_id, align_order);
		return;
	}

	bitmap_tmp = ((__phys_to_pfn(paddr) - cma->base_pfn) >> cma->order_per_bit);
	if (bitmap_tmp != *bitmap_no) {
		pr_err("cma%u alloc bitmap is different. bitmap_no[0x%lx] bitmap_tmp[0x%lx]",
			cma_id, *bitmap_no, bitmap_tmp);
		*bitmap_no = bitmap_tmp;
	}
}
INIT_VENDOR_HOOK(ldk_vh_cma_alloc, hook_cma_alloc);

static void hook_cma_release(void *args, const struct cma *cma,
			     const struct page *pages, unsigned int count)
{
	unsigned int cma_id = cma - &cma_areas[0];

	liblinux_pal_cma_release(cma_id, page_to_virt(pages), page_to_phys(pages),
				 count << PAGE_SHIFT);
}
INIT_VENDOR_HOOK(ldk_vh_cma_release, hook_cma_release);
