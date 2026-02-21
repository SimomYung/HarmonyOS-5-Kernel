/*
 * drivers/dma-buf/mm/of_mm_dma_heap.c
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
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/fdtable.h>
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/platform_drivers/lpcpu_idle_sleep.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/dmabuf/mm_dma_heap.h>
#ifdef CONFIG_DMABUF_RECLAIM
#include <linux/dmabuf/dmabuf_map_info.h>
#endif

#include <asm/cacheflush.h>
#include <asm/cputype.h>

#include "mm_heap_priv.h"
#include "mm_heap_helpers.h"

#define MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES_THRESHOLD  0x1000000 /* 16MB */

atomic_long_t dmaheap_alloc_total_size;
atomic_long_t dmaheap_magic;

struct rb_root dmaheap_buffers_rb;
struct mutex dmaheap_buffers_lock;

struct dentry *debug_root;
struct dentry *heaps_debug_root;

struct dma_buf *file_to_dma_buf(struct file *file)
{
	return file->private_data;
}

unsigned long get_dmaheap_alloc_total_size(void)
{
	return (unsigned long)atomic_long_read(&dmaheap_alloc_total_size);
}

static void __mm_dmaheap_buffer_add(struct mm_heap_helper_buffer *buffer)
{
	struct rb_node **p = &dmaheap_buffers_rb.rb_node;
	struct rb_node *parent = NULL;
	struct mm_heap_helper_buffer *entry = NULL;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct mm_heap_helper_buffer, node);

		if (buffer < entry) {
			p = &(*p)->rb_left;
		} else if (buffer > entry) {
			p = &(*p)->rb_right;
		} else {
			pr_err("%s: buffer already found", __func__);
			WARN_ON(1);
		}
	}

	rb_link_node(&buffer->node, parent, p);
	rb_insert_color(&buffer->node, &dmaheap_buffers_rb);
}

void mm_dmaheap_buffer_del(struct mm_heap_helper_buffer *buffer)
{
	mutex_lock(&dmaheap_buffers_lock);
	rb_erase(&buffer->node, &dmaheap_buffers_rb);
	mutex_unlock(&dmaheap_buffers_lock);
}

#ifdef CONFIG_DMABUF_RECLAIM
static struct mm_heap_helper_buffer *mm_dmaheap_buffer_get(unsigned long ino_num)
{
	struct rb_root *rbroot = NULL;
	struct rb_node *node = NULL;
	struct mm_heap_helper_buffer *hbuf = NULL;
	struct inode *buf_inode = NULL;

	mutex_lock(&dmaheap_buffers_lock);
	rbroot = &dmaheap_buffers_rb;
	if (!rbroot) {
		pr_err("rbroot is NULL\n");
		return NULL;
	}
	for (node = rb_first(rbroot); node; node = rb_next(node)) {
		hbuf = rb_entry(node, struct mm_heap_helper_buffer, node);
		if (!hbuf || !hbuf->dmabuf) {
			pr_err("hbuf or hbuf->dmabuf\n");
			continue;
		}

		if (hbuf->dmabuf->file == NULL) {
			pr_err("hbuf->dmabuf->file is NULL\n");
			continue;
		}

		buf_inode = file_inode(hbuf->dmabuf->file);
		if (buf_inode == NULL) {
			pr_err("buf_inode is NULL\n");
			continue;
		}

		if (buf_inode->i_ino == ino_num) {
			mutex_unlock(&dmaheap_buffers_lock);
			return hbuf;
		}
	}
	mutex_unlock(&dmaheap_buffers_lock);

	return NULL;
}

struct dma_buf *mm_dmaheap_dmabuf_get(unsigned long ino)
{
	struct mm_heap_helper_buffer *hbuf = NULL;

	hbuf = mm_dmaheap_buffer_get(ino);
	if (!hbuf) {
		pr_err("%s, buffer get fail, ino[%lu]\n", __func__, ino);
		return NULL;
	}

	if (!is_system_heap(hbuf->heap))
		return NULL;

	return hbuf->dmabuf;
}
#endif /* CONFIG_DMABUF_RECLAIM */

