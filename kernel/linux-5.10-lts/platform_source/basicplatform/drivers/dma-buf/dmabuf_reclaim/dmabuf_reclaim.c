/*
 * dmabuf_reclaim.c
 *
 * Copyright (C) 2024 Hisilicon Technologies Co., Ltd.
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
#include <asm/page.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/iommu.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/pagemap.h>
#include <linux/proc_fs.h>
#include <linux/scatterlist.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/dmabuf/dmabuf_reclaim.h>
#include <linux/dmabuf/dmabuf_swap.h>
#include <linux/dmabuf/dmabuf_map_info.h>

#include <asm/tlbflush.h>
#include <securec.h>

#include <linux/delay.h>
#include <linux/mutex.h>

static struct dmabuf_gpu_callback dmabuf_gpu_cb[DMABUF_CB_GPU_MAX];
static DEFINE_MUTEX(dmabuf_gpu_cb_mutex);
static DEFINE_MUTEX(reclaim_dmabuf_enable_mutex);
static bool reclaim_dmabuf_enable;

unsigned long __pgprot_to_posix_prot(pgprot_t pgprot);

int dmabuf_register_gpu_callback(enum dmabuf_gpu_cb_type type,
	int (*cb)(struct dma_buf *dmabuf))
{
	unsigned int index = type;

	if (cb == NULL || type >= DMABUF_CB_GPU_MAX)
		return -EINVAL;

	mutex_lock(&dmabuf_gpu_cb_mutex);

	if (dmabuf_gpu_cb[index].cb != NULL) {
		pr_err("%s, had register callback func already\n", __func__);
		mutex_unlock(&dmabuf_gpu_cb_mutex);
		return -EEXIST;
	}

	dmabuf_gpu_cb[index].cb = cb;
	mutex_unlock(&dmabuf_gpu_cb_mutex);

	return 0;
}

int try_to_unmap_uva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->uva_info_mutex);
	list_for_each_safe(pos, q, &info->uva_info) {
		struct uva_map_info *uva_info =
				list_entry(pos, struct uva_map_info, list);

		ret = mm_dmabuf_page_runmap_user(uva_info->vma);
		if (ret)
			break;
	}
	mutex_unlock(&info->uva_info_mutex);

	return ret;
}

int try_to_map_uva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->uva_info_mutex);
	list_for_each_safe(pos, q, &info->uva_info) {
		struct uva_map_info *uva_info =
				list_entry(pos, struct uva_map_info, list);

		ret = mm_dmabuf_page_rmap_user(dmabuf, uva_info->vma);
		if (ret)
			break;
	}
	mutex_unlock(&info->uva_info_mutex);

	return ret;
}

int try_to_unmap_kva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->kva_info_mutex);
	list_for_each_safe(pos, q, &info->kva_info) {
		struct kva_map_info *kva_info =
				list_entry(pos, struct kva_map_info, list);

		mm_dmabuf_page_runmap_kernel(kva_info->kva, kva_info->len);
	}
	mutex_unlock(&info->kva_info_mutex);

	return ret;
}

static int to_dmabuf_pages(struct dma_buf *dmabuf, struct page **pages, unsigned int npages)
{
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page **tmp = pages;
	int i, j;

	if (!dmabuf->priv)
		return -EINVAL;

	table = buffer_sgtable_get(dmabuf->priv);
	if (!table)
		return -EINVAL;

	for_each_sg(table->sgl, sg, table->nents, i) {
		int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
		struct page *page = sg_page(sg);

		WARN_ON(i >= npages);
		for (j = 0; j < npages_this_entry; j++)
			*(tmp++) = page++;
	}

	return 0;
}

int try_to_map_kva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	struct page **pages = NULL;
	unsigned int npages;
	unsigned int flag;
	int ret = 0;

	npages = PAGE_ALIGN(dmabuf->size) / PAGE_SIZE;
	pages = kvmalloc(sizeof(struct page *) * npages, GFP_KERNEL);
	if (!pages)
		return -EINVAL;

	ret = to_dmabuf_pages(dmabuf, pages, npages);
	if (ret) {
		ret = -EINVAL;
		goto done;
	}

	info = buffer_info_get(dmabuf->priv);
	if (!info) {
		ret = -EINVAL;
		goto done;
	}

	mutex_lock(&info->kva_info_mutex);
	list_for_each_safe(pos, q, &info->kva_info) {
		struct kva_map_info *kva_info =
				list_entry(pos, struct kva_map_info, list);

		flag = LIBLINUX_PAL_REMAP_CACHE;
		if (pgprot_val(pgprot_writecombine(kva_info->prot)) == pgprot_val(kva_info->prot))
			flag = LIBLINUX_PAL_REMAP_NORMAL_NC;

		ret = do_vmap(kva_info->kva, pages, npages, flag, __pgprot_to_posix_prot(kva_info->prot));
		if (ret < 0)
			break;
	}
	mutex_unlock(&info->kva_info_mutex);

done:
	kvfree(pages);

	return ret;
}

int try_to_unmap_iova(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->iova_info_mutex);
	list_for_each_safe(pos, q, &info->iova_info) {
		struct iova_map_info *iova_info =
				list_entry(pos, struct iova_map_info, list);

		ret = mm_iommu_unmap_dmabuf_page(iova_info->dev, iova_info->iova, iova_info->len);
		if (ret)
			break;
	}
	mutex_unlock(&info->iova_info_mutex);

	return ret;
}

int try_to_map_iova(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->iova_info_mutex);
	list_for_each_safe(pos, q, &info->iova_info) {
		struct iova_map_info *iova_info =
				list_entry(pos, struct iova_map_info, list);

		ret = mm_iommu_map_dmabuf_page(iova_info->dev, dmabuf, iova_info->iova,
			iova_info->len, iova_info->prot);
		if (ret)
			break;
	}
	mutex_unlock(&info->iova_info_mutex);

	return ret;
}

/* gva is not support now, if need, should add gva map/unmap call */
int try_to_unmap_gva(struct dma_buf *dmabuf)
{
	int ret = 0;

	if (dmabuf_gpu_cb[DMABUF_CB_GPU_UNMAP].cb)
		dmabuf_gpu_cb[DMABUF_CB_GPU_UNMAP].cb(dmabuf);

	return ret;
}

