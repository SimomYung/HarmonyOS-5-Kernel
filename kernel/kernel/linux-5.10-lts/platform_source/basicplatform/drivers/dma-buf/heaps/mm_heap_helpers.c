/*
 * drivers/dma-buf/heaps/mm_heap_helpers.c
 *
 * Copyright (C) 2022 Hisilicon, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/device.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/idr.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <uapi/linux/dma-heap.h>
#include <linux/dmabuf/mm_dma_heap.h>
#if defined(CONFIG_MM_SVM) || defined(CONFIG_ARM_SMMU_V3) || defined(CONFIG_MM_SMMU_V3)
#include <linux/iommu/mm_svm.h>
#endif
#ifdef CONFIG_DMABUF_RECLAIM
#include <linux/dmabuf/dmabuf_map_info.h>
#endif
#include "deferred-free-helper.h"
#include "mm_heap_priv.h"
#include "mm_heap_helpers.h"
#ifdef CONFIG_MM_LB
#include <linux/mm_lb/mm_lb.h>
#endif

#define LOCK_RECURSIVE 1

int mm_heap_mutex_lock_recursive(struct mutex *lock)
{
	int is_lock_recursive = 0;

	if (mutex_owner(lock) == current)
		is_lock_recursive = LOCK_RECURSIVE;
	else
		mutex_lock(lock);

	return is_lock_recursive;
}

void mm_heap_mutex_unlock_recursive(struct mutex *lock, int is_lock_recursive)
{
	if (is_lock_recursive == LOCK_RECURSIVE)
		return;

	mutex_unlock(lock);
}

void init_mm_heap_helper_buffer(struct mm_heap_helper_buffer *buffer,
			     void (*free)(struct mm_heap_helper_buffer *))
{
	buffer->priv_virt = NULL;
	mutex_init(&buffer->lock);
	buffer->vmap_cnt = 0;
	buffer->vaddr = NULL;
	INIT_LIST_HEAD(&buffer->attachments);
	buffer->free = free;
}

struct dma_buf *mm_heap_helper_export_dmabuf(struct mm_heap_helper_buffer *buffer,
					  int fd_flags)
{
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);

	exp_info.ops = &mm_heap_helper_ops;
	exp_info.size = buffer->size;
	exp_info.flags = fd_flags;
	exp_info.priv = buffer;

	return dma_buf_export(&exp_info);
}

#ifdef CONFIG_MM_LB
static void mm_heap_helper_buffer_detach_lb(struct mm_heap_helper_buffer *buffer)
{
	int i;
	struct scatterlist *sg = NULL;
	struct sg_table *table = NULL;
	unsigned int plc_id = buffer->plc_id;

	pr_info("%s:magic-%lu,bufsize-0x%lx,lbsize-0x%lx\n", __func__,
		buffer->magic, buffer->size, buffer->lb_size);

	if (!buffer->sg_table || !buffer->sg_table->sgl)
		return;

	table = buffer->sg_table;
	if (plc_id != PID_NPU && !IS_ENABLED(CONFIG_MM_LB_V550)) {
		(void)lb_sg_detach(plc_id, table->sgl, table->nents);
	} else {
		for_each_sg(table->sgl, sg, table->nents, i) {
			if (PageLB(sg_page(sg)))
				(void)lb_pages_detach(plc_id, sg_page(sg),
					sg->length >> PAGE_SHIFT);
		}
	}
}
#endif

void mm_dma_heap_buffer_destroy(struct mm_heap_helper_buffer *buffer)
{
	mm_dmaheap_buffer_del(buffer);

	if (buffer->vmap_cnt > 0) {
		WARN(1, "%s: buffer still mapped in the kernel\n", __func__);
		vunmap(buffer->vaddr);
	}

#ifdef CONFIG_MM_LB_FULL_COHERENCY
	unsigned int osh;
	osh = !!(buffer->flags & DMA_HEAP_FLAG_MM_LB_OSH_MASK);

	if (osh)
		lb_sg_set_osh(buffer->sg_table->sgl, buffer->sg_table->nents, 0);
#endif

#ifdef CONFIG_MM_LB
	/*
	 * will inv cache with gid va,
	 * and need before free
	 */
	if (buffer->plc_id)
		mm_heap_helper_buffer_detach_lb(buffer);
