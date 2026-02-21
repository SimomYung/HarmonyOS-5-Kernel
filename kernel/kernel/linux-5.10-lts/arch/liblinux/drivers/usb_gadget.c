/* SPDX-License-Identifier: GPL-2.0-only */
#include <linux/mm.h>
#include <linux/scatterlist.h>
#include <linux/vmalloc.h>
#include <trace/hooks/liblinux.h>

static void hook_ffs_build_sg_list(void *__data, struct sg_table *sgt, size_t sz, void **raddr)
{
	struct page **pages;
	unsigned int n_pages;
	void *vaddr = NULL;
	int i;

	*raddr = NULL;
	n_pages = PAGE_ALIGN(sz) >> PAGE_SHIFT;
	pages = kvmalloc_array(n_pages, sizeof(struct page *), GFP_KERNEL);
	if (!pages) {
		pr_err("ffs_build_sg_list: kvmalloc_array failed\n");
		return;
	}
	for (i = 0; i < n_pages; i++) {
		pages[i] = alloc_page(GFP_KERNEL);
		if (pages[i] == NULL) {
			pr_err("ffs_build_sg_list: alloc_pages failed\n");
			n_pages = i;
			goto free_pg;
		}
	}

	vaddr = vmap(pages, n_pages, VM_MAP, PAGE_KERNEL);
	if (!vaddr) {
		pr_err("ffs_build_sg_list: vmap failed\n");
		goto free_pg;
	}

	if (sg_alloc_table_from_pages(sgt, pages, n_pages, 0, sz, GFP_KERNEL)) {
		pr_err("ffs_build_sg_list: sg_alloc_table_from_pages failed\n");
		vunmap(vaddr);
		goto free_pg;
	}

	kvfree(pages);

	*raddr = vaddr;
	return;

free_pg:
	for (i = 0; i < n_pages; i++) {
		__free_page(pages[i]);
	}
	kvfree(pages);
}
INIT_VENDOR_HOOK(ldk_rvh_ffs_build_sg_list, hook_ffs_build_sg_list);

static void hook_ffs_free_buffer(void *__data, void *vaddr, struct sg_table *sgt)
{
	struct sg_page_iter piter;
	struct page *p;

	vunmap(vaddr);
	for_each_sgtable_page(sgt, &piter, 0) {
		p = sg_page_iter_page(&piter);
		__free_page(p);
	}
	sg_free_table(sgt);
}
INIT_VENDOR_HOOK(ldk_rvh_ffs_free_buffer, hook_ffs_free_buffer);
