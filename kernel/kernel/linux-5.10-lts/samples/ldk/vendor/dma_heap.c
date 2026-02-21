// SPDX-License-Identifier: GPL-2.0-only

#include <linux/compiler.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/highmem.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/dmabuf/mm_dma_heap.h>
#if defined(CONFIG_MM_SVM) || defined(CONFIG_ARM_SMMU_V3) || defined(CONFIG_MM_SMMU_V3)
#include <linux/iommu/mm_svm.h>
#endif

#include <asm/cacheflush.h>

#include "mm_heap_priv.h"
#include "mm_heap_helpers.h"
#include "dma_buf_info.h"

#define MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES_THRESHOLD  0x1000000 /* 16MB */

static void __dma_map_user_area(const void *addr, size_t len, int dir)
{
	unsigned int cmd;

	if (dir == DMA_FROM_DEVICE) {
		cmd = LIBLINUX_PAL_DCACHE_INVAL_RANGE;
	} else {
		cmd = LIBLINUX_PAL_DCACHE_CLEAN_RANGE;
	}

	liblinux_pal_flush_cache_range(cmd | LIBLINUX_PAL_FLUSH_USER_ADDR,
				       (uintptr_t)addr,
				       (uintptr_t)addr + len);
}

static void __dma_unmap_user_area(const void *addr, size_t len, int dir)
{
	if (dir == DMA_TO_DEVICE) {
		return;
	}
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_INVAL_RANGE |
				       LIBLINUX_PAL_FLUSH_USER_ADDR,
				       (uintptr_t)addr,
				       (uintptr_t)addr + len);
}

#ifndef CONFIG_LIBLINUX_READ_MIDR_EL0
void __override dma_heap_flush_all_cpus_caches(void)
{
	flush_cache_all();
#ifdef CONFIG_MM_LB_L3_EXTENSION
	lb_ci_cache_exclusive();
#endif
}
#endif

static int mm_dma_heap_do_cache_op(int fd, unsigned long uaddr,
				   unsigned long length, unsigned int cmd)
{
	struct dma_buf *dmabuf = NULL;
	struct mm_heap_helper_buffer *buffer = NULL;
	unsigned long flags = 0;
	int ret = 0;

	/* Get dma_buf by fd */
	dmabuf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, get dma_buf by fd, error.\n", __func__);
		return -EFAULT;
	}

	/* Get dma_buf by fd */
	if (!is_dma_heap_dmabuf(dmabuf)) {
		pr_err("%s, is not dma-heap buffer.\n", __func__);
		dma_buf_put(dmabuf);
		return -EFAULT;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s: dmabuf priv pointer is NULL!\n", __func__);
		dma_buf_put(dmabuf);
		return -EINVAL;
	}

	/* Get flags from dma_buf */
	flags = buffer->flags;
	if (dma_buf_is_uncached(flags)) {
		dma_buf_put(dmabuf);
		pr_err("%s, dmabuf is noncached!\n", __func__);
		return 0;
	}

	switch (cmd) {
	case DMA_HEAP_MM_CLEAN_CACHES:
		uaccess_ttbr0_enable();
		__dma_map_user_area((void *)(uintptr_t)uaddr, length, DMA_BIDIRECTIONAL);
		uaccess_ttbr0_disable();
		break;
	case DMA_HEAP_MM_INV_CACHES:
		uaccess_ttbr0_enable();
		__dma_unmap_user_area((void *)(uintptr_t)uaddr, length, DMA_FROM_DEVICE);
		uaccess_ttbr0_disable();
		break;
	default:
		pr_info("%s, Invalidate CMD(0x%x)\n", __func__, cmd);
		ret = -EINVAL;
	}
	dma_buf_put(dmabuf);

	return ret;
}

int __override mm_dma_heap_cache_operate(int fd, unsigned long uaddr,
	unsigned long offset, unsigned long length, unsigned int cmd)
{
	unsigned long start = 0;
	int ret = 0;
#ifdef DMABUF_UDK_EN
	struct dma_buf *dmabuf = NULL;

	dmabuf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, get dma_buf by fd, error.\n", __func__);
		return -EFAULT;
	}
	// if dmabuf is created in udk, then deal in udk
	if (dmabuf->from_udk == true) {
		return dma_buf_call_heap_cache_operate(fd, uaddr, offset, length, cmd);
	}
#endif
#ifndef CONFIG_MM_LB_FULL_COHERENCY
	if (length >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES_THRESHOLD) {
		dma_heap_flush_all_cpus_caches();
		return 0;
	}