void mm_dmaheap_buffer_add(struct mm_heap_helper_buffer *buffer)
{
	mutex_lock(&dmaheap_buffers_lock);
	__mm_dmaheap_buffer_add(buffer);
	mutex_unlock(&dmaheap_buffers_lock);
}

static void dma_heap_flush_cache_all(void *dummy)
{
	/* if need support A73 core have to adapt */
	flush_cache_all();
#ifdef CONFIG_MM_LB_L3_EXTENSION
	lb_ci_cache_exclusive();
#endif
}

#if defined(CONFIG_LIBLINUX_OVERRIDE) && !defined(CONFIG_LIBLINUX_READ_MIDR_EL0)
noinline
#endif
void dma_heap_flush_all_cpus_caches(void)
{
	int cpu;
	cpumask_t mask;
	unsigned int idle_cpus;

	cpumask_clear(&mask);

	preempt_disable();

	idle_cpus = lpcpu_get_idle_cpumask();
	for_each_online_cpu(cpu) { /*lint !e574 */
		if ((idle_cpus & BIT(cpu)) == 0)
			cpumask_set_cpu(cpu, &mask);
	}

	if ((idle_cpus & 0x0f) == 0x0f)
		cpumask_set_cpu(0, &mask);

	if ((idle_cpus & 0xf0) == 0xf0)
		cpumask_set_cpu(4, &mask);

	on_each_cpu_mask(&mask, dma_heap_flush_cache_all, NULL, 1);

	preempt_enable();
}

#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
void dma_heap_flush_all_cpus_caches_raw(void)
{
	int cpu;
	cpumask_t mask;

	cpumask_clear(&mask);

	preempt_disable();

	for_each_online_cpu(cpu)
		cpumask_set_cpu(cpu, &mask);

	on_each_cpu_mask(&mask, dma_heap_flush_cache_all, NULL, 1);

	preempt_enable();
}
#endif

#ifdef CONFIG_MM_LB_FULL_COHERENCY
void dma_ci_sgt(struct sg_table *table)
{
	struct scatterlist *sg = NULL;
	u32 i;

	if (!table || !table->sgl)
		return;

	for_each_sg(table->sgl, sg, table->nents, i) {
		if(PageLB(sg_page(sg)))
			__inval_dcache_area(lb_page_to_virt(sg_page(sg)), sg->length);
		else
			__inval_dcache_area(page_address(sg_page(sg)), sg->length);
	}
}

void dma_ci_pages(struct page *start_pg, u64 size)
{
	if(!start_pg)
		return;
	if(PageLB(start_pg))
		__inval_dcache_area(lb_page_to_virt(start_pg), size);
	else
		__inval_dcache_area(page_address(start_pg), size);
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

#ifdef CONFIG_LIBLINUX_OVERRIDE
noinline
#endif
int mm_dma_heap_sglist_zero(struct scatterlist *sgl, unsigned int nents,
				pgprot_t pgprot)
{
	int p = 0;
	int ret = 0;
	struct sg_page_iter piter;
	struct page *pages[32];

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

int mm_dma_heap_buffer_zero(struct mm_heap_helper_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;
	pgprot_t pgprot = PAGE_KERNEL;

	if (dma_buf_is_uncached(buffer->flags))
		pgprot = pgprot_writecombine(PAGE_KERNEL);

	return mm_dma_heap_sglist_zero(table->sgl, table->nents, pgprot);
}

int mm_dma_heap_pages_zero(struct page *page, size_t size, pgprot_t pgprot)
{
	struct scatterlist sg;

	sg_init_table(&sg, 1);
	sg_set_page(&sg, page, size, 0);
	return mm_dma_heap_sglist_zero(&sg, 1, pgprot);
}

#ifndef CONFIG_LIBLINUX_CDC
int dma_heap_alloc(const char *name, size_t len, unsigned int heap_flags)
{
	struct dma_heap *heap = NULL;
	int fd;

	heap = dma_heap_find(name);
	if (!heap) {
#ifdef CONFIG_DMABUF_UDK_EN
		struct dma_buf *buf = dma_buf_call_heap_alloc(name, len, heap_flags);
		if (!IS_ERR_OR_NULL(buf) && buf->fd > 0) {
			pr_info("%s, [%s] alloc succ fd:%lx\n", __func__, name, buf->fd);
			return buf->fd;
		}
#endif
		pr_err("%s: can not find [%s]\n", __func__, name);
		return -EINVAL;
	}
	dma_heap_put(heap);

	fd = dma_heap_bufferfd_alloc(heap, len, O_RDWR | O_CLOEXEC, heap_flags);
	if (fd < 0) {
		pr_err("%s, [%s] alloc fail\n", __func__, name);
		return fd;
	}

	return fd;
}
#endif

void dma_heap_free(int fd)
{
	ksys_close(fd);
}

bool is_dma_heap_dmabuf(struct dma_buf *dmabuf)
{
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, dmabuf err\n", __func__);
		return false;
	}

	return (dmabuf->ops == &mm_heap_helper_ops);
}

struct device *dmabuf_heap_device_get(struct dma_buf *dmabuf)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	if (!dmabuf)
		return NULL;
#ifdef CONFIG_DMABUF_UDK_EN
	if (dmabuf->from_udk == true)
		return dma_buf_call_device_get(dmabuf);
#endif
	buffer = dmabuf->priv;
	if (!buffer || !buffer->heap) {
		pr_err("%s: buffer or buffer->heap is NULL!\n", __func__);
		return NULL;
	}

	return buffer->heap->heap_dev;
}

