/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of libdh remap
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 19 14:32:42 2023
 */
#include <hongmeng/errno.h>

#include <libdevhost/mm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>

static inline uint64_t hint_get_pbha_bit(uint32_t flags)
{
	uint64_t hint_bit = 0ULL;
	uint64_t pbha = ((flags & LIBDH_REMAP_PBHA) >> LIBDH_REMAP_PBHA_SHIFT);
	if (pbha != 0U) {
		hint_bit = (uint64_t)pbha << PGTABLE_ATTR_PBHA_SHIFT;
	}
	return hint_bit;
}

static inline uint64_t hint_get_sh_bit(uint32_t flags)
{
	uint64_t sh_bit = 0ULL;
	uint64_t sh = ((flags & LIBDH_REMAP_SH) >> LIBDH_REMAP_SH_SHIFT);
	if (sh != 0U) {
		sh_bit = (uint64_t)sh << PGTABLE_ATTR_SH_SHIFT;
	}
	return sh_bit;
}

uint64_t libdh_iomem_map_hint(unsigned int prot, unsigned int flags)
{
	uint64_t hint = 0;

	hint = prot_to_hint(prot, 0, 0);
	if (((prot & PROT_GROWSDOWN) != 0) || ((prot & PROT_GROWSUP) != 0)) {
		hm_warn("unsupported prot%s%s\n",
			((prot & PROT_GROWSDOWN) != 0) ? "PROT_GROWSDOWN" : "",
			((prot & PROT_GROWSUP) != 0) ? "PROT_GROWSUP"   : "");
	}

	if ((flags & LIBDH_REMAP_DEVICE) == LIBDH_REMAP_DEVICE) {
		hint |= PGTABLE_ATTR_DEVICE;
	} else if ((flags & LIBDH_REMAP_NORMAL_NC) == LIBDH_REMAP_NORMAL_NC) {
		hint |= PGTABLE_ATTR_NOCACHE;
	} else if ((flags & LIBDH_REMAP_CACHE) == 0U) {
		hint |= PGTABLE_ATTR_DEVICE;
	} else {
		/* do nothing */
	}

	if ((flags & LIBDH_REMAP_PBHA) != 0) {
		hint |= hint_get_pbha_bit(flags);
	}
	if ((flags & LIBDH_REMAP_SH) != 0) {
		hint |= hint_get_sh_bit(flags);
	}

	return hint;
}

int libdh_iomem_map(void *vaddr, paddr_t paddr,
		    size_t size, unsigned int prot,
		    unsigned int flags)
{
	int ret = E_HM_OK;
	uint64_t hint;
	uint64_t iomem_flags;

	if (!libdh_check_remap_flags_valid(flags)) {
		return E_HM_INVAL;
	}
	hint = libdh_iomem_map_hint(prot, flags);
	iomem_flags = (flags & LIBDH_REMAP_DMA_BUF) ? MFLAG_IOMAP_TYPE_DMA_BUF : MFLAG_IOMAP_TYPE_DEFAULT;
	ret = actvxactcall_hmcall_mem_iomem_mmap_page(
			true, /* is_fwd */
			true, /* allow_refwd */
			ERR_TO_REF(E_HM_INVAL), /* obsolete vspace_rref */
			paddr,
			vaddr,
			size, hint, iomem_flags);
	if (ret != E_HM_OK) {
		hm_error("pal_iomem_map failed: size=0x%zx, "
			 "prot=0x%x, flags=%x, hint=%"PRIx64": %s\n",
			 size, prot, flags, hint, hmstrerror(ret));
	}

	return ret;
}

int libdh_iomem_map_batch_pid(void *vaddr, const struct mem_raw_ranges *pa_array,
			      unsigned int pa_array_size, unsigned int prot,
			      unsigned int flags, int pid, unsigned long long vr_key)
{
	int ret = E_HM_OK;
	uint64_t hint;
	uint64_t iomem_flags;
	if (!libdh_check_remap_flags_valid(flags)) {
		return E_HM_INVAL;
	}
	hint = libdh_iomem_map_hint(prot, flags);
	iomem_flags = (flags & LIBDH_REMAP_DMA_BUF) ? MFLAG_IOMAP_TYPE_DMA_BUF : MFLAG_IOMAP_TYPE_DEFAULT;
	ret = actvxactcall_hmcall_mem_iomem_mmap_page_batch_fault(false, false,
			vaddr, pa_array, pa_array_size, hint, iomem_flags, pid, vr_key);
	if (ret != E_HM_OK) {
		hm_error("pal_iomem_map_batch_pid failed for %d: pa_array_size=%u, "
			 "prot=0x%x, flags=%x, hint=%"PRIx64": %s\n", pid,
			 pa_array_size, prot, flags, hint, hmstrerror(ret));
	}

	return ret;
}

