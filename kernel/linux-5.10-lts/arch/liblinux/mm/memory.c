// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mm_types.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/pfn_t.h>
#include <linux/slab.h>
#include <linux/dma-buf.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <asm/compiler.h>
#include <asm/pgalloc.h>
#include "mm_internal.h"

static bool _check_vma_mm_valid(struct vm_area_struct *vma)
{
	if (vma == NULL) {
		pr_err("vma is null\n");
		return false;
	}
	if (vma->vm_mm == NULL || vma->vm_mm == &init_mm) {
		pr_err("the vma is invalid\n");
		return false;
	}
	if (VSPACE_KEY(vma->vm_mm) == 0 || VSPACE_KEY(vma->vm_mm) == (unsigned long long)(-1)) {
		pr_err("invalid mm key\n");
		return false;
	}
	return true;
}

int remap_pfn_range_batch(struct vm_area_struct *vma, unsigned long addr,
			  const struct pfn_range *array, unsigned long cnt, pgprot_t pgprot)
{
	unsigned int flags = LIBLINUX_PAL_REMAP_CACHE;
#ifdef CONFIG_MM_LB_GID_PBHA
	unsigned int gid;
#endif
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	unsigned int sh;
#endif
	unsigned long prot = __pgprot_to_posix_prot(pgprot);

	if (!_check_vma_mm_valid(vma))
		return -EINVAL;
	if (pgprot_val(pgprot_writecombine(pgprot)) == pgprot_val(pgprot)) {
		flags = LIBLINUX_PAL_REMAP_NORMAL_NC;
	}
	if ((pgprot_val(pgprot_device(pgprot)) == pgprot_val(pgprot)) ||
	    (pgprot_val(pgprot_noncached(pgprot)) == pgprot_val(pgprot))) {
		flags = LIBLINUX_PAL_REMAP_DEVICE;
	}

	if ((vma != NULL) && (vma->vm_file != NULL) &&
		is_dma_buf_file(vma->vm_file) &&
		!is_direct_reclaim_dmabuf(vma->vm_file->private_data))
		flags |= LIBLINUX_PAL_REMAP_DMA_BUF;

#ifdef CONFIG_MM_LB_GID_PBHA
	gid = PGPROT_GID(pgprot_val(pgprot));
	if (gid)
		flags |= ((gid << LIBLINUX_PAL_REMAP_PBHA_SHIFT) & LIBLINUX_PAL_REMAP_PBHA);
#endif
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	sh = PGPROT_SH(pgprot_val(pgprot));
	if (sh)
		flags |= ((sh << LIBLINUX_PAL_REMAP_SH_SHIFT) & LIBLINUX_PAL_REMAP_SH);
#endif
	return liblinux_pal_remap_pa_range_batch_mm(addr, array, cnt, prot, flags, VSPACE_KEY(vma->vm_mm));
}

int remap_pfn_range(struct vm_area_struct *vma, unsigned long addr,
		    unsigned long pfn, unsigned long size, pgprot_t pgprot)
{
	unsigned int flags = LIBLINUX_PAL_REMAP_CACHE;
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	unsigned int sh;
#endif
	unsigned long prot = __pgprot_to_posix_prot(pgprot);
	struct pfn_range pfn_arr = { pfn, pfn + (PAGE_ALIGN(size) / PAGE_SIZE) };

	if (!_check_vma_mm_valid(vma))
		return -EINVAL;
	if (pgprot_val(pgprot_writecombine(pgprot)) == pgprot_val(pgprot)) {
		flags = LIBLINUX_PAL_REMAP_NORMAL_NC;
	}
	if ((pgprot_val(pgprot_device(pgprot)) == pgprot_val(pgprot)) ||
	    (pgprot_val(pgprot_noncached(pgprot)) == pgprot_val(pgprot))) {
		flags = LIBLINUX_PAL_REMAP_DEVICE;
	}

	if ((vma != NULL) && (vma->vm_file != NULL) &&
		is_dma_buf_file(vma->vm_file) &&
		!is_direct_reclaim_dmabuf(vma->vm_file->private_data))
		flags |= LIBLINUX_PAL_REMAP_DMA_BUF;

#ifdef CONFIG_MM_LB_FULL_COHERENCY
	sh = PGPROT_SH(pgprot_val(pgprot));
	if (sh)
		flags |= ((sh << LIBLINUX_PAL_REMAP_SH_SHIFT) & LIBLINUX_PAL_REMAP_SH);
#endif

	return liblinux_pal_remap_pa_range_batch_mm(addr, &pfn_arr, 1U, prot, flags, VSPACE_KEY(vma->vm_mm));
}
EXPORT_SYMBOL(remap_pfn_range);