int try_to_map_gva(struct dma_buf *dmabuf)
{
	int ret = 0;

	if (dmabuf_gpu_cb[DMABUF_CB_GPU_MAP].cb)
		dmabuf_gpu_cb[DMABUF_CB_GPU_MAP].cb(dmabuf);

	return ret;
}

/* if need support gva, should add gva map/unmap in this func */
static int try_to_unmap_dmabuf_page(struct dma_buf *dmabuf)
{
	int ret = 0;
	ret = try_to_unmap_uva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_uva fail\n", __func__);
		return ret;
	}

	ret = try_to_unmap_kva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_kva fail\n", __func__);
		goto map_uva;
	}

	ret = try_to_unmap_iova(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_iova fail\n", __func__);
		goto map_kva;
	}

	return 0;

map_kva:
	(void)try_to_map_kva(dmabuf);
map_uva:
	(void)try_to_map_uva(dmabuf);

	return ret;
}

static int try_to_map_dmabuf_page(struct dma_buf *dmabuf)
{
	int ret = 0;
	ret = try_to_map_uva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_uva fail\n", __func__);
		return ret;
	}

	ret = try_to_map_kva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_kva fail\n", __func__);
		goto unmap_uva;
	}

	ret = try_to_map_iova(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_uva fail\n", __func__);
		goto unmap_kva;
	}

	return 0;
unmap_kva:
	(void)try_to_unmap_kva(dmabuf);
unmap_uva:
	(void)try_to_unmap_uva(dmabuf);

	return ret;
}

/* Not used currently */
static void init_dmabuf_page(struct dma_buf *dmabuf)
{
}