#endif

	atomic_long_sub(buffer->size, &dmaheap_alloc_total_size);

	buffer->free(buffer);
}

static struct sg_table *mm_dma_heap_dup_sg_table(struct sg_table *table)
{
	struct sg_table *new_table =  NULL;
	int ret, i;
	struct scatterlist *sg = NULL, *new_sg = NULL;

	new_table = kzalloc(sizeof(*new_table), GFP_KERNEL);
	if (!new_table)
		return ERR_PTR(-ENOMEM);

	ret = sg_alloc_table(new_table, table->orig_nents, GFP_KERNEL);
	if (ret) {
		kfree(new_table);
		return ERR_PTR(-ENOMEM);
	}

	new_sg = new_table->sgl;
	for_each_sgtable_sg(table, sg, i) {
		sg_set_page(new_sg, sg_page(sg), sg->length, sg->offset);
		sg_dma_address(new_sg) = sg_phys(sg);
		sg_dma_len(new_sg) = sg->length;
		new_sg = sg_next(new_sg);
	}

	return new_table;
}

static int mm_dma_heap_attach(struct dma_buf *dmabuf,
			   struct dma_buf_attachment *attachment)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a = NULL;
	struct sg_table *table = NULL;

	a = kzalloc(sizeof(*a), GFP_KERNEL);
	if (!a)
		return -ENOMEM;
#ifdef CONFIG_DMABUF_RECLAIM
	mutex_lock(&buffer->lock);
	if (is_direct_reclaim_dmabuf(dmabuf)) {
		if (buffer->is_reclaim) {
			pr_err("%s buf has already released\n", __func__);
			mutex_unlock(&buffer->lock);
			kfree(a);
			return -EINVAL;
		}
		buffer_attach_num_inc(buffer);
	}
	mutex_unlock(&buffer->lock);
#endif
	table = mm_dma_heap_dup_sg_table(buffer->sg_table);
	if (IS_ERR(table))
		goto err;

	a->table = table;
	a->dev = attachment->dev;
	INIT_LIST_HEAD(&a->list);
	a->mapped = false;
	a->uncached = dma_buf_is_uncached(buffer->flags);
	attachment->priv = a;

	mutex_lock(&buffer->lock);
	list_add(&a->list, &buffer->attachments);
	mutex_unlock(&buffer->lock);

	return 0;
err:
#ifdef CONFIG_DMABUF_RECLAIM
	mutex_lock(&buffer->lock);
	if (is_direct_reclaim_dmabuf(dmabuf))
		buffer_attach_num_dec(buffer);
	mutex_unlock(&buffer->lock);
#endif
	kfree(a);
	return -ENOMEM;
}

static void mm_dma_heap_detach(struct dma_buf *dmabuf,
			    struct dma_buf_attachment *attachment)
{
	struct dma_heap_attachment *a = attachment->priv;
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;

	mutex_lock(&buffer->lock);
#ifdef CONFIG_DMABUF_RECLAIM
	if (is_direct_reclaim_dmabuf(dmabuf))
		buffer_attach_num_dec(buffer);
#endif
	list_del(&a->list);
	mutex_unlock(&buffer->lock);

	sg_free_table(a->table);
	kfree(a->table);
	kfree(a);
}

static struct sg_table *mm_dma_heap_map_dma_buf(struct dma_buf_attachment *attachment,
				      enum dma_data_direction direction)
{
	struct dma_heap_attachment *a = attachment->priv;
	struct sg_table *table = a->table;

