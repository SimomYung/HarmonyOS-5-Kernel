// SPDX-License-Identifier: GPL-2.0-only
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/mman.h>		/* for PROT_{READ|WRITE|EXEC} */
#include <linux/io.h>		/* fro phys_to_virt() */
#include <linux/kmemleak.h>

#include <asm/uaccess.h>
#include "mm_internal.h"

static bool is_kmap_addr(const void *x)
{
	unsigned long addr = (unsigned long)x;
	unsigned long k_start = (unsigned long)phys_to_virt(PHYS_OFFSET);
	unsigned long k_end = k_start + (1UL << CONFIG_LIBLINUX_KMAP_BITS);

	return addr >= k_start && addr < k_end;
}

bool is_vmalloc_addr(const void *x)
{
	return !is_kmap_addr(x);
}
EXPORT_SYMBOL(is_vmalloc_addr);

void __weak arch_module_vmalloc_init(void) { }

void __init vmalloc_init(void)
{
	arch_module_vmalloc_init();
}

unsigned long vmalloc_nr_pages(void)
{
	return liblinux_pal_vmalloc_nr_pages();
}

static int pgprot_to_vmalloc_prot(pgprot_t pgprot, unsigned long *prot)
{
	if (pgprot_val(pgprot) == pgprot_val(PAGE_KERNEL)) {
		*prot = PROT_READ | PROT_WRITE;
	} else if (pgprot_val(pgprot) == pgprot_val(PAGE_KERNEL_EXEC)) {
		*prot = PROT_READ | PROT_WRITE | PROT_EXEC;
	} else if (pgprot_val(pgprot) == pgprot_val(PAGE_KERNEL_ROX)) {
		*prot = PROT_READ | PROT_EXEC;
	} else {
		return -1;
	}

	return 0;
}

static unsigned long vmflags_to_palflags(unsigned long vm_flags)
{
	if ((vm_flags & VM_USERMAP) != 0) {
		return LIBLINUX_PAL_VM_USERMAP;
	}
	return 0UL;
}

void *__vmalloc_node_range(unsigned long size, unsigned long align,
		unsigned long start, unsigned long end, gfp_t gfp_mask,
		pgprot_t pgprot, unsigned long vm_flags, int node,
		const void *caller)
{
	unsigned long prot;
	void *ptr = NULL;
	int err = pgprot_to_vmalloc_prot(pgprot, &prot);
	unsigned long pal_vm_flags = vmflags_to_palflags(vm_flags);
	unsigned long real_size = size;

	if (err != 0) {
		printk("~~ UN-IMPL ~~: %s: pgprot=0x%llx\n", __func__,
		       (unsigned long long)pgprot_val(pgprot));
		return NULL;
	}

	size = PAGE_ALIGN(size);
	if (!size || (size >> PAGE_SHIFT) > totalram_pages()) {
		warn_alloc(gfp_mask, NULL,
			   "vmalloc size %lu allocation failure", size);
		return NULL;
	}

	ptr = liblinux_pal_vmalloc_range_ex(real_size, align, start, end, prot, pal_vm_flags, caller);
	if (ptr == NULL)
		return NULL;

	if (gfp_mask & __GFP_ZERO)
		memset(ptr, 0, real_size);

	kmemleak_alloc(ptr, size, 1, gfp_mask);
	return ptr;
}

struct page *vmalloc_to_page(const void *vmalloc_addr)
{
	unsigned long long pa;

	int ret = liblinux_va_to_pa(PAGE_DOWN((unsigned long)vmalloc_addr), &pa);
	if (ret) {
		return NULL;
	}
	/*
	 * This is a temporary solution, correct solution should prepare page
	 * struct when doing vmalloc * and unprepare page struct when doing
	 * vfree.
	 * However, since we disable vmemmap DPA (for memory accounting at
	 * start time) and disable unpopulate (for performance), leaving the
	 * page struct "leaked" is not a real problem.
	 * Vmalloc_to_page can be used for vmap address, vmapped address has
	 * associated page array.
	 */
	if (!pfn_valid(phys_to_pfn(pa)))
		__prepare_pages_info(phys_to_virt(pa), PAGE_SIZE, ZONE_IOFAST);
	return phys_to_page(pa);
}
EXPORT_SYMBOL(vmalloc_to_page);

void vfree(const void *addr)
{
	if (!addr)
		return;

	kmemleak_free(addr);
	(void)liblinux_pal_vm_unmap(addr);
}
EXPORT_SYMBOL(vfree);