int libdh_iomem_map_batch_mm(void *vaddr, const struct mem_raw_ranges *pa_array,
			      unsigned int pa_array_size, unsigned int prot,
			      unsigned int flags, unsigned long long vs_key)
{
	int ret = E_HM_OK;
	uint64_t hint;
	uint64_t iomem_flags;

	if (!libdh_check_remap_flags_valid(flags)) {
		return E_HM_INVAL;
	}
	hint = libdh_iomem_map_hint(prot, flags);
	iomem_flags = (flags & LIBDH_REMAP_DMA_BUF) ? MFLAG_IOMAP_TYPE_DMA_BUF : MFLAG_IOMAP_TYPE_DEFAULT;
	ret = actvxactcall_hmcall_mem_iomem_mmap_page_batch_mm(false, false,
			vaddr, pa_array, pa_array_size, hint, iomem_flags, vs_key);
	if (ret != E_HM_OK) {
		hm_error("pal_iomem_map_batch_mm failed for pa_array_size=%u, "
			 "prot=0x%x, flags=%x, hint=%"PRIx64": %s\n", 
			 pa_array_size, prot, flags, hint, hmstrerror(ret));
	}

	return ret;
}

int libdh_iomem_map_batch(void *vaddr, const struct mem_raw_ranges *pa_array,
			  unsigned int pa_array_size, unsigned int prot,
			  unsigned int flags)
{
	int ret = E_HM_OK;
	uint64_t hint;
	uint64_t iomem_flags;

	if (!libdh_check_remap_flags_valid(flags)) {
		return E_HM_INVAL;
	}
	hint = libdh_iomem_map_hint(prot, flags);
	iomem_flags = (flags & LIBDH_REMAP_DMA_BUF) ? MFLAG_IOMAP_TYPE_DMA_BUF : MFLAG_IOMAP_TYPE_DEFAULT;
	ret = actvxactcall_hmcall_mem_iomem_mmap_page_batch(
			true, /* is_fwd */
			true, /* allow_refwd */
			vaddr,
			pa_array, pa_array_size, hint, iomem_flags);
	if (ret != E_HM_OK) {
		hm_error("pal_iomem_map_batch failed: pa_array_size=%u, "
			 "prot=0x%x, flags=%x, hint=%"PRIx64": %s\n",
			 pa_array_size, prot, flags, hint, hmstrerror(ret));
	}

	return ret;
}

int libdh_vm_mmap_batch(const struct mem_raw_ranges *pa_array,
			unsigned int pa_array_size,
			void *vaddr,
			unsigned int prot, unsigned int flags)
{
	int ret;
	uint64_t hint;

	BUG_ON(ptr_to_ulong(vaddr) & PAGE_OFFMASK);

	if (!libdh_check_remap_flags_valid(flags)) {
		return E_HM_INVAL;
	}
	hint = libdh_iomem_map_hint(prot, flags);
	ret = actvxactcall_hmcall_mem_iomem_mmap_page_batch(
			!!(flags & LIBDH_VM_TO_USER), /* `fwd` */
			true, /* `allow refwd` */
			vaddr,
			(void *)pa_array, pa_array_size, hint, 0);
	if (ret != E_HM_OK) {
		hm_error("iomem_mmap_page(hint=%"PRIx64") failed, err=%s\n",
			 hint, hmstrerror(ret));
	}
	return ret;
}

int libdh_vmalloc_mmap_page(unsigned long uaddr, unsigned long kaddr, unsigned long size, unsigned int prot,
	unsigned int flags)
{
	int ret = E_HM_OK;
	uint64_t hint;

	if (!libdh_check_remap_flags_valid(flags)) {
		hm_error("libdh_check_remap_flags_valid failed, flags 0x%x \n", flags);
		return E_HM_INVAL;
	}
	hint = libdh_iomem_map_hint(prot, flags);
	ret = actvxactcall_hmcall_mem_vmalloc_mmap_page(
			true, /* is_fwd */
			true, /* allow_refwd */
			uaddr,
			kaddr,
			size,
			hint);
	if (ret != E_HM_OK) {
		hm_error("vmalloc_mmap_page failed: "
				"prot=0x%x, flags=%x, hint=%"PRIx64": %s\n",
				prot, flags, hint, hmstrerror(ret));
	}

	return ret;
}
