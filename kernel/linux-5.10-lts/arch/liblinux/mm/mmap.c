// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mman.h>
#include <linux/hugetlb.h>
#include <linux/kmemleak.h>
#include <liblinux/pal.h>
#include "mm_internal.h"

/* description of effects of mapping type and prot in current implementation.
 * this is due to the limited x86 page protection hardware.  The expected
 * behavior is in parens:
 *
 * map_type	prot
 *		PROT_NONE	PROT_READ	PROT_WRITE	PROT_EXEC
 * MAP_SHARED	r: (no) no	r: (yes) yes	r: (no) yes	r: (no) yes
 *		w: (no) no	w: (no) no	w: (yes) yes	w: (no) no
 *		x: (no) no	x: (no) yes	x: (no) yes	x: (yes) yes
 *
 * MAP_PRIVATE	r: (no) no	r: (yes) yes	r: (no) yes	r: (no) yes
 *		w: (no) no	w: (no) no	w: (copy) copy	w: (no) no
 *		x: (no) no	x: (no) yes	x: (no) yes	x: (yes) yes
 */
pgprot_t protection_map[16] __ro_after_init = {
	__P000, __P001, __P010, __P011, __P100, __P101, __P110, __P111,
	__S000, __S001, __S010, __S011, __S100, __S101, __S110, __S111
};

#ifndef CONFIG_ARCH_HAS_FILTER_PGPROT
static inline pgprot_t arch_filter_pgprot(pgprot_t prot)
{
	return prot;
}
#endif

pgprot_t vm_get_page_prot(unsigned long vm_flags)
{
	pgprot_t ret = __pgprot(pgprot_val(protection_map[vm_flags &
				(VM_READ|VM_WRITE|VM_EXEC|VM_SHARED)]) |
			pgprot_val(arch_vm_get_page_prot(vm_flags)));

	return arch_filter_pgprot(ret);
}
EXPORT_SYMBOL(vm_get_page_prot);

static inline u64 file_mmap_size_max(struct file *file, struct inode *inode)
{
	if (S_ISREG(inode->i_mode))
		return MAX_LFS_FILESIZE;

	if (S_ISBLK(inode->i_mode))
		return MAX_LFS_FILESIZE;

	/* Special "we do even unsigned file positions" case */
	if (file->f_mode & FMODE_UNSIGNED_OFFSET)
		return 0;

	/* Yes, random drivers might want more. But I'm tired of buggy drivers */
	return ULONG_MAX;
}

static inline bool file_mmap_ok(struct file *file, struct inode *inode,
				unsigned long pgoff, unsigned long len)
{
	u64 maxsize = file_mmap_size_max(file, inode);

	if (maxsize && len > maxsize)
		return false;
	maxsize -= len;
	if (pgoff > maxsize >> PAGE_SHIFT)
		return false;
	return true;
}

static unsigned long __do_mmap(struct file *file, unsigned long addr,
			unsigned long len, unsigned long prot, unsigned long flags,
			unsigned long pgoff, unsigned long *populate, struct list_head *uf)
{
	vm_flags_t vm_flags = 0;

	if (file) {
		struct inode *inode = file_inode(file);

		if (!file_mmap_ok(file, inode, pgoff, len))
			return -EOVERFLOW;

		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			if ((prot&PROT_WRITE) && !(file->f_mode&FMODE_WRITE))
				return -EACCES;

			/*
			 * Make sure we don't allow writing to an append-only
			 * file..
			 */
			if (IS_APPEND(inode) && (file->f_mode & FMODE_WRITE))
				return -EACCES;

			/*
			 * Make sure there are no mandatory locks on the file.
			 */
			if (locks_verify_locked(file))
				return -EAGAIN;

			vm_flags |= VM_SHARED | VM_MAYSHARE;
			if (!(file->f_mode & FMODE_WRITE))
				vm_flags &= ~(VM_MAYWRITE | VM_SHARED);

			/* fall through */
		case MAP_PRIVATE:
			if (!(file->f_mode & FMODE_READ))
				return -EACCES;

			if (!file->f_op->mmap)
				return -ENODEV;
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
			break;

		default:
			return -EINVAL;
		}
	} else {
		switch (flags & MAP_TYPE) {
		case MAP_SHARED:
			if (vm_flags & (VM_GROWSDOWN|VM_GROWSUP))
				return -EINVAL;
			/*
			 * Ignore pgoff.
			 */
			pgoff = 0;
			vm_flags |= VM_SHARED | VM_MAYSHARE;
			break;
		case MAP_PRIVATE:
			/*
			 * Set pgoff according to addr for anon_vma.
			 */
			pgoff = addr >> PAGE_SHIFT;
			break;
		default:
			return -EINVAL;
		}
	}

	/*
	 * Set 'VM_NORESERVE' if we should not account for the
	 * memory use of this mapping.
	 */
	if (flags & MAP_NORESERVE) {
		/* We honor MAP_NORESERVE if allowed to overcommit */
		if (sysctl_overcommit_memory != OVERCOMMIT_NEVER)
			vm_flags |= VM_NORESERVE;

		/* hugetlb applies strict overcommit unless MAP_NORESERVE */
		if (file && is_file_hugepages(file))
			vm_flags |= VM_NORESERVE;
	}
	vm_flags |= calc_vm_prot_bits(prot, 0);
	addr = mmap_region(file, addr, len, vm_flags, pgoff, uf);
	if (!IS_ERR_VALUE(addr) &&
	    ((vm_flags & VM_LOCKED) ||
	     (flags & (MAP_POPULATE | MAP_NONBLOCK)) == MAP_POPULATE))
		*populate = len;
	return addr;
}