void *vmalloc_exec(unsigned long size)
{
	return __vmalloc_node_range(size, 1, VMALLOC_START, VMALLOC_END,
			GFP_KERNEL | __GFP_HIGHMEM, PAGE_KERNEL_EXEC, 0, NUMA_NO_NODE,
			__builtin_return_address(0));
}

void *__vmalloc_node(unsigned long size, unsigned long align, gfp_t gfp_mask,
		     int node, const void *caller)
{
	return __vmalloc_node_range(size, align, VMALLOC_START, VMALLOC_END,
				gfp_mask, PAGE_KERNEL, 0, node, caller);
}

static inline void *__vmalloc_node_flags(unsigned long size,
					 int node, gfp_t flags)
{
	return __vmalloc_node(size, 1, flags, node, __builtin_return_address(0));
}

void *__vmalloc(unsigned long size, gfp_t gfp_mask)
{
	return __vmalloc_node(size, 1, gfp_mask, NUMA_NO_NODE, __builtin_return_address(0));
}
EXPORT_SYMBOL(__vmalloc);

void *vmalloc(unsigned long size)
{
	return __vmalloc_node_flags(size, NUMA_NO_NODE,
				    GFP_KERNEL | __GFP_HIGHMEM);
}
EXPORT_SYMBOL(vmalloc);

void *vzalloc(unsigned long size)
{
	return __vmalloc_node_flags(size, NUMA_NO_NODE,
				GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO);
}
EXPORT_SYMBOL(vzalloc);

void *vmalloc_node(unsigned long size, int node)
{
	return __vmalloc_node(size, 1, GFP_KERNEL | __GFP_HIGHMEM,
			      node, __builtin_return_address(0));
}
EXPORT_SYMBOL(vmalloc_node);

void *vzalloc_node(unsigned long size, int node)
{
	return __vmalloc_node_flags(size, node,
			 GFP_KERNEL | __GFP_HIGHMEM | __GFP_ZERO);
}
EXPORT_SYMBOL(vzalloc_node);