struct sg_table *dmabuf_heap_sg_table_get(struct dma_buf *dmabuf)
{
	struct mm_heap_helper_buffer *buffer;
	if (!dmabuf || !is_dma_heap_dmabuf(dmabuf)) {
		return NULL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		return NULL;
	}

	return buffer->sg_table;
}

static struct dma_buf *__cpa_fallback_alloc(const char *heap_name, int h_name_len,
	size_t len, unsigned int fd_flags, unsigned long heap_flags)
{
	struct dma_heap *fallback_heap = NULL;
	struct dma_buf *dmabuf = NULL;

	fallback_heap = dma_heap_find(heap_name);
	if (!fallback_heap) {
		pr_err("%s, can not find [%s]\n", __func__, heap_name);
		return ERR_PTR(-EINVAL);
	}
	dma_heap_put(fallback_heap);

	dmabuf = dma_heap_buffer_alloc(fallback_heap, len, fd_flags, heap_flags);
	if (IS_ERR(dmabuf)) {
		pr_err("%s, fallback alloc %s failed\n", __func__, heap_name);
		return ERR_PTR(-ENOMEM);
	} else {
		return dmabuf;
	}
}

struct dma_buf *cpa_fallback_alloc(struct dma_heap *heap, size_t len,
			    unsigned int fd_flags,
			    unsigned long heap_flags)
{
	struct dma_buf *dmabuf = ERR_PTR(-ENOMEM);

	if (!strncmp(heap->name, "cpa_heap", MM_DMA_HEAP_NAME_LEN)) {
		/* Render Service need use DRM_REDRAW CMA to avoid memory alloc failed */
		if (heap_flags & DMA_HEAP_FLAG_DRM_REDRAW) {
			dmabuf = __cpa_fallback_alloc("protect_drm_redraw_heap",
				strlen("protect_drm_redraw_heap"), len, fd_flags, heap_flags);
			if (IS_ERR(dmabuf))
				pr_err("%s, fallback alloc to mm_cma\n", __func__);
			else
				return dmabuf;
		}

		dmabuf = __cpa_fallback_alloc("protect_heap",
			strlen("protect_heap"), len, fd_flags, heap_flags);
	}
	return dmabuf;
}

static void __dma_heap_flush_buffer_cache(struct mm_heap_helper_buffer *buffer)
{
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	dma_ci_sgt(buffer->sg_table);
#else
	if (buffer->size >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES) {
		dma_heap_flush_all_cpus_caches();
	} else {
		dma_map_sgtable(dma_heap_get_dev(buffer->heap),
			buffer->sg_table, DMA_BIDIRECTIONAL, 0);
		dma_unmap_sgtable(dma_heap_get_dev(buffer->heap),
			buffer->sg_table, DMA_BIDIRECTIONAL, 0);
	}
#endif
}