int remap_pfn_range_batch_sp(struct vm_area_struct *vma, unsigned long addr,
			  const struct pfn_range *array, unsigned long cnt, pgprot_t prot)
{
	unsigned int flags = LIBLINUX_PAL_REMAP_CACHE;
	unsigned long pgprot = __pgprot_to_posix_prot(prot);

	if (!_check_vma_mm_valid(vma))
		return -EINVAL;
	flags = pgprot_to_liblinux_flags(prot);
	if ((vma != NULL) && (vma->vm_file != NULL) &&
		is_dma_buf_file(vma->vm_file) &&
		!is_direct_reclaim_dmabuf(vma->vm_file->private_data))
		flags |= LIBLINUX_PAL_REMAP_DMA_BUF;

	return liblinux_pal_remap_pa_range_batch_mm(addr, array, cnt, pgprot, flags, VSPACE_KEY(vma->vm_mm));
}

int vm_iomap_memory(struct vm_area_struct *vma, phys_addr_t start, unsigned long len)
{
	unsigned long vm_len, pfn, pages;

	/* Check that the physical memory area passed in looks valid */
	if (start + len < start)
		return -EINVAL;
	/*
	 * You *really* shouldn't map things that aren't page-aligned,
	 * but we've historically allowed it because IO memory might
	 * just have smaller alignment.
	 */
	len += start & ~PAGE_MASK;
	pfn = start >> PAGE_SHIFT;
	pages = (len + ~PAGE_MASK) >> PAGE_SHIFT;
	if (pfn + pages < pfn)
		return -EINVAL;

	/* We start the mapping 'vm_pgoff' pages into the area */
	if (vma->vm_pgoff > pages)
		return -EINVAL;
	pfn += vma->vm_pgoff;
	pages -= vma->vm_pgoff;

	/* Can we fit all of the mapping? */
	vm_len = vma->vm_end - vma->vm_start;
	if (vm_len >> PAGE_SHIFT > pages)
		return -EINVAL;

	/* Ok, let it rip */
	return io_remap_pfn_range(vma, vma->vm_start, pfn, vm_len, vma->vm_page_prot);
}
EXPORT_SYMBOL(vm_iomap_memory);

unsigned long zero_pfn __read_mostly;
EXPORT_SYMBOL(zero_pfn);

void zap_vma_ptes(struct vm_area_struct *vma, unsigned long address,
		unsigned long size)
{
	(void)liblinux_zap_vma_ptes(vma, address, size);
}
EXPORT_SYMBOL(zap_vma_ptes);

int liblinux_zap_vma_ptes(struct vm_area_struct *vma, unsigned long address,
			  unsigned long size)
{
	int ret;

	if (!_check_vma_mm_valid(vma))
		return -EINVAL;
	if (address < vma->vm_start || address + size > vma->vm_end ||
				!(vma->vm_flags & VM_PFNMAP)) {
		pr_err("zap range is invalid\n");
		return -EINVAL;
	}

	ret = liblinux_pal_vm_zap_ptes(address, size, VSPACE_KEY(vma->vm_mm));
	if (ret != 0 && ret != -ENOMEM)
		pr_err_ratelimited("zap vma ptes (0x%p + %lu) for 0x%llx failed, ret %d\n", (void*)address, size, VSPACE_KEY(vma->vm_mm), ret);

	return ret;
}
EXPORT_SYMBOL(liblinux_zap_vma_ptes);

int liblinux_zap_vma_ptes_sp( unsigned long address, unsigned long size, int tgid)
{
	int ret = liblinux_pal_vm_zap_sp(address, size, tgid);
	if (ret != 0)
		pr_err("zap vma ptes (0x%p + %lu) for %d failed, ret %d\n", (void*)address, size, tgid, ret);

	return ret;
}
#ifdef CONFIG_NEED_CHANGE_MAPPING
void change_secpage_range(phys_addr_t phys, unsigned long addr,
		      unsigned long size, pgprot_t prot)
{
	unsigned long end = addr + size;

	if (WARN_ON(addr >= end))
		return;

	if (!PAGE_ALIGNED(phys) || !PAGE_ALIGNED(addr) || !PAGE_ALIGNED(size))
		return;

	liblinux_pal_change_secpage_range(phys, addr, size, pgprot_to_liblinux_flags(prot));
}
#endif

