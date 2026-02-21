// SPDX-License-Identifier: GPL-2.0-only

#include <linux/compiler.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/highmem.h>
#include <ion.h>
#include <linux/ion/mm_ion.h>
#include <linux/bg_dmabuf_reclaim/bg_dmabuf_ops.h>
#include <linux/bg_dmabuf_reclaim/bg_dmabuf_info_manager.h>
#if defined(CONFIG_MM_SVM) || defined(CONFIG_ARM_SMMU_V3) || defined(CONFIG_MM_SMMU_V3)
#include <linux/iommu/mm_svm.h>
#endif

#include <asm/cacheflush.h>

#define MAX_CNT 240
/* dma_heap ops */
int __override ion_heap_map_user(struct ion_heap *heap, struct ion_buffer *buffer,
				    struct vm_area_struct *vma)
{
	int i;
	int ret;
	struct pfn_range *array = NULL;
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	unsigned long addr = vma->vm_start;
	unsigned long addr_start = vma->vm_start;
	unsigned long offset = vma->vm_pgoff * PAGE_SIZE;
	struct page *page = NULL;
	unsigned long len = 0;
	int index = 0;

	if (is_bg_buffer_release(buffer)) {
		WARN(1, "%s buf has already release\n", __func__);
		return -EINVAL;
	}

	array = kvmalloc(sizeof(struct pfn_range) * MAX_CNT, GFP_KERNEL);
	if (!array) {
		pr_err("alloc array fail!\n");
		return -ENOMEM;
	}

#if defined(CONFIG_MM_LB) && !defined(CONFIG_HIFORCE_ION_SYSTEM_HEAP)
	if (buffer->plc_id)
		pr_info("%s:magic-%lu,start-0x%p,end-0x%p\n", __func__,
			buffer->magic, (void*)vma->vm_start, (void*)vma->vm_end);

	if (buffer->plc_id && buffer->plc_id != PID_NPU)
		lb_pid_prot_build(buffer->plc_id, &vma->vm_page_prot);
#endif

	/*
	 * when buffer is bg_reclaim type, vm_ops is vma_init.dummy_vm_ops
	 * just replace with bg_buffer ops
	 */
	(void)bg_buffer_register_vma_ops((void *)buffer, vma);

	for_each_sg(table->sgl, sg, table->nents, i) {
		unsigned long remainder = vma->vm_end - addr;
		page = sg_page(sg);
		len = sg->length;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		} else if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}
		len = min(len, remainder);

		/* Add batch for reduce number of actvcall. */
		if (index >= MAX_CNT) {
			ret = remap_pfn_range_batch(vma, addr_start, array,
						    index, vma->vm_page_prot);
			if (ret)
				goto out;
			index = 0;
			addr_start = addr;
		}
		array[index].start = page_to_pfn(page);
		array[index].end = page_to_pfn(page) + len / PAGE_SIZE;
		index++;
		addr += len;
		if (addr >= vma->vm_end)
			goto done;
	}
done:
	if (index == 1 && addr_start == vma->vm_start) {
		ret = remap_pfn_range(vma, addr_start, page_to_pfn(page), len,
				      vma->vm_page_prot);
	} else {
		ret = remap_pfn_range_batch(vma, addr_start, array,
					    index, vma->vm_page_prot);
	}
	if (ret)
		goto out;

#if defined(CONFIG_MM_SVM) || defined(CONFIG_ARM_SMMU_V3) || defined(CONFIG_MM_SMMU_V3)
	if (test_bit(MMF_SVM, &vma->vm_mm->flags))
		mm_svm_flush_cache(vma->vm_mm, vma->vm_start,
				vma->vm_end - vma->vm_start);
#endif

	/* record info error should not effect mmap result */
	(void)record_bg_buffer_uva_info((void *)buffer, vma);

out:
	kvfree(array);
	return ret;
}