void dma_heap_flush_buffer_cache(struct mm_heap_helper_buffer *buffer,
				unsigned long heap_flags)
{
	if (IS_ERR_OR_NULL(buffer)) {
		pr_err("%s, mm_heap_helper_buffer err\n", __func__);
		return;
	}

	if (IS_ERR_OR_NULL(buffer->heap) || IS_ERR_OR_NULL(buffer->sg_table)) {
		pr_err("%s, can not find heap or sg_table\n", __func__);
		return;
	}

	if (dma_buf_is_uncached(buffer->flags)) {
		if (buffer->size > DMABUF_LARGE_BUF_SIZE)
			pr_err("%s, size %d start flush\n", __func__, buffer->size);
#ifdef CONFIG_MM_LB_FULL_COHERENCY
		dma_ci_sgt(buffer->sg_table);
#else
		if (buffer->size >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES) {
			dma_heap_flush_all_cpus_caches();
		} else {
			dma_map_sgtable(dma_heap_get_dev(buffer->heap),
				buffer->sg_table, DMA_BIDIRECTIONAL, 0);
			dma_unmap_sgtable(dma_heap_get_dev(buffer->heap),
				buffer->sg_table, DMA_BIDIRECTIONAL, 0);
		}
#endif
		if (buffer->size > DMABUF_LARGE_BUF_SIZE)
			pr_err("%s, size %16zu flush end\n", __func__, buffer->size);
  }
}

#ifdef CONFIG_DMABUF_RECLAIM
void dma_heap_direct_flush_buffer_cache(struct mm_heap_helper_buffer *buffer)
{
	if (IS_ERR_OR_NULL(buffer)) {
		pr_err("%s, mm_heap_helper_buffer err\n", __func__);
		return;
	}

	if (IS_ERR_OR_NULL(buffer->heap) || IS_ERR_OR_NULL(buffer->sg_table)) {
		pr_err("%s, can not find heap or sg_table\n", __func__);
		return;
	}

	__dma_heap_flush_buffer_cache(buffer);
}

bool is_direct_reclaim_buffer(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return false;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return (is_system_heap(buffer->heap) && dma_buf_is_direct_reclaim(buffer->flags));
}

bool is_direct_reclaim_dmabuf(struct dma_buf *dmabuf)
{
	if (!is_dma_heap_dmabuf(dmabuf)) {
		pr_err("%s, is not dma_heap dmabuf\n", __func__);
		return false;
	}
	return is_direct_reclaim_buffer(dmabuf->priv);
}
void buffer_reclaim_status_set(void *buf, bool is_reclaim)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return;

	buffer = (struct mm_heap_helper_buffer *)buf;
	buffer->is_reclaim = is_reclaim;
}

bool buffer_reclaim_status_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return false;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return buffer->is_reclaim;
}

bool buffer_reclaim_status_get_lock(void *buf)
{
	struct mm_heap_helper_buffer *buffer;
	bool is_reclaim = false;

	if (!buf)
		return false;

	buffer = (struct mm_heap_helper_buffer *)buf;
	mutex_lock(&buffer->lock);
	is_reclaim = buffer->is_reclaim;
	mutex_unlock(&buffer->lock);

	return is_reclaim;
}

bool buffer_reclaim_down_read(void *buf)
{
	struct dma_buf *dmabuf = NULL;
	struct mm_heap_helper_buffer *buffer = NULL;
	int ret;

	dmabuf = (struct dma_buf *)buf;
	if (!dmabuf)
		return false;

	buffer = dmabuf->priv;
	if (!buffer)
		return false;

	ret = down_read_killable(&buffer->rwsem);
	if (ret)
		return false;

	return true;
}

void buffer_reclaim_up_read(void *buf)
{
	struct dma_buf *dmabuf = NULL;
	struct mm_heap_helper_buffer *buffer = NULL;

	dmabuf = (struct dma_buf *)buf;
	if (!dmabuf) {
		pr_err("%s, dmabuf invailed, rwsem up failed\n", __func__);
		return;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s, buffer invailed, rwsem up failed\n", __func__);
		return;
	}

	up_read(&buffer->rwsem);
}