/* Not used currently */
static void clear_dmabuf_page(struct dma_buf *dmabuf)
{
}

static int __dmabuf_reclaim(struct dma_buf *dmabuf)
{
	int ret = 0;

	ret = dmabuf_page_swap_alloc_extid(dmabuf);
	if (ret) {
		pr_err("%s, alloc dmabuf extentid fail\n", __func__);
		return ret;
	}

	ret = try_to_unmap_dmabuf_page(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_dmabuf_page fail\n", __func__);
		goto free_extid;
	}

	dma_heap_direct_flush_buffer_cache(dmabuf->priv);

	/* dmabuf page data swap out */
	ret = dmabuf_page_swap_out(dmabuf);
	if (ret) {
		pr_err("%s, dmabuf page swap out fail\n", __func__);
		goto map_dmabuf;
	}

	clear_dmabuf_page(dmabuf);

	ret = buffer_free_sgtable(dmabuf->priv);
	if (ret) {
		pr_err("%s, dmabuf free sgtable fail\n", __func__);
		goto init_dmabuf;
	}

	pr_debug("%s, dmabuf page reclaim succ\n", __func__);
	return ret;

init_dmabuf:
	init_dmabuf_page(dmabuf);
map_dmabuf:
	(void)try_to_map_dmabuf_page(dmabuf);
free_extid:
	dmabuf_page_swap_free_extid(dmabuf);

	return ret;
}

int dmabuf_reclaim(struct dma_buf *dmabuf)
{
	struct mutex *lock = NULL;
	struct rw_semaphore *rwsem = NULL;
	int ret = 0;

	if (!dmabuf || !dmabuf->priv) {
		pr_err("%s, dmabuf or dmabuf->priv is NULL\n", __func__);
		return 0;
	}

	rwsem = buffer_rwsem_get(dmabuf->priv);
	if (!rwsem) {
		pr_err("%s, buffer get rwsem fail\n", __func__);
		return 0;
	}

	ret = down_write_killable(rwsem);
	if (ret) {
		pr_err("%s, buffer down-ret-%d, exit\n", __func__, ret);
		return 0;
	}

	lock = buffer_lock_get(dmabuf->priv);
	if (!lock) {
		pr_err("%s, buffer get lock fail\n", __func__);
		goto release_rwsem;
	}

	mutex_lock(lock);
	if (!is_direct_reclaim_dmabuf(dmabuf))
		goto done;

	if (buffer_reclaim_status_get(dmabuf->priv))
		goto done;

	if (buffer_attach_num_get(dmabuf->priv)) {
		ret = -EINVAL;
		pr_err("%s: dmabuf attach num > 0\n", __func__);
		goto done;
	}

	ret = __dmabuf_reclaim(dmabuf);
	if (ret) {
		pr_err("%s, fail, ret[%d]\n", __func__, ret);
		goto done;
	}

	buffer_reclaim_status_set(dmabuf->priv, true);
done:
	mutex_unlock(lock);
release_rwsem:
	up_write(rwsem);

	return ret;
}

int mm_dmabuf_assign_reclaim(unsigned long ino_num)
{
	struct dma_buf *dmabuf = NULL;

	dmabuf = mm_dmaheap_dmabuf_get(ino_num);
	if (!dmabuf) {
		pr_err("%s, dmabuf get by ino_num[%lu] fail\n", __func__, ino_num);
		return 0;
	}

	return dmabuf_reclaim(dmabuf);
}