	a->mapped = true;

	return table;
}

static void mm_dma_heap_unmap_dma_buf(struct dma_buf_attachment *attachment,
				   struct sg_table *table,
				   enum dma_data_direction direction)
{
	struct dma_heap_attachment *a = attachment->priv;

	a->mapped = false;
}

static int mm_dma_heap_mmap(struct dma_buf *dmabuf, struct vm_area_struct *vma)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	int ret;

	if (!buffer->heap->ops->map_user) {
		pr_err("%s: this heap does not define a method for mapping to userspace\n",
		       __func__);
		return -EINVAL;
	}

	if (dma_buf_is_uncached(buffer->flags))
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	mutex_lock(&buffer->lock);
	/* now map it to userspace */
	ret = buffer->heap->ops->map_user(buffer->heap, buffer, vma);
	mutex_unlock(&buffer->lock);

	if (ret)
		pr_err("%s: failure mapping buffer to userspace\n",
			__func__);

	return ret;
}

static void mm_dma_heap_dma_buf_release(struct dma_buf *dmabuf)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;

	mm_dma_heap_buffer_destroy(buffer);
}

static int mm_dma_heap_dma_buf_begin_cpu_access(struct dma_buf *dmabuf,
					     enum dma_data_direction direction)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a = NULL;

#ifndef CONFIG_MM_LB_FULL_COHERENCY
	if (dmabuf->size >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES) {
		dma_heap_flush_all_cpus_caches();
	} else {
#endif
		mutex_lock(&buffer->lock);
#ifdef CONFIG_DMABUF_RECLAIM
		if (dma_buf_is_direct_reclaim(buffer->flags) && buffer->is_reclaim) {
			pr_err("%s, buffer has already released, no need to sync cache\n", __func__);
			mutex_unlock(&buffer->lock);
			return 0;
		}
#endif
		if (buffer->vmap_cnt)
			invalidate_kernel_vmap_range(buffer->vaddr, buffer->size);

		dma_sync_sgtable_for_cpu(dma_heap_get_dev(buffer->heap),
				buffer->sg_table, direction);
		if (dma_buf_is_cached(buffer->flags)) {
			list_for_each_entry(a, &buffer->attachments, list) {
				if (!a->mapped)
					continue;
				dma_sync_sgtable_for_cpu(a->dev, a->table, direction);
			}
		}
		mutex_unlock(&buffer->lock);
#ifndef CONFIG_MM_LB_FULL_COHERENCY
	}
#endif

	return 0;
}

static int mm_dma_heap_dma_buf_end_cpu_access(struct dma_buf *dmabuf,
					   enum dma_data_direction direction)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	struct dma_heap_attachment *a = NULL;

#ifndef CONFIG_MM_LB_FULL_COHERENCY
	if (dmabuf->size >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES) {
		dma_heap_flush_all_cpus_caches();
	} else {
#endif
		mutex_lock(&buffer->lock);
#ifdef CONFIG_DMABUF_RECLAIM
		if (dma_buf_is_direct_reclaim(buffer->flags) && buffer->is_reclaim) {
			pr_err("%s, buffer has already released, no need to sync cache\n", __func__);
			mutex_unlock(&buffer->lock);
			return 0;
		}
#endif
		if (buffer->vmap_cnt)
			flush_kernel_vmap_range(buffer->vaddr, buffer->size);

		dma_sync_sgtable_for_device(dma_heap_get_dev(buffer->heap),
				buffer->sg_table, direction);
		if (dma_buf_is_cached(buffer->flags)) {
			list_for_each_entry(a, &buffer->attachments, list) {
				if (!a->mapped)
					continue;
				dma_sync_sgtable_for_device(a->dev, a->table, direction);
			}
		}
		mutex_unlock(&buffer->lock);
#ifndef CONFIG_MM_LB_FULL_COHERENCY
	}
#endif

	return 0;
}