static unsigned long fault_around_bytes __read_mostly = 0;
static vm_fault_t __do_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	int ret;

	ret = vma->vm_ops->fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY |
			    VM_FAULT_DONE_COW)))
		return ret;

	if (unlikely(PageHWPoison(vmf->page))) {
		if (ret & VM_FAULT_LOCKED)
			unlock_page(vmf->page);
		put_page(vmf->page);
		vmf->page = NULL;
		return VM_FAULT_HWPOISON;
	}

	if (unlikely(!(ret & VM_FAULT_LOCKED)))
		lock_page(vmf->page);
	else
		VM_BUG_ON_PAGE(!PageLocked(vmf->page), vmf->page);

	return ret;
}

static vm_fault_t alloc_set_pte(struct vm_fault *vmf, struct page *page);

vm_fault_t finish_fault(struct vm_fault *vmf)
{
	struct page *page = NULL;
	int ret = 0;

	/* Did we COW the page? */
	if ((vmf->flags & FAULT_FLAG_WRITE) &&
	    !(vmf->vma->vm_flags & VM_SHARED))
		page = vmf->cow_page;
	else
		page = vmf->page;
	if (!page)
		return VM_FAULT_OOM;

	ret = alloc_set_pte(vmf, page);
	if (ret != 0)
		return VM_FAULT_RETRY;
	return 0;
}

static vm_fault_t do_fault_around(struct vm_fault *vmf)
{
	unsigned long address = vmf->address, nr_pages, mask;
	pgoff_t start_pgoff = vmf->pgoff;
	pgoff_t end_pgoff;
	int off, ret = 0;

	nr_pages = READ_ONCE(fault_around_bytes) >> PAGE_SHIFT;
	mask = ~(nr_pages * PAGE_SIZE - 1) & PAGE_MASK;

	vmf->address = max(address & mask, vmf->vma->vm_start);
	off = ((address - vmf->address) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
	start_pgoff -= off;

	/*
	 *  end_pgoff is either end of page table or end of vma
	 *  or fault_around_pages() from start_pgoff, depending what is nearest.
	 */
	end_pgoff = start_pgoff -
		((vmf->address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1)) +
		PTRS_PER_PTE - 1;
	end_pgoff = min3(end_pgoff, vma_pages(vmf->vma) + vmf->vma->vm_pgoff - 1,
			start_pgoff + nr_pages - 1);

	vmf->vma->vm_ops->map_pages(vmf, start_pgoff, end_pgoff);

	/* ->map_pages() haven't done anything useful. Cold page cache? */
	if (!vmf->pte)
		goto out;

	/* check if the page fault is solved */
	vmf->pte -= (vmf->address >> PAGE_SHIFT) - (address >> PAGE_SHIFT);
	if (!pte_none(*vmf->pte))
		ret = VM_FAULT_NOPAGE;
out:
	vmf->address = address;
	vmf->pte = NULL;
	return ret;
}

static vm_fault_t do_page_mkwrite(struct vm_fault *vmf)
{
	vm_fault_t ret;
	struct page *page = vmf->page;
	unsigned int old_flags = vmf->flags;

	vmf->flags = FAULT_FLAG_WRITE | FAULT_FLAG_MKWRITE;

	if (vmf->vma->vm_file &&
	    IS_SWAPFILE(vmf->vma->vm_file->f_mapping->host))
		return VM_FAULT_SIGBUS;

	ret = vmf->vma->vm_ops->page_mkwrite(vmf);
	/* Restore original flags so that caller is not surprised */
	vmf->flags = old_flags;
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE)))
		return ret;
	if (unlikely(!(ret & VM_FAULT_LOCKED))) {
		lock_page(page);
		if (!page->mapping) {
			unlock_page(page);
			return 0; /* retry */
		}
		ret |= VM_FAULT_LOCKED;
	} else {
		VM_BUG_ON_PAGE(!PageLocked(page), page);
	}
	return ret;
}

static vm_fault_t mark_page_dirty_for_write(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	vm_fault_t ret = VM_FAULT_HINDEX_MASK;

	/*
	 * Check if the backing address space wants to know that the page is
	 * about to become writable
	 */
	if (vma->vm_ops->page_mkwrite && (vma->vm_flags & VM_WRITE)) {
		unlock_page(vmf->page);
		ret = do_page_mkwrite(vmf);
		if (unlikely(!ret ||
				(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE)))) {
			put_page(vmf->page);
			return ret;
		}
	}
	return ret;
}

