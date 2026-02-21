// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/vmalloc.h>

#include <trace/hooks/liblinux.h>

static void hook_sk_vm_insert_page(void *args, struct vm_area_struct *vma,
				  unsigned long addr, unsigned long kaddr, struct page *page, int *ret)
{
	unsigned long pfn;

	if (is_vmalloc_addr(kaddr)) {
		*ret = remap_vmalloc_range_partial(vma, addr, kaddr, 0, PAGE_SIZE);
	} else {
		pfn = page_to_pfn(page);
		*ret = remap_pfn_range(vma, addr, pfn, PAGE_SIZE, vma->vm_page_prot);
	}
}

INIT_VENDOR_HOOK(ldk_rvh_sk_vm_insert_page, hook_sk_vm_insert_page);