static int __dmabuf_resume(struct dma_buf *dmabuf)
{
	int ret = 0;

	ret = buffer_alloc_sgtable(dmabuf->priv);
	if (ret) {
		pr_err("%s, dmabuf realloc sgtable fail\n", __func__);
		return -EINVAL;
	}
 
	/* dmabuf page data swap in */
	ret = dmabuf_page_swap_in(dmabuf);
	if (ret) {
		pr_err("%s, dmabuf page swap in fail\n", __func__);
		goto free_sgtable;
	}

	dma_heap_direct_flush_buffer_cache(dmabuf->priv);

	ret = try_to_map_dmabuf_page(dmabuf);
	if (ret) {
		pr_err("%s, try_to_map_dmabuf_page fail\n", __func__);
		goto free_sgtable;
	}

	init_dmabuf_page(dmabuf);

	ret = dmabuf_page_swap_free_extid(dmabuf);
	if (ret)
		pr_err("%s, free extid fail\n", __func__);

	return ret;

free_sgtable:
	(void)buffer_free_sgtable(dmabuf->priv);

	return ret;
}

int dmabuf_resume(struct dma_buf *dmabuf)
{
	struct mutex *lock = NULL;
	struct rw_semaphore *rwsem = NULL;
	int ret = 0;

	if (!dmabuf || !dmabuf->priv) {
		pr_err("%s, dmabuf or dmabuf->priv is NULL\n", __func__);
		return 0;
	}

	rwsem = buffer_rwsem_get(dmabuf->priv);
	if (!rwsem) {
		pr_err("%s, buffer get rwsem fail\n", __func__);
		return 0;
	}

	ret = down_write_killable(rwsem);
	if (ret) {
		pr_err("%s, buffer down-ret-%d, exit\n", __func__, ret);
		return 0;
	}

	lock = buffer_lock_get(dmabuf->priv);
	if (!lock) {
		pr_err("%s, buffer get lock fail\n", __func__);
		goto release_rwsem;
	}

	mutex_lock(lock);
	if (!is_direct_reclaim_dmabuf(dmabuf))
		goto done;

	if (!buffer_reclaim_status_get(dmabuf->priv))
		goto done;

	if (buffer_attach_num_get(dmabuf->priv)) {
		ret = -EINVAL;
		pr_err("%s: dmabuf attach num > 0 err!!!!\n", __func__);
		goto done;
	}

	ret = __dmabuf_resume(dmabuf);
	if (ret) {
		pr_err("%s, fail, ret[%d]\n", __func__, ret);
		goto done;
	}

	buffer_reclaim_status_set(dmabuf->priv, false);
done:
	mutex_unlock(lock);
release_rwsem:
	up_write(rwsem);

	return ret;
}

int mm_dmabuf_assign_resume(unsigned long ino_num)
{
	struct dma_buf *dmabuf = NULL;

	dmabuf = mm_dmaheap_dmabuf_get(ino_num);
	if (!dmabuf) {
		pr_err("%s, dmabuf get by ino_num[%lu] fail\n", __func__, ino_num);
		return 0;
	}

	return dmabuf_resume(dmabuf);
}

static int reclaim_dmabuf_enable_show(struct seq_file *m, void *v)
{
	seq_printf(m, "reclaim_dmabuf_enable = %d\n", reclaim_dmabuf_enable);
	return 0;
}

static int reclaim_dmabuf_enable_open(struct inode *inode, struct file *file)
{
	return single_open(file, reclaim_dmabuf_enable_show, NULL);
}

#define DMABUF_SWAP_SIZE	(0x40000000)
static ssize_t reclaim_dmabuf_enable_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char buffer[16] = {}; // 16: max buffer length
	int enable = -1;
	int ret = 0;

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count)) {
		return -EFAULT;
	}

	if (sscanf_s(buffer, "%d", &enable) != 1) { // 1: one param
		pr_err("Input parameters are invalid\n");
		return -EFAULT;
	}

	pr_info("reclaim_dmabuf_enable_write: enable=%d\n", enable);
	if (enable == 1) {
		mutex_lock(&reclaim_dmabuf_enable_mutex);
		if (!reclaim_dmabuf_enable) {
			pr_debug("will call dmabuf_page_swap_space_init\n");
			ret = dmabuf_page_swap_space_init(DMABUF_SWAP_SIZE);
			if (ret == 0) {
				pr_err("dmabuf_page_swap_space_init success\n");
				reclaim_dmabuf_enable = true;
			}
		}
		mutex_unlock(&reclaim_dmabuf_enable_mutex);
	} else if (enable == 0) {
		mutex_lock(&reclaim_dmabuf_enable_mutex);
		reclaim_dmabuf_enable = false;
		mutex_unlock(&reclaim_dmabuf_enable_mutex);
	} else {
		pr_err("reclaim_dmabuf_enable_write: invalid input = %d\n", enable);
	}

	return ret;
}