static vm_fault_t do_read_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	vm_fault_t ret;

	/*
	 * Let's call ->map_pages() first and use ->fault() as fallback
	 * if page by the offset is not ready to be mapped (cold cache or
	 * something).
	 */
	if ((vma->vm_ops->map_pages) && ((fault_around_bytes >> PAGE_SHIFT) > 1)) {
		ret = do_fault_around(vmf);
		if (ret)
			return ret;
	}

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

	ret |= finish_fault(vmf);
	unlock_page(vmf->page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		put_page(vmf->page);

	return ret;
}

static vm_fault_t do_shared_fault(struct vm_fault *vmf)
{
	vm_fault_t ret, tmp;

	ret = __do_fault(vmf);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE | VM_FAULT_RETRY)))
		return ret;

	tmp = mark_page_dirty_for_write(vmf);
	if (unlikely(!tmp || (tmp & (VM_FAULT_ERROR | VM_FAULT_NOPAGE))))
		return tmp;

	ret |= finish_fault(vmf);
	unlock_page(vmf->page);
	if (unlikely(ret & (VM_FAULT_ERROR | VM_FAULT_NOPAGE |
					VM_FAULT_RETRY)))
		put_page(vmf->page);

	return ret;
}

static unsigned long _vma_page_pgoff(struct vm_area_struct *vma, unsigned long addr)
{
	return ((addr - vma->vm_start) >> PAGE_SHIFT);
}

static vm_fault_t do_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	int ret;

	/* The VMA was not fully populated on mmap() or missing VM_DONTEXPAND */
	if (!vma->vm_ops->fault)
		ret = VM_FAULT_SIGBUS;
	else if (!(vmf->flags & FAULT_FLAG_WRITE))
		ret = do_read_fault(vmf);
	else if (!(vma->vm_flags & VM_SHARED))
		ret = VM_FAULT_SIGBUS;
	else
		ret = do_shared_fault(vmf);

	return ret;
}

static vm_fault_t handle_pte_fault(struct vm_fault *vmf)
{
	if (!vmf->pte) {
		if (vma_is_anonymous(vmf->vma))
			return VM_FAULT_FALLBACK;
		else
			return do_fault(vmf);
	}

	return VM_FAULT_FALLBACK;
}

static gfp_t __get_fault_gfp_mask(struct vm_area_struct *vma)
{
	struct file *vm_file = vma->vm_file;

	if (vm_file)
		return mapping_gfp_mask(vm_file->f_mapping) | __GFP_FS | __GFP_IO;

	/*
	 * Special mappings (e.g. VDSO) do not have any file so fake
	 * a default GFP_KERNEL for them.
	 */
	return GFP_KERNEL;
}

static int __handle_mm_fault(struct vm_area_struct *vma, unsigned long address,
		unsigned int flags)
{
	vm_fault_t ret;
	struct vm_fault vmf = {
		.vma = vma,
		.address = address & PAGE_MASK,
		.flags = flags,
		.pgoff = linear_page_index(vma, address),
		.gfp_mask = __get_fault_gfp_mask(vma),
		.pte = NULL,
	};

	ret = handle_pte_fault(&vmf);
	if ((ret & (VM_FAULT_ERROR | VM_FAULT_RETRY)) != 0) {
		pr_warn("handle_pte_fault ret %d\n", ret);
		return -EFAULT;
	}
	return 0;
}

vm_fault_t handle_mm_fault(struct vm_area_struct *vma, unsigned long address,
			   unsigned int flags, struct pt_regs *regs)
{
	int ret;

	if (unlikely(is_vm_hugetlb_page(vma)))
		ret = -EINVAL;
	else
		ret = __handle_mm_fault(vma, address, flags);

	return ret;
}
EXPORT_SYMBOL_GPL(handle_mm_fault);

unsigned int pgprot_to_liblinux_flags(pgprot_t prot)
{
	unsigned int flags = LIBLINUX_PAL_REMAP_CACHE;

#ifdef CONFIG_ARM64
	if (pgprot_val(pgprot_writecombine(prot)) == pgprot_val(prot))
		flags = LIBLINUX_PAL_REMAP_NORMAL_NC;

	if ((pgprot_val(pgprot_device(prot)) == pgprot_val(prot)) ||
	    (pgprot_val(pgprot_noncached(prot)) == pgprot_val(prot)))
		flags = LIBLINUX_PAL_REMAP_DEVICE;
#endif
#ifdef CONFIG_ARM
	if ((pgprot_val(pgprot_device(prot)) == pgprot_val(prot)))
		flags = LIBLINUX_PAL_REMAP_DEVICE;

	if ((pgprot_val(pgprot_noncached(prot)) == pgprot_val(prot)) ||
	    (pgprot_val(pgprot_stronglyordered(prot)) == pgprot_val(prot)))
		flags = LIBLINUX_PAL_REMAP_NORMAL_NC;
#endif
	return flags;
}