void buffer_info_set(void *buf, void *info)
{
	struct mm_heap_helper_buffer *buffer;
	struct buffer_map_info *buf_info;

	if (!buf || !info)
		return;

	buffer = (struct mm_heap_helper_buffer *)buf;
	buf_info = (struct buffer_map_info *)info;
	buffer->info = buf_info;
}

struct buffer_map_info *buffer_info_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return buffer->info;
}

void buffer_attach_num_inc(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return;

	buffer = (struct mm_heap_helper_buffer *)buf;
	atomic_inc(&buffer->attach_num);
}

void buffer_attach_num_dec(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return;

	buffer = (struct mm_heap_helper_buffer *)buf;
	atomic_dec(&buffer->attach_num);
}

int buffer_attach_num_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return atomic_read(&buffer->attach_num);
}

struct rw_semaphore *buffer_rwsem_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer = NULL;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return &buffer->rwsem;
}

struct mutex *buffer_lock_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return &buffer->lock;
}

struct sg_table *buffer_sgtable_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return buffer->sg_table;
}

int *buffer_extid_table_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return buffer->extid_table;
}

int buffer_extid_num_get(void *buf)
{
	struct mm_heap_helper_buffer *buffer;

	if (!buf)
		return NULL;

	buffer = (struct mm_heap_helper_buffer *)buf;
	return buffer->extid_num;
}
#endif /* CONFIG_DMABUF_RECLAIM */

int mm_dma_heap_secmem_get_phys(struct dma_buf *dmabuf,
		phys_addr_t *addr, size_t *len)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct dma_heap *heap = NULL;
	int ret = -EINVAL;

	if (IS_ERR_OR_NULL(dmabuf) || IS_ERR_OR_NULL(addr) ||
						IS_ERR_OR_NULL(len)) {
		pr_err("%s, intput param err\n", __func__);
		return -EINVAL;
	}

	if (!is_dma_heap_dmabuf(dmabuf)) {
		pr_err("%s: dmabuf is not coherent with a dma_heap buffer !\n",
			__func__);
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s: dmabuf priv pointer is NULL!\n", __func__);
		return -EINVAL;
	}

	heap = buffer->heap;
	if (!heap) {
		pr_err("%s, can not find dma_heap\n", __func__);
		return -EINVAL;
	}

	if (heap->ops->get_heap_phys) {
		ret = heap->ops->get_heap_phys(heap, dmabuf, addr, len);
		if (ret)
			pr_err("%s, can not find heap phys\n", __func__);
	}

	return ret;
}

int mm_dma_heap_secmem_get_buffer(int fd, struct sg_table **table, u64 *id,
		      enum SEC_SVC *type)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct dma_buf *dmabuf = NULL;

	if (IS_ERR_OR_NULL(table) || IS_ERR_OR_NULL(id) ||
					IS_ERR_OR_NULL(type)) {
		pr_err("%s, intput param err\n", __func__);
		return -EINVAL;
	}

	dmabuf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, invalid fd!\n", __func__);
		return -EINVAL;
	}

	if (!is_dma_heap_dmabuf(dmabuf)) {
		pr_err("%s: dmabuf is not coherent with a dma_heap buffer !\n",
			__func__);
		goto err;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s: dmabuf->priv is NULL!\n", __func__);
		goto err;
	}

	if (buffer->id) {
		*id = buffer->id;
		*type = SEC_DRM_TEE;
	} else {
		*table = buffer->sg_table;
		*type = SEC_SVC_MAX;
	}

	dma_buf_put(dmabuf);

	return 0;
err:
	dma_buf_put(dmabuf);

	return -EINVAL;
}