#endif

	start = uaddr + offset;
	if (uaddr > start) {
		pr_err("%s, overflow start:0x%p!\n", __func__, (void*)start);
		return -EINVAL;
	}

	ret = mm_dma_heap_do_cache_op(fd, start, length, cmd);
	if (ret)
		pr_err("%s, cache op fail!\n", __func__);

	return ret;
}

static void mm_dma_buf_adjust_prot(struct vm_area_struct *vma, struct file *file, struct mm_heap_helper_buffer *buffer)
{
	unsigned long vm_flags = vma->vm_flags;
	pgprot_t vm_page_prot = vma->vm_page_prot;
 
	if (!file) {
		pr_err("%s, file is NULL\n", __func__);
		return;
	}
 
	if (!(file->f_mode & FMODE_WRITE)) {
		if (vm_flags & (VM_WRITE | VM_MAYWRITE)) {
			vma->vm_flags &= ~(VM_WRITE | VM_MAYWRITE);
			vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) & (~PTE_WRITE));
			pr_info("%s, file not support write, buffer->magic : 0x%llx, vma->vm_flags change from 0x%lx to 0x%lx, vma->vm_page_prot change from 0x%lx to 0x%lx\n",
				__func__, buffer->magic, vm_flags, vma->vm_flags, pgprot_val(vm_page_prot), pgprot_val(vma->vm_page_prot));
		}		
	}
}

#define MAX_CNT 240
/* dma_heap ops */
int __override mm_dma_heap_map_user(struct dma_heap *heap, struct mm_heap_helper_buffer *buffer,
				    struct vm_area_struct *vma)
{
	int i;
	int ret;
	struct pfn_range array[MAX_CNT];
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	unsigned long addr = vma->vm_start;
	unsigned long addr_start = vma->vm_start;
	unsigned long offset = vma->vm_pgoff * PAGE_SIZE;
	struct page *page = NULL;
	unsigned long len = 0;
	int index = 0;
	pgprot_t newprot;
	pgprot_t oldprot;
	unsigned int gid = 0;
	struct dma_buf *dmabuf = buffer->dmabuf;

	if (!dmabuf) {
		pr_err("%s, dmabuf is NULL\n", __func__);
        return -ENOMEM;
    }

	if ((buffer->adjust_prot_flag == 1) && (buffer->flags & DMA_HEAP_FLAG_NPU_SHARE)) {
		mm_dma_buf_adjust_prot(vma, dmabuf->file, buffer);
	}

	newprot = vma->vm_page_prot;
	oldprot = vma->vm_page_prot;

#ifdef CONFIG_MM_LB
	if (buffer->plc_id)
		pr_info("%s: magic-%lu,start-0x%p,end-0x%p,plc_id-%u,offset-0x%lx\n",
			__func__, buffer->magic, (void*)vma->vm_start, (void*)vma->vm_end, buffer->plc_id, offset);
#endif

	for_each_sg(table->sgl, sg, table->nents, i) {
		unsigned long remainder = vma->vm_end - addr;
		page = sg_page(sg);
		len = sg->length;
#ifdef CONFIG_MM_LB_GID_PBHA
		gid = lb_page_to_gid(page);
		newprot = pgprot_lb(vma->vm_page_prot, gid);
#endif
#ifdef CONFIG_MM_LB_FULL_COHERENCY
		if (lb_get_page_osh_flag(page))
			newprot = __pgprot_modify(newprot, PTE_SH_MASK, PTE_OUTER_SHARED);
#endif

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		} else if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}
		len = min(len, remainder);

		if (index == 0 && pgprot_val(newprot) != pgprot_val(oldprot))
			oldprot = newprot;

		/* Add batch for reduce number of actvcall. */
		if (index >= MAX_CNT || pgprot_val(newprot) != pgprot_val(oldprot)) {
			ret = remap_pfn_range_batch(vma, addr_start, array,
				index, oldprot);
			if (ret)
				return ret;
			index = 0;
			addr_start = addr;
			oldprot = newprot;
		}
#if defined(CONFIG_MM_LB) && !defined(CONFIG_MM_LB_GID_PBHA)
		if (buffer->plc_id == PID_NPU || buffer->plc_id == PID_ISPNN)
			gid = lb_page_to_gid(page);

		array[index].start = page_to_pfn_lb(page, gid);
		array[index].end = page_to_pfn_lb(page, gid) + len / PAGE_SIZE;
#else
		array[index].start = page_to_pfn(page);
		array[index].end = page_to_pfn(page) + len / PAGE_SIZE;