static void *mm_dma_heap_buffer_kmap_get(struct mm_heap_helper_buffer *buffer)
{
	void *vaddr = NULL;

	if (buffer->vmap_cnt) {
		if (buffer->vmap_cnt == INT_MAX)
			return ERR_PTR(-EOVERFLOW);

		buffer->vmap_cnt++;
		return buffer->vaddr;
	}
	vaddr = buffer->heap->ops->map_kernel(buffer->heap, buffer);
	if (WARN_ONCE(!vaddr,
		      "heap->ops->map_kernel should return ERR_PTR on error"))
		return ERR_PTR(-EINVAL);
	if (IS_ERR(vaddr))
		return vaddr;
	buffer->vaddr = vaddr;
	buffer->vmap_cnt++;

	return vaddr;
}

static void *mm_dma_heap_dma_buf_kmap(struct dma_buf *dmabuf, unsigned long offset)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	void *vaddr = NULL;

	if (buffer->heap->ops->map_kernel) {
		mutex_lock(&buffer->lock);
		vaddr = mm_dma_heap_buffer_kmap_get(buffer);
		mutex_unlock(&buffer->lock);
		if (IS_ERR(vaddr))
			return NULL;

		return vaddr + offset * PAGE_SIZE;
	}

	return NULL;
}

static void mm_dma_heap_buffer_kmap_put(struct mm_heap_helper_buffer *buffer)
{
	buffer->vmap_cnt--;
	if (!buffer->vmap_cnt) {
		buffer->heap->ops->unmap_kernel(buffer->heap, buffer);
		buffer->vaddr = NULL;
	}
}

static void mm_dma_heap_dma_buf_kunmap(struct dma_buf *dmabuf, unsigned long offset,
			       void *ptr)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;

	if (buffer->heap->ops->unmap_kernel) {
		mutex_lock(&buffer->lock);
		mm_dma_heap_buffer_kmap_put(buffer);
		mutex_unlock(&buffer->lock);
	}
}
#ifdef CONFIG_MM_LB
static void mm_dma_heap_lb_err(struct sg_table *table, int nents, unsigned int pid)
{
	int i;
	unsigned long len;
	struct scatterlist *sg = NULL;

	for_each_sg(table->sgl, sg, nents, i) {
		if (!sg)
			continue;

		len = sg->length;
		if (PageLB(sg_page(sg)))
			(void)lb_pages_detach(pid, sg_page(sg),
				len >> PAGE_SHIFT);
	}
}
static int mm_dma_heap_attach_lb(struct dma_buf *dmabuf,
	unsigned int pid, unsigned long offset, size_t size)
{
	int i;
	unsigned long len;
	struct scatterlist *sg = NULL;
	struct sg_table *table = NULL;
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;

	if (!buffer) {
		pr_err("%s:buffer is invalid\n", __func__);
		return -EINVAL;
	}

	pr_info("%s:magic-%lu,bufsize-0x%lx,lbsize-0x%lx\n", __func__,
		buffer->magic, buffer->size, size);

	if (!pid || (offset + size) < offset ||
			(offset + size) > buffer->size) {
		pr_err("%s:offset or size is invalid\n", __func__);
		return -EINVAL;
	}

	if (!IS_ALIGNED(size, SZ_1M) || !IS_ALIGNED(offset, SZ_1M)) {
		pr_err("%s:offset or size is not aligned\n", __func__);
		return -EINVAL;
	}

	if (!buffer->sg_table || !buffer->sg_table->sgl)
		return -EINVAL;

	table = buffer->sg_table;

	mutex_lock(&buffer->lock);
	buffer->plc_id = lb_get_pid_by_userpid(pid);
	buffer->offset = offset;
	buffer->lb_size = size;

#ifdef CONFIG_DMABUF_RECLAIM
	if (is_direct_reclaim_dmabuf(dmabuf))
		buffer_attach_num_inc(buffer);
#endif
	for_each_sg(table->sgl, sg, table->nents, i) {
		len = sg->length;
		if (size == 0)
			break;

		if (offset < len) {
			if (lb_pages_attach(pid, sg_page(sg),
					len >> PAGE_SHIFT))
				goto err;

			size = size < (len - offset) ? 0 : (size - (len - offset));
			offset = 0;
		} else {
			offset -= len;
		}
	}

	mutex_unlock(&buffer->lock);

	return 0;
err:
#ifdef CONFIG_DMABUF_RECLAIM
	if (is_direct_reclaim_dmabuf(dmabuf))
		buffer_attach_num_dec(buffer);
#endif
	mm_dma_heap_lb_err(table, i, pid);
	mutex_unlock(&buffer->lock);

	pr_err("%s:lb attach fail\n", __func__);

	return -EINVAL;
}

