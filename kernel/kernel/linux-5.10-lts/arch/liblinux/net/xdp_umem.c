// SPDX-License-Identifier: GPL-2.0-only
#include <asm/page.h>
#include <asm/io.h>
#include <liblinux/pal.h>
#include <linux/export.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <trace/hooks/liblinux.h>

static void hook_xdp_pin_user_pages(void *args, unsigned long start, unsigned long nr_pages,
		unsigned int gup_flags, struct page **pages, struct vm_area_struct **vmas, long *ret)
{
	unsigned long long size = nr_pages * PAGE_SIZE;
	size_t idx, array_size = nr_pages;
	unsigned long long *pa_array;
	struct page *page;

	if (!start) {
		*ret = -EFAULT;
		return;
	}

	pa_array = kcalloc(array_size, sizeof(*pa_array), GFP_KERNEL);
	if(!pa_array) {
		*ret = -ENOMEM;
		return;
	}
	*ret = liblinux_pal_get_user_pages(start, size, pa_array, array_size);
	if (*ret) {
		kfree(pa_array);
		return;
	}
	for(idx = 0; idx < array_size; idx++) {
		if (pa_array[idx] == 0)
			break;

		pages[idx] = (struct page *)phys_to_page(pa_array[idx]);
		page_ref_add(pages[idx], GUP_PIN_COUNTING_BIAS);
	}
	kfree(pa_array);
	*ret = idx;
}

INIT_VENDOR_HOOK(ldk_rvh_xdp_pin_user_pages, hook_xdp_pin_user_pages);

static void hook_xdp_unpin_user_pages(void *args, struct page **pages,
				unsigned long npages, bool make_dirty)
{
	unsigned long long *pa_array = NULL;
	int refs = GUP_PIN_COUNTING_BIAS;
	size_t array_size = npages;
	unsigned long long addr;
	struct page *page;

	pa_array = kcalloc(array_size, sizeof(*pa_array), GFP_KERNEL);
	if (WARN_ON(!pa_array))
		return;

	for (unsigned int idx = 0; idx < array_size; idx++) {
		pa_array[idx] = page_to_phys(pages[idx]);
		if (WARN_ON(page_ref_count(pages[idx]) < refs)) {
			kfree(pa_array);
			return;
		}
		if (pa_array[idx]) {
			(void)liblinux_pal_put_user_pages(0, PAGE_SIZE, &pa_array[idx], 1);
		}
		page_ref_sub(pages[idx], refs);
		pages[idx] = phys_to_page(0);
	}
	kfree(pa_array);
}

INIT_VENDOR_HOOK(ldk_rvh_xdp_unpin_user_pages, hook_xdp_unpin_user_pages);