static int check_vaddr_bounds(struct mm_struct *mm,
	int fd, unsigned long start, unsigned long length)
{
	struct dma_buf *dmabuf = NULL;
	struct vm_area_struct *vma = NULL;

	if (start >= start + length) {
		pr_err("%s, addr is overflow!\n", __func__);
		return -EINVAL;
	}

	if (!access_ok(start, length)) {
		pr_err("%s,  addr can not access!\n", __func__);
		return -EINVAL;
	}

	if (!PAGE_ALIGNED(start) || !PAGE_ALIGNED(length)) {
		pr_err("%s, PAGE_ALIGNED!\n", __func__);
		return -EINVAL;
	}

	vma = find_vma(mm, start);
	if (!vma) {
		pr_err("%s, vma is null!\n", __func__);
		return -EINVAL;
	}

	if (start + length > vma->vm_end) {
		pr_err("%s, start + length > vma->vm_end(0x%lx)!\n",
			__func__, vma->vm_end);
		return -EINVAL;
	}

	dmabuf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(dmabuf)) {
		pr_err("%s, not dma buf!\n", __func__);
		return -EINVAL;
	}
	if (!vma->vm_file || file_to_dma_buf(vma->vm_file) != dmabuf) {
		pr_err("%s, not dma buf's va!\n", __func__);
		dma_buf_put(dmabuf);
		return -EINVAL;
	}
	dma_buf_put(dmabuf);

	return 0;
}

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
		__dma_map_area((void *)(uintptr_t)uaddr, length, DMA_BIDIRECTIONAL);
		uaccess_ttbr0_disable();
		break;
	case DMA_HEAP_MM_INV_CACHES:
		uaccess_ttbr0_enable();
		__dma_unmap_area((void *)(uintptr_t)uaddr, length, DMA_FROM_DEVICE);
		uaccess_ttbr0_disable();
		break;
	default:
		pr_info("%s, Invalidate CMD(0x%x)\n", __func__, cmd);
		ret = -EINVAL;
	}
	dma_buf_put(dmabuf);

	return ret;
}

#ifdef CONFIG_LIBLINUX_OVERRIDE
noinline
#endif
int mm_dma_heap_cache_operate(int fd, unsigned long uaddr,
	unsigned long offset, unsigned long length, unsigned int cmd)
{
	struct mm_struct *mm = NULL;
	unsigned long start = 0;
	int ret = 0;

#ifndef CONFIG_MM_LB_FULL_COHERENCY
	if (length >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES_THRESHOLD) {
		dma_heap_flush_all_cpus_caches();
		return 0;
	}
#endif

	start = uaddr + offset;
	if (uaddr > start) {
		pr_err("%s, overflow start:0x%lx!\n", __func__, start);
		return -EINVAL;
	}

	mm = get_task_mm(current);
	if (!mm) {
		pr_err("%s, Invalid thread: %d\n", __func__, fd);
		return  -ENOMEM;
	}

	down_read(&mm->mmap_lock);
	if (check_vaddr_bounds(mm, fd, start, length)) {
		pr_err("%s, invalid virt 0x%lx 0x%lx\n",
			__func__, start, length);

		up_read(&mm->mmap_lock);
		mmput(mm);
		return -EINVAL;
	}

	ret = mm_dma_heap_do_cache_op(fd, start, length, cmd);
	if (ret)
		pr_err("%s, cache op fail!\n", __func__);

	up_read(&mm->mmap_lock);
	mmput(mm);

	return ret;
}

static int dmaheap_memtrack_show(struct seq_file *s, void *d)
{
	struct rb_node *n = NULL;

	seq_printf(s, "task_comm	pid		buf_size	magic\n");
	mutex_lock(&dmaheap_buffers_lock);
	for (n = rb_first(&dmaheap_buffers_rb); n; n = rb_next(n)) {
		struct mm_heap_helper_buffer *buffer = rb_entry(n, struct mm_heap_helper_buffer,
				node);

		seq_printf(s, "%-16.s %-16u %-16zu %-16llu\n", buffer->task_comm,
				buffer->pid, buffer->size, buffer->magic);
	}
	mutex_unlock(&dmaheap_buffers_lock);

	return 0;
}
#ifdef CONFIG_DFX_DMABUF_HEAPS
void memtrack_show(void)
{
	unsigned long long total_size = 0;
	struct rb_node *n = NULL;
	pr_info("memtrack_show start\n");
	pr_info("fd		buf_size		magic		buf->tgid		buf->pid		buf->task_comm\n");
	mutex_lock(&dmaheap_buffers_lock);
	for (n = rb_first(&dmaheap_buffers_rb); n; n = rb_next(n)) {
		struct mm_heap_helper_buffer *buffer = rb_entry(n, struct mm_heap_helper_buffer, node);
		struct dma_buf *dmabuf = buffer->dmabuf;
		pr_info("%u %zu %llu %u %u %-.16s-%-.16s\n",
				dmabuf->fd, buffer->size, buffer->magic, dmabuf->tgid,
				dmabuf->pid, dmabuf->exp_task_comm, dmabuf->exp_thread_comm);
		total_size += buffer->size;
    }
	pr_info("dmabuf total alloc size is %llu byte\n", total_size);
    mutex_unlock(&dmaheap_buffers_lock);
}
#endif /* CONFIG_DFX_DMABUF_HEAPS */