static int mm_dma_heap_detach_lb(struct dma_buf *dmabuf)
{
	int i;
	unsigned long len;
	struct scatterlist *sg = NULL;
	struct sg_table *table = NULL;
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	unsigned int pid;

	if (!buffer || !buffer->sg_table || !buffer->plc_id)
		return -EINVAL;

	pr_info("%s:magic-%lu,bufsize-0x%lx,lbsize-0x%lx\n", __func__,
		buffer->magic, buffer->size, buffer->lb_size);

	pid = buffer->plc_id;
	table = buffer->sg_table;
	if (!table->sgl)
		return -EINVAL;

	mutex_lock(&buffer->lock);
	for_each_sg(table->sgl, sg, table->nents, i) {
		len = sg->length;
		if (PageLB(sg_page(sg)))
			(void)lb_pages_detach(pid, sg_page(sg),
				len >> PAGE_SHIFT);
	}

	buffer->plc_id = 0;
#ifdef CONFIG_DMABUF_RECLAIM
	if (is_direct_reclaim_dmabuf(dmabuf))
		buffer_attach_num_dec(buffer);
#endif
	mutex_unlock(&buffer->lock);

	return 0;
}

static int mm_dma_heap_mk_lb_prot(struct dma_buf *dmabuf)
{
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	unsigned int prot = (unsigned int)buffer->plc_id << IOMMU_PORT_SHIFT;

	return (int)prot;
}
#endif
static void *mm_dma_heap_dma_buf_vmap(struct dma_buf *dmabuf)
{
	return mm_dma_heap_dma_buf_kmap(dmabuf, 0);
}

static void mm_dma_heap_dma_buf_vunmap(struct dma_buf *dmabuf, void *vaddr)
{
	mm_dma_heap_dma_buf_kunmap(dmabuf, 0, vaddr);
}

static void dma_buf_set_file_fmode(struct mm_heap_helper_buffer *buffer, struct file *file, unsigned int target_flags)
{
	const struct file_operations *fop = file->f_op;
	fmode_t old_fmode = file->f_mode;
	fmode_t new_fmode = OPEN_FMODE(target_flags);
	file->f_mode &= ~(FMODE_WRITE | FMODE_CAN_WRITE);
	file->f_mode |= new_fmode;
 
	if ((new_fmode & FMODE_WRITE) &&
		(fop && (fop->write || fop->write_iter))) {
		file->f_mode |= FMODE_CAN_WRITE;
	}
	dma_buf_pr_info("%s, buffer->magic : 0x%llx, file->f_mode had changed from 0x%x to 0x%x\n", __func__, buffer->magic, old_fmode, file->f_mode);
}