#endif
		index++;
		addr += len;
		if (addr >= vma->vm_end)
			goto done;
	}
done:
	/* PBHA only support batch map */
	if (index == 1 && addr_start == vma->vm_start && (!IS_ENABLED(CONFIG_MM_LB_GID_PBHA) || gid == 0)) {
#if defined(CONFIG_MM_LB) && !defined(CONFIG_MM_LB_GID_PBHA)
		if (buffer->plc_id == PID_NPU || buffer->plc_id == PID_ISPNN)
			gid = lb_page_to_gid(page);

		ret = remap_pfn_range(vma, addr_start, page_to_pfn_lb(page, gid), len,
				      vma->vm_page_prot);
#else
		ret = remap_pfn_range(vma, addr_start, page_to_pfn(page), len,
				      newprot);
#endif
	} else {
		ret = remap_pfn_range_batch(vma, addr_start, array,
					    index, newprot);
	}
	if (ret)
		return ret;

#ifdef CONFIG_LDK_SVM
	if (is_svm_task(current->tgid))
		mm_svm_flush_cache(current->tgid, vma->vm_start,
				vma->vm_end - vma->vm_start);
#endif
	return 0;
}

#ifdef CONFIG_DMABUF_RECLAIM
int mm_dmabuf_page_rmap_user(struct dma_buf *dmabuf, struct vm_area_struct *vma)
{
	struct pfn_range *array;
	struct mm_heap_helper_buffer *buffer;
	struct sg_table *sgt = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	unsigned long addr = vma->vm_start;
	unsigned long offset = vma->vm_pgoff * PAGE_SIZE;
	unsigned long len = 0;
	int i, ret;

	if (!dmabuf || !dmabuf->priv) {
		pr_err("para err!\n");
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	sgt = buffer->sg_table;

	array = kvmalloc(sgt->nents * sizeof(*array), GFP_KERNEL);
	if (!array) {
		pr_err("alloc array fail!\n");
		return -ENOMEM;
	}
	for_each_sg(sgt->sgl, sg, sgt->nents, i) {
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

			array[i].start = page_to_pfn(page);
			array[i].end = page_to_pfn(page) + len / PAGE_SIZE;

			addr += len;
			if (addr >= vma->vm_end) {
				i += 1;
				goto done;
			}
	}
done:
	ret = remap_pfn_range_batch_sp(vma, vma->vm_start, array,
						    i, vma->vm_page_prot);
	kvfree(array);

	return ret;
}

int mm_dmabuf_page_runmap_user(struct vm_area_struct *vma)
{
	return liblinux_pal_vm_zap_ptes(vma->vm_start, vma->vm_end - vma->vm_start, VSPACE_KEY(vma->vm_mm));
}

int mm_dmabuf_page_runmap_kernel(unsigned long kva, unsigned long size)
{
	return liblinux_pal_vm_zap(kva, size, 0);
}

#endif

static int mm_dma_heap_clear_pages(struct page **pages, int num, pgprot_t pgprot)
{
	void *addr = vmap(pages, num, VM_MAP, pgprot);

	if (!addr)
		return -ENOMEM;
	memset(addr, 0, PAGE_SIZE * num);
	vunmap(addr);

	return 0;
}

static int mm_dma_heap_sglist_zero_no_cache(struct scatterlist *sgl, unsigned int nents,
					    pgprot_t pgprot)
{
	int p = 0;
	int ret = 0;
	struct sg_page_iter piter;
	struct page *pages[256];

	for_each_sg_page(sgl, &piter, nents, 0) {
		pages[p++] = sg_page_iter_page(&piter);
		if (p == ARRAY_SIZE(pages)) {
			ret = mm_dma_heap_clear_pages(pages, p, pgprot);
			if (ret)
				return ret;
			p = 0;
		}
	}
	if (p)
		ret = mm_dma_heap_clear_pages(pages, p, pgprot);

	return ret;
}

int __override mm_dma_heap_sglist_zero(struct scatterlist *sgl, unsigned int nents,
				       pgprot_t pgprot)
{
	return  mm_dma_heap_sglist_zero_no_cache(sgl, nents, pgprot);
}

#ifdef CONFIG_DMABUF_MM
static bool is_mm_heap_helper_buffer(struct dma_buf *dmabuf)
{
	return dmabuf->ops == &mm_heap_helper_ops;
}

int dmaheap_debug_dma_buf_info(struct file *file, struct dma_buf_info *file_info)
{
	struct dma_buf *dbuf = NULL;
	struct mm_heap_helper_buffer *hbuf = NULL;
	int ret;

	if (file == NULL || file_info == NULL)
		return -EINVAL;

	if (!is_dma_buf_file(file))
		return -EINVAL;

	dbuf = file_to_dma_buf(file);
	if (!dbuf)
		return -EINVAL;

	if (!is_mm_heap_helper_buffer(dbuf))
		return -EINVAL;

	hbuf = dbuf->priv;
	if (!hbuf)
		return -EINVAL;

	file_info->size = dbuf->size;
	file_info->magic = hbuf->magic;
	file_info->allocator_pid = hbuf->pid;
	ret = strncpy_s(file_info->allocator_comm, TASK_COMM_LEN, hbuf->task_comm, TASK_COMM_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->allocator_comm copy failed, err=%d\n", ret);
	}
	return 0;
}
#else /* CONFIG_DMABUF_MM */
int dmaheap_debug_dma_buf_info(struct file *file, struct dma_buf_info *file_info)
{
	return -EPERM;
}
#endif /* CONFIG_DMABUF_MM */
EXPORT_SYMBOL(dmaheap_debug_dma_buf_info);

#if defined(CONFIG_DFX_MEMCHECK) || defined(CONFIG_DMABUF_PROCESS_INFO)
int memcheck_debug_ion_info(struct file *file, struct ion_info *file_info)
{
	struct dma_buf *dbuf = NULL;
	int ret;

	if (file == NULL || file_info == NULL)
		return -EINVAL;

	if (!is_dma_buf_file(file))
		return -EPERM;

	dbuf = file_to_dma_buf(file);
	if (!dbuf)
		return -EPERM;

	file_info->size = dbuf->size;
	file_info->i_ino = file_inode(dbuf->file)->i_ino;
	file_info->allocator_tgid = dbuf->tgid;
	file_info->allocator_pid = dbuf->pid;
	file_info->reclaimable = is_direct_reclaim_dmabuf(dbuf);
	if (file_info->reclaimable == false) {
		file_info->is_reclaimed = false;
	} else {
		file_info->is_reclaimed = buffer_reclaim_status_get_lock(dbuf->priv);
	}

	ret = strncpy_s(file_info->exp_task_comm, TASK_COMM_LEN, dbuf->exp_task_comm, TASK_COMM_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->exp_task_comm copy failed, err=%d\n", ret);
	}
	ret = strncpy_s(file_info->exp_thread_comm, TASK_COMM_LEN, dbuf->exp_thread_comm, TASK_COMM_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->exp_thread_comm copy failed, err=%d\n", ret);
	}

	spin_lock(&dbuf->name_lock);
	if (dbuf->name)
		ret = strncpy_s(file_info->name, DMA_BUF_NAME_LEN, dbuf->name, DMA_BUF_NAME_LEN - 1);
	else
		ret = strncpy_s(file_info->name, DMA_BUF_NAME_LEN, "NULL", DMA_BUF_NAME_LEN - 1);
	spin_unlock(&dbuf->name_lock);
	if (ret < 0) {
		pr_err("file_info->name copy failed, err=%d\n", ret);
	}

	if (dbuf->exp_name)
		ret = strncpy_s(file_info->exp_name, DMA_BUF_NAME_LEN, dbuf->exp_name, DMA_BUF_NAME_LEN - 1);
	else
		ret = strncpy_s(file_info->exp_name, DMA_BUF_NAME_LEN, "NULL", DMA_BUF_NAME_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->exp_name copy failed, err=%d\n", ret);
	}

	return 0;
}
#else /* CONFIG_DFX_MEMCHECK || CONFIG_DMABUF_PROCESS_INFO */
int memcheck_debug_ion_info(struct file *file, struct ion_info *file_info)
{
	return -EPERM;
}
#endif /* CONFIG_DFX_MEMCHECK || CONFIG_DMABUF_PROCESS_INFO */
EXPORT_SYMBOL(memcheck_debug_ion_info);

#ifdef CONFIG_DMABUF_MM
void __override mm_dmaheap_init_dump(struct mm_heap_helper_buffer *buffer)
{
	struct dma_buf *dmabuf = buffer->dmabuf;
	if (dmabuf == NULL) {
		strlcpy(buffer->task_comm, "invalid task",
				sizeof(buffer->task_comm));
		buffer->pid = 0;
	} else {
		(void)strncpy_s(buffer->task_comm, TASK_COMM_LEN, dmabuf->exp_task_comm,
				TASK_COMM_LEN - 1);
		buffer->pid = dmabuf->tgid;
	}
}
EXPORT_SYMBOL(mm_dmaheap_init_dump);
#endif