static int dmaheap_memtrack_open(struct inode *inode, struct file *file)
{
	return single_open(file,
			dmaheap_memtrack_show, PDE_DATA(file_inode(file)));
}

static const struct proc_ops memtrack_fops = {
	.proc_open = dmaheap_memtrack_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

struct dmaheap_debug_process_heap_args {
	struct seq_file *seq;
	struct task_struct *tsk;
	size_t *total_dmaheap_alloc_size;
};

static int dmaheap_debug_process_heap_cb(const void *data,
					struct file *f, unsigned int fd)
{
	const struct dmaheap_debug_process_heap_args *args = data;
	struct task_struct *tsk = args->tsk;
	struct dma_buf    *dbuf = NULL;
	struct mm_heap_helper_buffer *hbuf = NULL;

	if (!is_dma_buf_file(f))
		return 0;

	dbuf = file_to_dma_buf(f);
	if (!dbuf)
		return 0;

	hbuf = dbuf->priv;
	if (!hbuf)
		return 0;

	*args->total_dmaheap_alloc_size += dbuf->size;
	seq_printf(args->seq,
			"%16s %16u %16u %16zu %16llu %16u %16s %016lx %16s\n",
			tsk->comm, tsk->pid,
			fd, dbuf->size, hbuf->magic,
			hbuf->pid, hbuf->task_comm, hbuf->flags, dbuf->name ? dbuf->name : "null");

	return 0;
}

static int dmaheap_debug_process_heap_show(struct seq_file *s, void *d)
{
	struct task_struct *tsk = NULL;
	size_t task_alloc_size;
	struct dmaheap_debug_process_heap_args cb_args;

	seq_puts(s, "Process dma_heap info:\n");
	seq_puts(s, "----------------------------------------------------\n");
	seq_printf(s, "%16s %16s %16s %16s %16s %16s %16s %16s %16s\n",
			"Process name", "Process ID",
			"fd", "size",
			"magic", "buf->pid",
			"buf->task_comm", "buf->flags", "buf->name");

	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->flags & PF_KTHREAD)
			continue;

		task_alloc_size = 0;
		cb_args.seq = s;
		cb_args.tsk = tsk;
		cb_args.total_dmaheap_alloc_size = &task_alloc_size;

		task_lock(tsk);
		iterate_fd(tsk->files, 0,
			   dmaheap_debug_process_heap_cb, (void *)&cb_args);
		if (task_alloc_size > 0)
			seq_printf(s, "%16s[%-16s] %16zu\n",
					"Total dmaheap size of ",
					tsk->comm, task_alloc_size);
		task_unlock(tsk);
	}
	rcu_read_unlock();

	seq_puts(s, "----------------------------------------------------\n");
	return 0;
}

static int dmaheap_debug_process_heap_open(struct inode *inode, struct file *file)
{
	return single_open(file, dmaheap_debug_process_heap_show, inode->i_private);
}