static long mm_dma_heap_dma_buf_set_fd_flags(struct dma_buf *dmabuf, int fd_flags)
{
	unsigned int target_flags;
	unsigned int old_flags;
	struct file *file;
	struct mm_heap_helper_buffer *buffer = dmabuf->priv;
	long ret = 0;

	if ((fd_flags & ~DMA_HEAP_VALID_FD_FLAGS) || (fd_flags & O_ACCMODE) == O_WRONLY) {
		pr_err("%s, input fd_flags is invalid, fd_flags:0x%x\n", __func__, fd_flags);
		return -EINVAL;
	}
	target_flags = fd_flags & (O_ACCMODE | O_NONBLOCK);

	mutex_lock(&dmabuf->lock);
	file = dmabuf->file;
	old_flags = file->f_flags;

#ifdef CONFIG_DFX_MEMCHECK
	pid_t tgid = dmabuf->tgid;
	if ((tgid != current->tgid)) {
        pr_err("%s only hiaisever can call this func, hiaisever_tgid:%d, current_tgid:%d, hiaisever_task_comm:%s, current_task_comm:%s\n",
			__func__, tgid, current->tgid, dmabuf->exp_task_comm, current->group_leader->comm);
        ret = -EINVAL;
		goto out;
	}
#endif

	if (target_flags == old_flags) {
		dma_buf_pr_info("%s fd_flags is aleady %d\n", __func__, fd_flags);
		ret = 0;
		goto out;
	}

	if ((!(buffer->flags & DMA_HEAP_FLAG_NPU_SHARE)) || ((buffer->flags & DMA_HEAP_FLAG_NPU_SHARE) && (buffer->flags & DMA_HEAP_FLAG_DIRECT_RECLAIM))) {
		pr_err("%s current heap_flags: 0x%lx is not support change fd_flags\n", __func__, buffer->flags);
		ret = -EINVAL;
		goto out;
	}

	buffer->adjust_prot_flag = 1;
	file->f_flags = target_flags;
	dma_buf_set_file_fmode(buffer, file, target_flags);
    dma_buf_pr_info("%s buffer->magic : 0x%llx, file->fd_flags had changed from 0x%lx to 0x%lx\n", __func__, buffer->magic, old_flags, file->f_flags);
 
out:
	mutex_unlock(&dmabuf->lock);
 
	return ret;
}

const struct dma_buf_ops mm_heap_helper_ops = {
	.map_dma_buf = mm_dma_heap_map_dma_buf,
	.unmap_dma_buf = mm_dma_heap_unmap_dma_buf,
	.mmap = mm_dma_heap_mmap,
	.release = mm_dma_heap_dma_buf_release,
	.attach = mm_dma_heap_attach,
	.detach = mm_dma_heap_detach,
#ifdef CONFIG_MM_LB
	.attach_lb = mm_dma_heap_attach_lb,
	.detach_lb = mm_dma_heap_detach_lb,
	.mk_prot = mm_dma_heap_mk_lb_prot,
#endif
	.begin_cpu_access = mm_dma_heap_dma_buf_begin_cpu_access,
	.end_cpu_access = mm_dma_heap_dma_buf_end_cpu_access,
	.vmap = mm_dma_heap_dma_buf_vmap,
	.vunmap = mm_dma_heap_dma_buf_vunmap,
	.map = mm_dma_heap_dma_buf_kmap,
	.unmap = mm_dma_heap_dma_buf_kunmap,
	.set_fd_flags = mm_dma_heap_dma_buf_set_fd_flags,
};
#ifdef CONFIG_MM_LB
static int remap_lb_pfn_range(struct mm_heap_helper_buffer *buffer, struct page *page,
	struct vm_area_struct *vma, unsigned long addr, unsigned long len)
{
	int ret = 0;
	unsigned int gid_idx;
	pgprot_t newprot = vma->vm_page_prot;

	gid_idx = lb_page_to_gid(page);
	newprot = pgprot_lb(newprot, gid_idx);
	ret = remap_pfn_range(vma, addr, page_to_pfn(page), len, newprot);

	return ret;
}
#endif
/* dma_heap ops */
#ifdef CONFIG_LIBLINUX_OVERRIDE
noinline
#endif
int mm_dma_heap_map_user(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer, struct vm_area_struct *vma)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	unsigned long addr = vma->vm_start;
	unsigned long offset = vma->vm_pgoff * PAGE_SIZE;
	int i;
	int ret;

