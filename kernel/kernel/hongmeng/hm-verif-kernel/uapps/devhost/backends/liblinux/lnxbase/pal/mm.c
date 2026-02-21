/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for mm operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <hmasm/page.h>
#include <sys/mman.h>
#include <libmem/flags.h>
#include <libmem/types.h>
#include <libmem/umem.h>
#include <libmem/utils.h>
#include <devhost/memory.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/vmap.h>
#include <hmkernel/mm/pgtable.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <libdevhost/mm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>

#include "shrinker.h"
#include <devhost/log.h>

void liblinux_pal_change_secpage_range(unsigned long long phys_addr, unsigned long vaddr,
				       unsigned long size, unsigned int flags)
{
	uint64_t hint;
	int ret;

	hint = libdh_iomem_map_hint(0, flags);

	ret = actvcall_hmcall_mem_change_secmem_linmap_attr(vaddr, phys_addr, size, hint, SECMEM_LINMAP_FLAG_DEVHOST);
	if (ret < 0) {
		dh_error("change secpage attr failed, phys_addr=%p, size=%lu, flags=%x err=%s\n",
			 ulong_to_ptr(phys_addr, void), size, flags, hmstrerror(ret));
	}
}

int liblinux_pal_remap_pfn_range(unsigned long addr, unsigned long pfn,
				 unsigned long size, unsigned long prot,
				 unsigned int flags)
{
	int ret;
	ret = libdh_iomem_map(ulong_to_ptr(addr, void), (pfn << PAGE_SHIFT), size, (unsigned int)prot, flags);
	if (ret != 0) {
		dh_error("iomem map failed, err=%s\n", hmstrerror(ret));
	}
	return -hmerrno2posix(ret);
}

int liblinux_pal_remap_pa_range_batch_sp(unsigned long addr, const void *pa_array,
					 unsigned int pa_array_size, unsigned long prot,
					 unsigned int flags, int pid, unsigned long long vr_key)
{
	int ret;
	ret = libdh_iomem_map_batch_pid(ulong_to_ptr(addr, void), pa_array, pa_array_size,
					(unsigned int)prot, flags, pid, vr_key);
	if (ret != 0) {
		if (ret == E_HM_OBJEXIST) {
			ret = E_HM_POSIX_EXIST;
		}
	}
	return -hmerrno2posix(ret);
}

int liblinux_pal_remap_pa_range_batch_mm(unsigned long addr, const void *pa_array,
					 unsigned int pa_array_size, unsigned long prot,
					 unsigned int flags, unsigned long long vs_key)
{
	int ret;
	ret = libdh_iomem_map_batch_mm(ulong_to_ptr(addr, void), pa_array, pa_array_size,
					(unsigned int)prot, flags, vs_key);
	if (ret != 0) {
		if (ret == E_HM_OBJEXIST) {
			ret = E_HM_POSIX_EXIST;
		}
	}
	return -hmerrno2posix(ret);
}

int liblinux_pal_remap_pa_range_batch(unsigned long addr, const void *pa_array,
				      unsigned int pa_array_size, unsigned long prot,
				      unsigned int flags)
{
	int ret;
	ret = libdh_iomem_map_batch(ulong_to_ptr(addr, void), pa_array, pa_array_size,
				    (unsigned int)prot, flags);
	if (ret != 0) {
		dh_error("iomem map batch failed, err=%s\n", hmstrerror(ret));
	}
	return -hmerrno2posix(ret);
}

static int __vm_mem_munmap(struct vmap_node *node)
{
	int ret;

	ret = hm_mem_munmap((void *)node->vaddr, (size_t)node->size);
	if (ret != E_HM_OK) {
		dh_warn("unmap failed, err=%s\n", hmstrerror(ret));
	}

	free(node);
	return -hmerrno2posix(ret);
}

static int __init_vmap_node(struct vmap_node *node, struct vmap_range range, unsigned long type,
			    const void *caller, int (*release)(struct vmap_node *node))
{
	int ret = 0;

	node->vaddr = range.vaddr;
	node->size = range.size;
	node->flags = 0;
	node->release = release;
	node->type = type;
	node->caller = caller;

	ret = vmap_tree_insert(node);
	if (ret < 0) {
		dh_debug("insert vmap_node failed\n");
	}
	return ret;
}