extern void __liblinux_assign_vma_info(void *dh_vma, void *ldk_vma);
/*
 * The caller must hold down_write(&current->mm->mmap_sem).
 */
unsigned long do_mmap(struct file *file, unsigned long addr,
		      unsigned long len, unsigned long prot, unsigned long flags,
		      unsigned long pgoff, unsigned long *populate, struct list_head *uf)
{
	unsigned long ret = 0;
	void *priv = NULL;

	*populate = 0;

	if (!len)
		return -EINVAL;

	/* Careful about overflows.. */
	len = PAGE_ALIGN(len);
	if (!len)
		return -ENOMEM;

	/* offset overflow? */
	if ((pgoff + (len >> PAGE_SHIFT)) < pgoff)
		return -EOVERFLOW;

	addr = liblinux_pal_usermap_prepare(addr, len, prot, flags, &priv);
	if (IS_ERR_OR_NULL((const void *)addr))
		return addr;

	ret = __do_mmap(file, addr, len, prot, flags, pgoff, populate, uf);

	/* if __do_mmap failed, will do unprepare */
	if (!IS_ERR_VALUE(ret)) {
		__liblinux_assign_vma_info(priv, current->vmacache.vmas[0]);
		kmemleak_not_leak((void *)current->vmacache.vmas[0]);
		current->vmacache.vmas[0] = NULL;
	}
	liblinux_pal_usermap_finish((const void *)priv, !IS_ERR_VALUE(ret));

	return ret;
}

/* keep consistent with `alloc_vma` at devhost/core.c */
#ifdef CONFIG_LIBLINUX_MEM_SHRINK
#define alloc_vma()	((struct vm_area_struct *)kzalloc(sizeof(struct vm_area_struct), GFP_KERNEL))
#define free_vma(vma)	kfree(vma)
#else
#define alloc_vma()	((struct vm_area_struct *)liblinux_pal_calloc(1, sizeof(struct vm_area_struct)))
#define free_vma(vma)	liblinux_pal_free(vma)
#endif

unsigned long mmap_region(struct file *file, unsigned long addr,
		unsigned long len, vm_flags_t vm_flags, unsigned long pgoff,
		struct list_head *uf)
{
	int error = 0;
	struct vm_area_struct *vma;

	/*
	 * In order to only use read lock in devhost_umap_ops_munmap,
	 * we cannot allow vma to grow down/up.
	 */
	BUG_ON((vm_flags & VM_GROWSDOWN) || (vm_flags & VM_GROWSUP));

	vma = alloc_vma();
	if (!vma) {
		return -ENOMEM;
	}

	vma->vm_start = addr;
	vma->vm_end = addr + len;
	vma->vm_flags = vm_flags;
	vma->vm_page_prot = vm_get_page_prot(vm_flags);
	vma->vm_pgoff = pgoff;
	vma->vm_mm = current->mm;

	if (file) {
		if (!file->f_op || !file->f_op->mmap) {
			free_vma(vma);
			return -ENODEV;
		}
		vma->vm_file = get_file(file);
		error = file->f_op->mmap(file, vma);
		if (error) {
			vma->vm_file = NULL;
			fput(file);
		}
	} else if (vm_flags & VM_SHARED) {
		error = -ENOSYS;
	}
	if (error) {
		free_vma(vma);
		return error;
	} else {
		/* `mmap_region` is defined at `linux/mm.h`. return vma by `current` */
		current->vmacache.vmas[0] = vma;
		return addr;
	}
}

int vm_munmap(unsigned long start, size_t len)
{
	return liblinux_pal_usermap_munmap(start, len);
}
EXPORT_SYMBOL(vm_munmap);

unsigned long vm_mmap_pgoff(struct file *file, unsigned long addr,
			    unsigned long len, unsigned long prot,
			    unsigned long flag, unsigned long pgoff)
{
	unsigned long ret;
	unsigned long populate;
	LIST_HEAD(uf);
	ret = do_mmap(file, addr, len, prot, flag, pgoff, &populate, &uf);
	return ret;
}

unsigned long vm_mmap(struct file *file, unsigned long addr,
		      unsigned long len, unsigned long prot,
		      unsigned long flag, unsigned long offset)
{
	if (unlikely(offset + PAGE_ALIGN(len) < offset))
		return -EINVAL;
	if (unlikely(offset_in_page(offset)))
		return -EINVAL;

	return vm_mmap_pgoff(file, addr, len, prot, flag, offset >> PAGE_SHIFT);
}
EXPORT_SYMBOL(vm_mmap);

struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr)
{
	struct vm_area_struct *vma = NULL;

	if (mm == NULL || mm == &init_mm) {
		pr_err("don't support find vma for kthread\n");
		dump_stack();
		return NULL;
	}
	vma = liblinux_pal_find_vma_by_mm(addr, VSPACE_KEY(mm));
	return vma;
}
EXPORT_SYMBOL(find_vma);

unsigned long vm_unmapped_area(struct vm_unmapped_area_info *info)
{
	struct pal_vm_info vm_info = {0};

	vm_info.flags = info->flags;
	vm_info.length = info->length;
	vm_info.low_limit = info->low_limit;
	vm_info.high_limit = info->high_limit;
	vm_info.align_mask = info->align_mask;
	vm_info.align_offset = info->align_offset;
	return liblinux_pal_usermap_alloc_va(&vm_info);
}

void __mmdrop(struct mm_struct *mm)
{
	BUG_ON(mm == &init_mm);
	WARN_ON_ONCE(mm == current->mm);
	WARN_ON_ONCE(mm == current->active_mm);
	liblinux_free_mm(mm);
}
EXPORT_SYMBOL_GPL(__mmdrop);