#ifdef CONFIG_MM_LB
	if (buffer->plc_id)
		pr_info("%s:magic-%lu,start-0x%lx,end-0x%lx\n", __func__,
			buffer->magic, vma->vm_start, vma->vm_end);

	if (buffer->plc_id && buffer->plc_id != PID_NPU)
		lb_pid_prot_build(buffer->plc_id, &vma->vm_page_prot);
#endif

	for_each_sg(table->sgl, sg, table->nents, i) {
		struct page *page = sg_page(sg);
		unsigned long remainder = vma->vm_end - addr;
		unsigned long len = sg->length;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		} else if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}
		len = min(len, remainder);

#ifdef CONFIG_MM_LB
		if (buffer->plc_id == PID_NPU)
			ret = remap_lb_pfn_range(buffer, page, vma, addr, len);
		else
#endif
		ret = remap_pfn_range(vma, addr, page_to_pfn(page), len,
				vma->vm_page_prot);
		if (ret)
			return ret;
		addr += len;
		if (addr >= vma->vm_end)
			goto done;
	}
done:
#if defined(CONFIG_MM_SVM) || defined(CONFIG_ARM_SMMU_V3) || defined(CONFIG_MM_SMMU_V3)
	if (test_bit(MMF_SVM, &vma->vm_mm->flags))
		mm_svm_flush_cache(vma->vm_mm, vma->vm_start,
				vma->vm_end - vma->vm_start);
#endif
	return 0;
}

void *mm_dma_heap_map_kernel(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer)
{
	void *vaddr = NULL;
	struct scatterlist *sg = NULL;
	struct page **pages = NULL;
	struct page **tmp = NULL;
	struct sg_table *table = buffer->sg_table;
	pgprot_t pgprot = PAGE_KERNEL;
	int npages = PAGE_ALIGN(buffer->size) / PAGE_SIZE;
	int i, j;

#ifdef CONFIG_DMABUF_RECLAIM
	if (dma_buf_is_direct_reclaim(buffer->flags) && buffer->is_reclaim) {
		pr_err("%s, buf has already released\n", __func__);
		return ERR_PTR(-EINVAL);
	}
#endif

	pages = vmalloc(sizeof(struct page *) * npages);
	if (!pages)
		return ERR_PTR(-ENOMEM);

	if (dma_buf_is_uncached(buffer->flags))
		pgprot = pgprot_writecombine(PAGE_KERNEL);

#ifdef CONFIG_MM_LB
	if (buffer->plc_id) {
		pr_info("%s:magic-%lu,lb_pid-%u\n", __func__,
			buffer->magic, buffer->plc_id);
		lb_pid_prot_build(buffer->plc_id, &pgprot);
	}
#endif
	tmp = pages;
	for_each_sg(table->sgl, sg, table->nents, i) {
		int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
		struct page *page = sg_page(sg);

		WARN_ON(i >= npages);
		for (j = 0; j < npages_this_entry; j++)
			*(tmp++) = page++;
	}
	vaddr = vmap(pages, npages, VM_MAP, pgprot);
	vfree(pages);

	if (!vaddr)
		return ERR_PTR(-ENOMEM);
#ifdef CONFIG_DMABUF_RECLAIM
	(void)record_buffer_kva_info(buffer, vaddr, buffer->size, pgprot);
#endif
	return vaddr;
}

void mm_dma_heap_unmap_kernel(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer)
{
#ifdef CONFIG_DMABUF_RECLAIM
	if (dma_buf_is_direct_reclaim(buffer->flags) && buffer->is_reclaim)
		pr_err("%s, buf has already released\n", __func__);

	delete_buffer_kva_info(buffer, buffer->vaddr);
#endif
	vunmap(buffer->vaddr);
}