static const struct file_operations debug_process_heap_fops = {
	.open = dmaheap_debug_process_heap_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct proc_ops debug_process_heap_fops2 = {
	.proc_open = dmaheap_debug_process_heap_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

int mm_dmaheap_debug_node_init(void)
{
	struct proc_dir_entry *entry = NULL;
	struct dentry *debug_file = NULL;

	entry = proc_create_data("memtrack", 0664,
				 NULL, &memtrack_fops, NULL);
	if (!entry)
		pr_err("Failed to create heap debug memtrack\n");

	entry = proc_create_data("dmaheap_process_info", 0444,
				 NULL, &debug_process_heap_fops2, NULL);
	if (!entry)
		pr_err("Failed to create ion buffer debug info\n");

	debug_root = debugfs_create_dir("dmaheap", NULL);
	if (!debug_root) {
		pr_err("dmaheap: failed to create debugfs root directory.\n");
		goto debugfs_done;
	}
	heaps_debug_root = debugfs_create_dir("heaps", debug_root);
	if (!heaps_debug_root) {
		pr_err("dmaheap: failed to create debugfs heaps directory.\n");
		goto debugfs_done;
	}

	debug_file = debugfs_create_file(
		"process_heap_info", 0660, heaps_debug_root, NULL,
		&debug_process_heap_fops);
	if (!debug_file) {
		char buf[256], *path;

		path = dentry_path(heaps_debug_root, buf, 256);
		pr_err("Failed to create process heap debugfs at %s/%s\n",
			path, "process_heap_info");
	}

	return 0;

debugfs_done:
	return -EINVAL;
}

int mm_dmaheap_heap_show(struct dma_heap *heap)
{
	struct rb_node *n = NULL;
	size_t total_size = 0;

	pr_err("%16s %10s %16s  %4s       %8s       %8s      %8s\n",
			"proc", "pid", "size", "map_cnt", "magic", "alloc_time_start", "alloc_time_end");
	pr_err("------------------------------------------------------------------------\n");
	mutex_lock(&dmaheap_buffers_lock);
	for (n = rb_first(&dmaheap_buffers_rb); n; n = rb_next(n)) {
		struct mm_heap_helper_buffer *buffer = rb_entry(n, struct mm_heap_helper_buffer,
						     node);
		if (strncmp(buffer->heap->name, heap->name, MM_DMA_HEAP_NAME_LEN))
			continue;
		total_size += buffer->size;

		pr_err("%16s %10d %16zu %4d %8llu %16lld %16lld\n",
			   buffer->task_comm, buffer->pid,
			   buffer->size, buffer->vmap_cnt, buffer->magic, buffer->alloc_time_start, buffer->alloc_time_end);
	}
	mutex_unlock(&dmaheap_buffers_lock);
	pr_err("------------------------------------------------------------------------\n");
	pr_err("%16s %16zu\n", "total ", total_size);
	pr_err("------------------------------------------------------------------------\n");

	return 0;
}

static int mm_dmaheap_debug_heap_show(struct seq_file *s, void *d)
{
	struct dma_heap *heap = s->private;
	struct rb_node *n = NULL;
	size_t total_size = 0;

	seq_printf(s, "%16s %10s %16s  %4s       %8s       %8s      %8s\n",
			"proc", "pid", "size", "map_cnt", "magic", "alloc_time_start", "alloc_time_end");
	seq_puts(s, "------------------------------------------------------------------------\n");
	mutex_lock(&dmaheap_buffers_lock);
	for (n = rb_first(&dmaheap_buffers_rb); n; n = rb_next(n)) {
		struct mm_heap_helper_buffer *buffer = rb_entry(n, struct mm_heap_helper_buffer,
						     node);
		if (strncmp(buffer->heap->name, heap->name, MM_DMA_HEAP_NAME_LEN))
			continue;
		total_size += buffer->size;

		seq_printf(s, "%16s %10u %16zu %4d %8llu %16llu %16llu\n",
			   buffer->task_comm, buffer->pid,
			   buffer->size, buffer->vmap_cnt, buffer->magic, buffer->alloc_time_start, buffer->alloc_time_end);
	}
	mutex_unlock(&dmaheap_buffers_lock);
	seq_puts(s, "------------------------------------------------------------------------\n");
	seq_printf(s, "%16s %16zu\n", "total ", total_size);
	seq_puts(s, "------------------------------------------------------------------------\n");

	if (!strncmp(heap->name, "system_heap", MM_DMA_HEAP_NAME_LEN))
		mm_dmaheap_system_heap_info(s, heap);

	return 0;
}

static int mm_dmaheap_debug_heap_open(struct inode *inode, struct file *file)
{
	return single_open(file, mm_dmaheap_debug_heap_show, inode->i_private);
}

const struct file_operations debug_heap_fops = {
	.open = mm_dmaheap_debug_heap_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};