#define MAX_VMAP_PA_ARRAY	128
int do_vmap(void *vaddr, struct page **pages, unsigned int count,
		   unsigned int flag, unsigned long prot)
{
	int ret = 0;
	unsigned int i;
	unsigned int index = 0;
	unsigned int page_cnt = 0;
	uintptr_t addr = (uintptr_t)vaddr;
	struct pfn_range pa_arr[MAX_VMAP_PA_ARRAY];
	unsigned long gid = 0;
#ifdef CONFIG_MM_LB
#ifdef CONFIG_MM_LB_GID_PBHA
	gid = (flag & LIBLINUX_PAL_REMAP_PBHA) >> LIBLINUX_PAL_REMAP_PBHA_SHIFT;
#else
	gid = PROT_GID(prot);
#endif
#endif

	/* PBHA not support single page map */
	if (count == 1 && (!IS_ENABLED(CONFIG_MM_LB_GID_PBHA) || gid == 0))
#if defined(CONFIG_MM_LB) && !defined(CONFIG_MM_LB_GID_PBHA)
		return liblinux_pal_vm_mmap(page_to_phys_lb(pages[0], gid),
					   (uintptr_t)vaddr,
					   PAGE_SIZE, prot, flag);
#else
		return liblinux_pal_vm_mmap(page_to_phys(pages[0]),
					   (uintptr_t)vaddr,
					   PAGE_SIZE, prot, flag);
#endif

	for (i = 0; i < count; i++) {
#if defined(CONFIG_MM_LB) && !defined(CONFIG_MM_LB_GID_PBHA)
		if (index > 0 && pa_arr[index - 1].end == page_to_pfn_lb(pages[i], gid)) {
#else
		if (index > 0 && pa_arr[index - 1].end == page_to_pfn(pages[i])) {
#endif
			pa_arr[index - 1].end++;
			page_cnt++;
			continue;
		}
		if (index >= MAX_VMAP_PA_ARRAY) {
			ret = liblinux_pal_vm_mmap_batch(pa_arr, MAX_VMAP_PA_ARRAY, addr,
							 prot, flag);
			if (ret < 0)
				return ret;
			addr += page_cnt << PAGE_SHIFT;
			page_cnt = 0;
			index = 0;
		}

#if defined(CONFIG_MM_LB) && !defined(CONFIG_MM_LB_GID_PBHA)
		pa_arr[index].start = page_to_pfn_lb(pages[i], gid);
#else
		pa_arr[index].start = page_to_pfn(pages[i]);
#endif
		pa_arr[index].end = pa_arr[index].start + 1;
		page_cnt++;
		index++;
	}

	return liblinux_pal_vm_mmap_batch(pa_arr, index, addr,
					 prot, flag);
}

void *vmap(struct page **pages, unsigned int count,
	   unsigned long flags, pgprot_t pgprot)
{
	int ret;
	unsigned int i;
	void *vaddr = NULL;
	unsigned long size;
	unsigned long prot;
	unsigned int flag;
#if defined(CONFIG_MM_LB) || defined(CONFIG_MM_LB_GID_PBHA)
	unsigned int gid;
#endif
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	unsigned int sh;
#endif

	prot = __pgprot_to_posix_prot(pgprot);
#if defined(CONFIG_MM_LB) && !defined(CONFIG_MM_LB_GID_PBHA)
	gid = PGPROT_GID(pgprot_val(pgprot));
	if (gid)
		prot |= PROT_LB(gid);
#endif

	flag = LIBLINUX_PAL_REMAP_CACHE;
	if (pgprot_val(pgprot_writecombine(pgprot)) == pgprot_val(pgprot)) {
		flag = LIBLINUX_PAL_REMAP_NORMAL_NC;
	}

#ifdef CONFIG_MM_LB_GID_PBHA
	gid = PGPROT_GID(pgprot_val(pgprot));
	if (gid)
		flag |= ((gid << LIBLINUX_PAL_REMAP_PBHA_SHIFT) & LIBLINUX_PAL_REMAP_PBHA);
#endif
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	sh = PGPROT_SH(pgprot_val(pgprot));
	if (sh)
		flag |= ((sh << LIBLINUX_PAL_REMAP_SH_SHIFT) & LIBLINUX_PAL_REMAP_SH);
#endif

	flag |= vmflags_to_palflags(flags);

	size = (unsigned long)count << PAGE_SHIFT;
	vaddr = liblinux_pal_vm_prepare(0, size, prot, flag);
	if (vaddr == NULL) {
		return NULL;
	}

	ret = do_vmap(vaddr, pages, count, flag, prot);
	if (ret < 0) {
		(void)liblinux_pal_vm_unmap(vaddr);
		return NULL;
	}

	return vaddr;
}
EXPORT_SYMBOL(vmap);

void vunmap(const void *addr)
{
	(void)liblinux_pal_vm_unmap(addr);
}
EXPORT_SYMBOL(vunmap);

void *vmalloc_user(unsigned long size)
{
	return __vmalloc_node_range(size, SHMLBA, VMALLOC_START, VMALLOC_END,
				    GFP_KERNEL | __GFP_ZERO, PAGE_KERNEL,
				    VM_USERMAP, NUMA_NO_NODE,
				    __builtin_return_address(0));
}
EXPORT_SYMBOL(vmalloc_user);

int remap_vmalloc_range(struct vm_area_struct *vma, void *addr,
						unsigned long pgoff)
{
	return remap_vmalloc_range_partial(vma, vma->vm_start, addr, pgoff,
		(vma->vm_end - vma->vm_start));
}
EXPORT_SYMBOL(remap_vmalloc_range);

int remap_vmalloc_range_partial(struct vm_area_struct *vma,
    unsigned long uaddr, void *kaddr, unsigned long pgoff, unsigned long size)
{
	unsigned long off;
	unsigned long end_index;

	size = PAGE_ALIGN(size);
	if (!(PAGE_ALIGNED(kaddr)) || !(PAGE_ALIGNED(uaddr))) {
		pr_warn("liblinux: %s(PAGE_ALIGNED failed)\n", __func__);
		return -EINVAL;
	}
	if (check_shl_overflow(pgoff, PAGE_SHIFT, &off) || check_add_overflow(size, off, &end_index)) {
		pr_warn("liblinux: %s(check size fail pgoff=0x%lx, size 0x%lx)\n",
			__func__, pgoff, size);
		return -EINVAL;
	}
	if (liblinux_pal_check_vmalloc_size(kaddr, end_index) != 0) {
		return -EINVAL;
	}

	kaddr += off;
	return liblinux_pal_remap_vmalloc_range(uaddr, (unsigned long)kaddr, size,
		vm_flags_to_prot(vma->vm_flags), pgprot_to_liblinux_flags(vma->vm_page_prot));
}
EXPORT_SYMBOL(remap_vmalloc_range_partial);

void __weak vmalloc_sync_all(void)
{
}