static const struct proc_ops enable_fops = {
	.proc_open = reclaim_dmabuf_enable_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = reclaim_dmabuf_enable_write,
};

static int dmabuf_swapout_show(struct seq_file *m, void *v)
{
	seq_printf(m, "dmabuf_swapout\n"); 
	return 0;
}

static int dmabuf_swapout_open(struct inode *inode, struct file *file)
{
	return single_open(file, dmabuf_swapout_show, NULL);
}

static ssize_t dmabuf_swapout_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char buffer[16] = {}; // 16: max buffer length
	unsigned long ino = 0;

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	if (sscanf_s(buffer, "%lu", &ino) != 1) { // 1: one params
		pr_err("Input parameters are invalid\n");
		return -EFAULT;
	}

	pr_debug("dmabuf_swapout_write: ino=%lu\n", ino);
	return mm_dmabuf_assign_reclaim(ino);
}

static const struct proc_ops swapout_fops = {
	.proc_open = dmabuf_swapout_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = dmabuf_swapout_write,
};

static int dmabuf_swapin_show(struct seq_file *m, void *v)
{
	seq_printf(m, "dmabuf_swapin\n"); 
	return 0;
}

static int dmabuf_swapin_open(struct inode *inode, struct file *file)
{
	return single_open(file, dmabuf_swapin_show, NULL);
}

static ssize_t dmabuf_swapin_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char buffer[16]; // 16: max buffer length
	unsigned long ino = 0;

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	if (sscanf_s(buffer, "%lu", &ino) != 1) { // 1: one params
		pr_err("Input parameters are invalid\n");
		return -EFAULT;
	}

	pr_debug("dmabuf_swapin_write: ino=%lu\n", ino);
	return mm_dmabuf_assign_resume(ino);
}

static const struct proc_ops swapin_fops = {
	.proc_open = dmabuf_swapin_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = dmabuf_swapin_write,
};

static int dmabuf_reclaim_file_node_init(void)
{
	struct proc_dir_entry *entry = NULL;

	entry = proc_create_data("ldk_reclaim_process_dmabuf_enable", 0664,
				NULL, &enable_fops, NULL);
	if (!entry) {
		pr_err("Failed to create swapout_process_dmabuf\n");
		return -EFAULT;
	}

	entry = proc_create_data("ldk_swapout_process_dmabuf", 0664,
				NULL, &swapout_fops, NULL);
	if (!entry) {
		pr_err("Failed to create swapout_process_dmabuf\n");
		return -EFAULT;
	}

	entry = proc_create_data("ldk_swapin_process_dmabuf", 0664,
				NULL, &swapin_fops, NULL);
	if (!entry) {
		pr_err("Failed to create swapin_process_dmabuf\n");
		return -EFAULT;
	}

	return 0;
}

static int __init dmabuf_reclaim_init(void)
{
	int ret;

	mutex_lock(&reclaim_dmabuf_enable_mutex);
	reclaim_dmabuf_enable = false;

	ret = dmabuf_reclaim_file_node_init();
	if (ret) {
		pr_err("%s, create file node fail\n", __func__);
		goto err;
	}
	mutex_unlock(&reclaim_dmabuf_enable_mutex);

	pr_err("%s: init succ!\n", __func__);
	return 0;

err:
	pr_err("%s: init fail!\n", __func__);

	mutex_unlock(&reclaim_dmabuf_enable_mutex);
	return ret;
}

subsys_initcall(dmabuf_reclaim_init);