unsigned long vm_flags_to_prot(unsigned long vm_flags)
{
	unsigned long prot = PROT_NONE;
	if (vm_flags & VM_READ)
		prot |= PROT_READ;

	/* NOTE: should support `page_mkwrite` by `write-page-fault` */
	if (vm_flags & VM_WRITE)
		prot |= PROT_WRITE;

	if (vm_flags & VM_EXEC)
		prot |= PROT_EXEC;

	return prot;
}

static vm_fault_t alloc_set_pte(struct vm_fault *vmf, struct page *page)
{
	struct vm_area_struct *vma = vmf->vma;
	bool write = vmf->flags & FAULT_FLAG_WRITE;
	pte_t entry;
	int ret;

	/* Re-check under ptl */
	if (unlikely(!pte_none(*vmf->pte)))
		return VM_FAULT_NOPAGE;

	entry = mk_pte(page, vma->vm_page_prot);
	if (write)
		entry = maybe_mkwrite(pte_mkdirty(entry), vma);
	set_pte_at(vma->vm_mm, vmf->address, vmf->pte, entry);

	ret = liblinux_pal_remap_pfn_range(vmf->address, page_to_pfn(page), PAGE_SIZE,
			vm_flags_to_prot(vma->vm_flags), pgprot_to_liblinux_flags(vma->vm_page_prot));
	if (ret == 0)
		return 0;

	pte_clear(vma->vm_mm, vmf->address, vmf->pte);
	return VM_FAULT_RETRY;
}

vm_fault_t vmf_insert_pfn_prot(struct vm_area_struct *vma, unsigned long addr,
			       unsigned long pfn, pgprot_t prot)
{
	int ret;
	struct pfn_range range = { pfn, pfn + 1U };

	ret = liblinux_pal_remap_pa_range_batch_sp(addr, &range, 1,
			vm_flags_to_prot(vma->vm_flags), pgprot_to_liblinux_flags(prot), current->tgid, current->thread.fault_vr_key);
	if (ret != 0 && ret != -EEXIST)
		return VM_FAULT_SIGBUS;

	return VM_FAULT_NOPAGE;
}
EXPORT_SYMBOL(vmf_insert_pfn_prot);

vm_fault_t vmf_insert_pfn(struct vm_area_struct *vma, unsigned long addr,
			  unsigned long pfn)
{
	return vmf_insert_pfn_prot(vma, addr, pfn, vma->vm_page_prot);
}
EXPORT_SYMBOL(vmf_insert_pfn);

vm_fault_t vmf_insert_pfn_batch_prot(struct vm_area_struct *vma, unsigned long addr,
				     const struct pfn_range *array, unsigned long cnt, pgprot_t prot)
{
	int ret;
	ret = liblinux_pal_remap_pa_range_batch_sp(addr, array, cnt,
			vm_flags_to_prot(vma->vm_flags), pgprot_to_liblinux_flags(prot), current->tgid, current->thread.fault_vr_key);
	if (ret != 0 && ret != -EEXIST)
		return VM_FAULT_SIGBUS;

	return VM_FAULT_NOPAGE;
}
EXPORT_SYMBOL(vmf_insert_pfn_batch_prot);

vm_fault_t vmf_insert_pfn_batch(struct vm_area_struct *vma, unsigned long addr,
				const struct pfn_range *array, unsigned long cnt)
{
	return vmf_insert_pfn_batch_prot(vma, addr, array, cnt, vma->vm_page_prot);
}
EXPORT_SYMBOL(vmf_insert_pfn_batch);

vm_fault_t vmf_insert_pfn_batch_sp(struct vm_area_struct *vma, unsigned long addr,
			const struct pfn_range *array, unsigned long cnt, int tgid)
{
	int ret;

	if (current->tgid != tgid) {
		pr_warn("current tgid(%d) != tgt(%d)\n", current->tgid, tgid);
		return VM_FAULT_SIGBUS;
	}
	ret = liblinux_pal_remap_pa_range_batch_sp(addr, array, cnt,
			vm_flags_to_prot(vma->vm_flags), pgprot_to_liblinux_flags(vma->vm_page_prot), tgid, current->thread.fault_vr_key);
	if (ret != 0 && ret != -EEXIST)
		return VM_FAULT_SIGBUS;

	return VM_FAULT_NOPAGE;
}
