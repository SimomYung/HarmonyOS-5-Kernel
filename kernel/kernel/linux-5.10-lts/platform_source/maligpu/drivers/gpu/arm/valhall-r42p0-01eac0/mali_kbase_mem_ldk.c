/*
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-2.0.html.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 */

#include <mali_kbase.h>
#include <mali_kbase_mem_linux.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <mmu/mali_kbase_mmu.h>

#define MAX_CNT 240
struct vma_batch_reg {
	unsigned long va_start;
	unsigned long va_end;
	unsigned long pfn;
	unsigned long pgcnt;
	struct pfn_range array[MAX_CNT];
	unsigned long index;
};

static vm_fault_t flush_mapping(struct vm_area_struct *vma, struct vma_batch_reg *vreg)
{
	vm_fault_t ret;

	ret = vmf_insert_pfn_batch(vma, vreg->va_start, vreg->array, vreg->index);
	if (ret != VM_FAULT_NOPAGE) {
		pr_err("%s: map pa to user va [0x%lx] failed, ret=%d\n",
		       __func__, vreg->va_start, ret);
	}

	vreg->index = 0;
	vreg->va_start = vreg->va_end;
	return ret;
}

static vm_fault_t kbase_mgm_insert_pfn_batch(struct vm_area_struct *vma, unsigned long addr,
					     unsigned long pfn, pgprot_t pgprot)
{
	vm_fault_t ret = VM_FAULT_NOPAGE;
	struct vma_batch_reg *vreg = (struct vma_batch_reg *)vma->anon_vma;

	if (vreg->pgcnt == 0) {
		vreg->pfn = pfn;
		vreg->pgcnt = 1U;
	} else if (vreg->pfn + 1 != pfn) {
		vreg->array[vreg->index].start = vreg->pfn;
		vreg->array[vreg->index].end = vreg->pfn + vreg->pgcnt;
		vreg->index++;
		vreg->va_end = addr;
		vreg->pfn = pfn;
		vreg->pgcnt = 1U;
	} else {
		vreg->pgcnt++;
	}

	if (vreg->index >= MAX_CNT)
		ret = flush_mapping(vma, vreg);

	return ret;
}

vm_fault_t kbase_mem_fault(struct vm_area_struct *vma, size_t map_start,
			   size_t map_end, struct tagged_addr *pages)
{
	pgoff_t addr;
	vm_fault_t ret = VM_FAULT_SIGBUS;
	struct vma_batch_reg vreg = {0};
	struct anon_vma *anon_old = vma->anon_vma;

	vreg.va_start = vma->vm_start;
	vreg.va_end = vma->vm_start;
	vma->anon_vma = (struct anon_vma *)&vreg;
	addr = (pgoff_t)(vma->vm_start >> PAGE_SHIFT);

	while (map_start < map_end && (addr < vma->vm_end >> PAGE_SHIFT)) {
		ret = kbase_mgm_insert_pfn_batch(vma, addr << PAGE_SHIFT,
			PFN_DOWN(as_phys_addr_t(pages[map_start])), vma->vm_page_prot);
		if (ret != VM_FAULT_NOPAGE)
			return ret;

		map_start++; addr++;
	}

	vma->anon_vma = anon_old;
	if (vreg.pfn != 0) {
		vreg.array[vreg.index].start = vreg.pfn;
		vreg.array[vreg.index].end = vreg.pfn + vreg.pgcnt;
		vreg.index++;
	}
	if (vreg.index != 0)
		ret = flush_mapping(vma, &vreg);

	return ret;
}

unsigned long kbase_unmapped_area(struct vm_unmapped_area_info *info)
{
	info->align_offset = 0;
	info->flags |= VM_UNMAPPED_AREA_TOPDOWN;
	return vm_unmapped_area(info);
}

#ifdef CONFIG_MALI_GPU_RECLAIM
long kbase_reg_vma_zap_pte(struct kbase_context *ctx, struct kbase_va_region *reg, unsigned long addr, unsigned long len)
{
	struct vm_area_struct *vma = NULL;
	struct kbase_cpu_mapping *map = NULL;
	long ret = -1;

	list_for_each_entry(map, &reg->cpu_alloc->mappings, mappings_list) {
		vma = map->vma;
		if (map != NULL && vma != NULL) {
			if (vma->vm_private_data != map) {
				continue;
			}
		}
		if (vma->vm_ops != kbase_mem_get_vm_ops() || vma->vm_private_data == NULL) {
			dev_err(ctx->kbdev->dev, "not kbase vm opts!\n");
			return ret;
		}
		if (!down_write_trylock(&ctx->process_mm->mmap_lock)) {
			dev_warn(ctx->kbdev->dev, "device is busy, down_write_trylock failed\n");
			return ret;
		}
		ret = liblinux_zap_vma_ptes(vma, vma->vm_start, vma->vm_end - vma->vm_start);
		up_write(&ctx->process_mm->mmap_lock);
		if (ret != 0){
			dev_err(ctx->kbdev->dev, "liblinux_zap_vma_ptes failed! addr = 0x%lx size = %lu\n",
				vma->vm_start, (vma->vm_end - vma->vm_start));
			return ret;
		}
    }

	return ret;
}
#endif