static int add_new_vmap_node(const void *vaddr, unsigned long size,
			     unsigned long type, const void *caller)
{
	int ret;
	struct vmap_node *node = NULL;
	struct vmap_range range = {
		.vaddr = vaddr,
		.size = size,
	};

	node = (struct vmap_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*node);

	ret = __init_vmap_node(node, range, type, caller, __vm_mem_munmap);
	if (ret < 0) {
		free(node);
	}
	return ret;
}

static int __release_ioremap_node(struct vmap_node *_node)
{
	struct ioremap_node *node = container_of(_node, struct ioremap_node, vmap);
	int ret;

	ret = hm_mem_munmap((void *)node->vmap.vaddr, (size_t)node->vmap.size);
	if (ret != E_HM_OK) {
		dh_warn("unmap failed, err=%s\n", hmstrerror(ret));
	}

	free(node);
	return -hmerrno2posix(ret);
}

static int add_ioremap_node(const void *vaddr, unsigned long size, unsigned long phys_addr,
			    unsigned long type, const void *caller)
{
	int ret;
	struct ioremap_node *node = NULL;
	struct vmap_range range = {
		.vaddr = vaddr,
		.size = size,
	};

	node = (struct ioremap_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*node);
	node->phys_addr = phys_addr;

	ret = __init_vmap_node(&node->vmap, range, type, caller, __release_ioremap_node);
	if (ret < 0) {
		free(node);
	}
	return ret;
}

void *liblinux_pal_ioremap_ex(unsigned long phys_addr, unsigned long size, unsigned long prot, unsigned int flags,
			      const void *caller)
{
	void *vaddr = NULL;

	vaddr = libdh_ioremap_ex((paddr_t)phys_addr, size, (unsigned int)prot, flags);
	if (vaddr != NULL) {
		if (add_ioremap_node(vaddr, size, phys_addr, VMAP_TYPE_IOREMAP, caller) < 0) {
			libdh_iounmap(vaddr, size);
			vaddr = NULL;
		}
	} else {
		dh_warn("ioremap failed\n");
	}

	return vaddr;
}

void *liblinux_pal_ioremap(unsigned long phys_addr, unsigned long size, unsigned long prot, const void *caller)
{
	return liblinux_pal_ioremap_ex((paddr_t)phys_addr, size, (unsigned int)prot, LIBLINUX_PAL_REMAP_DEVICE, caller);
}

int liblinux_pal_iofast_page_info_init(unsigned long pg_struct_va_start, size_t pg_struct_size,
				       unsigned int pg_slab_shift)
{
	int ret;

	ret = actvcall_hmcall_mem_iofast_ldk_page_info_init(pg_struct_va_start,
							    pg_struct_size, pg_slab_shift);
	return -hmerrno2posix(ret);
}

/* caller should ensure PAGE_ALINGN size and PAGE_MASK vaddr */
void *liblinux_pal_vm_prepare(unsigned long vaddr, unsigned long size,
			      unsigned long prot, unsigned int flags)
{
	int ret;
	struct __actvret_hmcall_mem_ioremap_prepare func_ret;

	BUG_ON(size != PAGE_ALIGN_UP(size));
	BUG_ON(vaddr & PAGE_OFFMASK);

	if (____IS(flags & LIBLINUX_PAL_REMAP_NORMAL_NC)) {
		prot |= MPROT_NOCACHE;
	}

	ret = actvxactcall_hmcall_mem_ioremap_prepare(
			(flags & LIBLINUX_PAL_VM_TO_USER) != 0, /* `fwd` */
			true, /* `allow refwd` */
			(const void *)vaddr, size, (unsigned int)prot,
			trans_vmflag_to_mapflag(flags),
			&func_ret);
	if (ret != E_HM_OK) {
		dh_error("ioremap_prepare(size=0x%lx, prot=0x%lx) failed, err=%s\n",
			 size, prot, hmstrerror(ret));
		return NULL;
	}

	if (____NOT(flags & LIBLINUX_PAL_VM_TO_USER)) {
		ret = add_new_vmap_node(u64_to_ptr(func_ret.rvaddr, const void),
					size, VMAP_TYPE_MAP, lnxbase_return_address(1U));
		if (ret < 0) {
			goto add_node_err;
		}
	}

	return u64_to_ptr(func_ret.rvaddr, void);

add_node_err:
	ret = hm_mem_munmap(u64_to_ptr(func_ret.rvaddr, void), (size_t)size);
	if (ret != E_HM_OK) {
		dh_warn("unmap failed, err=%s\n", hmstrerror(ret));
	}
	return NULL;
}

/* caller should ensure PAGE_ALINGN size and PAGE_MASK paddr */
int liblinux_pal_vm_mmap(unsigned long paddr, unsigned long vaddr,
			 unsigned long size, unsigned long prot,
			 unsigned int flags)
{
	int ret;
	uint64_t hint;

	BUG_ON(size != PAGE_ALIGN_UP(size));
	BUG_ON(paddr & PAGE_OFFMASK);
	BUG_ON(vaddr & PAGE_OFFMASK);

	hint = prot_to_hint((uint32_t)prot, 0, 0);
	if (____IS(flags & LIBLINUX_PAL_REMAP_NORMAL_NC)) {
		hint |= PGTABLE_ATTR_NOCACHE;
	} else if (____NOT(flags & LIBLINUX_PAL_REMAP_CACHE)) {
		hint |= PGTABLE_ATTR_DEVICE;
	}

	if ((flags & LIBLINUX_PAL_REMAP_SH) != 0) {
		hint |= libdh_iomem_map_hint(0, flags);
	}

	ret = actvxactcall_hmcall_mem_iomem_mmap_page(
			(flags & LIBLINUX_PAL_VM_TO_USER) != 0, /* `fwd` */
			true, /* `allow refwd` */
			ERR_TO_REF(E_HM_INVAL), /* obsolete vspace_rref */
			(unsigned long long)paddr,
			(void *)vaddr, size, hint, 0);
	if (ret != E_HM_OK) {
		dh_error("iomem_mmap_page(size=0x%lx, hint=%"PRIx64") failed, err=%s\n",
			 size, hint, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

/* caller should ensure PAGE_ALINGN size and PAGE_MASK paddr */
int liblinux_pal_vm_mmap_batch(const void *pa_array,
			       unsigned int pa_array_size,
			       unsigned long vaddr,
			       unsigned long prot, unsigned int flags)
{
	int ret;

	ret = libdh_vm_mmap_batch(pa_array, pa_array_size, ulong_to_ptr(vaddr, void),
				  (unsigned int)prot, flags);
	if (ret != 0) {
		dh_error("vm mmap batch failed, err=%s\n", hmstrerror(ret));
	}
	return -hmerrno2posix(ret);
}

/*
 * unmap a vmap node for page_alloc/ioremap/vmap/dma_alloc/vmalloc
 *
 * @param   addr [I] the address to be unmapped
 * @return  unmap success or failed
 */
int liblinux_pal_vm_unmap(const void *addr)
{
	struct vmap_node *node = NULL;

	node = vmap_tree_remove(addr);
	if (node == NULL) {
#ifdef CONFIG_PAGE_ALLOC_VMAP
		dh_info("vm unmap addr is invalid\n");
		return -EINVAL;
#else
		return -ENOSYS;
#endif
	}

	BUG_ON(node->release == NULL);
	return node->release(node);
}

int liblinux_pal_vm_unpopulate(void *addr, unsigned long size)
{
	int ret;

	if ((addr == NULL) || (size == 0UL)) {
		dh_debug("param is invaild\n");
		return -EINVAL;
	}

	ret = actvcall_hmcall_mem_iomap_unpopulate(addr, size);
	if (ret < 0) {
		dh_error("unpopulate va failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return ret;
}

void liblinux_pal_mem_enable_rsv_mem(unsigned long pfn_start, unsigned long pfn_end)
{
	int ret;

	ret = hm_mem_enable_rsv_mem(pfn_start, pfn_end);
	if (ret != E_HM_OK)
		dh_debug("enable rsv mem failed, err=%s\n", hmstrerror(ret));
}

int liblinux_pal_vm_zap(unsigned long addr, unsigned long len, unsigned int flags)
{
	int ret;

	ret = devhost_pal_vm_zap(addr, len, flags);
	return -hmerrno2posix(ret);
}

unsigned long liblinux_pal_usermap_prepare(unsigned long addr, unsigned long len,
					   unsigned long prot, unsigned long flags,
					   void **priv)
{
	int ret;
	unsigned int hm_flags = (unsigned int)flags;

	/* flags will not exceed 32bit */
	BUG_ON(flags != (unsigned long)hm_flags);

	ret = devhost_pal_usermap_prepare(addr, len, prot, hm_flags, false,
					  (struct devhost_vma_info **)priv);
	ret = -hmerrno2posix(ret);
	if (ret < 0) {
		return (unsigned long)(long)ret;
	}

	return (*(struct devhost_vma_info **)priv)->start;
}

void liblinux_pal_usermap_finish(const void *priv, int success)
{
	devhost_pal_usermap_finish((const struct devhost_vma_info *)priv, false, success, 0);
}

int liblinux_pal_usermap_munmap(unsigned long addr, unsigned long len)
{
	int ret;

	ret = devhost_pal_usermap_munmap(addr, len);

	return -hmerrno2posix(ret);
}

void *liblinux_pal_find_vma(unsigned long addr)
{
	if (liblinux_pal_is_thread_ctx()) {
		return NULL;
	}
	return devhost_pal_find_vma(addr, 0);
}

void *liblinux_pal_find_vma_by_mm(unsigned long addr, unsigned long long vspace_key)
{
	if (vspace_key == 0) {
		return NULL;
	}
	return devhost_pal_find_vma(addr, vspace_key);
}

int liblinux_pal_copy_from_user(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = devhost_pal_copy_from_user(dst, src, n);
	return -hmerrno2posix(ret);
}

int liblinux_pal_strcpy_from_user(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = devhost_pal_strcpy_from_user(dst, src, n);
	if (unlikely(ret < 0)) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

int liblinux_pal_copy_to_user(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = devhost_pal_copy_to_user(dst, src, n);
    if (unlikely(ret < 0)) {
        return -hmerrno2posix(ret);
    }

    return ret;
}

int liblinux_pal_copy_in_user(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = devhost_pal_copy_in_user(dst, src, n);
    if (unlikely(ret < 0)) {
        return -hmerrno2posix(ret);
    }

    return ret;
}

int liblinux_pal_copy_msghdr_from_user(void *dst, void *src)
{
	int ret;

	ret = devhost_pal_copy_msghdr_from_user(dst, src);
	if (unlikely(ret < 0)) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

int liblinux_pal_copy_msghdr_to_user(void *dst, void *src, void *uaddr, void *ucmsg, int ulen)
{
	int ret;

	ret = devhost_pal_copy_msghdr_to_user(dst, src, uaddr, ucmsg, ulen);
	if (unlikely(ret < 0)) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

int liblinux_pal_set_pageattr(const void *addr, int numpages, int set_prot, int clear_prot)
{
	uint32_t prot = PROT_READ | PROT_WRITE | PROT_EXEC;
	size_t size;
	int ret;

	prot |= (uint32_t)set_prot;
	prot &= ~(uint32_t)clear_prot;

	size = PAGE_SIZE * (unsigned int)numpages;

	ret = hm_mem_mprotect(addr, size, prot);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int liblinux_pal_set_pageattr_ex(const void *addr, int numpages, int set_prot, int clear_prot)
{
	size_t size;
	int ret;

	size = PAGE_SIZE * (unsigned int)numpages;

	ret = hm_mem_amend_prot(addr, size, set_prot, clear_prot);

	return -hmerrno2posix(ret);
}

void liblinux_pal_kasan_poison(const void *address, unsigned long size, unsigned char value)
{
	UNUSED(address, size, value);
	return;
}

void liblinux_pal_kasan_check(unsigned long addr, unsigned long size, unsigned int is_write, unsigned long ret_addr)
{
	UNUSED(addr, size, is_write, ret_addr);
	return;
}

void liblinux_pal_kasan_report(unsigned long addr, unsigned long size, unsigned int is_write, unsigned long ret_addr)
{
	UNUSED(addr, size, is_write, ret_addr);
	return;
}

static
struct lnxbase_sub_shrinker *__liblinux_sub_shrinker_to_lnxbase(struct liblinux_pal_sub_shrinker shrinker)
{
	struct lnxbase_sub_shrinker *sub_shrinker;

	sub_shrinker = (struct lnxbase_sub_shrinker *)malloc(sizeof(struct lnxbase_sub_shrinker));
	if (sub_shrinker == NULL) {
		return NULL;
	}
	mem_zero_s(*sub_shrinker);

	sub_shrinker->query = shrinker.query;
	sub_shrinker->shrink = shrinker.shrink;
	sub_shrinker->priv = shrinker.priv;

	return sub_shrinker;
}

void liblinux_pal_sub_shrinker_register(const struct liblinux_page_allocator *_allocator,
					struct liblinux_pal_sub_shrinker shrinker)
{
	struct shrinkable_allocator *shrinker_allocator;
	struct lnxbase_sub_shrinker *sub_shrinker;

	sub_shrinker = __liblinux_sub_shrinker_to_lnxbase(shrinker);
	if (sub_shrinker == NULL) {
		dh_error("malloc lnxbase sub shrinker failed\n");
		return;
	}

	if (_allocator == NULL) {
		lnxbase_shrinker_register(sub_shrinker);
	} else {
		shrinker_allocator = container_of(_allocator, struct shrinkable_allocator, allocator);
		lnxbase_sub_shrinker_register(&shrinker_allocator->shrinker, sub_shrinker);
	}
}

void liblinux_pal_sub_shrinker_unregister(const struct liblinux_page_allocator *_allocator,
					struct liblinux_pal_sub_shrinker shrinker)
{
	struct shrinkable_allocator *shrinker_allocator;
	struct lnxbase_sub_shrinker sub_shrinker;

	sub_shrinker.priv = shrinker.priv;
	if (_allocator == NULL) {
		dh_warn("unregister lnxbase_shrinker not supported\n");
	} else {
		shrinker_allocator = container_of(_allocator, struct shrinkable_allocator, allocator);
		lnxbase_sub_shrinker_unregister(&shrinker_allocator->shrinker, &sub_shrinker);
	}
}

unsigned long liblinux_pal_query_pgd(unsigned int pid)
{
	struct __actvret_hmcall_mem_query_pgd aret;
	int ret;

	ret = actvcall_hmcall_mem_query_pgd(pid, &aret);
	if (ret != 0) {
		dh_error("query pgd failed: %s\n", hmstrerror(ret));
		return 0;
	}

	return aret.pgd;
}

unsigned long liblinux_pal_usermap_alloc_va(const struct pal_vm_info *info)
{
	void *alloc_va = NULL;

	if (info == NULL) {
		return (unsigned long)-EINVAL;
	}

	alloc_va = hm_mem_alloc_range_restricted((size_t)info->length, info->low_limit, info->high_limit,
						 (uint32_t)info->flags, info->align_mask, info->align_offset);
	if (alloc_va == MAP_FAILED) {
		return (unsigned long)-ENOMEM;
	}
	return (unsigned long)(uintptr_t)alloc_va;
}

int liblinux_pal_charge_mlock(unsigned long mlock_size)
{
	return hm_umem_charge_mlock(mlock_size);
}

int liblinux_pal_uncharge_mlock(unsigned long mlock_size)
{
	return hm_umem_uncharge_mlock(mlock_size);
}

#include <devhost/shrinker.h>
#define PREPARE_MODE_MASK	(3 << 6U)
#define PREPARE_MODE_WORK	(0 << 6U)
#define PREPARE_MODE_ALLOC	(1 << 6U)
#define PREPARE_MODE_POST	(2 << 6U) // do_size_remaining

void liblinux_pal_cpa_prepare_memory(struct liblinux_pal_cpa_prepare_memory_args *args)
{
	int ret;

	ret = hm_mem_cpa_prepare_memory(args->flags, args->water_mark_page,
					args->alloc_size, args->heap_size,
					args->allocated_size);
	if (ret != E_HM_OK) {
		dh_error("cpa_prepare_memory failed: flags 0x%x water_mark_page %lu heap_size %lu"
			 "allocated_size %lx size 0x%lx order %u, err=%s\n",
			 args->flags, args->water_mark_page, args->heap_size,
			 args->allocated_size, args->alloc_size, args->order, hmstrerror(ret));
	}

	int shrinker_mode = MAX_SHRINKER_MODE;
	if ((args->flags & PREPARE_MODE_MASK) == PREPARE_MODE_WORK) {
	        shrinker_mode = DEVHOST_SHRINKER_FAST;
	}

	if (shrinker_mode != MAX_SHRINKER_MODE) {
		int avail_pages = devhost_shrinker_query(shrinker_mode);
		if (avail_pages > 0) {
			(void)devhost_shrinker_shrink(avail_pages);
		}
	}
}

int liblinux_pal_mm_ama_ddr_intr(unsigned int flow_type, unsigned int ch)
{
	int ret;

	ret = actvxactcall_hmcall_mem_ama_ddr_intr_notify(false, false, flow_type, ch);
	if (ret != E_HM_OK) {
		hm_error("pal_mm_ama_ddr_intr failed: %s",  hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

int liblinux_pal_mm_extfrag_index_read(void *buf, size_t len)
{
	return hm_mem_extfrag_index_read(buf, len);
}
